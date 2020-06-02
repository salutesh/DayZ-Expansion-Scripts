class ExpansionMissionEventAI extends ExpansionMissionEventBase
{
	ref ExpansionAIMissionMeta MissionMeta;

	ref array< ref ExpansionSoldierLocation > SoldierLocation;
	ref array< ref ExpansionLootLocation > LootLocation;

	[NonSerialized()]
	autoptr array< ItemBase > m_Barrels;

	[NonSerialized()]
	autoptr array< ExpansionAIBase > m_Soldiers;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAI
	// ------------------------------------------------------------
	void ExpansionMissionEventAI()
	{
		m_EventName = "AI";

		SoldierLocation = new array< ref ExpansionSoldierLocation >;
		LootLocation = new array< ref ExpansionLootLocation >;

		m_Barrels = new array< ItemBase >;
		m_Soldiers = new array< ExpansionAIBase >;
	}
	
	override void Event_OnStart()
	{
		for ( int sl = 0; sl < SoldierLocation.Count(); sl++ ) 
		{
			ref ExpansionSoldierLocation soldierLocation = SoldierLocation.Get( sl );
			if (soldierLocation)
			{
				m_Soldiers.Insert(CreateExpansionAI(soldierLocation.Name, Vector(soldierLocation.x, soldierLocation.y, soldierLocation.z), "0 0 0", soldierLocation.Clothes));
			}
		}

		for ( int ll = 0; ll < LootLocation.Count(); ll++ ) 
		{
			ref ExpansionLootLocation lootLocation = LootLocation.Get( sl );
			if (lootLocation)
			{
				ItemBase container = ItemBase.Cast( SpawnObject(lootLocation.Name, Vector(lootLocation.x, lootLocation.y, lootLocation.z), "0.000000 0.000000 0.000000") );
				if (container.IsInherited(Barrel_ColorBase))
				{
					Barrel_ColorBase barrel = Barrel_ColorBase.Cast(container);
					barrel.Open();
				}

				for (int p = 0; p < lootLocation.Items; p++)
				{
					container.GetInventory().CreateInInventory(lootLocation.Loot.GetRandomElement());
				}

				m_Barrels.Insert(container);
			}
		}
	}

	override void Event_OnEnd()
	{
		while ( m_Barrels.Count() > 0 )
		{
			int barrelIndex = m_Barrels.Count() - 1;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, barrelIndex * 5, false, m_Barrels[ barrelIndex ] );
			m_Barrels.Remove( barrelIndex );
		}

		while ( m_Soldiers.Count() > 0 )
		{
			int soldierIndex = m_Soldiers.Count() - 1;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, soldierIndex * 5, false, m_Soldiers[ soldierIndex ] );
			m_Soldiers.Remove( soldierIndex );
		}
	}

	protected override void OnLoadMission()
	{
		JsonFileLoader<ExpansionMissionEventAI>.JsonLoadFile( m_FileName, this );
	}

	protected override void OnSaveMission()
	{
		JsonFileLoader<ExpansionMissionEventAI>.JsonSaveFile( m_FileName, this );
	}

	override int MaxDefaultMissions()
	{
		return 1;
	}

	protected override string OnDefaultMission( int index )
	{
		Enabled = true;

		Weight = 5;
		MissionMaxTime = 100;
	
		switch ( index )
        {
        default:
        case 0:
			Weight = 5;
			MissionMeta = new ExpansionAIMissionMeta( "Zaprudino", "A high tier soldiers mission appeared near Zaprudino" );

			SoldierLocation.Insert( new ExpansionSoldierLocation("ExpansionSoldierLewis", 5626.089844, 158.704315, 12602.038086, {
				"ExpansionTacticalGlovesDesert",
				"USMCPants_Desert",
				"ExpansionDesertTortilla",
				"ExpansionMich2001Desert",
				"NVGoggles",
				"JungleBoots_Beige",
				"USMCJacket_Desert",
				"SportGlasses_Black",
				"UKAssVest_Khaki",	
			}));

			LootLocation.Insert( new ExpansionLootLocation( "Barrel_Blue", 5593.746582, 158.851974, 12603.505859, 10, {
				"SVD",
				"SVD",
				"SKS",
				"M4A1",
				"AKM",
				"Mag_STANAG_30Rnd",
				"Mag_AKM_30Rnd",
				"Mag_SVD_10Rnd",
				"Mag_VSS_10Rnd",
				"PlateCarrierVest",
				"PlaceCarrierHolster",
				"PlaceCarrierPouches",
				"Rangefinder",
			}));

			break;
		}

		MissionName = MissionMeta.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
        return fname;
	}

	ExpansionAIBase CreateExpansionAI( string type, vector position, vector orientation, array<string> gear )
	{
		Object obj = GetGame().CreateObject( type, position );

		ExpansionAIBase bot = ExpansionAIBase.Cast( obj );

		if ( !bot && obj ) 
		{
			GetGame().ObjectDelete( obj ); 
			return NULL;
		}

		bot.SetOrientation( orientation );
		
		if ( gear )
		{
			for( int i = 0; i < gear.Count(); i++ )
			{
				bot.GetInventory().CreateAttachment( gear[i] );
			}
		}

		return bot;
	}

	Object SpawnObject( string type, vector position, vector orientation, float lifetime = 0 )
	{
		Object obj;
		obj = GetGame().CreateObject( type, position, true, false, true );
		
		if ( obj )
		{
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			vector roll = obj.GetOrientation();
			roll [ 2 ] = roll [ 2 ] - 1;
			obj.SetOrientation( roll );
			roll [ 2 ] = roll [ 2 ] + 1;
			obj.SetOrientation( roll );

			if (lifetime != 0)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( GetGame().ObjectDelete, lifetime * 60 * 1000, false, obj );
			}
		}

		return obj;
	}
}