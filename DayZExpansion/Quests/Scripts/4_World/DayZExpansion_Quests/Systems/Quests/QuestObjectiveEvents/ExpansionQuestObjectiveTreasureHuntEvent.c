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
	protected ref map<string, int> m_LootItemsMap = new map<string, int>;
	protected ref array<EntityAI> m_LootItems = new array<EntityAI>;
	protected bool m_LootedItemFromChest;
	protected UndergroundStash m_Stash;
	protected ExpansionQuestContainerBase m_Chest;
	protected vector m_StashPos;
	protected bool m_DestinationReached;
	protected ExpansionTravelObjectiveSphereTrigger m_ObjectiveTrigger;
	protected ref ExpansionQuestObjectiveTreasureHuntConfig m_Config;

	//! Event called when the player starts the quest.
	override bool OnEventStart()
	{
		ObjectivePrint(ToString() + "::OnEventStart - Start");

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

		ObjectivePrint(ToString() + "::OnEventStart - End and return TRUE.");

		return true;
	}

	//! Event called when the player starts the quest.
	override bool OnContinue()
	{
		ObjectivePrint(ToString() + "::OnContinue - Start");

		if (!super.OnContinue())
			return false;

		//! Only create the stash trigger when not already completed!
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
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

		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::OnContinue - End and return TRUE.");

		return true;
	}

	override bool OnCleanup()
	{
		ObjectivePrint(ToString() + "::OnCleanup - Start");

		if (!super.OnCleanup())
			return false;

		//! Only cleanup the loot and stash if quest is not completed
		if (m_Quest.GetQuestState() == ExpansionQuestState.STARTED)
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

		ObjectivePrint(ToString() + "::OnCleanup - End and return TRUE.");

		return true;
	}

	override bool OnCancel()
	{
		ObjectivePrint(ToString() + "::OnCancel - Start");

		if (!super.OnCancel())
			return false;

		//! Only cleanup the loot if quest is not completed
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

		ObjectivePrint(ToString() + "::OnCancel - End and return TRUE.");

		return true;
	}

	protected void CreateTreasure()
	{
		ObjectivePrint(ToString() + "::CreateTreasure - Start");

		if (!m_Config)
			return;
		
		bool useStash = m_Config.DigInStash();
		string containerName = m_Config.GetContainerName();
		if (!containerName.ToType().IsInherited(ExpansionQuestContainerBase))
			return;
		
		//m_StashPos[1] = GetGame().SurfaceY(m_StashPos[0], m_StashPos[2]);

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
			m_Stash.SetQuestID(m_Quest.GetQuestConfig().GetID());
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
			GetGame().ObjectDelete(m_Chest);
			return;
		}

		m_Chest.SetQuestID(m_Quest.GetQuestConfig().GetID());

		m_Chest.ExpansionSetCanReceiveItems(true);
		ExpansionLootSpawner.SpawnLoot(m_Chest, m_Config.GetLoot(), m_Config.GetLootItemsAmount(), m_LootItems, m_LootItemsMap);
		m_Chest.ExpansionSetCanReceiveItems(false);	

		ObjectivePrint(ToString() + "::CreateTreasure - End");
	}

#ifdef EXPANSIONMODNAVIGATION
	override void CreateMarkers()
	{
		if (!Class.CastTo(m_Config, m_ObjectiveConfig))
			return;

		ObjectivePrint(ToString() + "::CreateMarkers - Start");
		string markerName = m_Config.GetMarkerName();
		CreateObjectiveMarker(m_StashPos, markerName, m_Config.GetMarkerVisibility());
		ObjectivePrint(ToString() + "::CreateMarkers - End");
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
		ObjectivePrint(ToString() + "::CreateTrigger - Start");

		Class.CastTo(m_ObjectiveTrigger, GetGame().CreateObjectEx("ExpansionTravelObjectiveSphereTrigger", pos, ECE_NONE));
		m_ObjectiveTrigger.SetPosition(pos);
		m_ObjectiveTrigger.SetObjectiveData(this);

		ObjectivePrint(ToString() + ":: CreateTrigger - Created objective trigger at position: " + pos + ".");
		ObjectivePrint(ToString() + "::CreateTrigger - End");
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
		ObjectivePrint(ToString() + "::SetReachedLocation - Start");
		ObjectivePrint(ToString() + "::SetReachedLocation - State: " + state);
		
		if (state)
		{
			CreateTreasure();
		}
		else
		{
			if (m_Chest)
				GetGame().ObjectDelete(m_Chest);

			if (m_Stash)
				GetGame().ObjectDelete(m_Stash);
		}
		
		if (!m_DestinationReached)
			SetLocationState(state);
		
		m_Quest.UpdateQuest(false);
		m_Quest.QuestCompletionCheck();

		ObjectivePrint(ToString() + "::SetReachedLocation - End");
	}

	void SetLocationState(bool state)
	{
		m_DestinationReached = state;
	}

	void LootedItemFromChest()
	{
		ObjectivePrint(ToString() + "::LootedItemFromChest - Start");
		m_LootedItemFromChest = true;
		m_Quest.UpdateQuest(false);
		m_Quest.QuestCompletionCheck();
		ObjectivePrint(ToString() + "::LootedItemFromChest - End");
	}

	void OnInventoryItemLocationChange(ItemBase item, ExpansionQuestItemState state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - State: " + typename.EnumToString(ExpansionQuestItemState, state));
		
		if (m_LootItems.Find(item) == -1)
			return;
		
		int amount;
		if (!m_LootItemsMap.Find(item.GetType(), amount))
			return;
		
		int itemAmount = ExpansionQuestModule.GetModuleInstance().GetItemAmount(item);
		if (itemAmount != amount)
		{
			m_LootedItemFromChest = true;

			m_Quest.UpdateQuest(false);
			m_Quest.QuestCompletionCheck();
		}
		
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - End");
	}

	bool HasLootedItemFromChest()
	{
		return m_LootedItemFromChest;
	}

	override bool CanComplete()
	{
		ObjectivePrint(ToString() + "::CanComplete - Start");
		ObjectivePrint(ToString() + "::CanComplete - m_DestinationReached: " + m_DestinationReached);
		ObjectivePrint(ToString() + "::CanComplete - m_LootedItemFromChest: " + m_LootedItemFromChest);

		bool conditionsResult = m_DestinationReached && m_LootedItemFromChest;
		if (!conditionsResult)
		{
			ObjectivePrint(ToString() + "::CanComplete - End and return: FALSE");
			return false;
		}

		ObjectivePrint(ToString() + "::CanComplete - End and return: TRUE");

		return super.CanComplete();
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

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.TREASUREHUNT;
	}

	override bool HasDynamicState()
	{
		return true;
	}
};