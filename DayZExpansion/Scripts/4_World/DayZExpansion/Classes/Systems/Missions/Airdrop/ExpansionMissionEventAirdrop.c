/**
 * ExpansionMissionEventAirdrop.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMissionEventAirdrop extends ExpansionMissionEventBase
{
	bool ShowNotification;
	
	float Height;
	float Speed;

	string Container;
	
	ref ExpansionAirdropLocation DropLocation;

	ref array < ref ExpansionAirdropLoot > Loot;
	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;

	[NonSerialized()]
	ExpansionAirdropPlane m_Plane;

	[NonSerialized()]
	ExpansionAirdropContainerBase m_Container;

	[NonSerialized()]
	static int m_LocationsCount = -1;

	[NonSerialized()]
	static ref array < ref ExpansionLocatorArray > m_AvailableLocations = new array < ref ExpansionLocatorArray >;

	[NonSerialized()]
	static ref array < ref ExpansionLocatorArray > m_SelectedLocations = new array < ref ExpansionLocatorArray >;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAirdrop
	// ------------------------------------------------------------
	void ExpansionMissionEventAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionEventAirdrop - Start");
		#endif
		
		m_EventName = "Airdrop";
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionEventAirdrop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnStart
	// ------------------------------------------------------------
	// handle mission start
	override void Event_OnStart()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnStart - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			ExpansionAirdropLootContainer container;

			if ( Loot.Count() == 0 || Infected.Count() == 0 )
			{
				//! No mission-specific loot and/or infected defined, fall back to airdrop settings

				array< ref ExpansionAirdropLootContainer > containers = new array< ref ExpansionAirdropLootContainer >;

				//! Get all containers enabled for mission use that match our container name (or any if random)
				string containerName = Container;
				containerName.ToLower();
				for ( int i = 0; i < GetExpansionSettings().GetAirdrop().Containers.Count(); i++ )
				{
					container = GetExpansionSettings().GetAirdrop().Containers[i];
					if ( ( container.Usage == 0 || container.Usage == 1 ) && ( container.Container == Container || containerName == "random" ) )
					{
						containers.Insert( container );
					}
				}

				if ( containers.Count() == 0 )
				{
					string errorMsg = "No mission-specific loot and/or infected defined for mission " + MissionName + " and airdrop settings do not contain compatible container (looked for " + Container + " with Usage set to 0 or 1)";
					ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "ERROR"), new StringLocaliser(errorMsg)).Error();
					return;
				}

				//! To make weights work like the previous system where there were
				//! <number of locations = n> * <number of containers = 4> mission files, we do some adjustments.
				//! The reason we want this is because locations with low 'Weight' (probability) have a higher chance to spawn a container with
				//! high 'Weight' (probability) and vice versa, which makes it (e.g.) more likely to get a military airdrop
				//! (with our default container probabilities) when a location with low 'Weight' gets picked.

				//! 1. Find max weight and calculate sum
				float maxWeight;
				float weightSum;
				for ( i = 0; i < containers.Count(); i++ )
				{
					if ( containers[i].Weight > maxWeight )
						maxWeight = containers[i].Weight;

					weightSum += containers[i].Weight;
				}
				//! 2. Take mission weight into account
				float weight = ( Weight + weightSum - maxWeight ) / containers.Count();
				array< float > weights = new array< float >;
				for ( i = 0; i < containers.Count(); i++ )
				{
					float cweight;
					if ( weight - maxWeight >= 0 )
						cweight = weight - maxWeight + containers[i].Weight;
					else
						cweight = containers[i].Weight;
					weights.Insert( cweight );
				}
				EXPrint("[ExpansionMissionEventAirdrop] Adjusted container weights:");
				for ( i = 0; i < weights.Count(); i++ )
				{
					EXPrint(containers[i].Container + " " + weights[i]);
				}

				//! Pick a container (weighted random)
				container = ExpansionAirdropLootContainer.GetWeightedRandomContainer( containers, weights );

				Container = container.Container;

				if ( Loot.Count() == 0 )
					Loot = container.Loot;

				if ( Infected.Count() == 0 )
					Infected = container.Infected;

				if ( ItemCount <= 0 )
					ItemCount = container.ItemCount;

				if ( InfectedCount == -1 )
					InfectedCount = container.InfectedCount;
			}

			#ifdef EXPANSION_MISSION_EVENT_DEBUG
			EXPrint("[ExpansionMissionEventAirdrop] " + MissionName + " - selected container: " + Container);
			#endif

			if ( ItemCount <= 0 )
				ItemCount = GetExpansionSettings().GetAirdrop().ItemCount;

			container = new ExpansionAirdropLootContainer( Container, 1, 1, Loot, Infected, ItemCount, InfectedCount );

			StringLocaliser warningProximityMsg;
			StringLocaliser airdropCreatedMsg;

			if ( ShowNotification )
			{
				warningProximityMsg = new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON", DropLocation.Name );
				airdropCreatedMsg = new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED", DropLocation.Name );
			}
	
			m_Plane = ExpansionAirdropPlane.CreatePlane( Vector( DropLocation.x, 0, DropLocation.z ), DropLocation.Name, DropLocation.Radius, Height, Speed, container, warningProximityMsg, airdropCreatedMsg );
			
			if ( m_Plane )
			{
				if (GetExpansionSettings().GetNotification().ShowAirdropStarted && ShowNotification)
				{
					CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_HEADING_TOWARDS", DropLocation.Name ), "set:expansion_notification_iconset image:icon_airdrop", 7 );
				}

				if ( GetExpansionSettings().GetLog().MissionAirdrop )
					GetExpansionSettings().GetLog().PrintLog( "[MissionAirdrop] An airdrop is heading towards \"" + DropLocation.Name + "\" (pos=" + Vector( DropLocation.x, 0, DropLocation.z ) + " type="+ MissionName +") with a "+ Container );
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnStart - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnEnd
	// ------------------------------------------------------------
	// handle mission cleanup
	override void Event_OnEnd()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnEnd - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			//! After mission ends check all 60 seconds if a player is nearby the airdrop crate and if not delete the container
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CleanupCheck, 60 * 1000, true );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnEnd - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CleanupCheck
	// ------------------------------------------------------------
	void CleanupCheck()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::CleanupCheck - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			if ( !m_Container )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CleanupCheck );
				return;
			}
			
			//! Check if a player is nearby the container in a 1500 meter radius
			if ( !m_Container.IsPlayerNearby(1500) )
			{
				m_Container.RemoveContainer();
				
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove( this.CleanupCheck );

				if ( GetExpansionSettings().GetNotification().ShowAirdropEnded && ShowNotification )
				{
					CreateNotification(new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_ENDED", DropLocation.Name), EXPANSION_NOTIFICATION_ICON_AIRDROP, 7);
				}
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::CleanupCheck - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnUpdate
	// ------------------------------------------------------------
	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnUpdate - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			if ( m_Plane )
			{			
				if ( m_Plane.AirdropCreated() && !m_Container )
				{	
					m_Container = m_Plane.GetContainer();
					
					if ( m_Container )
					{
						m_CurrentMissionTime = 0;
					}
				}
			}
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::Event_OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnLoadMission
	// ------------------------------------------------------------
	protected override void OnLoadMission()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnLoadMission - Start");
		#endif

		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionMissionEventAirdrop>.JsonLoadFile( m_FileName, this );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnLoadMission - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnSaveMission
	// ------------------------------------------------------------
	protected override void OnSaveMission()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnSaveMission - Start");
		#endif

		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionMissionEventAirdrop>.JsonSaveFile( m_FileName, this );
		}
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::OnSaveMission - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion MaxDefaultMissions
	// ------------------------------------------------------------
	override int MaxDefaultMissions()
	{		
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}
		
		switch (world_name)
		{
			case "chernarusplus":
				return 13; //! amount of locations
			break;
			case "enoch":
				return 12;
			break;
			case "deerisle":
				return 13;
			break;
			case "namalsk":
				return 9;
			break;
			case "chiemsee":
				return 13;
			break;
			case "sandbox":
				return 5;
			break;
			/*
			case "rostow":
				return 9; //! TODO
			break;
			case "esseker":
				return 9; //! TODO
			break;
			case "valning":
				return 9; //! TODO
			break;
			case "banov":
				return 9; //! TODO
			break;
			case "takistanplus":
				return 9; //! TODO
			break;
			case "expansiontest":
				return 9; //! TODO
			break;
			*/
		}

		//! Unknown map
		if ( m_LocationsCount == -1 )
		{
			//! Get possible locations from world config
			array< ref ExpansionLocatorArray > locs = ExpansionLocatorModule.GetWorldLocations();

			for ( int i = 0; i < locs.Count(); i ++ )
			{
				if ( locs[i].type.Contains( "Capital" ) || locs[i].type.Contains( "City" ) || locs[i].type.Contains( "Village" ) )
				{
					m_AvailableLocations.Insert( locs[i] );
				}
			}

			m_LocationsCount = m_AvailableLocations.Count();
		}

		return Math.Min( m_LocationsCount, 13 );
	}
	
	// ------------------------------------------------------------
	// Expansion OnDefaultMission
	// ------------------------------------------------------------
	protected override string OnDefaultMission( int index )
	{
		Enabled = true;

		//! To make weights work like the previous system where there were
		//! <number of locations = n> * <number of containers = 4> mission files, we do some adjustments.
		//! The reason we want this is because locations with low 'Weight' (probability) have a higher chance to spawn a container with
		//! high 'Weight' (probability) and vice versa, which makes it (e.g.) more likely to get a military airdrop
		//! (with our default container probabilities) when a location with low 'Weight' gets picked.
		Weight = 0;
		int max = MaxDefaultMissions() * 4;
		for ( int i = 0; i < max; i++ )
		{
			if ( (int) Math.Floor( i / 4.0 ) == index )
				Weight += 1 + ( ( i + 1 ) * 5 );
		}

		MissionMaxTime = 1200; // 20 minutes

		Speed = 25.0;
		Height = 450.0;
		
		ShowNotification = true;

		MissionName = "Random";
		Container = "Random";

		Loot = {};
		Infected = {};

		ItemCount = -1;  //! -1 = use value from container from airdrop settings
		InfectedCount = -1;  //! -1 = use value from container from airdrop settings
		
		//! Set default locations depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}
				
		switch (world_name)
		{
			case "chernarusplus":
				return ExpansionMissionAirdropChernarus( index );
			break;
			case "enoch":
				return ExpansionMissionAirdropLivonia( index );
			break;
			case "deerisle":
				return ExpansionMissionAirdropDeerIsle( index );
			break;
			case "namalsk":
				return ExpansionMissionAirdropNamalsk( index );
			break;
			case "chiemsee":
				return ExpansionMissionAirdropChiemsee( index );
			break;
			case "rostow":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "esseker":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "valning":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "banov":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "sandbox":
				return ExpansionMissionAirdropSandbox( index );
			break;
			case "expansiontest":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
		}
		
		//! Map unknown, loading default config with random locations from map
		return ExpansionMissionAirdropRandom( index );
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropChernarus
	// ------------------------------------------------------------
	string ExpansionMissionAirdropChernarus(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChernarus::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 4807, 9812, 100, "NWAF" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 12159, 12583, 100, "NEAF" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 11464, 8908, 100, "Berezino" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 5043, 2505, 100, "Balota" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 2351, 5393, 100, "Zelenogorsk" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 2036, 7491, 100, "Myshkinko" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 11125, 14040, 100, "Novodmitrovsk" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 6128, 2497, 100, "Chernogorsk" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 9371, 2229, 100, "Elektrozavodsk" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 13452, 3112, 100, "Skalisty Island" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 2700, 6193, 100, "Sosnovka" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 7436, 7720, 100, "Novy Sobor" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 5823, 7764, 100, "Stary Sobor" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChernarus::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropLivonia
	// ------------------------------------------------------------
	string ExpansionMissionAirdropLivonia(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropLivonia::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 3478, 10130, 100, "NWAF" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 1367, 7057, 100, "Topolin" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 6692, 11442, 100, "Brena" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 9976, 11000, 100, "Tarnow" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 7209, 6073, 100, "Radunin" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 5996, 4513, 100, "Nadbor" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 11491, 763, 100, "Dolnik" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 10899, 4515, 100, "Gieraltow" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 9975, 7242, 100, "Karlin" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 8676, 12114, 100, "Kolembrody" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 8399, 8803, 100, "Zapadlisko" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 4559, 6852, 100, "Muratyn" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropLivonia::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropDeerIsle
	// ------------------------------------------------------------
	string ExpansionMissionAirdropDeerIsle(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropDeerIsle::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 6055, 13429, 100, "Portland" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 7911, 8864, 100, "Mountainville" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 4627, 7507, 100, "Sandy Bay" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 1849, 8830, 100, "Old-Town" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 3614, 6360, 100, "Westbrook" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 6117, 7004, 100, "Oldfield" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 6924, 5473, 100, "Warren-Cove" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 4522, 4370, 100, "Georgstown" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 4539, 2317, 100, "Hazelands" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 4658, 1414, 100, "Greenville" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 6973, 1701, 100, "Stonington" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 7630, 3717, 100, "Kushville" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 10179, 4722, 100, "Oceanville" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropDeerIsle::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropNamalsk
	// ------------------------------------------------------------
	string ExpansionMissionAirdropNamalsk(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropNamalsk::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 6751, 10900, 100, "Vorkuta" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 8008, 10747, 100, "Jalovisko" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 4680, 10872, 100, "Lubjansk" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 5024, 8919, 100, "Sedbjan" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 7493, 7577, 100, "Tara Harbor" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 7114, 6000, 100, "Tara SawMill" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 3974, 7350, 100, "Norisnk" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 6140, 9189, 100, "Airstrip" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 4327, 5230, 100, "Brensk" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropNamalsk::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropSandbox
	// ------------------------------------------------------------
	string ExpansionMissionAirdropSandbox(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropSandbox::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 500, 500, 100, "Middle" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 0, 0, 100, "BottomLeft" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 0, 1000, 100, "BottomRight" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 1000, 0, 100, "TopLeft" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 1000, 1000, 100, "TopRight" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropSandbox::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropChiemsee
	// ------------------------------------------------------------
	string ExpansionMissionAirdropChiemsee(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChiemsee::Defaults - Start");
		#endif

		//! NOTE: Order of locations determines their 'Weight' (probability).
		//! Locations that should get a LOW probability should be at the TOP.
		//! Locations that should get a HIGH probability should be at the BOTTOM.
		switch ( idx )
		{
		default:
		case 0:
			DropLocation = new ExpansionAirdropLocation( 3099, 8394, 100, "Atomkraftwerk" );
			break;
		case 1:
			DropLocation = new ExpansionAirdropLocation( 1137, 2551, 100, "Bachham" );
			break;
		case 2:
			DropLocation = new ExpansionAirdropLocation( 4669, 5240, 100, "Camprefugi" );
			break;
		case 3:
			DropLocation = new ExpansionAirdropLocation( 8957, 8931, 100, "Gollenhausen" );
			break;
		case 4:
			DropLocation = new ExpansionAirdropLocation( 7218, 3495, 100, "Herrnchiemsee" );
			break;
		case 5:
			DropLocation = new ExpansionAirdropLocation( 7262, 9618, 100, "Krater" );
			break;
		case 6:
			DropLocation = new ExpansionAirdropLocation( 3123, 4342, 100, "Mauerkirchen" );
			break;
		case 7:
			DropLocation = new ExpansionAirdropLocation( 6316, 9204, 100, "Obing" );
			break;
		case 8:
			DropLocation = new ExpansionAirdropLocation( 3924, 2662, 100, "Osternach" );
			break;
		case 9:
			DropLocation = new ExpansionAirdropLocation( 4452, 2196, 100, "Prien" );
			break;
		case 10:
			DropLocation = new ExpansionAirdropLocation( 3059, 1707, 100, "Schlachtfeld" );
			break;
		case 11:
			DropLocation = new ExpansionAirdropLocation( 4619, 7618, 100, "Stein" );
			break;
		case 12:
			DropLocation = new ExpansionAirdropLocation( 6222, 9723, 100, "Weitmoos" );
			break;
		}

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionAirdropChiemsee::Defaults - End");
		#endif
	}

	string ExpansionMissionAirdropRandom(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionAirdropRandom - Start");
		#endif

		if ( m_AvailableLocations.Count() == 0 )
		{
			//! Fall back to Chernarus defaults
			return ExpansionMissionAirdropChernarus( idx );
		}

		Weight = 1;  //! As locations are chosen randomly, set all weights equal

		int locIdx;

		if ( m_SelectedLocations.Count() == 0 )
		{
			//! Pick a random first location
			locIdx = m_AvailableLocations.GetRandomIndex();
		} else
		{
			//! Search for a location that is at least some distance from already picked locations
			array< int > candidates = new array< int >;

			int minDistance = 1500;

			while ( m_SelectedLocations.Count() + candidates.Count() < MaxDefaultMissions() )
			{
				for ( int i = 0; i < m_AvailableLocations.Count(); i++ )
				{
					for ( int j = 0; j < m_SelectedLocations.Count(); j ++ )
					{
						float distance = vector.Distance( m_AvailableLocations[i].position, m_SelectedLocations[j].position );

						if ( distance > minDistance )
						{
							candidates.Insert( i );
						}
					}
				}

				if ( minDistance == 0 )
					break;

				//! In case we didn't find enough candidates, reduce min distance and try again
				minDistance -= 250;
			}

			locIdx = candidates.GetRandomElement();
		}

		ExpansionLocatorArray loc = m_AvailableLocations[locIdx];

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXPrint("ExpansionMissionEventAirdrop::ExpansionMissionAirdropRandom - " + loc.name);
		#endif

		m_SelectedLocations.Insert( loc );

		m_AvailableLocations.Remove( locIdx );

		int radius = ExpansionLocatorModule.GetRadius( loc.type );

		float offset_x = Math.RandomFloatInclusive( -radius, radius );
		float offset_y = Math.RandomFloatInclusive( -radius, radius );

		if ( offset_x < 0 && offset_x > -100 )
			offset_x = -100;

		if ( offset_y < 0 && offset_y > -100 )
			offset_y = -100;

		if ( offset_x >= 0 && offset_x < 100 )
			offset_x = 100;

		if ( offset_y >= 0 && offset_y < 100 )
			offset_y = 100;
		
		float x = loc.position[0] + offset_x;
		float y = loc.position[2] + offset_y;

		float size = GetDayZGame().GetWorldSize();

		x = Math.Min( Math.Max( x, 500 ), size - 500 );
		y = Math.Min( Math.Max( y, 500 ), size - 500 );

		DropLocation = new ExpansionAirdropLocation( x, y, 100, loc.name );

		MissionName = MissionName + "_" + DropLocation.Name;

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
		
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::ExpansionMissionAirdropRandom - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SampleSpawnPosition
	// ------------------------------------------------------------
	private vector SampleSpawnPosition( vector position, float maxRadius, float innerRadius )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SampleSpawnPosition - Start");
		#endif
		
		float a = Math.RandomFloatInclusive( 0.0, 1.0 ) * Math.PI2;
		float r = maxRadius * Math.RandomFloatInclusive( innerRadius / maxRadius, 1 );

		float spawnX = r * Math.Cos( a );
		float spawnZ = r * Math.Sin( a );

		vector nPosition = "0 0 0";

		nPosition[0] = position[0] + spawnX;
		nPosition[2] = position[2] + spawnZ;
		nPosition[1] = GetGame().SurfaceY( nPosition[0], nPosition[2] );

		AIWorld aiWorld = GetGame().GetWorld().GetAIWorld();

		PGFilter filter = new PGFilter();
		filter.SetFlags( PGPolyFlags.NONE, PGPolyFlags.NONE, PGPolyFlags.NONE );
		filter.SetCost( PGAreaType.TERRAIN, 10 );

		aiWorld.SampleNavmeshPosition( nPosition, maxRadius, filter, nPosition );

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		EXLogPrint("ExpansionMissionEventAirdrop::SampleSpawnPosition - End and return nPosition: " + nPosition.ToString() );
		#endif
		
		return nPosition;
	}
};