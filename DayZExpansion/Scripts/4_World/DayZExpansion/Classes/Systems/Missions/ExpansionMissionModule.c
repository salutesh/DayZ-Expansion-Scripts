class ExpansionMissionModule: JMModuleBase
{
	static ref ScriptInvoker SI_Started = new ScriptInvoker();
	static ref ScriptInvoker SI_Ended = new ScriptInvoker();

	private autoptr array< ref ExpansionMissionEventBase > m_Missions;
	private autoptr map< typename, ref array< ExpansionMissionEventBase > > m_MissionsTyped;

	private typename m_MissionConstructor;
	private autoptr map< string, typename > m_MissionTypes;
	private autoptr array< typename > m_MissionTypesArray;
	
	private bool m_lowPlayerCheckRunning;

	//! Local reference to the actual settings, this is handled by the GC 
	private ExpansionMissionSettings m_MissionSettings;

	private autoptr array< ExpansionMissionEventBase > m_RunningMissions;

	// ------------------------------------------------------------
	// ExpansionMissionModule Constructor
	// ------------------------------------------------------------
	void ExpansionMissionModule()
	{
		ExpansionSettings.SI_Mission.Insert( OnSettingsUpdated );

		m_MissionConstructor = ExpansionMissionConstructor;
	}

	void ~ExpansionMissionModule()
	{
		ExpansionSettings.SI_Mission.Remove( OnSettingsUpdated );

		if(m_lowPlayerCheckRunning)
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove(StartNewMissions);
			m_lowPlayerCheckRunning = false;
		}
		
		SI_OnMissionEnd.Remove( RemoveMission );
	}

	void SetMissionConstructor( typename missionConstructor )
	{
		if ( missionConstructor.IsInherited( ExpansionMissionConstructor ) )
		{
			m_MissionConstructor = missionConstructor;
		} else
		{
			Error( "Parameter '" + missionConstructor + "' is not of type 'ExpansionMissionConstructor'" );
		}
	}

	override void OnInit()
	{
		super.OnInit();

		SI_OnMissionEnd.Insert( RemoveMission );

		m_Missions = new array< ref ExpansionMissionEventBase >;

		m_MissionTypesArray = new array< typename >;

		m_MissionsTyped = new map< typename, ref array< ExpansionMissionEventBase > >;

		m_MissionSettings = GetExpansionSettings().GetMission();

		m_RunningMissions = new array< ExpansionMissionEventBase >;

		ExpansionMissionSettings.SI_OnSave.Insert( SaveMissions );
	}

	override void OnMissionLoaded()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::OnMissionLoaded - Start" );
		#endif

		super.OnMissionLoaded();

		ExpansionMissionConstructor missionConstructor;
		if ( Class.CastTo( missionConstructor, m_MissionConstructor.Spawn() ) )
		{
			missionConstructor.GenerateMissionTypes( m_MissionTypes );
		} else
		{
			m_MissionTypes = new map< string, typename >;
		}

		for ( int i = 0; i < m_MissionTypes.Count(); i++ )
		{
			m_MissionTypesArray.Insert( m_MissionTypes.GetElement( i ) );
		}

		if ( m_MissionSettings.DidGenerateDefaults() )
		{
			DefaultMissions();
		} else
		{
			LoadMissions();
		}

		ProcessMissions();

		StartNewMissions();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::OnMissionLoaded - End" );
		#endif
	}

	override bool IsServer()
	{
		return true;
	}

	override bool IsClient()
	{
		return false;
	}

	protected void SaveMissions()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::SaveMissions - Start" );
		#endif

		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			m_Missions[i].SaveMission();
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::SaveMissions - End" );
		#endif
	}

	protected void ProcessMissions()
	{
		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			ProcessMission( m_Missions[ i ] );
		}
	}

	protected void ProcessMission( ExpansionMissionEventBase evt )
	{
		ref array< ExpansionMissionEventBase > missions;
		if ( m_MissionsTyped.Find( evt.Type(), missions ) )
		{
			missions.Insert( evt );
		} else
		{
			missions = new ref array< ExpansionMissionEventBase >;
			missions.Insert( evt );

			m_MissionsTyped.Insert( evt.Type(), missions );
		}
	}
	
	typename GetMissionType( ExpansionMissionEventBase evt )
	{
		for ( int i = 0; i < m_MissionTypesArray.Count(); i++ )
		{
			if ( evt.IsInherited( m_MissionTypesArray[ i ] ) )
			{
				return m_MissionTypesArray[ i ];
			}
		}

		return typename;
	}

	protected void DefaultMissions()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::DefaultMissions" );
		#endif

		for ( int i = 0; i < m_MissionTypes.Count(); i++ )
		{
			typename missionType = m_MissionTypes.GetElement( i );

			ExpansionMissionEventBase missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );

			if ( !missionEvent )
				continue;

			int maxDefaults = missionEvent.MaxDefaultMissions();
			if ( maxDefaults <= 0 )
				continue;

			for ( int j = 0; j < maxDefaults; j++ )
			{
				missionEvent.LoadDefault( j );
				m_Missions.Insert( missionEvent );

				ExpansionMissionMeta missionMeta = new ExpansionMissionMeta;

				missionMeta.MissionPath = missionEvent.GetPath();
				missionMeta.MissionType = m_MissionTypes.GetKey( i );
				missionMeta.MissionEvent = missionEvent;

				m_MissionSettings.Missions.Insert( missionMeta );

				if ( j != maxDefaults - 1 )
				{
					missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );
				}
			}
		}

		m_MissionSettings.Save();
	}

	protected void LoadMissions()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::LoadMissions - Start" );
		#endif

		ExpansionMissionMeta mission;

		for ( int i = 0; i < m_MissionSettings.Missions.Count(); i++ )
		{
			mission = m_MissionSettings.Missions.Get( i );
			if ( !mission )
				continue;

			typename missionType = m_MissionTypes.Get( mission.MissionType );
			ExpansionMissionEventBase missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );

			if ( !missionEvent )
				continue;

			missionEvent.LoadMission( mission.MissionPath );

			mission.MissionEvent = missionEvent;

			m_Missions.Insert( missionEvent );
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::LoadMissions - End" );
		#endif
	}

	void StartNewMissions()
	{
		if ( !m_MissionSettings || !m_MissionSettings.Enabled )
			return;

		#ifdef EXPANSION_MISSION_PLAYERCOUNT_CHECK
		//Check if Min Players is disabled
		if ( m_MissionSettings.MinPlayersToStartMissions != 0 )
		{
			int playerCount = PlayerBase.GetAll().Count();

			#ifdef EXPANSIONEXLOGPRINT
			Print( "ExpansionMissionModule::StartNewMissions - ["+ playerCount + "/" + m_MissionSettings.MinPlayersToStartMissions +"] players" );
			#endif

			//If player count is too low
			if ( playerCount < m_MissionSettings.MinPlayersToStartMissions )
			{				
				if( !m_lowPlayerCheckRunning )
				{
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissions, 10000, true );
					m_lowPlayerCheckRunning = true;
				}

				return;
			}

			if( m_lowPlayerCheckRunning )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove(StartNewMissions);
				m_lowPlayerCheckRunning = false;
			}

			
		}
		#endif
		
		//If we reach this we should have the correct setting/player count on the server/ find the missions like normal
		while ( m_RunningMissions.Count() < m_MissionSettings.MaxMissions )
			FindNewMission();
	}

	int GetNumberRunningMissions()
	{
		return m_RunningMissions.Count();
	}

	void RemoveMission( ExpansionMissionEventBase mission )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionMissionModule::RemoveMission" );
		#endif

		m_RunningMissions.RemoveItem( mission );

		SI_Ended.Invoke( mission );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissions, m_MissionSettings.TimeBetweenMissions, false );
	}

	protected void FindNewMission()
	{
		//! Print( "ExpansionMissionModule::FindNewMission" );

		array< float > weights = new array< float >;

		float weightSum = 0;
		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			float weight = m_Missions[i].Weight;
			weightSum += weight;
			weights.Insert( weight );
		}
		
		int index = 0;
		int lastIndex = weights.Count();
		ExpansionMissionEventBase mission;
		while ( index < lastIndex )
		{
			if ( m_Missions[ index ].Enabled && !m_Missions[ index ].IsRunning() && Math.RandomFloat( 0, weightSum ) < weights[ index ] )
			{
				mission = m_Missions[ index ];
				break;
			}
	
			weightSum -= weights[index++];
		}

		if ( mission )
		{
			StartMissionInternal( mission );
		}
	}

	void AddMission( ExpansionMissionEventBase evt )
	{
		m_Missions.Insert( evt );

		ExpansionMissionMeta missionMeta = new ExpansionMissionMeta;

		missionMeta.MissionPath = evt.GetPath();
		missionMeta.MissionType = GetMissionType( evt ).ToString();
		missionMeta.MissionEvent = evt;

		m_MissionSettings.Missions.Insert( missionMeta );

		ProcessMission( evt );
	}

	bool DeleteMission( ExpansionMissionEventBase evt )
	{
		int idx = m_Missions.Find( evt );
		if ( idx < 0 )
			return false;

		m_Missions.Remove( idx );

		idx = m_MissionSettings.Missions.Find( evt.m_MissionMeta );
		if ( idx >= 0 )
			m_MissionSettings.Missions.Remove( idx );

		return true;
	}

	private void StartMissionInternal( ExpansionMissionEventBase mission )
	{
		mission.Start();

		m_RunningMissions.Insert( mission );

		SI_Started.Invoke( mission );
	}

	bool StartMission( ExpansionMissionEventBase mission )
	{
		if ( m_RunningMissions.Count() >= m_MissionSettings.MaxMissions )
			return false;

		if ( m_Missions.Find( mission ) < 0 )
			return false;

		StartMissionInternal( mission );

		return true;
	}

	ExpansionMissionEventBase FindMission( string missionType, string missionName )
	{
		array< ExpansionMissionEventBase > missions = new array< ExpansionMissionEventBase >;
		if ( m_MissionsTyped.Find( m_MissionTypes.Get( missionType ), missions ) )
		{
			for ( int i = 0; i < missions.Count(); i++ )
			{
				if ( missions[i].MissionName == missionName )
				{
					if ( missions[i].Enabled )
					{
						return missions[i];
					}

					return NULL;
				}
			}
		}

		return NULL;
	}

	bool StartMission( string missionType, string missionName )
	{
		ExpansionMissionEventBase evt = FindMission( missionType, missionName );
		if ( !evt )
			return false;

		return StartMission( evt );
	}

	bool EndMission( string missionType, string missionName )
	{
		ExpansionMissionEventBase evt = FindMission( missionType, missionName );
		if ( !evt )
			return false;

		evt.End();

		return true;
	}

	ref array< ref ExpansionMissionSerializedType > Serialize()
	{
		ref array< ref ExpansionMissionSerializedType > serialized = new ref array< ref ExpansionMissionSerializedType >;

		for ( int i = 0; i < m_MissionsTyped.Count(); i++ )
		{
			serialized.Insert( new ExpansionMissionSerializedType( m_MissionsTyped.GetKey( i ).ToString(), m_MissionsTyped.GetElement( i ) ) );
		}

		return serialized;
	}
}