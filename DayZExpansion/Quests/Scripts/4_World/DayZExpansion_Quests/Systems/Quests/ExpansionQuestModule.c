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
	protected ref map<int, ref ExpansionQuestConfig> m_QuestConfigs; //! Server

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
	protected ref map<int, ref ExpansionQuestObjectiveAIEscordConfig> m_AIEscordObjectivesConfigs; //! Server
	static ref map<int, ref array<eAIDynamicPatrol>> m_GlobalAIPatrols;
#endif

	protected ref array<ref ExpansionQuestObjectSet> m_QuestObjectSets;	//! Server

	protected const float CHECK_TICK_TIME = 300;
	protected ref ExpansionQuestPersistentServerData m_ServerData; //! Server

	//! Client only
	protected ref array<ref ExpansionQuestConfig> m_QuestClientConfigs;	//! Client
	protected ref ExpansionQuestPersistentData m_ClientQuestData; //! Client
	protected ref ScriptInvoker m_QuestMenuInvoker; //! Client
#ifdef EXPANSIONMODBOOK
	protected ref ScriptInvoker m_QuestLogCallbackInvoker; //! Client
#endif

#ifdef EXPANSIONMODNAVIGATION
	protected ref array<ref ExpansionQuestClientMarker> m_QuestMarkers; //! Client
