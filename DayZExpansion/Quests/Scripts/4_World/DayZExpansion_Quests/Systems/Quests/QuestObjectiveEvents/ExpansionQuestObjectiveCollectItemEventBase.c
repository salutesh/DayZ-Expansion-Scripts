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

class ExpansionQuestObjectiveCollectItemEventBase: ExpansionQuestObjectiveEventBase
{
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected bool m_DestinationReached;
	protected ref map<string, int> m_ObjectiveItemsMap;
	protected ref map<string, int> m_ObjectiveInventoryItemsMap;
	protected ref array<EntityAI> m_ObjectiveItems;
	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected int m_ObjectiveItemsAmount = 0;
	protected int m_ObjectiveItemsCount = 0;
	protected int m_UpdateCount = 0;

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase Constructor
	// -----------------------------------------------------------
	void ExpansionQuestObjectiveCollectItemEventBase(ExpansionQuest quest)
	{
		if (!m_ObjectiveItems)
			m_ObjectiveItems = new array<EntityAI>;

		if (!m_ObjectiveItemsMap)
			m_ObjectiveItemsMap = new map<string, int>;

		if (!m_ObjectiveInventoryItemsMap)
			m_ObjectiveInventoryItemsMap = new map<string, int>;

		if (!m_DeliveryData)
			m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase Destructor
	// -----------------------------------------------------------
	void ~ExpansionQuestObjectiveCollectItemEventBase()
	{
		m_ObjectiveItems.Clear();
		m_ObjectiveItemsMap.Clear();
		m_ObjectiveInventoryItemsMap.Clear();
		m_DeliveryData.Clear();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase UpdateObjectiveItemsMap
	// -----------------------------------------------------------
	//! Populate objective item map with the data from the objectives deliveries configuration
	protected void UpdateObjectiveItemsMap()
	{
		ObjectivePrint(ToString() + "::UpdateObjectiveItemsMap - Start");

		if (!m_ObjectiveConfig || !m_ObjectiveConfig.GetDeliveries() || m_ObjectiveConfig.GetDeliveries().Count() == 0)
			return;

		//m_ObjectiveItemsMap.Clear();

		array<ref ExpansionQuestObjectiveDelivery> deliveries = m_ObjectiveConfig.GetDeliveries();
		foreach (ExpansionQuestObjectiveDelivery objectiveDelivery: deliveries)
		{
			string name = objectiveDelivery.GetClassName(); //! Type name of the item of current delivery item.
			int amount = objectiveDelivery.GetAmount(); //! Needed item amount of current delivery item.
			m_ObjectiveItemsAmount += amount;

			int current;
			if (m_ObjectiveItemsMap.Find(name, current))
			{
				int newAmount = current + amount;
				m_ObjectiveItemsMap.Set(name, newAmount);
			}
			else
			{
				m_ObjectiveItemsMap.Insert(name, amount);
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
	// ExpansionQuestObjectiveCollectItemEventBase UpdateObjectivesInventoryItemsMap
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

		foreach (EntityAI deliveryItem: m_ObjectiveItems)
		{
			string typeName = deliveryItem.GetType();
			int deliveryItemCount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(deliveryItem);
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
	// ExpansionQuestObjectiveCollectItemEventBase UpdateDeliveryData
	// -----------------------------------------------------------
	protected void UpdateDeliveryData()
	{
		ObjectivePrint(ToString() + "::UpdateDeliveryData - Start");

		if (!m_ObjectiveInventoryItemsMap || m_ObjectiveInventoryItemsMap.Count() == 0)
		{
			Print(ToString() + "::UpdateDeliveryData - Objective inventory items map is empty. Skip..");
			return;
		}

		if (!m_DeliveryData)
			m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;

		m_DeliveryData.Clear();

		int index = 0;
		foreach (string deliveryName, int deliveryAmount: m_ObjectiveInventoryItemsMap)
		{
			ObjectivePrint(ToString() + "::UpdateDeliveryData - [" + index + "] " + deliveryName + " | Amount: " + deliveryAmount);
			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(index, deliveryAmount);
			m_DeliveryData.Insert(deliveryData);
			index++;
		}

		m_Quest.UpdateQuest();

		ObjectivePrint(ToString() + "::UpdateDeliveryData - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase OnInventoryItemLocationChange
	// -----------------------------------------------------------
	void OnInventoryItemLocationChange(ItemBase item, Man player, string state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - State: " + state);

		string typeName = item.GetType();
		int amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
		int needed;
		int newAmount;
		int index = -1;

		if (!m_ObjectiveItemsMap.Find(typeName, needed))
			return;

		if (state == "INV_EXIT")
		{
			index = m_ObjectiveItems.Find(item);
			if (index > -1)
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Remove " + item.GetType() + " from objective items. Item amount: " + amount);
				newAmount = m_ObjectiveItemsCount - amount;
				if (newAmount >= 0)
					m_ObjectiveItemsCount = newAmount;
				m_ObjectiveItems.Remove(index);
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items count: " + m_ObjectiveItemsCount);
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items in entity array: " + m_ObjectiveItems.Count());
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
				
				newAmount = m_ObjectiveItemsCount + amount;
				if (newAmount <= needed)
				{
					ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Add " + item.GetType() + " to objective items. Item amount: " + amount);
					m_ObjectiveItemsCount += amount;
					m_ObjectiveItems.Insert(item);
					ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items count: " + m_ObjectiveItemsCount);
					ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Objective items in entity array: " + m_ObjectiveItems.Count());
				}
			}
			else
			{
				ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Item " + item.GetType() + " is already in objective items.");
			}
		}

	#ifdef EXPANSIONMODNAVIGATION
		if (m_ObjectiveItemsCount == m_ObjectiveItemsAmount)
		{
			OnRecreateClientMarkers();
		}
	#endif

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

#ifdef EXPANSIONMODNAVIGATION
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase OnRecreateClientMarkers
	// -----------------------------------------------------------
	override void OnRecreateClientMarkers()
	{
		vector playerPos = m_Quest.GetPlayer().GetPosition();
		Object target = m_Quest.GetQuestModule().GetClosestQuestNPCForQuest(m_Quest.GetQuestConfig().GetQuestTurnInIDs(), playerPos);
		if (!target)
			return;

		vector markerPosition = target.GetPosition();
		string markerName = m_ObjectiveConfig.GetMarkerName();
		if (markerName != string.Empty || markerPosition != vector.Zero)
			m_Quest.CreateClientMarker(markerPosition, markerName);
	}
#endif

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase UpdateObjective
	// -----------------------------------------------------------
	protected void UpdateObjective()
	{
		ObjectivePrint(ToString() + "::UpdateObjective - Start");

		UpdateObjectivesInventoryItemsMap();
		UpdateDeliveryData();
		m_Quest.UpdateQuest();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::UpdateObjective - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase CanComplete
	// -----------------------------------------------------------
	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint(ToString() + "::CanComplete - m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);
		ObjectivePrint(ToString() + "::CanComplete - m_DestinationReached: " + m_DestinationReached);

		bool conditionsResult = (m_ObjectiveItemsCount == m_ObjectiveItemsAmount) && m_DestinationReached;
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase SetReachedLocation
	// -----------------------------------------------------------
	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + ":: SetReachedLocation - State: " + state);
		m_DestinationReached = state;
		m_Quest.QuestCompletionCheck();
		ObjectivePrint(ToString() + "::CanComplete - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase GetLocationState
	// -----------------------------------------------------------
	bool GetLocationState()
	{
		return m_DestinationReached;
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
	// ExpansionQuestObjectiveCollectItemEventBase CreateTrigger
	// -----------------------------------------------------------
	protected void CreateTrigger()
	{
		ObjectivePrint(ToString() + ":: CreateTrigger - Start");

		if (!m_Quest || !m_Quest.GetQuestConfig() || !m_Quest.GetQuestConfig().GetQuestTurnInIDs() || !m_Quest.GetPlayer())
			return;

		vector playerPos = m_Quest.GetPlayer().GetPosition();
		array<int> questNPCTurnInIDs = m_Quest.GetQuestConfig().GetQuestTurnInIDs();
		if (playerPos == vector.Zero || !questNPCTurnInIDs || questNPCTurnInIDs.Count() == 0)
			return;

		Object npcObj = m_Quest.GetQuestModule().GetClosestQuestNPCForQuest(questNPCTurnInIDs, playerPos);
		if (!npcObj)
			return;

		vector npcPos = npcObj.GetPosition();
		if (npcPos == vector.Zero)
			return;

		int triggerDistance = 10;
		if (m_ObjectiveConfig.GetMaxDistance() > 0)
			triggerDistance = m_ObjectiveConfig.GetMaxDistance();

		if (!Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", npcPos, ECE_NONE)))
			return;

		m_ObjectiveTrigger.SetPosition(npcPos);
		m_ObjectiveTrigger.SetObjectiveData(this);

		ObjectivePrint(ToString() + ":: CreateTrigger - Created objective trigger at position: " + npcPos + " | Trigger sphere distance: " + triggerDistance);
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase GetDeliveryData
	// -----------------------------------------------------------
	array<ref ExpansionQuestDeliveryObjectiveData> GetDeliveryData()
	{
		return m_DeliveryData;
	}
};