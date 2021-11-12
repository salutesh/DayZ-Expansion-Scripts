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

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	private static ref array<ExpansionVehicleAttachmentSave> m_allAttachments = new array<ExpansionVehicleAttachmentSave>;

	ref array<ref ExpansionVehicleModule> m_Modules = new array<ref ExpansionVehicleModule>();
	ref array<ExpansionVehicleAxle> m_Axles = new array<ExpansionVehicleAxle>();
	ref array<ExpansionVehicleWheel> m_Wheels = new array<ExpansionVehicleWheel>();
	ref array<ExpansionVehicleAerofoil> m_Aerofoils = new array<ExpansionVehicleAerofoil>();
	ref array<ExpansionVehicleEngineBase> m_Engines = new array<ExpansionVehicleEngineBase>();
	ref array<ExpansionVehicleGearbox> m_Gearboxes = new array<ExpansionVehicleGearbox>();

	ref ExpansionController m_Controller = new ExpansionController();
	ref ExpansionPhysicsState m_State = new ExpansionPhysicsStateT<CarScript>(this);

	ref ExpansionVehicleModuleEvent m_Event_TEMP_DeferredInit = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_SettingsChanged = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Control = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_PreSimulate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Simulate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Animate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_NetworkSend = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_NetworkRecieve = new ExpansionVehicleModuleEvent();

	int m_Expansion_EnginesOn;

	// ------------------------------------------------------------
	//! Constant Values - Set in Constructor, Errors occur if not.
	// ------------------------------------------------------------
	protected float m_MaxSpeed; // (km/h)
	protected float m_MaxSpeedMS; // (m/s)

	protected float m_AltitudeFullForce; // (m)
	protected float m_AltitudeNoForce; // (m)

	protected float m_Expansion_Mass;

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
	protected vector m_Expansion_TowPointCenter;
	protected int m_Expansion_TowConnectionIndex;
	protected bool m_Expansion_IsBeingTowed;
	protected bool m_Expansion_IsTowing;

	protected EntityAI m_Expansion_ParentTow;
	protected int m_Expansion_ParentTowNetworkIDLow;
	protected int m_Expansion_ParentTowNetworkIDHigh;
	protected int m_Expansion_ParentTowPersistentIDA;
	protected int m_Expansion_ParentTowPersistentIDB;
	protected int m_Expansion_ParentTowPersistentIDC;
	protected int m_Expansion_ParentTowPersistentIDD;

	protected EntityAI m_Expansion_ChildTow;
	protected int m_Expansion_ChildTowNetworkIDLow;
	protected int m_Expansion_ChildTowNetworkIDHigh;
	protected int m_Expansion_ChildTowPersistentIDA;
	protected int m_Expansion_ChildTowPersistentIDB;
	protected int m_Expansion_ChildTowPersistentIDC;
	protected int m_Expansion_ChildTowPersistentIDD;

	// Physics
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
	vector m_DbgTransform[4];
#endif

	protected bool m_IsPhysicsHost;
	bool m_Expansion_AcceptingAttachment;

	protected float m_BoundingRadius;
	protected vector m_BoundingBox[2];

	protected bool m_CarBatteryVanillaState;
	protected bool m_CarBatteryVanillaStateDefault;

	protected Particle m_SmokeParticle;

	// Lights
	ref array<ref ExpansionPointLight> m_Lights;
	ref array<ref Particle> m_Particles;

	protected vector m_Orientation;
	protected vector m_Position;

	protected autoptr TStringArray m_Doors;
	protected bool m_CanHaveLock;

	private EffectSound m_SoundLock;

	protected bool m_Expansion_EngineIsOn;

	//! Settings
	protected bool m_TowingEnabled;

	//! Debugging
	private ref array<Shape> m_DebugShapes;

	//! Skinning
	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array<ExpansionSkin> m_Skins;

	protected float m_ModelAnchorPointY = -1;

	protected bool m_Expansion_CanPlayerAttach;
	protected bool m_Expansion_CanPlayerAttachSet;

	protected bool m_Expansion_EngineSync1;
	protected bool m_Expansion_EngineSync2;
	protected bool m_Expansion_EngineSync3;

	protected bool m_IsStoreLoaded;
	protected bool m_IsCECreated;

	float m_Expansion_FuelConsumptionPerTick;

	bool m_Expansion_dBodyIsActive;  //! Used for debugging. PLEASE leave this in here.

	void CarScript()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
#endif

		SetEventMask(EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT);

		RegisterNetSyncVariableInt("m_PersistentIDA");
		RegisterNetSyncVariableInt("m_PersistentIDB");
		RegisterNetSyncVariableInt("m_PersistentIDC");
		RegisterNetSyncVariableInt("m_PersistentIDD");
		RegisterNetSyncVariableInt("m_VehicleLockedState");

		RegisterNetSyncVariableInt("m_Expansion_CurrentEngine");

		RegisterNetSyncVariableBool("m_Expansion_AcceptingAttachment");

		RegisterNetSyncVariableBool("m_Expansion_IsBeingTowed");
		RegisterNetSyncVariableBool("m_Expansion_IsTowing");
		RegisterNetSyncVariableInt("m_Expansion_TowConnectionIndex");
		RegisterNetSyncVariableInt("m_Expansion_ParentTowNetworkIDLow");
		RegisterNetSyncVariableInt("m_Expansion_ParentTowNetworkIDHigh");
		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDLow");
		RegisterNetSyncVariableInt("m_Expansion_ChildTowNetworkIDHigh");

		RegisterNetSyncVariableBool("m_HornSynchRemote");

		RegisterNetSyncVariableBool("m_Expansion_EngineIsOn");

		m_DebugShapes = new array<Shape>();

		m_Lights = new array<ref ExpansionPointLight>;
		m_Particles = new array<ref Particle>;

		m_Doors = new TStringArray;
		ConfigGetTextArray("doors", m_Doors);

		m_CanHaveLock = m_Doors.Count() > 0;

		LoadConstantVariables();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LongDeferredInit, 1000);

		ExpansionSettings.SI_Vehicle.Insert(OnSettingsUpdated);

		RegisterNetSyncVariableInt("m_CurrentSkinSynchRemote");

		Class.CastTo(m_SkinModule, GetModuleManager().GetModule(ExpansionSkinModule));

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
			AddModule(Expansion_CreateEngine(this, enginePath));
		}

		if (m_Engines.Count() == 0 && (!IsCar() || (IsCar() && IsBoat())))
		{
			AddModule(Expansion_CreateEngine(this, "CfgVehicles " + GetType() + " SimulationModule Engine"));
		}

		if (m_Gearboxes.Count() == 0)
		{
			path = "CfgVehicles " + GetType() + " SimulationModule Gearbox";
			AddModule(Expansion_CreateGearbox(this, path));
		}

		m_Expansion_FuelConsumptionPerTick = GameConstants.CARS_FLUIDS_TICK * GetGame().ConfigGetFloat("CfgVehicles " + GetType() + " fuelConsumption") / 3600;

		//! This exists so it can be overridden (e.g.) by server owners who don't have access to unbinarized models
		path = "CfgVehicles " + GetType() + " mass";
		if (GetGame().ConfigIsExisting(path))
			m_Expansion_Mass = GetGame().ConfigGetFloat(path);
		else
			m_Expansion_Mass = dBodyGetMass(this);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - End");
#endif
	}

	void ~CarScript()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - Start");
#endif

		int i;

		if (IsMissionClient())
		{
			if (m_SmokeParticle)
			{
				m_SmokeParticle.Stop();
			}

			for (i = 0; i < m_Lights.Count(); i++)
			{
				m_Lights[i].ExpansionSetEnabled(false);

				GetGame().ObjectDelete(m_Lights[i]);
			}

			for (i = 0; i < m_Particles.Count(); i++)
			{
				m_Particles[i].Stop();

				GetGame().ObjectDelete(m_Particles[i]);
			}
		}

		ExpansionSettings.SI_Vehicle.Remove(OnSettingsUpdated);

		//RemoveServerMarker();

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();
#endif

		delete m_DebugShapes;

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - End");
#endif
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_IsStoreLoaded = true;
	}

	void LoadConstantVariables()
	{
	}

	void OnAfterLoadConstantVariables()
	{
		m_Event_TEMP_DeferredInit.TEMP_DeferredInit();
	}

	void LongDeferredInit()
	{
		OnSettingsUpdated();
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		int i;

		for (i = 0; i < m_Modules.Count(); i++)
		{
			m_Modules[i].Init();

			if (m_Modules[i].m_TEMP_DeferredInit)
				m_Event_TEMP_DeferredInit.Add(m_Modules[i]);

			if (m_Modules[i].m_SettingsChanged)
				m_Event_SettingsChanged.Add(m_Modules[i]);

			if (m_Modules[i].m_Control)
				m_Event_Control.Add(m_Modules[i]);

			if (m_Modules[i].m_PreSimulate)
				m_Event_PreSimulate.Add(m_Modules[i]);

			if (m_Modules[i].m_Simulate)
				m_Event_Simulate.Add(m_Modules[i]);

			if (m_Modules[i].m_Animate)
				m_Event_Animate.Add(m_Modules[i]);

			if (m_Modules[i].m_Network)
			{
				m_Event_NetworkSend.Add(m_Modules[i]);
				m_Event_NetworkRecieve.Add(m_Modules[i]);
			}
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

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnAfterLoadConstantVariables, 100, false);

		m_Expansion_dBodyIsActive = dBodyIsActive(this);

		if (GetGame().IsServer() && m_IsStoreLoaded)
		{
			//! Setting state to inactive fixes issues with vehicles being simulated at server start (jumpy helis, boats being always active when in water, not needed for cars)
			if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionBoatScript))
				dBodyActive(this, ActiveState.INACTIVE);

			SetSynchDirty();
		}
	}

	void OnSettingsUpdated()
	{
		m_TowingEnabled = GetExpansionSettings().GetVehicle().Towing;

		m_Event_SettingsChanged.SettingsChanged();
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

		AddAction(ExpansionActionPairKey);
		AddAction(ExpansionActionAdminUnpairKey);

		AddAction(ExpansionActionLockVehicle);
		AddAction(ExpansionActionUnlockVehicle);
	}

	void Expansion_CreateTow(Object tow, int index)
	{
		if (m_Expansion_IsTowing)
			return;

		CarScript car;
		ItemBase item;

		if (!Class.CastTo(car, tow) && !Class.CastTo(item, tow))
			return;

		if ((item && item.Expansion_IsBeingTowed()) || (car && car.Expansion_IsBeingTowed()) || !IsMissionHost())
			return;

		if (car)
			m_Expansion_IsTowing = car.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), index);
		else if (item)
			m_Expansion_IsTowing = item.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), index);

		if (m_Expansion_IsTowing)
		{
			Class.CastTo(m_Expansion_ChildTow, tow);

			if (!IsMissionOffline())
			{
				m_Expansion_ChildTow.GetNetworkID(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh);
			}
		}

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool Expansion_OnTowCreated(Object parent, vector towPos, int index)
	{
		if (index < 0 || index >= Expansion_NumberTowConnections())
			return false;

		vector connectionPoint;
		vector connectionSize;

		Expansion_GetTowConnection(index, connectionPoint, connectionSize);

		m_Expansion_TowPointCenter = towPos - connectionPoint;

		m_Expansion_TowConnectionIndex = index;
		m_Expansion_ParentTow = EntityAI.Cast(parent);
		m_Expansion_IsBeingTowed = true;

		if (!IsMissionOffline())
			m_Expansion_ParentTow.GetNetworkID(m_Expansion_ParentTowNetworkIDLow, m_Expansion_ParentTowNetworkIDHigh);

		if (GetGame().IsServer())
			SetSynchDirty();

		return true;
	}

	EntityAI Expansion_GetTowedEntity()
	{
		if (m_Expansion_IsTowing)
			return m_Expansion_ChildTow;

		return NULL;
	}

	void Expansion_DestroyTow()
	{
		if (!m_Expansion_IsTowing)
			return;

		CarScript cs;
		ItemBase evs;

		if (Class.CastTo(cs, m_Expansion_ChildTow))
		{
			cs.Expansion_OnTowDestroyed();
		}

		if (Class.CastTo(evs, m_Expansion_ChildTow))
		{
			evs.Expansion_OnTowDestroyed();
		}

		m_Expansion_ChildTow = NULL;
		m_Expansion_IsTowing = false;

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	void Expansion_OnTowDestroyed()
	{
		m_Expansion_ParentTow = null;
		m_Expansion_IsBeingTowed = false;

		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool Expansion_IsBeingTowed()
	{
		return m_Expansion_IsBeingTowed;
	}

	bool Expansion_IsTowing()
	{
		return m_Expansion_IsTowing;
	}

	int Expansion_NumberTowConnections()
	{
		return 1;
	}

	void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
		vector minMax[2];
		GetCollisionBox(minMax);

		position = Vector(0.0, 0.0, minMax[1][2] + Expansion_GetTowLength());
		position[0] = dBodyGetCenterOfMass(this)[0];
		position[2] = position[2] - dBodyGetCenterOfMass(this)[2];

		size = "0.5 0.5 0.5";
	}

	bool Expansion_GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		index = -1;

		towPosition = WorldToModel(towPosition);

		for (int i = 0; i < Expansion_NumberTowConnections(); i++)
		{
			vector conPos, conSize;
			Expansion_GetTowConnection(i, conPos, conSize);

			if (Math3D.IntersectSphereBox(towPosition, towRadius, conPos - conSize, conPos + conSize))
			{
				index = i;
				return true;
			}
		}

		return false;
	}

	vector Expansion_GetTowPosition()
	{
		vector minMax[2];
		GetCollisionBox(minMax);
		return Vector(0.0, minMax[0][1], minMax[0][2] - dBodyGetCenterOfMass(this)[2]);
	}

	vector Expansion_GetTowDirection()
	{
		return -GetDirection();
	}

	float Expansion_GetTowLength()
	{
		return 0.4;
	}

	bool Expansion_CanPlayerAttach()
	{
#ifdef EXPANSION_PLAYER_ATTACHMENT_CANATTACH_OVERRIDE
		m_Expansion_CanPlayerAttach = true;
#else
		if (!m_Expansion_CanPlayerAttachSet)
		{
			m_Expansion_CanPlayerAttachSet = true;
			foreach (ExpansionVehiclesConfig vehcfg : GetExpansionSettings().GetVehicle().VehiclesConfig)
			{
				if (IsKindOf(vehcfg.ClassName))
				{
					m_Expansion_CanPlayerAttach = vehcfg.CanPlayerAttach;
					break;
				}
			}
		}
#endif

		return m_Expansion_CanPlayerAttach;
	}

	/**
	 * @brief is it a car ? Is it already towing something ? And is it locked ?
	 */
	bool Expansion_CanConnectTow(notnull Object other)
	{
		ItemBase item;
		ExpansionVehicleBase evs;
		CarScript cs;
		if (Class.CastTo(evs, other))
		{
			return evs.Expansion_NumberTowConnections() > 0 && evs.Expansion_IsCar() && !evs.Expansion_IsTowing() && !evs.IsLocked();
		}
		else if (Class.CastTo(cs, other))
		{
			return cs.Expansion_NumberTowConnections() > 0 && cs.Expansion_IsCar() && !cs.Expansion_IsTowing() && !cs.IsLocked();
		}
		else if (Class.CastTo(item, other))
		{
			return item.Expansion_NumberTowConnections() > 0 && !item.Expansion_IsTowing();
		}

		//! don't...
		return false;
	}

	float Expansion_GetMass()
	{
		return m_Expansion_Mass;
	}

	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	protected void KeyMessage(string message)
	{
#ifdef EXPANSION_CARKEY_LOGGING
		if (IsMissionClient())
		{
			Message(GetPlayer(), message);

			Print(message);
		}
		else
		{
			Print(message);
		}
#endif
	}

	bool IsLocked()
	{
		if (HasKey())
		{
			KeyMessage("CarScript::IsLocked HasKey() true and " + (m_VehicleLockedState == ExpansionVehicleLockState.LOCKED));
			return m_VehicleLockedState == ExpansionVehicleLockState.LOCKED;
		}

		KeyMessage("CarScript::IsLocked false");
		return false;
	}

	int GetPersistentIDA()
	{
		return m_PersistentIDA;
	}

	int GetPersistentIDB()
	{
		return m_PersistentIDB;
	}

	int GetPersistentIDC()
	{
		return m_PersistentIDC;
	}

	int GetPersistentIDD()
	{
		return m_PersistentIDD;
	}

	void SetPersistentIDA(int newIDA)
	{
		m_PersistentIDA = newIDA;
	}

	void SetPersistentIDB(int newIDB)
	{
		m_PersistentIDB = newIDB;
	}

	void SetPersistentIDC(int newIDC)
	{
		m_PersistentIDC = newIDC;
	}

	void SetPersistentIDD(int newIDD)
	{
		m_PersistentIDD = newIDD;
	}

	void SetLockedState(ExpansionVehicleLockState newLockState)
	{
		m_VehicleLockedState = newLockState;
	}

	bool HasKey()
	{
		return m_VehicleLockedState != ExpansionVehicleLockState.NOLOCK;
	}

	void PairKeyTo(ExpansionCarKey key)
	{
#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::PairKeyTo - Start");
#endif

		if (key)
		{
			key.PairToVehicle(this);

			m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;

			SetSynchDirty();
		}

		KeyMessage("PairKeyTo (" + this + ", " + key + ")");

#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::PairKeyTo - End");
#endif
	}

	/**
	 * @brief Only call this after all keys have been confirmed to be removed
	 */
	void ResetKeyPairing()
	{
		if (IsMissionHost())
		{
			m_PersistentIDA = 0;
			m_PersistentIDB = 0;
			m_PersistentIDC = 0;
			m_PersistentIDD = 0;

			m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

			SetSynchDirty();
		}
	}

	bool CanBeLocked()
	{
		return true;
	}

	void OnCarDoorOpened(string source)
	{
		if (HasKey())
		{
			if (m_VehicleLockedState == ExpansionVehicleLockState.READY_TO_LOCK)
			{
				m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
				KeyMessage("OnCarDoorOpened::UNLOCKED");

				SetSynchDirty();
			}
		}
	}

	void OnCarDoorClosed(string source)
	{
	}

	bool IsCarKeys(ExpansionCarKey key)
	{
		if (!HasKey())
		{
			KeyMessage("IsCarKeys::HasKey");
			return false;
		}

		if (!key.IsPairedTo(this))
		{
			KeyMessage("IsCarKeys not paired!");
			return false;
		}

		KeyMessage("IsCarKeys is paired");

		return true;
	}

	void LockCar(ExpansionCarKey key)
	{
		KeyMessage("LockCar");
		KeyMessage("key=" + key);
		if (key && !IsCarKeys(key) && !key.IsInherited(ExpansionCarAdminKey))
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.READY_TO_LOCK;
		KeyMessage("LockCar::READY_TO_LOCK");

		SetSynchDirty();
	}

	void UnlockCar(ExpansionCarKey key)
	{
		if (key && !IsCarKeys(key) && !key.IsInherited(ExpansionCarAdminKey))
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
		KeyMessage("UnlockCar::UNLOCKED");

		OnCarUnlocked();
		SetSynchDirty();
	}

	void UnlockCarWithoutKey()
	{
		m_VehicleLockedState = ExpansionVehicleLockState.FORCEDUNLOCKED;

		OnCarUnlocked();
		SetSynchDirty();
	}

	void OnCarLocked()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarLocked - Start");
#endif

		KeyMessage("OnCarLocked");

		if (GetGame().IsServer())
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(this, ExpansionVehicleRPC.PlayLockSound, true, NULL);
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarLocked - Stop");
#endif
	}

	void OnCarUnlocked()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarUnlocked - Start");
#endif

		KeyMessage("OnCarUnlocked");

		if (GetGame().IsServer())
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send(this, ExpansionVehicleRPC.PlayLockSound, true, NULL);
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnCarUnlocked - End");
#endif
	}

	void OnHornSoundPlay()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundPlay - Start");
#endif

		string soundFile = m_HornSoundSetEXT;
		if (GetGame().GetPlayer().IsCameraInsideVehicle())
			soundFile = m_HornSoundSetINT;

		m_HornSound = SEffectManager.PlaySoundOnObject(soundFile, this);
		m_HornSound.SetSoundAutodestroy(true);
		m_HornSound.SetSoundLoop(true);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundPlay - End");
#endif
	}

	void OnHornSoundStop()
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundStop - Start");
#endif

		m_HornSound.SetSoundLoop(false);
		m_HornSound.SoundStop();

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnHornSoundStop - End");
#endif
	}

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

	bool IsSoundSynchRemote()
	{
		return m_HornSynchRemote;
	}

	override void Explode(int damageType, string ammoType = "")
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::Explode - Start");
#endif

		if (GetGame().IsServer() && !m_Exploded)
		{
			if (ammoType == "")
				ammoType = this.ConfigGetString("ammoType");

			if (ammoType == "")
				ammoType = "Dummy_Heavy";

			ExpansionOnExplodeServer(damageType, ammoType);

			if (GetGame().IsServer() && !GetGame().IsMultiplayer())
			{
				ExpansionOnExplodeClient(damageType, ammoType);
			}
			else
			{
				GetGame().RPCSingleParam(this, ERPCs.RPC_EXPLODE_EVENT, new Param2<int, string>(damageType, ammoType), true);
			}
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::Explode - End");
#endif
	}

	/**
	 * @brief Explodes the vehicle on the server, telling the client that they should explode by setting the sync boolean.
	 */
	void ExpansionOnExplodeServer(int damageType, string ammoType)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeServer - Start");
