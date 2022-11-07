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

class ExpansionQuestObjectiveDeliveryEvent: ExpansionQuestObjectiveDeliveryEventBase
{
	override bool OnTurnIn()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnTurnIn");
	#endif

		CleanupDeliveryItems();

		if (!super.OnTurnIn())
			return false;

		return true;
	}

	override bool OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif

		if (!super.OnCancel())
			return false;

		CleanupDeliveryItems();

		return true;
	}
	
	override bool OnEventStart(bool continues = false)
	{
		if (!continues)
		{
			PlayerBase player = GetQuest().GetPlayer();
			if (!player)
				return false;

			vector position = player.GetPosition();
			vector orientation = player.GetOrientation();
			array<ref ExpansionQuestObjectiveDelivery> deliveries = GetObjectiveConfig().GetDeliveries();
			foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
			{
				EntityAI playerEntity = player;
				Object object = Spawn(delivery, player, playerEntity, position, orientation);
				if (!object)
					return false;;

				ItemBase item;
				if (!Class.CastTo(item, object))
				{
					GetGame().ObjectDelete(object);
					return false;
				}

				item.SetQuestID(GetQuest().GetQuestConfig().GetID());
			}
		}

		if (!GetQuest().IsGroupQuest())
		{
			if (!m_PlayerItems)
				m_PlayerItems = new array<EntityAI>;

			EnumeratePlayerInventory(GetQuest().GetPlayer());
			if (continues && !HasAllObjectiveItems())
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			if (!m_GroupItems)
				m_GroupItems = new array<EntityAI>;

			EnumerateGroupInventory(GetQuest().GetGroup());
			if (continues && !HasGroupAllObjectiveItems())
				return false;
		}
	#endif

		return true;
	}

	protected void DeleteDeliveryItem(EntityAI item, inout int amountToDelete)
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

	protected void CleanupDeliveryItems()
	{
		ObjectivePrint(ToString() + "::CleanupDeliveryItems - Start");

		array<ref ExpansionQuestObjectiveDelivery> deliveries = GetObjectiveConfig().GetDeliveries();
		int amountToDelete;
		string name;
		if (!GetQuest().IsGroupQuest())
		{
			foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
			{
				amountToDelete = delivery.GetAmount();
				name = delivery.GetClassName();

				ObjectivePrint(ToString() + "::CleanupDeliveryItems - Search for delivery Item: " + name + " | Amount to delete: " + amountToDelete);

				foreach (EntityAI item: m_PlayerItems)
				{
					if (name == item.GetType() && amountToDelete > 0)
					{
						ItemBase itemIB;
						if (Class.CastTo(itemIB, item))
						{
							if (itemIB.GetQuestID() > -1 && itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
							{
								ObjectivePrint(ToString() + "::CleanupDeliveryItems - Clean up delivery item: " + item.GetType() + " | Amount: " + amountToDelete);
								DeleteDeliveryItem(item, amountToDelete);
								ObjectivePrint(ToString() + "::CleanupDeliveryItems - Cleaned up delivery item: " + item.GetType() + " | Amount left: " + amountToDelete);
							}
						}
					}
				}
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetQuest().IsGroupQuest())
		{
			foreach (ExpansionQuestObjectiveDelivery groupDelivery: deliveries)
			{
				amountToDelete = groupDelivery.GetAmount();
				name = groupDelivery.GetClassName();

				ObjectivePrint(ToString() + "::CleanupDeliveryItems - Search for delivery Item: " + name + " | Amount to delete: " + amountToDelete);

				foreach (EntityAI groupItem: m_GroupItems)
				{
					if (name == groupItem.GetType() && amountToDelete > 0)
					{
						ItemBase groupItemIB;
						if (Class.CastTo(groupItemIB, groupItem))
						{
							if (groupItemIB.GetQuestID() > -1 && groupItemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
							{
								ObjectivePrint(ToString() + "::CleanupDeliveryItems - Clean up delivery item: " + groupItem.GetType() + " | Amount: " + amountToDelete);
								DeleteDeliveryItem(groupItem, amountToDelete);
								ObjectivePrint(ToString() + "::CleanupDeliveryItems - Cleaned up delivery item: " + groupItem.GetType() + " | Amount left: " + amountToDelete);
							}
						}
					}
				}
			}
		}
	#endif

		ObjectivePrint(ToString() + "::CleanupDeliveryItems - End");
	}

	Object Spawn(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, delivery.GetAmount());
		return obj;
	}

#ifdef EXPANSIONMODGROUPS
	bool HasAnyDeliveryGroupItem(string playerUID)
	{
		foreach (EntityAI item: m_GroupItems)
		{
			ItemBase itemIB;
			if (item && Class.CastTo(itemIB, item))
			{
				if (itemIB.GetHierarchyRootPlayer() && itemIB.GetHierarchyRootPlayer().GetIdentity())
				{
					if (itemIB.GetHierarchyRootPlayer().GetIdentity().GetId() == playerUID)
						return true;
				}
			}
		}

		return false;
	}
#endif

	array<ref ExpansionQuestDeliveryObjectiveData> GetDeliveryData()
	{
		return m_DeliveryData;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			bool hasItems;
			bool conditionCheck;
			if (!GetQuest().IsGroupQuest())
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
				EnumeratePlayerInventory(player);
				
				hasItems = HasAllObjectiveItems();
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
				
				conditionCheck = hasItems && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
					ObjectivePrint("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - INCOMPLETE");
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
					ObjectivePrint("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - COMPLETE");
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());
				
				hasItems = HasGroupAllObjectiveItems();
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
				
				conditionCheck = hasItems && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
					ObjectivePrint("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - INCOMPLETE");
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
					ObjectivePrint("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - COMPLETE");
					SetCompleted(true);
					OnComplete();
				}
			}
		#endif

			m_UpdateQueueTimer = 0.0;
		}
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}
};