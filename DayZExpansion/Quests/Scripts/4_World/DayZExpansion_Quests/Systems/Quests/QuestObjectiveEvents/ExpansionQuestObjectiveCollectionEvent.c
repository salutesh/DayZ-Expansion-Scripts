/**
 * ExpansionQuestObjectiveCollectionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionEvent: ExpansionQuestObjectiveCollectItemEventBase
{
	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent OnEventStart
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

		if (!m_ObjectiveTrigger)
			CreateTrigger();

		CheckQuestPlayersForObjectiveItems();
		UpdateDeliveryData();

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent OnContinue
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
		UpdateDeliveryData();
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectItemEventBase CheckQuestPlayersForObjectiveItems
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

	#ifdef EXPANSIONMODMARKET
		AddItemsToServerData(playerUID);
	#endif

		DeleteObjectiveItems();

		ObjectivePrint(ToString() + "::OnTurnIn - End and return TRUE.");

		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent OnCleanup
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
	// ExpansionQuestObjectiveCollectionEvent GetObjectiveType
	// -----------------------------------------------------------
	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent HasDynamicState
	// -----------------------------------------------------------
	override bool HasDynamicState()
	{
		return true;
	}

	// -----------------------------------------------------------
	// ExpansionQuestObjectiveCollectionEvent QuestDebug
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