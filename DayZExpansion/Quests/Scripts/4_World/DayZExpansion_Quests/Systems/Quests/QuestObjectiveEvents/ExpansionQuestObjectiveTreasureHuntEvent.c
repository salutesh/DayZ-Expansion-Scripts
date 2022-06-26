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
	#ifdef EXPANSIONMODQUESTSDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::OnStart - Start");
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

		super.OnStart();

	#ifdef EXPANSIONMODQUESTSDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::OnStart - End");
	#endif
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::OnContinue - Start");
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

		super.OnContinue();
		
	#ifdef EXPANSIONMODQUESTSDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::OnContinue - Start");
	#endif		
	}

	override void OnCleanup()
	{		
		ExpansionQuestPersistentData questPlayerData = GetQuest().GetQuestModule().GetPlayerQuestDataByUID(GetQuest().GetPlayerUID());
		if (!questPlayerData)
			return;
		
		int state = questPlayerData.GetQuestStateByQuestID(GetQuest().GetQuestConfig().GetID());
		//! Only cleanup the treasure if quest is not completed
		if (state == ExpansionQuestState.STARTED)
		{
			foreach (Object obj: LootItems)
			{
				GetGame().ObjectDelete(obj);
			}
	
			GetGame().ObjectDelete(Chest);
			GetGame().ObjectDelete(Stash);
		}
		
		super.OnCleanup();
	}

	override void OnCancel()
	{
		foreach (Object obj: LootItems)
		{
			GetGame().ObjectDelete(obj);
		}

		GetGame().ObjectDelete(Chest);
		GetGame().ObjectDelete(Stash);

		super.OnCancel();
	}

	void CreateTreasure(ExpansionQuestObjectiveTreasureHunt treasureHunt)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::CreateTreasure - Start");
	#endif

		vector pos = treasureHunt.GetPositions().GetRandomElement();
		StashPos = pos;

		//! Create the underground stash and hide it
		if (!Class.CastTo(Stash, GetGame().CreateObjectEx("UndergroundStash", StashPos, ECE_PLACE_ON_SURFACE)))
			return;

		if (Stash)
		{
			Stash.PlaceOnGround();
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::CreateTreasure - Spawned stash on pos: " + StashPos);
	#endif

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

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::CreateTreasure - Spawned chest in stash!");
	#endif

		Chest.ExpansionSetContainerOwner(questPlayer);

		//! Spawn the loot in the chest
		EntityAI chestEntity;
		if (!Class.CastTo(chestEntity, Chest))
			return;

		for (int i = 0; i < treasureHunt.GetItems().Count(); i++)
		{
			string name = treasureHunt.GetItems().GetKey(i);
			int amount = treasureHunt.GetItems().GetElement(i);

		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("ExpansionQuestObjectiveTreasureHuntEvent::CreateTreasure - Add item to chest: " + name + " x" + amount);
		#endif
			Object item = Spawn(name, amount, questPlayer, chestEntity, StashPos, Vector(0, 0, 0));

			LootItems.Insert(item);
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("ExpansionQuestObjectiveTreasureHuntEvent::CreateTreasure - End");
	#endif
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

		//! Set the position of the group member that has the shortest distance to the target location
		//! as our current position if the quest is a group quest.
		array<vector> groupMemberPos = new array<vector>;
		if (GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetGroup())
		{
			//Print(ToString() + "::OnUpdate - GROUP QUEST");
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
				
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::OnUpdate - Target position: " + position);
				Print(ToString() + "::OnUpdate - Index: " + p);
				Print(ToString() + "::OnUpdate - Position: " + groupMemberPos[p]);
				Print(ToString() + "::OnUpdate - Distance: " + dist);
				Print(ToString() + "::OnUpdate - Smallest distance: " + smallestDistance);
			#endif
			}
			
			currentDistance = vector.Distance(groupMemberPos[posIndex], position);
			groupMemberPos.Clear();
		#endif
		}
		else if (!GetQuest().IsGroupQuest() && GetQuest() && GetQuest().GetPlayer())
		{
			vector playerPos = GetQuest().GetPlayer().GetPosition();
			currentDistance = vector.Distance(playerPos, position);
		}

		position[1] = GetGame().SurfaceY(position[0], position[2]);
	
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnUpdate - Current distance: " + currentDistance);
	#endif

		if (currentDistance <= maxDistance && !IsCompleted())
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::OnUpdate - Complete!");
		#endif
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