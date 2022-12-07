/**
 * ExpansionQuestModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionQuestModuleCallback
{
	NONE = 0,
	CANCELED_QUEST = 2,
	MISSING_REQUIRED_ITEMS = 4
};

[CF_RegisterModule(ExpansionQuestModule)]
class ExpansionQuestModule: CF_ModuleWorld
{
	//! Server only
	protected ref map<string, ref ExpansionQuestPersistentData> m_PlayerDatas; //! Server

#ifdef EXPANSIONMODGROUPS
	protected ref map<string, ref array<int>> m_PlayerGroups; //! Server
#endif
	protected ref array<ref ExpansionQuest> m_ActiveQuests; //! Server
	protected ref map<int, ref ExpansionQuestNPCData> m_QuestsNPCs; //! Server
	protected ref map<int, ref ExpansionQuestConfig> m_QuestConfigs;	//! Server

	protected ref map<int, ExpansionQuestNPCBase> m_QuestNPCEntities; //! Server
#ifdef EXPANSIONMODAI
	protected ref map<int, ExpansionQuestNPCAIBase> m_QuestNPCAIEntities; //! Server
#endif
	protected ref map<int, ExpansionQuestStaticObject> m_QuestObjectEntities; //! Server

	//! Default server data
	protected ref ExpansionDefaultQuestNPCData m_DefaultQuestNPCData; //! Server
	protected ref ExpansionDefaultQuestData m_DefaultQuestConfigData;  //! Server
	protected ref ExpansionDefaultObjectiveData m_DefaultObjectiveConfigData; //! Server

	protected ref map<int, ref ExpansionQuestObjectiveTravelConfig> m_TravelObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveDeliveryConfig> m_DeliveryObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveTargetConfig> m_TargetObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveCollectionConfig> m_CollectionObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveTreasureHuntConfig> m_TreasureHuntObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveActionConfig> m_ActionObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveCraftingConfig> m_CraftingObjectivesConfigs; //! Server

#ifdef EXPANSIONMODAI
	protected ref map<int, ref ExpansionQuestObjectiveAIPatrolConfig> m_AIPatrolObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveAICampConfig> m_AICampObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveAIVIPConfig> m_AIVIPObjectivesConfigs; //! Server
	static ref map<int, ref array<eAIDynamicPatrol>> m_GlobalAIPatrols;
#endif

	protected ref array<ref ExpansionQuestObjectSet> m_QuestObjectSets;	//! Server

	protected float m_UpdateQueueTimer; //! Server update que timer
	protected int m_CurrentQuestTick; //! Server
	protected const float UPDATE_TICK_TIME = 1.0; // refreshes 100 active quests every ten seconds (10 every sec.)
	protected const int UPDATE_QUESTS_PER_TICK = 10; //! Server

	protected float m_CheckResetTimer;
	protected const float CHECK_TICK_TIME = 60.0;
	protected ref ExpansionQuestPersistentServerData m_ServerData; //! Server

	//! Client only
	protected ref map<int, ref ExpansionQuestConfig> m_QuestClientConfigs;	//! Client
	protected ref ExpansionQuestPersistentData m_PlayerQuestData; //! Client
	protected ref ScriptInvoker m_QuestMenuInvoker; //! Client
#ifdef EXPANSIONMODBOOK
	protected ref ScriptInvoker m_QuestLogInvoker; //! Client
	protected ref ScriptInvoker m_QuestLogCallbackInvoker; //! Client
#endif

#ifdef EXPANSIONMODNAVIGATION
	protected ref array<ref ExpansionQuestClientMarker> m_QuestMarkers; //! Client
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::OnInit - Start");

		super.OnInit();

		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		EnableClientLogout();
		EnableClientLogoutCancelled();
		EnableClientNew();
		EnableClientReady();
		EnableClientDisconnect();
		EnableUpdate();
		EnableRPC();

		QuestModulePrint(ToString() + "::OnInit - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateDirectoryStructure
	// Server
	// ------------------------------------------------------------
	protected void CreateDirectoryStructure()
	{
		if (!FileExist(EXPANSION_QUESTS_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_QUESTS_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_QUESTS_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_PLAYERDATA_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_PLAYERDATA_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_NPCS_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_NPCS_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER);

	#ifdef EXPANSIONMODAI
		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);

		if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER))
			MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
	#endif
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnMissionLoaded");
	#endif

		QuestModulePrint(ToString() + "::OnMissionLoaded - Start");

		super.OnMissionLoaded(sender, args);

		m_QuestsNPCs = new map<int, ref ExpansionQuestNPCData>; //! Server & Client
		m_QuestConfigs = new map<int, ref ExpansionQuestConfig>; //! Server & Client
		m_PlayerDatas = new map<string, ref ExpansionQuestPersistentData>; //! Server & Client

	#ifdef EXPANSIONMODGROUPS
		m_PlayerGroups = new map<string, ref array<int>>; //! Server & Client
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			ServerModuleInit();
		}

		if (GetGame().IsClient())
		{
			ClientModuleInit();
		}

		QuestModulePrint(ToString() + "::OnMissionLoaded - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ServerModuleInit
	// ------------------------------------------------------------
	void ServerModuleInit()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "ServerModuleInit");
	#endif

		QuestModulePrint(ToString() + "::ServerModuleInit - Start");

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_TravelObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTravelConfig>;	//! Server
			m_DeliveryObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveDeliveryConfig>; //! Server
			m_TargetObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTargetConfig>; //! Server
			m_CollectionObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveCollectionConfig>; //! Server
			m_TreasureHuntObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTreasureHuntConfig>; //! Server
			m_ActionObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveActionConfig>; //! Server
			m_CraftingObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveCraftingConfig>; //! Server

		#ifdef EXPANSIONMODAI
			m_AIPatrolObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIPatrolConfig>; //! Server
			m_AICampObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAICampConfig>; //! Server
			m_AIVIPObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIVIPConfig>; //! Server
			m_GlobalAIPatrols = new map<int, ref array<eAIDynamicPatrol>>; //! Server
		#endif

			m_QuestObjectSets = new array<ref ExpansionQuestObjectSet>;

			m_ActiveQuests = new array<ref ExpansionQuest>;
			m_QuestNPCEntities = new map<int, ExpansionQuestNPCBase>;

		#ifdef EXPANSIONMODAI
			m_QuestNPCAIEntities = new map<int, ExpansionQuestNPCAIBase>;
		#endif

			m_QuestObjectEntities = new map<int, ExpansionQuestStaticObject>;

			if (!GetExpansionSettings().GetQuest().EnableQuests)
				return;

			CreateDirectoryStructure();

			string worldName = ExpansionStatic.GetCanonicalWorldName();

			m_DefaultQuestNPCData = new ExpansionDefaultQuestNPCData(worldName);
			m_DefaultQuestConfigData = new ExpansionDefaultQuestData(worldName);
			m_DefaultObjectiveConfigData = new ExpansionDefaultObjectiveData(worldName);

			if (!FileExist(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE))
			{
				m_ServerData = new ExpansionQuestPersistentServerData();
				m_ServerData.Save();
			}
			else
			{
				m_ServerData = ExpansionQuestPersistentServerData.Load();
			}

			//! OBJECTIVE FILES
			//! TRAVEL OBJECTIVES
			array<string> travelObjectiveFiles = new array<string>;
			travelObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER, ".json");
			if (travelObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(travelObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER);
			}
			else
			{
				DefaultTravelObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! TARGET OBJECTIVES
			array<string> targetObjectiveFiles = new array<string>;
			targetObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER, ".json");
			if (targetObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(targetObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER);
			}
			else
			{
				DefaultTargetObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! DELIVERY OBJECTIVES
			array<string> deliveryObjectiveFiles = new array<string>;
			deliveryObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER, ".json");
			if (deliveryObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(deliveryObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER);
			}
			else
			{
				DefaultDeliveryObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! COLLECTION OBJECTIVES
			array<string> collectionObjectiveFiles = new array<string>;
			collectionObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER, ".json");
			if (collectionObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(collectionObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER);
			}
			else
			{
				DefaultCollectionObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! TREASURE HUNT OBJECTIVES
			array<string> treasureHuntObjectiveFiles = new array<string>;
			treasureHuntObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER, ".json");
			if (treasureHuntObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(treasureHuntObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);
			}
			else
			{
				DefaultTreasureHuntObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! ACTION OBJECTIVES
			array<string> actionObjectiveFiles = new array<string>;
			actionObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER, ".json");
			if (actionObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(actionObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER);
			}
			else
			{
				DefaultActionObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! CRAFTING OBJECTIVES
			array<string> craftingObjectiveFiles = new array<string>;
			craftingObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER, ".json");
			if (craftingObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(craftingObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER);
			}
			else
			{
				DefaultCraftingObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

		#ifdef EXPANSIONMODAI
			//! AI PATROL OBJECTIVES
			array<string> aiPatrolObjectiveFiles = new array<string>;
			aiPatrolObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER, ".json");
			if (aiPatrolObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(aiPatrolObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);
			}
			else
			{
				DefaultAIPatrolObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! AI CAMP OBJECTIVES
			array<string> aiCampObjectiveFiles = new array<string>;
			aiCampObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER, ".json");
			if (aiCampObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(aiCampObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);
			}
			else
			{
				DefaultAICampObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! AI VIP OBJECTIVES
			array<string> aiVIPObjectiveFiles = new array<string>;
			aiVIPObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER, ".json");
			if (aiVIPObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(aiVIPObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
			}
			else
			{
				DefaultAIVIPObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}
		#endif

			//! QUEST FILES
			array<string> questFiles = new array<string>;
			questFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_QUESTS_FOLDER, ".json");
			if (questFiles.Count() > 0)
			{
				LoadQuestData(questFiles); //! Server: Load existing quest data files from the server and load them into m_QuestConfigs.
			}
			else
			{
				DefaultQuestData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! GET QUEST OBJECT SETS FROM QUESTS
			LoadObjectSets();

			//! QUEST NPC FILES
			array<string> questNPCFiles = new array<string>;
			questNPCFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_NPCS_FOLDER, ".json");
			if (questNPCFiles.Count() > 0)
			{
				LoadQuestNPCData(questNPCFiles); //! Server: Load existing quest NPCs data files from the server and load them into m_QuestsNPCs.
			}
			else
			{
				DefaultQuestNPCData(); //! Server: Create default quest NPCs data on the server and load them into m_QuestsNPCs.
			}

			SpawnQuestNPCs(); //! Server: Spawn all quest NPCs on the server based on the loaded data.

			PlayerQuestDataCheck(); //! Server: Debug check method to scan persistent quest data.
		}

		QuestModulePrint(ToString() + "::ServerModuleInit - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ClientModuleInit
	// ------------------------------------------------------------
	void ClientModuleInit()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "ClientModuleInit");
	#endif

		QuestModulePrint(ToString() + "::ClientModuleInit - Start");

		if (GetGame().IsClient())
		{
			if (!m_QuestClientConfigs)
				m_QuestClientConfigs = new map<int, ref ExpansionQuestConfig>; //! Client

			if (!m_QuestMenuInvoker)
				m_QuestMenuInvoker = new ScriptInvoker(); //! Client

		#ifdef EXPANSIONMODBOOK
			if (!m_QuestLogInvoker)
				m_QuestLogInvoker = new ScriptInvoker(); //! Client

			if (!m_QuestLogCallbackInvoker)
				m_QuestLogCallbackInvoker = new ScriptInvoker(); //! Client
		#endif

		#ifdef EXPANSIONMODNAVIGATION
			if (!m_QuestMarkers)
				m_QuestMarkers = new array<ref ExpansionQuestClientMarker>; //! Client
		#endif
		}

		QuestModulePrint(ToString() + "::ClientModuleInit - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnInvokeConnect
	// ------------------------------------------------------------
	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnInvokeConnect");
	#endif

		QuestModulePrint(ToString() + "::OnInvokeConnect - Start");

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			QuestModulePrint(ToString() + "::OnInvokeConnect - Setup player quests for player with UID: " + cArgs.Identity.GetId());
			SetupClientData(cArgs.Identity);
		}

		QuestModulePrint(ToString() + "::OnInvokeConnect - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientDisconnect
	// -----------------------------------------------------------
	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnClientDisconnect");
	#endif

		QuestModulePrint(ToString() + "::OnClientDisconnect - Start");

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		//! [Server] Player disconnects and we look for any existing ExpansionQuestPersistentData of the player with his UID.
		// If we find existing data we sync that data from the players client to the server and save it for the next session.
		//! Maybe move this to the OnClientLogout method
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			CleanupPlayerQuests(cArgs.UID);
		}

		QuestModulePrint(ToString() + "::OnClientDisconnect - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientLogout
	// -----------------------------------------------------------
	override void OnClientLogout(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnClientLogout");
	#endif

		QuestModulePrint(ToString() + "::OnClientLogout - Start");

		super.OnClientLogout(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			if (!cArgs.Identity)
				return;

			HandleClientLogout(cArgs.Identity);
		}

		QuestModulePrint(ToString() + "::OnClientLogout - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientLogout
	// -----------------------------------------------------------
	override void OnClientLogoutCancelled(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnClientLogoutCancelled");
	#endif

		QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Start");

		super.OnClientLogoutCancelled(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - cArgs: " + cArgs.ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Player: " + cArgs.Player.ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Identity from cArgs: " + cArgs.Identity.ToString()); //! Always NULL Welp
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Identity from Player: " + cArgs.Player.GetIdentity().ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - UID: " + cArgs.Player.GetIdentity().GetId());

			if (!cArgs.Player)
				return;

			HandleClientLogoutCancelled(cArgs.Player.GetIdentity().GetId());
		}

		QuestModulePrint(ToString() + "::OnClientLogoutCancelled - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnMissionFinish
	// -----------------------------------------------------------
	override void OnMissionFinish(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnMissionFinish - Start");

		super.OnMissionFinish(sender, args);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
			{
				ExpansionQuest quest = m_ActiveQuests[i];
				//! Update and save the persistent player quest data
				UpdateQuestForQuestPlayers(quest, quest.GetQuestState());

				//! Delete quest
				quest.OnQuestCleanup();
				//quest.QuestDebug();
				m_ActiveQuests.RemoveOrdered(i);
			}
		}

		QuestModulePrint(ToString() + "::OnMissionFinish - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule HandleClientLogoutCancelled
	// -----------------------------------------------------------
	//! Handles recreation of quest markers and quest items if
	//! a quest player cancles a logout.
	protected void HandleClientLogoutCancelled(string playerUID)
	{
		//! Prepare recreation of all quest markers for the client
		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (!quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID))
				continue;
		#endif

		#ifdef EXPANSIONMODNAVIGATION
			quest.OnRecreateClientMarkers();
		#endif
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionQuestModuleRPC.INVALID;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionQuestModuleRPC.COUNT;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetRPCMax
	// ------------------------------------------------------------
	override void OnRPC(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnRPC");
	#endif

		QuestModulePrint(ToString() + "::OnRPC - Start");

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionQuestModuleRPC.SendPlayerQuestData:
			{
				RPC_SendPlayerQuestData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RequestOpenQuestMenu:
			{
				RPC_RequestOpenQuestMenu(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.CreateQuestInstance:
			{
				RPC_CreateQuestInstance(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RequestCompleteQuest:
			{
				RPC_RequestCompleteQuest(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.CancelQuest:
			{
				RPC_CancelQuest(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.CallbackClient:
			{
				RPC_CallbackClient(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RequestPlayerQuests:
			{
				RPC_RequestPlayerQuests(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.SendPlayerQuests:
			{
				RPC_SendPlayerQuests(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		#ifdef EXPANSIONMODNAVIGATION
			case ExpansionQuestModuleRPC.CreateClientMarker:
			{
				RPC_CreateClientMarker(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RemoveClientMarkers:
			{
				RPC_RemoveClientMarkers(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		#endif
		}

		QuestModulePrint(ToString() + "::OnRPC - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestPlayerQuests
	// Called on client
	// ------------------------------------------------------------
	//! Called to update players quest data on the client after it has been changed
	void RequestPlayerQuests()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RequestPlayerQuests - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestPlayerQuests - Tryed to call RequestPlayerQuests on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestPlayerQuests, true);

		QuestModulePrint(ToString() + "::RequestPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdatePlayerQuests
	// Called on server
	// ------------------------------------------------------------
	void UpdatePlayerQuests(ExpansionQuest quest)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::UpdatePlayerQuests - Start");

		if (!quest)
		{
			Error(ToString() + "::UpdatePlayerQuests - Could not get quest!");
			return;
		}

	#ifdef EXPANSIONMODGROUPS
		ExpansionPartyModule partyModule;
		if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			Error(ToString() + "::UpdatePlayerQuests - Could not get party module!");
			return;
		}
	#endif

		if (!quest.IsGroupQuest())
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(quest.GetPlayerUID());
			if (player)
				SendPlayerQuestServer(player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			ExpansionPartyData questGroup = partyModule.GetPartyByID(quest.GetGroupID());
			if (!questGroup)
			{
				Error(ToString() + "::UpdatePlayerQuests - Could not get group data!");
				return;
			}

			array<ref ExpansionPartyPlayerData> groupPlayers = questGroup.GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				string playerUID = playerGroupData.GetID();
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (!groupPlayer)
					continue;

				SendPlayerQuestServer(groupPlayer.GetIdentity());
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdatePlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SendPlayerQuestServer
	// Called on server
	// ------------------------------------------------------------
	void SendPlayerQuestServer(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this, "SendPlayerQuestServer", identity.GetId());

		QuestModulePrint(ToString() + "::SendPlayerQuestServer - Start");
		string playerUID = identity.GetId();
		array<ref ExpansionQuestConfig> validQuests = GetPlayerQuests(playerUID);
		if (!validQuests)
		{
			QuestModulePrint(ToString() + "::SendPlayerQuestServer - Could not get any valid quest for player with UID: " + playerUID);
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		int questCount = validQuests.Count();
		rpc.Write(questCount);

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = validQuests[i];
			if (!questConfig)
			{
				Error(ToString() + "::SendPlayerQuestServer - Coult on get quest config data from valid quest array!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuests, true, identity);

		QuestModulePrint(ToString() + "::SendPlayerQuestServer - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestPlayerQuests
	// Called on server
	// ------------------------------------------------------------
	protected void RPC_RequestPlayerQuests(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_RequestPlayerQuests - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestPlayerQuests - Tryed to call RPC_RequestPlayerQuests on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::RPC_RequestPlayerQuests - Could not get player identity!");
			return;
		}

		SendPlayerQuestData(identity);

		string playerUID = identity.GetId();
		array<ref ExpansionQuestConfig> validQuests = GetPlayerQuests(playerUID);
		if (!validQuests)
		{
			Error(ToString() + "::SendPlayerQuestData - Could not create valid quest array for player with UID: " + playerUID);
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		int questCount = validQuests.Count();
		rpc.Write(questCount);

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = validQuests[i];
			if (!questConfig)
			{
				Error(ToString() + "::RPC_RequestPlayerQuests - Coult on get quest config data from valid quest array!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuests, true, identity);

		QuestModulePrint(ToString() + "::RPC_RequestPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendPlayerQuests
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_SendPlayerQuests(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuests - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendPlayerQuests - Tryed to call RPC_SendPlayerQuests on Server!");
			return;
		}

		if (!m_QuestClientConfigs)
			m_QuestClientConfigs = new map<int, ref ExpansionQuestConfig>;

		int questCount;
		if (!ctx.Read(questCount))
		{
			Error(ToString() + "::RPC_SendPlayerQuests - Could not read quest config data count!");
			return;
		}

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = new ExpansionQuestConfig();
			if (!questConfig.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendPlayerQuests - Error on recieving quest config!");
				return;
			}

		#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
			questConfig.QuestDebug();
		#endif

			m_QuestClientConfigs.Insert(questConfig.GetID(), questConfig);
		}

	#ifdef EXPANSIONMODBOOK
		GetQuestLogSI().Invoke(m_QuestClientConfigs);
	#endif

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SendPlayerQuestData
	// Called on server
	// ------------------------------------------------------------
	//! Called to update players quest data on the client
	void SendPlayerQuestData(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this, "SendPlayerQuestData", identity.GetId());

		QuestModulePrint(ToString() + "::SendPlayerQuestData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendPlayerQuestData - Tryed to call SendPlayerQuestData on Client!");
			return;
		}

		if (identity)
		{
			//! Get existing player quest data if there is a exiting one in m_PlayerDatas
			string playerUID = identity.GetId();
			ExpansionQuestPersistentData questPlayerData = GetPlayerQuestDataByUID(playerUID);
			if (questPlayerData)
			{
				QuestModulePrint(ToString() + "::SendPlayerQuestData - Send player quest data to client for UID: " + playerUID);
				auto rpc = ExpansionScriptRPC.Create();
				questPlayerData.OnWrite(rpc);
				rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuestData, true, identity);
			}
		}

		QuestModulePrint(ToString() + "::SendPlayerQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendPlayerQuestData
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_SendPlayerQuestData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuestData - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendPlayerQuestData - Tryed to call RPC_SendPlayerQuestData on Server!");
			return;
		}

		ExpansionQuestPersistentData data = new ExpansionQuestPersistentData();
		//! Need to apply the variable from a new instance here here and cant use the existing one?!
		if (!data.OnRead(ctx))
		{
			Error(ToString() + "::RPC_SendPlayerQuestData - Error on recieving quest player data!");
			return;
		}

		m_PlayerQuestData = data;
		m_PlayerQuestData.SetQuestModule(this);
		m_PlayerQuestData.QuestDebug();

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuestData - m_PlayerQuestData: " + m_PlayerQuestData);
		QuestModulePrint(ToString() + "::RPC_SendPlayerQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SetupClientData
	// Called on server
	// ------------------------------------------------------------
	//! This method is only called once on every player connection to recreate his quests
	// and to send the needed data from the quest module to the client
	void SetupClientData(PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::SetupClientData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SetupClientData - Tryed to call SetupClientData on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::SetupClientData - Identity is NULL!");
			return;
		}

		string playerUID = identity.GetId();
		ExpansionQuestPersistentData questPlayerData = GetPlayerQuestDataByUID(playerUID);
		if (!questPlayerData)
		{
			//! If we don't have cached player quest data, check if file exists and load it, else use fresh instance as-is
			questPlayerData = new ExpansionQuestPersistentData();
			m_PlayerDatas.Insert(playerUID, questPlayerData);
			if (questPlayerData.Load(playerUID))
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - SetupClientData - Loaded existing player quest data for player with UID: " + playerUID);
			}
			else
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - SetupClientData - Created new persistent player quest data for player UID: " + playerUID);
			}
		}
		else
		{
			GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - SetupClientData - Got cached player quest data for player with UID: " + playerUID);
			questPlayerData.QuestDebug();
		}

		questPlayerData.SetQuestModule(this);
		PlayerQuestsInit(questPlayerData, identity); //! If the player has existing quest data we need to initialize any exisitng quests that he started on the server
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendPlayerQuestServer, 3000, false, identity);
		QuestModulePrint(ToString() + "::SetupClientData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestOpenQuestMenu
	// Called on server
	// ------------------------------------------------------------
	void RequestOpenQuestMenu(array<int> questNPCIDs, PlayerIdentity identity)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(identity.GetId());
		if (!player)
			return;

		Object target = GetClosestQuestNPCByID(questNPCIDs, player.GetPosition());
		if (!target)
			target = GetClosestQuestObjectByID(questNPCIDs, player.GetPosition());
	#ifdef EXPANSIONMODAI
		if (!target)
			target = GetClosestQuestNPCAIByID(questNPCIDs, player.GetPosition());
	#endif

		if (target)
			RequestOpenQuestMenu(target, identity);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestOpenQuestMenu
	// Called on server
	// ------------------------------------------------------------
	void RequestOpenQuestMenu(Object target, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RequestOpenQuestMenu - Tryed to call RequestOpenQuestMenu on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::RequestOpenQuestMenu - identity is NULL!");
			return;
		}

		auto npc = ExpansionQuestNPCBase.Cast(target);
	#ifdef ENFUSION_AI_PROJECT
		auto npcAI = ExpansionQuestNPCAIBase.Cast(target);
	#endif
		auto npcObject = ExpansionQuestStaticObject.Cast(target);

	#ifdef ENFUSION_AI_PROJECT
		if (!npc && !npcAI && !npcObject)
	#else
		if (!npc && !npcObject)
	#endif
		{
			Error(ToString() + "::RequestOpenQuestMenu - NPC is NULL!");
			return;
		}

		int questNPCID;
		if (npc)
			questNPCID = npc.GetQuestNPCID();
	#ifdef ENFUSION_AI_PROJECT
		else if (npcAI)
			questNPCID = npcAI.GetQuestNPCID();
	#endif
		else if (npcObject)
			questNPCID = npcObject.GetQuestNPCID();

		ExpansionQuestNPCData questNPCData = GetQuestNPCDataByID(questNPCID);
		if (!questNPCData)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get quest npc data!");
			return;
		}

		SendPlayerQuestData(identity);

		string playerUID = identity.GetId();
		array<ref ExpansionQuestConfig> validQuests = new array<ref ExpansionQuestConfig>;
		validQuests = GetNPCQuests(questNPCID, playerUID);
		if (!validQuests)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not create npc quest array!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questNPCData.GetDefaultNPCText());

		int questCount = validQuests.Count();
		rpc.Write(questCount);

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = validQuests[i];
			if (!questConfig)
			{
				Error(ToString() + "::RequestOpenQuestMenu - Could not get quest config!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

	#ifdef EXPANSIONMODAI
		if (npcAI)
		{
			EmoteManager npcEmoteManager = npcAI.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				if (validQuests.Count() > 0)
					npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_GREETING);
				else if (validQuests.Count() == 0)
					npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_SHRUG);

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif

		rpc.Send(target, ExpansionQuestModuleRPC.RequestOpenQuestMenu, true, identity);

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestOpenQuestMenu
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_RequestOpenQuestMenu(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Tryed to call RPC_RequestOpenQuestMenu on Server!");
			return;
		}

		if (!target)
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - No target!");
			return;
		}

		string defaultText;
		if (!ctx.Read(defaultText))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read default text!");
			return;
		}

		int questCount;
		if (!ctx.Read(questCount))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read quest data count!");
			return;
		}

		array<ref ExpansionQuestConfig> validQuests = new array<ref ExpansionQuestConfig>;
		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = new ExpansionQuestConfig();
			if (!questConfig.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_RequestOpenQuestMenu - Error on recieving quest config!");
				return;
			}

			questConfig.QuestDebug();
			validQuests.Insert(questConfig);
		}

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Valid quests: " + validQuests.Count());

		OpenQuestMenu();
		m_QuestMenuInvoker.Invoke(validQuests, target.GetDisplayName(), defaultText);

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Called on Server
	// ------------------------------------------------------------
	ExpansionQuest CreateQuestInstance(string className)
	{
		return ExpansionQuest.Cast(className.ToType().Spawn());
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Called on Client
	// ------------------------------------------------------------
	void CreateQuestInstance(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CreateQuestInstance - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::CreateQuestInstance - Tryed to call CreateQuestInstance on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CreateQuestInstance, true);

		QuestModulePrint(ToString() + "::CreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstanceServer
	// Called on Server
	// ------------------------------------------------------------
	void CreateQuestInstanceServer(int questID, string playerUID)
	{
		QuestModulePrint(ToString() + "::CreateQuestInstanceServer - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CreateQuestInstanceServer - Tryed to call CreateQuestInstanceServer on Client!");
			return;
		}

		if (playerUID == string.Empty)
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player || !player.GetIdentity())
			return;

		CreateQuest(questID, player.GetIdentity());

		QuestModulePrint(ToString() + "::CreateQuestInstanceServer - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CreateQuestInstance
	// Called on Server
	// ------------------------------------------------------------
	protected void RPC_CreateQuestInstance(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_CreateQuestInstance - Tryed to call CreateQuestInstance on Client!");
			return;
		}

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_CreateQuestInstance - Could not get read quest ID!");
			return;
		}

		CreateQuest(questID, senderRPC);

		QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuest
	// Called on Server
	// ------------------------------------------------------------
	protected void CreateQuest(int questID, PlayerIdentity sender)
	{
		//! Get quest config instance and create a copy of that config for our new quest instance
		ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
		if (!configInstance)
			return;

		string playerUID = sender.GetId();
		ExpansionQuestPersistentData playerData = GetPlayerQuestDataByUID(playerUID);
		if (!playerData)
			return;

		if (!playerData.HasDataForQuest(questID))
		{
			playerData.AddQuestData(configInstance);
		}
		//! Check if the quest is a daylie/weekly quest and if the player has still a cooldown on it.
		else if (playerData.HasDataForQuest(questID) && (configInstance.IsDailyQuest() || configInstance.IsWeeklyQuest()))
		{
			int daylie = 86400; //! Day in seconds
			int weekly = 604800; //! Week in seconds
			bool hasCooldown = false;
			int timeDifference;
			int cooldown;

			if (configInstance.IsDailyQuest())
			{
				hasCooldown = playerData.HasCooldown(configInstance.GetID(), daylie, timeDifference);
				cooldown = daylie - timeDifference;
			}
			else if (configInstance.IsWeeklyQuest())
			{
				hasCooldown = playerData.HasCooldown(configInstance.GetID(), weekly, timeDifference);
				cooldown = weekly - timeDifference;
			}

			if (hasCooldown)
			{
				string timestamp = ExpansionStatic.FormatTimestamp(cooldown, false);
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCooldownTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCooldownText, timestamp), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}
		}

		configInstance.QuestDebug();

		//! Create new quest
		ExpansionQuest newQuestInstance;
		if (configInstance.GetType() == ExpansionQuestType.SCRIPTED && configInstance.GetQuestClassName() != string.Empty)
		{
			newQuestInstance = CreateQuestInstance(configInstance.GetQuestClassName());
		}
		else
		{
			newQuestInstance = new ExpansionQuest(this);
		}

		if (!newQuestInstance)
		{
			Error(ToString() + "::RPC_CreateQuestInstance - Could not create new quest instance!");
			return;
		}

		newQuestInstance.SetPlayerUID(playerUID);

	#ifdef EXPANSIONMODGROUPS
		if (configInstance.IsGroupQuest())
		{
			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Quest is a group quest. Apply needed parameters!");
			//! If the player that is accepting the quest has a group we set the group owner as the main quest player
			ExpansionPartyModule partyModule;
			if (!Class.CastTo(partyModule, CF_ModuleCoreManager.Get(ExpansionPartyModule)))
			{
				Error(ToString() + "::RPC_CreateQuestInstance - Could not get party module!");
				return;
			}

			ExpansionPartyPlayerData groupPlayerData = partyModule.GetPartyPlayerData(playerUID);
			if (!groupPlayerData)
			{
				//! Send notification that quest can only be accepted while in a group.
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			int partyID = groupPlayerData.GetParty().GetPartyID();
			ExpansionPartyData groupData = partyModule.GetPartyByID(partyID);
			if (!groupData)
			{
				Error(ToString() + "::RPC_CreateQuestInstance - Could not get party data for party with ID: " + partyID);
				return;
			}

			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Group data of player with ID: " + playerUID);
			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Group data: " + groupData.ToString());
			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Group ID: " + groupData.GetPartyID());

			//! Only group members accept quest!
			auto settings = GetExpansionSettings().GetQuest();
			if (settings.GroupQuestMode < 1 && groupData.GetOwnerUID() != playerUID)
			{
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			newQuestInstance.SetGroup(partyID);

			array<ref ExpansionPartyPlayerData> groupPlayers = groupData.GetPlayers();
			foreach (ExpansionPartyPlayerData playerPartyData: groupPlayers)
			{
				ExpansionQuestPersistentData partyPlayerQuestData = GetPlayerQuestDataByUID(playerPartyData.GetID());
				if (!partyPlayerQuestData)
					continue;

				if (!partyPlayerQuestData.HasDataForQuest(questID))
				{
					partyPlayerQuestData.AddQuestData(configInstance);
				}
			}
		}
	#endif

		newQuestInstance.SetQuestConfig(configInstance);

		//newQuestInstance.QuestDebug();

		m_ActiveQuests.Insert(newQuestInstance);

		if (!newQuestInstance.OnQuestStart())
		{
			QuestModulePrint(ToString() + "::CreateQuestInstance - Quest start event failed for quest with quest id: " + questID);
			CancelQuestServer(newQuestInstance.GetQuestConfig().GetID(), sender);
			return;
		}

	#ifdef EXPANSIONMODAI
		ExpansionQuestNPCAIBase npc = GetClosestQuestAINPC(configInstance.GetQuestGiverIDs(), newQuestInstance.GetPlayer().GetPosition());
		if (npc)
		{
			EmoteManager npcEmoteManager = npc.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_NOD);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CreateQuestInstance - Created new quest instance for player with UID " + sender.GetId() + " for quest " + questID);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Client
	// ------------------------------------------------------------
	protected void CompleteQuestClient(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CompleteQuestClient - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::CompleteQuestClient - Tryed to call CompleteQuestClient on Server!");
			return;
		}

		string playerUID = GetGame().GetPlayer().GetIdentity().GetId();
		if (playerUID == string.Empty)
		{
			Error(ToString() + "::CompleteQuestClient - Could not get player UID!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Write(playerUID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CompleteQuest, true);

		QuestModulePrint(ToString() + "::CompleteQuestClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HandleClientLogout
	// Called on server
	// ------------------------------------------------------------
	//! Handles deletion of quest markers and quest items if
	//! a quest player starts a logout.
	void HandleClientLogout(PlayerIdentity identity)
	{
		//! Prepare client marker deletion
		string playerUID = identity.GetId();
		foreach (ExpansionQuest currentQuest: m_ActiveQuests)
		{
			if (!currentQuest.IsGroupQuest() && currentQuest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (currentQuest.IsGroupQuest() && !currentQuest.IsQuestGroupMember(playerUID))
				continue;
		#endif

		#ifdef EXPANSIONMODNAVIGATION
			int questID = currentQuest.GetQuestConfig().GetID();
			if (!currentQuest.IsGroupQuest())
			{
				RemoveClientMarkers(questID, identity);
			}
		#ifdef EXPANSIONMODGROUPS
			else if (currentQuest.IsGroupQuest())
			{
				RemoveClientMarkers(questID, identity);
			}
		#endif
		#endif
		}
	}

#ifdef EXPANSIONMODNAVIGATION
	// ------------------------------------------------------------
	// ExpansionQuestModule CreateClientMarker
	// Called on server
	// ------------------------------------------------------------
	void CreateClientMarker(vector pos, string text, int questID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CreateClientMarker - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CreateClientMarker - Tryed to call CreateClientMarker on Client!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(pos);
		rpc.Write(text);
		rpc.Write(questID);
		rpc.Write(GetExpansionSettings().GetMap().CanCreate3DMarker);
		rpc.Send(null, ExpansionQuestModuleRPC.CreateClientMarker, true, identity);

		QuestModulePrint(ToString() + "::CreateClientMarker - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CreateClientMarker
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_CreateClientMarker(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_CreateClientMarker - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_CreateClientMarker - Tryed to call RPC_CreateClientMarker on Server!");
			return;
		}

		vector pos;
		if (!ctx.Read(pos))
			return;

		string text;
		if (!ctx.Read(text))
			return;

		int questID;
		if (!ctx.Read(questID))
			return;

		bool is3D;
		if (!ctx.Read(is3D))
			return;

		CreateMarkerClient(pos, text, questID, is3D);

		QuestModulePrint(ToString() + "::RPC_CreateClientMarker - End");
	}

	// ------------------------------------------------------------
	// ExpansionMarkerModule CreateMarkerClient
	// Called on client
	// ------------------------------------------------------------
	protected void CreateMarkerClient(vector pos, string text, int questID, bool is3D)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CreateMarkerClient - Start");

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		if (!markerModule)
			return;

		pos[1] = pos[1] + 2.0;
		ExpansionMarkerData markerData = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, "", false);
		markerData.SetName(text);
		markerData.SetIcon("Questionmark");
		markerData.SetColor(ARGB(255,241,196,15));
		markerData.SetPosition(pos);
		markerData.Set3D(is3D);
		markerData.SetLockState(true);
		markerModule.CreateMarker(markerData);

		string markerUID = markerData.GetUID();
		array<string> markerUIDs;

		QuestModulePrint("ExpansionMarkerModule::CreateMarkerClient - Create marker with UID: " + markerUID + " for quest " + questID);

		if (!m_QuestMarkers)
			m_QuestMarkers = new array<ref ExpansionQuestClientMarker>;

		ExpansionQuestClientMarker markerClientData = new ExpansionQuestClientMarker(questID, markerUID);
		m_QuestMarkers.Insert(markerClientData);

		QuestModulePrint(ToString() + "::CreateMarkerClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RemoveClientMarkers
	// Called on server
	// ------------------------------------------------------------
	void RemoveClientMarkers(int questID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RemoveClientMarkers - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RemoveClientMarkers - Tryed to call RemoveClientMarkers on Client!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Send(null, ExpansionQuestModuleRPC.RemoveClientMarkers, true, identity);

		QuestModulePrint(ToString() + "::RemoveClientMarkers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RemoveClientMarkers
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_RemoveClientMarkers(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_RemoveClientMarkers - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_RemoveClientMarkers - Tryed to call RPC_RemoveClientMarkers on Server!");
			return;
		}

		int questID;
		if (!ctx.Read(questID))
			return;

		RemoveMarkersClient(questID);

		QuestModulePrint(ToString() + "::RPC_RemoveClientMarkers - End");
	}

	// ------------------------------------------------------------
	// ExpansionMarkerModule CreateMarkerClient
	// Called on client
	// ------------------------------------------------------------
	protected void RemoveMarkersClient(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RemoveMarkersClient - Start");

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		if (!markerModule)
		{
			Error(ToString() + "::RemoveMarkersClient - Could not get Expansion Marker Module!");
			return;
		}

		QuestModulePrint("ExpansionMarkerModule::RemoveMarkersClient - m_QuestMarkers: " + m_QuestMarkers);
		QuestModulePrint("ExpansionMarkerModule::RemoveMarkersClient - m_QuestMarkers count: " + m_QuestMarkers.Count());

		foreach (ExpansionQuestClientMarker markerData: m_QuestMarkers)
		{
			string markerUID = markerData.GetMarkerUID();
			int markerQuestID = markerData.GetQuestID();

			if (markerQuestID != questID)
				continue;

			QuestModulePrint("ExpansionMarkerModule::RemoveMarkersClient - Remove marker with UID: " + markerUID + " for quest " + questID);
			markerModule.RemovePersonalMarkerByUID(markerUID);
		}

		QuestModulePrint(ToString() + "::RemoveMarkersClient - End");
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule CancelQuest
	// Called on client
	// ------------------------------------------------------------
	//! Called to update players quest data on the client after it has been changed
	//! Always call it with the init false parameter unless you know what you are doing
	void CancelQuest(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CancelQuest - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::CancelQuest - Tryed to call CancelQuest on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CancelQuest, true);

		QuestModulePrint(ToString() + "::CancelQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CancelQuest
	// Called on server
	// ------------------------------------------------------------
	protected void RPC_CancelQuest(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_CancelQuest - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_CancelQuest - Tryed to call RPC_CancelQuest on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::RPC_CancelQuest - Could not get identity!");
			return;
		}

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_CancelQuest - Could not read quest ID!");
			return;
		}

		CancelQuestServer(questID, identity);

		QuestModulePrint(ToString() + "::RPC_CancelQuest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule CancelQuestServer
	// Server
	// -----------------------------------------------------------
	void CancelQuestServer(int questID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CancelQuestServer - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CancelQuestServer - Tryed to call CancelQuestServer on Client!");
			return;
		}

		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::CancelQuestServer - Could not get quest from active quest array!");
				continue;
			}

			if (quest.GetQuestConfig().GetID() != questID || quest.GetPlayerUID() != identity.GetId())
				continue;

			int timestamp;
			if (!quest.IsGroupQuest())
			{
			#ifdef EXPANSIONMODNAVIGATION
				RemoveClientMarkers(questID, identity);
			#endif

				ExpansionQuestPersistentData playerData = GetPlayerQuestDataByUID(identity.GetId());
				if (!playerData)
				{
					Error(ToString() + "::CancelQuestServer - Could not get players quest data!");
					continue;
				}

				if (playerData.HasCooldownOnQuest(questID, timestamp))
				{
					playerData.UpdateQuestTimestamp(questID, 0);
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else if (quest.IsGroupQuest() && quest.GetGroup())
			{
				ExpansionPartyData group = quest.GetGroup();
				if (!group)
				{
					Error(ToString() + "::CancelQuestServer - Could not get players group data!");
					continue;
				}

				array<ref ExpansionPartyPlayerData> groupPlayers = quest.GetGroup().GetPlayers();
				foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
				{
					string groupPlayerUID = playerGroupData.GetID();
					ExpansionQuestPersistentData groupPlayerData = GetPlayerQuestDataByUID(groupPlayerUID);
					if (!groupPlayerData)
					{
						Error(ToString() + "::CancelQuestServer - Could not get group players quest data for UID: " + groupPlayerUID);
						continue;
					}

					if (groupPlayerData.HasCooldownOnQuest(questID, timestamp))
						playerData.UpdateQuestTimestamp(questID, 0);

					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(groupPlayerUID);
					if (!groupPlayer || !groupPlayer.GetIdentity())
						continue;

				#ifdef EXPANSIONMODNAVIGATION
					RemoveClientMarkers(questID, groupPlayer.GetIdentity());
				#endif

					auto rpc = ExpansionScriptRPC.Create();
					rpc.Write(ExpansionQuestModuleCallback.CANCELED_QUEST);
					rpc.Send(NULL, ExpansionQuestModuleRPC.CallbackClient, true, groupPlayer.GetIdentity());
				}
			}
		#endif

			if (!quest.OnQuestCancel())
				QuestModulePrint(ToString() + "::CancelQuestServer - Quest cancel evet failed for quest with quest id: " + questID);

			m_ActiveQuests.RemoveOrdered(i);
		}

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CancelQuestServer - Player with UID " + identity.GetId() + " has cancelled quest " + questID);
		QuestModulePrint(ToString() + "::CancelQuestServer - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule RPC_CallbackClient
	// Client
	// -----------------------------------------------------------
	protected void RPC_CallbackClient(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_CallbackClient - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_CallbackClient - Tryed to call RPC_CallbackClient on Server!");
			return;
		}

		int callBackID;
		if (!ctx.Read(callBackID))
		{
			Error(ToString() + "::RPC_CallbackClient - Could not callback ID!");
			return;
		}

		switch (callBackID)
		{
			case ExpansionQuestModuleCallback.CANCELED_QUEST:
			{
				QuestModulePrint(ToString() + "::RPC_CallbackClient - CANCELED_QUEST");
			#ifdef EXPANSIONMODBOOK
				m_QuestLogCallbackInvoker.Invoke();
			#endif
				break;
			}
		}

		QuestModulePrint(ToString() + "::RPC_CallbackClient - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultQuestNPCData
	// Server
	// -----------------------------------------------------------
	protected void DefaultQuestNPCData()
	{
		//! Quest NPC #1
		ExpansionQuestNPCData questNPC_1 = new ExpansionQuestNPCData();
		questNPC_1 = GetDefaultQuestNPCData().QuestNPCData_1();
		questNPC_1.Save("QuestNPC_1");
		m_QuestsNPCs.Insert(1, questNPC_1);

		//! Quest NPC #2
		ExpansionQuestNPCData questNPC_2 = new ExpansionQuestNPCData();
		questNPC_2 = GetDefaultQuestNPCData().QuestNPCData_2();
		questNPC_2.Save("QuestNPC_2");
		m_QuestsNPCs.Insert(2, questNPC_2);

		//! Quest NPC #3
		ExpansionQuestNPCData questNPC_3 = new ExpansionQuestNPCData();
		questNPC_3 = GetDefaultQuestNPCData().QuestNPCData_3();
		questNPC_3.Save("QuestNPC_3");
		m_QuestsNPCs.Insert(3, questNPC_3);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTravelObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultTravelObjectivesData()
	{
		ExpansionQuestObjectiveTravelConfig objective_t_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_001();
		objective_t_1.Save("Objective_T_1");
		m_TravelObjectivesConfigs.Insert(1, objective_t_1);

		ExpansionQuestObjectiveTravelConfig objective_t_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_002();
		objective_t_2.Save("Objective_T_2");
		m_TravelObjectivesConfigs.Insert(2, objective_t_2);

		ExpansionQuestObjectiveTravelConfig objective_t_3 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_003();
		objective_t_3.Save("Objective_T_3");
		m_TravelObjectivesConfigs.Insert(3, objective_t_3);

		ExpansionQuestObjectiveTravelConfig objective_t_4 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_004();
		objective_t_4.Save("Objective_T_4");
		m_TravelObjectivesConfigs.Insert(4, objective_t_4);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTravelObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultTargetObjectivesData()
	{
		ExpansionQuestObjectiveTargetConfig objective_ta_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Target_001();
		objective_ta_1.Save("Objective_TA_1");
		m_TargetObjectivesConfigs.Insert(1, objective_ta_1);

		ExpansionQuestObjectiveTargetConfig objective_ta_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Target_002();
		objective_ta_2.Save("Objective_TA_2");
		m_TargetObjectivesConfigs.Insert(2, objective_ta_2);

		ExpansionQuestObjectiveTargetConfig objective_ta_3 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Target_003();
		objective_ta_3.Save("Objective_TA_3");
		m_TargetObjectivesConfigs.Insert(3, objective_ta_3);

		ExpansionQuestObjectiveTargetConfig objective_ta_4 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Target_004();
		objective_ta_4.Save("Objective_TA_4");
		m_TargetObjectivesConfigs.Insert(4, objective_ta_4);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultDeliveryObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultDeliveryObjectivesData()
	{
		ExpansionQuestObjectiveDeliveryConfig objective_d_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Delivery_001();
		objective_d_1.Save("Objective_D_1");
		m_DeliveryObjectivesConfigs.Insert(1, objective_d_1);

		ExpansionQuestObjectiveDeliveryConfig objective_d_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Delivery_002();
		objective_d_2.Save("Objective_D_2");
		m_DeliveryObjectivesConfigs.Insert(2, objective_d_2);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultCollectionObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultCollectionObjectivesData()
	{
		ExpansionQuestObjectiveCollectionConfig objective_c_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_001();
		objective_c_1.Save("Objective_C_1");
		m_CollectionObjectivesConfigs.Insert(1, objective_c_1);

		ExpansionQuestObjectiveCollectionConfig objective_c_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_002();
		objective_c_2.Save("Objective_C_2");
		m_CollectionObjectivesConfigs.Insert(2, objective_c_2);

		ExpansionQuestObjectiveCollectionConfig objective_c_3 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_003();
		objective_c_3.Save("Objective_C_3");
		m_CollectionObjectivesConfigs.Insert(3, objective_c_3);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultTreasureHuntObjectivesData()
	{
		ExpansionQuestObjectiveTreasureHuntConfig objective_th_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_TreasureHunt_001();
		objective_th_1.Save("Objective_TH_1");
		m_TreasureHuntObjectivesConfigs.Insert(1, objective_th_1);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultActionObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultActionObjectivesData()
	{
		ExpansionQuestObjectiveActionConfig objective_a_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Action_001();
		objective_a_1.Save("Objective_A_1");
		m_ActionObjectivesConfigs.Insert(1, objective_a_1);

		ExpansionQuestObjectiveActionConfig objective_a_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Action_002();
		objective_a_2.Save("Objective_A_2");
		m_ActionObjectivesConfigs.Insert(2, objective_a_2);
		
		ExpansionQuestObjectiveActionConfig objective_a_3 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Action_003();
		objective_a_3.Save("Objective_A_3");
		m_ActionObjectivesConfigs.Insert(3, objective_a_3);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultCraftingObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultCraftingObjectivesData()
	{
		ExpansionQuestObjectiveCraftingConfig objective_cr_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Crafting_001();
		objective_cr_1.Save("Objective_CR_1");
		m_CraftingObjectivesConfigs.Insert(1, objective_cr_1);
	}

#ifdef EXPANSIONMODAI
	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultAIPatrolObjectivesData()
	{
		ExpansionQuestObjectiveAIPatrolConfig objective_aip_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIPatrol_001();
		objective_aip_1.Save("Objective_AIP_1");
		m_AIPatrolObjectivesConfigs.Insert(1, objective_aip_1);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultAICampObjectivesData()
	{
		ExpansionQuestObjectiveAICampConfig objective_aic_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AICamp_001();
		objective_aic_1.Save("Objective_AIC_1");
		m_AICampObjectivesConfigs.Insert(1, objective_aic_1);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultAIVIPObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultAIVIPObjectivesData()
	{
		ExpansionQuestObjectiveAIVIPConfig objective_aivip_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIVIP_001();
		objective_aivip_1.Save("Objective_AIVIP_1");
		m_AIVIPObjectivesConfigs.Insert(1, objective_aivip_1);
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultQuestData
	// Server
	// -----------------------------------------------------------
	protected void DefaultQuestData()
	{
		//! Quest #1 - Example template for a quest chain of 3 quests with different objectives (move to location + deliver item -> kill targets -> move to location + deliver item)
		ExpansionQuestConfig quest_1 = m_DefaultQuestConfigData.ExpansionQuestConfig001();
		quest_1.Save("Quest_1");
		m_QuestConfigs.Insert(1, quest_1);

		//! Quest #2
		ExpansionQuestConfig quest_2 = m_DefaultQuestConfigData.ExpansionQuestConfig002();
		quest_2.Save("Quest_2");
		m_QuestConfigs.Insert(2, quest_2);

		//! Quest #3
		ExpansionQuestConfig quest_3 = m_DefaultQuestConfigData.ExpansionQuestConfig003();
		quest_3.Save("Quest_3");
		m_QuestConfigs.Insert(3, quest_3);

		//! Quest #4 - Example template for a daylie treasure hunt quest that autocompletes
		ExpansionQuestConfig quest_4 = m_DefaultQuestConfigData.ExpansionQuestConfig004();
		quest_4.Save("Quest_4");
		m_QuestConfigs.Insert(4, quest_4);

	#ifdef EXPANSIONMODAI
		//! Quest #5 - Example template for a ai patrol target quest
		ExpansionQuestConfig quest_5 = m_DefaultQuestConfigData.ExpansionQuestConfig005();
		quest_5.Save("Quest_5");
		m_QuestConfigs.Insert(5, quest_5);

		//! Quest #6 - Example tempalte for a ai camp quest (Not working yet)
		ExpansionQuestConfig quest_6 = m_DefaultQuestConfigData.ExpansionQuestConfig006();
		quest_6.Save("Quest_6");
		m_QuestConfigs.Insert(6, quest_6);
	#endif

	#ifdef EXPANSIONMODGROUPS
		//! Quest #7 - Example template for a target group quest
		ExpansionQuestConfig quest_7 = m_DefaultQuestConfigData.ExpansionQuestConfig007();
		quest_7.Save("Quest_7");
		m_QuestConfigs.Insert(7, quest_7);
	#endif

		//! Quest #8 - Achievement
		ExpansionQuestConfig quest_8 = m_DefaultQuestConfigData.ExpansionQuestConfig008();
		quest_8.Save("Quest_8");
		m_QuestConfigs.Insert(8, quest_8);

		//! Quest #9 - Achievement
		ExpansionQuestConfig quest_9 = m_DefaultQuestConfigData.ExpansionQuestConfig009();
		quest_9.Save("Quest_9");
		m_QuestConfigs.Insert(9, quest_9);

	#ifdef EXPANSIONMODAI
		//! Quest #10 - Example template for a ai vip quest
		ExpansionQuestConfig quest_10 = m_DefaultQuestConfigData.ExpansionQuestConfig010();
		quest_10.Save("Quest_10");
		m_QuestConfigs.Insert(10, quest_10);
	#endif

		//! Quest #11 - Example template for a action quest
		ExpansionQuestConfig quest_11 = m_DefaultQuestConfigData.ExpansionQuestConfig011();
		quest_11.Save("Quest_11");
		m_QuestConfigs.Insert(11, quest_11);

		//! Quest #12 - Example template for a collection quest
		ExpansionQuestConfig quest_12 = m_DefaultQuestConfigData.ExpansionQuestConfig012();
		quest_12.Save("Quest_12");
		m_QuestConfigs.Insert(12, quest_12);

		//! Quest #13 - Example template for a collection quest
		ExpansionQuestConfig quest_13 = m_DefaultQuestConfigData.ExpansionQuestConfig013();
		quest_13.Save("Quest_13");
		m_QuestConfigs.Insert(13, quest_13);

		//! Quest #14 - Example template for a auto-start quest
		ExpansionQuestConfig quest_14 = m_DefaultQuestConfigData.ExpansionQuestConfig014();
		quest_14.Save("Quest_14");
		m_QuestConfigs.Insert(14, quest_14);

		//! Quest #15 - Example template for a action quest
		ExpansionQuestConfig quest_15 = m_DefaultQuestConfigData.ExpansionQuestConfig015();
		quest_15.Save("Quest_15");
		m_QuestConfigs.Insert(15, quest_15);

		//! Quest #16 - Example template for a quest that unlocks other quests
		ExpansionQuestConfig quest_16 = m_DefaultQuestConfigData.ExpansionQuestConfig016();
		quest_16.Save("Quest_16");
		m_QuestConfigs.Insert(16, quest_16);

		//! Quest #17 - Example template for a unlocked weekly quest.
		ExpansionQuestConfig quest_17 = m_DefaultQuestConfigData.ExpansionQuestConfig017();
		quest_17.Save("Quest_17");
		m_QuestConfigs.Insert(17, quest_17);

		//! Quest #18 - Example template for a unlocked weekly quest.
		ExpansionQuestConfig quest_18 = m_DefaultQuestConfigData.ExpansionQuestConfig018();
		quest_18.Save("Quest_18");
		m_QuestConfigs.Insert(18, quest_18);

		//! Quest #19 - Expample template for a unlocked weekly quest that gets unlocked when all the pre-quests are completed.
		ExpansionQuestConfig quest_19 = m_DefaultQuestConfigData.ExpansionQuestConfig019();
		quest_19.Save("Quest_19");
		m_QuestConfigs.Insert(19, quest_19);

	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline(false).IsLoaded() && GetExpansionSettings().GetHardline().UseReputation)
		{
			//! Quest #20 - Expample template for a repeatable quest that has a reputation reward.
			ExpansionQuestConfig quest_20 = m_DefaultQuestConfigData.ExpansionQuestConfig020();
			quest_20.Save("Quest_20");
			m_QuestConfigs.Insert(20, quest_20);

			//! Quest #21 - Expample template for a repeatable quest that has a reputation requirement.
			ExpansionQuestConfig quest_21 = m_DefaultQuestConfigData.ExpansionQuestConfig021();
			quest_21.Save("Quest_21");
			m_QuestConfigs.Insert(21, quest_21);
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule SpawnQuestNPCs
	// Server
	// -----------------------------------------------------------
	protected void SpawnQuestNPCs()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SpawnQuestNPCs - Tryed to call SpawnQuestNPCs on Client!");
			return;
		}

		if (!m_QuestsNPCs)
		{
			Error(ToString() + "::SpawnQuestNPCs - NPC config array is NULL!");
			return;
		}

		foreach (int id, ExpansionQuestNPCData questNPCData: m_QuestsNPCs)
		{
			if (!questNPCData.IsAI() && !questNPCData.IsStatic())
			{
				ExpansionQuestNPCBase npc = questNPCData.SpawnNPC();
				if (npc)
				{
					npc.SetQuestNPCID(questNPCData.GetID());
					npc.SetQuestNPCData(questNPCData);
					m_QuestNPCEntities.Insert(questNPCData.GetID(), npc);

					npc.SetPosition(questNPCData.GetPosition());
					npc.SetOrientation(questNPCData.GetOrientation());
				}
			}
			else if (!questNPCData.IsAI() && questNPCData.IsStatic())
			{
				ExpansionQuestStaticObject object = questNPCData.SpawnObject();
				if (object)
				{
					object.SetQuestNPCID(questNPCData.GetID());
					object.SetQuestNPCData(questNPCData);
					m_QuestObjectEntities.Insert(questNPCData.GetID(), object);

					object.SetPosition(questNPCData.GetPosition());
					object.SetOrientation(questNPCData.GetOrientation());
				}
			}
			#ifdef EXPANSIONMODAI
			else if (questNPCData.IsAI() && !questNPCData.IsStatic())
			{
				ExpansionQuestNPCAIBase npcAI = questNPCData.SpawnNPCAI();
				if (npcAI)
				{
					npcAI.SetQuestNPCID(questNPCData.GetID());
					npcAI.SetQuestNPCData(questNPCData);
					m_QuestNPCAIEntities.Insert(questNPCData.GetID(), npcAI);

					npcAI.SetPosition(questNPCData.GetPosition());
					npcAI.SetOrientation(questNPCData.GetOrientation());
					npcAI.Expansion_SetEmote(questNPCData.GetEmoteID(), !questNPCData.IsEmoteStatic());

					eAIGroup ownerGrp = npcAI.GetGroup();

					array<vector> waypoints = questNPCData.GetWaypoints();
					foreach (vector point: waypoints)
					{
						ownerGrp.AddWaypoint(point);
					}

					ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
				}
			}
			#endif
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadQuestNPCData
	// Server
	// -----------------------------------------------------------
	protected void LoadQuestNPCData(array<string> questNPCFiles)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		foreach (string file: questNPCFiles)
		{
			GetQuestNPCData(file);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void LoadObjectivesData(array<string> objectiveFiles, string path)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		foreach (string file: objectiveFiles)
		{
			GetObjectiveData(file, path);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetObjectiveData
	// Server
	// -----------------------------------------------------------
	protected void GetObjectiveData(string fileName, string path)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetObjectiveData - Tryed to call GetObjectiveData on Client!");
			return;
		}

		ExpansionQuestObjectiveConfig objectiveBaseData = new ExpansionQuestObjectiveConfig();
		if (!ExpansionJsonFileParser<ExpansionQuestObjectiveConfig>.Load(path + fileName, objectiveBaseData))
			return;

		int objectiveID = objectiveBaseData.GetID();
		int objectiveType = objectiveBaseData.GetObjectiveType();
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig = ExpansionQuestObjectiveTravelConfig.Load(path + fileName);
				if (!travelConfig)
					return;

				m_TravelObjectivesConfigs.Insert(objectiveID, travelConfig);
				travelConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = ExpansionQuestObjectiveDeliveryConfig.Load(path + fileName);
				if (!deliveryConfig)
					return;

				m_DeliveryObjectivesConfigs.Insert(objectiveID, deliveryConfig);
				deliveryConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = ExpansionQuestObjectiveTargetConfig.Load(path + fileName);
				if (!targetConfig)
					return;

				m_TargetObjectivesConfigs.Insert(objectiveID, targetConfig);
				targetConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = ExpansionQuestObjectiveCollectionConfig.Load(path + fileName);
				if (!collectionConfig)
					return;

				m_CollectionObjectivesConfigs.Insert(objectiveID, collectionConfig);
				collectionConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureConfig = ExpansionQuestObjectiveTreasureHuntConfig.Load(path + fileName);
				if (!treasureConfig)
					return;

				m_TreasureHuntObjectivesConfigs.Insert(objectiveID, treasureConfig);
				treasureConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionConfig = ExpansionQuestObjectiveActionConfig.Load(path + fileName);
				if (!actionConfig)
					return;

				m_ActionObjectivesConfigs.Insert(objectiveID, actionConfig);
				actionConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig = new ExpansionQuestObjectiveCraftingConfig();
				if (!ExpansionJsonFileParser<ExpansionQuestObjectiveCraftingConfig>.Load(path + fileName, craftingConfig))
					return;

				m_CraftingObjectivesConfigs.Insert(objectiveID, craftingConfig);
				craftingConfig.QuestDebug();
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = ExpansionQuestObjectiveAIPatrolConfig.Load(path + fileName);
				if (!aiPatrolConfig)
					return;

				m_AIPatrolObjectivesConfigs.Insert(objectiveID, aiPatrolConfig);
				aiPatrolConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = ExpansionQuestObjectiveAICampConfig.Load(path + fileName);
				if (!aiCampConfig)
					return;

				m_AICampObjectivesConfigs.Insert(objectiveID, aiCampConfig);
				aiCampConfig.QuestDebug();
			}
			break;

			case ExpansionQuestObjectiveType.AIVIP:
			{
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig = ExpansionQuestObjectiveAIVIPConfig.Load(path + fileName);
				if (!aiVIPConfig)
					return;

				m_AIVIPObjectivesConfigs.Insert(objectiveID, aiVIPConfig);
				aiVIPConfig.QuestDebug();
			}
			break;
		#endif
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadQuestData
	// Server
	// -----------------------------------------------------------
	protected void LoadQuestData(array<string> questFiles)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		foreach (string file: questFiles)
		{
			GetQuestData(file);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadObjectSets
	// Server
	// -----------------------------------------------------------
	protected void LoadObjectSets()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::LoadObjectSets - Start");

		foreach (ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (questConfig.GetObjectSetFileName() != string.Empty)
			{
				ExpansionQuestObjectSet questObjectSet = new ExpansionQuestObjectSet();
				questObjectSet.SetQuestID(questConfig.GetID());
				questObjectSet.SetObjectSetFileName(EXPANSION_QUESTS_OBJECTS_FOLDER, questConfig.GetObjectSetFileName());

				m_QuestObjectSets.Insert(questObjectSet);
				QuestModulePrint(ToString() + "::LoadObjectSets - Added objects set for quest ID [" + questConfig.GetID() + "]: " + questObjectSet.ToString());
			}
		}

		QuestModulePrint(ToString() + "::LoadObjectSets - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadObjectSets
	// Server
	// -----------------------------------------------------------
	void CheckAndSpawnObjectSet(int questID)
	{
		QuestModulePrint(ToString() + "::CheckAndSpawnObjectSet - Start");

		foreach (ExpansionQuestObjectSet objectSet: m_QuestObjectSets)
		{
			if (objectSet.GetQuestID() == questID && !objectSet.IsSpawned())
			{
				objectSet.Spawn();
				QuestModulePrint(ToString() + "::CheckAndSpawnObjectSet - Spawned object set: " + objectSet.ToString());
			}
		}

		QuestModulePrint(ToString() + "::CheckAndSpawnObjectSet - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadObjectSets
	// Server
	// -----------------------------------------------------------
	void CheckAndDeleteObjectSet(int questID)
	{
		QuestModulePrint(ToString() + "::CheckAndDeleteObjectSet - Start");

		foreach (ExpansionQuestObjectSet objectSet: m_QuestObjectSets)
		{
			if (objectSet.GetQuestID() == questID && objectSet.IsSpawned())
			{
				objectSet.Delete();
				QuestModulePrint(ToString() + "::CheckAndDeleteObjectSet - Deleted object set: " + objectSet.ToString());
			}
		}

		QuestModulePrint(ToString() + "::CheckAndDeleteObjectSet - Start");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCData
	// Server
	// -----------------------------------------------------------
	protected void GetQuestNPCData(string fileName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetQuestNPCData - Tryed to call GetQuestNPCData on Client!");
			return;
		}

		ExpansionQuestNPCData questNPCData = ExpansionQuestNPCData.Load(fileName);
		if (questNPCData)
		{
			QuestModulePrint(ToString() + "::GetQuestNPCData - Adding quest npc data from file " + fileName);
			m_QuestsNPCs.Insert(questNPCData.GetID(), questNPCData);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestData
	// Server
	// -----------------------------------------------------------
	protected void GetQuestData(string fileName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetQuestData - Tryed to call GetQuestData on Client!");
			return;
		}

		ExpansionQuestConfig questData = ExpansionQuestConfig.Load(fileName);
		if (questData)
		{
			QuestModulePrint(ToString() + "::GetQuestData - Try adding quest data from file: " + fileName);
			if (!questData.ValidateQuestConfiguration(fileName))
			{
				Error(ToString() + "::GetQuestData - Quest configuration validation failed! Quest was not added to the file system. Please check the quest configuration file: " + fileName);
			}

			m_QuestConfigs.Insert(questData.GetID(), questData);
			questData.QuestDebug();
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasCompletedQuest
	// 	Server & Client
	// ------------------------------------------------------------
	bool HasCompletedQuest(int questID, string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		ExpansionQuestPersistentData playerQuestData;
		if (IsMissionHost())
			playerQuestData = GetPlayerQuestDataByUID(playerUID);
		else if (IsMissionClient())
			playerQuestData = m_PlayerQuestData;

		if (playerQuestData)
		{
			int questState =  playerQuestData.GetQuestStateByQuestID(questID);
			int completionCount = playerQuestData.GetQuestCompletionCountByQuestID(questID);
			if (questState == ExpansionQuestState.COMPLETED || completionCount > 0)
			{
				QuestModulePrint(ToString() + "::HasCompletedQuest - Player with UID: " + playerUID + " has completed quest with ID " + questID + " once! Return TRUE");
				return true;
			}
		}

		QuestModulePrint(ToString() + "::HasCompletedQuest - Player with UID: " + playerUID + " has never completed quest with ID " + questID + " yet! Return FALSE");

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetNPCQuests
	// Server
	// ------------------------------------------------------------
	//! Collects all the quests of the given quest npc and compares them with the quest states data of the given player UID
	//! Will return only quests that can be accapted or can be completed by the player
	array<ref ExpansionQuestConfig> GetNPCQuests(int npcID, string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		array<ref ExpansionQuestConfig> npcQuests = new array<ref ExpansionQuestConfig>;
		ExpansionQuestNPCData questNPCData = GetQuestNPCDataByID(npcID);
		if (questNPCData)
		{
			array<int> questIDs = questNPCData.GetQuestIDs();
			foreach (int questID: questIDs)
			{
				ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
				if (questConfig && !questConfig.IsAchivement() && HasQuestAtNPC(npcID, playerUID, questID))
					npcQuests.Insert(questConfig);
			}
		}

		return npcQuests;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetPlayerQuests
	// Server
	// ------------------------------------------------------------
	//! Collects all the quests of the given player that should get displayed in the questlog
	//! Will return only quests that are accapted or can be completed by the player
	array<ref ExpansionQuestConfig> GetPlayerQuests(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		array<ref ExpansionQuestConfig> playerQuests = new array<ref ExpansionQuestConfig>;
		ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
		if (!playerQuestData)
			return NULL;

		foreach (int questID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (questConfig && !questConfig.IsAchivement())
			{
				//! We only want to add quests here that the player should see in the quest log.
				bool completed = HasCompletedQuest(questID, playerUID);
				bool completedPreQuest = true;

				if (questConfig.GetPreQuestIDs().Count() > 0)
				{
					array<int> preQuestIDs = questConfig.GetPreQuestIDs();
					foreach (int preQuest: preQuestIDs)
					{
						if (!HasCompletedQuest(preQuest, playerUID))
						{
							completedPreQuest = false;
							break;
						}
					}
				}

				if (completedPreQuest && (!completed || questConfig.IsRepeatable()))
					playerQuests.Insert(questConfig);
			}
		}

		return playerQuests;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasQuestAtNPC
	// Server
	// ------------------------------------------------------------
	//! Will return true if player has any quests that can be accapted or can be completed at the given NPC
	//! Can also be used to check a single quest when a quest id is given as the 3th param.
	bool HasQuestAtNPC(int npcID, string playerUID, int questIDToCheck = -1)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		ExpansionQuestNPCData questNPCData = GetQuestNPCDataByID(npcID);
		ExpansionQuestPersistentData playerQuestData;
		if (IsMissionHost()) playerQuestData = GetPlayerQuestDataByUID(playerUID);
		else if (IsMissionClient()) playerQuestData = m_PlayerQuestData;

		bool hasQuest = false;
		if (questNPCData && playerQuestData)
		{
			int questState;
			ExpansionQuestConfig questConfig;

			if (questIDToCheck == -1)
			{
				array<int> questIDs = questNPCData.GetQuestIDs();
				foreach (int questID: questIDs)
				{
					QuestModulePrint(ToString() + "::HasQuestAtNPC - Checking quest with ID: " + questID);
					questState =  playerQuestData.GetQuestStateByQuestID(questID);
					questConfig = GetQuestConfigByID(questID);
					if (!questConfig)
					{
						Error(ToString() + "::HasQuestAtNPC - Could not get quest config data for quest ID " + questID + "!");
						hasQuest = false;
					}

					//! We look for all the quests that have no pre-quest ID or when the player has completed the pre quest.
					if (QuestConditionsCheck(questConfig, playerQuestData, playerUID, questState, npcID)) //! Check quest conditions of the curret quest
					{
						QuestModulePrint(ToString() + "::HasQuestAtNPC - TRUE " + " | Quest ID: " + questID);
						hasQuest = true;
					}
					else
					{
						QuestModulePrint(ToString() + "::HasQuestAtNPC - FALSE " + " | Quest ID: " + questID);
						hasQuest = false;
					}
				}
			}
			else
			{
				QuestModulePrint(ToString() + "::HasQuestAtNPC - Checking quest with ID: " + questIDToCheck);
				questState =  playerQuestData.GetQuestStateByQuestID(questIDToCheck);
				questConfig = GetQuestConfigByID(questIDToCheck);
				if (!questConfig)
				{
					Error(ToString() + "::HasQuestAtNPC - Could not get quest config data for quest ID " + questIDToCheck + "!");
					hasQuest = false;
				}

				//! We look for all the quests that have no pre-quest ID or when the player has completed the pre quest.
				if (QuestConditionsCheck(questConfig, playerQuestData, playerUID, questState, npcID)) //! Check quest conditions of the curret quest
				{
					QuestModulePrint(ToString() + "::HasQuestAtNPC - TRUE " + " | Quest ID: " + questIDToCheck);
					hasQuest = true;
				}
				else
				{
					QuestModulePrint(ToString() + "::HasQuestAtNPC - FALSE " + " | Quest ID: " + questIDToCheck);
					hasQuest = false;
				}
			}
		}

		return hasQuest;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestConditionsCheck
	// Server & Client
	// ------------------------------------------------------------
	protected bool QuestConditionsCheck(ExpansionQuestConfig questConfig, ExpansionQuestPersistentData playerQuestData, string playerUID,  int questState, int npcID)
	{
		bool completedPreQuest = true;
		if (questConfig.GetPreQuestIDs().Count() > 0)
		{
			array<int> preQuestIDs = questConfig.GetPreQuestIDs();
			foreach (int preQuest: preQuestIDs)
			{
				if (!HasCompletedQuest(preQuest, playerUID))
				{
					completedPreQuest = false;
					break;
				}
			}
		}

		if (!completedPreQuest)
		{
			QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest has pre quest! Player [UID: " + playerUID + "] has not completed pre-quest for quest " + questConfig.GetID());
			return false;
		}

		if (!QuestStateCheck(questConfig, questState, npcID))
		{
			QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest state invalid!");
			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().UseReputation && questConfig.GetReputationRequirement() > 0)
		{
			ExpansionHardlineModule hardlineModule;
			if (Class.CastTo(hardlineModule, CF_ModuleCoreManager.Get(ExpansionHardlineModule)))
			{
				ExpansionHardlinePlayerData hardlinePlayerData;
				if (IsMissionHost()) hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(playerUID);
				else if (IsMissionClient()) hardlinePlayerData = hardlineModule.GetHardlineClientData();

				if (hardlinePlayerData)
				{
					int reputation = hardlinePlayerData.GetReputation();
					QuestModulePrint(ToString() + "::QuestConditionsCheck - Reputation for Player [UID: " + playerUID + "]: " + reputation);
					if (reputation < questConfig.GetReputationRequirement())
					{
						QuestModulePrint(ToString() + "::QuestConditionsCheck - Player has not enough reputation to start this quest! Player [UID: " + playerUID + "] is not a bandit!");
						return false;
					}
				}
			}
		}
	#endif

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestStateCheck
	// Server & Client
	// ------------------------------------------------------------
	protected bool QuestStateCheck(ExpansionQuestConfig questConfig, int questState, int npcID)
	{
		if (questState == ExpansionQuestState.NONE && questConfig.GetQuestGiverIDs().Find(npcID) > -1)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: NONE - Npc " + npcID + " is quest giver npc for quest with ID " + questConfig.GetID() + " - Quest giver IDs: " + questConfig.GetQuestGiverIDs());
			return true;
		}
		else if (questState == ExpansionQuestState.STARTED && questConfig.GetQuestGiverIDs().Find(npcID) > -1)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: STARTED - Npc is quest giver npc for quest with ID " + questConfig.GetID() + ".");
			return true;
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN && questConfig.GetQuestTurnInIDs().Find(npcID) > -1)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: CAN_TURNIN - Npc is quest turn in npc for quest with ID " + questConfig.GetID() + ".");
			return true;
		}
		else if (questState == ExpansionQuestState.COMPLETED && questConfig.IsRepeatable() && questConfig.GetQuestGiverIDs().Find(npcID) > -1)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: COMPLETED - Quest is completed!");
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestCooldownCheck
	// Server & Client
	// ------------------------------------------------------------
	protected bool QuestCooldownCheck(ExpansionQuestConfig questConfig, ExpansionQuestPersistentData playerQuestData)
	{
		if (!questConfig.IsDailyQuest() && !questConfig.IsWeeklyQuest())
			return false;

		int daylie = 86400; //! Day in seconds
		int weekly = 604800; //! Week in seconds
		int remaining;

		int time = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		if (questConfig.IsDailyQuest())
		{
			return playerQuestData.HasCooldown(questConfig.GetID(), daylie, remaining);
		}
		else if (questConfig.IsWeeklyQuest())
		{
			return playerQuestData.HasCooldown(questConfig.GetID(), weekly, remaining);
		}

		return false;
	}


	// ------------------------------------------------------------
	// ExpansionQuestModule OpenQuestMenu
	// Client
	// ------------------------------------------------------------
	//! Opens quest menu
	void OpenQuestMenu()
	{
		if (!GetGame().IsClient())
		{
			Error(ToString() + "::OpenQuestMenu - Tryed to call OpenQuestMenu on Server!");
			return;
		}

		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionQuestMenu");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestTurnInQuestClient
	// Client
	// ------------------------------------------------------------
	//! Gets called when a player truns in a certain quest
	void RequestTurnInQuestClient(int questID, bool selectReward = false, ExpansionQuestRewardConfig selectedReward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RequestTurnInQuestClient - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestTurnInQuestClient - Tryed to call RequestTurnInQuestClient on Server!");
			return;
		}

		if (!GetClientQuestData())
		{
			Error(ToString() + "::RequestTurnInQuestClient - Could not get players client quest data!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Write(selectReward);
		if (selectReward)
			selectedReward.OnSend(rpc);

		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestCompleteQuest, true);

		QuestModulePrint(ToString() + "::RequestTurnInQuestClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestCompleteQuest
	// Called on Server
	// ------------------------------------------------------------
	protected void RPC_RequestCompleteQuest(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Tryed to call RPC_RequestCompleteQuest on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Could not get player identity!");
			return;
		}

		string playerUID = identity.GetId();

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Read questID");
			return;
		}

		bool rewardSelected;
		if (!ctx.Read(rewardSelected))
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Read rewardSelected");
			return;
		}

		ExpansionQuestRewardConfig reward;
		if (rewardSelected)
		{
			reward = new ExpansionQuestRewardConfig();
			if (!reward.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_RequestCompleteQuest - Read reward");
				return;
			}
		}

		if (rewardSelected)
		{
			if (!reward)
				return;

			RequestCompleteQuestServer(questID, playerUID, identity, false, reward);
		}
		else
		{
			RequestCompleteQuestServer(questID, playerUID, identity);
		}

		QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestCompleteQuestServer
	// Called on Server
	// ------------------------------------------------------------
	void RequestCompleteQuestServer(int questID, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			bool isPlayerQuest;
			//! Get quest from active quest instances
			if (quest.GetQuestConfig().GetID() == questID)
			{
				if (!quest.IsGroupQuest() && quest.GetPlayerUID() == playerUID)
				{
					isPlayerQuest = true;
				}
			#ifdef EXPANSIONMODGROUPS
				else if (quest.IsGroupQuest())
				{
					ExpansionPartyData group = quest.GetGroup();
					if (!group || !group.IsMember(playerUID))
						continue;

					auto settings = GetExpansionSettings().GetQuest();
					if (settings.GroupQuestMode <= 1 && group.GetOwnerUID() == playerUID || group.IsMember(playerUID))
					{
						isPlayerQuest = true;
					}
					else
					{
						ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
						return;
					}
				}
			#endif


				if (isPlayerQuest && quest.CanCompleteQuest())
					CompleteQuest(quest, playerUID, identity, isAutoComplete, reward);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Server
	// ------------------------------------------------------------
	protected void CompleteQuest(ExpansionQuest quest, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CompleteQuest - Start");

		if (!quest)
		{
			Error(ToString() + "::CompleteQuest - Could not get active quest!");
			return;
		}

		if (quest.GetQuestConfig().IsDailyQuest() || quest.GetQuestConfig().IsWeeklyQuest())
		{
			int time = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
			QuestModulePrint(ToString() + "::CompleteQuest - Current timestamp: " + ExpansionStatic.FormatTimestamp(time, false));
			QuestModulePrint(ToString() + "::CompleteQuest - Current time: " + ExpansionStatic.FormatTime(time, false));
			int year, month, day, hour, minute, second;
			CF_Date.TimestampToDate(time, year, month, day, hour, minute, second);
			QuestModulePrint(ToString() + "::CompleteQuest - Formated time: " + day + "." + month + "." + year + " | " + hour + ":" + minute + ":" + second);

			int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyQuestResetHour;
			int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyQuestResteMinute;
			int dailyResetHour = GetExpansionSettings().GetQuest().DailyQuestResetHour;
			int dailyResetMinute = GetExpansionSettings().GetQuest().DailyQuestResetMinute;

			if (quest.GetQuestConfig().IsDailyQuest())
			{
				hour = dailyResetHour;
				minute = dailyResetMinute;
			}
			else if (quest.GetQuestConfig().IsWeeklyQuest())
			{
				hour = weeklyResetHour;
				minute = weeklyResetMinute;
			}

			time = CF_Date.CreateDateTime(year, month, day, hour, minute, second).GetTimestamp();

			UpdateQuestTimestampForQuestPlayers(quest, time);
		}

		if (!quest.OnQuestTurnIn(playerUID, reward))
		{
			QuestModulePrint(ToString() + "::CompleteQuest - Quest turn-in event failed for quest with quest id: " + quest.GetQuestConfig().GetID());
			CancelQuestServer(quest.GetQuestConfig().GetID(), identity);
			return;
		}

	#ifdef EXPANSIONMODNAVIGATION
		RemoveClientMarkers(quest.GetQuestConfig().GetID(), identity);
	#endif

		if (!isAutoComplete)
		{
		#ifdef EXPANSIONMODAI
			ExpansionQuestNPCAIBase npc = GetClosestQuestAINPC(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
			if (npc)
			{
				EmoteManager npcEmoteManager = npc.GetEmoteManager();
				if (!npcEmoteManager.IsEmotePlaying())
				{
					npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_CLAP);
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
				}
			}
		#endif
		}
		else
		{
			//! Add the following quest regardless when the pre-quest was a auto-complete quest
			int followUpQuest = quest.GetQuestConfig().FollowUpQuest;
			if (followUpQuest > -1)
			{
				ExpansionQuestConfig followUpQuestConfig = GetQuestConfigByID(followUpQuest);
				if (followUpQuestConfig)
				{
					CreateQuestInstanceServer(followUpQuest, playerUID);
				}
			}
		}

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CompleteQuest - Player with UID " + identity.GetId() + " has completed quest " + quest.GetQuestConfig().GetID());
		QuestModulePrint(ToString() + "::CompleteQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule PlayerQuestInit
	// Server
	// ------------------------------------------------------------
	//! Handles reinitialisation of quests for a player from persistent data
	//! We need to get and handle the persistent quest progress and objective data of the player
	//! so the progress of the quest objectives contiues and no quest progress is lost.
	protected void PlayerQuestsInit(ExpansionQuestPersistentData playerData, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::PlayerQuestsInit - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::PlayerQuestsInit - Tryed to call PlayerQuestsInit on Client!");
			return;
		}

		if (!playerData)
		{
			Error(ToString() + "::PlayerQuestsInit - Could not get player quest data!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::PlayerQuestsInit - Could not get player identity!");
			return;
		}

		string playerUID = identity.GetId();
		QuestModulePrint(ToString() + "::PlayerQuestsInit - Player UID: " + playerUID);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! Check for all active group quests the player can participate if he is member of a group
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	#ifdef EXPANSIONMODGROUPS
		array<int> activeGroupQuestIDs = new array<int>;
		ExpansionPartyModule partyModule;
		if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			Error(ToString() + "::PlayerQuestsInit - Could not get party module!");
			return;
		}

		bool hasGroup;
		ExpansionPartyPlayerData playerGroupData = partyModule.GetPartyPlayerData(playerUID);
		ExpansionPartyData group;

		if (!playerGroupData)
		{
			QuestModulePrint(ToString() + "::PlayerQuestsInit - Could not get players group player data for UID: " + playerUID + ". Player is not in a group?!");
		}
		else
		{
			QuestModulePrint(ToString() + "::PlayerQuestsInit - Found players group player data. UID: " + playerUID);
			group = playerGroupData.GetParty();
			if (!group)
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Could not get players group data for UID: " + playerUID + ". Players group does not exist?!");
			}
			else
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Found players group data for UID: " + playerUID + ". Player is a group!");
				hasGroup = true;
			}
		}

		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (!activeQuest || !activeQuest.IsInitialized())
			{
				Error(ToString() + "::PlayerQuestsInit - Could not get quest from active quests array!");
				continue;
			}

			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//! Check if player is group member of current checked active quest
			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if (activeQuest.IsGroupQuest() && activeQuest.IsQuestGroupMember(playerUID))
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - There is already a active quest instance for this quest. Skip this quest for quest init!");
				activeGroupQuestIDs.Insert(activeQuest.GetQuestConfig().GetID());
				if (!playerData.HasDataForQuest(activeQuest.GetQuestConfig().GetID()))
				{
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Add quest to players quest data!");
					ExpansionQuestConfig activeQuestConfig = GetQuestConfigByID(activeQuest.GetQuestConfig().GetID());
					playerData.AddQuestData(activeQuestConfig);
					playerData.UpdateQuestState(activeQuestConfig.GetID(), activeQuest.GetQuestState());
					UpdatePlayerQuestObjectiveData(activeQuest, playerUID);
				}
			}
		}
	#endif

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! New quest creation
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! Recreate old quests from players quest data
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		array<ref ExpansionQuestPersistentQuestData> playerQuestData = playerData.GetQuestDatas();
		array<int> questDatasToCleanUp = new array<int>;
		foreach (ExpansionQuestPersistentQuestData data: playerQuestData)
		{
			int questID = data.QuestID;
			int questState = data.State;

			QuestModulePrint(ToString() + "::PlayerQuestsInit - Checking quest with id: " + questID + " | State: " + questState);

			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//! Check if we want to create a already active group quest
			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		#ifdef EXPANSIONMODGROUPS
			int findIndex = -1;
			findIndex = activeGroupQuestIDs.Find(questID);
			if (findIndex > -1)
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Quest is in skip list. Skip quest creation!");
				continue;
			}
		#endif

			if (HasActiveQuestWithID(playerUID, questID))
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Player has already a active quest with this quest ID: " + questID + " | Player UID: " + playerUID);
				continue;
			}

			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Create new quest instance for quest: " + questID + " - Creator UID: " + playerUID);
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - PlayerQuestsInit - Create new quest instance for quest: " + questID + " - Creator UID: " + playerUID);
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Get quest config. for the quest
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
				if (!configInstance)
				{
					Error(ToString() + "::PlayerQuestsInit - Could not get any quest configuration for quest ID: " + questID);
					continue;
				}

				ExpansionQuest newQuestInstance;
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Create quest instance and set current quest status from persistent player quest data
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if (configInstance.GetType() == ExpansionQuestType.SCRIPTED && configInstance.GetQuestClassName() != string.Empty)
				{
					newQuestInstance = CreateQuestInstance(configInstance.GetQuestClassName());
				}
				else
				{
					newQuestInstance = new ExpansionQuest(this);
				}

				newQuestInstance.SetPlayerUID(playerUID); //! Set the quest player!

			#ifdef EXPANSIONMODGROUPS
				if (configInstance.IsGroupQuest() && hasGroup)
				{
					int partyID = group.GetPartyID();
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Player is member of group with ID: " + partyID + " | Set group for quest " + questID);
					newQuestInstance.SetGroup(partyID); //! Set the quest group!
				}
				else if (configInstance.IsGroupQuest() && !hasGroup)
				{
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Can't create instance for group quest with ID: " + questID + ". Player [UID: " + playerUID + "] has no group.");
					questDatasToCleanUp.Insert(questID);
					newQuestInstance = NULL;
					continue;
				}
			#endif

				newQuestInstance.SetQuestConfig(configInstance);
				newQuestInstance.SetQuestState(questState);

				//! Get quest objectives progress from persistent player quest data
				GetObjectiveProgressFromQuestData(playerData, newQuestInstance);

				m_ActiveQuests.Insert(newQuestInstance);

				if (!newQuestInstance.OnQuestContinue())
				{
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Quest continue event failed for quest with quest id: " + questID);
					CancelQuestServer(newQuestInstance.GetQuestConfig().GetID(), identity);
					continue;
				}
			}
		}

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! Create archivement and auto-start quests
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		foreach (int configQuestID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (HasActiveQuestWithID(playerUID, configQuestID))
				continue;

			if (questConfig.IsGroupQuest() || questConfig.GetQuestGiverIDs().Count() > 0 && questConfig.GetQuestGiverIDs()[0] != -1 || questConfig.GetPreQuestIDs().Count() > 0)
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Quest with ID: " + configQuestID +" is not a a valid achivement or auto start quest. Skip!");
				continue;
			}

			int playerQuestState = playerData.GetQuestStateByQuestID(configQuestID);
			ExpansionQuest autoQuest;

			//! If player has no quest state for this achievement quest we create the quest and update the players persistent quest data
			if (playerQuestState == ExpansionQuestState.NONE)
			{
				if (playerData.HasDataForQuest(questConfig.GetID()))
					continue;

				playerData.AddQuestData(questConfig);

				//! Create quest
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Create new achievement/auto-start quest for quest ID: " + configQuestID);
				autoQuest = new ExpansionQuest(this);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.SetQuestConfig(questConfig);

				m_ActiveQuests.Insert(autoQuest);

				if (!autoQuest.OnQuestStart())
				{
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Quest start event failed for quest with quest id: " + questID);
					CancelQuestServer(autoQuest.GetQuestConfig().GetID(), identity);
					continue;
				}

				playerData.UpdateQuestState(questConfig.GetID(), ExpansionQuestState.STARTED);
				UpdatePlayerQuestObjectiveData(autoQuest, playerUID);
			}
			//! If the player is already on this archiement/auto-start quest recreate it and update the progress from the players persistent quest data
			else if (playerQuestState == ExpansionQuestState.STARTED || playerQuestState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint(ToString() + "::PlayerQuestsInit - Create achievement/auto-start quest for quest ID: " + configQuestID + " and add progress from player quest data [UID: " + playerUID + "]");

				//! Create quest
				autoQuest = new ExpansionQuest(this);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.SetQuestConfig(questConfig);

				//! Get quest objectives progress from persistent player quest data
				GetObjectiveProgressFromQuestData(playerData, autoQuest);

				m_ActiveQuests.Insert(autoQuest);

				if (!autoQuest.OnQuestContinue())
				{
					Error(ToString() + "::PlayerQuestsInit - Quest continue event failed for quest quest quest id: " + questConfig.GetID());
					CancelQuestServer(autoQuest.GetQuestConfig().GetID(), identity);
					continue;
				}
			}
		}

		QuestItemsCheckAndCleanup(playerUID);

		if (questDatasToCleanUp.Count() > 0)
		{
			foreach (int questDataToCleanUp: questDatasToCleanUp)
			{
				playerData.RemoveQuestDataByQuestID(questDataToCleanUp);
			}

			playerData.Save(playerUID);
		}

		QuestModulePrint(ToString() + "::PlayerQuestsInit - End");
	}

	//! Used to cleanup quest items in the players inventory for all items where he has no active quest for
	protected void QuestItemsCheckAndCleanup(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;

		array<EntityAI> items = new array<EntityAI>;
	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item : items)
		{
			ItemBase itemIB;
			if (Class.CastTo(itemIB, item))
			{
				if (itemIB.GetQuestID() > -1 && !HasActiveQuestWithID(playerUID, itemIB.GetQuestID()))
					GetGame().ObjectDelete(item);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasActiveQuestWithID
	// Called on server
	// ------------------------------------------------------------
	bool HasActiveQuestWithID(string playerUID, int questID)
	{
		foreach (ExpansionQuest activeQuestInstance: m_ActiveQuests)
		{
			if (activeQuestInstance.GetQuestConfig().GetID() == questID && activeQuestInstance.GetPlayerUID() == playerUID)
				return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetObjectiveProgressFromQuestData
	// Server
	// ------------------------------------------------------------
	//! Handles reinitialisation of quests objectives for a player from his persistent quest data
	//! We need to get and handle the persistent objective data of the player so the progress of the quest objectives continues and no quest progress is lost.
	protected void GetObjectiveProgressFromQuestData(ExpansionQuestPersistentData playerData, ExpansionQuest quest)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Start");

		if (!playerData)
		{
			Error(ToString() + "::GetObjectiveProgressFromQuestData - Could not get player quest data!");
			return;
		}

		//! If the quest is a group quest we check the persistent data of all group members if they have one
		//! and get the latest updated progress
	#ifdef EXPANSIONMODGROUPS
		if (quest.IsGroupQuest())
		{
			ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
			if (!partyModule)
				return;

			int latestQuestUpdateTime = -1;
			ExpansionQuestPersistentData latestQuestData;
			ExpansionPartyData groupData = partyModule.GetPartyByID(quest.GetGroupID());

			array<ref ExpansionPartyPlayerData> groupPlayers = groupData.GetPlayers();
			foreach (ExpansionPartyPlayerData groupPlayerData: groupPlayers)
			{
				ExpansionQuestPersistentData groupPlayerQuestData = GetPlayerQuestDataByUID(groupPlayerData.GetID());
				if (!groupPlayerQuestData)
					continue;

				ExpansionQuestPersistentQuestData questData = groupPlayerQuestData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
				if (!questData)
					continue;

				if (latestQuestUpdateTime == -1)
				{
					latestQuestUpdateTime = questData.LastUpdateTime;
					latestQuestData = groupPlayerQuestData;
				}
				else if (questData.LastUpdateTime < latestQuestUpdateTime)
				{
					latestQuestUpdateTime = questData.LastUpdateTime;
					latestQuestData = groupPlayerQuestData;
				}
			}

			playerData = latestQuestData;
		}
	#endif

		if (!quest.ObjectivesCreated())
		{
			Error(ToString() + "::GetObjectiveProgressFromQuestData - Quest objectives are not created!");
			return;
		}

		if (!quest.GetObjectives() || quest.GetObjectives().Count() == 0)
			return;

		array<ref ExpansionQuestPersistentQuestData> playerQuestData = playerData.GetQuestDatas();
		foreach (ExpansionQuestPersistentQuestData data: playerQuestData)
		{
			if (data.QuestID != quest.GetQuestConfig().GetID())
				continue;

			array<ref ExpansionQuestObjectiveData> questObjectives = data.QuestObjectives;
			foreach (ExpansionQuestObjectiveData objectiveData: questObjectives)
			{
				QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective data type: " + objectiveData.GetObjectiveType());
				QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective data index: " + objectiveData.GetObjectiveIndex());

				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Get the correct objective event base cass with the objective index
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				int currentActiveObjectiveIndex = 0;
				array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
				foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
				{
					if (objective.GetIndex() == objectiveData.GetObjectiveIndex())
					{
						objective.SetCompleted(objectiveData.IsCompleted());
						objective.SetIsActive(objectiveData.IsActive());
						objective.SetTimeLimit(objectiveData.GetTimeLimit());

						if (objectiveData.IsActive())
							currentActiveObjectiveIndex = objectiveData.GetObjectiveIndex();

						switch (objectiveData.GetObjectiveType())
						{
							case ExpansionQuestObjectiveType.ACTION:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: ACTION");
								ExpansionQuestObjectiveActionEvent action = ExpansionQuestObjectiveActionEvent.Cast(objective);
								if (action)
								{
									action.SetActionState(objectiveData.GetActionState());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: ACTION");
								}
							}
							break;

							case ExpansionQuestObjectiveType.TARGET:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: TARGET");
								ExpansionQuestObjectiveTargetEvent target = ExpansionQuestObjectiveTargetEvent.Cast(objective);
								if (target)
								{
									// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
									//! Get kill count progress from persistent data
									// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
									target.SetCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: TARGET");
								}
							}
							break;

							case ExpansionQuestObjectiveType.CRAFTING:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: CRAFTING");
								ExpansionQuestObjectiveCraftingEvent craft = ExpansionQuestObjectiveCraftingEvent.Cast(objective);
								if (craft)
								{
									craft.SetCraftingState(objectiveData.GetActionState());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: CRAFTING");
								}
							}
							break;

						#ifdef EXPANSIONMODAI
							case ExpansionQuestObjectiveType.AIPATROL:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: AIPATROL");
								ExpansionQuestObjectiveAIPatrolEvent aiPatrolHunt = ExpansionQuestObjectiveAIPatrolEvent.Cast(objective);
								if (aiPatrolHunt)
								{
									aiPatrolHunt.SetKillCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: AIPATROL");
								}
							}
							break;

							case ExpansionQuestObjectiveType.AICAMP:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: AICAMP");
								ExpansionQuestObjectiveAICampEvent aiCampHunt = ExpansionQuestObjectiveAICampEvent.Cast(objective);
								if (aiCampHunt)
								{
									aiCampHunt.SetKillCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: AICAMP");
								}
							}
							break;
						#endif
						}
					}
				}

				quest.SetCurrentObjectiveIndex(currentActiveObjectiveIndex);
			}
		}

		QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CleanupPlayerQuests
	// Server
	// ------------------------------------------------------------
	protected void CleanupPlayerQuests(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - Start");
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CleanupPlayerQuests - Tryed to call CleanupPlayerQuests on Client!");
			return;
		}

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - Look for quests of player with UID [" + playerUID + "]");
		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			QuestModulePrint(ToString() + "::CleanupPlayerQuests - Check active quest. ID: " + quest.GetQuestConfig().GetID() + " | Player UID: " + quest.GetPlayerUID());
			if (quest.GetPlayerUID() == playerUID)
			{
				QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest " + quest.GetQuestConfig().GetID() + " is a quest of player with UID [" + playerUID + "] Delete quest!");
			#ifdef EXPANSIONMODGROUPS
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Dont delete the quest if its a group quest and a other group member is still online
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if (quest.IsGroupQuest())
				{
					if (quest.IsOtherGroupMemberOnline(playerUID))
					{
						QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest is a group quest and other group players are still online! Skip quest deletion!");
						continue;
					}
				}
			#endif

				//! Delete quest
				CheckAndDeleteObjectSet(quest.GetQuestConfig().GetID());
				quest.OnQuestCleanup();
				m_ActiveQuests.RemoveOrdered(i);
				QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest deleted!");
			}
		}

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RemovePlayerQuestData
	// Server
	// ------------------------------------------------------------
	protected void RemovePlayerQuestData(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::RemovePlayerQuestData - Start");
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RemovePlayerQuestData - Tryed to call RemovePlayerQuestData on Client!");
			return;
		}

		if (m_PlayerDatas.Get(playerUID))
			m_PlayerDatas.Remove(playerUID);

		QuestModulePrint(ToString() + "::RemovePlayerQuestData - End");
	}

#ifdef EXPANSIONMODAI
	void KilledVIPCheck(EntityAI victim, EntityAI killSource, Man killerPlayer)
	{
		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
			{
				if (objective.GetObjectiveConfig().GetObjectiveType() != ExpansionQuestObjectiveType.AIVIP)
					continue;

				ExpansionQuestObjectiveAIVIPEvent aiVIPObjective;
				if (Class.CastTo(aiVIPObjective, objective))
					aiVIPObjective.OnEntityKilled(victim, killSource, killerPlayer);
			}
		}
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule OnEntityKilled
	// Server
	// ------------------------------------------------------------
	//! Plan here is to get all the active quests of the given player uid on the server
	//! to check there quest objectives for any target objective types.
	//! If we find one we check if we can add progress to that quest objective by checking
	//! the killed enity type name and compare it with the target objective class name.
	void OnEntityKilled(EntityAI victim, EntityAI killSource, Man killerPlayer = NULL)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!GetGame().IsServer())
		{
			return;
		}

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		if (!killerPlayer)
		{
			//! Kill source hierarchy root was not a player, call OnEntityKilled for all players in radius
			float radiusSq = 150 * 150;
			array<Man> players();
			GetGame().GetPlayers(players);
			foreach (Man player: players)
			{
				if (player.GetIdentity() && vector.DistanceSq(victim.GetPosition(), player.GetPosition()) < radiusSq)
				{
					OnEntityKilled(victim, killSource, player);
				}
			}
			return;
		}

		if (!killerPlayer.GetIdentity())
			return;

	#ifdef EXPANSIONMODAI
		if (victim.IsInherited(eAIBase))
			KilledVIPCheck(victim, killSource, killerPlayer);
	#endif

		string playerUID = killerPlayer.GetIdentity().GetId();

		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (!quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID))
				continue;
		#endif

			if (quest.IsCompeleted() || quest.GetQuestState() >= ExpansionQuestState.COMPLETED)
				continue;

			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
			{
				if (!objective.IsActive() || objective.IsCompleted())
					continue;

				//! Run thrue all possible objective types
				switch (objective.GetObjectiveConfig().GetObjectiveType())
				{
					case ExpansionQuestObjectiveType.TARGET:
					{
						ExpansionQuestObjectiveTargetEvent targetObjective;
						if (Class.CastTo(targetObjective, objective))
							targetObjective.OnEntityKilled(victim, killSource, killerPlayer);
					}
					break;

				#ifdef EXPANSIONMODAI
					case ExpansionQuestObjectiveType.AICAMP:
					{
						ExpansionQuestObjectiveAICampEvent aiCampObjective;
						if (Class.CastTo(aiCampObjective, objective))
							aiCampObjective.OnEntityKilled(victim, killSource, killerPlayer);
					}
					break;

					case ExpansionQuestObjectiveType.AIPATROL:
					{
						ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
						if (Class.CastTo(aiPatrolObjective, objective))
							aiPatrolObjective.OnEntityKilled(victim, killSource, killerPlayer);
					}
					break;

					case ExpansionQuestObjectiveType.AIVIP:
					{
						ExpansionQuestObjectiveAIVIPEvent aiVIPObjective;
						if (Class.CastTo(aiVIPObjective, objective))
							aiVIPObjective.OnEntityKilled(victim, killSource, killerPlayer);
					}
					break;
				#endif
				}
			}
		}

		QuestModulePrint(ToString() + "::OnEntityKilled - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnActionUsed
	// Server
	// ------------------------------------------------------------
	void OnActionUsed(ActionBase actionBase, ActionData actionData)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::OnActionUsed - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::OnActionUsed - Tryed to call OnActionUsed on Client!");
			return;
		}

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		if (!actionData || !actionBase)
			return;

		QuestModulePrint(ToString() + "::OnActionUsed - Action name: " + actionBase.ClassName());
		QuestModulePrint(ToString() + "::OnActionUsed - Action base: " + actionBase.ToString());
		QuestModulePrint(ToString() + "::OnActionUsed - Action data: " + actionData.ToString());

		PlayerBase player = actionData.m_Player;
		if (!player || !player.GetIdentity())
			return;

		string playerUID = player.GetIdentity().GetId();
		if (playerUID == string.Empty)
			return;

		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (!quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID))
				continue;
		#endif

			if (quest.IsCompeleted() || quest.GetQuestState() == ExpansionQuestState.COMPLETED)
				continue;

			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
			{
				if (!objective.IsActive() || objective.IsCompleted())
					continue;

				//! Run thrue all possible objective types
				switch (objective.GetObjectiveConfig().GetObjectiveType())
				{
					case ExpansionQuestObjectiveType.ACTION:
					{
						ExpansionQuestObjectiveActionEvent actionObjective;
						if (Class.CastTo(actionObjective, objective))
							actionObjective.OnActionUsed(actionBase, actionData);
					}
					break;
				}
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnItemsCrafted
	// Server
	// ------------------------------------------------------------
	void OnItemsCrafted(PlayerBase player, array<ItemBase> spawned_objects)
	{
		QuestModulePrint(ToString() + "::OnItemsCrafted - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::OnItemsCrafted - Tryed to call OnItemsCrafted on Client!");
			return;
		}

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		if (!player || !player.GetIdentity())
			return;

		string playerUID = player.GetIdentity().GetId();
		if (playerUID == string.Empty)
			return;

		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (!quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID))
				continue;
		#endif

			if (quest.IsCompeleted() || quest.GetQuestState() == ExpansionQuestState.COMPLETED)
				continue;

			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
			{
				if (!objective.IsActive() || objective.IsCompleted())
					continue;

				//! Run thrue all possible objective types
				switch (objective.GetObjectiveConfig().GetObjectiveType())
				{
					case ExpansionQuestObjectiveType.CRAFTING:
					{
						ExpansionQuestObjectiveCraftingEvent craftingObjective;
						if (Class.CastTo(craftingObjective, objective))
							craftingObjective.OnItemsCrafted(player, spawned_objects);
					}
					break;
				}
			}
		}
	}

	void UpdatePlayerQuestObjectiveData(ExpansionQuest quest, string playerUID)
	{
		QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Start");

		if (!quest)
		{
			Error(ToString() + "::UpdatePlayerQuestObjectiveData - Could not get quest!");
			return;
		}

		ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);

		UpdatePlayerQuestObjectiveData(quest, playerQuestData, playerUID);

		QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - End");
	}

	void UpdatePlayerQuestObjectiveData(ExpansionQuest quest, ExpansionQuestPersistentData playerQuestData, string playerUID)
	{
		if (playerQuestData) //! FFS
		{
			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase currentObjective: questObjectiveEvents)
			{
				QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Try update from objective event base: " + currentObjective.ToString());
				ExpansionQuestObjectiveData objectiveData = playerQuestData.GetQuestObjectiveByQuestIDAndIndex(quest.GetQuestConfig().GetID(), currentObjective.GetIndex());
				bool isNewObjectiveData;
				if (!objectiveData)
				{
					isNewObjectiveData = true;
					objectiveData = new ExpansionQuestObjectiveData();
					objectiveData.SetObjectiveType(currentObjective.GetObjectiveType());
					objectiveData.SetObjectiveIndex(currentObjective.GetIndex());
				}

				objectiveData.SetActive(currentObjective.IsActive());
				bool wasCompleted = objectiveData.IsCompleted();
				objectiveData.SetCompleted(currentObjective.IsCompleted());
				objectiveData.SetTimeLimit(currentObjective.GetTimeLimit());

				ExpansionQuestNPCBase npc;
				ExpansionQuestStaticObject object;
			#ifdef EXPANSIONMODAI
				ExpansionQuestNPCAIBase npcAI;
			#endif
				vector npcPos;

				int objectiveType = currentObjective.GetObjectiveConfig().GetObjectiveType();
				switch (objectiveType)
				{
					case ExpansionQuestObjectiveType.TARGET:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: TARGET");
						ExpansionQuestObjectiveTargetEvent targetObjective;
						if (Class.CastTo(targetObjective, currentObjective) && (!wasCompleted || !objectiveData.IsCompleted()))
						{
							objectiveData.SetObjectiveAmount(targetObjective.GetAmount());
							objectiveData.SetObjectiveCount(targetObjective.GetCount());
						}
					}
					break;

					case ExpansionQuestObjectiveType.TRAVEL:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: TRAVEL");
						ExpansionQuestObjectiveTravelEvent travelObjective;
						if (Class.CastTo(travelObjective, currentObjective))
						{
							objectiveData.SetObjectivePosition(travelObjective.GetPosition());
						}
					}
					break;

					case ExpansionQuestObjectiveType.COLLECT:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: COLLECT");
						ExpansionQuestObjectiveCollectionEvent collectionObjective;
						if (Class.CastTo(collectionObjective, currentObjective))
						{
							if (quest.GetQuestConfig().GetQuestTurnInIDs().Count() > 0 && !quest.GetQuestConfig().IsAutocomplete() && quest.GetPlayer())
							{
								npc = GetClosestQuestNPCByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
								object = GetClosestQuestObjectByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
							#ifdef EXPANSIONMODAI
								npcAI = GetClosestQuestNPCAIByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
							#endif

								if (npc)
								{
									npcPos = npc.GetPosition();
								}
								else if (object)
								{
									npcPos = object.GetPosition();
								}
							#ifdef EXPANSIONMODAI
								else if (npcAI)
								{
									npcPos = npcAI.GetPosition();
								}
							#endif
							}
							else
							{
								if (quest.GetPlayer())
									npcPos = quest.GetPlayer().GetPosition();
							}

							if (npcPos != vector.Zero)
								objectiveData.SetObjectivePosition(npcPos);

							array<ref ExpansionQuestDeliveryObjectiveData> collectionData = collectionObjective.GetCollectionData();
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Set collection data: " + collectionData.ToString() + " | Count: " + collectionData.Count());
							objectiveData.SetDeliveries(collectionData);
						}
					}
					break;

					case ExpansionQuestObjectiveType.DELIVERY:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: DELIVERY");
						ExpansionQuestObjectiveDeliveryEvent deliveryObjective;
						if (Class.CastTo(deliveryObjective, currentObjective))
						{
							if (quest.GetQuestConfig().GetQuestTurnInIDs().Count() > 0 && !quest.GetQuestConfig().IsAutocomplete() && quest.GetPlayer())
							{
								npc = GetClosestQuestNPCByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
								object = GetClosestQuestObjectByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
							#ifdef EXPANSIONMODAI
								npcAI = GetClosestQuestNPCAIByID(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
							#endif

								if (npc)
								{
									npcPos = npc.GetPosition();
								}
								else if (object)
								{
									npcPos = object.GetPosition();
								}
							#ifdef EXPANSIONMODAI
								else if (npcAI)
								{
									npcPos = npcAI.GetPosition();
								}
							#endif
							}
							else
							{
								if (quest.GetPlayer())
									npcPos = quest.GetPlayer().GetPosition();
							}

							if (npcPos != vector.Zero)
								objectiveData.SetObjectivePosition(npcPos);

							array<ref ExpansionQuestDeliveryObjectiveData> deliveryData = deliveryObjective.GetDeliveryData();
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Set delivery data: " + deliveryData.ToString() + " | Count: " + deliveryData.Count());
							objectiveData.SetDeliveries(deliveryData);
						}
					}
					break;

					case ExpansionQuestObjectiveType.TREASUREHUNT:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: TREASUREHUNT");
						ExpansionQuestObjectiveTreasureHuntEvent treasureObjective;
						if (Class.CastTo(treasureObjective, currentObjective))
						{
							objectiveData.SetObjectivePosition(treasureObjective.GetPosition());
						}
					}
					break;

					case ExpansionQuestObjectiveType.ACTION:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: ACTION");
						ExpansionQuestObjectiveActionEvent actionObjective;
						if (Class.CastTo(actionObjective, currentObjective))
						{
							objectiveData.SetActionState(actionObjective.GetActionState());
						}
					}
					break;

					case ExpansionQuestObjectiveType.CRAFTING:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: CRAFTING");
						ExpansionQuestObjectiveCraftingEvent craftingObjective;
						if (Class.CastTo(craftingObjective, currentObjective))
						{
							objectiveData.SetActionState(craftingObjective.GetCraftingState());
						}
					}
					break;

				#ifdef EXPANSIONMODAI
					case ExpansionQuestObjectiveType.AICAMP:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: AICAMP");
						ExpansionQuestObjectiveAICampEvent aiCampObjective;
						if (Class.CastTo(aiCampObjective, currentObjective) && (!wasCompleted || !objectiveData.IsCompleted()))
						{
							objectiveData.SetObjectiveAmount(aiCampObjective.GetAmount());
							objectiveData.SetObjectiveCount(aiCampObjective.GetCount());
						}
					}
					break;

					case ExpansionQuestObjectiveType.AIPATROL:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: AIPATROL");
						ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
						if (Class.CastTo(aiPatrolObjective, currentObjective) && (!wasCompleted || !objectiveData.IsCompleted()))
						{
							objectiveData.SetObjectiveAmount(aiPatrolObjective.GetAmount());
							objectiveData.SetObjectiveCount(aiPatrolObjective.GetCount());
						}
					}
					break;

					case ExpansionQuestObjectiveType.AIVIP:
					{
						QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: AIVIP");
						ExpansionQuestObjectiveAIVIPEvent aiVIPObjective;
						if (Class.CastTo(aiVIPObjective, currentObjective))
						{
							objectiveData.SetObjectivePosition(aiVIPObjective.GetPosition());
						}
					}
					break;
				#endif
				}

				if (isNewObjectiveData)
				{
					playerQuestData.UpdateObjective(quest.GetQuestConfig().GetID(), objectiveData.GetObjectiveIndex(), objectiveData);
				}
				else
				{
					ExpansionQuestPersistentQuestData questData = playerQuestData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
					if (questData)
						questData.UpdateLastUpdateTime();
				}

				QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Added new objective data for type: " + objectiveType);
			}

			playerQuestData.Save(playerUID);
			playerQuestData.QuestDebug();

			PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
			if (player)
				SendPlayerQuestData(player.GetIdentity());
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Sends updated quest and state data to all quest clients
	void UpdateQuestForQuestPlayers(ExpansionQuest quest, int state)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		if (!quest.IsGroupQuest())
		{
			QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + quest.GetPlayerUID() + " | State: " + state);
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				if (playerQuestData.GetQuestStateByQuestID(quest.GetQuestConfig().GetID()) != state)
				{
					if (quest.GetQuestConfig().IsRepeatable() && state == ExpansionQuestState.COMPLETED)
					{
						ExpansionQuestPersistentQuestData questData = playerQuestData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
						questData.CompletionCount++;
						QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Updated completion count for quest. Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + quest.GetPlayerUID() + " | Completion count: " + questData.CompletionCount);
					}

					playerQuestData.UpdateQuestState(quest.GetQuestConfig().GetID(), state);
				}

				UpdatePlayerQuestObjectiveData(quest, playerQuestData, quest.GetPlayerUID());
			}
		}
		else
		{
		#ifdef EXPANSIONMODGROUPS
			if (!quest.GetGroup())
			{
				Error(ToString() + "::UpdateQuestForQuestPlayers - Quest is a group quest but could not get group from quest!");
				return;
			}

			array<ref ExpansionPartyPlayerData> groupPlayers = quest.GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + playerGroupData.GetID() + " | State: " + state);
				ExpansionQuestPersistentData groupPlayerQuestData = GetPlayerQuestDataByUID(playerGroupData.GetID());
				if (groupPlayerQuestData)
				{
					if (groupPlayerQuestData.GetQuestStateByQuestID(quest.GetQuestConfig().GetID()) != state)
					{
						if (quest.GetQuestConfig().IsRepeatable() && state == ExpansionQuestState.COMPLETED)
						{
							ExpansionQuestPersistentQuestData groupPlayerPersQuestData = groupPlayerQuestData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
							groupPlayerPersQuestData.CompletionCount++;
							QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Updated completion count for quest. Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + quest.GetPlayerUID() + " | Completion count: " + groupPlayerPersQuestData.CompletionCount);
						}

						groupPlayerQuestData.UpdateQuestState(quest.GetQuestConfig().GetID(), state);
					}

					UpdatePlayerQuestObjectiveData(quest, groupPlayerQuestData, playerGroupData.GetID());
				}
			}
		#endif
		}

		QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestTimestampForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Sends updated quest time data to all quest clients
	protected void UpdateQuestTimestampForQuestPlayers(ExpansionQuest quest, int time)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - Start");

		if (!quest.IsGroupQuest())
		{
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				playerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				playerQuestData.Save(quest.GetPlayerUID());
				playerQuestData.QuestDebug();

				PlayerBase player = PlayerBase.GetPlayerByUID(quest.GetPlayerUID());
				if (player)
					SendPlayerQuestData(player.GetIdentity());
			}
		}
		else
		{
		#ifdef EXPANSIONMODGROUPS
			array<ref ExpansionPartyPlayerData> groupPlayers = quest.GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (!groupPlayer)
					continue;

				ExpansionQuestPersistentData groupsPlayerQuestData = GetPlayerQuestDataByUID(groupPlayer.GetIdentity().GetId());
				if (!groupsPlayerQuestData)
					continue;

				groupsPlayerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				groupsPlayerQuestData.Save(groupPlayer.GetIdentity().GetId());
				groupsPlayerQuestData.QuestDebug();
				SendPlayerQuestData(groupPlayer.GetIdentity());
			}
		#endif
		}

		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CheckWeeklyTimer
	// ------------------------------------------------------------
	void CheckQuestResetTime()
	{
		auto now = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime);
		int weekDay = now.GetDayOfWeek();
		int minutes = now.GetMinutes();
		int hours = now.GetHours();
		string dayName = GetDayName(weekDay);
		string resetDay = GetExpansionSettings().GetQuest().WeeklyQuestResetDay;
		int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyQuestResetHour;
		int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyQuestResteMinute;
		int dailyResetHour = GetExpansionSettings().GetQuest().DailyQuestResetHour;
		int dailyResetMinute = GetExpansionSettings().GetQuest().DailyQuestResetMinute;

		int lastWeeklyReset = m_ServerData.LastWeeklyReset;
		int lastDailyReset = m_ServerData.LastDailyReset;

		if (lastWeeklyReset == 0)
		{
			m_ServerData.SetWeeklyResetTime();
			lastWeeklyReset = m_ServerData.LastWeeklyReset;
		}

		if (lastDailyReset == 0)
		{
			m_ServerData.SetWeeklyResetTime();
			lastDailyReset = m_ServerData.LastDailyReset;
		}

		//! Get last weekly reset time
		int lastWeeklyYear, lastWeeklyMonth, lastWeeklyDay, lastWeeklyHour, lastWeeklyMinute, lastWeeklySecond;
		CF_Date.TimestampToDate(lastWeeklyReset, lastWeeklyYear, lastWeeklyMonth, lastWeeklyDay, lastWeeklyHour, lastWeeklyMinute, lastWeeklySecond);

		//! Get last daily reset time
		int lastDailyYear, lastDailyMonth, lastDailyDay, lastDailyHour, lastDailyMinute, lastDailySecond;
		CF_Date.TimestampToDate(lastDailyReset, lastDailyYear, lastDailyMonth, lastDailyDay, lastDailyHour, lastDailyMinute, lastDailySecond);

		string lastWeeklyResetText = string.Format("%1-%2-%3 - %4:%5:%6", lastWeeklyDay, lastWeeklyMonth, lastWeeklyYear, lastWeeklyHour, lastWeeklyMinute, lastWeeklySecond);
		string lastDailyResetText = string.Format("%1-%2-%3 - %4:%5:%6", lastDailyDay, lastDailyMonth, lastDailyYear, lastDailyHour, lastDailyMinute, lastDailySecond);

		QuestModulePrint(ToString() + "::CheckQuestResetTime - Todays day: " + dayName);
		QuestModulePrint(ToString() + "::CheckQuestResetTime - Reset day: " + resetDay);
		QuestModulePrint(ToString() + "::CheckQuestResetTime - Last Weekly reset was: " + lastWeeklyResetText);
		QuestModulePrint(ToString() + "::CheckQuestResetTime - Last Daily reset was: " + lastWeeklyResetText);

		if (dayName == resetDay)
		{
			QuestModulePrint(ToString() + "::CheckQuestResetTime - Today is weekly quest reset day! Reset at " + weeklyResetHour + ":" + weeklyResetMinute);
			if (!m_ServerData.HasWeeklyResetCooldown() && minutes >= weeklyResetMinute && hours >= weeklyResetHour)
			{
				QuestModulePrint(ToString() + "::CheckQuestResetTime - Execute weekly quest reset!");
				ResetWeeklyQuestCooldowns();
				m_ServerData.SetWeeklyResetTime();
				m_ServerData.Save();
			}
		}

		if (!m_ServerData.HasDailyResetCooldown() && minutes >= dailyResetMinute && hours >= dailyResetHour)
		{
			QuestModulePrint(ToString() + "::CheckQuestResetTime - Execute daily quest reset!");
			ResetDailyQuestCooldowns();
			m_ServerData.SetDailyResetTime();
			m_ServerData.Save();
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ResetWeeklyQuestCooldowns
	// ------------------------------------------------------------
	void ResetWeeklyQuestCooldowns()
	{
		foreach (string playerUID, ExpansionQuestPersistentData playerData: m_PlayerDatas)
		{
			array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
			foreach (ExpansionQuestPersistentQuestData data: questDatas)
			{
				ExpansionQuestConfig questConfig = GetQuestConfigByID(data.QuestID);
				if (!questConfig)
					continue;

				if (questConfig.IsWeeklyQuest())
				{
					//! Reset started/completed or ready to turn-in weekly quests.
					if (data.State >= ExpansionQuestState.STARTED)
						data.State == ExpansionQuestState.NONE; //! Will cleanup this quest entry when data gets saved

					data.Timestamp = 0;
					playerData.Save(playerUID);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ResetDailyQuestCooldowns
	// ------------------------------------------------------------
	void ResetDailyQuestCooldowns()
	{
		foreach (string playerUID, ExpansionQuestPersistentData playerData: m_PlayerDatas)
		{
			array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
			foreach (ExpansionQuestPersistentQuestData data: questDatas)
			{
				ExpansionQuestConfig questConfig = GetQuestConfigByID(data.QuestID);
				if (!questConfig)
					continue;

				if (questConfig.IsDailyQuest())
				{
					//! Reset started/completed or ready to turn-in weekly quests.
					if (data.State >= ExpansionQuestState.STARTED)
						data.State == ExpansionQuestState.NONE; //! Will cleanup this quest entry when data gets saved

					data.Timestamp = 0;
					playerData.Save(playerUID);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetDayName
	// ------------------------------------------------------------
	string GetDayName(int day)
	{
		string dayName;
		switch (day)
		{
			case 0:
				dayName = "Sunday";
				break;
			case 1:
				dayName = "Monday";
				break;
			case 2:
				dayName = "Tuesday";
				break;
			case 3:
				dayName = "Wednesday";
				break;
			case 4:
				dayName = "Thursday";
				break;
			case 5:
				dayName = "Friday";
				break;
			case 6:
				dayName = "Saturday";
				break;
			default:
				dayName = "NONE";
				break;
		}

		return dayName;
	}

#ifdef EXPANSIONMODGROUPS
	// ------------------------------------------------------------
	// ExpansionQuestModule AddPlayerGroupID
	// ------------------------------------------------------------
	void AddPlayerGroupID(string playerUID, int groupID)
	{
		array<int> groupIDs;
		if (m_PlayerGroups.Find(playerUID, groupIDs))
		{
			if (groupIDs.Find(groupID) == -1)
			{
				groupIDs.Insert(groupID);
			}
		}
		else
		{
			groupIDs = new array<int>;
			groupIDs.Insert(groupID);
			m_PlayerGroups.Insert(playerUID, groupIDs);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule AddPlayerGroupID
	// ------------------------------------------------------------
	bool WasPlayerInGroup(string playerUID, int groupID)
	{
		array<int> groupIDs;
		if (m_PlayerGroups.Find(playerUID, groupIDs))
		{
			if (groupIDs.Find(groupID) > -1)
				return true;
		}

		return false;
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestModuleQuestModulePrint
	// ------------------------------------------------------------
	void QuestModulePrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print(text);
	#endif
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnUpdate
	// Server & Client
	// ------------------------------------------------------------
	//! Updates every active quest on the server ang updates there quest opbjectives
	//! to update current quest states and progession.
	//! RN the thread refreshes 100 quests every ten seconds.
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);

		if (GetGame().IsClient() || !GetExpansionSettings().GetQuest().EnableQuests)
			return;

		auto update = CF_EventUpdateArgs.Cast(args);

		m_UpdateQueueTimer += update.DeltaTime;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			if (m_ActiveQuests.Count() > 0)
			{
				for (int i = 0; i < UPDATE_QUESTS_PER_TICK; ++i)
				{
					if (m_CurrentQuestTick >= m_ActiveQuests.Count())
						m_CurrentQuestTick = 0;

					ExpansionQuest quest = m_ActiveQuests.Get(m_CurrentQuestTick);
					if (quest)
					{
						if (quest.IsInitialized())
						{
							if (quest.IsCompeleted())
							{
								quest.OnQuestCleanup();
								m_ActiveQuests.Remove(m_CurrentQuestTick);
							}
							else
							{
								quest.OnUpdate(m_UpdateQueueTimer);
							}
						}

						m_CurrentQuestTick++;
						if (m_CurrentQuestTick == m_ActiveQuests.Count())
							break;
					}
				}
			}
			else
			{
				m_CurrentQuestTick = 0;
			}

			m_UpdateQueueTimer = 0.0;
		}

		m_CheckResetTimer += update.DeltaTime;
		if (m_CheckResetTimer >= CHECK_TICK_TIME)
		{
			CheckQuestResetTime();
			m_CheckResetTimer = 0.0;
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetClientQuestData
	// Client
	// ------------------------------------------------------------
	ExpansionQuestPersistentData GetClientQuestData()
	{
		return m_PlayerQuestData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestMenuSI
	// Client
	// ------------------------------------------------------------
	ScriptInvoker GetQuestMenuSI()
	{
		return m_QuestMenuInvoker;
	}

#ifdef EXPANSIONMODBOOK
	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestLogSI
	// Client
	// ------------------------------------------------------------
	ScriptInvoker GetQuestLogSI()
	{
		return m_QuestLogInvoker;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestLogCallbackSI
	// Client
	// ------------------------------------------------------------
	ScriptInvoker GetQuestLogCallbackSI()
	{
		return m_QuestLogCallbackInvoker;
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestByID
	// Server
	// -----------------------------------------------------------
	ExpansionQuestConfig GetQuestConfigByID(int id)
	{
		return m_QuestConfigs.Get(id);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestByID
	// Client
	// -----------------------------------------------------------
	ExpansionQuestConfig GetQuestConfigClientByID(int id)
	{
		return m_QuestClientConfigs.Get(id);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCDataByID
	// Server & client
	// -----------------------------------------------------------
	ExpansionQuestNPCData GetQuestNPCDataByID(int id)
	{
		ExpansionQuestNPCData foundData;
		if (m_QuestsNPCs.Find(id, foundData))
			return foundData;

		return NULL;
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetPlayerQuestDataByUID
	// Server
	// -----------------------------------------------------------
	ExpansionQuestPersistentData GetPlayerQuestDataByUID(string playerUID)
	{
		return m_PlayerDatas.Get(playerUID);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetDefaultQuestNPCData
	// Server
	// ------------------------------------------------------------
	ExpansionDefaultQuestNPCData GetDefaultQuestNPCData()
	{
		return m_DefaultQuestNPCData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetTravelObjectiveByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveTravelConfig GetTravelObjectiveConfigByID(int id)
	{
		return m_TravelObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetTargetObjectiveByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveTargetConfig GetTargetObjectiveConfigByID(int id)
	{
		return m_TargetObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetDeliveryObjectiveByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveDeliveryConfig GetDeliveryObjectiveConfigByID(int id)
	{
		return m_DeliveryObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetCollectionObjectiveByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveCollectionConfig GetCollectionObjectiveConfigByID(int id)
	{
		return m_CollectionObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetTreasureHuntObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveTreasureHuntConfig GetTreasureHuntObjectiveConfigByID(int id)
	{
		return m_TreasureHuntObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetActionObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveActionConfig GetActionObjectiveConfigByID(int id)
	{
		return m_ActionObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetCraftingObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveCraftingConfig GetCraftingObjectiveConfigByID(int id)
	{
		return m_CraftingObjectivesConfigs.Get(id);
	}

#ifdef EXPANSIONMODAI
	// ------------------------------------------------------------
	// ExpansionQuestModule GetCollectionObjectiveByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveAIPatrolConfig GetAIPatrolObjectiveConfigByID(int id)
	{
		return m_AIPatrolObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetAICampObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveAICampConfig GetAICampObjectiveConfigByID(int id)
	{
		return m_AICampObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetAIVIPObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveAIVIPConfig GetAIVIPObjectiveConfigByID(int id)
	{
		return m_AIVIPObjectivesConfigs.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestNPCAIBase GetClosestQuestNPCAIByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCAIBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCAIBase npc = m_QuestNPCAIEntities.Get(npcID);
			if (!npc)
				continue;

			npcPos = npc.GetPosition();
			distance = Math.Round(vector.Distance(playerPos, npcPos));
			if (shortestDistance == 0 || shortestDistance > distance)
			{
				shortestDistance = distance;
				closestNPC = npc;
			}
		}

		return closestNPC;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestPatrolExists
	// Server
	// ------------------------------------------------------------
	bool QuestPatrolExists(int questID, out array<eAIDynamicPatrol> patrols)
	{
		array<eAIDynamicPatrol> foundPatrols;
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			patrols = foundPatrols;
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SetQuestPatrol
	// Server
	// ------------------------------------------------------------
	void SetQuestPatrols(int questID, array<eAIDynamicPatrol> patrols)
	{
		array<eAIDynamicPatrol> foundPatrols;
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			//! Check if current patrol has been killed.
			int killedPatrolsCount;
			foreach (eAIDynamicPatrol questPatrol: foundPatrols)
			{
				if (questPatrol.WasGroupDestroyed())
					killedPatrolsCount++;
			}

			if (foundPatrols.Count() == killedPatrolsCount)
			{
				foundPatrols.Clear();
				foreach (eAIDynamicPatrol patrol: patrols)
				{
					foundPatrols.Insert(patrol);
				}
			}
		}
		else
		{
			m_GlobalAIPatrols.Insert(questID, patrols);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RemoveQuestPatrol
	// Server
	// ------------------------------------------------------------
	void RemoveQuestPatrol(int questID)
	{
		array<eAIDynamicPatrol> foundPatrols;
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			foundPatrols.Clear();
			m_GlobalAIPatrols.Remove(questID);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CanDeleteQuestPatrol
	// Server
	// ------------------------------------------------------------
	bool CanDeleteQuestPatrol(int questID)
	{
		int incompleteCount = 0;
		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (quest.GetQuestConfig().GetID() != questID)
				continue;

			array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
			foreach (ExpansionQuestObjectiveEventBase objective: questObjectiveEvents)
			{
				if (objective.GetObjectiveType() != ExpansionQuestObjectiveType.AIPATROL || objective.GetObjectiveType() != ExpansionQuestObjectiveType.AICAMP)
					continue;

				if (!objective.IsCompleted())
					incompleteCount++;
			}
		}

		if (incompleteCount > 0)
			return false;

		return true;
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule GetActiveQuests
	// Server
	// ------------------------------------------------------------
	array<ref ExpansionQuest> GetActiveQuests()
	{
		return m_ActiveQuests;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestNPCBase GetClosestQuestNPCByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCBase npc = m_QuestNPCEntities.Get(npcID);
			if (!npc)
				continue;

			npcPos = npc.GetPosition();
			distance = Math.Round(vector.Distance(playerPos, npcPos));
			if (shortestDistance == 0 || shortestDistance > distance)
			{
				shortestDistance = distance;
				closestNPC = npc;
			}
		}

		return closestNPC;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestStaticObject GetClosestQuestObjectByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestStaticObject closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestStaticObject npc = m_QuestObjectEntities.Get(npcID);
			if (!npc)
				continue;

			npcPos = npc.GetPosition();
			distance = Math.Round(vector.Distance(playerPos, npcPos));
			if (shortestDistance == 0 || shortestDistance > distance)
			{
				shortestDistance = distance;
				closestNPC = npc;
			}
		}

		return closestNPC;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule IsOtherQuestInstanceActive
	// Server
	// ------------------------------------------------------------
	bool IsOtherQuestInstanceActive(ExpansionQuest quest)
	{
		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (activeQuest != quest && activeQuest.GetQuestConfig().GetID() == quest.GetQuestConfig().GetID())
				return true;
		}

		return false;
	}

	#ifdef EXPANSIONMODAI
	ExpansionQuestNPCAIBase GetClosestQuestAINPC(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCAIBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCAIBase npc = m_QuestNPCAIEntities.Get(npcID);
			if (!npc)
				continue;

			npcPos = npc.GetPosition();
			distance = Math.Round(vector.Distance(playerPos, npcPos));
			if (shortestDistance == 0 || shortestDistance > distance)
			{
				shortestDistance = distance;
				closestNPC = npc;
			}
		}

		return closestNPC;
	}

	ExpansionQuestNPCAIBase GetQuestNPCAIByID(int id)
	{
		return  m_QuestNPCAIEntities.Get(id);
	}
	#endif

	ExpansionQuestStaticObject GetQuestObjectByID(int id)
	{
		return  m_QuestObjectEntities.Get(id);
	}

	ExpansionQuestNPCBase GetQuestNPCByID(int id)
	{
		return m_QuestNPCEntities.Get(id);
	}

	Object GetClosestQuestNPCForQuest(int questID, vector position)
	{
		ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
		if (!questConfig)
			return null;

		array<int> questNPCIDs = questConfig.GetQuestTurnInIDs();
		Object target = GetClosestQuestNPCByID(questNPCIDs, position);
		if (!target)
			target = GetClosestQuestObjectByID(questNPCIDs, position);
	#ifdef EXPANSIONMODAI
		if (!target)
			target = GetClosestQuestNPCAIByID(questNPCIDs, position);
	#endif

		return target;
	}

	void PlayerQuestDataCheck()
	{
		array<string> playerDataFiles = new array<string>;
		playerDataFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_PLAYERDATA_FOLDER, ".bin");
		if (playerDataFiles.Count() > 0)
		{
			foreach (string file: playerDataFiles)
			{
				file.Replace(".bin", "");
				Print(ToString() + "::PlayerQuestDataCheck - Check player data file: " + file);
				ExpansionQuestPersistentData questPlayerData = new ExpansionQuestPersistentData();
				if (!questPlayerData.Load(file))
				{
					Error(ToString() + "::PlayerQuestDataCheck - Check for player data file: " + file + " failed. Deleting file!");
					DeleteFile(EXPANSION_QUESTS_PLAYERDATA_FOLDER + file + ".bin");
				}

				questPlayerData.QuestDebug();
			}
		}
	}

	static int GetQuestColor(ExpansionQuestConfig quest)
	{
		int color = ARGB(200, 241, 196, 15);
		if (quest.GetQuestColor() != 0)
		{
			return quest.GetQuestColor();
		}
		else
		{
			if (quest.IsGroupQuest())
			{
				return ARGB(200, 221, 38, 38);
			}
			else
			{
				if (quest.IsDailyQuest())
				{
					return ARGB(200, 211, 84, 0);
				}
				else if (quest.IsWeeklyQuest())
				{
					return ARGB(200, 72, 52, 212);
				}
				else if (quest.IsRepeatable() && !quest.IsDailyQuest() && !quest.IsWeeklyQuest())
				{
					return ARGB(200, 63, 212, 252);
				}
			}
		}

		return color;
	}
 };