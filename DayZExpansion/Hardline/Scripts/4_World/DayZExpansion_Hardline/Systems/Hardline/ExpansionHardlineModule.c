/**
 * ExpansionHardlineModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionHardlineModuleRPC
{
	INVALID = 70000,
	SendPlayerHardlineData,
	RequestHardlineItemData,
	SendHardlineItemData
	COUNT
};

[CF_RegisterModule(ExpansionHardlineModule)]
class ExpansionHardlineModule: CF_ModuleWorld
{
	//! Server only
	private autoptr map<string, ref ExpansionHardlinePlayerData> m_PlayerDatas; //! Server
	
	//! Client only
	private ref ExpansionHardlinePlayerData m_HardlinePlayerData; //! Client
	private ref ScriptInvoker m_HardlineHUDInvoker; //! Client
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule Constructor
	// ------------------------------------------------------------
	void ExpansionHardlineModule()
	{

	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		EnableInvokeDisconnect();
		EnableClientLogout();
		EnableClientReady();
		EnableClientDisconnect();
		EnableUpdate();
		EnableRPC();
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnMissionStart");
	#endif

		super.OnMissionStart(sender, args);
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnMissionLoaded");
	#endif

		super.OnMissionLoaded(sender, args);
		
		m_PlayerDatas = new map<string, ref ExpansionHardlinePlayerData>;

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (!FileExist(EXPANSION_HARDLINE_FOLDER))
			{
				MakeDirectory(EXPANSION_HARDLINE_FOLDER);
				MakeDirectory(EXPANSION_HARDLINE_PLAYERDATA_FOLDER);
			}
		}
	}
	
	// -----------------------------------------------------------
	// ExpansionHardlineModule OnInvokeDisconnect
	// -----------------------------------------------------------
	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnInvokeConnect");
	#endif

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseHumanity)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SetupClientData, 1000, false, cArgs.Identity);
		}
		
		if (GetGame().IsClient())
		{
			if (!m_HardlineHUDInvoker && GetExpansionSettings().GetHardline().ShowHardlineHUD)
				m_HardlineHUDInvoker = new ScriptInvoker(); //! Client
		}
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule OnClientReady
	// ------------------------------------------------------------
	override void OnClientReady(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnClientReady");
	#endif

		super.OnClientReady(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseHumanity)
		{
			//! Send needed hardline module data to the client.
			SetupClientData(cArgs.Identity);
		}
	}

	// -----------------------------------------------------------
	// ExpansionHardlineModule OnClientDisconnect
	// -----------------------------------------------------------
	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnClientDisconnect");
	#endif

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);
		
		//! Maybe move this to the OnClientLogout method
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetHardline().UseHumanity)
		{
			if (cArgs.Identity)
			{
				SavePlayerHardlineData(cArgs.Identity.GetId());
			}
			else if (cArgs.Player.GetIdentity())
			{
				SavePlayerHardlineData(cArgs.Player.GetIdentity().GetId());
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule SavePlayerHardlineData
	// Called on Server
	// ------------------------------------------------------------
	private void SavePlayerHardlineData(string playerUID)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		ExpansionHardlinePlayerData playerData = GetPlayerHardlineDataByUID(playerUID);
		if (playerData)
			playerData.Save(playerUID);
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule SetupClientData
	// Called on server
	// ------------------------------------------------------------
	//! This method is only called once on player connection
	// and to send the needed data from the hardline module to the client
	void SetupClientData(PlayerIdentity identity)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (!identity)
			return;

		string playerUID = identity.GetId();
		ref ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(playerUID);
		if (!hardlinePlayerData)
		{
			//! If we don't have cached player hardline data, check if file exists and load it, else use fresh instance as-is
			hardlinePlayerData = new ExpansionHardlinePlayerData();
			m_PlayerDatas.Insert(playerUID, hardlinePlayerData);
			if (hardlinePlayerData.Load(playerUID))
				HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Loaded player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
			else
				HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Created new player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
		}
		else if (hardlinePlayerData)
		{
			HardlineModulePrint("ExpansionHardlineModule::SetupClientData - Got cached player hardline data for player " + identity.GetName() + "[" + playerUID + "]");
			PlayerHardlineInit(hardlinePlayerData, identity);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule PlayerHardlineInit
	// Server
	// ------------------------------------------------------------
	void PlayerHardlineInit(ExpansionHardlinePlayerData playerData, PlayerIdentity identity)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (!playerData)
			return;

		SendPlayerHardlineData(playerData, identity);
	}
	
	// -----------------------------------------------------------
	// ExpansionHardlineModule GetPlayerHardlineDataByUID
	// ! Server
	// -----------------------------------------------------------
	ExpansionHardlinePlayerData GetPlayerHardlineDataByUID(string playerUID)
	{
		return m_PlayerDatas.Get(playerUID);
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule OnEntityKilled
	// Server
	// ------------------------------------------------------------
	void OnEntityKilled(EntityAI victim, Object killer)
	{
		HardlineModulePrint(ToString() + "::OnEntityKilled - Start");
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		EntityAI killerEntity;
		if (!Class.CastTo(killerEntity, killer))
			return;
			
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim type: " + victim.GetType());
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer type: " + killerEntity.GetType());
				
		PlayerBase killerPlayer;
		ZombieBase killerInfected;
		PlayerBase victimPlayer;
		ZombieBase victimInfected;
		bool killerIsPlayer = false;
		bool killerIsAI = false;
		bool killerIsInfected = false;
		bool victimIsPlayer = false;
		bool victimIsAI = false;
		bool victimIsInfected = false;
		
		ExpansionHardlinePlayerData killerPlayerData;
		ExpansionHardlinePlayerData victimPlayerData;
		
		//! Set killer entity
		if (Class.CastTo(killerPlayer, killerEntity))
		{
			killerIsPlayer = true;
		#ifdef EXPANSIONMODAI
			if (killerPlayer.IsAI())
				killerIsAI = true;
		#endif
		}
		else if (Class.CastTo(killerPlayer, killerEntity.GetHierarchyRootPlayer()))
		{
			killerIsPlayer = true;
		#ifdef EXPANSIONMODAI
			if (killerPlayer.IsAI())
				killerIsAI = true;
		#endif
		}
		else if (Class.CastTo(killerInfected, killerEntity))
		{
			killerIsInfected = true;
		}
		
		//! Set victim entity
		if (Class.CastTo(victimPlayer, victim))
		{
			victimIsPlayer = true;
		#ifdef EXPANSIONMODAI
			if (victimPlayer.IsAI())
				victimIsAI = true;
		#endif
		}
		else if (Class.CastTo(victimPlayer, victim.GetHierarchyRootPlayer()))
		{
			victimIsPlayer = true;
		#ifdef EXPANSIONMODAI
			if (victimPlayer.IsAI())
				victimIsAI = true;
		#endif
		}
		else if (Class.CastTo(victimInfected, victim))
		{
			victimIsInfected = true;
		}
		
		if (!victimPlayer && !killerIsPlayer)
			return;
		
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Player: " + killerIsPlayer);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is AI: " + killerIsAI);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Infected: " + killerIsInfected);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is Player: " + victimIsPlayer);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is AI: " + victimIsAI);
		HardlineModulePrint(ToString() + "::OnEntityKilled - Victim is Infected: " + victimIsInfected);
		
		StringLocaliser title;
		StringLocaliser text;
			
		if (killerIsPlayer && !killerIsAI)
		{
			HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Player - START");
			killerPlayerData = GetPlayerHardlineDataByUID(killerPlayer.GetIdentity().GetId());
			
			if (victimIsPlayer && !victimIsAI) 
			{
				HardlineModulePrint(ToString() + "::OnEntityKilled - Player victim type: " + victimPlayer.GetType());
				HardlineModulePrint(ToString() + "::OnEntityKilled - Player killer type: " + killerPlayer.GetType());
			#ifdef EXPANSIONMODAI
				HardlineModulePrint(ToString() + "::OnEntityKilled - Player victim is AI: " + victimPlayer.IsAI());
				HardlineModulePrint(ToString() + "::OnEntityKilled - Player killer is AI: " + killerPlayer.IsAI());
			#endif
				
				if (killerPlayer == victimPlayer)
					return;
								
				int killerHumanity = killerPlayerData.GetHumanity();
				string humanityText;
				int humanityChange;
												
				victimPlayerData = GetPlayerHardlineDataByUID(victimPlayer.GetIdentity().GetId());
				if (!victimPlayerData)
					return;
				
				int victimHumanity = victimPlayerData.GetHumanity();
				
				bool killerIsBandit = killerHumanity <= GetExpansionSettings().GetHardline().RankKleptomaniac;
				bool killerIsHero = killerHumanity >= GetExpansionSettings().GetHardline().RankScout;

				bool victimIsBandit = victimHumanity <= GetExpansionSettings().GetHardline().RankKleptomaniac;
				bool victimIsHero = victimHumanity >= GetExpansionSettings().GetHardline().RankScout;

				if (victimIsBandit)
				{
					humanityChange = GetExpansionSettings().GetHardline().HumanityOnKillBandit * victimPlayerData.GetRank();
					killerPlayerData.OnKillBandit();
				}
				else if (victimIsHero)
				{
					humanityChange = GetExpansionSettings().GetHardline().HumanityOnKillHero * victimPlayerData.GetRank();
					killerPlayerData.OnKillHero();
				}
				else
				{
					humanityChange = GetExpansionSettings().GetHardline().HumanityOnKillBambi;
					killerPlayerData.OnKillBambi();
				}

				if ((killerIsBandit || killerIsHero) && victimIsBandit)
				{
					killerPlayerData.AddHumanity(humanityChange);
					humanityText = "Added";
				}
				else
				{
					killerPlayerData.RemoveHumanity(humanityChange);
					humanityText = "Removed";
				}
					
				killerPlayerData.Save(killerPlayer.GetIdentity().GetId());		
				SendPlayerHardlineData(killerPlayerData, killerPlayer.GetIdentity());
				
				SendPlayerHardlineData(victimPlayerData, victimPlayer.GetIdentity());
						
				title = new StringLocaliser("Killed %1", victimPlayerData.GetRankName());
				text = new StringLocaliser("%1 %2 Humanity", humanityText, humanityChange.ToString());
				ExpansionNotification(title, text, ExpansionIcons.GetPath("Skull 2"),  COLOR_EXPANSION_NOTIFICATION_EXPANSION, 2, ExpansionNotificationType.ACTIVITY).Create(killerPlayer.GetIdentity());
			}
			else if (victimIsInfected)
			{
				int humanityOnKillInfected = GetExpansionSettings().GetHardline().HumanityOnKillInfected;
				killerPlayerData.AddHumanity(humanityOnKillInfected);
				killerPlayerData.OnInfectedKilled();
												
				killerPlayerData.Save(killerPlayer.GetIdentity().GetId());		
				SendPlayerHardlineData(killerPlayerData, killerPlayer.GetIdentity());
				
				title = new StringLocaliser("Infected Killed");
				text = new StringLocaliser("Added %1 Humanity", humanityOnKillInfected.ToString());
				ExpansionNotification(title, text, ExpansionIcons.GetPath("Infected 2"),  ARGB(255, 160, 223, 59), 2, ExpansionNotificationType.ACTIVITY).Create(killerPlayer.GetIdentity());
			}
		#ifdef EXPANSIONMODAI
			else if (victimIsPlayer && victimIsAI)
			{
				int humanityOnKillAI = GetExpansionSettings().GetHardline().HumanityOnKillAI;
				
				//! If killerPlayer is bandit
				if (killerHumanity <= GetExpansionSettings().GetHardline().RankKleptomaniac)
				{
					killerPlayerData.RemoveHumanity(humanityOnKillAI);
					humanityText = "Removed";
				}
				//! If killerPlayer is hero or neutral
				else
				{
					killerPlayerData.AddHumanity(humanityOnKillAI);
					humanityText = "Added";
				}
				
				killerPlayerData.OnAIKilled();
				
								
				killerPlayerData.Save(killerPlayer.GetIdentity().GetId());		
				SendPlayerHardlineData(killerPlayerData, killerPlayer.GetIdentity());
						
				title = new StringLocaliser("Killed %1", victim.GetDisplayName());
				text = new StringLocaliser("%1 %2 Humanity", humanityText, humanityOnKillAI.ToString());
				ExpansionNotification(title, text, ExpansionIcons.GetPath("Skull 2"),  COLOR_EXPANSION_NOTIFICATION_EXPANSION, 2, ExpansionNotificationType.ACTIVITY).Create(killerPlayer.GetIdentity());
			}
		#endif
		}
		else if (killerIsInfected)
		{
			HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is Infected - START");
			
			if (victimPlayer && !victimIsAI)
				OnPlayerDeath(victimPlayer);
		}
		else if (!killerIsPlayer && !killerIsAI && !killerIsInfected)
		{
			HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is UNKNOWN - START");
			
			if (victimIsAI)
				return;
			
			if (victimPlayer)			
				OnPlayerDeath(victimPlayer);
		}
	#ifdef EXPANSIONMODAI
		else if (killerIsPlayer && killerIsAI)
		{
			HardlineModulePrint(ToString() + "::OnEntityKilled - Killer is AI - START");
			
			if (victimPlayer && !victimPlayer.IsAI())
				OnPlayerDeath(victimPlayer);
		}
	#endif
		
		HardlineModulePrint(ToString() + "::OnEntityKilled - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule OnPlayerDeath
	// Server
	// ------------------------------------------------------------
	void OnPlayerDeath(PlayerBase victimPlayer)
	{
		HardlineModulePrint(ToString() + "::OnPlayerDeath - Start");
		
		ExpansionHardlinePlayerData victimPlayerData = GetPlayerHardlineDataByUID(victimPlayer.GetIdentity().GetId());
		if (!victimPlayerData)
			return;
		
		int humaityLossOnDeath = GetExpansionSettings().GetHardline().HumanityLossOnDeath;
		if (victimPlayer.IsHero())
		{
			victimPlayerData.RemoveHumanity(humaityLossOnDeath);
		}
		else if (victimPlayer.IsBandit())
		{
			victimPlayerData.AddHumanity(humaityLossOnDeath);
		}
		else if (!victimPlayer.IsBandit() && !victimPlayer.IsHero())
		{
			victimPlayerData.ResetHumanity();
		}
		
		victimPlayerData.OnPlayerDeath();
		victimPlayerData.Save(victimPlayer.GetIdentity().GetId());
		
		HardlineModulePrint(ToString() + "::OnPlayerDeath - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule OnPlayerPositiveAction
	// Server
	// ------------------------------------------------------------
	void OnPlayerPositiveAction(PlayerBase player, int humanity)
	{
	#ifdef EXPANSIONMODAI
		if (player.IsAI())
			return;
	#endif
		
		string playerID = player.GetIdentity().GetId();		
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(playerID);
		if (!hardlinePlayerData)
			return;
		
		hardlinePlayerData.AddHumanity(humanity);
			
		StringLocaliser title = new StringLocaliser("Positive Action");
		StringLocaliser text = new StringLocaliser("Added %1 Humanity", humanity.ToString());
		
		ExpansionNotification(title, text, ExpansionIcons.GetPath("Star"),  COLOR_EXPANSION_NOTIFICATION_AMETHYST, 2, ExpansionNotificationType.ACTIVITY).Create(player.GetIdentity());
		
		hardlinePlayerData.Save(player.GetIdentity().GetId());		
		SendPlayerHardlineData(hardlinePlayerData, player.GetIdentity());
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule OnPlayerNegativeAction
	// Server
	// ------------------------------------------------------------
	void OnPlayerNegativeAction(PlayerBase player, int humanity)
	{
	#ifdef EXPANSIONMODAI
		if (player.IsAI())
			return;
	#endif
		
		string playerID = player.GetIdentity().GetId();		
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(playerID);
		if (!hardlinePlayerData)
			return;
		
		hardlinePlayerData.RemoveHumanity(humanity);
			
		StringLocaliser title = new StringLocaliser("Negative Action");
		StringLocaliser text = new StringLocaliser("Removed %1 Humanity", humanity.ToString());
		
		ExpansionNotification(title, text, ExpansionIcons.GetPath("Star"),  COLOR_EXPANSION_NOTIFICATION_AMETHYST, 2, ExpansionNotificationType.ACTIVITY).Create(player.GetIdentity());
		
		hardlinePlayerData.Save(player.GetIdentity().GetId());		
		SendPlayerHardlineData(hardlinePlayerData, player.GetIdentity());
	}
	
#ifdef WRDG_DOGTAGS
	// ------------------------------------------------------------
	// ExpansionHardlineModule UpdatePlayerDogTag
	// Server
	// ------------------------------------------------------------	
	void UpdatePlayerDogTag(string playerID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerID);		
		if (player && player.HasDogtag())
		{
			int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
			Dogtag_Base newTag;
			Dogtag_Base currentTag = Dogtag_Base.Cast(player.FindAttachmentBySlotName("Dogtag"));
			if (currentTag)
			{
				ExpansionHardlinePlayerData data = GetPlayerHardlineDataByUID(playerID);
				if (data)
				{
					string dogTagType;
					switch (data.GetRank())
					{
						case ExpansionPlayerRank.SCOUT:
						{
							dogTagType = "Dogtag_Scout";
							break;
						}
						case ExpansionPlayerRank.PATHFINDER:
						{
							dogTagType = "Dogtag_Pathfinder";
							break;
						}
						case ExpansionPlayerRank.HERO:
						{
							dogTagType = "Dogtag_Hero";
							break;
						}
						case ExpansionPlayerRank.SUPERHERO:
						{
							dogTagType = "Dogtag_Superhero";
							break;
						}
						case ExpansionPlayerRank.LEGEND:
						{
							dogTagType = "Dogtag_Legend";
							break;
						}
						case ExpansionPlayerRank.KLEPTOMANIAC:
						{
							dogTagType = "Dogtag_Kleptomaniac";
							break;
						}
						case ExpansionPlayerRank.BULLY:
						{
							dogTagType = "Dogtag_Bully";
							break;
						}
						case ExpansionPlayerRank.BANDIT:
						{
							dogTagType = "Dogtag_Bandit";
							break;
						}
						case ExpansionPlayerRank.KILLER:
						{
							dogTagType = "Dogtag_Killer";
							break;
						}
						case ExpansionPlayerRank.MADMAN:
						{
							dogTagType = "Dogtag_Madman";
							break;
						}
					}
					
					if (dogTagType != "" && currentTag.GetType() != dogTagType)
					{
						currentTag.ReplaceDogtag(dogTagType);
					}
				}
			}
		}
	}
#endif

	// ------------------------------------------------------------
	// ExpansionHardlineModule HardlineModulePrint
	// ------------------------------------------------------------
	void HardlineModulePrint(string text)
	{
	#ifdef EXPANSIONMODHARDLINEDEBUG
		Print(text);
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionHardlineModuleRPC.INVALID;
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionHardlineModuleRPC.COUNT;
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule GetRPCMax
	// ------------------------------------------------------------
	override void OnRPC(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.HARDLINE, this, "OnRPC");
	#endif

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionHardlineModuleRPC.SendPlayerHardlineData:
			{
				RPC_SendPlayerHardlineData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule SendPlayerHardlineData
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerHardlineData(PlayerIdentity identity)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		//! Get existing player hardline data if there is a exiting one in m_PlayerDatas
		ExpansionHardlinePlayerData hardlinePlayerData = GetPlayerHardlineDataByUID(identity.GetId());

		SendPlayerHardlineData(hardlinePlayerData, identity);
	}

	void SendPlayerHardlineData(ExpansionHardlinePlayerData hardlinePlayerData, PlayerIdentity identity)
	{
		if (!hardlinePlayerData)
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(identity.GetId());
		if (player)
		{
			player.SetIsBandit(false);
			player.SetIsHero(false);
						
			if (hardlinePlayerData.GetHumanity() <= GetExpansionSettings().GetHardline().RankKleptomaniac)
			{
				player.SetIsBandit(true);
			}
			else if (hardlinePlayerData.GetHumanity() >= GetExpansionSettings().GetHardline().RankScout)			
			{
				player.SetIsHero(true);
			}
			
			player.SetHumanity(hardlinePlayerData.GetHumanity());
		}
		
	#ifdef WRDG_DOGTAGS
		UpdatePlayerDogTag(identity.GetId());
	#endif
		
		ScriptRPC rpc = new ScriptRPC();
		hardlinePlayerData.OnSend(rpc);
		GetExpansionSettings().GetHardline().OnSend(rpc);
		rpc.Send(NULL, ExpansionHardlineModuleRPC.SendPlayerHardlineData, false, identity);
	}

	// ------------------------------------------------------------
	// ExpansionHardlineModule RPC_SendPlayerHardlineData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerHardlineData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!GetGame().IsClient())
			return;

		if (!m_HardlinePlayerData)
			m_HardlinePlayerData = new ExpansionHardlinePlayerData();
		
		if (!m_HardlinePlayerData.OnRecieve(ctx))
		{
			Error("ExpansionHardlineModule::RPC_SendPlayerHardlineData - Error on recieving hardline player data!");
			return;
		}
		
		if (!GetExpansionSettings().GetHardline().OnRecieve(ctx))
			return;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			player.SetIsBandit(false);
			player.SetIsHero(false);
			
			if (m_HardlinePlayerData.GetHumanity() <= GetExpansionSettings().GetHardline().RankKleptomaniac)
			{
				player.SetIsBandit(true);
			}
			else if (m_HardlinePlayerData.GetHumanity() >= GetExpansionSettings().GetHardline().RankScout)			
			{
				player.SetIsHero(true);
			}
			
			player.SetHumanity(m_HardlinePlayerData.GetHumanity());
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule GetHardlineClientData
	// Called on client
	// ------------------------------------------------------------	
	ExpansionHardlinePlayerData GetHardlineClientData()
	{
		return m_HardlinePlayerData;
	}
	
	// ------------------------------------------------------------
	// ExpansionHardlineModule GetHardlineHUDInvoker
	// Called on client
	// ------------------------------------------------------------
	ScriptInvoker GetHardlineHUDInvoker()
	{
		return m_HardlineHUDInvoker;
	}
 };