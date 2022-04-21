#ifdef EXPANSIONMODAI
class ExpansionMissionEventAI extends ExpansionMissionEventBase
{
	ref ExpansionAIMissionMeta MissionMeta;

	ref array< ref ExpansionSoldierLocation > SoldierLocation;
	ref array< ref ExpansionLootLocation > LootLocation;

	[NonSerialized()]
	autoptr array< ItemBase > m_Barrels;

	[NonSerialized()]
	autoptr array< eAIDynamicPatrol > m_Soldiers;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAI
	// ------------------------------------------------------------
	void ExpansionMissionEventAI()
	{
		m_EventName = "AI";

		SoldierLocation = new array< ref ExpansionSoldierLocation >;
		LootLocation = new array< ref ExpansionLootLocation >;

		m_Barrels = new array< ItemBase >;
		m_Soldiers = new array< eAIDynamicPatrol >;
	}
	
	override void Event_OnStart()
	{
		for ( int sl = 0; sl < SoldierLocation.Count(); sl++ ) 
		{
			ref ExpansionSoldierLocation soldierLocation = SoldierLocation.Get( sl );
			if (soldierLocation)
			{
				m_Soldiers.Insert(CreateExpansionAI(soldierLocation.StartPosition, soldierLocation.Waypoints, soldierLocation.Loadout, soldierLocation.GroupSize, soldierLocation.Faction));
			}
		}

		for ( int ll = 0; ll < LootLocation.Count(); ll++ ) 
		{
			ref ExpansionLootLocation lootLocation = LootLocation.Get( sl );
			if (lootLocation)
			{
				ItemBase container = ItemBase.Cast( SpawnObject(lootLocation.Name, lootLocation.Position, "0.000000 0.000000 0.000000") );
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

			vector start_position = "5626.089844 158.704315 12602.038086";
			array<vector> patrol = new array<vector>();
			patrol.Insert(start_position);
			SoldierLocation.Insert( new ExpansionSoldierLocation(start_position, patrol, "SoldierLoadout.json", "West"));

			SoldierLocation.Insert( new ExpansionSoldierLocation(start_position, patrol, "HumanLoadout.json", "Civilian"));

			SoldierLocation.Insert( new ExpansionSoldierLocation(start_position, patrol, "SoldierLoadout.json", "East"));

			LootLocation.Insert( new ExpansionLootLocation( "Barrel_Blue", start_position, 10, {
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

	eAIDynamicPatrol CreateExpansionAI(vector startpos, array<vector> waypoints, string loadout = "", int groupsize = -1, string faction = "")
	{
		eAIFaction efaction;
		eAIWaypointBehavior waypointbehavior = eAIWaypointBehavior.REVERSE;
		switch (faction)
		{
			case "West":
			{
				efaction = new eAIFactionWest();
				break;
			}
			case "East":
			{
				efaction = new eAIFactionEast();
				break;
			}
			case "Raiders":
			{
				efaction = new eAIFactionRaiders();
				break;
			}
			case "Civilian":
			{
				efaction = new eAIFactionCivilian();
				break;
			}
			default:
			{
				efaction = new eAIFactionCivilian();
				break;
			}
		}

		if ( groupsize < 0 )
			groupsize = Math.RandomInt(1,-groupsize);

		if ( waypoints.Count() <= 1 )
			waypointbehavior = eAIWaypointBehavior.HALT;

		return eAIDynamicPatrol.Create(startpos, waypoints, waypointbehavior, loadout, groupsize, 48000, efaction, true, 10, 800, 2);
	}

	Object SpawnObject( string type, vector position, vector orientation, float lifetime = 0 )
	{
		Object obj;
		//GetGame().CreateObjectEx( type, position, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_AIRBORNE );
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
#endif