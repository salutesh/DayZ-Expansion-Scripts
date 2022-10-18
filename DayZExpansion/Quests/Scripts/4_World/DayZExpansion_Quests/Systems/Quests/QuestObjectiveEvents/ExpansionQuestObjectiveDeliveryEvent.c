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
	protected autoptr array<ItemBase> ObjectiveItems = new array<ItemBase>;
	protected float m_UpdateQueueTimer = 0;
	protected const float UPDATE_TICK_TIME = 2.0;

	override bool OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		if (!super.OnStart())
			return false;
		
		if (!DeliveryEventStart(false))
			return false;

		return true;
	}

	override bool OnTurnIn()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnTurnIn");
	#endif

		if (!super.OnTurnIn())
			return false;
		
		for (int i = ObjectiveItems.Count() - 1; i >= 0; i--)
		{
			ItemBase item = ObjectiveItems[i];
			GetGame().ObjectDelete(item);
			ObjectiveItems.RemoveOrdered(i);
		}
		
		foreach (ItemBase obj: ObjectiveItems)
		{
			GetGame().ObjectDelete(obj);
		}

		return true;
	}

	override bool CompletionCheck()
	{
		if (!HasAllObjectivesItems())
			return false;

		return true;
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

	/*override bool OnCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif

		foreach (ItemBase item: ObjectiveItems)
		{
			GetGame().ObjectDelete(item);
		}

		return super.OnCleanup();
	}*/

	override bool OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif
		
		if (!super.OnCancel())
			return false;

		foreach (ItemBase item: ObjectiveItems)
		{
			GetGame().ObjectDelete(item);
		}

		return true;
	}

	protected bool DeliveryEventStart(bool continues = false)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "DeliveryEventStart");
	#endif

		if (!GetObjectiveConfig())
		{
			Error(ToString() + "::DeliveryEventStart - Could not get quest configuration!");
			return false;
		}

		if (continues)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::DeliveryEventStart - Player continues quest. Check for objective items..");
		#endif
			array<ItemBase> playerDeliveryItems = GetPlayerDeliveryItems();			
			if (playerDeliveryItems && playerDeliveryItems.Count() > 0 && playerDeliveryItems.Count() == GetObjectiveConfig().GetDeliveries().Count())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::DeliveryEventStart - Player has objective items, add them to ObjectiveItems array..");
			#endif
				foreach (ItemBase playerDeliverItem: playerDeliveryItems)
				{
					ObjectiveItems.Insert(playerDeliverItem);
				}
			}
			else if (playerDeliveryItems && playerDeliveryItems.Count() > 0 && playerDeliveryItems.Count() < GetObjectiveConfig().GetDeliveries().Count())
			{
				Error(ToString() + "::DeliveryEventStart - Player continued quest and has not all of the delivery items!");
				foreach (ItemBase playerItem: playerDeliveryItems)
				{
					GetGame().ObjectDelete(playerItem);
				}
	
				return false;
			}
			else if (!playerDeliveryItems || playerDeliveryItems.Count() == 0)
			{
				Error(ToString() + "::DeliveryEventStart - Player continued quest and has none of the delivery items!");
				return false;
			}
		}
		else
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::DeliveryEventStart - Player started quest. Spawn objective items..");
		#endif
			
			PlayerBase player = GetQuest().GetPlayer();
			if (!player)
			{
				Error(ToString() + "::DeliveryEventStart - Could not get quest player!");
				return false;
			}
		
			vector position = player.GetPosition();
			vector orientation = player.GetOrientation();
			
			foreach (ExpansionQuestObjectiveDelivery delivery: GetObjectiveConfig().GetDeliveries())
			{
				EntityAI playerEntity = player;
				Object object = Spawn(delivery, player, playerEntity, position, orientation);
				if (!object)
					continue;
	
				ItemBase item;
				if (!Class.CastTo(item, object))
				{
					GetGame().ObjectDelete(object);
					continue;
				}
	
				item.SetQuestID(GetQuest().GetQuestConfig().GetID());
				ObjectiveItems.Insert(item);
			}
		}
		
	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();

		if (markerName != string.Empty || position != vector.Zero)
			GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif

		return true;
	}

	Object Spawn(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, delivery.GetAmount());
		return obj;
	}

	bool HasAllObjectivesItems()
	{
		if (!ObjectiveItems.Count())
			return false;

		foreach (ItemBase item: ObjectiveItems)
		{
			if (!item || !item.GetHierarchyRootPlayer())
				return false;
			
			PlayerBase parentPlayer;
			if (!Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()))
				return false;
			
			if (parentPlayer != GetQuest().GetPlayer())
				return false;
		}

		return true;
	}

