/**
 * ExpansionQuestContainerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestContainerBase: Container_Base
{
	protected bool m_ExpansionCanReceiveItems;
	protected bool m_ExpansionStashDelete;

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase Constructor
	// ------------------------------------------------------------
	void ExpansionQuestContainerBase()
	{
		if (IsMissionHost())  //! Server or COM
		{
			SetAllowDamage(false);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase Destructor
	// ------------------------------------------------------------	
	void ~ExpansionQuestContainerBase()
	{
		if (m_ExpansionStashDelete)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionDeleteStorage);
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo(EntityAI parent)
	{
		if (parent.IsInherited(UndergroundStash))
			return true;
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase CanReceiveAttachment
	// ------------------------------------------------------------
	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase CanReceiveItemIntoCargo
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase CanSwapItemInCargo
	// ------------------------------------------------------------
	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase ExpansionCheckStorage
	// ------------------------------------------------------------
	void ExpansionCheckStorage()
	{
		if (!IsInStash() && !m_ExpansionStashDelete)
			ExpansionDeleteStashAfterCooldown();

		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase ExpansionDeleteStashAfterCooldown
	// ------------------------------------------------------------
	protected void ExpansionDeleteStashAfterCooldown()
	{
		m_ExpansionStashDelete = true;
		if (IsMissionHost())  //! Server or COM
		{
			//! Delete after 20 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase IsInStash
	// ------------------------------------------------------------
	bool IsInStash()
	{
		UndergroundStash stash;
		if (Class.CastTo(stash, GetParent()))
			return true;

		return false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestContainerBase ExpansionDeleteStorage
	// ------------------------------------------------------------
	protected void ExpansionDeleteStorage()
	{
		GetGame().ObjectDelete(this);
		if (m_ExpansionStashDelete)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionDeleteStorage);
	}
	
	// ------------------------------------------------------------
	// UndergroundStash OnInventoryExit
	// ------------------------------------------------------------	
	override void EECargoOut(EntityAI item)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::EECargoOut - Start");
	#endif
		
		super.EECargoOut(item);
		
		if (m_Expansion_QuestID > -1)
		{
			CheckAssignedObjectives(item);
			ExpansionCheckStorage();
		}
		
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::EECargoOut - End");
	#endif
	}
	
	// ------------------------------------------------------------
	// UndergroundStash CheckAssignedObjectives
	// ------------------------------------------------------------
	protected void CheckAssignedObjectives(EntityAI item)
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

		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			if (!objective || !objective.GetQuest())
				continue;
			
			int questID = objective.GetQuest().GetQuestConfig().GetID();
			Print(ToString() + "::CheckAssignedObjectives - Objective quest ID: " + questID);
			Print(ToString() + "::CheckAssignedObjectives - Stash quest ID: " + m_Expansion_QuestID);
			
			if (questID != m_Expansion_QuestID)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectives - Objective quest ID mismatch! Skip..");
			#endif
				continue;
			}
			
			if (objective.GetObjectiveType() != ExpansionQuestObjectiveType.TREASUREHUNT)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectives - Objective type mismatch! Skip..");
			#endif
				continue;
			}
			
			ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
			if (!Class.CastTo(treasureHuntEvent, objective))
			{
				Error(ToString() + "::CheckAssignedObjectives - Could not get Treasure Hunt objective!");
				continue;
			}
			
			ExpansionQuestSeaChest objectiveChest = ExpansionQuestSeaChest.Cast(this);
			if (treasureHuntEvent.GetChest() != objectiveChest)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectives - Objective stash is not this stash! Skip..");
			#endif
				continue;
			}
			
			array<EntityAI> lootItems = treasureHuntEvent.GetLootItems();
			if (lootItems.Find(item) == -1)
				return;
			
			if (!treasureHuntEvent.HasLootedItemFromChest())
				treasureHuntEvent.LootedItemFromChest();
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectives - End");
	#endif
	}
};

class ExpansionQuestSeaChest: ExpansionQuestContainerBase {};