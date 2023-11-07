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
class ExpansionQuestObjectiveAICampEvent: ExpansionQuestObjectiveAIEventBase
{
	protected ExpansionAICampObjectiveSphereTrigger m_ObjectiveTrigger;
	protected ref ExpansionQuestObjectiveAICampConfig m_AICampConfig;

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
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

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return false;
		
		if (!super.OnContinue())
			return false;

		return true;
	}

	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCleanup())
			return false;

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		return true;
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = m_AICampConfig.GetAICamp();
		if (!aiCamp)
			return;

		//! If the ai camp need to be killed with a special weapon check incoming killer class type
		if (aiCamp.NeedSpecialWeapon())
		{
			if (!ExpansionStatic.IsAnyOf(killer, aiCamp.GetAllowedWeapons(), true))
				return;
		}

		//! Check if killed entities class name is a valid one from our objective config
		bool found = ExpansionStatic.IsAnyOf(victim, aiCamp.GetClassNames(), true);
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

		if (!Class.CastTo(m_AICampConfig, m_ObjectiveConfig))
			return;

		CheckQuestAIPatrol(m_TotalUnitsAmount);
	}

	override void CreateQuestAIPatrol()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = m_AICampConfig.GetAICamp();
		if (!aiCamp)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		for (int i = 0; i < m_UnitsToSpawn; i++)
		{
			vector pos = aiCamp.GetPositions()[i];
			TVectorArray waypoint = new TVectorArray;
			waypoint.Insert(pos);

			ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(1, aiCamp.GetNPCSpeed(), "SPRINT", aiCamp.GetNPCMode(), aiCamp.GetNPCFaction(), aiCamp.GetNPCLoadoutFile(), m_AICampConfig.CanLootAI(), true, waypoint);
			group.Formation = "RANDOM";  //! Just set a default, it's not really used as the NPCs are separate
			group.AccuracyMin = aiCamp.NPCAccuracyMin;
			group.AccuracyMax = aiCamp.NPCAccuracyMax;
			eAIDynamicPatrol patrol = CreateQuestPatrol(group, 0, 600, 300, m_AICampConfig.GetMinDistRadius(), m_AICampConfig.GetMaxDistRadius(), m_AICampConfig.GetDespawnRadius());
			if (!patrol)
				return;

			questPatrols.Insert(patrol);
		}

		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

		if (m_AICampConfig.GetInfectedDeletionRadius() > 0 && !m_ObjectiveTrigger)
			CreateTrigger(aiCamp.GetPositions()[0], m_AICampConfig.GetInfectedDeletionRadius());

	#ifdef EXPANSIONMODNAVIGATION
		if (m_AICampConfig.GetObjectiveText() != string.Empty)
			CreateMarkers();
	#endif
	}

	protected void CreateTrigger(vector pos, int radius)
	{
		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionAICampObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetTriggerRadius(radius);
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		ExpansionQuestObjectiveAICamp aiCamp = m_AICampConfig.GetAICamp();
		if (!aiCamp)
			return;

		string markerName = m_AICampConfig.GetObjectiveText();
		array<vector> positions = aiCamp.GetPositions();
		if (positions)
			CreateObjectiveMarker(positions[0], markerName);
	}
#endif

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif