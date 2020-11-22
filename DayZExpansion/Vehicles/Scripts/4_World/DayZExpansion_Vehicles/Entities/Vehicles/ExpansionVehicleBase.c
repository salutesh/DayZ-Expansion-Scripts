/**
 * ExpansionVehicleBase.c
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
class ExpansionVehicleBase extends Transport
#else
class ExpansionVehicleBase extends ItemBase
#endif
{
	private static ref set< ExpansionVehicleBase > m_allVehicles = new set< ExpansionVehicleBase >;

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
	
	protected float m_AirDragFrontTotal;

	float m_BodyMass;
	float m_InvMass;
	vector m_BodyCenterOfMass;

	private float m_TimeSlice;
	protected bool m_IsPhysicsHost;
	protected bool m_PerformInterpolation;
	private float m_RenderFrameTime;
	private int m_PhysicsCreationTimer;
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

	protected ExpansionSoundProxyBase m_SoundProxy; //! not needed anymore
	protected ref array< float > m_SoundVariables;
	protected ref array< string > m_SoundControls;
	protected ref array< ref ExpansionVehicleSound > m_SoundControllers;

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
	private bool m_NetworkClientSideServerProcessing;
	private vector m_LastCheckedNetworkPosition;

	ref array< ref ExpansionPointLight > m_Lights;
	ref array< ref Particle > m_Particles;

	protected autoptr TStringArray m_Doors;
	protected bool m_CanHaveLock;

	protected bool m_ClutchState;

	protected ref array< float > m_FluidCapacities;

	// Debugging
	private ref array< Shape > m_DebugShapes;
	private EntityAI m_DebugApple; //! yay ;(

	//! CarScript
	protected float m_Time;

	static float DROWN_ENGINE_THRESHOLD = 0.5;
	static float DROWN_ENGINE_DAMAGE = 350.0;
	
	//! keeps ammount of each fluid
	protected float m_FuelAmmount;
	protected float m_CoolantAmmount;
	protected float m_OilAmmount;
	protected float m_BrakeAmmount;

	//!
	protected float m_dmgContactCoef;
	protected float m_EnviroHeatComfortOverride;

	//!
	protected float m_DrownTime;
	static vector m_DrownEnginePos;
	
	//!
	protected float m_EngineHealth;
	protected float m_RadiatorHealth;
	protected float m_FuelTankHealth;
	protected float m_BatteryHealth;
	protected float m_PlugHealth;

	//! Particles
	protected ref EffVehicleSmoke m_coolantFx;
	protected ref EffVehicleSmoke m_engineFx;
	protected ref EffVehicleSmoke m_exhaustFx;
		
	protected int m_enginePtcFx;
	protected int m_coolantPtcFx;
	protected int m_exhaustPtcFx;
	
	protected vector m_exhaustPtcPos;
	protected vector m_exhaustPtcDir;
	protected vector m_enginePtcPos;
	protected vector m_coolantPtcPos;
	
	protected vector m_fuelPos;

	protected vector m_enginePos;
	protected vector m_frontPos;
	protected vector m_backPos;
	protected vector m_side_1_1Pos;
	protected vector m_side_1_2Pos;
	protected vector m_side_2_1Pos;
	protected vector m_side_2_2Pos;	
	
	//!Sounds
	string m_EngineStartOK = "";
	string m_EngineStartBattery = "";
	string m_EngineStartPlug = "";
	string m_EngineStartFuel = "";
	string m_EngineStopFuel = "";
	
	string m_CarDoorOpenSound = "";
	string m_CarDoorCloseSound = "";
	
	protected bool m_PlayCrashSoundLight;
	protected bool m_PlayCrashSoundHeavy;
	
	protected bool m_HeadlightsOn;
	protected bool m_HeadlightsState;
	protected bool m_BrakesArePressed; // synchronized variable
	
	CarLightBase 		m_Headlight;
	CarRearLightBase 	m_RearLight;
	
	// Memory points for lights
	static string m_ReverseLightPoint = "light_reverse";
	static string m_LeftHeadlightPoint = "light_left";
	static string m_RightHeadlightPoint = "light_right";
	static string m_LeftHeadlightTargetPoint = "light_left_dir";
	static string m_RightHeadlightTargetPoint = "light_right_dir";
	
	static string m_DrownEnginePoint = "drown_engine";
	
	// Model selection IDs for texture/material changes
	// If each car needs different IDs, then feel free to remove the 'static' flag and overwrite these numbers down the hierarchy
	static const int SELECTION_ID_FRONT_LIGHT_L 	= 0;
	static const int SELECTION_ID_FRONT_LIGHT_R 	= 1;
	static const int SELECTION_ID_BRAKE_LIGHT_L 	= 2;
	static const int SELECTION_ID_BRAKE_LIGHT_R 	= 3;
	static const int SELECTION_ID_REVERSE_LIGHT_L 	= 4;
	static const int SELECTION_ID_REVERSE_LIGHT_R 	= 5;
	static const int SELECTION_ID_TAIL_LIGHT_L 		= 6;
	static const int SELECTION_ID_TAIL_LIGHT_R 		= 7;
	static const int SELECTION_ID_DASHBOARD_LIGHT 	= 8;

	// ------------------------------------------------------------
	void ExpansionVehicleBase()
	{
		//! Print( "[" + this + "] ExpansionVehicleBase" );
		
		SetFlags( EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false );
		SetEventMask( EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.FRAME | EntityEvent.PHYSICSMOVE );

		m_Time = 0;
		
		// sets max health for all components at init
		m_EngineHealth = 1;
		m_FuelTankHealth = 1;
		m_RadiatorHealth = -1;
		m_BatteryHealth = -1;
		m_PlugHealth = -1;

		m_enginePtcFx = -1;
		m_coolantPtcFx = -1;
		m_exhaustPtcFx = -1;

		m_dmgContactCoef = 0;
		m_EnviroHeatComfortOverride = 0;

		m_PlayCrashSoundLight = false;
		m_PlayCrashSoundHeavy = false;

		RegisterNetSyncVariableBool("m_PlayCrashSoundLight");
		RegisterNetSyncVariableBool("m_PlayCrashSoundHeavy");
		RegisterNetSyncVariableBool("m_HeadlightsOn");
		RegisterNetSyncVariableBool("m_BrakesArePressed");

		if 	( MemoryPointExists("ptcExhaust_end") )
		{
			//Print( "CarScript Init " + GetType() );
			m_exhaustPtcPos = GetMemoryPointPos("ptcExhaust_end");
			if ( MemoryPointExists("ptcExhaust_start") )
			{
				vector exhaustStart = GetMemoryPointPos("ptcExhaust_start");
				vector tempOri = vector.Direction( exhaustStart, m_exhaustPtcPos);//.Normalized();//.VectorToAngles();
				
				//m_exhaustPtcDir = tempOri;

				m_exhaustPtcDir[0] = -tempOri[2];
				m_exhaustPtcDir[1] = tempOri[1];
				m_exhaustPtcDir[2] = tempOri[0];
				
				m_exhaustPtcDir = m_exhaustPtcDir.Normalized().VectorToAngles();

			}
		}
		else
		{
			m_exhaustPtcPos = "0 0 0";
			m_exhaustPtcDir = "1 1 1";
		}
	
		if ( MemoryPointExists("ptcEnginePos") )
			m_enginePtcPos = GetMemoryPointPos("ptcEnginePos");
		else
			m_enginePtcPos = "0 0 0";		

		if ( MemoryPointExists("ptcCoolantPos") )
			m_coolantPtcPos = GetMemoryPointPos("ptcCoolantPos");
		else
			m_coolantPtcPos = "0 0 0";

		if ( MemoryPointExists("refill") )
			m_fuelPos = GetMemoryPointPos("refill");
		else
			m_fuelPos = "0 0 0";
		
		if ( MemoryPointExists("drown_engine") )
			m_DrownEnginePos = GetMemoryPointPos("drown_engine");
		else
			m_DrownEnginePos = "0 0 0";	

		if ( MemoryPointExists("dmgZone_engine") )
			m_enginePos = GetMemoryPointPos("dmgZone_engine");
		else
			m_enginePos = "0 0 0";

		if ( MemoryPointExists("dmgZone_front") )
			m_frontPos = GetMemoryPointPos("dmgZone_front");
		else
			m_frontPos = "0 0 0";

		if ( MemoryPointExists("dmgZone_back") )
			m_backPos = GetMemoryPointPos("dmgZone_back");
		else
			m_backPos = "0 0 0";		
				
		if ( MemoryPointExists("dmgZone_fender_1_1") )
			m_side_1_1Pos = GetMemoryPointPos("dmgZone_fender_1_1");
		else
			m_side_1_1Pos = "0 0 0";

		if ( MemoryPointExists("dmgZone_fender_1_2") )
			m_side_1_2Pos = GetMemoryPointPos("dmgZone_fender_1_2");
		else
			m_side_1_2Pos = "0 0 0";
		
		if ( MemoryPointExists("dmgZone_fender_2_1") )
			m_side_2_1Pos = GetMemoryPointPos("dmgZone_fender_2_1");
		else
			m_side_2_1Pos = "0 0 0";

		if ( MemoryPointExists("dmgZone_fender_2_2") )
			m_side_2_2Pos = GetMemoryPointPos("dmgZone_fender_2_2");
		else
			m_side_2_2Pos = "0 0 0";

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );

		RegisterNetSyncVariableBool( "m_EngineIsOn" );
		RegisterNetSyncVariableBool( "m_HornSynchRemote" );
		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" ); 
		RegisterNetSyncVariableBool( "m_SafeZoneSynchRemote" );
		
		RegisterNetSyncVariableFloat( "m_FuelAmmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_CoolantAmmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_OilAmmount", 0, 0, 4 );
		RegisterNetSyncVariableFloat( "m_BrakeAmmount", 0, 0, 4 );
		
		RegisterNetSyncVariableInt( "m_NetworkMode" );
		RegisterNetSyncVariableBool( "m_NetworkClientSideServerProcessing" );

		m_Axles = new array< ref ExpansionVehicleAxle >;
		m_Crew = new array< ref ExpansionVehicleCrew >;

		m_Wheels = new array< ExpansionVehicleWheel >;

		m_DebugShapes = new array< Shape >();

		m_allVehicles.Insert( this );
		
		string path;
		
		path = "CfgVehicles " + GetType() + " SimulationModule airDragFrontTotal";
		if ( GetGame().ConfigIsExisting( path ) )
			m_AirDragFrontTotal = GetGame().ConfigGetFloat( path );

		int i = 0;
		int count = 0;

		path = "CfgVehicles " + GetType() + " SimulationModule Axles";
		count = GetGame().ConfigGetChildrenCount( path );
		for ( i = 0; i < count; i++ )
		{
			string axleName;
			GetGame().ConfigGetChildName( path, i, axleName );

			string wheelPath = "CfgVehicles " + GetType() + " SimulationModule Axles " + axleName + " Wheels";
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

		m_SoundVariables = new array< float >();
		m_SoundControls = new array< string >();

		m_SoundControllers = new array< ref ExpansionVehicleSound >();

		if ( IsMissionClient() )
		{
			GetSoundControls( m_SoundControls );

			for ( i = 0; i < m_SoundControls.Count(); i++ )
				m_SoundVariables.Insert( 0 );

			path = "CfgVehicles " + GetType() + " Sounds soundSetsFilter";
			array< string > soundSetNames = new array< string >();
			GetGame().ConfigGetTextArray( path, soundSetNames );
			for ( i = 0; i < soundSetNames.Count(); i++ )
			{
				ref ExpansionVehicleSound soundController = new ExpansionVehicleSound( this, soundSetNames[i], m_SoundControls );
				m_SoundControllers.Insert( soundController );
			}
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

		m_NetworkMode = ExpansionVehicleNetworkMode.SERVER_ONLY;
		m_NetworkClientSideServerProcessing = false;

		m_SyncState = new ExpansionVehicleSyncState( this );
		m_SyncState.RegisterNetVariables();

		m_FluidCapacities = new array< float >();
		m_FluidCapacities.Insert( ConfigGetFloat( "fuelCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "oilCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "brakeFluidCapacity" ) );
		m_FluidCapacities.Insert( ConfigGetFloat( "coolantCapacity" ) );

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

		m_Lights = new array< ref ExpansionPointLight >;
		m_Particles = new array< ref Particle >;

		m_Transform = new Transform;

		m_Controller = GetControllerInstance();

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredInit );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );

		ExpansionSettings.SI_Vehicle.Insert( OnSettingsUpdated );
		OnSettingsUpdated();

		// #ifdef EXPANSION_USING_TRANSPORT_BASE
		// OnCreatePhysics();
		// #endif

		LoadConstantVariables();

		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			ExpansionNetSyncDebugObject netsync = ExpansionNetSyncDebugObject.Cast( GetGame().CreateObjectEx( "ExpansionNetSyncDebugObject", "0 0 0", ECE_PLACE_ON_SURFACE ) );
			netsync.SetSyncObject(this);
			m_DebugApple = netsync;
		} else
		{
			m_DebugApple = EntityAI.Cast( GetGame().CreateObjectEx( "Orange", "0 0 0", ECE_TRACE|ECE_LOCAL ) );
		}
	}

	// ------------------------------------------------------------
	void ~ExpansionVehicleBase()
	{
		ExpansionSettings.SI_Vehicle.Remove( OnSettingsUpdated );

		if (m_DebugApple)
			GetGame().ObjectDelete(m_DebugApple);

		if ( dBodyIsSet( this ) )
		{
			dBodyDestroy( this );
		}

		int i;

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( i = 0; i < m_DebugShapes.Count(); i++ )
			m_DebugShapes[i].Destroy();
		#endif
		
		delete m_DebugShapes;

		i = m_allVehicles.Find( this );
		if ( i >= 0 )
		{
			m_allVehicles.Remove( i );
		}
	}

	static set< ExpansionVehicleBase > GetAll()
	{
		return m_allVehicles;
	}

	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
				SEffectManager.Stop(m_coolantPtcFx);

			if ( SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
				SEffectManager.Stop( m_exhaustPtcFx );

			if ( SEffectManager.IsEffectExist( m_enginePtcFx ) )
				SEffectManager.Stop( m_enginePtcFx );
			
			if ( m_Headlight )
				m_Headlight.Destroy();
			
			if ( m_RearLight )
				m_RearLight.Destroy();
			
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
		EXPrint("ExpansionVehicleBase::DeferredInit - Start");
		#endif

		m_BoundingRadius = ClippingInfo( m_BoundingBox );

		m_MaxSpeedMS = m_MaxSpeed * ( 1.0 / 3.6 );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnAfterLoadConstantVariables, 100, false );

		if (GetGame().IsClient())
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(this, ExpansionVehicleRPC.RequestCrewSync, true, NULL);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionVehicleBase::DeferredInit - End");
		#endif
	}

	void OnSettingsUpdated()
	{
		m_NetworkMode = GetExpansionSettings().GetVehicle().VehicleSync;

		SetSynchDirty();
	}

	private void CreateDynamic()
	{
		int layer = 0;

		if ( m_IsPhysicsHost )
			layer = PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.CHARACTER | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE;
		else
			layer = PhxInteractionLayers.VEHICLE | PhxInteractionLayers.CHARACTER;

		CreateDynamicPhysics( PhxInteractionLayers.VEHICLE );

		EnableDynamicCCD( true );
		SetDynamicPhysicsLifeTime( -1 );
		
		dBodyDynamic( this, true );
		dBodyActive( this, ActiveState.ALWAYS_ACTIVE );
		dBodySetInteractionLayer( this, layer );
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

		//GetGame().CreateObject("ExpansionDebugBox_Red", );

		array<string> damageZones = new array<string>;
		GetDamageZones(damageZones);

		string zoneName = "";
		vector zonePosition;
		float largestDistance = 32000.0;
		vector contactMS = WorldToModel(extra.Position);

		foreach (string damageZone : damageZones)
		{
			vector pos = GetDamageZonePos(damageZone);
			if (vector.Distance(pos, contactMS) < largestDistance)
			{
				zonePosition = pos;
				zoneName = damageZone;
			}
		}

		if (zoneName != "")
			OnContact(zoneName, contactMS, other, extra);
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


		m_Time += timeSlice;

		//! move it to constants.c const float CAR_UPDATE_INTERVAL = 1.0
		if ( m_Time >= GameConstants.CARS_FLUIDS_TICK )
		{
			m_Time = 0;

			CarPartsHealthCheck();

			//First of all check if the car should stop the engine
			if ( GetGame().IsServer() && (EngineIsOn() || MotorIsOn()) )
			{
				if ( GetFluidFraction(CarFluid.FUEL) <= 0 || m_EngineHealth <= 0 )
					EngineStop();

				CheckVitalItem( IsVitalCarBattery(), "CarBattery" );
				CheckVitalItem( IsVitalTruckBattery(), "TruckBattery" );
				CheckVitalItem( IsVitalAircraftBattery(), "ExpansionAircraftBattery" );
				CheckVitalItem( IsVitalHelicopterBattery(), "ExpansionHelicopterBattery" );
				CheckVitalItem( IsVitalIgniterPlug(), "ExpansionIgniterPlug" );
				CheckVitalItem( IsVitalHydraulicHoses(), "ExpansionHydraulicHoses" );
				CheckVitalItem( IsVitalSparkPlug(), "SparkPlug" );
				CheckVitalItem( IsVitalGlowPlug(), "GlowPlug" );
				// engine belt is not needed right now
				//CheckVitalItem( IsVitalEngineBelt(), "EngineBelt" );
			}

			//! actions runned when the engine on
			if ( EngineIsOn() || MotorIsOn() )
			{
				if ( GetGame().IsServer() )
				{
					float dmg;

					if ( EngineGetRPM() >= EngineGetRPMRedline() )
					{
						if ( EngineGetRPM() > EngineGetRPMMax() )
							AddHealth( "Engine", "Health", -GetMaxHealth("Engine", "")); //CAR_RPM_DMG
							
						dmg = EngineGetRPM() * 0.001 * Math.RandomFloat( 0.02, 1.0 );  //CARS_TICK_DMG_MIN; //CARS_TICK_DMG_MAX
						//AddHealth( "Engine", "Health", -dmg); //CAR_RPM_DMG
						ProcessDirectDamage( DT_CUSTOM, null, "Engine", "EnviroDmg", "0 0 0", dmg );
					}

					//! leaking of coolant from radiator when damaged
					if ( IsVitalRadiator() )
					{
						if ( GetFluidFraction(CarFluid.COOLANT) > 0 && m_RadiatorHealth < 0.5 ) //CARS_LEAK_THRESHOLD
							LeakFluid( CarFluid.COOLANT );
					}

					if ( GetFluidFraction(CarFluid.FUEL) > 0 && m_FuelTankHealth < 0.5 )
						LeakFluid( CarFluid.FUEL );

					if ( GetFluidFraction(CarFluid.BRAKE) > 0 && m_EngineHealth < 0.5 )
						LeakFluid( CarFluid.BRAKE );

					if ( GetFluidFraction(CarFluid.OIL) > 0 && m_EngineHealth < 0.5 )
						LeakFluid( CarFluid.OIL );

					if ( m_EngineHealth < 0.25 )
						LeakFluid( CarFluid.OIL );
/*Commented out till indicator of Oil in HUD will be ready
					if ( GetFluidFraction( CarFluid.OIL ) < 1 )
					{
						dmg = Math.Lerp( 0.02, 10, 1 - GetFluidFraction( CarFluid.OIL ) );  //CARS_TICK_DMG_MIN; //CARS_TICK_DMG_MAX
						AddHealth( "Engine", "Health", -dmg);
					}
*/
					if ( IsVitalRadiator() )
					{
						if ( GetFluidFraction( CarFluid.COOLANT ) < 0.5 && GetFluidFraction( CarFluid.COOLANT ) >= 0 )
						{
							dmg = ( 1 - GetFluidFraction(CarFluid.COOLANT) ) * Math.RandomFloat( 0.02, 10.00 );  //CARS_DMG_TICK_MIN_COOLANT; //CARS_DMG_TICK_MAX_COOLANT
							AddHealth( "Engine", "Health", -dmg );
						}
					}
				}
				
				//FX only on Client and in Single
				if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
				{
					if ( !SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
					{
						m_exhaustFx = new EffExhaustSmoke();
						m_exhaustPtcFx = SEffectManager.PlayOnObject( m_exhaustFx, this, m_exhaustPtcPos, m_exhaustPtcDir );
					}

					m_exhaustFx.SetParticleStateLight();
				
					if ( IsVitalRadiator() && SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						SEffectManager.Stop(m_coolantPtcFx);
					
					if ( IsVitalRadiator() && GetFluidFraction( CarFluid.COOLANT ) < 0.5 )
					{
						if ( !SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						{
							m_coolantFx = new EffCoolantSteam();
							m_coolantPtcFx = SEffectManager.PlayOnObject( m_coolantFx, this, m_coolantPtcPos, Vector(0,0,0), true );
						}

						if ( GetFluidFraction( CarFluid.COOLANT ) > 0 )
							m_coolantFx.SetParticleStateLight();
						else
							m_coolantFx.SetParticleStateHeavy();
/*
						Particle ptc;
						if ( GetCarDoorsState("NivaHood") == CarDoorState.DOORS_CLOSED )
						{
							if ( Class.CastTo(ptc, m_coolantFx.GetParticle() ) )
							{
								ptc.SetParameter( -1, EmitorParam.AIR_RESISTANCE, 55 );
								ptc.SetParameter( -1, EmitorParam.SIZE, 0.05 );
							}
						}
						else
						{
							if ( Class.CastTo(ptc, m_coolantFx.GetParticle() ) )
							{
								ptc.SetParameter( -1, EmitorParam.AIR_RESISTANCE, 2 );
								ptc.SetParameter( -1, EmitorParam.SIZE, 0.5 );
							}
						}
*/
					}
					else
					{
						if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
							SEffectManager.Stop(m_coolantPtcFx);
					}
				}
			}
			else
			{
				//FX only on Client and in Single
				if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
				{
					if ( SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
						SEffectManager.Stop(m_exhaustPtcFx);
					
					if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						SEffectManager.Stop(m_coolantPtcFx);
				}
			}
		}

		if ( !GetGame().IsClient() && m_NetworkMode == ExpansionVehicleNetworkMode.CLIENT )
		{
			return;
		}
	}
	
	// ------------------------------------------------------------
	override void EOnFrame( IEntity other, float timeSlice )
	{
		if ( m_PerformInterpolation && GetGame().IsClient() )
		{
			m_RenderFrameTime += timeSlice;

			bool useSetTargetMatrix = GetExpansionSettings().GetDebug().DebugVehicleTransformSet == 1;

			float predictionDelta = ( GetTimeForSync() + m_SyncState.m_TimeDelta - m_SyncState.m_Time ) / 40.0;

			ExpansionPhysics.IntegrateTransform( m_SyncState.m_InitialTransform, m_SyncState.m_LinearVelocity, m_SyncState.m_AngularVelocity, Math.Max( predictionDelta, 0.0 ) + m_RenderFrameTime, m_SyncState.m_PredictedTransform );

			if ( useSetTargetMatrix && dBodyIsDynamic( this ) )
			{
				dBodySetTargetMatrix( this, m_SyncState.m_PredictedTransform, 0.0 );
			} else
			{
				SetTransform( m_SyncState.m_PredictedTransform );
			}
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
	bool IsPhysicsHost()
	{
		return m_IsPhysicsHost;
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
		
		OnUpdate(pDt);
	}

	// ------------------------------------------------------------
	protected void OnSimulation( float pDt, out vector force, out vector torque )
	{
	}

	// ------------------------------------------------------------
	protected void OnPostSimulation( float pDt )
	{
		OnAnimationUpdate( pDt );
		
		m_SoundVariables[0] = OnSound( CarSoundCtrl.RPM, EngineGetRPM() + 1200 );
		m_SoundVariables[1] = OnSound( CarSoundCtrl.ENGINE, EngineIsOn() || MotorIsOn() );
		m_SoundVariables[2] = OnSound( CarSoundCtrl.PLAYER, 0 );
		m_SoundVariables[3] = OnSound( CarSoundCtrl.DOORS, 0 );
		m_SoundVariables[4] = OnSound( CarSoundCtrl.SPEED, GetSpeedometer() );
		m_SoundVariables[5] = 1.0;
		
		int start = TickCount(0);
				
		foreach ( ExpansionVehicleSound sound : m_SoundControllers )
			sound.Update( pDt, m_SoundVariables );
		
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_SOUND, "Ticks: " + TickCount( start ) );
	}

	// ------------------------------------------------------------
	protected void OnParticleUpdate( float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnAnimationUpdate( float pDt )
	{
		for ( int i = 0; i < m_Axles.Count(); i++ )
			m_Axles[i].Animate( pDt, m_IsPhysicsHost );
	}
	
	int stateF = -1;
	int stateB = -1;
	float stateTime = 0;

	// ------------------------------------------------------------
	override void EOnSimulate( IEntity owner, float dt ) 
	{
		m_DebugApple.SetPosition( GetPosition() );
		m_DebugApple.SetOrientation( GetOrientation() );
		m_DebugApple.SetSynchDirty();

		//Print( "[" + this + "] EOnSimulate" );

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( int dbg = 0; dbg < m_DebugShapes.Count(); ++dbg )
			m_DebugShapes[dbg].Destroy();

		m_DebugShapes.Clear();
		#endif

		m_TimeSlice = dt;

		//if ( !CanSimulate() )
		//	return;

		ExpansionDebugUI( "[[ " + this + " ]]" );

		DayZPlayerImplement driver = DayZPlayerImplement.Cast( CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );

		//Print(typename.EnumToString(ExpansionVehicleNetworkMode, m_NetworkMode));
		//Print(driver);
		//Print(GetGame().GetPlayer());
		
		m_PerformInterpolation = false;
		bool forceCreatePhysics = false;

		bool prevForceCreate = m_IsPhysicsHost;

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

			forceCreatePhysics = GetGame().GetPlayer().GetParent() == this;
			if ( !forceCreatePhysics )
				forceCreatePhysics = GetExpansionSettings().GetDebug().DebugVehicleSync == 1;

			ExpansionDebugUI( "m_IsPhysicsHost: " + m_IsPhysicsHost );
			ExpansionDebugUI( "forceCreatePhysics: " + forceCreatePhysics );
		} else if ( GetGame().IsServer() )
		{
			m_NetworkClientSideServerProcessing = true;

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

				Math3D.YawPitchRollMatrix( m_SyncState.m_Orientation, m_SyncState.m_InitialTransform );
				m_SyncState.m_InitialTransform[3] = m_SyncState.m_Position;
			} else
			{
				if (!prevForceCreate)
				{
					forceCreatePhysics = GetExpansionSettings().GetDebug().DebugVehicleSync == 1;
				}

				m_LinearVelocity = m_SyncState.m_LinearVelocity;
				m_AngularVelocity = m_SyncState.m_AngularVelocity;
			}

			if ( vector.Distance( m_LastCheckedNetworkPosition, GetPosition() ) > 30.0 )
			{
				m_LastCheckedNetworkPosition = GetPosition();

				for ( int cI = 0; cI < m_Crew.Count(); cI++ )
					m_Crew[cI].NetworkBubbleFix();
			}
		} else
		{
			m_IsPhysicsHost = false;

			Error("Should not reach here...");
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
		
		if ( m_IsPhysicsHost && dBodyIsDynamic( this ) )
		{
			stateF = 0;
			
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

			vector gravity = "0 -9.8 0" * m_BodyMass;
			impulse += gravity * dt;

			ApplyPhysics( dt, impulse, impulseTorque );
			
			if ( IsMissionClient() )
				NetworkSend();
		} else if ( ( m_IsPhysicsHost || forceCreatePhysics ) && !dBodyIsDynamic( this ) )
		{
			stateF = 1;

			CreateDynamic();
		} else if ( !m_IsPhysicsHost && !forceCreatePhysics && dBodyIsDynamic( this ) )
		{
			stateF = 2;
			
			SetDynamicPhysicsLifeTime( 0.01 );
		} else
		{
			stateF = 3;

			m_PerformInterpolation = true;
			
			if ( IsMissionHost() )
				m_NetworkClientSideServerProcessing = false;

			/*if ( GetGame().IsClient() )
			{
				float predictionDelta = ( GetTimeForSync() + m_SyncState.m_TimeDelta - m_SyncState.m_Time ) / 40.0;

				ExpansionPhysics.IntegrateTransform( m_SyncState.m_InitialTransform, m_SyncState.m_LinearVelocity, m_SyncState.m_AngularVelocity, Math.Max( predictionDelta, 0.0 ), m_SyncState.m_PredictedTransform );

				MoveTo( dt, m_SyncState.m_PredictedTransform );
			} else*/
			{
				MoveTo( dt, m_SyncState.m_InitialTransform );
			}

			SetSynchDirty();
		}

		ExpansionDebugUI( "stateF: " + stateF );
		
		if ( stateF != stateB )
		{
			Print( "" + this + " state changed to " + stateF + " from " + stateB );
			stateB = stateF;
		}

		OnPostSimulation( dt );

		if ( GetGame().IsMultiplayer() )
			SetSynchDirty();
		
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_WHEELS );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_CAR );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_ENGINE );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_SOUND );
	}

	// ------------------------------------------------------------
	private void HandleSync_Server()
	{
		switch ( m_NetworkMode )
		{
		case ExpansionVehicleNetworkMode.SERVER_ONLY:
			m_SyncState.m_LinearVelocity = GetVelocity( this );
			m_SyncState.m_AngularVelocity = dBodyGetAngularVelocity( this );
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

			m_SyncState.ClientSync();
			return;
		}
	}

	// ------------------------------------------------------------
	void MoveTo( float pDt, vector pTarget[4] )
	{
		if ( GetExpansionSettings().GetDebug().DebugVehicleTransformSet < 1 || !dBodyIsDynamic( this ) )
		{
			SetTransform( pTarget );
			return;
		}

		//! old way, causes mem leak and other issues
		//dBodySetTargetMatrix( this, pTarget, pDt );
		//return;

		vector transform[4];
		GetTransform( transform );

		vector linearVelocity;
		vector angularVelocity;

		ExpansionPhysics.CalculateVelocity( transform, pTarget, 1.0, linearVelocity, angularVelocity );

		if ( linearVelocity.Length() > ( 10.0 / pDt ) || angularVelocity.Length() > ( Math.PI / pDt ) )
		{
			//SetTransform( pTarget );
			//return;
		}

		if ( GetGame().IsServer() )
			ApplyPhysics( pDt, linearVelocity, angularVelocity, false, false );
	}

	// ------------------------------------------------------------
	void ApplyPhysics( float pDt, vector pImpulse, vector pImpulseTorque, bool pSync = true, bool pUseImpulse = true )
	{
		if (pUseImpulse)
		{
			dBodyApplyImpulse( this, pImpulse );
			dBodyApplyTorqueImpulse( this, pImpulseTorque );
		} else
		{
			m_LinearVelocity = GetVelocity( this );
			m_AngularVelocity = dBodyGetAngularVelocity( this );

			SetVelocity(this, pImpulse);
			dBodySetAngularVelocity(this, pImpulseTorque);
		}

		//SetVelocity(this, "0 0 0");
		//dBodySetAngularVelocity(this, "0 0 0");

		m_Transform.Get( this );

		m_LastLinearVelocity = m_LinearVelocity;
		m_LinearVelocity = GetVelocity( this );
		m_LinearAcceleration = ( m_LastLinearVelocity - m_LinearVelocity ) * pDt;

		m_LastAngularVelocity = m_AngularVelocity;
		m_AngularVelocity = dBodyGetAngularVelocity( this );
		m_AngularAcceleration = ( m_LastAngularVelocity - m_AngularVelocity ) * pDt;
		
		m_LastLinearVelocityMS = m_LinearVelocityMS;
		m_LinearVelocityMS = m_LinearVelocity.InvMultiply3( m_Transform.data );
		m_LinearAccelerationMS = ( m_LastLinearVelocityMS - m_LinearVelocityMS ) * pDt;

		m_LastAngularVelocityMS = m_AngularVelocityMS;
		m_AngularVelocityMS = m_AngularVelocity.InvMultiply3( m_Transform.data );
		m_AngularAccelerationMS = ( m_LastAngularVelocityMS - m_AngularVelocityMS ) * pDt;

		if ( !pSync )
			return;

		if ( !GetGame().IsClient() )
		{
			HandleSync_Server();
		} else if ( !GetGame().IsServer() )
		{
			HandleSync_Client();
		}
	}

	void AnglesToQuat( vector angles, out float[] quat )
	{
		vector rotationMatrix[3];
		Math3D.YawPitchRollMatrix( angles, rotationMatrix );
		Math3D.MatrixToQuat( rotationMatrix, quat );
	}

	// ------------------------------------------------------------
	// Mirek <3
	// ------------------------------------------------------------
	override bool OnNetworkTransformUpdate( out vector pos, out vector ypr )
	{
		m_RenderFrameTime = 0;

		if ( m_NetworkMode == ExpansionVehicleNetworkMode.CLIENT && !m_NetworkClientSideServerProcessing )
		{
			vector newPos = pos;
			vector newOrientation = ypr * Math.RAD2DEG;
			
			if ( !m_IsPhysicsHost )
			{
				float dt = ( m_SyncState.m_LastRecievedTime - m_SyncState.m_Time ) / 40.0;
				m_SyncState.m_LastRecievedTime = m_SyncState.m_Time;
				m_SyncState.m_Time = GetTimeForSync();
				m_SyncState.m_TimeDelta = 0;

				vector m1[];
				vector m2[];
				vector m3[];

				Math3D.YawPitchRollMatrix( newOrientation, m1 );
				Math3D.YawPitchRollMatrix( m_SyncState.m_Orientation, m2 );
				Math3D.MatrixMultiply3( m1, m2, m3 );

				Math3D.YawPitchRollMatrix( newOrientation, m_SyncState.m_InitialTransform );
				m_SyncState.m_InitialTransform[3] = newPos;
			}
			
			m_SyncState.m_Position = newPos;
			m_SyncState.m_Orientation = newOrientation;

			pos = GetPosition();
			ypr = GetOrientation() * Math.DEG2RAD;

			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC( sender, rpc_type, ctx );

		int crewIdx;

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

				m_SyncState.m_ClientSimulationTime = time;

				// time delta should always be positive, this is not the latency
				m_SyncState.m_TimeDelta = GetTimeForSync() - time;

				ctx.Read( m_SyncState.m_Position );
				ctx.Read( m_SyncState.m_Orientation );
				ctx.Read( m_SyncState.m_LinearVelocity );
				ctx.Read( m_SyncState.m_AngularVelocity );
				
				Math3D.YawPitchRollMatrix( m_SyncState.m_Orientation, m_SyncState.m_InitialTransform );
				m_SyncState.m_InitialTransform[3] = m_SyncState.m_Position;

				return;
			}
			case ExpansionVehicleRPC.CrewSync:
			{
				ctx.Read(crewIdx);

				m_Crew[crewIdx].OnRead(ctx);

				return;
			}
			case ExpansionVehicleRPC.CrewSyncInit:
			{
				for (crewIdx = 0; crewIdx < m_Crew.Count(); crewIdx++)
					m_Crew[crewIdx].OnRead(ctx);

				return;
			}
			case ExpansionVehicleRPC.RequestCrewSync:
			{
				ScriptRPC rpc = new ScriptRPC();
				for (crewIdx = 0; crewIdx < m_Crew.Count(); crewIdx++)
					m_Crew[crewIdx].OnSend(rpc);

				rpc.Send(this, ExpansionVehicleRPC.CrewSyncInit, true, sender);

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

		if ( GetCrashHeavySound() )
			PlayCrashHeavySound();
		else if ( GetCrashLightSound() )
			PlayCrashLightSound();
		
		UpdateLights();

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
		super.EEItemAttached( item, slot_name );
		if ( GetGame().IsServer() )
		{
			if ( slot_name == "Reflector_1_1" )
				SetHealth( "Reflector_1_1", "Health", item.GetHealth() );

			if ( slot_name == "Reflector_2_1" )
				SetHealth( "Reflector_2_1", "Health", item.GetHealth() );

			if ( slot_name == "CarBattery" )
				m_BatteryHealth = item.GetHealth01();
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = item.GetHealth01();

			if ( slot_name == "SparkPlug" )
				m_PlugHealth = item.GetHealth01();
			
			if ( slot_name == "GlowPlug" )
				m_PlugHealth = item.GetHealth01();
			
			Synchronize();
		}
		UpdateHeadlightState();
		UpdateLights();

		ExpansionWheelBase wheel;
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
				return;
			}
		}
	}
	
	// Updates state of attached headlight bulbs for faster access
	void UpdateHeadlightState()
	{
		EntityAI bulb_L = FindAttachmentBySlotName("Reflector_1_1");
		EntityAI bulb_R = FindAttachmentBySlotName("Reflector_2_1");
		
		if (bulb_L  &&  !bulb_L.IsRuined()  &&  bulb_R  &&  !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.BOTH;
		}
		else if (bulb_L  &&  !bulb_L.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.LEFT;
		}
		else if (bulb_R  &&  !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.RIGHT;
		}
		else if ( ( !bulb_L  ||  bulb_L.IsRuined() )  &&  ( !bulb_R  ||  bulb_R.IsRuined() ) )
		{
			m_HeadlightsState = CarHeadlightBulbsState.NONE;
		}
	}
	
	override void OnAttachmentRuined(EntityAI attachment)
	{
		super.OnAttachmentRuined(attachment);
		
		UpdateHeadlightState();
		UpdateLights();
	}

	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name )
	{
		super.EEItemAttached( item, slot_name );
		if ( GetGame().IsServer() )
		{
			//int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
			if ( IsScriptedLightsOn() )
			{
				if ( slot_name == "CarBattery" || slot_name == "TruckBattery" )
				{
					ToggleHeadlights();
				}
			}

			if ( EngineIsOn() )
			{
				//EngineBelt
				if ( slot_name == "GlowPlug" || slot_name == "SparkPlug" || slot_name == "CarBattery" || slot_name == "TruckBattery" )
					EngineStop();
			}
			
			if ( slot_name == "CarBattery" )
				m_BatteryHealth = -1;
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = -1;

			if ( slot_name == "SparkPlug" )
				m_PlugHealth = -1;
			
			if ( slot_name == "GlowPlug" )
				m_PlugHealth = -1;

			if ( slot_name == "CarRadiator" )
			{
				LeakAll( CarFluid.COOLANT );
				SetHealth( "Radiator", "Health", 0);
			}

			Synchronize();
		}
		
		UpdateHeadlightState();
		UpdateLights();

		ExpansionWheelBase wheel;
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
				return;
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
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].SetPlayer(DayZPlayerImplement.Cast(player));

		if (GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			ScriptRPC rpc = new ScriptRPC();
			m_Crew[posIdx].OnSend(rpc);
			rpc.Send(this, ExpansionVehicleRPC.CrewSync, true, null);
		}
	}

	//! Performs transfer of player from vehicle into world from given position.
	Human CrewGetOut( int posIdx )
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return null;

		Human human = m_Crew[posIdx].GetPlayer();

		m_Crew[posIdx].SetPlayer(null);

		if (GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			ScriptRPC rpc = new ScriptRPC();
			m_Crew[posIdx].OnSend(rpc);
			rpc.Send(this, ExpansionVehicleRPC.CrewSync, true, null);
		}

		return human;
	}
	
	void CrewEntry( int posIdx, out vector pos, out vector dir )
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetIn(pos, dir);
	}

	void CrewEntryWS( int posIdx, out vector pos, out vector dir )
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetIn( pos, dir );

		pos = ModelToWorld(pos);
		dir = vector.Direction(GetPosition(), ModelToWorld(dir));
	}
	
	void CrewTransform( int posIdx, out vector mat[4] )
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetTransform(mat);
	}

	void CrewTransformWS( int posIdx, out vector mat[4] )
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetTransform(mat);

		mat[3] = ModelToWorld(mat[3]);
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
		string conPointName = GetDoorConditionPointFromSelection(pSeatSelection);
		if (conPointName.Length() > 0)
		{
			if( MemoryPointExists(conPointName) )
			{
				vector conPointMS = GetMemoryPointPos(conPointName);
				vector conPointWS = ModelToWorld(conPointMS);
				
				//! skip the height for now
				conPointWS[1] = 0;
				pFromPos[1] = 0;
				
				if (vector.Distance(pFromPos, conPointWS) <= pDistance)
				{
					return true;
				}
			}
		}

		return false;		
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

	void Synchronize()
	{

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
		AddAction( ExpansionActionFillFuel );
		AddAction( ExpansionActionSwitchLights );

		AddAction( ExpansionActionGetInExpansionVehicle );
		AddAction( ExpansionActionOpenVehicleDoor );
		AddAction( ExpansionActionCloseVehicleDoor );
	}

