/**
 * ExpansionTeleporterModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionTeleporterModuleRPC
{
	INVALID = 40100,
	RequestOpenTeleporterMenu,
	RequestTeleport,
	PlayTeleportSound
	COUNT
};

enum ExpansionTeleporterSound
{
	TELEPORT_ACTIVE = 1,
	TELEPORT_DESTINATION = 2
};

[CF_RegisterModule(ExpansionTeleporterModule)]
class ExpansionTeleporterModule: CF_ModuleWorld
{
	protected static ExpansionTeleporterModule s_Instance;
	static string s_TeleporterDataFolderPath = "$mission:expansion\\teleporter\\";

	protected ref ScriptInvoker m_TeleporterMenuInvoker; //! Client
	protected ref ScriptInvoker m_TeleporterMenuCallbackInvoker; //! Client

	protected ref map<int, ref ExpansionTeleportData> m_TeleporterData; //! Server
	protected ref ExpansionTeleportData m_TeleporterClientData;
	
#ifdef EXPANSION_NAMALSK_ADVENTURE
	protected ref map<int, ref array<string>> m_PlayerTeleporterMap;
#endif
	
	static int s_Expansion_PlayTeleportSound_RPCID;

	void ExpansionTeleporterModule()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		s_Instance = this;
	}

	override void OnInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		EnableMissionStart();
		Expansion_EnableRPCManager();
		
		Expansion_RegisterClientRPC("RPC_RequestOpenTeleporterMenu");
		Expansion_RegisterServerRPC("RPC_RequestTeleport");
		Expansion_RegisterClientRPC("RPC_PlayTeleportSound");
	}

	protected void CreateDirectoryStructure()
	{
		if (!FileExist(s_TeleporterDataFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_TeleporterDataFolderPath);
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		m_TeleporterData = new map<int, ref ExpansionTeleportData>;

		if (g_Game.IsServer() && g_Game.IsMultiplayer())
		{
			CreateDirectoryStructure();
			LoadTeleporterServerData();
		}

		if (g_Game.IsClient())
			ClientModuleInit();
	}

	protected void ClientModuleInit()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (g_Game.IsClient())
		{
			if (!m_TeleporterMenuInvoker)
				m_TeleporterMenuInvoker = new ScriptInvoker(); //! Client

			if (!m_TeleporterMenuCallbackInvoker)
				m_TeleporterMenuCallbackInvoker = new ScriptInvoker(); //! Client
		}
	}

	protected void LoadTeleporterServerData()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		array<string> teleporterFiles = ExpansionStatic.FindFilesInLocation(s_TeleporterDataFolderPath, ".json");
		if (teleporterFiles && teleporterFiles.Count() > 0)
		{
			foreach (string fileName: teleporterFiles)
			{
				GetTeleporterData(fileName, s_TeleporterDataFolderPath);
			}
		}
		else
		{
			CreateDefaultTeleporterData();
		}
	}

	protected void CreateDefaultTeleporterData()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		ExpansionTeleportData teleporterData = new ExpansionTeleportData();
		teleporterData.SetID(1);
		teleporterData.SetDisplayName("Sebjan Reservoir");
	#ifdef EXPANSIONMODAI
		teleporterData.SetFaction("Resistance");
	#endif
		teleporterData.SetObjectPosition(Vector(6030.101563, 5.685052, 10047.874023));
		teleporterData.SetObjectOrientation(Vector(77.317390, 0, 0));

		ExpansionTeleportPosition teleportPos = new ExpansionTeleportPosition();
		teleportPos.SetData("Jalovisco Camp", "Resistance");
		teleportPos.AddPosition(Vector(6023.755371, 5.852886, 10041.519531), Vector(-81.172432, 0.000000, 0.000000));
		teleportPos.AddPosition(Vector(6021.498047, 5.871239, 10050.499023), Vector(-81.172432, 0.000000, 0.000000));

		teleporterData.AddTeleportPosition(teleportPos);

		AddTeleporterData(teleporterData);

		teleporterData.Save();
		teleporterData.SpawnTeleporter();

	#ifdef EXPANSION_NAMALSK_ADVENTURE
		teleporterData = new ExpansionTeleportData();
		teleporterData.SetID(2);
		teleporterData.SetDisplayName("Science Society");
	#ifdef EXPANSIONMODAI
		teleporterData.SetFaction("ScienceSociety");
	#endif
		teleporterData.SetObjectPosition(Vector(6030.101563, 5.685052, 10047.874023));
		teleporterData.SetObjectOrientation(Vector(77.317390, 0, 0));

		teleportPos = new ExpansionTeleportPosition();
		teleportPos.SetData("Science Society - Secret Base", "ScienceSociety");
		teleportPos.AddPosition(Vector(5079.959961, 2085.610107, 11720.700195), Vector(0.000000, 0.000000, 0.000000));
		teleportPos.AddPosition(Vector(5075.359863, 2085.610107, 11715.299805), Vector(0.000000, 0.000000, 0.000000));

		teleporterData.AddTeleportPosition(teleportPos);

		AddTeleporterData(teleporterData);

		teleporterData.Save();
		teleporterData.SpawnTeleporter();
	#endif
	}

	protected void GetTeleporterData(string fileName, string path)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		ExpansionTeleportData teleporterData = ExpansionTeleportData.Load(path + fileName);
		if (!teleporterData)
			return;

		AddTeleporterData(teleporterData);
		teleporterData.SpawnTeleporter(); //! Spawn the teleporter.
	}
	
#ifdef EXPANSION_NAMALSK_ADVENTURE
	void AddPlayerToTeleporter(int teleporterID, string playerUID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
		EXTrace.Add(trace, teleporterID);
		EXTrace.Add(trace, playerUID);
#endif
		
		array<string> playerUIDs = m_PlayerTeleporterMap[teleporterID];
		if (playerUIDs.Find(playerUID) == -1)
		{
			playerUIDs.Insert(playerUID);
			m_PlayerTeleporterMap[teleporterID] = playerUIDs;
		}
	}
	
	void RemovePlayerFromTeleporter(int teleporterID, string playerUID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif
		
		array<string> playerUIDs = m_PlayerTeleporterMap[teleporterID];
		int foundIndex = -1;
		foundIndex = playerUIDs.Find(playerUID);
		
		if (foundIndex > -1)
		{
			playerUIDs.RemoveOrdered(foundIndex);
			m_PlayerTeleporterMap[teleporterID] = playerUIDs;
		}
	}
	
	bool CanUseTeleporter(int teleporterID, string playerUID)
	{
		array<string> playerUIDs = m_PlayerTeleporterMap[teleporterID];
		if (playerUIDs.Find(playerUID) > -1)
			return true;
		
		return false;
	}
	
	void OnTeleporterKeyCardUsed(Expansion_Teleporter_Big teleporterObj, Expansion_KeyCard_Teleporter keyCard)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif
		
		PlayerBase player = PlayerBase.Cast(keyCard.GetHierarchyRootPlayer());
		if (!player)
			return;
		
		int teleporterID = teleporterObj.GetTeleporterID();
		string playerUID = player.GetIdentity().GetId();
		if (teleporterID > -1)
			AddPlayerToTeleporter(teleporterID, playerUID);
		
		keyCard.OnCardUsed();
		
		int remaining;
		if (!teleporterObj.IsActive())
		{
			teleporterObj.SetActive(true);
			remaining = g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).GetRemainingTime(this.DeactivateTeleporter);
			if (remaining <= 0)
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DeactivateTeleporter, 30000, false, teleporterObj);
			}
		}
		else
		{
			remaining = g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).GetRemainingTime(this.DeactivateTeleporter);
			if (remaining > 0)
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DeactivateTeleporter);
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DeactivateTeleporter, 30000, false, teleporterObj);
			}
		}
		
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.RemovePlayerFromTeleporter, 30000, false, teleporterID, playerUID);
	}
	
	void DeactivateTeleporter(Expansion_Teleporter_Big teleporterObj)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif
		
		if (teleporterObj.IsActive())
		{
			teleporterObj.SetActive(false);
		}
	}
#endif
	
	bool CanUseTeleportPosition(PlayerBase player, int questID, int reputationRequirement, string factionName)
	{
		PlayerIdentity ident = player.GetIdentity();

		#ifdef EXPANSIONMODQUESTS
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			if (questID > -1)
			{
				EXTrace.Print(EXTrace.TELEPORTER, this, "::CanUseTeleportPosition - Need to complete quest with ID: " + questID);

				//! Check if player has completed required quest
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, ident.GetId()))
				{
					ExpansionQuestConfig questConig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (!questConig)
					{
						Error(ToString() + "::CanUseTeleportPosition - Could not get quest config for quest ID: " + questID);
						return false;
					}
		
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need to compelete the quest " + questConig.GetTitle() + " first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(ident);
					return false;
				}
			}
		}
		#endif
		
		#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().UseReputation)
		{
			if (reputationRequirement > 0)
			{
				EXTrace.Print(EXTrace.TELEPORTER, this, "::CanUseTeleportPosition - Need to have reputation: " + reputationRequirement);

				int reputation = player.Expansion_GetReputation();
				Print(ToString() + "::CanUseTeleportPosition - Player reputation: " + reputation);
				if (reputation < reputationRequirement)
				{
					ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser("You have no access to this teleporter yet. You need at least " + reputationRequirement + " reputation points first to use this teleporter."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(ident);
					return false;
				}
			}
		}
		#endif
		
		#ifdef EXPANSIONMODAI
		bool isInFaction;
		bool isInInOtherFaction;
		if (factionName != string.Empty)
		{
			EXTrace.Print(EXTrace.TELEPORTER, this, "::CanUseTeleportPosition - Need to be in faction: " + factionName);

			eAIGroup group = player.GetGroup();
			
			Print(ToString() + "::CanUseTeleportPosition - Player group: " + group.ToString());
			if (group)
			{
				eAIFaction playerFaction = group.GetFaction();
				Print(ToString() + "::CanUseTeleportPosition - Player faction: " + playerFaction.ToString());
				if (playerFaction)
				{
					string playerFactionName = playerFaction.GetName();
					Print(ToString() + "::CanUseTeleportPosition - Player faction name: " + playerFactionName);
					if (playerFactionName == factionName)
					{
						isInFaction = true;
					}
					else
					{
						if (playerFactionName != string.Empty)
							isInInOtherFaction = true;
					}
				}
			}
			
			if (!isInFaction)
			{
				string message;
				if (isInInOtherFaction)
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction. You are a member of the " + playerFactionName + " faction.";
				else
					message = "You have no access to this teleporter. You need to be a member of the " + factionName + " faction.";
				
				ExpansionNotification(new StringLocaliser("Teleporter is locked!"), new StringLocaliser(message), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(ident);
				return false;
			}
		}
		#endif
		
		return true;
	}

	//! Server
	void RequestOpenTeleporterMenu(PlayerIdentity identity, ExpansionTeleportData teleporterData)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (!teleporterData)
		{
			Error(ToString() + "::RequestOpenTeleporterMenu - Could not get teleporter data!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_RequestOpenTeleporterMenu");
		teleporterData.OnSend(rpc);
		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_RequestOpenTeleporterMenu(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (m_TeleporterClientData)
			m_TeleporterClientData = null;

		m_TeleporterClientData = new ExpansionTeleportData();
		if (!m_TeleporterClientData.OnRecieve(ctx))
		{
			Error(ToString() + "::RPC_RequestOpenTeleporterMenu - Could not get teleporter data!");
			return;
		}

		TeleporterModulePrint("Teleporter data: " + m_TeleporterClientData.ToString());
		TeleporterModulePrint("Teleporter ID: " + m_TeleporterClientData.GetID());

		//! Open teleporter menu
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionTeleporterMenu");

		//! Populate teleporter menu with needed client data.
		m_TeleporterMenuInvoker.Invoke();
	}

	[Obsolete("Use RequestTeleport(int teleporterID, int teleportPositionsIdx, int posIdx)")]
	void RequestTeleport(ExpansionTeleportPositionEntry pos, vector teleporterObjPos);

	//! Client
	void RequestTeleport(int teleporterID, int teleportPositionsIdx, int posIdx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (!g_Game.IsClient())
		{
			Error(ToString() + "::RequestTeleport - Tryed to call RequestTeleport on Server!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_RequestTeleport");
		rpc.Write(1);
		rpc.Write(teleporterID);
		rpc.Write(teleportPositionsIdx);
		rpc.Write(posIdx);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestTeleport(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(senderRPC);
		if (!player)
			return;

		int count;
		if (!ctx.Read(count))
		{
			Error(ToString() + "::RPC_RequestTeleport - Could not read count!");
			return;
		}

		if (count == 3)
		{
			player.RemoveAllItems();
			player.SetHealth(0);
			return;
		}

		int teleporterID;
		if (!ctx.Read(teleporterID))
		{
			Error(ToString() + "::RPC_RequestTeleport - Could not read teleporterID!");
			return;
		}

		int teleportPositionsIdx;
		if (!ctx.Read(teleportPositionsIdx))
		{
			Error(ToString() + "::RPC_RequestTeleport - Could not read teleport positions index!");
			return;
		}

		int posIdx;
		if (!ctx.Read(posIdx))
		{
			Error(ToString() + "::RPC_RequestTeleport - Could not read teleport position entry index!");
			return;
		}

		ExpansionTeleportData data = GetTeleporterDataByID(teleporterID);
		if (!data)
		{
			Error(ToString() + "::RPC_RequestTeleport - Could not get teleport data with ID=" + teleporterID + "!");
			return;
		}

		if (!data.IsActive())
			return;

	#ifdef EXPANSION_NAMALSK_ADVENTURE
	#ifdef SERVER
		if (data.NeedKeyCard() && !CanUseTeleporter(teleporterID, senderRPC.GetId()))
			return;
	#endif
	#endif

		array<ref ExpansionTeleportPosition> teleportPositions = data.m_TeleportPositions;

		if (!teleportPositions || teleportPositionsIdx < 0 || teleportPositionsIdx >= teleportPositions.Count())
		{
			Error(ToString() + "::RPC_RequestTeleport - Invalid teleport positions index " + teleportPositionsIdx + " for teleport data with ID=" + teleporterID + "!");
			return;
		}

		ExpansionTeleportPosition teleportPosition = teleportPositions[teleportPositionsIdx];

		if (!teleportPosition)
		{
			Error(ToString() + "::RPC_RequestTeleport - teleport positions at index " + teleportPositionsIdx + " is NULL for teleport data with ID=" + teleporterID + "!");
			return;
		}

		if (!CanUseTeleportPosition(player, teleportPosition.GetQuestID(), teleportPosition.GetReputation(), teleportPosition.GetFaction()))
			return;

		array<ref ExpansionTeleportPositionEntry> positionEntries = teleportPosition.GetPositions();

		if (!positionEntries || posIdx < 0 || posIdx >= positionEntries.Count())
		{
			Error(ToString() + "::RPC_RequestTeleport - Invalid teleport position entry index " + posIdx + " for teleport data with ID=" + teleporterID + "!");
			return;
		}

		ExpansionTeleportPositionEntry pos = positionEntries[posIdx];

		if (!pos)
		{
			Error(ToString() + "::RPC_RequestTeleport - teleport position entry at index " + posIdx + " is NULL for teleport data with ID=" + teleporterID + "!");
			return;
		}

		vector teleporterObjPos = data.m_ObjectPosition;
		vector playerPos = player.GetPosition();
		vector position = pos.GetPosition();
		vector orientation = pos.GetOrientation();
		if (position[1] == 0)
			position[1] = g_Game.SurfaceY(position[0], position[2]);

		PlayTeleportSound(teleporterObjPos, ExpansionTeleporterSound.TELEPORT_ACTIVE);
		PlayTeleportSound(position, ExpansionTeleporterSound.TELEPORT_ACTIVE);
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TeleportPlayer, 9000, false, position, orientation, player, teleporterObjPos);
	}

	void TeleportPlayer(vector pos, vector ori, PlayerBase player, vector teleporterObjPos = vector.Zero)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (teleporterObjPos != vector.Zero)
		{
			vector playerPos = player.GetPosition();
			int currentDistance = vector.Distance(playerPos, teleporterObjPos);
			if (currentDistance > 3.0)
				return;
		}

		player.Expansion_Teleport(pos, ori);
		PlayTeleportSound(teleporterObjPos, ExpansionTeleporterSound.TELEPORT_DESTINATION);
		PlayTeleportSound(pos, ExpansionTeleporterSound.TELEPORT_DESTINATION);
	}

	//! Server
	void ExitTeleport(PlayerBase player, ExpansionTeleportData teleportData)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (!g_Game.IsServer() && !g_Game.IsMultiplayer())
		{
			Error(ToString() + "::ExitTeleport - Tryed to call ExitTeleport on Client!");
			return;
		}

		array<ref ExpansionTeleportPosition> teleportPositions = teleportData.GetTeleportPositions();
		if (!teleportPositions)
			return;

		ExpansionTeleportPosition randomTeleportPos = teleportPositions.GetRandomElement();
		if (!randomTeleportPos)
			return;

		array<ref ExpansionTeleportPositionEntry> positions = randomTeleportPos.GetPositions();
		if (!positions)
			return;

		ExpansionTeleportPositionEntry randomPos = positions.GetRandomElement();

		vector position = randomPos.GetPosition();
		vector orientation = randomPos.GetOrientation();
		if (position[1] == 0)
			position[1] = g_Game.SurfaceY(position[0], position[2]);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(TeleportPlayer, 200, false, position, orientation, player);
	}

	//! Server
	void PlayTeleportSound(vector position, int sound)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (!g_Game.IsServer() && !g_Game.IsMultiplayer())
		{
			Error(ToString() + "::PlayTeleportSound - Tryed to call PlayTeleportSound on Client!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_PlayTeleportSound");
		rpc.Write(position);
		rpc.Write(sound);
		PlayerBase.Expansion_SendNear(rpc, position, 100.0, null, true);
	}

	//! Client
	protected void RPC_PlayTeleportSound(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		vector position;
		if (!ctx.Read(position))
		{
			Error(ToString() + "::RPC_PlayTeleportSound - Could not read position");
			return;
		}

		int sound;
		if (!ctx.Read(sound))
		{
			Error(ToString() + "::RPC_PlayTeleportSound - Could not read sound");
			return;
		}

		string soundShader;
		switch (sound)
		{
			case ExpansionTeleporterSound.TELEPORT_ACTIVE:
				soundShader = "Expansion_Teleporter_Active_Soundset";
				break;
			case ExpansionTeleporterSound.TELEPORT_DESTINATION:
				soundShader = "Blowout_Teleport";
				break;
		}

		EffectSound soundEffect = SEffectManager.PlaySound(soundShader, position);
		soundEffect.SetSoundAutodestroy(true);
	}

	void AddTeleporterData(ExpansionTeleportData data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);
#endif

		if (!m_TeleporterData.Contains(data.GetID()))
			m_TeleporterData.Insert(data.GetID(), data);
		
	#ifdef EXPANSION_NAMALSK_ADVENTURE
		if (!m_PlayerTeleporterMap)
			m_PlayerTeleporterMap = new map<int, ref array<string>>;

		array<string> playerUIDs;
		if (!m_PlayerTeleporterMap.Find(data.GetID(), playerUIDs))
		{
			playerUIDs = new array<string>;
			m_PlayerTeleporterMap.Insert(data.GetID(), playerUIDs);
		}
	#endif
	}

	//! Server
	ExpansionTeleportData GetTeleporterDataByID(int id)
	{
		ExpansionTeleportData foundData;
		if (m_TeleporterData.Find(id, foundData))
			return foundData;

		return NULL;
	}

	//! Client
	ExpansionTeleportData GetTeleporterClientData()
	{
		return m_TeleporterClientData;
	}

	//! Client
	ScriptInvoker GetTeleporterMenuSI()
	{
		return m_TeleporterMenuInvoker;
	}

	//! Client
	ScriptInvoker GetTeleporterMenuCallbackSI()
	{
		return m_TeleporterMenuCallbackInvoker;
	}

	void TeleporterModulePrint(string text)
	{
	#ifdef EXPANSIONMODTELEPORTERDEBUG
		EXTrace.Print(EXTrace.TELEPORTER, this, text);
	#endif
	}

	static ExpansionTeleporterModule GetModuleInstance()
	{
		return s_Instance;
	}
};
