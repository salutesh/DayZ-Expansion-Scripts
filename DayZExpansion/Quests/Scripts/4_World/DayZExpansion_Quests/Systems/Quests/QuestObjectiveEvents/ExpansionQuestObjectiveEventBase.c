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

class ExpansionQuestObjectiveEventBase
{
	private int m_Index = -1;
	private ExpansionQuest m_Quest;
	private bool m_Completed = false;
	private bool m_Initialized = false;
	private bool m_Active = false;
	private ref ExpansionQuestObjectiveConfigBase m_ObjectiveConfig;
	private int m_TimeLimit = -1;	
	private float m_TimeLimitTimer = 0;
	private const float UPDATE_TIME_LIMIT_TICK_TIME = 1.0;
	private float m_TimeLimitSyncTimer = 0;
	private const float UPDATE_TIME_SYNC_TIME = 10.0;

	void ExpansionQuestObjectiveEventBase(ExpansionQuest quest)
	{
		m_Quest = quest;

		OnInit();
	}

	//! Init
	void OnInit();

	void SetIndex(int index)
	{
		m_Index = index;
	}

	int GetIndex()
	{
		return m_Index;
	}

	ExpansionQuest GetQuest()
	{
		return m_Quest;
	}

	bool IsInitialized()
	{
		return m_Initialized;
	}

	bool IsActive()
	{
		return m_Active;
	}

	void SetIsActive(bool state)
	{
		m_Active = state;
	}

	bool IsCompleted()
	{
		return m_Completed;
	}

	void SetCompleted(bool state)
	{
		m_Completed = state;
	}

	void SetInitialized(bool state)
	{
		m_Initialized = state;
	}

	void SetObjectiveConfig(ExpansionQuestObjectiveConfigBase config)
	{
		m_ObjectiveConfig = config;
	}

	ExpansionQuestObjectiveConfigBase GetObjectiveConfigBase()
	{
		return m_ObjectiveConfig;
	}
	
	void SetTimeLimit(int time)
	{
		m_TimeLimit = time;
	}
	
	int GetTimeLimit()
	{		
		return m_TimeLimit;
	}
	
	void OnTimeLimitReached()
	{
		if (!GetQuest().GetPlayer().GetIdentity())
			return;
		
		GetQuest().GetQuestModule().CancelQuestServer(GetQuest().GetQuestConfig().GetID(), GetQuest().GetPlayer().GetIdentity());
	}
	
	//! Event called when the player starts the quest
	void OnStart()
	{
		SetInitialized(true);
		SetIsActive(true);
		
		GetQuest().UpdateQuestPlayersObjectiveData();
	}

	//! Event called when objective is completed
	void OnComplete()
	{
		if (!GetQuest())
			return;
		
		GetQuest().CompletionCheck();
	}

	void OnIncomplete()
	{
		if (!GetQuest())
			return;

		SetIsActive(true);
		
		GetQuest().CompletionCheck();
	}

	//! Event called when quest is completed and turned-in
	void OnTurnIn()
	{
		SetInitialized(false);
		SetIsActive(false);
		
		GetQuest().UpdateQuestPlayersObjectiveData();
	}

	void OnCancel()
	{
		SetInitialized(false);
		SetIsActive(false);
		
		GetQuest().UpdateQuestPlayersObjectiveData();
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	void OnContinue()
	{
		SetInitialized(true);
		SetIsActive(true);
		
		GetQuest().UpdateQuestPlayersObjectiveData();
	}

	//! Event called when the quest gets cleaned up
	void OnCleanup()
	{
		SetInitialized(false);
	}

#ifdef EXPANSIONMODNAVIGATION
	//! Event called when the quest markers should get recreated
	void OnRecreateClientMarkers();
#endif

	void OnUpdate(float timeslice)
	{
		if (!IsInitialized() || !IsActive() || IsCompleted() || !GetQuest() || !GetQuest().GetPlayer() || !GetObjectiveConfig())
			return;
		
		if (m_TimeLimit > -1)
		{
			m_TimeLimitTimer += timeslice;
			if (m_TimeLimitTimer >= UPDATE_TIME_LIMIT_TICK_TIME)
			{
				m_TimeLimit -= UPDATE_TIME_LIMIT_TICK_TIME;
				if (m_TimeLimit <= 0)
				{
					OnTimeLimitReached();
					m_TimeLimit = -1;
				}
				
				m_TimeLimitTimer = 0;
			}
			
			m_TimeLimitSyncTimer +=timeslice;
			if (m_TimeLimit > 0 && m_TimeLimitSyncTimer >= UPDATE_TIME_SYNC_TIME)
			{
				GetQuest().UpdateQuestPlayersObjectiveData();
				m_TimeLimitSyncTimer = 0;
			}
		}
	}

	ExpansionQuestObjectiveConfigBase GetObjectiveConfig()
	{
		ExpansionQuestObjectiveTravelConfig travelObjective;
		ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
		ExpansionQuestObjectiveTargetConfig targetObjective;
		ExpansionQuestObjectiveCollectionConfig collectionObjective;
		ExpansionQuestObjectiveActionConfig actionObjective;
	#ifdef EXPANSIONMODAI
		ExpansionQuestObjectiveAIPatrolConfig aiPatrolObjective;
		ExpansionQuestObjectiveAICampConfig aiCampObjective;
		ExpansionQuestObjectiveAIVIPConfig aiVIPObjective;
	#endif

		if (Class.CastTo(travelObjective, m_ObjectiveConfig))
		{
			return travelObjective;
		}
		else if (Class.CastTo(deliveryObjective, m_ObjectiveConfig))
		{
			return deliveryObjective;
		}
		else if (Class.CastTo(targetObjective, m_ObjectiveConfig))
		{
			return targetObjective;
		}
		else if (Class.CastTo(collectionObjective, m_ObjectiveConfig))
		{
			return collectionObjective;
		}
		else if (Class.CastTo(collectionObjective, m_ObjectiveConfig))
		{
			return collectionObjective;
		}
		else if (Class.CastTo(actionObjective, m_ObjectiveConfig))
		{
			return actionObjective;
		}
	#ifdef EXPANSIONMODAI
		else if (Class.CastTo(aiPatrolObjective, m_ObjectiveConfig))
		{
			return aiPatrolObjective;
		}
		else if (Class.CastTo(aiCampObjective, m_ObjectiveConfig))
		{
			return aiCampObjective;
		}
		else if (Class.CastTo(aiVIPObjective, m_ObjectiveConfig))
		{
			return aiVIPObjective;
		}
	#endif

		return m_ObjectiveConfig;
	}

	//! Event method for condition checks when ever a quest completion is requested
	bool CompletionCheck()
	{
		return true;
	}
	
	void QuestDebug()
	{
		ObjectivePrint("------------------------------------------------------------");
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective index: " + m_Index);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective quest: " + m_Quest);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective completed: " + m_Completed);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective Initialized: " + m_Initialized);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective active: " + m_Active);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective config: " + m_ObjectiveConfig);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective time limit: " + m_TimeLimit);
		ObjectivePrint(ToString() + "::ObjectiveDebug - Objective time remaining: " + GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).GetRemainingTime(OnTimeLimitReached));
		ObjectivePrint("------------------------------------------------------------");
	}

	void ObjectivePrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}

	int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.NONE;
	}
};