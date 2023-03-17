/**
 * ExpansionQuestObjectiveDeliveryEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveDeliveryEvent: ExpansionQuestObjectiveCollectionEventBase
{
	protected ref ExpansionQuestObjectiveDeliveryConfig m_Config;

	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");
		
		if (!super.OnEventStart())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;
		
		if (!CreateObjectiveTrigger())
			return false;
		
		if (!GetObjectiveDataFromConfig())
			return false;
		
		if (!SpawnObjectiveDeliveryItems())
			return false;
		
		CheckQuestPlayersForObjectiveItems();

		UpdateDeliveryData();

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif
		
		m_Quest.QuestCompletionCheck();
		
		ObjectivePrint(ToString() + "::OnEventStart - End");
		
		return true;
	}
	
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		if (!super.OnContinue())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;
		
		//! Only create the trigger if not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			if (!CreateObjectiveTrigger())
				return false;
		}
		
		if (!GetObjectiveDataFromConfig())
			return false;
		
		CheckQuestPlayersForObjectiveItems();
		
		UpdateDeliveryData();
		
	#ifdef EXPANSIONMODNAVIGATION
		//! Only create the marker if not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
		{
			if (m_Config.GetMarkerName() != string.Empty)
				CreateMarkers();
		}
	#endif
		
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End");
		
		return true;
	}

	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		bool conditionsResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount) && m_DestinationReached;
	#ifdef EXPANSIONMODNAVIGATION
		bool markerConditionResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount);
	#endif
		
	#ifdef EXPANSIONMODNAVIGATION
		if (markerConditionResult && !m_CreatedMarker && m_Config.GetMarkerName() != string.Empty)
		{
			CreateMarkers();
			m_CreatedMarker = true;
		}
		else if (m_CreatedMarker && !markerConditionResult)
		{
			RemoveObjectiveMarkers();
			m_CreatedMarker = false;
		}
	#endif
		
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return true;
	}
	
	override bool OnTurnIn(string playerUID, int selectedObjItemIndex = -1)
	{
		ObjectivePrint(ToString() + "::OnTurnIn - Start");

		if (!CleanupObjectiveItems(selectedObjItemIndex))
			return false;
		
	#ifdef EXPANSIONMODMARKET
		if (m_Config.AddItemsToNearbyMarketZone())
			AddItemsToMarketZone(playerUID);
	#endif
		
		if (!super.OnTurnIn(playerUID, selectedObjItemIndex))
			return false;

		ObjectivePrint(ToString() + "::OnTurnIn - End");

		return true;
	}
	
	override bool OnIncomplete()
	{
		ObjectivePrint(ToString() + "::OnIncomplete - Start");

		if (!super.OnIncomplete())
			return false;
		
	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
		{
			RemoveObjectiveMarkers();
			m_CreatedMarker = false;

			bool markerConditionResult = m_ObjectiveItemsAmount != 0 && (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount);
			if (markerConditionResult && m_Config.GetMarkerName() != string.Empty)
			{
				CreateMarkers();
				m_CreatedMarker = true;
			}
		}
	#endif
		
		ObjectivePrint(ToString() + "::OnIncomplete - End");

		return true;
	}

	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		CheckQuestPlayersForObjectiveItems();
		DeleteRemainingObjectiveItems();
		
		if (!super.OnCancel())
			return false;
		
		ObjectivePrint(ToString() + "::OnCancel - End");
		
		return true;
	}

	protected void DeleteRemainingObjectiveItems()
	{
		for (int i = m_ObjectiveItems.Count() - 1; i >= 0; i--)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			GetGame().ObjectDelete(objItem.GetItem());
			m_ObjectiveItems.RemoveOrdered(i);
		}
	}

	protected bool SpawnObjectiveDeliveryItems()
	{
		ObjectivePrint(ToString() + "::SpawnObjectiveDeliveryItems - Start");
		
		PlayerBase player = m_Quest.GetPlayer();
		if (!player || !player.GetIdentity())
			return false;
		
		vector position = player.GetPosition();
		vector orientation = player.GetOrientation();
		
		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_Config.GetCollections();
		foreach (ExpansionQuestObjectiveDelivery delivery: objectiveDeliveries)
		{
			if (!SpawnDeliveryItems(delivery, player, player, position, orientation))
				return false;
		}
		
		ObjectivePrint(ToString() + "::SpawnObjectiveDeliveryItems - End");
		
		return true;
	}

	protected bool SpawnDeliveryItems(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, EntityAI parent, vector position, vector orientation)
	{
		ObjectivePrint(ToString() + "::SpawnDeliveryItems - Start");

		if (m_Quest.GetQuestConfig().GetID() == -1)
			return false;
		
	    int amountToSpawn = delivery.GetAmount();
		while (amountToSpawn > 0)
	    {
	        Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, amountToSpawn);
	        if (!obj)
	            break;

	        ItemBase questItem;
	        if (!Class.CastTo(questItem, obj))
	        {
	            GetGame().ObjectDelete(obj);
	            return false;
	        }

	        questItem.SetQuestID(m_Quest.GetQuestConfig().GetID());
			ExpansionQuestObjectiveItem objItem = new ExpansionQuestObjectiveItem(questItem);
			m_ObjectiveItems.Insert(objItem);
	    }

		ObjectivePrint(ToString() + "::SpawnDeliveryItems - End");
		
		return true;
	}

	protected bool CreateObjectiveTrigger()
	{
		ObjectivePrint(ToString() + ":: CreateObjectiveTrigger - Start");

		if (!m_Quest || !m_Quest.GetQuestConfig())
			return false;
		
		array<int> questNPCTurnInIDs = m_Quest.GetQuestConfig().GetQuestTurnInIDs();
		if (!questNPCTurnInIDs || questNPCTurnInIDs.Count() == 0)
			return false;
		
		PlayerBase player = m_Quest.GetPlayer();
		if (!player || !player.GetIdentity())
			return false;

		vector playerPos = player.GetPosition();
		Object npcObj = m_Quest.GetQuestModule().GetClosestQuestNPCForQuest(questNPCTurnInIDs, playerPos);
		if (!npcObj)
			return false;

		vector npcPos = npcObj.GetPosition();
		int triggerDistance = 5.0;
		if (m_Config.GetMaxDistance() > 0)
			triggerDistance = m_Config.GetMaxDistance();

		Object trigger = GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", npcPos, ECE_NONE);
		if (!Class.CastTo(m_ObjectiveTrigger, trigger))
		{
			GetGame().ObjectDelete(trigger);
			return false;
		}

		m_ObjectiveTrigger.SetObjectiveData(this);
		m_ObjectiveTrigger.SetPosition(npcPos);

		ObjectivePrint(ToString() + ":: CreateObjectiveTrigger - Created objective trigger at position: " + npcPos + " | Trigger sphere distance: " + triggerDistance);
		
		return true;
	}
	
	protected bool GetObjectiveDataFromConfig()
	{
		ObjectivePrint(ToString() + "::GetObjectiveDataFromConfig - Start");
		
		m_ObjectiveItemsMap.Clear();
		
		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_Config.GetCollections();
		if (!objectiveDeliveries || objectiveDeliveries.Count() == 0)
		{
			int objectiveID = m_Config.GetID();
			Error(ToString() + "::GetObjectiveDataFromConfig - Collection objective with ID " + objectiveID + " has no collections defined!");
			return false;
		}
		
		foreach (ExpansionQuestObjectiveDelivery objectiveDelivery: objectiveDeliveries)
		{
			string typeName = objectiveDelivery.GetClassName();
			int amount = objectiveDelivery.GetAmount();
			ObjectivePrint(ToString() + "::GetObjectiveDataFromConfig - Add collection data for type name: " + typeName + " | Amount: " + amount);
			
			int current;
			if (!m_ObjectiveItemsMap.Find(typeName, current))
			{
				m_ObjectiveItemsMap.Insert(typeName, amount);
				m_ObjectiveItemsAmount += amount;
				ObjectivePrint(ToString() + "::GetObjectiveDataFromConfig - Added collection data for type name: " + typeName + " | Amount: " + amount + " to objective items map.");
			}
			else
			{
				int neededNew = current + amount;
				m_ObjectiveItemsMap.Set(typeName, neededNew);
				m_ObjectiveItemsAmount += amount;
			}
		}
			
		
		ObjectivePrint(ToString() + "::GetObjectiveDataFromConfig - End and return TRUE");
		
		return true;
	}
	
	protected bool CheckQuestPlayersForObjectiveItems(bool continues = false)
	{
		ObjectivePrint(ToString() + "::CheckQuestPlayersForObjectiveItems - Start");
			
		m_ObjectiveItems.Clear();
		m_ObjectiveItemsCount = 0;
		
		Man player;		
		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			player = m_Quest.GetPlayer();
			if (!player || !player.GetIdentity())
			{
				Error(ToString() + "::CheckQuestPlayersForObjectiveItems - Could not get quest player!");
				return false;
			}
			
			CheckPlayerForObjectiveItems(player);
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			array<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				if (m_ObjectiveItemsCount >= m_ObjectiveItemsAmount)
					break;

				player = PlayerBase.GetPlayerByUID(playerUID);
				if (!player || !player.GetIdentity())
					continue;
				
				CheckPlayerForObjectiveItems(player);
			}
		}
	#endif
		
		if (continues && m_ObjectiveItemsCount < m_ObjectiveItemsAmount)
			return false;
		
		ObjectivePrint(ToString() + "::CheckQuestPlayersForObjectiveItems - End and return TRUE");
		
		return true;
	}

	protected void CheckPlayerForObjectiveItems(Man player)
	{
		ObjectivePrint(ToString() + "::CheckPlayerForObjectiveItems - Start");
		
		array<EntityAI> items = new array<EntityAI>;
  	 	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			string typeName = item.GetType();
			if (!m_ObjectiveItemsMap.Contains(typeName))
				continue;

			int amount = GetItemAmount(item);
			if (!IsObjectiveItem(item))
			{
				ItemBase itemIB;
				if (!Class.CastTo(itemIB, item))
					continue;

				if (m_Config.GetObjectiveType() == ExpansionQuestObjectiveType.COLLECT && itemIB.IsQuestItem() || m_Config.GetObjectiveType() == ExpansionQuestObjectiveType.DELIVERY && !itemIB.IsQuestItem())
					continue;

				ObjectivePrint(ToString() + "::CheckPlayerForObjectiveItems - Add player item to valid objective items: " + typeName + " | Amount: " + amount);
				ExpansionQuestObjectiveItem objItem = new ExpansionQuestObjectiveItem(item);
				m_ObjectiveItems.Insert(objItem);
				m_ObjectiveItemsCount += amount;
			}
		}
		
		ObjectivePrint(ToString() + "::CheckPlayerForObjectiveItems - End");
	}
	
	protected void UpdateDeliveryData()
	{
		ObjectivePrint(ToString() + "::UpdateDeliveryData - Start");

		m_DeliveryData.Clear();
		
		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_Config.GetCollections();
		for (int i = 0; i < objectiveDeliveries.Count(); i++)
		{
			ExpansionQuestObjectiveDelivery objectiveDelivery = objectiveDeliveries[i];
			string typeName = objectiveDelivery.GetClassName();
			int needed = objectiveDelivery.GetAmount();
			int inventoryCount = 0;
			
			ObjectivePrint(ToString() + "::UpdateDeliveryData - Check objective items for items with type name " + typeName);
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassName() == typeName)
				{
					int amount = GetItemAmount(objItem.GetItem());
					ObjectivePrint(ToString() + "::UpdateDeliveryData - Count objective item " + typeName + " | Amount: " + amount);
					inventoryCount += amount;
				}
			}
			
			ObjectivePrint(ToString() + "::UpdateDeliveryData - Add delivery data entry for item: " + typeName + " | Inventory count: " + inventoryCount + " | Needed: " + needed + " | Index: " + i);
			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(i, inventoryCount);
			m_DeliveryData.Insert(deliveryData);
		}

		m_Quest.UpdateQuest(false);

		ObjectivePrint(ToString() + "::UpdateDeliveryData - End");
	}
	
	void OnInventoryItemLocationChange(EntityAI item, Man player, string state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		string typeName = item.GetType();
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item: " + typeName);
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Inventory location state: " + state);
		
		if (!m_ObjectiveItemsMap.Contains(typeName))
			return;
		
		int amount = GetItemAmount(item);
		if (state == "INV_EXIT")
		{
			int foundIndex = -1;
			if (IsObjectiveItem(item, foundIndex))
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Found item in current objective items array: " + typeName + " | Amount: " + amount + " | Index: " + foundIndex);
				m_ObjectiveItemsCount -= amount;
				m_ObjectiveItems.Remove(foundIndex);
				UpdateDeliveryData();
			}
		}
		else if (state == "INV_ENTER")
		{
			if (player && player.GetIdentity())
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item is not in objective items array: " + typeName + ". Check player..");
				CheckPlayerForObjectiveItems(player);
				UpdateDeliveryData();
			}
		}
		else if (state == "QUANTITY_CHANGED")
		{
			if (IsObjectiveItem(item))
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - The quantity of a objective item has changed: " + typeName + " | Amount: " + amount);
				CheckQuestPlayersForObjectiveItems();
				UpdateDeliveryData();
			}
		}
		
		m_Quest.QuestCompletionCheck();
		
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - End");
	}
	
	override protected bool IsObjectiveItem(EntityAI item, out int index = -1)
	{
		for (int i = 0; i < m_ObjectiveItems.Count(); i++)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			if (objItem.GetItem() == item)
			{
				ItemBase itemIB;
				if (Class.CastTo(itemIB, item) && itemIB.GetQuestID() == m_Quest.GetQuestConfig().GetID())
				{
					index = i;
					return true;
				}
			}
		}
		
		return false;
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return;

		ObjectivePrint(ToString() + "::CreateMarkers - Start");
		string markerName = m_Config.GetMarkerName();
		vector npcPos = m_Quest.GetClosestQuestNPCPosition(m_Quest.GetQuestConfig().GetQuestTurnInIDs(), m_Quest.GetPlayer().GetPosition());
		CreateObjectiveMarker(npcPos, markerName);
		ObjectivePrint(ToString() + "::CreateMarkers - End");
	}
#endif
	
	protected bool CleanupObjectiveItems(int selectedObjItemIndex = -1)
	{
		ObjectivePrint(ToString() + "::CleanupObjectiveItems - Start");
		ObjectivePrint(ToString() + "::CleanupObjectiveItems - Selected objective item index: " + selectedObjItemIndex);
		
		int all = 0;
		int allCollection = 0;
		array<ExpansionQuestObjectiveItem> changedItems = new array<ExpansionQuestObjectiveItem>;
		
		for (int i = 0; i < m_ObjectiveItemsMap.Count(); i++)
		{
			string typeName = m_ObjectiveItemsMap.GetKey(i);
			int needed = m_ObjectiveItemsMap.Get(typeName);
						
			ObjectivePrint(ToString() + "::CleanupObjectiveItems - Check objective items for item " + typeName + " | Needed: " + needed);
			int remove = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassName() == typeName)
				{
					int amount = GetItemAmount(objItem.GetItem());
					int remaining;
					ObjectivePrint(ToString() + "::CleanupObjectiveItems - Check objective item " + typeName + " | Amount: " + amount);
					if (amount <= needed)
					{
						remaining = amount - amount;
						ObjectivePrint(ToString() + "::CleanupObjectiveItems - Item amount is lower or exacly the needed amount for item " + typeName + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: " + remaining);
						objItem.SetRemaining(remaining);
						remove += amount;
						changedItems.Insert(objItem);
					}
					else if (amount > needed)
					{
						remaining = amount - needed;
						ObjectivePrint(ToString() + "::CleanupObjectiveItems - Item amount is higher then the needed amount for item " + typeName + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: " + remaining);
						objItem.SetRemaining(remaining);
						remove += needed;
						changedItems.Insert(objItem);
					}
				}
				
				if (remove >= needed)
					break;
			}
						
			if (remove < needed)
			{
				ObjectivePrint(ToString() + "::CleanupObjectiveItems - E1 - Could not get all objective items! Needed: " + needed + " | Found to remove: " + remove);
				changedItems.Clear();
				return false;
			}
		
			all += remove;
			allCollection = needed;
			
			if (all >= m_ObjectiveItemsAmount)
				break;
		}

		if (all < m_ObjectiveItemsAmount)
		{
			ObjectivePrint(ToString() + "::CleanupObjectiveItems - E2 - Could not get all objective items! All needed: " + m_ObjectiveItemsAmount + " | Found to remove: " + all);
			changedItems.Clear();
			return false;
		}
		
		foreach (ExpansionQuestObjectiveItem itemtoDelete: changedItems)
		{
			itemtoDelete.DestroyItem();
		}

		ObjectivePrint(ToString() + "::CleanupObjectiveItems - End");
		
		return true;
	}

#ifdef EXPANSIONMODMARKET
	//! Add the delivered delivery items to the market zone if there is one nearby.
	protected void AddItemsToMarketZone(string playerUID)
	{
		ObjectivePrint(ToString() + "::AddItemsToMarketZone - Start");

		if (m_Config.GetObjectiveType() != ExpansionQuestObjectiveType.COLLECT )
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
		ObjectivePrint(ToString() + "::AddItemsToMarketZone - Market zone pos: " + zonePos.ToString());
		array<ref ExpansionQuestObjectiveDelivery> deliveries = m_Config.GetCollections();
		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			string name = delivery.GetClassName();
			string nameLower = name;
			nameLower.ToLower();
			int amount = delivery.GetAmount();

			ExpansionMarketItem marketItem = marketSettings.GetItem(nameLower);
			if (!marketItem)
			{
				ObjectivePrint(ToString() + "::AddItemsToMarketZone - Item " + name + " is not a market item. Skip..");
				continue;
			}

			serverData.AddQuestMarketItem(zonePos, name, amount);
		}

		serverData.Save();

		ObjectivePrint(ToString() + "::AddItemsToMarketZone - End");
	}
#endif

	override bool HasDynamicState()
	{
		return true;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}
};