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
	private autoptr array<Object> ObjectivesObjects = new array<Object>;
	private float m_UpdateQueueTimer = 0;
	private const float UPDATE_TICK_TIME = 2.0;
	
	override void OnStart()
	{
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

			ObjectivesObjects.Insert(object);
		}

	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif

		super.OnStart();
	}

	override void OnTurnIn()
	{
		if (ObjectivesObjects.Count() > 0)
		{
			foreach (Object obj: ObjectivesObjects)
			{
				GetGame().ObjectDelete(obj);
			}
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

			ObjectivesObjects.Insert(object);
		}

	#ifdef EXPANSIONMODNAVIGATION
		vector markerPosition = GetObjectiveConfig().GetPosition();
		string markerName = GetObjectiveConfig().GetMarkerName();
		GetQuest().CreateClientMarker(markerPosition, markerName);
	#endif

		super.OnContinue();
	}

	override void OnCleanup()
	{
		if (ObjectivesObjects.Count() > 0)
		{
			foreach (Object obj: ObjectivesObjects)
			{
				GetGame().ObjectDelete(obj);
			}
		}

		super.OnCleanup();
	}

	override void OnCancel()
	{
		if (ObjectivesObjects.Count() > 0)
		{
			foreach (Object obj: ObjectivesObjects)
			{
				GetGame().ObjectDelete(obj);
			}
		}

		super.OnCancel();
	}

	Object Spawn(ExpansionQuestObjectiveDelivery delivery, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(delivery.GetClassName(), player, parent, delivery.GetAmount());
		return obj;
	}

	bool HasAllObjectivesItems()
	{
		//! Remove all quest objectives items from player/world
		for (int i = 0; i < ObjectivesObjects.Count(); i++)
		{
			Object obj = ObjectivesObjects[i];
			if (!obj)
				continue;

			ItemBase item = ItemBase.Cast(obj);
			if (item.GetHierarchyParent())
			{
				ItemBase parentItem;
				PlayerBase parentPlayer;
				if (Class.CastTo(parentItem, item.GetHierarchyParent()))
				{
					if (!Class.CastTo(parentPlayer, parentItem.GetHierarchyRootPlayer()))
					{
						return false;
					}
					else if (Class.CastTo(parentPlayer, parentItem.GetHierarchyRootPlayer()))
					{
						if (parentPlayer != GetQuest().GetPlayer())
							return false;
					}
				}
				else if (Class.CastTo(parentPlayer, item.GetHierarchyParent()) )
				{
					if (parentPlayer != GetQuest().GetPlayer())
						return false;
				}
			}
			else
			{
				return false;
			}
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
			vector position = GetObjectiveConfig().GetPosition();
			float maxDistance = GetObjectiveConfig().GetMaxDistance();
			float currentDistance;

			//! Set the position of the group member that has the shortest distance to the target location
			//! as our current position if the quest is a group quest.
			array<vector> groupMemberPos = new array<vector>;
			if (GetQuest().IsGroupQuest())
			{
			#ifdef EXPANSIONMODGROUPS
				ExpansionPartyData group = GetQuest().GetGroup();
				if (!group)
					return;

				for (int i = 0; i < group.GetPlayers().Count(); i++)
				{
					ExpansionPartyPlayerData playerGroupData = group.GetPlayers()[i];
					if (!playerGroupData)
						continue;

					PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
					if (!groupPlayer)
						continue;

					groupMemberPos.Insert(groupPlayer.GetPosition());
				}

				float smallestDistance;
				int posIndex;
				bool firstSet = false;
				for (int p = 0; p < groupMemberPos.Count(); p++)
				{
					vector pos = groupMemberPos[p];
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
					
					currentDistance = vector.Distance(groupMemberPos[posIndex], position);
				}
			#endif
			}
			else
			{
				vector playerPos = GetQuest().GetPlayer().GetPosition();
				currentDistance = vector.Distance(playerPos, position);
			}

			position[1] = GetGame().SurfaceY(position[0], position[2]);

			if (maxDistance < 10)
				maxDistance = 10.0;

			if (position != vector.Zero && currentDistance <= maxDistance && HasAllObjectivesItems() && !IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Complete!");
			#endif
				SetCompleted(true);
				OnComplete();
			}
			else if ((position != vector.Zero && currentDistance > maxDistance || !HasAllObjectivesItems()) && IsCompleted())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Incomplete!");
			#endif
				SetCompleted(false);
				OnIncomplete();
			}

			m_UpdateQueueTimer = 0.0;
		}
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.DELIVERY;
	}
};