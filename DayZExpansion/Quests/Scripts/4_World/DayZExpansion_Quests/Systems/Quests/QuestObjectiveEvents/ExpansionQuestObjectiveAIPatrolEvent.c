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
class ExpansionQuestObjectiveAIPatrolEvent: ExpansionQuestObjectiveAIEventBase
{
	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEntityKilled");
	#endif

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
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

		ObjectivePrint(ToString() + "::OnEntityKilled - Target found: " + found);

		if (!found)
			return;

		m_TotalUnitsAmount = aiPatrol.GetNPCUnits();
		super.OnEntityKilled(victim, killer, killerPlayer);
	}

	override protected void CheckQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - Start");

		if (!GetQuest() || !GetQuest().GetQuestModule() || !GetQuest().GetQuestConfig() || !GetObjectiveConfig())
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		m_TotalUnitsAmount = aiPatrol.GetNPCUnits();

		CheckQuestAIPatrol(1);
	}

	override void CreateQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - Start");

		ExpansionQuestObjectiveAIPatrol aiPatrol = GetObjectiveConfig().GetAIPatrol();
		if (!aiPatrol)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), "ALTERNATE", aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), GetObjectiveConfig().CanLootAI(), false, aiPatrol.GetWaypoints());
		group.Formation = aiPatrol.NPCFormation;
		group.AccuracyMin = aiPatrol.NPCAccuracyMin;
		group.AccuracyMax = aiPatrol.NPCAccuracyMax;
		eAIDynamicPatrol patrol = ExpansionQuestObjectiveAIEventBase.CreateQuestPatrol(group, 0, 600, 300, GetObjectiveConfig().GetMinDistRadius(), GetObjectiveConfig().GetMaxDistRadius(), GetObjectiveConfig().GetDespawnRadius());
		if (!patrol)
			return;

		questPatrols.Insert(patrol);
		GetQuest().GetQuestModule().SetQuestPatrols(GetQuest().GetQuestConfig().GetID(), questPatrols);

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = GetQuest().GetQuestConfig().GetObjectives().Get(GetIndex()).GetObjectiveText();
		if (markerName != string.Empty)
			GetQuest().CreateClientMarker(aiPatrol.GetWaypoints()[0], markerName);
	#endif

		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - End");
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif