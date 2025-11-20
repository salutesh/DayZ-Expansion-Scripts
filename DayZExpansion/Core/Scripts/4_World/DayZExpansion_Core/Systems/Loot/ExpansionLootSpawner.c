/**
 * ExpansionLootSpawner.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLootSpawner
{
	static void AddItem(EntityAI container, ExpansionLoot loot, array<EntityAI> spawnedEntities = null, map<string, int> spawnedEntitiesMap = null, bool spawnOnGround = false, float damagePercentMin = 0, float damagePercentMax = 0)
	{
		string className = loot.Name;
		
		array<ref ExpansionLootVariant> attachments = loot.Attachments;

		if ( loot.Variants && loot.Variants.Count() > 0 )
		{
			array< float > chances = new array< float >;

			int count = loot.Variants.Count();
			float chance;
			float chancesSum;

			foreach (ExpansionLootVariant variant: loot.Variants)
			{
				chance = variant.Chance;
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
				if ( loot.Variants[index].Attachments && loot.Variants[index].Attachments.Count() > 0 )
					attachments = loot.Variants[index].Attachments;
			}
		}

		if ( loot.m_Remaining > 0 )
			loot.m_Remaining--;

		if ( loot.m_Remaining == 0 )
			loot.m_RemainingChance = 0;

		Spawn( className, container, loot.QuantityPercent, attachments, spawnedEntities, spawnedEntitiesMap, spawnOnGround, damagePercentMin, damagePercentMax ); 
	}

	static void Spawn( string className, EntityAI parent, int quantityPercent, array<ref ExpansionLootVariant> attachments = null, array<EntityAI> spawnedEntities = null, map<string, int> spawnedEntitiesMap = null, bool spawnOnGround = false, float damagePercentMin = 0, float damagePercentMax = 0 )
	{
        ItemBase itemParent;
		ItemBase item;
		
		if ( !spawnOnGround )
		{
			if ( Class.CastTo( itemParent, parent ) )
	        {
	            item = ItemBase.Cast( itemParent.ExpansionCreateInInventory( className ) );
	        }
	        else
	        {
	            item = ItemBase.Cast( parent.GetInventory().CreateInInventory( className ) );
	        }
		}
		else
		{
			vector spawnPos = ExpansionMath.GetRandomPointInRing(parent.GetWorldPosition(), 3, 5);
			item = ItemBase.Cast(g_Game.CreateObjectEx(className, spawnPos, ECE_PLACE_ON_SURFACE));
		}

		if ( item )
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

			if ( Class.CastTo( mag, item ) )
			{
				//! Ammo/magazines

				if (quantityPercent == -1)
					quantity = mag.GetAmmoMax();
				else
					quantity = Math.Round( mag.GetAmmoMax() * quantity01 );

				if (!quantity && item.IsAmmoPile())
					quantity = 1;

				mag.ServerSetAmmoCount( quantity );
			}
			else if ( item.HasQuantity() )
			{
				if (quantityPercent == -1)
					quantity = item.GetQuantityInit();
				else
					quantity = Math.Round( item.GetQuantityMax() * quantity01 );

				item.SetQuantity( quantity );
			}

			if ( attachments )
			{
				//! Need to copy the attachments array because passing in NULL in the recursive call below
				//! nulls our original attachments array in caller :-(
				//! https://feedback.bistudio.com/T173458
				array<ref ExpansionLootVariant> attachmentsTmp = {};
				foreach (ExpansionLootVariant attachment: attachments)
				{
					if ( Math.AbsFloat(attachment.Chance) == 1.0 || attachment.Chance > Math.RandomFloat(0.0, 1.0) )
						attachmentsTmp.Insert(attachment);
				}

				foreach ( ExpansionLootVariant attachmentTmp: attachmentsTmp )
				{
					//! Need to pass in spawnedEntities and spawnedEntitiesMap due to T173458 as well
					Spawn( attachmentTmp.Name, item, quantityPercent, NULL, spawnedEntities, spawnedEntitiesMap, false, damagePercentMin, damagePercentMax );
				}
			}
			
			if (damagePercentMin > 0 || damagePercentMax > 0)
			{
				float maxHealth = item.GetMaxHealth("", "");
				float healthModifier = Math.RandomFloatInclusive(damagePercentMin, damagePercentMax);
				item.SetHealth("", "", maxHealth * healthModifier);
			}

			if (spawnedEntitiesMap)
			{
				int current;
				if (spawnedEntitiesMap.Find(className, current))
					spawnedEntitiesMap.Set(className, current + item.Expansion_GetStackAmount());
				else
					spawnedEntitiesMap.Insert(className, item.Expansion_GetStackAmount());
			}

			if (spawnedEntities)
				spawnedEntities.Insert(item);
		}
		else
		{
			if (ExpansionStatic.ItemExists(className))
				EXPrint("ExpansionLootSpawner::Spawn - WARNING: Couldn't spawn " + className + " on " + parent);
			else
				Error("ExpansionLootSpawner::Spawn - ERROR: Couldn't spawn " + className + " - item with that classname does not exist!");
		}
	}

	static void SpawnLoot(EntityAI container, array < ref ExpansionLoot > loot, int itemCount, array<EntityAI> spawnedEntities = null, map<string, int> spawnedEntitiesMap = null, bool spawnOnGround = false, float damagePercentMin = 0, float damagePercentMax = 0 )
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, ExpansionLootSpawner);
	#endif

		if (itemCount < 0)
			itemCount = Math.RandomInt(1, -itemCount);

		array< float > chances = new array< float >;

		int lootItemsSpawned = 0;

		//! Spawn min number of items first
		foreach (ExpansionLoot lootItem: loot)
		{
			lootItem.m_RemainingChance = lootItem.Chance;
			lootItem.m_Remaining = lootItem.Max;

			int min = lootItem.Min;
			while (min > 0 && lootItemsSpawned < itemCount)
			{
				lootItemsSpawned++;
				min--;

				AddItem( container, lootItem, spawnedEntities, spawnedEntitiesMap, spawnOnGround, damagePercentMin, damagePercentMax );
			}

			chances.Insert(lootItem.m_RemainingChance);
		}

		//! Spawn remaining items randomly (if any)
		while ( lootItemsSpawned < itemCount )
		{
			//! Chances are treated as weights here, otherwise it wouldn't make sense as we always want a fixed number of items
			int index = ExpansionStatic.GetWeightedRandom( chances );

			if ( index > -1 )
			{
				lootItemsSpawned++;

				ExpansionLoot randomLootItem = loot[index];

				AddItem( container, randomLootItem, spawnedEntities, spawnedEntitiesMap, spawnOnGround, damagePercentMin, damagePercentMax );

				if ( randomLootItem.m_Remaining == 0 )
					chances[index] = 0;
			} else
			{
				Print("ExpansionLootSpawner::SpawnLoot couldn't select a loot item to spawn (all chances zero?) - items spawned : " + lootItemsSpawned);
				break;
			}
		}
	}

	static bool IsPlayerNearby(EntityAI container, float radius)
	{
		return IsPlayerNearby(container.GetPosition(), radius);
	}

	static bool IsPlayerNearby(vector pos, float radius)
	{
		if (GetCEApi())
			return !GetCEApi().AvoidPlayer(pos, radius);

		array<Man> players = new array<Man>;
		g_Game.GetWorld().GetPlayerList(players);
		float distanceSq;
		float radiusSq = radius * radius;
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			Man player = players[i];
			if (!player) continue;
			
			distanceSq = vector.DistanceSq( pos, player.GetPosition() );
			
			if ( distanceSq <= radiusSq )
			{
				return true;
			}
		}
		
		return false;
	}
};
