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
		return m_ExpansionCanReceiveItems;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return m_ExpansionCanReceiveItems;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	void ExpansionSetCanReceiveItems(bool state)
	{
		m_ExpansionCanReceiveItems = state;
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

	override bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		return false;
	}

	override void EECargoOut(EntityAI item)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		super.EECargoOut(item);

		if (m_Expansion_QuestID > -1)
		{
			CheckAssignedObjectives(item);
			ExpansionCheckStorage();
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (m_Expansion_QuestID > -1)
		{
			CheckAssignedObjectives(item);
			ExpansionCheckStorage();
		}
	}

	protected void CheckAssignedObjectives(EntityAI item)
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

		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			if (!objective.GetQuest())
				continue;

			int questID = objective.GetQuest().GetQuestConfig().GetID();
			EXTrace.Print(EXTrace.QUESTS, this, "Objective quest ID: " + questID);
			EXTrace.Print(EXTrace.QUESTS, this, "Stash quest ID: " + m_Expansion_QuestID);

			if (questID != m_Expansion_QuestID)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective quest ID mismatch! Skip..");
			#endif
				continue;
			}

			if (objective.GetObjectiveType() != ExpansionQuestObjectiveType.TREASUREHUNT)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective type mismatch! Skip..");
			#endif
				continue;
			}

			ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
			if (!Class.CastTo(treasureHuntEvent, objective))
			{
				EXTrace.Print(EXTrace.QUESTS, this, "Could not get Treasure Hunt objective!");
				continue;
			}

			ExpansionQuestContainerBase objectiveChest = ExpansionQuestContainerBase.Cast(this);
			if (treasureHuntEvent.GetChest() != objectiveChest)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective stash is not this stash! Skip..");
			#endif
				continue;
			}

			array<EntityAI> lootItems = treasureHuntEvent.GetLootItems();
			if (lootItems.Find(item) == -1)
				return;

			if (!treasureHuntEvent.HasLootedItemFromChest())
				treasureHuntEvent.LootedItemFromChest();
		}
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
 