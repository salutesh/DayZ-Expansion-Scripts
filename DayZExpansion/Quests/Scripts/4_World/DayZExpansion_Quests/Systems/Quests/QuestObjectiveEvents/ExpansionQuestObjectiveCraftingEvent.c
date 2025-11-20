/**
 * ExpansionQuestObjectiveCraftingEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCraftingEvent: ExpansionQuestObjectiveEventBase
{
	protected ref map<string, int> m_ObjectiveItemsMap;
	protected ref map<string, int> m_ObjectiveInventoryItemsMap;
	protected ref array<ItemBase> m_ObjectiveItems;
	protected int m_ObjectiveItemsAmount;
	protected int m_ObjectiveItemsCount;
	protected int m_ObjectiveActionAmount;
	protected int m_ObjectiveActionCount;
	protected ref ExpansionQuestObjectiveCraftingConfig m_CraftingConfig;

	void ExpansionQuestObjectiveCraftingEvent(ExpansionQuest quest)
	{
		m_ObjectiveItems = new array<ItemBase>;
		m_ObjectiveItemsMap = new map<string, int>;
		m_ObjectiveInventoryItemsMap = new map<string, int>;
	}

	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_CraftingConfig, m_ObjectiveConfig))
			return false;

		UpdateObjectiveItemsMap();

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		int objEntry = 1;
		foreach (string objectiveItem, int needed: m_ObjectiveItemsMap)
		{
			ObjectivePrint("Objective items needed: [" + objEntry + "] " + objectiveItem + " | Needed: " + needed);
			objEntry++;
		}
	#endif

		if (!super.OnEventStart())
			return false;

		return true;
	}

	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!Class.CastTo(m_CraftingConfig, m_ObjectiveConfig))
			return false;

		UpdateObjectiveItemsMap();

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		int objEntry = 1;
		foreach (string objectiveItem, int needed: m_ObjectiveItemsMap)
		{
			ObjectivePrint("Objective items needed: [" + objEntry + "] " + objectiveItem + " | Needed: " + needed);
			objEntry++;
		}
	#endif

		CheckQuestPlayersForObjectiveItems();
		m_Quest.QuestCompletionCheck(true);

		if (!super.OnContinue())
			return false;

		return true;
	}

	override bool OnCancel()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCancel())
			return false;

		CheckQuestPlayersForObjectiveItems();
		DeleteObjectiveItems();

		return true;
	}

	//! Populate objective item map with the data from the objectives deliveries configuration
	protected void UpdateObjectiveItemsMap()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_CraftingConfig || !m_CraftingConfig.GetItemNames() || m_CraftingConfig.GetItemNames().Count() == 0)
			return;

		array<string> itemNames = m_CraftingConfig.GetItemNames();
		foreach (string name: itemNames)
		{
			m_ObjectiveItemsAmount += 1;

			name.ToLower();
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
			Error(ToString() + "::UpdateObjectiveItemsMap - There are no deliveries defined for the delivery objective with ID " + m_CraftingConfig.GetID() + ". Cancel quest..");
			m_Quest.CancelQuest();
		}
	}

	protected void CheckQuestPlayersForObjectiveItems()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ObjectiveItems.Clear();
		m_ObjectiveItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			CheckPlayerForObjectiveItems(m_Quest.GetPlayer());
		}
		else
		{
			set<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				if (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
					break;

				CheckPlayerForObjectiveItems(playerUID);
			}
		}

		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
	}

	protected void CheckPlayerForObjectiveItems(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);

		CheckPlayerForObjectiveItems(player);
	}

	protected void CheckPlayerForObjectiveItems(PlayerBase player)
	{
		if (!player)
			return;

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, player.GetIdentity().GetId());
	#endif

		foreach (string typeNameLower, int needed: m_ObjectiveItemsMap)
		{
			EXTrace.Print(EXTrace.QUESTS, this, typeNameLower + " needed: " + needed);
			ExpansionInventoryItemType itemType = player.Expansion_GetInventoryItemType(typeNameLower);
			if (itemType)
			{
				foreach (ItemBase item: itemType.Items)
				{
					if (item.IsRuined())
						continue;

					int current = m_ObjectiveInventoryItemsMap[typeNameLower];
					EXTrace.Print(EXTrace.QUESTS, this, typeNameLower + " current: " + current);
					if (current >= needed)
						break;

					int amount = item.Expansion_GetStackAmount();
					EXTrace.Print(EXTrace.QUESTS, this, "add: " + amount);
					m_ObjectiveInventoryItemsMap[typeNameLower] = current + amount;
					m_ObjectiveItems.Insert(item);
					m_ObjectiveItemsCount += amount;
				}
			}
		}
	}

	override bool OnTurnIn(string playerUID, int selectedObjItemIndex = -1)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		DeleteObjectiveItems();

		if (!super.OnTurnIn(playerUID, selectedObjItemIndex))
			return false;

		return true;
	}

	protected void DeleteObjectiveItems()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		foreach (EntityAI item: m_ObjectiveItems)
		{
			ObjectivePrint("Delete ojective item " + item.GetType());
			g_Game.ObjectDelete(item);
		}
	}

	void OnObjectiveActionExecuted(PlayerBase player, array<ItemBase> spawned_objects)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (m_ObjectiveActionCount == m_ObjectiveActionAmount)
			return;

		foreach (ItemBase craftedItem: spawned_objects)
		{
			string typeNameLower = craftedItem.Expansion_GetTypeLower();
			int amount = craftedItem.Expansion_GetStackAmount();
			int needed;
			if (m_ObjectiveItemsMap.Find(typeNameLower, needed))
			{
				if (m_ObjectiveActionCount < m_ObjectiveActionAmount)
				{
					//! Set quest ID on item as it is now a quest item?!
					//craftedItem.SetQuestID(m_Quest.GetQuestConfig().GetID());
					m_ObjectiveActionCount++;
				}
			}
		}

		m_Quest.QuestCompletionCheck(true);
	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		string typeNameLower = item.Expansion_GetTypeLower();

		if (!m_ObjectiveItemsMap.Contains(typeNameLower))
			return;

		ObjectivePrint("State: " + typename.EnumToString(ExpansionQuestItemState, state));

		int amount = item.Expansion_GetStackAmount();
		bool update;
		switch (state)
		{
		case ExpansionQuestItemState.INV_EXIT:
			int index = m_ObjectiveItems.Find(item);
			if (index > -1)
			{
				ObjectivePrint("Remove " + item.GetType() + " from objective items. Item amount: " + amount);
				m_ObjectiveItemsCount -= amount;
				m_ObjectiveItems.Remove(index);
				ObjectivePrint("Objective items count: " + m_ObjectiveItemsCount);
				update = true;
			}
			else
			{
				ObjectivePrint("Item " + item.GetType() + " is not in objective items. Skip..");
			}
			break;
		case ExpansionQuestItemState.INV_ENTER:
			ObjectivePrint("Item " + item.GetType() + " is not in objective items. Check item...");
			ObjectivePrint("Check m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
			ObjectivePrint("Check m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

			if (m_ObjectiveItemsCount < m_ObjectiveItemsAmount && CanAddObjectiveItem(item))
			{
				ObjectivePrint("Add " + item.GetType() + " to objective items. Item amount: " + amount);
				m_ObjectiveItemsCount += amount;
				m_ObjectiveItems.Insert(item);
				ObjectivePrint("Objective items count: " + m_ObjectiveItemsCount);
				update = true;
			}
			break;
		}

		ObjectivePrint("Should update? " + update);
		ObjectivePrint("Should update? m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);

		if (update)
		{
			ObjectivePrint("Update delivery and quest-data for quest players..");
			ObjectivePrint("Objective items count: " + m_ObjectiveItemsCount);
			m_Quest.QuestCompletionCheck(true);
		}
	}

	protected bool CanAddObjectiveItem(ItemBase item)
	{
		if (item.IsRuined())
			return false;

		return true;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint("m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		if (m_ObjectiveItemsAmount == 0 || m_ObjectiveItemsCount != m_ObjectiveItemsAmount)
			return false;

		return GetCraftingState();
	}

	void SetExecutionAmount(int amount)
	{
		m_ObjectiveActionAmount = amount;
	}

	bool GetCraftingState()
	{
		return (m_ObjectiveActionCount <= m_ObjectiveActionAmount);
	}

	int GetObjectiveItemsAmount()
	{
		return m_ObjectiveItemsAmount;
	}

	int GetObjectiveItemsCount()
	{
		return m_ObjectiveItemsCount;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.CRAFTING;
	}
};
