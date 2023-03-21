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
		
		SpawnLoot(m_Chest, m_Config.GetLoot(), m_Config.GetLootItemsAmount());	

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
		return obj;
	}
	
	void Spawn(string className, EntityAI parent, int quantityPercent, TStringArray attachments = null)
	{
        ItemBase itembs;
		ItemBase item;

		if (Class.CastTo(itembs, parent))
        {
            item = ItemBase.Cast(itembs.ExpansionCreateInInventory(className));
        }
        else
        {
            item = ItemBase.Cast(parent.GetInventory().CreateInInventory(className));
        } 
		
		if (item)
		{			
			Magazine_Base mag;

			float quantity;
			float quantity01;

			if (quantityPercent > 0)
			{
				quantity01 = quantityPercent / 100;
			}
			else if (quantityPercent == -2)
			{
				CEItemProfile profile = item.GetEconomyProfile();

				if (profile)
				{
					float quantityMin01 = profile.GetQuantityMin();
					float quantityMax01 = profile.GetQuantityMax();

					if (quantityMin01 >= 0 && quantityMax01 > 0)
						quantity01 = Math.RandomFloatInclusive( quantityMin01, quantityMax01 );
					else
						quantityPercent = -1;
				}
			}

			if (Class.CastTo(mag, item))
			{
				//! Ammo/magazines

				if (quantityPercent == -1)
					quantity = mag.GetAmmoMax();
				else
					quantity = Math.Round(mag.GetAmmoMax() * quantity01);

				if (!quantity && item.IsAmmoPile())
					quantity = 1;

				mag.ServerSetAmmoCount( quantity );
			}
			else if (item.HasQuantity())
			{
				if (quantityPercent == -1)
					quantity = item.GetQuantityInit();
				else
					quantity = Math.Round(item.GetQuantityMax() * quantity01);

				item.SetQuantity(quantity);
			}

			if (attachments)
			{
				//! Yes this is needed, otherwise you get NULL pointers if there is more than one attachment :-(
				TStringArray attachmentsTmp();
				attachmentsTmp.Copy(attachments);

				foreach (string attachment: attachmentsTmp)
				{
					Spawn(attachment, item, quantityPercent);
				}
			}
			
			int current;
			if (m_LootItemsMap.Find(className, current))
			{
				int newAmount = current + quantity;
				m_LootItemsMap.Set(className, newAmount);
			}
			else
			{
				m_LootItemsMap.Insert(className, quantity);
			}
			
			m_LootItems.Insert(item);
		}
	}
	
	void SpawnLoot(EntityAI container, array <ref ExpansionLoot> Loot, int ItemCount)
	{
		array<float> chances = new array<float>;
		array<int> max = new array<int>;

		for (int i = 0; i < Loot.Count(); ++i)
		{
			chances.Insert(Loot[i].Chance);

			max.Insert(Loot[i].Max);
		}

		if (ItemCount < 0)
			ItemCount = Math.RandomInt(1, -ItemCount);

		int LootItemsSpawned = 0;
		while (LootItemsSpawned < ItemCount)
		{
			//! Chances are treated as weights here, otherwise it wouldn't make sense as we always want a fixed number of items
			int index = ExpansionStatic.GetWeightedRandom(chances);

			if (index > -1)
			{
				LootItemsSpawned++;
				AddItem( container, Loot.Get( index ) );

				if ( max[index] > 0 )
					max[index] = max[index] - 1;

				if ( max[index] == 0 )
					chances[index] = 0;
			} 
			else
			{
				Print(ToString() + "::SpawnLoot couldn't select a loot item to spawn (all chances zero?) - items spawned : " + LootItemsSpawned);
				break;
			}
		}
	}
	
	void AddItem(EntityAI container, ExpansionLoot loot )
	{
		string className = loot.Name;
		
		TStringArray attachments = loot.Attachments;

		if (loot.Variants && loot.Variants.Count() > 0)
		{
			array<float> chances = new array< float >;

			int count = loot.Variants.Count();
			float chance;
			float chancesSum;

			for ( int j = 0; j < count; ++j )
			{
				chance = loot.Variants[j].Chance;
				chances.Insert( chance );
				chancesSum += chance;
			}

			//! Determine chance for parent item
			if ( chancesSum < 1.0 )
			{
				//! Chances are treated as actual chances here, i.e. total sum is 1.0
				chance = 1.0 - chancesSum;
			} else
			{
				//! Just give parent item a 1.0 chance
				chance = 1.0;
			}

			chances.Insert( chance );

			int index = ExpansionStatic.GetWeightedRandom( chances );

			if ( index > -1 && index < count )
			{
				className = loot.Variants[index].Name;
				if (loot.Variants[index].Attachments && loot.Variants[index].Attachments.Count() > 0)
					attachments = loot.Variants[index].Attachments;
			}
		}

		Spawn(className, container, loot.QuantityPercent, attachments); 
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

	void OnInventoryItemLocationChange(ItemBase item, string state)
	{
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - Start");
		ObjectivePrint(ToString() + "::OnInventoryItemLocationChange - State: " + state);
		
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