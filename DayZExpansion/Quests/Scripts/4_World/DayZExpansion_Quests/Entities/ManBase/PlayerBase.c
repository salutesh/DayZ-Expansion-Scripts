/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	protected static ref array<ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new ref array<ExpansionQuestObjectiveEventBase>;

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("PlayerBase::AssignQuestObjective - Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("PlayerBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.Remove(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("PlayerBase::DeassignQuestObjective - Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("PlayerBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	protected void CheckAssignedObjectivesForEntity(Object killer)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectivesForEntity - Start");
	#endif

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
			if (!objective || !objective.GetQuest())
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
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AICAMP:
				{
					ExpansionQuestObjectiveAICampEvent aiCampEvent;
					if (Class.CastTo(aiCampEvent, objective))
						aiCampEvent.OnEntityKilled(this, killSource, killerPlayer);
				}
				break;
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					ExpansionQuestObjectiveAIPatrolEvent aiPatrolEvent;
					if (Class.CastTo(aiPatrolEvent, objective))
						aiPatrolEvent.OnEntityKilled(this, killSource, killerPlayer);
				}
				break;
				case ExpansionQuestObjectiveType.AIESCORT:
				{
					ExpansionQuestObjectiveAIEscortEvent aiEscortEvent;
					if (Class.CastTo(aiEscortEvent, objective))
						aiEscortEvent.OnEntityKilled(this, killSource, killerPlayer);
				}
				break;
			#endif
			}
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectivesForEntity - End");
	#endif
	}

	override void EEKilled(Object killer)
	{
		CheckAssignedObjectivesForEntity(killer);

		super.EEKilled(killer);
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionOpenQuestMenu, InputActionMap);
	}
};
