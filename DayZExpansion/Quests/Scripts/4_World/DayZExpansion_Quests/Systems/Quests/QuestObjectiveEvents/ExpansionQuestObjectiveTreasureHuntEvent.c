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
	private ref array<Object> LootItems = new array<Object>;
	private UndergroundStash Stash;
	private ExpansionQuestSeaChest Chest;
	private vector StashPos;
	
	override void OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		TreasureHuntEventStart();

		super.OnStart();
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif
	
		TreasureHuntEventStart();

		super.OnContinue();
	}

	override void OnCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCleanup");
	#endif
		
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
		
		super.OnCleanup();
	}

	override void OnCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnCancel");
	#endif

		foreach (Object obj: LootItems)
		{
			GetGame().ObjectDelete(obj);
		}

		GetGame().ObjectDelete(Chest);
		GetGame().ObjectDelete(Stash);

		super.OnCancel();
	}
	
	private void TreasureHuntEventStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "TreasureHuntEventStart");
	#endif

		if (!GetQuest().GetPlayer())
		{
			GetQuest().SetPlayer();
			if (!GetQuest().GetPlayer())
				return;
		}
		
		ExpansionQuestObjectiveTreasureHunt treasureHunt = GetObjectiveConfig().GetTreasureHunt();
		if (!treasureHunt)
			return;

		CreateTreasure(treasureHunt);
	}

	void CreateTreasure(ExpansionQuestObjectiveTreasureHunt treasureHunt)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CreateTreasure");
	#endif

		vector pos = treasureHunt.GetPositions().GetRandomElement();
		StashPos = pos;

		//! Create the underground stash and hide it
		if (!Class.CastTo(Stash, GetGame().CreateObjectEx("UndergroundStash", StashPos, ECE_PLACE_ON_SURFACE)))
			return;

		if (Stash)
		{
			Stash.SetIsQuestItem(true);
			Stash.PlaceOnGround();
		}

		EntityAI stashEntity;
		if (!Class.CastTo(stashEntity, Stash))
			return;

		//! Spawn the chest in the underground stash
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(GetQuest().GetPlayerUID());
		if (!questPlayer)
			return;

		Object chestObj = Spawn("ExpansionQuestSeaChest", 1, questPlayer, stashEntity, StashPos, Vector(0, 0, 0));
		if (!Class.CastTo(Chest, chestObj))
			return;

		Chest.ExpansionSetContainerOwner(questPlayer);

		//! Spawn the loot in the chest
		EntityAI chestEntity;
		if (!Class.CastTo(chestEntity, Chest))
			return;

		map<string, int> items = treasureHunt.GetItems();
		foreach (string name, int amount: items)
		{
			Object item = Spawn(name, amount, questPlayer, chestEntity, StashPos, Vector(0, 0, 0));
			LootItems.Insert(item);
		}
	}

	Object Spawn(string name, int amount, PlayerBase player, inout EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(name, player, parent, amount);
		return obj;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		vector position = StashPos;
		float maxDistance = 5.0;
		float currentDistance;
		array<vector> groupMemberPos = new array<vector>;
		
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

		if (currentDistance <= maxDistance && !IsCompleted())
		{
			SetCompleted(true);
			OnComplete();
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
};