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
	private float m_UpdateQueueTimer;
	private const float UPDATE_TICK_TIME = 2.0;
	private ref ExpansionQuestsPlayerInventory m_PlayerEntityInventory;
	private ref array<EntityAI> m_PlayerItems;
#ifdef EXPANSIONMODGROUPS
	private ref ExpansionQuestsGroupInventory m_GroupEntityInventory;
	private ref array<EntityAI> m_GroupItems;
#endif

	override void OnStart()
	{
		if (GetQuest().IsGroupQuest() && !GetQuest().GetGroup())
			return;

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

		super.OnStart();
	}

	override void OnComplete()
	{
		super.OnComplete();
	}

	override void OnTurnIn()
	{
		ExpansionQuestObjectiveCollection collection = GetObjectiveConfig().GetCollection();
		if (!collection)
			return;

		if (!GetQuest().IsGroupQuest())
		{
			if (m_PlayerItems.Count() != collection.GetAmount())
				return;

			for (int i = 0; i < m_PlayerItems.Count(); i++)
			{
				EntityAI item = m_PlayerItems[i];
				GetGame().ObjectDelete(item);
			}
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			if (m_GroupItems.Count() != collection.GetAmount())
				return;

			for (int g = 0; g < m_GroupItems.Count(); g++)
			{
				EntityAI groupItem = m_PlayerItems[g];
				GetGame().ObjectDelete(groupItem);
			}
		}
	#endif

		super.OnTurnIn();
	}

	override void OnContinue()
	{
		super.OnContinue();
	}

	override void OnCleanup()
	{
		super.OnCleanup();
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
			for (int i = 0; i < items.Count(); i++)
			{
				EntityAI item = items[i];
				if (CanCountItem(item, true))
				{
					m_PlayerItems.Insert(item);
					currentCount++;
				}
			}

			if (currentCount == collection.GetAmount())
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
			for (int i = 0; i < items.Count(); i++)
			{
				EntityAI item = items[i];
				if (CanCountItem(item, true))
				{
					m_GroupItems.Insert(item);
					currentCount++;
				}
			}

			if (currentCount < collection.GetAmount())
			{
				return false;
			}
		}

		return true;
	}
#endif

	private bool CanCountItem(EntityAI item, bool checkIfRuined = false)
	{
		if (checkIfRuined && item.IsRuined())
			return false;

		if (item.GetInventory())
		{
			//! Check if the item has a container and any items in it
			if (item.HasAnyCargo())
				return false;

			//! Check if any of the item's attachments has any cargo
			for (int i = 0; i < item.GetInventory().AttachmentCount(); i++)
			{
				EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(i);
				if (attachment && attachment.HasAnyCargo())
					return false;
			}

			//! Check if item is attachment that can be released
			if (item.GetInventory().IsAttachment())
				return !item.IsMagazine() && item.GetHierarchyParent().CanReleaseAttachment(item) && item.GetHierarchyParent().GetInventory().CanRemoveAttachment(item);
		}

		#ifdef EXPANSIONMODVEHICLE
		//! If this is a master key of a vehicle, don't allow to count it
		ExpansionCarKey key;
		if (Class.CastTo(key, item) && key.IsMaster())
			return false;
		#endif

	#ifdef WRDG_DOGTAGS
		Dogtag_Base dogTag;
		if (Class.CastTo(dogTag, item))
		{
		#ifdef EXPANSIONMODQUESTSDEBUG
			Print(ToString() + "::CanCountItem - Item is Dogtag");
		#endif
			int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
			Dogtag_Base playerDogTag;
			PlayerBase player = PlayerBase.Cast(dogTag.GetHierarchyRootPlayer());
			EntityAI slotItem = player.GetInventory().FindAttachment(slotId);
			if (Class.CastTo(playerDogTag, slotItem))
			{
				if (dogTag == playerDogTag)
				{
				#ifdef EXPANSIONMODQUESTSDEBUG
					Print(ToString() + "::CanCountItem - Dogtag is players dogtag! Don't add!");
				#endif
					return false;
				}
			}
		}
	#endif

		return true;
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
		if (!IsInitialized())
			return;

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			if (!GetQuest().IsGroupQuest())
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
				EnumeratePlayerInventory(player);

				if (!HasAllCollectionItems())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - TRUE");
				#endif
					SetCompleted(false);
				}
				else
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - FALSE");
				#endif
					SetCompleted(true);
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest())
			{
				EnumerateGroupInventory(GetQuest().GetGroup());

				if (!HasGroupAllCollectionItems())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - TRUE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveCollectionEvent::OnUpdate - FALSE");
				#endif
					SetCompleted(true);
					OnComplete();
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

#ifdef WRDG_DOGTAGS
	void CreateRewardFromDogTagStats(Dogtag_Base dogtag)
	{

	}
#endif

	int GetAmmount()
	{
		return GetObjectiveConfig().GetCollection().GetAmount();
	}

	int GetCount()
	{
		if (!GetQuest().IsGroupQuest() && m_PlayerItems)
		{
			return m_PlayerItems.Count();
		}
		else if (GetQuest().IsGroupQuest() && m_GroupItems)
		{
			return m_GroupItems.Count();
		}

		return -1;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.COLLECT;
	}
};