#endif

		m_Exploded = true;
		m_ExplodedSynchRemote = true;

		LeakAll(CarFluid.COOLANT);
		LeakAll(CarFluid.FUEL);
		LeakAll(CarFluid.OIL);
		LeakAll(CarFluid.BRAKE);

		TStringArray dmgZones = new TStringArray;
		GetDamageZones(dmgZones);
		foreach (string dmgZone: dmgZones)
		{
			SetHealth(dmgZone, "", 0);
		}

		EntityAI attachment;
		//! Ruining doors may pop them off, which changes attachment count, so do this in reverse order
		for (int j = GetInventory().AttachmentCount() - 1; j >= 0; j--)
		{
			attachment = GetInventory().GetAttachmentFromIndex(j);
			attachment.SetHealth(0);
		}

		if (GetExpansionSettings().GetVehicle().ExplodingVehicleDropsAttachments)
			Expansion_PhysicsDropAttachments(this);

		for (int i = 0; i < CrewSize(); i++)
		{
			Human crew = CrewMember(i);
			if (!crew)
				continue;

			if (GetGame().IsMultiplayer())
			{
				crew.ProcessDirectDamage(damageType, this, "", ammoType, "0 0 0", crew.GetMaxHealth());
			}
		}

		//! Attached players
		IEntity child = GetChildren();
		EntityAI entity;
		while (child)
		{
			if (Class.CastTo(entity, child))
				entity.ProcessDirectDamage(damageType, this, "", ammoType, "0 0 0", entity.GetMaxHealth());

			child = child.GetSibling();
		}

		dBodyApplyForce(this, Vector(0, 20.0 * m_State.m_Mass, 0));

		ExpansionCreateExplosion(this, "", 2.5, 100);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeServer - End");
#endif
	}

	/**
	 * @brief Explodes the vehicle on the client
	 */
	void ExpansionOnExplodeClient(int damageType, string ammoType)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeClient - Start");
#endif

		StopSounds();

		EffectSound sound = SEffectManager.PlaySound("Expansion_Explosive_Vehicle_SoundSet", GetPosition());
		sound.SetSoundAutodestroy(true);

		vector n = "0 1 0";

		ExpansionPointLight explosionLight = ExpansionPointLight.Cast(ScriptedLightBase.CreateLight(ExpansionPointLight, Vector(0, 0, 0)));

		explosionLight.SetDiffuseColor(1, 0.25, 0);
		explosionLight.SetPosition(GetPosition());
		explosionLight.SetRadiusTo(20 * m_ExplosionSize);
		explosionLight.SetBrightnessTo(4.5);
		explosionLight.SetLifetime(0.1);
		explosionLight.SetFadeOutTime(0.1);
		explosionLight.SetFlareVisible(false);

		vector localPos = GetMemoryPointPos("velka osa");
		Particle p;

		if (IsHelicopter() || IsPlane() || IsBoat())
		{
			int z;

			for (z = 0; z < m_Particles.Count(); z++)
			{
				m_Particles[z].Stop();

				GetGame().ObjectDelete(m_Particles[z]);
			}

			for (z = 0; z < m_Lights.Count(); z++)
			{
				m_Lights[z].ExpansionSetEnabled(false);

				GetGame().ObjectDelete(m_Lights[z]);
			}

			if (GetGame().GetWaterDepth(GetPosition()) < 1)
			{
				if (!m_SmokeParticle)
				{
					m_SmokeParticle = Particle.PlayOnObject(ParticleList.EXPANSION_FIRE_HELICOPTER, this, GetMemoryPointPos("velka osa"), "0 0 0", true);
					m_SmokeParticle.SetOrientation(Vector(0, 1, 0).VectorToAngles());
				}
			}

			if (ExpansionStatic.SurfaceIsWater(GetPosition()))
			{
				p = Particle.PlayInWorld(ParticleList.EXPANSION_EXPLOSION_WATER, ModelToWorld(localPos));
				p.SetOrientation(n);
			}
			else
			{
				p = Particle.PlayInWorld(ParticleList.EXPANSION_EXPLOSION_HELICOPTER, ModelToWorld(localPos));
				p.SetOrientation(n);
			}
		}
		else if (IsBoat())
		{
			p = Particle.PlayInWorld(ParticleList.EXPANSION_EXPLOSION_WATER, ModelToWorld(localPos));
			p.SetOrientation(n);
		}

		float distance_to_player = vector.Distance(GetPosition(), GetGame().GetPlayer().GetPosition());

		float strength_factor = Math.InverseLerp(GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player);

		// GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake( strength_factor * 16.0 );

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnExplodeClient - End");
#endif
	}

	/**
	 * @brief Only ran on the entity entity that was already exploded but the client never knew about it (i.e. outside of network bubble)
	 */
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

	/**
	 * @note Does nothing, vanilla code shall not run
	 */
	override void OnExplodeClient()
	{
	}

	/**
	 * @note Does nothing, vanilla code shall not run
	 */
	override void SynchExplosion()
	{
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnRPC - Start");
#endif

		switch (rpc_type)
		{
		case ExpansionVehicleRPC.ControllerSync:
		{
			if (m_Controller)
			{
				PlayerBase driverBase;
				//! @note sender and driverBase.GetIdentity() will NOT be the same object even if they point to the same player identity (same ID)!
				if (Class.CastTo(driverBase, CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER)) && driverBase.GetIdentityUID() == sender.GetId())
				{
					m_Event_NetworkRecieve.NetworkRecieve(ctx);
				}
			}

			return;
		}
		case ERPCs.RPC_EXPLODE_EVENT:
		{
			if (GetGame().IsClient())
			{
				Param2<int, string> params;

				if (ctx.Read(params))
				{
					ExpansionOnExplodeClient(params.param1, params.param2);
				}
			}

			return;
		}
		case ExpansionVehicleRPC.PlayLockSound:
		{
			if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			{
				if (m_SoundLock)
					delete m_SoundLock;

				m_SoundLock = SEffectManager.PlaySound("Expansion_Car_Lock_SoundSet", GetPosition());
				m_SoundLock.SetSoundAutodestroy(true);
			}
		}
		}

		super.OnRPC(sender, rpc_type, ctx);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnRPC - End");
#endif
	}

	void NetworkSend()
	{
		if (IsMissionOffline())
			return;

		ScriptRPC rpc = new ScriptRPC();

		m_Event_NetworkSend.NetworkSend(rpc);

		if (IsMissionHost())
		{
			Human human = CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
			if (human != NULL)
				rpc.Send(this, ExpansionVehicleRPC.ControllerSync, true, human.GetIdentity());
		}
		else
		{
			rpc.Send(this, ExpansionVehicleRPC.ControllerSync, true, NULL);
		}
	}

	void StopSounds()
	{
	}

	bool IsVitalIgniterPlug()
	{
		return false;
	}

	bool IsVitalHydraulicHoses()
	{
		return false;
	}

	void SetAttachmentHealth(string itemName, float health)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetAttachmentHealth - Start");
#endif
		EntityAI item = FindAttachmentBySlotName(itemName);

		if (item)
			item.SetHealth("", "", health);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::SetAttachmentHealth - End");
#endif
	}

	override EntityAI FindAttachmentBySlotName(string slot_name)
	{
		EntityAI item = NULL;
		if (!m_CarBatteryVanillaStateDefault && m_CarBatteryVanillaState && slot_name == "CarBattery")
		{
			if (IsVitalHelicopterBattery())
				item = super.FindAttachmentBySlotName("ExpansionHelicopterBattery");
			if (IsVitalAircraftBattery())
				item = super.FindAttachmentBySlotName("ExpansionAircraftBattery");
		}
		else
		{
			item = super.FindAttachmentBySlotName(slot_name);
		}

		return item;
	}

	void SetCarBatteryStateForVanilla(bool active)
	{
		if (!m_CarBatteryVanillaState)
		{
			m_CarBatteryVanillaStateDefault = IsVitalCarBattery();
		}

		m_CarBatteryVanillaState = active;
	}

	bool HasBattery()
	{
		ItemBase battery;

		if (IsVitalCarBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		if (!battery)
			return false;

		return true;
	}

	override ItemBase GetBattery()
	{
		if (IsVitalCarBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		return null;
	}

	bool IsBatteryWorking()
	{
		ItemBase battery;

		if (IsVitalCarBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));
		if (IsVitalTruckBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("TruckBattery"));
		if (IsVitalHelicopterBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery())
			battery = ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		if (!battery || battery.IsRuined())
			return false;

		return true;
	}

	bool IsVitalHelicopterBattery()
	{
		return false;
	}

	bool IsVitalAircraftBattery()
	{
		return false;
	}

	bool IsCar()
	{
		return true;
	}

	bool IsBoat()
	{
		return false;
	}

	bool IsHelicopter()
	{
		return false;
	}

	bool IsPlane()
	{
		return false;
	}

	bool IsExploded()
	{
		return m_Exploded;
	}

	bool CanObjectAttach(Object obj)
	{
		return Expansion_CanPlayerAttach();
	}

	bool Expansion_IsPlane()
	{
		return IsPlane();
	}

	bool Expansion_IsBoat()
	{
		return IsBoat();
	}

	bool Expansion_IsHelicopter()
	{
		return IsHelicopter();
	}

	bool Expansion_IsCar()
	{
		return IsCar();
	}

	bool Expansion_CanObjectAttach(Object obj)
	{
		return Expansion_CanPlayerAttach();
	}

	/**
	 * @note only called if CanObjectAttach returns true.
	 */ 
	bool LeavingSeatDoesAttachment(int posIdx)
	{
		return false;
	}

	override bool CanBeDamaged()
	{
		if (GetExpansionSettings().GetVehicle().DisableVehicleDamage)
		{
			return false;
		}

		return super.CanBeDamaged();
	}

	override bool OnBeforeSwitchLights(bool toOn)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeSwitchLights - Start");
#endif

		SetCarBatteryStateForVanilla(true);

		bool ret = super.OnBeforeSwitchLights(toOn);

		SetCarBatteryStateForVanilla(false);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnBeforeSwitchLights - End");
#endif

		return ret;
	}

	override bool OnBeforeEngineStart()
	{
		if (!super.OnBeforeEngineStart())
			return false;

		return OnBeforeEngineStart(0);
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemAttached - Start");
#endif

		if (m_IsPhysicsHost)
		{
			string slot = slot_name;
			slot.ToLower();
			if (slot.Contains("wheel"))
			{
				vector force = Vector(0, m_State.m_Mass * 50.0, 0);

				string selection = "";
				InventorySlots.GetSelectionForSlotId(InventorySlots.GetSlotIdFromString(slot_name), selection);
				vector position = GetSelectionPositionMS(selection);

				//dBodyApplyForceAt( this, ModelToWorld( position ), force );
			}
		}

		if (IsMissionHost())
		{
			if (slot_name == "ExpansionHelicopterBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "ExpansionAircraftBattery")
				m_BatteryHealth = item.GetHealth01();
		}

		super.EEItemAttached(item, slot_name);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemAttached - End");
#endif
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemDetached - Start");
#endif

		if (m_IsPhysicsHost)
		{
			string slot = slot_name;
			slot.ToLower();
			if (slot.Contains("wheel"))
			{
				vector force = Vector(0, m_State.m_Mass * 50.0, 0);

				string selection = "";
				InventorySlots.GetSelectionForSlotId(InventorySlots.GetSlotIdFromString(slot_name), selection);
				vector position = GetSelectionPositionMS(selection);

				//dBodyApplyForceAt( this, ModelToWorld( position ), force );
			}
		}

		if (IsMissionHost())
		{
			if (IsScriptedLightsOn())
			{
				if (slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery")
					ToggleHeadlights();

				if (slot_name == "Reflector_1_1")
				{
					SetHealth("Reflector_1_1", "Health", 0);

					item = FindAttachmentBySlotName("Reflector_2_1");
					if (!item || GetHealth01("Reflector_2_1", "") <= 0)
						ToggleHeadlights();
				}

				if (slot_name == "Reflector_2_1")
				{
					SetHealth("Reflector_2_1", "Health", 0);

					item = FindAttachmentBySlotName("Reflector_1_1");
					if (!item || GetHealth01("Reflector_1_1", "") <= 0)
						ToggleHeadlights();
				}
			}

			if (Expansion_EngineIsOn())
			{
				if (slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery")
					Expansion_EngineStop();
			}

			if (slot_name == "ExpansionHelicopterBattery")
				m_BatteryHealth = -1;

			if (slot_name == "ExpansionAircraftBattery")
				m_BatteryHealth = -1;
		}

		super.EEItemDetached(item, slot_name);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEItemDetached - End");
#endif
	}

	override bool IsIgnoredObject(Object o)
	{
		if (CarDoor.Cast(o))
			return true;

		if (CarWheel.Cast(o))
			return true;

		if (CarRadiator.Cast(o))
			return true;

		return super.IsIgnoredObject(o);
	}

	bool CanUpdateCarLock(float pDt)
	{
		return (HasKey() && m_VehicleLockedState == ExpansionVehicleLockState.READY_TO_LOCK && IsMissionHost());
	}

	bool DoorCount()
	{
		return m_Doors.Count();
	}

	bool AllDoorsClosed()
	{
		for (int z = 0; z < m_Doors.Count(); z++)
		{
			if (GetCarDoorsState(m_Doors[z]) != CarDoorState.DOORS_CLOSED)
			{
				return false;
			}
		}

		return true;
	}

	void UpdateCarLock(float pDt)
	{
		if (AllDoorsClosed() || GetExpansionSettings() && !GetExpansionSettings().GetVehicle().VehicleRequireAllDoors)
		{
			m_VehicleLockedState = ExpansionVehicleLockState.LOCKED;

			KeyMessage("OnCarDoorClosed::LOCKED");

			OnCarLocked();
			SetSynchDirty();
		}
	}

	bool CanUpdateHealth(float pDt)
	{
		m_FluidCheckTime += pDt;

		return m_FluidCheckTime >= GameConstants.CARS_FLUIDS_TICK;
	}

	void UpdateHealth(float pDt)
	{
		m_FluidCheckTime = 0;

		CarPartsHealthCheck();

		if ( IsMissionHost() && Expansion_EngineIsOn() )
		{
			if ( GetFluidFraction( CarFluid.FUEL ) <= 0 || m_EngineHealth <= 0 )
				Expansion_EngineStop();

			CheckVitalItem( IsVitalCarBattery(), "CarBattery" );
			CheckVitalItem( IsVitalTruckBattery(), "TruckBattery" );
			CheckVitalItem( IsVitalSparkPlug(), "SparkPlug" );
			CheckVitalItem( IsVitalGlowPlug(), "GlowPlug" );
			CheckVitalItem( IsVitalHelicopterBattery(), "ExpansionHelicopterBattery" );
			CheckVitalItem( IsVitalAircraftBattery(), "ExpansionAircraftBattery" );
		}

		if ( Expansion_EngineIsOn() )
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

				if (m_Expansion_CurrentEngine > 0)
					Leak(CarFluid.FUEL, m_Expansion_FuelConsumptionPerTick);
			}
			
			if (IsMissionClient())
			{
				if (!SEffectManager.IsEffectExist(m_exhaustPtcFx))
				{
					if (!IsInherited(ExpansionBoatScript) && !IsInherited(ExpansionHelicopterScript))
					{
						m_exhaustFx = new EffExhaustSmoke();
						m_exhaustPtcFx = SEffectManager.PlayOnObject(m_exhaustFx, this, m_exhaustPtcPos, m_exhaustPtcDir);
					}
				}

				if (m_exhaustFx)
					m_exhaustFx.SetParticleStateLight();

				if (IsVitalRadiator() && SEffectManager.IsEffectExist(m_coolantPtcFx))
					SEffectManager.Stop(m_coolantPtcFx);

				if (IsVitalRadiator() && GetFluidFraction(CarFluid.COOLANT) < 0.5)
				{
					if (!IsInherited(ExpansionBoatScript) && !IsInherited(ExpansionHelicopterScript))
					{
						if (!SEffectManager.IsEffectExist(m_coolantPtcFx))
						{
							m_coolantFx = new EffCoolantSteam();
							m_coolantPtcFx = SEffectManager.PlayOnObject(m_coolantFx, this, m_coolantPtcPos);
						}

						if (GetFluidFraction(CarFluid.COOLANT) > 0)
							m_coolantFx.SetParticleStateLight();
						else
							m_coolantFx.SetParticleStateHeavy();
					}
				}
				else
				{
					if (SEffectManager.IsEffectExist(m_coolantPtcFx))
						SEffectManager.Stop(m_coolantPtcFx);
				}
			}
		}
		else
		{
			if (IsMissionClient())
			{
				if (SEffectManager.IsEffectExist(m_exhaustPtcFx))
					SEffectManager.Stop(m_exhaustPtcFx);

				if (SEffectManager.IsEffectExist(m_coolantPtcFx))
					SEffectManager.Stop(m_coolantPtcFx);
			}
		}
	}

	bool CanUpdateHorn(float pDt)
	{
		return m_HornPlaying && IsMissionHost();
	}

	void UpdateHorn(float pDt)
	{
		NoiseParams npar = new NoiseParams();
		npar.LoadFromPath("CfgVehicles " + GetType() + " NoiseCarHorn");
		//GetGame().GetNoiseSystem().AddNoise( this, npar );
	}

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - Start");
#endif

		super.EOnPostSimulate(other, timeSlice);

		//! Prevent vanilla fluid checks from running
		m_Time = -1;

		if (CanUpdateHorn(timeSlice))
		{
			UpdateHorn(timeSlice);
		}

		if (CanUpdateHealth(timeSlice))
		{
			UpdateHealth(timeSlice);
		}

		if (CanUpdateCarLock(timeSlice))
		{
			UpdateCarLock(timeSlice);
		}

		if (IsMissionClient())
		{
			if (IsDamageDestroyed())
			{
				if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionVehiclePlaneBase) || IsInherited(ExpansionBoatScript))
				{
					if (m_engineFx)
					{
						m_engineFx.SetParticleStateExpansion();
					}

					if (m_Particles.Count() <= 0)
					{
						CreateParticle(this, "fire_pos", ParticleList.EXPANSION_FIRE_HELICOPTER);
					}
				}
			}
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EOnPostSimulate - End");
#endif
	}

	void CreateLights(Object lod, string point, typename type, vector color, vector ambient, float radius, float brigthness, bool flare, bool shadows, float default = 0)
	{
		array<Selection> lodSelections = new array<Selection>();

		LOD lodLod = lod.GetLODByName("memory");
		if (lodLod)
		{
			if (lodLod.GetSelections(lodSelections))
			{
				for (int i = 0; i < lodSelections.Count(); i++)
				{
					if (lodSelections[i].GetName() == point)
					{
						for (int j = 0; j < lodSelections[i].GetVertexCount(); j++)
						{
							ExpansionPointLight light = ExpansionPointLight.Cast(ExpansionPointLight.CreateLight(type, "0 0 0"));
							light.m_Val = default;

							light.SetRadiusTo(radius);
							light.SetBrightnessTo(brigthness);

							light.SetDiffuseColor(color[0], color[1], color[2]);
							light.SetAmbientColor(ambient[0], ambient[1], ambient[2]);

							light.SetFlareVisible(flare);
							light.SetCastShadow(shadows);

							light.AttachOnObject(lod, lodSelections[i].GetVertexPosition(lodLod, j), "0 0 0");

							light.ExpansionSetEnabled(true);

							m_Lights.Insert(light);
						}
					}
				}
			}
		}
	}

	void CreateParticle(Object lod, string point, int type)
	{
		array<Selection> lodSelections = new array<Selection>();

		LOD lodLod = lod.GetLODByName("memory");
		if (lodLod)
		{
			if (lodLod.GetSelections(lodSelections))
			{
				for (int i = 0; i < lodSelections.Count(); i++)
				{
					if (lodSelections[i].GetName() == point)
					{
						for (int j = 0; j < lodSelections[i].GetVertexCount(); j++)
						{
							Particle particle = Particle.PlayOnObject(type, lod, lodSelections[i].GetVertexPosition(lodLod, j));
							//! AddChild( particle.GetDirectParticleEffect(), -1, true );

							m_Particles.Insert(particle);
						}
					}
				}
			}
		}
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::UpdateLights - Start");
#endif

		SetCarBatteryStateForVanilla(true);

		super.UpdateLights(new_gear);

		SetCarBatteryStateForVanilla(false);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::UpdateLights - End");
#endif
	}

	override CarRearLightBase CreateRearLight()
	{
		if (!IsInherited(ExpansionHelicopterScript) || !IsInherited(ExpansionBoatScript))
		{
			return CarRearLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackRearLight) );
		}

		return NULL;
	}

	override CarLightBase CreateFrontLight()
	{
		if (!IsInherited(ExpansionHelicopterScript) || !IsInherited(ExpansionBoatScript))
		{
			return CarLightBase.Cast( ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight) );
		}

		return NULL;
	}

	void GetBoundingPositionTransform(Object obj, vector position, vector wsTrans[4], out vector trans[4])
	{
		vector bbTrans[4];
		Math3D.YawPitchRollMatrix("0 0 0", bbTrans);
		bbTrans[3] = position;

		Math3D.MatrixMultiply4(wsTrans, bbTrans, trans);

		vector contact_pos;
		vector contact_dir
			vector hitNormal;
		int contactComponent;
		float hitFraction;
		Object hitObject;

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.WATERLAYER;
		DayZPhysics.RayCastBullet(trans[3] + "0 10 0", trans[3] - "0 10 0", collisionLayerMask, obj, hitObject, contact_pos, hitNormal, hitFraction);
		trans[3] = contact_pos;
	}

	void Ex_AttachPlayer(DayZPlayerImplement player)
	{
	}

	void Ex_DetachPlayer(DayZPlayerImplement player)
	{
	}

	bool Expansion_CanSimulate()
	{
		return false;
	}

	bool Expansion_ShouldDisableSimulation()
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

		m_Event_Control.Control(m_State, driver);

		Expansion_OnHandleController(driver, dt);

		// force active
		dBodyActive(this, ActiveState.ACTIVE);
	}

	void Expansion_OnHandleController(DayZPlayerImplement driver, float dt)
	{
	}

	protected void OnPreSimulation(float pDt)
	{
		if (IsMissionClient())
		{
			OnParticleUpdate(pDt);
		}
	}

	protected void OnNoSimulation(float pDt)
	{
		if ( IsMissionClient() )
		{
			OnParticleUpdate( pDt );
		}

		OnAnimationUpdate(pDt);
	}

	protected void OnSimulation(ExpansionPhysicsState pState)
	{
	}

	protected void OnPostSimulation(float pDt)
	{
		OnAnimationUpdate(pDt);
	}

	protected void OnParticleUpdate(float pDt)
	{
	}

	protected void OnAnimationUpdate(float pDt)
	{
		m_Event_Animate.Animate(m_State);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Is Physics Host", m_IsPhysicsHost);
		instance.Add("Is Active", dBodyIsActive(this));
		instance.Add("Can Simulate", Expansion_CanSimulate());
		instance.Add("Should Disable Simulate", Expansion_ShouldDisableSimulation());

		//super.CF_OnDebugUpdate(instance, type);

		instance.Add(m_Controller);
		instance.Add(m_State);

		instance.Add("Num Modules", m_Modules.Count());
		for (int i = 0; i < m_Modules.Count(); i++)
			instance.Add(m_Modules[i]);

		return true;
	}
