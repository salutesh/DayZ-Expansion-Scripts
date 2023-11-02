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
	protected ref ExpansionQuestObjectiveTreasureHuntConfig m_Config;
	protected int m_LootItemsCount;

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnEventStart())
			return false;

		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		vector pos = m_Config.GetPositions().GetRandomElement();
		m_StashPos = pos;

		if (!m_ObjectiveTrigger)
			CreateTrigger(m_StashPos);

	#ifdef EXPANSIONMODNAVIGATION
		if (m_Config.GetMarkerName() != string.Empty)
			CreateMarkers();
	#endif

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnContinue())
			return false;
		
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return false;

		if (IsCompleted())
		{
			if (!CheckQuestPlayersForLootItems())
			{
				m_Quest.CancelQuest();
				return false;
			}
			else
			{
				if (m_LootItemsCount >= m_Config.GetLootItemsAmount())
				{
					m_LootedItemFromChest = true;
					m_DestinationReached = true;
					return true;
				}
				else
				{
					m_Quest.CancelQuest();
					return false;
				}
			}
		}
		else
		{
			if (!Class.CastTo(m_Config, m_ObjectiveConfig))
				return false;

			if (m_StashPos == vector.Zero)
				return false;

			if (!m_ObjectiveTrigger)
				CreateTrigger(m_StashPos);

		#ifdef EXPANSIONMODNAVIGATION
			if (m_Config.GetMarkerName() != string.Empty)
				CreateMarkers();
		#endif
		}
		
		m_Quest.QuestCompletionCheck(true);

		return true;
	}

	override bool OnCleanup()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		if (!super.OnCleanup())
			return false;

		//! Only cleanup the loot and chest if objective is not completed yet
		if (!IsCompleted())
		{
			foreach (EntityAI obj: m_LootItems)
			{
				GetGame().ObjectDelete(obj);
			}
			
			if (m_Chest)
				GetGame().ObjectDelete(m_Chest);
		}

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

		foreach (EntityAI obj: m_LootItems)
		{
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

		if (m_LootItems.Count() < m_Config.GetLootItemsAmount())
		{
			if (!CheckQuestPlayersForLootItems())
			{
				ObjectivePrint("Stash loot items check failed! Cancel quest..");
				m_Quest.CancelQuest();
				return false;
			}
			else
			{
				if (m_LootItemsCount >= m_Config.GetLootItemsAmount())
				{
					ObjectivePrint("All stash loot items found! Clear loot flags..");
					ClearLootFlags();
				}
				else
				{
					ObjectivePrint("Not all loot items found! Cancel quest..");
					foreach (EntityAI obj: m_LootItems)
					{
						GetGame().ObjectDelete(obj);
					}

					m_Quest.CancelQuest();
					return false;
				}
			}
		}
		else
		{
			ObjectivePrint("All stash loot items found! Clear loot flags..");
			ClearLootFlags();
		}

		return true;
	}
	
	protected bool CheckQuestPlayersForLootItems()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		m_LootItems.Clear();
		m_LootItemsCount = 0;

		if (!m_Quest.GetQuestConfig().IsGroupQuest())
		{
			if (!CheckPlayerForLootItems(m_Quest.GetPlayer()))
				return false;
		}
	#ifdef EXPANSIONMODGROUPS
		else
		{
			set<string> playerUIDs = m_Quest.GetPlayerUIDs();
			foreach (string playerUID: playerUIDs)
			{
				if (m_LootItemsCount >= m_Config.GetLootItemsAmount())
					break;

				if (!CheckPlayerForLootItems(playerUID))
					return false;
			}
		}
	#endif

		ObjectivePrint("End and return TRUE");

		return true;
	}

	protected bool CheckPlayerForLootItems(string playerUID)
	{
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		return CheckPlayerForLootItems(player);
	}

	protected bool CheckPlayerForLootItems(PlayerBase player)
	{
		if (!player)
			return false;

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, player.GetIdentity().GetId());
	#endif
		
		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(player.GetInventory().CountInventory());

		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count(); i++)
		{
			ItemBase playerItem;
			if (!Class.CastTo(playerItem, items[i]))
				continue;
			
			ObjectivePrint("Checking player item. [Type: " + playerItem.GetType() + " | Quest ID: " + playerItem.Expansion_GetQuestID() + " | Objective Loot: " + playerItem.Expansion_IsObjectiveLoot() + "]");
			
			if (playerItem.Expansion_GetQuestID() == m_Quest.GetQuestConfig().GetID() && playerItem.Expansion_IsObjectiveLoot())
			{
				m_LootItems.Insert(items[i]);
				m_LootItemsCount++;
				ObjectivePrint("Added stash loot item to array: " + playerItem.GetType() + " | Items count: " + m_LootItemsCount);
			}

			if (m_LootItemsCount >= m_Config.GetLootItemsAmount())
				break;
		}

		return true;
	}
	
	protected void ClearLootFlags()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif

		foreach (EntityAI obj: m_LootItems)
		{
			ItemBase lootIB = ItemBase.Cast(obj);
			if (lootIB)
			{
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

		if (!m_Config)
			return;

		bool useStash = m_Config.DigInStash();
		string containerName = m_Config.GetContainerName();
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
		ExpansionLootSpawner.SpawnLoot(m_Chest, m_Config.GetLoot(), m_Config.GetLootItemsAmount(), m_LootItems, m_LootItemsMap);
		foreach (EntityAI lootItem: m_LootItems)
		{
			ItemBase lootIB = ItemBase.Cast(lootItem);
			if (!lootIB)
			{
				GetGame().ObjectDelete(lootItem);
				return;
			}
			
			lootIB.Expansion_SetQuestID(m_Config.GetID());
			lootIB.Expansion_SetIsObjectiveLoot(true);
		}

		m_Chest.ExpansionSetCanReceiveItems(false);
		m_Chest.ExpansionSetContainerOwner(questPlayer);
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return;

		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		string markerName = m_Config.GetMarkerName();
		CreateObjectiveMarker(m_StashPos, markerName, m_Config.GetMarkerVisibility());

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
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		ObjectivePrint("State: " + typename.EnumToString(ExpansionQuestItemState, state));

		if (m_LootItems.Find(item) == -1)
			return;

		int amount;
		if (!m_LootItemsMap.Find(item.GetType(), amount))
			return;

		int itemAmount = item.Expansion_GetStackAmount();
		if (itemAmount != amount)
		{
			m_LootedItemFromChest = true;
			m_Quest.QuestCompletionCheck(true);
		}
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