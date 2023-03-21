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

	void ExpansionQuestContainerBase()
	{
		if (IsMissionHost())  //! Server or COM
		{
			SetAllowDamage(false);
		}
	}

	void ~ExpansionQuestContainerBase()
	{
		if (m_ExpansionStashDelete)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionDeleteStorage);
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		if (!super.CanPutInCargo(parent))
			return false;

		if (parent.IsInherited(UndergroundStash))
			return true;

		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	void ExpansionCheckStorage()
	{
		if (!IsInStash() && !m_ExpansionStashDelete)
			ExpansionDeleteStashAfterCooldown();

		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	protected void ExpansionDeleteStashAfterCooldown()
	{
		m_ExpansionStashDelete = true;
		if (IsMissionHost())  //! Server or COM
		{
			//! Delete after 20 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}
	}

	bool IsInStash()
	{
		UndergroundStash stash;
		if (Class.CastTo(stash, GetParent()))
			return true;

		return false;
	}

	protected void ExpansionDeleteStorage()
	{
		GetGame().ObjectDelete(this);
		if (m_ExpansionStashDelete)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionDeleteStorage);
	}

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

			ExpansionQuestContainerBase objectiveChest = ExpansionQuestContainerBase.Cast(this);
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
class ExpansionQuestDryBag_ColorBase: ExpansionQuestContainerBase {};
class ExpansionQuestDryBag_Orange: ExpansionQuestDryBag_ColorBase {};
class ExpansionQuestDryBag_Yellow: ExpansionQuestDryBag_ColorBase {};
class ExpansionQuestDryBag_Blue: ExpansionQuestDryBag_ColorBase {};
class ExpansionQuestDryBag_Green: ExpansionQuestDryBag_ColorBase {};
class ExpansionQuestDryBag_Black: ExpansionQuestDryBag_ColorBase {};
class ExpansionQuestDryBag_Red: ExpansionQuestDryBag_ColorBase {};
 