#endif

	override void EOnSimulate(IEntity owner, float dt)
	{
#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("[" + this + "] EOnSimulate");
#endif

		int i;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();
#endif

		DayZPlayerImplement driver = DayZPlayerImplement.Cast(CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER));

		if (GetGame().IsClient())
		{
			m_IsPhysicsHost = driver == GetGame().GetPlayer();
		}
		else if (GetGame().IsServer())
		{
			m_IsPhysicsHost = true;

			if (dBodyIsActive(this) && !m_Expansion_AcceptingAttachment)
			{
				m_Expansion_AcceptingAttachment = true;
				SetSynchDirty();
			}
			else if (!dBodyIsActive(this) && m_Expansion_AcceptingAttachment)
			{
				m_Expansion_AcceptingAttachment = false;
				SetSynchDirty();
			}

			if (!driver && Expansion_CanSimulate())
			{
				m_State.m_DeltaTime = dt;

				m_Event_Control.Control(m_State, null);

				Expansion_OnHandleController(null, dt);

				if (!m_Expansion_IsBeingTowed && Expansion_ShouldDisableSimulation())
				{
					dBodyActive(this, ActiveState.INACTIVE);
					return;
				}
			}
		}
		else
		{
			m_IsPhysicsHost = false;

			Error("Game is both not server or client");
		}

		if (m_IsPhysicsHost && m_Expansion_IsBeingTowed)
		{
			// shouldn't ever be NULL but just incase
			if (!m_Expansion_ParentTow)
				return;

			if (m_TowingEnabled && dBodyIsActive(m_Expansion_ParentTow) && dBodyIsDynamic(m_Expansion_ParentTow))
			{
				vector towTransform[4];
				m_Expansion_ParentTow.GetTransform(towTransform);
				towTransform[3] = m_Expansion_ParentTow.ModelToWorld(m_Expansion_TowPointCenter);

				dBodySetTargetMatrix(this, towTransform, dt);
			}

			return;
		}

		/**
		 * 'Expansion_HandleController' will force the vehicle to be active.
		 * If no driver is present, 'Expansion_HandleController' is not called.
		 * A driver not present will not automatically make the vehicle inactive
		 * 'Expansion_ShouldDisableSimulation' will be called when there is no driver
		 * that will set the vehicle to be inactive.
		 */
		if (!dBodyIsActive(this))
		{
			if (GetGame().IsServer() && !Expansion_CanSimulate())
			{
				SetVelocity(this, "0 0 0");
				dBodySetAngularVelocity(this, "0 0 0");

				dBodyActive(this, ActiveState.INACTIVE);

				SetSynchDirty();
			}

			//! This needs to run on client as well so dust particle is stopped if no driver
			OnNoSimulation(dt);
			
			#ifdef EXPANSION_CARSCRIPT_ACTIVESTATE_DEBUG
			if (m_Expansion_dBodyIsActive)
			{
				EXPrint(ToString() + " " + GetPosition() + " CarScript::EOnSimulate dBodyIsActive false");
				m_Expansion_dBodyIsActive = false;
			}
			#endif

			return;
		}
		#ifdef EXPANSION_CARSCRIPT_ACTIVESTATE_DEBUG
		else if (!m_Expansion_dBodyIsActive)
		{
			EXPrint(ToString() + " " + GetPosition() + " CarScript::EOnSimulate dBodyIsActive true");
			m_Expansion_dBodyIsActive = true;
		}
		#endif

		m_Controller.m_Yaw = GetController().GetSteering();
		//m_Controller.m_Throttle[0] = GetController().GetThrust();
		m_Controller.m_Brake[0] = GetController().GetBrake();
		m_Controller.m_Gear[0] = GetController().GetGear();

		TFloatArray gears = new TFloatArray;
		GetGame().ConfigGetFloatArray("CfgVehicles " + GetType() + " SimulationModule Gearbox ratios", gears);

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

		OnPreSimulation(dt);

		m_State.m_HasDriver = driver && m_IsPhysicsHost;
		m_State.m_Exploded = m_Exploded;

		if (m_IsPhysicsHost)
		{
			m_State.PreSimulate(dt);

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
			m_State.EstimateTransform(dt, m_DbgTransform);
#endif

			m_State.CalculateAltitudeLimiter();
			
			m_Event_PreSimulate.PreSimulate(m_State);

			OnSimulation(m_State);
			
			//OnSimulation(m_State);
			//force = force * 0.5;
			//torque = torque * 0.5;

			m_Event_Simulate.Simulate(m_State);

			if ( IsMissionClient() )
			{
				NetworkSend();
			}
		}

		m_State.PostSimulate(dt, m_IsPhysicsHost, ExpansionVehicleNetworkMode.SERVER, false);

		OnPostSimulation(dt);

		if (GetGame().IsServer())
			SetSynchDirty();

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

		if (module.IsInherited(ExpansionVehicleEngineBase))
		{
			auto engine = ExpansionVehicleEngineBase.Cast(module);
			engine.m_EngineIndex = m_Engines.Count() + 1;
			RegisterNetSyncVariableBool("m_Expansion_EngineSync" + engine.m_EngineIndex);
			if (engine.m_EngineIndex >= 4)
			{
				Error(GetType() + ": " + engine.m_EngineIndex + " engines added were added, max is 4.");
			}

			m_Engines.Insert(engine);

			m_Expansion_MaximumEngine = m_Engines.Count() + 1;
			return;
		}

		if (module.IsInherited(ExpansionVehicleGearbox))
		{
			auto gearbox = ExpansionVehicleGearbox.Cast(module);
			gearbox.m_GearIndex = m_Gearboxes.Count() + 1;
			if (gearbox.m_GearIndex >= 4)
			{
				Error(GetType() + ": " + gearbox.m_GearIndex + " gearboxes added were added, max is 4.");
			}

			m_Gearboxes.Insert(gearbox);
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

	/**
	 * @brief Returns engine's maximal working rpm without damaging the engine.
	 */
	float Expansion_EngineGetRPMRedline()
	{
		return Expansion_EngineGetRPMRedline(m_Expansion_CurrentEngine);
	}

	/**
	 * @brief Returns engine's rpm value.
	 */
	float Expansion_EngineGetRPM()
	{
		return Expansion_EngineGetRPM(m_Expansion_CurrentEngine);
	}

	/**
	 * @brief Returns true when engine is running, false otherwise.
	 */
	bool Expansion_EngineIsOn()
	{
		return Expansion_EngineIsOn(m_Expansion_CurrentEngine);
	}

	/**
	 * @brief Starts the engine.
	 */
	void Expansion_EngineStart()
	{
		Expansion_EngineStart(m_Expansion_CurrentEngine);
	}

	/**
	 * @brief Stops the engine.
	 */
	void Expansion_EngineStop()
	{
		Expansion_EngineStop(m_Expansion_CurrentEngine);
	}

	/**
	 * @brief Returns engine's max rpm before engine blows up.
	 */
	float Expansion_EngineGetRPMMax(int index)
	{
		if (index == 0)
			return EngineGetRPMMax();

		return m_Engines[index].m_RPMMax;
	}

	/**
	 * @brief Returns engine's maximal working rpm without damaging the engine.
	 */
	float Expansion_EngineGetRPMRedline(int index)
	{
		if (index == 0)
			return EngineGetRPMRedline();

		return m_Engines[index].m_RPMRedline;
	}

	/**
	 * @brief Returns engine's rpm value.
	 */
	float Expansion_EngineGetRPM(int index)
	{
		if (index == 0)
			return EngineGetRPM();

		return m_Engines[index].m_RPM;
	}

	/**
	 * @brief Returns true when engine is running, false otherwise.
	 */
	bool Expansion_EngineIsOn(int index)
	{
		if (index == 0)
			return EngineIsOn();

		return m_Controller.m_State[index];
	}

	/**
	 * @brief Starts the engine.
	 */
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

			if (index == 1)
				m_Expansion_EngineSync1 = true;
			else if (index == 2)
				m_Expansion_EngineSync2 = true;
			else if (index == 3)
				m_Expansion_EngineSync3 = true;

			OnEngineStart(index);
		}

		SetSynchDirty();
	}

	
	/**
	 * @brief Is called every time the game wants to start the engine.
	 * @return true if the engine can start, false otherwise.
	 */
	protected bool OnBeforeEngineStart(int index)
	{
		if (m_Exploded)
			return false;

		return true;
	}

	/**
	 * @brief Is called every time the engine starts.
	 */
	protected void OnEngineStart(int index)
	{
#ifdef TRADER
		PlayerBase player = PlayerBase.Cast(CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER));
		if (player)
			m_Trader_LastDriverId = player.GetIdentity().GetId();