#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		super.OnRecreateClientMarkers();

		if (GetQuest().GetQuestState() == ExpansionQuestState.STARTED)
		{
			vector position = GetObjectiveConfig().GetPosition();
			string markerName = GetObjectiveConfig().GetMarkerName();

			if (markerName != string.Empty || position != vector.Zero)
				GetQuest().CreateClientMarker(position, markerName);
		}
	}
#endif

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{
			m_UpdateQueueTimer = 0.0;

			vector position = GetObjectiveConfig().GetPosition();
			if (position == vector.Zero)
				return;

			float currentDistanceSq;

			if (!GetQuest().IsGroupQuest() && GetQuest().GetPlayer())
			{
				vector playerPos = GetQuest().GetPlayer().GetPosition();
				currentDistanceSq = vector.DistanceSq(playerPos, position);
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest() && GetQuest().GetGroup())
			{
				//! Set the position of the group member that has the shortest distance to the target location
				//! as our current position if the quest is a group quest.
				ExpansionPartyData group = GetQuest().GetGroup();
				if (!group)
					return;

				currentDistanceSq = int.MAX;
				auto groupPlayers = group.GetPlayers();
				foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
				{
					if (!playerGroupData)
						continue;

					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
					if (!groupPlayer)
						continue;

					vector pos = groupPlayer.GetPosition();
					float distSq = vector.DistanceSq(pos, position);
					if (distSq < currentDistanceSq)
					{
						currentDistanceSq = distSq;
					}
				}
			}
		#endif
			else
			{
				return;
			}

			float maxDistanceSq = Math.Pow(GetObjectiveConfig().GetMaxDistance(), 2);
			if (maxDistanceSq < 100.0)
				maxDistanceSq = 100.0;

			bool objectiveCondition = currentDistanceSq <= maxDistanceSq && HasAllObjectivesItems();
			if (objectiveCondition && !IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Complete!");
			#endif
				SetCompleted(true);
				OnComplete();
			}
			else if (!objectiveCondition && IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Incomplete!");
			#endif
				SetCompleted(false);
				OnIncomplete();
			}
		}
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}

	array<ItemBase> GetPlayerDeliveryItems()
	{
		array<ItemBase> deliveryItems = new array<ItemBase>;
		array<string> deliveryTypeNames = new array<string>;
		foreach (ExpansionQuestObjectiveDelivery delivery: GetObjectiveConfig().GetDeliveries())
		{
			deliveryTypeNames.Insert(delivery.GetClassName());
		}

		PlayerBase player = GetQuest().GetPlayer();
		array<EntityAI> items = new array<EntityAI>;
	   	player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);

		foreach (EntityAI item : items)
		{
			if (deliveryTypeNames.Find(item.GetType()) > -1)
			{
				ItemBase itemIB;
				if (Class.CastTo(itemIB, item))
				{
					if (itemIB.GetQuestID() > -1 && itemIB.GetQuestID() == GetQuest().GetQuestConfig().GetID())
					{
					#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
						Print(ToString() + "::GetPlayerDeliveryItems - Found objective item: " + itemIB.GetType() + " | Quest ID: " + itemIB.GetQuestID());
					#endif
						deliveryItems.Insert(itemIB);
					}
				}
			}
		}

		return deliveryItems;
	}
};