/**
 * ExpansionQuestObjectiveAIPatrolEvent.c
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
class ExpansionQuestObjectiveAICampEvent: ExpansionQuestObjectiveAIEventBase
{
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent CleanupPatrol
	// -----------------------------------------------------------
	override protected void CleanupPatrol(bool despawn = false)
	{
		ObjectivePrint(ToString() + "::CleanupPatrol - Start");

		super.CleanupPatrol(despawn);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CleanupZeds);

		ObjectivePrint(ToString() + "::CleanupPatrol - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent OnEntityKilled
	// -----------------------------------------------------------
	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
		ObjectivePrint(ToString() + "::OnEntityKilled - Start");

		ExpansionQuestObjectiveAICamp aiCamp = m_ObjectiveConfig.GetAICamp();
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
		ObjectivePrint(ToString() + "::OnEntityKilled - Target found: " + found);
		if (!found)
			return;

		super.OnEntityKilled(victim, killer, killerPlayer);

		ObjectivePrint(ToString() + "::OnEntityKilled - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent CheckQuestAIPatrol
	// -----------------------------------------------------------
	override protected void CheckQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - Start");

		if (!GetObjectiveConfig())
			return;

		ExpansionQuestObjectiveAICamp aiCamp = m_ObjectiveConfig.GetAICamp();
		if (!aiCamp)
			return;

		m_TotalUnitsAmount = aiCamp.GetPositions().Count();

		CheckQuestAIPatrol(m_TotalUnitsAmount);

		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent CreateQuestAIPatrol
	// -----------------------------------------------------------
	override void CreateQuestAIPatrol()
	{
		ExpansionQuestObjectiveAICamp aiCamp = m_ObjectiveConfig.GetAICamp();
		if (!aiCamp)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		for (int i = 0; i < m_UnitsToSpawn; i++)
		{
			vector pos = aiCamp.GetPositions()[i];
			array<vector> waypoint = new array<vector>;
			waypoint.Insert(pos);

			ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(1, aiCamp.GetNPCSpeed(), aiCamp.GetNPCMode(), "HALT", aiCamp.GetNPCFaction(), aiCamp.GetNPCLoadoutFile(), m_ObjectiveConfig.CanLootAI(), false, waypoint);
			group.Formation = "RANDOM";  //! Just set a default, it's not really used as the NPCs are separate
			group.AccuracyMin = aiCamp.NPCAccuracyMin;
			group.AccuracyMax = aiCamp.NPCAccuracyMax;
			eAIDynamicPatrol patrol = CreateQuestPatrol(group, 0, 600, 300, m_ObjectiveConfig.GetMinDistRadius(), m_ObjectiveConfig.GetMaxDistRadius(), m_ObjectiveConfig.GetDespawnRadius());
			if (!patrol)
				return;

			questPatrols.Insert(patrol);
		}

		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupZeds, 30000, true);

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = m_ObjectiveConfig.GetObjectiveText();

		if (markerName != string.Empty)
			m_Quest.CreateClientMarker(aiCamp.GetPositions()[0], markerName);
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent CleanupZeds
	// -----------------------------------------------------------
	//! ToDo: Spawn a trigger that calls that method on infected that get spawned in the objective area.
	//! Maybe make this also optional as a objective configuration setting.
	protected override void CleanupZeds()
	{
		ExpansionQuestObjectiveAICamp aiCamp = m_ObjectiveConfig.GetAICamp();
		if (!aiCamp)
			return;

		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(aiCamp.GetPositions()[0], 500, objects, NULL);
		foreach (Object obj: objects)
		{
			if (obj.IsInherited(ZombieBase))
				GetGame().ObjectDelete(obj);
		}
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAICampEvent CleanupZeds
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif