/**
 * ExpansionQuestObjectiveAIPatrolEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIPatrolEvent: ExpansionQuestObjectiveAIEventBase
{
	protected ref ExpansionQuestObjectiveAIPatrolConfig m_AIPatrolConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AIPatrolConfig, m_ObjectiveConfig))
			return false;

		if (!super.OnEventStart())
			return false;

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AIPatrolConfig, m_ObjectiveConfig))
			return false;
		
		if (!super.OnContinue())
			return false;

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		ExpansionQuestObjectiveAIPatrol aiPatrol = m_AIPatrolConfig.GetAIPatrol();
		if (!aiPatrol)
			return;

		string markerName = m_AIPatrolConfig.GetObjectiveText();
		array<vector> waypoints = aiPatrol.GetWaypoints();
		if (waypoints)
			CreateObjectiveMarker(waypoints[0], markerName);
	}
#endif

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_AIPatrolConfig.GetAIPatrol();
		if (!aiPatrol)
			return;

		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiPatrol.NeedSpecialWeapon())
		{
			if (!ExpansionStatic.IsAnyOf(killer, aiPatrol.GetAllowedWeapons(), true))
				return;
		}

		//! Check if killed entities class name is a valid one from our objective config
		bool found = ExpansionStatic.IsAnyOf(victim, aiPatrol.GetClassNames(), true);
		ObjectivePrint("Target found: " + found);
		if (!found)
			return;

		super.OnEntityKilled(victim, killer, killerPlayer);
	}

	override protected void CheckQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_AIPatrolConfig)
			return;

		CheckQuestAIPatrol(1);
	}

	override void CreateQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_AIPatrolConfig.GetAIPatrol();
		if (!aiPatrol)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), "SPRINT", aiPatrol.GetNPCMode(), aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), m_AIPatrolConfig.CanLootAI(), true, aiPatrol.GetWaypoints());
		group.Formation = aiPatrol.NPCFormation;
		group.AccuracyMin = aiPatrol.NPCAccuracyMin;
		group.AccuracyMax = aiPatrol.NPCAccuracyMax;
		eAIDynamicPatrol patrol = ExpansionQuestObjectiveAIEventBase.CreateQuestPatrol(group, 0, 600, 300, m_AIPatrolConfig.GetMinDistRadius(), m_AIPatrolConfig.GetMaxDistRadius(), m_AIPatrolConfig.GetDespawnRadius());
		if (!patrol)
			return;

		questPatrols.Insert(patrol);
		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

	#ifdef EXPANSIONMODNAVIGATION
		if (m_AIPatrolConfig.GetObjectiveText() != string.Empty)
			CreateMarkers();
	#endif
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif