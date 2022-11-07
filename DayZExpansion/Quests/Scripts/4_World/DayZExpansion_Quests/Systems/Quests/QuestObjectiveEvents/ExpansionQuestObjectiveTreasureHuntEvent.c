/**
 * ExpansionQuestObjectiveTreasureHuntEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTreasureHuntEvent: ExpansionQuestObjectiveEventBase
{
	protected float m_UpdateQueueTimer;
	protected const float UPDATE_TICK_TIME = 2.0;
	
	protected ref array<Object> LootItems = new array<Object>;
	protected UndergroundStash Stash;
	protected ExpansionQuestSeaChest Chest;
	protected vector StashPos;

	override bool OnCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif

		if (!super.OnCleanup())
			return false;

		int state = ExpansionQuestState.NONE;
		ExpansionQuestPersistentData questPlayerData = GetQuest().GetQuestModule().GetPlayerQuestDataByUID(GetQuest().GetPlayerUID());
		if (questPlayerData)
			state = questPlayerData.GetQuestStateByQuestID(GetQuest().GetQuestConfig().GetID());

		//! Only cleanup the treasure if quest is not completed
		if (state == ExpansionQuestState.STARTED)
		{
			foreach (Object obj: LootItems)
			{
				GetGame().ObjectDelete(obj);
			}
		}

		GetGame().ObjectDelete(Chest);
		GetGame().ObjectDelete(Stash);

		return true;
	}

	override bool OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif

		if (!super.OnCancel())
			return false;

		foreach (Object obj: LootItems)
		{
			GetGame().ObjectDelete(obj);
		}

		GetGame().ObjectDelete(Chest);
		GetGame().ObjectDelete(Stash);

		return true;
	}

	override bool OnEventStart(bool continues = false)
	{
		ExpansionQuestObjectiveTreasureHunt treasureHunt = GetObjectiveConfig().GetTreasureHunt();
		if (!treasureHunt)
			return false;

		return CreateTreasure(treasureHunt);
	}

	protected bool CreateTreasure(ExpansionQuestObjectiveTreasureHunt treasureHunt)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CreateTreasure");
	#endif

		vector pos = treasureHunt.GetPositions().GetRandomElement();
		StashPos = pos;

		//! Check if there is already a stash on this position for this quest ID.
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(pos, 3.0, objects, NULL);
		foreach (Object obj: objects)
		{
			UndergroundStash existingStash;
			if (Class.CastTo(existingStash, obj))
			{
				if (existingStash && existingStash.GetQuestID() == GetQuest().GetQuestConfig().GetID())
					return false;
			}
		}

		//! Create the underground stash and hide it
		if (!Class.CastTo(Stash, GetGame().CreateObjectEx("UndergroundStash", StashPos, ECE_PLACE_ON_SURFACE)))
			return false;

		Stash.SetQuestID(GetQuest().GetQuestConfig().GetID());
		Stash.PlaceOnGround();

		EntityAI stashEntity;
		if (!Class.CastTo(stashEntity, Stash))
			return false;

		//! Spawn the chest in the underground stash
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
		if (!questPlayer)
			return false;

		Object chestObj = Spawn("ExpansionQuestSeaChest", 1, questPlayer, stashEntity, StashPos, Vector(0, 0, 0));
		if (!Class.CastTo(Chest, chestObj))
			return false;

		ExpansionQuestSeaChest chestIB;
		if (!Class.CastTo(chestIB, chestObj))
			return false;

		chestIB.SetQuestID(GetQuest().GetQuestConfig().GetID());

		//! Spawn the loot in the chest
		EntityAI chestEntity;
		if (!Class.CastTo(chestEntity, Chest))
			return false;

		map<string, int> items = treasureHunt.GetItems();
		foreach (string name, int amount: items)
		{
			Object item = Spawn(name, amount, questPlayer, chestEntity, StashPos, Vector(0, 0, 0));
			LootItems.Insert(item);
		}

		return true;
	}

	Object Spawn(string name, int amount, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(name, player, parent, amount);
		return obj;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		
		if (!Chest)
			return;
		
		m_UpdateQueueTimer += timeslice;
		if (m_UpdateQueueTimer >= UPDATE_TICK_TIME)
		{						
			vector position = StashPos;
			float maxDistance = 5.0;
			float currentDistance;
			array<vector> groupMemberPos = new array<vector>;
			bool isChestStashed = true;

			if (!GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetPlayer())
			{
				vector playerPos = GetQuest().GetPlayer().GetPosition();
				currentDistance = vector.Distance(playerPos, position);
			}
		#ifdef EXPANSIONMODGROUPS
			else if (GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetGroup())
			{
				//! Set the position of the group member that has the shortest distance to the target location
				//! as our current position if the quest is a group quest.
				ExpansionPartyData group = GetQuest().GetGroup();
				if (!group)
					return;
	
				array<ref ExpansionPartyPlayerData> groupPlayers = group.GetPlayers();
				foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
				{
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
					float dist = vector.Distance(groupMemberPos[p], position);
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
				groupMemberPos.Clear();
			}
		#endif
	
			position[1] = GetGame().SurfaceY(position[0], position[2]);
	
			bool conditionsCheck = currentDistance <= maxDistance;
			if (conditionsCheck && !IsCompleted())
			{
				SetCompleted(true);
				OnComplete();
			}
			else if (!conditionsCheck && IsCompleted())
			{
				SetCompleted(false);
				OnIncomplete();
			}
			
			m_UpdateQueueTimer = 0.0;
		}
	}

	vector GetPosition()
	{
		return StashPos;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TREASUREHUNT;
	}
	
	override bool HasDynamicState()
	{
		return true;
	}
};