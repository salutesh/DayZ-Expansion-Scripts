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
	protected ExpansionQuestModule m_QuestModule;

	protected ExpansionQuestConfig Config;
	protected int State = ExpansionQuestState.NONE;
	protected ref array<ItemBase> QuestItems = new array<ItemBase>;	//! Normal items the player will recieve on quest start.
	protected ref array<ref ExpansionQuestObjectiveEventBase> QuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;	//! Quest objectives
	protected PlayerBase m_Player;
	protected string m_PlayerUID;
	protected bool IsCompeleted = false;
	protected bool m_Initialized = false;

	protected float m_UpdateQueueTimer; //! Server update que timer
	protected int m_CurrentObjectiveTick;
	protected const float UPDATE_TICK_TIME = 1.0; // refreshes 50 quests objectives every ten seconds (1 every sec.)
	protected const int UPDATE_OBJECTIVES_PER_TICK = 5;

	protected bool m_IsGroupQuest = false;
#ifdef EXPANSIONMODGROUPS
	protected int m_GroupID = -1;
#endif

	protected int m_ObjectiveIndex = 0;
	protected bool m_ObjectivesCreated = false;

	protected int m_CurrentObjectiveIndex = -1;

	// -----------------------------------------------------------
	// ExpansionQuest Constructor
	// -----------------------------------------------------------
	void ExpansionQuest(ExpansionQuestModule module)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
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
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::~ExpansionQuest - Start");

		m_Player = NULL;

		QuestPrint(ToString() + "::~ExpansionQuest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetQuestConfig
	// -----------------------------------------------------------
	void SetQuestConfig(ExpansionQuestConfig config)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
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
	protected void CreateObjectivesFromConfig()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::CreateObjectivesFromConfig - Start");

		if (!Config || !Config.GetObjectives())
			return;

		//! Create quest objectives based on quest config.
		array<ref ExpansionQuestObjectiveConfig>  questObjectives = Config.GetObjectives();
		foreach (ExpansionQuestObjectiveConfig objectiveConfig: questObjectives)
		{
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
				CF_Log.Error(ToString() + "::CreateObjectivesFromConfig - Could not create quest objective for quest with ID:" + Config.GetID() + "[Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + m_ObjectiveIndex + "]");
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
	protected bool CreateObjective(int index, int objectiveType, int objectiveID, int time)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
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
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingObjectiveConfig = m_QuestModule.GetCraftingObjectiveConfigByID(objectiveID);
				if (!craftingObjectiveConfig)
					return false;

				ExpansionQuestObjectiveCraftingEvent craftingObjectiveEvent = new ExpansionQuestObjectiveCraftingEvent(this);
				craftingObjectiveEvent.SetIndex(index);
				craftingObjectiveEvent.SetObjectiveConfig(craftingObjectiveConfig);
				craftingObjectiveEvent.SetTimeLimit(time);
				QuestObjectives.Insert(craftingObjectiveEvent);
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
	array<ItemBase> GetQuestItems()
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
	//! Event called when a quest is started
	bool OnQuestStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestStart - Start");

		if (!m_ObjectivesCreated)
		{
			CF_Log.Error(ToString() + "::OnQuestStart - Quest objectives not created!");
			return false;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				CF_Log.Error(ToString() + "::OnQuestStart - Could not get quest config!");
				return false;
			}

			//! Start quest objectives
			ExpansionQuestObjectiveEventBase objective = GetObjectives()[0];
			//! We only start the first objective as we will progress thrue all objective events in a sequential order.
			//! Only exception for now is when the quest has a COLLECT and TREASUREHUNT objective, then we want to activate them all.
			if (objective && objective.GetIndex() == 0)
			{
				m_CurrentObjectiveIndex = 0;
				if (!objective.OnStart())
					return false;

				ExpansionQuestObjectiveEventBase nextObjective = GetObjectives()[1];
				if ((objective.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT || objective.GetObjectiveType() == ExpansionQuestObjectiveType.TREASUREHUNT) && nextObjective && (nextObjective.GetObjectiveType() == ExpansionQuestObjectiveType.TREASUREHUNT || nextObjective.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT))
				{
					m_CurrentObjectiveIndex = 1;
					if (!nextObjective.OnStart())
					{
						m_CurrentObjectiveIndex = 0;
						return false;
					}
				}
			}

			//! Add all quest items to the players inventory
			CreateQuestItems();

			SetInitialized(true);

			SetQuestState(ExpansionQuestState.STARTED);

			if (!Config.IsAchivement())
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedText, Config.GetTitle()), ExpansionIcons.GetPath("Questionmark 2"), COLOR_EXPANSION_NOTIFICATION_INFO);
		}

		UpdateQuest();

		//QuestDebug();

		QuestPrint(ToString() + "::OnQuestStart - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesComplete
	// -----------------------------------------------------------
	//! Event called when all quest objectives are completed
	void OnQuestObjectivesComplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - Start");

		if (GetQuestState() >= ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestObjectivesComplete - QUEST STATE IS ALREADY TURNIN!");
			return;
		}

		if (!m_QuestModule)
		{
			CF_Log.Error(ToString() + "::OnQuestObjectivesComplete - Can't get quest module!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				CF_Log.Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest config!");
				return;
			}

			SetQuestState(ExpansionQuestState.CAN_TURNIN);

			if (!Config.IsAchivement())
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedText, Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);

		#ifdef EXPANSIONMODNAVIGATION
		#ifdef EXPANSIONMODAI
			if (Config.GetType() == ExpansionQuestType.AICAMP || Config.GetType() == ExpansionQuestType.AIPATROL || Config.GetType() == ExpansionQuestType.AIVIP || Config.GetType() == ExpansionQuestType.DELIVERY || Config.GetType() == ExpansionQuestType.COLLECT || Config.GetType() == ExpansionQuestType.TRAVEL || Config.GetType() == ExpansionQuestType.TARGET)
		#else
			if (Config.GetType() == ExpansionQuestType.DELIVERY || Config.GetType() == ExpansionQuestType.COLLECT || Config.GetType() == ExpansionQuestType.TRAVEL || Config.GetType() == ExpansionQuestType.TARGET)
		#endif
			{
				RemoveMarkers();
			}

			//! Create a marker on the closest quest npc location for the player
			vector npcPos = GetClosestQuestNPCPosition(Config.GetQuestTurnInIDs(), m_Player.GetPosition());
			CreateClientMarker(npcPos, "Turn In Quest");
		#endif

			if (Config.IsAutocomplete())
				m_QuestModule.RequestCompleteQuestServer(Config.GetID(), GetPlayerUID(), m_Player.GetIdentity(), true);
		}

		UpdateQuest();
		
		//QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesIncomplete
	// -----------------------------------------------------------
	//! Event called when a quest objective state has changed to incomplete after it was completed once
	void OnQuestObjectivesIncomplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - Start");

		if (!m_QuestModule)
		{
			CF_Log.Error(ToString() + "::OnQuestObjectivesIncomplete - Can't get quest module!");
			return;
		}

		SetQuestState(ExpansionQuestState.STARTED);

		#ifdef EXPANSIONMODNAVIGATION
			RemoveMarkers();
		#endif
		
		UpdateQuest();
		
		//QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestTurnIn
	// -----------------------------------------------------------
	//! Event called when ever a quest is completed and turned-in
	bool OnQuestTurnIn(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestTurnIn - Start");

		if (GetQuestState() > ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestTurnIn - QUEST STATE INVALID FOR TURN-IN!");
			return false;
		}

		if (!m_QuestModule)
		{
			CF_Log.Error(ToString() + "::OnQuestTurnIn - Can't get quest module!");
			return false;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config && Config.GetID() == -1)
			{
				CF_Log.Error(ToString() + "::OnQuestTurnIn - Could not get quest config!");
				return false;
			}

			//! Add all quest rewards to the players inventory
			if (GetQuestConfig().GetRewards().Count() > 0 || GetQuestConfig().GetHumanityReward() > 0)
				SpawnQuestRewards(reward);

			SetQuestState(ExpansionQuestState.COMPLETED);

			//! Call end event on all quest objectives
			for (int i = QuestObjectives.Count() - 1; i >= 0; i--)
			{
				ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
				if (!objective.OnTurnIn())
					return false;

				QuestObjectives.RemoveOrdered(i);
			}

			//! Remove all quest items from player/world
			CleanupQuestItems();

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
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_QuestModule.RequestOpenQuestMenu, 1000, false, Config.GetQuestTurnInIDs(), GetPlayer().GetIdentity());
			}
		}

		UpdateQuest();
		//QuestDebug();
	
	#ifdef EXPANSIONMODNAVIGATION
		RemoveMarkers();
	#endif

		QuestPrint(ToString() + "::OnQuestTurnIn - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCancel
	// -----------------------------------------------------------
	//! Event called when a quest is manualy canceled by the player
	bool OnQuestCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestCancel - Start");

		SetQuestState(ExpansionQuestState.NONE);

		//! Cancel all active quest objectives
		for (int i = QuestObjectives.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
			if (!objective.OnCancel())
				return false;

			QuestObjectives.RemoveOrdered(i);
		}

		//! Remove all quest items from player/world
		CleanupQuestItems();

		if (!Config.IsAchivement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledText, Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(Config.GetID());
		
		UpdateQuest();

		SetInitialized(false);
		//QuestDebug();
		QuestPrint(ToString() + "::OnQuestCancel - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestContinue
	// -----------------------------------------------------------
	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	bool OnQuestContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestContinue - Start");

		if (!m_ObjectivesCreated)
		{
			CF_Log.Error(ToString() + "::OnQuestContinue - Quest objectives not created!");
			return false;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!Config)
			{
				CF_Log.Error(ToString() + "::OnQuestContinue - Could not get quest config!");
				return false;
			}

			//! Continue active quest objectives
			for (int i = 0; i < QuestObjectives.Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
				//! We only start the last active objective as we will progress thrue all objective events in a sequential order.
				//! Only exception for now is when the quest has a COLLECT and TREASUREHUNT objective, then we want to activate them all.
				if (objective && objective.GetIndex() == m_CurrentObjectiveIndex)
				{
					if (!objective.IsInitialized()) //! Only start objectives that are not initialized yet
					{
						if (!objective.OnContinue())
						{
							CF_Log.Error(ToString() + "::OnQuestContinue - Quest objective continue event failed for quest with quest id: " + Config.GetID());
							return false;
						}

						int prev = i - 1;
						ExpansionQuestObjectiveEventBase prevObjective = GetObjectives()[prev];
						if ((objective.GetObjectiveType() == ExpansionQuestObjectiveType.TREASUREHUNT || objective.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT) && prevObjective && (prevObjective.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT || prevObjective.GetObjectiveType() == ExpansionQuestObjectiveType.TREASUREHUNT))
						{
							if (!prevObjective.OnContinue())
							{
								CF_Log.Error(ToString() + "::OnQuestContinue - Quest objective continue event failed for quest with quest id: " + Config.GetID());
								return false;
							}
						}
					}
				}
			}

			if (Config.GetQuestItems().Count() > 0)
			{
				array<ItemBase> playerQuestItems = GetPlayerQuestItems(m_Player, Config.GetID());
				if (playerQuestItems && playerQuestItems.Count() == Config.GetQuestItems().Count())
				{
					QuestPrint(ToString() + "::OnQuestContinue - Player has quest items. Add them to QuestItems array.. ");
					foreach (ItemBase item: playerQuestItems)
					{
						QuestItems.Insert(item);
					}
				}
				else
				{
					return false;
				}
			}

			SetInitialized(true);
		}

		UpdateQuest();

		QuestPrint(ToString() + "::OnQuestContinue - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CleanupQuestItems
	// -----------------------------------------------------------
	void CleanupQuestItems()
	{
		QuestPrint(ToString() + "::CleanupQuestItems - Start");

		for (int i = QuestItems.Count() - 1; i >= 0; i--)
		{
			ItemBase item = QuestItems[i];
			GetGame().ObjectDelete(item);
			QuestItems.RemoveOrdered(i);
		}

		QuestPrint(ToString() + "::CleanupQuestItems - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateQuestItems
	// -----------------------------------------------------------
	void CreateQuestItems()
	{
		if (!m_IsGroupQuest && m_Player)
		{
			EntityAI playerEntity = m_Player;
			Object obj;
			ItemBase questItemBase;

			//! Add all quest items to the players inventory
			array<ref ExpansionQuestItemConfig> questItemConfigs = Config.GetQuestItems();
			foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
			{
				obj = Spawn(questItem, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation(), questItem.GetAmount());
				if (Class.CastTo(questItemBase, obj))
				{
					questItemBase.SetQuestID(Config.GetID());
					QuestItems.Insert(questItemBase);
				}
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && GetGroup())
		{
			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				QuestPrint(ToString() + "::CreateQuestItems - Spawn quest items for player with UID: " + playerGroupData.GetID());
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				EntityAI groupPlayerEntity = groupPlayer;
				if (groupPlayer)
				{
					array<ref ExpansionQuestItemConfig> questItems = Config.GetQuestItems();
					foreach (ExpansionQuestItemConfig questConfigItem: questItems)
					{
						obj = Spawn(questConfigItem, groupPlayer, groupPlayerEntity, groupPlayer.GetPosition(), groupPlayer.GetOrientation(), questConfigItem.GetAmount());
						if (Class.CastTo(questItemBase, obj))
						{
							questItemBase.SetQuestID(Config.GetID());
							QuestItems.Insert(questItemBase);
						}
					}
				}
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnGroupMemberJoined
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member joins/rejoins the quest group
#ifdef EXPANSIONMODGROUPS
	void OnGroupMemberJoined(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif
		
		QuestPrint(ToString() + "::OnGroupMemberJoined - Start");
		
		if (m_IsGroupQuest && GetGroup() && Config.GetQuestItems().Count() > 0)
		{
			QuestPrint(ToString() + "::OnGroupMemberJoined - New group member joined. Check if he has already all quest items..");
			
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
			EntityAI groupPlayerEntity = groupPlayer;
			if (groupPlayer)
			{
				array<ItemBase> playerQuestItems = GetPlayerQuestItems(groupPlayer, Config.GetID());
				if (!playerQuestItems || playerQuestItems.Count() == 0)
				{
					QuestPrint(ToString() + "::OnGroupMemberJoined - Player has no quest items. Add items");
					array<ref ExpansionQuestItemConfig> questItemConfigs = Config.GetQuestItems();
					foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
					{
						Object obj = Spawn(questItem, groupPlayer, groupPlayerEntity, groupPlayer.GetPosition(), groupPlayer.GetOrientation(), questItem.GetAmount());
						ItemBase questItemBase;
						if (Class.CastTo(questItemBase, obj))
						{
							questItemBase.SetQuestID(Config.GetID());
							QuestItems.Insert(questItemBase);
						}
					}
				}
				else if (playerQuestItems && playerQuestItems.Count() != 0 && playerQuestItems.Count() < Config.GetQuestItems().Count())
				{
					QuestPrint(ToString() + "::OnGroupMemberJoined - Player has some quest items but is missing one or more. Cancel quest..");
					GetQuestModule().CancelQuestServer(Config.GetID(), groupPlayer.GetIdentity());
				}
				else if (playerQuestItems && playerQuestItems.Count() == Config.GetQuestItems().Count())
				{
					foreach (ItemBase item: playerQuestItems)
					{
						QuestItems.Insert(item);
					}
				}
			}
			
			ExpansionQuestObjectiveEventBase currentActiveObjective = QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
				return;
	
			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized())
			{
				currentActiveObjective.OnGroupMemberJoined(playerUID);
			}
		}
						
		QuestPrint(ToString() + "::OnGroupMemberJoined - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnGroupMemberLeave
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif
		
		QuestPrint(ToString() + "::OnGroupMemberLeave - Start");
		
		if (m_IsGroupQuest && GetGroup() && Config.GetQuestItems().Count() > 0)
		{
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
			if (groupPlayer)
			{
				array<ItemBase> playerQuestItems = GetPlayerQuestItems(groupPlayer, Config.GetID());
				if (playerQuestItems && playerQuestItems.Count() == Config.GetQuestItems().Count())
				{
					foreach (ItemBase item: playerQuestItems)
					{
						int findIndex = QuestItems.Find(item);
						if (findIndex > -1)
						{
							QuestItems.Remove(findIndex);
							GetGame().ObjectDelete(item);
						}
					}
				}
			}
			
			ExpansionQuestObjectiveEventBase currentActiveObjective = QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
				return;
	
			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized())
			{
				currentActiveObjective.OnGroupMemberLeave(playerUID);
			}
		}
		
		QuestPrint(ToString() + "::OnGroupMemberLeave - End");
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCleanup
	// -----------------------------------------------------------
	//! Event called when quest instance is destroyed/cleaned-up
	bool OnQuestCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

		QuestPrint(ToString() + "::OnQuestCleanup - Start");
		//! Call cleanup event on all active quest objectives
		for (int i = QuestObjectives.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveEventBase objective = QuestObjectives[i];
			if (!objective.OnCleanup())
			{
				QuestPrint(ToString() + "::OnQuestCleanup - Something went wrong on objective event cleanup for quest with id: " + Config.GetID());
			}

			QuestObjectives.RemoveOrdered(i);
		}

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(Config.GetID());
		SetInitialized(false);
		QuestPrint(ToString() + "::OnQuestCleanup - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CompletionCheck
	// -----------------------------------------------------------
	//! Main quest objective completion check
	void CompletionCheck()
	{
		QuestPrint(ToString() + "::CompletionCheck - Start");
		int complededObjectives = 0;
		foreach (ExpansionQuestObjectiveEventBase currentObjective: QuestObjectives)
		{
			if (currentObjective.IsActive() && currentObjective.IsCompleted())
				complededObjectives++;
		}

		QuestPrint(ToString() + "::CompletionCheck - Total objective count: " + QuestObjectives.Count());
		QuestPrint(ToString() + "::CompletionCheck - Total completed objectives: " + complededObjectives);

		if (QuestObjectives.Count() == 0)
		{
			QuestPrint(ToString() + "::CompletionCheck - Quest has no configured objectives. Skip completion check!");
			return;
		}
			
		if (complededObjectives == QuestObjectives.Count() && State == ExpansionQuestState.STARTED)
		{
			QuestPrint(ToString() + "::CompletionCheck - ALL COMPLETED");
			OnQuestObjectivesComplete();
		}
		else if (complededObjectives < QuestObjectives.Count())
		{
			if (State == ExpansionQuestState.CAN_TURNIN)
			{
				QuestPrint(ToString() + "::CompletionCheck - INCOMPLETE");
				OnQuestObjectivesIncomplete();
				QuestPrint(ToString() + "::CompletionCheck - Current active objective index: " + m_CurrentObjectiveIndex);
			}
	
			ExpansionQuestObjectiveEventBase currentActiveObjective = QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
				return;

			//! Check if our currect active objective is completed and if so try to get the next objective in the list and start it
			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized() && currentActiveObjective.IsCompleted())
			{
				m_CurrentObjectiveIndex++;
				QuestPrint(ToString() + "::CompletionCheck - Current active objective is complered! Try to start next objective with index: " + m_CurrentObjectiveIndex);

				//! Get the next objective from our objective list and start it
				ExpansionQuestObjectiveEventBase nextObjective = QuestObjectives[m_CurrentObjectiveIndex];
				if (!nextObjective || nextObjective.IsInitialized() || nextObjective.IsActive())
					return;

			#ifdef EXPANSIONMODNAVIGATION
				RemoveMarkers();
			#endif

				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedText, currentActiveObjective.GetObjectiveConfig().GetObjectiveText(), Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);

				//! Start the next objective
				nextObjective.OnStart();

				QuestPrint(ToString() + "::CompletionCheck - Started next quest objective event with index: " + m_CurrentObjectiveIndex);
			}
		}

		UpdateQuest();
		
		QuestPrint(ToString() + "::CompletionCheck - End");
	}

	protected void SpawnReward(ExpansionQuestRewardConfig reward, PlayerBase player, EntityAI parent, vector pos, vector ori)
	{
		int remainingAmount = reward.GetAmount();
		while (remainingAmount > 0)
		{
			int remainingAmountBefore = remainingAmount;
			Object obj = Spawn(reward, player, parent, pos, ori, remainingAmount);
			if (!obj)
			{
				CF_Log.Error("Error: Couldn't spawn " + reward.GetClassName());
				break;
			}

			if (remainingAmount == remainingAmountBefore)
			{
				//! Should not be possible, just in case...
				CF_Log.Error("Error: Spawning " + reward.GetClassName() + " did not affect remaining amount!");
				break;
			}
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuest Spawn
	// -----------------------------------------------------------
	//! Spawn object method
	protected Object Spawn(ExpansionQuestItemConfig item, PlayerBase player, inout EntityAI parent, vector position, vector orientation, out int remainingAmount)
	{
		Object obj;
		if (!item.IsVehicle())
		{
			obj = ExpansionItemSpawnHelper.SpawnOnParent(item.GetClassName(), player, parent, remainingAmount);
		}
		else
		{
			obj = ExpansionItemSpawnHelper.SpawnVehicle(item.GetClassName(), player, parent, position, orientation, remainingAmount);
		}

		return obj;
	}

	// -----------------------------------------------------------
	// ExpansionQuest Spawn
	// -----------------------------------------------------------
	protected Object Spawn(ExpansionQuestRewardConfig item, PlayerBase player, inout EntityAI parent, vector position, vector orientation, out int remainingAmount)
	{
		Object obj;
		if (!item.IsVehicle())
		{
			obj = ExpansionItemSpawnHelper.SpawnOnParent(item.GetClassName(), player, parent, remainingAmount, -1, item.GetAttachments());
		}
		else
		{
			obj = ExpansionItemSpawnHelper.SpawnVehicle(item.GetClassName(), player, parent, position, orientation, remainingAmount, item.GetAttachments());
		}

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
	// ExpansionQuest CanCompleteQuest
	// -----------------------------------------------------------
	bool CanCompleteQuest()
	{
		int complededObjectives = 0;
		foreach (ExpansionQuestObjectiveEventBase objective: QuestObjectives)
		{
			if (objective.IsCompleted())
				complededObjectives++;
		}

		if (complededObjectives == QuestObjectives.Count())
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
	void SetGroup(int groupID)
	{
		QuestPrint(ToString() + "::SetGroup - Start");

		m_GroupID = groupID;
		m_IsGroupQuest = true;

		QuestPrint(ToString() + "::SetGroup - Group ID: " + m_GroupID);
		QuestPrint(ToString() + "::SetGroup - Quest is group quest: " + m_IsGroupQuest);

		QuestPrint(ToString() + "::SetGroup - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetGroup
	// -----------------------------------------------------------
	ExpansionPartyData GetGroup()
	{
		ExpansionPartyModule partyModule;
		if (Class.CastTo(partyModule, CF_ModuleCoreManager.Get(ExpansionPartyModule)))
		{
			return partyModule.GetPartyByID(m_GroupID);
		}

		return NULL;
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
		if (!GetGroup())
			return false;

		array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
		foreach(ExpansionPartyPlayerData playerGroupData: groupPlayers)
		{
			if (playerGroupData.GetID() == playerUID)
				return true;
		}

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsOtherGroupMemberOnline
	// -----------------------------------------------------------
	bool IsOtherGroupMemberOnline(string excludeUID)
	{
		if (!GetGroup())
			return false;

		array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
		foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
		{
			if (playerGroupData.GetID() != excludeUID)
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
	void SpawnQuestRewards(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS, this);
	#endif

	#ifdef EXPANSIONMODHARDLINE
		int humanity;
		ExpansionHardlineModule hardlineModule;
		ExpansionHardlinePlayerData hardlinePlayerData;
	#endif

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);

		if (!m_IsGroupQuest)
		{
			EntityAI playerEntity = m_Player;
			int remainingAmount;
			int remainingAmountBefore;
			Object obj;

			if (Config.NeedToSelectReward())
			{
				if (reward)
				{
					QuestPrint(ToString() + "::SpawnQuestRewards - Spawn selected reward: " + reward.ToString());
					SpawnReward(reward, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
				}
			}
			else
			{
				array<ref ExpansionQuestRewardConfig> questRewards = Config.GetRewards();
				foreach (ExpansionQuestRewardConfig questReward: questRewards)
				{
					SpawnReward(questReward, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
				}
			}

		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().UseHumanity && Config.GetHumanityReward() != 0)
			{
				hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
				if (hardlineModule)
				{
					hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(m_Player.GetIdentity().GetId());
					if (hardlinePlayerData)
					{
						hardlinePlayerData.AddHumanity(Config.GetHumanityReward());
						hardlinePlayerData.Save(m_Player.GetIdentity().GetId());
						hardlineModule.SendPlayerHardlineData(hardlinePlayerData, m_Player.GetIdentity());
					}
				}
			}
		#endif
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && GetGroup())
		{
			QuestPrint(ToString() + "::SpawnQuestRewards - Group: " + GetGroup());
			QuestPrint(ToString() + "::SpawnQuestRewards - Group players: " + GetGroup().GetPlayers());
			QuestPrint(ToString() + "::SpawnQuestRewards - Group players count: " + GetGroup().GetPlayers().Count());

			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				if (Config.RewardsForGroupOwnerOnly() && playerGroupData.GetID() != m_PlayerUID)
					continue;

				QuestPrint(ToString() + "::SpawnQuestRewards - Spawn quest reward for player with UID: " + playerGroupData.GetID());
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				EntityAI groupPlayerEntity = groupPlayer;
				if (groupPlayer)
				{
					if (Config.NeedToSelectReward)
					{
						if (reward)
						{
							QuestPrint(ToString() + "::SpawnQuestRewards - Spawn selected reward: " + reward.ToString());
							SpawnReward(reward, groupPlayer, groupPlayerEntity, m_Player.GetPosition(), m_Player.GetOrientation());
						}
					}
					else
					{
						//! Add all quest rewards to the players inventory
						array<ref ExpansionQuestRewardConfig> groupQuestRewards = Config.GetRewards();
						foreach (ExpansionQuestRewardConfig groupQuestReward: groupQuestRewards)
						{
							SpawnReward(groupQuestReward, groupPlayer, groupPlayerEntity, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
						}
					}
				}

			#ifdef EXPANSIONMODHARDLINE
				if (GetExpansionSettings().GetHardline().UseHumanity && Config.GetHumanityReward() != 0)
				{
					hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
					if (hardlineModule)
					{
						hardlinePlayerData = hardlineModule.GetPlayerHardlineDataByUID(groupPlayer.GetIdentity().GetId());
						if (hardlinePlayerData)
						{
							hardlinePlayerData.AddHumanity(Config.GetHumanityReward());
							hardlinePlayerData.Save(groupPlayer.GetIdentity().GetId());
							hardlineModule.SendPlayerHardlineData(hardlinePlayerData, groupPlayer.GetIdentity());
						}
					}
				}
			#endif
			}
		}
	#endif
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
			if (!m_Player)
				return;

			ExpansionNotification(title, text, icon, color, 7, ExpansionNotificationType.TOAST).Create(m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && GetGroup())
		{
			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (!groupPlayer)
					continue;

				ExpansionNotification(title, text, icon, color, 7, ExpansionNotificationType.TOAST).Create(groupPlayer.GetIdentity());
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
			if (m_Player && !m_Player.IsAlive())
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else if (m_IsGroupQuest && GetGroup())
		{
			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer && !groupPlayer.IsAlive())
					return false;
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
			if (m_Player)
				m_QuestModule.CreateClientMarker(pos, name, Config.GetID(), m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetGroup())
		{
			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer)
					m_QuestModule.CreateClientMarker(pos, name, Config.GetID(), groupPlayer.GetIdentity());
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest RemoveMarkers
	// -----------------------------------------------------------
	void RemoveMarkers()
	{
		if (!m_IsGroupQuest)
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (m_Player)
				m_QuestModule.RemoveClientMarkers(Config.GetID(), m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetGroup())
		{
			array<ref ExpansionPartyPlayerData> groupPlayers = GetGroup().GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (groupPlayer)
					m_QuestModule.RemoveClientMarkers(Config.GetID(), groupPlayer.GetIdentity());
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnRecreateClientMarkers
	// -----------------------------------------------------------
	void OnRecreateClientMarkers()
	{
		foreach (ExpansionQuestObjectiveEventBase objective: QuestObjectives)
		{
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

			m_UpdateQueueTimer = 0.0;
		}
		
		if (QuestObjectives.Count() > 0)
		{
			m_CurrentObjectiveTick += timeslice;
			for (int i = 0; i < UPDATE_OBJECTIVES_PER_TICK; ++i)
			{
				if (m_CurrentObjectiveTick >= QuestObjectives.Count())
				{
					m_CurrentObjectiveTick = 0;
				}

				ExpansionQuestObjectiveEventBase objective = QuestObjectives.Get(m_CurrentObjectiveTick);
				if (!objective)
					continue;

				if (objective.IsInitialized() && objective.IsActive())
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
	}

	// -----------------------------------------------------------
	// ExpansionQuest UpdateQuest
	// -----------------------------------------------------------
	void UpdateQuest()
	{
		if (!m_QuestModule)
			return;

		m_QuestModule.UpdateQuestStatesForQuestPlayers(this, State);
		m_QuestModule.UpdateQuestPlayersObjectiveData(this);
		m_QuestModule.UpdatePlayerQuests(this);
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
	// ExpansionQuest GetClosestQuestNPCPosition
	// -----------------------------------------------------------
	vector GetClosestQuestNPCPosition(array<int> ids, vector playerPos)
	{
		float shortestDistance;
		float distance;
		vector npcPos;
		vector closestPos;

		foreach (int npcID: ids)
		{
			ExpansionQuestNPCData questNPC = GetQuestModule().GetQuestNPCDataByID(npcID);
			if (!questNPC)
				continue;

			npcPos = questNPC.GetPosition();
			distance = Math.Round(vector.Distance(playerPos, npcPos));
			if (shortestDistance == 0 || shortestDistance > distance)
			{
				shortestDistance = distance;
				closestPos = npcPos;
			}
		}

		return closestPos;
	}

	array<ItemBase> GetPlayerQuestItems(PlayerBase player, int questID)
	{
		array<ItemBase> questItems = new array<ItemBase>;
		array<string> questItemNames = new array<string>;
		array<ref ExpansionQuestItemConfig> configQuestItems = Config.GetQuestItems();
		foreach (ExpansionQuestItemConfig questItem: configQuestItems)
		{
			questItemNames.Insert(questItem.GetClassName());
		}

		array<EntityAI> items = new array<EntityAI>;
	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item : items)
		{
			if (questItemNames.Find(item.GetType()) > -1)
			{
				ItemBase itemIB;
				if (Class.CastTo(itemIB, item))
				{
					if (itemIB.GetQuestID() == questID)
					{
						QuestPrint(ToString() + " ::GetPlayerQuestItems - Add quest item: " + itemIB.GetType() + " | Quest ID: " + itemIB.GetQuestID());
						questItems.Insert(itemIB);
					}
				}
			}
		}

		return questItems;
	}
};