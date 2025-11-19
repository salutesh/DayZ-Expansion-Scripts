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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, RecipeBase);
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
		auto trace = EXTrace.Start(EXTrace.QUESTS, RecipeBase);
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

	protected void CheckAssignedObjectives(PlayerBase player, array<ItemBase> spawned_objects)
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

			OnObjectiveActionExecuted(objective, player, spawned_objects);

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(ToString() + " ::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
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
		int questID = -1;
		ItemBase ingredient1 = ingredients[0];
		ItemBase ingredient2 = ingredients[1];
		if (ingredient1 && ingredient1.Expansion_IsQuestItem())
			questID = ingredient1.Expansion_GetQuestID();
		
		if (questID == -1 && ingredient2 && ingredient2.Expansion_IsQuestItem())
			questID = ingredient2.Expansion_GetQuestID();
		
		super.SpawnItems(ingredients, player, spawned_objects);
		
		if (questID > -1)
		{
			foreach (ItemBase resultIB: spawned_objects)
			{
				resultIB.Expansion_SetQuestID(questID);
			}
		}
		
		CheckAssignedObjectives(player, spawned_objects);
	}
}
