/**
 * ExpansionQuestObjectiveCraftingEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCraftingEvent: ExpansionQuestObjectiveEventBase
{
	protected ref map<string, int> m_ObjectiveItemsMap;
	protected ref map<string, int> m_ObjectiveInventoryItemsMap;
	protected ref array<EntityAI> m_ObjectiveItems;
	protected int m_ObjectiveItemsAmount = 0;
	protected int m_ObjectiveItemsCount = 0;
	protected int m_UpdateCount = 0;
	protected int m_ObjectiveActionAmount = 0;
	protected int m_ObjectiveActionCount = 0;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent Constructor
	// -----------------------------------------------------------
	void ExpansionQuestObjectiveCraftingEvent(ExpansionQuest quest)
	{
		if (!m_ObjectiveItems)
			m_ObjectiveItems = new array<EntityAI>;

		if (!m_ObjectiveItemsMap)
			m_ObjectiveItemsMap = new map<string, int>;

		if (!m_ObjectiveInventoryItemsMap)
			m_ObjectiveInventoryItemsMap = new map<string, int>;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent OnEventStart
	// -----------------------------------------------------------
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEventStart");
	#endif

		if (!super.OnEventStart())
			return false;

		m_ObjectiveActionAmount = m_ObjectiveConfig.GetExecutionAmount();

		UpdateObjectiveItemsMap();

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		int objEntry = 1;
		foreach (string objectiveItem, int needed: m_ObjectiveItemsMap)
		{
			Print(ToString() + "::OnEventStart - Objective items needed: [" + objEntry + "] " + objectiveItem + " | Needed: " + needed);
			objEntry++;
		}
	#endif

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent OnContinue
	// -----------------------------------------------------------
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		if (!super.OnContinue())
			return false;

		m_ObjectiveActionAmount = m_ObjectiveConfig.GetExecutionAmount();

		UpdateObjectiveItemsMap();

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		int objEntry = 1;
		foreach (string objectiveItem, int needed: m_ObjectiveItemsMap)
		{
			Print(ToString() + "::OnEventStart - Objective items needed: [" + objEntry + "] " + objectiveItem + " | Needed: " + needed);
			objEntry++;
		}
	#endif

		CheckQuestPlayersForObjectiveItems();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent UpdateObjectiveItemsMap
	// -----------------------------------------------------------
	//! Populate objective item map with the data from the objectives deliveries configuration
	protected void UpdateObjectiveItemsMap()
	{
		ObjectivePrint(ToString() + "::UpdateObjectiveItemsMap - Start");

		if (!m_ObjectiveConfig || !m_ObjectiveConfig.GetItemNames() || m_ObjectiveConfig.GetItemNames().Count() == 0)
			return;

		array<string> itemNames = m_ObjectiveConfig.GetItemNames();
		foreach (string name: itemNames)
		{
			m_ObjectiveItemsAmount += 1;

			int current;
			if (m_ObjectiveItemsMap.Find(name, current))
			{
				int newAmount = current + 1;
				m_ObjectiveItemsMap.Set(name, newAmount);
			}
			else
			{
				m_ObjectiveItemsMap.Insert(name, 1);
			}
		}

		if (!m_ObjectiveItemsMap || m_ObjectiveItemsMap.Count() == 0)
		{
			Error(ToString() + "::UpdateObjectiveItemsMap - There are no deliveries defined for the delivery objective with ID " + m_ObjectiveConfig.GetID() + ". Cancel quest..");
			m_Quest.CancelQuest();
		}

		ObjectivePrint(ToString() + "::UpdateObjectiveItemsMap - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent CheckQuestPlayersForObjectiveItems
	// -----------------------------------------------------------
	protected void CheckQuestPlayersForObjectiveItems()
	{
		ObjectivePrint(ToString() + "::CheckQuestPlayersForObjectiveItems - Start");

		PlayerBase player;
		ItemBase itemIB;
		array<EntityAI> items;
		int amount;
		int needed;
		int current;
		int newAmount;
		string typeName;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			player = PlayerBase.GetPlayerByUID(m_Quest.GetPlayerUID());
			if (!player)
				return;

			items = new array<EntityAI>;
	  	 	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
			foreach (EntityAI item: items)
			{
				if (!Class.CastTo(itemIB, item))
					continue;

				typeName = itemIB.GetType();
				if (!m_ObjectiveItemsMap.Find(typeName, needed))
					continue;

				amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(itemIB);
				if (!m_ObjectiveInventoryItemsMap.Find(typeName, current))
				{
					if (amount <= needed)
					{
						m_ObjectiveItemsCount += amount;
						m_ObjectiveInventoryItemsMap.Insert(typeName, amount);
						m_ObjectiveItems.Insert(item);
					}
				}
				else
				{
					newAmount = current + amount;
					if (newAmount <= needed)
					{
						m_ObjectiveItemsCount += amount;
						m_ObjectiveInventoryItemsMap.Set(typeName, newAmount);
						m_ObjectiveItems.Insert(item);
					}
				}
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				player = PlayerBase.GetPlayerByUID(playerUID);
				if (!player)
					continue;

				items = new array<EntityAI>;
		  	 	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
				foreach (EntityAI itemM: items)
				{
					if (!Class.CastTo(itemIB, itemM))
						continue;

					typeName = itemIB.GetType();
					if (!m_ObjectiveItemsMap.Find(typeName, needed))
						continue;

					amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(itemM);
					if (!m_ObjectiveInventoryItemsMap.Find(typeName, current))
					{
						if (amount <= needed)
						{
							m_ObjectiveItemsCount += amount;
							m_ObjectiveInventoryItemsMap.Insert(typeName, amount);
							m_ObjectiveItems.Insert(itemM);
						}
					}
					else
					{
						newAmount = current + amount;
						if (newAmount <= needed)
						{
							m_ObjectiveItemsCount += amount;
							m_ObjectiveInventoryItemsMap.Set(typeName, newAmount);
							m_ObjectiveItems.Insert(itemM);
						}
					}
				}
			}
		}
	#endif

		ObjectivePrint(ToString() + "::CheckQuestPlayersForObjectiveItems - m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint(ToString() + "::CheckQuestPlayersForObjectiveItems - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent OnTurnIn
	// -----------------------------------------------------------
	override bool OnTurnIn(string playerUID)
	{
		ObjectivePrint(ToString() + "::OnTurnIn - Start");

		if (!super.OnTurnIn(playerUID))
			return false;

		DeleteObjectiveItems();

		ObjectivePrint(ToString() + "::OnTurnIn - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase DeleteObjectiveItems
	// -----------------------------------------------------------
	protected void DeleteObjectiveItems()
	{
		ObjectivePrint(ToString() + "::DeleteObjectiveItems - Start");

		foreach (EntityAI item: m_ObjectiveItems)
		{
			ObjectivePrint(ToString() + "::DeleteObjectiveItems - Delete ojective item " + item.GetType());
			GetGame().ObjectDelete(item);
		}

		ObjectivePrint(ToString() + "::DeleteObjectiveItems - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent OnObjectiveActionExecuted
	// -----------------------------------------------------------
	void OnObjectiveActionExecuted(PlayerBase player, array<ItemBase> spawned_objects)
	{
		ObjectivePrint(ToString() + "::OnObjectiveActionExecuted - Start");

		if (m_ObjectiveActionCount == m_ObjectiveActionAmount)
			return;

		foreach (ItemBase craftedItem: spawned_objects)
		{
			string typeName = craftedItem.GetType();
			int amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(craftedItem);
			int needed;
			if (m_ObjectiveItemsMap.Find(typeName, needed))
			{
				if (amount <= needed)
				{
					m_ObjectiveItemsCount += amount;
					m_ObjectiveActionCount += 1;
					m_ObjectiveItems.Insert(craftedItem);
				}
			}
		}

		UpdateObjective();

		ObjectivePrint(ToString() + "::OnObjectiveActionExecuted - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent OnInventoryItemLocationChange
	// -----------------------------------------------------------
	void OnInventoryItemLocationChange(ItemBase item, Man player, string state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - State: " + state);

		string typeName = item.GetType();
		int amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
		int needed;
		int index = -1;

		if (!m_ObjectiveItemsMap.Find(typeName, needed))
			return;

		if (state == "INV_EXIT")
		{
			index = m_ObjectiveItems.Find(item);
			if (index > -1)
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Remove " + item.GetType() + " from objective items. Item amount: " + amount);
				m_ObjectiveItemsCount = m_ObjectiveItemsCount - amount;
				m_ObjectiveItems.Remove(index);
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items count: " + m_ObjectiveItemsCount);
			}
			else
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item " + item.GetType() + " is not in objective items. Skip..");
			}
		}
		else if (state == "INV_ENTER")
		{
			index = m_ObjectiveItems.Find(item);
			if (index == -1)
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item " + item.GetType() + " is not in objective items. Check item..");
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Check m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Check m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

				if (m_ObjectiveItemsCount < m_ObjectiveItemsAmount)
				{
					ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Add " + item.GetType() + " to objective items. Item amount: " + amount);
					m_ObjectiveItemsCount += amount;
					m_ObjectiveItems.Insert(item);
					ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items count: " + m_ObjectiveItemsCount);
				}
			}
			else
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item " + item.GetType() + " is already in objective items.");
			}
		}

		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Should update? m_UpdateCount: " + m_UpdateCount);
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Should update? m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);

		if (m_UpdateCount != m_ObjectiveItemsCount)
		{
			ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Update delivery and quest-data for quest players..");
			ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items count: " + m_ObjectiveItemsCount);
			m_UpdateCount = m_ObjectiveItemsCount;
			UpdateObjective();
		}

		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent UpdateObjectivesInventoryItemsMap
	// -----------------------------------------------------------
	//! Populate objective item inventory map with the data from the given items in m_ObjectiveItems if there are any.
	protected void UpdateObjectivesInventoryItemsMap()
	{
		ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - Start");

		if (!m_ObjectiveItems)
		{
			ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - No objective item entities collected yet!");
		}

		m_ObjectiveInventoryItemsMap.Clear();

		//! Add a entry for each item from the m_ObjectiveItemsMap to m_ObjectiveInventoryItemsMap first.
		foreach (string deliveryName, int deliveryAmount: m_ObjectiveItemsMap)
		{
			m_ObjectiveInventoryItemsMap.Insert(deliveryName, 0);
		}

		foreach (EntityAI item: m_ObjectiveItems)
		{
			string typeName = item.GetType();
			int deliveryItemCount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
			int current;
			int needed;

			if (!m_ObjectiveItemsMap.Find(typeName, needed))
			{
				ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - Item " + typeName + " is not in objective items map! Skip..");
				return;
			}

			if (m_ObjectiveInventoryItemsMap.Find(typeName, current))
			{
				int newAmount = current + deliveryItemCount;
				if (newAmount <= needed)
				{
					ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - Add item to inventory items map: " + typeName);
					ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - Needed amount: " + needed);
					ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - Current collected amount: " + current);
					ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - New collected amount: " + newAmount);
					m_ObjectiveInventoryItemsMap.Set(typeName, newAmount);
				}
			}
		}

		ObjectivePrint(ToString() + "::UpdateObjectivesInventoryItemsMap - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent UpdateObjective
	// -----------------------------------------------------------
	protected void UpdateObjective()
	{
		ObjectivePrint(ToString() + "::UpdateObjective - Start");

		UpdateObjectivesInventoryItemsMap();
		m_Quest.UpdateQuest();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::UpdateObjective - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		bool conditionsResult = (m_ObjectiveItemsCount == m_ObjectiveItemsAmount) && GetCraftingState();
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent GetCraftingState
	// -----------------------------------------------------------
	bool GetCraftingState()
	{
		return (m_ObjectiveActionCount <= m_ObjectiveActionAmount);
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent GetObjectiveItemsAmount
	// -----------------------------------------------------------
	int GetObjectiveItemsAmount()
	{
		return m_ObjectiveItemsAmount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent GetObjectiveItemsCount
	// -----------------------------------------------------------
	int GetObjectiveItemsCount()
	{
		return m_ObjectiveItemsCount;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent HasDynamicState
	// -----------------------------------------------------------
	override bool HasDynamicState()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCraftingEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.CRAFTING;
	}
};