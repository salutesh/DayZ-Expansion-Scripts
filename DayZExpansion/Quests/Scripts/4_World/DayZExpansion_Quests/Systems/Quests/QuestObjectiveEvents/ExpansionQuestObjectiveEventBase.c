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

	//! Event called when the player starts the quest
	void OnStart()
	{
		SetInitialized(true);
		SetIsActive(true);
	}

	//! Event called when objective is completed
	void OnComplete()
	{
		if (!m_Quest)
			return;
		
		for (int i = 0; i < m_Quest.GetObjectives(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = m_Quest.GetObjectives()[i];
			if (!objective)
				return;
			
			if (objective.GetIndex() == (GetIndex() + 1)) //! Get the next objective in the quests objective array
				objective.OnStart();
		}
		
		SetIsActive(false);
	}
	
	void OnIncomplete()
	{
		if (!m_Quest)
			return;
		
		SetIsActive(true);
	}

	//! Event called when quest is completed and turned-in
	void OnTurnIn();

	void OnCancel()
	{
		SetInitialized(false);
		SetIsActive(false);
	}

	//! Event called when the player continues the quest after a server restart/reconnect
	void OnContinue()
	{
		SetInitialized(true);
		SetIsActive(true);
	}

	//! Event called when the quest gets cleaned up
	void OnCleanup()
	{
		SetInitialized(false);
		SetIsActive(false);
	}

#ifdef EXPANSIONMODNAVIGATION
	//! Event called when the quest markers should get recreated
	void OnRecreateClientMarkers();
#endif

	void OnUpdate(float timeslice)
	{
		if (!IsInitialized() || IsCompleted() || !GetQuest() || !GetQuest().GetPlayer())
			return;
	}

	ExpansionQuestObjectiveConfigBase GetObjectiveConfig()
	{
		ExpansionQuestObjectiveTravelConfig travelObjective;
		ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
		ExpansionQuestObjectiveTargetConfig targetObjective;
		ExpansionQuestObjectiveCollectionConfig collectionObjective;
	#ifdef EXPANSIONMODAI
		ExpansionQuestObjectiveAIPatrolConfig aiPatrolObjective;
		ExpansionQuestObjectiveAICampConfig aiCampObjective;
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
	#ifdef EXPANSIONMODAI
		else if (Class.CastTo(aiPatrolObjective, m_ObjectiveConfig))
		{
			return aiPatrolObjective;
		}
		else if (Class.CastTo(aiCampObjective, m_ObjectiveConfig))
		{
			return aiCampObjective;
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
		ObjectivePrint("------------------------------------------------------------");
	}

	void ObjectivePrint(string text)
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	//#endif
	}
	
	int GetType()
	{
		return ExpansionQuestObjectiveType.NONE;
	}
};