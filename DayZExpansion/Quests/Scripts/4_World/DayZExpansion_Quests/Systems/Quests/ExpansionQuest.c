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
	protected ExpansionQuestConfig m_Config;
	protected int m_QuestState = ExpansionQuestState.NONE;
	protected ref array<ItemBase> m_QuestItems;	//! Normal items the player will recieve on quest start.
	protected ref array<ref ExpansionQuestObjectiveEventBase> m_QuestObjectives;	//! Quest objectives
	protected PlayerBase m_Player;
	protected string m_PlayerUID;
	protected bool m_IsCompeleted = false;
	protected bool m_Initialized = false;
	protected bool m_IsGroupQuest = false;
#ifdef EXPANSIONMODGROUPS
	protected int m_GroupID = -1;
	protected ref array<string> m_PlayerUIDs;
#endif
	protected int m_ObjectiveIndex;
	protected bool m_ObjectivesCreated = false;
	protected int m_CurrentObjectiveIndex = -1;
	protected int m_CompletedObjectivesCount;

	// -----------------------------------------------------------
	// ExpansionQuest Constructor
	// -----------------------------------------------------------
	void ExpansionQuest(ExpansionQuestModule module)
	{
		QuestDebugPrint(ToString() + "::ExpansionQuest - Start");

		m_QuestModule = module;

		if (!m_QuestItems)
			m_QuestItems = new array<ItemBase>;

		if (!m_QuestObjectives)
			m_QuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	#ifdef EXPANSIONMODGROUPS
		if (!m_PlayerUIDs)
			m_PlayerUIDs = new array<string>;
	#endif

		QuestDebugPrint(ToString() + "::ExpansionQuest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetQuestConfig
	// -----------------------------------------------------------
	void SetQuestConfig(ExpansionQuestConfig config)
	{
		QuestDebugPrint(ToString() + "::SetQuestConfig - Start");

		SetConfig(config);
		CreateObjectivesFromConfig(); //! Create objective instances from quest config
		m_QuestModule.CheckAndSpawnObjectSet(m_Config.GetID());  //! Create quest objects from quest config if not spawned already

		QuestDebugPrint(ToString() + "::SetQuestConfig - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetConfig
	// -----------------------------------------------------------
	void SetConfig(ExpansionQuestConfig config)
	{
		m_Config = config;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateObjectivesFromConfig
	// -----------------------------------------------------------
	protected void CreateObjectivesFromConfig()
	{
		QuestDebugPrint(ToString() + "::CreateObjectivesFromConfig - Start");

		if (!m_Config || !m_Config.GetObjectives())
			return;

		//! Create quest objectives based on quest config.
		array<ref ExpansionQuestObjectiveConfig>  questObjectives = m_Config.GetObjectives();
		foreach (ExpansionQuestObjectiveConfig objectiveConfig: questObjectives)
		{
			int objectiveID = objectiveConfig.GetID();
			int objectiveType = objectiveConfig.GetObjectiveType();
			int time = objectiveConfig.GetTimeLimit();

			if (CreateObjective(m_ObjectiveIndex, objectiveType, objectiveID, time))
			{
				QuestDebugPrint(ToString() + "::CreateObjectivesFromConfig - Created quest objective for quest with ID:" + m_Config.GetID() + " [Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + m_ObjectiveIndex + "]");
				m_ObjectiveIndex++;
			}
			else
			{
				Error(ToString() + "::CreateObjectivesFromConfig - Could not create quest objective for quest with ID:" + m_Config.GetID() + "[Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + m_ObjectiveIndex + "]");
				return;
			}
		}

		if (m_QuestObjectives.Count() == m_Config.GetObjectives().Count())
			m_ObjectivesCreated = true;

		QuestDebugPrint(ToString() + "::CreateObjectivesFromConfig - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateObjective
	// -----------------------------------------------------------
	protected bool CreateObjective(int index, int objectiveType, int objectiveID, int time)
	{
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
				m_QuestObjectives.Insert(travelObjectiveEvent);
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
				m_QuestObjectives.Insert(deliveryObjectiveEvent);
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
				m_QuestObjectives.Insert(targetObjectiveEvent);
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
				m_QuestObjectives.Insert(collectionObjectiveEvent);
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
				m_QuestObjectives.Insert(treasureHuntObjectiveEvent);
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
				m_QuestObjectives.Insert(actionObjectiveEvent);
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
				m_QuestObjectives.Insert(craftingObjectiveEvent);
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
				m_QuestObjectives.Insert(aiPatrolObjectiveEvent);
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
				m_QuestObjectives.Insert(aiCampObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.AIESCORD:
			{
				ExpansionQuestObjectiveAIEscordConfig aiEscordConfig = m_QuestModule.GetAIEscordObjectiveConfigByID(objectiveID);
				if (!aiEscordConfig)
					return false;

				ExpansionQuestObjectiveAIEscortEvent aiEscordObjectiveEvent = new ExpansionQuestObjectiveAIEscortEvent(this);
				aiEscordObjectiveEvent.SetIndex(index);
				aiEscordObjectiveEvent.SetObjectiveConfig(aiEscordConfig);
				aiEscordObjectiveEvent.SetTimeLimit(time);
				m_QuestObjectives.Insert(aiEscordObjectiveEvent);
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
		return m_Config;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetQuestState
	// -----------------------------------------------------------
	void SetQuestState(int state)
	{
		m_QuestState = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestState
	// -----------------------------------------------------------
	int GetQuestState()
	{
		return m_QuestState;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetPlayerUID
	// -----------------------------------------------------------
	void SetPlayerUID(string playerUID)
	{
		QuestDebugPrint(ToString() + "::SetPlayerUID - Start");
		QuestDebugPrint(ToString() + "::SetPlayerUID - Set UID: " + playerUID);

		m_PlayerUID = playerUID;
		m_Player = PlayerBase.GetPlayerByUID(playerUID);

	#ifdef EXPANSIONMODGROUPS
		if (m_PlayerUIDs.Find(playerUID) == -1)
			m_PlayerUIDs.Insert(playerUID);
	#endif

		QuestDebugPrint(ToString() + "::SetPlayerUID - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayer
	// -----------------------------------------------------------
	PlayerBase GetPlayer()
	{
		if (!m_Player)
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		return m_Player;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayerUID
	// -----------------------------------------------------------
	string GetPlayerUID()
	{
		return m_PlayerUID;
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetQuestItems
	// -----------------------------------------------------------
	array<ItemBase> GetQuestItems()
	{
		return m_QuestItems;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsCompeleted
	// -----------------------------------------------------------
	bool IsCompeleted()
	{
		return m_IsCompeleted;
	}

	// -----------------------------------------------------------
	// ExpansionQuest SetIsCompleted
	// -----------------------------------------------------------
	void SetIsCompleted(bool state)
	{
		m_IsCompeleted = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestStart
	// -----------------------------------------------------------
	//! Event called when a quest is started
	bool OnQuestStart()
	{
		QuestDebugPrint(ToString() + "::OnQuestStart - Start");

		if (!ValidateQuest())
		{
			Error(ToString() + "::OnQuestStart - Quest validation failed!");
			return false;
		}

		//! Start quest objectives
		ExpansionQuestObjectiveEventBase objective = GetObjectives()[0];
		//! We only start the first objective as we will progress thrue all objective events in a sequential order.
		if (objective && objective.GetIndex() == 0)
		{
			m_CurrentObjectiveIndex = 0;
			if (!objective.OnStart(false))
				return false;
		}

		//! Add all quest items to the players inventory
		CreateQuestItems();

		SetInitialized(true);

		SetQuestState(ExpansionQuestState.STARTED);

		if (!m_Config.IsAchivement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedText, m_Config.GetTitle()), ExpansionIcons.GetPath("Questionmark 2"), COLOR_EXPANSION_NOTIFICATION_INFO);

		UpdateQuest();

		QuestDebugPrint(ToString() + "::OnQuestStart - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest ValidateQuest
	// -----------------------------------------------------------
	protected bool ValidateQuest()
	{
		if (m_PlayerUID == string.Empty)
		{
			Error(ToString() + "::OnQuestStart - Client UID not set!");
			return false;
		}

		if (!m_ObjectivesCreated)
		{
			Error(ToString() + "::ValidateQuest - Quest objectives not created!");
			return false;
		}

		if (!m_Config)
		{
			Error(ToString() + "::OnQuestStart - Could not get quest config!");
			return false;
		}

	#ifdef EXPANSIONMODGROUPS
		if (m_Config.IsGroupQuest() && m_GroupID == -1)
		{
			Error(ToString() + "::OnQuestStart - Group ID not set!");
			return false;
		}
	#endif


		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesComplete
	// -----------------------------------------------------------
	//! Event called when all quest objectives are completed
	void OnQuestObjectivesComplete(bool createTurnInMarker = false)
	{
		QuestDebugPrint(ToString() + "::OnQuestObjectivesComplete - Start");

		if (GetQuestState() >= ExpansionQuestState.CAN_TURNIN)
		{
			QuestDebugPrint(ToString() + "::OnQuestObjectivesComplete - QUEST STATE IS ALREADY TURNIN!");
			return;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Can't get quest module!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (!m_Player)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest player!");
			return;
		}

		if (!m_Config)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest config!");
			return;
		}

		SetQuestState(ExpansionQuestState.CAN_TURNIN);

		if (!m_Config.IsAchivement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);

	#ifdef EXPANSIONMODNAVIGATION
		if (createTurnInMarker)
		{
			RemoveMarkers();
			if (!m_Config.IsAutocomplete())
			{
				//! Create a marker on the closest quest npc location for the player
				vector npcPos = GetClosestQuestNPCPosition(m_Config.GetQuestTurnInIDs(), m_Player.GetPosition());
				CreateClientMarker(npcPos, "#STR_EXPANSION_QUEST_MARKER_TURNIN");
			}
		}
	#endif

		if (m_Config.IsAutocomplete())
			m_QuestModule.RequestCompleteQuestServer(m_Config.GetID(), GetPlayerUID(), m_Player.GetIdentity(), true);

		QuestDebugPrint(ToString() + "::OnQuestObjectivesComplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestObjectivesIncomplete
	// -----------------------------------------------------------
	//! Event called when a quest objective state has changed to incomplete after it was completed once
	void OnQuestObjectivesIncomplete()
	{
		QuestDebugPrint(ToString() + "::OnQuestObjectivesIncomplete - Start");

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesIncomplete - Can't get quest module!");
			return;
		}

		SetQuestState(ExpansionQuestState.STARTED);
		UpdateQuest();
		QuestDebugPrint(ToString() + "::OnQuestObjectivesIncomplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestTurnIn
	// -----------------------------------------------------------
	//! Event called when ever a quest is completed and turned-in
	bool OnQuestTurnIn(string playerUID, ExpansionQuestRewardConfig reward = null)
	{
		QuestDebugPrint(ToString() + "::OnQuestTurnIn - Start");

		if (GetQuestState() != ExpansionQuestState.CAN_TURNIN)
		{
			QuestDebugPrint(ToString() + "::OnQuestTurnIn - QUEST STATE INVALID FOR TURN-IN!");
			return false;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestTurnIn - Can't get quest module!");
			return false;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (m_Player)
		{
			if (!m_Config && m_Config.GetID() == -1)
			{
				Error(ToString() + "::OnQuestTurnIn - Could not get quest config!");
				return false;
			}

			//! Call end event on all quest objectives
			for (int i = m_QuestObjectives.Count() - 1; i >= 0; i--)
			{
				ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
				if (!objective.OnTurnIn(playerUID))
					return false;

				m_QuestObjectives.RemoveOrdered(i);
			}

			//! Add all quest rewards to the players inventory
			if (GetQuestConfig().GetRewards().Count() > 0 || GetQuestConfig().GetReputationReward() > 0)
				SpawnQuestRewards(playerUID, reward);

			if (!m_Config.IsAchivement())
			{
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
			}
			else
			{
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().AchivementCompletedTitle, m_Config.GetTitle()), new StringLocaliser(GetExpansionSettings().GetQuest().AchivementCompletedText, m_Config.GetObjectiveText()), ExpansionIcons.GetPath("Star"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);
			}

			SetIsCompleted(true);

			if (m_Config.GetFollowUpQuestID() > -1)
			{
				PlayerBase questPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (!questPlayer || !questPlayer.GetIdentity())
					return false;

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_QuestModule.RequestOpenQuestMenuCB, 1000, false, m_Config.GetQuestTurnInIDs(), questPlayer.GetIdentity());
			}
		}

		SetQuestState(ExpansionQuestState.COMPLETED);
		UpdateQuest();
		OnQuestCleanup();

		QuestDebugPrint(ToString() + "::OnQuestTurnIn - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCancel
	// -----------------------------------------------------------
	//! Event called when a quest is manualy canceled by the player
	bool OnQuestCancel()
	{
		QuestDebugPrint(ToString() + "::OnQuestCancel - Start");

		SetQuestState(ExpansionQuestState.NONE);

		//! Cancel all active quest objectives
		for (int i = m_QuestObjectives.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
			if (!objective.OnCancel())
				return false;

			m_QuestObjectives.RemoveOrdered(i);
		}

		//! Call on quest cleanup event when quest is canceled.
		OnQuestCleanup();

		if (!m_Config.IsAchivement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);

		UpdateQuest();

		QuestDebugPrint(ToString() + "::OnQuestCancel - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestContinue
	// -----------------------------------------------------------
	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	bool OnQuestContinue()
	{
		QuestDebugPrint(ToString() + "::OnQuestContinue - Start");
		QuestDebugPrint(ToString() + "::OnQuestContinue - Quest ID: " + m_Config.GetID() + " | State: " + m_QuestState + " | Current objective index: " + m_CurrentObjectiveIndex);
		
		if (!ValidateQuest())
		{
			Error(ToString() + "::OnQuestContinue - Quest validation failed!");
			return false;
		}

		//! Continue active quest objectives
		for (int i = 0; i < m_QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
			//! We only start all active objectives in range of the m_CurrentObjectiveIndex as we will progress thrue all objective events in a sequential order.
			if (objective && objective.GetIndex() <= m_CurrentObjectiveIndex)
			{
				//! We Only start objectives that are not initialized yet or not completed when they have no dynamic state.
				//! If the objective event has a dynamic state or was set active from the progression data we allow also the start of objective events here.
				if (!objective.IsInitialized() && (objective.HasDynamicState() || objective.IsActive()))
				{
					QuestDebugPrint(ToString() + "::OnQuestContinue - Continue quest objective. Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
					objective.QuestDebug();

					if (!objective.OnStart(true))
					{
						QuestDebugPrint(ToString() + "::OnQuestContinue - Continue quest objective faile! Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
						return false;
					}
				}
			}
		}

		if (m_Config.GetQuestItems().Count() > 0)
		{
			PlayerBase player;
			array<string> questItemsNames = new array<string>;
			map<string, int> questItemsMap = new map<string, int>;
			map<string, int> questItemsInventoryMap;
			array<ItemBase> playerItems = new array<ItemBase>;

			foreach (ExpansionQuestItemConfig itemConfig: m_Config.GetQuestItems())
			{
				string typeName = itemConfig.GetClassName();
				int needed = itemConfig.GetAmount();
				int current;
				if (!questItemsMap.Find(typeName, current))
				{
					questItemsNames.Insert(typeName);
					questItemsMap.Insert(typeName, needed);
				}
				else
				{
					int newAmount = current + needed;
					questItemsMap.Set(typeName, newAmount);
				}
			}

			int overallCount;
			int overallNeeded;

			if (!m_IsGroupQuest)
			{
				player = PlayerBase.GetPlayerByUID(m_PlayerUID);
				if (!player)
					return false;

				questItemsInventoryMap = new map<string, int>;
				playerItems = GetPlayerQuestItems(player, m_Config.GetID(), questItemsNames, questItemsInventoryMap);
				foreach (ItemBase item: playerItems)
				{
					m_QuestItems.Insert(item);
				}

				foreach (string itemName, int itemNeeded: questItemsMap)
				{
					overallNeeded += itemNeeded;

					int amount;
					if (questItemsInventoryMap.Find(itemName, amount))
					{
						overallCount += amount;
					}
				}

				if (overallCount != overallNeeded)
				{
					QuestDebugPrint(ToString() + "::OnQuestContinue - Overall needed items: " + overallNeeded);
					QuestDebugPrint(ToString() + "::OnQuestContinue - Overall inventory items: " + overallCount);
					return false;
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				foreach (string playerUID: m_PlayerUIDs)
				{
					player = PlayerBase.GetPlayerByUID(playerUID);
					if (!player)
						return false;

					questItemsInventoryMap = new map<string, int>;
					playerItems = GetPlayerQuestItems(player, m_Config.GetID(), questItemsNames, questItemsInventoryMap);
					foreach (ItemBase memberItem: playerItems)
					{
						m_QuestItems.Insert(memberItem);
					}

					foreach (string itemNameG, int itemNeededG: questItemsMap)
					{
						overallNeeded += itemNeededG;

						int amountG;
						if (questItemsInventoryMap.Find(itemNameG, amountG))
						{
							overallCount += amountG;
						}
					}

					if (overallCount != overallNeeded)
					{
						QuestDebugPrint(ToString() + "::OnQuestContinue - Overall needed items: " + overallNeeded);
						QuestDebugPrint(ToString() + "::OnQuestContinue - Overall inventory items: " + overallNeeded);
						return false;
					}
				}
			}
		#endif
		}

		SetInitialized(true);
		UpdateQuest();

		QuestDebugPrint(ToString() + "::OnQuestContinue - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CleanupQuestItems
	// -----------------------------------------------------------
	void CleanupQuestItems()
	{
		QuestDebugPrint(ToString() + "::CleanupQuestItems - Start");

		for (int i = m_QuestItems.Count() - 1; i >= 0; i--)
		{
			ItemBase item = m_QuestItems[i];
			GetGame().ObjectDelete(item);
			m_QuestItems.RemoveOrdered(i);
		}

		QuestDebugPrint(ToString() + "::CleanupQuestItems - End");
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
			array<ref ExpansionQuestItemConfig> questItemConfigs = m_Config.GetQuestItems();
			foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
			{
				obj = Spawn(questItem, m_Player, playerEntity, m_Player.GetPosition(), m_Player.GetOrientation(), questItem.GetAmount());
				if (Class.CastTo(questItemBase, obj))
				{
					questItemBase.SetQuestID(m_Config.GetID());
					m_QuestItems.Insert(questItemBase);
				}
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				QuestDebugPrint(ToString() + "::CreateQuestItems - Spawn quest items for player with UID: " + memberUID);
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				array<ref ExpansionQuestItemConfig> questItems = m_Config.GetQuestItems();
				foreach (ExpansionQuestItemConfig questConfigItem: questItems)
				{
					obj = Spawn(questConfigItem, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation(), questConfigItem.GetAmount());
					if (Class.CastTo(questItemBase, obj))
					{
						questItemBase.SetQuestID(m_Config.GetID());
						m_QuestItems.Insert(questItemBase);
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
		QuestDebugPrint(ToString() + "::OnGroupMemberJoined - Start");

		if (m_IsGroupQuest)
		{
			if (m_Config.GetQuestItems().Count() > 0)
			{
				QuestDebugPrint(ToString() + "::OnGroupMemberJoined - New group member joined. Check if he has already all quest items..");
	
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (!groupPlayer)
				{
					Error(ToString() + "::OnGroupMemberJoined - Could not get player with UID: " + playerUID);
					return;
				}
	
				array<ItemBase> playerQuestItems = GetPlayerQuestItems(groupPlayer, m_Config.GetID(), null);
				if (!playerQuestItems || playerQuestItems.Count() == 0)
				{
					QuestDebugPrint(ToString() + "::OnGroupMemberJoined - Player has no quest items. Add items");
					array<ref ExpansionQuestItemConfig> questItemConfigs = m_Config.GetQuestItems();
					foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
					{
						Object obj = Spawn(questItem, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation(), questItem.GetAmount());
						ItemBase questItemBase;
						if (Class.CastTo(questItemBase, obj))
						{
							questItemBase.SetQuestID(m_Config.GetID());
							m_QuestItems.Insert(questItemBase);
						}
					}
				}
			}
			
			ExpansionQuestObjectiveEventBase currentActiveObjective = m_QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
			{
				Error(ToString() + "::OnGroupMemberJoined - Could not get current active objective for index " + m_CurrentObjectiveIndex);
				return;
			}

			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized())
				currentActiveObjective.OnGroupMemberJoined(playerUID);
			
			if (m_PlayerUIDs.Find(playerUID) == -1)
				m_PlayerUIDs.Insert(playerUID);
		}

		QuestDebugPrint(ToString() + "::OnGroupMemberJoined - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnGroupMemberLeave
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID)
	{
		QuestDebugPrint(ToString() + "::OnGroupMemberLeave - Start");

		if (m_IsGroupQuest)
		{
			if (m_Config.GetQuestItems().Count() > 0)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (!groupPlayer)
				{
					Error(ToString() + "::OnGroupMemberLeave - Could not get player with UID: " + playerUID);
					return;
				}
				
				array<ItemBase> playerQuestItems = GetPlayerQuestItems(groupPlayer, m_Config.GetID(), null);
				if (playerQuestItems && playerQuestItems.Count() == m_Config.GetQuestItems().Count())
				{
					foreach (ItemBase item: playerQuestItems)
					{
						int findIndex = m_QuestItems.Find(item);
						if (findIndex > -1)
						{
							m_QuestItems.Remove(findIndex);
							GetGame().ObjectDelete(item);
						}
					}
				}
			}

			ExpansionQuestObjectiveEventBase currentActiveObjective = m_QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
			{
				Error(ToString() + "::OnGroupMemberLeave - Could not get current active objective for index " + m_CurrentObjectiveIndex);
				return;
			}

			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized())
				currentActiveObjective.OnGroupMemberLeave(playerUID);
			
			int index = m_PlayerUIDs.Find(playerUID);
			if (index > -1)
				m_PlayerUIDs.Remove(index);
		}

		QuestDebugPrint(ToString() + "::OnGroupMemberLeave - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayerUIDs
	// -----------------------------------------------------------
	array<string> GetPlayerUIDs()
	{
		return m_PlayerUIDs;
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuest OnQuestCleanup
	// -----------------------------------------------------------
	//! Event called when quest instance is destroyed/cleaned-up
	bool OnQuestCleanup()
	{
		QuestDebugPrint(ToString() + "::OnQuestCleanup - Start");

		//! Call cleanup event on all active quest objectives
		for (int i = m_QuestObjectives.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
			if (!objective.OnCleanup())
				QuestDebugPrint(ToString() + "::OnQuestCleanup - Something went wrong on objective event cleanup for quest with id: " + m_Config.GetID());
			
			objective = null;
			m_QuestObjectives.RemoveOrdered(i);
		}

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(m_Config.GetID());

		if (m_QuestState == ExpansionQuestState.NONE || m_QuestState == ExpansionQuestState.COMPLETED)
			CleanupQuestItems();

	#ifdef EXPANSIONMODNAVIGATION
		RemoveMarkers();
	#endif
		SetInitialized(false);

		QuestDebugPrint(ToString() + "::OnQuestCleanup - End");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestCompletionCheck
	// -----------------------------------------------------------
	//! Main quest objective completion check
	void QuestCompletionCheck()
	{
		QuestDebugPrint(ToString() + "::QuestCompletionCheck - Start");
		int complededObjectives;	//! Amount of completed quest objectives.
		int objectivesCount;	//! Amount of all quest objectives.
		bool createTurnInMarker = true;

		if (!m_QuestObjectives || m_QuestObjectives.Count() == 0)
		{
			QuestDebugPrint(ToString() + "::QuestCompletionCheck - Quest has no configured objectives. Cancel quest!");
			CancelQuest();
			return;
		}

		//! Collect the amount of completed objectives and fire there OnComplete or OnIncomplete events depending on the given conditions.
		foreach (ExpansionQuestObjectiveEventBase currentObjective: m_QuestObjectives)
		{
			objectivesCount++;

			//! If the objective if active + incomplete + can be completed then call the objectives OnComplete event.
			if (currentObjective.IsActive() && !currentObjective.IsCompleted() && currentObjective.CanComplete())
				currentObjective.OnComplete();

			//! If the objective if active + completed + can no longer be completed then call the objectives OnIncomplete event.
			if (currentObjective.IsActive() && currentObjective.IsCompleted() && !currentObjective.CanComplete())
				currentObjective.OnIncomplete();

			//! If the ojectective is now still completed then count it as completed ojective.
			if (currentObjective.IsCompleted())
				complededObjectives++;
		}

		QuestDebugPrint(ToString() + "::QuestCompletionCheck - Total objective count: " + objectivesCount);
		QuestDebugPrint(ToString() + "::QuestCompletionCheck - Total completed objectives: " + complededObjectives);

		//! If all quest objectives are completed then fire the OnQuestObjectivesComplete event.
		if (complededObjectives == objectivesCount && m_QuestState == ExpansionQuestState.STARTED)
		{
			QuestDebugPrint(ToString() + "::QuestCompletionCheck - All quest objectives completed!");
			OnQuestObjectivesComplete(createTurnInMarker);
		}
		else if (complededObjectives < objectivesCount)
		{
			//! If not all quest objectives are completed but the quest state is on CAN_TURNIN then the objective state has changed and we fire the OnQuestObjectivesIncomplete event.
			if (m_QuestState == ExpansionQuestState.CAN_TURNIN)
			{
				QuestDebugPrint(ToString() + "::QuestCompletionCheck - INCOMPLETE");
				OnQuestObjectivesIncomplete();
				QuestDebugPrint(ToString() + "::QuestCompletionCheck - Current active objective index: " + m_CurrentObjectiveIndex);
			}

			ExpansionQuestObjectiveEventBase currentActiveObjective = m_QuestObjectives[m_CurrentObjectiveIndex];
			if (!currentActiveObjective)
			{
				if (!currentActiveObjective)
				{
					QuestDebugPrint(ToString() + "::QuestCompletionCheck - Could not get current active objective instance! Cancel quest..");
					CancelQuest();
					return;
				}
			}

			//! Check if our currect active objective is completed and if so try to get the next objective in the list and start it.
			if (currentActiveObjective.IsActive() && currentActiveObjective.IsInitialized() && currentActiveObjective.IsCompleted())
			{
				SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestObjectiveCompletedText, currentActiveObjective.GetObjectiveConfig().GetObjectiveText(), m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);
				m_CurrentObjectiveIndex++;
				QuestDebugPrint(ToString() + "::QuestCompletionCheck - Current active objective is completed! Try to start next objective with index: " + m_CurrentObjectiveIndex);

				//! Get the next quest objective from m_QuestObjectives and start it.
				ExpansionQuestObjectiveEventBase nextObjective = m_QuestObjectives[m_CurrentObjectiveIndex];
				if (nextObjective && !nextObjective.IsInitialized() && !nextObjective.IsActive())
				{
				#ifdef EXPANSIONMODNAVIGATION
					RemoveMarkers();
				#endif

					//! Start the next quest objective
					if (!nextObjective.OnStart(false))
					{
						QuestDebugPrint(ToString() + "::QuestCompletionCheck - Could not start next quest objective instance! Cancel quest..");
						CancelQuest();
						return;
					}

					QuestDebugPrint(ToString() + "::QuestCompletionCheck - Started next quest objective event with index: " + m_CurrentObjectiveIndex);
				}
			}
		}

		if (m_CompletedObjectivesCount != complededObjectives)
		{
			m_CompletedObjectivesCount = complededObjectives;
			UpdateQuest();
		}

		QuestDebugPrint(ToString() + "::QuestCompletionCheck - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest SpawnReward
	// -----------------------------------------------------------
	protected void SpawnReward(ExpansionQuestRewardConfig reward, PlayerBase player, EntityAI parent, vector pos, vector ori)
	{
		int remainingAmount = reward.GetAmount();
		while (remainingAmount > 0)
		{
			int remainingAmountBefore = remainingAmount;
			Object obj = Spawn(reward, player, parent, pos, ori, remainingAmount);
			if (!obj)
			{
				Error("Error: Couldn't spawn " + reward.GetClassName());
				break;
			}

			if (remainingAmount == remainingAmountBefore)
			{
				//! Should not be possible, just in case...
				Error("Error: Spawning " + reward.GetClassName() + " did not affect remaining amount!");
				break;
			}
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuest Spawn
	// -----------------------------------------------------------
	//! Spawn object method
	protected Object Spawn(ExpansionQuestItemConfig item, PlayerBase player, EntityAI parent, vector position, vector orientation, out int remainingAmount)
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
	protected Object Spawn(ExpansionQuestRewardConfig item, PlayerBase player, EntityAI parent, vector position, vector orientation, out int remainingAmount)
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

		float dmg = item.GetDamagePercent();

		Print(ToString() + "::Spawn - Set object damage %: " + dmg);

		EntityAI objEntity;
		if (Class.CastTo(objEntity, obj))
			ProcessHealth01(objEntity, 1 - dmg / 100);

		return obj;
	}

	// -----------------------------------------------------------
	// ExpansionQuest ProcessHealth01
	// -----------------------------------------------------------
	protected void ProcessHealth01(EntityAI parent, float health01)
	{
		parent.SetHealth("", "Health", parent.GetMaxHealth("", "Health") * health01);

		array<EntityAI> items = new array<EntityAI>;
		parent.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item.IsInherited(SurvivorBase))
				continue;

			item.SetHealth("", "Health", item.GetMaxHealth("", "Health") * health01);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuest GetObjectives
	// -----------------------------------------------------------
	array<ref ExpansionQuestObjectiveEventBase> GetObjectives()
	{
		return m_QuestObjectives;
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
		foreach (ExpansionQuestObjectiveEventBase objective: m_QuestObjectives)
		{
			if (objective.IsCompleted())
				complededObjectives++;
		}

		if (complededObjectives == m_QuestObjectives.Count())
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
		QuestDebugPrint(ToString() + "::SetGroup - Start");

		m_GroupID = groupID;
		m_IsGroupQuest = true;

		QuestDebugPrint(ToString() + "::SetGroup - Group ID: " + m_GroupID);
		QuestDebugPrint(ToString() + "::SetGroup - Quest is group quest: " + m_IsGroupQuest);
		QuestDebugPrint(ToString() + "::SetGroup - End");
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
		foreach (string memberUID: m_PlayerUIDs)
		{
			if (memberUID == playerUID)
				return true;
		}

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsOtherGroupMemberOnline
	// -----------------------------------------------------------
	bool IsOtherGroupMemberOnline(string excludeUID)
	{
		foreach (string memberUID: m_PlayerUIDs)
		{
			if (memberUID == excludeUID)
				continue;
			
			PlayerBase player = PlayerBase.GetPlayerByUID(memberUID);
			if (player)
				return true;
		}

		return false;
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuest SpawnQuestRewards
	// -----------------------------------------------------------
	void SpawnQuestRewards(string playerUID, ExpansionQuestRewardConfig reward = null)
	{
	//! If group mod is not loaded but the quest is flaged as a group quest make it a normal one.
	#ifndef EXPANSIONMODGROUPS
		if (m_IsGroupQuest)
			m_IsGroupQuest = false;
	#endif

		array<ref ExpansionQuestRewardConfig> questRewards = m_Config.GetRewards();
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(playerUID);
		if (!m_IsGroupQuest)
		{
			EntityAI playerEntity = questPlayer;
			int remainingAmount;
			int remainingAmountBefore;
			Object obj;

			if (m_Config.NeedToSelectReward())
			{
				if (reward)
				{
					QuestDebugPrint(ToString() + "::SpawnQuestRewards - Spawn selected reward: " + reward.ToString());
					SpawnReward(reward, questPlayer, playerEntity, questPlayer.GetPosition(), questPlayer.GetOrientation());
				}
			}
			else
			{
				foreach (ExpansionQuestRewardConfig questReward: questRewards)
				{
					SpawnReward(questReward, questPlayer, playerEntity, questPlayer.GetPosition(), questPlayer.GetOrientation());
				}
			}

		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().UseReputation && m_Config.GetReputationReward() != 0)
			{
				if (questPlayer)
				{
					questPlayer.Expansion_AddReputation(m_Config.GetReputationReward());
				}
			}
		#endif
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			bool isGroupOwnerOnline = false;
			PlayerBase groupOwner = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (groupOwner)
				isGroupOwnerOnline = true;

			if (m_Config.RewardsForGroupOwnerOnly() && !isGroupOwnerOnline)
			{
				//! ToDo: Notification to online group- aka quest-members.
				return;
			}

			foreach (string memberUID: m_PlayerUIDs)
			{
				if (m_Config.RewardsForGroupOwnerOnly())
				{
					QuestDebugPrint(ToString() + "::SpawnQuestRewards - Quest rewards for quest " + GetQuestConfig().GetID() + " are for the quest owner only.");
					if (memberUID != m_PlayerUID)
					{
						QuestDebugPrint(ToString() + "::SpawnQuestRewards - Player [UID: " + memberUID + "] is not the group owner. Skip!");
						continue;
					}
				}

				QuestDebugPrint(ToString() + "::SpawnQuestRewards - Try to spawn quest reward for player with UID: " + memberUID);
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
				{
					QuestDebugPrint(ToString() + "::SpawnQuestRewards - Could not get player with UID: " + memberUID + ". Not online?!");
					continue;
				}

				if (m_Config.NeedToSelectReward && reward)
				{
					QuestDebugPrint(ToString() + "::SpawnQuestRewards - Spawn selected reward: " + reward.ToString());
					SpawnReward(reward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), m_Player.GetOrientation());
				}
				else if (!m_Config.NeedToSelectReward)
				{
					//! Add all quest rewards to the players inventory
					foreach (ExpansionQuestRewardConfig groupQuestReward: questRewards)
					{
						SpawnReward(groupQuestReward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
					}
				}

			#ifdef EXPANSIONMODHARDLINE
				if (GetExpansionSettings().GetHardline().UseReputation && m_Config.GetReputationReward() != 0)
				{
					if (groupPlayer)
					{
						groupPlayer.Expansion_AddReputation(m_Config.GetReputationReward());
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
		return m_QuestObjectives.Count();
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
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
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
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (groupPlayer && !groupPlayer.IsAlive())
					return false;
			}
		}
	#endif

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuest IsQuestPlayer
	// -----------------------------------------------------------
	bool IsQuestPlayer(string uid)
	{
		if (!m_IsGroupQuest)
		{
			if (GetPlayerUID() == uid)
				return true;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				if (memberUID == uid)
					return true;
			}
		}
	#endif

		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuest CreateClientMarker
	// -----------------------------------------------------------
#ifdef EXPANSIONMODNAVIGATION
	void CreateClientMarker(vector pos, string name)
	{
		if (!m_IsGroupQuest)
		{
			if (m_Player)
				m_QuestModule.CreateClientMarker(pos, name, m_Config.GetID(), m_Player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				m_QuestModule.CreateClientMarker(pos, name, m_Config.GetID(), groupPlayer.GetIdentity());
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
			PlayerBase player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (!player)
				return;

			m_QuestModule.RemoveClientMarkers(m_Config.GetID(), player.GetIdentity());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				m_QuestModule.RemoveClientMarkers(m_Config.GetID(), groupPlayer.GetIdentity());
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuest OnRecreateClientMarkers
	// -----------------------------------------------------------
	void OnRecreateClientMarkers()
	{
		foreach (ExpansionQuestObjectiveEventBase objective: m_QuestObjectives)
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
		QuestDebugPrint(ToString() + "::SetCurrentObjectiveIndex - Start");

		m_CurrentObjectiveIndex = index;
		
		QuestDebugPrint(ToString() + "::SetCurrentObjectiveIndex - Set current objective index: " + m_CurrentObjectiveIndex);
		QuestDebugPrint(ToString() + "::SetCurrentObjectiveIndex - End");
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

	// -----------------------------------------------------------
	// ExpansionQuest GetPlayerQuestItems
	// -----------------------------------------------------------
	array<ItemBase> GetPlayerQuestItems(PlayerBase player, int questID, array<string> questItemsNames, inout map<string, int> questItemsInventoryMap = null)
	{
		QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Start");
		QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Check player for quest items. UID: " + player.GetIdentity().GetId() + " | Quest ID: " + questID);

		array<ItemBase> questItems = new array<ItemBase>;
		array<EntityAI> items = new array<EntityAI>;
	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item : items)
		{
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;

			string typeName = itemIB.GetType();
			if (questItemsNames.Find(typeName) == -1)
			{
				QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Item " + typeName + " is not in the quest items configuration. Skip..");
				continue;
			}

			QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Item Quest ID: " + itemIB.GetQuestID());

			if (itemIB.GetQuestID() != questID)
			{
				QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Item " + typeName + " is not a quest item for this quest. Skip..");
				continue;
			}

			int amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(itemIB);
			int current;

			if (questItemsInventoryMap)
			{
				if (!questItemsInventoryMap.Find(typeName, current))
				{
					QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Add item " + typeName + " to inventory item map. Amount: " + amount);
					questItemsInventoryMap.Insert(typeName, amount);
				}
				else
				{
					int newAmount = current + amount;
					QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Set item " + typeName + " in inventory item map. Amount: " + newAmount);
					questItemsInventoryMap.Set(typeName, newAmount);
				}
			}

			QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - Add quest item: " + itemIB.GetType() + " | Quest ID: " + itemIB.GetQuestID());
			questItems.Insert(itemIB);
		}

		QuestDebugPrint(ToString() + " ::GetPlayerQuestItems - End");

		return questItems;
	}

	// -----------------------------------------------------------
	// ExpansionQuest UpdateQuest
	// -----------------------------------------------------------
	void UpdateQuest()
	{
		QuestDebugPrint(ToString() + "::UpdateQuest - Start");

		if (!m_QuestModule)
		{
			Error(ToString() + "::UpdateQuest - Could not get quest module!");
			return;
		}

		//! Set updated quest state and objective data on all quest players and send it to them.
		m_QuestModule.UpdateQuestForQuestPlayers(this, m_QuestState);

		QuestDebugPrint(ToString() + "::UpdateQuest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuest CancelQuest
	// -----------------------------------------------------------
	void CancelQuest()
	{
		m_QuestModule.CancelQuestServer(this);
	}

	// -----------------------------------------------------------
	// ExpansionQuest QuestDebugPrint
	// -----------------------------------------------------------
	void QuestDebugPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		Print(text);
	#endif
	}
};