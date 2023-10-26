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
	protected ref ExpansionQuestObjectiveAIPatrolConfig m_Config;

	override bool OnEventStart()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (!super.OnEventStart())
			return false;

		return true;
	}

	override bool OnContinue()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;
		
		if (!super.OnContinue())
			return false;

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!m_Config)
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_Config.GetAIPatrol();
		if (!aiPatrol)
			return;

		string markerName = m_Config.GetObjectiveText();
		array<vector> waypoints = aiPatrol.GetWaypoints();
		if (waypoints)
			CreateObjectiveMarker(waypoints[0], markerName);
	}
#endif

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_Config.GetAIPatrol();
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_Config)
			return;

		CheckQuestAIPatrol(1);
	}

	override void CreateQuestAIPatrol()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_Config.GetAIPatrol();
		if (!aiPatrol)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), "SPRINT", aiPatrol.GetNPCMode(), aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), m_Config.CanLootAI(), true, aiPatrol.GetWaypoints());
		group.Formation = aiPatrol.NPCFormation;
		group.AccuracyMin = aiPatrol.NPCAccuracyMin;
		group.AccuracyMax = aiPatrol.NPCAccuracyMax;
		eAIDynamicPatrol patrol = ExpansionQuestObjectiveAIEventBase.CreateQuestPatrol(group, 0, 600, 300, m_Config.GetMinDistRadius(), m_Config.GetMaxDistRadius(), m_Config.GetDespawnRadius());
		if (!patrol)
			return;

		questPatrols.Insert(patrol);
		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetObjectiveText() != string.Empty)
			CreateMarkers();
	#endif
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif