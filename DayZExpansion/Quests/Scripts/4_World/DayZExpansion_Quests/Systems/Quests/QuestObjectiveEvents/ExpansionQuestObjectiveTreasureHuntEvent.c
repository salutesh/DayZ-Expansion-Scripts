/**
 * ExpansionQuestObjectiveTreasureHuntEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTreasureHuntEvent: ExpansionQuestObjectiveEventBase
{
	protected ref map<string, int> m_LootItemsMap = new map<string, int>;
	protected ref array<EntityAI> m_LootItems = new array<EntityAI>;
	protected bool m_LootedItemFromChest;
	protected UndergroundStash m_Stash;
	protected ExpansionQuestContainerBase m_Chest;
	protected vector m_StashPos;
	protected bool m_DestinationReached;
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected ref ExpansionQuestObjectiveTreasureHuntConfig m_TreasureHuntConfig;
	protected int m_LootItemsCount;

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
			return false;

		vector pos = m_TreasureHuntConfig.GetPositions().GetRandomElement();
		m_StashPos = pos;

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_StashPos);

	#ifdef EXPANSIONMODNAVIGATION
		if (m_TreasureHuntConfig.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif

		return true;
	}

	//! Event called when the player continues the quest.
	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;
		
		if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
			return false;

		if (IsCompleted())
		{
			m_LootedItemFromChest = true;
			m_DestinationReached = true;
			return true;
		}
		else
		{
			if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
				return false;

			if (m_StashPos == vector.Zero)
				return false;

			CheckQuestPlayersForLootItems();

			if (!m_LootedItemFromChest)
			{
				if (!m_ObjectiveTrigger)
					CreateTrigger(m_StashPos);

			#ifdef EXPANSIONMODNAVIGATION
				if (m_TreasureHuntConfig.GetMarkerName() != string.Empty)
					CreateMarkers();
			#endif
			}
		}
		
		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	//! @note if quest was canceled, OnCancel takes care of deleting loot items, so we do not need to do that here
	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCleanup())
			return false;

		//! Only cleanup the chest if objective is not completed yet, because it may yet contain items if quest is autocomplete
		//! (autocomplete will complete the quest if the 1st item is taken out of the chest)
		if (m_Chest && !IsCompleted())
			GetGame().ObjectDelete(m_Chest);

		if (m_Stash)
			GetGame().ObjectDelete(m_Stash);

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint("End and return TRUE.");

		return true;
	}

	override bool OnCancel()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCancel())
			return false;

		if (m_LootItems.Count() < m_TreasureHuntConfig.GetLootItemsAmount())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::OnCancel - loot items: " + m_LootItems.Count());
			CheckQuestPlayersForLootItems();
		}

		EXTrace.Print(EXTrace.QUESTS, this, "::OnCancel - loot items: " + m_LootItems.Count());
		foreach (EntityAI obj: m_LootItems)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::OnCancel - loot item: " + obj);
			GetGame().ObjectDelete(obj);
		}

		if (m_Chest)
			GetGame().ObjectDelete(m_Chest);

		if (m_Stash)
			GetGame().ObjectDelete(m_Stash);

		if (m_ObjectiveTrigger)
			GetGame().ObjectDelete(m_ObjectiveTrigger);

		ObjectivePrint("End and return TRUE.");

		return true;
	}
	
	override bool OnTurnIn(string playerUID, int selectedObjItemIndex = -1)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnTurnIn(playerUID, selectedObjItemIndex))
			return false;

		if (m_LootItems.Count() < m_TreasureHuntConfig.GetLootItemsAmount())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "::OnTurnIn - loot items: " + m_LootItems.Count());
			CheckQuestPlayersForLootItems();
		}

		ObjectivePrint("All stash loot items found! Clear loot flags..");
		ClearLootFlags();

		return true;
	}
	
	protected bool CheckQuestPlayersForLootItems()
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	//#endif

		m_LootItems.Clear();
		m_LootItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "" + m_Quest.GetPlayer());
			if (!CheckEntityForLootItems(m_Quest.GetPlayer()))
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			set<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				if (!CheckPlayerForLootItems(playerUID))
					return false;

				if (m_LootItemsCount >= m_TreasureHuntConfig.GetLootItemsAmount())
					break;
			}
		}
	#endif

		EXTrace.Add(trace, true);

		return true;
	}

	protected bool CheckPlayerForLootItems(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		return CheckEntityForLootItems(player);
	}

	protected bool CheckEntityForLootItems(EntityAI entity)
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "" + entity);
	//#endif
		
		if (!entity)
			return false;

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(entity.GetInventory().CountInventory());

		entity.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			ItemBase entityItem;
			if (!Class.CastTo(entityItem, item))
				continue;
			
			EXTrace.Print(EXTrace.QUESTS, this, "Checking entity item. [Type: " + entityItem.GetType() + " | Quest ID: " + entityItem.Expansion_GetQuestID() + " | Objective Loot: " + entityItem.Expansion_IsObjectiveLoot() + "]");
			
			if (entityItem.Expansion_GetQuestID() == m_Quest.GetQuestConfig().GetID() && entityItem.Expansion_IsObjectiveLoot())
			{
				m_LootItems.Insert(item);
				m_LootItemsCount++;
				m_LootedItemFromChest = true;
				ObjectivePrint("Added entity loot item to array: " + entityItem.GetType() + " | Items count: " + m_LootItemsCount);
			}

			if (m_LootItemsCount >= m_TreasureHuntConfig.GetLootItemsAmount())
				break;
		}

		return true;
	}

	protected void ClearLootFlags()
	{
	//#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	//#endif

		foreach (EntityAI obj: m_LootItems)
		{
			ItemBase lootIB = ItemBase.Cast(obj);
			if (lootIB)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "::ClearLootFlags - loot item: " + lootIB);
				lootIB.Expansion_SetQuestID(-1);
				lootIB.Expansion_SetIsObjectiveLoot(false);
			}
		}
	}

	protected void CreateTreasure()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!m_TreasureHuntConfig)
			return;

		bool useStash = m_TreasureHuntConfig.DigInStash();
		string containerName = m_TreasureHuntConfig.GetContainerName();
		if (!containerName.ToType().IsInherited(ExpansionQuestContainerBase))
			return;

		if (useStash)
		{
			//! Create the underground stash and hide it
			Object stashObj = GetGame().CreateObjectEx("UndergroundStash", m_StashPos, ECE_KEEPHEIGHT);
			if (!Class.CastTo(m_Stash, stashObj))
			{
				GetGame().ObjectDelete(stashObj);
				return;
			}

			m_Stash.SetPosition(m_StashPos);
			vector ori = GetGame().GetSurfaceOrientation(m_StashPos[0], m_StashPos[2]);
			m_Stash.SetOrientation(ori);
			m_Stash.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
		}

		//! Spawn the chest in the underground stash
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(m_Quest.GetPlayerUID());
		if (!questPlayer)
			return;

		Object chestObj;
		if (useStash)
		{
			chestObj = Spawn(containerName, 1, questPlayer, m_Stash, m_StashPos, Vector(0, 0, 0));
		}
		else
		{
			chestObj = GetGame().CreateObjectEx(containerName, m_StashPos, ECE_PLACE_ON_SURFACE);
		}


		if (!Class.CastTo(m_Chest, chestObj))
			return;

		if (!m_Chest)
		{
			GetGame().ObjectDelete(chestObj);
			return;
		}

		m_Chest.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
		set<string> playerUIDs = new set<string>;
		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			playerUIDs.Insert(m_Quest.GetPlayerUID());
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			playerUIDs = m_Quest.GetPlayerUIDs();
		}
	#endif

		m_Chest.ExpansionSetCanReceiveItems(true);
		ExpansionLootSpawner.SpawnLoot(m_Chest, m_TreasureHuntConfig.GetLoot(), m_TreasureHuntConfig.GetLootItemsAmount(), m_LootItems, m_LootItemsMap);
		foreach (EntityAI lootItem: m_LootItems)
		{
			ItemBase lootIB = ItemBase.Cast(lootItem);
			if (!lootIB)
			{
				GetGame().ObjectDelete(lootItem);
				return;
			}
			
			lootIB.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
			lootIB.Expansion_SetIsObjectiveLoot(true);
		}

		m_Chest.ExpansionSetCanReceiveItems(false);
		m_Chest.ExpansionSetContainerOwner(questPlayer);
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
			return;

		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		string markerName = m_TreasureHuntConfig.GetMarkerName();
		CreateObjectiveMarker(m_StashPos, markerName, m_TreasureHuntConfig.GetMarkerVisibility());

	}
#endif

	Object Spawn(string name, int amount, PlayerBase player, EntityAI parent, vector position, vector orientation)
	{
		Object obj = ExpansionItemSpawnHelper.SpawnOnParent(name, player, parent, amount);
		if (!obj)
			Error(ToString() + "::Spawn - couldn't spawn " + name + " on " + parent + " at " + position);
		return obj;
	}

	protected void CreateTrigger(vector pos)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(this);
	}

	vector GetPosition()
	{
		return m_StashPos;
	}

	void SetStashPosition(vector pos)
	{
		m_StashPos = pos;
	}

	//! Used by the trigger
	void SetReachedLocation(bool state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("State: " + state);

		if (state)
		{
			CreateTreasure();
		#ifdef EXPANSIONMODNAVIGATION
			RemoveObjectiveMarkers();
		#endif
		}
		else
		{
			if (m_Chest)
				GetGame().ObjectDelete(m_Chest);

			if (m_Stash)
				GetGame().ObjectDelete(m_Stash);

		#ifdef EXPANSIONMODNAVIGATION
			CreateMarkers();
		#endif
		}

		if (!IsCompleted())
			SetLocationState(state);
	
		m_Quest.QuestCompletionCheck(true);
	}

	void SetLocationState(bool state)
	{
		m_DestinationReached = state;
	}

	void LootedItemFromChest()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		m_LootedItemFromChest = true;
		m_Quest.QuestCompletionCheck(true);

	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "State: " + typename.EnumToString(ExpansionQuestItemState, state));
	#endif
	}

	bool HasLootedItemFromChest()
	{
		return m_LootedItemFromChest;
	}

	override bool CanComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("m_DestinationReached: " + m_DestinationReached);
		ObjectivePrint("m_LootedItemFromChest: " + m_LootedItemFromChest);

		if (!super.CanComplete())
			return false;
		
		if (m_DestinationReached && m_LootedItemFromChest)
		{
			ObjectivePrint("End and return: TRUE");
			return true;
		}

		ObjectivePrint("End and return: FALSE");
		return false;
	}

	UndergroundStash GetStash()
	{
		return m_Stash;
	}

	ExpansionQuestContainerBase GetChest()
	{
		return m_Chest;
	}

	array<EntityAI> GetLootItems()
	{
		return m_LootItems;
	}

	map<string, int> GetLootItemsMap()
	{
		return m_LootItemsMap;
	}
	
#ifdef EXPANSIONMODNAVIGATION
	override bool CanCreateMarkers()
	{
		return !m_DestinationReached;
	}
#endif

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TREASUREHUNT;
	}
};