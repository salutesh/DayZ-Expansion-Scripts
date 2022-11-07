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
	protected float m_UpdateQueueTimer;
	protected const float UPDATE_TICK_TIME = 2.0;
	protected bool m_CraftingState;
	protected bool m_CallLater;
	protected ItemBase m_CraftedItem;

	void OnItemsCrafted(PlayerBase player, array<ItemBase> spawned_objects)
	{
		ObjectivePrint(ToString() + "::OnItemsCrafted - Start");

		if (CraftedItemsCheck(spawned_objects))
		{
			m_CraftingState = true;
			SetCompleted(true);
			OnComplete();
		}

		ObjectivePrint(ToString() + "::OnItemsCrafted - End");
	}

	override bool OnEventStart(bool continues = false)
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

		if (continues)
		{
			array<EntityAI> items = new array<EntityAI>;
			array<ItemBase> itemsIB = new array<ItemBase>;
			GetQuest().GetPlayer().GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

			foreach (EntityAI item: items)
			{
				ItemBase itemIB = ItemBase.Cast(item);
				itemsIB.Insert(itemIB);
			}

			if (!CraftedItemsCheck(itemsIB, continues))
				CancelQuest();
		}

		ObjectivePrint(ToString() + "::OnEventStart - End");

		return true;
	}

	protected bool CraftedItemsCheck(array<ItemBase> items, bool continued = false)
	{
		ObjectivePrint(ToString() + "::CraftedItemsCheck - Start");

		if (!items)
			return false;

		foreach (ItemBase item: items)
		{
			if (!item)
				continue;

			ObjectivePrint(ToString() + "::CraftedItemsCheck - Checking item: " + item.GetType());
			if (ExpansionStatic.IsAnyOf(item, GetObjectiveConfig().GetItemNames(), true))
			{
				if (!continued)
				{
					m_CraftedItem = item;
					m_CraftedItem.SetQuestID(GetQuest().GetQuestConfig().GetID());

					ObjectivePrint(ToString() + "::CraftedItemsCheck - Found crafted item: " + m_CraftedItem.GetType());
					ObjectivePrint(ToString() + "::CraftedItemsCheck - End - TRUE");

					return true;
				}
				else
				{
					if (item.GetQuestID() == GetQuest().GetQuestConfig().GetID())
					{
						m_CraftedItem = item;
						ObjectivePrint(ToString() + "::CraftedItemsCheck - Found crafted item: " + m_CraftedItem.GetType());
						ObjectivePrint(ToString() + "::CraftedItemsCheck - End - TRUE");

						return true;
					}
				}

				ObjectivePrint(ToString() + "::CraftedItemsCheck - Found crafted item: " + m_CraftedItem.GetType());
				ObjectivePrint(ToString() + "::CraftedItemsCheck - End - TRUE");

			}
		}

		ObjectivePrint(ToString() + "::CraftedItemsCheck - End - FALSE");
		return false;
	}
	
	protected bool CraftedItemsCheck(array<EntityAI> items, bool continued = false)
	{
		ObjectivePrint(ToString() + "::CraftedItemsCheck - Start");

		if (!items)
			return false;

		foreach (EntityAI item: items)
		{
			if (!item)
				continue;
			
			ItemBase itemIB;
			if (!Class.CastTo(itemIB, item))
				continue;

			ObjectivePrint(ToString() + "::CraftedItemsCheck - Checking item: " + itemIB.GetType());
			if (ExpansionStatic.IsAnyOf(itemIB, GetObjectiveConfig().GetItemNames(), true))
			{
				if (itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
				{
					m_CraftedItem = itemIB;
					ObjectivePrint(ToString() + "::CraftedItemsCheck - Found crafted item: " + m_CraftedItem.GetType());
					ObjectivePrint(ToString() + "::CraftedItemsCheck - End - TRUE");

					return true;
				}

				ObjectivePrint(ToString() + "::CraftedItemsCheck - Found crafted item: " + m_CraftedItem.GetType());
				ObjectivePrint(ToString() + "::CraftedItemsCheck - End - TRUE");

			}
		}

		ObjectivePrint(ToString() + "::CraftedItemsCheck - End - FALSE");
		return false;
	}

	override bool OnTurnIn()
	{
		ObjectivePrint(ToString() + "::OnTurnIn - Start");

		if (!super.OnTurnIn())
			return false;

		if (m_CraftedItem)
		{
			ObjectivePrint(ToString() + "::OnTurnIn - Cleanup crafted item: " + m_CraftedItem.GetType());
			GetGame().ObjectDelete(m_CraftedItem);
		}

		ObjectivePrint(ToString() + "::OnTurnIn - End");

		return true;
	}

	bool GetCraftingState()
	{
		return m_CraftingState;
	}

	void SetCraftingState(bool state)
	{
		m_CraftingState = state;
	}
	
	bool HasPlayerCraftedItem()
	{
		array<EntityAI> items = new array<EntityAI>;
		if (m_PlayerEntityInventory && m_PlayerEntityInventory.HasItem(m_CraftedItem.GetType(), items))
		{
			return CraftedItemsCheck(items);
		}
		
		return false;
	}
	
#ifdef EXPANSIONMODGROUPS
	bool HasGroupCraftedItem()
	{
		array<EntityAI> items = new array<EntityAI>;
		if (m_GroupEntityInventory && m_GroupEntityInventory.HasItem(m_CraftedItem.GetType(), items))
		{			
			return CraftedItemsCheck(items);
		}
		
		return false;
	}
	
	bool HasCraftedGroupItem(string playerUID)
	{
		if (m_CraftedItem.GetHierarchyRootPlayer() && m_CraftedItem.GetHierarchyRootPlayer().GetIdentity())
		{
			if (m_CraftedItem.GetHierarchyRootPlayer().GetIdentity().GetId() == playerUID)
				return true;
		}

		return false;
	}
#endif
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{			
			if (!GetQuest().IsGroupQuest())
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
				EnumeratePlayerInventory(player);
				
				bool conditionCheck = HasPlayerCraftedItem();
				if (!conditionCheck && IsCompleted())
				{
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
					m_CraftingState = true;
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());
				
				bool conditionCheckGroup = HasGroupCraftedItem();
				if (!conditionCheckGroup && IsCompleted())
				{
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheckGroup && !IsCompleted())
				{
					m_CraftingState = true;
					SetCompleted(true);
					OnComplete();
				}
			}
		#endif

			m_UpdateQueueTimer = 0.0;
		}
	}

	override bool HasDynamicState()
	{
		return true;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.CRAFTING;
	}
};