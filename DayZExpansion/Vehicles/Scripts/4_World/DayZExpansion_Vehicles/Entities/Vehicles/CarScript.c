/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

CarScript g_Expansion_Car;

/**
 * @class		CarScript
 * @brief
 */
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

	bool m_Expansion_HasPilot;

	ref array<ref ExpansionDoor> m_Doors = new array<ref ExpansionDoor>();
	ref map<string, ExpansionDoor> m_DoorMap = new map<string, ExpansionDoor>();

	// ------------------------------------------------------------ //
	// Constant Values - Set in Constructor, Errors occur if not.   //
	// ------------------------------------------------------------ //
	protected float m_MaxSpeed; // (km/h)
	protected float m_MaxSpeedMS; // (m/s)

	protected float m_AltitudeFullForce; // (m)
	protected float m_AltitudeNoForce; // (m)

	protected float m_Expansion_Mass;

	// ------------------------------------------------------------ //
	// Member values                                                //
	// ------------------------------------------------------------ //
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
	ref array<ExpansionPointLight> m_Lights;
	ref array<Particle> m_Particles;

	protected vector m_Orientation;
	protected vector m_Position;

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

	protected float m_ModelZeroPointDistanceFromGround = -1;

	protected bool m_Expansion_CanPlayerAttach;
	protected float m_Expansion_LockComplexity = 1.0;

	protected bool m_Expansion_EngineSync1;
	protected bool m_Expansion_EngineSync2;
	protected bool m_Expansion_EngineSync3;

	protected bool m_IsCECreated;

	protected bool m_Expansion_ForcedStoreLoadedPositionAndOrientation;
	protected bool m_Expansion_WasMissionLoadedAtVehicleInstantiation;

	bool m_Expansion_dBodyIsActive; //! Used for forcing storeloaded position/orientation on 1st inactive after load

	bool m_Expansion_Killed;

	bool m_Expansion_EOnPostSimulate;

	float m_Expansion_VehicleAutoCoverTimestamp;
	bool m_Expansion_HasLifetime;

	bool m_Expansion_CollisionDamageIfEngineOff;
	float m_Expansion_CollisionDamageMinSpeed; 

	ref map<string, int> m_Expansion_WheelsToAdd = new map<string, int>();
	int m_Expansion_AttachedWheelsCount;

	protected ref EffVehicleSmoke m_Expansion_ExhaustFx[3];
	protected vector m_Expansion_ExhaustPtcPos[3];
	protected vector m_Expansion_ExhaustPtcDir[3];
	protected int m_Expansion_ExhaustPtcFx[3];

	void CarScript()
	{
		g_Expansion_Car = this;

		RegisterNetSyncVariableInt("m_PersistentIDA");
		RegisterNetSyncVariableInt("m_PersistentIDB");
		RegisterNetSyncVariableInt("m_PersistentIDC");
		RegisterNetSyncVariableInt("m_PersistentIDD");
		RegisterNetSyncVariableInt("m_VehicleLockedState");

		RegisterNetSyncVariableInt("m_Expansion_CurrentEngine");

		RegisterNetSyncVariableBool("m_Expansion_AcceptingAttachment");
		RegisterNetSyncVariableBool("m_Expansion_CanPlayerAttach");
		RegisterNetSyncVariableFloat("m_Expansion_LockComplexity", 0, 0, 2);

		RegisterNetSyncVariableBool("m_HornSynchRemote");

		RegisterNetSyncVariableBool("m_Expansion_EngineIsOn");

		RegisterNetSyncVariableBool("m_Expansion_HasLifetime");

#ifndef EXPANSION_VEHICLE_DESYNC_PROTECTION_DISABLE
		m_State.RegisterSync_CarScript("m_State");
#endif

		//m_DoorJoints.Insert(new ExpansionCarDoorJoint(this, "axis_doors_driver", "doors_driver"));

		m_DebugShapes = new array<Shape>();

		m_Lights = new array<ExpansionPointLight>;
		m_Particles = new array<Particle>;

		int i;
		int count;
		string path;

		if (ConfigIsExisting("ExpansionAttachments"))
		{
			path = "CfgVehicles " + GetType() + " ExpansionAttachments";
			count = GetGame().ConfigGetChildrenCount(path);

			m_CanHaveLock = false;
			for (i = 0; i < count; i++)
			{
				string attachmentName;
				GetGame().ConfigGetChildName(path, i, attachmentName);

				string attachmentPath = path + " " + attachmentName;
				m_Doors.Insert(new ExpansionDoor(this, attachmentName, attachmentPath));

				m_CanHaveLock |= m_Doors[i].m_IsDoor;

				string slotName = m_Doors[i].m_InventorySlot;
				slotName.ToLower();

				m_DoorMap.Insert(slotName, m_Doors[i]);
			}
		}

		if (ConfigIsExisting("doors"))
		{
			TStringArray doors();
			ConfigGetTextArray("doors", doors);

			foreach (auto door : doors)
			{
				if (!m_DoorMap.Contains(door))
				{
					m_Doors.Insert(new ExpansionDoor(this, door, string.Empty));

					door.ToLower();
					m_DoorMap.Insert(door, m_Doors[i]);

					m_CanHaveLock |= true;
				}
			}
		}

		LoadConstantVariables();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LongDeferredInit, 1000);

		ExpansionSettings.SI_Vehicle.Insert(OnSettingsUpdated);

		RegisterNetSyncVariableInt("m_CurrentSkinSynchRemote");

		CF_Modules<ExpansionSkinModule>.Get(m_SkinModule);

		ExpansionSetupSkins();

		if (IsMissionHost())
		{
			m_Expansion_WasMissionLoadedAtVehicleInstantiation = GetDayZGame().GetExpansionGame().IsLoaded();
		}

		m_Expansion_MinimumEngine = 0;
		if (!IsCar())
			m_Expansion_MinimumEngine = 1;

		m_Expansion_MaximumEngine = 1;

		path = "CfgVehicles " + GetType() + " SimulationModule Engines";
		count = GetGame().ConfigGetChildrenCount(path);

		AddModule(new ExpansionVehicleEngine_CarScript(this, "CfgVehicles " + GetType() + " SimulationModule Engine"));
		AddModule(new ExpansionVehicleGearbox_CarScript(this, "CfgVehicles " + GetType() + " SimulationModule Gearbox"));

		for (i = 0; i < count; i++)
		{
			string engineName;
			GetGame().ConfigGetChildName(path, i, engineName);

			string enginePath = path + " " + engineName;
			AddModule(Expansion_CreateEngine(this, enginePath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Gearboxes";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string gearName;
			GetGame().ConfigGetChildName(path, i, gearName);

			string gearPath = path + " " + gearName;
			AddModule(Expansion_CreateGearbox(this, gearPath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Props";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string propName;
			GetGame().ConfigGetChildName(path, i, propName);

			string propPath = path + " " + propName;
			AddModule(new ExpansionVehicleProp(this, propPath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Buoyancy";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string buoyancyName;
			GetGame().ConfigGetChildName(path, i, buoyancyName);

			string buoyancyPath = path + " " + buoyancyName;
			AddModule(new ExpansionVehicleBuoyantPoint(this, buoyancyPath));
		}

		//! This exists so it can be overridden (e.g.) by server owners who don't have access to unbinarized models
		path = "CfgVehicles " + GetType() + " mass";
		if (GetGame().ConfigIsExisting(path))
			m_Expansion_Mass = GetGame().ConfigGetFloat(path);
		else
			m_Expansion_Mass = dBodyGetMass(this);

		path = "CfgVehicles " + GetType() + " hornSoundSetEXT";
		if (GetGame().ConfigIsExisting(path))
			m_HornSoundSetEXT = GetGame().ConfigGetTextOut(path);

		path = "CfgVehicles " + GetType() + " hornSoundSetINT";
		if (GetGame().ConfigIsExisting(path))
			m_HornSoundSetINT = GetGame().ConfigGetTextOut(path);

		if (GetGame().IsServer())
		{
			auto settings = GetExpansionSettings().GetVehicle();

			foreach (ExpansionVehiclesConfig vehcfg: settings.VehiclesConfig)
			{
				if (IsKindOf(vehcfg.ClassName))
				{
					m_Expansion_CanPlayerAttach = vehcfg.CanPlayerAttach;
					m_Expansion_LockComplexity = vehcfg.LockComplexity;
					break;
				}
			}

			m_Expansion_CollisionDamageIfEngineOff = settings.CollisionDamageIfEngineOff;
			m_Expansion_CollisionDamageMinSpeed = settings.CollisionDamageMinSpeedKmh / 3.6;  //! Converted to m/s

			TStringArray wheels = new TStringArray();
		#ifdef DAYZ_1_19
			ConfigGetTextArray("wheels", wheels);
		#endif

			if ((IsHelicopter() || IsBoat()) && wheels.Count() < 4)
			{
				//! Make sure we have at least four wheels
				TStringArray attachmentSlotNames = new TStringArray();
				ConfigGetTextArray("attachments", attachmentSlotNames);
				foreach (string attachmentSlotName: attachmentSlotNames)
				{
					attachmentSlotName.ToLower();
					if (attachmentSlotName.IndexOf("nivawheel_") == 0)
						wheels.Insert("HatchbackWheel");
				}
			}

			foreach (string wheelType: wheels)
			{
				wheelType.ToLower();
				m_Expansion_WheelsToAdd[wheelType] = m_Expansion_WheelsToAdd[wheelType] + 1;
			}
		}

		m_Expansion_ExhaustPtcPos[0] = m_exhaustPtcPos;
		m_Expansion_ExhaustPtcDir[0] = m_exhaustPtcDir;

		for (i = 1; i < 3; i++)
		{
			string exhaustEnd = string.Format("ptcExhaust%1_end", i);
			if (MemoryPointExists(exhaustEnd))
			{
				m_Expansion_ExhaustPtcPos[i] = GetMemoryPointPos(exhaustEnd);
				string exhaustStart = string.Format("ptcExhaust%1_start", i);
				if (MemoryPointExists(exhaustStart))
				{
					vector exhaustStartPos = GetMemoryPointPos(exhaustStart);
					vector tempOri = vector.Direction(exhaustStartPos, m_Expansion_ExhaustPtcPos[i]);

					vector exhaustPtcDir;
					exhaustPtcDir[0] = -tempOri[2];
					exhaustPtcDir[1] = tempOri[1];
					exhaustPtcDir[2] = tempOri[0];

					m_Expansion_ExhaustPtcDir[i] = exhaustPtcDir.Normalized().VectorToAngles();
				}
			}
			else
			{
				m_Expansion_ExhaustPtcDir[i] = "1 1 1";
				m_Expansion_ExhaustPtcFx[i] = -1;
			}
		}
	}

	void ~CarScript()
	{
		int i;

		if (IsMissionClient())
		{
			if (m_SmokeParticle)
			{
				m_SmokeParticle.Stop();
			}

			for (i = 0; i < m_Lights.Count(); i++)
			{
				if (!m_Lights[i])
					continue;

				m_Lights[i].ExpansionSetEnabled(false);

				GetGame().ObjectDelete(m_Lights[i]);
			}

			for (i = 0; i < m_Particles.Count(); i++)
			{
				if (!m_Particles[i])
					continue;

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
	}

	override void EEInit()
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);

		super.EEInit();

		if (GetGame().IsServer() && (IsHelicopter() || IsBoat()))
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_AddWheels);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

#ifdef SERVER
		if (GetExpansionSettings().GetLog().VehicleDeleted)
		{
			GetExpansionSettings().GetLog().PrintLog("[VehicleDeleted] " + GetType() + " (id=" + GetVehiclePersistentIDString() + " pos=" + GetPosition().ToString() + ")");
		}
#endif
	}

	override bool NameOverride(out string output)
    {
        if (IsLocked())
        {
            output = string.Format("%1 (%2)", ConfigGetString("displayName"), "#locked");
            return true;
        }

        return false;
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "LongDeferredInit");
#endif

		if (GetGame().IsServer())
			m_Event_SettingsChanged.SettingsChanged();
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

		if (GetGame().IsServer())
		{
			if (m_Expansion_IsStoreLoaded)
			{
				//! Setting state to inactive fixes issues with vehicles being simulated at server start (jumpy helis, boats being always active when in water, not needed for cars)
				if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionBoatScript))
					dBodyActive(this, ActiveState.INACTIVE);
			}

			m_Expansion_HasLifetime = GetLifetime() > 0;

			SetSynchDirty();
		}

		m_Expansion_VehicleAutoCoverTimestamp = GetGame().GetTickTime();
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetActions");
#endif

		super.SetActions();

		AddAction(ExpansionActionPairKey);
		AddAction(ExpansionActionAdminUnpairKey);

		AddAction(ExpansionActionLockVehicle);
	}

	bool Expansion_CanPlayerAttach()
	{
#ifdef EXPANSION_PLAYER_ATTACHMENT_CANATTACH_OVERRIDE
		m_Expansion_CanPlayerAttach = true;
#endif

		return m_Expansion_CanPlayerAttach;
	}

	float Expansion_GetMass()
	{
		return m_Expansion_Mass;
	}

	float Expansion_GetRequiredTorque()
	{
		vector minMax[2];
		GetCollisionBox(minMax);
		return ( ( minMax[1][1] - minMax[0][1] ) * Expansion_GetMass() );
	}

	float Expansion_LockComplexity()
	{
		return m_Expansion_LockComplexity;
	}

	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	protected void KeyMessage(string message)
	{
#ifdef EXPANSION_CARKEY_LOGGING
#ifdef JM_COT
		if (IsMissionClient())
		{
			Message(GetPlayer(), message);
		}
#endif
		Print(message);
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

	//! ID of the paired master key
	string GetPersistentIDString()
	{
		string id;
		id += ExpansionStatic.IntToHex(m_PersistentIDA);
		id += ExpansionStatic.IntToHex(m_PersistentIDB);
		id += ExpansionStatic.IntToHex(m_PersistentIDC);
		id += ExpansionStatic.IntToHex(m_PersistentIDD);
		return id;
	}

	//! ID of the vehicle itself
	string GetVehiclePersistentIDString()
	{
		return ExpansionStatic.GetPersistentIDString(this);
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
		KeyMessage("OnCarLocked");

		if (GetGame().IsServer())
		{
			auto rpc = ExpansionScriptRPC.Create();
			rpc.Send(this, ExpansionVehicleRPC.PlayLockSound, true, NULL);
		}
	}

	void OnCarUnlocked()
	{
		KeyMessage("OnCarUnlocked");

		if (GetGame().IsServer())
		{
			auto rpc = ExpansionScriptRPC.Create();
			rpc.Send(this, ExpansionVehicleRPC.PlayLockSound, true, NULL);
		}
	}

	void OnHornSoundPlay()
	{
		string soundFile = m_HornSoundSetEXT;
		if (GetGame().GetPlayer().IsCameraInsideVehicle())
			soundFile = m_HornSoundSetINT;

		m_HornSound = SEffectManager.PlaySoundOnObject(soundFile, this);
		m_HornSound.SetSoundAutodestroy(true);
		m_HornSound.SetSoundLoop(true);
	}

	void OnHornSoundStop()
	{
		m_HornSound.SetSoundLoop(false);
		m_HornSound.SoundStop();
	}

	void PlayHorn()
	{
		m_HornSynchRemote = true;
		m_HornPlaying = false;

		SetSynchDirty();
	}

	void StopHorn()
	{
		m_HornSynchRemote = false;
		m_HornPlaying = true;

		SetSynchDirty();
	}

	bool IsSoundSynchRemote()
	{
		return m_HornSynchRemote;
	}

	override void Explode(int damageType, string ammoType = "")
	{
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
	}

	/**
	 * @brief Explodes the vehicle on the server, telling the client that they should explode by setting the sync boolean.
	 */
	void ExpansionOnExplodeServer(int damageType, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "ExpansionOnExplodeServer").Add(damageType).Add(ammoType);
#endif

		m_Exploded = true;
		m_ExplodedSynchRemote = true;

		LeakAll(CarFluid.COOLANT);
		LeakAll(CarFluid.FUEL);
		LeakAll(CarFluid.OIL);
		LeakAll(CarFluid.BRAKE);

		TStringArray dmgZones = new TStringArray;
		GetDamageZones(dmgZones);
		foreach (string dmgZone : dmgZones)
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
	}

	/**
	 * @brief Explodes the vehicle on the client
	 */
	void ExpansionOnExplodeClient(int damageType, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "ExpansionOnExplodeClient").Add(damageType).Add(ammoType);
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
	}

	/**
	 * @brief Only ran on the entity entity that was already exploded but the client never knew about it (i.e. outside of network bubble)
	 */
	void ExpansionOnSpawnExploded()
	{
		m_Exploded = true;
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
		switch (rpc_type)
		{
			case ExpansionVehicleRPC.ControllerSync:
			{
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					return;	
					
				if (m_Controller)
				{
					PlayerBase driverBase;
					//! @note sender and driverBase.GetIdentity() will NOT be the same object even if they point to the same player identity (same ID)!
					if (Class.CastTo(driverBase, CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER)) && driverBase.GetIdentityUID() == sender.GetId())
					{
						m_Event_NetworkRecieve.NetworkRecieve(ctx);
					}
	
					//! Leave this here for pilot desync debug. TODO: Needs to be adapted when we ever have AI drivers.
					if (driverBase && !driverBase.GetIdentityUID())
						EXPrint("WARNING: Received controller sync, but driver has no identity! " + driverBase.GetIdentity());
				}
				else
				{
					//! Leave this here for pilot desync debug
					EXPrint("WARNING: Received controller sync, but m_Controller is NULL!");
				}
	
				return;
			}
			
			case ERPCs.RPC_EXPLODE_EVENT:
			{
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					return;	
					
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
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					return;	
				
				if (!GetGame().IsDedicatedServer())
				{
					if (m_SoundLock)
						delete m_SoundLock;
	
					m_SoundLock = SEffectManager.PlaySound("Expansion_Car_Lock_SoundSet", GetPosition());
					m_SoundLock.SetSoundAutodestroy(true);
				}
	
				return;
			}
			
			case ExpansionVehicleRPC.ClientPing:
			{
				if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
					return;	
				
				m_State.OnPing(ctx);
				return;
			}
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	void NetworkSend()
	{
		if (IsMissionOffline())
			return;

		auto rpc = ExpansionScriptRPC.Create();

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalIgniterPlug");
#endif

		return false;
	}

	bool IsVitalHydraulicHoses()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHydraulicHoses");
#endif

		return false;
	}

	void SetAttachmentHealth(string itemName, float health)
	{
		EntityAI item = FindAttachmentBySlotName(itemName);

		if (item)
			item.SetHealth("", "", health);
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
		Error("DEPRECATED, use GetBattery");

		return GetBattery() != null;
	}

	override ItemBase GetBattery()
	{
		if (IsVitalHelicopterBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("HelicopterBattery"));
		if (IsVitalAircraftBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		return super.GetBattery();
	}

	bool IsBatteryWorking()
	{
		ItemBase battery = GetBattery();

		if (!battery || battery.IsRuined())
			return false;

		return true;
	}

	bool IsVitalHelicopterBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
	}

	bool IsVitalAircraftBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalAircraftBattery");
