class ExpansionMissionEventHorde extends ExpansionMissionEventBase
{
	int MinInfectedAmount;
	int MaxInfectedAmount;

	vector Position;

	float MinimumSpawnRadius;
	float MaximumSpawnRadius;
	
	autoptr array< ref BehaviourGroupInfectedPackWaypointParams > WayPoints;

	[NonSerialized()]
	autoptr array< Object > m_Infected;

	[NonSerialized()]
	AIGroup m_AIGroup;

	// ------------------------------------------------------------
	// Expansion ExpansionMissionEventHorde
	// ------------------------------------------------------------
	void ExpansionMissionEventHorde()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::ExpansionMissionEventHorde");
		#endif

		m_EventName = "Hordes";

		WayPoints = new array< ref BehaviourGroupInfectedPackWaypointParams >;

		m_Infected = new array< Object >;
	}
	
	// handle mission start
	override void Event_OnStart()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::Event_OnStart");
		#endif

		m_AIGroup = GetGame().GetWorld().GetAIWorld().CreateGroup( "ExpansionInfectedPatrolGroupBeh" );

		BehaviourGroupInfectedPack bgip;
		Class.CastTo( bgip, m_AIGroup.GetBehaviour() );
		if ( bgip )
		{
			bgip.SetWaypoints( WayPoints, 0, true, false );
		}

		int TargetInfectedAmount = Math.RandomInt(MinInfectedAmount, MaxInfectedAmount);

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::Event_OnStart - MinInfectedAmount is "+MinInfectedAmount);
		EXLogPrint("ExpansionMissionEventHorde::Event_OnStart - TargetInfectedAmount is "+TargetInfectedAmount);
		EXLogPrint("ExpansionMissionEventHorde::Event_OnStart - MaxInfectedAmount is "+MaxInfectedAmount);
		#endif

		while ( m_Infected.Count() < TargetInfectedAmount )
		{
			int index = m_Infected.Count() + 1;

			SpawnInfectedRemaining( Position, MinimumSpawnRadius, MaximumSpawnRadius, TargetInfectedAmount );

			m_Infected.Remove( index );
		}
	
		CreateNotification( new StringLocaliser( "STR_EXPANSION_MISSION_HORDE_SPAWNED", MissionName ), "set:expansion_notification_iconset image:icon_bandit", 7 );
	}

	// handle mission cleanup
	override void Event_OnEnd()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::Event_OnEnd");
		#endif

		while ( m_Infected.Count() > 0 )
		{
			int index = m_Infected.Count() - 1;

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().ObjectDelete, index * 5, false, m_Infected[ index ] );

			m_Infected.Remove( index );
		}
	}

	// update tick for the mission
	override void Event_OnUpdate( float delta )
	{
	}

	protected override void OnLoadMission()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::OnLoadMission");
		#endif

		JsonFileLoader<ExpansionMissionEventHorde>.JsonLoadFile( m_FileName, this );
	}

	protected override void OnSaveMission()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::OnSaveMission");
		#endif
		
		JsonFileLoader<ExpansionMissionEventHorde>.JsonSaveFile( m_FileName, this );
	}

	override int MaxDefaultMissions()
	{
		return 13;
	}

	protected override string OnDefaultMission( int index )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventHorde::OnDefaultMission");
		#endif

		string name = "";

		switch ( index )
		{
		default:
		case 0:
			Weight = 5;
			Position = Vector( 4807, 9812, 100 );
			MissionName = "NWAF";
			break;
		case 1:
			Weight = 10;
			Position = Vector( 12159, 12583, 100 );
			MissionName = "NEAF";
			break;
		case 2:
			Weight = 15;
			Position = Vector( 11464, 0, 8908 );
			MissionName = "Berezino";
			break;
		case 3:
			Weight = 20;
			Position = Vector( 5043, 0, 2505 );
			MissionName = "Balota";
			break;
		case 4:
			Weight = 25;
			Position = Vector( 2351, 0, 5393 );
			MissionName = "Zelenogorsk";
			break;
		case 5:
			Weight = 30;
			Position = Vector( 2036, 0, 7491 );
			MissionName = "Myshkinko";
			break;
		case 6:
			Weight = 35;
			Position = Vector( 11125, 0, 14040 );
			MissionName = "Novodmitrovsk";
			break;
		case 7:
			Weight = 40;
			Position = Vector( 6128, 0, 2497 );
			MissionName = "Chernogorsk";
			break;
		case 8:
			Weight = 45;
			Position = Vector( 9371, 0, 2229 );
			MissionName = "Elektrozavodsk";
			break;
		case 9:
			Weight = 50;
			Position = Vector( 13452, 0, 3112 );
			MissionName = "Skalisty Island";
			break;
		case 10:
			Weight = 55;
			Position = Vector( 2700, 0, 6193 );
			MissionName = "Sosnovka";
			break;
		case 11:
			Weight = 60;
			Position = Vector( 7436, 0, 7720 );
			MissionName = "Novy Sobor";
			break;
		case 12:
			Weight = 65;
			Position = Vector( 5823, 0, 7764 );
			MissionName = "Stary Sobor";
			break;
		}

		Enabled = true;
		MissionMaxTime = 120; // 20 minutes

		MinInfectedAmount = 10.0;
		MaxInfectedAmount = 20.0;
		MinimumSpawnRadius = 10.0;
		MaximumSpawnRadius = 20.0;

		Position[1] = GetGame().SurfaceY( Position[0], Position[2] );

		WayPoints.Insert( new BehaviourGroupInfectedPackWaypointParams( Position, 40.0 ) );

		string fname = MissionName;
		fname.Replace( " ", "-" );
		return fname;
	}

	protected void SpawnInfectedRemaining( vector centerPosition, float innerRadius, float spawnRadius, int remaining )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventAirdrop::SpawnInfectedRemaining");
		#endif

		if ( remaining <= 0 )
			return;

		vector spawnPosition = SampleSpawnPosition( centerPosition, spawnRadius, innerRadius );

		Object obj = GetGame().CreateObject( ExpansionWorkingZombieClasses().GetRandomElement(), spawnPosition, false, false, true );

		DayZCreatureAI creature;
		Class.CastTo( creature, obj );
		if ( creature )
		{
			creature.InitAIAgent( m_AIGroup );

			m_Infected.Insert( obj );
		}

		//SpawnInfectedRemaining( centerPosition, innerRadius, spawnRadius, remaining - 1 );
		// GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( this.SpawnInfectedRemaining, centerPosition, innerRadius, spawnRadius, remaining - 1 );
	}

	protected vector SampleSpawnPosition( vector position, float maxRadius, float innerRadius )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMissionEventAirdrop::SampleSpawnPosition");
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

		return nPosition;
	}
}