/**
 * ExpansionVehicleScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_USING_TRANSPORT_BASE
class ExpansionVehicleScript extends Transport
#else
class ExpansionVehicleScript extends ItemBase
#endif
{
	protected ref array< ref ExpansionVehicleAxle > m_Axles;
	protected ref array< ref ExpansionVehicleCrew > m_Crew;

	protected ref array< ExpansionVehicleWheel > m_Wheels;

	protected ref ExpansionVehicleController m_Controller;
	protected bool m_HasDriver;
	
	// ------------------------------------------------------------
	//! Constant Values - Set in Constructor, Errors occur if not.
	// ------------------------------------------------------------
	protected float m_MaxSpeed; // (km/h)
	protected float m_MaxSpeedMS; // (m/s)

	protected float m_AltitudeFullForce; // (m)
	protected float m_AltitudeNoForce; // (m)

	float m_BodyMass;
	float m_InvMass;
	vector m_BodyCenterOfMass;

	private float m_TimeSlice;
	protected bool m_IsPhysicsHost;
	bool m_PhysicsCreated;
	bool m_PhysicsDestroyed;
	
	protected float m_BoundingRadius;
	protected vector m_BoundingBox[2];

	protected EffectSound m_HornSound;

	protected string m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
	protected string m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";

	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;
	protected int m_ExplosionSize;

	protected bool m_HornPlaying;
	protected bool m_HornSynchRemote;
	
	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;

	// Towing
	protected vector m_TowPointCenter;
	protected vector m_TowPointCenterSelf;
	protected bool m_IsBeingTowed;
	protected bool m_IsTowing;

	protected EntityAI m_ParentTow;
	protected int m_ParentTowNetworkIDLow;
	protected int m_ParentTowNetworkIDHigh;
	protected int m_ParentTowPersistentIDA;
	protected int m_ParentTowPersistentIDB;
	protected int m_ParentTowPersistentIDC;
	protected int m_ParentTowPersistentIDD;

	protected EntityAI m_ChildTow;
	protected int m_ChildTowNetworkIDLow;
	protected int m_ChildTowNetworkIDHigh;
	protected int m_ChildTowPersistentIDA;
	protected int m_ChildTowPersistentIDB;
	protected int m_ChildTowPersistentIDC;
	protected int m_ChildTowPersistentIDD;

	protected bool m_EngineIsOn;

	protected ExpansionVehicleLockState m_VehicleLockedState;

	protected int m_PersistentIDA = 0;
	protected int m_PersistentIDB = 0;
	protected int m_PersistentIDC = 0;
	protected int m_PersistentIDD = 0;

	protected float m_dmgContactCoef;

	protected ExpansionSoundProxyBase m_SoundProxy; //! not needed anymore

	protected vector m_LinearVelocity; // World Space
	protected vector m_LinearVelocityMS; // Model Space

	protected vector m_LinearAcceleration; // World Space
	protected vector m_LinearAccelerationMS; // Model Space

	protected vector m_LastLinearVelocity; // World Space
	protected vector m_LastLinearVelocityMS; // Model Space

	protected vector m_AngularVelocity; // World Space
	protected vector m_AngularVelocityMS; // Model Space

	protected vector m_AngularAcceleration; // World Space
	protected vector m_AngularAccelerationMS; // Model Space

	protected vector m_LastAngularVelocity; // World Space
	protected vector m_LastAngularVelocityMS; // Model Space

	ref Transform m_Transform;
	
	// Floating
	protected float m_WaterVolume;
	protected float m_TotalVolume;

	// Effects
	protected float m_AltitudeLimiter;

	vector m_InertiaTensor;
	vector m_InvInertiaTensor;

	private ref ExpansionVehicleSyncState m_SyncState;
	private ExpansionVehicleNetworkMode m_NetworkMode;

	ref array< ref ExpansionPointLight > m_Lights;
	ref array< ref Particle > m_Particles;

	protected bool m_PlayCrashSoundLight;
	protected bool m_PlayCrashSoundHeavy;
	
	protected bool m_HeadlightsOn;
	protected bool m_HeadlightsState;
	protected bool m_BrakesArePressed;
	
	CarLightBase m_Headlight;
	CarRearLightBase m_RearLight;

	protected ExpansionMarkerModule m_MarkerModule;
	protected string m_ServerMarker;
	protected static int m_ServerMarkerIndex = 0;

	protected autoptr TStringArray m_Doors;
	protected bool m_CanHaveLock;

	protected bool m_ClutchState;

	protected ref array< float > m_FluidCapacities;

	protected float m_FluidFuelAmount;
	protected float m_FluidCoolantAmount;
	protected float m_FluidOilAmount;
	protected float m_FluidBrakeAmount;

	// Debugging
	private ref array< Shape > m_DebugShapes;

	// ------------------------------------------------------------
	void ExpansionVehicleScript()
	{
		//! Print( "[" + this + "] ExpansionVehicleScript" );
		
		SetFlags( EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false );
		SetEventMask( EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT | EntityEvent.FRAME | EntityEvent.PHYSICSMOVE );

		m_Axles = new array< ref ExpansionVehicleAxle >;
		m_Crew = new array< ref ExpansionVehicleCrew >;

		m_Wheels = new array< ExpansionVehicleWheel >;

		m_DebugShapes = new array< Shape >();

		string path = "";
		int i = 0;
		int count = 0;

		path = "CfgVehicles " + GetType() + " VehicleSimulation Axles";
		count = GetGame().ConfigGetChildrenCount( path );
		for ( i = 0; i < count; i++ )
		{
			string axleName;
			GetGame().ConfigGetChildName( path, i, axleName );

			string wheelPath = "CfgVehicles " + GetType() + " VehicleSimulation Axles " + axleName + " Wheels";
			int wheelCount = GetGame().ConfigGetChildrenCount( wheelPath );

			ref ExpansionVehicleAxle axle = NULL;
			
			if ( wheelCount == 1 )
			{
				axle = new ExpansionVehicleOneWheelAxle( this, axleName );

				axle.AddWheel( "Center", m_Wheels );
			} else if ( wheelCount == 2 )
			{
				axle = new ExpansionVehicleTwoWheelAxle( this, axleName );

				axle.AddWheel( "Left", m_Wheels );
				axle.AddWheel( "Right", m_Wheels );
			} else
			{
				axle = new ExpansionVehicleAxle( this, axleName );

				for ( int j = 0; j < wheelCount; j++ )
				{
					string wheelName;
					GetGame().ConfigGetChildName( wheelPath, j, wheelName );

					axle.AddWheel( wheelName, m_Wheels );
				}
			}

			m_Axles.Insert( axle );
		}

		path = "CfgVehicles " + GetType() + " Crew";
		count = GetGame().ConfigGetChildrenCount( path );
		for ( i = 0; i < count; i++ )
		{
			string crewName;
			GetGame().ConfigGetChildName( path, i, crewName );

			m_Crew.Insert( new ExpansionVehicleCrew( this, crewName ) );
		}

		m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

		m_Doors = new TStringArray;
		ConfigGetTextArray( "doors", m_Doors );

		m_CanHaveLock = m_Doors.Count() > 0;
		
		m_SafeZone = false;

		string cHSSE = "hornSoundSetEXT";
		if ( ConfigIsExisting( cHSSE ) )
		{
			m_HornSoundSetEXT = ConfigGetString( cHSSE );
		} else
		{
			m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		}
		
		string cHSSI = "hornSoundSetINT";
		if ( ConfigIsExisting( cHSSI ) )
		{
			m_HornSoundSetINT = ConfigGetString( cHSSI );
		} else
		{		
			m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
		}

		GetPersistentID( m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD );

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );

		RegisterNetSyncVariableBool( "m_EngineIsOn" );
		RegisterNetSyncVariableBool( "m_HornSynchRemote" );
		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" ); 
		RegisterNetSyncVariableBool( "m_SafeZoneSynchRemote" ); 

		// RegisterNetSyncVariableBool( "m_IsWinched" );

		m_NetworkMode = ExpansionVehicleNetworkMode.SERVER_ONLY;
		RegisterNetSyncVariableInt( "m_NetworkMode" );

		m_SyncState = new ExpansionVehicleSyncState( this );
		float min = 0;
		float max = 0;
		int prec = 8;
		
		//! For prediction sync state, unfortuantely have to always sync even when not used, 
		//! should be forced synced every update on the server so this is the most optimized
		//! way when it is required.
		RegisterNetSyncVariableFloat( "m_SyncState.m_LinVelX", min, max, prec );
		RegisterNetSyncVariableFloat( "m_SyncState.m_LinVelY", min, max, prec );
		RegisterNetSyncVariableFloat( "m_SyncState.m_LinVelZ", min, max, prec );

		RegisterNetSyncVariableFloat( "m_SyncState.m_AngVelX", min, max, prec );
		RegisterNetSyncVariableFloat( "m_SyncState.m_AngVelY", min, max, prec );
		RegisterNetSyncVariableFloat( "m_SyncState.m_AngVelZ", min, max, prec );

		m_FluidCapacities = new array< float >();
		m_FluidCapacities.Insert( ConfigGetFloat( "fuelCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "oilCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "brakeFluidCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "coolantCapacity" ) );

		RegisterNetSyncVariableFloat( "m_FluidFuelAmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_FluidCoolantAmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_FluidOilAmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_FluidBrakeAmount", 0, 0, 4 );

		if ( IsMissionClient() )
		{		
			string sound_controller_path = "CfgVehicles " + GetType() + " soundController";
			string sound_controller = GetGame().ConfigGetTextOut( sound_controller_path );
		
			if ( Class.CastTo( m_SoundProxy, GetGame().CreateObject( sound_controller, GetPosition(), true ) ) )
			{
				m_SoundProxy.SetPosition( "0 0 0" );
				AddChild( m_SoundProxy, -1 );
		
				m_SoundProxy.SetExpansionVehicle( this );
			}
		}

		m_Transform = new Transform;

		m_Controller = GetControllerInstance();

		m_MarkerModule = ExpansionMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMarkerModule ) );
		m_ServerMarker = "";

		ExpansionSettings.SI_Vehicle.Insert( OnSettingsUpdated );
		OnSettingsUpdated();

		// #ifdef EXPANSION_USING_TRANSPORT_BASE
		// OnCreatePhysics();
		// #endif

		LoadConstantVariables();
	}

	// ------------------------------------------------------------
	void ~ExpansionVehicleScript()
	{
		ExpansionSettings.SI_Vehicle.Remove( OnSettingsUpdated );

		if ( dBodyIsSet( this ) )
		{
			dBodyDestroy( this );
		}

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( int i = 0; i < m_DebugShapes.Count(); i++ )
			m_DebugShapes[i].Destroy();
		#endif
		
		delete m_DebugShapes;
	}

	// ------------------------------------------------------------
	// Expansion CreateServerMarker
	// ------------------------------------------------------------
	void CreateServerMarker()
	{
		if ( IsMissionHost() )
		{
			if ( m_ServerMarker != "" )
				return;
			
			m_ServerMarkerIndex++;
			m_ServerMarker = "[DBG:" + m_ServerMarkerIndex + "]" + GetDisplayName();
			
			if ( IsCar() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Car", GetPosition(), ARGB(255, 255, 0, 0), true );
			else if ( IsHelicopter() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Helicopter", GetPosition(), ARGB(255, 0, 255, 0), true );
			else if ( IsBoat() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Boat", GetPosition(), ARGB(255, 0, 0, 255), true );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveServerMarker
	// ------------------------------------------------------------
	void RemoveServerMarker()
	{
		if ( IsMissionHost() )
		{
			if ( m_ServerMarker != "" )
			{
				m_MarkerModule.RemoveServerMarker( m_ServerMarker );
				m_ServerMarker = "";
			}
		}
	}

	// ------------------------------------------------------------
	void LoadConstantVariables()
	{
	}

	// ------------------------------------------------------------
	void OnAfterLoadConstantVariables()
	{
	}

	// ------------------------------------------------------------
	override void DeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleScript::DeferredInit - Start");
		#endif

		m_BoundingRadius = ClippingInfo( m_BoundingBox );

		m_MaxSpeedMS = m_MaxSpeed * ( 1.0 / 3.6 );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnAfterLoadConstantVariables, 100, false );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleScript::DeferredInit - End");
		#endif
	}

	void OnSettingsUpdated()
	{
		m_NetworkMode = GetExpansionSettings().GetVehicle().VehicleSync;

		SetSynchDirty();
	}

	private void CreateDynamic()
	{
		CreateDynamicPhysics( 288 );

		EnableDynamicCCD( true );
		SetDynamicPhysicsLifeTime( -1 );
		
		dBodyDynamic( this, true );
		dBodyActive( this, ActiveState.ALWAYS_ACTIVE );
		dBodySetInteractionLayer( this, 288 );
		dBodyEnableGravity( this, false );

		m_BodyMass = dBodyGetMass( this );
		m_InvMass = 1.0 / m_BodyMass;
		m_BodyCenterOfMass = dBodyGetCenterOfMass( this );

		SetVelocity( this, m_LinearVelocity );
		dBodySetAngularVelocity( this, m_AngularVelocity );

		EnableCollisionsWithCharacter( false );

		SetAllowDamage( false ); //! Temporary
	}

	// ------------------------------------------------------------
	override void OnCreatePhysics()
	{
	}

	// ------------------------------------------------------------
	override void EOnContact(IEntity other, Contact extra) //!EntityEvent.CONTACT
	{
		//Print( "[" + this + "] EOnContact " + other );
	}

	// ------------------------------------------------------------
	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - Start");
		#endif

		if ( CanUpdateCarLock( timeSlice ) )
		{
			UpdateCarLock( timeSlice );
		}

		if ( !GetGame().IsClient() && m_NetworkMode == ExpansionVehicleNetworkMode.CLIENT )
		{
			return;
		}
	}

	// ------------------------------------------------------------
	void ExpansionDebugUI( string message = "" )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CAR, message );
	}

	// ------------------------------------------------------------
	void UpdateExpansionController()
	{
		if ( m_Controller )
			m_Controller.Update();
	}
	
	// ------------------------------------------------------------
	void DBGDrawImpulseMS( vector position, vector impulse, int color = 0x44FFFFFF )
	{
		DBGDrawImpulse( position.Multiply4( m_Transform.data ), impulse.Multiply3( m_Transform.data ), color );
	}

	// ------------------------------------------------------------
	void DBGDrawImpulse( vector position, vector impulse, int color = 0x44FFFFFF )
	{
		vector acceleration = impulse;
		acceleration[0] = acceleration[0] / m_TimeSlice / m_BodyMass;
		acceleration[1] = acceleration[1] / m_TimeSlice / m_BodyMass;
		acceleration[2] = acceleration[2] / m_TimeSlice / m_BodyMass;
		
		DBGDrawLine( position, position + acceleration, color );
	}
	
	// ------------------------------------------------------------
	void DBGDrawLineMS( vector start, vector end, int color = 0x44FFFFFF )
	{
		DBGDrawLine( start.Multiply4( m_Transform.data ), end.Multiply4( m_Transform.data ), color );
	}

	// ------------------------------------------------------------
	void DBGDrawLine( vector start, vector end, int color = 0x44FFFFFF )
	{
		vector pts[2]
		pts[0] = start;
		pts[1] = end;
		
		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		DBGAddShape( Shape.CreateLines( color, ShapeFlags.TRANSP | ShapeFlags.NOZBUFFER, pts, 2 ) );
		#endif
	}
	
	// ------------------------------------------------------------
	void DBGDrawLineDirectionMS( vector start, vector direction, int color = 0x44FFFFFF )
	{
		DBGDrawLineDirection( start.Multiply4( m_Transform.data ), direction.Multiply3( m_Transform.data ), color );
	}
	
	// ------------------------------------------------------------
	void DBGDrawLineDirection( vector start, vector direction, int color = 0x44FFFFFF )
	{
		DBGDrawLine( start, start + direction, color );
	}

	// ------------------------------------------------------------
	void DBGAddShape( Shape shape )
	{
		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_DebugShapes.Insert( shape );
		#endif
	}

	// ------------------------------------------------------------
	int GetTimeForSync()
	{
		return GetSimulationTimeStamp();
	}

	// ------------------------------------------------------------
	protected bool CanSimulate()
	{
		return true;
	}

	// ------------------------------------------------------------
	protected void OnHumanPilot( PlayerBase driver, float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnAIPilot( ExpansionAIBase driver, float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnPreSimulation( float pDt )
	{
		if ( IsMissionClient() )
		{
			OnParticleUpdate( pDt );
		}

		OnAnimationUpdate( pDt );

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	protected void OnSimulation( float pDt, out vector force, out vector torque )
	{
	}

	// ------------------------------------------------------------
	protected void OnPostSimulation( float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnParticleUpdate( float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnAnimationUpdate( float pDt )
	{
	}

	// ------------------------------------------------------------
	override void EOnSimulate( IEntity owner, float dt ) 
	{
		//Print( "[" + this + "] EOnSimulate" );

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( int dbg = 0; dbg < m_DebugShapes.Count(); ++dbg )
			m_DebugShapes[dbg].Destroy();

		m_DebugShapes.Clear();
		#endif

		m_TimeSlice = dt;

		if ( !CanSimulate() )
			return;

		ExpansionDebugUI( "[[ " + this + " ]]" );

		DayZPlayerImplement driver = DayZPlayerImplement.Cast( CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );

		//Print(typename.EnumToString(ExpansionVehicleNetworkMode, m_NetworkMode));
		//Print(driver);
		//Print(GetGame().GetPlayer());
		
		if ( !GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			m_IsPhysicsHost = true;
		} else if ( GetGame().IsClient() )
		{
			m_IsPhysicsHost = driver == GetGame().GetPlayer();

			if ( m_IsPhysicsHost )
			{
				m_IsPhysicsHost = m_NetworkMode != ExpansionVehicleNetworkMode.SERVER_ONLY;
			}
		} else if ( GetGame().IsServer() )
		{
			m_IsPhysicsHost = m_NetworkMode != ExpansionVehicleNetworkMode.CLIENT;

			if ( !m_IsPhysicsHost )
			{
				//! TODO: Fix, causes server crash
				m_IsPhysicsHost = driver == null || !driver.HasNetworkID() || !driver.IsAlive();
			}

			if ( m_IsPhysicsHost )
			{
				m_SyncState.m_LinearVelocity = GetVelocity( this );
				m_SyncState.m_AngularVelocity = dBodyGetAngularVelocity( this );
				m_SyncState.m_Position = GetPosition();
				m_SyncState.m_Orientation = GetOrientation();

				m_SyncState.m_TimeDelta = 0;
				m_SyncState.m_Time = GetTimeForSync();
				m_SyncState.m_LastRecievedTime = GetTimeForSync();

				Math3D.YawPitchRollMatrix( m_SyncState.m_Orientation, m_SyncState.m_InitialTransform.data );
				m_SyncState.m_InitialTransform.data[3] = m_SyncState.m_Position;
				m_SyncState.m_InitialTransform.UpdateUnion();
			} else
			{
				m_LinearVelocity = m_SyncState.m_LinearVelocity;
				m_AngularVelocity = m_SyncState.m_AngularVelocity;
			}

			for ( int cI = 0; cI < m_Crew.Count(); cI++ )
				m_Crew[cI].NetworkBubbleFix();
		} else
		{
			m_IsPhysicsHost = false;
		}

		m_HasDriver = false;
		if ( driver && m_IsPhysicsHost )
		{
			ExpansionAIBase aiDriver;
			PlayerBase humanDriver;

			if ( Class.CastTo( aiDriver, driver ) )
			{
				m_HasDriver = true;
				
				OnAIPilot( aiDriver, dt );
			} else if ( Class.CastTo( humanDriver, driver ) )
			{
				m_HasDriver = true;

				if ( !GetGame().IsClient() && GetGame().IsMultiplayer() )
					m_Controller.Update();

				OnHumanPilot( humanDriver, dt );
			}
		}

		OnPreSimulation( dt );

		if ( !GetGame().IsClient() && !m_IsPhysicsHost && m_NetworkMode == ExpansionVehicleNetworkMode.CLIENT )
		{
			if ( !dBodyIsDynamic( this ) )
			{
				float predictionDelta = ( GetTimeForSync() + m_SyncState.m_TimeDelta - m_SyncState.m_Time ) / 1000.0;

				ExpansionPhysics.IntegrateTransform( m_SyncState.m_InitialTransform, m_SyncState.m_LinearVelocity, m_SyncState.m_AngularVelocity, predictionDelta, m_SyncState.m_PredictedTransform );

				MoveInTime( m_SyncState.m_PredictedTransform.data, dt );

				SetSynchDirty();
			} else
			{
				SetDynamicPhysicsLifeTime( 0.01 );
			}

			ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_CAR );

			return;
		}

		if ( m_IsPhysicsHost && dBodyIsDynamic( this ) )
		{
			float invDt = 1.0 / dt;
			
			dBodyEnableGravity( this, false );

			ExpansionDebugUI( "Mass: " + m_BodyMass + " (Center of Mass: " + m_BodyCenterOfMass + ")");

			ExpansionDebugUI( "Linear Velocity (WS): " + m_LinearVelocity );
			ExpansionDebugUI( "Linear Velocity (MS): " + m_LinearVelocityMS );

			m_InvInertiaTensor = dBodyGetLocalInertia( this );
			
			CalculateAltitudeLimiter();
			
			int i = 0;
			int numWheelsGrounded = 0;
			for ( i = 0; i < m_Axles.Count(); i++ )
				m_Axles[i].SetupSimulation( dt, numWheelsGrounded );

			vector force = vector.Zero;
			vector torque = vector.Zero;
			OnSimulation( dt, force, torque );

			vector impulse = force * dt;
			vector impulseTorque = torque * dt;
			
			for ( i = 0; i < m_Axles.Count(); i++ )
				m_Axles[i].Simulate( dt, numWheelsGrounded, impulse, impulseTorque );

			if ( GetNumWheels() != numWheelsGrounded )
			{
				vector gravity = "0 -9.8 0" * m_BodyMass;
				//float invWheelsGrounded = 1.0 / ( GetNumWheels() - numWheelsGrounded );
				impulse += gravity * dt;// * invWheelsGrounded;
			}

			ApplyPhysics( dt, impulse, impulseTorque );

			m_Transform.Get( this );

			m_LastLinearVelocity = m_LinearVelocity;
			m_LinearVelocity = GetVelocity( this );
			m_LinearAcceleration = ( m_LastLinearVelocity - m_LinearVelocity ) * dt;

			m_LastAngularVelocity = m_AngularVelocity;
			m_AngularVelocity = dBodyGetAngularVelocity( this );
			m_AngularAcceleration = ( m_LastAngularVelocity - m_AngularVelocity ) * dt;
			
			m_LastLinearVelocityMS = m_LinearVelocityMS;
			m_LinearVelocityMS = m_LinearVelocity.InvMultiply3( m_Transform.data );
			m_LinearAccelerationMS = ( m_LastLinearVelocityMS - m_LinearVelocityMS ) * dt;

			m_LastAngularVelocityMS = m_AngularVelocityMS;
			m_AngularVelocityMS = m_AngularVelocity.InvMultiply3( m_Transform.data );
			m_AngularAccelerationMS = ( m_LastAngularVelocityMS - m_AngularVelocityMS ) * dt;
			
			if ( IsMissionClient() )
				NetworkSend();
			else if ( GetGame().IsMultiplayer() )
				SetSynchDirty();
		} else if ( m_IsPhysicsHost && !dBodyIsDynamic( this ) )
		{
			CreateDynamic();
		} else if ( dBodyIsDynamic( this ) )
		{
			SetDynamicPhysicsLifeTime( 0.01 );
		}

		OnPostSimulation( dt );
		
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_WHEELS );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_CAR );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_ENGINE );
	}

	// ------------------------------------------------------------
	private void HandleSync_Server()
	{
		switch ( m_NetworkMode )
		{
		case ExpansionVehicleNetworkMode.SERVER_ONLY:
			m_SyncState.m_LinearVelocity = GetVelocity( this );
			m_SyncState.m_AngularVelocity = dBodyGetAngularVelocity( this );

			m_SyncState.SynchronizeVariables( m_NetworkMode );
			return;
		case ExpansionVehicleNetworkMode.PREDICTION:
			return;
		case ExpansionVehicleNetworkMode.CLIENT:
			return;
		}
	}

	// ------------------------------------------------------------
	private void HandleSync_Client()
	{
		switch ( m_NetworkMode )
		{
		case ExpansionVehicleNetworkMode.SERVER_ONLY:
			return;
		case ExpansionVehicleNetworkMode.PREDICTION:
			return;
		case ExpansionVehicleNetworkMode.CLIENT:
			m_SyncState.m_LinearVelocity = GetVelocity( this );
			m_SyncState.m_AngularVelocity = dBodyGetAngularVelocity( this );
			m_SyncState.m_Position = GetPosition();
			m_SyncState.m_Orientation = GetOrientation();

			m_SyncState.SynchronizeVariables( m_NetworkMode );
			return;
		}
	}

	// ------------------------------------------------------------
	void ApplyPhysics( float pDt, vector impulse, vector impulseTorque )
	{
		dBodyApplyImpulse( this, impulse );
		dBodyApplyTorqueImpulse( this, impulseTorque );

		if ( !GetGame().IsClient() )
		{
			HandleSync_Server();
		} else if ( !GetGame().IsServer() )
		{
			HandleSync_Client();
		}
	}

	// ------------------------------------------------------------
	override bool OnNetworkTransformUpdate( out vector pos, out vector ypr )
	{
		m_SyncState.m_Position = pos;
		m_SyncState.m_Orientation[0] = ypr[0] * Math.RAD2DEG;
		m_SyncState.m_Orientation[1] = ypr[1] * Math.RAD2DEG;
		m_SyncState.m_Orientation[2] = ypr[2] * Math.RAD2DEG;

		if ( m_IsPhysicsHost )
		{
			pos = GetPosition();
			ypr = GetOrientation();
			ypr[0] = ypr[0] * Math.DEG2RAD;
			ypr[1] = ypr[1] * Math.DEG2RAD;
			ypr[2] = ypr[2] * Math.DEG2RAD;

			return true;
		}
		
		return true;
	}

	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC( sender, rpc_type, ctx );

		switch( rpc_type )
		{
			case ExpansionVehicleRPC.ControllerSync:
			{
				if ( m_Controller )
				{
					if ( CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) == PlayerBase.GetPlayerByUID( sender.GetId() ) )
					{
						if ( m_Controller.NetworkRecieve( ctx ) )
						{
							OnNetworkRecieve( ctx );
						}
					}
				}

				return;
			}
			case ERPCs.RPC_EXPLODE_EVENT:
			{
				if ( GetGame().IsClient() )
				{
					Param2< int, string > params;

					if ( ctx.Read( params ) )
					{
						ExpansionOnExplodeClient( params.param1, params.param2 );
					}
				}
				
				return;
			}
			case ExpansionVehicleRPC.ClientSync:
			{
				if ( GetGame().IsClient() || m_NetworkMode != ExpansionVehicleNetworkMode.CLIENT )
					return;

				int time;
				ctx.Read( time );

				// check if this is an old state and if so, remove it
				if ( m_SyncState.m_Time > time )
					return;

				m_SyncState.m_LastRecievedTime = m_SyncState.m_Time;
				m_SyncState.m_Time = time;

				// time delta should always be positive, this is not the latency
				m_SyncState.m_TimeDelta = GetTimeForSync() - time;

				ctx.Read( m_SyncState.m_Position );
				ctx.Read( m_SyncState.m_Orientation );
				ctx.Read( m_SyncState.m_LinearVelocity );
				ctx.Read( m_SyncState.m_AngularVelocity );
				
				Math3D.YawPitchRollMatrix( m_SyncState.m_Orientation, m_SyncState.m_InitialTransform.data );
				m_SyncState.m_InitialTransform.data[3] = m_SyncState.m_Position;
				m_SyncState.m_InitialTransform.UpdateUnion();

				return;
			}
		}
	}

	// ------------------------------------------------------------
	void NetworkSend()
	{
		if ( IsMissionOffline() )
			return;

		ScriptRPC rpc = new ScriptRPC();

		m_Controller.NetworkSend( rpc );
		OnNetworkSend( rpc );

		if ( IsMissionHost() )
		{
			Human human = CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER );
			if ( human != NULL )
				rpc.Send( NULL, ExpansionVehicleRPC.ControllerSync, true, human.GetIdentity() );
		} else
		{
			rpc.Send( NULL, ExpansionVehicleRPC.ControllerSync, true, NULL );
		}
	}

	// ------------------------------------------------------------
	protected void OnNetworkSend( ref ParamsWriteContext ctx )
	{
	}

	// ------------------------------------------------------------
	protected void OnNetworkRecieve( ref ParamsReadContext ctx )
	{
	}

	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		m_SyncState.OnVariablesSynchronized();
	}

	// ------------------------------------------------------------
	void ApplyAxleTorque( int axle, float torque )
	{
		m_Axles[axle].ApplyTorque( torque );
	}

	// ------------------------------------------------------------
	void ApplyAxleBrake( int axle, float amt )
	{
		m_Axles[axle].ApplyBrake( amt );
	}

	// ------------------------------------------------------------
	void ApplyAxleSteering( int axle, float amt )
	{
		m_Axles[axle].SetSteering( amt );
	}

	// ------------------------------------------------------------
	float GetAxleCount()
	{
		return m_Axles.Count();
	}

	// ------------------------------------------------------------
	float GetNumWheels()
	{
		return m_Wheels.Count();
	}

	// ------------------------------------------------------------
	vector GetModelVelocityAt( vector relPos )
	{
		return m_LinearVelocityMS + ( m_AngularVelocityMS * relPos );
	}

	// ------------------------------------------------------------
	vector GetWorldVelocityAt( vector relPos )
	{
		return GetModelVelocityAt( relPos ).Multiply3( m_Transform.GetBasis().data );
	}

	// ------------------------------------------------------------
	vector GetEstimatedPosition( float pDt )
	{
		return GetPosition() + ( m_LinearVelocity * pDt );
	}

	// ------------------------------------------------------------
	private void CalculateAltitudeLimiter()
	{
		float altitude = GetPosition()[1];
		
		if ( altitude > m_AltitudeFullForce )
		{
			if ( altitude > m_AltitudeNoForce )
			{
				m_AltitudeLimiter = 0;
			} else
			{
				m_AltitudeLimiter = 1 - ( altitude - m_AltitudeFullForce ) * ( 1 / ( m_AltitudeNoForce - m_AltitudeFullForce ) );
			}
		} else
		{
			m_AltitudeLimiter = 1;
		}
	}

	/**
	 * Returns an extrapolated orientation based on the current velocity
	 * 
	 * @param pDt delta to extrapolate by
	 * @return	current orientation extrapolated based on the time from the angular velocity
	 * 			if pDt = 0, no extrapolation, if pDt = 1/40 then orientation next frame
	 */
	Matrix3 GetEstimatedOrientation( float pDt )
	{
		Matrix3 estimatedOrientation;
		Matrix3.Tilda( ( ( m_AngularVelocity * pDt ) + ( m_AngularAcceleration * pDt ) ), estimatedOrientation );

		Math3D.MatrixInvMultiply3( estimatedOrientation.data, m_Transform.data, estimatedOrientation.data );
		
		estimatedOrientation[0] = m_Transform[0] + estimatedOrientation[0];
		estimatedOrientation[1] = m_Transform[1] + estimatedOrientation[1];
		estimatedOrientation[2] = m_Transform[2] + estimatedOrientation[2];

		estimatedOrientation[0].Normalized();
		estimatedOrientation[1].Normalized();
		estimatedOrientation[2].Normalized();

		return estimatedOrientation;
	}

	// ------------------------------------------------------------
	ref ExpansionVehicleAxle GetAxle( int axle )
	{
		return m_Axles[axle];
	}

	// ------------------------------------------------------------
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		ExpansionWheel wheel;
		if ( !Class.CastTo( wheel, item ) )
			return;

		string slot = slot_name;
		slot.ToLower();

		for ( int i = 0; i < m_Wheels.Count(); i++ )
		{
			string inv_slot = m_Wheels[i].GetInventorySlot();
			inv_slot.ToLower();

			if ( inv_slot == slot )
			{
				m_Wheels[i].OnWheelAttach( wheel );
			}
		}
	}

	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name )
	{
		ExpansionWheel wheel;
		if ( !Class.CastTo( wheel, item ) )
			return;

		string slot = slot_name;
		slot.ToLower();

		for ( int i = 0; i < m_Wheels.Count(); i++ )
		{
			string inv_slot = m_Wheels[i].GetInventorySlot();
			inv_slot.ToLower();

			if ( inv_slot == slot )
			{
				m_Wheels[i].OnWheelDetach();
			}
		}
	}

	// ------------------------------------------------------------
	ref ExpansionVehicleController GetExpansionController()
	{
		return m_Controller;
	}

	// ------------------------------------------------------------
	void SetAttachmentHealth( string itemName, float health )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetAttachmentHealth - Start");
		#endif
		EntityAI item = FindAttachmentBySlotName( itemName );

		if ( item )
			item.SetHealth( "", "", health );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetAttachmentHealth - End");
		#endif
	}

	void CreateLights( Object lod, string point, typename type, vector color, vector ambient, float radius, float brigthness, bool flare, bool shadows, float default = 0 )
	{
		ref array<Selection> lodSelections = new array<Selection>();

		LOD lodLod = lod.GetLODByName( "memory" );
		if ( lodLod )
		{
			if ( lodLod.GetSelections( lodSelections ) )
			{
				for ( int i = 0; i < lodSelections.Count(); i++ )
				{	 
					if ( lodSelections[i].GetName() == point )
					{
						for( int j = 0; j < lodSelections[i].GetVertexCount(); j++ )
						{
							ExpansionPointLight light = ExpansionPointLight.Cast( ExpansionPointLight.CreateLight( type, "0 0 0" ) );
							light.m_Val = default;
 
							light.SetRadiusTo( radius );
							light.SetBrightnessTo( brigthness );

							light.SetDiffuseColor( color[0], color[1], color[2] );
							light.SetAmbientColor( ambient[0], ambient[1], ambient[2] );
											
							light.SetFlareVisible( flare );
							light.SetCastShadow( shadows );

							light.AttachOnObject( lod, lodSelections[i].GetVertexPosition(lodLod, j), "0 0 0" );

							light.ExpansionSetEnabled( true );

							m_Lights.Insert( light );
						}
					}
				}
			}
		}
	}

	void CreateParticle( Object lod, string point, int type )
	{
		ref array<Selection> lodSelections = new array<Selection>();

		LOD lodLod = lod.GetLODByName( "memory" );
		if ( lodLod )
		{
			if ( lodLod.GetSelections( lodSelections ) )
			{
				for ( int i = 0; i < lodSelections.Count(); i++ )
				{	 
					if ( lodSelections[i].GetName() == point )
					{
						for( int j = 0; j < lodSelections[i].GetVertexCount(); j++ )
						{
							Particle particle = Particle.PlayOnObject( type, lod, lodSelections[i].GetVertexPosition(lodLod, j) );
							//! AddChild( particle.GetDirectParticleEffect(), -1, true );

							m_Particles.Insert( particle );
						}
					}
				}
			}
		}
	}

