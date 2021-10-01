/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef DAYZ_1_13
class CarContactData
{
	vector localPos;
	IEntity other;
	float impulse;
	
	void CarContactData(vector _localPos, IEntity _other, float _impulse)
	{
		localPos = _localPos;
		other = _other;
		impulse = _impulse;
	}
};

typedef map<string, ref array<ref CarContactData>> CarContactCache;
#endif

enum ExpansionVehicleLockState
{
	NOLOCK = 0,
	UNLOCKED,
	READY_TO_LOCK,
	LOCKED,
	FORCEDUNLOCKED,
	//Use count here, for network optimization to know max 
	COUNT
};

class ExpansionVehicleAttachmentSave
{
	vector m_Position;
	vector m_Orientation;

	void ExpansionVehicleAttachmentSave( vector position = vector.Zero, vector orientation = vector.Zero )
	{
		m_Position = position;
		m_Orientation = orientation;
	}
	#ifdef CF_MODULE_MODSTORAGE
	void OnWrite( CF_ModStorage storage )
	{
		storage.Write( m_Position );
		storage.Write( m_Orientation );
	}

	bool OnRead( CF_ModStorage storage )
	{
		storage.Read( m_Position );
		storage.Read( m_Orientation );

		return true;
	}
	#endif
};

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	private static ref array< ExpansionVehicleAttachmentSave > m_allAttachments = new array< ExpansionVehicleAttachmentSave >;
	
	#ifdef DAYZ_1_13
	ref CarContactCache m_ContactCache;
	#endif

	ref array<ref ExpansionVehicleModule> m_Modules = new array<ref ExpansionVehicleModule>();
	ref array<ExpansionVehicleAxle> m_Axles = new array<ExpansionVehicleAxle>();
	ref array<ExpansionVehicleWheel> m_Wheels = new array<ExpansionVehicleWheel>();
	ref array<ExpansionVehicleAerofoil> m_Aerofoils = new array<ExpansionVehicleAerofoil>();
	ref array<ExpansionVehicleEngine> m_Engines = new array<ExpansionVehicleEngine>();
	
	ref ExpansionController m_Controller = new ExpansionController();
	ref ExpansionPhysicsState m_State = new ExpansionPhysicsStateT<CarScript>(this);

	int m_Expansion_EnginesOn;
	
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

	protected int m_Expansion_MinimumEngine;
	protected int m_Expansion_MaximumEngine;
	protected int m_Expansion_CurrentEngine;

	// Horn
	protected EffectSound m_HornSound;

	protected string m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
	protected string m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
	protected bool m_HornPlaying;
	protected bool m_HornSynchRemote;

	// Vehicle locking
	protected ExpansionVehicleLockState m_VehicleLockedState;

	//! After pairing a key, it's the ID of the master key.
	//! This allows "changing locks" on vehicles so old paired keys no longer work
	protected int m_PersistentIDA;
	protected int m_PersistentIDB;
	protected int m_PersistentIDC;
	protected int m_PersistentIDD;

	// Explosion
	protected bool m_Exploded;
	protected bool m_ExplodedSynchRemote;
	protected int m_ExplosionSize;

	// Timeslice for Engine damage via rev or fluids
	protected float m_FluidCheckTime;

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

	// Physics
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
	vector m_DbgTransform[4];
#endif

	protected bool m_IsPhysicsHost;
	
	protected float m_BoundingRadius;
	protected vector m_BoundingBox[2];
	
	protected bool m_CarBatteryVanillaState;
	protected bool m_CarBatteryVanillaStateDefault;

	protected Particle m_SmokeParticle;

	// Lights
	ref array< ref ExpansionPointLight > m_Lights;
	ref array< ref Particle > m_Particles;

	protected vector m_Orientation;
	protected vector m_Position;
	
	protected autoptr TStringArray m_Doors;
	protected bool m_CanHaveLock;

	private EffectSound m_SoundLock;

	protected bool m_Expansion_EngineIsOn;

	//! Settings
	protected bool m_TowingEnabled;

	//! Debugging
	private ref array< Shape > m_DebugShapes;

	//! Skinning
	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ExpansionSkin > m_Skins;

	protected bool m_CanSimulate;

	protected float m_ModelAnchorPointY = -1;

	protected bool m_Expansion_CanPlayerAttach;
	protected bool m_Expansion_CanPlayerAttachSet;

	protected bool m_Expansion_EngineSync1;
	protected bool m_Expansion_EngineSync2;
	protected bool m_Expansion_EngineSync3;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
		#endif

		SetEventMask( EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT );
		
		#ifdef DAYZ_1_13
		m_ContactCache = new CarContactCache;
		#endif

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );
		RegisterNetSyncVariableInt( "m_VehicleLockedState" );

		RegisterNetSyncVariableInt( "m_Expansion_CurrentEngine" );

		RegisterNetSyncVariableBool( "m_IsBeingTowed" );
		RegisterNetSyncVariableBool( "m_IsTowing" );
		RegisterNetSyncVariableInt( "m_ParentTowNetworkIDLow" );
		RegisterNetSyncVariableInt( "m_ParentTowNetworkIDHigh" );
		RegisterNetSyncVariableInt( "m_ChildTowNetworkIDLow" );
		RegisterNetSyncVariableInt( "m_ChildTowNetworkIDHigh" );

		RegisterNetSyncVariableBool( "m_HornSynchRemote" );

		RegisterNetSyncVariableBool( "m_Expansion_EngineIsOn" );
		RegisterNetSyncVariableBool( "m_CanSimulate" );

		m_DebugShapes = new array< Shape >();

		m_Lights = new array< ref ExpansionPointLight >;
		m_Particles = new array< ref Particle >;
		
		m_Doors = new TStringArray;
		ConfigGetTextArray( "doors", m_Doors );

		m_CanHaveLock = m_Doors.Count() > 0;

		LoadConstantVariables();

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );

		ExpansionSettings.SI_Vehicle.Insert( OnSettingsUpdated );

		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote" );
		
		Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) );

		ExpansionSetupSkins();

		if (IsMissionHost())
			SetAllowDamage(CanBeDamaged());

		m_Expansion_MinimumEngine = 0;
		if (!IsCar())
			m_Expansion_MinimumEngine = 1;

		m_Expansion_MaximumEngine = 1;

		int i;
		int count;

		string path;

		path = "CfgVehicles " + GetType() + " SimulationModule Engines";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string engineName;
			GetGame().ConfigGetChildName(path, i, engineName);

			string enginePath = path + " " + engineName;
			AddModule(new ExpansionVehicleEngine(this, enginePath));
		}

		if (m_Engines.Count() == 0 && (!IsCar() || (IsCar() && IsBoat())))
		{
			AddModule(new ExpansionVehicleEngine(this, "CfgVehicles " + GetType() + " SimulationModule Engine"));
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - Start");
		#endif

		int i;

		if ( IsMissionClient( ) )
		{
			if ( m_SmokeParticle )
			{
				m_SmokeParticle.Stop( );
			}

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

		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( i = 0; i < m_DebugShapes.Count(); i++ )
			m_DebugShapes[i].Destroy();
		#endif
		
		delete m_DebugShapes;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - End");
		#endif
	}

	void LoadConstantVariables()
	{
	}

	void OnAfterLoadConstantVariables()
	{
		for (int i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].TEMP_DeferredInit();
		}
	}

	void LongDeferredInit()
	{
		OnSettingsUpdated();
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		for (int i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].Init();
		}

		Expansion_EngineGetCurrent();

		m_State.m_MaxSpeed = m_MaxSpeed;
		m_State.m_AltitudeFullForce = m_AltitudeFullForce;
		m_State.m_AltitudeNoForce = m_AltitudeNoForce;

		m_State.Init();

		m_BoundingRadius = m_State.m_BoundingRadius;
		m_BoundingBox[0] = m_State.m_BoundingBox[0];
		m_BoundingBox[1] = m_State.m_BoundingBox[1];
		m_MaxSpeedMS = m_State.m_MaxSpeedMS;

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( OnAfterLoadConstantVariables, 100, false );

		if ( IsMissionHost() )
		{
			SetSynchDirty();
		}
	}

	void OnSettingsUpdated()
	{
		m_TowingEnabled = GetExpansionSettings().GetVehicle().Towing;

		for (int i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].SettingsChanged();
		}
	}

	ExpansionController GetExpansionController()
	{
		return m_Controller;
	}

	void UpdateVisuals()
	{
	}

	bool HasRoof()
	{
		return true;
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionPairKey );
		AddAction( ExpansionActionAdminUnpairKey );

		AddAction( ExpansionActionLockVehicle );
		AddAction( ExpansionActionUnlockVehicle );
	}

