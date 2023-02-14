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
	protected static ref array<ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ExpansionQuestObjectiveEventBase>;

	// ------------------------------------------------------------
	// ActionBase AssignQuestObjective
	// ------------------------------------------------------------
	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("ActionBase::AssignQuestObjective - Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("ActionBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	// ------------------------------------------------------------
	// ActionBase DeassignQuestObjective
	// ------------------------------------------------------------
	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.Remove(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("ActionBase::DeassignQuestObjective - Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("ActionBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	// ------------------------------------------------------------
	// ActionBase CheckAssignedObjectives
	// ------------------------------------------------------------
	protected void CheckAssignedObjectives(ActionData action_data, bool isInit)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectives - Start");
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (!s_Expansion_AssignedQuestObjectives || s_Expansion_AssignedQuestObjectives.Count() == 0)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::CheckAssignedObjectives - No quest objectives assigned!");
		#endif
			return;
		}

		if (!action_data.m_Player || !action_data.m_Player.GetIdentity())
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::CheckAssignedObjectives - Could not get player of action data!");
		#endif
			return;
		}

		string playerUID = action_data.m_Player.GetIdentity().GetId();
		if (playerUID == string.Empty)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::CheckAssignedObjectives - Could not get player UID!");
		#endif
			return;
		}

		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			if (!objective || !objective.GetQuest())
				continue;

			//! Check if the current objective belongs to the item owner if we got the UID.
			if (!objective.GetQuest().IsQuestPlayer(playerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectives - Player with UID [" + playerUID + "] is not a quest player of this quest objective. Skip..");
			#endif
				return;
			}

			//! Check if the current objective is active
			if (!objective.IsActive())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectives - Objective is not active. Skip..");
			#endif
				continue;
			}

			OnObjectiveActionExecuted(objective, action_data, isInit);
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectives - End");
	#endif
	}

	// ------------------------------------------------------------
	// ActionBase OnObjectiveActionExecuted
	// ------------------------------------------------------------
	protected void OnObjectiveActionExecuted(ExpansionQuestObjectiveEventBase objective, ActionData action_data, bool isInit)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnObjectiveActionExecuted - Start");
	#endif

		//! Run thrue all possible objective types
		switch (objective.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionEvent actionObjective;
				if (Class.CastTo(actionObjective, objective))
				{
					actionObjective.OnObjectiveActionExecuted(this, action_data, isInit);
				}
			}
			break;
			
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORD:
			{
				if (ClassName() != "ExpansionActionDismissAI")
					return;

				ExpansionQuestObjectiveAIEscortEvent aiEscordObjective;
				if (!Class.CastTo(aiEscordObjective, objective))
					return;
				
				eAIBase aiUnit;
				if (!Class.CastTo(aiUnit, action_data.m_Target.GetObject()))
					return;
				
				if (aiUnit != aiEscordObjective.GetAIVIP())
					return;
					
				aiEscordObjective.OnDissmissAIGroup();
			}
			break;
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnObjectiveActionExecuted - End");
	#endif
	}
};

modded class ActionContinuousBase
{
	override void OnStartAnimationLoop(ActionData action_data)
	{
		super.OnStartAnimationLoop(action_data);
		CheckAssignedObjectives(action_data, true);
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);
		CheckAssignedObjectives(action_data, false);
	}
};


modded class AnimatedActionBase
{
	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);
		CheckAssignedObjectives(action_data, true);
	}

	override protected void OnExecuteServer(ActionData action_data)
	{
		super.OnExecuteServer(action_data);
		CheckAssignedObjectives(action_data, false);
	}
};