#endif

		return false;
	}

	bool Expansion_HasVehicleHorn()
	{
		return m_CarHornLongSoundName != string.Empty;
	}

	bool Expansion_HasVehicleHornShort()
	{
		return m_CarHornShortSoundName != string.Empty;
	}

	bool IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return true;
	}

	bool IsDuck()
	{
		return false;
	}

	bool IsBoat()
	{
		return false;
	}

	bool IsHelicopter()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsHelicopter");
#endif

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

	override bool Expansion_CanObjectAttach(Object obj)
	{
#ifdef EXPANSIONMODAI
#ifdef EXPANSION_DISABLE_AI_ATTACHMENT
		if (obj.IsInherited(eAIBase))
			return false;
#endif
#endif
		return Expansion_CanPlayerAttach();
	}

	override bool Expansion_IsVehicleFunctional(bool checkOptionalParts = false)
	{
		if (!super.Expansion_IsVehicleFunctional(checkOptionalParts))
			return false;

		EntityAI item;
		
		if (IsVitalHelicopterBattery() || IsVitalAircraftBattery())
		{
			item = GetBattery();
			if (!item || item.IsRuined() || item.GetCompEM().GetEnergy() < m_BatteryEnergyStartMin)
				return false;
		}
	
		if (IsVitalHydraulicHoses())
		{
			item = FindAttachmentBySlotName("ExpansionHydraulicHoses");
			if (!item || item.IsRuined())
				return false;
		}
	
		if (IsVitalIgniterPlug())
		{
			item = FindAttachmentBySlotName("ExpansionIgniterPlug");
			if (!item || item.IsRuined())
				return false;
		}

		return true;
	}

	/**
	 * @note only called if CanObjectAttach returns true.
	 */
	bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

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
		SetCarBatteryStateForVanilla(true);

		bool ret = super.OnBeforeSwitchLights(toOn);

		SetCarBatteryStateForVanilla(false);

		return ret;
	}

	override bool OnBeforeEngineStart()
	{
		SetCarBatteryStateForVanilla(true);

		bool result;

		if (super.OnBeforeEngineStart())
			result = true;

		SetCarBatteryStateForVanilla(false);

		if (!result)
			return false;

		return OnBeforeEngineStart(0);
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		if (GetGame().IsServer())
		{
			if (slot_name == "ExpansionHelicopterBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "ExpansionAircraftBattery")
				m_BatteryHealth = item.GetHealth01();

#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
			ExpansionDoor door;
			slot_name.ToLower();
			if (m_DoorMap.Find(slot_name, door))
			{
				door.SetDoor(CarDoor.Cast(item));
			}
#endif

			if (item.IsInherited(CarWheel))
			{
				EXTrace.Print(EXTrace.VEHICLES, this, string.Format("Attached %1 (type=%2)", item.ToString(), item.GetType()));
				m_Expansion_AttachedWheelsCount++;
				string wheelType = item.GetType();
				wheelType.ToLower();
				if (m_Expansion_WheelsToAdd[wheelType])
				{
					int wheelCount = m_Expansion_WheelsToAdd[wheelType];
					if (wheelCount > 1)
						m_Expansion_WheelsToAdd[wheelType] = wheelCount - 1;
					else
						m_Expansion_WheelsToAdd.Remove(wheelType);
				}
			}
		}

		if (item.GetType() == "HatchbackWheel" && !IsCar() && !IsDuck())
		{
			if (!GetGame().IsDedicatedServer())
				item.SetInvisible(true);
		#ifndef DAYZ_1_19
			//! DayZ 1.20+
			if (GetGame().IsServer())
				item.SetAllowDamage(false);
		#endif
		}

		super.EEItemAttached(item, slot_name);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		if (IsMissionHost())
		{
			if (IsScriptedLightsOn())
			{
				if (slot_name == "ExpansionHelicopterBattery" || slot_name == "ExpansionAircraftBattery")
					ToggleHeadlights();

				EntityAI bulb;

				if (slot_name == "Reflector_1_1")
				{
					SetHealth("Reflector_1_1", "Health", 0);

					bulb = FindAttachmentBySlotName("Reflector_2_1");
					if (!bulb || GetHealth01("Reflector_2_1", "") <= 0)
						ToggleHeadlights();
				}

				if (slot_name == "Reflector_2_1")
				{
					SetHealth("Reflector_2_1", "Health", 0);

					bulb = FindAttachmentBySlotName("Reflector_1_1");
					if (!bulb || GetHealth01("Reflector_1_1", "") <= 0)
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

			if (item.IsInherited(CarWheel))
				m_Expansion_AttachedWheelsCount--;
		}

		super.EEItemDetached(item, slot_name);
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
			if (GetCarDoorsState(m_Doors[z].m_InventorySlot) != CarDoorState.DOORS_CLOSED)
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
		float time = m_FluidCheckTime;
		m_FluidCheckTime = 0;

		int i;

		if (GetGame().IsServer())
		{
			if (HasRadiator())
			{
				m_RadiatorHealth = GetRadiator().GetHealth01("", "");
			}
			else
			{
				m_RadiatorHealth = 0;
			}

			m_FuelTankHealth = GetHealth01("FuelTank", "");

			if (Expansion_EngineIsOn())
			{
				CheckVitalItem(IsVitalCarBattery(), "CarBattery");
				CheckVitalItem(IsVitalTruckBattery(), "TruckBattery");
				CheckVitalItem(IsVitalAircraftBattery(), "ExpansionAircraftBattery");
				CheckVitalItem(IsVitalHelicopterBattery(), "ExpansionHelicopterBattery");
				CheckVitalItem(IsVitalIgniterPlug(), "ExpansionIgniterPlug");
				CheckVitalItem(IsVitalHydraulicHoses(), "ExpansionHydraulicHoses");
				CheckVitalItem(IsVitalSparkPlug(), "SparkPlug");
				CheckVitalItem(IsVitalGlowPlug(), "GlowPlug");
				// engine belt is not needed right now
				//CheckVitalItem(IsVitalEngineBelt(), "EngineBelt");

				float fuel = GetFluidFraction(CarFluid.FUEL);
				float coolant = GetFluidFraction(CarFluid.COOLANT);
				float oil = GetFluidFraction(CarFluid.OIL);
				float brake = GetFluidFraction(CarFluid.BRAKE);
				if (!IsVitalRadiator())
					coolant = 1.0;

				oil = 1.0;

				float fuelConsumption;
				m_EngineHealth = 0;
				for (i = 0; i < m_Engines.Count(); i++)
				{
					m_Engines[i].ProcessHealth(time, fuel, coolant, oil, brake, m_EngineHealth, fuelConsumption);
				}
				m_EngineHealth = m_EngineHealth / m_Engines.Count();

				//! 1.19
				if (IsVitalFuelTank())
				{
					if (m_FuelTankHealth == GameConstants.DAMAGE_RUINED_VALUE && m_EngineHealth > GameConstants.DAMAGE_RUINED_VALUE)
					{
						SetHealth("Engine", "Health", GameConstants.DAMAGE_RUINED_VALUE);
					}
				}

				//! leaking of coolant from radiator when damaged
				if (IsVitalRadiator() && coolant > 0.0 && m_RadiatorHealth < 0.5) //CARS_LEAK_THRESHOLD
					LeakFluid(CarFluid.COOLANT);

				if (fuel > 0.0 && m_FuelTankHealth < GameConstants.DAMAGE_DAMAGED_VALUE)
					LeakFluid(CarFluid.FUEL);

				if (brake > 0.0 && m_EngineHealth < GameConstants.DAMAGE_DAMAGED_VALUE)
					LeakFluid(CarFluid.BRAKE);

				if (oil > 0.0 && m_EngineHealth < 0.25)
					LeakFluid(CarFluid.OIL);

				if (fuelConsumption > 0.0)
				{
					Leak(CarFluid.FUEL, fuelConsumption);
					if (GetFluidFraction(CarFluid.FUEL) <= 0)
						Expansion_EngineStop();
				}
			}
		}

		//FX only on Client and in Single
		if (!GetGame().IsDedicatedServer())
		{
			if (Expansion_EngineIsSpinning())
			{
				auto helicopter = ExpansionHelicopterScript.Cast(this);

				for (i = 0; i < 3; i++)
				{
					if (m_Expansion_ExhaustPtcPos[i] == vector.Zero)
						continue;

					if (!SEffectManager.IsEffectExist(m_Expansion_ExhaustPtcFx[i]))
					{
						m_Expansion_ExhaustFx[i] = new EffExhaustSmoke();
						m_Expansion_ExhaustPtcFx[i] = SEffectManager.PlayOnObject(m_Expansion_ExhaustFx[i], this, m_Expansion_ExhaustPtcPos[i], m_Expansion_ExhaustPtcDir[i]);
						if (helicopter)
						{
							m_Expansion_ExhaustFx[i].Expansion_SetParticleStateHelicopter();
							m_Expansion_ExhaustFx[i].SetEnableEventFrame(true);
						}
						else
						{
							m_Expansion_ExhaustFx[i].SetParticleStateLight();
						}
					}

					if (helicopter)
						m_Expansion_ExhaustFx[i].Expansion_SetRotorSpeed(helicopter.m_Simulation.m_RotorSpeed);
				}

				if (IsVitalRadiator() && GetFluidFraction(CarFluid.COOLANT) < 0.5)
				{
					if (!SEffectManager.IsEffectExist(m_coolantPtcFx))
					{
						m_coolantFx = new EffCoolantSteam();
						m_coolantPtcFx = SEffectManager.PlayOnObject(m_coolantFx, this, m_coolantPtcPos, vector.Zero);
					}

					if (m_coolantFx)
					{
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
			else
			{
				for (i = 0; i < 3; i++)
				{
					if (m_Expansion_ExhaustPtcFx[i] != -1 && SEffectManager.IsEffectExist(m_Expansion_ExhaustPtcFx[i]))
					{
						SEffectManager.Stop(m_Expansion_ExhaustPtcFx[i]);
						m_Expansion_ExhaustPtcFx[i] = -1;
					}
				}

				if (SEffectManager.IsEffectExist(m_coolantPtcFx))
				{
					SEffectManager.Stop(m_coolantPtcFx);
					m_coolantPtcFx = -1;
				}
			}
		}
	}

	bool Expansion_EngineIsSpinning()
	{
		return Expansion_EngineIsOn();
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
		//! Prevent vanilla fluid checks from running
		m_Time = -1;

		//! All the vanilla cars call super in EOnPostSimulate :-(
		//! https://feedback.bistudio.com/T164047
		if (!m_Expansion_EOnPostSimulate)
			return;

		m_Expansion_EOnPostSimulate = false; //! Reset in EOnSimulate

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
	}

	override void OnUpdate(float dt)
	{
		if (GetGame().IsServer() && m_DrownTime > 0 && !CanBeDamaged())
			m_DrownTime = 0;

		super.OnUpdate(dt);
	}

	void Expansion_AddWheels()
	{
		if (ToDelete())
			return;

		if (!m_Expansion_WheelsToAdd.Count())
			return;

		foreach (string type, int count: m_Expansion_WheelsToAdd)
		{
			while (count > 0)
			{
				count--;
				EXTrace.Print(EXTrace.VEHICLES, this, "Trying to add " + type);
				EntityAI wheel = GetInventory().CreateAttachment(type);
				if (!wheel)
					Error(string.Format("Vehicle %1 (type=%2, pos=%3): Failed to add %4", ToString(), GetType(), GetPosition(), type));
			}
		}

		if (m_Expansion_AttachedWheelsCount < 4)
			Error(string.Format("Vehicle %1 (type=%2, pos=%3) has only %4 attached wheels!", ToString(), GetType(), GetPosition(), m_Expansion_AttachedWheelsCount));
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

	void CreateParticle(Object lod, string point, int type, vector local_ori = "0 0 0", bool force_world_rotation = false)
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
							CreateParticleEx(type, lod, lodSelections[i].GetVertexPosition(lodLod, j), local_ori, force_world_rotation);
						}
					}
				}
			}
		}
	}

	void CreateParticleEx(int type, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0", bool force_world_rotation = false)
	{
		Particle particle = Particle.PlayOnObject(type, parent_obj, local_pos, local_ori, force_world_rotation);
		//! AddChild( particle.GetDirectParticleEffect(), -1, true );

		m_Particles.Insert(particle);
	}

	override void UpdateLights(int new_gear = -1)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "UpdateLights").Add(new_gear);
#endif

		SetCarBatteryStateForVanilla(true);

		if (IsCar())
		{
			super.UpdateLights(new_gear);
		}
		else
		{
			#ifndef SERVER
			Expansion_UpdateLightsClient(new_gear);
			#endif
			Expansion_UpdateLightsServer(new_gear);
		}

		SetCarBatteryStateForVanilla(false);
	}

	void Expansion_UpdateLightsClient(int newGear = -1)
	{
		ItemBase battery;

		if ( IsVitalCarBattery() )
			battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
		else if ( IsVitalTruckBattery() )
			battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );

		if ( battery && battery.IsAlive() && battery.GetCompEM().GetEnergy() > 0 )
		{
			int gear;

			if (newGear == -1)
			{
				//! 1.19
				gear = GetGear();
			}
			else
			{
				gear = newGear;
			}

			if (m_HeadlightsOn)
			{
				if (!m_Headlight && m_HeadlightsState != CarHeadlightBulbsState.NONE)
				{
					m_Headlight = CreateFrontLight();
				}

				if (m_Headlight)
				{
					switch (m_HeadlightsState)
					{
					case CarHeadlightBulbsState.LEFT:
						m_Headlight.AttachOnMemoryPoint(this, m_LeftHeadlightPoint, m_LeftHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						break;
					case CarHeadlightBulbsState.RIGHT:
						m_Headlight.AttachOnMemoryPoint(this, m_RightHeadlightPoint, m_RightHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						break;
					case CarHeadlightBulbsState.BOTH:
						vector local_pos_left = GetMemoryPointPos(m_LeftHeadlightPoint);
						vector local_pos_right = GetMemoryPointPos(m_RightHeadlightPoint);

						vector local_pos_middle = (local_pos_left + local_pos_right) * 0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						break;
					default:
						m_Headlight.FadeOut();
						m_Headlight = null;
					}
				}
			}
			else
			{
				if (m_Headlight)
				{
					m_Headlight.FadeOut();
					m_Headlight = null;
				}
			}

			// brakes
			if (m_BrakesArePressed)
			{
				switch (gear)
				{
				case CarGear.REVERSE:
				//! 1.19
				case CarAutomaticGearboxMode.R:
					m_RearLightType = CarRearLightType.BRAKES_AND_REVERSE;
					break;
				default:
					m_RearLightType = CarRearLightType.BRAKES_ONLY;
				}
				//Debug.Log(string.Format("m_BrakesArePressed=%1, m_RearLightType=%2", m_BrakesArePressed.ToString(), EnumTools.EnumToString(CarRearLightType, m_RearLightType)));
			}
			else
			{
				switch (gear)
				{
				case CarGear.REVERSE:
				//! 1.19
				case CarAutomaticGearboxMode.R:
					m_RearLightType = CarRearLightType.REVERSE_ONLY;
					break;
				default:
					m_RearLightType = CarRearLightType.NONE;
				}
				//Debug.Log(string.Format("m_BrakesArePressed=%1, m_RearLightType=%2", m_BrakesArePressed.ToString(), EnumTools.EnumToString(CarRearLightType, m_RearLightType)));
			}

			if (!m_RearLight && m_RearLightType != CarRearLightType.NONE && m_HeadlightsState != CarHeadlightBulbsState.NONE)
			{
				m_RearLight = CreateRearLight();
				vector local_pos = GetMemoryPointPos(m_ReverseLightPoint);
				m_RearLight.AttachOnObject(this, local_pos, "180 0 0");
			}

			// rear lights
			if (m_RearLight && m_RearLightType != CarRearLightType.NONE && m_HeadlightsState != CarHeadlightBulbsState.NONE)
			{
				switch (m_RearLightType)
				{
				case CarRearLightType.BRAKES_ONLY:
					m_RearLight.SetAsSegregatedBrakeLight();
					break;
				case CarRearLightType.REVERSE_ONLY:
					m_RearLight.SetAsSegregatedReverseLight();
					break;
				case CarRearLightType.BRAKES_AND_REVERSE:
					m_RearLight.AggregateLight();
					m_RearLight.SetFadeOutTime(1);
					break;
				default:
					m_RearLight.FadeOut();
					m_RearLight = null;
				}
			}
			else
			{
				if (m_RearLight)
				{
					m_RearLight.FadeOut();
					m_RearLight = null;
				}
			}
		}
		else
		{
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

	void Expansion_UpdateLightsServer(int newGear = -1)
	{
		ItemBase battery;

		if ( IsVitalCarBattery() )
			battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
		else if ( IsVitalTruckBattery() )
			battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );

		if ( battery && battery.IsAlive() && battery.GetCompEM().GetEnergy() > 0 )
		{
			int gear;

			if (newGear == -1)
			{
				//! 1.19
				gear = GetGear();
				if (GearboxGetType() == CarGearboxType.AUTOMATIC)
				{
					gear = GearboxGetMode();
				}
			}
			else
			{
				gear = newGear;
			}

			if (m_HeadlightsOn)
			{
				DashboardShineOn();
				TailLightsShineOn();

				switch (m_HeadlightsState)
				{
				case CarHeadlightBulbsState.LEFT:
					LeftFrontLightShineOn();
					RightFrontLightShineOff();
					break;
				case CarHeadlightBulbsState.RIGHT:
					LeftFrontLightShineOff();
					RightFrontLightShineOn();
					break;
				case CarHeadlightBulbsState.BOTH:
					LeftFrontLightShineOn();
					RightFrontLightShineOn();
					break;
				default:
					LeftFrontLightShineOff();
					RightFrontLightShineOff();
				}

				//Debug.Log(string.Format("m_HeadlightsOn=%1, m_HeadlightsState=%2", m_HeadlightsOn.ToString(), EnumTools.EnumToString(CarHeadlightBulbsState, m_HeadlightsState)));
			}
			else
			{
				TailLightsShineOff();
				DashboardShineOff();
				LeftFrontLightShineOff();
				RightFrontLightShineOff();
			}

			// brakes
			if (m_BrakesArePressed)
			{
				switch (gear)
				{
				case CarGear.REVERSE:
				//! 1.19
				case CarAutomaticGearboxMode.R:
					m_RearLightType = CarRearLightType.BRAKES_AND_REVERSE;
					break;
				default:
					m_RearLightType = CarRearLightType.BRAKES_ONLY;
				}

				//Debug.Log(string.Format("m_BrakesArePressed=%1, m_RearLightType=%2", m_BrakesArePressed.ToString(), EnumTools.EnumToString(CarRearLightType, m_RearLightType)));
			}
			else
			{
				switch (gear)
				{
				case CarGear.REVERSE:
				//! 1.19
				case CarAutomaticGearboxMode.R:
					m_RearLightType = CarRearLightType.REVERSE_ONLY;
					break;
				default:
					m_RearLightType = CarRearLightType.NONE;
				}

				//Debug.Log(string.Format("m_BrakesArePressed=%1, m_RearLightType=%2", m_BrakesArePressed.ToString(), EnumTools.EnumToString(CarRearLightType, m_RearLightType)));
			}


			// rear lights
			if (m_RearLightType != CarRearLightType.NONE && m_HeadlightsState != CarHeadlightBulbsState.NONE)
			{
				switch (m_RearLightType)
				{
				case CarRearLightType.BRAKES_ONLY:
					ReverseLightsShineOff();
					BrakeLightsShineOn();
					break;
				case CarRearLightType.REVERSE_ONLY:
					ReverseLightsShineOn();
					BrakeLightsShineOff();
					break;
				case CarRearLightType.BRAKES_AND_REVERSE:
					BrakeLightsShineOn();
					ReverseLightsShineOn();
					break;
				default:
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
			LeftFrontLightShineOff();
			RightFrontLightShineOff();
			DashboardShineOff();
			BrakeLightsShineOff();
			ReverseLightsShineOff();
		}
	}

	override CarRearLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif
		if (!IsInherited(ExpansionHelicopterScript) || !IsInherited(ExpansionBoatScript))
		{
			return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(OffroadHatchbackRearLight));
		}

		return NULL;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		if (!IsInherited(ExpansionHelicopterScript) || !IsInherited(ExpansionBoatScript))
		{
			return CarLightBase.Cast(ScriptedLightBase.CreateLight(OffroadHatchbackFrontLight));
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_CanSimulate");
#endif

		return false;
	}

	bool Expansion_ShouldDisableSimulation()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_ShouldDisableSimulation");
#endif

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "Expansion_OnHandleController").Add(driver).Add(dt);
#endif

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
		if (IsMissionClient())
		{
			OnParticleUpdate(pDt);
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

	override void EOnSimulate(IEntity other, float dt)
	{
		if (!m_Initialized)
			return;

		m_Expansion_EOnPostSimulate = true;

		int i;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();
#endif

		DayZPlayerImplement driver = DayZPlayerImplement.Cast(CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER));

		//! Detect if pilot has been disconnected
		if (!driver && m_Expansion_HasPilot)
		{
			ExpansionHelicopterScript heli;
			m_Expansion_HasPilot = false;
			if (Class.CastTo(heli, this))
			{
				if (!heli.IsAutoHover())
					heli.SwitchAutoHover();  //! Turn autohover on
				Expansion_EngineStop();  //! Stop engine. Heli will autorotate to ground.
			}
		}

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
				m_State.m_IsSync = true;

				m_Event_Control.Control(m_State, null);

				Expansion_OnHandleController(null, dt);

				if (Expansion_ShouldDisableSimulation())
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

		if (m_IsPhysicsHost)
		{
#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
			foreach (auto door : m_Doors)
			{
				door.OnUpdate(dt);
			}
#endif
		}

		//! If driver managed to get in vehicle before forcing initial storeloaded position, skip it
		if (driver && m_Expansion_IsStoreLoaded && !m_Expansion_ForcedStoreLoadedPositionAndOrientation)
			m_Expansion_ForcedStoreLoadedPositionAndOrientation = true;

		bool isActive = dBodyIsActive(this);

		if (!isActive)
		{
			if (m_Expansion_dBodyIsActive)
			{
#ifdef DIAG
				EXTrace.Print(EXTrace.VEHICLES, this, "CarScript::EOnSimulate - pos/ori " + GetPosition() + " " + GetOrientation() + " - dBodyIsActive false");
#endif
				m_Expansion_dBodyIsActive = false;

				if (m_Expansion_IsStoreLoaded && !m_Expansion_ForcedStoreLoadedPositionAndOrientation && !m_Expansion_WasMissionLoadedAtVehicleInstantiation)
				{
					//! Vehicle has become inactive after initial store load. Force position/orientation to stored values.
					m_Expansion_ForcedStoreLoadedPositionAndOrientation = true;
					Expansion_ForcePositionAndOrientation(m_Position, m_Orientation);
#ifdef DIAG
					EXTrace.Print(EXTrace.VEHICLES, this, "CarScript::EOnSimulate - restored pos/ori " + GetPosition() + " " + GetOrientation());
#endif
				}
			}
		}
#ifdef DIAG
		else if (!m_Expansion_dBodyIsActive)
		{
			EXTrace.Print(EXTrace.VEHICLES, this, "CarScript::EOnSimulate - pos/ori " + GetPosition() + " " + GetOrientation() + " - dBodyIsActive true");
			m_Expansion_dBodyIsActive = true;
		}
#endif

		if (!Expansion_CanSimulate())
		{
			return;
		}

		/**
		 * 'Expansion_HandleController' will force the vehicle to be active.
		 * If no driver is present, 'Expansion_HandleController' is not called.
		 * A driver not present will not automatically make the vehicle inactive
		 * 'Expansion_ShouldDisableSimulation' will be called when there is no driver
		 * that will set the vehicle to be inactive.
		 */
		if (!isActive)
		{
			//! This needs to run on client as well so dust particle is stopped if no driver
			OnNoSimulation(dt);

			return;
		}
		//! 1.19
		m_Controller.m_Yaw = GetSteering();
		//m_Controller.m_Throttle[0] = GetThrust();
		m_Controller.m_Brake[0] = GetBrake();
		m_Controller.m_Gear[0] = GetGear();

		TFloatArray gears = new TFloatArray;
		GetGame().ConfigGetFloatArray("CfgVehicles " + GetType() + " SimulationModule Gearbox ratios", gears);

		//! 1.19
		if (GetGear() > 2)
		{
			m_Controller.m_Ratio[0] = gears[GetGear() - 2];
		}
		else if (GetGear() == 0)
		{
			m_Controller.m_Ratio[0] = GetGame().ConfigGetFloat("CfgVehicles " + GetType() + " SimulationModule Gearbox reverse");
		}
		else
		{
			m_Controller.m_Ratio[0] = 0;
		}

		OnPreSimulation(dt);

		m_State.m_HasDriver = driver && m_IsPhysicsHost;
		m_State.m_Exploded = m_Exploded;

		if (m_IsPhysicsHost)
		{
			m_State.SetupSimulation(dt);

			m_State.CalculateAltitudeLimiter();

			m_Event_PreSimulate.PreSimulate(m_State);

			OnSimulation(m_State);

			m_Event_Simulate.Simulate(m_State);

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
			m_State.EstimateTransform(dt, m_DbgTransform);
#endif
		}

		if (m_IsPhysicsHost && GetGame().IsClient())
		{
			NetworkSend();
		}

#ifndef EXPANSION_VEHICLE_DESYNC_PROTECTION_DISABLE
		m_State.ApplySimulation_CarScript(dt, m_IsPhysicsHost, driver);
#else
		m_State.ApplySimulation(dt);
#endif

		OnPostSimulation(dt);

		if (GetGame().IsServer())
		{
			SetSynchDirty();
		}
	}

	void Expansion_ForcePositionAndOrientation(vector position, vector orientation)
	{
		for ( int i = 0; i < 2; i++)
		{
			SetPosition(position);
			orientation[2] = orientation[2] - 1;
			SetOrientation(orientation);
			orientation[2] = orientation[2] + 1;
			SetOrientation(orientation);
			Synchronize();
		}
	}

	void SetHasPilot(bool state)
	{
		//! So we are able to detect if pilot got disconnected or got out on own accord
		m_Expansion_HasPilot = state;
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

			m_Expansion_MaximumEngine = m_Engines.Count();
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
		auto trace = EXTrace.Start(ExpansionTracing.VEHICLES, this, index.ToString());

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

		//! Vanilla does this check for us in OnBeforeEngineStart() if it's a car
		if (!IsCar() && GetFluidFraction(CarFluid.FUEL) <= 0)
		{
			return false;
		}

		EntityAI item2;

		if (IsVitalIgniterPlug() || IsVitalSparkPlug() || IsVitalGlowPlug())
		{
			item2 = NULL;
			if (IsVitalIgniterPlug())
				item2 = FindAttachmentBySlotName("ExpansionIgniterPlug");
			if (IsVitalSparkPlug())
				item2 = FindAttachmentBySlotName("SparkPlug");
			if (IsVitalGlowPlug())
				item2 = FindAttachmentBySlotName("GlowPlug");

			if (!item2 || item2.IsRuined())
				return false;
		}

		if (IsVitalHydraulicHoses() || IsVitalRadiator())
		{
			item2 = NULL;
			if (IsVitalHydraulicHoses())
				item2 = FindAttachmentBySlotName("ExpansionHydraulicHoses");
			if (IsVitalRadiator())
				item2 = FindAttachmentBySlotName("CarRadiator");

			if (!item2 || item2.IsRuined())
				return false;
		}

		if (IsVitalHelicopterBattery() || IsVitalCarBattery() || IsVitalTruckBattery())
		{
			item2 = NULL;
			if (IsVitalHelicopterBattery())
				item2 = FindAttachmentBySlotName("ExpansionHelicopterBattery");
			if (IsVitalCarBattery())
				item2 = FindAttachmentBySlotName("CarBattery");
			if (IsVitalTruckBattery())
				item2 = FindAttachmentBySlotName("TruckBattery");

			if (!item2 || item2.IsRuined() || item2.GetCompEM().GetEnergy() < m_BatteryEnergyStartMin)
				return false;
		}

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

	//! Get the number of Expansion engines that are on (does NOT include vanilla engine!)
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

	void DBGDrawLine(vector start, vector end, int color = 0x44FFFFFF)
	{
		vector pts[2];
		pts[0] = start;
		pts[1] = end;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		DBGAddShape(Shape.CreateLines(color, ShapeFlags.TRANSP | ShapeFlags.NOZBUFFER, pts, 2));
#endif
	}

	void DBGDrawSphere(vector position, float size, int color = 0x44FFFFFF)
	{
		DBGAddShape(Shape.CreateSphere(color, ShapeFlags.WIREFRAME | ShapeFlags.NOZBUFFER, position, size));
	}

	void DGBDrawBoundingBox(vector transform[4], vector box[2], int color = 0x1fff7f7f)
	{
#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		Shape shape = Shape.Create(ShapeType.BBOX, color, ShapeFlags.TRANSP | ShapeFlags.NOZWRITE, box[0], box[1]);
		shape.SetMatrix(transform);
		DBGAddShape(shape);
#endif
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

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

		ctx.Write(m_PersistentIDA);
		ctx.Write(m_PersistentIDB);
		ctx.Write(m_PersistentIDC);
		ctx.Write(m_PersistentIDD);

		int lockState = m_VehicleLockedState;
		ctx.Write(lockState);

		ctx.Write(m_Exploded);

		if (!m_Expansion_IsStoreLoaded || m_Expansion_ForcedStoreLoadedPositionAndOrientation || m_Expansion_WasMissionLoadedAtVehicleInstantiation)
			GetCurrentOrientation();
		ctx.Write(m_Orientation);

		if (!m_Expansion_IsStoreLoaded || m_Expansion_ForcedStoreLoadedPositionAndOrientation || m_Expansion_WasMissionLoadedAtVehicleInstantiation)
			GetCurrentPosition();
		ctx.Write(m_Position);

		ctx.Write(false);
		ctx.Write(false);

		ctx.Write(m_CurrentSkinName);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (!ctx.Read(m_PersistentIDA))
			return false;

		if (!ctx.Read(m_PersistentIDB))
			return false;

		if (!ctx.Read(m_PersistentIDC))
			return false;

		if (!ctx.Read(m_PersistentIDD))
			return false;

		int lockState;
		if (!ctx.Read(lockState))
			return false;
		m_VehicleLockedState = lockState;

		if (!ctx.Read(m_Exploded))
			return false;

		if (!ctx.Read(m_Orientation))
			return false;

		if (!ctx.Read(m_Position))
			return false;

		bool isTowing;
		bool isBeingTowed;

		int id;

		if (!ctx.Read(isBeingTowed))
			return false;

		if (!ctx.Read(isTowing))
			return false;

		if (isBeingTowed)
		{
			if (!ctx.Read(id))
				return false;

			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
		}

		if (isTowing)
		{
			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
			if (!ctx.Read(id))
				return false;
		}

		return true;
	}
#endif

	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

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
		super.OnVariablesSynchronized();

#ifndef EXPANSION_VEHICLE_DESYNC_PROTECTION_DISABLE
		m_State.OnVariablesSynchronized_CarScript();
#endif

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
	}

	protected void ExpansionSetupSkins()
	{
		m_Skins = new array<ExpansionSkin>;

		if (CF_Modules<ExpansionSkinModule>.Get(m_SkinModule))
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
	}

	override array<ExpansionSkin> ExpansionGetSkins()
	{
		return m_Skins;
	}

	override bool ExpansionHasSkin(int skinIndex)
	{
		return skinIndex > -1 && skinIndex < m_Skins.Count();
	}
	
	ExpansionSkin ExpansionGetCurrentSkin()
	{
		return m_CurrentSkin;
	}

	string ExpansionGetCurrentSkinName()
	{
		return m_CurrentSkinName;
	}
	
	int ExpansionGetCurrentSkinIndex()
	{
		return m_CurrentSkinIndex;
	}

	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "EEHealthLevelChanged").Add(oldLevel).Add(newLevel).Add(zone);
#endif

		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

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

		bool isGlobalOrEngineRuined = (!zone || zone == "GlobalHealth" || zone == "Engine") && newLevel == GameConstants.STATE_RUINED;

		if (IsMissionClient())
		{
			vector fxPos;
			if (MemoryPointExists("fire_pos"))
			{
				fxPos = GetMemoryPointPos("fire_pos");
			}
			else
			{
				vector minMax[2];
				GetCollisionBox(minMax);
				fxPos = Vector(0, (minMax[1][1] - minMax[0][1]) / 2, 0);
			}

			if (zone == "Engine" && newLevel >= GameConstants.STATE_DAMAGED)
			{
				if (!SEffectManager.IsEffectExist(m_enginePtcFx))
				{
					if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionBoatScript))
					{
						m_engineFx = new EffEngineSmoke();
						m_enginePtcFx = SEffectManager.PlayOnObject(m_engineFx, this, fxPos, "0 0 0", true);
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

			if (isGlobalOrEngineRuined)
			{
				if (IsInherited(ExpansionHelicopterScript) || IsInherited(ExpansionBoatScript))
				{
					if (m_engineFx)
						m_engineFx.Stop();

					if (!m_Particles.Count())
					{
						CreateParticle(this, "fire_pos", ParticleList.EXPANSION_FIRE_HELICOPTER, "0 0 0", true);
						if (!m_Particles.Count())
							CreateParticleEx(ParticleList.EXPANSION_FIRE_HELICOPTER, this, fxPos, "0 0 0", true);
					}
				}
			}
		}

		if (IsMissionHost() && isGlobalOrEngineRuined && Expansion_EngineIsOn())
			Expansion_EngineStop();
	}

	override void ExpansionSetSkin(int skinIndex)
	{
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
	}

	void ExpansionOnSkinDamageZoneUpdate(ExpansionSkinDamageZone zone, int level)
	{
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
	}

	void ExpansionOnSkinUpdate()
	{
		if (!m_CurrentSkin)
		{
			CF_Log.Debug("ItemBase::ExpansionOnSkinUpdate called but m_CurrentSkin is NULL!");

			return;
		}

		if (m_CurrentSkin.HornEXT != "")
		{
			m_HornSoundSetEXT = m_CurrentSkin.HornEXT;
		}

		if (m_CurrentSkin.HornINT != "")
		{
			m_HornSoundSetINT = m_CurrentSkin.HornINT;
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

		if (attachment.IsInherited(CarWheel))
			return true;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SwitchGear");
#endif

		if (GetAnimationPhase("gear") == 0)
		{
			SetAnimationPhase("gear", 1);
		}
		else
		{
			SetAnimationPhase("gear", 0);
		}
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_IsCECreated = true;

		array<EntityAI> items = new array<EntityAI>;
		items.Reserve(GetInventory().CountInventory());

		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		for (int i = 0; i < items.Count(); i++)
		{
			ExpansionCarKey key;
			if (Class.CastTo(key, items[i]))
			{
				PairKeyTo(key);
			}
		}
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		Expansion_OnCEUpdate();
	}

	void Expansion_OnCEUpdate()
	{
		//! Prevent autocover before forcing initial storeloaded position
		if (m_Expansion_IsStoreLoaded && !m_Expansion_ForcedStoreLoadedPositionAndOrientation && !m_Expansion_WasMissionLoadedAtVehicleInstantiation)
			return;

		auto settings = GetExpansionSettings().GetVehicle(false);

		if (!settings.EnableVehicleCovers || settings.VehicleAutoCoverTimeSeconds <= 0)
			return;

		//! Prevent autocover if this is a CE spawned vehicle (lifetime will be 0 in that case) and autocovering spawned vehicles is disabled
		if (GetLifetime() <= 0 && !settings.EnableAutoCoveringDEVehicles)
			return;

		//! Defer autocover if engine is on or player is within vehicle bounding radius
		float playerAvoidanceRadius = m_BoundingRadius * 1.5;
		if (playerAvoidanceRadius <= 0)
			playerAvoidanceRadius = 150;
		if (Expansion_EngineIsOn() || !GetCEApi().AvoidPlayer(GetPosition(), playerAvoidanceRadius) || Expansion_GetVehicleCrew(false).Count())
		{
			m_Expansion_VehicleAutoCoverTimestamp = GetGame().GetTickTime();
			return;
		}

		if (GetGame().GetTickTime() - m_Expansion_VehicleAutoCoverTimestamp > settings.VehicleAutoCoverTimeSeconds)
		{
#ifdef EXPANSIONMODGARAGE
			//! Check if vehicle has any cargo items that are not attachments if the "CanStoreWithCargo" setting is enabled.
			if (!GetExpansionSettings().GetGarage().CanStoreWithCargo && MiscGameplayFunctions.Expansion_HasAnyCargo(this))
			{
				m_Expansion_VehicleAutoCoverTimestamp = GetGame().GetTime();
				return;
			}
#endif

			EntityAI cover = FindAttachmentBySlotName("CamoNet");
			if (settings.VehicleAutoCoverRequireCamonet && !cover)
			{
				m_Expansion_VehicleAutoCoverTimestamp = GetGame().GetTime();
				return;
			}

			if (Expansion_CanCover())
				Expansion_CoverVehicle(cover);
		}
	}

	bool Expansion_CoverVehicle(EntityAI cover = null, out ExpansionEntityStoragePlaceholder placeholder = null)
	{
		string coverType;

		if (cover)
			coverType = cover.GetType();
		else
			coverType = "CamoNet";

		string placeholderType = Expansion_GetPlaceholderType(coverType);

		bool storeCargo = GetExpansionSettings().GetVehicle().UseVirtualStorageForCoverCargo;
		if (ExpansionEntityStoragePlaceholder.Expansion_StoreEntityAndReplace(this, placeholderType, GetPosition(), ECE_OBJECT_SWAP, placeholder, storeCargo))
		{
			EXTrace.Print(EXTrace.VEHICLES, this, "Covered vehicle " + GetType() + " " + GetPosition() + " with " + coverType);

			//! If the cover was on the vehicle itself, it will be pending deletion and must not be moved to placeholder
			if (cover && !cover.IsSetForDeletion())
			{
				Man player = cover.GetHierarchyRootPlayer();
				if (player)
				{
					bool result = player.ServerTakeEntityToTargetAttachmentEx(placeholder, cover, InventorySlots.GetSlotIdFromString("CamoNet"));
					EXTrace.Print(EXTrace.VEHICLES, this, "Moved " + cover + " to " + placeholder + "? " + result);
				}
			}

			return true;
		}

		return false;
	}

	bool Expansion_CanCover()
	{		
		if (IsDamageDestroyed())
			return false;

		auto settings = GetExpansionSettings().GetVehicle();

		if (!settings.EnableVehicleCovers)
			return false;

		if (!m_Expansion_HasLifetime && !settings.AllowCoveringDEVehicles)
			return false;

		if (Expansion_GetVehicleCrew().Count())
			return false;

		if (!settings.CanCoverWithCargo)
		{
			if (MiscGameplayFunctions.Expansion_HasAnyCargo(this))
				return false;
		}

		return true;
	}

	string Expansion_GetPlaceholderType(string coverType)
	{
		string type = GetType();

		string skinBase = ConfigGetString("skinBase");
		if (skinBase)
			type = skinBase;

		string placeholderType;

		TStringArray coverVariants = {"Civil", "Desert", "Winter", ""};
		foreach (string coverVariant: coverVariants)
		{
			if (coverVariant == string.Empty)
				placeholderType = type + "_Cover";
			else if (coverType.Contains(coverVariant))
				placeholderType = type + "_Cover_" + coverVariant;
			else
				continue;

			if (GetGame().ConfigIsExisting("CfgVehicles " + placeholderType))
				break;
			else
				placeholderType = "Expansion_Generic_Vehicle_Cover";
		}

		return placeholderType;
	}

	float GetCameraHeight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraHeight");
#endif

		Error("DEPRECATED, use GetTransportCameraOffset");

		return GetTransportCameraOffset()[1];
	}

	float GetCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraDistance");
#endif

		Error("DEPRECATED, use GetTransportCameraDistance");

		return GetTransportCameraDistance();
	}

	float GetModelZeroPointDistanceFromGround()
	{
		if (m_ModelZeroPointDistanceFromGround < 0)
		{
			string path = "CfgVehicles " + GetType() + " modelZeroPointDistanceFromGround";
			if (GetGame().ConfigIsExisting(path))
			{
				m_ModelZeroPointDistanceFromGround = GetGame().ConfigGetFloat(path);
			}
			else
			{
				vector minMax[2];
				GetCollisionBox(minMax);
				float diff = -minMax[0][1];
				if (diff > 0)
					m_ModelZeroPointDistanceFromGround = diff;
				else
					m_ModelZeroPointDistanceFromGround = 0;
			}

			EXTrace.Print(EXTrace.VEHICLES, this, GetType() + " modelZeroPointDistanceFromGround " + m_ModelZeroPointDistanceFromGround);
		}

		return m_ModelZeroPointDistanceFromGround;
	}

	float GetWreckAltitude()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetWreckAltitude");
