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

//! @note QUESTION and EXCLAMATION map to respective parameter of particle
enum ExpansionQuestIndicatorState
{
	NOT_SET = -1,
	QUESTION,
	EXCLAMATION
};

[CF_RegisterModule(ExpansionQuestModule)]
class ExpansionQuestModule: CF_ModuleWorld
{
	static ref map<int, ExpansionQuestNPCBase> s_QuestNPCEntities = new map<int, ExpansionQuestNPCBase>; //! Server & Client
	static ref map<int, ExpansionQuestStaticObject> s_QuestObjectEntities = new map<int, ExpansionQuestStaticObject>; //! Server & Client
#ifdef EXPANSIONMODAI
	static ref map<int, ExpansionQuestNPCAIBase> s_QuestNPCAIEntities = new map<int, ExpansionQuestNPCAIBase>; //! Server & Client
#endif

	static ref map<int, ExpansionQuestIndicatorState> s_QuestNPCIndicatorStates = new map<int, ExpansionQuestIndicatorState>; //! Client
	protected ref map<int, ref ExpansionQuestConfig> m_QuestConfigs; //! Server & Client

	//! Server only
	protected ref map<string, ref ExpansionQuestPersistentData> m_PlayerDatas; //! Server
#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionPartyModule m_PartyModule;
#endif
	//! <player UID or group ID> -> <quest ID> -> quest
	//! Since player UID is base64 and group ID is int, they can't collide.
	//! For group quests, we always add one instance once under group ID key.
	protected ref map<string, ref map<int, ref ExpansionQuest>> m_ActiveQuests; //! Server
	protected ref map<int, ref set<ref ExpansionQuest>> m_ActiveQuestInstances; //! Server
	protected ref map<int, ref ExpansionQuestNPCData> m_QuestsNPCs; //! Server

	//! Default server data.
	protected ref ExpansionDefaultQuestNPCData m_DefaultQuestNPCData; //! Server
	protected ref ExpansionDefaultQuestData m_DefaultQuestConfigData;  //! Server
	protected ref ExpansionDefaultObjectiveData m_DefaultObjectiveConfigData; //! Server

	//! Quest objective configuration data.
	protected ref map<int, ref ExpansionQuestObjectiveTravelConfig> m_TravelObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveDeliveryConfig> m_DeliveryObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveTargetConfig> m_TargetObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveCollectionConfig> m_CollectionObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveTreasureHuntConfig> m_TreasureHuntObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveActionConfig> m_ActionObjectivesConfigs; //! Server
	protected ref map<int, ref ExpansionQuestObjectiveCraftingConfig> m_CraftingObjectivesConfigs; //! Server

#ifdef EXPANSIONMODAI
	//! Quest AI objective configuration data.
	protected ref map<int, ref ExpansionQuestObjectiveAIPatrolConfig> m_AIPatrolObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveAICampConfig> m_AICampObjectivesConfigs;	//! Server
	protected ref map<int, ref ExpansionQuestObjectiveAIEscortConfig> m_AIEscortObjectivesConfigs; //! Server

	//! Map of active AI patrols used for quest objectives.
	static ref map<int, ref array<eAIQuestPatrol>> m_GlobalAIPatrols; //! Server
#endif

	//! Array of active object set spawns used by quests.
	protected ref array<ref ExpansionQuestObjectSet> m_QuestObjectSets;	//! Server

	//! Persistent server quest data
	protected ref ExpansionQuestPersistentServerData m_ServerData; //! Server

	//! Client only
	protected ref ExpansionQuestPersistentData m_ClientQuestData; //! Client
	protected ref ScriptInvoker m_QuestMenuInvoker; //! Client
	protected ref ScriptInvoker m_QuestMenuCallbackInvoker; //! Client
	protected ref ScriptInvoker m_QuestHUDCallbackInvoker; //! Client

	protected ref array<int> m_TempQuestHolders;

	//! Map of active quest objective triggers.
	static ref map<int, ref array<ExpansionObjectiveTriggerBase>> m_GlobalTriggers; //! Server

	protected static ExpansionQuestModule s_ModuleInstance;

	void ExpansionQuestModule()
	{
		s_ModuleInstance = this;

		m_QuestsNPCs = new map<int, ref ExpansionQuestNPCData>; //! Server
		m_QuestConfigs = new map<int, ref ExpansionQuestConfig>; //! Server
		m_PlayerDatas = new map<string, ref ExpansionQuestPersistentData>; //! Server

	#ifdef EXPANSIONMODGROUPS
		CF_Modules<ExpansionPartyModule>.Get(m_PartyModule);
	#endif

	#ifdef SERVER
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
		m_GlobalAIPatrols = new map<int, ref array<eAIQuestPatrol>>; //! Server
	#endif

		m_QuestObjectSets = new array<ref ExpansionQuestObjectSet>;

		m_ActiveQuests = new map<string, ref map<int, ref ExpansionQuest>>;
		m_ActiveQuestInstances = new map<int, ref set<ref ExpansionQuest>>;
	#else
		m_QuestMenuInvoker = new ScriptInvoker(); //! Client
		m_QuestMenuCallbackInvoker = new ScriptInvoker(); //! Client
		m_QuestHUDCallbackInvoker = new ScriptInvoker(); //! Client
	#endif

		m_GlobalTriggers = new map<int, ref array<ExpansionObjectiveTriggerBase>>; //! Server
	}

	override void OnInit()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		EnableClientDisconnect();
		EnableClientNew();
		Expansion_EnableRPCManager();

		Expansion_RegisterClientRPC("RPC_SendClientQuestConfigs");
		Expansion_RegisterClientRPC("RPC_SendClientQuestData");
		Expansion_RegisterClientRPC("RPC_RequestOpenQuestMenu");
		Expansion_RegisterServerRPC("RPC_ExitQuestMenu");
		Expansion_RegisterServerRPC("RPC_RequestCreateQuestInstance");

		Expansion_RegisterClientRPC("RPC_CreateClientMarker");
		Expansion_RegisterClientRPC("RPC_RemoveClientMarkers");
		Expansion_RegisterServerRPC("RPC_CancelQuest");
		Expansion_RegisterClientRPC("RPC_CallbackClient");
		Expansion_RegisterServerRPC("RPC_RequestCompleteQuest");

		Expansion_RegisterServerRPC("RPC_RequestShareQuest");
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnMissionStart(sender, args);

