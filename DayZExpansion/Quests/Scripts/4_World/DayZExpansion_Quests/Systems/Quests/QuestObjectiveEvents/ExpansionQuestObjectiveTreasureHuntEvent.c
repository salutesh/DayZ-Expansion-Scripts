/**
 * ExpansionQuestObjectiveTreasureHuntEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTreasureHuntEvent: ExpansionQuestObjectiveEventBase
{
	protected ref map<string, int> m_LootItemsMap = new map<string, int>;
	protected ref array<EntityAI> m_LootItems = new array<EntityAI>;
	protected bool m_LootedItemFromChest;
	protected UndergroundStash m_Stash;
	protected ExpansionQuestSeaChest m_Chest;
	protected vector m_StashPos;
	protected bool m_DestinationReached;
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent OnEventStart
	// -----------------------------------------------------------
	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

		if (!super.OnEventStart())
			return false;

		ExpansionQuestObjectiveTreasureHunt treasureHunt = m_ObjectiveConfig.GetTreasureHunt();
		if (!treasureHunt)
			return false;

		vector pos = treasureHunt.GetPositions().GetRandomElement();
		m_StashPos = pos;

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_StashPos);

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent OnEventStart
	// -----------------------------------------------------------
	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		if (!super.OnContinue())
			return false;

		//! Only create the stash trigger when not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			ExpansionQuestObjectiveTreasureHunt treasureHunt = m_ObjectiveConfig.GetTreasureHunt();
			if (!treasureHunt)
				return false;

			if (m_StashPos == vector.Zero)
				return false;

			if (!m_ObjectiveTrigger)
				CreateTrigger(m_StashPos);
		}

		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent OnCleanup
	// -----------------------------------------------------------
	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!super.OnCleanup())
			return false;

		//! Only cleanup the loot if quest is not completed
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			foreach (EntityAI obj: m_LootItems)
			{
				GetGame().ObjectDelete(obj);
			}
		}

		if (m_Chest)
			GetGame().ObjectDelete(m_Chest);

		if (m_Stash)
			GetGame().ObjectDelete(m_Stash);

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint(ToString() + "::OnCleanup - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent OnCancel
	// -----------------------------------------------------------
	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		if (!super.OnCancel())
			return false;

		//! Only cleanup the loot if quest is not completed
		foreach (EntityAI obj: m_LootItems)
		{
			GetGame().ObjectDelete(obj);
		}

		if (m_Chest)
			GetGame().ObjectDelete(m_Chest);

		if (m_Stash)
			GetGame().ObjectDelete(m_Stash);

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint(ToString() + "::OnCancel - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent CreateTreasure
	// -----------------------------------------------------------
	protected void CreateTreasure()
	{
		ObjectivePrint(ToString() + "::CreateTreasure - Start");

		ExpansionQuestObjectiveTreasureHunt treasureHunt = m_ObjectiveConfig.GetTreasureHunt();
		if (!treasureHunt)
			return;

		//! Create the underground stash and hide it
		Object stashObj = GetGame().CreateObjectEx("UndergroundStash", m_StashPos, ECE_PLACE_ON_SURFACE);
		if (!Class.CastTo(m_Stash, stashObj))
			return;

		m_Stash.SetQuestID(m_Quest.GetQuestConfig().GetID());
		m_Stash.PlaceOnGround();

		//! Spawn the chest in the underground stash
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(m_Quest.GetPlayerUID());
		if (!questPlayer)
			return;

		Object chestObj = Spawn("ExpansionQuestSeaChest", 1, questPlayer, m_Stash, m_StashPos, Vector(0, 0, 0));
		if (!Class.CastTo(m_Chest, chestObj))
			return;

		if (!m_Chest)
		{
			GetGame().ObjectDelete(m_Chest);
			return;
		}

		m_Chest.SetQuestID(m_Quest.GetQuestConfig().GetID());

		//! Spawn the loot in the chest
		EntityAI chestEntity;
		if (!Class.CastTo(chestEntity, m_Chest))
			return;

		map<string, int> items = treasureHunt.GetItems();
		foreach (string name, int amount: items)
		{
			Object obj = Spawn(name, amount, questPlayer, chestEntity, m_StashPos, Vector(0, 0, 0));
			EntityAI item;
			if (!Class.CastTo(item, obj))
			{
				GetGame().ObjectDelete(obj);
				return;
			}
			
			int current;
			if (m_LootItemsMap.Find(name, current))
			{
				int newAmount = current + amount;
				m_LootItemsMap.Set(name, newAmount);
			}
			else
			{
				m_LootItemsMap.Insert(name, amount);
			}
			
			m_LootItems.Insert(item);
		}

		ObjectivePrint(ToString() + "::CreateTreasure - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent Spawn
	// -----------------------------------------------------------
	Object Spawn(string name, int amount, PlayerBase player, EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(name, player, parent, amount);
		return obj;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent CreateTrigger
	// -----------------------------------------------------------
	protected void CreateTrigger(vector pos)
	{
		ObjectivePrint(ToString() + "::CreateTrigger - Start");

		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(this);

		ObjectivePrint(ToString() + ":: CreateTrigger - Created objective trigger at position: " + pos + ".");
		ObjectivePrint(ToString() + "::CreateTrigger - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent GetPosition
	// -----------------------------------------------------------
	vector GetPosition()
	{
		return m_StashPos;
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent SetStashPosition
	// -----------------------------------------------------------	
	void SetStashPosition(vector pos)
	{
		m_StashPos = pos;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent SetReachedLocation
	// -----------------------------------------------------------
	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
		ObjectivePrint(ToString() + "::SetReachedLocation - Start");
		ObjectivePrint(ToString() + "::SetReachedLocation - State: " + state);

		if (m_DestinationReached == state)
			return:

		SetLocationState(state);

		if (m_DestinationReached)
		{
			CreateTreasure();
		}
		else
		{
			if (m_Chest)
				GetGame().ObjectDelete(m_Chest);

			if (m_Stash)
				GetGame().ObjectDelete(m_Stash);
		}
		
		m_Quest.UpdateQuest();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::SetReachedLocation - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent SetLocationState
	// -----------------------------------------------------------
	void SetLocationState(bool state)
	{
		m_DestinationReached = state;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent LootedItemFromChest
	// -----------------------------------------------------------
	void LootedItemFromChest()
	{
		ObjectivePrint(ToString() + "::LootedItemFromChest - Start");

		//int amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
		//m_LootedItemsAmount += amount;
		m_LootedItemFromChest = true;

		m_Quest.UpdateQuest();
		m_Quest.QuestCompletionCheck();
		ObjectivePrint(ToString() + "::LootedItemFromChest - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent OnInventoryItemLocationChange
	// -----------------------------------------------------------
	void OnInventoryItemLocationChange(ItemBase item, string state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - State: " + state);
		
		if (m_LootItems.Find(item) == -1)
			return;
		
		int amount;
		if (!m_LootItemsMap.Find(item.GetType(), amount))
			return;
		
		int itemAmount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
		if (itemAmount != amount)
		{
			m_LootedItemFromChest = true;

			m_Quest.UpdateQuest();
			m_Quest.QuestCompletionCheck();
		}
		
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - End");
	}
	
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent HasLootedItemFromChest
	// -----------------------------------------------------------	
	bool HasLootedItemFromChest()
	{
		return m_LootedItemFromChest;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTravelEvent CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_DestinationReached: " + m_DestinationReached);
		ObjectivePrint(ToString() + "::CanComplete - m_LootedItemFromChest: " + m_LootedItemFromChest);

		bool conditionsResult = m_DestinationReached && m_LootedItemFromChest;
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent GetStash
	// -----------------------------------------------------------
	UndergroundStash GetStash()
	{
		return m_Stash;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent GetChest
	// -----------------------------------------------------------
	ExpansionQuestSeaChest GetChest()
	{
		return m_Chest;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent GetLootItems
	// -----------------------------------------------------------
	array<EntityAI> GetLootItems()
	{
		return m_LootItems;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TREASUREHUNT;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveTreasureHuntEvent HasDynamicState
	// -----------------------------------------------------------
	override bool HasDynamicState()
	{
		return true;
	}
};