/**
 * ExpansionMissionEventAirdrop.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMissionEventAirdropBase: ExpansionMissionEventBase
{
	bool ShowNotification;
	
	float Height;
	float Speed;

	string Container;
	float FallSpeed = 4.5;
	
	ref ExpansionAirdropLocation DropLocation;

	ref TStringArray Infected;
	int ItemCount;
	int InfectedCount;
}

class ExpansionMissionEventAirdropV0: ExpansionMissionEventAirdropBase
{
	ref array < ref ExpansionLootV1 > Loot = {};
}

class ExpansionMissionEventAirdrop: ExpansionMissionEventAirdropBase
{
	static const int VERSION = 2;

	string AirdropPlaneClassName;

	ref array < ref ExpansionLoot > Loot = {};

	[NonSerialized()]
	ExpansionAirdropPlaneBase m_Plane;

	[NonSerialized()]
	ExpansionAirdropContainerBase m_Container;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventAirdrop
	// ------------------------------------------------------------
	void ExpansionMissionEventAirdrop()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		m_EventName = "Airdrop";
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnStart
	// ------------------------------------------------------------
	// handle mission start
	override void Event_OnStart()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
			auto settings = GetExpansionSettings().GetAirdrop();

			ExpansionLootContainer container;

			if ( Loot.Count() == 0 || Infected.Count() == 0 )
			{
				//! No mission-specific loot and/or infected defined, fall back to airdrop settings

				array< ref ExpansionLootContainer > containers = new array< ref ExpansionLootContainer >;

				//! Get all containers enabled for mission use that match our container name (or any if random)
				string containerName = Container;
				containerName.ToLower();
				for ( int i = 0; i < settings.Containers.Count(); i++ )
				{
					container = settings.Containers[i];
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
				container = ExpansionLootContainer.GetWeightedRandomContainer( containers, weights );

				Container = container.Container;

				if ( Loot.Count() == 0 )
					Loot = container.Loot;

				if ( Infected.Count() == 0 )
					Infected = container.Infected;

				if ( ItemCount <= 0 )
					ItemCount = container.ItemCount;

				if ( InfectedCount == -1 )
					InfectedCount = container.InfectedCount;

				if (FallSpeed <= 0)
					FallSpeed = container.FallSpeed;
			}

			#ifdef EXPANSION_MISSION_EVENT_DEBUG
			EXPrint("[ExpansionMissionEventAirdrop] " + MissionName + " - selected container: " + Container);
			#endif

			if ( ItemCount <= 0 )
				ItemCount = settings.ItemCount;

			container = new ExpansionLootContainer( Container, 1, 1, Loot, Infected, ItemCount, InfectedCount, false, FallSpeed );

			StringLocaliser warningProximityMsg;
			StringLocaliser airdropCreatedMsg;

			if ( ShowNotification )
			{
				warningProximityMsg = new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON", DropLocation.Name );
				airdropCreatedMsg = new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED", DropLocation.Name );
			}
	
			m_Plane = ExpansionAirdropPlaneBase.CreatePlane( AirdropPlaneClassName, Vector( DropLocation.x, 0, DropLocation.z ), DropLocation.Name, DropLocation.Radius, Height, Speed, container, warningProximityMsg, airdropCreatedMsg, MissionMaxTime );
			
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
	}
	
	override bool CanEnd()
	{
		if ((!m_Plane || m_Plane.AirdropCreated()) && (!m_Container || (m_Container.HasLanded() && !ExpansionLootSpawner.IsPlayerNearby(m_Container, 1100))))
			return true;

		return false;
	}

	// ------------------------------------------------------------
	// Expansion Event_OnEnd
	// ------------------------------------------------------------
	// handle mission cleanup
	override void Event_OnEnd()
	{
		#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif
		
		if ( IsMissionHost() )
		{
			if ( m_Container )
			{
				GetGame().ObjectDelete(m_Container);
			}

			if ( GetExpansionSettings().GetNotification().ShowAirdropEnded && ShowNotification )
			{
				CreateNotification(new StringLocaliser( "STR_EXPANSION_MISSION_AIRDROP_ENDED", DropLocation.Name), EXPANSION_NOTIFICATION_ICON_AIRDROP, 7);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Event_OnUpdate
	// ------------------------------------------------------------
	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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

			if (m_CurrentMissionTime >= MissionMaxTime && m_Container && m_Container.GetHealth("", "") > 0.0)
			{
				m_Container.SetHealth("", "", 0.0);
				m_Container.SetLifetimeMax(0.0);
				m_Container.ToggleLight();
			}
		}
	}

	void Copy(ExpansionMissionEventAirdropBase mission)
	{
		ShowNotification = mission.ShowNotification;
		Height = mission.Height;
		Speed = mission.Speed;
		Container = mission.Container;
		FallSpeed = mission.FallSpeed;
		DropLocation = mission.DropLocation;
		Infected = mission.Infected;
		ItemCount = mission.ItemCount;
		InfectedCount = mission.InfectedCount;

		ExpansionMissionEventBase missionBase = mission;
		Copy(missionBase);
	}
	
	// ------------------------------------------------------------
	// Expansion OnLoadMission
	// ------------------------------------------------------------
	protected override void OnLoadMission()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoadMission");
#endif

		if ( IsMissionHost() )
		{
			ExpansionMissionEventAirdropBase airdropBase;
			if (!ExpansionJsonFileParser<ExpansionMissionEventAirdropBase>.Load(m_FileName, airdropBase))
				return;

			if (airdropBase.m_Version < VERSION)
			{
				if (airdropBase.m_Version < 1)
				{
					Copy(airdropBase);

					ExpansionMissionEventAirdropV0 airdropV0;
					if (!ExpansionJsonFileParser<ExpansionMissionEventAirdropV0>.Load(m_FileName, airdropV0))
						return;

					foreach (ExpansionLootV1 lootV1: airdropV0.Loot)
					{
						Loot.Insert(lootV1.Convert());
					}
				}
				else if (!ExpansionJsonFileParser<ExpansionMissionEventAirdrop>.Load(m_FileName, this))
				{
					return;
				}

				m_Version = VERSION;

				ExpansionJsonFileParser<ExpansionMissionEventAirdrop>.Save(m_FileName, this);
			}
			else
			{
				ExpansionJsonFileParser<ExpansionMissionEventAirdrop>.Load(m_FileName, this);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnSaveMission
	// ------------------------------------------------------------
	protected override void OnSaveMission()
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		if ( IsMissionHost() )
		{
			ExpansionJsonFileParser<ExpansionMissionEventAirdrop>.Save(m_FileName, this);
		}
	}
	
	// ------------------------------------------------------------
	// Expansion MaxDefaultMissions
	// ------------------------------------------------------------
	override int MaxDefaultMissions()
	{
		switch (ExpansionStatic.GetCanonicalWorldName())
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
			*/
		}

		//! Unknown map
		return super.MaxDefaultMissions();
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

		AirdropPlaneClassName = "";

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
		switch (ExpansionStatic.GetCanonicalWorldName())
		{
			case "banov":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "chernarusplus":
				return ExpansionMissionAirdropChernarus( index );
			break;
			case "chiemsee":
				return ExpansionMissionAirdropChiemsee( index );
			break;
			case "deerisle":
				return ExpansionMissionAirdropDeerIsle( index );
			break;
			case "enoch":
				return ExpansionMissionAirdropLivonia( index );
			break;
			case "esseker":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "iztek":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "namalsk":
				return ExpansionMissionAirdropNamalsk( index );
			break;
			case "takistanplus":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "rostow":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
			case "sandbox":
				return ExpansionMissionAirdropSandbox( index );
			break;
			case "valning":
				return ExpansionMissionAirdropRandom( index ); //! TODO
			break;
		}
		
		//! Map unknown, loading default config with random locations from map
		return ExpansionMissionAirdropRandom( index );
	}

	override protected string DefaultMission( int index )
	{
		return ExpansionMissionAirdropChernarus(index);
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropChernarus
	// ------------------------------------------------------------
	string ExpansionMissionAirdropChernarus(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropLivonia
	// ------------------------------------------------------------
	string ExpansionMissionAirdropLivonia(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropDeerIsle
	// ------------------------------------------------------------
	string ExpansionMissionAirdropDeerIsle(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropNamalsk
	// ------------------------------------------------------------
	string ExpansionMissionAirdropNamalsk(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropSandbox
	// ------------------------------------------------------------
	string ExpansionMissionAirdropSandbox(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMissionAirdropChiemsee
	// ------------------------------------------------------------
	string ExpansionMissionAirdropChiemsee(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
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
	}

	string ExpansionMissionAirdropRandom(int idx)
	{
		#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
		#endif

		ExpansionLocation loc;
		string fname = RandomMission(idx, loc);
		if (!loc)
			return fname;

		int radius = loc.Radius;

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
		
		float x = loc.Position[0] + offset_x;
		float y = loc.Position[2] + offset_y;

		float size = GetDayZGame().GetWorldSize();

		x = Math.Min( Math.Max( x, 500 ), size - 500 );
		y = Math.Min( Math.Max( y, 500 ), size - 500 );

		DropLocation = new ExpansionAirdropLocation( x, y, 100, loc.Name );

		return fname;
	}
};
