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
	private autoptr array<ItemBase> ObjectiveItems = new array<ItemBase>;
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;
	
	override void OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif
		
		DeliveryEventStart();

		super.OnStart();
	}

	override void OnTurnIn()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnTurnIn");
	#endif
		
		foreach (ItemBase obj: ObjectiveItems)
		{
			GetGame().ObjectDelete(obj);
		}

		super.OnTurnIn();
	}

	override bool CompletionCheck()
	{
		if (!HasAllObjectivesItems())
			return false;

		return true;
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif
		
		DeliveryEventStart();

		super.OnContinue();
	}
	
	override void OnCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif
		
		if (ObjectiveItems.Count() > 0)
		{
			foreach (ItemBase item: ObjectiveItems)
			{
				GetGame().ObjectDelete(item);
			}
		}

		super.OnCleanup();
	}

	override void OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif	
		
		if (ObjectiveItems.Count() > 0)
		{
			foreach (ItemBase item: ObjectiveItems)
			{
				GetGame().ObjectDelete(item);
			}
		}

		super.OnCancel();
	}

	private void DeliveryEventStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "DeliveryEventStart");
	#endif
		
		if (!GetObjectiveConfig())
			return;

		for (int i = 0; i < GetObjectiveConfig().GetDeliveries().Count(); i++)
		{
			ExpansionQuestObjectiveDelivery delivery = GetObjectiveConfig().GetDeliveries()[i];
			if (!delivery)
				continue;

			PlayerBase player = GetQuest().GetPlayer();
			EntityAI playerEntity = player;
			vector position = player.GetPosition();
			vector orientation = player.GetOrientation();
			Object object = Spawn(delivery, player, playerEntity, position, orientation);
			if (!object)
				continue;

			ItemBase item;
			if (!Class.CastTo(item, object))
			{
				GetGame().ObjectDelete(object);
				continue;
			}
			
			item.SetIsQuestItem(true);
			ObjectiveItems.Insert(item);
		}

	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif
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
			PlayerBase parentPlayer;
			if (!Class.CastTo(parentPlayer, item.GetHierarchyRootPlayer()) || parentPlayer != GetQuest().GetPlayer())
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
			
			if (GetQuest().GetPlayer())
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
};