/**
 * ExpansionObjectiveTriggerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionObjectiveTriggerType
{
	TRAVEL = 1,
	AICAMP,
	AIESCORT
};

class ExpansionObjectiveTriggerBase: Trigger
{
	protected ExpansionObjectiveTriggerType m_TriggerType;
	protected int m_QuestID = -1;
	protected int m_ObjectiveType = ExpansionQuestObjectiveType.NONE;
	protected int m_ObjectiveID = -1;
	
	void SetObjectiveData(int questID, int objectiveType, int objectiveID)
	{
		m_QuestID = questID;
		m_ObjectiveType = objectiveType;
		m_ObjectiveID = objectiveID;
	}
	
	int GetQuestID()
	{
		return m_QuestID;
	}
	
	int GetObjectiveType()
	{
		return m_ObjectiveType;
	}
	
	int GetObjectiveID()
	{
		return m_ObjectiveID;
	}
	
	void SetTriggerRadius(int radius)
	{
		SetCollisionSphere(radius);
	}

	//! Condition checks on given entity.
	bool EntityConditions(IEntity other)
	{
		return true;
	}

	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}

	override protected void RemoveInsiderByObject(Object object)
	{
		//! Do nothing..
	}

	ExpansionObjectiveTriggerType GetTriggerType()
	{
		return m_TriggerType;
	}

#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
	protected void TriggerDebug()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		vector pos = GetPosition();
		QuestPrint("Position: " + pos);
	}
#endif

	protected void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, text);
	#endif
	}
};