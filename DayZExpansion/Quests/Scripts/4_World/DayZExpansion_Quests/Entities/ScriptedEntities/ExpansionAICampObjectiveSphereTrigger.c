/**
 * ExpansionAICampObjectiveSphereTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAICampObjectiveSphereTrigger: ExpansionObjectiveTriggerBase
{
	void ExpansionAICampObjectiveSphereTrigger()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		SetEventMask(EntityEvent.ENTER);
		m_TriggerType = ExpansionObjectiveTriggerType.AICAMP;
	}

	//! Event called when an entity enters the trigger area.
	override void EOnEnter(IEntity other, int extra)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

	#ifdef SERVER
		ZombieBase infected;
		if (Class.CastTo(infected, other))
			g_Game.ObjectDelete(infected);
	#endif
	}
};
