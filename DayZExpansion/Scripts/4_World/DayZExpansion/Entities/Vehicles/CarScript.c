/*
 * ExpansionCarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleLockState
{
	NOLOCK = 0,
	UNLOCKED, 
	READY_TO_LOCK,
	LOCKED,
	//Use count here, for network optimization to know max 
	COUNT
}

class ExpansionVehicleAttachmentSave
{
	vector m_Position;
	vector m_Orientation;

	void ExpansionVehicleAttachmentSave( vector position, vector orientation )
	{
		m_Position = position;
		m_Orientation = orientation;
	}
}

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	private static ref array< ExpansionVehicleAttachmentSave > m_allAttachments = new array< ExpansionVehicleAttachmentSave >;
	private static ref set< CarScript > m_allVehicles = new set< CarScript >;

	// ------------------------------------------------------------
	//! Constant Values - Set in Constructor, Errors occur if not.
	// ------------------------------------------------------------
	protected float m_MaxSpeed; // (km/h)
	protected float m_MaxSpeedMS; // (m/s)

	protected float m_AltitudeFullForce; // (m)
	protected float m_AltitudeNoForce; // (m)

	// ------------------------------------------------------------
	//! Member values
	// ------------------------------------------------------------
	private int m_ExpansionSaveVersion;

	// Horn
	protected EffectSound m_HornSound;

	protected string m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
	protected string m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";

	protected bool m_HornPlaying;
	protected bool m_HornSynchRemote;

	// Vehicle locking
	protected ExpansionVehicleLockState m_VehicleLockedState;

	protected int m_PersistentIDA = 0;
	protected int m_PersistentIDB = 0;
	protected int m_PersistentIDC = 0;
	protected int m_PersistentIDD = 0;

	// Explosion
	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;
	protected int m_ExplosionSize;

	//Timeslice for Engine damage via rev or fluids
	protected float m_FluidCheckTime;
	
	// Safezone
	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;

	// Winching
	protected CarScript m_WinchedObject;
	protected dJoint m_WinchedJoint;
	protected bool m_IsWinched;

	// Towing
	protected vector m_TowPointCenter;
	protected bool m_IsBeingTowed;
	protected bool m_IsTowing;

	protected CarScript m_ParentTow;
	protected int m_ParentTowNetworkIDLow;
	protected int m_ParentTowNetworkIDHigh;
	protected int m_ParentTowPersistentIDA;
	protected int m_ParentTowPersistentIDB;
	protected int m_ParentTowPersistentIDC;
	protected int m_ParentTowPersistentIDD;

	protected CarScript m_ChildTow;
	protected int m_ChildTowNetworkIDLow;
	protected int m_ChildTowNetworkIDHigh;
	protected int m_ChildTowPersistentIDA;
	protected int m_ChildTowPersistentIDB;
	protected int m_ChildTowPersistentIDC;
	protected int m_ChildTowPersistentIDD;

	// Physics
	protected float m_BodyMass;
	protected vector m_BodyCenterOfMass;

	protected vector m_LinearVelocity; // World Space
	protected vector m_LinearVelocityMS; // Model Space

	protected vector m_LinearAcceleration; // World Space
	protected vector m_LinearAccelerationMS; // Model Space

	protected vector m_LastLinearVelocity; // World Space
	protected vector m_LastLinearVelocityMS; // Model Space

	protected vector m_AngularVelocity;

	protected vector m_LastAngularVelocity;

	protected bool m_IsPhysicsHost;
	
	protected float m_BoundingRadius;
	protected vector m_BoundingBox[2];

	protected ref Transform m_Transform;
	
	// Floating
	protected float m_WaterVolume;
	protected float m_TotalVolume;

	// Controllers
	protected ref ExpansionController m_Controller;
	protected bool m_HasDriver;

	// Effects
	protected float m_AltitudeLimiter;
	protected bool m_CarBatteryVanillaState;
	protected bool m_CarBatteryVanillaStateDefault;

	protected Particle m_SmokeParticle;

	// Skinning
	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ExpansionSkin > m_Skins;

	// Lights
	ref array< ref ExpansionPointLight > m_Lights;
	ref array< ref Particle > m_Particles;

	protected ExpansionMarkerModule m_MarkerModule;
	protected string m_ServerMarker;
	protected static int m_ServerMarkerIndex = 0;

	protected vector m_Orientation;
	protected vector m_Position;
	
	protected autoptr TStringArray m_Doors;
	protected bool m_CanHaveLock;

	protected bool m_MonitorEnabled;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
		#endif

		SetEventMask( EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT );

		m_allVehicles.Insert( this );

		m_SafeZone = false;

		m_Lights = new array< ref ExpansionPointLight >;
		m_Particles = new array< ref Particle >;
		
		Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) );

		m_Controller = new ExpansionCarController( this );

		m_Transform = new Transform;
		
		m_Doors = new TStringArray;
		ConfigGetTextArray( "doors", m_Doors );

		m_CanHaveLock = m_Doors.Count() > 0;

		LoadConstantVariables();

		ExpansionSetupSkins();

		GetPersistentID( m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD );

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );

		RegisterNetSyncVariableBool( "m_IsBeingTowed" );
		RegisterNetSyncVariableBool( "m_IsTowing" );
		RegisterNetSyncVariableInt( "m_ParentTowNetworkIDLow" );
		RegisterNetSyncVariableInt( "m_ParentTowNetworkIDHigh" );
		RegisterNetSyncVariableInt( "m_ChildTowNetworkIDLow" );
		RegisterNetSyncVariableInt( "m_ChildTowNetworkIDHigh" );

		RegisterNetSyncVariableBool( "m_HornSynchRemote" );
		RegisterNetSyncVariableBool( "m_ExplodedSynchRemote" ); 
		RegisterNetSyncVariableBool( "m_SafeZoneSynchRemote" ); 
		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote", 0, m_Skins.Count() );
		
		RegisterNetSyncVariableInt( "m_VehicleLockedState", ExpansionVehicleLockState.NOLOCK, ExpansionVehicleLockState.COUNT );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredInit );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );

		m_MarkerModule = ExpansionMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMarkerModule ) );
		m_ServerMarker = "";

		ExpansionSettings.SI_Vehicle.Insert( OnSettingsUpdated );
		OnSettingsUpdated();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - End");
		#endif
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
	// Destructor
	// ------------------------------------------------------------
	void ~CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - Start");
		#endif

		int idx = m_allVehicles.Find( this );
		if ( idx >= 0 )
		{
			m_allVehicles.Remove( idx );
		}

		if ( IsMissionClient( ) )
		{
			if ( m_SmokeParticle )
			{
				m_SmokeParticle.Stop( );
			}

			int i;

			for ( i = 0; i < m_Lights.Count(); i++ )
			{
				m_Lights[i].ExpansionSetEnabled( false );

				GetGame().ObjectDelete( m_Lights[i] );
			}

			for ( i = 0; i < m_Particles.Count(); i++ )
			{
				m_Particles[i].Stop( );

				GetGame().ObjectDelete( m_Particles[i] );
			}
		}

		ExpansionSettings.SI_Vehicle.Remove( OnSettingsUpdated );

		//RemoveServerMarker();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - End");
		#endif
	}

	void LoadConstantVariables()
	{
	}

	void OnAfterLoadConstantVariables()
	{
	}

	void LongDeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::LongDeferredInit - Start");
		#endif

		if ( m_SkinModule )
			m_SkinModule.PerformCESkinSwap( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::LongDeferredInit - End");
		#endif
	}

	void DeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DeferredInit - Start");
		#endif

		m_BoundingRadius = ClippingInfo( m_BoundingBox );

		m_MaxSpeedMS = m_MaxSpeed * ( 1.0 / 3.6 );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnAfterLoadConstantVariables, 100, false );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DeferredInit - End");
		#endif
	}

	void OnSettingsUpdated()
	{

	}

	static set< CarScript > GetAll()
	{
		return m_allVehicles;
	}

	ref ExpansionController GetExpansionController()
	{
		return m_Controller;
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetActions - Start");
		#endif

		super.SetActions();

		//RemoveAction( ActionGetInTransport );

		//AddAction( ExpansionActionGetInTransport );

		AddAction( ExpansionActionPairKey );
		AddAction( ExpansionActionAdminUnpairKey );

		AddAction( ExpansionActionLockVehicle );
		AddAction( ExpansionActionUnlockVehicle );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetActions - End");
		#endif
	}

#ifdef EXPANSION_VEHICLE_TOWING
	void CreateTow( CarScript tow )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CreateTow - Start");
		#endif

		if ( tow.m_IsBeingTowed || !IsMissionHost() )
			return;

		m_ChildTow = tow;
		m_IsTowing = true;

		m_ChildTow.m_ParentTow = this;
		m_ChildTow.m_IsBeingTowed = true;

		m_TowPointCenter = GetTowCenterPosition( m_ChildTow );
		
		if ( !IsMissionOffline() )
		{
			m_ChildTow.GetNetworkID( m_ChildTowNetworkIDLow, m_ChildTowNetworkIDHigh );
			GetNetworkID( m_ChildTow.m_ParentTowNetworkIDLow, m_ChildTow.m_ParentTowNetworkIDHigh );
			
			m_ChildTow.SetSynchDirty();
			SetSynchDirty();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CreateTow - End");
		#endif
	}

	void DestroyTow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DestroyTow - Start");
		#endif

		if ( m_IsTowing )
		{
			m_ChildTow.m_IsBeingTowed = false;
			m_ChildTow = NULL;

			m_IsTowing = false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::DestroyTow - End");
		#endif
	}
#endif

	vector GetTowCenterPosition( CarScript other )
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

	bool CanConnectTow( CarScript other )
	{
		//! is it a car ? Is it already towing something ? And is it locked ?
		return other.IsCar() && !other.IsTowing() && !other.IsLocked();
	}

	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	protected void KeyMessage( string message )
	{
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );
		}	
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( message );
			#endif
		}
	}

	// ------------------------------------------------------------
	// Expansion IsLocked
	// ------------------------------------------------------------
	bool IsLocked()
	{
		if ( HasKey() )
		{
			//KeyMessage("CarScript::IsLocked HasKey() true and " + (m_VehicleLockedState == ExpansionVehicleLockState.LOCKED));
			return m_VehicleLockedState == ExpansionVehicleLockState.LOCKED;
		}

		//KeyMessage("CarScript::IsLocked false");
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
	// Expansion HasKey
	// ------------------------------------------------------------
	bool HasKey()
	{
		return m_VehicleLockedState != ExpansionVehicleLockState.NOLOCK;
	}

	// ------------------------------------------------------------
	// Expansion PairKeyTo
	// ------------------------------------------------------------
	void PairKeyTo( ExpansionCarKey key )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("CarScript::PairKeyTo - Start");
		#endif

		if ( key )
		{
			key.PairToVehicle( this );

			m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;

			SetSynchDirty();
		}

		//KeyMessage( "PairKeyTo (" + this + ", " + key + ")" );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("CarScript::PairKeyTo - End");
		#endif
	}
	
	// Only call this after all keys have been confirmed to be removed
	void ResetKeyPairing()
	{
		m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

		SetSynchDirty();
	}

	bool CanBeLocked()
	{
		return true;
	}

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

	void OnCarDoorClosed( string source )
	{

	}

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

	void LockCar( ExpansionCarKey key )
	{
		if ( key && !IsCarKeys( key ) && !key.IsInherited(ExpansionCarAdminKey) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.READY_TO_LOCK;
		//KeyMessage( "LockCar::READY_TO_LOCK" );

		SetSynchDirty();
	}

	void UnlockCar( ExpansionCarKey key )
	{
		if ( key && !IsCarKeys( key ) && !key.IsInherited(ExpansionCarAdminKey) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
		//KeyMessage( "UnlockCar::UNLOCKED" );

		OnCarUnlocked();
		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// Expansion OnCarLocked
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
	// Expansion OnCarUnlocked
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
	
	// ------------------------------------------------------------
	// Expansion OnHonkSoundPlay
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
	// Expansion OnHonkSoundStop
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
	// Expansion PlayHonk
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
	// Expansion StopHonk
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
	// Expansion IsSoundSynchRemote
	// ------------------------------------------------------------
	bool IsSoundSynchRemote()
	{	
		return m_HornSynchRemote;
	}

	// ------------------------------------------------------------
	// Expansion Explode
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
	// Expansion ExpansionOnExplodeServer
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

			if ( GetGame().IsMultiplayer() )
			{
				crew.SetHealth( 0.0 );
			}
		}

		dBodyApplyForce( this, Vector( 0, 20.0 * m_BodyMass, 0 ) );

		ExpansionCreateExplosion( this, "", 2.5, 100 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionOnExplodeClient
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

		vector n = "0 1 0";

		ExpansionPointLight explosionLight = ExpansionPointLight.Cast( ScriptedLightBase.CreateLight( ExpansionPointLight, Vector( 0, 0, 0 ) )  ); 

		explosionLight.SetDiffuseColor( 1, 0.25, 0 );
		explosionLight.SetPosition( GetPosition() );
		explosionLight.SetRadiusTo( 20 * m_ExplosionSize );
		explosionLight.SetBrightnessTo( 4.5 ); 
		explosionLight.SetLifetime( 0.1 );
		explosionLight.SetFadeOutTime( 0.1 );
		explosionLight.SetFlareVisible( false );

		vector localPos = GetMemoryPointPos( "velka osa" );
		Particle p;

		if ( IsHelicopter() || IsPlane() || IsBoat() )
		{
			int z;

			for ( z = 0; z < m_Particles.Count(); z++ )
			{
				m_Particles[z].Stop( );

				GetGame().ObjectDelete( m_Particles[z] );
			}

			for ( z = 0; z < m_Lights.Count(); z++ )
			{
				m_Lights[z].ExpansionSetEnabled( false );

			   	GetGame().ObjectDelete( m_Lights[z] );
			}
			
			if ( GetGame().GetWaterDepth( GetPosition() ) < 1 )
			{
				if ( !m_SmokeParticle )
				{
					m_SmokeParticle = Particle.PlayOnObject( ParticleList.EXPANSION_FIRE_HELICOPTER, this, GetMemoryPointPos( "velka osa" ), "0 0 0", true );
					m_SmokeParticle.SetOrientation( Vector(0, 1, 0).VectorToAngles() );
				}
			}

			if ( IsSurfaceWater( GetPosition() ) )
			{
				p = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_WATER, ModelToWorld( localPos ) );
				p.SetOrientation( n );
			} else 
			{
				p = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_HELICOPTER, ModelToWorld( localPos ) );
				p.SetOrientation( n );
			}
		} else if ( IsBoat() )
		{
			p = Particle.PlayInWorld( ParticleList.EXPANSION_EXPLOSION_WATER, ModelToWorld( localPos ) );
			p.SetOrientation( n );
		}

		float distance_to_player = vector.Distance( GetPosition(), GetGame().GetPlayer().GetPosition() );

		float strength_factor = Math.InverseLerp( GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player );

		// GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake( strength_factor * 16.0 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeClient - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionOnSpawnExploded
	// The entity was already exploded but the client never knew about it (i.e. outside of network bubble)
	// ------------------------------------------------------------	
	void ExpansionOnSpawnExploded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSpawnExploded - Start");
		#endif

		m_Exploded = true;

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSpawnExploded - End");
		#endif
	}

	// ------------------------------------------------------------
	// DayZ OnExplodeClient
	// Does nothing, vanilla code shall not run
	// ------------------------------------------------------------	
	override void OnExplodeClient()
	{
	}

	// ------------------------------------------------------------
	// DayZ SynchExplosion
	// Does nothing, vanilla code shall not run
	// ------------------------------------------------------------	
	override void SynchExplosion()
	{
	}

	// ------------------------------------------------------------
	// DayZ OnRPC
	// ------------------------------------------------------------	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnRPC - Start");
		#endif

		switch ( rpc_type )
		{
			case ExpansionVehicleRPC.ControllerSync:
			{
				if ( m_Controller )
				{
					PlayerBase driverBase;
					if ( Class.CastTo( driverBase, CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) ) && driverBase.GetIdentityUID() == sender.GetId() )
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
		}
		
		super.OnRPC( sender, rpc_type, ctx );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnRPC - End");
		#endif
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
				rpc.Send( this, ExpansionVehicleRPC.ControllerSync, true, human.GetIdentity() );
		} else
		{
			rpc.Send( this, ExpansionVehicleRPC.ControllerSync, true, NULL );
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
	void StopSounds()
	{
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

	override EntityAI FindAttachmentBySlotName( string slot_name )
	{
		//! Print( "CarScript::FindAttachmentBySlotName - Start" );
		//! Print( m_CarBatteryVanillaState );
		//! Print( slot_name );

		EntityAI item = NULL;
		if ( !m_CarBatteryVanillaStateDefault && m_CarBatteryVanillaState && slot_name == "CarBattery" )
		{
			//! Print( IsVitalHelicopterBattery() );
			//! Print( IsVitalAircraftBattery() );
			if ( IsVitalHelicopterBattery() )
				item = super.FindAttachmentBySlotName( "ExpansionHelicopterBattery" );
			if ( IsVitalAircraftBattery() )
				item = super.FindAttachmentBySlotName( "ExpansionAircraftBattery" );
		} else
		{
			item = super.FindAttachmentBySlotName( slot_name );
		}

		//! Print( item );

		return item;
	}

	// ------------------------------------------------------------
	void SetCarBatteryStateForVanilla( bool active )
	{
		if ( !m_CarBatteryVanillaState )
		{
			m_CarBatteryVanillaStateDefault = IsVitalCarBattery();
		}

		m_CarBatteryVanillaState = active;
	}

	// ------------------------------------------------------------
	bool IsVitalHelicopterBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsVitalAircraftBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsCar()
	{
		return true;
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
	bool IsPlane()
	{
		return false;
	}

	// ------------------------------------------------------------
	bool IsExploded()
	{
		return m_Exploded;
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
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnEnterSafeZone - Start");
		#endif

		m_SafeZone = true;

		/*if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = true;

			SetAllowDamage( false );

			SetSynchDirty();
		}

		if ( IsMissionClient() )
		{
			
		}
		*/

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnEnterSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnLeftSafeZone - Start");
		#endif
		
		m_SafeZone = false;

		/*if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = false;

			SetAllowDamage( true );
			
			SetSynchDirty();
		}

		if ( IsMissionClient() )
		{
			
		}*/

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnLeftSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------	
	override bool OnBeforeSwitchLights( bool toOn )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeSwitchLights - Start");
		#endif

		SetCarBatteryStateForVanilla( true );

		bool ret = super.OnBeforeSwitchLights( toOn );

		SetCarBatteryStateForVanilla( false );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeSwitchLights - End");
		#endif

		return ret;
	}

	// ------------------------------------------------------------	
	override bool OnBeforeEngineStart()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeEngineStart - Start");
		#endif
		
		if ( m_Exploded )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("CarScript::OnBeforeEngineStart - End - Exploded");
			#endif
			return false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeEngineStart - End");
		#endif
		
		return super.OnBeforeEngineStart();
	}

	// ------------------------------------------------------------	
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemAttached - Start");
		#endif

		if ( m_IsPhysicsHost )
		{
			string slot = slot_name;
			slot.ToLower();
			if ( slot.Contains( "wheel" ) )
			{
				vector force = Vector( 0, m_BodyMass * 50.0, 0 );

				string selection = "";
				InventorySlots.GetSelectionForSlotId( InventorySlots.GetSlotIdFromString( slot_name ), selection );
				vector position = GetSelectionPositionMS( selection );

				//dBodyApplyForceAt( this, ModelToWorld( position ), force );
			}
		}

		if ( IsMissionHost() )
		{
			if ( slot_name == "Reflector_1_1" )
				SetHealth( "Reflector_1_1", "Health", item.GetHealth() );

			if ( slot_name == "Reflector_2_1" )
				SetHealth( "Reflector_2_1", "Health", item.GetHealth() );

			if ( slot_name == "CarBattery" )
				m_BatteryHealth = item.GetHealth01();
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = item.GetHealth01();

			if ( slot_name == "ExpansionHelicopterBattery" )
				m_BatteryHealth = item.GetHealth01();

			if ( slot_name == "ExpansionAircraftBattery" )
				m_BatteryHealth = item.GetHealth01();
			
			if ( slot_name == "SparkPlug" )
				m_PlugHealth = item.GetHealth01();
			
			if ( slot_name == "GlowPlug" )
				m_PlugHealth = item.GetHealth01();

			Synchronize();
		}

		UpdateHeadlightState();
		UpdateLights();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemAttached - End");
		#endif
	}
	
	// ------------------------------------------------------------	
	override void EEItemDetached( EntityAI item, string slot_name )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemDetached - Start");
		#endif

		if ( m_IsPhysicsHost )
		{
			string slot = slot_name;
			slot.ToLower();
			if ( slot.Contains( "wheel" ) )
			{
				vector force = Vector( 0, m_BodyMass * 50.0, 0 );

				string selection = "";
				InventorySlots.GetSelectionForSlotId( InventorySlots.GetSlotIdFromString( slot_name ), selection );
				vector position = GetSelectionPositionMS( selection );

				//dBodyApplyForceAt( this, ModelToWorld( position ), force );
			}
		}
			
		if ( IsMissionHost() )
		{
			if ( IsScriptedLightsOn() )
			{
				if ( slot_name == "CarBattery" || slot_name == "TruckBattery" || slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery" )
					ToggleHeadlights();

				if ( slot_name == "Reflector_1_1" )
				{
					SetHealth( "Reflector_1_1", "Health", 0 );
				
					item = FindAttachmentBySlotName( "Reflector_2_1" );
					if ( !item || GetHealth01("Reflector_2_1", "") <= 0 )
						ToggleHeadlights();
				}

				if ( slot_name == "Reflector_2_1" )
				{
					SetHealth( "Reflector_2_1", "Health", 0 );

					item = FindAttachmentBySlotName( "Reflector_1_1" );
					if ( !item || GetHealth01("Reflector_1_1", "") <= 0 )
						ToggleHeadlights();
				}
			}

			if ( EngineIsOn() )
			{
				if ( slot_name == "GlowPlug" || slot_name == "SparkPlug" || slot_name == "CarBattery" || slot_name == "TruckBattery" || slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery" )
					EngineStop();
			}
			
			if ( slot_name == "CarBattery" )
				m_BatteryHealth = -1;
					
			if ( slot_name == "TruckBattery" )
				m_BatteryHealth = -1;
			
			if ( slot_name == "ExpansionHelicopterBattery" )
				m_BatteryHealth = -1;

			if ( slot_name == "ExpansionAircraftBattery" )
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemDetached - End");
		#endif
	}

	// ------------------------------------------------------------	
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
	bool CanUpdateHealth( float pDt )
	{
		m_FluidCheckTime += pDt;

		return m_FluidCheckTime >= GameConstants.CARS_FLUIDS_TICK;
	}

	// ------------------------------------------------------------	
	void UpdateHealth( float pDt )
	{
		m_FluidCheckTime = 0;

		CarPartsHealthCheck();

		if ( IsMissionHost() && EngineIsOn() )
		{
			if ( GetFluidFraction( CarFluid.FUEL ) <= 0 || m_EngineHealth <= 0 )
				EngineStop();

			CheckVitalItem( IsVitalCarBattery(), "CarBattery" );
			CheckVitalItem( IsVitalTruckBattery(), "TruckBattery" );
			CheckVitalItem( IsVitalSparkPlug(), "SparkPlug" );
			CheckVitalItem( IsVitalGlowPlug(), "GlowPlug" );
			CheckVitalItem( IsVitalHelicopterBattery(), "ExpansionHelicopterBattery" );
			CheckVitalItem( IsVitalAircraftBattery(), "ExpansionAircraftBattery" );
		}

		if ( EngineIsOn() )
		{
			if ( IsMissionHost() )
			{
				float dmg;
				
				if ( EngineGetRPM() >= EngineGetRPMRedline() )
				{
					if ( EngineGetRPM() > EngineGetRPMMax() )
						AddHealth( "Engine", "Health", -GetMaxHealth("Engine", ""));
						
					dmg = EngineGetRPM() * 0.001 * Math.RandomFloat( 0.02, 1.0 ); 
				
					ProcessDirectDamage( 3, null, "Engine", "EnviroDmg", "0 0 0", dmg );
				}

				if ( IsVitalRadiator() )
				{
					if ( GetFluidFraction(CarFluid.COOLANT) > 0 && m_RadiatorHealth < 0.5 ) 
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

				if ( IsVitalRadiator() )
				{
					if ( GetFluidFraction( CarFluid.COOLANT ) < 0.5 && GetFluidFraction( CarFluid.COOLANT ) >= 0 )
					{
						dmg = ( 1 - GetFluidFraction(CarFluid.COOLANT) ) * Math.RandomFloat( 0.02, 10.00 ); 
						AddHealth( "Engine", "Health", -dmg );
					}
				}
			}
			
			if ( IsMissionClient() )
			{
				if ( !SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
				{
					if ( !IsInherited( ExpansionBoatScript ) && !IsInherited( ExpansionHelicopterScript ) )
					{
						m_exhaustFx = new EffExhaustSmoke();
						m_exhaustPtcFx = SEffectManager.PlayOnObject( m_exhaustFx, this, m_exhaustPtcPos, m_exhaustPtcDir );
					}
				}

				if ( m_exhaustFx )
					m_exhaustFx.SetParticleStateLight();
			
				if ( IsVitalRadiator() && SEffectManager.IsEffectExist( m_coolantPtcFx ) )
					SEffectManager.Stop(m_coolantPtcFx);
				
				if ( IsVitalRadiator() && GetFluidFraction( CarFluid.COOLANT ) < 0.5 )
				{
					if ( !IsInherited( ExpansionBoatScript ) && !IsInherited( ExpansionHelicopterScript ) )
					{
						if ( !SEffectManager.IsEffectExist( m_coolantPtcFx ) )
						{
							m_coolantFx = new EffCoolantSteam();
							m_coolantPtcFx = SEffectManager.PlayOnObject( m_coolantFx, this, m_coolantPtcPos );
						}

						if ( GetFluidFraction( CarFluid.COOLANT ) > 0 )
							m_coolantFx.SetParticleStateLight();
						else
							m_coolantFx.SetParticleStateHeavy();
					}
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
			if ( IsMissionClient() )
			{
				if ( SEffectManager.IsEffectExist( m_exhaustPtcFx ) )
					SEffectManager.Stop(m_exhaustPtcFx);
				
				if ( SEffectManager.IsEffectExist( m_coolantPtcFx ) )
					SEffectManager.Stop(m_coolantPtcFx);
			}
		}
	}

	// ------------------------------------------------------------	
	bool CanUpdateHorn( float pDt )
	{
		return m_HornPlaying && IsMissionHost();
	}

	// ------------------------------------------------------------	
	void UpdateHorn( float pDt )
	{
		ref NoiseParams npar = new NoiseParams();
		npar.LoadFromPath( "CfgVehicles " + GetType() + " NoiseCarHorn" );
		//GetGame().GetNoiseSystem().AddNoise( this, npar );
	}

	// ------------------------------------------------------------	
	override void EOnPostSimulate( IEntity other, float timeSlice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - Start");
		#endif

		//! Prevent vanilla fluid checks from running
		m_Time = -1; 

		if ( CanUpdateHorn( timeSlice ) )
		{
			UpdateHorn( timeSlice );
		}

		if ( CanUpdateHealth( timeSlice ) )
		{
			UpdateHealth( timeSlice );
		}

		if ( CanUpdateCarLock( timeSlice ) )
		{
			UpdateCarLock( timeSlice );
		}

		if ( IsMissionClient() )
		{
			if ( IsDamageDestroyed() )
			{
				if ( !SEffectManager.IsEffectExist( m_enginePtcFx ) )
				{
					if ( !IsInherited( ExpansionHelicopterScript ) || !IsInherited( ExpansionPlaneScript ) || !IsInherited( ExpansionBoatScript ) ) 
					{
						m_engineFx = new EffEngineSmoke();
						m_enginePtcFx = SEffectManager.PlayOnObject( m_engineFx, this, m_enginePtcPos );
						m_engineFx.SetParticleStateHeavy();
					}
				}

				if ( IsInherited( ExpansionHelicopterScript ) || IsInherited( ExpansionPlaneScript ) || IsInherited( ExpansionBoatScript ) ) 
				{
					if ( m_engineFx ) 
					{
						m_engineFx.SetParticleStateExpansion();
					}

					if ( m_Particles.Count() <= 0 )
					{
						CreateParticle( this, "fire_pos", ParticleList.EXPANSION_FIRE_HELICOPTER );
					}
				}
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - End");
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

	// ------------------------------------------------------------
	// UpdateLights
	// ------------------------------------------------------------	
	override void UpdateLights( int new_gear = -1 ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::UpdateLights - Start");
		#endif

		SetCarBatteryStateForVanilla( true );

		super.UpdateLights( new_gear );

		SetCarBatteryStateForVanilla( false );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::UpdateLights - End");
		#endif
	}

	// ------------------------------------------------------------
	// CreateRearLight
	// ------------------------------------------------------------	
	override CarRearLightBase CreateRearLight()
	{
		if ( !IsInherited( ExpansionHelicopterScript ) || !IsInherited( ExpansionBoatScript ) ) 
		{
			return CarRearLightBase.Cast( ScriptedLightBase.CreateLight( OffroadHatchbackFrontLight ) );
		}

		return NULL;
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
		Matrix3.Tilda( m_AngularVelocity, estimatedOrientation );

		Math3D.MatrixInvMultiply3( estimatedOrientation.data, m_Transform.GetBasis().data, estimatedOrientation.data );
		
		estimatedOrientation.data[0] = m_Transform.data[0] + ( estimatedOrientation.data[0] * pDt );
		estimatedOrientation.data[1] = m_Transform.data[1] + ( estimatedOrientation.data[1] * pDt );
		estimatedOrientation.data[2] = m_Transform.data[2] + ( estimatedOrientation.data[2] * pDt );

		estimatedOrientation.data[0] = estimatedOrientation.data[0].Normalized();
		estimatedOrientation.data[1] = estimatedOrientation.data[1].Normalized();
		estimatedOrientation.data[2] = estimatedOrientation.data[2].Normalized();

		return estimatedOrientation;
	}

	void GetHeadingTransform( Object obj, out vector trans[4] )
	{
		float heading = obj.GetOrientation()[0] * Math.DEG2RAD;

		trans[1] = "0 1 0";
		trans[2] = Vector( Math.Sin( heading ), 0, Math.Cos( heading ) );
		trans[0] = -(trans[2] * trans[1]);

		trans[3] = obj.GetPosition();
	}

	void GetBoundingPositionTransform( Object obj, vector position, vector wsTrans[4], out vector trans[4] )
	{
		vector bbTrans[4];
		Math3D.YawPitchRollMatrix( "0 0 0", bbTrans );
		bbTrans[3] = position;

		Math3D.MatrixMultiply4( wsTrans, bbTrans, trans );

		vector contact_pos;
		vector contact_dir
		vector hitNormal;
		int contactComponent;
		float hitFraction;
		Object hitObject;
		
		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER;
		DayZPhysics.RayCastBullet( trans[3] + "0 10 0", trans[3] - "0 10 0", collisionLayerMask, obj, hitObject, contact_pos, hitNormal, hitFraction );
		trans[3] = contact_pos;
	}

	// ------------------------------------------------------------
	protected void ExpansionDebugUI( string message = "" )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CAR, message );
	}

	// ------------------------------------------------------------
	protected bool CanSimulate()
	{
		return false;
	}

	// ------------------------------------------------------------
	protected void OnHumanPilot( PlayerBase driver, float pDt )
	{
		//! Handled in vanilla
	}

	// ------------------------------------------------------------
	protected void OnAIPilot( ExpansionAIBase driver, float pDt )
	{
		//! Do not call super unless you want car driving AI logic.
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[" + this + "] EOnSimulate" );
		#endif

		DayZPlayerImplement driver = DayZPlayerImplement.Cast( CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) );

		if ( GetGame().IsClient() )
		{
			m_IsPhysicsHost = driver == GetGame().GetPlayer();
		} else if ( GetGame().IsServer() )
		{
			m_IsPhysicsHost = true;
		} else
		{
			m_IsPhysicsHost = false;
		}

		//Print( this );
		//Print( m_IsPhysicsHost );
		//Print( m_IsBeingTowed );
		//Print( m_ParentTow );

		if ( m_IsPhysicsHost && m_IsBeingTowed )
		{
			if ( m_ParentTow ) // shouldn't ever be NULL but just incase
			{
				if ( dBodyIsActive( m_ParentTow ) && dBodyIsDynamic( m_ParentTow ) )
				{
					vector towTransform[4];
					m_ParentTow.GetTransform( towTransform );
					towTransform[3] = m_ParentTow.ModelToWorld( m_ParentTow.m_TowPointCenter );

					dBodySetTargetMatrix( this, towTransform, 1.0 / 40.0 );
				}
			}

			return;
		}

		if ( !CanSimulate() )
			return;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnSimulate - Start");
		#endif

		ExpansionDebugUI( "[[ " + this + " ]]" );

		OnPreSimulation( dt );

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
				OnHumanPilot( humanDriver, dt );
			}
		}

		if ( m_IsPhysicsHost )
		{
			m_Controller.Update();

			m_Transform.Get( this );
			
			m_LastLinearVelocity = m_LinearVelocity;
			m_LinearAcceleration = m_LastLinearVelocity - m_LinearVelocity;
			m_LinearVelocity = GetVelocity( this );

			m_LastAngularVelocity = m_LastAngularVelocity;
			m_AngularVelocity = dBodyGetAngularVelocity( this );
			
			m_LastLinearVelocityMS = m_LinearVelocityMS;
			m_LinearAccelerationMS = m_LastLinearVelocityMS - m_LinearVelocityMS;
			m_LinearVelocityMS = m_LinearVelocity.InvMultiply3( m_Transform.GetBasis().data );

			m_BodyMass = dBodyGetMass( this );
			m_BodyCenterOfMass = dBodyGetCenterOfMass( this );

			CalculateAltitudeLimiter();

			vector force = vector.Zero;
			vector torque = vector.Zero;

			OnSimulation( dt, force, torque );

			//if ( force.Length() != 0 )
				dBodyApplyImpulse( this, force * dt );

			//if ( torque.Length() != 0 )
				dBodyApplyTorqueImpulse( this, torque * dt );

			if ( GetGame().IsMultiplayer() )
			{
				SetSynchDirty();
			}

			if ( IsMissionClient() )
			{
				NetworkSend();
			}
		}

		OnPostSimulation( dt );

		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_CAR );
		ExpansionDebugger.Push( EXPANSION_DEBUG_VEHICLE_CONTROLLER );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnSimulate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// GetCurrentOrientation
	// ------------------------------------------------------------	
	protected vector GetCurrentOrientation()
	{
		m_Orientation = GetOrientation();
		
		return m_Orientation;
	}
	
	// ------------------------------------------------------------
	// GetCurrentPosition
	// ------------------------------------------------------------	
	protected vector GetCurrentPosition()
	{
		m_Position = GetPosition();
		
		return m_Position;
	}

	// ------------------------------------------------------------
	// GetExpansionSaveVersion
	// ------------------------------------------------------------	
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	// ------------------------------------------------------------
	// OnStoreSave
	// ------------------------------------------------------------	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		super.OnStoreSave( ctx );

		ctx.Write( m_PersistentIDA );
		ctx.Write( m_PersistentIDB );
		ctx.Write( m_PersistentIDC );
		ctx.Write( m_PersistentIDD );

		int lockState = m_VehicleLockedState;
		ctx.Write( lockState );

		ctx.Write( m_Exploded );

		ctx.Write( m_CurrentSkinName );
		
		GetCurrentOrientation();
		ctx.Write( m_Orientation );	
		
		GetCurrentPosition();
		ctx.Write( m_Position );

		ctx.Write( m_allAttachments );

		
		ctx.Write( m_IsBeingTowed );
		ctx.Write( m_IsTowing );

		if ( m_IsBeingTowed )
		{
			ctx.Write( m_ParentTowPersistentIDA );
			ctx.Write( m_ParentTowPersistentIDB );
			ctx.Write( m_ParentTowPersistentIDC );
			ctx.Write( m_ParentTowPersistentIDD );
		}

		if ( m_IsTowing )
		{
			ctx.Write( m_ChildTowPersistentIDA );
			ctx.Write( m_ChildTowPersistentIDB );
			ctx.Write( m_ChildTowPersistentIDC );
			ctx.Write( m_ChildTowPersistentIDD );
		}
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("CarScript::OnStoreLoad - Start");
		#endif

		// Use GetExpansionSaveVersion()
		// Making sure this is read before everything else.
		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDA ), "[" + this + "] Failed reading m_PersistentIDA" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDB ), "[" + this + "] Failed reading m_PersistentIDB" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDC ), "[" + this + "] Failed reading m_PersistentIDC" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDD ), "[" + this + "] Failed reading m_PersistentIDD" ) )
			return false;
		
		if ( GetExpansionSaveVersion() <= 5 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_VehicleLockedState ), "[" + this + "] Failed reading m_VehicleLockedState" ) )
				return false;
		} else
		{
			int lockState;
			if ( Expansion_Assert_False( ctx.Read( lockState ), "[" + this + "] Failed reading lockState" ) )
				return false;

			m_VehicleLockedState = lockState;
		}

		if ( Expansion_Assert_False( ctx.Read( m_Exploded ), "[" + this + "] Failed reading m_Exploded" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_Orientation ), "[" + this + "] Failed reading m_Orientation" ) )
			return false;
			
		if ( Expansion_Assert_False( ctx.Read( m_Position ), "[" + this + "] Failed reading m_Position" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_allAttachments ), "[" + this + "] Failed reading m_allAttachments" ) )
			return false;

		if ( GetExpansionSaveVersion() >= 7 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_IsBeingTowed ), "[" + this + "] Failed reading m_IsBeingTowed" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_IsTowing ), "[" + this + "] Failed reading m_IsTowing" ) )
				return false;

			if ( m_IsBeingTowed )
			{
				if ( Expansion_Assert_False( ctx.Read( m_ParentTowPersistentIDA ), "[" + this + "] Failed reading m_ParentTowPersistentIDA" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ParentTowPersistentIDB ), "[" + this + "] Failed reading m_ParentTowPersistentIDB" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ParentTowPersistentIDC ), "[" + this + "] Failed reading m_ParentTowPersistentIDC" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ParentTowPersistentIDD ), "[" + this + "] Failed reading m_ParentTowPersistentIDD" ) )
					return false;
			}

			if ( m_IsTowing )
			{
				if ( Expansion_Assert_False( ctx.Read( m_ChildTowPersistentIDA ), "[" + this + "] Failed reading m_ChildTowPersistentIDA" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ChildTowPersistentIDB ), "[" + this + "] Failed reading m_ChildTowPersistentIDB" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ChildTowPersistentIDC ), "[" + this + "] Failed reading m_ChildTowPersistentIDC" ) )
					return false;
				if ( Expansion_Assert_False( ctx.Read( m_ChildTowPersistentIDD ), "[" + this + "] Failed reading m_ChildTowPersistentIDD" ) )
					return false;
			}
		}
		
		SetSynchDirty();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("CarScript::OnStoreLoad - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

		if ( m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = ExpansionSkin.Cast( m_Skins[ m_CurrentSkinIndex ] );

			ExpansionOnSkinUpdate();
		}

		if ( m_IsBeingTowed && !m_ParentTow )
		{
			m_ParentTow = CarScript.Cast( GetGame().GetEntityByPersitentID( m_ParentTowPersistentIDA, m_ParentTowPersistentIDB, m_ParentTowPersistentIDC, m_ParentTowPersistentIDD ) );
			if ( m_ParentTow )
			{
				m_ParentTow.m_IsTowing = true;
				m_ParentTow.m_ChildTow = this;
			} else
			{
				m_IsBeingTowed = false;
			}
		}

		if ( m_IsTowing && !m_ChildTow )
		{
			m_ChildTow = CarScript.Cast( GetGame().GetEntityByPersitentID( m_ChildTowPersistentIDA, m_ChildTowPersistentIDB, m_ChildTowPersistentIDC, m_ChildTowPersistentIDD ) );
			if ( m_ParentTow )
			{
				m_ChildTow.m_IsBeingTowed = true;
				m_ChildTow.m_ParentTow = this;
			} else
			{
				m_IsTowing = false;
			}
		}
	}

	// ------------------------------------------------------------
	override void OnEngineStop()
	{
		super.OnEngineStop();

		SetAnimationPhase( "EnableMonitor", -1 );

		m_MonitorEnabled = false;

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	override void OnEngineStart()
	{
		super.OnEngineStart();
		
		SetAnimationPhase( "EnableMonitor", 0 );

		m_MonitorEnabled = true;

		SetSynchDirty();
	}

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();
		
		if ( m_SafeZoneSynchRemote && !m_SafeZone )
		{
			OnEnterSafeZone();
		} else if ( !m_SafeZoneSynchRemote && m_SafeZone )
		{
			OnLeftSafeZone();
		}

		if ( IsSoundSynchRemote() && !m_HornPlaying )
		{
			OnHonkSoundPlay();
		} else if ( !IsSoundSynchRemote() && m_HornPlaying )
		{
			OnHonkSoundStop();
		}

		if ( m_ExplodedSynchRemote && !m_Exploded ) 
		{
			ExpansionOnSpawnExploded();
		}

		if ( m_CanBeSkinned && m_CurrentSkinSynchRemote != m_CurrentSkinIndex )
		{
			m_CurrentSkinIndex = m_CurrentSkinSynchRemote;
			if ( m_CurrentSkinIndex >= 0 && m_CurrentSkinIndex < m_Skins.Count() )
			{
				m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
				m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];
			} else
			{
				m_CurrentSkinName = "";
				m_CurrentSkin = NULL;	
			}

			ExpansionOnSkinUpdate();
		}

		if ( m_MonitorEnabled )
		{
			SetAnimationPhase( "EnableMonitor", 0 );
		} else 
		{
			SetAnimationPhase( "EnableMonitor", -1 );
		}

		if ( m_IsBeingTowed )
		{
			m_ParentTow = CarScript.Cast( GetGame().GetObjectByNetworkId( m_ParentTowNetworkIDLow, m_ParentTowNetworkIDHigh ) );
		} else
		{
			m_ParentTow = NULL;
		}

		if ( m_IsTowing )
		{
			m_ChildTow = CarScript.Cast( GetGame().GetObjectByNetworkId( m_ChildTowNetworkIDLow, m_ChildTowNetworkIDHigh ) );
		} else
		{
			m_ChildTow = NULL;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnVariablesSynchronized - End");
		#endif
	}

	protected void ExpansionSetupSkins()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetupSkins - Start");
		#endif

		m_Skins = new array< ExpansionSkin >;

		if ( Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) ) )
		{
			m_SkinModule.RetrieveSkins( GetType(), m_Skins, m_CurrentSkinName );
		}

		m_CanBeSkinned = m_Skins.Count() != 0;

		if ( m_CanBeSkinned )
		{
			if ( m_CurrentSkinName != "" )
			{
				m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			} else
			{
				m_CurrentSkinIndex = 0;
				
				m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
			}
			
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

			ExpansionOnSkinUpdate();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetupSkins - End");
		#endif
	}	

	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - Start");
		#endif

		if ( m_CanBeSkinned )
		{
			string sZone = zone;
			sZone.ToLower();

			if ( m_CurrentSkin )
			{
				if ( m_CurrentSkin.DamageZones )
				{
					if ( m_CurrentSkin.DamageZones.Count() > 0 )
					{
						for ( int i = 0; i < m_CurrentSkin.DamageZones.Count(); i++ )
						{
							string cZone = m_CurrentSkin.DamageZones[i].Zone;
							cZone.ToLower();

							if ( cZone == sZone )
							{
								ExpansionOnSkinDamageZoneUpdate( m_CurrentSkin.DamageZones[i], newLevel );
							}
						}
					}
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - End");
		#endif
	}

	void ExpansionSetSkin( int skinIndex )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - Start");
		#endif
		
		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CanBeSkinned );
		Print( skinIndex );
		#endif

		if ( !m_CanBeSkinned )
		{
			m_CurrentSkinName = "";
			return;
		}

		m_CurrentSkinIndex = skinIndex;

		if ( m_CurrentSkinIndex < 0 )
		{
			m_CurrentSkinIndex = 0;
		}

		if ( m_CurrentSkinIndex >= m_Skins.Count() )
		{
			m_CurrentSkinIndex = 0;
		}

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CurrentSkinIndex );
		#endif

		m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
		m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
		m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CurrentSkinName );
		Print( m_CurrentSkinSynchRemote );
		Print( m_CurrentSkin );
		#endif

		ExpansionOnSkinUpdate();

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - End");
		#endif
	}

	void ExpansionOnSkinDamageZoneUpdate( ExpansionSkinDamageZone zone, int level )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinDamageZoneUpdate - Start");
		#endif
		
		#ifdef EXPANSION_SKIN_LOGGING
		Print( zone );
		Print( level );
		#endif

		for ( int i = 0; i < zone.HiddenSelections.Count(); i++ )
		{
			int selectionIndex = GetHiddenSelectionIndex( zone.HiddenSelections[i] );

			#ifdef EXPANSION_SKIN_LOGGING
			Print( "HiddenSelection: " + zone.HiddenSelections[i] );
			Print( "SelectionIndex: " + selectionIndex );
			#endif

			if ( level >= 0 && level < zone.HealthLevels.Count() )
			{
				ExpansionSkinHealthLevel healthLevel = zone.HealthLevels[level];

				#ifdef EXPANSION_SKIN_LOGGING
				Print( "RVTexture: " + healthLevel.RVTexture );
				Print( "RVMaterial: " + healthLevel.RVMaterial );
				#endif

				SetObjectTexture( selectionIndex, healthLevel.RVTexture );
				SetObjectMaterial( selectionIndex, healthLevel.RVMaterial );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinDamageZoneUpdate - End");
		#endif
	}

	void ExpansionOnSkinUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinUpdate - Start");
		#endif

		if ( !m_CurrentSkin )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint( "ItemBase::ExpansionOnSkinUpdate called but m_CurrentSkin is NULL!" );
			#endif

			return;
		}

		if ( m_CurrentSkin.HornEXT != "" )
		{
			m_HornSoundSetEXT = m_CurrentSkin.HornEXT;
		} else
		{
			m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		}
		
		if ( m_CurrentSkin.HornINT != "" )
		{
			m_HornSoundSetINT = m_CurrentSkin.HornINT;
		} else
		{		
			m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
		}

		for ( int i = 0; i < m_CurrentSkin.HiddenSelections.Count(); i++ )
		{
			ExpansionSkinHiddenSelection selection = m_CurrentSkin.HiddenSelections[ i ];

			int selectionIndex = GetHiddenSelectionIndex(  selection.HiddenSelection );

			#ifdef EXPANSION_SKIN_LOGGING
			Print( "HiddenSelection: " + selection.HiddenSelection );
			Print( "SelectionIndex: " + selectionIndex );
			Print( "RVTexture: " + selection.RVTexture );
			Print( "RVMaterial: " + selection.RVMaterial );
			#endif

			SetObjectTexture( selectionIndex, selection.RVTexture );
			SetObjectMaterial( selectionIndex, selection.RVMaterial );
		}
		
		for ( i = 0; i < m_CurrentSkin.DamageZones.Count(); i++ )
		{
			ExpansionOnSkinDamageZoneUpdate( m_CurrentSkin.DamageZones[i], GetHealthLevel( m_CurrentSkin.DamageZones[i].Zone ) );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinUpdate - End");
		#endif
	}

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
		}
		else 
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
	float GetCameraHeight()
	{
		return 5;
	}

	// ------------------------------------------------------------
	float GetCameraDistance()
	{
		return 15;
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
	
	string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUniversalWheel"; //this should never happen
	}

	override void OnContact( string zoneName, vector localPos, IEntity other, Contact data )
	{
		if ( m_IsBeingTowed )
			return;

		super.OnContact( zoneName, localPos, other, data );
		
		Object tree = Object.Cast( other );
		if ( tree.GetType().Contains( "TreeHard" ) ||  tree.GetType().Contains( "TreeSoft" ) )
		{
			if ( tree.IsTree() || tree.IsBush() )
			{
				EntityAI cutting_tool = EntityAI.Cast( GetGame().CreateObject("WoodAxe", vector.Zero, false, true) );

				if ( data.Impulse > 7500 )
				{
					if ( IsMissionClient() )
					{
						SoundHardTreeFallingPlay();
					}	

					if ( tree.GetType().Contains("TreeHard") )
					{
						TreeHard treeHard = TreeHard.Cast( tree );
						if ( treeHard )
						{
							treeHard.DecreaseHealth("", "", 100, true);		
							treeHard.OnTreeCutDown( cutting_tool );
						}
					}

					if ( tree.GetType().Contains("TreeSoft") )
					{
						TreeSoft treeSoft= TreeSoft.Cast( tree );
						if ( treeSoft )
						{
							treeSoft.DecreaseHealth("", "", 100, true);		
							treeSoft.OnTreeCutDown( cutting_tool );
						}
					}

					dBodyDestroy(tree);
				}

				GetGame().ObjectDelete(cutting_tool);
			}
		}
		
		if ( IsMissionHost() )
		{
			float dmgMin = 150.0;	
			float dmgThreshold = 750.0;
			float dmgKillCrew = 3000.0;
			float dmg = data.Impulse * m_dmgContactCoef;

			if ( dmg >= dmgThreshold )
			{
				IEntity child = GetChildren();
				while ( child )
				{
					PlayerBase player;
					if ( Class.CastTo( player, child ) )
					{
						if ( dmg > dmgKillCrew )
						{
							player.SetHealth(0.0);
						} else
						{
							//! Deal shock to player
							float shockTemp = Math.InverseLerp( dmgThreshold, dmgKillCrew, dmg );
							float shock = Math.Lerp( 50, 100, shockTemp );
							float hp = Math.Lerp( 2, 60, shockTemp );

							player.AddHealth( "", "Shock", -shock );
							player.AddHealth( "", "Health", -hp );
						}
					}
					
					child = child.GetSibling();
				}
			}
		}
	}
}

static TransferInventoryResult ExpansionTransferInventory( EntityAI sourceItem, EntityAI targetItem )
{
	TransferInventoryResult result = TransferInventoryResult.Ok;

	array<EntityAI> children = new array<EntityAI>;
	sourceItem.GetInventory().EnumerateInventory(InventoryTraversalType.LEVELORDER, children);
	int count = children.Count();
	for (int i = 0; i < count; i++)
	{
		EntityAI child = children.Get(i);
		if (child)
		{
			InventoryLocation child_src = new InventoryLocation;
			child.GetInventory().GetCurrentInventoryLocation( child_src );
				
			InventoryLocation child_dst = new InventoryLocation;
			child_dst.Copy( child_src );
			child_dst.SetParent( targetItem );

			bool drop = false;

			if (GameInventory.LocationCanAddEntity(child_dst))
			{
				sourceItem.LocalTakeToDst(child_src, child_dst);
			}
			else
			{
				drop = true;
			}

			if (drop)
			{
				sourceItem.LocalDropEntity(child);
				GetGame().RemoteObjectTreeCreate(child);
				result = TransferInventoryResult.DroppedSome;
			}
		}
	}
	return result;
}