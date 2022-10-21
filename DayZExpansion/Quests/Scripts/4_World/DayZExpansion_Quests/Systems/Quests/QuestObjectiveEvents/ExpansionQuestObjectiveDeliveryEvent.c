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

class ExpansionQuestObjectiveDeliveryEvent: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer;
	protected const float UPDATE_TICK_TIME = 2.0;
	protected ref ExpansionQuestsPlayerInventory m_PlayerEntityInventory;
	protected ref array<EntityAI> m_PlayerItems;
#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionQuestsGroupInventory m_GroupEntityInventory;
	protected ref array<EntityAI> m_GroupItems;
#endif

	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected int m_UpdateCount;
	
	void ExpansionQuestObjectiveDeliveryEvent(ExpansionQuest quest)
	{
		m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
	}

	override bool OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		if (!super.OnStart())
			return false;

		if (!DeliveryEventStart())
			return false;

		return true;
	}

	protected bool DeliveryEventStart(bool continues = false)
	{
		if (!m_PlayerItems)
			m_PlayerItems = new array<EntityAI>;

	#ifdef EXPANSIONMODGROUPS
		if (!m_GroupItems)
			m_GroupItems = new array<EntityAI>;
	#endif

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
			EnumeratePlayerInventory(GetQuest().GetPlayer());
			if (continues && !HasAllDeliveryItems())
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			EnumerateGroupInventory(GetQuest().GetGroup());
			if (continues && !HasGroupAllDeliveryItems())
				return false;
		}
	#endif

	
	#ifdef EXPANSIONMODNAVIGATION
		vector playerPos = GetQuest().GetPlayer().GetPosition();
		Object target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), playerPos);
		if (!target)
			return false;
		
		vector markerPosition = target.GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		if (markerName != string.Empty || markerPosition != vector.Zero)
			GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif
		
		return true;
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
		if (!group)
			return;

		m_GroupEntityInventory = new ExpansionQuestsGroupInventory(group);
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

	override bool OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		if (!super.OnContinue())
			return false;

		if (!DeliveryEventStart(true))
			return false;

		return true;
	}

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
		Print(ToString() + "::CleanupDeliveryItems - Start");
		
		array<ref ExpansionQuestObjectiveDelivery> deliveries = GetObjectiveConfig().GetDeliveries();
		int amountToDelete;	
		string name;
		if (!GetQuest().IsGroupQuest())
		{
			foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
			{
				amountToDelete = delivery.GetAmount();	
				name = delivery.GetClassName();
				
				Print(ToString() + "::CleanupDeliveryItems - Search for delivery Item: " + name + " | Amount to delete: " + amountToDelete);
				
				foreach (EntityAI item: m_PlayerItems)
				{
					if (name == item.GetType() && amountToDelete > 0)
					{
						ItemBase itemIB;
						if (Class.CastTo(itemIB, item))
						{
							if (itemIB.GetQuestID() > -1 && itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
							{
								Print(ToString() + "::CleanupDeliveryItems - Clean up delivery item: " + item.GetType() + " | Amount: " + amountToDelete);
								DeleteDeliveryItem(item, amountToDelete);
								Print(ToString() + "::CleanupDeliveryItems - Cleaned up delivery item: " + item.GetType() + " | Amount left: " + amountToDelete);
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
				
				Print(ToString() + "::CleanupDeliveryItems - Search for delivery Item: " + name + " | Amount to delete: " + amountToDelete);
								
				foreach (EntityAI groupItem: m_GroupItems)
				{
					if (name == groupItem.GetType() && amountToDelete > 0)
					{
						ItemBase groupItemIB;
						if (Class.CastTo(groupItemIB, groupItem))
						{
							if (groupItemIB.GetQuestID() > -1 && groupItemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
							{
								Print(ToString() + "::CleanupDeliveryItems - Clean up delivery item: " + groupItem.GetType() + " | Amount: " + amountToDelete);
								DeleteDeliveryItem(groupItem, amountToDelete);
								Print(ToString() + "::CleanupDeliveryItems - Cleaned up delivery item: " + groupItem.GetType() + " | Amount left: " + amountToDelete);
							}
						}
					}
				}
			}
		}
	#endif
		
		Print(ToString() + "::CleanupDeliveryItems - End");
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

	bool HasAllDeliveryItems()
	{
		if (m_PlayerItems)
			m_PlayerItems.Clear();

		if (!m_PlayerItems)
			m_PlayerItems = new array<EntityAI>;
		
		if (m_DeliveryData)
			m_DeliveryData.Clear();

		int overallCount;
		int overallNeeded;
		
		array<ref ExpansionQuestObjectiveDelivery> deliveries = GetObjectiveConfig().GetDeliveries();
		int index;
		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			string name = delivery.GetClassName();
			int needed = delivery.GetAmount();
			overallNeeded += needed;

			array<EntityAI> items = new array<EntityAI>;
			if (m_PlayerEntityInventory && m_PlayerEntityInventory.HasItem(name, items))
			{
				if (!items)
				return false;

				int currentCount;
				foreach (EntityAI item: items)
				{
					if (name == item.GetType())
					{
						int itemCount = GetItemAmount(item);
						if (itemCount == needed)
						{
							overallCount += itemCount;
							ItemBase itemIB;
							if (Class.CastTo(itemIB, item))
							{
								if (itemIB.GetQuestID() > -1 && itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
								{
								#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
									Print(ToString() + "::HasAllDeliveryItems - Found objective item: " + itemIB.GetType() + " | Quest ID: " + itemIB.GetQuestID());
								#endif
									m_PlayerItems.Insert(item);
									
									ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
									deliveryData.SetFromDelivery(index, itemCount);
									m_DeliveryData.Insert(deliveryData);
								}
							}
						}
					}
				}
			}
			
			index++;
		}
		
		if (m_UpdateCount != overallCount)
		{
			m_UpdateCount = overallCount;
			GetQuest().UpdateQuest();
		}

		if (overallCount >= overallNeeded)
		{
			return true;
		}

		return false;
	}

#ifdef EXPANSIONMODGROUPS
	bool HasGroupAllDeliveryItems()
	{
		if (m_GroupItems)
			m_GroupItems.Clear();

		if (!m_GroupItems)
			m_GroupItems = new array<EntityAI>;
		
		if (m_DeliveryData)
			m_DeliveryData.Clear();

		int overallCount;
		int overallNeeded;
		
		array<ref ExpansionQuestObjectiveDelivery> deliveries = GetObjectiveConfig().GetDeliveries();
		int index;
		foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
		{
			string name = delivery.GetClassName();
			int needed = delivery.GetAmount();
			overallNeeded += needed;

			array<EntityAI> items = new array<EntityAI>;
			if (m_GroupEntityInventory && m_GroupEntityInventory.HasItem(name, items))
			{
				if (!items)
					return false;

				foreach (EntityAI item: items)
				{
					int itemCount = GetItemAmount(item);
					if (itemCount == needed)
					{
						overallCount += itemCount;
						ItemBase itemIB;
						if (Class.CastTo(itemIB, item))
						{
							if (itemIB.GetQuestID() > -1 && itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
							{
							#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
								Print(ToString() + "::HasGroupAllDeliveryItems - Found objective item: " + itemIB.GetType() + " | Quest ID: " + itemIB.GetQuestID() + " | Count: " + itemCount);
							#endif
								m_GroupItems.Insert(item);
								
								ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
								deliveryData.SetFromDelivery(index, itemCount);
								m_DeliveryData.Insert(deliveryData);
							}
						}
					}
				}
			}
			
			index++;
		}
		
		if (m_UpdateCount != overallCount)
		{
			m_UpdateCount = overallCount;
			GetQuest().UpdateQuest();
		}

		if (overallCount >= overallNeeded)
		{
			return true;
		}

		return false;
	}
#endif

	Object Spawn(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, delivery.GetAmount());
		return obj;
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

				bool conditionCheck = HasAllDeliveryItems() && DestinationCheck();
				if (!conditionCheck && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheck && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - COMPLETE");
				#endif
					SetCompleted(true);
					OnComplete();
				}
			}
		#ifdef EXPANSIONMODGROUPS
			else
			{
				EnumerateGroupInventory(GetQuest().GetGroup());
				
				bool conditionCheckGroup = HasGroupAllDeliveryItems() && DestinationCheck();
				if (!conditionCheckGroup && IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - INCOMPLETE");
				#endif
					SetCompleted(false);
					OnIncomplete();
				}
				else if (conditionCheckGroup && !IsCompleted())
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print("ExpansionQuestObjectiveDeliveryEvent::OnUpdate - COMPLETE");
				#endif
					SetCompleted(true);
					OnComplete();
				}
			}
		#endif

			m_UpdateQueueTimer = 0.0;
		}
	}
	
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
				
				if (!HasAnyDeliveryGroupItem(playerGroupData.GetID()))
					continue;

				groupMemberPos.Insert(groupPlayer.GetPosition());
			}

			float smallestDistance;
			int posIndex;
			bool firstSet = false;
			for (int p = 0; p < groupMemberPos.Count(); p++)
			{
				vector pos = groupMemberPos[p];
				target = GetQuest().GetQuestModule().GetClosestQuestNPCForQuest(GetQuest().GetQuestConfig().GetID(), playerPos);
				if (!target)
					return false;
				
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

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}
};