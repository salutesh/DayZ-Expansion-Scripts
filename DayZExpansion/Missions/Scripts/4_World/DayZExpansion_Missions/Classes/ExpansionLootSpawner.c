class ExpansionLootSpawner
{
	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	static void AddItem(EntityAI container, ExpansionAirdropLoot loot )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::AddItem - Start");
		#endif

		string className = loot.Name;
		
		TStringArray attachments = loot.Attachments;

		if ( loot.Variants && loot.Variants.Count() > 0 )
		{
			array< float > chances = new array< float >;

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
				if ( loot.Variants[index].Attachments && loot.Variants[index].Attachments.Count() > 0 )
					attachments = loot.Variants[index].Attachments;
			}
		}

		Spawn( className, container, loot.QuantityPercent, attachments ); 
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::AddItem - End");
		#endif
	}

	static void Spawn( string className, EntityAI parent, int quantityPercent, TStringArray attachments )
	{
        ItemBase itembs;
		ItemBase item;

		if ( Class.CastTo( itembs, parent ) )
        {
            item = ItemBase.Cast( itembs.ExpansionCreateInInventory( className ) );
        }
        else
        {
            item = ItemBase.Cast( parent.GetInventory().CreateInInventory( className ) );
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
				//! Yes this is needed, otherwise you get NULL pointers if there is more than one attachment :-(
				TStringArray attachmentsTmp();
				attachmentsTmp.Copy(attachments);

				foreach ( string attachment: attachmentsTmp )
				{
					Spawn( attachment, item, quantityPercent, NULL );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveContainer
	// ------------------------------------------------------------
	static void RemoveContainer(EntityAI container)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::RemoveContainer - Start");
		#endif
		
		GetGame().ObjectDelete( container );
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::RemoveContainer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SpawnLoot
	// ------------------------------------------------------------
	static void SpawnLoot(EntityAI container, array < ref ExpansionAirdropLoot > Loot, int ItemCount )
	{
		array< float > chances = new array< float >;
		array< int > max = new array< int >;

		for ( int i = 0; i < Loot.Count(); ++i )
		{
			chances.Insert( Loot[i].Chance );

			max.Insert( Loot[i].Max );
		}

		if (ItemCount < 0)
			ItemCount = Math.RandomInt(1,-ItemCount);

		int LootItemsSpawned = 0;
		while ( LootItemsSpawned < ItemCount )
		{
			//! Chances are treated as weights here, otherwise it wouldn't make sense as we always want a fixed number of items
			int index = ExpansionStatic.GetWeightedRandom( chances );

			if ( index > -1 )
			{
				LootItemsSpawned++;

				AddItem( container, Loot.Get( index ) );

				if ( max[index] > 0 )
					max[index] = max[index] - 1;

				if ( max[index] == 0 )
					chances[index] = 0;
			} else
			{
				Print("ExpansionLootSpawner::SpawnLoot couldn't select a loot item to spawn (all chances zero?) - items spawned : " + LootItemsSpawned);
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion IsPlayerNearby
	// Check if player is nearby container
	// ------------------------------------------------------------
	static bool IsPlayerNearby(EntityAI container, float radius)
	{		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::IsPlayerNearby - Start");
		#endif
		
		vector pos = container.GetPosition();
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);
		float distance;
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			Man player = players[i];
			if (!player) continue;
			
			distance = vector.Distance( pos, player.GetPosition() );
			
			if ( distance <= radius )
			{
				#ifdef EXPANSION_MISSION_EVENT_DEBUG
				EXLogPrint("ExpansionLootSpawner::IsPlayerNearby - End and return true");
				#endif
				return true;
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionLootSpawner::IsPlayerNearby - End and return false");
		#endif
		
		return false;
	}
};
