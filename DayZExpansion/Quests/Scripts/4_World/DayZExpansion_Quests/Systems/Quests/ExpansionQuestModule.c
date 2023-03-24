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
	protected ref map<int, ref ExpansionQuestObjectiveAIEscortConfig> m_AIEscortObjectivesConfigs; //! Server
	static ref map<int, ref array<eAIDynamicPatrol>> m_GlobalAIPatrols;
#endif

	protected ref array<ref ExpansionQuestObjectSet> m_QuestObjectSets;	//! Server

	protected const float CHECK_TICK_TIME = 300;
	protected ref ExpansionQuestPersistentServerData m_ServerData; //! Server

	//! Client only
	protected ref array<ref ExpansionQuestConfig> m_QuestClientConfigs;	//! Client
	protected ref ExpansionQuestPersistentData m_ClientQuestData; //! Client
	protected ref ScriptInvoker m_QuestMenuInvoker; //! Client
	protected ref ScriptInvoker m_QuestMenuCallbackInvoker; //! Client

	protected ref array<ref ExpansionQuestAction> m_QuestActions; //! Client

	protected static ExpansionQuestModule s_ModuleInstance;

	void ExpansionQuestModule()
	{
		s_ModuleInstance = this;
	}

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
		EnableClientDisconnect();
		EnableClientNew();
		EnableRPC();

		QuestModulePrint(ToString() + "::OnInit - End");
	}

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
			m_AIEscortObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIEscortConfig>; //! Server
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

			//! AI ESCORT OBJECTIVES
			array<string> aiVIPObjectiveFiles = new array<string>;
			aiVIPObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER, ".json");
			if (aiVIPObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(aiVIPObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
			}
			else
			{
				DefaultAIEscortObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
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

	void ClientModuleInit()
	{
		QuestModulePrint(ToString() + "::ClientModuleInit - Start");

		if (GetGame().IsClient())
		{
			if (!m_QuestsNPCs)
				m_QuestsNPCs = new map<int, ref ExpansionQuestNPCData>; //! Client

			if (!m_QuestClientConfigs)
				m_QuestClientConfigs = new array<ref ExpansionQuestConfig>; //! Client

			if (!m_QuestMenuInvoker)
				m_QuestMenuInvoker = new ScriptInvoker(); //! Client

			if (!m_QuestMenuCallbackInvoker)
				m_QuestMenuCallbackInvoker = new ScriptInvoker(); //! Client
		}

		QuestModulePrint(ToString() + "::ClientModuleInit - End");
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnInvokeConnect - Start");

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			QuestModulePrint(ToString() + "::OnInvokeConnect - Initialize quest system on the connected client with UID: [" + cArgs.Identity.GetId() + "].");
			//! Need to init in next frame because group init happens in OnInvokeConnect as well and needs to be done first
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(InitQuestSystemClient, cArgs.Identity);
		}

		QuestModulePrint(ToString() + "::OnInvokeConnect - End");
	}

	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
		QuestModulePrint(ToString() + "::OnClientDisconnect - Start");

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		//! [Server] Player disconnects and we look for any existing ExpansionQuestPersistentData of the player with his UID.
		//! If we find existing data we sync that data from the players client to the server and save it for the next session.
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
			CleanupPlayerQuests(cArgs.UID);

		QuestModulePrint(ToString() + "::OnClientDisconnect - End");
	}

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
				UpdateQuestForQuestPlayers(quest, true);

				//! Cleanup quest
				quest.OnQuestCleanup();
				m_ActiveQuests.RemoveOrdered(i);
			}
		}

		QuestModulePrint(ToString() + "::OnMissionFinish - End");
	}

	//! Server
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

		InitClientQuests(questPlayerData, identity);

		QuestModulePrint(ToString() + "::InitQuestSystemClient - End");
	}

	//! Server
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
		auto player = PlayerBase.Cast(identity.GetPlayer());
		if (player)
		{
			auto group = player.Expansion_GetParty();
			if (group)
				groupID = group.GetPartyID();
		}
		//! Check for all active group quests the player can participate in one of them if he is member of a group.
		InitClientQuests_Groups(playerData, playerUID, activeGroupQuestIDs);
	#endif

		//! Recreate old quests based on the data from the players persistent quest data.
		InitClientQuests_NormalQuests(playerData, playerUID, activeGroupQuestIDs, groupID);

		//! Create archievement and auto-start quests.
		InitClientQuests_AutoAndAchievementQuests(playerData, playerUID);

		//! Send all valid quest configurations to the client.
		SendClientQuestConfigs(identity);

		//! Send the players persistent quest data to the client.
		SendClientQuestData(identity);

		//! Call AfterClientInit method. Can be used to hook into the quest module and call events after client initalisation.
		AfterClientInit(playerData, identity);

		QuestModulePrint(ToString() + "::InitClientQuests - End");
	}

	//! Server
	//! Boilerplate method that can be used to call events/methods after quest module client initalisation.
	void AfterClientInit(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity);

	//! Server
	//! Check for all active group quests the player can participate in one of them if he is member of a group.
#ifdef EXPANSIONMODGROUPS
	protected void InitClientQuests_Groups(ExpansionQuestPersistentData playerData, string playerUID, inout array<int> activeGroupQuestIDs)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_Groups - Start");

		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (!activeQuest || !activeQuest.IsInitialized())
			{
				Error(ToString() + "::InitClientQuests_Groups - Could not get quest from active quests array!");
				continue;
			}

			//! Check if player is group member of current checked active quest
			if (activeQuest.GetQuestConfig().IsGroupQuest() && activeQuest.IsQuestGroupMember(playerUID))
			{
				QuestModulePrint(ToString() + "::InitClientQuests_Groups - There is already a active quest instance for this quest. Skip this quest for quest init!");
				activeGroupQuestIDs.Insert(activeQuest.GetQuestConfig().GetID());

				QuestModulePrint(ToString() + "::InitClientQuests_Groups - Add quest to players quest data!");
				ExpansionQuestConfig activeQuestConfig = GetQuestConfigByID(activeQuest.GetQuestConfig().GetID());
				if (!activeQuestConfig)
					continue;

				playerData.AddQuestData(activeQuestConfig.GetID(), activeQuest.GetQuestState());
				UpdateQuestForQuestPlayers(activeQuest, true);
			}
		}

		QuestModulePrint(ToString() + "::InitClientQuests_Groups - End");
	}