#ifndef EXPANSION_USING_TRANSPORT_BASE

////////////////////////////////////////////////////////////////////
// TRANSPORT CODE
////////////////////////////////////////////////////////////////////

	//! Returns crew capacity of this vehicle.
	int CrewSize()
	{
		return m_Crew.Count();
	}

	//! Returns crew member index based on action component index.
	//! -1 is returned when no crew position corresponds to given component index.
	int CrewPositionIndex( int componentIdx )
	{
		//Print( "CrewPositionIndex" );
		//Print( componentIdx );
		for ( int i = 0; i < m_Crew.Count(); i++ )
		{
			//Print( m_Crew[i].GetComponentIndex() );
			if ( m_Crew[i].GetComponentIndex() == componentIdx )
			{
				return i;
			}
		}

		return -1;
	}

	int CrewPositionIndexSelection( int componentIdx )
	{
		array<string> selections = new array<string>();
		GetActionComponentNameList( componentIdx, selections );
		
		for ( int i = 0; i < selections.Count(); i++ )
		{
			for ( int j = 0; j < m_Crew.Count(); j++ )
			{
				if ( m_Crew[j].GetComponentSelection() == selections[i] )
				{
					return j;
				}
			}
		}
		return -1;
	}

	//! Returns crew member index based on player's instance.
	//! -1 is returned when the player is not isnide.
	int CrewMemberIndex( Human player )
	{
		if ( player == NULL )
			return -1;

		for ( int i = 0; i < m_Crew.Count(); i++ )
		{
			if ( m_Crew[i].GetPlayer() == player )
			{
				return i;
			}
		}

		return -1;
	}

	//! Returns crew member based on position index.
	//! Null can be returned if no Human is present on the given position.
	Human CrewMember( int posIdx )
	{
		if ( posIdx >= m_Crew.Count() )
			return NULL;

		if ( posIdx < 0 )
			return NULL;
		
		return m_Crew[posIdx].GetPlayer();
	}

	//! Performs transfer of player from world into vehicle on given position.
	void CrewGetIn( Human player, int posIdx )
	{
		if ( posIdx >= m_Crew.Count() )
			return;

		if ( posIdx < 0 )
			return;

		m_Crew[posIdx].SetPlayer( DayZPlayerImplement.Cast( player ) );
	}

	//! Performs transfer of player from vehicle into world from given position.
	Human CrewGetOut( int posIdx )
	{
		if ( posIdx >= m_Crew.Count() )
			return NULL;

		if ( posIdx < 0 )
			return NULL;

		Human human = m_Crew[posIdx].GetPlayer();

		m_Crew[posIdx].SetPlayer( NULL );

		if ( human == NULL )
			return NULL;

		return human;
	}
	
	void CrewEntry( int posIdx, out vector pos, out vector dir )
	{
		if ( posIdx >= m_Crew.Count() )
			return;

		if ( posIdx < 0 )
			return;

		m_Crew[posIdx].GetIn( pos, dir );
	}

	void CrewEntryWS( int posIdx, out vector pos, out vector dir )
	{
		if ( posIdx >= m_Crew.Count() )
			return;

		if ( posIdx < 0 )
			return;

		m_Crew[posIdx].GetIn( pos, dir );

		pos = ModelToWorld( pos );
		dir = vector.Direction( GetPosition(), ModelToWorld( dir ) );
	}
	
	void CrewTransform( int posIdx, out vector mat[4] )
	{
		if ( posIdx >= m_Crew.Count() )
			return;

		if ( posIdx < 0 )
			return;

		m_Crew[posIdx].GetTransform( mat );
	}

	void CrewTransformWS( int posIdx, out vector mat[4] )
	{
		if ( posIdx >= m_Crew.Count() )
			return;

		if ( posIdx < 0 )
			return;

		m_Crew[posIdx].GetTransform( mat );

		mat[3] = ModelToWorld( mat[3] );
	}

	int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
	}
	
	int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}

	int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	int GetCrewIndex( string selection )
	{
		return -1;
	}

	bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		return true;	
	}

	bool CanReachSeatFromDoors( string pSeatSelection, vector pFromPos, float pDistance = 1.0 )
	{
		return true;		
	}

	bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}

	int GetSeatIndexFromDoor( string pDoorSelection )
	{
		//Potientially could be fixed some other way, currently follows the unfortunate pattern that CanReachDoorsFromSeat has created
		switch (pDoorSelection)
		{
			case "DoorsDriver":
				return 0;
				break;
			case "DoorsCoDriver":
				return 1;
				break;
			case "DoorsCargo1":
				return 2;
				break;
			case "DoorsCargo2":
				return 3;
				break;
		}
		return -1;
	}
	
	bool IsAreaAtDoorFree( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
	{
		vector crewPos;
		vector crewDir;
		CrewEntryWS( currentSeat, crewPos, crewDir );
		crewPos[1] = crewPos[1] + maxAllowedObjHeight + playerHeight * 0.5;
		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		excluded.Insert(this);
		excluded.Insert(GetGame().GetPlayer());
		GetGame().IsBoxColliding(crewPos, crewDir, Vector(horizontalExtents, playerHeight, horizontalExtents), excluded, collided); 
		foreach (Object o : collided)
		{
			vector minmax[2];
			if (o.GetCollisionBox(minmax))
				return false;
		}
		return true;
	}

	Shape DebugFreeAreaAtDoor( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
	{
		vector crewPos;
		vector crewDir;
		CrewEntryWS( currentSeat, crewPos, crewDir );
		crewPos[1] = crewPos[1] + maxAllowedObjHeight + playerHeight * 0.5;
		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		excluded.Insert(this);
		excluded.Insert(GetGame().GetPlayer());
		GetGame().IsBoxColliding(crewPos, crewDir, Vector(horizontalExtents, playerHeight, horizontalExtents), excluded, collided); 
		int color = ARGB(100, 0, 255, 0);
		foreach (Object o : collided)
		{
			vector minmax[2];
			if (o.GetCollisionBox(minmax))
			{
				color = ARGB(100, 255, 0, 0);
			}
		}

		return Debug.DrawCylinder(crewPos, horizontalExtents, playerHeight, color);
	}
#else
	static ref map<typename, ref TInputActionMap> m_AdvComTypeActionsMap = new map<typename, ref TInputActionMap>;
	TInputActionMap m_InputActionMap;
	bool m_ActionsInitialize;

	void InitializeActions()
	{
		m_InputActionMap = m_AdvComTypeActionsMap.Get( this.Type() );
		if(!m_InputActionMap)
		{
			TInputActionMap iam = new TInputActionMap;
			m_InputActionMap = iam;
			SetActions();
			m_AdvComTypeActionsMap.Insert(this.Type(), m_InputActionMap);
		}
	}
	
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if(!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}

	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		if(!action)
		{
			Debug.LogError("Action " + actionName + " dosn't exist!");
			return;
		}		
		
		typename ai = action.GetInputType();
		if(!ai)
		{
			m_ActionsInitialize = false;
			return;
		}
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("+ " + this + " add action: " + action + " input " + ai);
		#endif

		action_array.Insert(action);
	}
	
	void RemoveAction(typename actionName)
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		ActionBase action = player.GetActionManager().GetAction(actionName);
		typename ai = action.GetInputType();
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(action_array)
		{
			action_array.RemoveItem(action);
		}
	}
