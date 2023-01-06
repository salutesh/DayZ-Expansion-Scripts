/**
 * ExpansionQuestObjectiveDeliveryEventBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveDeliveryEventBase: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer;
	protected const float UPDATE_TICK_TIME = 2.0;

	protected ref array<ref ExpansionQuestDeliveryObjectiveData> m_DeliveryData;
	protected int m_UpdateCount;
	protected bool m_MarkerCreated;

	void ExpansionQuestObjectiveDeliveryEventBase(ExpansionQuest quest)
	{
		m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
	}

	override bool OnIncomplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnIncomplete");
	#endif
		
		GetQuest().CompletionCheck();

		return true;
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override void OnRecreateClientMarkers()
	{
		if (GetQuest().GetQuestConfig().GetQuestTurnInIDs().Count() > 0 && !GetQuest().GetQuestConfig().IsAutocomplete())
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

				if (!HasAnyObjectiveGroupItem(playerGroupData.GetID()))
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

	bool HasAllObjectiveItems()
	{
		ObjectivePrint(ToString() + "::HasAllObjectiveItems - Start");

		if (!m_PlayerItems)
			m_PlayerItems = new array<EntityAI>;
		else
			m_PlayerItems.Clear();

		if (!m_DeliveryData)
			m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		else
			m_DeliveryData.Clear();

		int overallCount = 0; //! Overall inventory items count of all collection items.
		int overallNeeded = 0; //! Overall needed item amount of all collection items.
		int index = 0; //! Current collections array entry index;
		array<ref ExpansionQuestObjectiveDelivery> collections = GetObjectiveConfig().GetDeliveries();
		foreach (ExpansionQuestObjectiveDelivery collection: collections)
		{
			string name = collection.GetClassName(); //! Type name of the needed item of current collection item.
			int needed = collection.GetAmount(); //! Overall needed item amount of current collection item.
			int overallItemCount = 0; //! Overall inventroy item count of current collection item.

			ObjectivePrint(ToString() + "::HasAllObjectiveItems - Check for item: " + name + " | Amount needed: " + needed);

			overallNeeded += needed;

			array<EntityAI> items = new array<EntityAI>;
			TStringArray typeNames = new TStringArray;
			typeNames.Insert(name);
			
			if (m_PlayerEntityInventory && m_PlayerEntityInventory.HasAnyOf(typeNames, items))
			{
				foreach (EntityAI item: items)
				{
					int count = GetItemAmount(item); //! Get stack size of current item
					if (overallItemCount < needed && count <= needed)
					{
						ObjectivePrint(ToString() + "::HasAllObjectiveItems - Checking item: " + item.GetType() + " | Item count: " + count + " | Amount needed: " + needed);
						overallItemCount += count; //! Add current item count to overall inventroy item count of current collection item.
						m_PlayerItems.Insert(item);
					}

					if (overallItemCount == needed)
						break;
				}
			}
			
			typeNames.Clear();
			overallCount += overallItemCount; //! Add current overall inventroy item count to overall collection inventory items count.

			ObjectivePrint(ToString() + "::HasAllObjectiveItems - Item: " + name + " | Inventory count: " + overallItemCount);

			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(index, overallItemCount);
			m_DeliveryData.Insert(deliveryData);
			index++;
		}

		ObjectivePrint(ToString() + "::HasAllObjectiveItems - Counts - Update count: " + m_UpdateCount + " | Overall count: " + overallCount);
		if (m_UpdateCount != overallCount)
		{
			m_UpdateCount = overallCount;
			GetQuest().UpdateQuest();
		}

		ObjectivePrint(ToString() + "::HasAllObjectiveItems - Counts - Overall count: " + overallCount + " | Overall needed: " + overallNeeded);
		if (overallCount >= overallNeeded)
		{
			ObjectivePrint(ToString() + "::HasAllObjectiveItems - End - TRUE");
			return true;
		}

		ObjectivePrint(ToString() + "::HasAllObjectiveItems - End - FALSE");

		return false;
	}

#ifdef EXPANSIONMODGROUPS
	bool HasGroupAllObjectiveItems()
	{
		if (!m_GroupItems)
			m_GroupItems = new array<EntityAI>;
		else
			m_GroupItems.Clear();

		if (!m_DeliveryData)
			m_DeliveryData = new array<ref ExpansionQuestDeliveryObjectiveData>;
		else
			m_DeliveryData.Clear();

		int overallCount = 0; //! Overall inventory items count of all collection items.
		int overallNeeded = 0; //! Overall needed item amount of all collection items.
		int index = 0; //! Current collections array entry index;
		array<ref ExpansionQuestObjectiveDelivery> collections = GetObjectiveConfig().GetDeliveries();
		foreach (ExpansionQuestObjectiveDelivery collection: collections)
		{
			string name = collection.GetClassName(); //! Type name of the needed item of current collection item.
			int needed = collection.GetAmount(); //! Overall needed item amount of current collection item.
			int overallItemCount = 0; //! Overall inventroy item count of current collection item.

			ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - Check for item: " + name + " | Amount needed: " + needed);

			overallNeeded += needed;

			array<EntityAI> items = new array<EntityAI>;
			TStringArray typeNames = new TStringArray;
			typeNames.Insert(name);
			
			if (m_GroupEntityInventory && m_GroupEntityInventory.HasAnyOf(typeNames, items))
			{
				foreach (EntityAI item: items)
				{
					int count = GetItemAmount(item);
					if (overallItemCount < needed && count <= needed)
					{
						ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - Checking item: " + item.GetType() + " | Item count: " + count);
						overallItemCount += count;
						m_GroupItems.Insert(item);
					}

					if (overallItemCount == needed)
						break;
				}
			}
			
			typeNames.Clear();
			overallCount += overallItemCount;

			ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - Item: " + name + " | Inventory count: " + overallItemCount);

			ExpansionQuestDeliveryObjectiveData deliveryData = new ExpansionQuestDeliveryObjectiveData();
			deliveryData.SetFromDelivery(index, overallItemCount);
			m_DeliveryData.Insert(deliveryData);
			index++;
		}

		ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - Counts - Update count: " + m_UpdateCount + " | Overall count: " + overallCount);
		if (m_UpdateCount != overallCount)
		{
			m_UpdateCount = overallCount;
			GetQuest().UpdateQuest();
		}

		ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - Counts - Overall count: " + overallCount + " | Overall needed: " + overallNeeded);
		if (overallCount >= overallNeeded)
		{
			ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - End - TRUE");
			return true;
		}

		ObjectivePrint(ToString() + "::HasGroupAllObjectiveItems - End - FALSE");

		return false;
	}

	bool HasAnyObjectiveGroupItem(string playerUID)
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
	
	override void OnGroupMemberJoined(string playerUID)
	{
		//! Dot create markers
	}
#endif

	override bool HasDynamicState()
	{
		return true;
	}
};