#endif

	protected ref array<ref ExpansionQuestAction> m_QuestActions; //! Client

	protected static ExpansionQuestModule s_ModuleInstance;

	// ------------------------------------------------------------
	// ExpansionQuestModule Constructor
	// ------------------------------------------------------------
	void ExpansionQuestModule()
	{
		s_ModuleInstance = this;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		QuestModulePrint(ToString() + "::OnInit - Start");

		super.OnInit();

	#ifdef EXPANSIONMODMARKET
		EnableMissionStart();
	#endif
		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		EnableClientLogout();
		EnableClientLogoutCancelled();
		EnableClientNew();
		EnableClientReady();
		EnableClientDisconnect();
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
	// ExpansionQuestModule OnMissionStart
	// ------------------------------------------------------------
#ifdef EXPANSIONMODMARKET
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnMissionStart - Start");

		super.OnMissionStart(sender, args);

		//! Add delivered/collected quest items from last session to the market zones.
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (FileExist(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE))
			{
				m_ServerData = ExpansionQuestPersistentServerData.Load();
				m_ServerData.AddQuestMarketItemsToZones();
			}
		}

		QuestModulePrint(ToString() + "::OnMissionStart - End");
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnMissionLoaded - Start");

		super.OnMissionLoaded(sender, args);

		m_QuestsNPCs = new map<int, ref ExpansionQuestNPCData>; //! Server & Client
		m_QuestConfigs = new map<int, ref ExpansionQuestConfig>; //! Server & Client
		m_PlayerDatas = new map<string, ref ExpansionQuestPersistentData>; //! Server & Client

	#ifdef EXPANSIONMODGROUPS
		m_PlayerGroups = new map<string, ref array<int>>; //! Server & Client
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			ServerModuleInit();

		if (GetGame().IsClient())
			ClientModuleInit();

		QuestModulePrint(ToString() + "::OnMissionLoaded - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ServerModuleInit
	// ------------------------------------------------------------
	void ServerModuleInit()
	{
		QuestModulePrint(ToString() + "::ServerModuleInit - Start");

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetQuest().EnableQuests)
				return;

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
			m_AIEscordObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIEscordConfig>; //! Server
			m_GlobalAIPatrols = new map<int, ref array<eAIDynamicPatrol>>; //! Server
		#endif

			m_QuestObjectSets = new array<ref ExpansionQuestObjectSet>;

			m_ActiveQuests = new array<ref ExpansionQuest>;
			m_QuestNPCEntities = new map<int, ExpansionQuestNPCBase>;

		#ifdef EXPANSIONMODAI
			m_QuestNPCAIEntities = new map<int, ExpansionQuestNPCAIBase>;
		#endif

			m_QuestObjectEntities = new map<int, ExpansionQuestStaticObject>;

			m_QuestActions = new array<ref ExpansionQuestAction>;

			CreateDirectoryStructure();

			string worldName = ExpansionStatic.GetCanonicalWorldName();

			m_DefaultQuestNPCData = new ExpansionDefaultQuestNPCData(worldName);
			m_DefaultQuestConfigData = new ExpansionDefaultQuestData(worldName);
			m_DefaultObjectiveConfigData = new ExpansionDefaultObjectiveData(worldName);

			if (!m_ServerData && !FileExist(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE))
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

			//! AI ESCORD OBJECTIVES
			array<string> aiVIPObjectiveFiles = new array<string>;
			aiVIPObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER, ".json");
			if (aiVIPObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(aiVIPObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
			}
			else
			{
				DefaultAIEscordObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
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

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckQuestResetTime, CHECK_TICK_TIME * 1000, true); //! Server: Check if daily/weekly reset time is happening.
		}

		QuestModulePrint(ToString() + "::ServerModuleInit - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ClientModuleInit
	// ------------------------------------------------------------
	void ClientModuleInit()
	{
		QuestModulePrint(ToString() + "::ClientModuleInit - Start");

		if (GetGame().IsClient())
		{
			if (!m_QuestClientConfigs)
				m_QuestClientConfigs = new array<ref ExpansionQuestConfig>; //! Client

			if (!m_QuestMenuInvoker)
				m_QuestMenuInvoker = new ScriptInvoker(); //! Client

		#ifdef EXPANSIONMODBOOK
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
		QuestModulePrint(ToString() + "::OnInvokeConnect - Start");

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			QuestModulePrint(ToString() + "::OnInvokeConnect - Initialize quest system on the connected client with UID: [" + cArgs.Identity.GetId() + "].");
			InitQuestSystemClient(cArgs.Identity);
		}

		QuestModulePrint(ToString() + "::OnInvokeConnect - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientDisconnect
	// -----------------------------------------------------------
	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnClientDisconnect - Start");

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		//! [Server] Player disconnects and we look for any existing ExpansionQuestPersistentData of the player with his UID.
		// If we find existing data we sync that data from the players client to the server and save it for the next session.
		//! Maybe move this to the OnClientLogout method
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
			CleanupPlayerQuests(cArgs.UID);

		QuestModulePrint(ToString() + "::OnClientDisconnect - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientLogout
	// -----------------------------------------------------------
	override void OnClientLogout(Class sender, CF_EventArgs args)
	{
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

	// ------------------------------------------------------------
	// ExpansionQuestModule HandleClientLogout
	// Called on server
	// ------------------------------------------------------------
	//! Handles deletion of quest markers and quest items if a quest player starts a logout.
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

	// -----------------------------------------------------------
	// ExpansionQuestModule OnClientLogout
	// -----------------------------------------------------------
	override void OnClientLogoutCancelled(Class sender, CF_EventArgs args)
	{
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

				//! Cleanup quest
				quest.OnQuestCleanup();
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
	// ExpansionQuestModule InitQuestSystemClient
	// Called on server
	// ------------------------------------------------------------
	//! This method is only called once on every player connection to recreate his quests
	// and to send the needed data from the quest module to the client
	void InitQuestSystemClient(PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::InitQuestSystemClient - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::InitQuestSystemClient - Tryed to call InitQuestSystemClient on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::InitQuestSystemClient - Identity is NULL!");
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
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Loaded existing player quest data for player with UID: " + playerUID);
			}
			else
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Created new persistent player quest data for player UID: " + playerUID);
			}
		}
		else
		{
			GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Got cached player quest data for player with UID: " + playerUID);
		}

		 //! If the player has existing quest data we need to initialize any exisitng quests that he started on the server
		InitClientQuests(questPlayerData, identity);

		QuestModulePrint(ToString() + "::InitQuestSystemClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ClientModuleInit
	// Server
	// ------------------------------------------------------------
	//! Handles reinitialisation of quests for a player from persistent data
	//! We need to get and handle the persistent quest progress and objective data of the player
	//! so the progress of the quest objectives contiues and no quest progress is lost.
	protected void InitClientQuests(ExpansionQuestPersistentData playerData, PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::InitClientQuests - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::InitClientQuests - Tryed to call InitClientQuests on Client!");
			return;
		}

		if (!playerData)
		{
			Error(ToString() + "::InitClientQuests - Could not get player quest data!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::InitClientQuests - Could not get player identity!");
			return;
		}

		string playerUID = identity.GetId();
		QuestModulePrint(ToString() + "::InitClientQuests - Player UID: " + playerUID);

		array<int> activeGroupQuestIDs = new array<int>;
		int groupID = -1;
	#ifdef EXPANSIONMODGROUPS
		//! Check for all active group quests the player can participate in one of them if he is member of a group.
		InitClientQuests_Groups(playerData, playerUID, activeGroupQuestIDs, groupID);
	#endif

		//! Recreate old quests based on the data from the players persistent quest data.
		InitClientQuests_NormalQuests(playerData, playerUID, activeGroupQuestIDs, groupID);

		//! Create archievement and auto-start quests.
		InitClientQuests_AutoAndAchievementQuests(playerData, playerUID);

		//! Send all valid quest configurations to the client.
		SendClientQuestConfigs(identity);

		//! Send the players persistent quest data to the client.
		SendClientQuestData(identity);

		QuestModulePrint(ToString() + "::InitClientQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule PlayerQuestsInit_Groups
	// Server
	// ------------------------------------------------------------
	//! Check for all active group quests the player can participate in one of them if he is member of a group.
#ifdef EXPANSIONMODGROUPS
	protected void InitClientQuests_Groups(ExpansionQuestPersistentData playerData, string playerUID, out array<int> activeGroupQuestIDs, out int groupID)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_Groups - Start");

		array<int> activeGroupQuestIDs = new array<int>;
		ExpansionPartyModule partyModule;
		if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			Error(ToString() + "::InitClientQuests_Groups - Could not get party module!");
			return;
		}

		ExpansionPartyPlayerData playerGroupData = partyModule.GetPartyPlayerData(playerUID);
		if (!playerGroupData)
		{
			QuestModulePrint(ToString() + "::InitClientQuests_Groups - Could not get players group player data for UID: " + playerUID + ". Player is not in a group?!");
			return;
		}

		QuestModulePrint(ToString() + "::InitClientQuests_Groups - Found players group player data. UID: " + playerUID);

		ExpansionPartyData group = playerGroupData.GetParty();
		if (!group)
		{
			QuestModulePrint(ToString() + "::InitClientQuests_Groups - Could not get players group data for UID: " + playerUID + ". Players group does not exist?!");
			return;
		}

		QuestModulePrint(ToString() + "::InitClientQuests_Groups - Found players group data for UID: " + playerUID + ". Player is a group!");

		groupID = group.GetPartyID();

		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (!activeQuest || !activeQuest.IsInitialized())
			{
				Error(ToString() + "::InitClientQuests_Groups - Could not get quest from active quests array!");
				continue;
			}

			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//! Check if player is group member of current checked active quest
			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			if (activeQuest.IsGroupQuest() && activeQuest.IsQuestGroupMember(playerUID))
			{
				QuestModulePrint(ToString() + "::InitClientQuests_Groups - There is already a active quest instance for this quest. Skip this quest for quest init!");
				activeGroupQuestIDs.Insert(activeQuest.GetQuestConfig().GetID());

				QuestModulePrint(ToString() + "::InitClientQuests_Groups - Add quest to players quest data!");
				ExpansionQuestConfig activeQuestConfig = GetQuestConfigByID(activeQuest.GetQuestConfig().GetID());
				if (!activeQuestConfig)
					continue;

				playerData.AddQuestData(activeQuestConfig.GetID(), activeQuest.GetQuestState());
				UpdateQuestForQuestPlayers(activeQuest, activeQuest.GetQuestState());
			}
		}

		QuestModulePrint(ToString() + "::InitClientQuests_Groups - End");
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule InitClientQuests_NormalQuests
	// Server
	// ------------------------------------------------------------
	//! Recreate old quests based on the data from the players persistent quest data.
	protected void InitClientQuests_NormalQuests(ExpansionQuestPersistentData playerData, string playerUID, array<int> activeGroupQuestIDs, int groupID = -1)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Start");

		array<ref ExpansionQuestPersistentQuestData> playerQuestData = playerData.GetQuestDatas();
		array<int> questDatasToCleanUp = new array<int>;
		foreach (ExpansionQuestPersistentQuestData data: playerQuestData)
		{
			int questID = data.QuestID;
			int questState = data.State;

			QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Checking quest with id: " + questID + " | State: " + questState);

			//! Check if we want to create a already active group quest
		#ifdef EXPANSIONMODGROUPS
			int findIndex = -1;
			findIndex = activeGroupQuestIDs.Find(questID);
			if (findIndex > -1)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Quest is in skip list. Skip quest creation!");
				continue;
			}
		#endif

			if (HasActiveQuestWithID(playerUID, questID))
			{
				QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Player has already a active quest with this quest ID: " + questID + " | Player UID: " + playerUID);
				continue;
			}

			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Create new quest instance for quest: " + questID + " - Creator UID: " + playerUID);
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitClientQuests_NormalQuests - Create new quest instance for quest: " + questID + " - Creator UID: " + playerUID);

				//! Get quest config. for the quest
				ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
				if (!configInstance)
				{
					Error(ToString() + "::InitClientQuests_NormalQuests - Could not get any quest configuration for quest ID: " + questID);
					continue;
				}

				//! Create quest instance and set current quest status from persistent player quest data
				ExpansionQuest newQuestInstance;
				if (configInstance.GetType() == ExpansionQuestType.SCRIPTED && configInstance.GetQuestClassName() != string.Empty)
				{
					newQuestInstance = CreateQuestInstance(configInstance.GetQuestClassName());
				}
				else
				{
					newQuestInstance = new ExpansionQuest(this);
				}

				//! Set the quest player!
				newQuestInstance.SetPlayerUID(playerUID);

			#ifdef EXPANSIONMODGROUPS
				if (configInstance.IsGroupQuest() && groupID > -1)
				{
					QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Player is member of group with ID: " + groupID + " | Set group for quest " + questID);
					newQuestInstance.SetGroup(groupID); //! Set the quest group!
				}
				else if (configInstance.IsGroupQuest() && groupID == -1)
				{
					QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Can't create instance for group quest with ID: " + questID + ". Player [UID: " + playerUID + "] has no group.");
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
					QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Quest continue event failed for quest with quest id: " + questID);
					CancelQuestServer(newQuestInstance);
					continue;
				}
			}
		}

		if (questDatasToCleanUp.Count() > 0)
		{
			foreach (int questDataToCleanUp: questDatasToCleanUp)
			{
				playerData.RemoveQuestDataByQuestID(questDataToCleanUp);
			}

			playerData.Save(playerUID);
		}

		QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule InitClientQuests_AutoAndAchievementQuests
	// Server
	// ------------------------------------------------------------
	//! Create archievement and auto-start quests.
	protected void InitClientQuests_AutoAndAchievementQuests(ExpansionQuestPersistentData playerData, string playerUID)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Start");

		foreach (int configQuestID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (HasActiveQuestWithID(playerUID, configQuestID))
				continue;

			if (questConfig.IsGroupQuest() || questConfig.GetQuestGiverIDs().Count() > 0 && questConfig.GetQuestGiverIDs()[0] != -1 || questConfig.GetPreQuestIDs().Count() > 0)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest with ID: " + configQuestID +" is not a a valid achivement or auto start quest. Skip!");
				continue;
			}

			int playerQuestState = playerData.GetQuestStateByQuestID(configQuestID);
			ExpansionQuest autoQuest;

			//! If player has no quest state for this achievement quest we create the quest and update the players persistent quest data
			if (playerQuestState == ExpansionQuestState.NONE)
			{
				//! Add quest data to players persistent quest data.
				playerData.AddQuestData(configQuestID, ExpansionQuestState.NONE);

				//! Create quest
				QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Create new achievement/auto-start quest for quest ID: " + configQuestID);
				autoQuest = new ExpansionQuest(this);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.SetQuestConfig(questConfig);

				m_ActiveQuests.Insert(autoQuest);

				if (!autoQuest.OnQuestStart())
				{
					QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest start event failed for quest with quest id: " + configQuestID);
					CancelQuestServer(autoQuest);
					continue;
				}

				UpdateQuestForQuestPlayers(autoQuest, autoQuest.GetQuestState());
			}
			//! If the player is already on this archiement/auto-start quest recreate it and update the progress from the players persistent quest data
			else if (playerQuestState == ExpansionQuestState.STARTED || playerQuestState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Create achievement/auto-start quest for quest ID: " + configQuestID + " and add progress from player quest data [UID: " + playerUID + "]");

				//! Create quest
				autoQuest = new ExpansionQuest(this);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.SetQuestConfig(questConfig);

				//! Get quest objectives progress from persistent player quest data
				GetObjectiveProgressFromQuestData(playerData, autoQuest);

				m_ActiveQuests.Insert(autoQuest);

				if (!autoQuest.OnQuestContinue())
				{
					Error(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest continue event failed for quest quest quest id: " + questConfig.GetID());
					CancelQuestServer(autoQuest);
					continue;
				}

				UpdateQuestForQuestPlayers(autoQuest, autoQuest.GetQuestState());
			}
		}

		QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - End");
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
		QuestModulePrint(ToString() + "::OnRPC - Start");

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionQuestModuleRPC.SendClientQuestConfigs:
			{
				RPC_SendClientQuestConfigs(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.SendClientQuestData:
			{
				RPC_SendClientQuestData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RequestOpenQuestMenu:
			{
				RPC_RequestOpenQuestMenu(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionQuestModuleRPC.RequestCreateQuestInstance:
			{
				RPC_RequestCreateQuestInstance(rpc.Context, rpc.Sender, rpc.Target);
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

	// ------------------------------------------------------------------------------------------------------------------------
	//! RPC Event classes
	// ------------------------------------------------------------------------------------------------------------------------
	// ------------------------------------------------------------
	// ExpansionQuestModule SendClientQuestConfigs
	// Called on server
	// ------------------------------------------------------------
	//! Sends all quest configurations to the player if it is not a archivement quest configuration.
	protected void SendClientQuestConfigs(PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::SendClientQuestConfigs - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendClientQuestConfigs - Tryed to call SendClientQuestConfigs on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::SendClientQuestConfigs - Could not get player identity!");
			return;
		}

		string playerUID = identity.GetId();
		if (!m_QuestConfigs || m_QuestConfigs.Count() == 0)
		{
			Error(ToString() + "::SendClientQuestConfigs - Could not get any quest configurations to send to player with UID: " + playerUID);
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		int questCount = m_QuestConfigs.Count();
		rpc.Write(questCount);

		foreach (ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (!questConfig)
			{
				Error(ToString() + "::SendClientQuestConfigs - Coult on get quest config data from valid quest array!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendClientQuestConfigs, true, identity);

		QuestModulePrint(ToString() + "::SendClientQuestConfigs - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendClientQuestConfigs
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_SendClientQuestConfigs(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
            return;

		QuestModulePrint(ToString() + "::RPC_SendClientQuestConfigs - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendClientQuestConfigs - Tryed to call RPC_SendClientQuestConfigs on Server!");
			return;
		}

		if (!m_QuestClientConfigs)
			m_QuestClientConfigs = new array<ref ExpansionQuestConfig>;
		else
			m_QuestClientConfigs.Clear();

		int questCount;
		if (!ctx.Read(questCount))
		{
			Error(ToString() + "::RPC_SendClientQuestConfigs - Could not read quest config data count!");
			return;
		}

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = new ExpansionQuestConfig();
			if (!questConfig.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendClientQuestConfigs - Error on recieving quest config!");
				return;
			}

			QuestModulePrint(ToString() + "::RPC_SendClientQuestConfigs - Insert config for quest with ID: " + questConfig.GetID() + " | Config: " + questConfig.ToString());
			m_QuestClientConfigs.Insert(questConfig);
		}

		QuestModulePrint(ToString() + "::RPC_SendClientQuestConfigs - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SendClientQuestData
	// Called on server
	// ------------------------------------------------------------
	//! Called to send the persistent quest data from the server to the given client.
	void SendClientQuestData(PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::SendClientQuestData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendClientQuestData - Tryed to call SendClientQuestData on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::SendClientQuestData - Identity is NULL!");
			return;
		}

		//! Get existing player quest data if there is a exiting one in m_PlayerDatas
		string playerUID = identity.GetId();
		ExpansionQuestPersistentData questPlayerData = GetPlayerQuestDataByUID(playerUID);
		if (questPlayerData)
		{
			QuestModulePrint(ToString() + "::SendClientQuestData - Send player quest data to client for UID: " + playerUID);
			auto rpc = ExpansionScriptRPC.Create();
			questPlayerData.OnWrite(rpc);
			rpc.Send(NULL, ExpansionQuestModuleRPC.SendClientQuestData, true, identity);
		}

		QuestModulePrint(ToString() + "::SendClientQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendClientQuestData
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_SendClientQuestData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_SendClientQuestData - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendClientQuestData - Tryed to call RPC_SendClientQuestData on Server!");
			return;
		}

		ExpansionQuestPersistentData data = new ExpansionQuestPersistentData();
		if (!data.OnRead(ctx))
		{
			Error(ToString() + "::RPC_SendClientQuestData - Error on recieving quest player data!");
			return;
		}

		m_ClientQuestData = data;
		m_ClientQuestData.QuestDebug();

		QuestModulePrint(ToString() + "::RPC_SendClientQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestOpenQuestMenuCB
	// Called on server
	// ------------------------------------------------------------
	//! Request the opening of the quest menu on the given client.
	void RequestOpenQuestMenuCB(array<int> questNPCIDs, PlayerIdentity identity)
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

		if (!target)
			return;

		RequestOpenQuestMenu(target, identity);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestOpenQuestMenu
	// Called on server
	// ------------------------------------------------------------
	void RequestOpenQuestMenu(Object target, PlayerIdentity identity)
	{
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

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questNPCID);
		rpc.Write(questNPCData.GetDefaultNPCText());
		rpc.Send(target, ExpansionQuestModuleRPC.RequestOpenQuestMenu, true, identity);

	#ifdef EXPANSIONMODAI
		if (npcAI)
		{
			EmoteManager npcEmoteManager = npcAI.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(questNPCData.NPCInteractionEmoteID);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestOpenQuestMenu
	// Called on client
	// ------------------------------------------------------------
	protected void RPC_RequestOpenQuestMenu(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
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

		int questNPCID;
		if (!ctx.Read(questNPCID))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read quest NPC ID!");
			return;
		}

		string defaultText;
		if (!ctx.Read(defaultText))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read default text!");
			return;
		}

		//! Open quest menu
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionQuestMenu");

		//! Populate quest menu with needed client data.
		m_QuestMenuInvoker.Invoke(target.GetDisplayName(), defaultText, questNPCID);

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Called on Client
	// ------------------------------------------------------------
	//! Request creation of selected quest from quest menu by the client and sends the request to the server.
	void RequestCreateQuestInstance(int questID)
	{
		QuestModulePrint(ToString() + "::RequestCreateQuestInstance - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestCreateQuestInstance - Tryed to call RequestCreateQuestInstance on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestCreateQuestInstance, true);

		QuestModulePrint(ToString() + "::RequestCreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestCreateQuestInstance
	// Called on Server
	// ------------------------------------------------------------
	protected void RPC_RequestCreateQuestInstance(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		QuestModulePrint(ToString() + "::RPC_RequestCreateQuestInstance - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestCreateQuestInstance - Tryed to call CreateQuestInstance on Client!");
			return;
		}

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestCreateQuestInstance - Could not get read quest ID!");
			return;
		}

		CreateQuestInstance(questID, senderRPC);

		QuestModulePrint(ToString() + "::RPC_RequestCreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Called on Server
	// ------------------------------------------------------------
	//! Create new quest instance.
	protected void CreateQuestInstance(int questID, PlayerIdentity sender)
	{
		QuestModulePrint(ToString() + "::CreateQuestInstance - Start");

		//! Get quest config instance and create a copy of that config for our new quest instance
		ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
		if (!configInstance)
		{
			Error(ToString() + "::CreateQuestInstance - Could not get config data for quest with ID: " + questID);
			return;
		}

		string playerUID = sender.GetId();

		//! Create new quest instance.
		ExpansionQuest newQuestInstance;
		if (configInstance.GetType() == ExpansionQuestType.SCRIPTED && configInstance.GetQuestClassName() != string.Empty)
		{
			newQuestInstance = CreateQuestInstance(configInstance.GetQuestClassName());
		}
		else
		{
			newQuestInstance = new ExpansionQuest(this);
		}

		ExpansionQuestPersistentQuestData questData;
		if (!configInstance.IsGroupQuest())
		{
			newQuestInstance.SetPlayerUID(playerUID);

			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				newQuestInstance = null;
				Error(ToString() + "::CreateQuestInstance - Could not get players persisten quest data!");
				return;
			}

			playerQuestData.AddQuestData(questID, ExpansionQuestState.NONE);
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			QuestModulePrint(ToString() + "::CreateQuestInstance - Quest is a group quest. Apply needed parameters!");
			ExpansionPartyModule partyModule;
			if (!Class.CastTo(partyModule, CF_ModuleCoreManager.Get(ExpansionPartyModule)))
			{
				newQuestInstance = null;
				Error(ToString() + "::CreateQuestInstance - Could not get party module!");
				return;
			}

			ExpansionPartyPlayerData groupPlayerData = partyModule.GetPartyPlayerData(playerUID);
			if (!groupPlayerData)
			{
				newQuestInstance = null;
				//! Send notification that quest can only be accepted while in a group.
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			int partyID = groupPlayerData.GetParty().GetPartyID();
			ExpansionPartyData groupData = partyModule.GetPartyByID(partyID);
			if (!groupData)
			{
				newQuestInstance = null;
				Error(ToString() + "::CreateQuestInstance - Could not get party data for party with ID: " + partyID);
				return;
			}

			QuestModulePrint(ToString() + "::CreateQuestInstance - Group data of player with ID: " + playerUID);
			QuestModulePrint(ToString() + "::CreateQuestInstance - Group data: " + groupData.ToString());
			QuestModulePrint(ToString() + "::CreateQuestInstance - Group ID: " + groupData.GetPartyID());

			//! Only group members accept quest!
			auto settings = GetExpansionSettings().GetQuest();
			if (settings.GroupQuestMode < 1 && groupData.GetOwnerUID() != playerUID)
			{
				newQuestInstance = null;
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			//! If the player that is accepting the quest has a group we set the group owner as the main quest player
			newQuestInstance.SetPlayerUID(groupData.GetOwnerUID());

			//! Set the group ID on the quest.
			newQuestInstance.SetGroup(partyID);

			array<ref ExpansionPartyPlayerData> groupMembers = groupData.GetPlayers();
			foreach (ExpansionPartyPlayerData member: groupMembers)
			{
				string memberUID = member.GetID();
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!memberQuestData)
				{
					newQuestInstance = null;
					Error(ToString() + "::CreateQuestInstance - Could not get players persisten quest data!");
					return;
				}

				memberQuestData.AddQuestData(questID, ExpansionQuestState.NONE);
			}
		}
	#endif

		newQuestInstance.SetQuestConfig(configInstance);
		m_ActiveQuests.Insert(newQuestInstance);

		if (!newQuestInstance.OnQuestStart())
		{
			QuestModulePrint(ToString() + "::CreateQuestInstance - Quest start event failed for quest with quest id: " + questID);
			CancelQuestServer(newQuestInstance);
			return;
		}

	#ifdef EXPANSIONMODAI
		ExpansionQuestNPCAIBase npc = GetClosestQuestAINPC(configInstance.GetQuestGiverIDs(), newQuestInstance.GetPlayer().GetPosition());
		if (npc)
		{
			EmoteManager npcEmoteManager = npc.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(npc.GetQuestNPCData().NPCQuestStartEmoteID);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CreateQuestInstance - Created new quest instance for player with UID " + sender.GetId() + " for quest " + questID);
		QuestModulePrint(ToString() + "::CreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Client
	// ------------------------------------------------------------
	protected void CompleteQuestClient(int questID)
	{
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

#ifdef EXPANSIONMODNAVIGATION
	// ------------------------------------------------------------
	// ExpansionQuestModule CreateClientMarker
	// Called on server
	// ------------------------------------------------------------
	void CreateClientMarker(vector pos, string text, int questID, PlayerIdentity identity)
	{
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
	//! Request from client to server to cancel a quest on the server .
	//! update players quest data on the client after it has been changed
	void CancelQuest(int questID)
	{
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

		string questPlayerUID;
		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::CancelQuestServer - Could not get quest from active quest array!");
				continue;
			}

			string playerUID = identity.GetId();
			if (quest.GetQuestConfig().GetID() != questID || !quest.IsQuestPlayer(playerUID))
				continue;

		#ifdef EXPANSIONMODAI
			ExpansionQuestNPCAIBase npc = GetClosestQuestAINPC(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
			if (npc)
			{
				EmoteManager npcEmoteManager = npc.GetEmoteManager();
				if (!npcEmoteManager.IsEmotePlaying())
				{
					npcEmoteManager.PlayEmote(npc.GetQuestNPCData().NPCQuestCancelEmoteID);
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
				}
			}
		#endif

			CancelQuestServer(quest, i);
			//! Log message about quest cancelation
			GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CancelQuestServer - Player with UID " + identity.GetId() + " has cancelled quest " + questID);
		}

		QuestModulePrint(ToString() + "::RPC_CancelQuest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule CancelActiveQuest
	// Server
	// -----------------------------------------------------------
	//! Only called when quest is cancelled by a quest player.
	void CancelQuestServer(ExpansionQuest quest, int questIndex)
	{
		QuestModulePrint(ToString() + "::CancelQuestServer - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CancelQuestServer - Tryed to call CancelQuestServer on Client!");
			return;
		}

		int questID = quest.GetQuestConfig().GetID();

		//! Reset the quest timestamp in the persisten quest data of the quest players.
		UpdateQuestTimestampForQuestPlayers(quest, 0);

		if (!quest.IsGroupQuest())
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(quest.GetPlayerUID());
			if (!player || !player.GetIdentity())
				return;

			//! Send callback about quest cancelation to client.
			SendCallbackToClient(ExpansionQuestModuleCallback.CANCELED_QUEST, player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer || !groupPlayer.GetIdentity())
					continue;

				//! Send callback about quest cancelation to client.
				SendCallbackToClient(ExpansionQuestModuleCallback.CANCELED_QUEST, groupPlayer.GetIdentity());
			}
		}
	#endif

		//! Call the OnQuestCancel on the active quest instance.
		if (!quest.OnQuestCancel())
			QuestModulePrint(ToString() + "::CancelQuestServer - Something went wrong in the cancel quest event for quest with ID: " + questID);

		//! Update and send the quest state and progession data to the quest players.
		//quest.UpdateQuest();

		quest = null;
		m_ActiveQuests.RemoveOrdered(questIndex);

		QuestModulePrint(ToString() + "::CancelQuestServer - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule CancelQuestServer
	// Server
	// -----------------------------------------------------------
	//! Only called by the quest system if somethig went wrong with a active quest.
	void CancelQuestServer(ExpansionQuest quest)
	{
		QuestModulePrint(ToString() + "::CancelQuestServer - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CancelQuestServer - Tryed to call CancelQuestServer on Client!");
			return;
		}

		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest activeQuest = m_ActiveQuests[i];
			if (!activeQuest)
			{
				Error(ToString() + "::CancelQuestServer - Could not get quest from active quest array!");
				continue;
			}

			if (activeQuest != quest)
				continue;

			CancelQuestServer(quest, i);
		}

		QuestModulePrint(ToString() + "::CancelQuestServer - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule SendCallbackToClient
	// Server
	// -----------------------------------------------------------
	protected void SendCallbackToClient(int callback, PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::SendCallbackToClient - Start");

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(callback);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CallbackClient, true, identity);

		QuestModulePrint(ToString() + "::SendCallbackToClient - End");
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

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestTurnInQuestClient
	// Client
	// ------------------------------------------------------------
	//! Gets called when a player truns in a certain quest
	void RequestTurnInQuestClient(int questID, bool selectReward = false, ExpansionQuestRewardConfig selectedReward = null)
	{
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
		foreach (ExpansionQuest quest: m_ActiveQuests)
		{
			if (!quest.IsQuestPlayer(playerUID))
				continue;

			if (quest.GetQuestConfig().GetID() != questID)
				continue;
			
		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest())
			{
				auto settings = GetExpansionSettings().GetQuest();
				if (settings.GroupQuestMode == 0)
					continue;

				if (settings.GroupQuestMode <= 1 && quest.GetPlayerUID() != playerUID)
				{
					ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
					return;
				}
			}
		#endif

			if (quest.CanCompleteQuest())
				CompleteQuest(quest, playerUID, identity, isAutoComplete, reward);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Server
	// ------------------------------------------------------------
	protected void CompleteQuest(ExpansionQuest quest, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null)
	{
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
			CancelQuestServer(quest);
			return;
		}

		UpdateQuestForQuestPlayers(quest, quest.GetQuestState());

	#ifdef EXPANSIONMODNAVIGATION
		RemoveClientMarkers(quest.GetQuestConfig().GetID(), identity);
	#endif

	#ifdef EXPANSIONMODAI
		ExpansionQuestNPCAIBase npc = GetClosestQuestAINPC(quest.GetQuestConfig().GetQuestTurnInIDs(), quest.GetPlayer().GetPosition());
		if (npc)
		{
			EmoteManager npcEmoteManager = npc.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(npc.GetQuestNPCData().NPCQuestCompleteEmoteID);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif

		//! Make this a config parameter in the quest configuration.
		/*if (!isAutoComplete)
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
		}*/

		RemoveActiveQuest(quest);

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CompleteQuest - Player with UID " + identity.GetId() + " has completed quest " + quest.GetQuestConfig().GetID());
		QuestModulePrint(ToString() + "::CompleteQuest - End");
	}

	// ----------------------------------------------------------------------------------------------------------------------
	//! Default quest data
	// ----------------------------------------------------------------------------------------------------------------------

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
	// ExpansionQuestModule DefaultAIEscordObjectivesData
	// Server
	// -----------------------------------------------------------
	protected void DefaultAIEscordObjectivesData()
	{
		ExpansionQuestObjectiveAIEscordConfig objective_aiescord_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIEscord_001();
		objective_aiescord_1.Save("Objective_AIESCORD_1");
		m_AIEscordObjectivesConfigs.Insert(1, objective_aiescord_1);
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

	// ----------------------------------------------------------------------------------------------------------------------
	//! System related functions and methods
	// ----------------------------------------------------------------------------------------------------------------------

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
				ExpansionQuestObjectiveTravelConfig travelConfig = ExpansionQuestObjectiveTravelConfig.Load(fileName);
				if (!travelConfig)
					return;

				m_TravelObjectivesConfigs.Insert(objectiveID, travelConfig);
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = ExpansionQuestObjectiveDeliveryConfig.Load(fileName);
				if (!deliveryConfig)
					return;

				deliveryConfig.CollectAllocationClasses();
				m_DeliveryObjectivesConfigs.Insert(objectiveID, deliveryConfig);
			}
			break;

			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = ExpansionQuestObjectiveTargetConfig.Load(fileName);
				if (!targetConfig)
					return;

				targetConfig.CollectAllocationClasses();
				m_TargetObjectivesConfigs.Insert(objectiveID, targetConfig);
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = ExpansionQuestObjectiveCollectionConfig.Load(fileName);
				if (!collectionConfig)
					return;

				collectionConfig.CollectAllocationClasses();
				m_CollectionObjectivesConfigs.Insert(objectiveID, collectionConfig);
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureConfig = ExpansionQuestObjectiveTreasureHuntConfig.Load(fileName);
				if (!treasureConfig)
					return;
				
				treasureConfig.CollectAllocationClasses();
				m_TreasureHuntObjectivesConfigs.Insert(objectiveID, treasureConfig);
			}
			break;

			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionConfig = ExpansionQuestObjectiveActionConfig.Load(fileName);
				if (!actionConfig)
					return;

				actionConfig.CollectAllocationClasses();
				m_ActionObjectivesConfigs.Insert(objectiveID, actionConfig);
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig = ExpansionQuestObjectiveCraftingConfig.Load(fileName);
				if (!craftingConfig)
					return;

				craftingConfig.CollectAllocationClasses();
				m_CraftingObjectivesConfigs.Insert(objectiveID, craftingConfig);
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = ExpansionQuestObjectiveAIPatrolConfig.Load(fileName);
				if (!aiPatrolConfig)
					return;

				aiPatrolConfig.CollectAllocationClasses();
				m_AIPatrolObjectivesConfigs.Insert(objectiveID, aiPatrolConfig);
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = ExpansionQuestObjectiveAICampConfig.Load(fileName);
				if (!aiCampConfig)
					return;

				aiCampConfig.CollectAllocationClasses();
				m_AICampObjectivesConfigs.Insert(objectiveID, aiCampConfig);
			}
			break;

			case ExpansionQuestObjectiveType.AIESCORD:
			{
				ExpansionQuestObjectiveAIEscordConfig aiEscordConfig = ExpansionQuestObjectiveAIEscordConfig.Load(fileName);
				if (!aiEscordConfig)
					return;

				aiEscordConfig.CollectAllocationClasses();
				m_AIEscordObjectivesConfigs.Insert(objectiveID, aiEscordConfig);
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
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasCompletedQuest
	// 	Server & Client
	// ------------------------------------------------------------
	bool HasCompletedQuest(int questID, string playerUID)
	{
		ExpansionQuestPersistentData playerQuestData;
		if (IsMissionHost())
		{
			playerQuestData = GetPlayerQuestDataByUID(playerUID);
		}
		else if (IsMissionClient())
		{
			playerQuestData = m_ClientQuestData;
		}

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
	// ExpansionQuestModule QuestItemsCheckAndCleanup
	// Called on server
	// ------------------------------------------------------------
	//! Used to cleanup quest items in the players inventory where he has no active quest for.
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
			if (activeQuestInstance.GetQuestConfig().GetID() == questID && activeQuestInstance.IsQuestPlayer(playerUID))
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
		QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Start");
		QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Quest ID: " + quest.GetQuestConfig().GetID());
		
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

		int questID = quest.GetQuestConfig().GetID();
		ExpansionQuestPersistentQuestData playerQuestData = playerData.GetQuestDataByQuestID(questID);
		if (!playerQuestData)
		{
			Error(ToString() + "::GetObjectiveProgressFromQuestData - Could not player quest data for quest with ID: " + questID);
			return;
		}
		
		array<ref ExpansionQuestObjectiveData> questObjectives = playerQuestData.GetQuestObjectives();
		if (!questObjectives || questObjectives.Count() == 0)
		{
			Error(ToString() + "::GetObjectiveProgressFromQuestData - Could not get any objectives from player quest data for quest with ID: " + questID);	
			return;
		}
				
		int activeObjectiveIndex;
		foreach (ExpansionQuestObjectiveData objectiveData: questObjectives)
		{
			int objectiveType = objectiveData.GetObjectiveType();
			int objectiveIndex = objectiveData.GetObjectiveIndex();

			QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective data type: " + objectiveType);
			QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective data index: " + objectiveIndex);
			objectiveData.QuestDebug();

			//! Get the correct objective event base cass with the objective index
			ExpansionQuestObjectiveEventBase objective = quest.GetObjectives().Get(objectiveIndex);
			if (!objective)
			{
				Error(ToString() + "::GetObjectiveProgressFromQuestData - Could not get objective event from quest for index " + objectiveIndex + "!");	
				return;
			}
			
			//! Check if objective types match.
			if (objective.GetObjectiveType() != objectiveType)
			{
				Error(ToString() + "::GetObjectiveProgressFromQuestData - Objective type missmatch for index " + objectiveIndex + "! Objective type: " + objective.GetObjectiveType() + " | Data type: " + objectiveType);
				return;
			}
			
			//! Set it as active objective index correctly if objective was active.
			if (objectiveData.IsActive())
				activeObjectiveIndex = objectiveIndex;
			
			quest.SetCurrentObjectiveIndex(activeObjectiveIndex);
			objective.SetIsActive(objectiveData.IsActive());
			objective.SetTimeLimit(objectiveData.GetTimeLimit());
			
			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.ACTION:
				{
					QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: ACTION");
					ExpansionQuestObjectiveActionEvent action = ExpansionQuestObjectiveActionEvent.Cast(objective);
					if (action)
					{
						//! Get action state progress from persistent data.
						action.SetActionState(objectiveData.GetActionState());
						//! Get execution count progress from persistent data.
						action.SetExecutionCount(objectiveData.GetObjectiveCount());
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
						//! Get kill count progress from persistent data.
						target.SetCount(objectiveData.GetObjectiveCount());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: TARGET");
					}
				}
				break;
				
				case ExpansionQuestObjectiveType.TRAVEL:
				{
					QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: TRAVEL");
					ExpansionQuestObjectiveTravelEvent travel = ExpansionQuestObjectiveTravelEvent.Cast(objective);
					if (travel)
					{
						//! Get destination check progress from persistent data.
						travel.SetLocationState(objectiveData.GetActionState());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: TRAVEL");
					}
				}
				break;
				
				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: TREASUREHUNT");
					ExpansionQuestObjectiveTreasureHuntEvent treasureHunt = ExpansionQuestObjectiveTreasureHuntEvent.Cast(objective);
					if (treasureHunt)
					{
						//! Get destination check progress from persistent data.
						treasureHunt.SetLocationState(objectiveData.GetActionState());
						//! Set the previous position for the stash location from persistent data.
						treasureHunt.SetStashPosition(objectiveData.GetObjectivePosition());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: TREASUREHUNT");
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
						//! Get kill count progress from persistent data.
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
						//! Get kill count progress from persistent data.
						aiCampHunt.SetKillCount(objectiveData.GetObjectiveCount());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: AICAMP");
					}
				}
				break;
			#endif
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
				//! Dont delete the quest if its a group quest and a other group member is still online
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
				quest.OnQuestCleanup();
				m_ActiveQuests.RemoveOrdered(i);
				QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest deleted!");
			}
		}

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdatePlayerQuestObjectiveData
	// Server
	// ------------------------------------------------------------
	//! Updates quest objective data for given client
	void UpdateQuestObjectiveData(ExpansionQuest quest, ExpansionQuestPersistentData playerQuestData, string playerUID)
	{
		array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
		if (!questObjectiveEvents || questObjectiveEvents.Count() == 0)
			return;

		foreach (ExpansionQuestObjectiveEventBase currentObjective: questObjectiveEvents)
		{
			QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Try update from objective event base: " + currentObjective.ToString());
			ExpansionQuestObjectiveData objectiveData = playerQuestData.GetQuestObjectiveByQuestIDAndIndex(quest.GetQuestConfig().GetID(), currentObjective.GetIndex());
			if (!objectiveData)
			{
				objectiveData = new ExpansionQuestObjectiveData();
				objectiveData.SetObjectiveType(currentObjective.GetObjectiveType());
				objectiveData.SetObjectiveIndex(currentObjective.GetIndex());
			}

			objectiveData.SetActive(currentObjective.IsActive());
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
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: TARGET");
					ExpansionQuestObjectiveTargetEvent targetObjective;
					if (Class.CastTo(targetObjective, currentObjective))
					{
						if (objectiveData.GetObjectiveAmount() <= 0)
							objectiveData.SetObjectiveAmount(targetObjective.GetAmount());
						objectiveData.SetObjectiveCount(targetObjective.GetCount());
					}
				}
				break;

				case ExpansionQuestObjectiveType.TRAVEL:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: TRAVEL");
					ExpansionQuestObjectiveTravelEvent travelObjective;
					if (Class.CastTo(travelObjective, currentObjective))
					{
						if (objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(travelObjective.GetPosition());
						objectiveData.SetActionState(travelObjective.GetLocationState());
					}
				}
				break;

				case ExpansionQuestObjectiveType.COLLECT:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: COLLECT");
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

						if (npcPos != vector.Zero && objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(npcPos);

						array<ref ExpansionQuestDeliveryObjectiveData> collectionData = collectionObjective.GetDeliveryData();
						QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Set collection data: " + collectionData.ToString() + " | Count: " + collectionData.Count());
						objectiveData.SetDeliveries(collectionData);
					}
				}
				break;

				case ExpansionQuestObjectiveType.DELIVERY:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: DELIVERY");
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

						if (npcPos != vector.Zero && objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(npcPos);

						array<ref ExpansionQuestDeliveryObjectiveData> deliveryData = deliveryObjective.GetDeliveryData();
						QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Set delivery data: " + deliveryData.ToString() + " | Count: " + deliveryData.Count());
						objectiveData.SetDeliveries(deliveryData);
					}
				}
				break;

				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: TREASUREHUNT");
					ExpansionQuestObjectiveTreasureHuntEvent treasureObjective;
					if (Class.CastTo(treasureObjective, currentObjective))
					{
						if (objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(treasureObjective.GetPosition());
					}
				}
				break;

				case ExpansionQuestObjectiveType.ACTION:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: ACTION");
					ExpansionQuestObjectiveActionEvent actionObjective;
					if (Class.CastTo(actionObjective, currentObjective))
					{
						objectiveData.SetActionState(actionObjective.GetActionState());
						if (objectiveData.GetObjectiveAmount() <= 0)
							objectiveData.SetObjectiveAmount(actionObjective.GetExecutionAmount());
						objectiveData.SetObjectiveCount(actionObjective.GetExecutionCount());
					}
				}
				break;

				case ExpansionQuestObjectiveType.CRAFTING:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: CRAFTING");
					ExpansionQuestObjectiveCraftingEvent craftingObjective;
					if (Class.CastTo(craftingObjective, currentObjective))
					{
						objectiveData.SetActionState(craftingObjective.GetCraftingState());
						if (objectiveData.GetObjectiveAmount() <= 0)
							objectiveData.SetObjectiveAmount(craftingObjective.GetObjectiveItemsAmount());
						objectiveData.SetObjectiveCount(craftingObjective.GetObjectiveItemsCount());
					}
				}
				break;

			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AICAMP:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: AICAMP");
					ExpansionQuestObjectiveAICampEvent aiCampObjective;
					if (Class.CastTo(aiCampObjective, currentObjective))
					{
						if (objectiveData.GetObjectiveAmount() <= 0)
							objectiveData.SetObjectiveAmount(aiCampObjective.GetAmount());
						objectiveData.SetObjectiveCount(aiCampObjective.GetCount());
					}
				}
				break;

				case ExpansionQuestObjectiveType.AIPATROL:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: AIPATROL");
					ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
					if (Class.CastTo(aiPatrolObjective, currentObjective))
					{
						if (objectiveData.GetObjectiveAmount() <= 0)
							objectiveData.SetObjectiveAmount(aiPatrolObjective.GetAmount());
						objectiveData.SetObjectiveCount(aiPatrolObjective.GetCount());
					}
				}
				break;

				case ExpansionQuestObjectiveType.AIESCORD:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: AIESCORD");
					ExpansionQuestObjectiveAIEscortEvent aiEscordObjective;
					if (Class.CastTo(aiEscordObjective, currentObjective))
					{
						if (objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(aiEscordObjective.GetPosition());
					}
				}
				break;
			#endif
			}

			objectiveData.QuestDebug();

			if (!playerQuestData.UpdateObjective(quest.GetQuestConfig().GetID(), objectiveData.GetObjectiveIndex(), objectiveData))
			{
				Error(ToString() + "::UpdateQuestObjectiveData - Something went wrong when updateing the quest objective data. Player UID: " + playerUID + " | Quest ID: " + quest.GetQuestConfig().GetID());
			}

			ExpansionQuestPersistentQuestData questData = playerQuestData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
			if (questData)
				questData.UpdateLastUpdateTime();

			playerQuestData.Save(playerUID);

			QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Added new objective data for type: " + objectiveType);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Updates quest objectives and the quest state data for all quest clients on the server and syncs that data to that clients afterwards.
	void UpdateQuestForQuestPlayers(ExpansionQuest quest, int state)
	{
		QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Start");

		if (!quest.IsGroupQuest())
		{
			string playerUID = quest.GetPlayerUID();
			QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + playerUID + " | State: " + state);
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for player with UID: " + playerUID);
				return;
			}

			ProcessUpdateAndSync(quest, playerQuestData, state, playerUID);
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: quest.GetPlayerUIDs())
			{
				QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + memberUID + " | State: " + state);
				ExpansionQuestPersistentData groupPlayerQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!groupPlayerQuestData)
				{
					Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for player with UID: " + memberUID);
					return;
				}

				ProcessUpdateAndSync(quest, groupPlayerQuestData, state, memberUID);
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule ProcessUpdateAndSync
	// Server
	// ------------------------------------------------------------
	protected void ProcessUpdateAndSync(ExpansionQuest quest, ExpansionQuestPersistentData questData, int state, string playerUID)
	{
		QuestModulePrint(ToString() + "::ProcessUpdateAndSync - Start");

		//! Update quest state for the player.
		UpdateQuestState(questData, playerUID, quest.GetQuestConfig().GetID(), state);

		//! Update quest objective data for the player.
		UpdateQuestObjectiveData(quest, questData, playerUID);

		//! Check and save persistent player data changes.
		questData.Save(playerUID);

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player || !player.GetIdentity())
		{
			Print(ToString() + "::ProcessUpdateAndSync - Could not get player with UID: " + playerUID);
			return;
		}

		//! Send persistent quest player data from the server to the client.
		SendClientQuestData(player.GetIdentity());

		QuestModulePrint(ToString() + "::ProcessUpdateAndSync - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestState
	// Server
	// ------------------------------------------------------------
	//! Updates quest state on given persistent quest data with the given quest state.
	protected void UpdateQuestState(ExpansionQuestPersistentData playerData, string playerUID, int questID, int state)
	{
		QuestModulePrint(ToString() + "::UpdateQuestState - Start");

		if (!playerData)
		{
			Error(ToString() + "::UpdateQuestState - Could not get player data!");
			return;
		}

		int currentState = playerData.GetQuestStateByQuestID(questID);
		if (currentState == state)
		{
			QuestModulePrint(ToString() + "::UpdateQuestState - State is already " + currentState + ". Skip update..");
			return;
		}

		//! If the state has changed and is on COMPLETED we check if the quest is a repeatable quest and add a count to the completion count in the persistent quest data of the client.
		if (state == ExpansionQuestState.COMPLETED)
		{
			ExpansionQuestPersistentQuestData questData = playerData.GetQuestDataByQuestID(questID);
			if (!questData)
			{
				Error(ToString() + "::UpdateQuestState - Could not get player persisten quest data for quest with ID " + questID);
				return;
			}

			questData.CompletionCount++;
			QuestModulePrint(ToString() + "::UpdateQuestState - Updated completion count for quest. Quest ID: " + questID + " | UID: " + playerData + " | Completion count: " + questData.CompletionCount);
		}

		//! Update the quest state on the given client data.
		playerData.UpdateQuestState(questID, state);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestTimestampForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Sends updated quest time data to all quest clients
	protected void UpdateQuestTimestampForQuestPlayers(ExpansionQuest quest, int time)
	{
		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - Start");

		if (!quest.IsGroupQuest())
		{
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				playerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				playerQuestData.Save(quest.GetPlayerUID());
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: quest.GetPlayerUIDs())
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				ExpansionQuestPersistentData groupsPlayerQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!groupsPlayerQuestData)
					continue;

				groupsPlayerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				groupsPlayerQuestData.Save(groupPlayer.GetIdentity().GetId());
			}
		}
	#endif

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
				int questID = data.QuestID;
				ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
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
				int questID = data.QuestID;
				ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
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
	// ExpansionQuestModule RemoveActiveQuest
	// Server
	// ------------------------------------------------------------
	protected void RemoveActiveQuest(ExpansionQuest quest)
	{
		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest activeQuest = m_ActiveQuests.Get(i);
			if (!activeQuest || !activeQuest.IsCompeleted())
				continue;

			if (activeQuest == quest)
			{
				Print(ToString() + "::RemoveActiveQuest - Removeing completed quest. ID: " + quest.GetQuestConfig().GetID() + " | Player UID: " + quest.GetPlayerUID());
				m_ActiveQuests.RemoveOrdered(i);
				return;
			}
		}
	}

	// ----------------------------------------------------------------------------------------------------------------------
	//! Getters / Misc functions & methods
	// ----------------------------------------------------------------------------------------------------------------------

	// ------------------------------------------------------------
	// ExpansionQuestModule GetClientQuestData
	// Client
	// ------------------------------------------------------------
	ExpansionQuestPersistentData GetClientQuestData()
	{
		return m_ClientQuestData;
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
	// ExpansionQuestModule GetQuestConfigsClient
	// Client
	// -----------------------------------------------------------
	array<ref ExpansionQuestConfig> GetQuestConfigsClient()
	{
		return m_QuestClientConfigs;
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestByID
	// Client
	// -----------------------------------------------------------
	ExpansionQuestConfig GetQuestConfigClientByID(int id)
	{
		ExpansionQuestConfig questConfig;
		foreach (ExpansionQuestConfig config: m_QuestClientConfigs)
		{
			if (config.GetID() == id)
				questConfig = config;
		}

		return questConfig;
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
	// ExpansionQuestModule GetAIEscordObjectiveConfigByID
	// Server
	// ------------------------------------------------------------
	ExpansionQuestObjectiveAIEscordConfig GetAIEscordObjectiveConfigByID(int id)
	{
		return m_AIEscordObjectivesConfigs.Get(id);
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
	// ------------------------------------------------------------
	// ExpansionQuestModule GetClosestQuestAINPC
	// ------------------------------------------------------------
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

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCAIByID
	// ------------------------------------------------------------
	ExpansionQuestNPCAIBase GetQuestNPCAIByID(int id)
	{
		return  m_QuestNPCAIEntities.Get(id);
	}
	#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestObjectByID
	// ------------------------------------------------------------
	ExpansionQuestStaticObject GetQuestObjectByID(int id)
	{
		return  m_QuestObjectEntities.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCByID
	// ------------------------------------------------------------
	ExpansionQuestNPCBase GetQuestNPCByID(int id)
	{
		return m_QuestNPCEntities.Get(id);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetClosestQuestNPCForQuest
	// ------------------------------------------------------------
	Object GetClosestQuestNPCForQuest(array<int> npcIDs, vector position)
	{
		Object target = GetClosestQuestNPCByID(npcIDs, position);
		if (!target)
			target = GetClosestQuestObjectByID(npcIDs, position);
	#ifdef EXPANSIONMODAI
		if (!target)
			target = GetClosestQuestNPCAIByID(npcIDs, position);
	#endif
		if (!target)
			return NULL;

		return target;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule PlayerQuestDataCheck
	// ------------------------------------------------------------
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

	// ------------------------------------------------------------
	// ExpansionQuestModule GetQuestColor
	// ------------------------------------------------------------
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

	// -----------------------------------------------------------
	// ExpansionQuestModule GetItemAmount
	// -----------------------------------------------------------
	int GetItemAmount(EntityAI item)
	{
		if (!item)
			Error(ToString() + "::GetItemAmount - Item is NULL!");

		int amount;
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item))
		{
			amount = itemBase.Expansion_GetStackAmount();
		}
		else
		{
			amount = 1;
		}

		if (!MiscGameplayFunctions.Expansion_IsLooseEntity(item, true))
			amount = -amount;

		return amount;
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
	// ExpansionQuestModule GetServerData
	// Called on Server
	// ------------------------------------------------------------
	ExpansionQuestPersistentServerData GetServerData()
	{
		return m_ServerData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetModuleInstance
	// ------------------------------------------------------------
	static ExpansionQuestModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}
 };