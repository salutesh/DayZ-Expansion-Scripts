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
class ExpansionEscortObjectiveSphereTrigger: Trigger
{
	protected ExpansionQuestObjectiveAIEscortEvent m_Objective;

	void ExpansionEscortObjectiveSphereTrigger()
	{
		SetEventMask(EntityEvent.ENTER);
	}

	void SetObjectiveData(int radius, ExpansionQuestObjectiveAIEscortEvent objective)
	{
		m_Objective = objective;
		SetCollisionSphere(radius);
	}
	
	override protected void AddInsider(Object obj)
	{
		//! Do nothing..
	}

	//! When an Object enters the trigger
	override void EOnEnter(IEntity other, int extra)
	{
		eAIBase eAIplayer;
		if (!Class.CastTo(eAIplayer, other))
			return;

		if (!eAIplayer.IsAI() || eAIplayer != m_Objective.GetAIVIP())
			return;

		if (!m_Objective.GetQuest())
			return;

		if (m_Objective && m_Objective.IsActive() && !m_Objective.IsCompleted())
		{
			m_Objective.SetReachedLocation(true);
		}
	}
};
#endif