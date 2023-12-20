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
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, ZombieBase);

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
		auto trace = EXTrace.Start(EXTrace.QUESTS, ZombieBase);

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.RemoveOrdered(index);
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

		ExpansionQuest quest;
		int failSafe = s_Expansion_AssignedQuestObjectives.Count() + 1;
		for (int i = 0, j = 0; i < s_Expansion_AssignedQuestObjectives.Count() && j < failSafe; j++)
		{
			ExpansionQuestObjectiveEventBase objective = s_Expansion_AssignedQuestObjectives[i];
			if (!objective)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "WARNING: Objective is NULL!");
				s_Expansion_AssignedQuestObjectives.RemoveOrdered(i);
				continue;
			}

			quest = objective.GetQuest();
			if (!quest)
			{
				i++;
				continue;
			}
			
			if (!quest.IsQuestPlayer(killerUID))
			{
				i++;
				continue;
			}

			int objectiveType = objective.GetObjectiveType();
			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.TARGET:
					ExpansionQuestObjectiveTargetEvent targetEvent;
					if (Class.CastTo(targetEvent, objective))
						targetEvent.OnEntityKilled(this, killSource, killerPlayer);
					break;
			}

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(ToString() + " ::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
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
