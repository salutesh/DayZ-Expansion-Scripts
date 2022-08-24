/**
 * ExpansionMissionModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionMissionModule)]
class ExpansionMissionModule: CF_ModuleWorld
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

	protected bool m_InitialMission;

	// ------------------------------------------------------------
	// ExpansionMissionModule Constructor
	// ------------------------------------------------------------
	void ExpansionMissionModule()
	{
		ExpansionSettings.SI_Mission.Insert( OnSettingsUpdated );

		m_MissionConstructor = ExpansionMissionConstructor;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule Destructor
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule SetMissionConstructor
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();

		EnableMissionLoaded();

		SI_OnMissionEnd.Insert( RemoveMission );

		m_Missions = new array< ref ExpansionMissionEventBase >;

		m_MissionTypesArray = new array< typename >;

		m_MissionsTyped = new map< typename, ref array< ExpansionMissionEventBase > >;

		m_RunningMissions = new array< ExpansionMissionEventBase >;

		ExpansionMissionSettings.SI_OnSave.Insert( SaveMissions );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "OnMissionLoaded");
#endif
	
		super.OnMissionLoaded(sender, args);

		ExpansionMissionConstructor missionConstructor;
		if ( Class.CastTo( missionConstructor, m_MissionConstructor.Spawn() ) )
		{
			missionConstructor.GenerateMissionTypes( m_MissionTypes );
		} else
		{
			m_MissionTypes = new map< string, typename >;
		}

		foreach (string missionClassName, typename missionType: m_MissionTypes)
		{
			m_MissionTypesArray.Insert(missionType);
		}

		m_MissionSettings = GetExpansionSettings().GetMission();

		if ( m_MissionSettings.DidGenerateDefaults() )
		{
			DefaultMissions();
		} else
		{
			LoadMissions();
		}

		ProcessMissions();

		StartNewMissions();
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule IsServer
	// ------------------------------------------------------------
	override bool IsServer()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule IsClient
	// ------------------------------------------------------------
	override bool IsClient()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule SaveMissions
	// ------------------------------------------------------------
	protected void SaveMissions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "SaveMissions");
#endif
	
		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			m_Missions[i].SaveMission();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule ProcessMissions
	// ------------------------------------------------------------
	protected void ProcessMissions()
	{
		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			ProcessMission( m_Missions[ i ] );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule ProcessMission
	// ------------------------------------------------------------
	protected void ProcessMission( ExpansionMissionEventBase evt )
	{
		array< ExpansionMissionEventBase > missions;
		if ( m_MissionsTyped.Find( evt.Type(), missions ) )
		{
			missions.Insert( evt );
		} else
		{
			missions = new array< ExpansionMissionEventBase >;
			missions.Insert( evt );

			m_MissionsTyped.Insert( evt.Type(), missions );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule GetMissionType
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule DefaultMissions
	// ------------------------------------------------------------
	protected void DefaultMissions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "DefaultMissions");
#endif
	
		foreach (string missionClassName, typename missionType: m_MissionTypes)
		{
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
				missionMeta.MissionType = missionClassName;
				missionMeta.MissionEvent = missionEvent;

				m_MissionSettings.Missions.Insert( missionMeta );

				if ( j != maxDefaults - 1 )
				{
					missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule LoadMissions
	// ------------------------------------------------------------
	protected void LoadMissions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "LoadMissions");
#endif
	
		TStringArray fileNames = ExpansionStatic.FindFilesInLocation(EXPANSION_MISSIONS_FOLDER, ".json");

		foreach ( string fileName: fileNames )
		{
			TStringArray tokens = new TStringArray;
			string missionClassName;
			fileName.Split("_", tokens);
			missionClassName = "ExpansionMissionEvent" + tokens[0];  //! E.g. ExpansionMissionEventAirdrop

			typename missionType = m_MissionTypes.Get( missionClassName );
			ExpansionMissionEventBase missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );

			if ( !missionEvent )
			{
				EXTrace.Print(EXTrace.MISSIONS, this, "Ignoring invalid mission " + missionClassName);
				continue;
			}

			EXTrace.Print(EXTrace.MISSIONS, this, "Loading mission " + fileName);

			missionEvent.LoadMission( EXPANSION_MISSIONS_FOLDER + fileName );

			if ( !missionEvent.Enabled )
			{
				EXTrace.Print(EXTrace.MISSIONS, this, "Ignoring disabled mission " + fileName);
				continue;
			}

			ExpansionMissionMeta missionMeta = new ExpansionMissionMeta;

			missionMeta.MissionPath = missionEvent.GetPath();
			missionMeta.MissionType = missionClassName;
			missionMeta.MissionEvent = missionEvent;

			m_MissionSettings.Missions.Insert( missionMeta );

			m_Missions.Insert( missionEvent );
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule StartNewMissions
	// ------------------------------------------------------------
	void StartNewMissions()
	{
		if ( !m_MissionSettings || !m_MissionSettings.Enabled || m_Missions.Count() == 0 )
			return;

		#ifdef EXPANSION_MISSION_PLAYERCOUNT_CHECK
		//! Check if Min Players is disabled
		if ( m_MissionSettings.MinPlayersToStartMissions != 0 )
		{
			array< Man > players = new array< Man >;
			GetGame().GetWorld().GetPlayerList( players );

			int playerCount = players.Count();

			CF_Log.Debug( "ExpansionMissionModule::StartNewMissions - ["+ playerCount + "/" + m_MissionSettings.MinPlayersToStartMissions +"] players" );

			//! If player count is too low
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
		
		//! If we reach this we should have the correct setting/player count on the server/ find the missions like normal

		float delay;
		if ( !m_InitialMission )
		{
			//! This is the first mission in this session
			m_InitialMission = true;
			delay = m_MissionSettings.InitialMissionStartDelay;
		}
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissionsInternal, delay, false );
	}

	void StartNewMissionsInternal()
	{
		if ( m_Missions.Count() == 0 )
			return;

		while ( m_RunningMissions.Count() < m_MissionSettings.MaxMissions )
		{
			if ( !FindNewMission() )
				break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule GetNumberRunningMissions
	// ------------------------------------------------------------
	int GetNumberRunningMissions()
	{
		return m_RunningMissions.Count();
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule RemoveMission
	// ------------------------------------------------------------
	void RemoveMission( ExpansionMissionEventBase mission )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MISSIONS, this, "RemoveMission");
#endif
	
		m_RunningMissions.RemoveItem( mission );

		SI_Ended.Invoke( mission );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissions, m_MissionSettings.TimeBetweenMissions, false );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule FindNewMission
	// ------------------------------------------------------------
	protected bool FindNewMission()
	{
		//! Print( "ExpansionMissionModule::FindNewMission" );

		array< float > weights = new array< float >;

		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			weights.Insert( m_Missions[i].Weight );
		}
		
		int index = ExpansionStatic.GetWeightedRandom( weights );

		EXTrace.Print(EXTrace.MISSIONS, this, "Selected mission index " + index);

		if ( index > -1 )
		{
			EXTrace.Print(EXTrace.MISSIONS, this, "Selected mission " + m_Missions[ index ].MissionName + " - weight: " + weights[index] );
			
			StartMissionInternal( m_Missions[ index ] );
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule AddMission
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule DeleteMission
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule StartMissionInternal
	// ------------------------------------------------------------
	private void StartMissionInternal( ExpansionMissionEventBase mission )
	{
		mission.Start();

		m_RunningMissions.Insert( mission );

		SI_Started.Invoke( mission );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule StartMission
	// ------------------------------------------------------------
	bool StartMission( ExpansionMissionEventBase mission )
	{
		if ( m_RunningMissions.Count() >= m_MissionSettings.MaxMissions )
			return false;

		if ( m_Missions.Find( mission ) < 0 )
			return false;

		StartMissionInternal( mission );

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule FindMission
	// ------------------------------------------------------------
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
	
	// ------------------------------------------------------------
	// ExpansionMissionModule StartMission
	// ------------------------------------------------------------
	bool StartMission( string missionType, string missionName )
	{
		ExpansionMissionEventBase evt = FindMission( missionType, missionName );
		if ( !evt )
			return false;

		return StartMission( evt );
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule EndMission
	// ------------------------------------------------------------
	bool EndMission( string missionType, string missionName )
	{
		ExpansionMissionEventBase evt = FindMission( missionType, missionName );
		if ( !evt )
			return false;

		evt.End();

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMissionModule Serialize
	// ------------------------------------------------------------
	array< ref ExpansionMissionSerializedType > Serialize()
	{
		array< ref ExpansionMissionSerializedType > serialized = new array< ref ExpansionMissionSerializedType >;

		foreach (typename missionType, array< ExpansionMissionEventBase > missions: m_MissionsTyped)
		{
			serialized.Insert( new ExpansionMissionSerializedType( missionType.ToString(), missions ) );
		}

		return serialized;
	}

	void OnSettingsUpdated()
	{
	}
};
