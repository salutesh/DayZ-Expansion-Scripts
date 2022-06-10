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
	CANCELED_QUEST = 1
};

[CF_RegisterModule(ExpansionQuestModule)]
class ExpansionQuestModule: CF_ModuleWorld
{
	//! Server only
	private autoptr map<string, ref ExpansionQuestPlayerData> m_PlayerDatas; //! Server
	private autoptr array<ref ExpansionQuest> m_ActiveQuests; //! Server
	private autoptr map<int, ref ExpansionQuestNpcData> m_QuestsNPCs; //! Server
	private autoptr map<int, ref ExpansionQuestConfig> m_QuestConfigs;	//! Server

	//private autoptr map<int, PlayerBase> m_QuestNPCEntities;
	private autoptr map<int, ExpansionQuestNpcBase> m_QuestNPCEntities; //! Server
#ifdef EXPANSIONMODAI
	private autoptr map<int, ExpansionQuestNpcAIBase> m_QuestNPCAIEntities; //! Server
#endif

	//! Default server data
	private ref ExpansionDefaultQuestNPCData m_DefaultQuestNPCData; //! Server
	private ref ExpansionDefaultQuestData m_DefaultQuestConfigData;  //! Server
	private ref ExpansionDefaultObjectiveData m_DefaultObjectiveConfigData; //! Server

	private autoptr map<int, ref ExpansionQuestObjectiveTravelConfig> m_TravelObjectivesConfigs; //! Server
	private autoptr map<int, ref ExpansionQuestObjectiveDeliveryConfig> m_DeliveryObjectivesConfigs; //! Server
	private autoptr map<int, ref ExpansionQuestObjectiveTargetConfig> m_TargetObjectivesConfigs;	//! Server
	private autoptr map<int, ref ExpansionQuestObjectiveCollectionConfig> m_CollectionObjectivesConfigs;	//! Server
	private autoptr map<int, ref ExpansionQuestObjectiveTreasureHuntConfig> m_TreasureHuntObjectivesConfigs;	//! Server

#ifdef EXPANSIONMODAI
	private autoptr map<int, ref ExpansionQuestObjectiveAIPatrolConfig> m_AIPatrolObjectivesConfigs;	//! Server
	private autoptr map<int, ref ExpansionQuestObjectiveAICampConfig> m_AICampObjectivesConfigs;	//! Server
	private autoptr map<int, ref ExpansionQuestObjectiveAIVIPConfig> m_AIVIPObjectivesConfigs; //! Server
#endif

	private float m_UpdateQueueTimer; //! Server update que timer
	private int m_CurrentQuestTick; //! Server
	private const float UPDATE_TICK_TIME = 1.0; // refreshes 100 active quests every ten seconds (10 every sec.)
	private const int UPDATE_QUESTS_PER_TICK = 10; //! Server

#ifdef EXPANSIONMODAI
	private float m_UpdateNpcQueueTimer; //! Server update que timer
	private int m_CurrentNpcTick; //! Server
	private const float UPDATE_NPC_TIME = 10.0; // refreshes 10 ai npc entities every ten seconds.
	private const int UPDATE_NPC_PER_TICK = 10; //! Server
#endif

	//! Client only
	private autoptr map<int, ref ExpansionQuestConfig> m_QuestClientConfigs;	//! Client
	private ref ExpansionQuestPlayerData m_PlayerQuestData; //! Client
	private ref ExpansionQuestNpcData m_ClientQuestNPC; //! Client
	private ref ScriptInvoker m_QuestMenuInvoker; //! Client
#ifdef EXPANSIONMODBOOK
	private ref ScriptInvoker m_QuestLogInvoker; //! Client
	private ref ScriptInvoker m_QuestLogCallbackInvoker; //! Client
#endif

#ifdef EXPANSIONMODNAVIGATION
	private autoptr array<ref ExpansionQuestClientMarker> m_QuestMarkers; //! Client
#endif

	private bool m_HandleLogout = false; //! Client

