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

class ExpansionObjectiveTriggerBase: Trigger
{
	protected ExpansionQuestObjectiveEventBase m_Objective;

	void ExpansionObjectiveTriggerBase()
	{
		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);
	}

	void SetObjectiveData(ExpansionQuestObjectiveEventBase objective)
	{
		m_Objective = objective;
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

#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
	protected void TriggerDebug()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		vector pos = GetPosition();
		QuestPrint("Position: " + pos);
		QuestPrint("Objective: " + m_Objective.ToString());
		m_Objective.QuestDebug();
	}
#endif

	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}

	override protected void RemoveInsiderByObject(Object object)
	{
		//! Do nothing..
	}

	protected void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, text);
	#endif
	}
};