#endif

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

	/**
	 * @brief Stops the engine.
	 */
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

			if (index == 1)
				m_Expansion_EngineSync1 = false;
			else if (index == 2)
				m_Expansion_EngineSync2 = false;
			else if (index == 3)
				m_Expansion_EngineSync3 = false;

			OnEngineStop(index);
		}

		SetSynchDirty();
	}

	/**
	 * @brief Is called every time the engine stops.
	 */
	protected void OnEngineStop(int index)
	{
		m_Expansion_EnginesOn--;

		if (m_Expansion_EnginesOn <= 0)
		{
			m_Expansion_EnginesOn = 0;

			m_Expansion_EngineIsOn = false;
			SetAnimationPhase("EnableMonitor", -1);
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

	void DBGAddShape(Shape shape)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		m_DebugShapes.Insert(shape);
#endif
	}

	vector DBGFixDebugPosition(inout vector position)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		position = position.Multiply4(m_DbgTransform);
#endif

		return position;
	}

	protected vector GetCurrentOrientation()
	{
		m_Orientation = GetOrientation();

		return m_Orientation;
	}

	protected vector GetCurrentPosition()
	{
		m_Position = GetPosition();

		return m_Position;
	}

	/**
	 * @deprecated
	 */
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	void TempReadSkin(ParamsReadContext ctx)
	{
		string m_CurrentSkinNameTEMP;
		Expansion_Assert_False(ctx.Read(m_CurrentSkinNameTEMP), "[" + this + "] Failed reading m_CurrentSkinNameTEMP");
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::OnStoreSave " + this + " " + GetGame().SaveVersion());
#endif

//! If we are saving after game version target for ModStorage support (1st stable)
#ifdef CF_MODULE_MODSTORAGE
		if (GetGame().SaveVersion() > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
		{
			super.OnStoreSave(ctx);
			return;
		}
#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write(m_ExpansionSaveVersion);

//! If we are saving game version target for ModStorage support (1st stable) or later
#ifdef CF_MODULE_MODSTORAGE
		if (GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
		{
			super.OnStoreSave(ctx);
			return;
		}
#endif

		super.OnStoreSave(ctx);

		ctx.Write(m_PersistentIDA);
		ctx.Write(m_PersistentIDB);
		ctx.Write(m_PersistentIDC);
		ctx.Write(m_PersistentIDD);

		int lockState = m_VehicleLockedState;
		ctx.Write(lockState);

		ctx.Write(m_Exploded);

		GetCurrentOrientation();
		ctx.Write(m_Orientation);

		GetCurrentPosition();
		ctx.Write(m_Position);

		ctx.Write(m_allAttachments);

		ctx.Write(m_Expansion_IsBeingTowed);
		ctx.Write(m_Expansion_IsTowing);

		if (m_Expansion_IsBeingTowed)
		{
			ctx.Write(m_Expansion_TowConnectionIndex);
			ctx.Write(m_Expansion_ParentTowPersistentIDA);
			ctx.Write(m_Expansion_ParentTowPersistentIDB);
			ctx.Write(m_Expansion_ParentTowPersistentIDC);
			ctx.Write(m_Expansion_ParentTowPersistentIDD);
		}

		if (m_Expansion_IsTowing)
		{
			ctx.Write(m_Expansion_ChildTowPersistentIDA);
			ctx.Write(m_Expansion_ChildTowPersistentIDB);
			ctx.Write(m_Expansion_ChildTowPersistentIDC);
			ctx.Write(m_Expansion_ChildTowPersistentIDD);
		}

		ctx.Write(m_CurrentSkinName);
	}

	/**
	 * The following code remains even though unused for backwards compatibility
	 */
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::OnStoreLoad " + this + " " + version);
#endif

#ifdef CF_MODULE_MODSTORAGE
		if (version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
			return super.OnStoreLoad(ctx, version);
#endif

		if (Expansion_Assert_False(ctx.Read(m_ExpansionSaveVersion), "[" + this + "] Failed reading m_ExpansionSaveVersion"))
			return false;

#ifdef CF_MODULE_MODSTORAGE
		if (m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET)
			return super.OnStoreLoad(ctx, version);
#endif

			//! With CF_ModStorage enabled, the code below won't be ran unless an old CE is loaded. To prevent server wipes, the code below will stay.

#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - Start");
#endif

		//! doubled data accidentally for version 15
		if (GetExpansionSaveVersion() == 15 && Expansion_Assert_False(ctx.Read(m_ExpansionSaveVersion), "[" + this + "] Failed reading m_ExpansionSaveVersion"))
		{
			return false;
		}

		if (Expansion_Assert_False(super.OnStoreLoad(ctx, version), "[" + this + "] Failed reading OnStoreLoad super"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_PersistentIDA), "[" + this + "] Failed reading m_PersistentIDA"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_PersistentIDB), "[" + this + "] Failed reading m_PersistentIDB"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_PersistentIDC), "[" + this + "] Failed reading m_PersistentIDC"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_PersistentIDD), "[" + this + "] Failed reading m_PersistentIDD"))
			return false;

		if (GetExpansionSaveVersion() <= 5)
		{
			if (Expansion_Assert_False(ctx.Read(m_VehicleLockedState), "[" + this + "] Failed reading m_VehicleLockedState"))
				return false;
		}
		else
		{
			int lockState;
			if (Expansion_Assert_False(ctx.Read(lockState), "[" + this + "] Failed reading lockState"))
				return false;

			m_VehicleLockedState = lockState;
		}

		if (Expansion_Assert_False(ctx.Read(m_Exploded), "[" + this + "] Failed reading m_Exploded"))
			return false;

		if (GetExpansionSaveVersion() < 15)
			TempReadSkin(ctx);

		if (Expansion_Assert_False(ctx.Read(m_Orientation), "[" + this + "] Failed reading m_Orientation"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_Position), "[" + this + "] Failed reading m_Position"))
			return false;

		if (Expansion_Assert_False(ctx.Read(m_allAttachments), "[" + this + "] Failed reading m_allAttachments"))
			return false;

		if (GetExpansionSaveVersion() >= 7)
		{
			if (Expansion_Assert_False(ctx.Read(m_Expansion_IsBeingTowed), "[" + this + "] Failed reading m_Expansion_IsBeingTowed"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_IsTowing), "[" + this + "] Failed reading m_Expansion_IsTowing"))
				return false;

			if (m_Expansion_IsBeingTowed)
			{
				if (GetExpansionSaveVersion() >= 34)
				{
					if (Expansion_Assert_False(ctx.Read(m_Expansion_TowConnectionIndex), "[" + this + "] Failed reading m_Expansion_TowConnectionIndex"))
						return false;
				}

				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDD"))
					return false;
			}

			if (m_Expansion_IsTowing)
			{
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDD"))
					return false;
			}

			m_Expansion_IsBeingTowed = false;
			m_Expansion_IsTowing = false;

			m_Expansion_ParentTowPersistentIDA = 0;
			m_Expansion_ParentTowPersistentIDB = 0;
			m_Expansion_ParentTowPersistentIDC = 0;
			m_Expansion_ParentTowPersistentIDD = 0;
			m_Expansion_ChildTowPersistentIDA = 0;
			m_Expansion_ChildTowPersistentIDB = 0;
			m_Expansion_ChildTowPersistentIDC = 0;
			m_Expansion_ChildTowPersistentIDD = 0;
		}

		SetSynchDirty();

		//! doubled data accidentally for version 15
		if (GetExpansionSaveVersion() == 15)
		{
			if (Expansion_Assert_False(ctx.Read(m_PersistentIDA), "[" + this + "] Failed reading m_PersistentIDA"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_PersistentIDB), "[" + this + "] Failed reading m_PersistentIDB"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_PersistentIDC), "[" + this + "] Failed reading m_PersistentIDC"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_PersistentIDD), "[" + this + "] Failed reading m_PersistentIDD"))
				return false;

			if (Expansion_Assert_False(ctx.Read(lockState), "[" + this + "] Failed reading lockState"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_Exploded), "[" + this + "] Failed reading m_Exploded"))
				return false;

			if (GetExpansionSaveVersion() < 15)
				TempReadSkin(ctx);

			if (Expansion_Assert_False(ctx.Read(m_Orientation), "[" + this + "] Failed reading m_Orientation"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_Position), "[" + this + "] Failed reading m_Position"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_allAttachments), "[" + this + "] Failed reading m_allAttachments"))
				return false;

			if (Expansion_Assert_False(ctx.Read(m_Expansion_IsBeingTowed), "[" + this + "] Failed reading m_Expansion_IsBeingTowed"))
				return false;
			if (Expansion_Assert_False(ctx.Read(m_Expansion_IsTowing), "[" + this + "] Failed reading m_Expansion_IsTowing"))
				return false;

			if (m_Expansion_IsBeingTowed)
			{
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ParentTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDD"))
					return false;
			}

			if (m_Expansion_IsTowing)
			{
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(ctx.Read(m_Expansion_ChildTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDD"))
					return false;
			}

#ifdef EXPANSION_CARSCRIPT_LOGGING
			EXLogPrint("CarScript::OnStoreLoad - End");
#endif
			return true;
		}

		m_Expansion_IsBeingTowed = false;
		m_Expansion_IsTowing = false;

		m_Expansion_ParentTowPersistentIDA = 0;
		m_Expansion_ParentTowPersistentIDB = 0;
		m_Expansion_ParentTowPersistentIDC = 0;
		m_Expansion_ParentTowPersistentIDD = 0;
		m_Expansion_ChildTowPersistentIDA = 0;
		m_Expansion_ChildTowPersistentIDB = 0;
		m_Expansion_ChildTowPersistentIDC = 0;
		m_Expansion_ChildTowPersistentIDD = 0;

		if (GetExpansionSaveVersion() < 21)
			return true;

		string currentSkinName = m_CurrentSkinName;

		if (Expansion_Assert_False(ctx.Read(m_CurrentSkinName), "[" + this + "] Failed reading m_CurrentSkinName"))
			return false;

		if (m_CurrentSkinName == "")
			m_CurrentSkinName = currentSkinName;

#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - End");
#endif

		return true;
	}

