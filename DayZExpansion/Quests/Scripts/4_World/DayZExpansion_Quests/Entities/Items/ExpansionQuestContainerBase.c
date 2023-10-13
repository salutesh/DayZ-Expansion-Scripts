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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Quest ID: " + m_Expansion_QuestID);

		super.EECargoOut(item);

		if (m_Expansion_QuestID > -1)
		{
			CheckAssignedObjectives(item);
			ExpansionCheckStorage();
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "Quest ID: " + m_Expansion_QuestID);

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

			int questID = objective.GetQuest().GetQuestConfig().GetID();
			EXTrace.Print(EXTrace.QUESTS, this, "Objective quest ID: " + questID);
			EXTrace.Print(EXTrace.QUESTS, this, "Stash quest ID: " + m_Expansion_QuestID);

			if (questID != m_Expansion_QuestID)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective quest ID mismatch! Skip..");
			#endif
				i++;
				continue;
			}

			if (objective.GetObjectiveType() != ExpansionQuestObjectiveType.TREASUREHUNT)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective type mismatch! Skip..");
			#endif
				i++;
				continue;
			}

			ExpansionQuestObjectiveTreasureHuntEvent treasureHuntEvent;
			if (!Class.CastTo(treasureHuntEvent, objective))
			{
				EXTrace.Print(EXTrace.QUESTS, this, "Could not get Treasure Hunt objective!");
				i++;
				continue;
			}

			if (treasureHuntEvent.GetChest() != this)
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective stash is not this stash! Skip..");
			#endif
				i++;
				continue;
			}

			array<EntityAI> lootItems = treasureHuntEvent.GetLootItems();
			if (lootItems.Find(item) == -1)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "Item is not loot item: " + item);
				if (EXTrace.QUESTS)
				{
					EXPrint(this, "Loot items: " + lootItems.Count());
					foreach (EntityAI lootItem: lootItems)
					{
						EXPrint(this, "Loot item: " + lootItem);
					}

					map<string, int> lootItemsMap = treasureHuntEvent.GetLootItemsMap();
					EXPrint(this, "Loot item counts: " + lootItemsMap.Count());
					foreach (string className, int count: lootItemsMap)
					{
						EXPrint(this, "Loot item count: " + className + " " + count);
					}
				}
				return;
			}

			if (!treasureHuntEvent.HasLootedItemFromChest())
				treasureHuntEvent.LootedItemFromChest();

			return;  //! There may only be one treasure hunt objective assigned per chest
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(this, "::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
	}
	
	override void AfterStoreLoad()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		super.AfterStoreLoad();

		GetGame().ObjectDelete(this);
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
 