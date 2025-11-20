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
	static ref ExpansionMissionModule s_Instance;

	static ref ScriptInvoker SI_Started = new ScriptInvoker();
	static ref ScriptInvoker SI_Ended = new ScriptInvoker();

	static int s_Expansion_SpawnParticle_RPCID;

	private autoptr array< ref ExpansionMissionEventBase > m_Missions;
	private autoptr ref array< float > m_AvailableMissions;
	private autoptr map< typename, ref array< ExpansionMissionEventBase > > m_MissionsTyped;

	private autoptr map< string, typename > m_MissionTypes;
	
	private bool m_lowPlayerCheckRunning;

	//! Local reference to the actual settings, this is handled by the GC 
	private ExpansionMissionSettings m_MissionSettings;

	private autoptr array< ExpansionMissionEventBase > m_RunningMissions;

	protected bool m_InitialMission;	// Are the first missions after a server restart started yet ?

	void ExpansionMissionModule()
	{
		s_Instance = this;

		ExpansionSettings.SI_Mission.Insert( OnSettingsUpdated );
	}

	void ~ExpansionMissionModule()
	{
		ExpansionMissionSettings.SI_OnSave.Remove( SaveMissions );
	}
	
	override void OnMissionFinish(Class sender, CF_EventArgs args)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MISSIONS, this);
#endif
		
		super.OnMissionFinish(sender, args);
		
		ExpansionSettings.SI_Mission.Remove( OnSettingsUpdated );

		if(m_lowPlayerCheckRunning)
		{
			g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove(StartNewMissions);
			m_lowPlayerCheckRunning = false;
		}

		ExpansionMissionEventBase.SI_OnMissionEnd.Remove( RemoveMission );
	}

	override void OnInit()
	{
		super.OnInit();

		EnableMissionLoaded();
		EnableMissionFinish();
		EnableInvokeConnect();
		Expansion_EnableRPCManager();
		Expansion_RegisterClientRPC("RPC_CreateAirdropPlane");
		Expansion_RegisterClientRPC("RPC_CreateAirdropContainer");
		Expansion_RegisterClientRPC("RPC_SynchContainerStateToClient");
		s_Expansion_SpawnParticle_RPCID = m_Expansion_RPCManager.RegisterClient("RPC_SpawnParticle");
		Expansion_RegisterClientRPC("RPC_DeleteAirdropContainer");

		ExpansionMissionEventBase.SI_OnMissionEnd.Insert( RemoveMission );

		m_Missions = new array< ref ExpansionMissionEventBase >;

		m_AvailableMissions = new array< float >;

		m_MissionsTyped = new map< typename, ref array< ExpansionMissionEventBase > >;

		m_RunningMissions = new array< ExpansionMissionEventBase >;

		ExpansionMissionSettings.SI_OnSave.Insert( SaveMissions );
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionAirdropPlaneBase.Expansion_SendCreatePlanesOnClient, 1000, false, cArgs.Player);

		if (ExpansionAirdropContainerBase.s_Expansion_CreateContainerOnClient)
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionAirdropContainerBase.Expansion_SendCreateContainersOnClient, 1000, false, cArgs.Player);
	}

	//! Client
	void RPC_CreateAirdropPlane(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int planeID;
		if (!ctx.Read(planeID))
			return;

		if (ExpansionAirdropPlaneBase.s_Expansion_AirdropPlanes[planeID])
			return;

		string planeClassName;
		if (!ctx.Read(planeClassName))
			return;

		vector position;
		if (!ctx.Read(position))
			return;

		vector spawnPoint;
		if (!ctx.Read(spawnPoint))
			return;

		vector dropPosition;
		if (!ctx.Read(dropPosition))
			return;

		bool heightIsRelativeToGround;
		if (!ctx.Read(heightIsRelativeToGround))
			return;

		float height;
		if (!ctx.Read(height))
			return;

		float dropHeight;
		if (!ctx.Read(dropHeight))
			return;

		float followTerrainFrac;
		if (!ctx.Read(followTerrainFrac))
			return;

		float speed;
		if (!ctx.Read(speed))
			return;

		float dropSpeed;
		if (!ctx.Read(dropSpeed))
			return;

		float dropProximityDist;
		if (!ctx.Read(dropProximityDist))
			return;

		auto plane = ExpansionAirdropPlaneBase.Cast(g_Game.CreateObjectEx(planeClassName, position, ECE_AIRBORNE | ECE_LOCAL) );

		plane.Expansion_SetAirdropPlaneID(planeID);
		plane.Expansion_SetupPlane(spawnPoint, dropPosition, "", 0.0, heightIsRelativeToGround, height, dropHeight, followTerrainFrac, speed, dropSpeed, dropProximityDist, null);
		plane.Expansion_EnableUpdate();
		plane.Expansion_PlayEngineSoundLoop();
	}

	//! Client
	void RPC_CreateAirdropContainer(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int containerID;
		if (!ctx.Read(containerID))
			return;

		if (ExpansionAirdropContainerBase.s_Expansion_AirdropContainers[containerID])
			return;

		string className;
		if (!ctx.Read(className))
			return;

		vector spawnPoint;
		if (!ctx.Read(spawnPoint))
			return;

		vector orientation;
		if (!ctx.Read(orientation))
			return;

		float fallSpeed;
		if (!ctx.Read(fallSpeed))
			return;

		float windImpactStrength;
		if (!ctx.Read(windImpactStrength))
			return;

		bool fxEnabled;
		if (!ctx.Read(fxEnabled))
			return;

		bool hasDiscardedParachute;
		if (!ctx.Read(hasDiscardedParachute))
			return;

		auto container = ExpansionAirdropContainerBase.Cast(g_Game.CreateObjectEx(className, spawnPoint, ECE_AIRBORNE | ECE_LOCAL) );
		container.SetOrientation(orientation);

		container.Expansion_SetAirdropContainerID(containerID);
		container.Expansion_InitAirdropClient(fallSpeed, windImpactStrength, fxEnabled, hasDiscardedParachute);
		container.Expansion_EnableUpdate();
	}

	//! Client
	void RPC_SynchContainerStateToClient(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int containerID;
		if (!ctx.Read(containerID))
			return;

		ExpansionAirdropContainerBase container;
		if (!ExpansionAirdropContainerBase.s_Expansion_AirdropContainers.Find(containerID, container))
			return;

		container.Expansion_ReadState(ctx);
	}

	//! Client
	void RPC_SpawnParticle(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		vector spawnPos;
		if (!ctx.Read( spawnPos ))
			return;
		
		SpawnParticle( spawnPos );
	}

	protected void SpawnParticle( vector spawnPos )
	{
		//! Play spawn sound
		SEffectManager.PlaySound( "Expansion_Airdrop_ZSpawn_SoundSet", spawnPos );

		//! Create dirt particle
		Particle particle = Particle.PlayInWorld( ParticleList.IMPACT_DIRT_RICOCHET, spawnPos );

		particle.ScaleParticleParam( EmitorParam.SIZE, 10 );
		particle.ScaleParticleParam( EmitorParam.BIRTH_RATE, 5 );
		particle.ScaleParticleParam( EmitorParam.BIRTH_RATE_RND, 5 );
		particle.ScaleParticleParam( EmitorParam.LIFETIME, 5 );
		particle.ScaleParticleParam( EmitorParam.LIFETIME_RND, 5 );
	}

	//! Client
	void RPC_DeleteAirdropContainer(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int containerID;
		if (!ctx.Read(containerID))
			return;

		ExpansionAirdropContainerBase container;
		if (!ExpansionAirdropContainerBase.s_Expansion_AirdropContainers.Find(containerID, container))
			return;

		container.Delete();
	}

	void CallAirdrop(vector position)
	{
		array< ref ExpansionLootContainer > containers = new array< ref ExpansionLootContainer >;
		ExpansionLootContainer container;

		auto settings = GetExpansionSettings().GetAirdrop();

		//! Get all containers enabled for player-called supply drop use
		for ( int i = 0; i < settings.Containers.Count(); i++ )
		{
			container = settings.Containers[i];
			if ( container.Usage == 0 || container.Usage == 2 )
			{
				containers.Insert( container );
			}
		}

		if ( containers.Count() == 0 )
		{
			string errorMsg = "Airdrop settings do not contain compatible container (looked for container with Usage set to 0 or 2)";

			CF_Log.Error( errorMsg );

			ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "ERROR"), new StringLocaliser(errorMsg), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();
			return;
		}

		//! Pick a container (weighted random)
		container = ExpansionLootContainer.GetWeightedRandomContainer( containers );

		CF_Log.Debug("[ExpansionSupplySignal] Selected container: " + container.Container);
				
		int itemCount = container.ItemCount;
		if ( container.ItemCount <= 0 )
			itemCount = settings.ItemCount;  //! Only kept for backwards compatibility, should be set per container

		TStringArray infected;
		int infectedCount;
		if ( container.SpawnInfectedForPlayerCalledDrops )
		{
			infected = container.Infected;
			infectedCount = container.InfectedCount;
		}

		container = new ExpansionLootContainer( container.Container, 2, 1, container.Loot, infected, itemCount, infectedCount, false, container.FallSpeed );

		StringLocaliser warningProximityMsg;
		StringLocaliser airdropCreatedMsg;

		if (GetExpansionSettings().GetNotification().ShowPlayerAirdropClosingOn)
			warningProximityMsg = new StringLocaliser("STR_EXPANSION_MISSION_AIRDROP_CLOSING_ON_PLAYER");

		if (GetExpansionSettings().GetNotification().ShowPlayerAirdropDropped)
			airdropCreatedMsg = new StringLocaliser("STR_EXPANSION_MISSION_AIRDROP_SUPPLIES_DROPPED_PLAYER");

		ExpansionAirdropPlaneBase plane = ExpansionAirdropPlane.Expansion_CreatePlane(Vector( position[0], 0, position[2] ), "", settings.Radius, settings.Height, settings.DropZoneHeight, settings.Speed, settings.DropZoneSpeed, container, warningProximityMsg, airdropCreatedMsg);

		if ( plane )
		{
			if ( GetExpansionSettings().GetNotification().ShowPlayerAirdropStarted )
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MISSION_NOTIF_TITLE", "Airdrop"), new StringLocaliser("STR_EXPANSION_MISSION_AIRDROP_HEADING_TOWARDS_PLAYER"), EXPANSION_NOTIFICATION_ICON_AIRDROP, COLOR_EXPANSION_NOTIFICATION_MISSION).Create();			

			if ( GetExpansionSettings().GetLog().MissionAirdrop )
				GetExpansionSettings().GetLog().PrintLog( "[Airdrop] A player called airdrop is heading toward " + Vector( position[0], 0, position[2] ) + " with a "+ container.Container );
		}
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MISSIONS, this);
#endif
		
		super.OnMissionLoaded(sender, args);

		GenerateMissionTypes();

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
	
	void GenerateMissionTypes()
	{
		TTypenameArray missionTypenames = new TTypenameArray;
		RegisterMissions( missionTypenames);

		missionTypenames.Sort();
		
		m_MissionTypes = new map< string, typename >;
		
		foreach (typename missionTypename: missionTypenames)
		{
			if (missionTypename.IsInherited( ExpansionMissionEventBase ) )
			{
				m_MissionTypes.Insert( missionTypename.ToString(), missionTypename );
			}
		}
	}

	void RegisterMissions( TTypenameArray missionTypenames )
	{
		//! Airdrops
		missionTypenames.Insert( ExpansionMissionEventAirdrop );
		
		//! DayZ rewritten Contaminated Areas
		missionTypenames.Insert( ExpansionMissionEventContaminatedArea );
		
	#ifdef EXPANSION_MISSION_HORDE
		//! Zombie Horde
		missionTypenames.Insert( ExpansionMissionEventHorde );
	#endif
	
	#ifdef EXPANSION_MISSION_AI_ENABLE
		//! AI General missions (Patrol, Faction War, Protecting Loot)
		missionTypenames.Insert( ExpansionMissionEventAI );
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
		for ( int i = 0; i < m_Missions.Count(); i++ )
		{
			m_Missions[i].SaveMission();
		}
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

	typename GetMissionType( ExpansionMissionEventBase evt )
	{
		foreach (string name, typename missionType: m_MissionTypes)
		{
			if (evt.IsInherited(missionType))
			{
				return missionType;
			}
		}

		return typename;
	}

	protected void DefaultMissions()
	{
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
				m_AvailableMissions.Insert( missionEvent.Weight );

				ExpansionMissionMeta missionMeta = new ExpansionMissionMeta;

				missionMeta.MissionPath = missionEvent.GetPath();
				missionMeta.MissionType = missionClassName;

				m_MissionSettings.Missions.Insert( missionMeta );

				if ( j != maxDefaults - 1 )
				{
					missionEvent = ExpansionMissionEventBase.Cast( missionType.Spawn() );
				}
			}
		}
	}

	protected void LoadMissions()
	{
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

			m_MissionSettings.Missions.Insert( missionMeta );

			m_AvailableMissions.Insert( missionEvent.Weight );
			m_Missions.Insert( missionEvent );
		}
	}

	void StartNewMissions()
	{
#ifdef EXPANSION_MISSION_EVENT_DEBUG
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif
		
		if ( !m_MissionSettings || !m_MissionSettings.Enabled || m_Missions.Count() == 0 )
			return;

	#ifdef EXPANSION_MISSION_PLAYERCOUNT_CHECK
		//! Check if Min Players is disabled
		if ( m_MissionSettings.MinPlayersToStartMissions != 0 )
		{
			array< Man > players = new array< Man >;
			g_Game.GetWorld().GetPlayerList( players );

			int playerCount = players.Count();

		#ifdef EXPANSION_MISSION_EVENT_DEBUG
			CF_Log.Debug( "ExpansionMissionModule::StartNewMissions - ["+ playerCount + "/" + m_MissionSettings.MinPlayersToStartMissions +"] players" );
		#endif

			//! If player count is too low
			if ( playerCount < m_MissionSettings.MinPlayersToStartMissions )
			{
				if( !m_lowPlayerCheckRunning )
				{
					g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissions, 10000, true );
					m_lowPlayerCheckRunning = true;
				}

				return;
			}

			if( m_lowPlayerCheckRunning )
			{
				g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove(StartNewMissions);
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
		g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissionsInternal, delay, false );
	}

	void StartNewMissionsInternal()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif
		
		if ( m_Missions.Count() == 0 )
			return;

		if ( m_RunningMissions.Count() < m_MissionSettings.MaxMissions )
		{
			bool skip;
			if ( m_RunningMissions.Count() > m_MissionSettings.MinMissions )
			{
				float chance = ExpansionMath.LinearConversion(m_MissionSettings.MaxMissions, m_MissionSettings.MinMissions, m_RunningMissions.Count()) + 0.125;

				if ( chance < Math.RandomFloatInclusive(0.0,1.0) )
					skip = true;
			}
			if ( skip || (FindNewMission() && m_RunningMissions.Count() < m_MissionSettings.MaxMissions) )
			{
				g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissionsInternal, 10000, false );
			}
		}
	}

	int GetNumberRunningMissions()
	{
		return m_RunningMissions.Count();
	}

	void RemoveMission( ExpansionMissionEventBase mission )
	{
		m_RunningMissions.RemoveItem( mission );
		
		m_AvailableMissions[m_Missions.Find(mission)] = mission.Weight;

		SI_Ended.Invoke( mission );

		g_Game.GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( StartNewMissions, m_MissionSettings.TimeBetweenMissions, false );
	}

	protected bool FindNewMission()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MISSIONS, this);
#endif
		
		int index = ExpansionStatic.GetWeightedRandom( m_AvailableMissions );

		EXTrace.Print(EXTrace.MISSIONS, this, "Selected mission index " + index);

		if ( index > -1 )
		{
			EXTrace.Print(EXTrace.MISSIONS, this, "Selected mission " + m_Missions[ index ].MissionName + " - weight: " + m_AvailableMissions[index] );
			
			StartMissionInternal( m_Missions[ index ] );
			m_AvailableMissions[index] = 0;
			return true;
		}

		return false;
	}

	void AddMission( ExpansionMissionEventBase evt )
	{
		m_Missions.Insert( evt );

		ExpansionMissionMeta missionMeta = new ExpansionMissionMeta;

		missionMeta.MissionPath = evt.GetPath();
		missionMeta.MissionType = GetMissionType( evt ).ToString();

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