#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorage storage, string modName)
	{
#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::CF_OnStoreSave " + this + " " + modName);
#endif

		super.CF_OnStoreSave(storage, modName);

		if (modName != "DZ_Expansion_Vehicles")
			return;

		storage.Write(m_PersistentIDA);
		storage.Write(m_PersistentIDB);
		storage.Write(m_PersistentIDC);
		storage.Write(m_PersistentIDD);

		int lockState = m_VehicleLockedState;
		storage.Write(lockState);

		storage.Write(m_Exploded);

		//storage.Write( m_CurrentSkinName );

		GetCurrentOrientation();
		storage.Write(m_Orientation);

		GetCurrentPosition();
		storage.Write(m_Position);

		storage.Write(m_Expansion_IsBeingTowed);
		storage.Write(m_Expansion_IsTowing);

		if (m_Expansion_IsBeingTowed)
		{
			storage.Write(m_Expansion_TowConnectionIndex);
			storage.Write(m_Expansion_ParentTowPersistentIDA);
			storage.Write(m_Expansion_ParentTowPersistentIDB);
			storage.Write(m_Expansion_ParentTowPersistentIDC);
			storage.Write(m_Expansion_ParentTowPersistentIDD);
		}

		if (m_Expansion_IsTowing)
		{
			storage.Write(m_Expansion_ChildTowPersistentIDA);
			storage.Write(m_Expansion_ChildTowPersistentIDB);
			storage.Write(m_Expansion_ChildTowPersistentIDC);
			storage.Write(m_Expansion_ChildTowPersistentIDD);
		}

		storage.Write(m_CurrentSkinName);
	}

	override bool CF_OnStoreLoad(CF_ModStorage storage, string modName)
	{
#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] CarScript::CF_OnStoreLoad " + this + " " + modName);
#endif

		if (!super.CF_OnStoreLoad(storage, modName))
			return false;

		if (modName != "DZ_Expansion_Vehicles")
			return true;

		m_ExpansionSaveVersion = storage.GetVersion();

		if (Expansion_Assert_False(storage.Read(m_PersistentIDA), "[" + this + "] Failed reading m_PersistentIDA"))
			return false;

		if (Expansion_Assert_False(storage.Read(m_PersistentIDB), "[" + this + "] Failed reading m_PersistentIDB"))
			return false;

		if (Expansion_Assert_False(storage.Read(m_PersistentIDC), "[" + this + "] Failed reading m_PersistentIDC"))
			return false;

		if (Expansion_Assert_False(storage.Read(m_PersistentIDD), "[" + this + "] Failed reading m_PersistentIDD"))
			return false;

		int lockState;
		if (Expansion_Assert_False(storage.Read(lockState), "[" + this + "] Failed reading lockState"))
			return false;

		m_VehicleLockedState = lockState;

		if (Expansion_Assert_False(storage.Read(m_Exploded), "[" + this + "] Failed reading m_Exploded"))
			return false;

		//if ( Expansion_Assert_False( storage.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
		//	return false;

		if (Expansion_Assert_False(storage.Read(m_Orientation), "[" + this + "] Failed reading m_Orientation"))
			return false;

		if (Expansion_Assert_False(storage.Read(m_Position), "[" + this + "] Failed reading m_Position"))
			return false;

		if (GetExpansionSaveVersion() >= 7)
		{
			if (Expansion_Assert_False(storage.Read(m_Expansion_IsBeingTowed), "[" + this + "] Failed reading m_Expansion_IsBeingTowed"))
				return false;
			if (Expansion_Assert_False(storage.Read(m_Expansion_IsTowing), "[" + this + "] Failed reading m_Expansion_IsTowing"))
				return false;

			if (m_Expansion_IsBeingTowed)
			{
				if (GetExpansionSaveVersion() >= 34)
				{
					if (Expansion_Assert_False(storage.Read(m_Expansion_TowConnectionIndex), "[" + this + "] Failed reading m_Expansion_TowConnectionIndex"))
						return false;
				}

				if (Expansion_Assert_False(storage.Read(m_Expansion_ParentTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ParentTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ParentTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ParentTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ParentTowPersistentIDD"))
					return false;
			}

			if (m_Expansion_IsTowing)
			{
				if (Expansion_Assert_False(storage.Read(m_Expansion_ChildTowPersistentIDA), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDA"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ChildTowPersistentIDB), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDB"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ChildTowPersistentIDC), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDC"))
					return false;
				if (Expansion_Assert_False(storage.Read(m_Expansion_ChildTowPersistentIDD), "[" + this + "] Failed reading m_Expansion_ChildTowPersistentIDD"))
					return false;
			}
		}

		if (GetExpansionSaveVersion() < 21)
			return true;

		string currentSkinName = m_CurrentSkinName;

		storage.Read(m_CurrentSkinName);

		if (m_CurrentSkinName == "")
			m_CurrentSkinName = currentSkinName;

		return true;
	}
#endif

	override void EEOnAfterLoad()
	{
#ifdef EXPANSIONEXLOGPRINT
		Print("CarScript::EEOnAfterLoad - Start " + GetType() + " " + GetPosition());
#endif

		super.EEOnAfterLoad();

		if (m_Expansion_IsTowing && !m_Expansion_ChildTow)
		{
			m_Expansion_ChildTow = GetGame().GetEntityByPersitentID(m_Expansion_ChildTowPersistentIDA, m_Expansion_ChildTowPersistentIDB, m_Expansion_ChildTowPersistentIDC, m_Expansion_ChildTowPersistentIDD);
			if (m_Expansion_ChildTow)
			{
				CarScript cs_child;
				ItemBase evs_child;

				if (Class.CastTo(cs_child, m_Expansion_ChildTow))
					cs_child.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), m_Expansion_TowConnectionIndex);
				else if (Class.CastTo(evs_child, m_Expansion_ChildTow))
					evs_child.Expansion_OnTowCreated(this, Expansion_GetTowPosition(), m_Expansion_TowConnectionIndex);
			}
			else
			{
				m_Expansion_IsTowing = false;
			}
		}

#ifdef EXPANSION_VEHICLE_SKIN_LOGGING
		Print(m_CanBeSkinned);
		Print(m_CurrentSkinName);
#endif

		if (m_CanBeSkinned)
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex(GetType(), m_CurrentSkinName);
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = ExpansionSkin.Cast(m_Skins[m_CurrentSkinIndex]);

			ExpansionOnSkinUpdate();
		}
#ifdef EXPANSION_VEHICLE_SKIN_LOGGING
		Print(m_CurrentSkinIndex);
		Print(m_CurrentSkin);
#endif

#ifdef EXPANSIONEXLOGPRINT
		Print("CarScript::EEOnAfterLoad - End");
#endif
	}

	override void OnEngineStop()
	{
		super.OnEngineStop();

		//! Something (probably vanilla?) is calling OnEngineStop in a loop on client, EVEN IF ENGINE IS RUNNING WHYYY WTF
		//! Prevent this by checking netsynched var that tells us if engine is really stopped or not
		if (GetGame().IsClient() && m_Expansion_EngineIsOn)
			return;

		OnEngineStop(0);
	}

	override void OnEngineStart()
	{
		super.OnEngineStart();

		OnEngineStart(0);
	}

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

		if (IsSoundSynchRemote() && !m_HornPlaying)
		{
			m_HornPlaying = true;
			OnHornSoundPlay();
		}
		else if (!IsSoundSynchRemote() && m_HornPlaying)
		{
			m_HornPlaying = false;
			OnHornSoundStop();
		}

		if (m_ExplodedSynchRemote && !m_Exploded)
		{
			ExpansionOnSpawnExploded();
		}

		if (!IsMissionOffline())
		{
			if (m_Expansion_IsBeingTowed)
			{
				m_Expansion_ParentTow = EntityAI.Cast(GetGame().GetObjectByNetworkId(m_Expansion_ParentTowNetworkIDLow, m_Expansion_ParentTowNetworkIDHigh));
			}
			else
			{
				m_Expansion_ParentTow = NULL;
			}

			if (m_Expansion_IsTowing)
			{
				m_Expansion_ChildTow = EntityAI.Cast(GetGame().GetObjectByNetworkId(m_Expansion_ChildTowNetworkIDLow, m_Expansion_ChildTowNetworkIDHigh));
			}
			else
			{
				m_Expansion_ChildTow = NULL;
			}
		}

		if (m_CanBeSkinned && m_CurrentSkinSynchRemote != m_CurrentSkinIndex)
		{
			m_CurrentSkinIndex = m_CurrentSkinSynchRemote;
			if (m_CurrentSkinIndex >= 0 && m_CurrentSkinIndex < m_Skins.Count())
			{
				m_CurrentSkinName = m_SkinModule.GetSkinName(GetType(), m_CurrentSkinIndex);
				m_CurrentSkin = m_Skins[m_CurrentSkinIndex];
			}
			else
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

		m_Skins = new array<ExpansionSkin>;

		if (Class.CastTo(m_SkinModule, GetModuleManager().GetModule(ExpansionSkinModule)))
		{
			m_SkinModule.RetrieveSkins(GetType(), m_Skins, m_CurrentSkinName);
		}

		m_CanBeSkinned = m_Skins.Count() != 0;

		if (m_CanBeSkinned)
		{
			if (m_CurrentSkinName != "")
			{
				m_CurrentSkinIndex = m_SkinModule.GetSkinIndex(GetType(), m_CurrentSkinName);
			}
			else
			{
				m_CurrentSkinIndex = 0;

				m_CurrentSkinName = m_SkinModule.GetSkinName(GetType(), m_CurrentSkinIndex);
			}

			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[m_CurrentSkinIndex];

			ExpansionOnSkinUpdate();
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetupSkins - End");
#endif
	}

	override array<ExpansionSkin> ExpansionGetSkins()
	{
		return m_Skins;
	}

	override bool ExpansionHasSkin(int skinIndex)
	{
		return skinIndex > -1 && skinIndex < m_Skins.Count();
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - Start");
#endif

		if (m_CanBeSkinned)
		{
			string sZone = zone;
			sZone.ToLower();

			if (m_CurrentSkin)
			{
				if (m_CurrentSkin.DamageZones)
				{
					if (m_CurrentSkin.DamageZones.Count() > 0)
					{
						for (int i = 0; i < m_CurrentSkin.DamageZones.Count(); i++)
						{
							string cZone = m_CurrentSkin.DamageZones[i].Zone;
							cZone.ToLower();

							if (cZone == sZone)
							{
								ExpansionOnSkinDamageZoneUpdate(m_CurrentSkin.DamageZones[i], newLevel);
							}
						}
					}
				}
			}
		}

		if (IsMissionClient())
		{
			if (zone == "Engine" && newLevel >= GameConstants.STATE_DAMAGED)
			{
				if (!SEffectManager.IsEffectExist(m_enginePtcFx))
				{
					if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionVehiclePlaneBase) || IsInherited(ExpansionBoatScript)) 
					{
						m_engineFx = new EffEngineSmoke();
						m_enginePtcFx = SEffectManager.PlayOnObject(m_engineFx, this, GetMemoryPointPos("fire_pos"));
					}
				}

				if (m_engineFx)
				{
					if (newLevel >= GameConstants.STATE_BADLY_DAMAGED)
						m_engineFx.SetParticleStateHeavy();
					else
						m_engineFx.SetParticleStateLight();
				}
			}
			else if (zone == "Engine" && m_engineFx)
			{
				m_engineFx.Stop();
			}
		}
		
		if (IsMissionHost() && (!zone || zone == "GlobalHealth" || zone == "Engine") && newLevel == GameConstants.STATE_RUINED && Expansion_EngineIsOn())
			Expansion_EngineStop();

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - End");
#endif
	}

	override void ExpansionSetSkin(int skinIndex)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - Start");
