/**
 * RecipeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class RecipeBase
{
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, RecipeBase);

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
		auto trace = EXTrace.Start(EXTrace.QUESTS, RecipeBase);

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

	protected void CheckAssignedObjectives(PlayerBase player, array<ItemBase> spawned_objects)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		if (s_Expansion_AssignedQuestObjectives.Count() == 0)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "No quest objectives assigned!");
		#endif
			return;
		}

		if (!player || !player.GetIdentity())
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "Could not get player of action data!");
		#endif
			return;
		}

		string playerUID = player.GetIdentity().GetId();
		if (playerUID == string.Empty)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "Could not get player UID!");
		#endif
			return;
		}

		ExpansionQuest quest;
		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			//! Check if the current objective is active
			if (!objective.IsActive())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective is not active. Skip...");
			#endif
				continue;
			}

			quest = objective.GetQuest();
			if (!quest)
				continue;

			//! Check if the current objective belongs to the item owner if we got the UID.
			if (!quest.IsQuestPlayer(playerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Player with UID [" + playerUID + "] is not a quest player of this quest objective. Skip...");
			#endif
				continue;
			}

			OnObjectiveActionExecuted(objective, player, spawned_objects);
		}
	}

	protected void OnObjectiveActionExecuted(ExpansionQuestObjectiveEventBase objective, PlayerBase player, array<ItemBase> spawned_objects)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		//! Run thrue all possible objective types
		switch (objective.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingEvent craftingObjective;
				if (Class.CastTo(craftingObjective, objective))
				{
					craftingObjective.OnObjectiveActionExecuted(player, spawned_objects);
				}
			}
			break;
		}
	}

	override void SpawnItems(ItemBase ingredients[], PlayerBase player, array<ItemBase> spawned_objects)
	{
		super.SpawnItems(ingredients, player, spawned_objects);
		CheckAssignedObjectives(player, spawned_objects);
	}
}
