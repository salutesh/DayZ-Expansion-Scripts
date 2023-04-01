/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{
	protected static ref array<ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new ref array<ExpansionQuestObjectiveEventBase>;

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS);

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS);

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.Remove(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	protected void CheckAssignedObjectivesForEntity(Object killer)
	{
		if (s_Expansion_AssignedQuestObjectives.Count() == 0)
			return;
		
		EntityAI killSource = EntityAI.Cast(killer);
		if (!killSource || killSource == this)
			return;

		Man killerPlayer = killSource.GetHierarchyRootPlayer();
		if (!killerPlayer || !killerPlayer.GetIdentity())
			return;

		string killerUID = killerPlayer.GetIdentity().GetId();
		if (killerUID == string.Empty)
			return;

		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			if (!objective.GetQuest())
				continue;
			
			if (!objective.GetQuest().IsQuestPlayer(killerUID))
				continue;

			int objectiveType = objective.GetObjectiveType();
			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.TARGET:
				{
					ExpansionQuestObjectiveTargetEvent targetEvent;
					if (Class.CastTo(targetEvent, objective))
						targetEvent.OnEntityKilled(this, killSource, killerPlayer);
				}
				break;
			}
		}
	}

	override void EEKilled(Object killer)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		EXTrace.Print(EXTrace.QUESTS, this, "Killed entity: " + GetType());
		EXTrace.Print(EXTrace.QUESTS, this, "Killer entity: " + killer.GetType());
	#endif

		super.EEKilled(killer);

		CheckAssignedObjectivesForEntity(killer);
	}
};