#endif

	override void SetActions()
	{
		AddAction( ExpansionActionGetInExpansionVehicle );
		AddAction( ExpansionActionOpenVehicleDoor );
		AddAction( ExpansionActionCloseVehicleDoor );
	}

////////////////////////////////////////////////////////////////////
// CAR/CARSCRIPT CODE
////////////////////////////////////////////////////////////////////

	//!	Returns the current speed of the vehicle in km/h.
	float GetSpeedometer()
	{
		return GetVelocity( this ).Length() * 3.6;
	}

	/*!
		Returns tank capacity for the specified vehicle's fluid.

		\param fluid the specified fluid type
	*/
	float GetFluidCapacity( CarFluid fluid )
	{
		return m_FluidCapacities.Get( fluid );
	}

	/*!
		Returns fraction value (in range <0, 1>)
		of the current state of the specified vehicle's fluid.

		\param[in] fluid the specified fluid type
	*/
	float GetFluidFraction( CarFluid fluid )
	{
		float amount;
		switch ( fluid )
		{
			case CarFluid.FUEL:
				amount = m_FluidFuelAmount;
				break;
			case CarFluid.OIL:
				amount = m_FluidCoolantAmount;
				break;
			case CarFluid.BRAKE:
				amount = m_FluidOilAmount;
				break;
			case CarFluid.COOLANT:
				amount = m_FluidBrakeAmount;
				break;
			default:
				return 0;
		}
		
		return amount / GetFluidCapacity( fluid );
	}

	//! Removes from the specified fluid the specified amount.
	void Leak( CarFluid fluid, float amount )
	{
		float previousAmt;
		float newAmt;

		switch ( fluid )
		{
			case CarFluid.FUEL:
				previousAmt = m_FluidFuelAmount;
				m_FluidFuelAmount -= amount;
				newAmt = m_FluidFuelAmount;
				break;
			case CarFluid.OIL:
				previousAmt = m_FluidCoolantAmount;
				m_FluidCoolantAmount -= amount;
				newAmt = m_FluidCoolantAmount;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_FluidOilAmount;
				m_FluidOilAmount -= amount;
				newAmt = m_FluidOilAmount;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_FluidBrakeAmount;
				m_FluidBrakeAmount -= amount;
				newAmt = m_FluidBrakeAmount;
				break;
			default:
				return;
		}

		OnFluidChanged( fluid, previousAmt, newAmt );
	}

	//! Removes all the specified fluid from vehicle.
	void LeakAll( CarFluid fluid )
	{
		float previousAmt;
		float newAmt;

		switch ( fluid )
		{
			case CarFluid.FUEL:
				previousAmt = m_FluidFuelAmount;
				m_FluidFuelAmount = 0;
				break;
			case CarFluid.OIL:
				previousAmt = m_FluidCoolantAmount;
				m_FluidCoolantAmount = 0;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_FluidOilAmount;
				m_FluidOilAmount = 0;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_FluidBrakeAmount;
				m_FluidBrakeAmount = 0;
				break;
			default:
				return;
		}

		OnFluidChanged( fluid, previousAmt, 0 );
	}

	//! Adds to the specified fluid the specified amount.
	void Fill( CarFluid fluid, float amount )
	{
		float previousAmt;
		float newAmt;

		switch ( fluid )
		{
			case CarFluid.FUEL:
				previousAmt = m_FluidFuelAmount;
				m_FluidFuelAmount += amount;
				newAmt = m_FluidFuelAmount;
				break;
			case CarFluid.OIL:
				previousAmt = m_FluidCoolantAmount;
				m_FluidCoolantAmount += amount;
				newAmt = m_FluidCoolantAmount;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_FluidOilAmount;
				m_FluidOilAmount += amount;
				newAmt = m_FluidOilAmount;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_FluidBrakeAmount;
				m_FluidBrakeAmount += amount;
				newAmt = m_FluidBrakeAmount;
				break;
			default:
				return;
		}

		OnFluidChanged( fluid, previousAmt, newAmt );
	}

	/*!
		Is called every time when the specified vehicle's fluid level
		changes eg. when car is consuming fuel.

		\param[in] fluid fluid identifier, \see CarFluid
		\param[in] newValue new fluid level
		\param[in] oldValue previous fluid level before change
	*/
	void OnFluidChanged( CarFluid fluid, float newValue, float oldValue )
	{
	}

	//! Returns engine's max rpm before engine blows up.
	float EngineGetRPMMax()
	{
		return 0;
	}

	//! Returns engine's maximal working rpm without damaging the engine.
	float EngineGetRPMRedline()
	{
		return 0;
	}

	//! Returns engine's rpm value.
	float EngineGetRPM()
	{
		return 0;
	}

	//! Returns true when engine is running, false otherwise.
	bool EngineIsOn()
	{
		return m_EngineIsOn;
	}

	//! Starts the engine.
	void EngineStart()
	{
		if ( OnBeforeEngineStart() )
		{
			m_EngineIsOn = true;

			OnEngineStart();
		}
	}

	/*!
		Is called every time the game wants to start the engine.

		\return true if the engine can start, false otherwise.
	*/
	protected bool OnBeforeEngineStart()
	{
		// engine can start by default
		return true;
	}

	//! Is called every time the engine starts.
	protected void OnEngineStart()
	{
	}

	//! Stops the engine.
	void EngineStop()
	{
		m_EngineIsOn = false;

		OnEngineStop();
	}

	//! Is called every time the engine stops.
	protected void OnEngineStop()
	{
	}

	//! Returns total number of gears.
	int GetGearsCount()
	{
		return 0;
	}

	void SetClutchState( bool pState )
	{
		m_ClutchState = pState;
	}

	float GetClutch()
	{
		return 0.0;
	}

	float GetSteering()
	{
		return 0.0;
	}

	float GetThrottle()
	{
		return 0.0;
	}

	float GetBraking()
	{
		return 0.0;
	}

	/*!
		Is called every time when the simulation changed gear.

		\param[in] newGear new gear level
		\param[in] oldGear previous gear level before gear shift
	*/
	void OnGearChanged( int newGear, int oldGear )
	{
	}

	/*!
		Is called every time when vehicle collides with other object.

		\param[in] zoneName configured vehicle's zone that was hit
		\param[in] localPos position where the vehicle was hit in vehicle's space
		\param[in] other object with which the vehicle is colliding
		\param[in] data contact properties
	*/
	void OnContact( string zoneName, vector localPos, IEntity other, Contact data )
	{
	}

	/*!
		Is called every sound simulation step.
		In this callback, user can modify behaviour of sound controllers.

		\param[in] ctrl sound controller identifier, \see CarSoundCtrl
		\param[in] oldValue already computed value by the game code
		\return new value of the specified sound controller.
	*/
	float OnSound( CarSoundCtrl ctrl, float oldValue )
	{
		// just use the computed value by the game code
		return oldValue;
	}