	#ifdef EXPANSIONMODMARKET
		//! Add delivered/collected quest items from last session to the market zones.
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (FileExist(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE))
			{
				m_ServerData = ExpansionQuestPersistentServerData.Load();
				m_ServerData.AddQuestMarketItemsToZones();
			}
		}
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			ServerModuleInit();
	}

	void ServerModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (!GetExpansionSettings().GetQuest().EnableQuests)
				return;

			if (!FileExist(EXPANSION_QUESTS_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_FOLDER);
			if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_FOLDER);
			if (!FileExist(EXPANSION_QUESTS_PLAYERDATA_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_PLAYERDATA_FOLDER);
			if (!FileExist(EXPANSION_QUESTS_GROUPDATA_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_GROUPDATA_FOLDER);

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
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER))
			{
				array<string> travelObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER, ".json");
				if (travelObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(travelObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER);
				DefaultTravelObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! TARGET OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER))
			{
				array<string> targetObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER, ".json");
				if (targetObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(targetObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER);
				DefaultTargetObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! DELIVERY OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER))
			{
				array<string> deliveryObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER, ".json");
				if (deliveryObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(deliveryObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER);
				DefaultDeliveryObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! COLLECTION OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER))
			{
				array<string> collectionObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER, ".json");
				if (collectionObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(collectionObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER);
				DefaultCollectionObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! TREASURE HUNT OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER))
			{
				array<string> treasureHuntObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER, ".json");
				if (treasureHuntObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(treasureHuntObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);
				DefaultTreasureHuntObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! ACTION OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER))
			{
				array<string> actionObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER, ".json");
				if (actionObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(actionObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER);
				DefaultActionObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! CRAFTING OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER))
			{
				array<string> craftingObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER, ".json");
				if (craftingObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(craftingObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER);
				DefaultCraftingObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

		#ifdef EXPANSIONMODAI
			//! AI PATROL OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER))
			{
				array<string> aiPatrolObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER, ".json");
				if (aiPatrolObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(aiPatrolObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);
				DefaultAIPatrolObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! AI CAMP OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER))
			{
				array<string> aiCampObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER, ".json");
				if (aiCampObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(aiCampObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);
				DefaultAICampObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! AI ESCORT OBJECTIVES
			if (FileExist(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER))
			{
				array<string> aiVIPObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER, ".json");
				if (aiVIPObjectiveFiles.Count() > 0)
				{
					LoadObjectivesData(aiVIPObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
				DefaultAIEscortObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}
		#endif

			//! QUEST FILES
			if (FileExist(EXPANSION_QUESTS_QUESTS_FOLDER))
			{
				array<string> questFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_QUESTS_FOLDER, ".json");
				if (questFiles.Count() > 0)
				{
					LoadQuestData(questFiles); //! Server: Load existing quest data files from the server and load them into m_QuestConfigs.
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_QUESTS_FOLDER);
				DefaultQuestData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! GET QUEST OBJECT SETS FROM QUESTS
			LoadObjectSets();

			//! QUEST NPC FILES
			if (FileExist(EXPANSION_QUESTS_NPCS_FOLDER))
			{
				array<string> questNPCFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_NPCS_FOLDER, ".json");
				if (questNPCFiles.Count() > 0)
				{
					LoadQuestNPCData(questNPCFiles); //! Server: Load existing quest NPCs data files from the server and load them into m_QuestsNPCs.
				}
			}
			else
			{
				MakeDirectory(EXPANSION_QUESTS_NPCS_FOLDER);
				DefaultQuestNPCData(); //! Server: Create default quest NPCs data on the server and load them into m_QuestsNPCs.
			}
		}
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnMissionLoaded(sender, args);

		//! Spawn NPCs late so mapping already loaded
		if (GetGame().IsServer())
			SpawnQuestNPCs();
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		//! If this is a respawn, need to do nothing of the below
		if (SyncEvents.s_Expansion_RespawningUIDs[cArgs.Identity.GetId()])
			return;

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			QuestModulePrint("Initialize quest system on the connected client with UID: [" + cArgs.Identity.GetId() + "].");
			//! Need to init in next frame because group init happens in OnInvokeConnect as well and needs to be done first
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(InitQuestSystemClient, cArgs.Identity);
		}
	}

	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			CleanupPlayerQuests(cArgs.UID);
			m_PlayerDatas.Remove(cArgs.UID);
		}
	}

	override void OnMissionFinish(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.OnMissionFinish(sender, args);

	#ifdef SERVER
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			foreach (set<ref ExpansionQuest> activeQuestInstances: m_ActiveQuestInstances)
			{
				foreach (ExpansionQuest quest: activeQuestInstances)
				{
					//! Update and save the persistent player quest data
					UpdateQuestForQuestPlayers(quest, true);

					//! Cleanup quest
					quest.OnQuestCleanup();
				}
			}
		}
	#endif
	}

	//! Server
	//! This method is only called once on every player connection to recreate his quests
	// and to send the needed data from the quest module/server to the client
	void InitQuestSystemClient(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		bool sendConfigs;

		string playerUID = identity.GetId();
		ExpansionQuestPersistentData questPlayerData = GetPlayerQuestDataByUID(playerUID);
		if (!questPlayerData)
		{
			//! If we don't have cached player quest data, check if file exists and load it, else use fresh instance as-is
			questPlayerData = new ExpansionQuestPersistentData();
			m_PlayerDatas.Insert(playerUID, questPlayerData);

			string path = EXPANSION_QUESTS_PLAYERDATA_FOLDER + playerUID + ".bin";
			if (!FileExist(path))
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Created new persistent player quest data for player UID: " + playerUID);
			}
			else if (questPlayerData.Load(playerUID, EXPANSION_QUESTS_PLAYERDATA_FOLDER))
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Loaded existing player quest data for player with UID: " + playerUID);
			}
			else
			{
				Error(ToString() + "::InitQuestSystemClient - Loading existing quest data for player with UID " + playerUID + " failed!");
			}

			questPlayerData.QuestDebug();
			sendConfigs = true;
		}
		else
		{
			GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - InitQuestSystemClient - Got cached player quest data for player with UID: " + playerUID);
			sendConfigs = false;
		}

		questPlayerData.m_SynchDirty = true;
		InitClientQuests(questPlayerData, identity, sendConfigs);
	}
	
	//! Handles quest player data conversion for new group quest data changes
	protected void QuestPlayerDataToGroupConversion(ExpansionQuestPersistentData questPlayerData, ExpansionQuestPersistentData groupData, int groupID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		bool save = false;
		foreach (int questID, ExpansionQuestPersistentQuestData questData: questPlayerData.QuestData)
		{
			ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
			if (!questConfig)
				continue;

			if (questConfig.IsGroupQuest())
			{
				ExpansionQuestPersistentQuestData playerQuestData = questPlayerData.GetQuestDataByQuestID(questID);
				ExpansionQuestPersistentQuestData groupQuestData = groupData.GetQuestDataByQuestID(questID);
				
				//! If there is no valid group quest data for this group quest we add it and use the current data from the owner.
				if (!groupQuestData && playerQuestData)
				{
					ExpansionQuestState questState = questPlayerData.GetQuestStateByQuestID(questID);
					groupData.AddQuestData(questID, questState);
					groupQuestData = groupData.GetQuestDataByQuestID(questID);
					
					groupQuestData.Timestamp = playerQuestData.Timestamp;
					
					for (int i = 0; i < playerQuestData.QuestObjectives.Count(); i++)
					{
						ExpansionQuestObjectiveData objectiveData = new ExpansionQuestObjectiveData();
						objectiveData.Copy(playerQuestData.QuestObjectives[i]);
						groupQuestData.QuestObjectives.InsertAt(objectiveData, i);
					}
					
					groupQuestData.LastUpdateTime = playerQuestData.LastUpdateTime;
					groupQuestData.CompletionCount = playerQuestData.CompletionCount;
					groupQuestData.UpdateLastUpdateTime();
					save = true;
				}
			}
		}
		
		if (save)
			groupData.Save(groupID.ToString(), EXPANSION_QUESTS_GROUPDATA_FOLDER);
	}

	//! Server
	//! Handles reinitialisation of quests for a player from persistent data
	//! We need to get and handle the persistent quest progress and objective data of the player
	//! so the progress of the quest objectives contiues and no quest progress is lost.
	protected void InitClientQuests(ExpansionQuestPersistentData playerQuestData, PlayerIdentity identity, bool sendConfigs = true)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::InitClientQuests - Tryed to call InitClientQuests on Client!");
			return;
		}

		if (!playerQuestData)
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
		QuestModulePrint("Player UID: " + playerUID);

		auto inhibitor = ExpansionInhibitor.Add(this);

		int groupID = -1;
		string ownerUID;

		auto player = PlayerBase.Cast(identity.GetPlayer());

		//! Check for all active group quests the player can participate in one of them if he is member of a group.
		if (player && GetGroupData(player, groupID, ownerUID))
			InitClientQuests_GroupQuestsCheck(playerQuestData, playerUID, groupID);

		//! Recreate existing quests (also group quests) based on the data from the players persistent quest data (or from persistent group quest data).
		InitClientQuests_Stage1(playerQuestData, playerUID, groupID, ownerUID);

		//! Create archievement and auto-start quests.
		InitClientQuests_Stage2(playerQuestData, playerUID);

		delete inhibitor;

		//! Save player quest data after quest initialization process has been finished as there could be modifications to it after load.
		//! @Note: Group quest data is saved in InitClientQuests_Stage1 if groupID is valid and group quest data for that group exists.
		playerQuestData.Save(playerUID, EXPANSION_QUESTS_PLAYERDATA_FOLDER);

		//! Send all valid quest configurations and the players persistent quest data to the client.
		SendClientQuestData(playerQuestData, identity, sendConfigs);

		//! Call MissionBase::Expansion_OnQuestPlayerInit method. Can be used to hook into the quest module and call events after client initalisation via override in Init.c.
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestPlayerInit(playerQuestData, identity);
	}

	//! Server
	void LoadGroupQuestData(string groupID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestPersistentData questGroupData = GetPlayerQuestDataByUID(groupID);
		if (!questGroupData)
		{
			//! If we don't have cached group quest data, check if file exists and load it, else use fresh instance as-is
			questGroupData = new ExpansionQuestPersistentData();
			m_PlayerDatas.Insert(groupID, questGroupData);
			string path = EXPANSION_QUESTS_GROUPDATA_FOLDER + groupID + ".bin";
			if (!FileExist(path))
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - LoadGroupQuestData - Created new persistent group quest data for group with ID: " + groupID);
			}
			else if (questGroupData.Load(groupID, EXPANSION_QUESTS_GROUPDATA_FOLDER))
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - LoadGroupQuestData - Loaded existing group quest data for group with ID: " + groupID);
			}
			else
			{
				Error(ToString() + "::LoadGroupQuestData - Loading existing group quest data for group with ID " + groupID + " failed!");
			}

			questGroupData.QuestDebug();
		}
		else
		{
			GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - LoadGroupQuestData - Got cached group quest data for group with ID: " + groupID);
		}
	}

	//! Server
	//! Check for all active group quests the player can participate in one of them if he is member of a group.
	protected void InitClientQuests_GroupQuestsCheck(ExpansionQuestPersistentData playerData, string playerUID, int groupID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		LoadGroupQuestData(groupID.ToString());

		map<int, ref ExpansionQuest> activeQuests = m_ActiveQuests[groupID.ToString()];
		if (!activeQuests)
			return;

		foreach (ExpansionQuest activeQuest: activeQuests)
		{
			if (!activeQuest.IsInitialized())
			{
				Error(ToString() + "::InitClientQuests_GroupQuestsCheck - quest not initialized!");
				continue;
			}

			QuestModulePrint("Add quest to players quest data!");
			int questID = activeQuest.GetQuestConfig().GetID();
			playerData.AddQuestData(questID, activeQuest.GetQuestState());
			AddActiveQuest(playerUID, activeQuest);

			if (playerData.m_SynchDirty)
			{
				UpdateQuestData(activeQuest, playerData, true);
				SaveAndSyncQuestData(playerData, playerUID, questID);
			}
		}
	}

	//! Server
	//! Recreate existing quests (also group quests) based on the data from the players persistent quest data (or from persistent group quest data).
	protected void InitClientQuests_Stage1(ExpansionQuestPersistentData playerData, string playerUID, int groupID = -1, string ownerID = string.Empty)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestPersistentData groupQuestData;
		map<int, ref ExpansionQuest> activeGroupQuests;
		if (groupID > -1)
		{
			activeGroupQuests = m_ActiveQuests[groupID.ToString()];
			groupQuestData = GetPlayerQuestDataByUID(groupID.ToString());
			
			//! We check the player data for quest data of group quests if the player is a group owner.
			//! If the group data does not contain quest data for a group quest that is in the owners data then we copy that data over to the group data.
			if (playerUID == ownerID)
				QuestPlayerDataToGroupConversion(playerData, groupQuestData, groupID);
		}

		array<int> questDatasToCleanUp = new array<int>;
		foreach (ExpansionQuestPersistentQuestData data: playerData.QuestData)
		{
			int questID = data.QuestID;
			ExpansionQuestState questState = data.State;
			string questStateName = typename.EnumToString(ExpansionQuestState, questState);
			QuestModulePrint("::InitClientQuests_Stage1 - Check quest init for quest with ID: " + questID + " | State: " + questStateName);

			//! Check if we want to create a already active group quest
			if (activeGroupQuests)
			{
				ExpansionQuest activeGroupQuest = activeGroupQuests[questID];
				if (activeGroupQuest && activeGroupQuest.IsInitialized())
				{
					QuestModulePrint("::InitClientQuests_Stage1 - Quest is in skip list. Skip quest creation!");
					continue;
				}
			}

			//! Get quest config. for the quest
			ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
			if (!configInstance || !QuestConfigComparison(data, configInstance))
			{
				Error(ToString() + "::InitClientQuests_Stage1 - Could not get any quest configuration for quest ID: " + questID);
				continue;
			}

			if (!configInstance.IsGroupQuest())
			{
				if (GetActiveQuestWithKey(playerUID, questID))
				{
					QuestModulePrint("::InitClientQuests_Stage1 - Player has already a active quest with this quest ID: " + questID + " | Player UID: " + playerUID);
					continue;
				}
			}
			else if (configInstance.IsGroupQuest() && groupID > -1)
			{
				if (GetActiveQuestWithKey(groupID.ToString(), questID))
				{
					QuestModulePrint("::InitClientQuests_Stage1 - There is already a active quest with this quest ID for this group: " + questID + " | Player UID: " + playerUID + " | Group ID: " + groupID);
					continue;
				}

				if (groupQuestData)
				{
					ExpansionQuestState groupQuestState = groupQuestData.GetQuestStateByQuestID(questID);
					string groupQuestStateName = typename.EnumToString(ExpansionQuestState, groupQuestState);

					//! If the quest has been completed by the group while this member was offline we delete the quest data for this certain quest from the players
					//! persistent quest data so no old progress is left and he can accept the quest again.
					if (playerData.GetQuestCompletionCountByQuestID(questID) < groupQuestData.GetQuestCompletionCountByQuestID(questID))
					{
						questDatasToCleanUp.Insert(questID);
					}

					QuestModulePrint("::InitClientQuests_Stage1 - Use quest state from persistent group data [Quest ID: " + questID + " | State: " + groupQuestStateName);
					questState = groupQuestState;
				}
			}

			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
			{
				GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - Create new quest instance for quest: " + questID + " - Player UID: " + playerUID);

				string instanceKey = playerUID;

				//! Create quest instance and set current quest status from persistent player quest data
				ExpansionQuest newQuestInstance = new ExpansionQuest(this, configInstance);
				if (!configInstance.IsGroupQuest())
				{
					newQuestInstance.SetPlayerUID(playerUID);
				}
				else if (groupID > -1)
				{
					//! If the player that is accepting the quest has a group we set the group owner as the main quest player
					newQuestInstance.SetPlayerUID(ownerID);
					QuestModulePrint("::InitClientQuests_Stage1 - Player is member of group with ID: " + groupID + " | Set group for quest " + questID);
					newQuestInstance.SetGroupID(groupID);
					instanceKey = groupID.ToString();
				}
				else
				{
					QuestModulePrint("::InitClientQuests_Stage1 - Can't create instance for group quest with ID: " + questID + ". Player [UID: " + playerUID + "] has no group.");
					questDatasToCleanUp.Insert(questID);
					newQuestInstance = NULL;
					continue;
				}

				newQuestInstance.Finalize();
				newQuestInstance.SetQuestState(questState);

				//! Get quest objectives progress from persistent player quest data or from group quest data when quest is a group quest.
				if (!configInstance.IsGroupQuest())
				{
					GetObjectiveProgressFromQuestData(playerData, newQuestInstance);
				}
				else if (configInstance.IsGroupQuest() && groupID > -1 && groupQuestData)
				{
					GetObjectiveProgressFromQuestData(groupQuestData, newQuestInstance);
				}

				AddActiveQuest(instanceKey, newQuestInstance);

				if (!newQuestInstance.OnQuestContinue())
				{
					QuestModulePrint("::InitClientQuests_Stage1 - Quest continue event failed for quest with quest id: " + questID + " | Player UID:");
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
		}

		if (groupID > -1 && groupQuestData)
		{
			groupQuestData.Save(groupID.ToString(), EXPANSION_QUESTS_GROUPDATA_FOLDER);
		}
	}

	//! Server
	//! Create archievement and auto-start quests.
	protected void InitClientQuests_Stage2(ExpansionQuestPersistentData playerData, string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		foreach (int configQuestID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (GetActiveQuestWithKey(playerUID, configQuestID))
				continue;

			if (questConfig.IsGroupQuest() || questConfig.GetQuestGiverIDs().Count() > 0 || questConfig.GetPreQuestIDs().Count() > 0)
			{
				QuestModulePrint("Quest with ID: " + configQuestID +" is not a a valid achievement or auto start quest. Skip!");
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
				QuestModulePrint("Create new achievement/auto-start quest for quest ID: " + configQuestID);
				autoQuest = new ExpansionQuest(this, questConfig);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.Finalize();

				AddActiveQuest(playerUID, autoQuest);

				if (!autoQuest.OnQuestStart())
				{
					QuestModulePrint("Quest start event failed for quest with quest id: " + configQuestID);
					CancelQuestServer(autoQuest);
					continue;
				}
			}
			//! If the player is already on this archiement/auto-start quest recreate it and update the progress from the players persistent quest data
			else if (playerQuestState == ExpansionQuestState.STARTED || playerQuestState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestModulePrint("Create achievement/auto-start quest for quest ID: " + configQuestID + " and add progress from player quest data [UID: " + playerUID + "]");

				ExpansionQuestPersistentQuestData playerQuestData = playerData.GetQuestDataByQuestID(questConfig.GetID());
				if (!playerQuestData || !QuestConfigComparison(playerQuestData, questConfig))
				{
					QuestModulePrint("Skip quest creation for quest with ID: " + questConfig.GetID() + ". Quest objectives configuration has changed.");
					continue;
				}

				//! Create quest
				autoQuest = new ExpansionQuest(this, questConfig);
				autoQuest.SetPlayerUID(playerUID);
				autoQuest.Finalize();

				//! Get quest objectives progress from persistent player quest data
				GetObjectiveProgressFromQuestData(playerData, autoQuest);

				AddActiveQuest(playerUID, autoQuest);

				if (!autoQuest.OnQuestContinue())
				{
					Error(ToString() + "::InitClientQuests_AutoAndAchievementQuests - Quest continue event failed for quest  with ID: " + questConfig.GetID());
					CancelQuestServer(autoQuest);
					continue;
				}
			}
		}
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

	// ------------------------------------------------------------------------------------------------------------------------
	//! RPC Event classes
	// ------------------------------------------------------------------------------------------------------------------------
	//! Server
	//! Sends all quest configurations to the player if it is not a archievement quest configuration or the player is not part of a certain faction.
	void SendClientQuestConfigs(PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		string playerUID = identity.GetId();
		if (!m_QuestConfigs || m_QuestConfigs.Count() == 0)
		{
			Error(ToString() + "::SendClientQuestConfigs - Could not get any quest configurations to send to player with UID: " + playerUID);
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_SendClientQuestConfigs");
		SendClientQuestConfigs(rpc, playerUID);
		rpc.Expansion_Send(true, identity);
	}

	void SendClientQuestConfigs(ScriptRPC rpc, string playerUID)
	{
		int questCount = m_QuestConfigs.Count();
		EXPrint(ToString() + " Sending " + questCount + " quest configs to client with UID: " + playerUID);
		rpc.Write(questCount);
		foreach (ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (!questConfig)
			{
				Error(ToString() + "::SendClientQuestConfigs - Could on get quest config data from valid quest array!");
				continue;
			}

		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			questConfig.QuestDebug();
		#endif
			questConfig.OnSend(rpc);
		}
	}

	//! Client
	protected void RPC_SendClientQuestConfigs(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		RPC_SendClientQuestConfigs(ctx);
	}

	protected bool RPC_SendClientQuestConfigs(ParamsReadContext ctx)
	{
		int i;

		m_QuestConfigs.Clear();

		int questCount;
		if (!ctx.Read(questCount))
		{
			Error(ToString() + "::RPC_SendClientQuestConfigs - Could not read quest config data count!");
			return false;
		}

		for (i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = new ExpansionQuestConfig();
			if (!questConfig.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendClientQuestConfigs - Error on recieving quest config!");
				return false;
			}
			
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			questConfig.QuestDebug();
		#endif
			
			QuestModulePrint("Insert config for quest with ID: " + questConfig.GetID() + " | Config: " + questConfig.ToString());
			m_QuestConfigs[questConfig.GetID()] = questConfig;
		}

		if (m_QuestConfigs.Count())
			EXPrint(ToString() + " Received " + m_QuestConfigs.Count() + " quest configs");
		else
			EXPrint(ToString() + " WARNING: Received zero quest configs!");

		return true;
	}

	//! Server
	//! Called to send the persistent quest data from the server to the given client.
	void SendClientQuestData(ExpansionQuestPersistentData questPlayerData, PlayerIdentity identity, bool sendConfigs = false, int questID = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (ExpansionInhibitor.Contains(this))
		{
			QuestModulePrint("SKIPPING SendClientQuestData");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_SendClientQuestData");
		string playerUID = identity.GetId();
		rpc.Write(sendConfigs);
		if (sendConfigs)
		{
			SendClientQuestConfigs(rpc, playerUID);
		}
		else
		{
			EXPrint(ToString() + " Quest configs already synched for player with UID: " + playerUID);
		}

		rpc.Write(questID);

		bool sendData = questPlayerData.m_SynchDirty;
		if (sendData)
		{
			if (questID == -1)
			{
				EXPrint(ToString() + " Sending player quest data for " + questPlayerData.QuestData.Count() + " quests to player with UID: " + playerUID);
			}
			else
			{
				EXPrint(ToString() + " Sending player quest data for quest ID " + questID + " to player with UID: " + playerUID);
			}

			questPlayerData.OnWrite(rpc, true, questID);
		}
		else if (sendConfigs)
		{
			EXPrint(ToString() + " Quest data already synched for player with UID: " + playerUID);
			rpc.Write(0);
		}

		if (sendConfigs || sendData)
		{
			rpc.Expansion_Send(true, identity);
		}
		else
		{
			EXPrint(ToString() + " Quest configs and data already synched for player with UID: " + playerUID);
		}
	}

	//! Client
	protected void RPC_SendClientQuestData(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		bool includeConfigs;
		if (!ctx.Read(includeConfigs))
		{
			Error(ToString() + "::RPC_SendClientQuestData - couldn't read includeConfigs");
			return;
		}

		if (includeConfigs)
		{
			if (!RPC_SendClientQuestConfigs(ctx))
				return;
		}
		else
		{
			if (m_QuestConfigs.Count())
				EXPrint(ToString() + " Quest configs already received (" + m_QuestConfigs.Count() + ")");
			else
				EXPrint(ToString() + " WARNING: No quest configs!");
		}

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_SendClientQuestData - couldn't read quest ID!");
			return;
		}

		ExpansionQuestPersistentData data = new ExpansionQuestPersistentData();
		if (!data.OnRead(ctx))
		{
			Error(ToString() + "::RPC_SendClientQuestData - Error on recieving quest player data!");
			return;
		}

		if (questID == -1)
		{
			//! Update complete quest data
			m_ClientQuestData = data;
			EXPrint(ToString() + " Received quest data for " + m_ClientQuestData.QuestData.Count() + " quests");
		}
		else
		{
			if (data.m_RemoveQuestData)
			{
				m_ClientQuestData.QuestData.Remove(questID);
				EXPrint(ToString() + " Removed quest data for quest ID " + questID);
			}
			else
			{
				auto questData = data.QuestData[questID];
				if (questData)
				{
					m_ClientQuestData.QuestData[questID] = questData;
					EXPrint(ToString() + " Updated quest data for quest ID " + questID);
				}
				else
				{
					EXPrint(ToString() + " WARNING: No data received for quest ID " + questID);
				}
			}
		}

		m_ClientQuestData.QuestDebug();

		if (GetGame().GetMission().GetHud())
			UpdateClient();
		else  //! HUD not yet initialized, delay a bit to allow for init
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(UpdateClient, 1000);
	}

	protected void UpdateClient()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_QuestHUDCallbackInvoker.Invoke();
		UpdateQuestNPCIndicators();
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
		{
			Error(ToString() + "::RequestOpenQuestMenuCB - Could not find quest NPC object!");
			return;
		}

		RequestOpenQuestMenu(target, identity);
	}

	//! Server
	void RequestOpenQuestMenu(Object target, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		int questNPCID = -1;
		if (npc)
		{
			questNPCID = npc.GetQuestNPCID();
		}
		else if (npcObject)
		{
			questNPCID = npcObject.GetQuestNPCID();
		}
	#ifdef EXPANSIONMODAI
		else if (npcAI)
		{
			questNPCID = npcAI.GetQuestNPCID();
		}
	#endif

		QuestModulePrint("Quest NPC ID: " + questNPCID);
		if (questNPCID == -1)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get quest npc ID!");
			return;
		}

		ExpansionQuestNPCData questNPCData = GetQuestNPCDataByID(questNPCID);
		if (!questNPCData)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get quest npc data!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_RequestOpenQuestMenu");
		rpc.Write(questNPCID);
		rpc.Write(questNPCData.GetDefaultNPCText());
		rpc.Write(-1);
		rpc.Write(CF_Date.Now(true).GetTimestamp());
		rpc.Expansion_Send(target, true, identity);

	#ifdef EXPANSIONMODAI
		if (npcAI)
		{
			npcAI.eAI_AddInteractingPlayer(identity.GetPlayer());

			EmoteManager npcEmoteManager = npcAI.GetEmoteManager();
			if (!npcEmoteManager.IsEmotePlaying())
			{
				npcEmoteManager.PlayEmote(questNPCData.NPCInteractionEmoteID);
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
			}
		}
	#endif
	}

	//! Server
	void RequestOpenQuestMenuForQuest(PlayerIdentity identity, int questID = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_RequestOpenQuestMenu");
		rpc.Write(-1);
		rpc.Write("");
		rpc.Write(questID);
		rpc.Write(CF_Date.Now(true).GetTimestamp());
		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_RequestOpenQuestMenu(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		string displayName;
		if (target)
			displayName = target.GetDisplayName();

		int questNPCID;
		if (!ctx.Read(questNPCID))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read quest NPC ID!");
			return;
		}

		QuestModulePrint("Quest NPC ID: " + questNPCID);

		string defaultText;
		if (!ctx.Read(defaultText))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not read default text!");
			return;
		}

		QuestModulePrint("Default text: " + defaultText);

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not get questID!");
			return;
		}

		int serverTime;
		if (!ctx.Read(serverTime))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not get serverTime!");
			return;
		}

		QuestModulePrint("Quest ID: " + questID);
		
		//! Open quest menu if no other menu is opened
		if (!GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu() && !GetGame().GetUIManager().GetMenu())
		{
			GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionQuestMenu");
			//! Populate quest menu with needed client data.
			m_QuestMenuInvoker.Invoke(displayName, defaultText, questNPCID, questID, serverTime);
		}
	}

	//! Client
	void ExitQuestMenu(int questNPCID)
	{
	#ifdef EXPANSIONMODAI
		auto npc = GetQuestNPCAIByID(questNPCID);
		if (npc)
		{
			auto rpc = Expansion_CreateRPC("RPC_ExitQuestMenu");
			rpc.Expansion_Send(npc, true);
		}
	#endif
	}

	//! Server
	void RPC_ExitQuestMenu(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
	#ifdef EXPANSIONMODAI
		eAIBase npc;
		if (Class.CastTo(npc, target))
			npc.eAI_RemoveInteractingPlayer(identity.GetPlayer());
	#endif
	}

	//! Client
	//! Request creation of selected quest from quest menu by the client and sends the request to the server.
	void RequestCreateQuestInstance(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_RequestCreateQuestInstance");
		rpc.Write(questID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestCreateQuestInstance(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestCreateQuestInstance - Could not get read quest ID!");
			return;
		}

		CreateQuestInstance(questID, identity);
	}

	//! Server
	//! Create new quest instance.
	void CreateQuestInstance(int questID, PlayerIdentity sender)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		//! Get quest config instance for our new quest instance.
		ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
		if (!configInstance)
		{
			Error(ToString() + "::CreateQuestInstance - Could not get config data for quest with ID: " + questID);
			return;
		}

		if (!MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_CanStartQuest(configInstance, sender))
			return;

		string playerUID = sender.GetId();

		//! Create new quest instance.
		ExpansionQuest newQuestInstance = new ExpansionQuest(this, configInstance);
		string questInstanceKey;

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

			questInstanceKey = playerUID;
		}
		else
		{
			QuestModulePrint("Quest is a group quest. Apply needed parameters!");

			PlayerBase player = PlayerBase.Cast(sender.GetPlayer());

			int groupID;
			string ownerUID;
			TStringArray memberUIDs = {};
			if (!GetGroupData(player, groupID, ownerUID, memberUIDs))
			{
				newQuestInstance = null;
				//! Send notification that quest can only be accepted while in a group.
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotInGroupText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			QuestModulePrint("Group data of player with ID: " + playerUID);
			QuestModulePrint("Group ID: " + groupID);

			//! Only group members accept quest!
			auto settings = GetExpansionSettings().GetQuest();
			if (settings.GroupQuestMode < 1 && ownerUID != playerUID)
			{
				newQuestInstance = null;
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(sender);
				return;
			}

			//! If the player that is accepting the quest has a group we set the group owner as the main quest player
			newQuestInstance.SetPlayerUID(ownerUID);
			newQuestInstance.SetGroupID(groupID);

			//! We add the new quest data to the groups persistent quest data
			ExpansionQuestPersistentData groupQuestData = GetPlayerQuestDataByUID(groupID.ToString());
			if (!groupQuestData)
			{
				Error(ToString() + "::CreateQuestInstance - Could not get players persisten quest data! Member offline?");
				return;
			}

			groupQuestData.AddQuestData(questID, ExpansionQuestState.NONE);

			foreach (string memberUID: memberUIDs)
			{
				//! We add a data set to the persistent quest data of each group member that can accept the quest.
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!memberQuestData)
					continue;

				PlayerBase memberPB = PlayerBase.GetPlayerByUID(memberUID);
				if (!memberPB)
					continue;

				if (QuestDisplayConditions(configInstance, memberPB, memberQuestData, -1, true))
				{
					memberQuestData.AddQuestData(questID, ExpansionQuestState.NONE);
					newQuestInstance.AddGroupMember(memberUID);
				}
			}

			questInstanceKey = groupID.ToString();
		}

		newQuestInstance.Finalize();

		AddActiveQuest(questInstanceKey, newQuestInstance);

		if (!newQuestInstance.OnQuestStart())
		{
			QuestModulePrint("Quest start event failed for quest with quest id: " + questID);
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

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CreateQuestInstance - Created new quest instance for with instance key " + questInstanceKey + " for quest " + questID);
	}

	//! 3rd party modding support helper function
	bool GetGroupData(PlayerBase player, out int groupID, out string ownerUID, out TStringArray memberUIDs = null)
	{
	#ifdef EXPANSIONMODGROUPS
		ExpansionPartyData groupData = player.Expansion_GetParty();
		if (!groupData)
			return false;

		groupID = groupData.GetPartyID();
		ownerUID = groupData.GetOwnerUID();

		if (memberUIDs)
		{
			array<ref ExpansionPartyPlayerData> groupMembers = groupData.GetPlayers();
			foreach (ExpansionPartyPlayerData member: groupMembers)
			{
				memberUIDs.Insert(member.GetID());
			}
		}

		return true;
	#endif

		return false;
	}

	//! Server
	void CreateClientMarker(vector pos, string text, int questID, PlayerIdentity identity, int objectiveIndex, int visibility = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (visibility < 0)
			visibility = EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP;

		auto rpc = Expansion_CreateRPC("RPC_CreateClientMarker");
		rpc.Write(pos);
		rpc.Write(text);
		rpc.Write(questID);
		rpc.Write(CanCreate3DMarker());
		rpc.Write(objectiveIndex);
		rpc.Write(visibility);
		rpc.Expansion_Send(true, identity);
	}

	//! 3rd party modding support helper function
	bool CanCreate3DMarker()
	{
	#ifdef EXPANSIONMODNAVIGATION
		return GetExpansionSettings().GetMap().CanCreate3DMarker;
	#endif

		return true;
	}

	//! Client
	protected void RPC_CreateClientMarker(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		int visibility;
		if (!ctx.Read(visibility))
			return;

		CreateMarkerClient(pos, text, questID, is3D, objectiveIndex, visibility);
	}

	/**
	 * @brief Check if marker exist for quest and optional objective index.
	 * 
	 * @param questID Quest ID
	 * @param objectiveIndex Objective index, -1 to check for existence of turn-in marker
	 * 
	 * @note always does exact match
	 * 
	 * @return True if marker exists, false if not or error.
	 */
	bool HasQuestMarker(int questID, int objectiveIndex)
	{
	#ifdef EXPANSIONMODNAVIGATION
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestModulePrint("Quest ID: " + questID + " | Objective index: " + objectiveIndex);

		ExpansionMarkerModule markerModule;
		Class.CastTo(markerModule, CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		ExpansionMarkerClientData clientMarkerData = markerModule.GetData();
		if (!clientMarkerData)
		{
			Error(ToString() + "::HasQuestMarker - Could not get client marker data!");
			return false;
		}

		QuestModulePrint("Client marker data: " + clientMarkerData.ToString());
		QuestModulePrint("Client personal markers data count: " + clientMarkerData.m_PersonalMarkers.Count());

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
				QuestModulePrint("Marker UID: " + uid + " Name: " + markerData.GetName());

				if (uid == search)
				{
					QuestModulePrint("End and return TRUE");
					return true;
				}
			}
		}
		QuestModulePrint("End and return FALSE");
	#endif

		return false;
	}

	//! Client
	protected void CreateMarkerClient(vector pos, string text, int questID, bool is3D, int objectiveIndex, int visibility)
	{
	#ifdef EXPANSIONMODNAVIGATION
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestModulePrint("Quest ID: " + questID + " | Objective index: " + objectiveIndex + " | Position: " + pos + " | Text: " + text);

		if (HasQuestMarker(questID, objectiveIndex))
			return;

		ExpansionMarkerModule markerModule;
		Class.CastTo(markerModule, CF_ModuleCoreManager.Get(ExpansionMarkerModule));

		bool showMap = false;
		if (is3D && (visibility & EXPANSION_MARKER_VIS_WORLD) == 0)
			is3D = false;

		pos[1] = pos[1] + 2.0;

		string uid = "Q" + questID + "_";
		if (objectiveIndex > -1)
		{
			uid += "" + objectiveIndex;
		}

		QuestModulePrint("Set marker UID: " + uid);

		ExpansionMarkerData markerData = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL, uid, false);
		markerData.SetName(text);
		markerData.SetIcon("Questionmark");
		markerData.SetColor(ARGB(255,241,196,15));
		markerData.SetPosition(pos);
		markerData.Set3D(is3D);
		markerData.ApplyVisibility(visibility);
		markerData.SetLockState(true);
		markerModule.CreateMarker(markerData);

		string markerUID = markerData.GetUID();
		QuestModulePrint("ExpansionMarkerModule::CreateMarkerClient - Create marker with UID: " + markerUID + " Name: " + markerData.GetName() + " for quest " + questID);
	#endif
	}

	//! Server
	void RemoveClientMarkers(int questID, PlayerIdentity identity, int objectiveIndex)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_RemoveClientMarkers");
		rpc.Write(questID);
		rpc.Write(objectiveIndex);
		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_RemoveClientMarkers(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID;
		if (!ctx.Read(questID))
			return;

		int objectiveIndex;
		if (!ctx.Read(objectiveIndex))
			return;

		RemoveMarkersClient(questID, objectiveIndex);
	}

	/**
	 * @brief Remove marker on client
	 * 
	 * @param questID Quest ID
	 * @param objectiveIndex Objective index, -1 to remove only turn-in marker, -2 to also remove objective markers
	 */
	protected void RemoveMarkersClient(int questID, int objectiveIndex)
	{
	#ifdef EXPANSIONMODNAVIGATION
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestModulePrint("Quest ID: " + questID + " || Objective index: " + objectiveIndex);

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

		QuestModulePrint("Client marker data: " + clientMarkerData.ToString());
		QuestModulePrint("Client personal markers data count: " + clientMarkerData.m_PersonalMarkers.Count());

		string search = "Q" + questID + "_";
		bool exactMatch = true;
		if (objectiveIndex > -1)
		{
			search += "" + objectiveIndex;
		}
		else if (objectiveIndex == -2)
		{
			exactMatch = false;
		}

		for (int i = clientMarkerData.m_PersonalMarkers.Count() - 1; i >= 0; i--)
		{
			ExpansionMarkerData markerData = clientMarkerData.m_PersonalMarkers[i];
			if (markerData)
			{
				string uid = markerData.GetUID();
				QuestModulePrint("Marker UID: " + uid + " Name: " + markerData.GetName());

				if ((exactMatch && uid == search) || (!exactMatch && uid.IndexOf(search) == 0))
				{
					QuestModulePrint("Delete marker!");
					markerModule.RemovePersonalMarkerByUID(uid);
					if (exactMatch)
						return;
				}
			}
		}
	#endif
	}

	//! Client
	//! Request from client to server to cancel a quest on the server .
	//! update players quest data on the client after it has been changed
	void CancelQuest(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_CancelQuest");
		rpc.Write(questID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_CancelQuest(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_CancelQuest - Could not read quest ID!");
			return;
		}

		ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
		if (!questConfig)
		{
			Error(ToString() + "::RPC_CancelQuest - Could not get quest configuration for quest with ID: " + questID);
			return;
		}

		string playerUID = identity.GetId();
		string instanceKey = playerUID;

		if (questConfig.IsGroupQuest())
		{
			PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
			if (player)
			{
				int groupID = GetGroupID(player);
				if (groupID == -1)
				{
					Error(ToString() + "::RPC_CancelQuest - Could get valid group ID from player with UID: " + playerUID + " | Group ID: " + instanceKey);
					return;
				}

				instanceKey = groupID.ToString();
			}
		}

		ExpansionQuest quest = GetActiveQuestWithKey(instanceKey, questID);
		if (!quest)
		{
			Error(ToString() + "::CancelQuestServer - No active quest with ID " + questID + " for instance key " + instanceKey);
			return;
		}

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

		CancelQuestServer(quest);
		//! Log message about quest cancelation
		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CancelQuestServer - Player with UID " + identity.GetId() + " has cancelled quest " + questID);
	}

	//! Server
	//! Only called when quest is cancelled by a quest player or by an internal quest event.
	void CancelQuestServer(ExpansionQuest quest)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		//! Call custom cancel event for override in init.c
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestCancel(quest);

		//! Reset the quest timestamp in the persisten quest data of the quest players.
		//! save = false because OnCancel quest will process and save the quest data of all quest players afterwards.
		UpdateQuestTimestampForQuestPlayers(quest, 0, false);

		//! Call the OnQuestCancel on the active quest instance.
		//! Sends update of quest data to all quest players also.
		if (!quest.OnQuestCancel())
		{
			int questID = quest.GetQuestConfig().GetID();
			QuestModulePrint("Something went wrong in the cancel quest event for quest with ID: " + questID);
		}

		string instanceKey = quest.GetPlayerUID();
		if (quest.GetQuestConfig().IsGroupQuest())
			instanceKey = quest.GetGroupID().ToString();

		//! cleanup = false because OnQuestCancel already called OnQuestCleanup
		CleanupActiveQuest(instanceKey, quest, false, ExpansionQuestModuleCallback.CANCELED_QUEST);
	}

	//! Server
	protected void SendCallbackToClient(int callback, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_CallbackClient");
		rpc.Write(callback);
		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_CallbackClient(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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
				QuestModulePrint("CANCELED_QUEST");
				m_QuestMenuCallbackInvoker.Invoke();
				break;
			}
		}
	}

	//! Client
	//! Gets called when a player truns in a certain quest
	void RequestTurnInQuestClient(int questID, bool selectReward = false, ExpansionQuestRewardConfig selectedReward = null, int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_ClientQuestData)
		{
			Error(ToString() + "::RequestTurnInQuestClient - Could not get players client quest data!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_RequestCompleteQuest");
		rpc.Write(questID);
		rpc.Write(selectReward);
		if (selectReward)
			selectedReward.OnSend(rpc);
		rpc.Write(selectedObjItemIndex);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestCompleteQuest(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		RequestCompleteQuestServer(questID, playerUID, identity, false, reward, selectedObjItemIndex);
	}

	//! Server
	void RequestCompleteQuestServer(int questID, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
		if (!questConfig)
		{
			Error(ToString() + "::RequestCompleteQuestServer - Could not get quest configuration for quest with ID: " + questID);
			return;
		}

		string instanceKey = playerUID;
		if (questConfig.IsGroupQuest())
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
			if (player)
			{
				int groupID = GetGroupID(player);
				if (groupID == -1)
				{
					Error(ToString() + "::RequestCompleteQuestServer - Could get valid group ID from player with UID: " + playerUID + " | Group ID: " + instanceKey);
					return;
				}

				instanceKey = groupID.ToString();
			}
		}

		ExpansionQuest quest = GetActiveQuestWithKey(instanceKey, questID);
		if (!quest)
		{
			Error(ToString() + "::RequestCompleteQuestServer - Could find valid quest instance for quest with ID: " + questID + " | Key: " + instanceKey);
			return;
		}

		if (questConfig.IsGroupQuest())
		{
			auto settings = GetExpansionSettings().GetQuest();
			if (settings.GroupQuestMode <= 1 && quest.GetPlayerUID() != playerUID)
			{
				ExpansionNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestNotGroupOwnerText), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
				return;
			}
		}

		if (MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_CanCompleteQuest(quest, identity))
		{
			CompleteQuest(quest, playerUID, identity, isAutoComplete, reward, selectedObjItemIndex);
			return;
		}
	}

	//! Server
	protected void CompleteQuest(ExpansionQuest quest, string playerUID, PlayerIdentity identity, bool isAutoComplete = false, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!quest)
		{
			Error(ToString() + "::CompleteQuest - Could not get active quest!");
			return;
		}

		if (!quest.OnQuestTurnIn(playerUID, reward, selectedObjItemIndex))
		{
			QuestModulePrint("Quest turn-in event failed for quest with quest id: " + quest.GetQuestConfig().GetID());
			ExpansionNotification(new StringLocaliser("Quest turn-In failed!"), new StringLocaliser("Something went wrong.."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
			return;
		}
		
		//! Check if we should show the quest log with the follow-up quest on quest completion.
		//! We only want to display the quest log with the follow-up quest when the follow-up quest has no quest giver and when the player meats all other quest display conditions for that quest.
		ExpansionQuestConfig questConfig = quest.GetQuestConfig();
		int followupQuestID = questConfig.GetFollowUpQuestID();
		if (followupQuestID > 0 && !questConfig.SuppressQuestLogOnCompetion())
		{
			ExpansionQuestConfig followUpQuest = GetQuestConfigByID(followupQuestID);
			array<int> questGivers = followUpQuest.GetQuestGiverIDs();
			if (followUpQuest && questGivers.Count() == 0)
			{
				ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
				if (playerQuestData)
				{
					PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(identity);
					//! We set to skip the pre-quest conditions check here as the player quest data does not contain the completion of the current quest yet and the QuestDisplayConditions check would return false otherwise then.
					if (player && QuestDisplayConditions(followUpQuest, player, playerQuestData, -1, false, true))
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(RequestOpenQuestMenuForQuest, 1000, false, identity, followUpQuest.GetID());
				}
			}
		}

		if (quest.GetQuestConfig().IsDailyQuest() || quest.GetQuestConfig().IsWeeklyQuest())
		{
			int weeklyResetDay = CF_Date.GetDayByName(GetExpansionSettings().GetQuest().WeeklyResetDay);
			int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyResetHour;
			int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyResetMinute;
			int dailyResetHour = GetExpansionSettings().GetQuest().DailyResetHour;
			int dailyResetMinute = GetExpansionSettings().GetQuest().DailyResetMinute;

			bool useUTC = GetExpansionSettings().GetQuest().UseUTCTime;
			CF_Date now = CF_Date.Now(useUTC);

			int localToUTC;
			if (!useUTC)
			{
				//! Convert local time to UTC
				CF_Date nowUTC = CF_Date.Now(true);
				localToUTC = nowUTC.GetTimestamp() - now.GetTimestamp();
				QuestModulePrint("Local time to UTC (seconds): " + localToUTC);
			}

			int days;
			int hours;
			int minutes;

			if (quest.GetQuestConfig().IsDailyQuest())
			{
				QuestModulePrint("It is: " + now.GetDayName() + " | " + now.GetHours() + ":" + now.GetMinutes());
				QuestModulePrint("Reset is at: " + now.GetDayName() + " | " + dailyResetHour + ":" + dailyResetMinute);
				if (now.GetHours() >= dailyResetHour && now.GetMinutes() >= dailyResetMinute)
				{
					days = 1;
					QuestModulePrint("Completed quest after reset! Add cooldown for next day!");
				}

				hours = dailyResetHour;
				minutes = dailyResetMinute;
			}
			else if (quest.GetQuestConfig().IsWeeklyQuest())
			{
				QuestModulePrint("It is: " + now.GetDayName() + " | " + now.GetHours() + ":" + now.GetMinutes());
				QuestModulePrint("Reset is at: " + GetExpansionSettings().GetQuest().WeeklyResetDay + " | " + weeklyResetHour + ":" + weeklyResetMinute);
				if (now.GetDayName() == GetExpansionSettings().GetQuest().WeeklyResetDay)
				{
					if (now.GetHours() >= weeklyResetHour && now.GetMinutes() >= weeklyResetMinute)
					{
						days = now.GetDaysToNextWeekDay(weeklyResetDay);
						QuestModulePrint("Completed quest after reset! Add cooldown for next week!");
					}
				}
				else
				{
					days = now.GetDaysToNextWeekDay(weeklyResetDay);
					QuestModulePrint("Completed quest after reset! Add cooldown for next week!");
				}

				hours = weeklyResetHour;
				minutes = weeklyResetMinute;
				QuestModulePrint("Days to next weekly reset: " + days);
			}

			now.SetHours(0);
			now.SetMinutes(0);
			now.SetSeconds(0);

			now.EpochToDate(now.GetTimestamp() + days * 86400 + hours * 3600 + minutes * 60 + localToUTC);

			QuestModulePrint("Reset is on (UTC): " + now.Format(CF_Date.DATETIME));

			UpdateQuestTimestampForQuestPlayers(quest, now.GetTimestamp());
		}

		UpdateQuestCompletionCountForQuestPlayers(quest);
		UpdateQuestForQuestPlayers(quest, true);

		RemoveClientMarkers(quest.GetQuestConfig().GetID(), identity, -1);

	#ifdef EXPANSIONMODAI
		if (quest.GetQuestConfig().GetQuestTurnInIDs().Count())
		{
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
		}
	#endif

		if (!quest.IsCompleted())
			return;

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			CleanupActiveQuest(playerUID, quest);
		}
		else
		{
			int groupID = quest.GetGroupID();
			CleanupActiveQuest(groupID.ToString(), quest);
		}

		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - CompleteQuest - Player with UID " + identity.GetId() + " has completed quest " + quest.GetQuestConfig().GetID());
	}

	//! Client
	void RequestShareQuest(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto rpc = Expansion_CreateRPC("RPC_RequestShareQuest");
		rpc.Write(questID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestShareQuest(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestShareQuest - Could not get read quest ID!");
			return;
		}

		ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);
		if (!configInstance)
		{
			Error(ToString() + "::RPC_RequestShareQuest - Could not get config data for quest with ID: " + questID);
			return;
		}

		ShareQuestWithGroup(identity, configInstance);
	}

	//! Server
	protected void ShareQuestWithGroup(PlayerIdentity identity, ExpansionQuestConfig config)
	{
	#ifdef EXPANSIONMODGROUPS
		PlayerBase player = PlayerBase.Cast(identity.GetPlayer());
		if (!player)
		{
			Error(ToString() + "::ShareQuestWithGroup - Could not get player entity!");
			return;
		}

		ExpansionPartyData groupData = player.Expansion_GetParty();
		if (!groupData)
		{
			Error(ToString() + "::ShareQuestWithGroup - Could not get players group data!");
			return;
		}

		array<ref ExpansionPartyPlayerData> groupMembers = groupData.GetPlayers();
		foreach (ExpansionPartyPlayerData member: groupMembers)
		{
			string memberUID = member.GetID();

			ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
			if (!memberQuestData)
				continue;

			ExpansionQuestState questState = memberQuestData.GetQuestStateByQuestID(config.GetID());
			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.COMPLETED)
				continue;

			PlayerBase memberPB = PlayerBase.GetPlayerByUID(memberUID);
			if (!memberPB)
				continue;

			if (QuestDisplayConditions(config, memberPB, memberQuestData, -1, true))
				RequestOpenQuestMenuForQuest(memberPB.GetIdentity(), config.GetID());
		}
	#endif
	}

	// ----------------------------------------------------------------------------------------------------------------------
	//! Default quest data
	// ----------------------------------------------------------------------------------------------------------------------
	protected void DefaultQuestNPCData()
	{
		//! Quest NPC #1
		ExpansionQuestNPCData questNPC_1 = new ExpansionQuestNPCData();
		questNPC_1 = m_DefaultQuestNPCData.QuestNPCData_1();
		questNPC_1.Save("QuestNPC_1");
		m_QuestsNPCs.Insert(1, questNPC_1);

		//! Quest NPC #2
		ExpansionQuestNPCData questNPC_2 = new ExpansionQuestNPCData();
		questNPC_2 = m_DefaultQuestNPCData.QuestNPCData_2();
		questNPC_2.Save("QuestNPC_2");
		m_QuestsNPCs.Insert(2, questNPC_2);

		//! Quest NPC #3
		ExpansionQuestNPCData questNPC_3 = new ExpansionQuestNPCData();
		questNPC_3 = m_DefaultQuestNPCData.QuestNPCData_3();
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
		
		ExpansionQuestObjectiveTravelConfig objective_t_5 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_005();
		objective_t_5.Save("Objective_T_5");
		m_TravelObjectivesConfigs.Insert(5, objective_t_5);
		
		ExpansionQuestObjectiveTravelConfig objective_t_6 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_006();
		objective_t_6.Save("Objective_T_6");
		m_TravelObjectivesConfigs.Insert(6, objective_t_6);
		
		ExpansionQuestObjectiveTravelConfig objective_t_7 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_007();
		objective_t_7.Save("Objective_T_7");
		m_TravelObjectivesConfigs.Insert(7, objective_t_7);
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
		
		//! Quest #22 - Expample template for a quest chain - Part 1
		ExpansionQuestConfig quest_22 = m_DefaultQuestConfigData.ExpansionQuestConfig022();
		quest_22.Save("Quest_22");
		m_QuestConfigs.Insert(22, quest_22);
		
		//! Quest #23 - Expample template for a quest chain - Part 2
		ExpansionQuestConfig quest_23 = m_DefaultQuestConfigData.ExpansionQuestConfig023();
		quest_23.Save("Quest_23");
		m_QuestConfigs.Insert(23, quest_23);
		
		//! Quest #24 - Expample template for a quest chain - Part 3
		ExpansionQuestConfig quest_24 = m_DefaultQuestConfigData.ExpansionQuestConfig024();
		quest_24.Save("Quest_24");
		m_QuestConfigs.Insert(24, quest_24);
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
			switch (questNPCData.GetNPCType())
			{
				case ExpansionQuestNPCType.NORMAL:
				{
					ExpansionQuestNPCBase npc = questNPCData.SpawnNPC();
					if (!npc)
						return;

					npc.SetQuestNPCID(questNPCData.GetID());
					npc.SetQuestNPCData(questNPCData);
					npc.SetPosition(questNPCData.GetPosition());
					npc.SetOrientation(questNPCData.GetOrientation());
				}
				break;
				case ExpansionQuestNPCType.OBJECT:
				{
					ExpansionQuestStaticObject object = questNPCData.SpawnObject();
					if (!object)
						return;

					object.SetQuestNPCID(questNPCData.GetID());
					object.SetQuestNPCData(questNPCData);
					object.SetPosition(questNPCData.GetPosition());
					object.SetOrientation(questNPCData.GetOrientation());
				}
				break;
			#ifdef EXPANSIONMODAI
				case ExpansionQuestNPCType.AI:
				{
					ExpansionQuestNPCAIBase npcAI = questNPCData.SpawnNPCAI();
					if (!npcAI)
						return;

					npcAI.SetQuestNPCID(questNPCData.GetID());
					npcAI.SetQuestNPCData(questNPCData);
					npcAI.Expansion_SetEmote(questNPCData.GetEmoteID(), !questNPCData.IsEmoteStatic());
				}
				break;
			#endif
			}
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
		
		if (!objectiveBaseData.IsActive())
		{
			string objectiveTypeName = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);
			Print(ToString() + "::GetObjectiveData - Objective with ID " + objectiveID + " | Type: [" + objectiveType + "] " + objectiveTypeName + " is set to inactive. Skip..");
		}
		
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig = ExpansionQuestObjectiveTravelConfig.Load(fileName);
				if (!travelConfig)
					return;
				
				if (m_TravelObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add TRAVEL objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}
				
				m_TravelObjectivesConfigs.Insert(objectiveID, travelConfig);
			}
			break;
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = ExpansionQuestObjectiveDeliveryConfig.Load(fileName);
				if (!deliveryConfig)
					return;
				
				if (m_DeliveryObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add DELIVERY objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				deliveryConfig.CollectAllocationClasses();
				m_DeliveryObjectivesConfigs.Insert(objectiveID, deliveryConfig);
			}
			break;
			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = ExpansionQuestObjectiveTargetConfig.Load(fileName);
				if (!targetConfig)
					return;
				
				if (m_TargetObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add TARGET objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				targetConfig.CollectAllocationClasses();
				m_TargetObjectivesConfigs.Insert(objectiveID, targetConfig);
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = ExpansionQuestObjectiveCollectionConfig.Load(fileName);
				if (!collectionConfig)
					return;
				
				if (m_CollectionObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add COLLECT objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}
				
				collectionConfig.CollectAllocationClasses();
				m_CollectionObjectivesConfigs.Insert(objectiveID, collectionConfig);
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureConfig = ExpansionQuestObjectiveTreasureHuntConfig.Load(fileName);
				if (!treasureConfig)
					return;
				
				if (m_TreasureHuntObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add TREASUREHUNT objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				treasureConfig.CollectAllocationClasses();
				m_TreasureHuntObjectivesConfigs.Insert(objectiveID, treasureConfig);
			}
			break;
			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionConfig = ExpansionQuestObjectiveActionConfig.Load(fileName);
				if (!actionConfig)
					return;
				
				if (m_ActionObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add ACTION objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				actionConfig.CollectAllocationClasses();
				m_ActionObjectivesConfigs.Insert(objectiveID, actionConfig);
			}
			break;
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig = ExpansionQuestObjectiveCraftingConfig.Load(fileName);
				if (!craftingConfig)
					return;
				
				if (m_CraftingObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add CRAFTING objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

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
				
				if (m_AIPatrolObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add AIPATROL objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				aiPatrolConfig.CollectAllocationClasses();
				m_AIPatrolObjectivesConfigs.Insert(objectiveID, aiPatrolConfig);
			}
			break;
			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = ExpansionQuestObjectiveAICampConfig.Load(fileName);
				if (!aiCampConfig)
					return;
				
				if (m_AICampObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add AICAMP objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

				aiCampConfig.CollectAllocationClasses();
				m_AICampObjectivesConfigs.Insert(objectiveID, aiCampConfig);
			}
			break;
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig aiEscortConfig = ExpansionQuestObjectiveAIEscortConfig.Load(fileName);
				if (!aiEscortConfig)
					return;
				
				if (m_AIEscortObjectivesConfigs.Contains(objectiveID))
				{
					Error(ToString() + "::GetObjectiveData - Tryied to add AIESCORT objective type with ID " + objectiveID + " |File: " + fileName + ". ID is already used! Skip..");
					return;
				}

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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		foreach (ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (questConfig.GetObjectSetFileName() != string.Empty)
			{
				ExpansionQuestObjectSet questObjectSet = new ExpansionQuestObjectSet();
				questObjectSet.SetQuestID(questConfig.GetID());
				questObjectSet.SetObjectSetFileName(EXPANSION_QUESTS_OBJECTS_FOLDER, questConfig.GetObjectSetFileName());

				m_QuestObjectSets.Insert(questObjectSet);
				QuestModulePrint("Added objects set for quest ID [" + questConfig.GetID() + "]: " + questObjectSet.ToString());
			}
		}
	}

	void CheckAndSpawnObjectSet(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		foreach (ExpansionQuestObjectSet objectSet: m_QuestObjectSets)
		{
			if (objectSet.GetQuestID() == questID && !objectSet.IsSpawned())
			{
				objectSet.Spawn();
				QuestModulePrint("Spawned object set: " + objectSet.ToString());
			}
		}
	}

	void CheckAndDeleteObjectSet(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		foreach (ExpansionQuestObjectSet objectSet: m_QuestObjectSets)
		{
			if (objectSet.GetQuestID() == questID && objectSet.IsSpawned())
			{
				objectSet.Delete();
				QuestModulePrint("Deleted object set: " + objectSet.ToString());
			}
		}
	}

	void AddQuestNPCData(int id, ExpansionQuestNPCData questNPCData)
	{
		if (!m_QuestsNPCs.Contains(id))
			m_QuestsNPCs.Insert(id, questNPCData);
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
			if (!questNPCData.IsActive())
			{
				Print(ToString() + "::GetQuestNPCData - NPC with ID " + questNPCData.GetID() + " is set to inactive. Skip..");
			}
			
			QuestModulePrint("Adding quest npc data from file " + fileName);
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
			if (!questData.IsActive())
			{
				Print(ToString() + "::GetQuestData - Quest with ID " + questData.GetID() + " is set to inactive. Skip..");
				return;
			}
			
			QuestModulePrint("Try adding quest data from file: " + fileName);
			if (!questData.ValidateQuestConfiguration(fileName))
			{
				Error(ToString() + "::GetQuestData - Quest configuration validation failed! Quest was not added to the file system. Please check the quest configuration file: " + fileName);
			}
			
			if (m_QuestConfigs.Contains(questData.GetID()))
			{
				Error(ToString() + "::GetQuestData - Tryied to add Quest configuration with ID " + questData.GetID() + " but the ID is already used! Skip..");
				return;
			}
			
			foreach (ExpansionQuestObjectiveConfigBase objectiveBaseData: questData.Objectives)
			{
				int objectiveID = objectiveBaseData.GetID();
				int objectiveType = objectiveBaseData.GetObjectiveType();

				switch (objectiveType)
				{
					case ExpansionQuestObjectiveType.TRAVEL:
					{
						if (!m_TravelObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + ") uses a TRAVEL objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.DELIVERY:
					{
						if (!m_DeliveryObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a DELIVERY objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.TARGET:
					{
						if (!m_TargetObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a TARGET objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.COLLECT:
					{
						if (!m_CollectionObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a COLLECT objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.TREASUREHUNT:
					{
						if (!m_TreasureHuntObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a TREASUREHUNT objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.ACTION:
					{
						if (!m_ActionObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a ACTION objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.CRAFTING:
					{
						if (!m_CraftingObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a CRAFTING objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
				#ifdef EXPANSIONMODAI
					case ExpansionQuestObjectiveType.AIPATROL:
					{
						if (!m_AIPatrolObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a AIPATROL objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.AICAMP:
					{
						if (!m_AICampObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a AICAMP objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
					case ExpansionQuestObjectiveType.AIESCORT:
					{
						if (!m_AIEscortObjectivesConfigs.Contains(objectiveID))
						{
							Error(ToString() + "::GetQuestData - Quest config with ID " + questData.GetID() + " (File: " + fileName + " uses a AIESCORT objective ID " + objectiveID + " witch does not exist! Skip..");
							return;
						}
					}
					break;
				#endif
				}
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
			QuestModulePrint("Could not get quest data for player with UID: " + playerUID + ". Return TRUE");
			return true;
		}

		ExpansionQuestState questState = playerQuestData.GetQuestStateByQuestID(questID);
		int completionCount = playerQuestData.GetQuestCompletionCountByQuestID(questID);
		if (questState != ExpansionQuestState.COMPLETED && completionCount == 0)
		{
			QuestModulePrint("Player with UID: " + playerUID + " has never completed quest with ID " + questID + "! Return FALSE");
			return false;
		}

		QuestModulePrint("Player with UID: " + playerUID + " has completed quest with ID " + questID + " once! Return TRUE");
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
	ExpansionQuest GetActiveQuestWithKey(string instanceKey, int questID)
	{
		map<int, ref ExpansionQuest> activeQuests = GetActiveQuests(instanceKey);
		if (activeQuests)
		{
			ExpansionQuest activeQuestInstance = activeQuests[questID];
			if (activeQuestInstance)
			{
				if (!activeQuestInstance.GetQuestConfig().IsGroupQuest())
				{
					if (activeQuestInstance.IsQuestPlayer(instanceKey))
						return activeQuestInstance;
				}
				else
				{
					if (activeQuestInstance.GetGroupID().ToString() == instanceKey)
						return activeQuestInstance;
				}
			}
		}

		return null;
	}

	//! Server
	ExpansionQuest GetActiveQuestWithQuestID(PlayerBase player, int questID)
	{
		string playerUID = player.GetIdentity().GetId();
		int groupID = GetGroupID(player);
		ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
		if (questConfig)
		{
			if (!questConfig.IsGroupQuest())
			{
				return GetActiveQuestWithKey(playerUID, questID);
			}
			else if (questConfig.IsGroupQuest() && groupID > -1)
			{
				return GetActiveQuestWithKey(groupID.ToString(), questID);
			}
		}

		return null;
	}

	//! 3rd party modding support helper function
	int GetGroupID(PlayerBase player)
	{
	#ifdef EXPANSIONMODGROUPS
		return player.Expansion_GetPartyID();
	#endif
		return -1;
	}

	//! Server
	//! Handles reinitialisation of quests objectives for a player from his persistent quest data
	//! We need to get and handle the persistent objective data of the player so the progress of the quest objectives continues and no quest progress is lost.
	protected void GetObjectiveProgressFromQuestData(ExpansionQuestPersistentData playerData, ExpansionQuest quest)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestModulePrint("Quest ID: " + quest.GetQuestConfig().GetID());

		if (!playerData)
		{
			Error(ToString() + "::GetObjectiveProgressFromQuestData - Could not get player quest data!");
			return;
		}

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

			QuestModulePrint("Objective data type: " + objectiveType);
			QuestModulePrint("Objective data index: " + objectiveIndex);
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
			objective.SetCompleted(objectiveData.IsCompleted());

			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.ACTION:
				{
					QuestModulePrint("Objective event type: ACTION");
					ExpansionQuestObjectiveActionEvent action = ExpansionQuestObjectiveActionEvent.Cast(objective);
					if (action)
					{
						//! Get action state progress from persistent data.
						action.SetActionState(objectiveData.GetActionState());
						//! Get execution count progress from persistent data.
						action.SetExecutionCount(objectiveData.GetObjectiveCount());
						QuestModulePrint("Objective progress added: ACTION");
					}
				}
				break;
				case ExpansionQuestObjectiveType.TARGET:
				{
					QuestModulePrint("Objective event type: TARGET");
					ExpansionQuestObjectiveTargetEvent target = ExpansionQuestObjectiveTargetEvent.Cast(objective);
					if (target)
					{
						//! Get kill count progress from persistent data.
						target.SetCount(objectiveData.GetObjectiveCount());
						QuestModulePrint("Objective progress added: TARGET");
					}
				}
				break;
				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					QuestModulePrint("Objective event type: TREASUREHUNT");
					ExpansionQuestObjectiveTreasureHuntEvent treasureHunt = ExpansionQuestObjectiveTreasureHuntEvent.Cast(objective);
					if (treasureHunt)
					{
						//! Get destination check progress from persistent data.
						treasureHunt.SetLocationState(objectiveData.GetActionState());
						//! Set the previous position for the stash location from persistent data.
						treasureHunt.SetStashPosition(objectiveData.GetObjectivePosition());
						QuestModulePrint("Objective progress added: TREASUREHUNT");
					}
				}
				break;
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					QuestModulePrint("Objective event type: AIPATROL");
					ExpansionQuestObjectiveAIPatrolEvent aiPatrolHunt = ExpansionQuestObjectiveAIPatrolEvent.Cast(objective);
					if (aiPatrolHunt)
					{
						//! Get kill count progress from persistent data.
						aiPatrolHunt.SetKillCount(objectiveData.GetObjectiveCount());
						QuestModulePrint("Objective progress added: AIPATROL");
					}
				}
				break;
				case ExpansionQuestObjectiveType.AICAMP:
				{
					QuestModulePrint("Objective event type: AICAMP");
					ExpansionQuestObjectiveAICampEvent aiCamp = ExpansionQuestObjectiveAICampEvent.Cast(objective);
					if (aiCamp)
					{
						//! Get kill count progress from persistent data.
						aiCamp.SetKillCount(objectiveData.GetObjectiveCount());
						QuestModulePrint("Objective progress added: AICAMP");
					}
				}
				break;
				case ExpansionQuestObjectiveType.AIESCORT:
				{
					QuestModulePrint("Objective event type is: AIESCORT");
					ExpansionQuestObjectiveAIEscortEvent aiEscort = ExpansionQuestObjectiveAIEscortEvent.Cast(objective);
					if (aiEscort)
					{
						//! Get last known VIP entity position from persistent data.
						aiEscort.SetLastVIPPosition(objectiveData.GetObjectivePosition());
						QuestModulePrint("Objective progress added: AIESCORT");
					}
				}
				break;
			#endif
			}
		}
	}

	//! Server
	protected void CleanupPlayerQuests(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CleanupPlayerQuests - Tryed to call CleanupPlayerQuests on Client!");
			return;
		}

		QuestModulePrint("Look for quests of player with UID [" + playerUID + "]");

		map<int, ref ExpansionQuest> activeQuests = m_ActiveQuests[playerUID];

		if (!activeQuests)
			return;

		array<ref ExpansionQuest> questsToRemove = {};

		foreach (ExpansionQuest quest: activeQuests)
		{
			questsToRemove.Insert(quest);
		}

		foreach (ExpansionQuest questToRemove: questsToRemove)
		{
			//! Dont delete the quest if its a group quest and other group member(s) still online
			if (questToRemove.GetQuestConfig().IsGroupQuest() && questToRemove.IsOtherGroupMemberOnline())
			{
				QuestModulePrint("Quest is a group quest and other group players are still online! Skip quest deletion!");
				RemoveActiveQuest(playerUID, questToRemove);
				continue;
			}

			QuestModulePrint("Quest " + questToRemove.GetQuestConfig().GetID() + " is a quest of player with UID [" + playerUID + "] Delete quest!");
			CleanupActiveQuest(playerUID, questToRemove);
			QuestModulePrint("Quest deleted!");
		}
	}

	//! Server
	//! Updates quest objective data in given quest data.
	void UpdateQuestObjectiveData(ExpansionQuest quest, ExpansionQuestPersistentData questData)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		array<ref ExpansionQuestObjectiveEventBase> questObjectiveEvents = quest.GetObjectives();
		if (!questObjectiveEvents || questObjectiveEvents.Count() == 0)
			return;

		foreach (ExpansionQuestObjectiveEventBase currentObjective: questObjectiveEvents)
		{
			QuestModulePrint("Try update from objective event base: " + currentObjective.ToString());
			ExpansionQuestObjectiveData objectiveData = questData.GetQuestObjectiveByQuestIDAndIndex(quest.GetQuestConfig().GetID(), currentObjective.GetIndex());
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
					QuestModulePrint("Objective event type is: TARGET");
					ExpansionQuestObjectiveTargetEvent targetObjective;
					if (Class.CastTo(targetObjective, currentObjective))
					{
						objectiveData.SetObjectiveAmount(targetObjective.GetAmount());
						objectiveData.SetObjectiveCount(targetObjective.GetCount());
					}
				}
				break;
				case ExpansionQuestObjectiveType.TRAVEL:
				{
					QuestModulePrint("Objective event type is: TRAVEL");
					ExpansionQuestObjectiveTravelEvent travelObjective;
					if (Class.CastTo(travelObjective, currentObjective))
					{
						QuestModulePrint("Objective position is: " + travelObjective.GetPosition());
						QuestModulePrint("Objective data position is: " + objectiveData.GetObjectivePosition());
						objectiveData.SetObjectivePosition(travelObjective.GetPosition());
					}
				}
				break;
				case ExpansionQuestObjectiveType.COLLECT:
				{
					QuestModulePrint("Objective event type is: COLLECT");
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

						objectiveData.SetObjectivePosition(npcPos);

						array<ref ExpansionQuestDeliveryObjectiveData> collectionData = collectionObjective.GetDeliveryData();
						QuestModulePrint("Set collection data: " + collectionData.ToString() + " | Count: " + collectionData.Count());
						objectiveData.SetDeliveries(collectionData);
					}
				}
				break;
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					QuestModulePrint("Objective event type is: DELIVERY");
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

						objectiveData.SetObjectivePosition(npcPos);

						array<ref ExpansionQuestDeliveryObjectiveData> deliveryData = deliveryObjective.GetDeliveryData();
						QuestModulePrint("Set delivery data: " + deliveryData.ToString() + " | Count: " + deliveryData.Count());
						objectiveData.SetDeliveries(deliveryData);
					}
				}
				break;
				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					QuestModulePrint("Objective event type is: TREASUREHUNT");
					ExpansionQuestObjectiveTreasureHuntEvent treasureObjective;
					if (Class.CastTo(treasureObjective, currentObjective))
					{
						objectiveData.SetObjectivePosition(treasureObjective.GetPosition());
					}
				}
				break;
				case ExpansionQuestObjectiveType.ACTION:
				{
					QuestModulePrint("Objective event type is: ACTION");
					ExpansionQuestObjectiveActionEvent actionObjective;
					if (Class.CastTo(actionObjective, currentObjective))
					{
						objectiveData.SetActionState(actionObjective.GetActionState());
						objectiveData.SetObjectiveAmount(actionObjective.GetExecutionAmount());
						objectiveData.SetObjectiveCount(actionObjective.GetExecutionCount());
					}
				}
				break;
				case ExpansionQuestObjectiveType.CRAFTING:
				{
					QuestModulePrint("Objective event type is: CRAFTING");
					ExpansionQuestObjectiveCraftingEvent craftingObjective;
					if (Class.CastTo(craftingObjective, currentObjective))
					{
						objectiveData.SetActionState(craftingObjective.GetCraftingState());
						objectiveData.SetObjectiveAmount(craftingObjective.GetObjectiveItemsAmount());
						objectiveData.SetObjectiveCount(craftingObjective.GetObjectiveItemsCount());
					}
				}
				break;
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AICAMP:
				{
					QuestModulePrint("Objective event type is: AICAMP");
					ExpansionQuestObjectiveAICampEvent aiCampObjective;
					if (Class.CastTo(aiCampObjective, currentObjective))
					{
						objectiveData.SetObjectiveAmount(aiCampObjective.GetAmount());
						objectiveData.SetObjectiveCount(aiCampObjective.GetCount());
					}
				}
				break;
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					QuestModulePrint("Objective event type is: AIPATROL");
					ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
					if (Class.CastTo(aiPatrolObjective, currentObjective))
					{
						objectiveData.SetObjectiveAmount(aiPatrolObjective.GetAmount());
						objectiveData.SetObjectiveCount(aiPatrolObjective.GetCount());
					}
				}
				break;
				case ExpansionQuestObjectiveType.AIESCORT:
				{
					QuestModulePrint("Objective event type is: AIESCORT");
					ExpansionQuestObjectiveAIEscortEvent aiEscortObjective;
					if (Class.CastTo(aiEscortObjective, currentObjective))
					{
						objectiveData.SetObjectivePosition(aiEscortObjective.GetVIPPosition());
					}
				}
				break;
			#endif
			}

			objectiveData.QuestDebug();

			if (!questData.UpdateObjective(quest.GetQuestConfig().GetID(), objectiveData.GetObjectiveIndex(), objectiveData))
			{
				Error(ToString() + "::UpdateQuestObjectiveData - Something went wrong when updateing the quest objective data. Key: " + questData.m_FileName + " | Quest ID: " + quest.GetQuestConfig().GetID());
			}

			ExpansionQuestPersistentQuestData persistentQuestData = questData.GetQuestDataByQuestID(quest.GetQuestConfig().GetID());
			if (persistentQuestData)
				persistentQuestData.UpdateLastUpdateTime();

			QuestModulePrint("Added new objective data for type: " + objectiveType);
		}
	}

	//! Server
	//! Updates quest objectives and the quest state data for all quest clients on the server and syncs that data to that clients afterwards.
	void UpdateQuestForQuestPlayers(ExpansionQuest quest, bool updateQuestState)
	{
		string questState = typename.EnumToString(ExpansionQuestState, quest.GetQuestState());
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Quest ID: " + quest.GetQuestConfig().GetID(), "State: " + questState);

		int questID = quest.GetQuestConfig().GetID();
		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			string playerUID = quest.GetPlayerUID();
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for player with UID: " + playerUID);
				return;
			}

			UpdateQuestData(quest, playerQuestData, true);
			SaveAndSyncQuestData(playerQuestData, playerUID, questID);
			QuestModulePrint("::UpdateQuestForQuestPlayers - Updated and saved persistent player quest data for player with UID: " + playerUID + " | Quest ID: " + questID);
		}
		else
		{
			ExpansionQuestPersistentData groupQuestData = GetPlayerQuestDataByUID(quest.GetGroupID().ToString());
			if (!groupQuestData)
			{
				Error(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for group with ID: " + quest.GetGroupID());
				return;
			}

			int groupID = quest.GetGroupID();
			//! Update quest state and objective progress data in the persistent group quest data.
			UpdateQuestData(quest, groupQuestData, true);
			//! Check and save persistent group data changes after update.
			groupQuestData.Save(groupID.ToString(), EXPANSION_QUESTS_GROUPDATA_FOLDER);
			QuestModulePrint("::UpdateQuestForQuestPlayers - Updated and saved persistent group data for group with ID: " + groupID + " | Quest ID: " + questID);

			//! Update, save and send quest state and objective progress data for all group members.
			//! @Note: Skips objective progress data update for group quests as the player data does not need the objective progress data for group quests.
			set<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				if (!memberQuestData)
				{
					QuestModulePrint(ToString() + "::UpdateQuestForQuestPlayers - Could not get persistent quest data for group member with UID: " + memberUID + ". Member offline?!");
					continue;
				}

				UpdateQuestData(quest, memberQuestData, true);
				SaveAndSyncQuestData(memberQuestData, memberUID, questID);
				QuestModulePrint("::UpdateQuestForQuestPlayers - Updated and saved persistent player quest data for group member with UID: " + memberUID + " | Group ID: " + groupID + " | Quest ID: " + questID);
			}
		}
	}

	//! Server
	//! Update quest state and objective data on given persistent quest data with the progress from the given quest instance.
	void UpdateQuestData(ExpansionQuest quest, ExpansionQuestPersistentData questData, bool updateQuestState = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		//! Update quest state of the given questData.
		if (updateQuestState)
			UpdateQuestState(questData, quest);

		//! Update quest objective progress data of the given questData.
		UpdateQuestObjectiveData(quest, questData);
	}

	//! Server
	//! Save and sync given persistent quest data to player with given uid.
	//! questID is used to replace the quest data for the given ID on client side.
	void SaveAndSyncQuestData(ExpansionQuestPersistentData questData, string uid, int questID, bool sendConfigs = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (ExpansionInhibitor.Contains(this))
		{
			QuestModulePrint("::SaveAndSyncQuestData - SKIPPING persistent player quest data save and sync!");
			return;
		}

		//! Check and save persistent player data changes.
		questData.Save(uid, EXPANSION_QUESTS_PLAYERDATA_FOLDER);

		//! Send persistent quest player data from the server to the client.
		PlayerBase player = PlayerBase.GetPlayerByUID(uid);
		if (!player || !player.GetIdentity())
		{
			QuestModulePrint("::SaveAndSyncQuestData - Could not get player with UID: " + uid);
			return;
		}

		SendClientQuestData(questData, player.GetIdentity(), sendConfigs, questID);
	}

	//! Server
	//! Updates quest state on given persistent quest data with the given quest state.
	protected void UpdateQuestState(ExpansionQuestPersistentData questData, ExpansionQuest quest, ExpansionQuestState state = ExpansionQuestState.INVALID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID = quest.GetQuestConfig().GetID();
		ExpansionQuestState questState = quest.GetQuestState();

		//! Update the quest state on the given quest data.
		if (state != ExpansionQuestState.INVALID)
			questState = state;

		if (questState == ExpansionQuestState.COMPLETED && quest.GetQuestConfig().IsRepeatable())
			questState = ExpansionQuestState.NONE;

		questData.UpdateQuestState(questID, questState);
	}

	//! Server
	//! Sets updated time data for all quest clients.
	protected void UpdateQuestTimestampForQuestPlayers(ExpansionQuest quest, int time, bool save = true)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				QuestModulePrint("Player quest data: " + playerQuestData.ToString());
				playerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				if (save)
					playerQuestData.Save(quest.GetPlayerUID(), EXPANSION_QUESTS_PLAYERDATA_FOLDER);
			}
		}
		else
		{
			string groupID = quest.GetGroupID().ToString();
			ExpansionQuestPersistentData groupQuestData = GetPlayerQuestDataByUID(groupID);
			if (groupQuestData)
			{
				QuestModulePrint("Group quest data: " + groupQuestData.ToString());
				groupQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				if (save)
					groupQuestData.Save(groupID, EXPANSION_QUESTS_GROUPDATA_FOLDER);
			}

			set<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				if (memberQuestData)
				{
					QuestModulePrint("Player quest data: " + memberQuestData.ToString());
					memberQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
					if (save)
						memberQuestData.Save(memberUID, EXPANSION_QUESTS_PLAYERDATA_FOLDER);
				}
			}
		}
	}

	//! Server
	//! Sets updated quest completion data for all quest clients.
	protected void UpdateQuestCompletionCountForQuestPlayers(ExpansionQuest quest)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			string playerUID = quest.GetPlayerUID();
			QuestModulePrint("Player UID: " + playerUID);
			ExpansionQuestPersistentData playerQuestData = GetPlayerQuestDataByUID(playerUID);
			if (playerQuestData)
			{
				QuestModulePrint("Player quest data: " + playerQuestData.ToString());
				playerQuestData.UpdateCompletionCount(quest.GetQuestConfig().GetID());
				playerQuestData.Save(playerUID, EXPANSION_QUESTS_PLAYERDATA_FOLDER);
			}
		}
		else
		{
			string groupID = quest.GetGroupID().ToString();
			ExpansionQuestPersistentData groupQuestData = GetPlayerQuestDataByUID(groupID);
			if (groupQuestData)
			{
				QuestModulePrint("Group quest data: " + groupQuestData.ToString());
				groupQuestData.UpdateCompletionCount(quest.GetQuestConfig().GetID());
				groupQuestData.Save(groupID, EXPANSION_QUESTS_GROUPDATA_FOLDER);
			}

			set<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				ExpansionQuestPersistentData memberQuestData = GetPlayerQuestDataByUID(memberUID);
				if (memberQuestData)
				{
					QuestModulePrint("Player quest data: " + memberQuestData.ToString());
					memberQuestData.UpdateCompletionCount(quest.GetQuestConfig().GetID());
					memberQuestData.Save(memberUID, EXPANSION_QUESTS_PLAYERDATA_FOLDER);
				}
			}
		}
	}

	static void QuestModulePrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		EXTrace.Print(EXTrace.QUESTS, s_ModuleInstance, text);
	#endif
	}

	//! Server
	protected void CleanupActiveQuest(string playerUID, ExpansionQuest quest, bool cleanup = true, int callback = ExpansionQuestModuleCallback.NONE)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (cleanup)
			quest.OnQuestCleanup(true);  //! needs to run BEFORE removal!

		if (!quest.GetQuestConfig().IsGroupQuest())
		{
			RemoveActiveQuestWithCallback(playerUID, quest, callback);
		}
		else
		{
			set<string> memberUIDs = quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				RemoveActiveQuestWithCallback(memberUID, quest, callback);
			}

			//! Deal with the case where playerUID may no longer be part of member UIDs because player may have left party already
			if (memberUIDs.Find(playerUID) == -1)
				RemoveActiveQuestWithCallback(playerUID, quest, callback);

			RemoveActiveQuest(quest.GetGroupID().ToString(), quest);
		}
	}

	//! Server
	protected void RemoveActiveQuestWithCallback(string playerUID, ExpansionQuest quest, int callback)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (callback)
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
			if (player && player.GetIdentity())
				SendCallbackToClient(callback, player.GetIdentity());
		}

		RemoveActiveQuest(playerUID, quest);
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

	//! Client
	ScriptInvoker GetQuestHUDCallbackSI()
	{
		return m_QuestHUDCallbackInvoker;
	}

	//! Server & Client
	ExpansionQuestConfig GetQuestConfigByID(int id)
	{
		return m_QuestConfigs[id];
	}

	//! Server & Client
	map<int, ref ExpansionQuestConfig> GetQuestConfigs()
	{
		return m_QuestConfigs;
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
	static ExpansionQuestNPCAIBase GetClosestQuestNPCAIByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCAIBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCAIBase npc = s_QuestNPCAIEntities.Get(npcID);
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
	bool QuestPatrolExists(int questID, out array<eAIQuestPatrol> patrols)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		array<eAIQuestPatrol> foundPatrols = {};
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			patrols = foundPatrols;
			QuestModulePrint("End and return TRUE");
			return true;
		}

		QuestModulePrint("End and return FALSE");
		return false;
	}

	//! Server
	void SetQuestPatrols(int questID, array<eAIQuestPatrol> patrols)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		int pIndex;
	#endif

		array<eAIQuestPatrol> foundPatrols = {};
		if (m_GlobalAIPatrols.Find(questID, foundPatrols))
		{
			QuestModulePrint("::SetQuestPatrols - Found to existing patrols for quest ID " + questID + " | Patrols: " + foundPatrols.ToString());
			//! Check if current patrol has been killed.
			int killedPatrolsCount;
			foreach (eAIQuestPatrol questPatrol: foundPatrols)
			{
				if (questPatrol.WasGroupDestroyed())
					killedPatrolsCount++;
			}

			QuestModulePrint("::SetQuestPatrols - Killed patrols for quest with ID " + questID + " | Killed: " + killedPatrolsCount);
			if (foundPatrols.Count() == killedPatrolsCount)
			{
				foundPatrols.Clear();
				foundPatrols = patrols;
				m_GlobalAIPatrols.Set(questID, foundPatrols);
				QuestModulePrint("::SetQuestPatrols - Replaced existing patrols for quest ID " + questID + " | Patrols: " + foundPatrols.ToString());
			#ifdef EXPANSIONMODQUESTSMODULEDEBUG
				pIndex = 0;
				foreach (eAIQuestPatrol paR: patrols)
				{
					pIndex++;
					QuestModulePrint("[" + pIndex + "] " + paR.ToString());
				}
			#endif
			}
		}
		else
		{
			m_GlobalAIPatrols.Insert(questID, patrols);
			QuestModulePrint("::SetQuestPatrols - Added new patrols for quest ID " + questID + " | Patrols: " + patrols.ToString());
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			pIndex = 0;
			foreach (eAIQuestPatrol paN: patrols)
			{
				pIndex++;
				QuestModulePrint("[" + pIndex + "] " + paN.ToString());
			}
		#endif
		}

		QuestModulePrint("----------------------------------------------------------");
	}

	//! Server
	//! @brief removes quest patrol from global array of quest patrols.
	//! Called from ExpansionQuestObjectiveAIEventBase::CleanupPatrol which deals with despawning AI if necessary.
	void RemoveQuestPatrol(int questID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		QuestModulePrint("::RemoveQuestPatrol - Removing existing patrols for quest ID " + questID + ".");
		m_GlobalAIPatrols.Remove(questID);
	}
