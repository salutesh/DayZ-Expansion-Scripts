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

class ExpansionMagazine
{
	Magazine Mag;
	int Count;

	void ExpansionMagazine(Magazine mag)
	{
		Mag = mag;
		Count = mag.GetAmmoCount();
	}
}

modded class PlayerBase
{
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	protected ref array<ref ExpansionMagazine> m_Expansion_AmmoInInventoryQuantity = {};

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, PlayerBase);

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
		auto trace = EXTrace.Start(EXTrace.QUESTS, PlayerBase);

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
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		
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
			
			//! Check if the current objective belongs to the quest player
			if (!quest.IsQuestPlayer(killerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Player with UID [" + killerUID + "] is not a quest player of this quest objective. Skip...");
			#endif
				i++;
				continue;
			}

			switch (objective.GetObjectiveType())
			{
				case ExpansionQuestObjectiveType.TARGET:
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AICAMP:
				case ExpansionQuestObjectiveType.AIPATROL:
				case ExpansionQuestObjectiveType.AIESCORT:
			#endif
					objective.OnEntityKilled(this, killSource, killerPlayer);
					break;
			}

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(this, "::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		ExpansionQuestModule.GetModuleInstance().CheckActivePlayerQuests(this);
		CheckAssignedObjectivesForEntity(killer);
	}

	override void SetActions(out TInputActionMap InputActionMap)
	{
		super.SetActions(InputActionMap);

		AddAction(ExpansionActionOpenQuestMenu, InputActionMap);
	}

	void Expansion_RememberAmmoInInventoryQuantity()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_Expansion_AmmoInInventoryQuantity.Clear();

		foreach (string type, ExpansionInventoryItemType itemType: m_Expansion_InventoryItemTypes)
		{
			if (ExpansionStatic.Is(type, Ammunition_Base))
			{
				foreach (ItemBase item: itemType.Items)
				{
					m_Expansion_AmmoInInventoryQuantity.Insert(new ExpansionMagazine(Magazine.Cast(item)));
				}
			}
		}
	}

	void Expansion_CheckAmmoInInventoryQuantityChanged()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		foreach (ExpansionMagazine ammo: m_Expansion_AmmoInInventoryQuantity)
		{
			if (ammo.Mag && ammo.Mag.GetAmmoCount() != ammo.Count)
			{
				ammo.Mag.Expansion_OnStackSizeChanged();
			}
		}

		m_Expansion_AmmoInInventoryQuantity.Clear();
	}
};
