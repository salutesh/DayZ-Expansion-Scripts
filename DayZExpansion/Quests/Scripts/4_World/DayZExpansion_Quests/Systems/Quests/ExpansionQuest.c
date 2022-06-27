/**
 * ExpansionQuest.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Quest instance that handles all the quest events
class ExpansionQuest
{
	private ExpansionQuestModule m_QuestModule;

	private ExpansionQuestConfig Config;
	private int State = ExpansionQuestState.NONE;
	private ref array<Object> QuestItems = new array<Object>;	//! Normal items the player will recieve on quest start.
	private ref array<ref ExpansionQuestObjectiveEventBase> QuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;	//! Quest objectives
	private PlayerBase m_Player;
	private string m_PlayerUID;
	private bool IsCompeleted = false;
	private bool m_Initialized = false;

	private float m_UpdateQueueTimer; //! Server update que timer
	private int m_CurrentObjectiveTick;
	private const float UPDATE_TICK_TIME = 1.0; // refreshes 50 quests objectives every ten seconds (1 every sec.)
	private const int UPDATE_OBJECTIVES_PER_TICK = 5;

	private bool m_IsGroupQuest = false;
#ifdef EXPANSIONMODGROUPS
	private ExpansionPartyData m_Group;
	private int m_GroupID = -1;
#endif

	private int m_ObjectiveIndex = 0;
	private bool m_ObjectivesCreated = false;

	private int m_CurrentObjectiveIndex = -1;
	
	// -----------------------------------------------------------
	// ExpansionQuest Constructor
	// -----------------------------------------------------------
	void ExpansionQuest(ExpansionQuestModule module)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "ExpansionQuest").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::ExpansionQuest - Start");

		m_QuestModule = module;

		QuestPrint(ToString() + "::ExpansionQuest - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest Desctructor
	// -----------------------------------------------------------
	void ~ExpansionQuest()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "~ExpansionQuest").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::~ExpansionQuest - Start");

		m_Player = NULL;
		CleanupQuestItems();
		QuestItems.Clear();
		QuestObjectives.Clear();

		QuestPrint(ToString() + "::~ExpansionQuest - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest SetQuestConfig
	// -----------------------------------------------------------
	void SetQuestConfig(ExpansionQuestConfig config)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SetQuestConfig").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::SetQuestConfig - Start");

		SetConfig(config);
		Config.QuestDebug();
		CreateObjectivesFromConfig(); //! Create objective instances from quest config
		m_QuestModule.CheckAndSpawnObjectSet(Config.GetID());  //! Create quest objects from quest config if not spawned already

		QuestPrint(ToString() + "::SetQuestConfig - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest SetConfig
	// -----------------------------------------------------------
	void SetConfig(ExpansionQuestConfig config)
	{
		Config = config;
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest CreateObjectivesFromConfig
	// -----------------------------------------------------------
	private void CreateObjectivesFromConfig()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CreateObjectivesFromConfig").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::CreateObjectivesFromConfig - Start");

		if (!Config || !Config.GetObjectives())
			return;

		//! Create quest objectives based on quest config.
		for (int i = 0; i < Config.GetObjectives().Count(); i++)
		{
			ref ExpansionQuestObjectiveConfigBase objectiveConfig = Config.GetObjectives()[i];
			int objectiveID = objectiveConfig.GetID();
			int objectiveType = objectiveConfig.GetObjectiveType();
			int time = objectiveConfig.GetTimeLimit();

			if (CreateObjective(m_ObjectiveIndex, objectiveType, objectiveID, time))
			{
				QuestPrint(ToString() + "::CreateObjectivesFromConfig - Created quest objective for quest with ID:" + Config.GetID() + " [Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + m_ObjectiveIndex + "]");
				m_ObjectiveIndex++;
			}
			else
			{
				Error(ToString() + "::CreateObjectivesFromConfig - Could not create quest objective for quest with ID:" + Config.GetID() + "[Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + m_ObjectiveIndex + "]");
				return;
			}
		}

		if (QuestObjectives.Count() == Config.GetObjectives().Count())
			m_ObjectivesCreated = true;

		QuestPrint(ToString() + "::CreateObjectivesFromConfig - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest CreateObjective
	// -----------------------------------------------------------
	private bool CreateObjective(int index, int objectiveType, int objectiveID, int time)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "CreateObjective").Add(sender).Add(ctx);
	#endif

		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig = m_QuestModule.GetTravelObjectiveConfigByID(objectiveID);
				if (!travelConfig)
					return false;

				ExpansionQuestObjectiveTravelEvent travelObjectiveEvent = new ExpansionQuestObjectiveTravelEvent(this);
				travelObjectiveEvent.SetIndex(index);
				travelObjectiveEvent.SetObjectiveConfig(travelConfig);
				travelObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(travelObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = m_QuestModule.GetDeliveryObjectiveConfigByID(objectiveID);
				if (!deliveryConfig)
					return false;

				ExpansionQuestObjectiveDeliveryEvent deliveryObjectiveEvent = new ExpansionQuestObjectiveDeliveryEvent(this);
				deliveryObjectiveEvent.SetIndex(index);
				deliveryObjectiveEvent.SetObjectiveConfig(deliveryConfig);
				deliveryObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(deliveryObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = m_QuestModule.GetTargetObjectiveConfigByID(objectiveID);
				if (!targetConfig)
					return false;

				ExpansionQuestObjectiveTargetEvent targetObjectiveEvent = new ExpansionQuestObjectiveTargetEvent(this);
				targetObjectiveEvent.SetIndex(index);
				targetObjectiveEvent.SetObjectiveConfig(targetConfig);
				targetObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(targetObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = m_QuestModule.GetCollectionObjectiveConfigByID(objectiveID);
				if (!collectionConfig)
					return false;

				ExpansionQuestObjectiveCollectionEvent collectionObjectiveEvent = new ExpansionQuestObjectiveCollectionEvent(this);
				collectionObjectiveEvent.SetIndex(index);
				collectionObjectiveEvent.SetObjectiveConfig(collectionConfig);
				collectionObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(collectionObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig = m_QuestModule.GetTreasureHuntObjectiveConfigByID(objectiveID);
				if (!treasureHuntConfig)
					return false;

				ExpansionQuestObjectiveTreasureHuntEvent treasureHuntObjectiveEvent = new ExpansionQuestObjectiveTreasureHuntEvent(this);
				treasureHuntObjectiveEvent.SetIndex(index);
				treasureHuntObjectiveEvent.SetObjectiveConfig(treasureHuntConfig);
				treasureHuntObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(treasureHuntObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionObjectiveConfig = m_QuestModule.GetActionObjectiveConfigByID(objectiveID);
				if (!actionObjectiveConfig)
					return false;

				ExpansionQuestObjectiveActionEvent actionObjectiveEvent = new ExpansionQuestObjectiveActionEvent(this);
				actionObjectiveEvent.SetIndex(index);
				actionObjectiveEvent.SetObjectiveConfig(actionObjectiveConfig);
				actionObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(actionObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.SCRIPTED:
			{
				//! Nothing to do here yet!
				return false;
			}
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = m_QuestModule.GetAIPatrolObjectiveConfigByID(objectiveID);
				if (!aiPatrolConfig)
					return false;

				ExpansionQuestObjectiveAIPatrolEvent aiPatrolObjectiveEvent = new ExpansionQuestObjectiveAIPatrolEvent(this);
				aiPatrolObjectiveEvent.SetIndex(index);
				aiPatrolObjectiveEvent.SetObjectiveConfig(aiPatrolConfig);
				aiPatrolObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(aiPatrolObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = m_QuestModule.GetAICampObjectiveConfigByID(objectiveID);
				if (!aiCampConfig)
					return false;

				ExpansionQuestObjectiveAICampEvent aiCampObjectiveEvent = new ExpansionQuestObjectiveAICampEvent(this);
				aiCampObjectiveEvent.SetIndex(index);
				aiCampObjectiveEvent.SetObjectiveConfig(aiCampConfig);
				aiCampObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(aiCampObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.AIVIP:
			{
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig = m_QuestModule.GetAIVIPObjectiveConfigByID(objectiveID);
				if (!aiVIPConfig)
					return false;

				ExpansionQuestObjectiveAIVIPEvent aiVIPObjectiveEvent = new ExpansionQuestObjectiveAIVIPEvent(this);
				aiVIPObjectiveEvent.SetIndex(index);
				aiVIPObjectiveEvent.SetObjectiveConfig(aiVIPConfig);
				aiVIPObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(aiVIPObjectiveEvent);
				return true;
			}
		#endif
		}

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestConfig
	// -----------------------------------------------------------
	ExpansionQuestConfig GetQuestConfig()
	{
		return Config;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetQuestState
	// -----------------------------------------------------------
	void SetQuestState(int state)
	{
		State = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestState
	// -----------------------------------------------------------
	int GetQuestState()
	{
		return State;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetPlayerUID
	// -----------------------------------------------------------
	void SetPlayerUID(string playerUID)
	{
		m_PlayerUID = playerUID;
		m_Player = PlayerBase.GetPlayerByUID(playerUID);
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayerUID
	// -----------------------------------------------------------
	string GetPlayerUID()
	{
		return m_PlayerUID;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetPlayer
	// -----------------------------------------------------------
	void SetPlayer()
	{
		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayer
	// -----------------------------------------------------------
	PlayerBase GetPlayer()
	{
		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		return m_Player;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestItems
	// -----------------------------------------------------------
	array<Object> GetQuestItems()
	{
		return QuestItems;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsCompeleted
	// -----------------------------------------------------------
	bool IsCompeleted()
	{
		return IsCompeleted;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetIsCompleted
	// -----------------------------------------------------------
	void SetIsCompleted(bool state)
	{
		IsCompeleted = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestStart
	// -----------------------------------------------------------
	//! Event called when a quest is stated
	void OnQuestStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestStart").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestStart - Start");

		if (!m_ObjectivesCreated)
		{
			Error(ToString() + "::OnQuestStart - Quest objectives not created!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				Error(ToString() + "::OnQuestStart - Could not get quest config!");
				return;
			}

			//! Start all quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				if (objective && objective.GetIndex() == 0) //! Only start the first objective as we will progress thrue all objective events in a sequential order
				{
					m_CurrentObjectiveIndex = objective.GetIndex();
					objective.OnStart();
				}
			}

			//! Add all quest items to the players inventory
			CreateQuestItems();

			SetInitialized(true);

			SetQuestState(ExpansionQuestState.STARTED);

			if (!Config.IsAchivement())
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedText, Config.GetTitle()), ExpansionIcons.GetPath("Questionmark 2"), COLOR_EXPANSION_NOTIFICATION_INFO);
		}
		
		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
		m_QuestModule.UpdatePlayerQuests(this);

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestStart - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesComplete
	// -----------------------------------------------------------
	//! Event called when all quest objectives are completed
	void OnQuestObjectivesComplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesComplete").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - Start");

		if (GetQuestState() >= ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestObjectivesComplete - QUEST STATE IS ALREADY TURNIN!");
			return;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Can't get quest module!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest config!");
				return;
			}

			SetQuestState(ExpansionQuestState.CAN_TURNIN);

			if (!Config.IsAchivement())
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedText, Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);

		#ifdef EXPANSIONMODNAVIGATION
		#ifdef EXPANSIONMODAI
			if (Config.GetType() == ExpansionQuestType.AICAMP || Config.GetType() == ExpansionQuestType.AIPATROL || Config.GetType() == ExpansionQuestType.AIVIP || Config.GetType() == ExpansionQuestType.DELIVERY || Config.GetType() == ExpansionQuestType.TRAVEL || Config.GetType() == ExpansionQuestType.TARGET)
		#else
			if (Config.GetType() == ExpansionQuestType.DELIVERY || Config.GetType() == ExpansionQuestType.TRAVEL || Config.GetType() == ExpansionQuestType.TARGET)
		#endif
			{
				RemoveMarkers();
			}

			//! Create a marker on quest npc location for the player
			ExpansionQuestNPCData questNPC = GetQuestModule().GetQuestNPCDataByID(Config.GetQuestTurnInID());
			if (questNPC)
			{
				vector npcPos = questNPC.GetPosition();
				CreateClientMarker(npcPos, "Turn-In Quest");
			}
		#endif

			if (Config.IsAutocomplete())
				m_QuestModule.RequestCompleteQuestServer(Config.GetID(), GetPlayerUID(), m_Player.GetIdentity(), true);
		}
		
		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesIncomplete
	// -----------------------------------------------------------
	//! Event called when a quest objective state has changed to incomplete after it was completed once
	void OnQuestObjectivesIncomplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesIncomplete").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - Start");

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesIncomplete - Can't get quest module!");
			return;
		}

		SetQuestState(ExpansionQuestState.STARTED);
		
		#ifdef EXPANSIONMODNAVIGATION
			RemoveMarkers();
		#endif
		
		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
		
		QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestTurnIn
	// -----------------------------------------------------------
	//! Event called when ever a quest is completed and turned-in
	void OnQuestTurnIn(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestTurnIn").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestTurnIn - Start");

		if (GetQuestState() > ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestTurnIn - QUEST STATE INVALID FOR TURN-IN!");
			return;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestTurnIn - Can't get quest module!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config && Config.GetID() == -1)
			{
				Error(ToString() + "::OnQuestTurnIn - Could not get quest config!");
				return;
			}

			//! Add all quest rewards to the players inventory
			if (GetQuestConfig().GetRewards().Count() > 0)
			{
				if (!SpawnQuestRewards(reward))
				{
					Error(ToString() + "::OnQuestTurnIn - Could not get spawn quest reward!");
					return;
				}
			}

			SetQuestState(ExpansionQuestState.COMPLETED);

			//! Call end event on all quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnTurnIn();
				delete objective;
			}

			if (!Config.IsAchivement())
			{
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInText, Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
			}
			else
			{
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().AchivementCompletedTitle, Config.GetTitle()), new StringLocaliser(GetExpansionSettings().GetQuest().AchivementCompletedText, Config.GetObjectiveText()), ExpansionIcons.GetPath("Star"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);
			}

			IsCompeleted = true;

			if (Config.GetFollowUpQuestID() > -1)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_QuestModule.RequestOpenQuestMenu, 1000, false, Config.GetQuestTurnInID(), GetPlayer().GetIdentity());
			}
		}

		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestTurnIn - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCancel
	// -----------------------------------------------------------
	//! Event called when a quest is manualy canceled by the player
	void OnQuestCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestCancel").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestCancel - Start");

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			SetQuestState(ExpansionQuestState.NONE);

			//! Cancel all active quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnCancel();
				delete objective;
			}

			//! Remove all quest items from player/world
			CleanupQuestItems();

			if (!Config.IsAchivement())
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledText, Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);
		}

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(Config.GetID());
		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State); //! Update the quest players persistent data

		SetInitialized(false);
		QuestDebug();
		QuestPrint(ToString() + "::OnQuestCancel - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestContinue
	// -----------------------------------------------------------
	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	void OnQuestContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestContinue").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestContinue - Start");

		if (!m_ObjectivesCreated)
		{
			Error(ToString() + "::OnQuestContinue - Quest objectives not created!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				Error(ToString() + "::OnQuestContinue - Could not get quest config!");
				return;
			}

			//! Continue active quest objectives
			for (int i = 0; i < QuestObjectives.Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
				if (objective.GetIndex() == m_CurrentObjectiveIndex)
				{
					if (!objective.IsInitialized()) //! Only start the last active objective
					{
						objective.OnContinue();
					}
				}
			}

			//! Recreate quest items
			CreateQuestItems();

			SetInitialized(true);
		}

		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
		
		QuestDebug();

		QuestPrint(ToString() + "::OnQuestContinue - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest CleanupQuestItems
	// -----------------------------------------------------------
	void CleanupQuestItems()
	{
		if (!QuestItems || QuestItems.Count() == 0)
			return;

		//! Remove all quest items from player/world
		for (int i = 0; i < QuestItems.Count(); i++)
		{
			Object obj = QuestItems[i];
			GetGame().ObjectDelete(obj);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateQuestItems
	// -----------------------------------------------------------
	void CreateQuestItems()
	{
		if (!m_IsGroupQuest && m_Player)
		{
			EntityAI playerEntity = m_Player;
			ExpansionQuestItemConfig questItem;
			Object obj;
			ItemBase questItemBase;
			
			//! Add all quest items again to the players inventory
			for (int i = 0; i < Config.GetQuestItems().Count(); i++)
			{
				questItem = Config.GetQuestItems()[i];
				obj = Spawn(questItem, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
				
				if (Class.CastTo(questItemBase, obj))
				{
					questItemBase.SetIsQuestItem(true);
				}
				
				QuestItems.Insert(obj);
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			for (int j = 0; j < m_Group.GetPlayers().Count(); j++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[j];
				if (!playerGroupData)
				{
					Error(ToString() + "::CreateQuestItems - Could not get group members party data!");
					continue;
				}
				
				QuestPrint(ToString() + "::CreateQuestItems - Spawn quest items for player with UID: " + playerGroupData.GetID());
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				EntityAI groupPlayerEntity = EntityAI.Cast(groupPlayer);
				if (groupPlayer)
				{
					for (int g = 0; g < Config.GetQuestItems().Count(); g++)
					{
						questItem = Config.GetQuestItems()[g];
						obj = Spawn(questItem, groupPlayer, groupPlayerEntity, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
						
						if (Class.CastTo(questItemBase, obj))
						{
							questItemBase.SetIsQuestItem(true);
						}
						
						QuestItems.Insert(obj);
					}
				}
			}
	#endif
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnGroupMemberJoined
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member joins/rejoins the quest group
	void OnGroupMemberJoined(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnGroupMemberJoined").Add(sender).Add(ctx);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnGroupMemberLeave
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnGroupMemberLeave").Add(sender).Add(ctx);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCleanup
	// -----------------------------------------------------------
	//! Event called when quest instance is destroyed/cleaned-up
	void OnQuestCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestCleanup").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestCleanup - Start");

		//! Call cleanup event on all active quest objectives
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
			if (objective)
			{
				objective.OnCleanup();
			}
		}

		//! Cleanup all spawned quest item objects if quest gets cleaned up while not completed
		//! The items get readded to the player if he continues the quest in his next session.
		if (IsCompeleted() && GetQuestConfig().GetType() != ExpansionQuestType.TREASUREHUNT)
		{
			CleanupQuestItems();
		}
		else if (!IsCompeleted())
		{
			CleanupQuestItems();
		}

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(Config.GetID());

		SetInitialized(false);

		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
		
		QuestDebug();

		QuestPrint(ToString() + "::OnQuestCleanup - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest CompletionCheck
	// -----------------------------------------------------------
	void CompletionCheck()
	{
		QuestPrint(ToString() + "::CompletionCheck - Start");

		int complededObjectives = 0;
		for (int i = 0; i < QuestObjectives.Count(); ++i)
		{
			ExpansionQuestObjectiveEventBase currentObjective = QuestObjectives.Get(i);
			if (currentObjective && currentObjective.IsCompleted() && currentObjective.CompletionCheck())
			{
				complededObjectives++;
			}
		}

		QuestPrint(ToString() + "::CompletionCheck - Completed objectives: " + complededObjectives);
		QuestPrint(ToString() + "::CompletionCheck - Objectives count: " + QuestObjectives.Count());

		if (complededObjectives == QuestObjectives.Count())
		{
			QuestPrint(ToString() + "::CompletionCheck - ALL COMPLETED");
			OnQuestObjectivesComplete();
		}
		else if (complededObjectives < QuestObjectives.Count())
		{
			QuestPrint(ToString() + "::CompletionCheck - INCOMPLETE");
			OnQuestObjectivesIncomplete();
			QuestPrint(ToString() + "::CompletionCheck - Current active objective index: " + m_CurrentObjectiveIndex);

			ExpansionQuestObjectiveEventBase currentActiveObjective = QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
				return;

			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized() && currentActiveObjective.IsCompleted()) //! Check if our currect active objective is completed and if so try to get the next objective in the list and start it
			{
				m_CurrentObjectiveIndex++;
				QuestPrint(ToString() + "::CompletionCheck - Current active objective is complered! Try to start next objective with index: " + m_CurrentObjectiveIndex);
				ExpansionQuestObjectiveEventBase nextObjective = QuestObjectives[m_CurrentObjectiveIndex]; //! Get the next objective from our objective list and start it
				if (!nextObjective || nextObjective.IsInitialized() || nextObjective.IsActive())
					return;

			#ifdef EXPANSIONMODNAVIGATION
				RemoveMarkers();
			#endif

				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedText, currentActiveObjective.GetObjectiveConfig().GetObjectiveText(), Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);

				nextObjective.OnStart();	//! Start the next objective
				
				m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
				m_QuestModule.UpdateQuestPlayersObjectiveData(this);
				
				QuestPrint(ToString() + "::CompletionCheck - Started next quest objective event with index: " + m_CurrentObjectiveIndex);
			}
		}

		QuestPrint(ToString() + "::CompletionCheck - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest Spawn
	// -----------------------------------------------------------
	//! Spawn object method
	private Object Spawn(ExpansionQuestItemConfig item, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj;
		if (!item.IsVehicle()) obj = ExpansionItemSpawnHelper.SpawnOnParent(item.GetClassName(), player, parent, item.GetAmount());
		else obj = ExpansionItemSpawnHelper.SpawnVehicle(item.GetClassName(), player, parent, position, orientation, item.GetAmount());

		return obj;
	}

	// -----------------------------------------------------------
	// ExpansionQuest Spawn
	// -----------------------------------------------------------
	private Object Spawn(ExpansionQuestRewardConfig item, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj;
		if (!item.IsVehicle()) obj = ExpansionItemSpawnHelper.SpawnOnParent(item.GetClassName(), player, parent, item.GetAmount());
		else obj = ExpansionItemSpawnHelper.SpawnVehicle(item.GetClassName(), player, parent, position, orientation, item.GetAmount());

		return obj;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetObjectives
	// -----------------------------------------------------------
	array<ref ExpansionQuestObjectiveEventBase> GetObjectives()
	{
		return QuestObjectives;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsInitialized
	// -----------------------------------------------------------
	bool IsInitialized()
	{
		return m_Initialized;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetInitialized
	// -----------------------------------------------------------
	void SetInitialized(bool state)
	{
		m_Initialized = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestModule
	// -----------------------------------------------------------
	ExpansionQuestModule GetQuestModule()
	{
		return m_QuestModule;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CanCompeteQuest
	// -----------------------------------------------------------
	bool CanCompeteQuest()
	{
		int complededObjectives = 0;
		for (int i = 0; i < GetObjectives().Count(); ++i)
		{
			ExpansionQuestObjectiveEventBase objective = GetObjectives().Get(i);
			if (objective && objective.CompletionCheck())
				complededObjectives++;
		}

		if (complededObjectives == GetObjectives().Count())
			return true;

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetIsGroupQuest
	// -----------------------------------------------------------
	void SetIsGroupQuest(bool state)
	{
		m_IsGroupQuest = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsGroupQuest
	// -----------------------------------------------------------
	bool IsGroupQuest()
	{
		return m_IsGroupQuest;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetGroup
	// -----------------------------------------------------------
#ifdef EXPANSIONMODGROUPS
	void SetGroup(ExpansionPartyData group)
	{
		QuestPrint(ToString() + "::SetGroup - Start");

		m_Group = group;
		m_GroupID = group.GetPartyID();
		m_IsGroupQuest = true;

		QuestPrint(ToString() + "::SetGroup - Group: " + m_Group);
		QuestPrint(ToString() + "::SetGroup - Group ID: " + m_GroupID);
		QuestPrint(ToString() + "::SetGroup - Quest is group quest: " + m_IsGroupQuest);

		QuestPrint(ToString() + "::SetGroup - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetGroup
	// -----------------------------------------------------------
	ExpansionPartyData GetGroup()
	{
		return m_Group;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetGroupID
	// -----------------------------------------------------------
	int GetGroupID()
	{
		return m_GroupID;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsQuestGroupMember
	// -----------------------------------------------------------
	bool IsQuestGroupMember(string playerUID)
	{
		if (!m_Group)
			return false;

		for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
		{
			ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
			if (playerGroupData && playerGroupData.GetID() == playerUID)
				return true;
		}

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsOtherGroupMemberOnline
	// -----------------------------------------------------------
	bool IsOtherGroupMemberOnline(string excludeUID)
	{
		if (!m_Group)
			return false;

		for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
		{
			ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
			if (playerGroupData && playerGroupData.GetID() != excludeUID)
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (player)
					return true;
			}
		}

		return false;
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuest SpawnQuestRewards
	// -----------------------------------------------------------
	bool SpawnQuestRewards(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SpawnQuestRewards").Add(sender).Add(ctx);
	#endif

		ExpansionQuestRewardConfig questReward;
	#ifdef EXPANSIONMODHARDLINE
		int humanity;
		ExpansionHardlineModule hardlineModule;
		ExpansionHardlinePlayerData hardlinePlayerData;
	#endif

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);

		if (!m_IsGroupQuest)
		{
			EntityAI playerEntity = m_Player;
			if (Config.NeedToSelectReward())
			{
				if (!reward)
					return false;

				Spawn(reward, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
			}
			else
			{
				for (int i = 0; i < Config.GetRewards().Count(); i++)
				{
					questReward = Config.GetRewards()[i];
					Spawn(questReward, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
				}
			}

		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().UseHumanity && Config.GetHumanityReward() != 0)
			{
				hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
				if (!hardlineModule)
					return false;

				hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(m_Player.GetIdentity().GetId());
				if (!hardlinePlayerData)
					return false;
				
				if (Config.GetHumanityReward() > 0)
				{
					hardlinePlayerData.AddHumanity(Config.GetHumanityReward());
				}
				else if (Config.GetHumanityReward() < 0)
				{
					hardlinePlayerData.RemoveHumanity(Config.GetHumanityReward());
				}
			}
		#endif
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			QuestPrint(ToString() + "::SpawnQuestRewards - Group: " + m_Group);
			QuestPrint(ToString() + "::SpawnQuestRewards - Group players: " + m_Group.GetPlayers());
			QuestPrint(ToString() + "::SpawnQuestRewards - Group players count: " + m_Group.GetPlayers().Count());
			
			for (int j = 0; j < m_Group.GetPlayers().Count(); j++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[j];
				if (!playerGroupData)
				{
					Error(ToString() + "::SpawnQuestRewards - Could not get group members party data!");
					continue;
				}
				
				if (Config.RewardsForGroupOwnerOnly() && playerGroupData.GetID() != m_PlayerUID)
					continue;
				
				QuestPrint(ToString() + "::SpawnQuestRewards - Spawn quest reward for player with UID: " + playerGroupData.GetID());
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				EntityAI groupPlayerEntity = EntityAI.Cast(groupPlayer);
				if (groupPlayer)
				{
					if (Config.NeedToSelectReward)
					{
						if (!reward)
						{
							QuestPrint(ToString() + "::SpawnQuestRewards - No reward selected!");
							return false;
						}
						
						QuestPrint(ToString() + "::SpawnQuestRewards - Spawn selected reward: " + reward.ToString());
						Spawn(reward, groupPlayer, groupPlayerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
					}
					else
					{
						//! Add all quest rewards to the players inventory
						for (int k = 0; k < Config.GetRewards().Count(); k++)
						{
							questReward = Config.GetRewards()[k];
							QuestPrint(ToString() + "::SpawnQuestRewards - Spawn reward: [" + k + "] " + questReward.ToString());	
							Spawn(questReward, groupPlayer, groupPlayerEntity, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
						}
					}
				}

			#ifdef EXPANSIONMODHARDLINE
				if (GetExpansionSettings().GetHardline().UseHumanity && Config.GetHumanityReward() != 0)
				{
					hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
					if (!hardlineModule)
						return false;

					hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(groupPlayer.GetIdentity().GetId());
					if (!hardlinePlayerData)
						return false;
					
					if (Config.GetHumanityReward() > 0)
					{
						hardlinePlayerData.AddHumanity(Config.GetHumanityReward());
					}
					else if (Config.GetHumanityReward() < 0)
					{
						hardlinePlayerData.RemoveHumanity(Config.GetHumanityReward());
					}
				}
			#endif
			}
		}
	#endif

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetObjectiveIndex
	// -----------------------------------------------------------
	int GetObjectiveIndex()
	{
		return m_ObjectiveIndex;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetObjectivesCount
	// -----------------------------------------------------------
	int GetObjectivesCount()
	{
		return QuestObjectives.Count();
	}

	// -----------------------------------------------------------
	// ExpansionQuest SendNotification
	// -----------------------------------------------------------
	void SendNotification(StringLocaliser title, StringLocaliser text, string icon, int color)
	{
		if (!m_IsGroupQuest)
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			ExpansionNotification(title, text, icon, color, 7, ExpansionNotificationType.TOAST).Create(m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
				if (!playerGroupData)
				{
					Error(ToString() + "::SendNotification - Can't get party data from quest group data for index: " + i);
					continue;
				}

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer)
				{
					ExpansionNotification(title, text, icon, color, 7, ExpansionNotificationType.TOAST).Create(groupPlayer.GetIdentity());
				}
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestPlayersAlive
	// -----------------------------------------------------------
	bool QuestPlayersAlive()
	{
		 if (!m_IsGroupQuest)
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (!m_Player.IsAlive())
			{
				return false;
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
				if (!playerGroupData)
					continue;

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer && !groupPlayer.IsAlive())
				{
					return false;
				}
			}
		}
	#endif

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateClientMarker
	// -----------------------------------------------------------
#ifdef EXPANSIONMODNAVIGATION
	void CreateClientMarker(vector pos, string name)
	{
		if (!m_IsGroupQuest)
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			m_QuestModule.CreateClientMarker(pos, name, Config.GetID(), m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
				if (!playerGroupData)
					continue;

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer)
				{
					m_QuestModule.CreateClientMarker(pos, name, Config.GetID(), groupPlayer.GetIdentity());
				}
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest RemoveMarkers
	// -----------------------------------------------------------
	void RemoveMarkers()
	{
		if (!m_IsGroupQuest && m_Player)
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			m_QuestModule.RemoveClientMarkers(Config.GetID(), m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && m_Group)
		{
			for (int i = 0; i < m_Group.GetPlayers().Count(); i++)
			{
				ExpansionPartyPlayerData playerGroupData = m_Group.GetPlayers()[i];
				if (!playerGroupData)
					continue;

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer)
				{
					m_QuestModule.RemoveClientMarkers(Config.GetID(), groupPlayer.GetIdentity());
				}
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnRecreateClientMarkers
	// -----------------------------------------------------------
	void OnRecreateClientMarkers()
	{
		for (int i = 0; i < QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = QuestObjectives.Get(i);
			if (!objective)
				continue;

			objective.OnRecreateClientMarkers();
		}
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuest ObjectivesCreated
	// -----------------------------------------------------------
	bool ObjectivesCreated()
	{
		return m_ObjectivesCreated;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CurrentObjectiveIndex
	// -----------------------------------------------------------
	int CurrentObjectiveIndex()
	{
		return m_ObjectiveIndex;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetCurrentObjectiveIndex
	// -----------------------------------------------------------
	void SetCurrentObjectiveIndex(int index)
	{
		m_CurrentObjectiveIndex = index;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnUpdate
	// -----------------------------------------------------------
	void OnUpdate(float timeslice)
	{
		if (GetGame().IsClient() || !IsInitialized() ||!ObjectivesCreated())
			return;

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			//! Autocancel quest on player dead if param is enabled
			if (Config.CancelQuestOnPlayerDeath() && !QuestPlayersAlive())
			{
				m_QuestModule.CancelQuestServer(Config.GetID(), m_Player.GetIdentity());
				return;
			}
			
			if (QuestObjectives.Count() > 0)
			{
				for (int i = 0; i < UPDATE_OBJECTIVES_PER_TICK; ++i)
				{
					if (m_CurrentObjectiveTick >= QuestObjectives.Count())
					{
						m_CurrentObjectiveTick = 0;
					}

					ExpansionQuestObjectiveEventBase objective = QuestObjectives.Get(m_CurrentObjectiveTick);
					if (!objective)
						continue;

					if (objective.IsInitialized())
						objective.OnUpdate(timeslice);

					m_CurrentObjectiveTick++;

					if (m_CurrentObjectiveTick == QuestObjectives.Count())
						break;
				}
			}
			else
			{
				m_CurrentObjectiveTick = 0;
			}

			m_UpdateQueueTimer = 0.0;
		}
	}
	
	// -----------------------------------------------------------
	// ExpansionQuest UpdateQuestPlayersObjectiveData
	// -----------------------------------------------------------
	void UpdateQuestPlayersObjectiveData()
	{
		if (!m_QuestModule)
			return;
		
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestPrint
	// -----------------------------------------------------------
	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		Print(text);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestLog
	// -----------------------------------------------------------
	void QuestLog()
	{
		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - ExpansionQuest - Quest owner ID: " +m_PlayerUID);
		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - ExpansionQuest - Quest ID: " + Config.GetID());
		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - ExpansionQuest - Quest state: " + State);
		GetExpansionSettings().GetLog().PrintLog("[Expansion Quests] - ExpansionQuest - Quest completed: " + IsCompeleted);
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestDebug
	// -----------------------------------------------------------
	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		QuestPrint("------------------------------------------------------------");
		QuestPrint(ToString() + "::QuestDebug - Quest config: " + Config);
		QuestPrint(ToString() + "::QuestDebug - Quest state: " + State);
		QuestPrint(ToString() + "::QuestDebug - Quest items: " + QuestItems);
		QuestPrint(ToString() + "::QuestDebug - Quest player UID: " + m_PlayerUID);
		QuestPrint(ToString() + "::QuestDebug - Quest player: " + m_Player);
		QuestPrint(ToString() + "::QuestDebug - Quest completed: " + IsCompeleted);
		QuestPrint(ToString() + "::QuestDebug - Quest initialized: " + m_Initialized);
	#ifdef EXPANSIONMODGROUPS
		QuestPrint(ToString() + "::QuestDebug - Quest is group quest: " + m_IsGroupQuest);
		QuestPrint(ToString() + "::QuestDebug - Quest group: " + m_Group);
	#endif
		QuestPrint("------------------------------------------------------------");
	#endif
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		QuestPrint(ToString() + "::QuestDebug - Quest objectives: " + QuestObjectives.ToString());
		for (int i = 0; i < QuestObjectives.Count(); ++i)
		{
			ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
			if (!objective || objective && objective.GetObjectiveType() == ExpansionQuestObjectiveType.NONE)
				return;

			switch (objective.GetObjectiveType())
			{
				case ExpansionQuestObjectiveType.TARGET:
				{
					ExpansionQuestObjectiveTargetEvent targetEvent;
					if (Class.CastTo(targetEvent, objective))
						targetEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.TRAVEL:
				{
					ExpansionQuestObjectiveTravelEvent travelEvent;
					if (Class.CastTo(travelEvent, objective))
						travelEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.COLLECT:
				{
					ExpansionQuestObjectiveCollectionEvent collectionEvent;
					if (Class.CastTo(collectionEvent, objective))
						collectionEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryEvent deliveryEvent;
					if (Class.CastTo(deliveryEvent, objective))
						deliveryEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					ExpansionQuestObjectiveTreasureHuntEvent treasureEvent;
					if (Class.CastTo(treasureEvent, objective))
						treasureEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.ACTION:
				{
					ExpansionQuestObjectiveActionEvent actionEvent;
					if (Class.CastTo(actionEvent, objective))
						actionEvent.QuestDebug();
				}
				break;
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					ExpansionQuestObjectiveAIPatrol aiPatrolEvent;
					if (Class.CastTo(aiPatrolEvent, objective))
						aiPatrolEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.AICAMP:
				{
					ExpansionQuestObjectiveAICamp aiCampEvent;
					if (Class.CastTo(aiCampEvent, objective))
						aiCampEvent.QuestDebug();
				}
				break;

				case ExpansionQuestObjectiveType.AIVIP:
				{
					ExpansionQuestObjectiveAIVIPEvent aiVIPEvent;
					if (Class.CastTo(aiVIPEvent, objective))
						aiVIPEvent.QuestDebug();
				}
				break;
			#endif
			}
		}
		QuestPrint("------------------------------------------------------------");
	#endif
	}
};