#endif

	//! Server
	bool QuestTriggerExists(int questID, int objectiveType, int objectiveID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		array<ExpansionObjectiveTriggerBase> foundTriggers = new array<ExpansionObjectiveTriggerBase>;
		if (m_GlobalTriggers.Find(questID, foundTriggers))
		{
			if (foundTriggers && foundTriggers.Count() > 0)
			{
				foreach (ExpansionObjectiveTriggerBase trigger: foundTriggers)
				{
					if (trigger.GetObjectiveType() == objectiveType && trigger.GetObjectiveID() == objectiveID)
					{
						QuestModulePrint("End and return TRUE");
						return true;
					}
				}
			}
		}

		QuestModulePrint("End and return FALSE");
		return false;
	}

	//! Server
	void SetQuestTriggers(int questID, array<ExpansionObjectiveTriggerBase> triggers)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		array<ExpansionObjectiveTriggerBase> foundTriggers;
		if (m_GlobalTriggers.Find(questID, foundTriggers))
		{
			foreach (ExpansionObjectiveTriggerBase trigger: triggers)
			{
				foundTriggers.Insert(trigger);
			}

			m_GlobalTriggers.Set(questID, foundTriggers);
		}
		else
		{
			m_GlobalTriggers.Insert(questID, triggers);
		}
	}

	//! Server
	void RemoveObjectiveTrigger(int questID, int triggerType, int objectiveType, int objectiveID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		bool removed = false;
		array<ExpansionObjectiveTriggerBase> foundTriggers;
		if (m_GlobalTriggers.Find(questID, foundTriggers))
		{
			for (int i = 0; i < foundTriggers.Count(); i++)
			{
				ExpansionObjectiveTriggerBase trigger = foundTriggers[i];
				if (trigger.GetTriggerType() == triggerType && trigger.GetObjectiveType() == objectiveType && trigger.GetObjectiveID() == objectiveID)
				{
					foundTriggers.Remove(i);
					removed = true;
				}
			}
		}

	#ifdef DIAG
		if (!removed)
			EXPrint(ToString() + "::RemoveObjectiveTrigger - Could not find/remove objective trigger! Quest ID: " + questID + " | Trigger type: " + typename.EnumToString(ExpansionObjectiveTriggerType, triggerType) + " | Objective Type: " + typename.EnumToString(ExpansionQuestObjectiveType, objectiveType) + "| Objective ID: " + objectiveID);
	#endif
	}

