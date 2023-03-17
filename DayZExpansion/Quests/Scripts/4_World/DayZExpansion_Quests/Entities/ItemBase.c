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

modded class ItemBase
{
	protected static ref array<ExpansionQuestObjectiveEventBase> s_Expansion_AssignedQuestObjectives = new array<ExpansionQuestObjectiveEventBase>;
	protected int m_Expansion_QuestID = -1;
	protected bool m_Expansion_IsQuestGiver;
	protected ref TStringArray m_ExpansionOjectiveItemExcludes = {"FireplaceBase"};

	void ItemBase()
	{
		RegisterNetSyncVariableInt("m_Expansion_QuestID");
		RegisterNetSyncVariableBool("m_Expansion_IsQuestGiver");
	}

	bool IsQuestItem()
	{
		return (m_Expansion_QuestID > -1);
	}

	int GetQuestID()
	{
		return m_Expansion_QuestID;
	}

	void SetQuestID(int id)
	{
		m_Expansion_QuestID = id;
	}

	bool IsQuestGiver()
	{
		return m_Expansion_IsQuestGiver;
	}

	void SetIsQuestGiver(bool state)
	{
		m_Expansion_IsQuestGiver = state;
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (m_Expansion_IsStoreLoaded && m_Expansion_QuestID > -1)
		{
			if (!GetHierarchyRootPlayer())
				GetGame().ObjectDelete(this);
		}

		Man player = GetHierarchyRootPlayer();
		if (player && player.GetIdentity())
			CheckAssignedObjectivesForEntity("INV_ENTER", player);
	}

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (player && player.GetIdentity())
			{
				if (m_Expansion_QuestID > -1 && !IsQuestGiver())
				{
					//! Delete this item when it has a quest ID assigned but the player who picks it up has not a active quest instance with that ID.
					string playerUID = player.GetIdentity().GetId();
					if (!ExpansionQuestModule.GetModuleInstance().HasActiveQuestWithID(playerUID, m_Expansion_QuestID))
						GetGame().ObjectDelete(this);
				}

				CheckAssignedObjectivesForEntity("INV_ENTER", player);
			}
		}
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			if (player && player.GetIdentity())
			{
				if (m_Expansion_QuestID > -1 && !IsQuestGiver())
				{
					//! Delete this item when it has a quest ID assigned but the player dropping it has not a active quest instance with that ID.
					string playerUID = player.GetIdentity().GetId();
					if (!ExpansionQuestModule.GetModuleInstance().HasActiveQuestWithID(playerUID, m_Expansion_QuestID))
						GetGame().ObjectDelete(this);
				}

				CheckAssignedObjectivesForEntity("INV_EXIT", player);
			}
		}
	}

	override void OnQuantityChanged(float delta)
	{
		super.OnQuantityChanged(delta);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			CheckAssignedObjectivesForEntity("QUANTITY_CHANGED");
		}
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionUseQuestItem);
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Quests];
		if (!ctx) return;

		ctx.Write(m_Expansion_QuestID);
		ctx.Write(m_Expansion_IsQuestGiver);
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

		return true;
	}
#endif

	override bool CanBeSplit()
	{
		if (IsQuestItem())
		{
			Man itemOwner = GetHierarchyRootPlayer();
			if (itemOwner && itemOwner.GetIdentity())
			{
				StringLocaliser text = new StringLocaliser("The item %1 is a quest item and can't be split!", GetDisplayName());
				ExpansionNotification("Can't split item", text, "Error", COLOR_EXPANSION_NOTIFICATION_ORANGE, 7).Create(itemOwner.GetIdentity());
			}

			return false;
		}

		return super.CanBeSplit();
	}

	static void AssignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index == -1)
		{
			s_Expansion_AssignedQuestObjectives.Insert(objective);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("ItemBase::AssignQuestObjective - Assigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("ItemBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is already assigned to this entity! Skiped");
		}
	#endif
	}

	static void DeassignQuestObjective(ExpansionQuestObjectiveEventBase objective)
	{
		int index = s_Expansion_AssignedQuestObjectives.Find(objective);
		if (index > -1)
		{
			s_Expansion_AssignedQuestObjectives.Remove(index);
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print("ItemBase::DeassignQuestObjective - Deassigned quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID());
		#endif
		}
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		else
		{
			Print("ItemBase::AssignQuestObjective - Quest objective: Type: " + objective.GetObjectiveType() + " | ID: " + objective.GetObjectiveConfig().GetID() + " is not assigned to this entity and cant be deassigned!");
		}
	#endif
	}

	protected void CheckAssignedObjectivesForEntity(string state, Man player = null)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectivesForEntity - Start");
	#endif

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;

		foreach (string typeName: m_ExpansionOjectiveItemExcludes)
		{
			if (GetType().ToType().IsInherited(typeName.ToType()))
				return;
		}

		if (!s_Expansion_AssignedQuestObjectives || s_Expansion_AssignedQuestObjectives.Count() == 0)
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::CheckAssignedObjectivesForEntity - No quest objectives assigned!");
		#endif
			return;
		}

		string playerUID;
		if (player && player.GetIdentity())
			playerUID = player.GetIdentity().GetId();

		if (state != "QUANTITY_CHANGED" && playerUID == string.Empty)
			return;

		foreach (ExpansionQuestObjectiveEventBase objective: s_Expansion_AssignedQuestObjectives)
		{
			if (!objective || !objective.GetQuest())
				continue;

			//! Check if the current objective belongs to the item owner if we got the UID.
			if (playerUID != string.Empty && !objective.GetQuest().IsQuestPlayer(playerUID))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectivesForEntity - Player with UID [" + playerUID + "] is not a quest player of this quest objective. Skip..");
			#endif
				return;
			}

			//! Check if the current objective is active
			if (!objective.IsActive())
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CheckAssignedObjectivesForEntity - Objective is not active. Skip..");
			#endif
				continue;
			}

			OnObjectiveItemInventoryChange(objective, player, state);
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CheckAssignedObjectivesForEntity - End");
	#endif
	}

	protected void OnObjectiveItemInventoryChange(ExpansionQuestObjectiveEventBase objective, Man player, string state)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnObjectiveItemInventoryChange - Start");
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
					//! So we compare the quest ID that has been applied to to item if so.
					int questID = deliveryObjective.GetQuest().GetQuestConfig().GetID();
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print(ToString() + "::OnObjectiveItemInventoryChange - " + objective.ToString() + " is a Delivery objective. Objective quest ID: " + questID + " | Item quest ID: " + m_Expansion_QuestID);
				#endif
					if (m_Expansion_QuestID != questID)
						return;

					deliveryObjective.OnInventoryItemLocationChange(this, player, state);
				}
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionEvent collectionObjective;
				if (Class.CastTo(collectionObjective, objective))
				{
					collectionObjective.OnInventoryItemLocationChange(this, player, state);
				}
			}
			break;
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingEvent craftingObjective;
				if (Class.CastTo(craftingObjective, objective))
				{
					craftingObjective.OnInventoryItemLocationChange(this, player, state);
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
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::OnObjectiveItemInventoryChange - End");
	#endif
	}

#ifdef EXPANSIONMODHARDLINE
	override ExpansionHardlineItemRarity Expansion_GetRarity()
	{
		if (IsQuestItem())
			return ExpansionHardlineItemRarity.Quest;

		return m_Expansion_Rarity;
	}
#endif
};