////////////////////////////////////////////////////////////////////
// EXPANSION CODE
////////////////////////////////////////////////////////////////////

#ifdef EXPANSION_VEHICLE_TOWING
	void CreateTow( Object tow )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CreateTow - Start");
		#endif

		CarScript cs;
		ExpansionVehicleScript evs;

		if ( !Class.CastTo( cs, tow ) && !Class.CastTo( evs, tow ) )
			return;

		if ( ( evs && evs.IsBeingTowed() ) || ( cs && cs.IsBeingTowed() ) || !IsMissionHost() )
			return;

		m_ChildTow = EntityAI.Cast( tow );
		m_IsTowing = true;

		m_TowPointCenter = GetTowCenterPosition( m_ChildTow );

		if ( cs )
			cs.OnTowCreated( this, m_TowPointCenter );
		else if ( evs )
			evs.OnTowCreated( this, m_TowPointCenter );
		
		if ( !IsMissionOffline() )
		{
			m_ChildTow.GetNetworkID( m_ChildTowNetworkIDLow, m_ChildTowNetworkIDHigh );
			
			m_ChildTow.SetSynchDirty();
			SetSynchDirty();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CreateTow - End");
		#endif
	}

	void OnTowCreated( Object parent, vector towPos )
	{
		m_ParentTow = EntityAI.Cast( parent );
		m_IsBeingTowed = true;
		m_TowPointCenterSelf = towPos;
		
		if ( !IsMissionOffline() )
			m_ParentTow.GetNetworkID( m_ParentTowNetworkIDLow, m_ParentTowNetworkIDHigh );
	}

	void DestroyTow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DestroyTow - Start");
		#endif

		if ( m_IsTowing )
		{
			CarScript cs;
			ExpansionVehicleScript evs;

			if ( Class.CastTo( cs, m_ChildTow ) )
			{
				cs.OnTowDestroyed();
			} 
			if ( Class.CastTo( evs, m_ChildTow ) )
			{
				evs.OnTowDestroyed();
			}
			
			m_ChildTow = NULL;

			m_IsTowing = false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DestroyTow - End");
		#endif
	}

	void OnTowDestroyed()
	{
		m_ParentTow = null;
		m_IsBeingTowed = false;
	}