#ifdef EXPANSIONMODAI
	static ExpansionQuestNPCAIBase GetClosestQuestAINPC(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCAIBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCAIBase npc = s_QuestNPCAIEntities.Get(npcID);
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

	static void AddQuestNPCAI(int id, ExpansionQuestNPCAIBase questNPCAI)
	{
		if (!s_QuestNPCAIEntities[id])  //! Can be NULL if object was deleted because it was no longer in network bubble
			s_QuestNPCAIEntities[id] = questNPCAI;

		if (!s_QuestNPCIndicatorStates.Contains(id))  //! Don't override if already set
			s_QuestNPCIndicatorStates[id] = ExpansionQuestIndicatorState.NOT_SET;
	}

	static void RemoveQuestNPCAI(int id)
	{
		if (s_QuestNPCAIEntities.Contains(id))
		{
			if (s_QuestNPCAIEntities.Get(id))
				GetGame().ObjectDelete(s_QuestNPCAIEntities.Get(id));

			s_QuestNPCAIEntities.Remove(id);
		}
	}

	static ExpansionQuestNPCAIBase GetQuestNPCAIByID(int id)
	{
		return  s_QuestNPCAIEntities.Get(id);
	}
#endif

	//! Server
	void AbandonQuest(ExpansionQuestPersistentData playerQuestData, ExpansionQuest quest, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		string playerUID = identity.GetId();
		auto questConfig = quest.GetQuestConfig();
		int questID = questConfig.GetID();

		//! Don't cleanup if other group member(s) still online
		if (questConfig.IsGroupQuest() && quest.IsOtherGroupMemberOnline())
			RemoveActiveQuest(quest.GetGroupID().ToString(), quest);
		else
			CleanupActiveQuest(playerUID, quest);

		playerQuestData.UpdateQuestState(questID, ExpansionQuestState.NONE);
		//UpdateQuestData(quest, playerQuestData, true);
		SaveAndSyncQuestData(playerQuestData, playerUID, questID);
	}

	//! Server
	map<string, ref map<int, ref ExpansionQuest>> GetActiveQuests()
	{
		return m_ActiveQuests;
	}

	//! Server
	void AddActiveQuest(string key, ExpansionQuest quest)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID = quest.GetQuestConfig().GetID();

		map<int, ref ExpansionQuest> activeQuests = GetActiveQuests(key);
		activeQuests[questID] = quest;

		set<ref ExpansionQuest> activeQuestInstances = GetActiveQuestInstances(questID);
		activeQuestInstances.Insert(quest);
		EXTrace.Print(EXTrace.QUESTS, this, "::AddActiveQuest - key: " + key + " quests: " + activeQuests.Count() + " quest ID: " + questID + " instances: " + activeQuestInstances.Count());
	}

	//! Server
	void RemoveActiveQuest(string key, ExpansionQuest quest)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int questID = quest.GetQuestConfig().GetID();

		int count;
		map<int, ref ExpansionQuest> activeQuests = m_ActiveQuests[key];
		if (activeQuests)
		{
			activeQuests.Remove(questID);
			count = activeQuests.Count();
			if (!count)
				m_ActiveQuests.Remove(key);
		}

		set<ref ExpansionQuest> activeQuestInstances = m_ActiveQuestInstances[questID];
		if (activeQuestInstances)
		{
			activeQuestInstances.RemoveItem(quest);
			int instanceCount = activeQuestInstances.Count();
			EXTrace.Print(EXTrace.QUESTS, this, "::RemoveActiveQuest - key: " + key + " quests: " + count + " quest ID: " + questID + " instances: " + instanceCount);
			if (!instanceCount)
				m_ActiveQuestInstances.Remove(questID);
		}
		else
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::RemoveActiveQuest - key: " + key + " quests: " + count + " quest ID: " + questID + " instances: 0");
		}
	}

	//! Server
	map<int, ref ExpansionQuest> GetActiveQuests(string key)
	{
		map<int, ref ExpansionQuest> activeQuests = m_ActiveQuests[key];
		if (!activeQuests)
		{
			activeQuests = new map<int, ref ExpansionQuest>;
			m_ActiveQuests[key] = activeQuests;
		}

		return activeQuests;
	}

	//! Server
	set<ref ExpansionQuest> GetActiveQuestInstances(int questID)
	{
		set<ref ExpansionQuest> activeQuestInstances = m_ActiveQuestInstances[questID];
		if (!activeQuestInstances)
		{
			activeQuestInstances = new set<ref ExpansionQuest>;
			m_ActiveQuestInstances[questID] = activeQuestInstances;
		}

		return activeQuestInstances;
	}

	//! Server
	static ExpansionQuestNPCBase GetClosestQuestNPCByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestNPCBase closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCBase npc = s_QuestNPCEntities.Get(npcID);
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
	static ExpansionQuestStaticObject GetClosestQuestObjectByID(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		ExpansionQuestStaticObject closestNPC;

		foreach (int npcID: ids)
		{
			ExpansionQuestStaticObject npc = s_QuestObjectEntities.Get(npcID);
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
	bool IsOtherQuestInstanceActive(int questID)
	{
		set<ref ExpansionQuest> activeQuestInstances = m_ActiveQuestInstances[questID];
		return activeQuestInstances && activeQuestInstances.Count() > 1;
	}

	bool IsGroupPlayer(int groupID, string playerUID)
	{
	#ifdef EXPANSIONMODGROUPS
		auto group = m_PartyModule.GetPartyByID(groupID);
		return group && group.GetPlayer(playerUID) != null;
	#endif
	}

	static ExpansionQuestStaticObject GetQuestObjectByID(int id)
	{
		return  s_QuestObjectEntities.Get(id);
	}

	static void AddStaticQuestObject(int id, ExpansionQuestStaticObject staticQustObject)
	{
		if (!s_QuestObjectEntities[id])  //! Can be NULL if object was deleted because it was no longer in network bubble
			s_QuestObjectEntities[id] = staticQustObject;

		if (!s_QuestNPCIndicatorStates.Contains(id))  //! Don't override if already set
			s_QuestNPCIndicatorStates[id] = ExpansionQuestIndicatorState.NOT_SET;
	}

	static void RemoveStaticQuestObject(int id)
	{
		if (s_QuestObjectEntities.Contains(id))
		{
			if (s_QuestObjectEntities.Get(id))
				GetGame().ObjectDelete(s_QuestObjectEntities.Get(id));

			s_QuestObjectEntities.Remove(id);
		}
	}

	static ExpansionQuestNPCBase GetQuestNPCByID(int id)
	{
		return s_QuestNPCEntities.Get(id);
	}

	static void AddQuestNPC(int id, ExpansionQuestNPCBase questNPC)
	{
		if (!s_QuestNPCEntities[id])  //! Can be NULL if object was deleted because it was no longer in network bubble
			s_QuestNPCEntities[id] = questNPC;

		if (!s_QuestNPCIndicatorStates.Contains(id))  //! Don't override if already set
			s_QuestNPCIndicatorStates[id] = ExpansionQuestIndicatorState.NOT_SET;
	}

	static void RemoveQuestNPC(int id)
	{
		if (s_QuestNPCEntities.Contains(id))
		{
			if (s_QuestNPCEntities.Get(id))
				GetGame().ObjectDelete(s_QuestNPCEntities.Get(id));

			s_QuestNPCEntities.Remove(id);
		}
	}

	static Object GetClosestQuestNPCForQuest(array<int> npcIDs, vector position)
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

	void UpdateQuestNPCIndicators()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!GetExpansionSettings().GetQuest().UseQuestNPCIndicators)
			return;

		if (!m_ClientQuestData)
			return;

		if (!m_QuestConfigs)
			return;

		foreach (int npcID, ExpansionQuestIndicatorState state: s_QuestNPCIndicatorStates)
		{
			QuestModulePrint("Resetting Quest indicator for NPC ID: " + npcID);
			s_QuestNPCIndicatorStates[npcID] = ExpansionQuestIndicatorState.NOT_SET;
		}

		ExpansionQuestIndicatorState currentState;
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		foreach (int questID, ExpansionQuestConfig config: m_QuestConfigs)
		{
			int questState = m_ClientQuestData.GetQuestStateByQuestID(config.GetID());
			QuestModulePrint("Quest ID: " + config.GetID() + " \"" + config.Title + "\" | State: " + typename.EnumToString(ExpansionQuestState, questState));

			//! @note ORDER MATTERS! Always set questionmark indicator FIRST if applicable,
			//! and then set exclamation only if no indicator on NPC set yet!
			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
			{
				array<int> questTurnInIDs = config.GetQuestTurnInIDs();
				foreach (int turnInID: questTurnInIDs)
				{
					if (s_QuestNPCIndicatorStates.Find(turnInID, currentState) && currentState == ExpansionQuestIndicatorState.QUESTION)
						continue;

					if (!QuestDisplayConditions(config, player, m_ClientQuestData, turnInID, true))
						continue;

					s_QuestNPCIndicatorStates[turnInID] = ExpansionQuestIndicatorState.QUESTION;
				}
			}

			array<int> questGiverIDs = config.GetQuestGiverIDs();
			foreach (int giverID: questGiverIDs)
			{
				if (s_QuestNPCIndicatorStates.Find(giverID, currentState) && currentState != ExpansionQuestIndicatorState.NOT_SET)
					continue;

				if (!QuestDisplayConditions(config, player, m_ClientQuestData, giverID, true))
					continue;

				s_QuestNPCIndicatorStates[giverID] = ExpansionQuestIndicatorState.EXCLAMATION;
			}
		}

		SetQuestNPCIndicators();
	}

	static void SetQuestNPCIndicators()
	{
		ExpansionQuestNPCBase questNPC;
	#ifdef EXPANSIONMODAI
		ExpansionQuestNPCAIBase questAINPC;
	#endif
		ExpansionQuestStaticObject questObj;

		foreach (int npcID, ExpansionQuestIndicatorState indicatorState: s_QuestNPCIndicatorStates)
		{
			QuestModulePrint("Setting Quest indicator for NPC ID: " + npcID + " | State: " + typename.EnumToString(ExpansionQuestIndicatorState, indicatorState));

			questNPC = GetQuestNPCByID(npcID);
		#ifdef EXPANSIONMODAI
			questAINPC = GetQuestNPCAIByID(npcID);
		#endif
			questObj = GetQuestObjectByID(npcID);

			if (questNPC)
			{
				questNPC.Expansion_SetQuestIndicator(indicatorState);
			}
		#ifdef EXPANSIONMODAI
			else if (questAINPC)
			{
				questAINPC.Expansion_SetQuestIndicator(indicatorState);
			}
		#endif
			else if (questObj)
			{
				questObj.Expansion_SetQuestIndicator(indicatorState);
			}
		}
	}

	static void SetQuestNPCIndicator(Object obj, inout ParticleSource particle, ExpansionQuestIndicatorState state)
	{
		auto trace = EXTrace.StartStack(EXTrace.QUESTS, s_ModuleInstance, obj.ToString() + " " + obj.GetPosition() + " | State: " + typename.EnumToString(ExpansionQuestIndicatorState, state));

		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if (state != ExpansionQuestIndicatorState.NOT_SET)
			{
				vector minMax[2];
				obj.GetCollisionBox(minMax);

				Human human;
				DayZInfected infected;
				float offsetY;
				float height;
				if (Class.CastTo(human, obj))
				{
					offsetY = human.GetBonePositionMS(human.GetBoneIndexByName("Head"))[1];  //! Center of head in current stance
					if (!offsetY)   //! This can happen when first entering network bubble under 1.21 :-(
						offsetY = minMax[1][1];
					offsetY += 0.2;
					height = 1.8;  //! Standing
				}
				else if (Class.CastTo(infected, obj))
				{
					offsetY = infected.GetBonePositionMS(infected.GetBoneIndexByName("Head"))[1];  //! Center of head in current stance
					if (!offsetY)   //! This can happen when first entering network bubble under 1.21 :-(
						offsetY = minMax[1][1];
					offsetY += 0.2;
					height = 1.8;  //! Standing
				}
				else
				{
					offsetY = minMax[1][1];
					height = offsetY - minMax[0][1];
				}

				float scale = Math.Max(0.5, height / 1.8);  //! 1.0 for human-sized object
				offsetY += 0.15 * scale;

				EXTrace.Add(trace, string.Format("| OffsetY: %1 Height: %2 Scale: %3", offsetY, height, scale));

				if (!particle)
				{

					particle = ParticleManager.GetInstance().PlayOnObject(ParticleList.EXPANSION_PARTICLE_QUEST_MARKER, obj , "0 0 0", "0 0 0", true);

					float size;
					particle.GetParameter(0, EmitorParam.SIZE, size);
					particle.SetParameter(-1, EmitorParam.SIZE, size * scale * obj.GetScale());

					float velocity;
					particle.GetParameter(0, EmitorParam.VELOCITY, velocity);
					particle.SetParameter(-1, EmitorParam.VELOCITY, velocity / obj.GetScale());
				}
				else if (!particle.IsParticlePlaying())
				{
					particle.PlayParticle();
				}

				if (state == ExpansionQuestIndicatorState.QUESTION)
				{
					SetQuestIndicator(particle, ExpansionQuestIndicatorState.EXCLAMATION, false);
				}
				else
				{
					SetQuestIndicator(particle, ExpansionQuestIndicatorState.QUESTION, false);
				}

				//! It may seem counter-intuitive, but we need to divide offset by object scale in case it's not 1.0,
				//! otherwise the visual will be way off
				SetParticleParm(particle, -1, EmitorParam.EMITOFFSET, Vector(0.0, offsetY / obj.GetScale(), 0.0));
				SetQuestIndicator(particle, state, true);
			}
			else if (particle)
			{
				particle.StopParticle();
			}
		}
	}

	static void SetQuestNPCIndicator(int npcID, Object obj, inout ParticleSource particle)
	{
		ExpansionQuestIndicatorState state;
		if (s_QuestNPCIndicatorStates.Find(npcID, state))
			SetQuestNPCIndicator(obj, particle, state);
	}

	static void SetQuestIndicator(ParticleSource particle, int emitter, bool show)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, s_ModuleInstance, particle.ToString() + " " + typename.EnumToString(ExpansionQuestIndicatorState, emitter) + " " + show);

		particle.SetParameter(emitter, EmitorParam.BIRTH_RATE, 1.0 * show);
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

	bool QuestDisplayConditions(ExpansionQuestConfig config, PlayerBase player, ExpansionQuestPersistentData playerQuestData = null, int questNPCID = -1, bool displayQuestsWithCooldown = false, bool skipPreQuestCheck = false)
	{
		if (!player || !player.GetIdentity())
			return false;

		string playerUID = player.GetIdentity().GetId();
		int questID = config.GetID();
		string stateText;

		//! Check if quest is completed if not repeatable.
		ExpansionQuestState questState = ExpansionQuestState.NONE;
		if (playerQuestData)
		{
			questState = playerQuestData.GetQuestStateByQuestID(config.GetID());
			stateText = typename.EnumToString(ExpansionQuestState, questState);
		}

		QuestModulePrint("::QuestDisplayConditions - Quest ID: " + questID + " | Quest state: " + stateText + " | PlayerUID: " + playerUID + " | Quest NPC ID: " + questNPCID);
		if (!config.IsRepeatable() && questState == ExpansionQuestState.COMPLETED)
		{
			QuestModulePrint("::QuestDisplayConditions - Return FALSE. Quest state is " + stateText + " and quest with ID " + questID + " is not repeatable.");
			return false;
		}

		//! Check if this quest is a quest of the NPC the client is interacting with.
		if (questNPCID > -1)
		{
			if (questState == ExpansionQuestState.COMPLETED && !config.IsRepeatable())
			{
				QuestModulePrint("::QuestDisplayConditions - Return FALSE. Quest already completed!");
				return false;
			}

			array<int> questGiverIDs = config.GetQuestGiverIDs();
			if (questGiverIDs && questGiverIDs.Count() > 0)
			{
				if ((questState == ExpansionQuestState.NONE || questState == ExpansionQuestState.STARTED) && questGiverIDs.Find(questNPCID) == -1)
				{
					QuestModulePrint("::QuestDisplayConditions - Return FALSE. NPC is not a quest giver NPC for quest with ID " + questID);
					return false;
				}
			}

			array<int> questTurnInIDs = config.GetQuestTurnInIDs();
			if (questTurnInIDs && questTurnInIDs.Count() > 0)
			{
				if ((questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN) && questTurnInIDs.Find(questNPCID) == -1)
				{
					QuestModulePrint("::QuestDisplayConditions - Return FALSE. NPC is not a quest turn-in NPC for quest with ID " + questID);
					return false;
				}
			}
		}

		//! Check if client has a cooldown on this quest if quest is repeatable.
		if (!displayQuestsWithCooldown)
		{
			int timestamp;
			bool hasCooldown = false;
			if (playerQuestData)
				hasCooldown = playerQuestData.HasCooldownOnQuest(questID, timestamp);

			if (config.IsRepeatable() && hasCooldown)
			{
				QuestModulePrint("::QuestDisplayConditions - Return FALSE. Quest with ID " + questID + " is repeatable but on cooldown!");
				return false;
			}
		}

		//! Check if quest is a achievement quest.
		if (config.IsAchievement())
		{
			QuestModulePrint("::QuestDisplayConditions - Return FALSE. Quest is achiement quest!");
			return false;
		}

		//! Check if all pre-quests are completed.
		array<int> incompletedQuestIDs = new array<int>;
		if (config.GetPreQuestIDs().Count() > 0 && playerQuestData && !skipPreQuestCheck)
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
						QuestModulePrint("::QuestDisplayConditions - Quest state for pre-quest with ID: " + preQuestID + " | State: " + preQuestStateText + ". Has completed quest!");
						completedPreQuestCount++;
					}
					else
					{
						incompletedQuestIDs.Insert(preQuestID);
					}
				}
			}

			if (completedPreQuestCount < config.GetPreQuestIDs().Count())
			{
				QuestModulePrint("--------------------------------------------------");
				QuestModulePrint("::QuestDisplayConditions - Cant display quest with ID: " + questID);
				foreach (int incompletedQuestID: incompletedQuestIDs)
				{
					QuestModulePrint("::QuestDisplayConditions - Has not completed quest with ID: " + incompletedQuestID);
				}
				QuestModulePrint("::QuestDisplayConditions - Return FALSE. Not all pre-quests completed!");
				QuestModulePrint("--------------------------------------------------");
				return false;
			}
		}

	#ifdef EXPANSIONMODHARDLINE
		//! Check if for reputation requirement if config has a value but system is disabled.
		if (config.GetReputationRequirement() > 0)
		{
			if (!GetExpansionSettings().GetHardline().UseReputation)
			{
				Error("ExpansionQuestModule::QuestDisplayConditions - Quest with ID " + config.GetID() + " has a reputation requirement but the reputation system disabled! Return FALSE");
				return false;
			}
			else
			{
				//! Check if client meets the reputation requirement.
				if (config.GetReputationRequirement() > 0)
				{
					int reputation = player.Expansion_GetReputation();
					QuestModulePrint("::QuestDisplayConditions - Reputation requirement: " + config.GetReputationRequirement());
					QuestModulePrint("::QuestDisplayConditions - Player reputation: " + reputation);
					if (reputation < config.GetReputationRequirement())
					{
						QuestModulePrint("::QuestDisplayConditions - Return FALSE. Reputation requirements not met!");
						return false;
					}
				}
			}
		}

	#ifdef EXPANSIONMODAI
		map<string, int> factionRepRequirements = config.GetFactionReputationRequirements();
		if (factionRepRequirements.Count() > 0)
		{
			if (!GetExpansionSettings().GetHardline().UseFactionReputation)
			{
				Error("ExpansionQuestModule::QuestDisplayConditions - Quest with ID " + config.GetID() + " has faction reputation requirements but the faction reputation system disabled! Return FALSE");
				return false;
			}
			else
			{
				//! Check if client meets the faction reputation requirements.
				int factionReqCount = factionRepRequirements.Count();
				int playerFactionReqCount;
				int currentFactionID = player.eAI_GetFactionTypeID();
				QuestModulePrint("::QuestDisplayConditions - Faction requirement count: " + factionReqCount);
				
				foreach (string faction, int repReq: factionRepRequirements)
				{
					typename factionType = eAIFaction.GetType(faction);
					int factionID = eAIRegisterFaction.s_FactionIDs[factionType];
					QuestModulePrint("::QuestDisplayConditions - Faction ID for faction type " + factionType.ToString() + ": " + factionID);
					if (factionID != 0)
					{
						int factionRep;
						if (factionID == currentFactionID)
						{
							factionRep = player.Expansion_GetReputation();
						}
						else
						{
							factionRep = player.Expansion_GetFactionReputation(factionID);
						}
	
						QuestModulePrint("::QuestDisplayConditions - Player faction reputation: " + factionType.ToString() + " | Reputation: " + factionRep + " | Requirement: " + repReq);
						if (factionRep >= repReq)
							playerFactionReqCount++;
					}
				}
				
				QuestModulePrint("::QuestDisplayConditions - Player is meeting " + playerFactionReqCount + " out of " + factionReqCount + " requirements..");
				if (playerFactionReqCount < factionReqCount)
				{
					QuestModulePrint("::QuestDisplayConditions - Return FALSE. Faction reputation requirements not met!");
					return false;
				}
			}
		}
	#endif
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

		QuestModulePrint("::QuestDisplayConditions - End and return TRUE.");

		return true;
	}

	//! Server
	//! @note: Gets called from PlayerBase::EEKilled to check if the killed player has a active quest that should be cancled
	//! when a quest player dies.
	void CheckActivePlayerQuests(PlayerBase player)
	{
		if (!player.GetIdentity())
			return;

		if (!GetExpansionSettings().GetQuest().EnableQuests)
			return;

		string playerUID = player.GetIdentity().GetId();
		map<int, ref ExpansionQuest> activeQuests = m_ActiveQuests[playerUID];
		if (!activeQuests)
			return;

		array<ref ExpansionQuest> questsToCancel = {};
		foreach (ExpansionQuest activeQuest: activeQuests)
		{
			if (activeQuest.GetQuestConfig().CancelQuestOnPlayerDeath)
				questsToCancel.Insert(activeQuest);
		}

		foreach (ExpansionQuest questToCancel: questsToCancel)
		{
			questToCancel.CancelQuest();
		}
	}

	//! Server
	void DeleteQuestHolder(int npcID, ExpansionQuestNPCType npcType)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::DeleteQuestHolder - Tryed to call SendClientQuestConfigs on Client!");
			return;
		}

		bool removed;
		switch (npcType)
		{
			case ExpansionQuestNPCType.NORMAL:
			{
				RemoveQuestNPC(npcID);
				removed = true;
			}
			break;
			case ExpansionQuestNPCType.OBJECT:
			{
				RemoveStaticQuestObject(npcID);
				removed = true;
			}
			break;
		#ifdef EXPANSIONMODAI
			case ExpansionQuestNPCType.AI:
			{
				RemoveQuestNPCAI(npcID);
				removed = true;
			}
			break;
		#endif
		}

		if (removed)
		{
			int foundIndex = -1;
			foundIndex = m_TempQuestHolders.Find(npcID);
			if (foundIndex > -1)
			{
				m_TempQuestHolders.Remove(foundIndex);
			}
		}
	}

	//! Server
	void SpawnQuestHolder(ExpansionTempQuestHolder questHolder, ExpansionTempQuestHolderPosition questHolderPos, bool createLocal = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_TempQuestHolders)
		{
			m_TempQuestHolders = new array<int>;
		}
		else
		{
			if (m_TempQuestHolders.Find(questHolder.ID) > -1)
			{
				Print(ToString() + "::SpawnQuestHolder - A quest holder with the ID " + questHolder.ID + " already exists! Already spawned or duplicate ID?!");
				return;
			}
		}

		TStringArray questNPCs = {"ExpansionQuestNPCBase"};
		TStringArray questAINPCs = {"ExpansionQuestNPCAIBase"};
		TStringArray questObject = {"ExpansionQuestStaticObject"};

		ExpansionQuestNPCData questNPCData = new ExpansionQuestNPCData;
		questNPCData.SetID(questHolder.ID); //! Unique NPC ID
		questNPCData.SetClassName(questHolder.ClassName); //! Class name of the NPC entity
		questNPCData.SetNPCName(questHolder.Name);
		questNPCData.SetDefaultNPCText(questHolder.DefaultText);

		questNPCData.SetPosition(questHolderPos.Position); //! Quest NPC position
		questNPCData.SetOrientation(questHolderPos.Orientation); //! Quest NPC orientation

		if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questObject))
		{
			questNPCData.SetNPCType(ExpansionQuestNPCType.OBJECT);

			ExpansionQuestStaticObject object = SpawnQuestObject(questNPCData, createLocal);
			if (!object)
			{
				Error(ToString() + "::SpawnQuestHolder - Could not spawn Object quest holder with class name: " + questHolder.ClassName);
				return;
			}

			object.SetQuestNPCID(questHolder.ID);
			object.SetQuestNPCData(questNPCData);
			AddStaticQuestObject(questHolder.ID, object);
			AddQuestNPCData(questHolder.ID, questNPCData);
			m_TempQuestHolders.Insert(questHolder.ID);

			Print(ToString() + "::SpawnQuestHolder - Quest holder spawned at position: " + object.GetPosition());
			return;
		}

		if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questNPCs))
		{
			questNPCData.SetNPCType(ExpansionQuestNPCType.NORMAL);
			if (questHolder.LoadoutName != string.Empty)
				questNPCData.SetLoadoutFile(questHolder.LoadoutName);

			ExpansionQuestNPCBase questNPC = SpawnQuestNPC(questNPCData, createLocal);
			if (!questNPC)
			{
				Error(ToString() + "::SpawnQuestHolder - Could not spawn NPC quest holder with class name: " + questHolder.ClassName);
				return;
			}

			questNPC.SetQuestNPCID(questHolder.ID);
			questNPC.SetQuestNPCData(questNPCData);
			AddQuestNPC(questHolder.ID, questNPC);
			AddQuestNPCData(questHolder.ID, questNPCData);
			m_TempQuestHolders.Insert(questHolder.ID);

			Print(ToString() + "::SpawnQuestHolder - Quest holder spawned at position: " + questNPC.GetPosition());
			return;
		}

	#ifdef EXPANSIONMODAI
		if (ExpansionStatic.IsAnyOf(questHolder.ClassName, questAINPCs))
		{
			questNPCData.SetNPCType(ExpansionQuestNPCType.AI);
			questNPCData.SetEmoteID(questHolder.NPCEmoteID);
			if (questHolder.LoadoutName != string.Empty)
				questNPCData.SetLoadoutFile(questHolder.LoadoutName);

			ExpansionQuestNPCAIBase questNPCAI = SpawnQuestNPCAI(questNPCData, createLocal);
			if (!questNPCAI)
			{
				Error(ToString() + "::SpawnQuestHolder - Could not spawn quest AI NPC holder with class name: " + questHolder.ClassName);
				return;
			}

			questNPCAI.SetQuestNPCID(questHolder.ID);
			questNPCAI.SetQuestNPCData(questNPCData);
			AddQuestNPCAI(questHolder.ID, questNPCAI);
			AddQuestNPCData(questHolder.ID, questNPCData);
			m_TempQuestHolders.Insert(questHolder.ID);

			Print(ToString() + "::SpawnQuestHolder - Quest holder spawned at position: " + questNPCAI.GetPosition());
			return;
		}
	#endif

		Error(ToString() + "::SpawnQuestHolder - Could not spawn quest holder with class name: " + questHolder.ClassName);
	}

	protected ExpansionQuestStaticObject SpawnQuestObject(ExpansionQuestNPCData questNPCData, bool createLocal = false)
	{
		int flags = ECE_KEEPHEIGHT | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME;
		if (createLocal)
			flags = flags | ECE_LOCAL;

	    Object obj = GetGame().CreateObjectEx(questNPCData.GetClassName(), questNPCData.GetPosition(), flags);
	    ExpansionQuestStaticObject questObject;
	    if (!ExpansionQuestStaticObject.CastTo(questObject, obj))
	    {
			GetGame().ObjectDelete(obj);
	        return null;
	    }

	    questObject.SetPosition(questNPCData.GetPosition());
	    questObject.SetOrientation(questNPCData.GetOrientation());

		if (questNPCData.GetNPCName() != string.Empty)
	    	questObject.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());
		
		questObject.Update();

	    return questObject;
	}

	protected ExpansionQuestNPCBase SpawnQuestNPC(ExpansionQuestNPCData questNPCData, bool createLocal = false)
	{
		Object obj = GetGame().CreateObject(questNPCData.GetClassName(), questNPCData.GetPosition(), createLocal);
 		ExpansionQuestNPCBase questNPC;
		if (!ExpansionQuestNPCBase.CastTo(questNPC, obj))
	    {
			GetGame().ObjectDelete(obj);
	        return null;
	    }

	    questNPC.SetPosition(questNPCData.GetPosition());
	    questNPC.SetOrientation(questNPCData.GetOrientation());

		if (questNPCData.GetNPCName() != string.Empty)
			questNPC.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());

		if (questNPCData.GetLoadoutFile() != string.Empty)
			ExpansionHumanLoadout.Apply(questNPC, questNPCData.GetLoadoutFile(), false);
		
		questNPC.Update();

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	protected ExpansionQuestNPCAIBase SpawnQuestNPCAI(ExpansionQuestNPCData questNPCData, bool createLocal = false)
	{
		vector position = ExpansionAIPatrol.GetPlacementPosition(questNPCData.GetPosition());

		Object obj = GetGame().CreateObject(questNPCData.GetClassName(), position, createLocal);
		if (!obj)
			return null;

		ExpansionQuestNPCAIBase questNPC = ExpansionQuestNPCAIBase.Cast(obj);
		if (!questNPC)
		{
			GetGame().ObjectDelete(obj);
			return null;
		}

		questNPC.SetPosition(position);
		questNPC.SetOrientation(questNPCData.GetOrientation());
		questNPC.m_Expansion_NetsyncData.Set(0, questNPCData.GetNPCName());
		ExpansionHumanLoadout.Apply(questNPC, questNPCData.GetLoadoutFile(), false);
		questNPC.Expansion_SetEmote(questNPCData.GetEmoteID(), !questNPCData.IsEmoteStatic());
		questNPC.Expansion_SetCanBeLooted(false);
		questNPC.eAI_SetUnlimitedReload(true);
		questNPC.eAI_SetAccuracy(1.0, 1.0);
		questNPC.eAI_SetThreatDistanceLimit(800);

		eAIGroup aiGroup = questNPC.GetGroup();

		if (questNPCData.GetFaction() != string.Empty)
		{
			eAIFaction faction = eAIFaction.Create(questNPCData.GetFaction());
			if (faction && aiGroup.GetFaction().Type() != faction.Type())
				aiGroup.SetFaction(faction);
		}

		aiGroup.SetFormation(new eAIFormationColumn(aiGroup));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

		array<vector> waypoints = questNPCData.GetWaypoints();
		for (int idx = 0; idx < waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(waypoints[idx]);
			if (waypoints[idx] == position)
				aiGroup.m_CurrentWaypointIndex = idx;
		}
		
		questNPC.Update();

		return questNPC;
	}
#endif

	bool TempQuestHolderExists(int npcID)
	{
		return m_TempQuestHolders.Find(npcID) > -1;
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