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

class ExpansionQuestObjectiveCollectionEvent: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer;
	protected const float UPDATE_TICK_TIME = 2.0;
	protected ref ExpansionQuestsPlayerInventory m_PlayerEntityInventory;
	protected ref array<EntityAI> m_PlayerItems;
#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionQuestsGroupInventory m_GroupEntityInventory;
	protected ref array<EntityAI> m_GroupItems;
#endif

	protected int m_UpdateCount;

	override bool OnStart()
	{
		if (!super.OnStart())
			return false;

		CollectionEventStart();

		return true;
	}

	override bool OnTurnIn()
	{
		ExpansionQuestObjectiveCollection collection = GetObjectiveConfig().GetCollection();
		if (!collection)
			return false;

		int amountToDelete = collection.GetAmount();
		if (!GetQuest().IsGroupQuest())
		{
			foreach (EntityAI item: m_PlayerItems)
			{
				if (amountToDelete > 0)
				{
					DeleteCollectionItem(item, amountToDelete);
				}

				if (amountToDelete == 0)
					break;
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetQuest().IsGroupQuest())
		{
			foreach (EntityAI groupItem: m_GroupItems)
			{
				if (amountToDelete > 0)
				{
					DeleteCollectionItem(groupItem, amountToDelete);
				}

				if (amountToDelete == 0)
					break;
			}
		}
	#endif

		if (!super.OnTurnIn())
			return false;

		return true;
	}

	override bool OnContinue()
	{
		if (!super.OnContinue())
			return false;

		CollectionEventStart();

		return true;
	}

	protected void CollectionEventStart()
	{
		if (!m_PlayerItems)
			m_PlayerItems = new array<EntityAI>;

	#ifdef EXPANSIONMODGROUPS
		if (!m_GroupItems)
			m_GroupItems = new array<EntityAI>;
	#endif

		if (!GetQuest().IsGroupQuest())
		{
			EnumeratePlayerInventory(GetQuest().GetPlayer());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			EnumerateGroupInventory(GetQuest().GetGroup());
		}
	#endif
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

	bool HasAllCollectionItems()
	{
		if (m_PlayerItems)
			m_PlayerItems.Clear();

		if (!m_PlayerItems)
			m_PlayerItems = new array<EntityAI>;

		ExpansionQuestObjectiveCollection collection = GetObjectiveConfig().GetCollection();
		if (!collection)
			return false;

		array<EntityAI> items = new array<EntityAI>;
		if (m_PlayerEntityInventory && m_PlayerEntityInventory.HasItem(collection.GetClassName(), items))
		{
			if (!items)
				return false;

			int currentCount;
			foreach (EntityAI item: items)
			{
				int itemCount = GetItemAmount(item);
				currentCount += itemCount;
				if (itemCount > 0)
				{
					m_PlayerItems.Insert(item);
				}
			}

			if (currentCount >= collection.GetAmount())
			{
				return true;
			}
		}

		return false;
	}

#ifdef EXPANSIONMODGROUPS
	bool HasGroupAllCollectionItems()
	{
		if (m_GroupItems)
			m_GroupItems.Clear();

		if (!m_GroupItems)
			m_GroupItems = new array<EntityAI>;

		ExpansionQuestObjectiveCollection collection = GetObjectiveConfig().GetCollection();
		if (!collection)
			return false;

		array<EntityAI> items = new array<EntityAI>;
		if (m_GroupEntityInventory && m_GroupEntityInventory.HasItem(collection.GetClassName(), items))
		{
			if (!items)
				return false;

			int currentCount;
			foreach (EntityAI item: items)
			{
				int itemCount = GetItemAmount(item);
				currentCount += itemCount;
				if (itemCount > 0)
				{
					m_GroupItems.Insert(item);
				}
			}

			if (currentCount >= collection.GetAmount())
			{
				return true;
			}
		}

		return false;
	}
#endif

	protected int GetItemAmount(EntityAI item)
	{
		int amount;
		ItemBase itemBase;

		if (Class.CastTo(itemBase, item))
		{
			amount = itemBase.Expansion_GetStackAmount();
		}
		else
		{
			amount = 1;
		}

		if (!MiscGameplayFunctions.Expansion_IsLooseEntity(item))
			amount = -amount;

		return amount;
	}

	void EnumeratePlayerInventory(PlayerBase player)
	{
		if (!player || !player.IsAlive() || !player.GetInventory())
			return;

		m_PlayerEntityInventory = new ExpansionQuestsPlayerInventory(player);
	}

#ifdef EXPANSIONMODGROUPS
	void EnumerateGroupInventory(ExpansionPartyData group)
	{
		m_GroupEntityInventory = new ExpansionQuestsGroupInventory(group);
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
				if (!HasAllCollectionItems() && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (HasAllCollectionItems() && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - COMPLETE");
				#endif
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());
				if (!HasGroupAllCollectionItems() && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (HasGroupAllCollectionItems() && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - COMPLETE");
				#endif
					SetCompleted(true);
					OnComplete();
				}
			}
		#endif

			if (!GetQuest().IsGroupQuest() && m_PlayerItems)
			{
				if (m_UpdateCount != m_PlayerItems.Count())
				{
					m_UpdateCount = m_PlayerItems.Count();
					GetQuest().UpdateQuestPlayersObjectiveData();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest() && m_GroupItems)
			{
				if (m_UpdateCount != m_GroupItems.Count())
				{
					m_UpdateCount = m_GroupItems.Count();
					GetQuest().UpdateQuestPlayersObjectiveData();
				}
			}
		#endif

			m_UpdateQueueTimer = 0.0;
		}
	}

	override bool CompletionCheck()
	{
		if (!GetQuest().IsGroupQuest())
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
			EnumeratePlayerInventory(player);

			if (!HasAllCollectionItems())
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			EnumerateGroupInventory(GetQuest().GetGroup());

			if (!HasGroupAllCollectionItems())
				return false;
		}
	#endif

		return true;
	}

	int GetAmount()
	{
		return GetObjectiveConfig().GetCollection().GetAmount();
	}

	int GetCount()
	{
		int count;
		int itemCount;
		if (!GetQuest().IsGroupQuest() && m_PlayerItems)
		{
			foreach (EntityAI playerItem: m_PlayerItems)
			{
				if (!playerItem)
					continue;

				itemCount = GetItemAmount(playerItem);
				count += itemCount;
			}

			return count;
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetQuest().IsGroupQuest() && m_GroupItems)
		{
			foreach (EntityAI groupItem: m_GroupItems)
			{
				itemCount = GetItemAmount(groupItem);
				count += itemCount;
			}

			return count;
		}
	#endif

		return 0;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};