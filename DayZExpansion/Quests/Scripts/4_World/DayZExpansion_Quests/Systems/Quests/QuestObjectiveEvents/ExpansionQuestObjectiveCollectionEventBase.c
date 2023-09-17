/**
 * ExpansionQuestObjectiveCollectionEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionEventBase: ExpansionQuestObjectiveEventBase
{
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected bool m_DestinationReached;
	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected ref map<string, int> m_ObjectiveItemsMap;
	protected ref array<ref ExpansionQuestObjectiveItem> m_ObjectiveItems;
	protected int m_ObjectiveItemsAmount;
	protected int m_ObjectiveItemsCount;
#ifdef EXPANSIONMODNAVIGATION
	protected bool m_CreatedMarker;
#endif
	protected vector m_Position;

	protected ref ExpansionQuestObjectiveDeliveryConfigBase m_Config;

	void ExpansionQuestObjectiveCollectionEventBase(ExpansionQuest quest)
	{
		m_ObjectiveItemsMap = new map<string, int>;
		m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		m_ObjectiveItems = new array<ref ExpansionQuestObjectiveItem>;
	}

	void ~ExpansionQuestObjectiveCollectionEventBase()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_ObjectiveItemsMap.Clear();
		m_DeliveryData.Clear();
		m_ObjectiveItems.Clear();
	}

	override bool OnTurnIn(string playerUID, int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!CleanupObjectiveItems(selectedObjItemIndex))
			return false;

	#ifdef EXPANSIONMODMARKET
		if (m_Config.AddItemsToNearbyMarketZone())
			AddItemsToMarketZone(playerUID);
	#endif

		if (!super.OnTurnIn(playerUID, selectedObjItemIndex))
			return false;

		return true;
	}

	protected bool CleanupObjectiveItems(int selectedObjItemIndex = -1)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("Selected objective item index: " + selectedObjItemIndex);

		int all = 0;
		int allCollection = 0;
		array<ExpansionQuestObjectiveItem> changedItems = new array<ExpansionQuestObjectiveItem>;

		foreach (string typeName, int needed: m_ObjectiveItemsMap)
		{
			if (m_Config.NeedAnyCollection())
			{
				if (selectedObjItemIndex == -1)
					continue;

				ExpansionQuestObjectiveDelivery delivery = m_Config.GetCollections().Get(selectedObjItemIndex);
				if (!delivery)
					continue;

				if (delivery.GetClassName() != typeName)
					continue;
			}

			ObjectivePrint("Check objective items for item " + typeName + " | Needed: " + needed);
			int remainingNeeded = needed;
			int remove = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
			    if (objItem.GetClassName() == typeName)
			    {
			        int amount = objItem.GetItem().Expansion_GetStackAmount();
			        int remaining;
			        ObjectivePrint("Check objective item " + typeName + " | Amount: " + amount);
			        if (amount <= remainingNeeded)
			        {
			            remainingNeeded -= amount;
			            ObjectivePrint("Item amount is lower or exacly the needed amount for item " + typeName + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: 0");
			            objItem.SetRemaining(0);
			            remove += amount;
			            changedItems.Insert(objItem);
			        }
			        else
			        {
			            remaining = amount - remainingNeeded;
			            ObjectivePrint("Item amount is higher then the needed amount for item " + typeName + " | Amount: " + amount + " | Needed: " + needed + " | Remaining: " + remaining);
			            objItem.SetRemaining(remaining);
			            remove += remainingNeeded;
						remainingNeeded = 0;
			            changedItems.Insert(objItem);
			        }
			    }
			
			    if (remainingNeeded == 0)
			        break;
			}

			if (!m_Config.NeedAnyCollection() && remove < needed)
			{
				ObjectivePrint("E1 - Could not get all objective items! Needed: " + needed + " | Found to remove: " + remove);
				changedItems.Clear();
				return false;
			}

			all += remove;
			allCollection = needed;

			if (m_Config.NeedAnyCollection() && remove >= needed)
				break;

			if (!m_Config.NeedAnyCollection() && all >= m_ObjectiveItemsAmount)
				break;
		}

		ObjectivePrint("Need any ollection: " + m_Config.NeedAnyCollection().ToString());

		if (!m_Config.NeedAnyCollection() && all < m_ObjectiveItemsAmount)
		{
			ObjectivePrint("E2 - Could not get all objective items! All needed: " + m_ObjectiveItemsAmount + " | Found to remove: " + all);
			changedItems.Clear();
			return false;
		}
		else if (m_Config.NeedAnyCollection() && (all < allCollection || selectedObjItemIndex == -1))
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

	override bool OnCleanup()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!super.OnCleanup())
			return false;

		DeleteObjectiveTrigger();

		return true;
	}

	protected void DeleteObjectiveTrigger()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		if (!m_ObjectiveTrigger)
			return;

		GetGame().ObjectDelete(m_ObjectiveTrigger);
	}

	void SetReachedLocation(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		ObjectivePrint("State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck(true);
	}

	bool GetLocationState()
	{
		return m_DestinationReached;
	}

	array<ref ExpansionQuestDeliveryObjectiveData> GetDeliveryData()
	{
		return m_DeliveryData;
	}

	protected bool CreateObjectiveTrigger()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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

		//! If trigger radius is large enough to cover the whole map, don't create it
		float worldSize = GetGame().GetWorld().GetWorldSize();
		//! Choose world pos at edge of map that is farthest from NPC pos
		vector worldPos;
		if (npcPos[0] < worldSize * 0.5)
			worldPos[0] = worldSize;

		worldPos[1] = npcPos[1];

		if (npcPos[2] < worldSize * 0.5)
			worldPos[2] = worldSize;

		if (m_Config.GetMaxDistance() <= 0 || m_Config.GetMaxDistance() >= vector.Distance(npcPos, worldPos))
		{
			m_DestinationReached = true;
			return true;
		}

		m_Position = npcPos;

		Object trigger = GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", npcPos, ECE_NONE);
		if (!Class.CastTo(m_ObjectiveTrigger, trigger))
		{
			GetGame().ObjectDelete(trigger);
			return false;
		}

		m_ObjectiveTrigger.SetObjectiveData(this);
		m_ObjectiveTrigger.SetPosition(npcPos);

		return true;
	}

	protected bool GetObjectiveDataFromConfig()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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
			ObjectivePrint("Add collection data for type name: " + typeName + " | Amount: " + amount);

			m_ObjectiveItemsAmount += amount;

			int current;
			if (!m_ObjectiveItemsMap.Find(typeName, current))
			{
				m_ObjectiveItemsMap.Insert(typeName, amount);
				ObjectivePrint("Added collection data for type name: " + typeName + " | Amount: " + amount + " to objective items map.");
			}
			else
			{
				int neededNew = current + amount;
				m_ObjectiveItemsMap.Set(typeName, neededNew);
			}
		}

		ObjectivePrint("End and return TRUE");

		return true;
	}

	protected bool CheckQuestPlayersForObjectiveItems(bool continues = false)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_ObjectiveItems.Clear();
		m_ObjectiveItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			if (!CheckPlayerForObjectiveItems(m_Quest.GetPlayer()))
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
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
	#endif

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

		auto trace = EXTrace.Start(EXTrace.QUESTS, this, player.GetIdentity().GetId());

		foreach (string typeName, int needed: m_ObjectiveItemsMap)
		{
			EXTrace.Print(EXTrace.QUESTS, this, typeName + " needed: " + needed);			
			ExpansionInventoryItemType itemType = player.Expansion_GetInventoryItemType(typeName);
			if (itemType)
			{
				int remainingNeeded = needed;
				foreach (ItemBase item: itemType.Items)
				{	
					if (item.IsRuined())
						continue;

					int amount = item.Expansion_GetStackAmount();			  
			        if (amount <= remainingNeeded)
			        {
			            remainingNeeded -= amount;
			            AddObjectiveItem(item);
			        }
			        else
			        {
			            remainingNeeded = 0;
			        }
					
					if (remainingNeeded == 0)
                    	break;
				}
			}
		}

		return true;
	}

	protected bool CanAddObjectiveItem(ItemBase item)
	{
		if (item.IsRuined())
			return false;
		
		switch (m_Config.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.COLLECT:
				return !item.Expansion_IsQuestItem();
			case ExpansionQuestObjectiveType.DELIVERY:
				return (item.Expansion_GetQuestID() == m_Quest.GetQuestConfig().GetID());
		}

		return true;
	}

	protected void AddObjectiveItem(ItemBase item)
	{
		if (m_Config.GetObjectiveType() == ExpansionQuestObjectiveType.DELIVERY)
			item.Expansion_SetIsDeliveryItem(true);

		int amount = item.Expansion_GetStackAmount();
		EXTrace.Print(EXTrace.QUESTS, this, "add: " + amount);
		ExpansionQuestObjectiveItem objItem = new ExpansionQuestObjectiveItem(item);
		m_ObjectiveItems.Insert(objItem);
		m_ObjectiveItemsCount += amount;
	}

	protected void UpdateDeliveryData()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_DeliveryData.Clear();

		array<ref ExpansionQuestObjectiveDelivery> objectiveDeliveries = m_Config.GetCollections();
		for (int i = 0; i < objectiveDeliveries.Count(); i++)
		{
			ExpansionQuestObjectiveDelivery objectiveDelivery = objectiveDeliveries[i];
			string typeName = objectiveDelivery.GetClassName();
			int needed = objectiveDelivery.GetAmount();
			int inventoryCount = 0;

			ObjectivePrint("Check objective items for items with type name " + typeName);
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassName() == typeName)
				{
					int amount = objItem.GetItem().Expansion_GetStackAmount();
					ObjectivePrint("Count objective item " + typeName + " | Amount: " + amount);
					inventoryCount += amount;
				}
			}

			ObjectivePrint("Add delivery data entry for item: " + typeName + " | Inventory count: " + inventoryCount + " | Needed: " + needed + " | Index: " + i);
			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(i, inventoryCount);
			m_DeliveryData.Insert(deliveryData);
		}
	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		string typeName = item.GetType();
		int needed = 0;
		if (!m_ObjectiveItemsMap.Find(typeName, needed))
			return;

		ObjectivePrint("::OnInventoryItemLocationChange - Item: " + typeName);
		ObjectivePrint("::OnInventoryItemLocationChange - Inventory location state: " + typename.EnumToString(ExpansionQuestItemState, state));

		int amount = item.Expansion_GetStackAmount();
		switch (state)
		{
		case ExpansionQuestItemState.INV_EXIT:
			int foundIndex;
			if (IsObjectiveItem(item, foundIndex))
			{
				ObjectivePrint("::OnInventoryItemLocationChange - Found item in current objective items array, removing: " + typeName + " | Amount: " + amount + " | Index: " + foundIndex);
				m_ObjectiveItemsCount -= amount;
				m_ObjectiveItems.Remove(foundIndex);
				UpdateDeliveryData();
			}
			break;
		case ExpansionQuestItemState.INV_ENTER:
			if (amount <= needed && m_ObjectiveItemsCount < m_ObjectiveItemsAmount && CanAddObjectiveItem(item))
			{
				ObjectivePrint("::OnInventoryItemLocationChange - Item is not in objective items array: " + typeName + ", adding");
				AddObjectiveItem(item);
				UpdateDeliveryData();
			}
			break;
		case ExpansionQuestItemState.QUANTITY_CHANGED:
			ObjectivePrint("::OnInventoryItemLocationChange - The quantity of a objective item has changed: " + typeName + " | Amount: " + amount);
			CheckQuestPlayersForObjectiveItems();
			UpdateDeliveryData();
			break;
		}

		m_Quest.QuestCompletionCheck(true);
	}

	protected bool IsObjectiveItem(ItemBase item, out int index = -1)
	{
		if (!CanAddObjectiveItem(item))
			return false;

		for (int i = 0; i < m_ObjectiveItems.Count(); i++)
		{
			ExpansionQuestObjectiveItem objItem = m_ObjectiveItems[i];
			if (objItem.GetItem() == item)
			{
				index = i;
				return true;
			}
		}

		return false;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return;

		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		string markerName = m_Config.GetMarkerName();
		array<int> questNPCTurnInIDs = m_Quest.GetQuestConfig().GetQuestTurnInIDs();
		if (!questNPCTurnInIDs || questNPCTurnInIDs.Count() == 0)
			return;

		vector playerPos = m_Quest.GetPlayer().GetPosition();
		vector npcPos = m_Quest.GetClosestQuestNPCPosition(questNPCTurnInIDs, playerPos);
		CreateObjectiveMarker(npcPos, markerName);
	}
#endif

	protected bool HasAnyCollectionCompleted()
	{
		foreach (string typeName, int needed: m_ObjectiveItemsMap)
		{
			int collectionItemCount = 0;
			foreach (ExpansionQuestObjectiveItem objItem: m_ObjectiveItems)
			{
				if (objItem.GetClassName() == typeName)
				{
					int amount = objItem.GetItem().Expansion_GetStackAmount();
					collectionItemCount += amount;
				}
			}

			if (collectionItemCount >= needed)
				return true;
		}

		return false;
	}

#ifdef EXPANSIONMODMARKET
	//! Add the delivered delivery items to the market zone if there is one nearby.
	protected void AddItemsToMarketZone(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

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
		ObjectivePrint("Market zone pos: " + zonePos.ToString());
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
				ObjectivePrint("Item " + name + " is not a market item. Skip..");
				continue;
			}

			serverData.AddQuestMarketItem(zonePos, name, amount);
		}

		serverData.Save();
	}
#endif

	protected void DestinationCheck()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		float currentDistance;
		if (!GetQuest().GetQuestConfig().IsGroupQuest())
		{
			vector playerPos = GetQuest().GetPlayer().GetPosition();
			currentDistance = vector.Distance(playerPos, m_Position);
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			//! Set the position of the group member that has the shortest distance to the target location
			//! as our current position if the quest is a group quest.
			array<vector> groupMemberPos = new array<vector>;
			set<string> memberUIDs = m_Quest.GetPlayerUIDs();
			foreach (string memberUID: memberUIDs)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(memberUID);
				if (!groupPlayer)
					continue;

				groupMemberPos.Insert(groupPlayer.GetPosition());
			}

			float smallestDistance;
			int posIndex;
			bool firstSet = false;
			for (int p = 0; p < groupMemberPos.Count(); p++)
			{
				vector pos = groupMemberPos[p];
				float dist = vector.Distance(pos, m_Position);
				if (!firstSet)
				{
					smallestDistance = dist;
					posIndex = p;
					firstSet = true;
				}
				else if (firstSet && dist < smallestDistance)
				{
					smallestDistance = dist;
					posIndex = p;
				}
			}

			currentDistance = vector.Distance(groupMemberPos[posIndex], m_Position);
		}
	#endif

		float maxDistance = m_Config.GetMaxDistance();
		if (currentDistance <= maxDistance)
		{
			ObjectivePrint("End and return TRUE");
			SetReachedLocation(true);
			return;
		}

		ObjectivePrint("End and return FALSE");
		SetReachedLocation(false);
	}
	
	protected void ObjectiveCheck()
	{
		DestinationCheck();
		CheckQuestPlayersForObjectiveItems();
		UpdateDeliveryData();		
		m_Quest.QuestCompletionCheck(true);
	}
};
