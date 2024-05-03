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
	protected ExpansionQuestState m_QuestState = ExpansionQuestState.NONE;
	protected ref array<ItemBase> m_QuestItems = new array<ItemBase>;	//! Normal items the player will recieve on quest start.
	protected ref array<ref ExpansionQuestObjectiveEventBase> m_QuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;
	protected PlayerBase m_Player;
	protected string m_PlayerUID;
	protected bool m_IsCompleted = false;
	protected bool m_Initialized = false;
	protected int m_GroupID = -1;
	protected ref set<string> m_PlayerUIDs = new set<string>;
	protected bool m_ObjectivesCreated = false;
	protected int m_CurrentObjectiveIndex = -1;
	protected int m_CompletedObjectivesCount;
	
	protected ItemBase m_QuestGiverItem;

	void ExpansionQuest(ExpansionQuestModule module, ExpansionQuestConfig config)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_QuestModule = module;
		m_Config = config;
	}

	void Finalize()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		CreateObjectivesFromConfig(); //! Create objective instances from quest config
		m_QuestModule.CheckAndSpawnObjectSet(m_Config.GetID());  //! Create quest objects from quest config if not spawned already
	}

	void SetConfig(ExpansionQuestConfig config)
	{
		m_Config = config;
	}

	protected void CreateObjectivesFromConfig()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_Config || !m_Config.GetObjectives())
			return;

		//! Create quest objectives based on quest config.
		array<ref ExpansionQuestObjectiveConfigBase>  questObjectives = m_Config.GetObjectives();
		for (int i = 0; i < questObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveConfigBase objectiveConfig = questObjectives[i];
			int objectiveID = objectiveConfig.GetID();
			int objectiveType = objectiveConfig.GetObjectiveType();

			if (CreateObjective(i, objectiveType, objectiveID))
			{
				QuestDebugPrint("Created quest objective for quest with ID: " + m_Config.GetID() + " [Type: " + objectiveType + " | ID: " + objectiveID + " | Index: " + i + "]");
			}
			else
			{
				Error(ToString() + "::CreateObjectivesFromConfig - Could not create quest objective for quest with ID:" + m_Config.GetID() + "[Type:" + objectiveType + " | ID: " + objectiveID + " | Index: " + i + "]");
				return;
			}
		}

		if (m_QuestObjectives.Count() == m_Config.GetObjectives().Count())
			m_ObjectivesCreated = true;
	}

	protected bool CreateObjective(int index, int objectiveType, int objectiveID)
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
				travelObjectiveEvent.SetTimeLimit(travelConfig.GetTimeLimit());
				travelObjectiveEvent.SetLocationPosition(travelConfig.GetPosition());
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
				deliveryObjectiveEvent.SetTimeLimit(deliveryConfig.GetTimeLimit());
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
				targetObjectiveEvent.SetTimeLimit(targetConfig.GetTimeLimit());
				targetObjectiveEvent.SetAmount(targetConfig.GetAmount());
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
				collectionObjectiveEvent.SetTimeLimit(collectionConfig.GetTimeLimit());
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
				treasureHuntObjectiveEvent.SetTimeLimit(treasureHuntConfig.GetTimeLimit());
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
				actionObjectiveEvent.SetTimeLimit(actionObjectiveConfig.GetTimeLimit());
				actionObjectiveEvent.SetExecutionAmount(actionObjectiveConfig.GetExecutionAmount());
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
				craftingObjectiveEvent.SetTimeLimit(craftingObjectiveConfig.GetTimeLimit());
				craftingObjectiveEvent.SetExecutionAmount(craftingObjectiveConfig.GetExecutionAmount());
				m_QuestObjectives.Insert(craftingObjectiveEvent);
				return true;
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
				aiPatrolObjectiveEvent.SetTimeLimit(aiPatrolConfig.GetTimeLimit());
				
				ExpansionQuestAISpawn aiPatrolSpawn = aiPatrolConfig.GetAISpawn();
				if (!aiPatrolSpawn)
					return false;

				aiPatrolObjectiveEvent.SetKillAmount(aiPatrolSpawn.GetNumberOfAI());
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
				aiCampObjectiveEvent.SetTimeLimit(aiCampConfig.GetTimeLimit());
				
				int unitsCountC;
				array<ref ExpansionQuestAISpawn> aiCampSpawns = aiCampConfig.GetAISpawns();
				if (!aiCampSpawns || aiCampSpawns.Count() == 0)
					return false;

				foreach (ExpansionQuestAISpawn campSpawn: aiCampSpawns)
				{
					unitsCountC += campSpawn.GetNumberOfAI();
				}

				aiCampObjectiveEvent.SetKillAmount(unitsCountC);
				m_QuestObjectives.Insert(aiCampObjectiveEvent);
				return true;
			}
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig aiEscortConfig = m_QuestModule.GetAIEscortObjectiveConfigByID(objectiveID);
				if (!aiEscortConfig)
					return false;

				ExpansionQuestObjectiveAIEscortEvent aiEscortObjectiveEvent = new ExpansionQuestObjectiveAIEscortEvent(this);
				aiEscortObjectiveEvent.SetIndex(index);
				aiEscortObjectiveEvent.SetObjectiveConfig(aiEscortConfig);
				aiEscortObjectiveEvent.SetTimeLimit(aiEscortConfig.GetTimeLimit());
				m_QuestObjectives.Insert(aiEscortObjectiveEvent);
				return true;
			}
		#endif
		}

		return false;
	}

	ExpansionQuestConfig GetQuestConfig()
	{
		return m_Config;
	}

	void SetQuestState(ExpansionQuestState state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		m_QuestState = state;
		string stateName = typename.EnumToString(ExpansionQuestState, m_QuestState);
		QuestDebugPrint("State: " + stateName);
	}

	ExpansionQuestState GetQuestState()
	{
		return m_QuestState;
	}

	void SetPlayerUID(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestDebugPrint("Set UID: " + playerUID);

		m_PlayerUID = playerUID;
		m_Player = PlayerBase.GetPlayerByUID(playerUID);

		AddGroupMember(playerUID);
	}

	PlayerBase GetPlayer()
	{
		if (!m_Player)
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		return m_Player;
	}

	string GetPlayerUID()
	{
		return m_PlayerUID;
	}

	array<ItemBase> GetQuestItems()
	{
		return m_QuestItems;
	}

	bool IsCompleted()
	{
		return m_IsCompleted;
	}

	void SetIsCompleted(bool state)
	{
		m_IsCompleted = state;
	}

	//! Event called when a quest is started
	bool OnQuestStart()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!ValidateQuest())
		{
			Error(ToString() + "::OnQuestStart - Quest validation failed!");
			return false;
		}

		auto inhibitor = ExpansionInhibitor.Add(this);

		//! Start quest objectives
		ExpansionQuestObjectiveEventBase objective;
		if (m_Config.SequentialObjectives())
		{
			objective = m_QuestObjectives.Get(0);
			//! We only start the first objective as we will progress thrue all objective events in a sequential order.
			if (objective && objective.GetIndex() == 0)
			{
				if (!objective.OnStart(false))
					return false;
				
				m_CurrentObjectiveIndex = 0;
			}
		}
		else
		{
			for (int i = 0; i < m_QuestObjectives.Count(); i++)
			{
				objective = m_QuestObjectives.Get(i);
				if (!objective.OnStart(false))
					return false;
				
				m_CurrentObjectiveIndex = i;
			}
		}
		
		//! Set quest-giver item if we can find one in the players inventory
		FindAndSetQuestGiverItem();
		
		//! Add all quest items to the players inventory
		CreateQuestItems();

		SetInitialized(true);

		SetQuestState(ExpansionQuestState.STARTED);

		if (!m_Config.IsAchievement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestAcceptedText, m_Config.GetTitle()), ExpansionIcons.GetPath("Questionmark 2"), COLOR_EXPANSION_NOTIFICATION_INFO);

		delete inhibitor;

		UpdateQuest(true);

		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestStart(this);

		return true;
	}
	
	protected void FindAndSetQuestGiverItem()
	{
		m_Player = GetPlayer();
		if (!m_Player)
			return;
		
		int questID = m_Config.GetID();
		ItemBase handItem = ItemBase.Cast(m_Player.GetHumanInventory().GetEntityInHands());
		if (handItem && handItem.Expansion_GetQuestID() == questID && handItem.Expansion_IsQuestGiver())
		{
			m_QuestGiverItem = handItem;
			return;
		}
		
		array<EntityAI> items = new array<EntityAI>;
   		m_Player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;

			if (itemIB.Expansion_GetQuestID() == m_Config.GetID() && itemIB.Expansion_IsQuestGiver())
			{
				m_QuestGiverItem = itemIB;
				break;
			}
		}
	}

	protected void DeleteQuestGiverItem()
	{
		if (m_QuestGiverItem)
		{
			m_QuestGiverItem.Expansion_SetDeletedByQuest(true);
			GetGame().ObjectDelete(m_QuestGiverItem);
		}
	}

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

		if (m_Config.IsGroupQuest() && m_GroupID == -1)
		{
			Error(ToString() + "::OnQuestStart - Group ID not set!");
			return false;
		}

		return true;
	}

	//! Event called when all quest objectives are completed
	void OnQuestObjectivesComplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Can't get quest module!");
			return;
		}

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (!m_Player)
		{
			//! This is not an error. Player object will already be deleted on disconnect
			return;
		}

		if (!m_Config)
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest config!");
			return;
		}

		if (!m_Config.IsAutocomplete() && m_Config.GetQuestTurnInIDs().Count() > 0 && m_Config.GetQuestTurnInIDs()[0] != -1)
			CreateQuestTurnInMarker();

		if (m_QuestState >= ExpansionQuestState.CAN_TURNIN)
		{
			QuestDebugPrint("State is already on turn-in or completed!");
			return;
		}

		if (!m_Config.IsAchievement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCompletedText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_INFO);
		
		SetQuestState(ExpansionQuestState.CAN_TURNIN);

		if (m_Config.IsAutocomplete())
			m_QuestModule.RequestCompleteQuestServer(m_Config.GetID(), GetPlayerUID(), m_Player.GetIdentity(), true);
		
		//! Open quest menu so player can turn in the quest when the quest has no quest turn-in ID and cant be autocompleted.
		if (!m_Config.IsAutocomplete() && !m_Config.IsAchievement() && m_Config.GetQuestTurnInIDs().Count() == 0)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenuForQuest, 1000, false, m_Player.GetIdentity(), m_Config.GetID());
		
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestObjectivesComplete(this);
	}

	//! Create the turn-in marker if the quest is not autocompleted or has no turn-in quest NPC ID.
	protected void CreateQuestTurnInMarker()
	{
		QuestDebugPrint("Create turn-in marker!");
		//! Create a marker on the closest quest npc location for the player
		vector npcPos = GetClosestQuestNPCPosition(m_Config.GetQuestTurnInIDs(), m_Player.GetPosition());
		CreateQuestMarker(npcPos, "#STR_EXPANSION_QUEST_MARKER_TURNIN");
	}

	//! Event called when a quest objective state has changed to incomplete after it was completed once
	void OnQuestObjectivesIncomplete()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestObjectivesIncomplete - Can't get quest module!");
			return;
		}

		//! Remove the turn-in markers in case there is one.
		RemoveQuestMarkers();

		SetQuestState(ExpansionQuestState.STARTED);
		
		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestObjectivesIncomplete(this);
	}

	//! Event called when ever a quest is completed and turned-in
	bool OnQuestTurnIn(string playerUID, ExpansionQuestRewardConfig reward = null, int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (m_QuestState != ExpansionQuestState.CAN_TURNIN)
		{
			QuestDebugPrint("QUEST STATE INVALID FOR TURN-IN!");
			return false;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::OnQuestTurnIn - Can't get quest module!");
			return false;
		}

		//! Delete quest giver item from if one exists.
		DeleteQuestGiverItem();

		m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
		if (!m_Player || !m_Player.GetIdentity())
		{
			Error(ToString() + "::OnQuestTurnIn - Could not get quest player!");
			return false;
		}

		if (!m_Config && m_Config.GetID() == -1)
		{
			Error(ToString() + "::OnQuestTurnIn - Could not get quest config!");
			return false;
		}

		//! Call end event on all quest objectives
		foreach (ExpansionQuestObjectiveEventBase objectiveEvent: m_QuestObjectives)
		{
			if (!objectiveEvent.OnTurnIn(playerUID, selectedObjItemIndex))
				return false;
		}

		//! Add all quest rewards to the quest players
		bool conditions;
		if (m_Config.GetRewards().Count() > 0)
			conditions = true;
	#ifdef EXPANSIONMODAI
		if (!conditions && m_Config.GetFactionReward() != string.Empty)
			conditions = true;
	#endif
	#ifdef EXPANSIONMODHARDLINE
		if (!conditions && m_Config.GetReputationReward())
			conditions = true;
	#ifdef EXPANSIONMODAI
		if (!conditions && m_Config.GetFactionReputationRewards().Count() > 0)
			conditions = true;
	#endif
	#endif
		if (conditions)
			SpawnQuestRewards(playerUID, reward);

		if (!m_Config.IsAchievement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestTurnInText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS);
		else
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().AchievementCompletedTitle, m_Config.GetTitle()), new StringLocaliser(GetExpansionSettings().GetQuest().AchievementCompletedText, m_Config.GetObjectiveText()), ExpansionIcons.GetPath("Star"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);

		SetIsCompleted(true);
		SetQuestState(ExpansionQuestState.COMPLETED);

		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestCompletion(this);

		return true;
	}

	void CleanupObjectives(bool callObjectiveCleanup = false)
	{
		for (int i = m_QuestObjectives.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
			if (callObjectiveCleanup)
				objective.OnCleanup();
			m_QuestObjectives.RemoveOrdered(i);
		}
	}

	//! Event called when a quest is manualy canceled by the player
	bool OnQuestCancel()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		auto inhibitor = ExpansionInhibitor.Add(this);
		
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
		OnQuestCleanup(false, true);

		if (!m_Config.IsAchievement())
			SendNotification(new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledTitle), new StringLocaliser(GetExpansionSettings().GetQuest().QuestCanceledText, m_Config.GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);

		delete inhibitor;

		UpdateQuest(true);

		return true;
	}

	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	bool OnQuestContinue()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "ID: " + m_Config.GetID(), "Current objective index: " + m_CurrentObjectiveIndex);

		if (!ValidateQuest())
		{
			Error(ToString() + "::OnQuestContinue - Quest validation failed!");
			return false;
		}

		if (m_CurrentObjectiveIndex == -1)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "Continue quest objective failed! Invalid current objective index!");
			return false;
		}

		auto inhibitor = ExpansionInhibitor.Add(this);

		//! Continue active quest objectives
		for (int i = 0; i < m_QuestObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = m_QuestObjectives[i];
			//! We start all active objectives in range of the m_CurrentObjectiveIndex.
			if (objective && objective.GetIndex() <= m_CurrentObjectiveIndex)
			{
				if (!objective.IsInitialized() && objective.IsActive())
				{
					QuestDebugPrint("Continue quest objective. Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
					objective.QuestDebug();

					if (!objective.OnStart(true))
					{
						Error(ToString() + "::OnQuestContinue - Continue quest objective failed! Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
						return false;
					}
				}
			}
		}
		
		//! Set quest-giver item if we can find one in the players inventory
		FindAndSetQuestGiverItem();

		//! Check if quest players still have all the configured quest items.
		if (m_Config.GetQuestItems() && m_Config.GetQuestItems().Count() > 0 /*&& m_Config.NeedQuestItems()*/)
		{
			if (m_Config.NeedQuestItems() && !CheckQuestPlayersForQuestItems())
			{
				return false;
			}
			else if (!m_Config.NeedQuestItems())
			{
				CheckQuestPlayersForQuestItems();
			}
		}

		SetInitialized(true);

		delete inhibitor;
		
		//! If the quest can be automatically completed we send a request to complete the quest to the module if we can turn it in.
		if (m_Config.IsAutocomplete() && m_QuestState == ExpansionQuestState.CAN_TURNIN)
			m_QuestModule.RequestCompleteQuestServer(m_Config.GetID(), GetPlayerUID(), m_Player.GetIdentity(), true);
		
		//! If the quest cant be automatically completed but has no turn-in NPC IDs then we try to open the quest menu so the player can turn-in the quest.
		if (!m_Config.IsAutocomplete() && !m_Config.IsAchievement() && m_QuestState == ExpansionQuestState.CAN_TURNIN && m_Config.GetQuestTurnInIDs().Count() == 0)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenuForQuest, 1000, false, m_Player.GetIdentity(), m_Config.GetID());

		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnQuestContinue(this);

		return true;
	}
	
	protected bool CheckQuestPlayersForQuestItems()
	{
		PlayerBase player;
		array<string> questItemsNames = new array<string>;
		map<string, int> questItemsMap = new map<string, int>;
		map<string, int> questItemsInventoryMap;
		array<ItemBase> playerItems = new array<ItemBase>;

		array<ref ExpansionQuestItemConfig> questItems = m_Config.GetQuestItems();
		foreach (ExpansionQuestItemConfig itemConfig: questItems)
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

		if (!m_Config.IsGroupQuest())
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
				QuestDebugPrint("Overall needed items: " + overallNeeded);
				QuestDebugPrint("Overall inventory items: " + overallCount);
				return false;
			}
		}
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
					QuestDebugPrint("Overall needed items: " + overallNeeded);
					QuestDebugPrint("Overall inventory items: " + overallNeeded);
					return false;
				}
			}
		}
		
		return true;
	}

	void CleanupQuestItems()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		for (int i = m_QuestItems.Count() - 1; i >= 0; i--)
		{
			ItemBase item = m_QuestItems[i];
			GetGame().ObjectDelete(item);
			m_QuestItems.RemoveOrdered(i);
		}
		
		if (!m_Config.IsGroupQuest())
		{
			CleanupAllItemsWithQuestID(m_Player);
		}
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				CleanupAllItemsWithQuestID(groupPlayer);
			}
		}
	}
	
	protected void CleanupAllItemsWithQuestID(PlayerBase player)
	{
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());
	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item : items)
		{
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;
			
			if (!itemIB.Expansion_IsQuestGiver() && !itemIB.Expansion_IsObjectiveLoot() && itemIB.Expansion_GetQuestID() == m_Config.GetID())
				GetGame().ObjectDelete(item);
		}
	}

	void SetQuestItemsToNormalItems()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		for (int i = m_QuestItems.Count() - 1; i >= 0; i--)
		{
			ItemBase item = m_QuestItems[i];
			if (!item)
				continue;

			item.Expansion_SetQuestID(-1);
			m_QuestItems.RemoveOrdered(i);
		}
	}

	void CreateQuestItems()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		array<ref ExpansionQuestItemConfig> questItemConfigs = m_Config.GetQuestItems();
		if (!m_Config.IsGroupQuest() && m_Player)
		{
			//! Add all quest items to the players inventory
			foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
			{
				SpawnQuestItem(questItem, m_Player, m_Player, m_Player.GetPosition(), m_Player.GetOrientation());
			}
		}
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				QuestDebugPrint("Spawn quest items for player with UID: " + memberUID);
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				foreach (ExpansionQuestItemConfig questItemMember: questItemConfigs)
				{
					SpawnQuestItem(questItemMember, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
				}
			}
		}
	}

	//! Event called for group quests only when a group member joins/rejoins the quest group
	void OnGroupMemberJoined(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (m_Config.IsGroupQuest() && m_GroupID > -1)
		{
			ExpansionQuestPersistentData groupQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(m_GroupID.ToString());
			if (!groupQuestData)
			{
				Error(ToString() + "::OnGroupMemberJoined - Could not get group quest data with ID: " + m_GroupID);
				return;
			}
			
			ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				Error(ToString() + "::OnGroupMemberJoined - Could not get player quest data for player with UID: " + playerUID);
				return;
			}

			//! Make sure player has the correct quest state for this quest in his quest data.
			if (!playerQuestData.QuestData.Contains(m_Config.GetID()))
				playerQuestData.AddQuestData(m_Config.GetID(), m_QuestState);

			if (m_Config.GetQuestItems().Count() > 0)
			{
				QuestDebugPrint("New group member joined. Check if he has already all quest items..");

				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
				if (!groupPlayer)
				{
					Error(ToString() + "::OnGroupMemberJoined - Could not get player with UID: " + playerUID);
					return;
				}

				array<ItemBase> playerQuestItems = GetPlayerQuestItems(groupPlayer, m_Config.GetID(), null);
				if (!playerQuestItems || playerQuestItems.Count() == 0)
				{
					QuestDebugPrint("Player has no quest items. Add items");
					array<ref ExpansionQuestItemConfig> questItemConfigs = m_Config.GetQuestItems();
					foreach (ExpansionQuestItemConfig questItem: questItemConfigs)
					{
						SpawnQuestItem(questItem, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
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

			AddGroupMember(playerUID);

			m_QuestModule.UpdateQuestData(this, playerQuestData, true);
			m_QuestModule.SaveAndSyncQuestData(playerQuestData, playerUID, m_Config.GetID());
		}
	}
	
	void AddGroupMember(string playerUID)
	{
		m_PlayerUIDs.Insert(playerUID);
	}

	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (m_Config.IsGroupQuest() && m_GroupID > -1)
		{
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerUID);
			if (!groupPlayer)
				return;
			
			ExpansionQuestPersistentData groupQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(m_GroupID.ToString());
			if (!groupQuestData)
			{
				Error(ToString() + "::OnGroupMemberLeave - Could not get group quest data with ID: " + m_GroupID);
				return;
			}

			ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestDataByUID(playerUID);
			if (!playerQuestData)
			{
				Error(ToString() + "::OnGroupMemberLeave - Could not get player quest data for player with UID: " + playerUID);
				return;
			}
			
			if (m_Config.GetQuestItems().Count() > 0)
			{
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

			if (!GroupExists(m_GroupID))
			{
				//! Group was deleted
				CancelQuest();
			}
			else
			{
				//! Quest still active for group, only remove for player
				m_QuestModule.AbandonQuest(playerQuestData, this, groupPlayer.GetIdentity());
			}

			m_PlayerUIDs.RemoveItem(playerUID);
		}
	}

	//! 3rd party modding support helper function
	bool GroupExists(int groupID)
	{
	#ifdef EXPANSIONMODGROUPS
		auto group = ExpansionPartyModule.s_Instance.GetPartyByID(m_GroupID);
		if (group && group.GetPlayers().Count() > 0)
			return true;
	#endif
		return false;
	}

	//! Players (online)
	set<string> GetPlayerUIDs()
	{
		return m_PlayerUIDs;
	}

	//! Event called when quest instance is destroyed/cleaned-up
	bool OnQuestCleanup(bool callObjectiveCleanup = false, bool canceledQuest = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(m_Config.GetID());

		if ((canceledQuest || m_Config.DeleteQuestItems()) && (m_QuestState == ExpansionQuestState.NONE || m_QuestState == ExpansionQuestState.COMPLETED))
		{
			CleanupQuestItems();
		}
		else if (!m_Config.DeleteQuestItems() && m_QuestState == ExpansionQuestState.COMPLETED)
		{
			SetQuestItemsToNormalItems();
		}

		//! Call cleanup event on all active quest objectives
		CleanupObjectives(callObjectiveCleanup);

		RemoveQuestMarkers(true);

		SetInitialized(false);

		return true;
	}

	//! Main quest objective completion check
	void QuestCompletionCheck(bool forceUpdateQuest = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int completedObjectives;	//! Amount of completed quest objectives.
		int objectivesCount = m_QuestObjectives.Count();	//! Amount of all quest objectives.

		if (!m_QuestObjectives || m_QuestObjectives.Count() == 0)
		{
			QuestDebugPrint("Quest has no configured objectives. Cancel quest!");
			CancelQuest();
			return;
		}

		//! Collect the amount of completed objectives and fire there OnComplete or OnIncomplete events depending on the given conditions.
		for (int i = 0; i < objectivesCount; ++i)
		{
			ExpansionQuestObjectiveEventBase currentObjective = m_QuestObjectives[i];

			if (currentObjective.IsActive())
			{
				//! If the objective can be completed then call the objectives OnComplete event.
				//! If the objective can no longer be completed then call the objectives OnIncomplete event.
				if (currentObjective.CanComplete())
				{
					currentObjective.OnComplete();
				}
				else
				{
					currentObjective.OnIncomplete();
				}
			}

			//! If the ojectective is now still completed then count it as completed ojective.
			if (currentObjective.IsCompleted())
			{
				completedObjectives++;
				if (i < m_QuestObjectives.Count())
				{
					ExpansionQuestObjectiveEventBase nextObjective = m_QuestObjectives[i + 1];
					if (nextObjective && !nextObjective.IsInitialized() && !nextObjective.IsActive())
					{
						if (!nextObjective.OnStart(false))
						{
							QuestDebugPrint("Could not start next quest objective instance! Cancel quest..");
							CancelQuest();
							return;
						}
					}
				}
			}
		}

		QuestDebugPrint("Total objective count: " + objectivesCount);
		QuestDebugPrint("Total completed objectives: " + completedObjectives);

		//! If all quest objectives are completed then fire the OnQuestObjectivesComplete event.
		//! If not all quest objectives are completed but the quest state is on CAN_TURNIN then the objective state has changed and we fire the OnQuestObjectivesIncomplete event.
		bool updateQuestState;
		if (completedObjectives == objectivesCount)
		{
			QuestDebugPrint("All quest objectives completed!");
			OnQuestObjectivesComplete();
			updateQuestState = true;
		}
		else
		{
			QuestDebugPrint("INCOMPLETE");
			OnQuestObjectivesIncomplete();
			QuestDebugPrint("Current active objective index: " + m_CurrentObjectiveIndex);
			updateQuestState = true;
		}

		if (m_CompletedObjectivesCount != completedObjectives)
		{
			m_CompletedObjectivesCount = completedObjectives;
			UpdateQuest(updateQuestState);
		}
		else if (forceUpdateQuest)
		{
			UpdateQuest(updateQuestState);
		}
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

	protected void SpawnQuestItem(ExpansionQuestItemConfig questItem, PlayerBase player, EntityAI parent, vector pos, vector ori)
	{
		int remainingAmount = questItem.GetAmount();
		while (remainingAmount > 0)
		{
			int remainingAmountBefore = remainingAmount;
			Object obj = ExpansionItemSpawnHelper.SpawnOnParent(questItem.GetClassName(), player, parent, remainingAmount, 10);
			if (!obj)
			{
				Error("Error: Couldn't spawn " + questItem.GetClassName());
				break;
			}

			ItemBase item;
			if (!Class.CastTo(item, obj))
			{
				GetGame().ObjectDelete(obj);
				Error("Error: Couldn't spawn " + questItem.GetClassName());
				break;
			}

			item.Expansion_SetQuestID(m_Config.GetID());
			
			Magazine mag;
			if (Class.CastTo(mag, item))
			{
				if (!mag.IsAmmoPile())
					mag.ServerSetAmmoCount(0);
			}
			
			m_QuestItems.Insert(item);

			if (remainingAmount == remainingAmountBefore)
			{
				//! Should not be possible, just in case...
				Error("Error: Spawning " + questItem.GetClassName() + " did not affect remaining amount!");
				break;
			}
		}
	}

	protected Object Spawn(ExpansionQuestRewardConfig item, PlayerBase player, EntityAI parent, vector position, vector orientation, out int remainingAmount)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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
		int questID = item.GetQuestID();
		ItemBase itemIB;
		if (Class.CastTo(itemIB, obj))
		{
			if (dmg > 0)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "Set object damage %: " + dmg);
				ProcessHealth01(itemIB, 1 - dmg / 100);
			}
			
			if (questID != -1)
			{
				itemIB.Expansion_SetIsQuestGiver(true);
				itemIB.Expansion_SetQuestID(questID);
			}
		}

		return obj;
	}

	protected void ProcessHealth01(EntityAI parent, float health01)
	{
		parent.SetHealth("", "Health", parent.GetMaxHealth("", "Health") * health01);

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(parent.GetInventory().CountInventory());

		parent.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			if (item.IsInherited(SurvivorBase))
				continue;

			item.SetHealth("", "Health", item.GetMaxHealth("", "Health") * health01);
		}
	}

	array<ref ExpansionQuestObjectiveEventBase> GetObjectives()
	{
		return m_QuestObjectives;
	}
	
	ExpansionQuestObjectiveEventBase GetObjective(int objectiveType, int objectiveID)
	{
		ExpansionQuestObjectiveEventBase foundObj;
		foreach (ExpansionQuestObjectiveEventBase obj: m_QuestObjectives)
		{
			if (obj.GetObjectiveConfig().GetObjectiveType() == objectiveType && obj.GetObjectiveConfig().GetID() == objectiveID)
			{
				foundObj = obj;
				break;
			}
		}
		
		return foundObj;
	}

	bool IsInitialized()
	{
		return m_Initialized;
	}

	void SetInitialized(bool state)
	{
		m_Initialized = state;
	}

	ExpansionQuestModule GetQuestModule()
	{
		return m_QuestModule;
	}

	bool CanCompleteQuest()
	{
		int completedObjectives = 0;
		foreach (ExpansionQuestObjectiveEventBase objective: m_QuestObjectives)
		{
			if (objective.IsCompleted())
				completedObjectives++;
		}

		if (completedObjectives == m_QuestObjectives.Count())
			return true;

		return false;
	}

	void SetGroupID(int groupID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_GroupID = groupID;

		QuestDebugPrint("Group ID: " + m_GroupID);
	}

	int GetGroupID()
	{
		return m_GroupID;
	}

	bool IsOtherGroupMemberOnline()
	{
		return m_PlayerUIDs.Count() > 1;
	}

	//! TODO: *Terrible*, everything duplicated from non-group quest handling. Please refactor at some point @Steve_aka_Salutesh
	void SpawnQuestRewards(string playerUID, ExpansionQuestRewardConfig reward = null)
	{
		array<ref ExpansionQuestRewardConfig> questRewards = m_Config.GetRewards();
		array<float> chances;
		int index = -1;
		int lootItemsSpawned = 0;
		int itemCount = 0;
		
		//! Collect reward chances from config if reward is random
		if (m_Config.RandomReward() && questRewards.Count() > 1)
		{
			itemCount = m_Config.GetRandomRewardAmount();
			chances = new array<float>;
			foreach (ExpansionQuestRewardConfig rewardConfig: questRewards)
			{
				chances.Insert(rewardConfig.GetChance());
			}
		}
		
		if (!m_Config.IsGroupQuest())
		{
			PlayerBase questPlayer = PlayerBase.GetPlayerByUID(playerUID);
			if (m_Config.NeedToSelectReward() && reward)
			{
				QuestDebugPrint("Spawn selected reward: " + reward.ToString());
				SpawnReward(reward, questPlayer, questPlayer, questPlayer.GetPosition(), questPlayer.GetOrientation());
			}
			else if (m_Config.RandomReward() && questRewards.Count() > 1)
			{
				if (m_Config.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_COMPLETION)
				{
					ExpansionQuestRewardConfig radomReward;
					
					if (itemCount > 0)
					{
						while (lootItemsSpawned < itemCount)
						{
							index = ExpansionStatic.GetWeightedRandom(chances);
							if (index > -1)
							{
								radomReward = questRewards[index];
								SpawnReward(radomReward, questPlayer, questPlayer, questPlayer.GetPosition(), questPlayer.GetOrientation());
								lootItemsSpawned++;
							}
							else
							{
								break;
							}
						}
					}
					else
					{
						index = ExpansionStatic.GetWeightedRandom(chances);
						if (index > -1)
						{
							radomReward = questRewards[index];
							SpawnReward(radomReward, questPlayer, questPlayer, questPlayer.GetPosition(), questPlayer.GetOrientation());
						}
					}
				}
				else if (m_Config.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_START)
				{
					foreach (ExpansionQuestRewardConfig randomReward: questRewards)
					{
						SpawnReward(randomReward, questPlayer, questPlayer, questPlayer.GetPosition(), questPlayer.GetOrientation());
					}
				}
			}
			else
			{
				if (questRewards && questRewards.Count() > 0)
				{					
					foreach (ExpansionQuestRewardConfig questReward: questRewards)
					{
						SpawnReward(questReward, questPlayer, questPlayer, questPlayer.GetPosition(), questPlayer.GetOrientation());
					}
				}
			}

		#ifdef EXPANSIONMODAI
			QuestDebugPrint("Faction reward: " + m_Config.GetFactionReward());
			if (m_Config.GetFactionReward() != string.Empty)
				ApplyPlayerFaction(m_Config.GetFactionReward(), questPlayer);
		#endif
		#ifdef EXPANSIONMODHARDLINE
			if (GetExpansionSettings().GetHardline().UseReputation && m_Config.GetReputationReward() != 0)
			{
				if (questPlayer)
					questPlayer.Expansion_AddReputation(m_Config.GetReputationReward());
			}
			
		#ifdef EXPANSIONMODAI
			map<string, int> factionRewards = m_Config.GetFactionReputationRewards();
			QuestDebugPrint("Faction reputation rewards: " + factionRewards.Count());
			if (GetExpansionSettings().GetHardline().UseFactionReputation && factionRewards.Count() > 0)
			{
				int playerFactionID = questPlayer.eAI_GetFactionTypeID();
				foreach (string faction, int reputation: factionRewards)
				{
					typename factionType = eAIFaction.GetType(faction);
					int factionID = eAIRegisterFaction.s_FactionIDs[factionType];
					if (factionID != 0)
					{
						QuestDebugPrint("Add faction reputation reward: " + factionType.ToString() + ":" + factionID + " | Reputation: " + reputation);
						if (factionID == playerFactionID)
						{
							questPlayer.Expansion_AddReputation(reputation);
						}
						else
						{
							questPlayer.Expansion_AddFactionReputation(reputation, factionID);
						}
					}
				}
			}
		#endif
		#endif
		}
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
				//! TODO: *Terrible*, everything duplicated from non-group quest handling. Please refactor at some point @Steve_aka_Salutesh

				if (m_Config.RewardsForGroupOwnerOnly())
				{
					QuestDebugPrint("Quest rewards for quest " + m_Config.GetID() + " are for the quest owner only.");
					if (memberUID != m_PlayerUID)
					{
						QuestDebugPrint("Player [UID: " + memberUID + "] is not the group owner. Skip!");
						continue;
					}
				}

				QuestDebugPrint("Try to spawn quest reward for player with UID: " + memberUID);
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
				{
					QuestDebugPrint("Could not get player with UID: " + memberUID + ". Not online?!");
					continue;
				}

				if (m_Config.NeedToSelectReward() && reward)
				{
					QuestDebugPrint("Spawn selected reward: " + reward.ToString());
					SpawnReward(reward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), m_Player.GetOrientation());
				}
				else if (m_Config.RandomReward() && questRewards.Count() > 1)
				{
					if (m_Config.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_COMPLETION)
					{
						ExpansionQuestRewardConfig groupPlayerReward;
						
						if (itemCount > 0)
						{
							while (lootItemsSpawned < itemCount)
							{
								index = ExpansionStatic.GetWeightedRandom(chances);
								if (index > -1)
								{
									groupPlayerReward = questRewards[index];
									SpawnReward(groupPlayerReward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
									lootItemsSpawned++;
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							index = ExpansionStatic.GetWeightedRandom(chances);
							if (index > -1)
							{
								groupPlayerReward = questRewards[index];
								SpawnReward(groupPlayerReward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
							}
						}
					}
					else if (m_Config.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_START)
					{
						foreach (ExpansionQuestRewardConfig randomGroupReward: questRewards)
						{
							SpawnReward(randomGroupReward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
						}
					}
				}
				else
				{
					 if (questRewards && questRewards.Count() > 0)
					{
						//! Add all quest rewards to the players inventory
						foreach (ExpansionQuestRewardConfig groupQuestReward: questRewards)
						{
							SpawnReward(groupQuestReward, groupPlayer, groupPlayer, groupPlayer.GetPosition(), groupPlayer.GetOrientation());
						}
					}
				}

			#ifdef EXPANSIONMODAI
				QuestDebugPrint("Faction reward: " + m_Config.GetFactionReward());
				if (m_Config.GetFactionReward() != string.Empty)
				{
					ApplyPlayerFaction(m_Config.GetFactionReward(), groupPlayer);
				}
			#endif
			#ifdef EXPANSIONMODHARDLINE
				if (GetExpansionSettings().GetHardline().UseReputation && m_Config.GetReputationReward() != 0)
				{
					if (groupPlayer)
						groupPlayer.Expansion_AddReputation(m_Config.GetReputationReward());
				}
				
			#ifdef EXPANSIONMODAI
				map<string, int> groupFactionRewards = m_Config.GetFactionReputationRewards();
				if (GetExpansionSettings().GetHardline().UseFactionReputation && groupFactionRewards.Count() > 0)
				{
					int groupPlayerFactionID = groupPlayer.eAI_GetFactionTypeID();
					foreach (string groupFaction, int groupRep: groupFactionRewards)
					{
						typename groupFactionType = eAIFaction.GetType(groupFaction);
						int groupFactionID = eAIRegisterFaction.s_FactionIDs[groupFactionType];
						if (groupFactionID != 0)
						{
							if (groupFactionID == groupPlayerFactionID)
							{
								groupPlayer.Expansion_AddReputation(groupRep);
							}
							else
							{
								groupPlayer.Expansion_AddFactionReputation(groupRep, groupFactionID);
							}
						}
					}
				}
			#endif
			#endif
			}
		}
	}

	int GetObjectivesCount()
	{
		return m_QuestObjectives.Count();
	}

	void SendNotification(StringLocaliser title, StringLocaliser text, string icon, int color)
	{
		if (!m_Config.IsGroupQuest())
		{
			m_Player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (!m_Player)
				return;

			ExpansionNotification(title, text, icon, color, 7, ExpansionNotificationType.TOAST).Create(m_Player.GetIdentity());
		}
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
	}

	bool IsQuestPlayer(string uid)
	{
		if (m_PlayerUID == uid)
		{
			return true;
		}
		else if (m_Config.IsGroupQuest())
		{
			return m_PlayerUIDs.Find(uid) > -1;
		}

		return false;
	}

	//! @note DO NOT use this to create objective markers! Use ExpansionQuestObjectivEEventBase::CreateObjectiveMarker
	private void CreateQuestMarker(vector pos, string name)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_Config.IsGroupQuest())
		{
			if (!m_Player)
				return;

			m_QuestModule.CreateClientMarker(pos, name, m_Config.GetID(), m_Player.GetIdentity(), -1);
		}
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				m_QuestModule.CreateClientMarker(pos, name, m_Config.GetID(), groupPlayer.GetIdentity(), -1);
			}
		}
	}

	/**
	 * @brief Remove quest markers
	 * 
	 * @param removeObjectiveMarkers If false, remove only turn-in marker, if true, also remove objective markers
	 */
	private void RemoveQuestMarkers(bool removeObjectiveMarkers = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int objectiveIndex = -1;
		if (removeObjectiveMarkers)
			objectiveIndex = -2;

		if (!m_Config.IsGroupQuest())
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(m_PlayerUID);
			if (!player)
				return;

			m_QuestModule.RemoveClientMarkers(m_Config.GetID(), player.GetIdentity(), objectiveIndex);
		}
		else
		{
			foreach (string memberUID: m_PlayerUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				m_QuestModule.RemoveClientMarkers(m_Config.GetID(), groupPlayer.GetIdentity(), objectiveIndex);
			}
		}
	}

	bool ObjectivesCreated()
	{
		return m_ObjectivesCreated;
	}

	int CurrentObjectiveIndex()
	{
		return m_CurrentObjectiveIndex;
	}

	void SetCurrentObjectiveIndex(int index)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_CurrentObjectiveIndex = index;

		QuestDebugPrint("Set current objective index: " + m_CurrentObjectiveIndex);
	}

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

	array<ItemBase> GetPlayerQuestItems(PlayerBase player, int questID, array<string> questItemsNames, inout map<string, int> questItemsInventoryMap = null)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		QuestDebugPrint("Check player for quest items. UID: " + player.GetIdentity().GetId() + " | Quest ID: " + questID);

		array<ItemBase> questItems = new array<ItemBase>;
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());

	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item : items)
		{
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;

			string typeName = itemIB.GetType();
			if (questItemsNames.Find(typeName) == -1)
			{
				QuestDebugPrint("Item " + typeName + " is not in the quest items configuration. Skip..");
				continue;
			}

			QuestDebugPrint("Item Quest ID: " + itemIB.Expansion_GetQuestID());

			if (itemIB.Expansion_GetQuestID() != questID || itemIB.Expansion_IsQuestGiver() || itemIB.Expansion_IsObjectiveLoot())
			{
				QuestDebugPrint("Item " + typeName + " is not a quest item for this quest. Skip..");
				continue;
			}

			int amount = itemIB.Expansion_GetStackAmount();
			int current;

			if (questItemsInventoryMap)
			{
				if (!questItemsInventoryMap.Find(typeName, current))
				{
					QuestDebugPrint("Add item " + typeName + " to inventory item map. Amount: " + amount);
					questItemsInventoryMap.Insert(typeName, amount);
				}
				else
				{
					int newAmount = current + amount;
					QuestDebugPrint("Set item " + typeName + " in inventory item map. Amount: " + newAmount);
					questItemsInventoryMap.Set(typeName, newAmount);
				}
			}

			QuestDebugPrint("Add quest item: " + itemIB.GetType() + " | Quest ID: " + itemIB.Expansion_GetQuestID());
			questItems.Insert(itemIB);
		}

		QuestDebugPrint("End");

		return questItems;
	}

	void UpdateQuest(bool updateQuestState)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "ID: " + m_Config.GetID(), "Update state: " + updateQuestState);

		if (ExpansionInhibitor.Contains(this))
		{
			QuestDebugPrint("SKIPPING UpdateQuest");
			return;
		}

		if (!m_QuestModule)
		{
			Error(ToString() + "::UpdateQuest - Could not get quest module!");
			return;
		}

		//! Set updated quest state and objective data on all quest players and send it to them.
		m_QuestModule.UpdateQuestForQuestPlayers(this, updateQuestState);
	}

	void CancelQuest()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		if (!m_QuestModule)
		{
			Error(ToString() + "::CancelQuest - Could not get quest module!");
			return;
		}
		
		m_QuestModule.CancelQuestServer(this);
	}

#ifdef EXPANSIONMODAI
	void ApplyPlayerFaction(string factionName, PlayerBase player)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		eAIGroup playerGroup = player.GetGroup();
		eAIFaction newFaction;
		eAIFaction currentFaction;
		if (playerGroup)
		{
			currentFaction = playerGroup.GetFaction();
			if (currentFaction.GetName() != factionName)
			{
				newFaction = eAIFaction.Create(factionName);
				playerGroup.SetFaction(newFaction);
			}
		}
		else
		{
			newFaction = eAIFaction.Create(factionName);
			eAIGroup.GetGroupByLeader(player, true, newFaction);
		}

		//! Update the players quest configuration files as there could be quests he now need to recieve or need to be removed after a faction change.
		ExpansionQuestModule.GetModuleInstance().SendClientQuestConfigs(player.GetIdentity());
	}
#endif
	
	ItemBase GetQuestGiverItem()
	{
		return m_QuestGiverItem;
	}

	void QuestDebugPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		EXTrace.Print(EXTrace.QUESTS, null, text);
	#endif
	}
};
