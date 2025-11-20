/**
 * ActionBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionBase
{
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, ActionBase);
#endif 
		
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, ActionBase);
#endif 
		
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

	protected void CheckAssignedObjectives(ActionData action_data)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!g_Game.IsServer() && !g_Game.IsMultiplayer())
			return;

		if (s_Expansion_AssignedQuestObjectives.Count() == 0)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "No quest objectives assigned!");
		#endif
			return;
		}

		if (!action_data.m_Player || !action_data.m_Player.GetIdentity())
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "Could not get player of action data!");
		#endif
			return;
		}

		string playerUID = action_data.m_Player.GetIdentity().GetId();
		if (playerUID == string.Empty)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "Could not get player UID!");
		#endif
			return;
		}

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

			//! Check if the current objective is active
			if (!objective.IsActive())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective is not active. Skip...");
			#endif
				i++;
				continue;
			}

			quest = objective.GetQuest();
			if (!quest)
			{
				i++;
				continue;
			}

			//! Check if the current objective belongs to the item owner if we got the UID.
			if (!quest.IsQuestPlayer(playerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Player with UID [" + playerUID + "] is not a quest player of this quest objective. Skip...");
			#endif
				i++;
				continue;
			}

			OnObjectiveActionExecuted(objective, action_data);

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(ToString() + " ::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
	}

	protected void OnObjectiveActionExecuted(ExpansionQuestObjectiveEventBase objective, ActionData action_data)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		//! Run thrue all possible objective types
		switch (objective.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionEvent actionObjective;
				if (Class.CastTo(actionObjective, objective))
				{
					actionObjective.OnObjectiveActionExecuted(this, action_data);
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				if (ClassName() != "ExpansionActionDismissAI")
					return;

				ExpansionQuestObjectiveAIEscortEvent aiEscortObjective;
				if (!Class.CastTo(aiEscortObjective, objective))
					return;

				eAIBase aiUnit;
				if (!Class.CastTo(aiUnit, action_data.m_Target.GetObject()))
					return;

				if (aiUnit != aiEscortObjective.GetAIVIP())
					return;

				aiEscortObjective.OnDissmissAIGroup();
			}
			break;
		#endif
		}
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		if (g_Game.IsServer() && IsInstant())
			CheckAssignedObjectives(action_data);
	}

	override void OnEnd(ActionData action_data)
	{
		super.OnEnd(action_data);

		if (g_Game.IsServer() && !IsInstant() && !IsInherited(AnimatedActionBase))
			CheckAssignedObjectives(action_data);
	}
};

modded class ActionContinuousBase
{
	override void OnFinishProgress(ActionData action_data)
	{
		super.OnFinishProgress(action_data);

		if (g_Game.IsServer())
			CheckAssignedObjectives(action_data);
	}
};


modded class AnimatedActionBase
{
	override void OnExecute(ActionData action_data)
	{
		super.OnExecute(action_data);

		if (g_Game.IsServer() && !IsInherited(ActionContinuousBase))
			CheckAssignedObjectives(action_data);
	}
};
