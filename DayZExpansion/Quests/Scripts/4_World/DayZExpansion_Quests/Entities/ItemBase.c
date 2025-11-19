/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionQuestItemState
{
	INV_ENTER,
	INV_EXIT,
	INV_DELETE,
	STACKSIZE_CHANGED,
	CONTENTQUANTITY_CHANGED
}

modded class ItemBase
{
	protected static ref array<ref ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ref ExpansionQuestObjectiveEventBase>;

	protected int m_Expansion_QuestID = -1;
	protected bool m_Expansion_IsQuestGiver = false;
	protected bool m_Expansion_DeletedByQuest = false;
	protected bool m_Expansion_IsDeliveryItem = false;
	protected bool m_Expansion_Quests_InventoryEnter;
	protected bool m_Expansion_IsSetForDeletion = false;
	protected bool m_Expansion_IsObjectiveLootItem = false;
	protected bool m_Expansion_SkipQuestItemCheck;

	protected const ref array<string> m_Expansion_ExcludedFromCombine = {"Ammunition_Base", "Edible_Base"};

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_Expansion_QuestID");
		RegisterNetSyncVariableBool("m_Expansion_IsQuestGiver");
		RegisterNetSyncVariableBool("m_Expansion_IsObjectiveLootItem");
	}

	bool Expansion_IsQuestItem()
	{
		return (m_Expansion_QuestID > -1);
	}

	int Expansion_GetQuestID()
	{
		return m_Expansion_QuestID;
	}

	void Expansion_SetQuestID(int id)
	{
		m_Expansion_QuestID = id;
		SetSynchDirty();
	}

	bool Expansion_IsQuestGiver()
	{
		return m_Expansion_IsQuestGiver;
	}

	void Expansion_SetIsQuestGiver(bool state)
	{
		m_Expansion_IsQuestGiver = state;
		SetSynchDirty();
	}

	void Expansion_SetDeletedByQuest(bool state)
	{
		m_Expansion_DeletedByQuest = state;
	}

	void Expansion_SetIsDeliveryItem(bool state)
	{
		m_Expansion_IsDeliveryItem = state;
	}

	bool Expansion_IsDeliveryItem()
	{
		return m_Expansion_IsDeliveryItem;
	}
	
	void Expansion_SetIsObjectiveLoot(bool state)
	{
		m_Expansion_IsObjectiveLootItem = state;
		SetSynchDirty();
	}

	bool Expansion_IsObjectiveLoot()
	{
		return m_Expansion_IsObjectiveLootItem;
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (m_Expansion_IsStoreLoaded && m_Expansion_QuestID > -1 && !Expansion_IsQuestGiver())
		{
			if (!GetHierarchyRootPlayer())
				g_Game.ObjectDelete(this);
		}
	}

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		if (g_Game.IsServer() && g_Game.IsMultiplayer())
		{
			PlayerBase playerPB = PlayerBase.Cast(player);
			if (playerPB && playerPB.GetIdentity())
			{
				if (m_Expansion_QuestID > -1 && !Expansion_IsQuestGiver())
				{
					//! Delete this item when it has a quest ID assigned but the player who picks it up has not a active quest instance with that ID.
					if (!ExpansionQuestModule.GetModuleInstance().GetActiveQuestWithQuestID(playerPB, m_Expansion_QuestID))
					{
						g_Game.ObjectDelete(this);
						return;
					}
				}

				//! @note use Call() so execution happens in next frame, after all operations in the current frame have executed
				//! (e.g. splitting/combining items etc)
				m_Expansion_Quests_InventoryEnter = true;
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(CheckAssignedObjectivesForEntity, ExpansionQuestItemState.INV_ENTER, player, 0.0);
			}
		}
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		if (g_Game.IsServer() && g_Game.IsMultiplayer())
		{
			PlayerBase playerPB = PlayerBase.Cast(player);
			if (playerPB && playerPB.GetIdentity())
			{
				if (m_Expansion_QuestID > -1)
				{
					ExpansionQuest questInstance = ExpansionQuestModule.GetModuleInstance().GetActiveQuestWithQuestID(playerPB, m_Expansion_QuestID);
					if (!Expansion_IsQuestGiver())
					{
						//! Delete this item when it has a quest ID assigned but the player dropping it has not a active quest instance with that ID.
						if (!questInstance)
							g_Game.ObjectDelete(this);
					}
					else if (questInstance && questInstance.GetQuestGiverItem())
					{
						//! Cancel quest that is active for the player that drops the item when item is a quest giver-item of a active quest.
						if (questInstance.GetQuestGiverItem() == this && !m_Expansion_DeletedByQuest)
							questInstance.CancelQuest();
					}
				}

				CheckAssignedObjectivesForEntity(ExpansionQuestItemState.INV_EXIT);
			}
		}
	}

	override void OnQuantityChanged(float delta)
	{
		super.OnQuantityChanged(delta);

		if (g_Game.IsServer() && g_Game.IsMultiplayer() && !m_Expansion_Quests_InventoryEnter)
		{
			//! @note use Call() so execution happens in next frame, after all operations in the current frame have executed
			//! (e.g. splitting/combining items etc)
			if (Expansion_IsStackable())
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_OnStackSizeChanged, delta);
			}
			else
			{
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_OnContentQuantityChanged, delta);
			}
		}
	}

	void Expansion_OnStackSizeChanged(float delta)
	{
		//! @note If item is set for deletion, INV_EXIT will deal with the necessary objective-related checks
		if (IsSetForDeletion())
			return;
		
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (player && player.GetIdentity())
			CheckAssignedObjectivesForEntity(ExpansionQuestItemState.STACKSIZE_CHANGED, player, delta);
	}
	
	void Expansion_OnContentQuantityChanged(float delta)
	{
		//! @note If item is set for deletion, INV_EXIT will deal with the necessary objective-related checks
		if (IsSetForDeletion())
			return;

		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if (player && player.GetIdentity())
		{
			ExpansionItemQuantityType quantityType;
			float quantity = Expansion_GetQuantity(quantityType);		
			if (quantityType >= ExpansionItemQuantityType.PERCENTAGE && quantityType <= ExpansionItemQuantityType.POWER)
			{
				CheckAssignedObjectivesForEntity(ExpansionQuestItemState.CONTENTQUANTITY_CHANGED, player, delta);
			}
		}
	}
	
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel,newLevel,zone);
		
		if (g_Game.IsServer())
		{
			//! If item state is ruined then check if it is a quest objective item and remove it from the certain objective.
			if (newLevel == GameConstants.STATE_RUINED)
			{
				PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
				if (player && player.GetIdentity())
					CheckAssignedObjectivesForEntity(ExpansionQuestItemState.INV_EXIT, player);
			}
		}
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUseQuestItem);
	}

	override bool CanBeSplit()
	{
		if (!super.CanBeSplit())
			return false;

		if (!m_Expansion_SkipQuestItemCheck && Expansion_IsQuestItem())
			return false;

		return true;
	}

	bool Expansion_CanBeSplit(bool skipQuestItemCheck = false)
	{
		m_Expansion_SkipQuestItemCheck = skipQuestItemCheck;

		bool canBeSplit = CanBeSplit();

		m_Expansion_SkipQuestItemCheck = false;

		return canBeSplit;
	}

	override void OnRightClick()
	{
		super.OnRightClick();

		if (!g_Game.IsDedicatedServer() && Expansion_IsQuestItem() && Expansion_CanBeSplit(true) && !GetDayZGame().IsLeftCtrlDown() && !g_Game.GetPlayer().GetInventory().HasInventoryReservation(this, null))
		{
			//! TODO: localization
			CF_Localiser text = new CF_Localiser("The item %1 is a quest item and can't be split!", GetDisplayName());
			ExpansionNotification("Can't split item", text, "Error", COLOR_EXPANSION_NOTIFICATION_ORANGE, 7).Create();
		}
	}

	override bool CanBeCombined(EntityAI other_item, bool reservation_check = true, bool stack_max_limit = false)
	{
		if (!super.CanBeCombined(other_item, reservation_check, stack_max_limit))
			return false;

		ItemBase otherIB = ItemBase.Cast(other_item);
		if (Expansion_IsQuestItem() || otherIB.Expansion_IsQuestItem())
		{
			//! Never allow quest delivery items to be combined with anything
			if (Expansion_IsDeliveryItem() || otherIB.Expansion_IsDeliveryItem())
				return false;

			//! If both items are quest items only allow combining them when they share the same quest ID.
			if (Expansion_IsQuestItem() && otherIB.Expansion_IsQuestItem() && Expansion_GetQuestID() != otherIB.Expansion_GetQuestID())
				return false;

			//! Only allow combining of quest items that are not excluded in general
			if (Expansion_IsQuestItem() && ExpansionStatic.IsAnyOf(this, m_Expansion_ExcludedFromCombine))
				return false;

			if (otherIB.Expansion_IsQuestItem() && ExpansionStatic.IsAnyOf(otherIB, m_Expansion_ExcludedFromCombine))
				return false;
		}

		return true;
	}

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, ItemBase);
#endif

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, ItemBase);
#endif

		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.RemoveOrdered(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, null, "Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	void CheckAssignedObjectivesForEntity(ExpansionQuestItemState state, Man player = null, float delta = 0.0)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, GetType());
	#endif

		if (m_Expansion_Quests_InventoryEnter)
			m_Expansion_Quests_InventoryEnter = false;

		if (s_Expansion_AssignedQuestObjectives.Count() == 0)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			EXTrace.Print(EXTrace.QUESTS, this, "No quest objectives assigned!");
		#endif
			return;
		}

		string playerUID;
		if (player && player.GetIdentity())
			playerUID = player.GetIdentity().GetId();

		ExpansionQuest quest;
		int failSafe = s_Expansion_AssignedQuestObjectives.Count() + 1;
		for (int i = 0, j = 0; i < s_Expansion_AssignedQuestObjectives.Count() && j < failSafe; j++)
		{
			ExpansionQuestObjectiveEventBase objective = s_Expansion_AssignedQuestObjectives[i];
			if (!objective)
			{
				EXTrace.Print(EXTrace.QUESTS, this, "WARNING: Objective is NULL!");
				s_Expansion_AssignedQuestObjectives.RemoveOrdered(i);
				continue;
			}

			//! Check if the current objective is active
			if (!objective.IsActive())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Objective is not active. Skip...");
			#endif
				i++;
				continue;
			}

			quest = objective.GetQuest();
			if (!quest)
			{
				i++;
				continue;
			}

			//! Check if the current objective belongs to the item owner if we got the UID.
			if (playerUID != string.Empty && !quest.IsQuestPlayer(playerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				EXTrace.Print(EXTrace.QUESTS, this, "Player with UID [" + playerUID + "] is not a quest player of this quest objective. Skip...");
			#endif
				i++;
				continue;
			}

			OnObjectiveItemInventoryChange(objective, state, delta);

			if (objective.IsAssigned())
				i++;
		}

		//! @note should not happen, this could mean an objective was no longer marked assigned but still in the list
		if (j == failSafe)
			EXPrint(ToString() + " ::CheckAssignedObjectivesForEntity - WARNING: Reached end of loop unexpectedly!");
	}

	protected void OnObjectiveItemInventoryChange(ExpansionQuestObjectiveEventBase objective, ExpansionQuestItemState state, float delta)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
	#endif
		//! Run thrue all possible objective types
		switch (objective.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryEvent deliveryObjective;
				if (Class.CastTo(deliveryObjective, objective))
				{
					//! For delivery objectives we only accept the items that have the respective quest ID of the quest that belongs to the objective.
					//! So we compare the quest ID that has been applied to the item if so.
					int questID = deliveryObjective.GetQuest().GetQuestConfig().GetID();
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					EXTrace.Print(EXTrace.QUESTS, this, objective.ToString() + " is a Delivery objective. Objective quest ID: " + questID + " | Item quest ID: " + m_Expansion_QuestID);
				#endif
					if (m_Expansion_QuestID != questID)
						return;

					deliveryObjective.OnInventoryItemLocationChange(this, state, delta);
				}
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionEvent collectionObjective;
				if (Class.CastTo(collectionObjective, objective))
				{
					collectionObjective.OnInventoryItemLocationChange(this, state, delta);
				}
			}
			break;
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingEvent craftingObjective;
				if (Class.CastTo(craftingObjective, objective))
				{
					craftingObjective.OnInventoryItemLocationChange(this, state);
				}
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntEvent treasureHuntObjective;
				if (Class.CastTo(treasureHuntObjective, objective))
				{
					treasureHuntObjective.OnInventoryItemLocationChange(this, state);
				}
			}
			break;
		}
	}
	
	void Expansion_SetForDeletion(bool state)
	{
		m_Expansion_IsSetForDeletion = state;
	}
	
	bool Expansion_IsSetForDeletion()
	{
		return m_Expansion_IsSetForDeletion;
	}

#ifdef EXPANSIONMODHARDLINE
	override ExpansionHardlineItemRarity Expansion_GetRarity()
	{
		if (Expansion_IsQuestItem())
			return ExpansionHardlineItemRarity.Quest;

		return m_Expansion_Rarity;
	}
#endif

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Quests];
		if (!ctx) return;

		ctx.Write(m_Expansion_QuestID);
		ctx.Write(m_Expansion_IsQuestGiver);
		ctx.Write(m_Expansion_IsObjectiveLootItem);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Quests];
		if (!ctx) return true;

		if (ctx.GetVersion() < 44)
			return true;

		if (!ctx.Read(m_Expansion_QuestID))
			return false;

		if (ctx.GetVersion() < 49)
			return true;

		if (!ctx.Read(m_Expansion_IsQuestGiver))
			return false;
		
		if (ctx.GetVersion() < 50)
			return true;

		if (!ctx.Read(m_Expansion_IsObjectiveLootItem))
			return false;

		return true;
	}
#endif
};