#endif

	//! Server
	//! Recreate old quests based on the data from the players persistent quest data.
	protected void InitClientQuests_NormalQuests(ExpansionQuestPersistentData playerData, string playerUID, array<int> activeGroupQuestIDs, int groupID = -1)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Start");

		array<ref ExpansionQuestPersistentQuestData> playerQuestData = playerData.GetQuestDatas();
		array<int> questDatasToCleanUp = new array<int>;
		foreach (ExpansionQuestPersistentQuestData data: playerQuestData)
		{
			int questID = data.QuestID;
			ExpansionQuestState questState = data.State;
			string stateName = typename.EnumToString(ExpansionQuestState, questState);
			QuestModulePrint(ToString() + "::InitClientQuests_NormalQuests - Checking quest with id: " + questID + " | State: " + stateName);

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
				if (!configInstance || !QuestConfigComparison(data, configInstance))
				{
					Error(ToString() + "::InitClientQuests_NormalQuests - Could not get any quest configuration for quest ID: " + questID);
					continue;
				}

				//! Create quest instance and set current quest status from persistent player quest data
				ExpansionQuest newQuestInstance = new ExpansionQuest(this);

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

	protected bool QuestConfigComparison(ExpansionQuestPersistentQuestData data, ExpansionQuestConfig questConfig)
	{
		array<ref ExpansionQuestObjectiveData> dataObjectives = data.GetQuestObjectives();
		foreach (ExpansionQuestObjectiveData objectiveData: dataObjectives)
		{
			ExpansionQuestObjectiveType objectiveType = objectiveData.GetObjectiveType();
			int index = objectiveData.GetObjectiveIndex();
			array<ref ExpansionQuestObjectiveConfigBase> questObjectives = questConfig.GetObjectives();
			ExpansionQuestObjectiveConfigBase objectiveConfig = questObjectives[index];
			if (!objectiveConfig || objectiveConfig.GetObjectiveType() != objectiveType)
				return false;
		}

		return true;
	}

	//! Server
	//! Create archievement and auto-start quests.
	protected void InitClientQuests_AutoAndAchievementQuests(ExpansionQuestPersistentData playerData, string playerUID)
	{
		QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Start");

		foreach (int configQuestID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (HasActiveQuestWithID(playerUID, configQuestID))
				continue;

			if (questConfig.IsGroupQuest() || questConfig.GetQuestGiverIDs().Count() > 0 || questConfig.GetPreQuestIDs().Count() > 0)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest with ID: " + configQuestID +" is not a a valid achievement or auto start quest. Skip!");
				continue;
			}

			ExpansionQuestState playerQuestState = playerData.GetQuestStateByQuestID(configQuestID);
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
			}
			//! If the player is already on this archiement/auto-start quest recreate it and update the progress from the players persistent quest data
			else if (playerQuestState == ExpansionQuestState.STARTED || playerQuestState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Create achievement/auto-start quest for quest ID: " + configQuestID + " and add progress from player quest data [UID: " + playerUID + "]");

				ExpansionQuestPersistentQuestData playerQuestData = playerData.GetQuestDataByQuestID(questConfig.GetID());
				if (!playerQuestData || !QuestConfigComparison(playerQuestData, questConfig))
				{
					Print(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Skip quest creation for quest with ID: " + questConfig.GetID() + ". Quest objectives configuration has changed.");
					continue;
				}

				//! Create quest
				autoQuest = new ExpansionQuest(this);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.SetQuestConfig(questConfig);

				//! Get quest objectives progress from persistent player quest data
				GetObjectiveProgressFromQuestData(playerData, autoQuest);

				m_ActiveQuests.Insert(autoQuest);

				if (!autoQuest.OnQuestContinue())
				{
					Error(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest continue event failed for quest  with ID: " + questConfig.GetID());
					CancelQuestServer(autoQuest);
					continue;
				}
			}
		}

		QuestModulePrint(ToString() + "::InitClientQuests_AutoAndAchievementQuests - End");
	}

	override int GetRPCMin()
	{
		return ExpansionQuestModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionQuestModuleRPC.COUNT;
	}

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
	//! Server
	//! Sends all quest configurations to the player if it is not a archievement quest configuration or the player is not part of a certain faction.
	void SendClientQuestConfigs(PlayerIdentity identity)
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
				Error(ToString() + "::SendClientQuestConfigs - Could on get quest config data from valid quest array!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendClientQuestConfigs, false, identity);

		QuestModulePrint(ToString() + "::SendClientQuestConfigs - End");
	}

	//! Client
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

		int i;

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

		for (i = 0; i < questCount; i++)
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

	//! Server
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

	//! Client
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

	//! Server
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

	//! Server
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
	#ifdef EXPANSIONMODAI
		auto npcAI = ExpansionQuestNPCAIBase.Cast(target);
	#endif
		auto npcObject = ExpansionQuestStaticObject.Cast(target);

	#ifdef EXPANSIONMODAI
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
	#ifdef EXPANSIONMODAI
		else if (npcAI)
			questNPCID = npcAI.GetQuestNPCID();
	#endif
		else if (npcObject)
			questNPCID = npcObject.GetQuestNPCID();

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - Quest NPC ID: " + questNPCID);
		ExpansionQuestNPCData questNPCData = GetQuestNPCDataByID(questNPCID);
		if (!questNPCData)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get quest npc data!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questNPCID);
		rpc.Write(questNPCData.GetDefaultNPCText());
		rpc.Write(-1);
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

	//! Server
	void RequestOpenQuestMenu(PlayerIdentity identity, int questID = -1)
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

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(-1);
		rpc.Write("");
		rpc.Write(questID);
		rpc.Send(null, ExpansionQuestModuleRPC.RequestOpenQuestMenu, true, identity);

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - End");
	}

	//! Client
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

		string displayName;
		if (target)
			displayName = target.GetDisplayName();

		int questNPCID;
		if (!ctx.Read(questNPCID))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read quest NPC ID!");
			return;
		}

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Quest NPC ID: " + questNPCID);

		string defaultText;
		if (!ctx.Read(defaultText))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read default text!");
			return;
		}

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Default text: " + defaultText);

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not get questID!");
			return;
		}

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Quest ID: " + questID);

		//! Open quest menu
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionQuestMenu");

		//! Populate quest menu with needed client data.
		m_QuestMenuInvoker.Invoke(displayName, defaultText, questNPCID, questID);

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - End");
	}

	//! Client
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

	//! Server
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

	//! Server
	//! Create new quest instance.
	void CreateQuestInstance(int questID, PlayerIdentity sender)
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
		ExpansionQuest newQuestInstance = new ExpansionQuest(this);

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
				QuestModulePrint(ToString() + "::CreateQuestInstance - Create quest instance for group member " + memberUID);
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				QuestModulePrint(ToString() + "::CreateQuestInstance - Member data: " + memberQuestData.ToString());
				if (!memberQuestData)
				{
					Print(ToString() + "::CreateQuestInstance - Could not get players persisten quest data! Member offline?");
					continue;
				}
				
				memberQuestData.AddQuestData(questID, ExpansionQuestState.NONE);
				
				newQuestInstance.AddGroupMember(memberUID);
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

	//! Client
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
	//! Server
	void CreateClientMarker(vector pos, string text, int questID, PlayerIdentity identity, int objectiveIndex, int visibility = 6)
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
		rpc.Write(objectiveIndex);
		rpc.Write(visibility);
		rpc.Send(null, ExpansionQuestModuleRPC.CreateClientMarker, true, identity);

		QuestModulePrint(ToString() + "::CreateClientMarker - End");
	}

	//! Client
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

		int objectiveIndex = -1;
		if (!ctx.Read(objectiveIndex))
			return;

		int visibility = 6;
		if (!ctx.Read(visibility))
			return;

		CreateMarkerClient(pos, text, questID, is3D, objectiveIndex, visibility);

		QuestModulePrint(ToString() + "::RPC_CreateClientMarker - End");
	}
	
	bool HasQuestMarker(int questID, int objectiveIndex)
	{
		QuestModulePrint(ToString() + "::HasQuestMarker - Start");
		QuestModulePrint(ToString() + "::HasQuestMarker - Quest ID: " + questID + " | Objective index: " + objectiveIndex);
		
		ExpansionMarkerModule markerModule;
		Class.CastTo(markerModule, CF_ModuleCoreManager.Get(ExpansionMarkerModule));	
		ExpansionMarkerClientData clientMarkerData = markerModule.GetData();
		if (!clientMarkerData)
		{
			Error(ToString() + "::HasQuestMarker - Could not get client marker data!");
			return false;
		}

		QuestModulePrint(ToString() + "::HasQuestMarker - Client marker data: " + clientMarkerData.ToString());
		QuestModulePrint(ToString() + "::HasQuestMarker - Client personal markers data count: " + clientMarkerData.m_PersonalMarkers.Count());
		
		string search = "Q" + questID + "_";
		if (objectiveIndex > -1)
		{	
			search += "" + objectiveIndex;			
		}
		
		for (int i = clientMarkerData.m_PersonalMarkers.Count() - 1; i >= 0; i--)
		{
			ExpansionMarkerData markerData = clientMarkerData.m_PersonalMarkers[i];
			if (markerData)
			{
				string uid = markerData.GetUID();
				QuestModulePrint(ToString() + "::HasQuestMarker - Marker UID: " + uid + " Name: " + markerData.GetName());
				
				if (uid.IndexOf(search) == 0)
				{
					Print(ToString() + "::HasQuestMarker - End and return TRUE");
					return true;
				}
			}
		}

		Print(ToString() + "::HasQuestMarker - End and return FALSE");
		return false;
	}

	//! Client
	protected void CreateMarkerClient(vector pos, string text, int questID, bool is3D, int objectiveIndex, int visibility)
	{
		QuestModulePrint(ToString() + "::CreateMarkerClient - Start");
		QuestModulePrint(ToString() + "::CreateMarkerClient - Quest ID: " + questID + " | Objective index: " + objectiveIndex + " | Position: " + pos + " | Text: " + text);

		if (HasQuestMarker(questID, objectiveIndex))
			return;
		
		ExpansionMarkerModule markerModule;
		Class.CastTo(markerModule, CF_ModuleCoreManager.Get(ExpansionMarkerModule));

		bool showMap = false;
		if (is3D && visibility <= 4)
			is3D = false;

		pos[1] = pos[1] + 2.0;
		
		string uid = "Q" + questID + "_";
		if (objectiveIndex > -1)
		{
			uid += "" + objectiveIndex;
		}
		
		QuestModulePrint(ToString() + "::CreateMarkerClient - Set marker UID: " + uid);
		
		ExpansionMarkerData markerData = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, uid, false);
		markerData.SetName(text);
		markerData.SetIcon("Questionmark");
		markerData.SetColor(ARGB(255,241,196,15));
		markerData.SetPosition(pos);
		markerData.Set3D(is3D);
		markerData.SetVisibility(visibility);
		markerData.SetLockState(true);
		markerModule.CreateMarker(markerData);
		
		string markerUID = markerData.GetUID();
		QuestModulePrint("ExpansionMarkerModule::CreateMarkerClient - Create marker with UID: " + markerUID + " Name: " + markerData.GetName() + " for quest " + questID);

		QuestModulePrint(ToString() + "::CreateMarkerClient - End");
	}

	//! Server
	void RemoveClientMarkers(int questID, PlayerIdentity identity, int objectiveIndex)
	{
		QuestModulePrint(ToString() + "::RemoveClientMarkers - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RemoveClientMarkers - Tryed to call RemoveClientMarkers on Client!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(questID);
		rpc.Write(objectiveIndex);
		rpc.Send(null, ExpansionQuestModuleRPC.RemoveClientMarkers, true, identity);

		QuestModulePrint(ToString() + "::RemoveClientMarkers - End");
	}

	//! Client
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

		int objectiveIndex;
		if (!ctx.Read(objectiveIndex))
			return;

		RemoveMarkersClient(questID, objectiveIndex);

		QuestModulePrint(ToString() + "::RPC_RemoveClientMarkers - End");
	}

	//! Client
	protected void RemoveMarkersClient(int questID, int objectiveIndex)
	{
		QuestModulePrint(ToString() + "::RemoveMarkersClient - Start");
		QuestModulePrint(ToString() + "::RemoveMarkersClient - Quest ID: " + questID + " || Objective index: " + objectiveIndex);

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		if (!markerModule)
		{
			Error(ToString() + "::RemoveMarkersClient - Could not get Expansion Marker Module!");
			return;
		}

		ExpansionMarkerClientData clientMarkerData = markerModule.GetData();
		if (!clientMarkerData)
		{
			Error(ToString() + "::RemoveMarkersClient - Could not get client marker data!");
			return;
		}

		QuestModulePrint(ToString() + "::RemoveMarkersClient - Client marker data: " + clientMarkerData.ToString());
		QuestModulePrint(ToString() + "::RemoveMarkersClient - Client personal markers data count: " + clientMarkerData.m_PersonalMarkers.Count());
		
		string search = "Q" + questID + "_";
		if (objectiveIndex > -1)
		{	
			search += "" + objectiveIndex;			
		}
		
		for (int i = clientMarkerData.m_PersonalMarkers.Count() - 1; i >= 0; i--)
		{
			ExpansionMarkerData markerData = clientMarkerData.m_PersonalMarkers[i];
			if (markerData)
			{
				string uid = markerData.GetUID();
				QuestModulePrint(ToString() + "::RemoveMarkersClient - Marker UID: " + uid + " Name: " + markerData.GetName());
				
				if (uid.IndexOf(search) == 0)
				{
					Print(ToString() + "::RemoveMarkersClient - Delete marker!");
					markerModule.RemovePersonalMarkerByUID(uid);
				}
			}
		}

		QuestModulePrint(ToString() + "::RemoveMarkersClient - End");
	}