#endif

	vector GetTowCenterPosition( Object other )
	{
		vector minMax[2];
		GetCollisionBox( minMax );
		vector pos = Vector( 0.0, 0.0, minMax[0][2] - GetTowLength() );
		other.GetCollisionBox( minMax );
		return pos + Vector( 0.0, 0.0, -minMax[1][2] );
	}

	bool IsBeingTowed()
	{
		return m_IsBeingTowed;
	}

	bool IsTowing()
	{
		return m_IsTowing;
	}
	
	vector GetTowPosition()
	{
		vector minMax[2];
		GetCollisionBox( minMax );

		return ModelToWorld( Vector( 0.0, 0.0, minMax[0][2] - GetTowLength() ) );
	}

	vector GetTowDirection()
	{
		return -GetDirection();
	}

	vector GetTowExtents()
	{
		return { 1.0, 1.0, 1.0 };
	}

	float GetTowLength()
	{
		return 0.1;
	}
	
	//! is it a car ? Is it already towing something ? And is it locked ?
	bool CanConnectTow( notnull Object other )
	{
		ExpansionVehicleScript evs;
		CarScript cs;
		if ( Class.CastTo( evs, other ) )
		{
			return evs.IsCar() && !evs.IsTowing() && !evs.IsLocked();
		} else if ( Class.CastTo( cs, other ) )
		{
			return cs.IsCar() && !cs.IsTowing() && !cs.IsLocked();
		}

		//! don't...
		return false;
	}

	// ------------------------------------------------------------
	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	// ------------------------------------------------------------
	void KeyMessage( string message )
	{
		Message( GetPlayer(), message );
	}

	// ------------------------------------------------------------
	override bool IsLocked()
	{
		if ( HasKey() )
		{
			return m_VehicleLockedState == ExpansionVehicleLockState.LOCKED;
		}

		return false;
	}

	// ------------------------------------------------------------
	bool HasKey()
	{
		return m_VehicleLockedState != ExpansionVehicleLockState.NOLOCK;
	}

	// ------------------------------------------------------------
	void PairKeyTo( ExpansionCarKey key )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PairKeyTo - Start");
		#endif

		if ( key )
		{
			key.PairToVehicle( this );

			m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;

			SetSynchDirty();
		}

		//KeyMessage( "PairKeyTo (" + this + ", " + key + ")" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PairKeyTo - End");
		#endif
	}

	// ------------------------------------------------------------
	bool CanBeLocked()
	{
		return true;
	}

	// ------------------------------------------------------------
	void OnCarDoorOpened( string source )
	{
		if ( HasKey() ) 
		{
			if ( m_VehicleLockedState == ExpansionVehicleLockState.READY_TO_LOCK )
			{
				m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
				//KeyMessage( "OnCarDoorOpened::UNLOCKED" );
				SetSynchDirty();
			}
		}
	}

	// ------------------------------------------------------------
	void OnCarDoorClosed( string source )
	{

	}

	// ------------------------------------------------------------
	bool IsCarKeys( ExpansionCarKey key )
	{
		if ( !HasKey() )
		{
			//KeyMessage( "IsCarKeys::HasKey" );
			return false;
		}

		if ( !key.IsPairedTo( this ) )
		{
			//KeyMessage( "IsCarKeys not paired!" );
			return false;
		}

		return true;
	}

	// ------------------------------------------------------------
	void LockCar( ExpansionCarKey key = NULL )
	{
		if ( key && !IsCarKeys( key ) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.READY_TO_LOCK;
		//KeyMessage( "LockCar::READY_TO_LOCK" );

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	void UnlockCar( ExpansionCarKey key = NULL )
	{
		if ( key && !IsCarKeys( key ) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
		//KeyMessage( "UnlockCar::UNLOCKED" );

		OnCarUnlocked();
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	void OnCarLocked()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarLocked - Start");
		#endif

		//KeyMessage( "OnCarLocked" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarLocked - Stop");
		#endif
	}

	// ------------------------------------------------------------
	void OnCarUnlocked()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarUnlocked - Start");
		#endif

		//KeyMessage( "OnCarUnlocked" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarUnlocked - End");
		#endif
	}

	bool CanUpdateCarLock( float pDt )
	{
		return ( HasKey() && m_VehicleLockedState == ExpansionVehicleLockState.READY_TO_LOCK && IsMissionHost() );
	}
	
	bool DoorCount()
	{
		return m_Doors.Count();
	}

	bool AllDoorsClosed()
	{
		for ( int z = 0; z < m_Doors.Count(); z++ )
		{
			if ( GetCarDoorsState( m_Doors[z] ) != CarDoorState.DOORS_CLOSED )
			{
				return false;
			}
		}

		return true;
	}

	// ------------------------------------------------------------	
	void UpdateCarLock( float pDt )
	{
		if ( AllDoorsClosed() || GetExpansionSettings() && !GetExpansionSettings().GetVehicle().VehicleRequireAllDoors )
		{
			m_VehicleLockedState = ExpansionVehicleLockState.LOCKED;

			//KeyMessage( "OnCarDoorClosed::LOCKED" );

			OnCarLocked();
			SetSynchDirty();
		}
	}
	
	// ------------------------------------------------------------
	void OnHonkSoundPlay()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHonkSoundPlay - Start");
		#endif

		m_HornPlaying = true;
		
		Object battery;
		if (IsVitalCarBattery()) battery = this.FindAttachmentBySlotName("CarBattery");
		if (IsVitalTruckBattery()) battery = this.FindAttachmentBySlotName("TruckBattery");
		
		if (battery)
		{
			if (GetGame().GetPlayer().IsCameraInsideVehicle())
				m_HornSound = SEffectManager.PlaySoundOnObject( m_HornSoundSetINT, this );
			else
				m_HornSound = SEffectManager.PlaySoundOnObject( m_HornSoundSetEXT, this );
		}
		
		m_HornSound.SetSoundAutodestroy( true );

		m_HornSound.SetSoundLoop( true );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHonkSoundPlay - End");
		#endif
	}

	// ------------------------------------------------------------
	void OnHonkSoundStop()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHonkSoundStop - Start");
		#endif

		m_HornPlaying = false;

		m_HornSound.SetSoundLoop( false );
		m_HornSound.SoundStop();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHonkSoundStop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void PlayHonk()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PlayHonk - Start");
		#endif

		m_HornSynchRemote = true;
		m_HornPlaying = false;

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PlayHonk - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void StopHonk()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::StopHonk - Start");
		#endif

		m_HornSynchRemote = false;
		m_HornPlaying = true;

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::StopHonk - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override bool IsSoundSynchRemote()
	{	
		return m_HornSynchRemote;
	}

	// ------------------------------------------------------------
	override void Explode( int damageType, string ammoType = "" )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::Explode - Start");
		#endif

		if ( GetGame().IsServer() && !m_Exploded )
		{
			if ( ammoType == "" )
				ammoType = this.ConfigGetString( "ammoType" );
			
			if ( ammoType == "" )
				ammoType = "Dummy_Heavy";

			ExpansionOnExplodeServer( damageType, ammoType );

			if ( GetGame().IsServer() && !GetGame().IsMultiplayer() )
			{
				ExpansionOnExplodeClient( damageType, ammoType );
			} else 
			{				
				GetGame().RPCSingleParam( this, ERPCs.RPC_EXPLODE_EVENT, new Param2< int, string >( damageType, ammoType ), true );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::Explode - End");
		#endif
	}

	// ------------------------------------------------------------
	// Explodes the vehicle on the server, telling the client that they should explode by setting the sync boolean.
	// ------------------------------------------------------------	
	void ExpansionOnExplodeServer( int damageType, string ammoType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeServer - Start");
		#endif

		m_Exploded = true;
		m_ExplodedSynchRemote = true;

		LeakAll( CarFluid.COOLANT );
		LeakAll( CarFluid.FUEL );
		LeakAll( CarFluid.OIL );
		LeakAll( CarFluid.BRAKE );

		SetHealth( "", "", 0.0 );

		SetAttachmentHealth( "ExpansionHelicopterBattery", 0.0 );
		SetAttachmentHealth( "ExpansionAircraftBattery", 0.0 );
		SetAttachmentHealth( "CarBattery", 0.0 );
		SetAttachmentHealth( "TruckBattery", 0.0 );

		SetAttachmentHealth( "GlowPlug", 0.0 );
		SetAttachmentHealth( "SparkPlug", 0.0 );
		
		SetAttachmentHealth( "Reflector_1_1", 0.0 );
		SetAttachmentHealth( "Reflector_2_1", 0.0 );

		SetAttachmentHealth( "EngineBelt", 0.0 );
		SetAttachmentHealth( "CarRadiator", 0.0 );

		for ( int i = 0; i < CrewSize(); i++ )
		{
			Human crew = CrewMember( i );
			if ( !crew )
				continue;

			crew.SetHealth( 0.0 );
		}

		vector force = GetVelocity( this ) * m_BodyMass;
		force[1] = m_BodyMass * 10 + force[1];
		dBodyApplyForce( this, force * 0.1 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Explodes the vehicle on the client, setting that the client has already exploded
	// ------------------------------------------------------------	
	void ExpansionOnExplodeClient( int damageType, string ammoType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeClient - Start");
		#endif

		StopSounds();
		EffectSound sound =	SEffectManager.PlaySound( "Expansion_Explosive_Vehicle_SoundSet", GetPosition() );
		sound.SetSoundAutodestroy( true );
		/*
		ExpansionPointLight explosionLight = ExpansionPointLight.Cast( ScriptedLightBase.CreateLight( ExpansionPointLight, Vector( 0, 0, 0 ) )  ); 

		explosionLight.SetDiffuseColor( 1, 0.25, 0 );
		explosionLight.SetPosition( GetPosition() );
		explosionLight.SetRadiusTo( 20 * m_ExplosionSize );
		explosionLight.SetBrightnessTo( 4.5 ); 
		explosionLight.SetLifetime( 0.1 );
		explosionLight.SetFadeOutTime( 0.1 );
		explosionLight.SetFlareVisible( false );

		vector localPos = GetMemoryPointPos( "velka osa" );
		Particle p = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_HELICOPTER, ModelToWorld( localPos ) );
		p.SetOrientation( n );

		Particle fire = Particle.PlayOnObject( ParticleList.EXPANSION_FIRE_HELICOPTER, this, localPos );
		//fire.SetOrientation( n );
		//AddChild(fire, 0);
		*/
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeClient - End");
		#endif
	}

	// ------------------------------------------------------------
	// The entity was already exploded but the client never knew about it (i.e. outside of network bubble)
	// ------------------------------------------------------------	
	void ExpansionOnSpawnExploded()
	{
		m_Exploded = true;

//		m_SmokeParticle = Particle.PlayOnObject( ParticleList.SMOKING_HELI_WRECK, this, GetMemoryPointPos( "velka osa" ) );
//		m_SmokeParticle.SetOrientation( n );
	}

	// ------------------------------------------------------------
	// Does nothing, vanilla code shall not run
	// ------------------------------------------------------------	
	override void OnExplodeClient()
	{
	}

	// ------------------------------------------------------------
	// Does nothing, vanilla code shall not run
	// ------------------------------------------------------------	
	override void SynchExplosion()
	{
	}

	// ------------------------------------------------------------
	bool IsSurfaceWater( vector position )
	{
		if ( GetGame().SurfaceIsSea( position[0], position[2] ) )
		{
			return true;
		} else if( GetGame().SurfaceIsPond( position[0], position[2] ) )
		{
			return true;
		}
		
		return false;
	}
	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if( !super.CanReceiveAttachment( attachment, slotId ) )
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if( !super.CanReleaseAttachment( attachment ) )
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool IsInventoryVisible()
	{
		if ( !super.IsInventoryVisible() )
			return false;

		if(GetExpansionSettings() && GetExpansionSettings().GetVehicle().VehicleLockedAllowInventoryAccess)
			return true;

		if(GetExpansionSettings().GetVehicle().VehicleLockedAllowInventoryAccessWithoutDoors && !AllDoorsClosed())
			return true;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}
	
	void SwitchGear()
	{
		if ( GetAnimationPhase("gear" ) == 0 )
		{
			SetAnimationPhase("gear", 1);
		} else 
		{
			SetAnimationPhase("gear", 0);
		}
	}
	
	// ------------------------------------------------------------
	//! Called when entity is being created as new by CE/ Debug
	override void EEOnCECreate()
	{
		#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - Start");
		#endif

		super.EEOnCECreate();

		array< EntityAI > items = new array< EntityAI >;
		GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
		for ( int i = 0; i < items.Count(); i++ )
		{
			ExpansionCarKey key;
			if ( Class.CastTo( key, items[i] ) )
			{
				PairKeyTo( key );
			}
		}
		
		if ( GetExpansionSettings().GetDebug().ShowVehicleDebugMarkers )
		{
			CreateServerMarker();
		}
		
		#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - End");
		#endif
	}

	// ------------------------------------------------------------
	override bool CanPutAsAttachment( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool CanRemoveFromCargo( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}

	// ------------------------------------------------------------	
	void StopSounds()
	{
	}

	// ------------------------------------------------------------
	bool IsExploded()
	{
		return m_Exploded;
	}

	// ------------------------------------------------------------
	string GetAnimSourceFromSelection( string selection )
	{
		return "";
	}

	// ------------------------------------------------------------
	string GetDoorConditionPointFromSelection( string selection )
	{
		return "";
	}
	
	// ------------------------------------------------------------
	string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		return "";
	}

	// ------------------------------------------------------------
	string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return "";
	}

	// ------------------------------------------------------------
	int GetCarDoorsState( string slotType )
	{
		return CarDoorState.DOORS_MISSING;
	}

	// ------------------------------------------------------------
	bool IsVitalAircraftBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalHelicopterBattery()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	bool IsVitalCarBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalSparkPlug()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	bool IsVitalRadiator()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	bool IsVitalGlowPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalTruckBattery()
	{
		return false;
	}

	//! Propper way to get if light is swiched on. Use instead of IsLightsOn().
	bool IsScriptedLightsOn()
	{
		return m_HeadlightsOn;
	}
	
	//! Switches headlights on/off, including the illumination of the control panel and synchronizes this change to all clients.
	void ToggleHeadlights()
	{
		m_HeadlightsOn = !m_HeadlightsOn;
		SetSynchDirty();
		
		UpdateLights();
	}

	bool IsReversing( int new_gear = -1 )
	{
		return false;
	}
	
	//! Updates representation of lights. Call on server and client after change in relevant mechanics.
	void UpdateLights(int new_gear = -1) // -1 is invalid gear.
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // client side
		{
			ItemBase battery;
			
			if ( IsVitalCarBattery() ) 
				battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
			
			if (battery)
			{
				// HEADLIGHTS
				
				if (m_HeadlightsOn)
				{
					DashboardShineOn();
					
					if (!m_Headlight  &&  m_HeadlightsState != CarHeadlightBulbsState.NONE)
					{
						m_Headlight = CreateFrontLight();
						TailLightsShineOn();
					}
					
					if (m_HeadlightsState == CarHeadlightBulbsState.LEFT)
					{
						m_Headlight.AttachOnMemoryPoint(this, CarScript.m_LeftHeadlightPoint, CarScript.m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.RIGHT)
					{
						m_Headlight.AttachOnMemoryPoint(this,CarScript. m_RightHeadlightPoint, CarScript.m_RightHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						RightFrontLightShineOn();
						LeftFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
					{
						vector local_pos_left = GetMemoryPointPos(CarScript.m_LeftHeadlightPoint);
						vector local_pos_right = GetMemoryPointPos(CarScript.m_RightHeadlightPoint);
						
						vector local_pos_middle = (local_pos_left + local_pos_right) *0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}
					
					if (m_Headlight  &&  m_HeadlightsState == CarHeadlightBulbsState.NONE)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
						LeftFrontLightShineOff();
						RightFrontLightShineOff();
						TailLightsShineOff();
					}
				}
				else
				{
					TailLightsShineOff();
					DashboardShineOff();
					LeftFrontLightShineOff();
					RightFrontLightShineOff();
					
					if (m_Headlight)
					{
						m_Headlight.FadeOut();
						m_Headlight = null;
					}
				}
			
				// REAR LIGHTS (brakes, reverse & tail lights)
			
				if ( EngineIsOn() )
				{
					int reverse_light_state = CarRearLightType.NONE;
					
					// reverse
										
					if ( IsReversing( new_gear ) )
					{
						reverse_light_state = CarRearLightType.REVERSE_ONLY;
					}
					
					// brakes
					
					if ( m_BrakesArePressed )
					{
						if ( reverse_light_state == CarRearLightType.REVERSE_ONLY )
						{
							reverse_light_state = CarRearLightType.BRAKES_AND_REVERSE;
						} else
						{
							reverse_light_state = CarRearLightType.BRAKES_ONLY;
						}
					}
					
					if (reverse_light_state != CarRearLightType.NONE	&&  m_HeadlightsState != CarHeadlightBulbsState.NONE  &&  !m_RearLight)
					{
						m_RearLight = CarRearLightBase.Cast( CreateRearLight() );
						vector local_pos = GetMemoryPointPos(CarScript.m_ReverseLightPoint);
						m_RearLight.AttachOnObject(this, local_pos, "180 0 0");
					}
					
					if (m_RearLight)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							m_RearLight.SetAsSegregatedReverseLight();

							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOn();
								BrakeLightsShineOff();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_ONLY)
						{
							m_RearLight.SetAsSegregatedBrakeLight();
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								ReverseLightsShineOff();
								BrakeLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.BRAKES_AND_REVERSE)
						{
							m_RearLight.AggregateLight();
							m_RearLight.SetFadeOutTime(1);
							
							if (m_HeadlightsState != CarHeadlightBulbsState.NONE)
							{
								BrakeLightsShineOn();
								ReverseLightsShineOn();
							}
						}
						else if (reverse_light_state == CarRearLightType.NONE)
						{
							m_RearLight.FadeOut();
							m_RearLight = null;
							ReverseLightsShineOff();
						}
					}
					else
					{
						ReverseLightsShineOff();
						BrakeLightsShineOff();
					}
				}
				else
				{
					BrakeLightsShineOff();
					ReverseLightsShineOff();
					
					if (m_RearLight)
					{
						m_RearLight.FadeOut();
						m_RearLight = null;
					}
				}
			}
			else
			{
				LeftFrontLightShineOff();
				RightFrontLightShineOff();
				DashboardShineOff();
				BrakeLightsShineOff();
				ReverseLightsShineOff();
				
				if (m_RearLight)
				{
					m_RearLight.FadeOut();
					m_RearLight = null;
				}
				
				if (m_Headlight)
				{
					m_Headlight.FadeOut();
					m_Headlight = null;
				}
			}
		}
	}
	
	void LeftFrontLightShineOn()
	{
		string material = ConfigGetString("frontReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_FRONT_LIGHT_L, material);
		}
	}
	
	void RightFrontLightShineOn()
	{
		string material = ConfigGetString("frontReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_FRONT_LIGHT_R, material);
		}
	}
	
	void LeftFrontLightShineOff()
	{
		string material = ConfigGetString("frontReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_FRONT_LIGHT_L, material);
		}
	}
	
	void RightFrontLightShineOff()
	{
		string material = ConfigGetString("frontReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_FRONT_LIGHT_R, material);
		}
	}
	
	void ReverseLightsShineOn()
	{
		string material = ConfigGetString("ReverseReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_REVERSE_LIGHT_L, material);
			SetObjectMaterial(CarScript.SELECTION_ID_REVERSE_LIGHT_R, material);
		}
	}
	
	void ReverseLightsShineOff()
	{
		string material = ConfigGetString("ReverseReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_REVERSE_LIGHT_L, material);
			SetObjectMaterial(CarScript.SELECTION_ID_REVERSE_LIGHT_R, material);
		}
	}
	
	void BrakeLightsShineOn()
	{
		string material = ConfigGetString("brakeReflectorMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_BRAKE_LIGHT_L, material);
			SetObjectMaterial(CarScript.SELECTION_ID_BRAKE_LIGHT_R, material);
		}
	}
	
	void BrakeLightsShineOff()
	{
		string material = ConfigGetString("brakeReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_BRAKE_LIGHT_L, material);
			SetObjectMaterial(CarScript.SELECTION_ID_BRAKE_LIGHT_R, material);
		}
	}
	
	void TailLightsShineOn()
	{
		string material = ConfigGetString("TailReflectorMatOn");
		string material_off = ConfigGetString("TailReflectorMatOff");
		
		if (material != ""  &&  material_off != "")
		{
			if (m_HeadlightsState == CarHeadlightBulbsState.LEFT)
			{
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_L, material);
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_R, material_off);
			}
			else if (m_HeadlightsState == CarHeadlightBulbsState.RIGHT)
			{
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_R, material);
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_L, material_off);
			}
			else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
			{
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_R, material);
				SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_L, material);
			}
		}
	}
	
	void TailLightsShineOff()
	{
		string material = ConfigGetString("TailReflectorMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_L, material);
			SetObjectMaterial(CarScript.SELECTION_ID_TAIL_LIGHT_R, material);
		}
	}
	
	void DashboardShineOn()
	{
		string material = ConfigGetString("dashboardMatOn");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_DASHBOARD_LIGHT, material);
		}
	}
	
	void DashboardShineOff()
	{
		string material = ConfigGetString("dashboardMatOff");
		
		if (material != "")
		{
			SetObjectMaterial(CarScript.SELECTION_ID_DASHBOARD_LIGHT, material);
		}
	}
	
	// ------------------------------------------------------------
	CarLightBase CreateRearLight()
	{
		return NULL;
	}
	
	// ------------------------------------------------------------
	CarLightBase CreateFrontLight()
	{
		return NULL;
	}

	// ------------------------------------------------------------
	bool IsPlane()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsBoat()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsHelicopter()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsCar()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool CanObjectAttach( Object obj )
	{
		return false;
	}

	// ------------------------------------------------------------
	bool LeavingSeatDoesAttachment( int posIdx )
	{
		// only called if CanObjectAttach returns true.

		return true;
	}
	
	// ------------------------------------------------------------
	float GetActionDistanceFuel()
	{
		return 2.5;
	}

	// ------------------------------------------------------------
	float GetCameraHeight()
	{
		return 1;
	}

	// ------------------------------------------------------------
	float GetCameraDistance()
	{
		return 20;
	}

	bool IsFreeLook()
	{
		return false;
	}

	override bool IsTransport()
	{
		return true;
	}

	// ------------------------------------------------------------
	float GetWreckAltitude()
	{
		return 1;
	}

	// ------------------------------------------------------------
	vector GetWreckOffset()
	{
		return "0 0 1";
	}

	// ------------------------------------------------------------
	string GetWreck()
	{
		return GetType() + "Wreck";
	}

	// ------------------------------------------------------------
	string GetActionCompNameFuel()
	{
		return "refill";
	}

	// ------------------------------------------------------------
	ExpansionVehicleController GetControllerInstance()
	{
		return NULL;
	} 
};