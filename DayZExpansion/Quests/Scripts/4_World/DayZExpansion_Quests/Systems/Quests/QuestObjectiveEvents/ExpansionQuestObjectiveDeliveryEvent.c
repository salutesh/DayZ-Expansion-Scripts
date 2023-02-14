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

class ExpansionQuestObjectiveDeliveryEvent: ExpansionQuestObjectiveCollectItemEventBase
{
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent OnEventStart
	// -----------------------------------------------------------
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnEventStart");
	#endif

		if (!super.OnEventStart())
			return false;

		UpdateObjectiveItemsMap();

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		int objEntry = 1;
		foreach (string objectiveItem, int needed: m_ObjectiveItemsMap)
		{
			Print(ToString() + "::OnEventStart - Objective items needed: [" + objEntry + "] " + objectiveItem + " | Needed: " + needed);
			objEntry++;
		}
	#endif

		PlayerBase player = PlayerBase.GetPlayerByUID(m_Quest.GetPlayerUID());
		if (!player || !player.GetIdentity())
		{
			Error(ToString() + "::OnEventStart - Could not get quest player!");
			return false;
		}

		vector position = player.GetPosition();
		vector orientation = player.GetOrientation();
		array<ref ExpansionQuestObjectiveDelivery> deliveries = m_ObjectiveConfig.GetDeliveries();
		if (!deliveries || deliveries.Count() == 0)
		{
			Error(ToString() + "::OnEventStart - Could not get delivery configuration!");
			m_Quest.CancelQuest();
		}

		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			SpawnDeliveryItems(delivery, player, player, position, orientation);
		}

		if (!m_ObjectiveTrigger)
			CreateTrigger();

		CheckQuestPlayersForObjectiveItems();
		UpdateObjectivesInventoryItemsMap();
		UpdateDeliveryData();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase CheckQuestPlayersForObjectiveItems
	// -----------------------------------------------------------
	protected void CheckQuestPlayersForObjectiveItems()
	{
		PlayerBase player;
		ItemBase itemIB;
		array<EntityAI> items;
		int amount;
		int newAmount;

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

				if (itemIB.GetQuestID() != m_Quest.GetQuestConfig().GetID())
					continue;
				
				amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
				newAmount = m_ObjectiveItemsCount + amount;
				if (newAmount <= m_ObjectiveItemsAmount)
				{
					m_ObjectiveItemsCount += amount;
					m_ObjectiveItems.Insert(item);
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

					if (itemIB.GetQuestID() != m_Quest.GetQuestConfig().GetID())
						continue;

					amount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(itemM);
					newAmount = m_ObjectiveItemsCount + amount;
					if (newAmount <= m_ObjectiveItemsAmount)
					{
						m_ObjectiveItemsCount += amount;
						m_ObjectiveItems.Insert(itemM);
					}
				}
			}
		}
	#endif
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent OnContinue
	// -----------------------------------------------------------
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		if (!super.OnContinue())
			return false;

		UpdateObjectiveItemsMap();

		if (!m_ObjectiveTrigger)
			CreateTrigger();

		CheckQuestPlayersForObjectiveItems();
		UpdateObjectivesInventoryItemsMap();
		UpdateDeliveryData();
		m_Quest.QuestCompletionCheck();

		//! When the quest continues we call this method to check if the quest players still holding all the objective items and if not we cancel the quest.
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ObjectiveItemsCheck, 3000);

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent ObjectiveItemsCheck
	// -----------------------------------------------------------
	protected void ObjectiveItemsCheck()
	{
		ObjectivePrint(ToString() + "::ObjectiveItemsCheck - Start");
		ObjectivePrint(ToString() + "::ObjectiveItemsCheck - m_ObjectiveItemsCount: " + m_ObjectiveItemsCount);
		ObjectivePrint(ToString() + "::ObjectiveItemsCheck - m_ObjectiveItemsAmount: " + m_ObjectiveItemsAmount);

		if (m_ObjectiveItemsCount == m_ObjectiveItemsAmount)
			return;

		m_Quest.CancelQuest();

		ObjectivePrint(ToString() + "::ObjectiveItemsCheck - End and CYA!!");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent OnTurnIn
	// -----------------------------------------------------------
	override bool OnTurnIn(string playerUID)
	{
		ObjectivePrint(ToString() + "::OnTurnIn - Start");

		if (!super.OnTurnIn(playerUID))
			return false;

	#ifdef EXPANSIONMODMARKET
		AddItemsToServerData(playerUID);
	#endif

		DeleteObjectiveItems();

		ObjectivePrint(ToString() + "::OnTurnIn - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent OnCancel
	// -----------------------------------------------------------
	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif

		if (!super.OnCancel())
			return false;
		
		DeleteObjectiveItems();

		ObjectivePrint(ToString() + "::OnCancel - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent OnCleanup
	// -----------------------------------------------------------
	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif

		if (!super.OnCleanup())
			return false;

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint(ToString() + "::OnCleanup - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent SpawnDeliveryItems
	// -----------------------------------------------------------
	protected void SpawnDeliveryItems(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, EntityAI parent, vector position, vector orientation)
	{
		ObjectivePrint(ToString() + "::SpawnDeliveryItems - Start");

		if (!m_ObjectiveItems)
			m_ObjectiveItems = new array<EntityAI>;

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
	            return;
	        }

	        questItem.SetQuestID(m_Quest.GetQuestConfig().GetID());
			m_ObjectiveItems.Insert(questItem);
	    }

		ObjectivePrint(ToString() + "::SpawnDeliveryItems - End");
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent HasDynamicState
	// -----------------------------------------------------------
	override bool HasDynamicState()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveDeliveryEvent QuestDebug
	// -----------------------------------------------------------
	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		super.QuestDebug();
		ObjectivePrint("------------------------------------------------------------");
		ObjectivePrint(ToString() + "::QuestDebug - Objective destination reached: " + m_DestinationReached);
		ObjectivePrint(ToString() + "::QuestDebug - Objective items needed: " + m_ObjectiveItemsAmount);
		ObjectivePrint(ToString() + "::QuestDebug - Objective items count: " + m_ObjectiveItemsCount);
		ObjectivePrint("------------------------------------------------------------");
	#endif
	}
};