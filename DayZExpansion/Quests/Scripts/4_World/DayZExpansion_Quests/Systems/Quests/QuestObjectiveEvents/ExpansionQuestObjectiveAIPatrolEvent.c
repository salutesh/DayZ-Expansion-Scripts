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
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIPatrolEvent OnEntityKilled
	// -----------------------------------------------------------
	override void OnEntityKilled(EntityAI victim, EntityAI killer, Man killerPlayer = NULL)
	{
		ObjectivePrint(ToString() + "::OnEntityKilled - Start");

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_ObjectiveConfig.GetAIPatrol();
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

		ObjectivePrint(ToString() + "::OnEntityKilled - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIPatrolEvent CheckQuestAIPatrol
	// -----------------------------------------------------------
	override protected void CheckQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - Start");

		if (!GetObjectiveConfig())
			return;

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_ObjectiveConfig.GetAIPatrol();
		if (!aiPatrol)
			return;

		m_TotalUnitsAmount = aiPatrol.GetNPCUnits();

		CheckQuestAIPatrol(1);

		ObjectivePrint(ToString() + "::CheckQuestAIPatrol - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIPatrolEvent CreateQuestAIPatrol
	// -----------------------------------------------------------
	override void CreateQuestAIPatrol()
	{
		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - Start");

		ExpansionQuestObjectiveAIPatrol aiPatrol = m_ObjectiveConfig.GetAIPatrol();
		if (!aiPatrol)
			return;

		m_UnitsToSpawn = m_TotalUnitsAmount - m_TotalKillCount;

		array<eAIDynamicPatrol> questPatrols = new array<eAIDynamicPatrol>;
		ExpansionQuestAIGroup group = new ExpansionQuestAIGroup(m_UnitsToSpawn, aiPatrol.GetNPCSpeed(), aiPatrol.GetNPCMode(), "ALTERNATE", aiPatrol.GetNPCFaction(), aiPatrol.GetNPCLoadoutFile(), m_ObjectiveConfig.CanLootAI(), false, aiPatrol.GetWaypoints());
		group.Formation = aiPatrol.NPCFormation;
		group.AccuracyMin = aiPatrol.NPCAccuracyMin;
		group.AccuracyMax = aiPatrol.NPCAccuracyMax;
		eAIDynamicPatrol patrol = ExpansionQuestObjectiveAIEventBase.CreateQuestPatrol(group, 0, 600, 300, m_ObjectiveConfig.GetMinDistRadius(), m_ObjectiveConfig.GetMaxDistRadius(), m_ObjectiveConfig.GetDespawnRadius());
		if (!patrol)
			return;

		questPatrols.Insert(patrol);
		ExpansionQuestModule.GetModuleInstance().SetQuestPatrols(m_Quest.GetQuestConfig().GetID(), questPatrols);

	#ifdef EXPANSIONMODNAVIGATION
		string markerName = m_Quest.GetQuestConfig().GetObjectives().Get(GetIndex()).GetObjectiveText();
		if (markerName != string.Empty)
			m_Quest.CreateClientMarker(aiPatrol.GetWaypoints()[0], markerName);
	#endif

		ObjectivePrint(ToString() + "::CreateQuestAIPatrol - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveAIPatrolEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.AIPATROL;
	}
};
#endif