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

class ExpansionQuestObjectiveCollectionEvent: ExpansionQuestObjectiveDeliveryEventBase
{
	override bool OnTurnIn()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnTurnIn");
	#endif

		CleanupCollectionItems();

		if (!super.OnTurnIn())
			return false;

		return true;
	}

	override bool OnEventStart(bool continues = false)
	{
		if (!GetQuest().IsGroupQuest())
		{
			if (!m_PlayerItems)
				m_PlayerItems = new array<EntityAI>;

			EnumeratePlayerInventory(GetQuest().GetPlayer());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			if (!m_GroupItems)
				m_GroupItems = new array<EntityAI>;

			EnumerateGroupInventory(GetQuest().GetGroup());
		}
	#endif

		return true;
	}

	protected void DeleteCollectionItem(EntityAI item, inout int amountToDelete)
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, item) && itemBase.Expansion_IsStackable())
		{
			itemBase.Expansion_DecreaseStackAmount(amountToDelete, true);
		}
		else
		{
			//! Everything else
			GetGame().ObjectDelete(item);
			amountToDelete--;
		}
	}

	protected void CleanupCollectionItems()
	{
		ObjectivePrint(ToString() + "::CleanupCollectionItems - Start");

		array<ref ExpansionQuestObjectiveDelivery> collections = GetObjectiveConfig().GetDeliveries();
		int amountToDelete;
		string name;
		if (!GetQuest().IsGroupQuest())
		{
			foreach (ExpansionQuestObjectiveDelivery collection: collections)
			{
				amountToDelete = collection.GetAmount();
				name = collection.GetClassName();

				ObjectivePrint(ToString() + "::CleanupCollectionItems - Search for collection Item: " + name + " | Amount to delete: " + amountToDelete);

				foreach (EntityAI item: m_PlayerItems)
				{
					ObjectivePrint(ToString() + "::CleanupCollectionItems - Check item: " + item.GetType());
					if ((name == item.GetType() || name == item.ClassName() || item.IsKindOf(name)) && amountToDelete > 0)
					{
						ObjectivePrint(ToString() + "::CleanupCollectionItems - Clean up collection item: " + item.GetType() + " | Amount: " + amountToDelete);
						DeleteCollectionItem(item, amountToDelete);
						ObjectivePrint(ToString() + "::CleanupCollectionItems - Cleaned up collection item: " + item.GetType() + " | Amount left: " + amountToDelete);
					}
				}
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetQuest().IsGroupQuest())
		{
			foreach (ExpansionQuestObjectiveDelivery groupCollection: collections)
			{
				amountToDelete = groupCollection.GetAmount();
				name = groupCollection.GetClassName();

				ObjectivePrint(ToString() + "::CleanupCollectionItems - Search for delivery Item: " + name + " | Amount to delete: " + amountToDelete);

				foreach (EntityAI groupItem: m_GroupItems)
				{
					if (name == groupItem.GetType() && amountToDelete > 0)
					{
						ObjectivePrint(ToString() + "::CleanupCollectionItems - Clean up delivery item: " + groupItem.GetType() + " | Amount: " + amountToDelete);
						DeleteCollectionItem(groupItem, amountToDelete);
						ObjectivePrint(ToString() + "::CleanupCollectionItems - Cleaned up delivery item: " + groupItem.GetType() + " | Amount left: " + amountToDelete);
					}
				}
			}
		}
	#endif

		ObjectivePrint(ToString() + "::CleanupCollectionItems - End");
	}

	array<ref ExpansionQuestDeliveryObjectiveData> GetCollectionData()
	{
		return m_DeliveryData;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			ObjectivePrint(ToString() + "::OnUpdate - Start");
			
			bool hasItems;
			bool conditionCheck;
			if (!GetQuest().IsGroupQuest())
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
				EnumeratePlayerInventory(player);
				
				hasItems = HasAllObjectiveItems();
			#ifdef EXPANSIONMODNAVIGATION
				if (hasItems && !m_MarkerCreated)
				{
					m_MarkerCreated = true;
					OnRecreateClientMarkers();
				}
				else if (!hasItems && m_MarkerCreated)
				{
					m_MarkerCreated = false;
					GetQuest().RemoveMarkers();
				}
			#endif
				
				conditionCheck = hasItems && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
					ObjectivePrint(ToString() + "::OnUpdate - INCOMPLETE");
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
					ObjectivePrint(ToString() + "::OnUpdate - COMPLETE");
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());

				hasItems = HasGroupAllObjectiveItems();
			#ifdef EXPANSIONMODNAVIGATION
				if (hasItems && !m_MarkerCreated)
				{
					m_MarkerCreated = true;
					OnRecreateClientMarkers();
				}
				else if (!hasItems && m_MarkerCreated)
				{
					m_MarkerCreated = false;
					GetQuest().RemoveMarkers();
				}
			#endif
				
				conditionCheck = hasItems && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
					ObjectivePrint(ToString() + "::OnUpdate - INCOMPLETE");
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
					ObjectivePrint(ToString() + "::OnUpdate - COMPLETE");
					SetCompleted(true);
					OnComplete();
				}
			}
		#endif

			ObjectivePrint(ToString() + "::OnUpdate - End");
			m_UpdateQueueTimer = 0.0;
		}
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};