#endif

		return 1;
	}

	vector GetWreckOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetWreckOffset");
#endif

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "OnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif

		auto item = ItemBase.Cast(other);
		if (item && !dBodyIsDynamic(item) && item.Expansion_CarContactActivates() && !item.GetHierarchyParent())
		{
			item.ExpansionCreateDynamicFromContact(this, data.Position, data);
		}

		if (s_ExpansionPhysicsStructure_Enabled)
		{
			auto bakedObject = ExpansionBakedMapObject.Cast(other);
			if (bakedObject)
			{
				bakedObject.Create(this, data.Position, data);
				return;
			}

			auto staticObject = ExpansionStaticMapObject.Cast(other);
			if (staticObject)
			{
				staticObject.Create(this, data.Position, data);
				return;
			}
		}

		ExpansionWorld.CheckTreeContact(other, data.Impulse);

		if (GetGame().IsServer() && (!m_Expansion_CollisionDamageIfEngineOff || m_Expansion_CollisionDamageMinSpeed))
		{
			CarScript otherVehicle;
			bool otherVehicleEngineOn = Class.CastTo(otherVehicle, other) && otherVehicle.Expansion_EngineIsOn();

			if (!m_Expansion_CollisionDamageIfEngineOff)
			{
				if (!Expansion_EngineIsOn() && !otherVehicleEngineOn)
					return;
			}

			if (m_Expansion_CollisionDamageMinSpeed)
			{
				float minSpeedSq = m_Expansion_CollisionDamageMinSpeed * m_Expansion_CollisionDamageMinSpeed;
				if (data.RelativeVelocityBefore.LengthSq() < minSpeedSq && !otherVehicleEngineOn)
					return;

				float dmg = data.Impulse * m_dmgContactCoef;

				if (dmg <= m_Expansion_CollisionDamageMinSpeed * 97.2)
					return;
			}
		}

		super.OnContact(zoneName, localPos, other, data);
	}

	override void CheckContactCache()
	{
		int contactZonesCount = m_ContactCache.Count();

		if (contactZonesCount == 0)
			return;

		foreach (string zoneName, array<ref CarContactData> data: m_ContactCache)
		{
			float dmg = Math.AbsInt(data[0].impulse * m_dmgContactCoef);

			if (dmg < GameConstants.CARS_CONTACT_DMG_MIN)
				continue;

			float crewDmgBase = Math.AbsInt((data[0].impulse / dBodyGetMass(this)) * 1000 * m_dmgContactCoef);// calculates damage as if the object's weight was 1000kg instead of its actual weight

			int pddfFlags;

			if (dmg < GameConstants.CARS_CONTACT_DMG_THRESHOLD)
			{
				SynchCrashLightSound(true);
				pddfFlags = ProcessDirectDamageFlags.NO_TRANSFER;
			}
			else
			{
				float crewDmg = crewDmgBase * GetExpansionSettings().GetVehicle().VehicleCrewDamageMultiplier;
				if (crewDmg > 0)
					DamageCrew(crewDmg);
				SynchCrashHeavySound(true);
				pddfFlags = 0;
			}

			//! 1.19
			#ifdef DEVELOPER
			m_DebugContactDamageMessage += string.Format("%1: %2\n", zoneName, dmg);
			#endif

			//! This in turn invokes EEHitBy which deals with exploding the heli if its health reaches 0
			if (CanBeDamaged())
			{
				float collisionDamage = dmg * GetExpansionSettings().GetVehicle().VehicleSpeedDamageMultiplier;
				if (collisionDamage > 0)
					ProcessDirectDamage(DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", collisionDamage, pddfFlags);
			}
		}

		UpdateHeadlightState();
		UpdateLights();

		m_ContactCache.Clear();

		super.CheckContactCache();
	}

	override float GetBatteryConsumption()
	{
		return m_BatteryConsume;
	}

	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );

		if (GetExpansionSettings().GetLog().VehicleDestroyed && !m_Expansion_Killed)
    		GetExpansionSettings().GetLog().PrintLog("[VehicleDestroyed] " + GetType() + " (id=" + GetVehiclePersistentIDString() + " pos=" + GetPosition() + ")");

		m_Expansion_Killed = true;
	}
};
