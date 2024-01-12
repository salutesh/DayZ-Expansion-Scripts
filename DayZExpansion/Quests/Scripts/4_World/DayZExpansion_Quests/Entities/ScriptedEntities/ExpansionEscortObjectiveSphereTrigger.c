/**
 * ExpansionEscortObjectiveSphereTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionEscortObjectiveSphereTrigger: ExpansionObjectiveTriggerBase
{
	protected ExpansionQuestObjectiveAIEscortEvent m_Objective;
	
	void ExpansionEscortObjectiveSphereTrigger()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		SetEventMask(EntityEvent.ENTER);
		m_TriggerType = ExpansionObjectiveTriggerType.AIESCORT;
	}

	void SetObjectiveEvent(ExpansionQuestObjectiveAIEscortEvent objective)
	{
		m_Objective = objective;
	}

	//! When an Object enters the trigger
	override void EOnEnter(IEntity other, int extra)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Entity: " + other.Type());
	#endif
		
	#ifdef SERVER
		if (!m_Objective || !m_Objective.IsActive() || m_Objective.IsCompleted())
			return;

		eAIBase eAIplayer;
		if (!Class.CastTo(eAIplayer, other))
			return;
		
		ExpansionQuestObjectiveAIEscortEvent escortEvent;
		if (!Class.CastTo(escortEvent, m_Objective))
			return;
		
		if (eAIplayer != escortEvent.GetAIVIP())
			return;
		
		escortEvent.SetReachedLocation(true);
	#endif
	}
};
#endif