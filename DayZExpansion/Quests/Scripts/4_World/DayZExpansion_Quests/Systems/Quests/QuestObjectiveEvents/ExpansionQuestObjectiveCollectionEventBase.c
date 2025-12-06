/**
 * ExpansionQuestObjectiveCollectionEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveItemCount
{
	int Needed;
	int RemainingNeeded;

	void ExpansionQuestObjectiveItemCount(int needed)
	{
		Needed = needed;
		RemainingNeeded = needed;
	}
}

class ExpansionQuestObjectiveCollectionEventBase: ExpansionQuestObjectiveEventBase
{
#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
	protected int s_CallCount;
#endif
	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected ref map<string, ref ExpansionQuestObjectiveItemCount> m_ObjectiveItemsMap;
	protected ref array<ref ExpansionQuestObjectiveItem> m_ObjectiveItems;
	protected ref map<string, int> m_CollectionsQuantityMap;
	protected int m_ObjectiveItemsAmount;
	protected int m_ObjectiveItemsCount;
	protected ref ExpansionQuestObjectiveDeliveryConfigBase m_DeliveryConfig;

	void ExpansionQuestObjectiveCollectionEventBase(ExpansionQuest quest)
	{
		m_ObjectiveItemsMap = new map<string, ref ExpansionQuestObjectiveItemCount>;
		m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		m_ObjectiveItems = new array<ref ExpansionQuestObjectiveItem>;
		m_CollectionsQuantityMap = new map<string, int>;
	}

	void ~ExpansionQuestObjectiveCollectionEventBase()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ObjectiveItemsMap.Clear();
		m_DeliveryData.Clear();
		m_ObjectiveItems.Clear();
		m_CollectionsQuantityMap.Clear();
	}

	override bool OnTurnIn(string playerUID, int selectedObjItemIndex = -1)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!CleanupObjectiveItems(selectedObjItemIndex))
			return false;

	#ifdef EXPANSIONMODMARKET
		if (m_DeliveryConfig.AddItemsToNearbyMarketZone())
			AddItemsToMarketZone(playerUID);
	#endif

		if (!super.OnTurnIn(playerUID, selectedObjItemIndex))
			return false;

		return true;
	}

	protected bool CleanupObjectiveItems(int selectedObjItemIndex = -1)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("Selected objective item index: " + selectedObjItemIndex);

		int all = 0;
		int allCollection = 0;
		array<ref ExpansionQuestObjectiveItem> changedItems = new array<ref ExpansionQuestObjectiveItem>;

		foreach (string typeNameLower, ExpansionQuestObjectiveItemCount count: m_ObjectiveItemsMap)
		{
			int needed = count.Needed;

			if (m_DeliveryConfig.NeedAnyCollection())
			{
				if (selectedObjItemIndex == -1)
					continue;

				ExpansionQuestObjectiveDelivery delivery = m_DeliveryConfig.GetCollections().Get(selectedObjItemIndex);
				if (!delivery)
					continue;

				if (delivery.GetClassNameLower() != typeNameLower)
					continue;
			}

			ObjectivePrint("Check objective items for item " + typeNameLower + " | Needed: " + needed);
			int remainingNeeded = needed;
			int remove = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
			    if (objItem.GetClassNameLower() == typeNameLower)
			    {
			        int amount = objItem.GetItem().Expansion_GetStackAmount();
			        int remaining;
			        ObjectivePrint("Check objective item " + typeNameLower + " | Amount: " + amount);
			        if (amount <= remainingNeeded)
			        {
			            remainingNeeded -= amount;
			            ObjectivePrint("Item amount is lower or exacly the needed amount for item " + typeNameLower + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: 0");
			            objItem.SetRemaining(0);
			            remove += amount;
			        }
			        else
			        {
			            remaining = amount - remainingNeeded;
			            ObjectivePrint("Item amount is higher then the needed amount for item " + typeNameLower + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: " + remaining);
			            objItem.SetRemaining(remaining);
			            remove += remainingNeeded;
						remainingNeeded = 0;
			        }
			        changedItems.Insert(objItem);
			    }

			    if (remainingNeeded == 0)
			        break;
			}

			if (!m_DeliveryConfig.NeedAnyCollection() && remove < needed)
			{
				ObjectivePrint("E1 - Could not get all objective items! Needed: " + needed + " | Found to remove: " + remove);
				changedItems.Clear();
				return false;
			}

			all += remove;
			allCollection = needed;

			if (m_DeliveryConfig.NeedAnyCollection() && remove >= needed)
				break;

			if (!m_DeliveryConfig.NeedAnyCollection() && all >= m_ObjectiveItemsAmount)
				break;
		}

		ObjectivePrint("Need any collection: " + m_DeliveryConfig.NeedAnyCollection().ToString());

		if (!m_DeliveryConfig.NeedAnyCollection() && all < m_ObjectiveItemsAmount)
		{
			ObjectivePrint("E2 - Could not get all objective items! All needed: " + m_ObjectiveItemsAmount + " | Found to remove: " + all);
			changedItems.Clear();
			return false;
		}
		else if (m_DeliveryConfig.NeedAnyCollection() && (all < allCollection || selectedObjItemIndex == -1))
		{
			ObjectivePrint("E2 - Could not get all collection objective items! All needed: " + allCollection + " | Found to remove: " + all);
			changedItems.Clear();
			return false;
		}

		foreach (ExpansionQuestObjectiveItem itemtoDelete: changedItems)
		{
			itemtoDelete.DestroyItem();
		}

		return true;
	}

	array<ref ExpansionQuestDeliveryObjectiveData> GetDeliveryData()
	{
		return m_DeliveryData;
	}

	protected bool GetObjectiveDataFromConfig()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ObjectiveItemsMap.Clear();
		m_CollectionsQuantityMap.Clear();

		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_DeliveryConfig.GetCollections();
		if (!objectiveDeliveries || objectiveDeliveries.Count() == 0)
		{
			int objectiveID = m_DeliveryConfig.GetID();
			Error(ToString() + "::GetObjectiveDataFromConfig - Collection objective with ID " + objectiveID + " has no collections defined!");
			return false;
		}

		foreach (ExpansionQuestObjectiveDelivery objectiveDelivery: objectiveDeliveries)
		{
			string typeNameLower = objectiveDelivery.GetClassNameLower();
			int amount = objectiveDelivery.GetAmount();
			ObjectivePrint("Add collection data for type name: " + typeNameLower + " | Amount: " + amount);

			m_ObjectiveItemsAmount += amount;

			ExpansionQuestObjectiveItemCount current;
			if (!m_ObjectiveItemsMap.Find(typeNameLower, current))
			{
				m_ObjectiveItemsMap.Insert(typeNameLower, new ExpansionQuestObjectiveItemCount(amount));
				ObjectivePrint("Added collection data for type name: " + typeNameLower + " | Amount: " + amount + " to objective items map.");
			}
			else
			{
				current.Needed += amount;
			}
			
			int quantityNeeded = objectiveDelivery.GetMinQuantity();
			if (quantityNeeded > 0 && !m_CollectionsQuantityMap.Contains(typeNameLower))
			{
				m_CollectionsQuantityMap.Insert(typeNameLower, quantityNeeded);
			}
		}

		ObjectivePrint("End and return TRUE");

		return true;
	}

	protected bool CheckQuestPlayersForObjectiveItems(bool continues = false)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_ObjectiveItems.Clear();
		m_ObjectiveItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			if (!CheckPlayerForObjectiveItems(m_Quest.GetPlayer()))
				return false;
		}
		else
		{
			set<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				if (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
					break;

				if (!CheckPlayerForObjectiveItems(playerUID))
					return false;
			}
		}

		ObjectivePrint("End and return TRUE");

		return true;
	}

	protected bool CheckPlayerForObjectiveItems(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);

		return CheckPlayerForObjectiveItems(player);
	}

	protected bool CheckPlayerForObjectiveItems(PlayerBase player)
	{
		if (!player)
			return false;

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		foreach (string typeNameLower, ExpansionQuestObjectiveItemCount count: m_ObjectiveItemsMap)
		{
			count.RemainingNeeded = count.Needed;  //! Reset

			EXTrace.Print(EXTrace.QUESTS, this, typeNameLower + " needed: " + count.Needed);
			ExpansionInventoryItemType itemType = player.Expansion_GetInventoryItemType(typeNameLower);
			if (itemType)
			{
				foreach (ItemBase item: itemType.Items)
				{
					if (!CanAddObjectiveItem(item))
						continue;

					AddObjectiveItem(item, count);

					if (count.RemainingNeeded == 0)
                    	break;
				}
			}

			if (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
				break;
		}

		return true;
	}

	protected bool CanAddObjectiveItem(ItemBase item)
	{
		if (!CanAddItem(item))
			return false;

		if (item.HasQuantity())
		{
			string typeNameLower = item.Expansion_GetTypeLower();
			int quantityNeeded = m_CollectionsQuantityMap[typeNameLower];
			if (quantityNeeded > 0)
			{
				ExpansionItemQuantityType quantityType;
				float quantity = item.Expansion_GetQuantity(quantityType);
				EXTrace.Print(EXTrace.QUESTS, this, typeNameLower + "= quantityNeeded: " + quantityNeeded + " | quantity: " + quantity + " | quantityType: " + typename.EnumToString(ExpansionItemQuantityType, quantityType));
				if (quantityType >= ExpansionItemQuantityType.PERCENTAGE && quantityType <= ExpansionItemQuantityType.POWER && quantity < quantityNeeded)
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::CanAddObjectiveItem - Can't add item, quantity to low!");
					return false;
				}
			}
		}

		switch (m_DeliveryConfig.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.COLLECT:
				return (!item.Expansion_IsQuestItem() || item.Expansion_GetQuestID() == m_Quest.GetQuestConfig().GetID() && item.Expansion_IsObjectiveLoot());
			case ExpansionQuestObjectiveType.DELIVERY:
				return (item.Expansion_GetQuestID() == m_Quest.GetQuestConfig().GetID());
		}

		return true;
	}

	protected bool CanAddItem(ItemBase item)
	{
		if (item.Expansion_IsSetForDeletion())
			return false;

		if (item.CanDecay())
		{
			Edible_Base edibleIB;
			if (Class.CastTo(edibleIB, item) && (edibleIB.GetFoodStageType() == FoodStageType.BURNED || edibleIB.GetFoodStageType() == FoodStageType.ROTTEN))
				return false;
		}
		
		//! Make sure we don't use items as money that should not be included, like attachments (e.g. Dogtags that are attached to the player itself) or that are in nested containers (1.25 change).
		if (!item.Expansion_IsQuestItem() && !MiscGameplayFunctions.Expansion_IsLooseEntity(item, true))
			return false;

		return true;
	}

	protected void AddObjectiveItem(ItemBase item, ExpansionQuestObjectiveItemCount count)
	{
		if (m_DeliveryConfig.GetObjectiveType() == ExpansionQuestObjectiveType.DELIVERY)
			item.Expansion_SetIsDeliveryItem(true);

		ExpansionQuestObjectiveItem objItem = new ExpansionQuestObjectiveItem(item);
		m_ObjectiveItems.Insert(objItem);
		int stack;
		int amount = GetNeededAmount(objItem, count, stack);
		EXTrace.Print(EXTrace.QUESTS, this, "add: " + item + " | stack: " + stack + " amount: " + amount + " remaining: " + objItem.GetRemaining() + " remaining needed: " + count.RemainingNeeded);
		m_ObjectiveItemsCount += amount;
	}

	protected int GetNeededAmount(ExpansionQuestObjectiveItem objItem, ExpansionQuestObjectiveItemCount count, out int stack = 0)
	{
		int amount;  //! Amount to add
		stack = objItem.GetItem().Expansion_GetStackAmount();
		if (stack <= count.RemainingNeeded)
		{
			amount = stack;
			count.RemainingNeeded -= stack;
		}
		else
		{
			amount = count.RemainingNeeded;
			count.RemainingNeeded = 0;
		}

		objItem.SetRemaining(stack - amount);

		return amount;
	}

	protected void UpdateDeliveryData()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_DeliveryData.Clear();
		m_ObjectiveItemsCount = 0;  //! Yes, always recount here - e.g. during item split, we only process INV_ENTER to save unnecessary network churn, which means counts need to be updated

		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_DeliveryConfig.GetCollections();
		for (int i = 0; i < objectiveDeliveries.Count(); i++)
		{
			ExpansionQuestObjectiveDelivery objectiveDelivery = objectiveDeliveries[i];
			string typeNameLower = objectiveDelivery.GetClassNameLower();
			int needed = objectiveDelivery.GetAmount();

			//! @note: This shouldn't be able to happen here
			if (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
			{
				Error("There are still objective items, but count has already reached needed total!");
				break;
			}

			ExpansionQuestObjectiveItemCount count = m_ObjectiveItemsMap[typeNameLower];
			count.RemainingNeeded = count.Needed;  //! Reset

			int inventoryCount = 0;

			ObjectivePrint("Check objective items for items with type name " + typeNameLower);
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassNameLower() == typeNameLower)
				{
					//! @note: This shouldn't be able to happen here
					if (count.RemainingNeeded == 0)
					{
						Error("There are still objective items, but remaining needed for " + typeNameLower + " is already zero!");
						break;
					}

					int stack = 0;
					int amount = GetNeededAmount(objItem, count, stack);

					EXTrace.Print(EXTrace.QUESTS, this, "Count objective item " + objItem.GetItem() + " | stack: " + stack + " amount: " + amount + " remaining: " + objItem.GetRemaining() + " remaing needed: " + count.RemainingNeeded);
					inventoryCount += stack;
					m_ObjectiveItemsCount += amount;
				}
			}

			EXTrace.Print(EXTrace.QUESTS, this, "Add delivery data entry for item: " + typeNameLower + " | Inventory count: " + inventoryCount + " | Needed: " + count.Needed + " | Index: " + i);
			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(i, Math.Min(inventoryCount, needed));
			m_DeliveryData.Insert(deliveryData);
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, "Objective items: " + m_ObjectiveItemsCount + " / " + m_ObjectiveItemsAmount);
	#endif
	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state, float delta = 0.0)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.StartStack(EXTrace.QUESTS, this, "Call: " + s_CallCount++, item.ToString(), typename.EnumToString(ExpansionQuestItemState, state), "Delta: " + delta);
	#endif

		string typeNameLower = item.Expansion_GetTypeLower();
		ExpansionQuestObjectiveItemCount count;
		if (!m_ObjectiveItemsMap.Find(typeNameLower, count))
			return;

		int foundIndex;
		int amount;
		ExpansionQuestObjectiveItem foundObjItem;
		
		int objectiveItemsCount = m_ObjectiveItemsCount;

		switch (state)
		{
		case ExpansionQuestItemState.INV_EXIT:
			if (IsObjectiveItem(item, foundIndex, foundObjItem))
			{
				CheckQuestPlayersForObjectiveItems();
				UpdateDeliveryData();
			}
			break;
		case ExpansionQuestItemState.INV_ENTER:
			if (count.RemainingNeeded > 0 && CanAddObjectiveItem(item))
			{
				EXTrace.Print(EXTrace.QUESTS, this, "::OnInventoryItemLocationChange - Item is not in objective items array: " + typeNameLower + ", adding");
				AddObjectiveItem(item, count);
				UpdateDeliveryData();
			}
			break;
		case ExpansionQuestItemState.STACKSIZE_CHANGED:
			if (IsObjectiveItem(item, foundIndex, foundObjItem))
			{
				EXTrace.Print(EXTrace.QUESTS, this, "::OnInventoryItemLocationChange - The stack size of an objective item has changed: " + typeNameLower + " | New amount: " + item.Expansion_GetStackAmount() + " | Index: " + foundIndex);
				CheckQuestPlayersForObjectiveItems();
				UpdateDeliveryData();
			}
			break;
		case ExpansionQuestItemState.CONTENTQUANTITY_CHANGED:
			if (IsObjectiveItem(item, foundIndex, foundObjItem))
			{
				int quantityNeeded = m_CollectionsQuantityMap[typeNameLower];
				ExpansionItemQuantityType quantityType;
				int quantity = item.Expansion_GetQuantity(quantityType);
				EXTrace.Print(EXTrace.QUESTS, this, "::OnInventoryItemLocationChange - content quantity of an objective item has changed: " + typeNameLower + " | quantityNeeded: " + quantityNeeded + " | quantity: " + quantity + " | quantityType: " + typename.EnumToString(ExpansionItemQuantityType, quantityType));			
				if (quantity < quantityNeeded)
				{
					CheckQuestPlayersForObjectiveItems();
					UpdateDeliveryData();
				}
			}
			else
			{
				if (count.RemainingNeeded > 0 && CanAddObjectiveItem(item))
				{
					EXTrace.Print(EXTrace.QUESTS, this, "::OnInventoryItemLocationChange - Item is not in objective items and quantity has changed to needed value. Adding " + typeNameLower);
					AddObjectiveItem(item, count);
					UpdateDeliveryData();
				}
			}
			break;
		case ExpansionQuestItemState.INV_DELETE:
			if (IsObjectiveItem(item, foundIndex, foundObjItem))
			{
				CheckQuestPlayersForObjectiveItems();
				UpdateDeliveryData();
			}
			break;
		}

		//! Only check quest completion if there was a change
		if (m_ObjectiveItemsCount != objectiveItemsCount)
			m_Quest.QuestCompletionCheck(true);
	}

	protected bool IsObjectiveItem(ItemBase item, out int index = -1, out ExpansionQuestObjectiveItem foundObjItem = null)
	{
		foreach (int i, ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
		{
			if (objItem.GetItem() == item)
			{
				index = i;
				foundObjItem = objItem;
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Is objective item " + item);
			#endif
				return true;
			}
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, "Is NOT objective item " + item);
	#endif
		return false;
	}

	protected bool HasAnyCollectionCompleted()
	{
		foreach (string typeNameLower, ExpansionQuestObjectiveItemCount count: m_ObjectiveItemsMap)
		{
			int collectionItemCount = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassNameLower() == typeNameLower)
				{
					int amount = objItem.GetItem().Expansion_GetStackAmount();
					collectionItemCount += amount;
				}
			}

			if (collectionItemCount >= count.Needed)
				return true;
		}

		return false;
	}

#ifdef EXPANSIONMODMARKET
	//! Add the delivered delivery items to the market zone if there is one nearby.
	protected void AddItemsToMarketZone(string playerUID)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (m_DeliveryConfig.GetObjectiveType() != ExpansionQuestObjectiveType.COLLECT)
			return;

		ExpansionQuestPersistentServerData serverData = ExpansionQuestModule.GetModuleInstance().GetServerData();
		if (!serverData)
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;

		auto marketSettings = GetExpansionSettings().GetMarket();
		ExpansionMarketTraderZone traderZone = marketSettings.GetTraderZoneByPosition(player.GetPosition());
		if (!traderZone)
			return;

		vector zonePos = traderZone.Position;
		ObjectivePrint("Market zone pos: " + zonePos.ToString());
		array<ref ExpansionQuestObjectiveDelivery> deliveries = m_DeliveryConfig.GetCollections();
		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			string nameLower = delivery.GetClassNameLower();
			int amount = delivery.GetAmount();

			ExpansionMarketItem marketItem = ExpansionMarketCategory.GetGlobalItem(nameLower);
			if (!marketItem)
			{
				ObjectivePrint("Item " + nameLower + " is not a market item. Skip..");
				continue;
			}

			serverData.AddQuestMarketItem(zonePos, nameLower, amount);
		}

		serverData.Save();
	}
#endif

	protected void ObjectiveCheck()
	{
		CheckQuestPlayersForObjectiveItems();
		UpdateDeliveryData();
		m_Quest.QuestCompletionCheck(true);
	}
};
