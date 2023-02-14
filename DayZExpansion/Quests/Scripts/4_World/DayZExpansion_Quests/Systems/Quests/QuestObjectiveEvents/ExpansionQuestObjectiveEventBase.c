/**
 * ExpansionQuestObjectiveEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Instance and methods are handled on server side only!
class ExpansionQuestObjectiveEventBase
{
	protected int m_Index = -1;
	protected ExpansionQuest m_Quest;
	protected bool m_Completed;
	protected bool m_Initialized;
	protected bool m_Active;
	protected ref ExpansionQuestObjectiveConfig m_ObjectiveConfig;
	protected int m_TimeLimit = -1;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase Constructor
	// -----------------------------------------------------------
	void ExpansionQuestObjectiveEventBase(ExpansionQuest quest)
	{
		m_Quest = quest;
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase Deconstructor
	// -----------------------------------------------------------
	void ~ExpansionQuestObjectiveEventBase()
	{
		DeassignObjectiveOnClasses();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetIndex
	// -----------------------------------------------------------
	void SetIndex(int index)
	{
		m_Index = index;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase GetIndex
	// -----------------------------------------------------------
	int GetIndex()
	{
		return m_Index;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase GetQuest
	// -----------------------------------------------------------
	ExpansionQuest GetQuest()
	{
		return m_Quest;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase IsInitialized
	// -----------------------------------------------------------
	bool IsInitialized()
	{
		return m_Initialized;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase IsActive
	// -----------------------------------------------------------
	bool IsActive()
	{
		return m_Active;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetIsActive
	// -----------------------------------------------------------
	void SetIsActive(bool state)
	{
		m_Active = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase IsCompleted
	// -----------------------------------------------------------
	bool IsCompleted()
	{
		return m_Completed;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetCompleted
	// -----------------------------------------------------------
	void SetCompleted(bool state)
	{
		m_Completed = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetInitialized
	// -----------------------------------------------------------
	void SetInitialized(bool state)
	{
		m_Initialized = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetObjectiveConfig
	// -----------------------------------------------------------
	void SetObjectiveConfig(ExpansionQuestObjectiveConfig config)
	{
		m_ObjectiveConfig = config;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SetTimeLimit
	// -----------------------------------------------------------
	void SetTimeLimit(int time)
	{
		m_TimeLimit = time;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase GetTimeLimit
	// -----------------------------------------------------------
	int GetTimeLimit()
	{
		return m_TimeLimit;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnTimeLimitReached
	// -----------------------------------------------------------
	//! Event called when objective time-limit is reached.
	void OnTimeLimitReached()
	{
		m_Quest.CancelQuest();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase SyncTimeLimitTime
	// -----------------------------------------------------------
	//! Event called when objective has a time-limit to update the current remainig time in the persistent quest data of the quest players.
	void SyncTimeLimitTime()
	{
		m_TimeLimit = GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).GetRemainingTime(OnTimeLimitReached) / 1000;
		m_Quest.UpdateQuest();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnStart
	// -----------------------------------------------------------
	//! Event called when the player starts or continues the quest.
	bool OnStart(bool continues)
	{
		SetInitialized(true);
		SetIsActive(true);

		if (m_TimeLimit > -1)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnTimeLimitReached, m_TimeLimit * 1000);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SyncTimeLimitTime, 10 * 1000, true);
		}

		AssignObjectiveOnClasses();

		if (!continues)
		{
			if (!OnEventStart())
				return false;
		}
		else
		{
			if (!OnContinue())
				return false;
		}

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase AssignObjectiveOnClasses
	// -----------------------------------------------------------
	//! Assign objective to allocated classes.
	//! ToDo: Dokumentation
	void AssignObjectiveOnClasses()
	{
		ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Start");

		array<string> allocationClassNames = m_ObjectiveConfig.GetAllocationClasseNames();
		if (!allocationClassNames || allocationClassNames.Count() == 0)
		{
			ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - No classes to allocate! Skip..");
			return;
		}

		foreach (string className: allocationClassNames)
		{
			switch (className)
			{
				case "ActionBase":
				{
					ActionBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to ActionBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;

				case "ZombieBase":
				{
					ZombieBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to ZombieBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;

				case "AnimalBase":
				{
					AnimalBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to AnimalBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;

				case "PlayerBase":
				{
					PlayerBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to PlayerBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;

				case "ItemBase":
				{
					ItemBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to ItemBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;

				case "RecipeBase":
				{
					RecipeBase.AssignQuestObjective(this);
					ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - Assign to RecipeBase. Type: " + GetObjectiveType() + " | ID: " + m_ObjectiveConfig.GetID());
				}
				break;
			}
		}

		ObjectivePrint(ToString() + "::AssignObjectiveOnClasses - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase DeassignObjectiveOnClasses
	// -----------------------------------------------------------
	//! Deassign objective from allocated classes.
	//! ToDo: Dokumentation
	void DeassignObjectiveOnClasses()
	{
		ObjectivePrint(ToString() + "::DeassignObjectiveOnClasses - Start");

		array<string> allocationClassNames = m_ObjectiveConfig.GetAllocationClasseNames();
		if (!allocationClassNames || allocationClassNames.Count() == 0)
			return;

		foreach (string className: allocationClassNames)
		{
			switch (className)
			{
				case "ActionBase":
				{
					ActionBase.DeassignQuestObjective(this);
				}
				break;

				case "ZombieBase":
				{
					ZombieBase.DeassignQuestObjective(this);
				}
				break;

				case "AnimalBase":
				{
					AnimalBase.DeassignQuestObjective(this);
				}
				break;

				case "PlayerBase":
				{
					PlayerBase.DeassignQuestObjective(this);
				}
				break;

				case "ItemBase":
				{
					ItemBase.DeassignQuestObjective(this);
				}
				break;
				
				case "RecipeBase":
				{
					RecipeBase.DeassignQuestObjective(this);
				}
				break;
			}
		}

		ObjectivePrint(ToString() + "::DeassignObjectiveOnClasses - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnEventStart
	// -----------------------------------------------------------
	//! Event called when the player starts the quest.
	bool OnEventStart()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnContinue
	// -----------------------------------------------------------
	//! Event called when the player continues the quest after a server restart/reconnect.
	bool OnContinue()
	{
		SetInitialized(true);
		SetIsActive(true);

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnTurnIn
	// -----------------------------------------------------------
	//! Event called when quest is completed and turned-in.
	bool OnTurnIn(string playerUID)
	{
		SetInitialized(false);
		SetIsActive(false);

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase CanComplete
	// -----------------------------------------------------------
	//! Control condition method that controlls if the objective can be completed or not.
	//! Used in ExpansionQuest::QuestCompletionCheck.
	bool CanComplete()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnComplete
	// -----------------------------------------------------------
	//! Event called when objective is completed
	bool OnComplete()
	{
		SetCompleted(true);

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnIncomplete
	// -----------------------------------------------------------
	bool OnIncomplete()
	{
		SetCompleted(false);

	/*#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif*/

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnCancel
	// -----------------------------------------------------------
	bool OnCancel()
	{
		SetInitialized(false);
		SetIsActive(false);

		if (!OnCleanup())
			return false;
		
		DeassignObjectiveOnClasses();

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnCleanup
	// -----------------------------------------------------------
	//! Event called when the quest gets cleaned up.
	bool OnCleanup()
	{
		SetInitialized(false);

		if (m_TimeLimit > -1)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnTimeLimitReached);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SyncTimeLimitTime);
			m_TimeLimit = -1;
		}

		DeassignObjectiveOnClasses();

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnRecreateClientMarkers
	// -----------------------------------------------------------
	//! Event called when the quest markers should get recreated
	void OnRecreateClientMarkers();

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnGroupMemberJoined
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member joins/rejoins the quest group
#ifdef EXPANSIONMODGROUPS
	void OnGroupMemberJoined(string playerUID)
	{
	#ifdef EXPANSIONMODNAVIGATION
		OnRecreateClientMarkers();
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase OnGroupMemberLeave
	// -----------------------------------------------------------
	//! Event called for group quests only when a group member leaves the quest group
	void OnGroupMemberLeave(string playerUID);
#endif

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase GetObjectiveConfig
	// -----------------------------------------------------------
	ExpansionQuestObjectiveConfig GetObjectiveConfig()
	{
		return m_ObjectiveConfig;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase HasDynamicState
	// -----------------------------------------------------------
	bool HasDynamicState()
	{
		return false;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase QuestDebug
	// -----------------------------------------------------------
	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		ObjectivePrint("------------------------------------------------------------");
		ObjectivePrint(ToString() + "::QuestDebug - Objective index: " + m_Index);
		ObjectivePrint(ToString() + "::QuestDebug - Objective quest: " + m_Quest);
		ObjectivePrint(ToString() + "::QuestDebug - Objective completed: " + m_Completed);
		ObjectivePrint(ToString() + "::QuestDebug - Objective Initialized: " + m_Initialized);
		ObjectivePrint(ToString() + "::QuestDebug - Objective active: " + m_Active);
		ObjectivePrint(ToString() + "::QuestDebug - Objective config: " + m_ObjectiveConfig);
		ObjectivePrint(ToString() + "::QuestDebug - Objective time limit: " + m_TimeLimit);
		ObjectivePrint("------------------------------------------------------------");
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase ObjectivePrint
	// -----------------------------------------------------------
	void ObjectivePrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveEventBase GetObjectiveType
	// -----------------------------------------------------------
	int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.NONE;
	}

#ifdef EXPANSIONMODMARKET
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase AddItemsToServerData
	// -----------------------------------------------------------
	//! Add the delivered delivery items to the market zone if there is one nearby.
	protected void AddItemsToServerData(string playerUID)
	{
		ObjectivePrint(ToString() + "::AddItemsToServerData - Start");

		ExpansionQuestPersistentServerData serverData = ExpansionQuestModule.GetModuleInstance().GetServerData();
		if (!serverData)
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;

		auto marketSettings = GetExpansionSettings().GetMarket();
		ExpansionMarketTraderZone traderZone = marketSettings.GetTraderZoneByPosition(player.GetPosition());
		if (!traderZone)
			return;

		vector zonePos = traderZone.Position;
		ObjectivePrint(ToString() + "::AddItemsToServerData - Market zone pos: " + zonePos.ToString());
		array<ref ExpansionQuestObjectiveDelivery> deliveries = m_ObjectiveConfig.GetDeliveries();
		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			string name = delivery.GetClassName();
			string nameLower = name;
			nameLower.ToLower();
			int amount = delivery.GetAmount();

			if (name.ToType().IsInherited(ExpansionQuestItemBase))
			{
				ObjectivePrint(ToString() + "::AddItemsToServerData - Item " + name + " extends from ExpansionQuestItemBase. Skip..");
				continue;
			}

			ExpansionMarketItem marketItem = marketSettings.GetItem(nameLower);
			if (!marketItem)
			{
				ObjectivePrint(ToString() + "::AddItemsToServerData - Item " + name + " is not a market item. Skip..");
				continue;
			}

			serverData.AddQuestMarketItem(zonePos, name, amount);
		}

		serverData.Save();

		ObjectivePrint(ToString() + "::AddItemsToServerData - End");
	}
#endif
};