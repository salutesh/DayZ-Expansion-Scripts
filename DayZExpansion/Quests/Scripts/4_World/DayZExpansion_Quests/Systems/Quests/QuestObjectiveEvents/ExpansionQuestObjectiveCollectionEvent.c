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
	
	#ifdef EXPANSIONMODNAVIGATION
		vector playerPos = GetQuest().GetPlayer().GetPosition();
		Object target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), playerPos);
		if (!target)
			return;
		
		vector markerPosition = target.GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		if (markerName != string.Empty || markerPosition != vector.Zero)
			GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		super.OnRecreateClientMarkers();

		if (GetQuest().GetQuestState() == ExpansionQuestState.STARTED)
		{
			vector playerPos = GetQuest().GetPlayer().GetPosition();
			Object target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), playerPos);
			if (!target)
				return;
			
			vector markerPosition = target.GetPosition();
			string markerName = GetObjectiveConfig().GetMarkerName();
			if (markerName != string.Empty || markerPosition != vector.Zero)
				GetQuest().CreateClientMarker(markerPosition, markerName);
		}
	}
#endif

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
		
	override protected bool DestinationCheck()
	{
		vector position;
		float maxDistance = GetObjectiveConfig().GetMaxDistance();
		float currentDistance;
		array<vector> groupMemberPos = new array<vector>;
		Object target;
		
		if (!GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetPlayer())
		{
			vector playerPos = GetQuest().GetPlayer().GetPosition();
			target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), playerPos);
			if (!target)
				return false;
			
			position = target.GetPosition();
			currentDistance = vector.Distance(playerPos, position);
		}
	#ifdef EXPANSIONMODGROUPS
		else if (GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetGroup())
		{
			//! Set the position of the group member that has the shortest distance to the target location
			//! as our current position if the quest is a group quest.
			ExpansionPartyData group = GetQuest().GetGroup();
			if (!group)
				return false;

			array<ref ExpansionPartyPlayerData> groupPlayers = group.GetPlayers();
			foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
			{
				PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
				if (!groupPlayer)
					continue;
				
				if (!HasAnyCollectionGroupItem(playerGroupData.GetID()))
					continue;

				groupMemberPos.Insert(groupPlayer.GetPosition());
			}

			float smallestDistance;
			int posIndex;
			bool firstSet = false;
			for (int p = 0; p < groupMemberPos.Count(); p++)
			{
				vector pos = groupMemberPos[p];
				target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), pos);
				if (!target)
					continue;

				position = target.GetPosition();
				float dist = vector.Distance(pos, position);
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

			currentDistance = vector.Distance(groupMemberPos[posIndex], position);
		}
	#endif

		position[1] = GetGame().SurfaceY(position[0], position[2]);
		if (position != vector.Zero && currentDistance <= maxDistance)
			return true;

		return false;
	}
	
	#ifdef EXPANSIONMODGROUPS
	bool HasAnyCollectionGroupItem(string playerUID)
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

	protected void EnumeratePlayerInventory(PlayerBase player)
	{
		if (!player || !player.IsAlive() || !player.GetInventory())
			return;

		m_PlayerEntityInventory = new ExpansionQuestsPlayerInventory(player);
	}

#ifdef EXPANSIONMODGROUPS
	protected void EnumerateGroupInventory(ExpansionPartyData group)
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
				
				bool conditionCheck = HasAllCollectionItems() && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					CF_Log.Debug("ExpansionQuestObjectiveCollectionEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					CF_Log.Debug("ExpansionQuestObjectiveCollectionEvent::OnUpdate - COMPLETE");
				#endif
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());
				
				bool conditionCheckGroup = HasGroupAllCollectionItems() && DestinationCheck();
				if (!conditionCheckGroup && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					CF_Log.Debug("ExpansionQuestObjectiveCollectionEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheckGroup && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					CF_Log.Debug("ExpansionQuestObjectiveCollectionEvent::OnUpdate - COMPLETE");
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
					GetQuest().UpdateQuest();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest() && m_GroupItems)
			{
				if (m_UpdateCount != m_GroupItems.Count())
				{
					m_UpdateCount = m_GroupItems.Count();
					GetQuest().UpdateQuest();
				}
			}
		#endif

			m_UpdateQueueTimer = 0.0;
		}
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