#endif

#ifdef EXPANSION_SKIN_LOGGING
		Print(m_CanBeSkinned);
		Print(skinIndex);
#endif

		if (!m_CanBeSkinned)
		{
			m_CurrentSkinName = "";
			return;
		}

		m_CurrentSkinIndex = skinIndex;

		if (m_CurrentSkinIndex < 0)
		{
			m_CurrentSkinIndex = 0;
		}

		if (m_CurrentSkinIndex >= m_Skins.Count())
		{
			m_CurrentSkinIndex = 0;
		}

#ifdef EXPANSION_SKIN_LOGGING
		Print(m_CurrentSkinIndex);
#endif

		m_CurrentSkinName = m_SkinModule.GetSkinName(GetType(), m_CurrentSkinIndex);
		m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
		m_CurrentSkin = m_Skins[m_CurrentSkinIndex];

#ifdef EXPANSION_SKIN_LOGGING
		Print(m_CurrentSkinName);
		Print(m_CurrentSkinSynchRemote);
		Print(m_CurrentSkin);
#endif

		ExpansionOnSkinUpdate();

		SetSynchDirty();

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - End");
#endif
	}

	void ExpansionOnSkinDamageZoneUpdate(ExpansionSkinDamageZone zone, int level)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinDamageZoneUpdate - Start");
#endif

#ifdef EXPANSION_SKIN_LOGGING
		Print(zone);
		Print(level);
#endif

		for (int i = 0; i < zone.HiddenSelections.Count(); i++)
		{
			int selectionIndex = GetHiddenSelectionIndex(zone.HiddenSelections[i]);

#ifdef EXPANSION_SKIN_LOGGING
			Print("HiddenSelection: " + zone.HiddenSelections[i]);
			Print("SelectionIndex: " + selectionIndex);
#endif

			if (level >= 0 && level < zone.HealthLevels.Count())
			{
				ExpansionSkinHealthLevel healthLevel = zone.HealthLevels[level];

#ifdef EXPANSION_SKIN_LOGGING
				Print("RVTexture: " + healthLevel.RVTexture);
				Print("RVMaterial: " + healthLevel.RVMaterial);
#endif

				SetObjectTexture(selectionIndex, healthLevel.RVTexture);
				SetObjectMaterial(selectionIndex, healthLevel.RVMaterial);
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

		if (!m_CurrentSkin)
		{
#ifdef EXPANSIONEXPRINT
			EXPrint("ItemBase::ExpansionOnSkinUpdate called but m_CurrentSkin is NULL!");
#endif

			return;
		}

		if (m_CurrentSkin.HornEXT != "")
		{
			m_HornSoundSetEXT = m_CurrentSkin.HornEXT;
		}
		else
		{
			m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		}

		if (m_CurrentSkin.HornINT != "")
		{
			m_HornSoundSetINT = m_CurrentSkin.HornINT;
		}
		else
		{
			m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
		}

		for (int i = 0; i < m_CurrentSkin.HiddenSelections.Count(); i++)
		{
			ExpansionSkinHiddenSelection selection = m_CurrentSkin.HiddenSelections[i];

			int selectionIndex = GetHiddenSelectionIndex(selection.HiddenSelection);

#ifdef EXPANSION_SKIN_LOGGING
			Print("HiddenSelection: " + selection.HiddenSelection);
			Print("SelectionIndex: " + selectionIndex);
			Print("RVTexture: " + selection.RVTexture);
			Print("RVMaterial: " + selection.RVMaterial);
#endif

			SetObjectTexture(selectionIndex, selection.RVTexture);
			SetObjectMaterial(selectionIndex, selection.RVMaterial);
		}

		for (i = 0; i < m_CurrentSkin.DamageZones.Count(); i++)
		{
			ExpansionOnSkinDamageZoneUpdate(m_CurrentSkin.DamageZones[i], GetHealthLevel(m_CurrentSkin.DamageZones[i].Zone));
		}

#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinUpdate - End");
#endif
	}

	bool IsSurfaceWater(vector position)
	{
		EXPrint(ToString() + "::IsSurfaceWater is deprecated, use ExpansionStatic::SurfaceIsWater");
		return ExpansionStatic.SurfaceIsWater(position);
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;

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
		if (GetAnimationPhase("gear") == 0)
		{
			SetAnimationPhase("gear", 1);
		}
		else
		{
			SetAnimationPhase("gear", 0);
		}
	}

	//! Called when entity is being created as new by CE/ Debug
	override void EEOnCECreate()
	{
#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - Start");
#endif

		super.EEOnCECreate();

		m_IsCECreated = true;

		array<EntityAI> items = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count(); i++)
		{
			ExpansionCarKey key;
			if (Class.CastTo(key, items[i]))
			{
				PairKeyTo(key);
			}
		}

#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - End");
#endif
	}

	float GetCameraHeight()
	{
		return 1.3;
	}

	float GetCameraDistance()
	{
		return 4.5;
	}

	float GetModelAnchorPointY()
	{
		if (m_ModelAnchorPointY < 0)
		{
			string path = "CfgVehicles " + GetType() + " modelAnchorPointY";
			if (GetGame().ConfigIsExisting(path))
				m_ModelAnchorPointY = GetGame().ConfigGetFloat(path);
			else
				m_ModelAnchorPointY = 0.0;
#ifdef EXPANSIONEXPRINT
			EXPrint(GetType() + " modelAnchorPointY " + m_ModelAnchorPointY);
#endif
		}

		return m_ModelAnchorPointY;
	}

	override float GetTransportCameraDistance()
	{
		return GetCameraDistance();
	}

	override vector GetTransportCameraOffset()
	{
		return Vector(0, GetCameraHeight(), 0);
	}

	float GetWreckAltitude()
	{
		return 1;
	}

	vector GetWreckOffset()
	{
		return "0 0 1";
	}

	string GetWreck()
	{
		string path = "CfgVehicles " + GetType() + " wreck";
		if (GetGame().ConfigIsExisting(path))
			return GetGame().ConfigGetTextOut(path);

		string className = GetType() + "Wreck";

		if (GetGame().ConfigIsExisting("CfgVehicles " + className))
			return className;

		GetGame().ConfigGetBaseName("CfgVehicles " + GetType(), className);

		return className + "Wreck";
	}

	string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUniversalWheel"; //this should never happen
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
		auto item = ItemBase.Cast(other);
		if (item && !dBodyIsDynamic(item) && item.Expansion_CarContactActivates() && !item.GetHierarchyParent())
		{
			item.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			// if contact is maintained then the lifetime will exceed 10 seconds until the object rests
			item.SetDynamicPhysicsLifeTime(10.0);
		}

		if (m_Expansion_IsBeingTowed)
			return;

		ExpansionCheckTreeContact(other, data.Impulse);

		super.OnContact(zoneName, localPos, other, data);
	}

	void ExpansionCheckTreeContact(IEntity other, float impulse)
	{
		Object tree = Object.Cast(other);
		if (tree)
		{
			if (tree.IsTree() || tree.IsBush())
			{
				//EXPrint(ToString() + " " + GetPosition() + " hit tree, impulse: " + impulse);

				if (impulse > 7500)
				{
					EntityAI cutting_tool = EntityAI.Cast(GetGame().CreateObject("WoodAxe", vector.Zero, false, true));

					if (IsMissionClient())
						SoundHardTreeFallingPlay();

					if (GetGame().IsServer())
						tree.DecreaseHealth("", "", 100, true);

					tree.OnTreeCutDown(cutting_tool);

					dBodyDestroy(tree);

					GetGame().ObjectDelete(cutting_tool);
				}
			}
		}
	}

	override void CheckContactCache()
	{
		int contactZonesCount = m_ContactCache.Count();

		if (contactZonesCount == 0)
			return;

		bool playLightSound = false;
		bool playHeavySound = false;

		for (int i = 0; i < contactZonesCount; ++i)
		{
			string zoneName = m_ContactCache.GetKey(i);
			array<ref CarContactData> data = m_ContactCache[zoneName];

			float dmg;

			int contactCount = data.Count();
			if (IsInherited(ExpansionHelicopterScript))
			{
				//! Use highest damage
				float contactDmg;
				for (int j = 0; j < contactCount; ++j)
				{
					contactDmg = data[j].impulse * m_dmgContactCoef;
					if (contactDmg > dmg)
						dmg = contactDmg;
				}
			}
			else
			{
				//! Vanilla needlessly iterates over all contact data entries but ends up using only the last one anyway.
				//! Do this properly.
				int lastContactIdx = contactCount - 1;
				dmg = data[lastContactIdx].impulse * m_dmgContactCoef;
			}

			if (dmg < GameConstants.CARS_CONTACT_DMG_MIN)
				continue;

			int pddfFlags;

			if (dmg < GameConstants.CARS_CONTACT_DMG_THRESHOLD)
			{
				playLightSound = true;
				pddfFlags = ProcessDirectDamageFlags.NO_TRANSFER;
			}
			else
			{
				float crewDmg = dmg * GetExpansionSettings().GetVehicle().VehicleCrewDamageMultiplier;
				if (crewDmg > 0)
					DamageCrew(crewDmg);
				playHeavySound = true;
				pddfFlags = 0;
			}

			//! This in turn invokes EEHitBy which deals with exploding the heli if its health reaches 0
			if (CanBeDamaged())
				ProcessDirectDamage(DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", dmg * GetExpansionSettings().GetVehicle().VehicleSpeedDamageMultiplier, pddfFlags);
		}

		if (playLightSound)
			SynchCrashLightSound(true);

		if (playHeavySound)
			SynchCrashHeavySound(true);

		UpdateHeadlightState();
		UpdateLights();

		m_ContactCache.Clear();

		super.CheckContactCache();
	}

	override float GetBatteryConsumption()
	{
		//! Should prevent towed cars to consume the battery
		if (Expansion_IsBeingTowed())
			return 0;

		return m_BatteryConsume;
	}
};