////////////////////////////////////////////////////////////////////
// CAR/CARSCRIPT CODE
////////////////////////////////////////////////////////////////////

	vector GetEnginePosWS()
	{
		return ModelToWorld( m_DrownEnginePos );
	}
	
	vector GetCoolantPtcPosWS()
	{
		return ModelToWorld( m_coolantPtcPos );
	}

	vector GetRefillPointPosWS()
	{
		return ModelToWorld( m_fuelPos );
	}
	
	vector GetEnginePointPosWS()
	{	
		return ModelToWorld( m_enginePos );
	}

	vector GetFrontPointPosWS()
	{	
		return ModelToWorld( m_frontPos );
	}

	vector GetBackPointPosWS()
	{	
		return ModelToWorld( m_backPos );
	}

	vector Get_1_1PointPosWS()
	{	
		return ModelToWorld( m_side_1_1Pos );
	}

	vector Get_1_2PointPosWS()
	{	
		return ModelToWorld( m_side_1_2Pos );
	}

	vector Get_2_1PointPosWS()
	{	
		return ModelToWorld( m_side_2_1Pos );
	}

	vector Get_2_2PointPosWS()
	{	
		return ModelToWorld( m_side_2_2Pos );
	}

	void OnBrakesPressed()
	{
		UpdateLights();
	}
	
	void OnBrakesReleased()
	{
		UpdateLights();
	}

	void OnUpdate( float dt )
    {
/*
//-----------------------------------------------------/
//------PREARATION FOR DROWNING PLAYRS IN THE CARS-----/
//-----------------------------------------------------/
        int crewCnt = CrewSize();
        if ( crewCnt > 0 )
        {
            for( int i = 0; i < CrewSize(); i++ )
            {
                Human crew = CrewMember( i );
                if ( !crew )
                    continue;
                
                int boneIdx = crew.GetBoneIndexByName("Head");
                if( boneIdx != -1 )
                {
                    vector pos1 = crew.GetBonePositionLS( boneIdx );
                    vector pos2 = crew.GetBonePositionMS( boneIdx );
                    vector pos3 = crew.GetBonePositionWS( boneIdx );

                    if ( GetGame().GetWaterDepth( pos1 ) > 0 )
                        Debug.DrawSphere(pos1, 0.03, COLOR_RED);
                    
					if ( GetGame().GetWaterDepth( pos2 ) > 0 )
                        Debug.DrawSphere(pos2, 0.03, COLOR_GREEN);
                    
                    if ( GetGame().GetWaterDepth( pos3 ) > 0 )
                        Debug.DrawSphere(pos3, 0.03, COLOR_YELLOW, ShapeFlags.NOZBUFFER | ShapeFlags.NOZWRITE);

                }
            }
        }
*/
		
		if ( GetGame().IsServer() )
		{
			if ( GetGame().GetWaterDepth( GetEnginePosWS() ) > 0 )
			{
				m_DrownTime += dt;
				if ( m_DrownTime > DROWN_ENGINE_THRESHOLD )
				{
					// *dt to get damage per second
					AddHealth( "Engine", "Health", -DROWN_ENGINE_DAMAGE * dt);
				}
				//Debug.DrawSphere(GetCoolantPtcPosWS(), 0.2, COLOR_RED, ShapeFlags.NOZBUFFER | ShapeFlags.NOZWRITE);
			}
			else
			{
				m_DrownTime = 0;
			}
		}
		
		//FX only on Client and in Single
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			if ( IsDamageDestroyed() && GetGame().GetWaterDepth( GetEnginePosWS() ) <= 0 )
			{
				if ( !SEffectManager.IsEffectExist( m_enginePtcFx ) )
				{
					m_engineFx = new EffEngineSmoke();
					m_enginePtcFx = SEffectManager.PlayOnObject( m_engineFx, this, m_enginePtcPos, Vector(0,0,0), true );
					//m_engineFx.SetParticleStateLight();
					m_engineFx.SetParticleStateHeavy();
				}
			}
		}
		
		// Visualisation of brake lights for all players
		float brake_coef = GetBrake();
		
		if ( brake_coef > 0 )
		{
			if ( !m_BrakesArePressed )
			{
				m_BrakesArePressed = true;
				SetSynchDirty();
				OnBrakesPressed();
			}
		}
		else
		{
			if ( m_BrakesArePressed )
			{
				m_BrakesArePressed = false;
				SetSynchDirty();
				OnBrakesReleased();
			}
		}
    }

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
				amount = m_FuelAmmount;
				break;
			case CarFluid.OIL:
				amount = m_CoolantAmmount;
				break;
			case CarFluid.BRAKE:
				amount = m_OilAmmount;
				break;
			case CarFluid.COOLANT:
				amount = m_BrakeAmmount;
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
				previousAmt = m_FuelAmmount;
				m_FuelAmmount -= amount;
				newAmt = m_FuelAmmount;
				break;
			case CarFluid.OIL:
				previousAmt = m_CoolantAmmount;
				m_CoolantAmmount -= amount;
				newAmt = m_CoolantAmmount;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_OilAmmount;
				m_OilAmmount -= amount;
				newAmt = m_OilAmmount;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_BrakeAmmount;
				m_BrakeAmmount -= amount;
				newAmt = m_BrakeAmmount;
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
				previousAmt = m_FuelAmmount;
				m_FuelAmmount = 0;
				break;
			case CarFluid.OIL:
				previousAmt = m_CoolantAmmount;
				m_CoolantAmmount = 0;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_OilAmmount;
				m_OilAmmount = 0;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_BrakeAmmount;
				m_BrakeAmmount = 0;
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
				previousAmt = m_FuelAmmount;
				m_FuelAmmount += amount;
				newAmt = m_FuelAmmount;
				break;
			case CarFluid.OIL:
				previousAmt = m_CoolantAmmount;
				m_CoolantAmmount += amount;
				newAmt = m_CoolantAmmount;
				break;
			case CarFluid.BRAKE:
				previousAmt = m_OilAmmount;
				m_OilAmmount += amount;
				newAmt = m_OilAmmount;
				break;
			case CarFluid.COOLANT:
				previousAmt = m_BrakeAmmount;
				m_BrakeAmmount += amount;
				newAmt = m_BrakeAmmount;
				break;
			default:
				return;
		}

		OnFluidChanged( fluid, previousAmt, newAmt );
	}
	
	float GetEnviroHeatComfortOverride()
	{
		return Math.Clamp(m_EnviroHeatComfortOverride, -1, 1);
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
	
	//!
	bool IsMoving()
	{
		if ( GetSpeedometer() > 3.5 )
			return true;
		
		return false;
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
		UpdateLights();
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
		UpdateLights();
	}

	//! Returns total number of gears.
	int GetGearsCount()
	{
		return 0;
	}

	int GetGear()
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

	float GetBrake()
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
		UpdateLights(newGear);
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
		if ( zoneName == "" )
		{
			Print("CarScript >> ERROR >> OnContact dmg zone not defined!");
			return;
		}
		
		switch( zoneName )
		{
/*
			case "dmgZone_lightsLF":
				//Print("dmgZone_lightsLF");
			break;
			
			case "dmgZone_lightsRF":
				//Print("dmgZone_lightsRF");
			break;
*/
			default:
				if ( GetGame().IsServer() && zoneName != "")
				{
					float dmgMin = 150.0;	
					float dmgThreshold = 750.0;
					float dmgKillCrew = 3000.0;
					float dmg = data.Impulse * m_dmgContactCoef;

					if ( dmg < dmgThreshold )
					{					
						if ( dmg > dmgMin )
						{
							//Print( GetType() + " >>> " + " smlHit >>> " + "zoneName: "  + zoneName + " >>> - " + dmg.ToString() + " HP >>> in " + GetSpeedometer() + " km/h");
							AddHealth( zoneName, "Health", -dmg);
							//m_PlayCrashSoundLight = true;
							SynchCrashLightSound( true );
						}
					}
					else
					{
						//Print( GetType() + " >>> " + " BIGHit >>> " + "zoneName: " + zoneName + " >>> - " + dmg.ToString() + " HP >>> in " + GetSpeedometer() + " km/h" );
						for( int i =0; i < CrewSize(); i++ )
						{
							Human crew = CrewMember( i );
							if ( !crew )
								continue;

							PlayerBase player;
							if ( Class.CastTo(player, crew ) )
							{
								if ( dmg > dmgKillCrew )
								{		
									player.SetHealth(0.0);
								}
								else
								{
									//deal shock to player
									float shockTemp = Math.InverseLerp(dmgThreshold, dmgKillCrew, dmg);
									float shock = Math.Lerp( 50, 100, shockTemp );
									float hp = Math.Lerp( 2, 60, shockTemp );

									player.AddHealth("", "Shock", -shock );
									player.AddHealth("", "Health", -hp );
									//Print( "SHOCK..........." + shock );
									//Print( "HEALTH..........." + hp );
								}
							}
						}

						//m_PlayCrashSoundHeavy = true;
						SynchCrashHeavySound( true );
						ProcessDirectDamage( DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", dmg );
					}
				}
			break;
		}
		
		
		UpdateHeadlightState();
		UpdateLights();
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
		if ( GetGame().IsServer() )
		{
			if ( GetCrashLightSound() )
				m_PlayCrashSoundLight = false;
		
			if ( GetCrashHeavySound() )
				m_PlayCrashSoundHeavy = false;
		}
		
		switch ( ctrl )
		{
			// in this controller the higher returned value the more muted sounds inside the car
			case CarSoundCtrl.DOORS:
				break;
		}

		// if you don't wanna change the behaviour, just return the old value.
		return oldValue;
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel,newLevel,zone);
		
		if ( newLevel ==  GameConstants.STATE_RUINED )
		{
			EffectSound sound_plug;
			switch( zone )
			{
				case "WindowLR":
				case "WindowRR":
					PlaySoundSet( sound_plug, "offroad_hit_window_small_SoundSet", 0, 0 );
				break;
				
				case "WindowFront":
				case "WindowBack":
				case "WindowFrontLeft":
				case "WindowFrontRight":
					PlaySoundSet( sound_plug, "offroad_hit_window_large_SoundSet", 0, 0 );
				break;
			}
		}
	}

	void GetSoundControls( inout array< string > names )
	{
		names.Insert("rpm");
		names.Insert("engineOn");
		names.Insert("campos");
		names.Insert("doors");
		names.Insert("speed");
		names.Insert("thrust");
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
		ExpansionVehicleBase evs;

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
			ExpansionVehicleBase evs;

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
		ExpansionVehicleBase evs;
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
	int GetPersistentIDA()
	{
		return m_PersistentIDA;
	}

	// ------------------------------------------------------------
	int GetPersistentIDB()
	{
		return m_PersistentIDB;
	}

	// ------------------------------------------------------------
	int GetPersistentIDC()
	{
		return m_PersistentIDC;
	}

	// ------------------------------------------------------------
	int GetPersistentIDD()
	{
		return m_PersistentIDD;
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
	// Expansion ResetKeyPairing
	// ------------------------------------------------------------
	// Only call this after all keys have been confirmed to be removed
	void ResetKeyPairing()
	{
		m_PersistentIDA = 0;
		m_PersistentIDB = 0;
		m_PersistentIDC = 0;
		m_PersistentIDD = 0;

		if ( IsMissionHost() )
		{
			GetPersistentID( m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD );
		}

		m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

		SetSynchDirty();
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

	void MotorStart()
	{
	}

	void MotorStop()
	{
	}

	bool MotorIsOn()
	{
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

		if ( ( EngineIsOn() || MotorIsOn() ) && IsMoving() )
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			auto ehcv = player.GetCommand_ExpansionVehicle();
			if (!ehcv || ehcv.GetTransport() == this)
				return true;
		}
	
		if (GetExpansionSettings() && GetExpansionSettings().GetVehicle().VehicleLockedAllowInventoryAccess)
			return true;

		if (GetExpansionSettings().GetVehicle().VehicleLockedAllowInventoryAccessWithoutDoors && !AllDoorsClosed())
			return true;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	bool HasGear()
	{
		return false;
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

		float maxVolume = GetFluidCapacity( CarFluid.FUEL );
		float amount = Math.RandomFloat(0.0, maxVolume * 0.35 );

		Fill( CarFluid.FUEL, amount );

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
	string GetActionCompNameCoolant()
	{
		return "radiator";
	}

	// ------------------------------------------------------------
	float GetActionDistanceCoolant()
	{
		return 2.0;
	}

	// ------------------------------------------------------------
	string GetActionCompNameFuel()
	{
		return "refill";
	}

	// ------------------------------------------------------------
	float GetActionDistanceFuel()
	{
		return 1.5;
	}
	
	// ------------------------------------------------------------
	string GetActionCompNameOil()
	{
		//return "refill";
		return "carradiator";
	}

	// ------------------------------------------------------------
	float GetActionDistanceOil()
	{
		return 2.0;
	}
	
	// ------------------------------------------------------------
	string GetActionCompNameBrakes()
	{
		return "carradiator";
	}

	// ------------------------------------------------------------
	float GetActionDistanceBrakes()
	{
		return 2.0;
	}

	// ------------------------------------------------------------
	bool IsVitalAircraftBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalHelicopterBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalIgniterPlug()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalHydraulicHoses()
	{
		return false;
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
			
				if ( EngineIsOn() || MotorIsOn() )
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

	protected void CheckVitalItem( bool isVital, string itemName )
	{
		if ( !isVital )
			return;

		EntityAI item = FindAttachmentBySlotName(itemName);

		if ( !item )
			EngineStop();
		else if ( item.IsRuined() )
			EngineStop();
	}

	protected void LeakFluid(CarFluid fluid)
	{
		float ammount = 0;
		
		switch (fluid)
		{
			case CarFluid.COOLANT:
				ammount = (1- m_RadiatorHealth) * Math.RandomFloat(0.02, 0.05);//CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
				Leak(fluid, ammount);
			break;
			
			case CarFluid.OIL:
				ammount =  ( 1 - m_EngineHealth ) * Math.RandomFloat(0.02, 1.0);//CARS_LEAK_OIL_MIN; CARS_LEAK_OIL_MAX
				Leak(fluid, ammount);
			break;
			
			case CarFluid.FUEL:
				ammount = ( 1 - m_FuelTankHealth ) * Math.RandomFloat(0.02, 0.05);//CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
				Leak(fluid, ammount);
			break;
		}
	}

	protected void CarPartsHealthCheck()
	{
		if ( GetGame().IsServer() )
		{
			ItemBase radiator;
			Class.CastTo( radiator, FindAttachmentBySlotName("CarRadiator"));
			if ( radiator )
			{
				m_RadiatorHealth = radiator.GetHealth01("", "");
			}
			else
			{
				m_RadiatorHealth = 0;
			}
			
			m_EngineHealth = GetHealth01("Engine", "");

			m_FuelTankHealth = GetHealth01("FuelTank", "");
		}
	}
	
	bool GetCrashLightSound()
	{
		return m_PlayCrashSoundLight;
	}
	
	void SynchCrashLightSound( bool play )
	{
		m_PlayCrashSoundLight = play;
		SetSynchDirty();
	}

	void PlayCrashLightSound()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{		
			EffectSound sound =	SEffectManager.PlaySound("offroad_hit_light_SoundSet", GetPosition() );
			sound.SetSoundAutodestroy( true );
			m_PlayCrashSoundLight = false;
		}
	}

	bool GetCrashHeavySound()
	{
		return m_PlayCrashSoundHeavy;
	}

	void SynchCrashHeavySound( bool play )
	{
		m_PlayCrashSoundHeavy = play;
		SetSynchDirty();
	}
	
	void PlayCrashHeavySound()
	{
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{		
			EffectSound sound =	SEffectManager.PlaySound("offroad_hit_heavy_SoundSet", GetPosition() );
			sound.SetSoundAutodestroy( true );
			m_PlayCrashSoundHeavy = false;
		}
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
	float GetCameraHeight()
	{
		return 1.3;
	}

	// ------------------------------------------------------------
	float GetCameraDistance()
	{
		return 4.5;
	}
	
	// ------------------------------------------------------------
	float GetTransportCameraDistance()
	{
		return GetCameraDistance();
	}
	
	// ------------------------------------------------------------
	vector GetTransportCameraOffset()
	{
		return Vector( 0, GetCameraHeight(), 0 );
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
	string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUniversalWheel"; //this should never happen
	}

	// ------------------------------------------------------------
	ExpansionVehicleController GetControllerInstance()
	{
		return NULL;
	} 
};