#ifdef EXPANSION_VEHICLE_TOWING
	void CreateTow( Object tow )
	{
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
	}

	void OnTowCreated( Object parent, vector towPos )
	{
		m_ParentTow = EntityAI.Cast( parent );
		m_IsBeingTowed = true;
		m_TowPointCenterSelf = towPos;
		
		if ( !IsMissionOffline() )
			m_ParentTow.GetNetworkID( m_ParentTowNetworkIDLow, m_ParentTowNetworkIDHigh );
	}

	EntityAI GetTowedEntity()
	{
		if ( m_IsTowing )
			return m_ChildTow;
		
		return NULL;
	}

	void DestroyTow()
	{
		if ( !m_IsTowing )
			return;

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

		m_IsTowing = false;

		if ( !IsMissionOffline() )
		{
			m_ChildTow.SetSynchDirty();
			SetSynchDirty();
		}
		
		m_ChildTow = NULL;
	}

	void OnTowDestroyed()
	{
		m_ParentTow = null;
		m_IsBeingTowed = false;
	}
#endif

	vector GetTowCenterPosition( Object other )
	{
		//!todo: calculate from wheel positions
		
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

	bool ExpansionCanPlayerAttach()
	{
#ifdef EXPANSION_PLAYER_ATTACHMENT_CANATTACH_OVERRIDE
		m_Expansion_CanPlayerAttach = true;
#else
		if (!m_Expansion_CanPlayerAttachSet)
		{
			m_Expansion_CanPlayerAttachSet = true;
			foreach ( ExpansionVehiclesConfig vehcfg : GetExpansionSettings().GetVehicle().VehiclesConfig )
			{
				if ( IsKindOf( vehcfg.ClassName ) )
				{
					m_Expansion_CanPlayerAttach = vehcfg.CanPlayerAttach;
					break;
				}
			}
		}
#endif

		return m_Expansion_CanPlayerAttach;
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

	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	protected void KeyMessage( string message )
	{
		#ifdef EXPANSION_CARKEY_LOGGING	
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );

			Print( message );
		} else
		{
			Print( message ); 
		}
		#endif
	}

	// ------------------------------------------------------------
	// Expansion IsLocked
	// ------------------------------------------------------------
	bool IsLocked()
	{
		if ( HasKey() )
		{
			KeyMessage( "CarScript::IsLocked HasKey() true and " + (m_VehicleLockedState == ExpansionVehicleLockState.LOCKED));
			return m_VehicleLockedState == ExpansionVehicleLockState.LOCKED;
		}

		KeyMessage( "CarScript::IsLocked false");
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
	void SetPersistentIDA(int newIDA)
	{
		m_PersistentIDA = newIDA;
	}

	// ------------------------------------------------------------
	void SetPersistentIDB(int newIDB)
	{
		m_PersistentIDB = newIDB;
	}

	// ------------------------------------------------------------
	void SetPersistentIDC(int newIDC)
	{
		m_PersistentIDC = newIDC;
	}

	// ------------------------------------------------------------
	void SetPersistentIDD(int newIDD)
	{
		m_PersistentIDD = newIDD;
	}

	// ------------------------------------------------------------
	void SetLockedState(ExpansionVehicleLockState newLockState)
	{
		m_VehicleLockedState = newLockState;
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
		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::PairKeyTo - Start");
		#endif

		if ( key )
		{
			key.PairToVehicle( this );

			m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;

			SetSynchDirty();
		}

		KeyMessage( "PairKeyTo (" + this + ", " + key + ")" );

		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::PairKeyTo - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ResetKeyPairing
	// ------------------------------------------------------------
	// Only call this after all keys have been confirmed to be removed
	void ResetKeyPairing()
	{
		if ( IsMissionHost() )
		{
			m_PersistentIDA = 0;
			m_PersistentIDB = 0;
			m_PersistentIDC = 0;
			m_PersistentIDD = 0;

			m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

			SetSynchDirty();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CanBeLocked
	// ------------------------------------------------------------
	bool CanBeLocked()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion OnCarDoorOpened
	// ------------------------------------------------------------
	void OnCarDoorOpened( string source )
	{
		if ( HasKey() ) 
		{
			if ( m_VehicleLockedState == ExpansionVehicleLockState.READY_TO_LOCK )
			{
				m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
				KeyMessage( "OnCarDoorOpened::UNLOCKED" );

				SetSynchDirty();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnCarDoorClosed
	// ------------------------------------------------------------
	void OnCarDoorClosed( string source )
	{

	}
	
	// ------------------------------------------------------------
	// Expansion IsCarKeys
	// ------------------------------------------------------------
	bool IsCarKeys( ExpansionCarKey key )
	{
		if ( !HasKey() )
		{
			KeyMessage( "IsCarKeys::HasKey" );
			return false;
		}

		if ( !key.IsPairedTo( this ) )
		{
			KeyMessage( "IsCarKeys not paired!" );
			return false;
		}
		
		KeyMessage( "IsCarKeys is paired" );

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion LockCar
	// ------------------------------------------------------------
	void LockCar( ExpansionCarKey key )
	{
		KeyMessage( "LockCar" );
		KeyMessage( "key=" + key );
		if ( key && !IsCarKeys( key ) && !key.IsInherited(ExpansionCarAdminKey) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.READY_TO_LOCK;
		KeyMessage( "LockCar::READY_TO_LOCK" );

		SetSynchDirty();
	}
	
	// ------------------------------------------------------------
	// Expansion UnlockCar
	// ------------------------------------------------------------
	void UnlockCar( ExpansionCarKey key )
	{
		if ( key && !IsCarKeys( key ) && !key.IsInherited(ExpansionCarAdminKey) )
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
		KeyMessage( "UnlockCar::UNLOCKED" );

		OnCarUnlocked();
		SetSynchDirty();
	}

	void UnlockCarWithoutKey()
	{
		m_VehicleLockedState = ExpansionVehicleLockState.FORCEDUNLOCKED;

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

		KeyMessage( "OnCarLocked" );

		if ( GetGame().IsServer() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionVehicleRPC.PlayLockSound, true, NULL );
		}

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

		KeyMessage( "OnCarUnlocked" );

		if ( GetGame().IsServer() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionVehicleRPC.PlayLockSound, true, NULL );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarUnlocked - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnHornSoundPlay
	// ------------------------------------------------------------
	void OnHornSoundPlay()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundPlay - Start");
		#endif

		string soundFile = m_HornSoundSetEXT;
		if (GetGame().GetPlayer().IsCameraInsideVehicle()) soundFile = m_HornSoundSetINT;

		m_HornSound = SEffectManager.PlaySoundOnObject( soundFile, this );
		m_HornSound.SetSoundAutodestroy( true );
		m_HornSound.SetSoundLoop( true );

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundPlay - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnHornSoundStop
	// ------------------------------------------------------------
	void OnHornSoundStop()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundStop - Start");
		#endif

		m_HornSound.SetSoundLoop( false );
		m_HornSound.SoundStop();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundStop - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion PlayHorn
	// ------------------------------------------------------------
	void PlayHorn()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PlayHorn - Start");
		#endif

		m_HornSynchRemote = true;
		m_HornPlaying = false;

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::PlayHorn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion StopHorn
	// ------------------------------------------------------------
	void StopHorn()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::StopHorn - Start");
		#endif

		m_HornSynchRemote = false;
		m_HornPlaying = true;

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::StopHorn - End");
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

		dBodyApplyForce( this, Vector( 0, 20.0 * m_State.m_Mass, 0 ) );

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

			if ( ExpansionStatic.SurfaceIsWater( GetPosition() ) )
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
					//! NOTE: sender and driverBase.GetIdentity() will NOT be the same object even if they point to the same player identity (same ID)!
					if ( Class.CastTo( driverBase, CrewMember( DayZPlayerConstants.VEHICLESEAT_DRIVER ) ) && driverBase.GetIdentityUID() == sender.GetId() )
					{
						OnNetworkRecieve( ctx );
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
			case ExpansionVehicleRPC.PlayLockSound:
			{
				if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
				{
					if ( m_SoundLock )
						delete m_SoundLock;

					m_SoundLock = SEffectManager.PlaySound("Expansion_Car_Lock_SoundSet", GetPosition());
					m_SoundLock.SetSoundAutodestroy( true );
				}
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
	protected void OnNetworkSend(  ParamsWriteContext ctx )
	{
		for (int i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].OnNetworkSend(ctx);
		}
	}

	// ------------------------------------------------------------	
	protected void OnNetworkRecieve( ParamsReadContext ctx )
	{
		for (int i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].OnNetworkRecieve(ctx);
		}
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
	bool HasBattery()
	{
		ItemBase battery;

		if (IsVitalCarBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		if (!battery) return false;

		return true;
	}

	// ------------------------------------------------------------
	override ItemBase GetBattery()
	{
		if (IsVitalCarBattery()) return ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery()) return ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery()) return ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery()) return ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		return null;
	}

	// ------------------------------------------------------------
	bool IsBatteryWorking()
	{
		ItemBase battery;

		if (IsVitalCarBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery()) battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		if (!battery || battery.IsRuined()) return false;

		return true;
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
		return ExpansionCanPlayerAttach();
	}

	// ------------------------------------------------------------
	bool LeavingSeatDoesAttachment( int posIdx )
	{
		// only called if CanObjectAttach returns true.

		return false;
	}

	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		if ( GetExpansionSettings().GetVehicle().DisableVehicleDamage )
		{
			return false;
		}

		return super.CanBeDamaged();
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
		if (!super.OnBeforeEngineStart())
			return false;
		
		return OnBeforeEngineStart(0);
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
				vector force = Vector( 0, m_State.m_Mass * 50.0, 0 );

				string selection = "";
				InventorySlots.GetSelectionForSlotId( InventorySlots.GetSlotIdFromString( slot_name ), selection );
				vector position = GetSelectionPositionMS( selection );

				//dBodyApplyForceAt( this, ModelToWorld( position ), force );
			}
		}

		if ( IsMissionHost() )
		{
			if ( slot_name == "ExpansionHelicopterBattery" )
				m_BatteryHealth = item.GetHealth01();

			if ( slot_name == "ExpansionAircraftBattery" )
				m_BatteryHealth = item.GetHealth01();
		}

		super.EEItemAttached( item, slot_name );

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
				vector force = Vector( 0, m_State.m_Mass * 50.0, 0 );

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
				if ( slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery" )
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

			if ( Expansion_EngineIsOn() )
			{
				if ( slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery" )
					EngineStop();
			}
			
			if ( slot_name == "ExpansionHelicopterBattery" )
				m_BatteryHealth = -1;

			if ( slot_name == "ExpansionAircraftBattery" )
				m_BatteryHealth = -1;
		}

		super.EEItemDetached( item, slot_name );

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

			KeyMessage( "OnCarDoorClosed::LOCKED" );

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
					if ( EngineGetRPM() > EngineGetRPMMax() && GetExpansionSettings().GetVehicle().RevvingOverMaxRPMRuinsEngineInstantly )
						AddHealth( "Engine", "Health", -GetMaxHealth("Engine", ""));
						
					dmg = EngineGetRPM() * 0.001 * Math.RandomFloat( 0.02, 1.0 ); 
				
					ProcessDirectDamage( DT_CUSTOM, null, "Engine", "EnviroDmg", "0 0 0", dmg );
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
		NoiseParams npar = new NoiseParams();
		npar.LoadFromPath( "CfgVehicles " + GetType() + " NoiseCarHorn" );
		//GetGame().GetNoiseSystem().AddNoise( this, npar );
	}

	// ------------------------------------------------------------	
	override void EOnPostSimulate( IEntity other, float timeSlice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - Start");
		#endif

		#ifdef DAYZ_1_13
		if ( GetGame().IsServer() )
			CheckContactCache();
		#endif

		super.EOnPostSimulate( other, timeSlice );

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
					if ( !IsInherited( ExpansionHelicopterScript ) || !IsInherited( ExpansionVehiclePlaneBase ) || !IsInherited( ExpansionBoatScript ) ) 
					{
						m_engineFx = new EffEngineSmoke();
						m_enginePtcFx = SEffectManager.PlayOnObject( m_engineFx, this, m_enginePtcPos );
						m_engineFx.SetParticleStateHeavy();
					}
				}

				if ( IsInherited( ExpansionHelicopterScript ) || IsInherited( ExpansionVehiclePlaneBase ) || IsInherited( ExpansionBoatScript ) ) 
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
		array<Selection> lodSelections = new array<Selection>();

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
		array<Selection> lodSelections = new array<Selection>();

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

	void Ex_AttachPlayer(DayZPlayerImplement player)
	{
	}

	void Ex_DetachPlayer(DayZPlayerImplement player)
	{
	}

	protected bool CanSimulate()
	{
		return false;
	}

	void Expansion_HandleController(DayZPlayerImplement driver, float dt)
	{				
		if (CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER) != driver)
			return;

		if (!driver.GetInputInterface())
			return;

		m_State.m_DeltaTime = dt;

		for (int i = 0; i < m_Modules.Count(); i++)
			m_Modules[i].Control(m_State, driver);
		
		Expansion_OnHandleController(driver, dt);
	}

	void Expansion_OnHandleController(DayZPlayerImplement driver, float dt)
	{

	}

	// ------------------------------------------------------------
	protected void OnPreSimulation( float pDt )
	{
		if ( IsMissionClient() )
		{
			OnParticleUpdate( pDt );
		}
	}

	// ------------------------------------------------------------
	protected void OnNoSimulation( float pDt )
	{
		OnAnimationUpdate( pDt );
	}

	// ------------------------------------------------------------
	protected void OnSimulation(ExpansionPhysicsState pState)
	{
	}

	// ------------------------------------------------------------
	protected void OnPostSimulation( float pDt )
	{
		OnAnimationUpdate( pDt );
	}

	// ------------------------------------------------------------
	protected void OnParticleUpdate( float pDt )
	{
	}

	// ------------------------------------------------------------
	protected void OnAnimationUpdate( float pDt )
	{
		for (int i = 0; i < m_Modules.Count(); i++)
			m_Modules[i].Animate(m_State);
	}

	#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add(m_Controller);
		instance.Add(m_State);
		
		instance.Add("Is Physics Host", m_IsPhysicsHost);

		instance.Add("Num Modules", m_Modules.Count());
		for (int i = 0; i < m_Modules.Count(); i++)
			instance.Add(m_Modules[i]);

		return true;
	}
	#endif

	// ------------------------------------------------------------
	override void EOnSimulate( IEntity owner, float dt ) 
	{
		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint( "[" + this + "] EOnSimulate" );
		#endif
		
		int i;
	
		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for ( i = 0; i < m_DebugShapes.Count(); i++ )
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();
		#endif

/*
		GetTransform(m_DbgTransform);

		for (i = 0; i < CrewSize(); i++)
		{
			vector crewPos;
			vector crewDir;

			CrewEntry(i, crewPos, crewDir);

			m_State.DBGDrawLineDirectionMS(crewPos, crewDir);
			m_State.DBGDrawSphereMS(crewPos, 0.5);
		}
*/

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

		if ( m_IsPhysicsHost && m_TowingEnabled && m_IsBeingTowed )
		{
			if ( m_ParentTow ) // shouldn't ever be NULL but just incase
			{
				if ( dBodyIsActive( m_ParentTow ) && dBodyIsDynamic( m_ParentTow ) )
				{
					vector towTransform[4];
					m_ParentTow.GetTransform( towTransform );
					towTransform[3] = m_ParentTow.ModelToWorld( m_TowPointCenterSelf );

					dBodySetTargetMatrix( this, towTransform, 1.0 / 40.0 );
				}
			}

			return;
		}

		if ( !CanSimulate() )
		{
			if ( m_CanSimulate && GetGame().IsServer() )
			{
				m_CanSimulate = false;
				SetSynchDirty();
			}

			OnNoSimulation( dt );
			return;
		}

		if ( !m_CanSimulate && GetGame().IsServer() )
		{
			m_CanSimulate = true;
		}

		m_Controller.m_Yaw = GetController().GetSteering();
		//m_Controller.m_Throttle[0] = GetController().GetThrust();
		m_Controller.m_Brake[0] = GetController().GetBrake();
		m_Controller.m_Gear[0] = GetController().GetGear();

		TFloatArray gears = new TFloatArray;			
		GetGame().ConfigGetFloatArray("CfgVehicles " + GetType() + " SimulationModule Gearbox ratios" , gears);
					
		if (GetController().GetGear() > 2)
		{
			m_Controller.m_Ratio[0] = gears[GetController().GetGear() - 2];
		}
		else if (GetController().GetGear() == 0)
		{
			m_Controller.m_Ratio[0] = GetGame().ConfigGetFloat("CfgVehicles " + GetType() + " SimulationModule Gearbox reverse");
		}
		else
		{
			m_Controller.m_Ratio[0] = 0;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnSimulate - Start");
		#endif

		OnPreSimulation( dt );

		m_State.m_HasDriver = driver && m_IsPhysicsHost;

		m_State.m_Exploded = m_Exploded;

		bool isServer = GetGame().IsServer();

		if ( m_IsPhysicsHost )
		{
			m_State.PreSimulate(dt);

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
			m_State.EstimateTransform(dt, m_DbgTransform);
#endif

			m_State.CalculateAltitudeLimiter();
			
			for (i = 0; i < m_Modules.Count(); i++)
				m_Modules[i].PreSimulate(m_State);

			OnSimulation(m_State);
			
			//OnSimulation(m_State);
			//force = force * 0.5;
			//torque = torque * 0.5;

			for (i = 0; i < m_Modules.Count(); i++)
				m_Modules[i].Simulate(m_State);

			if ( IsMissionClient() )
			{
				NetworkSend();
			}
		}

		m_State.PostSimulate(dt, m_IsPhysicsHost, ExpansionVehicleNetworkMode.SERVER, false);

		OnPostSimulation( dt );

		if ( GetGame().IsServer() )
		{
			SetSynchDirty();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnSimulate - End");
		#endif
	}

	void AddModule(ExpansionVehicleModule module)
	{
		m_Modules.Insert(module);

		if (module.IsInherited(ExpansionVehicleWheel))
		{
			m_Wheels.Insert(ExpansionVehicleWheel.Cast(module));
			return;
		}

		if (module.IsInherited(ExpansionVehicleAxle))
		{
			m_Axles.Insert(ExpansionVehicleAxle.Cast(module));
			return;
		}

		if (module.IsInherited(ExpansionVehicleAerofoil))
		{
			m_Aerofoils.Insert(ExpansionVehicleAerofoil.Cast(module));
			return;
		}

		if (module.IsInherited(ExpansionVehicleEngine))
		{
			auto engine = ExpansionVehicleEngine.Cast(module);
			engine.m_EngineIndex = m_Engines.Count() + 1;
			RegisterNetSyncVariableBool( "m_Expansion_EngineSync" + engine.m_EngineIndex );
			if (engine.m_EngineIndex >= 3)
			{
				Error(GetType() + ": " + engine.m_EngineIndex + " engines added were added, max is 4.");
			}

			m_Engines.Insert(engine);

			m_Expansion_MaximumEngine = m_Engines.Count() + 1;
			return;
		}
	}

	string Expansion_EngineGetName()
	{
		switch (m_Expansion_CurrentEngine)
		{
			case 0:
				return "Car";
			case 1:
				if (IsHelicopter())
					return "Helicopter";
				else if (IsPlane())
					return "Plane";
				else if (IsBoat())
					return "Boat";
				break;
		}
		return "UNKNOWN";
	}

	int Expansion_EngineStartAnimation()
	{
		return Expansion_EngineStartAnimation(m_Expansion_CurrentEngine);
	}

	int Expansion_EngineStopAnimation()
	{
		return Expansion_EngineStopAnimation(m_Expansion_CurrentEngine);
	}

	int Expansion_EngineGetCurrent()
	{
		if (m_Expansion_CurrentEngine < m_Expansion_MinimumEngine)
			m_Expansion_CurrentEngine = m_Expansion_MinimumEngine;
		if (m_Expansion_CurrentEngine >= m_Expansion_MaximumEngine)
			m_Expansion_CurrentEngine = m_Expansion_MinimumEngine;

		return m_Expansion_CurrentEngine;
	}

	int Expansion_EngineGetCount()
	{
		return m_Expansion_MaximumEngine - m_Expansion_MinimumEngine;
	}

	void Expansion_EngineSetNext()
	{
		m_Expansion_CurrentEngine++;
		if (m_Expansion_CurrentEngine >= m_Expansion_MaximumEngine)
			m_Expansion_CurrentEngine = m_Expansion_MinimumEngine;

		SetSynchDirty();
	}

	float Expansion_EngineGetRPMMax()
	{
		return Expansion_EngineGetRPMMax(m_Expansion_CurrentEngine);
	}

	//! Returns engine's maximal working rpm without damaging the engine.
	float Expansion_EngineGetRPMRedline()
	{
		return Expansion_EngineGetRPMRedline(m_Expansion_CurrentEngine);
	}

	//! Returns engine's rpm value.
	float Expansion_EngineGetRPM()
	{
		return Expansion_EngineGetRPM(m_Expansion_CurrentEngine);
	}

	//! Returns true when engine is running, false otherwise.
	bool Expansion_EngineIsOn()
	{
		return Expansion_EngineIsOn(m_Expansion_CurrentEngine);
	}

	//! Starts the engine.
	void Expansion_EngineStart()
	{
		Expansion_EngineStart(m_Expansion_CurrentEngine);
	}

	//! Stops the engine.
	void Expansion_EngineStop()
	{
		Expansion_EngineStop(m_Expansion_CurrentEngine);
	}

	//! Returns engine's max rpm before engine blows up.
	float Expansion_EngineGetRPMMax(int index)
	{
		if (index == 0)
			return EngineGetRPMMax();

		return m_Engines[index].m_RPMMax;
	}

	//! Returns engine's maximal working rpm without damaging the engine.
	float Expansion_EngineGetRPMRedline(int index)
	{
		if (index == 0)
			return EngineGetRPMRedline();

		return m_Engines[index].m_RPMRedline;
	}

	//! Returns engine's rpm value.
	float Expansion_EngineGetRPM(int index)
	{
		if (index == 0)
			return EngineGetRPM();
			
		return m_Engines[index].m_RPM;
	}

	//! Returns true when engine is running, false otherwise.
	bool Expansion_EngineIsOn(int index)
	{
		if (index == 0)
			return EngineIsOn();
		
		return m_Controller.m_State[index];
	}

	//! Starts the engine.
	void Expansion_EngineStart(int index)
	{
		if (index == 0)
		{
			EngineStart();
			return;
		}

		if (!m_Controller.m_State[index] && OnBeforeEngineStart(index))
		{
			m_Controller.m_State[index] = true;

			if (index == 1) m_Expansion_EngineSync1 = true;
			else if (index == 2) m_Expansion_EngineSync2 = true;
			else if (index == 3) m_Expansion_EngineSync3 = true;

			OnEngineStart(index);
		}

		SetSynchDirty();
	}

	/*!
		Is called every time the game wants to start the engine.

		\return true if the engine can start, false otherwise.
	*/
	protected bool OnBeforeEngineStart(int index)
	{
		if ( m_Exploded )
			return false;

		return true;
	}

	//! Is called every time the engine starts.
	protected void OnEngineStart(int index)
	{
		m_Expansion_EnginesOn++;

		if (!m_Expansion_EngineIsOn)
			SetAnimationPhase("EnableMonitor", 0);

		m_Expansion_EngineIsOn = true;

		if (index != 0)
		{
			dBodyActive(this, ActiveState.ACTIVE);

			UpdateLights();
		}

		SetSynchDirty();
	}

	//! Stops the engine.
	void Expansion_EngineStop(int index)
	{
		if (index == 0)
		{
			EngineStop();
			return;
		}

		if (m_Controller.m_State[index])
		{
			m_Controller.m_State[index] = false;

			if (index == 1) m_Expansion_EngineSync1 = false;
			else if (index == 2) m_Expansion_EngineSync2 = false;
			else if (index == 3) m_Expansion_EngineSync3 = false;

			OnEngineStop(index);
		}

		SetSynchDirty();
	}

	//! Is called every time the engine stops.
	protected void OnEngineStop(int index)
	{
		m_Expansion_EnginesOn--;

		if (m_Expansion_EnginesOn <= 0)
		{
			m_Expansion_EnginesOn = 0;

			m_Expansion_EngineIsOn = false;
			SetAnimationPhase( "EnableMonitor", -1 );
		}

		SetSynchDirty();
	}

	int Expansion_EnginesOn()
	{
		return m_Expansion_EnginesOn;
	}

	int Expansion_EngineStartAnimation(int index)
	{
		if (IsPlane())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		if (IsBoat())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		if (IsHelicopter())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;

		return DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
	}

	int Expansion_EngineStopAnimation(int index)
	{
		if (IsPlane())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		if (IsBoat())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		if (IsHelicopter())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;

		return DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
	}

	// ------------------------------------------------------------
	void DBGAddShape( Shape shape )
	{
		#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_DebugShapes.Insert( shape );
		#endif
	}

	vector DBGFixDebugPosition(inout vector position)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		position = position.Multiply4(m_DbgTransform);
#endif

		return position;
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
	// OBSOLETE
	// ------------------------------------------------------------	
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	void TempReadSkin(ParamsReadContext ctx)
	{
		string m_CurrentSkinNameTEMP;
		Expansion_Assert_False( ctx.Read( m_CurrentSkinNameTEMP ), "[" + this + "] Failed reading m_CurrentSkinNameTEMP" );
	}

	// ------------------------------------------------------------
	// OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::OnStoreSave " + this + " " + GetGame().SaveVersion());
		#endif

		//! If we are saving after game version target for ModStorage support (1st stable)
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		//! If we are saving game version target for ModStorage support (1st stable) or later
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_PersistentIDA );
		ctx.Write( m_PersistentIDB );
		ctx.Write( m_PersistentIDC );
		ctx.Write( m_PersistentIDD );

		int lockState = m_VehicleLockedState;
		ctx.Write( lockState );

		ctx.Write( m_Exploded );
		
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

		ctx.Write( m_CurrentSkinName );
	}
	
	/**
	 * The following code remains even though unused for backwards compatibility
	 */
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::OnStoreLoad " + this + " " + version);
		#endif

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		//! With CF_ModStorage enabled, the code below won't be ran unless an old CE is loaded. To prevent server wipes, the code below will stay.

		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - Start");
		#endif

		//! doubled data accidentally for version 15
		if ( GetExpansionSaveVersion() == 15 && Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
		{
			return false;
		}

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

		if ( GetExpansionSaveVersion() < 15 )
			TempReadSkin( ctx );
		
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

			m_IsBeingTowed = false;
			m_IsTowing = false;

			m_ParentTowPersistentIDA = 0;
			m_ParentTowPersistentIDB = 0;
			m_ParentTowPersistentIDC = 0;
			m_ParentTowPersistentIDD = 0;
			m_ChildTowPersistentIDA = 0;
			m_ChildTowPersistentIDB = 0;
			m_ChildTowPersistentIDC = 0;
			m_ChildTowPersistentIDD = 0;
		}
		
		SetSynchDirty();

		//! doubled data accidentally for version 15
		if ( GetExpansionSaveVersion() == 15 )
		{
			if ( Expansion_Assert_False( ctx.Read( m_PersistentIDA ), "[" + this + "] Failed reading m_PersistentIDA" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_PersistentIDB ), "[" + this + "] Failed reading m_PersistentIDB" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_PersistentIDC ), "[" + this + "] Failed reading m_PersistentIDC" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_PersistentIDD ), "[" + this + "] Failed reading m_PersistentIDD" ) )
				return false;
			
			if ( Expansion_Assert_False( ctx.Read( lockState ), "[" + this + "] Failed reading lockState" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_Exploded ), "[" + this + "] Failed reading m_Exploded" ) )
				return false;

			if ( GetExpansionSaveVersion() < 15 )
				TempReadSkin( ctx );
			
			if ( Expansion_Assert_False( ctx.Read( m_Orientation ), "[" + this + "] Failed reading m_Orientation" ) )
				return false;
				
			if ( Expansion_Assert_False( ctx.Read( m_Position ), "[" + this + "] Failed reading m_Position" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_allAttachments ), "[" + this + "] Failed reading m_allAttachments" ) )
				return false;

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

			#ifdef EXPANSION_CARSCRIPT_LOGGING
			EXLogPrint("CarScript::OnStoreLoad - End");
			#endif
			return true;
		}

		m_IsBeingTowed = false;
		m_IsTowing = false;

		m_ParentTowPersistentIDA = 0;
		m_ParentTowPersistentIDB = 0;
		m_ParentTowPersistentIDC = 0;
		m_ParentTowPersistentIDD = 0;
		m_ChildTowPersistentIDA = 0;
		m_ChildTowPersistentIDB = 0;
		m_ChildTowPersistentIDC = 0;
		m_ChildTowPersistentIDD = 0;

		if ( GetExpansionSaveVersion() < 21 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;
		
		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - End");
		#endif

		return true;
	}
	
	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::CF_OnStoreSave " + this + " " + modName);
		#endif

		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Vehicles" )
			return;

		storage.Write( m_PersistentIDA );
		storage.Write( m_PersistentIDB );
		storage.Write( m_PersistentIDC );
		storage.Write( m_PersistentIDD );

		int lockState = m_VehicleLockedState;
		storage.Write( lockState );

		storage.Write( m_Exploded );

		//storage.Write( m_CurrentSkinName );
		
		GetCurrentOrientation();
		storage.Write( m_Orientation );	
		
		GetCurrentPosition();
		storage.Write( m_Position );

		storage.Write( m_IsBeingTowed );
		storage.Write( m_IsTowing );

		if ( m_IsBeingTowed )
		{
			storage.Write( m_ParentTowPersistentIDA );
			storage.Write( m_ParentTowPersistentIDB );
			storage.Write( m_ParentTowPersistentIDC );
			storage.Write( m_ParentTowPersistentIDD );
		}

		if ( m_IsTowing )
		{
			storage.Write( m_ChildTowPersistentIDA );
			storage.Write( m_ChildTowPersistentIDB );
			storage.Write( m_ChildTowPersistentIDC );
			storage.Write( m_ChildTowPersistentIDD );
		}

		storage.Write( m_CurrentSkinName );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::CF_OnStoreLoad " + this + " " + modName);
		#endif

		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Vehicles" )
			return true;

		m_ExpansionSaveVersion = storage.GetVersion();

		if ( Expansion_Assert_False( storage.Read( m_PersistentIDA ), "[" + this + "] Failed reading m_PersistentIDA" ) )
			return false;

		if ( Expansion_Assert_False( storage.Read( m_PersistentIDB ), "[" + this + "] Failed reading m_PersistentIDB" ) )
			return false;

		if ( Expansion_Assert_False( storage.Read( m_PersistentIDC ), "[" + this + "] Failed reading m_PersistentIDC" ) )
			return false;

		if ( Expansion_Assert_False( storage.Read( m_PersistentIDD ), "[" + this + "] Failed reading m_PersistentIDD" ) )
			return false;
		
		int lockState;
		if ( Expansion_Assert_False( storage.Read( lockState ), "[" + this + "] Failed reading lockState" ) )
			return false;

		m_VehicleLockedState = lockState;

		if ( Expansion_Assert_False( storage.Read( m_Exploded ), "[" + this + "] Failed reading m_Exploded" ) )
			return false;

		//if ( Expansion_Assert_False( storage.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
		//	return false;
		
		if ( Expansion_Assert_False( storage.Read( m_Orientation ), "[" + this + "] Failed reading m_Orientation" ) )
			return false;
			
		if ( Expansion_Assert_False( storage.Read( m_Position ), "[" + this + "] Failed reading m_Position" ) )
			return false;

		if ( GetExpansionSaveVersion() >= 7 )
		{
			if ( Expansion_Assert_False( storage.Read( m_IsBeingTowed ), "[" + this + "] Failed reading m_IsBeingTowed" ) )
				return false;
			if ( Expansion_Assert_False( storage.Read( m_IsTowing ), "[" + this + "] Failed reading m_IsTowing" ) )
				return false;

			if ( m_IsBeingTowed )
			{
				if ( Expansion_Assert_False( storage.Read( m_ParentTowPersistentIDA ), "[" + this + "] Failed reading m_ParentTowPersistentIDA" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ParentTowPersistentIDB ), "[" + this + "] Failed reading m_ParentTowPersistentIDB" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ParentTowPersistentIDC ), "[" + this + "] Failed reading m_ParentTowPersistentIDC" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ParentTowPersistentIDD ), "[" + this + "] Failed reading m_ParentTowPersistentIDD" ) )
					return false;
			}

			if ( m_IsTowing )
			{
				if ( Expansion_Assert_False( storage.Read( m_ChildTowPersistentIDA ), "[" + this + "] Failed reading m_ChildTowPersistentIDA" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ChildTowPersistentIDB ), "[" + this + "] Failed reading m_ChildTowPersistentIDB" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ChildTowPersistentIDC ), "[" + this + "] Failed reading m_ChildTowPersistentIDC" ) )
					return false;
				if ( Expansion_Assert_False( storage.Read( m_ChildTowPersistentIDD ), "[" + this + "] Failed reading m_ChildTowPersistentIDD" ) )
					return false;
			}
		}

		if ( GetExpansionSaveVersion() < 21 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		storage.Read( m_CurrentSkinName );

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;

		return true;
	}
	#endif

	// ------------------------------------------------------------
	override void EEOnAfterLoad()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("CarScript::EEOnAfterLoad - Start " + GetType() + " " + GetPosition());
		#endif

		super.EEOnAfterLoad();
		
		if ( m_IsTowing && !m_ChildTow )
		{
			m_ChildTow = GetGame().GetEntityByPersitentID( m_ChildTowPersistentIDA, m_ChildTowPersistentIDB, m_ChildTowPersistentIDC, m_ChildTowPersistentIDD );
			if ( m_ChildTow )
			{
				m_TowPointCenter = GetTowCenterPosition( m_ChildTow );
				
				CarScript cs_child;
				ExpansionVehicleBase evs_child;
				
				if ( Class.CastTo( cs_child, m_ChildTow ) )
					cs_child.OnTowCreated( this, m_TowPointCenter );
				else if ( Class.CastTo( evs_child, m_ChildTow ) )
					evs_child.OnTowCreated( this, m_TowPointCenter );
			} else
			{
				m_IsTowing = false;
			}
		}

		#ifdef EXPANSION_VEHICLE_SKIN_LOGGING
		Print( m_CanBeSkinned );
		Print( m_CurrentSkinName );
		#endif

		if ( m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = ExpansionSkin.Cast( m_Skins[ m_CurrentSkinIndex ] );

			ExpansionOnSkinUpdate();
		}
		#ifdef EXPANSION_VEHICLE_SKIN_LOGGING
		Print( m_CurrentSkinIndex );
		Print( m_CurrentSkin );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print("CarScript::EEOnAfterLoad - End");
		#endif
	}

	// ------------------------------------------------------------
	override void OnEngineStop()
	{
		super.OnEngineStop();

		//! Something (probably vanilla?) is calling OnEngineStop in a loop on client, EVEN IF ENGINE IS RUNNING WHYYY WTF
		//! Prevent this by checking netsynched var that tells us if engine is really stopped or not
		if ( GetGame().IsClient() && m_Expansion_EngineIsOn )
			return;

		OnEngineStop(0);
	}

	// ------------------------------------------------------------
	override void OnEngineStart()
	{
		super.OnEngineStart();

		OnEngineStart(0);
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

		m_Controller.m_State[0] = EngineIsOn();

		if (m_Controller.m_State[1] != m_Expansion_EngineSync1)
		{
			m_Controller.m_State[1] = m_Expansion_EngineSync1;

			if (m_Expansion_EngineSync1)
			{
				OnEngineStart(1);
			}
			else
			{
				OnEngineStop(1);
			}
		}

		if (m_Controller.m_State[2] != m_Expansion_EngineSync2)
		{
			m_Controller.m_State[2] = m_Expansion_EngineSync2;

			if (m_Expansion_EngineSync2)
			{
				OnEngineStart(2);
			}
			else
			{
				OnEngineStop(2);
			}
		}

		if (m_Controller.m_State[3] != m_Expansion_EngineSync3)
		{
			m_Controller.m_State[3] = m_Expansion_EngineSync3;

			if (m_Expansion_EngineSync3)
			{
				OnEngineStart(3);
			}
			else
			{
				OnEngineStop(3);
			}
		}

		if ( IsSoundSynchRemote() && !m_HornPlaying )
		{
			m_HornPlaying = true;
			OnHornSoundPlay();
		} else if ( !IsSoundSynchRemote() && m_HornPlaying )
		{
			m_HornPlaying = false;
			OnHornSoundStop();
		}

		if ( m_ExplodedSynchRemote && !m_Exploded ) 
		{
			ExpansionOnSpawnExploded();
		}

		if ( m_IsBeingTowed )
		{
			m_ParentTow = EntityAI.Cast( GetGame().GetObjectByNetworkId( m_ParentTowNetworkIDLow, m_ParentTowNetworkIDHigh ) );
		} else
		{
			m_ParentTow = NULL;
		}

		if ( m_IsTowing )
		{
			m_ChildTow = EntityAI.Cast( GetGame().GetObjectByNetworkId( m_ChildTowNetworkIDLow, m_ChildTowNetworkIDHigh ) );
		} else
		{
			m_ChildTow = NULL;
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

	override array< ExpansionSkin > ExpansionGetSkins()
	{
		return m_Skins;
	}

	override bool ExpansionHasSkin( int skinIndex )
	{
		return skinIndex > -1 && skinIndex < m_Skins.Count();
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

	override void ExpansionSetSkin( int skinIndex )
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
		EXPrint(ToString() + "::IsSurfaceWater is deprecated, use ExpansionStatic::SurfaceIsWater");
		return ExpansionStatic.SurfaceIsWater(position);
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
		
		//! There is no need for this now as no wild car spawns with a key rn?!
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
	float GetCameraHeight()
	{
		return 1.3;
	}

	// ------------------------------------------------------------
	float GetCameraDistance()
	{
		return 4.5;
	}

	float GetModelAnchorPointY()
	{
		if ( m_ModelAnchorPointY < 0 )
		{
			string path = "CfgVehicles " + GetType() + " modelAnchorPointY";
			if ( GetGame().ConfigIsExisting( path ) )
				m_ModelAnchorPointY = GetGame().ConfigGetFloat( path );
			else
				m_ModelAnchorPointY = 0.0;
			#ifdef EXPANSIONEXPRINT
			EXPrint(GetType() + " modelAnchorPointY " + m_ModelAnchorPointY);
			#endif
		}

		return m_ModelAnchorPointY;
	}
	
	// ------------------------------------------------------------
	override float GetTransportCameraDistance()
	{
		return GetCameraDistance();
	}
	
	// ------------------------------------------------------------
	override vector GetTransportCameraOffset()
	{
		return Vector( 0, GetCameraHeight(), 0 );
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

		ExpansionCheckTreeContact(other, data.Impulse);

		#ifdef DAYZ_1_13
		if ( GetGame().IsServer() )
		{
			array<ref CarContactData> ccd;
			if (!m_ContactCache.Find(zoneName, ccd))
			{
				ccd = new array<ref CarContactData>;
				m_ContactCache.Insert(zoneName, ccd);
			}
			
			ccd.Insert(new CarContactData(localPos, other, data.Impulse));

			data.Impulse = 0;
		}
		#endif

		super.OnContact(zoneName, localPos, other, data);
	}
	
	void ExpansionCheckTreeContact(IEntity other, float impulse)
	{
		Object tree = Object.Cast( other );
		if ( tree )
		{
			if ( tree.IsTree() || tree.IsBush() )
			{
				EXPrint(ToString() + " " + GetPosition() + " hit tree, impulse: " + impulse);

				if ( impulse > 7500 )
				{
					EntityAI cutting_tool = EntityAI.Cast( GetGame().CreateObject("WoodAxe", vector.Zero, false, true) );

					if ( IsMissionClient() )
						SoundHardTreeFallingPlay();

					if ( GetGame().IsServer() )
						tree.DecreaseHealth("", "", 100, true);	

					tree.OnTreeCutDown( cutting_tool );

					dBodyDestroy(tree);

					GetGame().ObjectDelete(cutting_tool);
				}
			}
		}
	}

	#ifdef DAYZ_1_13
	void CheckContactCache()
	#else
	override void CheckContactCache()
	#endif
	{
		int contactZonesCount = m_ContactCache.Count();
		
		if ( contactZonesCount == 0 )
			return;
		
		bool playLightSound = false;
		bool playHeavySound = false;
		
		for ( int i = 0; i < contactZonesCount; ++i)
		{
			string zoneName = m_ContactCache.GetKey(i);
			array<ref CarContactData> data = m_ContactCache[zoneName];

			float dmg;
			
			int contactCount = data.Count();
			for ( int j = 0; j < contactCount; ++j )
				dmg = data[j].impulse * m_dmgContactCoef;
			
			#ifdef DAYZ_1_13
			if ( dmg < 150.0 )
			#else
			if ( dmg < GameConstants.CARS_CONTACT_DMG_MIN )
			#endif
				continue;
			
			int pddfFlags;

			#ifdef DAYZ_1_13
			if ( dmg < 750.0 )
			#else
			if ( dmg < GameConstants.CARS_CONTACT_DMG_THRESHOLD )
			#endif
			{				
				playLightSound = true;
				pddfFlags = ProcessDirectDamageFlags.NO_TRANSFER;
			}
			else
			{		
				DamageCrew(dmg * GetExpansionSettings().GetVehicle().VehicleCrewDamageMultiplier);
				playHeavySound = true;			
				pddfFlags = 0;
			}
			
			if ( CanBeDamaged() )
				ProcessDirectDamage( DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", dmg * GetExpansionSettings().GetVehicle().VehicleSpeedDamageMultiplier, pddfFlags );
		}
		
		if (playLightSound)
			SynchCrashLightSound( true );
		
		if (playHeavySound)
			SynchCrashHeavySound( true );
		
		UpdateHeadlightState();
		UpdateLights();
		
		m_ContactCache.Clear();

		#ifndef DAYZ_1_13
		super.CheckContactCache();
		#endif
	}

	#ifdef DAYZ_1_13
	void DamageCrew(float dmg)
	{
		for ( int c = 0; c < CrewSize(); ++c )
		{
			Human crew = CrewMember( c );
			if ( !crew )
				continue;

			PlayerBase player;
			if ( Class.CastTo(player, crew ) )
			{
				if ( dmg > 3000.0 )
				{		
					player.SetHealth(0.0);
				}
				else
				{
					float shockTemp = Math.InverseLerp(150.0, 3000.0, dmg);
					float shock = Math.Lerp( 50, 100, shockTemp );
					float hp = Math.Lerp( 2, 60, shockTemp );

					// These should ideally be ProcessDirectDamage...
					player.AddHealth("", "Shock", -shock );
					player.AddHealth("", "Health", -hp );
					//Print( "SHOCK..........." + shock );
					//Print( "HEALTH..........." + hp );
				}
			}
		}
	}
	#endif
	
	// ------------------------------------------------------------
	override float GetBatteryConsumption()
	{
		//! Should prevent towed cars to consume the battery
		if ( IsBeingTowed() )
			return 0;

		return m_BatteryConsume;
	}
};

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
};