	// ------------------------------------------------------------
	// ExpansionQuestModule Constructor
	// ------------------------------------------------------------
	void ExpansionQuestModule()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "ExpansionQuestModule").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::ExpansionQuestModule - Start");

		QuestModulePrint(ToString() + "::ExpansionQuestModule - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionQuestModule()
	{
		QuestModulePrint(ToString() + "::~ExpansionQuestModule - Start");

		QuestModulePrint(ToString() + "::~ExpansionQuestModule - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnInit").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::OnInit - Start");

		super.OnInit();

		EnableMissionFinish();
		EnableMissionLoaded();
		EnableInvokeDisconnect();
		EnableClientLogout();
		EnableClientLogoutCancelled();
		EnableClientReady();
		EnableClientDisconnect();
		EnableUpdate();
		EnableRPC();

		QuestModulePrint(ToString() + "::OnInit - End");
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

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
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

		#ifdef EXPANSIONMODAI
			if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);

			if (!FileExist(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER))
				MakeDirectory(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);
		#endif
		}

		m_QuestConfigs = new map<int, ref ExpansionQuestConfig>; //! Server
		m_PlayerDatas = new map<string, ref ExpansionQuestPlayerData>; //! Server

		m_TravelObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTravelConfig>;	//! Server
		m_DeliveryObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveDeliveryConfig>; //! Server
		m_TargetObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTargetConfig>; //! Server
		m_CollectionObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveCollectionConfig>; //! Server
		m_TreasureHuntObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveTreasureHuntConfig>; //! Server

	#ifdef EXPANSIONMODAI
		m_AIPatrolObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIPatrolConfig>; //! Server
		m_AICampObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAICampConfig>; //! Server
		m_AIVIPObjectivesConfigs = new map<int, ref ExpansionQuestObjectiveAIVIPConfig>; //! Server
	#endif

		m_QuestsNPCs = new map<int, ref ExpansionQuestNpcData>; //! Server & Client

		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_ActiveQuests = new array<ref ExpansionQuest>;
			m_QuestNPCEntities = new map<int, ExpansionQuestNpcBase>;

		#ifdef EXPANSIONMODAI
			m_QuestNPCAIEntities = new map<int, ExpansionQuestNpcAIBase>;
		#endif

			string worldName;
			GetGame().GetWorldName(worldName);
			worldName.ToLower();
			
			m_DefaultQuestNPCData = new ExpansionDefaultQuestNPCData(worldName);
			m_DefaultQuestConfigData = new ExpansionDefaultQuestData(worldName);
			m_DefaultObjectiveConfigData = new ExpansionDefaultObjectiveData(worldName);

			//! OBJECTIVE FILES
			//! TRAVEL OBJECTIVES
			array<string> travelObjectiveFiles = new array<string>;
			travelObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER);
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
			targetObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER);
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
			deliveryObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER);
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
			collectionObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER);
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
			treasureHuntObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);
			if (treasureHuntObjectiveFiles.Count() > 0)
			{
				LoadObjectivesData(treasureHuntObjectiveFiles, EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER);
			}
			else
			{
				DefaultTreasureHuntObjectivesData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

		#ifdef EXPANSIONMODAI
			//! AI PATROL OBJECTIVES
			array<string> aiPatrolObjectiveFiles = new array<string>;
			aiPatrolObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER);
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
			aiCampObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER);
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
			aiVIPObjectiveFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER);
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
			questFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_QUESTS_FOLDER);
			if (questFiles.Count() > 0)
			{
				LoadQuestData(questFiles); //! Server: Load existing quest data files from the server and load them into m_QuestConfigs.
			}
			else
			{
				DefaultQuestData(); //! Server: Create default quest data on the server and load them into m_QuestConfigs.
			}

			//! QUEST NPC FILES
			array<string> questNPCFiles = new array<string>;
			questNPCFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_NPCS_FOLDER);
			if (questNPCFiles.Count() > 0)
			{
				LoadQuestNPCData(questNPCFiles); //! Server: Load existing quest NPCs data files from the server and load them into m_QuestsNPCs.
			}
			else
			{
				DefaultQuestNPCData(); //! Server: Create default quest NPCs data on the server and load them into m_QuestsNPCs.
			}

			LoadPlayerQuestData(); //! Server: Load all existing player quest data information into m_PlayerDatas.
			SpawnQuestNPCs(); //! Server: Spawn all quest NPCs on the server based on the loaded data.
		}

		//! Client only
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
				m_QuestMarkers = new array<ref ExpansionQuestClientMarker>;
		#endif
		}

		QuestModulePrint(ToString() + "::OnMissionLoaded - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnClientReady
	// ------------------------------------------------------------
	override void OnClientReady(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnClientReady");
	#endif

		QuestModulePrint(ToString() + "::OnClientReady - Start");

		super.OnClientReady(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			QuestModulePrint(ToString() + "::OnClientReady - Setup player quests for player with UID: " + cArgs.Identity.GetId());

			SendQuestNPCDataServer(cArgs.Identity); //! Send all quest npc datas to the players client.

			SetupClientData(cArgs.Identity); //! Client setup procudure. Sends needed data like the players presistent quest data to the client and recreates existig quests for the player on the server.
		}
		QuestModulePrint(ToString() + "::OnClientReady - End");
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

		//! [Server] Player disconnects and we look for any existing ExpansionQuestPlayerData of the player with his UID.
		// If we find existing data we sync that data from the players client to the server and save it for the next session.
		//! Maybe move this to the OnClientLogout method
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CleanupPlayerQuests(cArgs.Identity.GetId());
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

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			HandleClientLogout(cArgs.Identity);

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

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - cArgs: " + cArgs.ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Player: " + cArgs.Player.ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Identity from cArgs: " + cArgs.Identity.ToString()); //! Always NULL Welp
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - Identity from Player: " + cArgs.Player.GetIdentity().ToString());
			QuestModulePrint(ToString() + "::OnClientLogoutCancelled - UID: " + cArgs.Player.GetIdentity().GetId());

			if (cArgs.Player && !m_HandleLogout)
			{
				HandleClientLogoutCancelled(cArgs.Player.GetIdentity().GetId());
			}
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
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			for (int i = 0; i < m_ActiveQuests.Count(); i++)
			{
				ExpansionQuest quest = m_ActiveQuests[i];
				if (!quest)
				{
					Error(ToString() + "::OnMissionFinish - Could not get quest!");
					continue;
				}

				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Update and save the presistent player quest data
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				if (quest.GetQuestState() == ExpansionQuestState.STARTED || quest.GetQuestState() == ExpansionQuestState.CAN_TURNIN)
				{
					int questID = quest.GetQuestConfig().GetID();
					ExpansionQuestTimestampPlayerData timestamp;
					ExpansionQuestPlayerData playerData;

					if (!quest.IsGroupQuest())
					{
						playerData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
						if (playerData && playerData.HasCooldownOnQuest(questID, timestamp))
						{
							timestamp.SetTimestamp(0);
						}
					}
				#ifdef EXPANSIONMODGROUPS
					else if (quest.IsGroupQuest() && quest.GetGroup())
					{
						ExpansionPartyData group = quest.GetGroup();
						{
							Error(ToString() + "::OnMissionFinish - Could not get group from quest!");
							return;
						}

						for (int j = 0; j < quest.GetGroup().GetPlayers().Count(); j++)
						{
							ExpansionPartyPlayerData playerGroupData = quest.GetGroup().GetPlayers()[j];
							if (!playerGroupData)
							{
								Error(ToString() + "::OnMissionFinish - Could not get group player data from groups player array!");
								continue;
							}

							playerData = GetPlayerQuestDataByUID(playerGroupData.GetID());
							if (playerData && playerData.HasCooldownOnQuest(questID, timestamp))
							{
								timestamp.SetTimestamp(0);
							}
						}
					}
				#endif

					playerData.UpdateQuestState(quest.GetQuestConfig().GetID(), quest.GetQuestState());
				}

				UpdateQuestPlayersObjectiveData(quest);

				//! Delete quest
				quest.OnQuestCleanup();
				quest.QuestDebug();
				m_ActiveQuests.Remove(i);
				delete quest;
			}
		}

		QuestModulePrint(ToString() + "::OnMissionFinish - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule HandleClientLogoutCancelled
	// -----------------------------------------------------------
	//! Handles recreation of quest markers and quest items if
	//! a quest player cancles a logout.
	private void HandleClientLogoutCancelled(string playerUID)
	{
		//! Prepare recreation of all quest markers for the client
		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::HandleClientLogoutCancelled - Could not get quest from active quest array!");
				continue;
			}

			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID) || !quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;

		#ifdef EXPANSIONMODNAVIGATION
			quest.OnRecreateClientMarkers();
		#endif

			quest.CreateQuestItems();
		}

		m_HandleLogout = true;
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
			case ExpansionQuestModuleRPC.SendQuestNPCData:
			{
				RPC_SendQuestNPCData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
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
		#ifdef EXPANSIONMODBOOK
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
		#endif
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
	// ExpansionQuestModule SendQuestNPCDataServer
	// Called on server
	// ------------------------------------------------------------
	void SendQuestNPCDataServer(PlayerIdentity identity)
	{
		QuestModulePrint(ToString() + "::SendQuestNPCDataServer - Start");

		string playerUID = identity.GetId();

		ScriptRPC rpc = new ScriptRPC();

		int npcCount = m_QuestsNPCs.Count();
		rpc.Write(npcCount);

		foreach (int npcID, ExpansionQuestNpcData npcData: m_QuestsNPCs)
		{
			if (!npcData)
			{
				Error(ToString() + "::SendQuestNPCDataServer - Error on getting quest npc data!");
				continue;
			}

			npcData.OnSend(rpc);
		}

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendQuestNPCData, false, identity);

		QuestModulePrint(ToString() + "::SendQuestNPCDataServer - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendQuestNPCData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendQuestNPCData(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_SendQuestNPCData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RPC_SendQuestNPCData - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendQuestNPCData - Tryed to call RPC_SendQuestNPCData on Server!");
			return;
		}

		if (!m_QuestsNPCs)
			m_QuestsNPCs = new map<int, ref ExpansionQuestNpcData>;

		int npcCount;
		if (!ctx.Read(npcCount))
		{
			Error(ToString() + "::RPC_SendQuestNPCData - Error on readinf quest npc data count!");
			return;
		}

		for (int i = 0; i < npcCount; i++)
		{
			ExpansionQuestNpcData npcData = new ExpansionQuestNpcData();
			if (!npcData.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendQuestNPCData - Error on recieving quest npc data!");
				return;
			}

			QuestModulePrint(ToString() + "::RPC_SendQuestNPCData - Add npc data for ID " + npcData.GetID() + " | " + npcData);
		#ifdef EXPANSIONMODQUESTSMODULEDEBUG
			npcData.QuestDebug();
		#endif

			m_QuestsNPCs.Insert(npcData.GetID(), npcData);
		}

		QuestModulePrint(ToString() + "::RPC_SendQuestNPCData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestPlayerQuests
	// Called on client
	// ------------------------------------------------------------
	//! Called to update players quest data on the client after it has been changed
	void RequestPlayerQuests()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RequestPlayerQuests").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RequestPlayerQuests - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestPlayerQuests - Tryed to call RequestPlayerQuests on Server!");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestPlayerQuests, false);

		QuestModulePrint(ToString() + "::RequestPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdatePlayerQuests
	// Called on server
	// ------------------------------------------------------------
	void UpdatePlayerQuests(ExpansionQuest quest)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "UpdatePlayerQuests").Add(sender).Add(ctx);
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

			for (int j = 0; j < questGroup.GetPlayers().Count(); j++)
			{
				ExpansionPartyPlayerData playerGroupData = questGroup.GetPlayers()[j];
				if (!playerGroupData)
				{
					Error(ToString() + "::UpdatePlayerQuests - Could not get group player data!");
					continue;
				}

				string playerUID = playerGroupData.GetID();
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (groupPlayer)
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
		string playerUID = identity.GetId();
		array<ref ExpansionQuestConfig> validQuests = new array<ref ExpansionQuestConfig>;
		validQuests = GetPlayerQuests(playerUID);
		if (!validQuests)
		{
			QuestModulePrint(ToString() + "::SendPlayerQuestServer - Could not get any valid quest for player with UID: " + playerUID);
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
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

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuests, false, identity);
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestPlayerQuests
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestPlayerQuests(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_RequestPlayerQuests").Add(sender).Add(ctx);
	#endif

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
		array<ref ExpansionQuestConfig> validQuests = new array<ref ExpansionQuestConfig>;
		validQuests = GetPlayerQuests(playerUID);
		if (!validQuests)
		{
			Error(ToString() + "::SendPlayerQuestData - Could not create valid quest array for player with UID: " + playerUID);
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
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

		rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuests, false, identity);

		QuestModulePrint(ToString() + "::RPC_RequestPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendPlayerQuests
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerQuests(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_SendPlayerQuests").Add(sender).Add(ctx);
	#endif

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

		GetQuestLogSI().Invoke(m_QuestClientConfigs);

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule SendPlayerQuestData
	// Called on server
	// ------------------------------------------------------------
	//! Called to update players quest data on the client
	void SendPlayerQuestData(PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SendPlayerQuestData").Add(sender).Add(ctx);
	#endif

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
			ExpansionQuestPlayerData questPlayerData = GetPlayerQuestDataByUID(playerUID);
			if (questPlayerData)
			{
				QuestModulePrint(ToString() + "::SendPlayerQuestData - Send player quest data to client for UID: " + playerUID);
				ScriptRPC rpc = new ScriptRPC();
				questPlayerData.OnSend(rpc);
				rpc.Send(NULL, ExpansionQuestModuleRPC.SendPlayerQuestData, false, identity);
			}
		}

		QuestModulePrint(ToString() + "::SendPlayerQuestData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_SendPlayerQuestData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendPlayerQuestData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_SendPlayerQuestData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RPC_SendPlayerQuestData - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendPlayerQuestData - Tryed to call RPC_SendPlayerQuestData on Server!");
			return;
		}

		//! Need to apply the variable from a new instance here here and cant use the existing one?!
		ExpansionQuestPlayerData playerQuestData = new ExpansionQuestPlayerData();
		if (!playerQuestData.OnRecieve(ctx))
		{
			Error(ToString() + "::RPC_SendPlayerQuestData - Error on recieving quest player data!");
			return;
		}

		m_PlayerQuestData = playerQuestData; //! MONKA WUT?!
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
	private void SetupClientData(PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SetupClientData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::SetupClientData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SetupClientData - Tryed to call SetupClientData on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::SetupClientData - Odentity is NULL!");
			return;
		}

		string playerUID = identity.GetId();
		ExpansionQuestPlayerData questPlayerData = GetPlayerQuestDataByUID(playerUID);
		if (!questPlayerData)
		{
			//! If the player has no exiting quest data then we create a new instance for him and add it to m_PlayerDatas
			questPlayerData = new ExpansionQuestPlayerData();
			questPlayerData.Save(playerUID);
			QuestModulePrint(ToString() + "::SetupClientData - Created new presistent player quest data for player UID: " + playerUID);
		}
		else
		{
			QuestModulePrint(ToString() + "::SetupClientData - Loaded existend player quest data for player with UID: " + playerUID);
			questPlayerData.QuestDebug();
		}

		questPlayerData.SetQuestModule(this);
		m_PlayerDatas.Insert(playerUID, questPlayerData);

		PlayerQuestsInit(questPlayerData, playerUID); //! If the player has existing player data we need to initialize any exisitng quests that he started on the server

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendPlayerQuestServer, 2000, false, identity); //! Send all quest config. datas the player should be able to see to the players client.

		QuestModulePrint(ToString() + "::SetupClientData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestOpenQuestMenu
	// Called on server
	// ------------------------------------------------------------
	void RequestOpenQuestMenu(int questNPCID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RequestOpenQuestMenu").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RequestOpenQuestMenu - Tryed to call RequestOpenQuestMenu on Client!");
			return;
		}

		if (!identity)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Odentity is NULL!");
			return;
		}

		ExpansionQuestNpcData questNPCData = GetQuestNPCDataByID(questNPCID);
		if (!questNPCData)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get quest npc data!");
			return;
		}

		string playerUID = identity.GetId();
		array<ref ExpansionQuestConfig> validQuests = new array<ref ExpansionQuestConfig>;
		validQuests = GetNPCQuests(questNPCID, playerUID);
		if (!validQuests)
		{
			Error(ToString() + "::RequestOpenQuestMenu - Could not get any valid npc quest data!");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		questNPCData.OnSend(rpc);

		int questCount = validQuests.Count();
		rpc.Write(questCount);

		for (int i = 0; i < questCount; i++)
		{
			ExpansionQuestConfig questConfig = validQuests[i];
			if (!questConfig)
			{
				Error(ToString() + "::RequestOpenQuestMenu - Odentity is NULL!");
				continue;
			}

			questConfig.OnSend(rpc);
		}

	#ifdef EXPANSIONMODAI
		ExpansionQuestNpcAIBase npc = GetQuestNPCAIByID(questNPCID);
		if (npc)
		{
			EmoteManager npcEmoteManager = npc.GetEmoteManager();
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

		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestOpenQuestMenu, false, identity);

		QuestModulePrint(ToString() + "::RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestOpenQuestMenu
	// Called on client
	// ------------------------------------------------------------
	private void RPC_RequestOpenQuestMenu(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_RequestOpenQuestMenu").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Tryed to call RPC_RequestOpenQuestMenu on Server!");
			return;
		}

		if (!m_ClientQuestNPC)
			m_ClientQuestNPC = new ExpansionQuestNpcData();

		if (!m_ClientQuestNPC.OnRecieve(ctx))
		{
			Error(ToString() + "::RPC_RequestOpenQuestMenu - Could not get valid npc quest data!");
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
		m_QuestMenuInvoker.Invoke(validQuests, m_ClientQuestNPC);

		QuestModulePrint(ToString() + "::RPC_RequestOpenQuestMenu - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Called on Client
	// ------------------------------------------------------------
	private void CreateQuestInstance(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CreateQuestInstance").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CreateQuestInstance - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::CreateQuestInstance - Tryed to call CreateQuestInstance on Server!");
			return;
		}

		string playerUID = GetGame().GetPlayer().GetIdentity().GetId();
		if (playerUID == string.Empty)
			return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(questID);
		rpc.Write(playerUID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CreateQuestInstance, false);

		QuestModulePrint(ToString() + "::CreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CreateQuestInstance
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_CreateQuestInstance(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_CreateQuestInstance").Add(sender).Add(ctx);
	#endif

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

		string playerUID;
		if (!ctx.Read(playerUID))
		{
			Error(ToString() + "::RPC_CreateQuestInstance - Could not get read player UID!");
			return;
		}

		//! Get quest config instance and create a copy of that config for our new quest instance
		ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);

		//! Check if the quest is a daylie/weekly quest and if the player has still a cooldown on it.
		//! ToDo: We want to do this check also for all group members so this cant be skipped!
		int time;
		if (configInstance.IsDaylieQuest() || configInstance.IsWeeklyQuest())
		{
			int daylie = 86400; //! Day in seconds
			int weekly = 604800; //! Week in seconds
			bool hasCooldown = false;
			int remaining;

			//time = CF_Date.Now(true).GetTimestamp();
			ExpansionQuestPlayerData playerData = GetPlayerQuestDataByUID(senderRPC.GetId());
			if (configInstance.IsDaylieQuest())
			{
				hasCooldown = playerData.HasCooldown(configInstance.GetID(), daylie, remaining);
				remaining = daylie - remaining;
			}
			else if (configInstance.IsWeeklyQuest())
			{
				hasCooldown = playerData.HasCooldown(configInstance.GetID(), weekly, remaining);
				remaining = weekly - remaining;
			}

			if (hasCooldown)
			{
				string timestamp = ExpansionStatic.FormatTimestamp(remaining, false);
				ExpansionNotification(new StringLocaliser("Quest Cooldown"), new StringLocaliser("This quest is still on cooldown! Come back in " + timestamp), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
				return;
			}
		}

		ExpansionQuestConfig newconfigInstance = new ExpansionQuestConfig();

		if (!Class.CastTo(newconfigInstance, configInstance))
		{
			Error(ToString() + "::RPC_CreateQuestInstance - Could not create new config instance!");
			return;
		}

		newconfigInstance.QuestDebug();
		
		//! Create new quest
		ExpansionQuest newQuestInstance;
		if (newconfigInstance.GetType() == ExpansionQuestType.CLASS && newconfigInstance.GetQuestClassName() != string.Empty)
		{
			newQuestInstance = CreateQuestInstance(newconfigInstance.GetQuestClassName());
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
		if (newconfigInstance.IsGroupQuest())
		{
			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Quest is a group quest. Apply needed parameters!");
			//! If the player that is accepting the quest has a group we set the group owner as the main quest player
			ExpansionPartyModule partyModule;
			if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
			{
				Error(ToString() + "::RPC_CreateQuestInstance - Could not get party module!");
				return;
			}

			ExpansionPartyPlayerData groupPlayerData = partyModule.GetPartyPlayerData(playerUID);
			if (!groupPlayerData)
			{
				//! Send notification that quest can only be accepted while in a group.
				ExpansionNotification(new StringLocaliser("Group Quest"), new StringLocaliser("Group quests can only accepted while in a group!"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
				delete newQuestInstance;
				return;
			}

			ExpansionPartyData groupData = groupPlayerData.GetParty();
			if (!groupData)
			{
				//ExpansionNotification(new StringLocaliser("Group Quest"), new StringLocaliser("Could not get group"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
				delete newQuestInstance;
				return;
			}

			//! Only group owner can accept quest!
			if (groupData.GetOwnerUID() != playerUID)
			{
				ExpansionNotification(new StringLocaliser("Group Quest"), new StringLocaliser("Only a group owner can accept a group quest!"), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
				delete newQuestInstance;
				return;
			}

			newQuestInstance.SetGroup(groupData);
		}
	#endif

		newQuestInstance.SetQuestConfig(newconfigInstance);
		m_ActiveQuests.Insert(newQuestInstance);

		newQuestInstance.QuestDebug();

		newQuestInstance.OnQuestStart();

		UpdateQuestStatesForQuestPlayers(newQuestInstance, ExpansionQuestState.STARTED);

		UpdateQuestPlayersObjectiveData(newQuestInstance);

		if (newconfigInstance.IsDaylieQuest() || newconfigInstance.IsWeeklyQuest())
		{
			time = CF_Date.Now(true).GetTimestamp();
			QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - Time: " + ExpansionStatic.FormatTimestamp(time, false));
			UpdateQuestTimestampForQuestPlayers(newQuestInstance, time);
		}

		UpdatePlayerQuests(newQuestInstance);

	#ifdef EXPANSIONMODAI
		ExpansionQuestNpcAIBase npc = GetQuestNPCAIByID(newconfigInstance.GetQuestGiverID());
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

		QuestModulePrint(ToString() + "::RPC_CreateQuestInstance - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Client
	// ------------------------------------------------------------
	private void CompleteQuestClient(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CompleteQuestClient").Add(sender).Add(ctx);
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

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(questID);
		rpc.Write(playerUID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CompleteQuest, false);

		QuestModulePrint(ToString() + "::CompleteQuestClient - End");
	}

#ifdef EXPANSIONMODNAVIGATION
	// ------------------------------------------------------------
	// ExpansionQuestModule CreateClientMarker
	// Called on server
	// ------------------------------------------------------------
	void CreateClientMarker(vector pos, string text, int questID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CreateClientMarker").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CreateClientMarker - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CreateClientMarker - Tryed to call CreateClientMarker on Client!");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(pos);
		rpc.Write(text);
		rpc.Write(questID);
		rpc.Send(null, ExpansionQuestModuleRPC.CreateClientMarker, true, identity);

		QuestModulePrint(ToString() + "::CreateClientMarker - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CreateClientMarker
	// Called on client
	// ------------------------------------------------------------
	private void RPC_CreateClientMarker(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_CreateClientMarker").Add(sender).Add(ctx);
	#endif

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

		CreateMarkerClient(pos, text, questID);

		QuestModulePrint(ToString() + "::RPC_CreateClientMarker - End");
	}

	// ------------------------------------------------------------
	// ExpansionMarkerModule CreateMarkerClient
	// Called on client
	// ------------------------------------------------------------
	private void CreateMarkerClient(vector pos, string text, int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CreateMarkerClient").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CreateMarkerClient - Start");

		ExpansionMarkerModule markerModule = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		if (!markerModule)
			return;

		pos[1] = GetGame().SurfaceY(pos[0], pos[2]) + 3.0;

		ExpansionMarkerData markerData = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL);
		markerData.SetName(text);
		markerData.SetIcon("Questionmark");
		markerData.SetColor(ARGB(255,241,196,15));
		markerData.SetPosition(pos);
		markerData.Set3D(GetExpansionSettings().GetMap().CanCreate3DMarker);
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
	// ExpansionQuestModule HandleClientLogout
	// Called on server
	// ------------------------------------------------------------
	//! Handles deletion of quest markers and quest items if
	//! a quest player starts a logout.
	void HandleClientLogout(PlayerIdentity identity)
	{
		//! Prepare client marker deletion
		string playerUID = identity.GetId();
		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest currentQuest = m_ActiveQuests[i];
			if (!currentQuest)
				continue;

			if (currentQuest.IsGroupQuest() && !currentQuest.IsQuestGroupMember(playerUID) || !currentQuest.IsGroupQuest() && currentQuest.GetPlayerUID() != playerUID)
				continue;

			int questID = currentQuest.GetQuestConfig().GetID();
		#ifdef EXPANSIONMODNAVIGATION
			if (!currentQuest.IsGroupQuest()) RemoveClientMarkers(questID, identity);
		#ifdef EXPANSIONMODGROUPS
			else if (currentQuest.IsGroupQuest()) RemoveClientMarkers(questID, identity);
		#endif
		#endif

			currentQuest.CleanupQuestItems();
		}

		m_HandleLogout = false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RemoveClientMarkers
	// Called on server
	// ------------------------------------------------------------
	void RemoveClientMarkers(int questID, PlayerIdentity identity)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RemoveClientMarkers").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RemoveClientMarkers - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RemoveClientMarkers - Tryed to call RemoveClientMarkers on Client!");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(questID);
		rpc.Send(null, ExpansionQuestModuleRPC.RemoveClientMarkers, true, identity);

		QuestModulePrint(ToString() + "::RemoveClientMarkers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RemoveClientMarkers
	// Called on client
	// ------------------------------------------------------------
	private void RPC_RemoveClientMarkers(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_RemoveClientMarkers").Add(sender).Add(ctx);
	#endif

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
	private void RemoveMarkersClient(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RemoveMarkersClient").Add(sender).Add(ctx);
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

		for (int i = 0; i < m_QuestMarkers.Count(); i++)
		{
			ExpansionQuestClientMarker markerData = m_QuestMarkers[i];
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
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CancelQuest").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CancelQuest - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::CancelQuest - Tryed to call CancelQuest on Server!");
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(questID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.CancelQuest, false);

		QuestModulePrint(ToString() + "::CancelQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_CancelQuest
	// Called on server
	// ------------------------------------------------------------
	private void RPC_CancelQuest(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_CancelQuest").Add(sender).Add(ctx);
	#endif

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
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CancelQuestServer").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CancelQuestServer - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CancelQuestServer - Tryed to call CancelQuestServer on Client!");
			return;
		}

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::CancelQuestServer - Could not get quest from active quest array!");
				continue;
			}

			if (quest.GetQuestConfig().GetID() != questID || quest.GetPlayerUID() != identity.GetId())
				continue;

			ExpansionQuestTimestampPlayerData timestamp;
			if (!quest.IsGroupQuest())
			{
				RemoveClientMarkers(questID, identity);

				ExpansionQuestPlayerData playerData = GetPlayerQuestDataByUID(identity.GetId());
				if (!playerData)
				{
					Error(ToString() + "::CancelQuestServer - Could not get players quest data!");
					continue;
				}

				if (playerData.HasCooldownOnQuest(questID, timestamp))
				{
					timestamp.SetTimestamp(0);
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else if (quest.IsGroupQuest() && quest.GetGroup())
			{
				for (int j = 0; j < quest.GetGroup().GetPlayers().Count(); ++j)
				{
					ExpansionPartyPlayerData playerGroupData = quest.GetGroup().GetPlayers()[j];
					if (!playerGroupData)
					{
						Error(ToString() + "::CancelQuestServer - Could not get players group data!");
						continue;
					}

					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
					if (groupPlayer)
					{
						RemoveClientMarkers(questID, groupPlayer.GetIdentity());

						ScriptRPC rpc = new ScriptRPC();
						rpc.Write(ExpansionQuestModuleCallback.CANCELED_QUEST);
						rpc.Send(NULL, ExpansionQuestModuleRPC.CallbackClient, false, groupPlayer.GetIdentity());
					}

					ExpansionQuestPlayerData groupPlayerData = GetPlayerQuestDataByUID(playerGroupData.GetID());
					if (!groupPlayerData)
					{
						Error(ToString() + "::CancelQuestServer - Could not get group players quest data!");
						continue;
					}

					if (groupPlayerData.HasCooldownOnQuest(questID, timestamp))
					{
						timestamp.SetTimestamp(0);
					}
				}
			}
		#endif

			UpdateQuestStatesForQuestPlayers(quest, ExpansionQuestState.NONE);

			quest.OnQuestCancel();
			m_ActiveQuests.RemoveItem(quest);
			delete quest;
		}

		QuestModulePrint(ToString() + "::CancelQuestServer - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule RPC_CallbackClient
	// Client
	// -----------------------------------------------------------
	private void RPC_CallbackClient(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
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
				m_QuestLogCallbackInvoker.Invoke();
				break;
			}
		}

		QuestModulePrint(ToString() + "::RPC_CallbackClient - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultQuestNPCData
	// Server
	// -----------------------------------------------------------
	private void DefaultQuestNPCData()
	{
		//! Quest NPC #1
		ExpansionQuestNpcData questNPC_1 = new ExpansionQuestNpcData();
		questNPC_1 = GetDefaultQuestNPCData().QuestNPCData_1();
		questNPC_1.Save("QuestNPC_1");
		m_QuestsNPCs.Insert(1, questNPC_1);

		//! Quest NPC #2
		ExpansionQuestNpcData questNPC_2 = new ExpansionQuestNpcData();
		questNPC_2 = GetDefaultQuestNPCData().QuestNPCData_2();
		questNPC_2.Save("QuestNPC_2");
		m_QuestsNPCs.Insert(2, questNPC_2);

	#ifdef EXPANSIONMODHARDLINE
		//! Quest NPC #3 - Bandit Quests
		ExpansionQuestNpcData questNPC_3 = new ExpansionQuestNpcData();
		questNPC_3 = GetDefaultQuestNPCData().QuestNPCData_3();
		questNPC_3.Save("QuestNPC_3");
		m_QuestsNPCs.Insert(3, questNPC_3);

		//! Quest NPC #4 - Hero Quests
		ExpansionQuestNpcData questNPC_4 = new ExpansionQuestNpcData();
		questNPC_4 = GetDefaultQuestNPCData().QuestNPCData_4();
		questNPC_4.Save("QuestNPC_4");
		m_QuestsNPCs.Insert(4, questNPC_4);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTravelObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultTravelObjectivesData()
	{
		ExpansionQuestObjectiveTravelConfig objective_t_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Travel_001();
		objective_t_1.Save("Objective_T_1");
		m_TravelObjectivesConfigs.Insert(1, objective_t_1);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTravelObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultTargetObjectivesData()
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
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultDeliveryObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultDeliveryObjectivesData()
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
	private void DefaultCollectionObjectivesData()
	{
	#ifdef WRDG_DOGTAGS
		ExpansionQuestObjectiveCollectionConfig objective_c_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_001();
		objective_c_1.Save("Objective_C_1");
		m_CollectionObjectivesConfigs.Insert(1, objective_c_1);
	#endif

		ExpansionQuestObjectiveCollectionConfig objective_c_2 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_002();
		objective_c_2.Save("Objective_C_2");
		m_CollectionObjectivesConfigs.Insert(2, objective_c_2);

	#ifdef WRDG_DOGTAGS
	#ifdef EXPANSIONMODHARDLINE
		//! Bandit token quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_c_3 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_003();
		objective_c_3.Save("Objective_C_3");
		m_CollectionObjectivesConfigs.Insert(3, objective_c_3);

		ExpansionQuestObjectiveCollectionConfig objective_c_4 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_004();
		objective_c_4.Save("Objective_C_4");
		m_CollectionObjectivesConfigs.Insert(4, objective_c_4);

		ExpansionQuestObjectiveCollectionConfig objective_c_5 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_005();
		objective_c_5.Save("Objective_C_5");
		m_CollectionObjectivesConfigs.Insert(5, objective_c_5);

		ExpansionQuestObjectiveCollectionConfig objective_c_6 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_006();
		objective_c_6.Save("Objective_C_6");
		m_CollectionObjectivesConfigs.Insert(6, objective_c_6);

		//! Hero token quest objectives
		ExpansionQuestObjectiveCollectionConfig objective_c_7 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_007();
		objective_c_7.Save("Objective_C_7");
		m_CollectionObjectivesConfigs.Insert(7, objective_c_7);

		ExpansionQuestObjectiveCollectionConfig objective_c_8 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_008();
		objective_c_8.Save("Objective_C_8");
		m_CollectionObjectivesConfigs.Insert(8, objective_c_8);

		ExpansionQuestObjectiveCollectionConfig objective_c_9 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_009();
		objective_c_9.Save("Objective_C_9");
		m_CollectionObjectivesConfigs.Insert(9, objective_c_9);

		ExpansionQuestObjectiveCollectionConfig objective_c_10 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_010();
		objective_c_10.Save("Objective_C_10");
		m_CollectionObjectivesConfigs.Insert(10, objective_c_10);

		ExpansionQuestObjectiveCollectionConfig objective_c_11 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_011();
		objective_c_11.Save("Objective_C_11");
		m_CollectionObjectivesConfigs.Insert(11, objective_c_11);

		ExpansionQuestObjectiveCollectionConfig objective_c_12 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_Collection_012();
		objective_c_12.Save("Objective_C_12");
		m_CollectionObjectivesConfigs.Insert(12, objective_c_12);
	#endif
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultTreasureHuntObjectivesData()
	{
		ExpansionQuestObjectiveTreasureHuntConfig objective_th_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_TreasureHunt_001();
		objective_th_1.Save("Objective_TH_1");
		m_TreasureHuntObjectivesConfigs.Insert(1, objective_th_1);
	}

#ifdef EXPANSIONMODAI
	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultAIPatrolObjectivesData()
	{
		ExpansionQuestObjectiveAIPatrolConfig objective_aip_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AIPatrol_001();
		objective_aip_1.Save("Objective_AIP_1");
		m_AIPatrolObjectivesConfigs.Insert(1, objective_aip_1);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultTreasureHuntObjectivesData
	// Server
	// -----------------------------------------------------------
	private void DefaultAICampObjectivesData()
	{
		ExpansionQuestObjectiveAICampConfig objective_aic_1 = m_DefaultObjectiveConfigData.ExpansionQuestObjective_AICamp_001();
		objective_aic_1.Save("Objective_AIC_1");
		m_AICampObjectivesConfigs.Insert(1, objective_aic_1);
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestModule DefaultAIVIPObjectivesData
	// Server
	// -----------------------------------------------------------	
	private void DefaultAIVIPObjectivesData()
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
	private void DefaultQuestData()
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

		//! Quest #4 - Example template for a repeatable token quest
	#ifdef WRDG_DOGTAGS
		ExpansionQuestConfig quest_4 = m_DefaultQuestConfigData.ExpansionQuestConfig004();
		quest_4.Save("Quest_4");
		m_QuestConfigs.Insert(4, quest_4);
	#endif

		//! Quest #5 - Example template for a daylie treasure hunt quest that autocompletes
		ExpansionQuestConfig quest_5 = m_DefaultQuestConfigData.ExpansionQuestConfig005();
		quest_5.Save("Quest_5");
		m_QuestConfigs.Insert(5, quest_5);

	#ifdef EXPANSIONMODAI
		//! Quest #6 - Example template for a ai patrol target quest
		ExpansionQuestConfig quest_6 = m_DefaultQuestConfigData.ExpansionQuestConfig006();
		quest_6.Save("Quest_6");
		m_QuestConfigs.Insert(6, quest_6);

		//! Quest #7 - Example tempalte for a ai camp quest (Not working yet)
		ExpansionQuestConfig quest_7 = m_DefaultQuestConfigData.ExpansionQuestConfig007();
		quest_7.Save("Quest_7");
		m_QuestConfigs.Insert(7, quest_7);
	#endif

	#ifdef EXPANSIONMODGROUPS
		//! Quest #8 - Example template for a target group quest
		ExpansionQuestConfig quest_8 = m_DefaultQuestConfigData.ExpansionQuestConfig008();
		quest_8.Save("Quest_8");
		m_QuestConfigs.Insert(8, quest_8);
	#endif

	#ifdef WRDG_DOGTAGS
	#ifdef EXPANSIONMODHARDLINE
		//! Quest #9 - Bandit token quests
		ExpansionQuestConfig quest_9 = m_DefaultQuestConfigData.ExpansionQuestConfig009();
		quest_9.Save("Quest_9");
		m_QuestConfigs.Insert(9, quest_9);

		//! Quest #10
		ExpansionQuestConfig quest_10 = m_DefaultQuestConfigData.ExpansionQuestConfig010();
		quest_10.Save("Quest_10");
		m_QuestConfigs.Insert(10, quest_10);

		//! Quest #11
		ExpansionQuestConfig quest_11 = m_DefaultQuestConfigData.ExpansionQuestConfig011();
		quest_11.Save("Quest_11");
		m_QuestConfigs.Insert(11, quest_11);

		//! Quest #12
		ExpansionQuestConfig quest_12 = m_DefaultQuestConfigData.ExpansionQuestConfig012();
		quest_12.Save("Quest_12");
		m_QuestConfigs.Insert(12, quest_12);

		//! Quest #13 - Hero token quests
		ExpansionQuestConfig quest_13 = m_DefaultQuestConfigData.ExpansionQuestConfig013();
		quest_13.Save("Quest_13");
		m_QuestConfigs.Insert(13, quest_13);

		//! Quest #14
		ExpansionQuestConfig quest_14 = m_DefaultQuestConfigData.ExpansionQuestConfig014();
		quest_14.Save("Quest_14");
		m_QuestConfigs.Insert(14, quest_14);

		//! Quest #15
		ExpansionQuestConfig quest_15 = m_DefaultQuestConfigData.ExpansionQuestConfig015();
		quest_15.Save("Quest_15");
		m_QuestConfigs.Insert(15, quest_15);

		//! Quest #16
		ExpansionQuestConfig quest_16 = m_DefaultQuestConfigData.ExpansionQuestConfig016();
		quest_16.Save("Quest_16");
		m_QuestConfigs.Insert(16, quest_16);

		//! Quest #17
		ExpansionQuestConfig quest_17 = m_DefaultQuestConfigData.ExpansionQuestConfig017();
		quest_17.Save("Quest_17");
		m_QuestConfigs.Insert(17, quest_17);

		//! Quest #18
		ExpansionQuestConfig quest_18 = m_DefaultQuestConfigData.ExpansionQuestConfig018();
		quest_18.Save("Quest_18");
		m_QuestConfigs.Insert(18, quest_18);
	#endif
	#endif

		//! Quest #19 - Achievement
		ExpansionQuestConfig quest_19 = m_DefaultQuestConfigData.ExpansionQuestConfig019();
		quest_19.Save("Quest_19");
		m_QuestConfigs.Insert(19, quest_19);

		//! Quest #20 - Achievement
		ExpansionQuestConfig quest_20 = m_DefaultQuestConfigData.ExpansionQuestConfig020();
		quest_20.Save("Quest_20");
		m_QuestConfigs.Insert(20, quest_20);
	
	#ifdef EXPANSIONMODAI
		//! Quest #22 - Example template for a ai vip quest
		ExpansionQuestConfig quest_22 = m_DefaultQuestConfigData.ExpansionQuestConfig022();
		quest_22.Save("Quest_22");
		m_QuestConfigs.Insert(22, quest_22);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule SpawnQuestNPCs
	// Server
	// -----------------------------------------------------------
	private void SpawnQuestNPCs()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SpawnQuestNPCs").Add(sender).Add(ctx);
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

		foreach (int id, ExpansionQuestNpcData questNPCData: m_QuestsNPCs)
		{
			if (!questNPCData.IsAI())
			{
				ExpansionQuestNpcBase npc = questNPCData.SpawnNPC();
				if (npc)
				{
					npc.SetQuestNPCID(questNPCData.GetID());
					npc.SetQuestNPCData(questNPCData);
					m_QuestNPCEntities.Insert(questNPCData.GetID(), npc);

					npc.SetPosition(questNPCData.GetPosition());
					npc.SetOrientation(questNPCData.GetOrientation());
				}
			}
		#ifdef EXPANSIONMODAI
			else
			{
				ExpansionQuestNpcAIBase npcAI = questNPCData.SpawnNPCAI();
				if (npcAI)
				{
					npcAI.SetQuestNPCID(questNPCData.GetID());
					npcAI.SetQuestNPCData(questNPCData);
					m_QuestNPCAIEntities.Insert(questNPCData.GetID(), npcAI);

					npcAI.SetPosition(questNPCData.GetPosition());
					npcAI.SetOrientation(questNPCData.GetOrientation());
					npcAI.SetEmote(questNPCData.GetEmoteID());

					#ifdef EXPANSIONMODAI
					eAIGroup ownerGrp = npcAI.GetGroup();
					#else
					if ( eAIGlobal_HeadlessClient )
						GetRPCManager().SendRPC( "eAI", "HCLinkObject", new Param1< PlayerBase >( npcAI ), false, eAIGlobal_HeadlessClient );

					eAIGame game = MissionServer.Cast( GetGame().GetMission() ).GetEAIGame();
					eAIGroup ownerGrp = game.GetGroupByLeader( npcAI );
					#endif
					ownerGrp.SetFaction( new eAIFactionCivilian() );

					for (int j = 0; j < questNPCData.GetWaypoints().Count(); j++ )
					{
						EXPrint("Adding waypoint " + questNPCData.GetWaypoints()[j]);
						ownerGrp.AddWaypoint( questNPCData.GetWaypoints()[j] );
					}

					#ifdef EXPANSIONMODAI
					ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
					#else
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( npcAI.RequestTransition, 10000, false, "Rejoin" );

					npcAI.SetAI( ownerGrp );
					#endif
				}
			}
		#endif
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetPlayerQuestData
	// Server
	// -----------------------------------------------------------
	private void GetPlayerQuestData(string fileName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetPlayerQuestData").Add(sender).Add(ctx);
	#endif
		QuestModulePrint(ToString() + "::GetPlayerQuestData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetPlayerQuestData - Tryed to call GetPlayerQuestData on Client!");
			return;
		}

		string playerUID = fileName.Substring(0, 44); //! Get playerUID from file name
		ExpansionQuestPlayerData questPlayerData = ExpansionQuestPlayerData.LoadPlayerQuestData(playerUID);
		if (questPlayerData)
		{
			QuestModulePrint(ToString() + "::GetPlayerQuestData - Adding player quest data for player with ID: " + playerUID);
			m_PlayerDatas.Insert(playerUID, questPlayerData);
		}

		QuestModulePrint(ToString() + "::GetPlayerQuestData - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadPlayerQuestData
	// Server
	// -----------------------------------------------------------
	private void LoadPlayerQuestData()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "LoadPlayerQuestData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::LoadPlayerQuestData - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::LoadPlayerQuestData - Tryed to call LoadPlayerQuestData on Client!");
			return;
		}

		array<string> playerQuestStateFiles = new array<string>;
		playerQuestStateFiles = ExpansionStatic.FindFilesInLocation(EXPANSION_QUESTS_PLAYERDATA_FOLDER);
		if (playerQuestStateFiles.Count() >= 0)
		{
			for (int i = 0; i < playerQuestStateFiles.Count(); i++)
			{
				GetPlayerQuestData(playerQuestStateFiles[i]);
			}
		}

		QuestModulePrint(ToString() + "::LoadPlayerQuestData - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadQuestNPCData
	// Server
	// -----------------------------------------------------------
	private void LoadQuestNPCData(array<string> questNPCFiles)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "LoadQuestNPCData").Add(sender).Add(ctx);
	#endif

		if (questNPCFiles.Count() >= 0)
		{
			for (int i = 0; i < questNPCFiles.Count(); i++)
			{
				GetQuestNPCData(questNPCFiles[i]);
			}
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadObjectivesData
	// Server
	// -----------------------------------------------------------
	private void LoadObjectivesData(array<string> objectiveFiles, string path)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "LoadObjectivesData").Add(sender).Add(ctx);
	#endif

		if (objectiveFiles.Count() >= 0)
		{
			for (int i = 0; i < objectiveFiles.Count(); i++)
			{
				GetObjectiveData(objectiveFiles[i], path);
			}
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetObjectiveData
	// Server
	// -----------------------------------------------------------
	private void GetObjectiveData(string fileName, string path)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetObjectiveData").Add(sender).Add(ctx);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetObjectiveData - Tryed to call GetObjectiveData on Client!");
			return;
		}

		ExpansionQuestObjectiveConfigBase objectiveBaseData = new ExpansionQuestObjectiveConfigBase();
		JsonFileLoader<ExpansionQuestObjectiveConfigBase>.JsonLoadFile(path + fileName, objectiveBaseData);
		if (!objectiveBaseData)
			return;

		int objectiveID = objectiveBaseData.GetID();
		int objectiveType = objectiveBaseData.GetObjectiveType();
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig = new ExpansionQuestObjectiveTravelConfig();
				JsonFileLoader<ExpansionQuestObjectiveTravelConfig>.JsonLoadFile(path + fileName, travelConfig);
				if (!travelConfig)
					return;

				m_TravelObjectivesConfigs.Insert(objectiveID, travelConfig);

				break;
			}
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = new ExpansionQuestObjectiveDeliveryConfig();
				JsonFileLoader<ExpansionQuestObjectiveDeliveryConfig>.JsonLoadFile(path + fileName, deliveryConfig);
				if (!deliveryConfig)
					return;

				m_DeliveryObjectivesConfigs.Insert(objectiveID, deliveryConfig);

				break;
			}
			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = new ExpansionQuestObjectiveTargetConfig();
				JsonFileLoader<ExpansionQuestObjectiveTargetConfig>.JsonLoadFile(path + fileName, targetConfig);
				if (!targetConfig)
					return;

				m_TargetObjectivesConfigs.Insert(objectiveID, targetConfig);

				break;
			}
			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = new ExpansionQuestObjectiveCollectionConfig();
				JsonFileLoader<ExpansionQuestObjectiveCollectionConfig>.JsonLoadFile(path + fileName, collectionConfig);
				if (!collectionConfig)
					return;

				m_CollectionObjectivesConfigs.Insert(objectiveID, collectionConfig);

				break;
			}
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureConfig = new ExpansionQuestObjectiveTreasureHuntConfig();
				JsonFileLoader<ExpansionQuestObjectiveTreasureHuntConfig>.JsonLoadFile(path + fileName, treasureConfig);
				if (!treasureConfig)
					return;

				m_TreasureHuntObjectivesConfigs.Insert(objectiveID, treasureConfig);

				break;
			}
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = new ExpansionQuestObjectiveAIPatrolConfig();
				JsonFileLoader<ExpansionQuestObjectiveAIPatrolConfig>.JsonLoadFile(path + fileName, aiPatrolConfig);
				if (!aiPatrolConfig)
					return;

				m_AIPatrolObjectivesConfigs.Insert(objectiveID, aiPatrolConfig);

				break;
			}
			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = new ExpansionQuestObjectiveAICampConfig();
				JsonFileLoader<ExpansionQuestObjectiveAICampConfig>.JsonLoadFile(path + fileName, aiCampConfig);
				if (!aiCampConfig)
					return;

				m_AICampObjectivesConfigs.Insert(objectiveID, aiCampConfig);

				break;
			}
			case ExpansionQuestObjectiveType.AIVIP:
			{
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig = new ExpansionQuestObjectiveAIVIPConfig();
				JsonFileLoader<ExpansionQuestObjectiveAIVIPConfig>.JsonLoadFile(path + fileName, aiVIPConfig);
				if (!aiVIPConfig)
					return;

				m_AIVIPObjectivesConfigs.Insert(objectiveID, aiVIPConfig);

				break;
			}
		#endif
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule LoadQuestData
	// Server
	// -----------------------------------------------------------
	private void LoadQuestData(array<string> questFiles)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "LoadQuestData").Add(sender).Add(ctx);
	#endif

		for (int i = 0; i < questFiles.Count(); i++)
		{
			GetQuestData(questFiles[i]);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCData
	// Server
	// -----------------------------------------------------------
	private void GetQuestNPCData(string fileName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetQuestNPCData").Add(sender).Add(ctx);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetQuestNPCData - Tryed to call GetQuestNPCData on Client!");
			return;
		}

		ExpansionQuestNpcData questNPCData = new ExpansionQuestNpcData();
		JsonFileLoader<ExpansionQuestNpcData>.JsonLoadFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName, questNPCData);
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
	private void GetQuestData(string fileName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetQuestData").Add(sender).Add(ctx);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::GetQuestData - Tryed to call GetQuestData on Client!");
			return;
		}

		ExpansionQuestConfig questData = new ExpansionQuestConfig();
		JsonFileLoader<ExpansionQuestConfig>.JsonLoadFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questData);
		if (questData)
		{
			QuestModulePrint(ToString() + "::GetQuestData - Adding quest data from file " + fileName);
			m_QuestConfigs.Insert(questData.GetID(), questData);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasCompletedQuest
	// 	Server & Client
	// ------------------------------------------------------------
	bool HasCompletedQuest(int questID, string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "HasCompletedQuest").Add(sender).Add(ctx);
	#endif

		ExpansionQuestPlayerData playerQuestData;
		if (IsMissionHost()) playerQuestData = GetPlayerQuestDataByUID(playerUID);
		else if (IsMissionClient()) playerQuestData = m_PlayerQuestData;

		if (playerQuestData)
		{
			int questState =  playerQuestData.GetQuestStateByQuestID(questID);
			if (questState == ExpansionQuestState.COMPLETED)
				return true;
		}

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
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetNPCQuests").Add(sender).Add(ctx);
	#endif

		array<ref ExpansionQuestConfig> npcQuests = new array<ref ExpansionQuestConfig>;
		ExpansionQuestNpcData questNPCData = GetQuestNPCDataByID(npcID);
		if (questNPCData)
		{
			for (int i = 0; i < questNPCData.GetQuestIDs().Count(); i++)
			{
				int questID = questNPCData.GetQuestIDs()[i];
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
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetPlayerQuests").Add(sender).Add(ctx);
	#endif

		array<ref ExpansionQuestConfig> playerQuests = new array<ref ExpansionQuestConfig>;
		ExpansionQuestPlayerData playerQuestData = GetPlayerQuestDataByUID(playerUID);
		if (playerQuestData)
		{
			for (int i = 0; i < playerQuestData.GetQuestStates().Count(); i++)
			{
				int questID = playerQuestData.GetQuestStates().GetKey(i);
				ExpansionQuestConfig questConfig = GetQuestConfigByID(questID);
				if (questConfig && !questConfig.IsAchivement())
				{
					//! We only want to add quests here that the player should see in the quest log (started or tunr-in quest state).
					if (!HasCompletedQuest(questID, playerUID) && !questConfig.IsRepeatable() || questConfig.IsRepeatable())
					{
						playerQuests.Insert(questConfig);
					}
				}
			}
		}

		return playerQuests;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule HasQuestAtNPC
	// Server & Client
	// ------------------------------------------------------------
	//! Will return true if player has any quests that can be accapted or can be completed at the given NPC
	//! Can also be used to check a single quest when a quest id is given as the 3th param.
	bool HasQuestAtNPC(int npcID, string playerUID, int questIDToCheck = -1)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "HasQuestAtNPC").Add(sender).Add(ctx);
	#endif

		ExpansionQuestNpcData questNPCData = GetQuestNPCDataByID(npcID);
		ExpansionQuestPlayerData playerQuestData;
		if (IsMissionHost()) playerQuestData = GetPlayerQuestDataByUID(playerUID);
		else if (IsMissionClient()) playerQuestData = m_PlayerQuestData;

		bool hasQuest = false;
		if (questNPCData && playerQuestData)
		{
			int questID;
			int questState;
			ExpansionQuestConfig questConfig;

			if (questIDToCheck == -1)
			{
				for (int i = 0; i < questNPCData.GetQuestIDs().Count(); i++)
				{
					questID = questNPCData.GetQuestIDs()[i];
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
						QuestModulePrint(ToString() + "::HasQuestAtNPC - QuestConditionsCheck - TRUE");
						hasQuest = true;
					}
					else
					{
						QuestModulePrint(ToString() + "::HasQuestAtNPC - QuestConditionsCheck - FALSE");
						hasQuest = false;
					}
				}
			}
			else
			{
				questID = questIDToCheck;
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
					QuestModulePrint(ToString() + "::HasQuestAtNPC - QuestConditionsCheck - TRUE");
					hasQuest = true;
				}
				else
				{
					QuestModulePrint(ToString() + "::HasQuestAtNPC - QuestConditionsCheck - FALSE");
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
	private bool QuestConditionsCheck(ExpansionQuestConfig questConfig, ExpansionQuestPlayerData playerQuestData, string playerUID,  int questState, int npcID)
	{
		if (questConfig.GetPreQuestID() != -1 && !HasCompletedQuest(questConfig.GetPreQuestID(), playerUID))
		{
			QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest has pre quest! Player [UID: " + playerUID + "] has not completed pre-quest for quest " + questConfig.GetID());
			return false;
		}

		if (!QuestStateCheck(questConfig, questState, npcID))
		{
			QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest state invalid!");
			return false;
		}

		/*if (QuestCooldownCheck(questConfig, playerQuestData))
		{
			QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest on cooldown!");
			return false;
		}*/

	#ifdef EXPANSIONMODHARDLINE
		if (questConfig.IsBanditQuest() || questConfig.IsHeroQuest())
		{
			ExpansionHardlineModule hardlineModule;
			if (Class.CastTo(hardlineModule, CF_ModuleCoreManager.Get(ExpansionHardlineModule)))
			{
				ExpansionHardlinePlayerData hardlinePlayerData;

				if (IsMissionHost()) hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(playerUID);
				else if (IsMissionClient()) hardlinePlayerData = hardlineModule.GetHardlineClientData();

				if (hardlinePlayerData)
				{
					int humanity = hardlinePlayerData.GetHumanity();
					QuestModulePrint(ToString() + "::QuestConditionsCheck - Humanity for Player [UID: " + playerUID + "]: " + humanity);
					if (questConfig.IsBanditQuest() && (humanity > -1000))
					{
						QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest is a bandit quest! Player [UID: " + playerUID + "] is not a bandit!");
						return false;
					}
					else if (questConfig.IsHeroQuest() && (humanity < 1000))
					{
						QuestModulePrint(ToString() + "::QuestConditionsCheck - Quest is a hero quest! Player [UID: " + playerUID + "] is not a hero!");
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
	private bool QuestStateCheck(ExpansionQuestConfig questConfig, int questState, int npcID)
	{
		if (questState == ExpansionQuestState.NONE && questConfig.GetQuestGiverID() != npcID)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: NONE - Npc " + npcID + " is not quest giver npc for quest with ID " + questConfig.GetID() + " - Quest giver ID: " + questConfig.GetQuestGiverID());
			return false;
		}
		else if (questState == ExpansionQuestState.STARTED && questConfig.GetQuestGiverID() != npcID)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: STARTED - Npc is not quest giver npc for quest with ID " + questConfig.GetID() + ".");
			return false;
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN && questConfig.GetQuestTurnInID() != npcID)
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: CAN_TURNIN - Npc is not quest turn in npc for quest with ID " + questConfig.GetID() + ".");
			return false;
		}
		else if (questState == ExpansionQuestState.COMPLETED && !questConfig.IsRepeatable())
		{
			QuestModulePrint(ToString() + "::QuestStateCheck - State: COMPLETED - Quest is completed!");
			return false;
		}

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule QuestCooldownCheck
	// Server & Client
	// ------------------------------------------------------------
	private bool QuestCooldownCheck(ExpansionQuestConfig questConfig, ExpansionQuestPlayerData playerQuestData)
	{
		if (!questConfig.IsDaylieQuest() && !questConfig.IsWeeklyQuest())
			return false;

		int daylie = 86400; //! Day in seconds
		int weekly = 604800; //! Week in seconds
		int remaining;

		int time = CF_Date.Now(true).GetTimestamp();
		if (questConfig.IsDaylieQuest())
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
	// ExpansionQuestModule OnAcceptQuest
	// Client
	// ------------------------------------------------------------
	//! Gets called when a player accepts a certain quest
	void AcceptQuestClient(ExpansionQuestConfig quest)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "AcceptQuestClient").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::AcceptQuestClient - Start");

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::AcceptQuestClient - Tryed to call AcceptQuestClient on Server!");
			return;
		}

		if (!GetClientQuestData())
		{
			Error(ToString() + "::AcceptQuestClient - Could not get players client quest data!");
			return;
		}

		CreateQuestInstance(quest.GetID());

		QuestModulePrint(ToString() + "::AcceptQuestClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnQuestObjectivesComplete
	// Server
	// ------------------------------------------------------------
	//! Gets called when a player completed all quest objectives for a certain quest
	void OnQuestObjectivesComplete(int questID, PlayerBase player)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesComplete").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::OnQuestObjectivesComplete - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Tryed to call OnQuestObjectivesComplete on Client!");
			return;
		}

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest from active quest array!");
				continue;
			}
			
			string playerUID = player.GetIdentity().GetId();
			if (quest.GetPlayerUID() != playerUID)
				continue;

			//! Get quest from active quest instances
			if (quest.GetQuestConfig().GetID() == questID)
				UpdateQuestStatesForQuestPlayers(quest, ExpansionQuestState.CAN_TURNIN);
		}

		QuestModulePrint(ToString() + "::OnQuestObjectivesComplete - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule OnQuestObjectivesIncomplete
	// Server
	// ------------------------------------------------------------
	void OnQuestObjectivesIncomplete(int questID, PlayerBase player)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesIncomplete").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::OnQuestObjectivesIncomplete - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::OnQuestObjectivesIncomplete - Tryed to call OnQuestObjectivesIncomplete on Client!");
			return;
		}

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::OnQuestObjectivesIncomplete - Could not get quest from active quest array!");
				continue;
			}

			string playerUID = player.GetIdentity().GetId();
			if (quest.GetPlayerUID() != playerUID)
				continue;
			
			//! Get quest from active quest instances
			if (quest.GetQuestConfig().GetID() == questID)
				UpdateQuestStatesForQuestPlayers(quest, ExpansionQuestState.STARTED);
		}

		//RemoveClientMarkers(questID, player.GetIdentity());

		QuestModulePrint(ToString() + "::OnQuestObjectivesIncomplete - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestTurnInQuestClient
	// Client
	// ------------------------------------------------------------
	//! Gets called when a player truns in a certain quest
	void RequestTurnInQuestClient(int questID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RequestTurnInQuestClient").Add(sender).Add(ctx);
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

		string playerUID = GetGame().GetPlayer().GetIdentity().GetId();
		if (playerUID == string.Empty)
			return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(questID);
		rpc.Write(playerUID);
		rpc.Send(NULL, ExpansionQuestModuleRPC.RequestCompleteQuest, false);

		QuestModulePrint(ToString() + "::RequestTurnInQuestClient - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RPC_RequestCompleteQuest
	// Called on Server
	// ------------------------------------------------------------
	private void RPC_RequestCompleteQuest(ParamsReadContext ctx, PlayerIdentity identity, Object target)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RPC_RequestCompleteQuest").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Tryed to call RPC_RequestCompleteQuest on Client!");
			return;
		}

		int questID;
		if (!ctx.Read(questID))
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Read questID");
			return;
		}

		string playerUID;
		if (!ctx.Read(playerUID))
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Read playerUID");
			return;
		}

		if (identity.GetId() != playerUID)
		{
			Error(ToString() + "::RPC_RequestCompleteQuest - Sender is not quest player!");
			return;
		}

		RequestCompleteQuestServer(questID, playerUID, identity);

		QuestModulePrint(ToString() + "::RPC_RequestCompleteQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RequestCompleteQuestServer
	// Called on Server
	// ------------------------------------------------------------
	void RequestCompleteQuestServer(int questID, string playerUID, PlayerIdentity identity, bool isAutoComplete = false)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RequestCompleteQuestServer").Add(sender).Add(ctx);
	#endif

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::RequestCompleteQuestServer - Could not get quest from active quest array!");
				continue;
			}

			//! Get quest from active quest instances
			if (quest.GetQuestConfig().GetID() == questID && quest.GetPlayerUID() == playerUID)
			{
				if (quest.CanCompeteQuest())
				{
					CompleteQuest(quest, playerUID, identity, isAutoComplete);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CompleteQuest
	// Called on Server
	// ------------------------------------------------------------
	private void CompleteQuest(ExpansionQuest quest, string playerUID, PlayerIdentity identity, bool isAutoComplete = false)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CompleteQuest").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CompleteQuest - Start");

		if (!quest)
		{
			Error(ToString() + "::CompleteQuest - Could not get active quest!");
			return;
		}

		quest.OnQuestTurnIn();
		RemoveClientMarkers(quest.GetQuestConfig().GetID(), identity);

		UpdateQuestPlayersObjectiveData(quest);
		UpdateQuestStatesForQuestPlayers(quest, ExpansionQuestState.COMPLETED);
		//UpdatePlayerQuests(quest);

	#ifdef EXPANSIONMODAI
		if (!isAutoComplete)
		{
			ExpansionQuestNpcAIBase npc = GetQuestNPCAIByID(quest.GetQuestConfig().GetQuestTurnInID());
			if (npc)
			{
				EmoteManager npcEmoteManager = npc.GetEmoteManager();
				if (!npcEmoteManager.IsEmotePlaying())
				{
					npcEmoteManager.PlayEmote(EmoteConstants.ID_EMOTE_CLAP);
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(npcEmoteManager.ServerRequestEmoteCancel, 2000);
				}
			}
		}
	#endif

		QuestModulePrint(ToString() + "::CompleteQuest - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule PlayerQuestInit
	// Server
	// ------------------------------------------------------------
	//! Handles reinitialisation of quests for a player from presistent data
	//! We need to get and handle the presistent quest progress and objective data of the player
	//! so the progress of the quest objectives contiues and no quest progress is lost.
	private void PlayerQuestsInit(ExpansionQuestPlayerData playerData, string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "PlayerQuestsInit").Add(sender).Add(ctx);
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

		if (playerUID == string.Empty)
		{
			Error(ToString() + "::PlayerQuestsInit - Could not get player UID!");
			return;
		}

		QuestModulePrint(ToString() + "::PlayerQuestsInit - Player UID: " + playerUID);

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! Check for all active group quests the player can participate if he is member of a group
		//! 
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	#ifdef EXPANSIONMODGROUPS
		array<int> activeGroupQuestIDs = new array<int>;

		ExpansionPartyModule partyModule;
		if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			Error(ToString() + "::PlayerQuestsInit - Could not get party module!");
			return;
		}

		ExpansionPartyPlayerData playerGroupData = partyModule.GetPartyPlayerData(playerUID);
		if (!playerGroupData)
		{
			QuestModulePrint(ToString() + "::PlayerQuestsInit - Could not get players group data for UID: " + playerUID + ". Player is not in a group!");
		}

		for (int q = 0; q < m_ActiveQuests.Count(); q++)
		{
			ExpansionQuest activeQuest = m_ActiveQuests[q];
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
			}
		}
	#endif

		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		//! New quest creation
		// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		if (playerData)
		{
			for (int i = 0; i < playerData.GetQuestStates().Count(); i++)
			{
				int questID = playerData.GetQuestStates().GetKey(i);
				int questState = playerData.GetQuestStates().Get(questID);

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

				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! If there is a active group quest instance for the player we want him to get the current state
				//! and needed quest data so he can participate on the quest even if he was offline while the quest
				//! has been accepted by a other group member.
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				for (int a = 0; a < m_ActiveQuests.Count(); a++)
				{
					ExpansionQuest activeQuestInstance = m_ActiveQuests[q];
					if (!activeQuestInstance)
						continue;
					
					if (!activeQuestInstance.IsGroupQuest())
						continue;
					
					if (!activeQuestInstance.IsQuestGroupMember(playerUID))
						continue;
					
					QuestModulePrint(ToString() + "::PlayerQuestsInit - There is a active group quest instance for this player! Add quest.");
					//! Make sure player has the correct quest state for this quest in his quest data.
					playerData.UpdateQuestState(activeQuestInstance.GetQuestConfig().GetID(), activeQuestInstance.GetQuestState());
					playerData.Save(playerUID);

					activeQuestInstance.OnGroupMemberJoined(playerUID);
				}
			#endif

				if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
				{
					QuestModulePrint(ToString() + "::PlayerQuestsInit - Create new quest instance for quest: " + questID + " - Creator UID: " + playerUID);
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Create new quest config. instance for the quest
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					ExpansionQuestConfig configInstance = GetQuestConfigByID(questID);

					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! For now dont reinit ai quests
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				/*#ifdef EXPANSIONMODAI
					if (configInstance.GetType() == ExpansionQuestType.AICAMP || configInstance.GetType() == ExpansionQuestType.AIPATROL)
					{
						playerData.UpdateQuestState(configInstance.GetID(), ExpansionQuestState.NONE);
						playerData.Save(playerUID);
						continue;
					}
				#endif*/

					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Don't recreate archiment quests here!
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					if (configInstance.IsAchivement())
						continue;

					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Create new quest config instance
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					ExpansionQuestConfig newConfigInstance = new ExpansionQuestConfig();
					if (!Class.CastTo(newConfigInstance, configInstance))
					{
						Error(ToString() + "::PlayerQuestsInit - Could not create new quest config instance for quest creation!");
						continue;
					}
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Create quest instance and set current quest status from presistent player quest data
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					ExpansionQuest quest = new ExpansionQuest(this);
					
					quest.SetPlayerUID(playerUID); //! Set the quest player!

				#ifdef EXPANSIONMODGROUPS
					if (newConfigInstance.IsGroupQuest() && playerGroupData)
					{
						ExpansionPartyData groupData = playerGroupData.GetParty();
						if (!groupData)
						{
							Error(ToString() + "::PlayerQuestsInit - Could not get group data from players group data!");
							delete quest;
							continue;
						}

						// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
						//! Check if player is the group owner otherwise skip group quest creation
						// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
						if (groupData.GetOwnerUID() != playerUID)
						{
							QuestModulePrint(ToString() + "::PlayerQuestsInit - Player is not the owner of his group! Skip quest creation!");
							delete quest;
							continue;
						}

						QuestModulePrint(ToString() + "::PlayerQuestsInit - Player is owner of group! Set group quest parameters for quest: " + questID);
						//! Set the quest group!
						quest.SetGroup(playerGroupData.GetParty());
					}
					else if (newConfigInstance.IsGroupQuest() && !playerGroupData)
					{
						QuestModulePrint(ToString() + "::PlayerQuestsInit - Can't create instance for group quest! Group does not exist anymore! Skip quest creation!");
						delete quest;
						continue;
					}
				#endif

					quest.SetQuestConfig(newConfigInstance);
					quest.SetQuestState(questState);

					//! Get quest objectives progress from presistent player quest data
					GetObjectiveProgressFromPlayerData(playerData, quest);
					m_ActiveQuests.Insert(quest);

					//! Feed presistent quest player data with current quest objectives data
					UpdateQuestPlayersObjectiveData(quest);
					 //! Update and sync quest states from presistent quest player to all quest clients
					UpdateQuestStatesForQuestPlayers(quest, quest.GetQuestState());

					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Continues quest 3 seconds after player has connected!
					//! We do this here because otherwise there is a high chance we cant get the player entity
					//! while we reinitalise the quest and so the quest cant be initialised correctly.
					//! ToDo: Replace this with a proper event handler and fire all quest reinitalisations properly
					//! after we can get the player entity or his group?!
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(quest.OnQuestContinue, 3000, false);
				}
			}

			//! Create server achievement quests
			//CreateAchievementQuests(playerData, playerUID); //! UNCOMMENT ME LATER !!!!!!!!!!!!!!
		}

		QuestModulePrint(ToString() + "::PlayerQuestsInit - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CreateAchievementQuests
	// Called on server
	// ------------------------------------------------------------
	private void CreateAchievementQuests(ExpansionQuestPlayerData playerData, string playerUID)
	{
		QuestModulePrint(ToString() + "::CreateAchievementQuests - Start");

		if (!playerData)
		{
			Error(ToString() + "::CreateAchievementQuests - Could not get player quest data!");
			return;
		}

		foreach (int questID, ExpansionQuestConfig questConfig: m_QuestConfigs)
		{
			if (questConfig)
			{
				if (!questConfig.IsAchivement())
				{
					QuestModulePrint(ToString() + "::CreateAchievementQuests - Quest with ID: " + questID +" is not a achievement quest. Skip!");
					continue;
				}

				int questState = playerData.GetQuestStateByQuestID(questID);
				ExpansionQuestConfig newConfig;
				ExpansionQuest quest;

				//! If player has no quest state for this achievement quest we create the quest and update the players presistent quest data
				if (questState == ExpansionQuestState.NONE)
				{
					//! Create new quest config instance
					newConfig = new ExpansionQuestConfig();
					if (!Class.CastTo(newConfig, questConfig))
					{
						Error(ToString() + "::CreateAchievementQuests - Could not create new quest config instance for quest creation!");
						continue;
					}

					newConfig.QuestDebug();

					//! Create quest
					QuestModulePrint(ToString() + "::CreateAchievementQuests - Create new achievement quest for quest ID: " + questID);
					quest = new ExpansionQuest(this);
					quest.SetPlayerUID(playerUID);
					quest.SetQuestConfig(newConfig);
					m_ActiveQuests.Insert(quest);

					//! Update player data
					UpdateQuestStatesForQuestPlayers(quest, ExpansionQuestState.STARTED);

					UpdateQuestPlayersObjectiveData(quest);

					//UpdatePlayerQuests(quest);

					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(quest.OnQuestStart, 3000, false);

					playerData.QuestDebug();

					quest.QuestDebug();
				}
				//! If the player is already on this archiment quest recreate it and update the progress from the players presistent quest data
				else if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
				{
					QuestModulePrint(ToString() + "::CreateAchievementQuests - Create achievement quest for quest ID: " + questID + " and add progress from player quest data [UID: " + playerUID + "]");
					newConfig = new ExpansionQuestConfig();
					if (!Class.CastTo(newConfig, questConfig))
					{
						Error(ToString() + "::PlayerQuestsInit - Could not create new quest config instance for quest creation!");
						continue;
					}

					newConfig.QuestDebug();

					//! Create quest
					quest = new ExpansionQuest(this);
					quest.SetPlayerUID(playerUID);
					quest.SetQuestConfig(newConfig);
					m_ActiveQuests.Insert(quest);

					//! Get quest objectives progress from presistent player quest data
					GetObjectiveProgressFromPlayerData(playerData, quest);

					playerData.QuestDebug();

					quest.QuestDebug();

					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Continues quest 3 seconds after player has connected!
					//! We do this here because otherwise there is a high chance we cant get the player entity
					//! while we reinitalise the quest and so the quest cant be initialised correctly.
					//! ToDo: Replace this with a proper event handler and fire all quest reinitalisations properly
					//! after we can get the player entity or his group?!
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(quest.OnQuestContinue, 3000, false);
				}
			}
		}

		QuestModulePrint(ToString() + "::CreateAchievementQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule PlayerQuestInit
	// Server
	// ------------------------------------------------------------
	//! Handles reinitialisation of quests objectives for a player from his presistent quest data
	//! We need to get and handle the presistent objective data of the player so the progress of the quest objectives continues and no quest progress is lost.
	private void GetObjectiveProgressFromPlayerData(ExpansionQuestPlayerData playerData, ExpansionQuest quest)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "GetObjectiveProgressFromPlayerData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Start");

		if (!playerData)
		{
			Error(ToString() + "::GetObjectiveProgressFromPlayerData - Could not get player quest data!");
			return;
		}

		QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objectives: " + playerData.GetQuestObjectives());
		QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objectives count: " + playerData.GetQuestObjectives().Count());
		
		for (int i = 0; i < playerData.GetQuestObjectives().Count(); i++)
		{
			ExpansionQuestObjectivePlayerData objectiveData = playerData.GetQuestObjectives()[i];
			if (objectiveData) //! FFS
			{
				if (objectiveData.GetQuestID() != quest.GetQuestConfig().GetID())
					continue;

				if (!quest.ObjectivesCreated())
				{
					Error(ToString() + "::GetObjectiveProgressFromPlayerData - Quest objectives are not created!");
					continue;
				}

				QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective data type: " + objectiveData.GetObjectiveType());
				QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective data index: " + objectiveData.GetObjectiveIndex());

				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Get the correct objective event base cass with the objective index
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//ExpansionQuestObjectiveEventBase objective = quest.GetObjectives()[i];
				int currentActiveObjectiveIndex = 0;
				for (int j = 0; j <  quest.GetObjectives().Count(); j++)
				{
					ExpansionQuestObjectiveEventBase objective = quest.GetObjectives()[i];
					if (objective && objective.GetIndex() == objectiveData.GetObjectiveIndex())  //! FFS
					{
						objective.SetIndex(objectiveData.GetObjectiveIndex());
						objective.SetCompleted(objectiveData.IsCompleted());
						objective.SetIsActive(objectiveData.IsActive());
						
						if (objectiveData.IsActive())
							currentActiveObjectiveIndex = objectiveData.GetObjectiveIndex();
						
						switch (objectiveData.GetObjectiveType())
						{
							case ExpansionQuestObjectiveType.TARGET:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: TARGET");
								ExpansionQuestObjectiveTargetEvent target = ExpansionQuestObjectiveTargetEvent.Cast(objective);
								if (target)
								{
									// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
									//! Get kill count progress from presistent data
									// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
									target.SetCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: TARGET");
								}
								break;
							}
							case ExpansionQuestObjectiveType.TRAVEL:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: TRAVEL");
								ExpansionQuestObjectiveTravelEvent travel = ExpansionQuestObjectiveTravelEvent.Cast(objective);
								if (travel)
								{
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: TRAVEL");
								}
								break;
							}
							case ExpansionQuestObjectiveType.DELIVERY:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: DELIVERY");
								ExpansionQuestObjectiveDeliveryEvent delivery = ExpansionQuestObjectiveDeliveryEvent.Cast(objective);
								if (delivery)
								{
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: DELIVERY");
								}
								break;
							}
							case ExpansionQuestObjectiveType.COLLECT:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: COLLECT");
								ExpansionQuestObjectiveCollectionEvent collection = ExpansionQuestObjectiveCollectionEvent.Cast(objective);
								if (collection)
								{
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: COLLECT");
								}
								break;
							}
							case ExpansionQuestObjectiveType.TREASUREHUNT:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: TREASUREHUNT");
								ExpansionQuestObjectiveTreasureHuntEvent treasureHunt = ExpansionQuestObjectiveTreasureHuntEvent.Cast(objective);
								if (treasureHunt)
								{
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: TREASUREHUNT");
								}
								break;
							}
						#ifdef EXPANSIONMODAI
							case ExpansionQuestObjectiveType.AIPATROL:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: AIPATROL");
								ExpansionQuestObjectiveAIPatrolEvent aiPatrolHunt = ExpansionQuestObjectiveAIPatrolEvent.Cast(objective);
								if (aiPatrolHunt)
								{
									aiPatrolHunt.SetKillCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: AIPATROL");
								}
								break;
							}
							case ExpansionQuestObjectiveType.AICAMP:
							{
								QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective event type: AICAMP");
								ExpansionQuestObjectiveAICampEvent aiCampHunt = ExpansionQuestObjectiveAICampEvent.Cast(objective);
								if (aiCampHunt)
								{
									aiCampHunt.SetKillCount(objectiveData.GetObjectiveCount());
									QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - Objective progress added: AICAMP");
								}
								break;
							}
						#endif
						}
					}
				}
				
				quest.SetCurrentObjectiveIndex(currentActiveObjectiveIndex);
			}
		}
		QuestModulePrint(ToString() + "::GetObjectiveProgressFromPlayerData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule CleanupPlayerQuests
	// Server
	// ------------------------------------------------------------
	private void CleanupPlayerQuests(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CleanupPlayerQuests").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - Start");

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::CleanupPlayerQuests - Tryed to call CleanupPlayerQuests on Client!");
			return;
		}

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest)
			{
				Error(ToString() + "::CleanupPlayerQuests - Could not get quest!");
				continue;
			}

			if (quest.GetPlayerUID() == playerUID)
			{
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				//! Update and save the presistent player quest data
				// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
				ExpansionQuestPlayerData playerQuestData = GetPlayerQuestDataByUID(playerUID);
				if (playerQuestData) //! Here it works?!
				{
					if (quest.GetQuestState() == ExpansionQuestState.STARTED || quest.GetQuestState() == ExpansionQuestState.CAN_TURNIN)
					{
						int questID = quest.GetQuestConfig().GetID();
						ExpansionQuestTimestampPlayerData timestamp;
						if (playerQuestData.HasCooldownOnQuest(questID, timestamp))
						{
							timestamp.SetTimestamp(0);
						}
					}

					playerQuestData.UpdateQuestState(quest.GetQuestConfig().GetID(), quest.GetQuestState());
					UpdateQuestPlayersObjectiveData(quest);

				#ifdef EXPANSIONMODGROUPS
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					//! Dont delete the quest if its a group quest and a other group member is still online
					// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
					if (quest.IsGroupQuest())
					{
						if (quest.IsOtherGroupMemberOnline(playerUID))
							continue;
					}
				#endif

					//! Delete quest
					quest.OnQuestCleanup();
					m_ActiveQuests.Remove(i);
					delete quest;
				}
			}
		}

		QuestModulePrint(ToString() + "::CleanupPlayerQuests - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule RemovePlayerQuestData
	// Server
	// ------------------------------------------------------------
	private void RemovePlayerQuestData(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "RemovePlayerQuestData").Add(sender).Add(ctx);
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

	// ------------------------------------------------------------
	// ExpansionQuestModule OnEntityKilled
	// Server
	// ------------------------------------------------------------
	//! Plan here is to get all the active quests of the given player uid on the server
	//! to check there quest objectives for any target objective types.
	//! If we find one we check if we can add progress to that quest objective by checking
	//! the killed enity type name and compare it with the target objective class name.
	void OnEntityKilled(EntityAI source, EntityAI killSource, string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnEntityKilled").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::OnEntityKilled - Start");

		string sourceType = source.ClassName();
		string killSourceType = killSource.GetType();
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::OnEntityKilled - Tryed to call OnEntityKilled on Client!");
			return;
		}

		for (int i = 0; i < m_ActiveQuests.Count(); i++)
		{
			ExpansionQuest quest = m_ActiveQuests[i];
			if (!quest.IsGroupQuest() && quest.GetPlayerUID() != playerUID)
				continue;
			
		#ifdef EXPANSIONMODGROUPS
			if (quest.IsGroupQuest() && !quest.IsQuestGroupMember(playerUID))
				continue;
		#endif
			
			if (quest.IsCompeleted() || quest.GetQuestState() >= ExpansionQuestState.COMPLETED)
				continue;

			for (int j = 0; j < quest.GetObjectives().Count(); j++)
			{
				ExpansionQuestObjectiveEventBase objective = quest.GetObjectives()[j];
				if (!objective)
				{
					Error(ToString() + "::OnEntityKilled - Could not get quest objective!");
					continue;
				}

				//! Run thrue all possible objective types
				switch (objective.GetObjectiveConfig().GetObjectiveType())
				{
					case ExpansionQuestObjectiveType.TARGET:
					{
						ExpansionQuestObjectiveTargetEvent targetObjective;
						if (Class.CastTo(targetObjective, objective))
							targetObjective.OnEntityKilled(sourceType, killSourceType);
					}
					break;
					
				#ifdef EXPANSIONMODAI
					case ExpansionQuestObjectiveType.AICAMP:
					{
						ExpansionQuestObjectiveAICampEvent aiCampObjective;
						if (Class.CastTo(aiCampObjective, objective))
							aiCampObjective.OnEntityKilled(sourceType, killSourceType);
					}
					break;
					
					case ExpansionQuestObjectiveType.AIPATROL:
					{
						ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
						if (Class.CastTo(aiPatrolObjective, objective))
							aiPatrolObjective.OnEntityKilled(sourceType, killSourceType);
					}
					break;
					
					case ExpansionQuestObjectiveType.AIVIP:
					{
						ExpansionQuestObjectiveAIVIPEvent aiVIPObjective;
						if (Class.CastTo(aiVIPObjective, objective))
							aiVIPObjective.OnEntityKilled(source, killSource);
					}
					break;
				#endif
				}
			}

			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			//! Update players quest objective data
			// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
			UpdateQuestPlayersObjectiveData(quest);
		}

		QuestModulePrint(ToString() + "::OnEntityKilled - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestPlayersObjectiveData
	// Server
	// ------------------------------------------------------------
	//! Updates quest objective data for all quest players based on given active quest objectives
	//! We use this data to syncronise it to the player and to display his objective states or to
	//! add the objective data back to the active quest objectives of the player after a server restart or reconnect.
	void UpdateQuestPlayersObjectiveData(ExpansionQuest quest)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "UpdateQuestPlayersObjectiveData").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestPlayersObjectiveData - Start");
		quest.QuestDebug();
		
		if (!quest)
		{
			Error(ToString() + "::UpdateQuestPlayersObjectiveData - Could not get quest!");
			return;
		}

	#ifdef EXPANSIONMODGROUPS
		ExpansionPartyModule partyModule;
		if (!Class.CastTo(partyModule,CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			Error(ToString() + "::UpdateQuestPlayersObjectiveData - Could not get party module!");
			return;
		}
	#endif

		if (!quest.IsGroupQuest())
		{
			UpdatePlayerQuestObjectiveData(quest, quest.GetPlayerUID());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			ExpansionPartyData questGroup = partyModule.GetPartyByID(quest.GetGroupID());
			if (!questGroup)
			{
				Error(ToString() + "::UpdateQuestPlayersObjectiveData - Could not get group data!");
				return;
			}

			for (int j = 0; j < questGroup.GetPlayers().Count(); j++)
			{
				ExpansionPartyPlayerData playerGroupData = questGroup.GetPlayers()[j];
				if (!playerGroupData)
				{
					Error(ToString() + "::UpdateQuestPlayersObjectiveData - Could not get group player data!");
					continue;
				}

				string playerUID = playerGroupData.GetID();
				UpdatePlayerQuestObjectiveData(quest, playerUID);
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestPlayersObjectiveData - End");
	}

	private void UpdatePlayerQuestObjectiveData(ExpansionQuest quest, string playerUID)
	{
		QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Start");

		if (!quest)
		{
			Error(ToString() + "::UpdatePlayerQuestObjectiveData - Could not get quest!");
			return;
		}

		ExpansionQuestPlayerData playerQuestData = GetPlayerQuestDataByUID(playerUID);
		if (playerQuestData) //! FFS
		{
			array<ref ExpansionQuestObjectivePlayerData> newObjectivePlayerData = new array<ref ExpansionQuestObjectivePlayerData>;
			for (int i = 0; i < quest.GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase currentObjective = quest.GetObjectives()[i];
				if (currentObjective) //! FFS
				{
					QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Try update from objective event base: " + currentObjective.ToString());
					ExpansionQuestObjectivePlayerData objectiveData = new ExpansionQuestObjectivePlayerData();
					objectiveData.SetQuestID(quest.GetQuestConfig().GetID());
					objectiveData.SetObjectiveType(currentObjective.GetType());
					objectiveData.SetObjectiveIndex(currentObjective.GetIndex());
					objectiveData.SetActive(currentObjective.IsActive());
					objectiveData.SetCompleted(currentObjective.IsCompleted());
					
					int objectiveType = currentObjective.GetObjectiveConfig().GetObjectiveType();

					switch (objectiveType)
					{
						case ExpansionQuestObjectiveType.TARGET:
						{
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: TARGET");
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
								objectiveData.SetObjectiveAmount(collectionObjective.GetAmmount());
								objectiveData.SetObjectiveCount(collectionObjective.GetCount());
							}
						}
						break;
						
						case ExpansionQuestObjectiveType.DELIVERY:
						{
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: DELIVERY");
							ExpansionQuestObjectiveDeliveryEvent deliveryObjective;
							if (Class.CastTo(deliveryObjective, currentObjective))
							{
								int questNPCID = quest.GetQuestConfig().GetQuestTurnInID();

								ExpansionQuestNpcBase npc;
							#ifdef EXPANSIONMODAI
								ExpansionQuestNpcAIBase npcAI;
							#endif
								vector npcPos;
								if (Class.CastTo(npc, GetQuestNPCByID(quest.GetQuestConfig().GetQuestTurnInID())))
								{
									npcPos = npc.GetPosition();
								}
							#ifdef EXPANSIONMODAI
								else if (Class.CastTo(npcAI, GetQuestNPCAIByID(quest.GetQuestConfig().GetQuestTurnInID())))
								{
									npcPos = npcAI.GetPosition();
								}
							#endif

								objectiveData.SetObjectivePosition(npcPos);
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
						
					#ifdef EXPANSIONMODAI
						case ExpansionQuestObjectiveType.AICAMP:
						{
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: AICAMP");
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
							QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Objective event type is: AIPATROL");
							ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjective;
							if (Class.CastTo(aiPatrolObjective, currentObjective))
							{
								objectiveData.SetObjectiveAmount(aiPatrolObjective.GetAmount());
								objectiveData.SetObjectiveCount(aiPatrolObjective.GetCount());
							}
						}
						break;
					#endif
					}

					newObjectivePlayerData.Insert(objectiveData);
					QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - Added new objective data for type: " + objectiveType);
				}
			}

			playerQuestData.UpdatedObjectives(quest.GetQuestConfig().GetID(), newObjectivePlayerData);
			playerQuestData.Save(playerUID);
			playerQuestData.QuestDebug();

			PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
			if (player)
				SendPlayerQuestData(player.GetIdentity());
		}

		QuestModulePrint(ToString() + "::UpdatePlayerQuestObjectiveData - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestStatesForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Sends updated quest state data to all quest clients
	private void UpdateQuestStatesForQuestPlayers(ExpansionQuest quest, int state)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "UpdateQuestStatesForQuestPlayers").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestStatesForQuestPlayers - Start");

		if (!quest.IsGroupQuest())
		{
			QuestModulePrint(ToString() + "::UpdateQuestStatesForQuestPlayers - Quest is a normal quest");
			ExpansionQuestPlayerData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
			if (playerQuestData)
			{
				playerQuestData.UpdateQuestState(quest.GetQuestConfig().GetID(), state);
				playerQuestData.Save(quest.GetPlayerUID());
				playerQuestData.QuestDebug();

				PlayerBase player = PlayerBase.GetPlayerByUID(quest.GetPlayerUID());
				if (player)
					SendPlayerQuestData(player.GetIdentity());
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (quest.IsGroupQuest())
		{
			if (!quest.GetGroup())
			{
				Error(ToString() + "::UpdateQuestStatesForQuestPlayers - Quest is a group quest but could not get group from quest!");
				return;
			}

			for (int i = 0; i < quest.GetGroup().GetPlayers().Count(); ++i)
			{
				ExpansionPartyPlayerData playerGroupData = quest.GetGroup().GetPlayers()[i];
				if (!playerGroupData)
				{
					Error(ToString() + "::UpdateQuestStatesForQuestPlayers - Quest is a group quest but could not get group member data!");
					continue;
				}

				QuestModulePrint(ToString() + "::UpdateQuestStatesForQuestPlayers - Update player quest data for UID: " + playerGroupData.GetID());
				ExpansionQuestPlayerData groupPlayerQuestData = GetPlayerQuestDataByUID(playerGroupData.GetID());
				if (groupPlayerQuestData)
				{
					groupPlayerQuestData.UpdateQuestState(quest.GetQuestConfig().GetID(), state);
					groupPlayerQuestData.Save(playerGroupData.GetID());
					groupPlayerQuestData.QuestDebug();

					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
					if (groupPlayer)
					{
						SendPlayerQuestData(groupPlayer.GetIdentity());
					}
				}
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestStatesForQuestPlayers - End");
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestTimestampForQuestPlayers
	// Server
	// ------------------------------------------------------------
	//! Sends updated quest time data to all quest clients
	private void UpdateQuestTimestampForQuestPlayers(ExpansionQuest quest, int time)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "UpdateQuestTimestampForQuestPlayers").Add(sender).Add(ctx);
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - Start");

		if (!quest.IsGroupQuest())
		{
			ExpansionQuestPlayerData playerQuestData = GetPlayerQuestDataByUID(quest.GetPlayerUID());
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
	#ifdef EXPANSIONMODGROUPS
		else
		{
			for (int i = 0; i < quest.GetGroup().GetPlayers().Count(); ++i)
			{
				ExpansionPartyPlayerData playerGroupData = quest.GetGroup().GetPlayers()[i];
				if (!playerGroupData)
					continue;

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (!groupPlayer)
					continue;

				ExpansionQuestPlayerData groupsPlayerQuestData = GetPlayerQuestDataByUID(groupPlayer.GetIdentity().GetId());
				if (!groupsPlayerQuestData)
					continue;

				groupsPlayerQuestData.UpdateQuestTimestamp(quest.GetQuestConfig().GetID(), time);
				groupsPlayerQuestData.Save(groupPlayer.GetIdentity().GetId());
				groupsPlayerQuestData.QuestDebug();
				SendPlayerQuestData(groupPlayer.GetIdentity());
			}
		}
	#endif

		QuestModulePrint(ToString() + "::UpdateQuestTimestampForQuestPlayers - End");
	}

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
					if (!quest)
					{
						Error(ToString() + "::OnUpdate - Could not get active quest! Quest was deleted?!");
						continue;
					}

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
			else
			{
				m_CurrentQuestTick = 0;
			}

			m_UpdateQueueTimer = 0.0;
		}

	#ifdef EXPANSIONMODAI
		m_UpdateNpcQueueTimer += update.DeltaTime;
		if (m_UpdateNpcQueueTimer >= UPDATE_NPC_TIME)
		{
			if (m_QuestNPCAIEntities.Count() > 0)
			{
				for (int n = 0; n < UPDATE_NPC_PER_TICK; ++n)
				{
					if (m_CurrentNpcTick >= m_QuestNPCAIEntities.Count())
						m_CurrentNpcTick = 0;

					ExpansionQuestNpcAIBase npc = m_QuestNPCAIEntities.GetElement(n);
					if (!npc)
						continue;

					npc.PlayEmote();
					npc.FixPositionAndOrientation();

					m_CurrentNpcTick++;

					if (m_CurrentNpcTick == m_QuestNPCAIEntities.Count())
						break;
				}
			}
			else
			{
				m_CurrentNpcTick = 0;
			}

			m_UpdateNpcQueueTimer = 0.0;
		}
	#endif
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetClientQuestData
	// Client
	// ------------------------------------------------------------
	ExpansionQuestPlayerData GetClientQuestData()
	{
		return m_PlayerQuestData;
	}

	// ------------------------------------------------------------
	// ExpansionQuestModule GetClientQuestNPCData
	// Client
	// ------------------------------------------------------------
	ExpansionQuestNpcData GetClientQuestNPCData()
	{
		return m_ClientQuestNPC;
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
	// ExpansionQuestModule GetCurrentClientQuestNPCData
	// Client
	// -----------------------------------------------------------
	ExpansionQuestNpcData GetCurrentClientQuestNPCData()
	{
		return m_ClientQuestNPC;
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetQuestNPCDataByID
	// Server &  client
	// -----------------------------------------------------------
	ExpansionQuestNpcData GetQuestNPCDataByID(int id)
	{
		return m_QuestsNPCs.Get(id);
	}

	// -----------------------------------------------------------
	// ExpansionQuestModule GetPlayerQuestDataByUID
	// Server
	// -----------------------------------------------------------
	ExpansionQuestPlayerData GetPlayerQuestDataByUID(string playerUID)
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
	ExpansionQuestNpcAIBase GetQuestNPCAIByID(int id)
	{
		return m_QuestNPCAIEntities.Get(id);
	}
#endif

	// ------------------------------------------------------------
	// ExpansionQuestModule UpdateQuestStatesForQuestPlayers
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
	ExpansionQuestNpcBase GetQuestNPCByID(int id)
	{
		return m_QuestNPCEntities.Get(id);
	}
	
	// ------------------------------------------------------------
	// ExpansionQuestModule CreateQuestInstance
	// Server
	// ------------------------------------------------------------
	ExpansionQuest CreateQuestInstance(string className)
	{
		return ExpansionQuest.Cast(className.ToType().Spawn());
	}
 };