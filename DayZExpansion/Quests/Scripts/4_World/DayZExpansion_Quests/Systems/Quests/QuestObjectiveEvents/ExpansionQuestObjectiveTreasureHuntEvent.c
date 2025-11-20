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

		if (!ExpansionQuestModule.GetModuleInstance().QuestTriggerExists(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_ObjectiveConfig.GetID()))
			CreateTrigger(m_StashPos);
	
		if (m_TreasureHuntConfig.GetMarkerName() != string.Empty)
			CreateMarkers();

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
				if (!ExpansionQuestModule.GetModuleInstance().QuestTriggerExists(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_ObjectiveConfig.GetID()))
					CreateTrigger(m_StashPos);

				if (m_TreasureHuntConfig.GetMarkerName() != string.Empty)
					CreateMarkers();
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

		if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
			return false;

		//! Only cleanup the chest if objective is not completed yet, because it may yet contain items if quest is autocomplete
		//! (autocomplete will complete the quest if the 1st item is taken out of the chest)
		if (m_Chest && !IsCompleted())
			g_Game.ObjectDelete(m_Chest);

		if (m_Stash)
			g_Game.ObjectDelete(m_Stash);

		int questID = m_Quest.GetQuestConfig().GetID();
		int objectiveType = GetObjectiveType();
		int objectiveID = m_TreasureHuntConfig.GetID();
		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(questID))
			ExpansionQuestModule.GetModuleInstance().RemoveObjectiveTrigger(questID, ExpansionObjectiveTriggerType.TRAVEL, objectiveType, objectiveID);

		if (!super.OnCleanup())
			return false;

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
			g_Game.ObjectDelete(obj);
		}

		if (m_Chest)
			g_Game.ObjectDelete(m_Chest);

		if (m_Stash)
			g_Game.ObjectDelete(m_Stash);

		ObjectivePrint("End and return TRUE.");

		return true;
	}

	override bool OnComplete()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		int questID = m_Quest.GetQuestConfig().GetID();
		int objectiveType = GetObjectiveType();
		int objectiveID = m_TreasureHuntConfig.GetID();
		if (!ExpansionQuestModule.GetModuleInstance().IsOtherQuestInstanceActive(questID))
			ExpansionQuestModule.GetModuleInstance().RemoveObjectiveTrigger(questID, ExpansionObjectiveTriggerType.TRAVEL, objectiveType, objectiveID);

		super.OnComplete();

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
	//#endif

		m_LootItems.Clear();
		m_LootItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "" + m_Quest.GetPlayer());
			if (!CheckEntityForLootItems(m_Quest.GetPlayer()))
				return false;
		}
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

#ifdef EXTRACE
		EXTrace.Add(trace, true);
#endif
		
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "" + entity);
#endif
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
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
			Object stashObj = g_Game.CreateObjectEx("UndergroundStash", m_StashPos, ECE_KEEPHEIGHT);
			if (!Class.CastTo(m_Stash, stashObj))
			{
				g_Game.ObjectDelete(stashObj);
				return;
			}

			m_Stash.SetPosition(m_StashPos);
			vector ori = g_Game.GetSurfaceOrientation(m_StashPos[0], m_StashPos[2]);
			m_Stash.SetOrientation(ori);
			m_Stash.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
		}

		//! Spawn the chest in the underground stash
		string playerUID = m_Quest.GetPlayerUID();
		PlayerBase questPlayer = PlayerBase.GetPlayerByUID(playerUID);
		if (!questPlayer)
			return;

		Object chestObj;
		if (useStash)
		{
			chestObj = m_Stash.GetInventory().CreateInInventory(containerName);
		}
		else
		{
			chestObj = g_Game.CreateObjectEx(containerName, m_StashPos, ECE_PLACE_ON_SURFACE);
		}

		if (!Class.CastTo(m_Chest, chestObj))
		{
			g_Game.ObjectDelete(chestObj);
			return;
		}

		m_Chest.ExpansionSetContainerOwner(questPlayer.GetIdentity());
		m_Chest.Expansion_SetQuestID(m_Quest.GetQuestConfig().GetID());
		m_Chest.ExpansionSetCanReceiveItems(true);
		ExpansionLootSpawner.SpawnLoot(m_Chest, m_TreasureHuntConfig.GetLoot(), m_TreasureHuntConfig.GetLootItemsAmount(), m_LootItems, m_LootItemsMap);
		m_Chest.ExpansionSetCanReceiveItems(false);
	}

	override void CreateMarkers()
	{
		if (!Class.CastTo(m_TreasureHuntConfig, m_ObjectiveConfig))
			return;

#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		string markerName = m_TreasureHuntConfig.GetMarkerName();
		if (markerName != string.Empty)
			CreateObjectiveMarker(m_StashPos, markerName, m_TreasureHuntConfig.GetMarkerVisibility());
	}

	protected void CreateTrigger(vector pos)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
	#endif

		array<ExpansionObjectiveTriggerBase> triggers = {};
		ExpansionTravelObjectiveSphereTrigger trigger = ExpansionTravelObjectiveSphereTrigger.Cast(g_Game.CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_LOCAL));
		if (!trigger)
			return;

		trigger.SetPosition(pos);
		trigger.SetObjectiveData(m_Quest.GetQuestConfig().GetID(), GetObjectiveType(), m_ObjectiveConfig.GetID());
		triggers.Insert(trigger);

		ExpansionQuestModule.GetModuleInstance().SetQuestTriggers(m_Quest.GetQuestConfig().GetID(), triggers);
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
	#endif
		ObjectivePrint("State: " + state);

		if (state)
		{
			if (!m_Chest)
				CreateTreasure();

			RemoveObjectiveMarkers();
		}
		else
		{
			if (m_Chest)
			{
				g_Game.ObjectDelete(m_Chest);
				m_Chest = null;
			}

			if (m_Stash)
			{
				g_Game.ObjectDelete(m_Stash);
				m_Stash = null;
			}

			CreateMarkers();
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		m_LootedItemFromChest = true;
		m_Quest.QuestCompletionCheck(true);
	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, "State: " + typename.EnumToString(ExpansionQuestItemState, state));
#endif
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

	override bool CanCreateMarkers()
	{
		return !m_DestinationReached;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TREASUREHUNT;
	}
};