#endif

	//! Client
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

	//! Server
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

	//! Server
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

		//! Call the OnQuestCancel on the active quest instance.
		//! Sends update of quest data to all quest players also.
		if (!quest.OnQuestCancel())
			QuestModulePrint(ToString() + "::CancelQuestServer - Something went wrong in the cancel quest event for quest with ID: " + questID);

		//! Reset the quest timestamp in the persisten quest data of the quest players.
		UpdateQuestTimestampForQuestPlayers(quest, 0);

		if (!quest.GetQuestConfig().IsGroupQuest())
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

		quest = null;
		m_ActiveQuests.RemoveOrdered(questIndex);

		QuestModulePrint(ToString() + "::CancelQuestServer - End");
	}

	//! Server
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

	//! Server
	protected void SendCallbackToClient(int callback, PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::SendCallbackToClient - Start");

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(callback);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CallbackClient, true, identity);

		QuestModulePrint(ToString() + "::SendCallbackToClient - End");
	}

	//! Client
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
				m_QuestMenuCallbackInvoker.Invoke();
				break;
			}
		}

		QuestModulePrint(ToString() + "::RPC_CallbackClient - End");
	}

	//! Client
	//! Gets called when a player truns in a certain quest
	void RequestTurnInQuestClient(int questID, bool selectReward = false, ExpansionQuestRewardConfig selectedReward = null, int selectedObjItemIndex = -1)
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
		rpc.Write(selectedObjItemIndex);

		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestCompleteQuest, true);

		QuestModulePrint(ToString() + "::RequestTurnInQuestClient - End");
	}

	//! Server
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

		int selectedObjItemIndex = -1;
		if (!ctx.Read(selectedObjItemIndex))
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Read selectedObjItemIndex");
			return;
		}

		if (rewardSelected)
		{
			if (!reward)
				return;

			RequestCompleteQuestServer(questID, playerUID, identity, false, reward, selectedObjItemIndex);
		}
		else
		{
			RequestCompleteQuestServer(questID, playerUID, identity, false, null, selectedObjItemIndex);
		}

		QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - End");
	}

	//! Server
	void RequestCompleteQuestServer(int questID, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
		QuestModulePrint(ToString() + "::RequestCompleteQuestServer - Start");
		
		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
				continue;

			if (!quest.IsQuestPlayer(playerUID))
				continue;

			int instanceQuestID = quest.GetQuestConfig().GetID();
			if (instanceQuestID != questID)
				continue;

		#ifdef EXPANSIONMODGROUPS
			if (quest.GetQuestConfig().IsGroupQuest())
			{
				auto settings = GetExpansionSettings().GetQuest();
				if (settings.GroupQuestMode <= 1 && quest.GetPlayerUID() != playerUID)
				{
					ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
					return;
				}
			}
		#endif

			QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - Quest: " + quest.ToString());
			QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - Quest ID: " + instanceQuestID);
			QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - Can complete quest: " + quest.CanCompleteQuest());

			if (quest.CanCompleteQuest())
			{
				CompleteQuest(quest, playerUID, identity, isAutoComplete, reward, selectedObjItemIndex);
				return;
			}
		}
		
		QuestModulePrint(ToString() + "::RequestCompleteQuestServer - End");
	}

	//! Server
	protected void CompleteQuest(ExpansionQuest quest, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
		QuestModulePrint(ToString() + "::CompleteQuest - Start");

		if (!quest)
		{
			Error(ToString() + "::CompleteQuest - Could not get active quest!");
			return;
		}

		if (!quest.OnQuestTurnIn(playerUID, reward, selectedObjItemIndex))
		{
			QuestModulePrint(ToString() + "::CompleteQuest - Quest turn-in event failed for quest with quest id: " + quest.GetQuestConfig().GetID());
			ExpansionNotification(new StringLocaliser("Quest turn-In failed!"), new StringLocaliser("Something went wrong.."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
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

			int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyResetHour;
			int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyResetMinute;
			int dailyResetHour = GetExpansionSettings().GetQuest().DailyResetHour;
			int dailyResetMinute = GetExpansionSettings().GetQuest().DailyResetMinute;

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

		UpdateQuestCompletionCountForQuestPlayers(quest);

		UpdateQuestForQuestPlayers(quest, true);

	#ifdef EXPANSIONMODNAVIGATION
		RemoveClientMarkers(quest.GetQuestConfig().GetID(), identity, -1);
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

		//! Make this a config parameter in the quest configuration?!
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

	protected void DefaultDeliveryObjectivesData()
	{
		ExpansionQuestObjectiveDeliveryConfig objective_d_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Delivery_001();
		objective_d_1.Save("Objective_D_1");
		m_DeliveryObjectivesConfigs.Insert(1, objective_d_1);

		ExpansionQuestObjectiveDeliveryConfig objective_d_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Delivery_002();
		objective_d_2.Save("Objective_D_2");
		m_DeliveryObjectivesConfigs.Insert(2, objective_d_2);
	}

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

	protected void DefaultTreasureHuntObjectivesData()
	{
		ExpansionQuestObjectiveTreasureHuntConfig objective_th_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_TreasureHunt_001();
		objective_th_1.Save("Objective_TH_1");
		m_TreasureHuntObjectivesConfigs.Insert(1, objective_th_1);
	}

	protected void DefaultActionObjectivesData()
	{
		ExpansionQuestObjectiveActionConfig objective_a_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Action_001();
		objective_a_1.Save("Objective_A_1");
		m_ActionObjectivesConfigs.Insert(1, objective_a_1);

		ExpansionQuestObjectiveActionConfig objective_a_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Action_002();
		objective_a_2.Save("Objective_A_2");
		m_ActionObjectivesConfigs.Insert(2, objective_a_2);
	}

	protected void DefaultCraftingObjectivesData()
	{
		ExpansionQuestObjectiveCraftingConfig objective_cr_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Crafting_001();
		objective_cr_1.Save("Objective_CR_1");
		m_CraftingObjectivesConfigs.Insert(1, objective_cr_1);
	}

#ifdef EXPANSIONMODAI
	protected void DefaultAIPatrolObjectivesData()
	{
		ExpansionQuestObjectiveAIPatrolConfig objective_aip_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIPatrol_001();
		objective_aip_1.Save("Objective_AIP_1");
		m_AIPatrolObjectivesConfigs.Insert(1, objective_aip_1);
	}

	protected void DefaultAICampObjectivesData()
	{
		ExpansionQuestObjectiveAICampConfig objective_aic_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AICamp_001();
		objective_aic_1.Save("Objective_AIC_1");
		m_AICampObjectivesConfigs.Insert(1, objective_aic_1);
	}

	protected void DefaultAIEscortObjectivesData()
	{
		ExpansionQuestObjectiveAIEscortConfig objective_aiescort_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIEscort_001();
		objective_aiescort_1.Save("Objective_AIESCORT_1");
		m_AIEscortObjectivesConfigs.Insert(1, objective_aiescort_1);
	}
#endif

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

	//! Server
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

	protected void LoadQuestNPCData(array<string> questNPCFiles)
	{
		foreach (string file: questNPCFiles)
		{
			GetQuestNPCData(file);
		}
	}

	protected void LoadObjectivesData(array<string> objectiveFiles, string path)
	{
		foreach (string file: objectiveFiles)
		{
			GetObjectiveData(file, path);
		}
	}

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

			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig aiEscortConfig = ExpansionQuestObjectiveAIEscortConfig.Load(fileName);
				if (!aiEscortConfig)
					return;

				aiEscortConfig.CollectAllocationClasses();
				m_AIEscortObjectivesConfigs.Insert(objectiveID, aiEscortConfig);
			}
			break;
		#endif
		}
	}

	protected void LoadQuestData(array<string> questFiles)
	{
		foreach (string file: questFiles)
		{
			GetQuestData(file);
		}
	}

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

	//! Server & Client
	bool HasCompletedQuest(int questID, string playerUID)
	{
		ExpansionQuestPersistentData playerQuestData = GetPlayerQuestData(playerUID);
		if (!playerQuestData)
		{
			QuestModulePrint(ToString() + "::HasCompletedQuest - Could not get quest data for player with UID: " + playerUID + ". Return TRUE");
			return true;
		}

		ExpansionQuestState questState = playerQuestData.GetQuestStateByQuestID(questID);
		int completionCount = playerQuestData.GetQuestCompletionCountByQuestID(questID);
		if (questState != ExpansionQuestState.COMPLETED && completionCount == 0)
		{
			QuestModulePrint(ToString() + "::HasCompletedQuest - Player with UID: " + playerUID + " has never completed quest with ID " + questID + "! Return FALSE");
			return false;
		}

		QuestModulePrint(ToString() + "::HasCompletedQuest - Player with UID: " + playerUID + " has completed quest with ID " + questID + " once! Return TRUE");
		return true;
	}

	//! Server & Client
	ExpansionQuestPersistentData GetPlayerQuestData(string playerUID)
	{
		ExpansionQuestPersistentData playerData;
		if (IsMissionHost())
		{
			playerData = GetPlayerQuestDataByUID(playerUID);
		}
		else if (IsMissionClient())
		{
			playerData = GetClientQuestData();
		}

		return playerData;
	}

	//! Server
	//! Used to cleanup quest items in the players inventory where he has no active quest for.
	protected void QuestItemsCheckAndCleanup(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());

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

	//! Server
	bool HasActiveQuestWithID(string playerUID, int questID)
	{
		foreach (ExpansionQuest activeQuestInstance: m_ActiveQuests)
		{
			if (activeQuestInstance.GetQuestConfig().GetID() == questID && activeQuestInstance.IsQuestPlayer(playerUID))
				return true;
		}

		return false;
	}

	//! Server
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
		if (quest.GetQuestConfig().IsGroupQuest())
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
						travel.SetLocationPosition(objectiveData.GetObjectivePosition());
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
						aiPatrolHunt.SetKillAmount(objectiveData.GetObjectiveAmount());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: AIPATROL");
					}
				}
				break;

				case ExpansionQuestObjectiveType.AICAMP:
				{
					QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective event type: AICAMP");
					ExpansionQuestObjectiveAICampEvent aiCamp = ExpansionQuestObjectiveAICampEvent.Cast(objective);
					if (aiCamp)
					{
						//! Get kill count progress from persistent data.
						aiCamp.SetKillCount(objectiveData.GetObjectiveCount());
						aiCamp.SetKillAmount(objectiveData.GetObjectiveAmount());
						QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - Objective progress added: AICAMP");
					}
				}
				break;
			#endif
			}
		}

		QuestModulePrint(ToString() + "::GetObjectiveProgressFromQuestData - End");
	}

	//! Server
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
				if (quest.GetQuestConfig().IsGroupQuest())
				{
					if (quest.IsOtherGroupMemberOnline(playerUID))
					{
						QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest is a group quest and other group players are still online! Skip quest deletion!");
						continue;
					}
				}
			#endif

				//! Delete quest
				quest.OnQuestCleanup(true);
				m_ActiveQuests.RemoveOrdered(i);
				QuestModulePrint(ToString() + "::CleanupPlayerQuests - Quest deleted!");
			}
		}

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - End");
	}

	//! Server
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

				case ExpansionQuestObjectiveType.AIESCORT:
				{
					QuestModulePrint(ToString() + "::UpdateQuestObjectiveData - Objective event type is: AIESCORT");
					ExpansionQuestObjectiveAIEscortEvent aiEscortObjective;
					if (Class.CastTo(aiEscortObjective, currentObjective))
					{
						if (objectiveData.GetObjectivePosition() == vector.Zero)
							objectiveData.SetObjectivePosition(aiEscortObjective.GetPosition());
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

	//! Server
	//! Updates quest objectives and the quest state data for all quest clients on the server and syncs that data to that clients afterwards.
	void UpdateQuestForQuestPlayers(ExpansionQuest quest, bool updateQuestState)
	{
		QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Start");
		string questState = typename.EnumToString(ExpansionQuestState, quest.GetQuestState());
		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			string playerUID = quest.GetPlayerUID();
			QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + playerUID + " | State: " + questState);
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for player with UID: " + playerUID);
				return;
			}

			ProcessUpdateAndSync(quest, playerQuestData, playerUID, updateQuestState);
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Quest ID: " + quest.GetQuestConfig().GetID() + " | UID: " + memberUID + " | State: " + questState);
				ExpansionQuestPersistentData groupPlayerQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!groupPlayerQuestData)
				{
					Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for player with UID: " + memberUID);
					return;
				}

				ProcessUpdateAndSync(quest, groupPlayerQuestData, memberUID, updateQuestState);
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - End");
	}

	//! Server
	protected void ProcessUpdateAndSync(ExpansionQuest quest, ExpansionQuestPersistentData questData, string playerUID, bool updateQuestState)
	{
		QuestModulePrint(ToString() + "::ProcessUpdateAndSync - Start");

		//! Update quest state for the player.
		if (updateQuestState)
		{
			UpdateQuestState(questData, playerUID, quest);
		}

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

	//! Server
	//! Updates quest state on given persistent quest data with the given quest state.
	protected void UpdateQuestState(ExpansionQuestPersistentData playerData, string playerUID, ExpansionQuest quest, ExpansionQuestState state = ExpansionQuestState.INVALID)
	{
		QuestModulePrint(ToString() + "::UpdateQuestState - Start");
		int questID = quest.GetQuestConfig().GetID();
		ExpansionQuestState questState = quest.GetQuestState();

		if (!playerData)
		{
			Error(ToString() + "::UpdateQuestState - Could not get player data!");
			return;
		}

		//! Update the quest state on the given client data.
		if (state != ExpansionQuestState.INVALID)
			questState = state;

		if (questState == ExpansionQuestState.COMPLETED && quest.GetQuestConfig().IsRepeatable())
			questState = ExpansionQuestState.NONE;

		playerData.UpdateQuestState(questID, questState);
	}

	//! Server
	//! Sets updated time data for all quest clients.
	protected void UpdateQuestTimestampForQuestPlayers(ExpansionQuest quest, int time)
	{
		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - Start");

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - Player quest data: " + playerQuestData.ToString());
				playerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				playerQuestData.Save(quest.GetPlayerUID());
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> memberUIDs = quest.GetPlayerUIDs();			
			foreach (string memberUID: memberUIDs)
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

	//! Server
	//! Sets updated quest completion data for all quest clients.
	protected void UpdateQuestCompletionCountForQuestPlayers(ExpansionQuest quest)
	{
		QuestModulePrint(ToString() + "::UpdateQuestCompletionCountForQuestPlayers - Start");

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			string playerUID = quest.GetPlayerUID();
			QuestModulePrint(ToString() + "::UpdateQuestCompletionCountForQuestPlayers - Player UID: " + playerUID);
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (playerQuestData)
			{
				QuestModulePrint(ToString() + "::UpdateQuestCompletionCountForQuestPlayers - Player quest data: " + playerQuestData.ToString());
				playerQuestData.UpdateCompletionCount(quest.GetQuestConfig().GetID());
				playerQuestData.Save(playerUID);
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				ExpansionQuestPersistentData groupsPlayerQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!groupsPlayerQuestData)
					continue;

				groupsPlayerQuestData.UpdateCompletionCount(quest.GetQuestConfig().GetID());
				groupsPlayerQuestData.Save(memberUID);
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - End");
	}

	void CheckQuestResetTime()
	{
		auto now = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime);
		int weekDay = now.GetDayOfWeek();
		int minutes = now.GetMinutes();
		int hours = now.GetHours();
		string dayName = GetDayName(weekDay);
		string resetDay = GetExpansionSettings().GetQuest().WeeklyResetDay;
		int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyResetHour;
		int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyResetMinute;
		int dailyResetHour = GetExpansionSettings().GetQuest().DailyResetHour;
		int dailyResetMinute = GetExpansionSettings().GetQuest().DailyResetMinute;

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

	void QuestModulePrint(string text)
	{
	//#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		EXTrace.Print(EXTrace.QUESTS, null, text);
	//#endif
	}

	//! Server
	protected void RemoveActiveQuest(ExpansionQuest quest)
	{
		for (int i = m_ActiveQuests.Count() - 1; i >= 0; i--)
		{
			ExpansionQuest activeQuest = m_ActiveQuests.Get(i);
			if (!activeQuest || !activeQuest.IsCompleted())
				continue;

			if (activeQuest == quest)
			{
				activeQuest.OnQuestCleanup();
				Print(ToString() + "::RemoveActiveQuest - Removeing completed quest. ID: " + quest.GetQuestConfig().GetID() + " | Player UID: " + quest.GetPlayerUID());
				m_ActiveQuests.RemoveOrdered(i);
				return;
			}
		}
	}

	// ----------------------------------------------------------------------------------------------------------------------
	//! Getters / Misc functions & methods
	// ----------------------------------------------------------------------------------------------------------------------

	//! Client
	ExpansionQuestPersistentData GetClientQuestData()
	{
		return m_ClientQuestData;
	}

	//! Client
	ScriptInvoker GetQuestMenuSI()
	{
		return m_QuestMenuInvoker;
	}

	//! Client
	ScriptInvoker GetQuestMenuCallbackSI()
	{
		return m_QuestMenuCallbackInvoker;
	}

	//! Server
	ExpansionQuestConfig GetQuestConfigByID(int id)
	{
		ExpansionQuestConfig config;
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			 config = m_QuestConfigs.Get(id);
		}
		else if (GetGame().IsClient())
		{
			config = GetQuestConfigClientByID(id);
		}

		return config;
	}

	//! Client
	array<ref ExpansionQuestConfig> GetQuestConfigsClient()
	{
		return m_QuestClientConfigs;
	}

	//! Client
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

	//! Server & Client
	ExpansionQuestNPCData GetQuestNPCDataByID(int id)
	{
		ExpansionQuestNPCData foundData;
		if (m_QuestsNPCs.Find(id, foundData))
			return foundData;

		return NULL;
	}

	//! Server
	ExpansionQuestPersistentData GetPlayerQuestDataByUID(string playerUID)
	{
		return m_PlayerDatas.Get(playerUID);
	}

	//! Server
	ExpansionDefaultQuestNPCData GetDefaultQuestNPCData()
	{
		return m_DefaultQuestNPCData;
	}

	//! Server
	ExpansionQuestObjectiveTravelConfig GetTravelObjectiveConfigByID(int id)
	{
		return m_TravelObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveTargetConfig GetTargetObjectiveConfigByID(int id)
	{
		return m_TargetObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveDeliveryConfig GetDeliveryObjectiveConfigByID(int id)
	{
		return m_DeliveryObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveCollectionConfig GetCollectionObjectiveConfigByID(int id)
	{
		return m_CollectionObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveTreasureHuntConfig GetTreasureHuntObjectiveConfigByID(int id)
	{
		return m_TreasureHuntObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveActionConfig GetActionObjectiveConfigByID(int id)
	{
		return m_ActionObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveCraftingConfig GetCraftingObjectiveConfigByID(int id)
	{
		return m_CraftingObjectivesConfigs.Get(id);
	}

#ifdef EXPANSIONMODAI
	//! Server
	ExpansionQuestObjectiveAIPatrolConfig GetAIPatrolObjectiveConfigByID(int id)
	{
		return m_AIPatrolObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveAICampConfig GetAICampObjectiveConfigByID(int id)
	{
		return m_AICampObjectivesConfigs.Get(id);
	}

	//! Server
	ExpansionQuestObjectiveAIEscortConfig GetAIEscortObjectiveConfigByID(int id)
	{
		return m_AIEscortObjectivesConfigs.Get(id);
	}

	//! Server
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

	//! Server
	bool QuestPatrolExists(int questID, out array<eAIDynamicPatrol> patrols)
	{
		QuestModulePrint(ToString() + "::QuestPatrolExists - Start");

		array<eAIDynamicPatrol> foundPatrols;
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			patrols = foundPatrols;
			QuestModulePrint(ToString() + "::QuestPatrolExists - End and return TRUE");
			return true;
		}

		QuestModulePrint(ToString() + "::QuestPatrolExists - End and return FALSE");

		return false;
	}

	//! Server
	void SetQuestPatrols(int questID, array<eAIDynamicPatrol> patrols)
	{
		QuestModulePrint(ToString() + "::SetQuestPatrols - Start");

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

		QuestModulePrint(ToString() + "::SetQuestPatrols - End");
	}

	//! Server
	//! @brief removes quest patrol from global array of quest patrols.
	//! Called from ExpansionQuestObjectiveAIEventBase::CleanupPatrol which deals with despawning AI if necessary.
	void RemoveQuestPatrol(int questID)
	{
		QuestModulePrint(ToString() + "::RemoveQuestPatrol - Start");

		m_GlobalAIPatrols.Remove(questID);

		QuestModulePrint(ToString() + "::RemoveQuestPatrol - End");
	}

	//! Server
	bool CanDeleteQuestPatrol(int questID)
	{
		QuestModulePrint(ToString() + "::CanDeleteQuestPatrol - Start");

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
		{
			QuestModulePrint(ToString() + "::CanDeleteQuestPatrol - End and return FALSE");
			return false;
		}

		QuestModulePrint(ToString() + "::CanDeleteQuestPatrol - End and return TRUE");
		return true;
	}

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

	//! Server
	array<ref ExpansionQuest> GetActiveQuests()
	{
		return m_ActiveQuests;
	}

	//! Server
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

	//! Server
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

	//! Server
	bool IsOtherQuestInstanceActive(ExpansionQuest quest)
	{
		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (activeQuest == quest)
				continue;

			if (activeQuest.GetQuestConfig().GetID() == quest.GetQuestConfig().GetID())
				return true;
		}

		return false;
	}

	ExpansionQuestStaticObject GetQuestObjectByID(int id)
	{
		return  m_QuestObjectEntities.Get(id);
	}

	ExpansionQuestNPCBase GetQuestNPCByID(int id)
	{
		return m_QuestNPCEntities.Get(id);
	}

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

		return amount;
	}

	bool QuestDisplayConditions(ExpansionQuestConfig config, PlayerBase player, ExpansionQuestPersistentData playerQuestData = null, int questNPCID = -1)
	{
		QuestModulePrint(ToString() + "::QuestDisplayConditions - Start");
		QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest ID: " + config.GetID());
		QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest NPC ID: " + questNPCID);
		if (playerQuestData)
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Player quest data: " + playerQuestData.ToString());

		if (!player || !player.GetIdentity())
			return false;

		string playerUID = player.GetIdentity().GetId();
		int questID = config.GetID();

		//! Check if quest is completed if not repeatable.
		ExpansionQuestState questState = ExpansionQuestState.NONE;
		if (playerQuestData)
		{
			questState = playerQuestData.GetQuestStateByQuestID(config.GetID());
			string stateText = typename.EnumToString(ExpansionQuestState, questState);
		}

		QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest state is: " + stateText + " | Quest ID: " + questID);
		if (!config.IsRepeatable() && questState == ExpansionQuestState.COMPLETED)
		{
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is already completed and not repeatable!");
			return false;
		}

		//! Check if this quest is a quest of the NPC the client is interacting with.
		if (questNPCID > -1)
		{
			array<int> questGiverIDs = config.GetQuestGiverIDs();
			int questGiverIndex;
			foreach (int questGiverID: questGiverIDs)
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest NPC giver ID [" + questGiverIndex + "]: " + questGiverID);
				questGiverIndex++;
			}

			if ((questState == ExpansionQuestState.NONE || questState == ExpansionQuestState.STARTED || (questState == ExpansionQuestState.COMPLETED && config.IsRepeatable())) && (!questGiverIDs || questGiverIDs.Count() == 0 || questGiverIDs.Find(questNPCID) == -1))
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. NPC is not quest giver NPC!");
				return false;
			}

			array<int> questTurnInIDs = config.GetQuestTurnInIDs();
			int turnInIDIndex;
			foreach (int turnInID: questTurnInIDs)
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest NPC turn-in ID [" + turnInIDIndex + "]: " + turnInID);
				turnInIDIndex++;
			}

			if (questState == ExpansionQuestState.CAN_TURNIN && (!questTurnInIDs || questTurnInIDs.Count() == 0 || questTurnInIDs.Find(questNPCID) == -1))
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. NPC is not quest turn-in NPC!");
				return false;
			}
		}

		//! Check if client has a cooldown on this quest if quest is repeatable.
		int timestamp;
		bool hasCooldown = false;
		if (playerQuestData)
			hasCooldown = playerQuestData.HasCooldownOnQuest(questID, timestamp);

		if (config.IsRepeatable() && hasCooldown)
		{
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is repeatable but on cooldown!");
			return false;
		}

		//! Check if quest is a achievement quest.
		if (config.IsAchievement())
		{
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is achiement quest!");
			return false;
		}

		//! Check if all pre-quests are completed.
		array<int> incompletedQuestIDs = new array<int>;
		if (config.GetPreQuestIDs().Count() > 0 && playerQuestData)
		{
			array<int> preQuestIDs = config.GetPreQuestIDs();
			int completedPreQuestCount;
			int completionCount;

			ExpansionQuestPersistentQuestData questPlayerData = playerQuestData.GetQuestDataByQuestID(questID);
			if (questPlayerData)
				completionCount = questPlayerData.CompletionCount;

			for (int i = 0; i < preQuestIDs.Count(); i++)
			{
				int preQuestID = preQuestIDs[i];
				ExpansionQuestPersistentQuestData preQuestPlayerData = playerQuestData.GetQuestDataByQuestID(preQuestID);
				if (preQuestPlayerData)
				{
					ExpansionQuestState preQuestState = preQuestPlayerData.State;
					int prequestCompletionCount = preQuestPlayerData.CompletionCount;
					if (preQuestState == ExpansionQuestState.COMPLETED || preQuestState == ExpansionQuestState.NONE && prequestCompletionCount > completionCount)
					{
						string preQuestStateText = typename.EnumToString(ExpansionQuestState, preQuestState);
						QuestModulePrint(ToString() + "::QuestDisplayConditions - Quest state for pre-quest with ID: " + preQuestID + " | State: " + preQuestStateText + ". Has completed quest!");
						completedPreQuestCount++;
					}
					else
					{
						incompletedQuestIDs.Insert(preQuestID);
					}
				}
			}
		}

		if (config.GetPreQuestIDs().Count() > 0 && completedPreQuestCount < config.GetPreQuestIDs().Count())
		{
			QuestModulePrint(ToString() + "::QuestDisplayConditions - --------------------------------------------------");
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Cant display quest with ID: " + questID);

			foreach (int incompletedQuestID: incompletedQuestIDs)
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Has not completed quest with ID: " + incompletedQuestID);
			}

			QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Not all pre-quests completed!");
			QuestModulePrint(ToString() + "::QuestDisplayConditions - --------------------------------------------------");

			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		//! Check if for reputation requirement if config has a value but system is disabled.
		if (config.GetReputationRequirement() > 0 && !GetExpansionSettings().GetHardline().UseReputation)
		{
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Reputation system disabled!");
			return false;
		}

		//! Check if client meats the reputation requirement.
		if (config.GetReputationRequirement() > 0)
		{
			int reputation = player.Expansion_GetReputation();
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Reputation requirement: " + config.GetReputationRequirement());
			QuestModulePrint(ToString() + "::QuestDisplayConditions - Player reputation: " + reputation);
			if (reputation < config.GetReputationRequirement())
			{
				QuestModulePrint(ToString() + "::QuestDisplayConditions - Return FALSE. Reputation requirements not met!");
				return false;
			}
		}
	#endif

	#ifdef EXPANSIONMODAI
		//! Check if player has required faction.
		if (config.GetRequiredFaction() != string.Empty)
		{
			eAIGroup group = player.GetGroup();
			if (!group)
				return false;

			eAIFaction playerFaction = group.GetFaction();
			if (!playerFaction)
				return false;

			string factionName = playerFaction.GetName();
			if (factionName != config.GetRequiredFaction())
				return false;
		}

		//! Skip quests that apply the same faction as reward the player has already.
		if (playerFaction && config.GetFactionReward() != string.Empty && playerFaction.GetName() == config.GetFactionReward())
			return false;
	#endif

		QuestModulePrint(ToString() + "::QuestDisplayConditions - End and return TRUE.");

		return true;
	}
	
	//! Server
	//! @note: Gets called from PlayerBase::EEKilled to check if the killed player has a active quest that should be cancled
	//! when a quest player dies.
	void CheckActivePlayerQuests(PlayerBase player)
	{
		if (!player.GetIdentity())
			return;

		string playerUID = player.GetIdentity().GetId();
		foreach (ExpansionQuest activeQuest: m_ActiveQuests)
		{
			if (activeQuest.IsQuestPlayer(playerUID) && activeQuest.GetQuestConfig().CancelQuestOnPlayerDeath)
				activeQuest.CancelQuest();
		}
	}

	//! Server
	ExpansionQuest CreateQuestInstance(string className)
	{
		return ExpansionQuest.Cast(className.ToType().Spawn());
	}

	//! Server
	ExpansionQuestPersistentServerData GetServerData()
	{
		return m_ServerData;
	}

	static ExpansionQuestModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}
 };
