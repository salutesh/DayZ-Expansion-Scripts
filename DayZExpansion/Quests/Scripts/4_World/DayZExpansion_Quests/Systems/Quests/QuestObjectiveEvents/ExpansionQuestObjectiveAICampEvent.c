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
	override protected void CleanupPatrol(bool despawn = false)
	{
		super.CleanupPatrol(despawn);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CleanupZeds);
	}

	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
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
	}

	override protected void CheckQuestAIPatrol()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CheckQuestAIPatrol");
	#endif

		if (!GetQuest() || !GetQuest().GetQuestModule() || !GetQuest().GetQuestConfig() || !GetObjectiveConfig())
			return;

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;

		m_TotalUnitsAmount = aiCamp.GetPositions().Count();

		CheckQuestAIPatrol(m_TotalUnitsAmount);
	}

	override void CreateQuestAIPatrol()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CreateQuestAIPatrol");
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
		if (!aiCamp)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		for (int i = 0; i < m_UnitsToSpawn; i++)
		{
			vector pos = aiCamp.GetPositions()[i];
			array<vector> waypoint = new array<vector>;
			waypoint.Insert(pos);

			ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(1, aiCamp.GetNPCSpeed(), aiCamp.GetNPCMode(), "HALT", aiCamp.GetNPCFaction(), aiCamp.GetNPCLoadoutFile(), GetObjectiveConfig().CanLootAI(), false, waypoint);
			group.Formation = "RANDOM";  //! Just set a default, it's not really used as the NPCs are separate
			group.AccuracyMin = aiCamp.NPCAccuracyMin;
			group.AccuracyMax = aiCamp.NPCAccuracyMax;
			eAIDynamicPatrol patrol = CreateQuestPatrol(group, 0, 600, 300, GetObjectiveConfig().GetMinDistRadius(), GetObjectiveConfig().GetMaxDistRadius(), GetObjectiveConfig().GetDespawnRadius());
			if (!patrol)
				return;

			questPatrols.Insert(patrol);
		}

		GetQuest().GetQuestModule().SetQuestPatrols(GetQuest().GetQuestConfig().GetID(), questPatrols);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CleanupZeds, 30000, true);

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = GetObjectiveConfig().GetObjectiveText();

		if (markerName != string.Empty)
			GetQuest().CreateClientMarker(aiCamp.GetPositions()[0], markerName);
	#endif
	}

	protected override void CleanupZeds()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CleanupZeds");
	#endif

		ExpansionQuestObjectiveAICamp aiCamp = GetObjectiveConfig().GetAICamp();
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

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AICAMP;
	}
};
#endif