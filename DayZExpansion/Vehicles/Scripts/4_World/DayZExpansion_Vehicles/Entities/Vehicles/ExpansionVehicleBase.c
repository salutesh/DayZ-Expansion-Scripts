/**
 * ExpansionVehicleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleDynamicState
{
	STATIC = 0,
	TRANSITIONING,
	DYNAMIC
};

class ExpansionVehicleBase extends ItemBase
{
	static ref set<ExpansionVehicleBase> m_allVehicles = new set<ExpansionVehicleBase>;

	ref array<ref ExpansionVehicleCrew> m_Crew = new array<ref ExpansionVehicleCrew>();

	ref array<ref ExpansionVehicleModule> m_Modules = new array<ref ExpansionVehicleModule>();
	ref array<ExpansionVehicleAxle> m_Axles = new array<ExpansionVehicleAxle>();
	ref array<ExpansionVehicleWheel> m_Wheels = new array<ExpansionVehicleWheel>();
	ref array<ExpansionVehicleAerofoil> m_Aerofoils = new array<ExpansionVehicleAerofoil>();
	ref array<ExpansionVehicleEngineBase> m_Engines = new array<ExpansionVehicleEngineBase>();
	ref array<ExpansionVehicleGearbox> m_Gearboxes = new array<ExpansionVehicleGearbox>();

	ref ExpansionController m_Controller = new ExpansionController();
	ref ExpansionPhysicsState m_State = new ExpansionPhysicsStateT<ExpansionVehicleBase>(this);

	ref ExpansionVehicleModuleEvent m_Event_TEMP_DeferredInit = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_SettingsChanged = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Control = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_PreSimulate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Simulate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_PostSimulate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_Animate = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_NetworkSend = new ExpansionVehicleModuleEvent();
	ref ExpansionVehicleModuleEvent m_Event_NetworkRecieve = new ExpansionVehicleModuleEvent();

	ExpansionVehicleDifferentialType m_DifferentialType;

	int m_CurrentEngine;
	int m_EnginesOn;
	float m_PreviousYaw;

	bool m_RecievedInitialSync;

	bool m_HasDriver;

	bool m_Expansion_HasPilot;

	float m_MaxSpeed;	// (km/h)
	float m_MaxSpeedMS; // (m/s)

	float m_AltitudeFullForce; // (m)
	float m_AltitudeNoForce;   // (m)

	float m_Expansion_Mass;

	float m_TimeSlice;
	bool m_IsPhysicsHost;

	float m_RenderFrameTimeSlice;

	bool m_PhysicsCreated;
	bool m_PhysicsDestroyed;

	float m_BoundingRadius;
	vector m_BoundingBox[2];

	float m_AirDragCoefficient;
	float m_AirDragArea;
	float m_AirDragConstant;

	EffectSound m_HornSound;

	string m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
	string m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";

	bool m_Exploded;
	bool m_ExplodedSynchRemote;
	int m_ExplosionSize;

	bool m_HornPlaying;
	bool m_HornSynchRemote;

	ExpansionVehicleLockState m_VehicleLockedState;

	//! After pairing a key, it's the ID of the master key.
	//! This allows "changing locks" on vehicles so old paired keys no longer work
	int m_PersistentIDA = 0;
	int m_PersistentIDB = 0;
	int m_PersistentIDC = 0;
	int m_PersistentIDD = 0;

	ref array<float> m_SoundVariables;
	ref array<string> m_SoundControls;
	ref array<ref ExpansionVehicleSound> m_SoundControllers;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
	vector m_DbgTransform[4];
#endif

	// Floating
	float m_WaterVolume;
	float m_TotalVolume;

	// Effects
	float m_AltitudeLimiter;

	ExpansionVehicleNetworkMode m_NetworkMode;
	vector m_LastCheckedNetworkPosition;

	ref array<ExpansionPointLight> m_Lights;
	ref array<Particle> m_Particles;

	autoptr TStringArray m_Doors;
	bool m_CanHaveLock;

	ref array<float> m_FluidCapacities;

	// Debugging
	ref array<Shape> m_DebugShapes;

	//! CarScript
	float m_Time;

	static float DROWN_ENGINE_THRESHOLD = 0.5;
	static float DROWN_ENGINE_DAMAGE = 350.0;

	//! keeps ammount of each fluid
	float m_FuelAmmount;
	float m_CoolantAmmount;
	float m_OilAmmount;
	float m_BrakeAmmount;

	//!
	float m_dmgContactCoef;
	float m_EnviroHeatComfortOverride;

	//!
	float m_DrownTime;
	static vector m_DrownEnginePos;

	//!
	float m_EngineHealth;
	float m_RadiatorHealth;
	float m_FuelTankHealth;
	float m_BatteryHealth;
	float m_PlugHealth;

	//! Particles
	ref EffVehicleSmoke m_coolantFx;
	ref EffVehicleSmoke m_engineFx;
	ref EffVehicleSmoke m_exhaustFx;

	int m_enginePtcFx;
	int m_coolantPtcFx;
	int m_exhaustPtcFx;

	vector m_exhaustPtcPos;
	vector m_exhaustPtcDir;
	vector m_enginePtcPos;
	vector m_coolantPtcPos;

	vector m_fuelPos;

	vector m_enginePos;
	vector m_frontPos;
	vector m_backPos;
	vector m_side_1_1Pos;
	vector m_side_1_2Pos;
	vector m_side_2_1Pos;
	vector m_side_2_2Pos;

	//!Sounds
	string m_EngineStartOK = "";
	string m_EngineStartBattery = "";
	string m_EngineStartPlug = "";
	string m_EngineStartFuel = "";
	string m_EngineStopFuel = "";

	string m_CarDoorOpenSound = "";
	string m_CarDoorCloseSound = "";

	bool m_PlayCrashSoundLight;
	bool m_PlayCrashSoundHeavy;

	bool m_HeadlightsOn;
	bool m_HeadlightsState;
	bool m_BrakesArePressed; // synchronized variable

	CarLightBase m_Headlight;
	CarRearLightBase m_RearLight;

	bool m_EngineSync0;
	bool m_EngineSync1;
	bool m_EngineSync2;
	bool m_EngineSync3;

	// Memory points for lights
	static string m_ReverseLightPoint = "light_reverse";
	static string m_LeftHeadlightPoint = "light_left";
	static string m_RightHeadlightPoint = "light_right";
	static string m_LeftHeadlightTargetPoint = "light_left_dir";
	static string m_RightHeadlightTargetPoint = "light_right_dir";

	static string m_DrownEnginePoint = "drown_engine";

	// Model selection IDs for texture/material changes
	// If each car needs different IDs, then feel free to remove the 'static' flag and overwrite these numbers down the hierarchy
	static const int SELECTION_ID_FRONT_LIGHT_L = 0;
	static const int SELECTION_ID_FRONT_LIGHT_R = 1;
	static const int SELECTION_ID_BRAKE_LIGHT_L = 2;
	static const int SELECTION_ID_BRAKE_LIGHT_R = 3;
	static const int SELECTION_ID_REVERSE_LIGHT_L = 4;
	static const int SELECTION_ID_REVERSE_LIGHT_R = 5;
	static const int SELECTION_ID_TAIL_LIGHT_L = 6;
	static const int SELECTION_ID_TAIL_LIGHT_R = 7;
	static const int SELECTION_ID_DASHBOARD_LIGHT = 8;

	float m_ModelZeroPointDistanceFromGround = -1;

	autoptr ExpansionZoneActor m_Expansion_SafeZoneInstance = new ExpansionZoneEntity<ExpansionVehicleBase>(this);
	bool m_SafeZone;

	protected string m_Expansion_LastDriverUID;
	protected bool m_Expansion_SynchLastDriverUID;
	protected bool m_Expansion_LastDriverUIDSynched;

	int m_Expansion_CargoCount;

#ifndef EXPANSIONMODBASEBUILDING
	ref ExpansionGlobalID m_Expansion_GlobalID;
#endif

	bool m_Expansion_HasLifetime;

	void ExpansionVehicleBase()
	{
#ifndef DIAG
		if (GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Delete);

		if (!GetGame().IsDedicatedServer())
			ExpansionNotification("ERROR", GetType() + " IS NOT READY FOR PRODUCTION! Use " + GetType().Substring(8, GetType().Length() - 8) + " instead.").Error();
	
		Print(GetType() + " IS NOT READY FOR PRODUCTION! Use " + GetType().Substring(8, GetType().Length() - 8) + " instead");
#endif

		m_Expansion_GlobalID = new ExpansionGlobalID();

		SetFlags(EntityFlags.ACTIVE | EntityFlags.SOLID | EntityFlags.VISIBLE, false);
		SetEventMask(EntityEvent.SIMULATE | EntityEvent.POSTSIMULATE | EntityEvent.INIT | EntityEvent.CONTACT | EntityEvent.FRAME | EntityEvent.POSTFRAME | EntityEvent.PHYSICSMOVE);

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

		RegisterNetSyncVariableBool("m_Expansion_AcceptingAttachment");
		RegisterNetSyncVariableBool("m_Expansion_CanPlayerAttach");

		RegisterNetSyncVariableInt("m_Expansion_CargoCount");

		RegisterNetSyncVariableInt("m_CurrentEngine");

		RegisterNetSyncVariableBool("m_PlayCrashSoundLight");
		RegisterNetSyncVariableBool("m_PlayCrashSoundHeavy");
		RegisterNetSyncVariableBool("m_HeadlightsOn");
		RegisterNetSyncVariableBool("m_BrakesArePressed");

		if (MemoryPointExists("ptcExhaust_end"))
		{
			//Print( "CarScript Init " + GetType() );
			m_exhaustPtcPos = GetMemoryPointPos("ptcExhaust_end");
			if (MemoryPointExists("ptcExhaust_start"))
			{
				vector exhaustStart = GetMemoryPointPos("ptcExhaust_start");
				vector tempOri = vector.Direction(exhaustStart, m_exhaustPtcPos); //.Normalized();//.VectorToAngles();

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

		if (MemoryPointExists("ptcEnginePos"))
			m_enginePtcPos = GetMemoryPointPos("ptcEnginePos");
		else
			m_enginePtcPos = "0 0 0";

		if (MemoryPointExists("ptcCoolantPos"))
			m_coolantPtcPos = GetMemoryPointPos("ptcCoolantPos");
		else
			m_coolantPtcPos = "0 0 0";

		if (MemoryPointExists("refill"))
			m_fuelPos = GetMemoryPointPos("refill");
		else
			m_fuelPos = "0 0 0";

		if (MemoryPointExists("drown_engine"))
			m_DrownEnginePos = GetMemoryPointPos("drown_engine");
		else
			m_DrownEnginePos = "0 0 0";

		if (MemoryPointExists("dmgZone_engine"))
			m_enginePos = GetMemoryPointPos("dmgZone_engine");
		else
			m_enginePos = "0 0 0";

		if (MemoryPointExists("dmgZone_front"))
			m_frontPos = GetMemoryPointPos("dmgZone_front");
		else
			m_frontPos = "0 0 0";

		if (MemoryPointExists("dmgZone_back"))
			m_backPos = GetMemoryPointPos("dmgZone_back");
		else
			m_backPos = "0 0 0";

		if (MemoryPointExists("dmgZone_fender_1_1"))
			m_side_1_1Pos = GetMemoryPointPos("dmgZone_fender_1_1");
		else
			m_side_1_1Pos = "0 0 0";

		if (MemoryPointExists("dmgZone_fender_1_2"))
			m_side_1_2Pos = GetMemoryPointPos("dmgZone_fender_1_2");
		else
			m_side_1_2Pos = "0 0 0";

		if (MemoryPointExists("dmgZone_fender_2_1"))
			m_side_2_1Pos = GetMemoryPointPos("dmgZone_fender_2_1");
		else
			m_side_2_1Pos = "0 0 0";

		if (MemoryPointExists("dmgZone_fender_2_2"))
			m_side_2_2Pos = GetMemoryPointPos("dmgZone_fender_2_2");
		else
			m_side_2_2Pos = "0 0 0";

		RegisterNetSyncVariableInt("m_PersistentIDA");
		RegisterNetSyncVariableInt("m_PersistentIDB");
		RegisterNetSyncVariableInt("m_PersistentIDC");
		RegisterNetSyncVariableInt("m_PersistentIDD");

		RegisterNetSyncVariableBool("m_HornSynchRemote");
		RegisterNetSyncVariableBool("m_ExplodedSynchRemote");

		RegisterNetSyncVariableFloat("m_FuelAmmount", 0, 0, 4);
		RegisterNetSyncVariableFloat("m_CoolantAmmount", 0, 0, 4);
		RegisterNetSyncVariableFloat("m_OilAmmount", 0, 0, 4);
		RegisterNetSyncVariableFloat("m_BrakeAmmount", 0, 0, 4);

		RegisterNetSyncVariableInt("m_NetworkMode");

		RegisterNetSyncVariableBool("m_Expansion_SynchLastDriverUID");

		m_DebugShapes = new array<Shape>();

		m_allVehicles.Insert(this);

		string path;

		path = "CfgVehicles " + GetType() + " SimulationModule airDragFrontTotal";
		if (GetGame().ConfigIsExisting(path))
			m_AirDragCoefficient = GetGame().ConfigGetFloat(path);

		vector airDragMin, airDragMax;
		bool airDragOverride = true;

		path = "CfgVehicles " + GetType() + " SimulationModule airDragSizeMin";
		airDragOverride = airDragOverride & GetGame().ConfigIsExisting(path);
		if (airDragOverride)
			airDragMin = GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		path = "CfgVehicles " + GetType() + " SimulationModule airDragSizeMax";
		airDragOverride = airDragOverride & GetGame().ConfigIsExisting(path);
		if (airDragOverride)
			airDragMax = GetMemoryPointPos(GetGame().ConfigGetTextOut(path));

		m_AirDragArea = -1;
		if (airDragOverride)
		{
			float ad_w = Math.AbsFloat(airDragMin[0]) + Math.AbsFloat(airDragMax[0]);
			float ad_h = Math.AbsFloat(airDragMin[1]) + Math.AbsFloat(airDragMax[1]);
			m_AirDragArea = ad_w * ad_h;
		}

		path = "CfgVehicles " + GetType() + " SimulationModule drive";

		m_DifferentialType = ExpansionVehicleDifferentialType.RWD;
		if (GetGame().ConfigIsExisting(path))
		{
			string drive;
			GetGame().ConfigGetText(path, drive);

			if (drive == "DRIVE_RWD")
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.RWD;
			}
			else if (drive == "DRIVE_FWD")
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.FWD;
			}
			else if (drive == "DRIVE_AWD")
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.AWD;
			}
			else if (drive == "DRIVE_662")
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.AWD;
			}
			else if (drive == "DRIVE_642")
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.AWD;
			}
			else
			{
				m_DifferentialType = ExpansionVehicleDifferentialType.AWD;
			}
		}

		int i = 0;
		int count = 0;

		path = "CfgVehicles " + GetType() + " SimulationModule Gearboxes";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string gearName;
			GetGame().ConfigGetChildName(path, i, gearName);

			string gearPath = path + " " + gearName;
			AddModule(Expansion_CreateGearbox(this, gearPath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Engines";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string engineName;
			GetGame().ConfigGetChildName(path, i, engineName);

			string enginePath = path + " " + engineName;
			AddModule(Expansion_CreateEngine(this, enginePath));
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

		path = "CfgVehicles " + GetType() + " SimulationModule Axles";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string axleName;
			GetGame().ConfigGetChildName(path, i, axleName);

			string wheelPath = "CfgVehicles " + GetType() + " SimulationModule Axles " + axleName + " Wheels";
			int wheelCount = GetGame().ConfigGetChildrenCount(wheelPath);

			ExpansionVehicleAxle axle = NULL;
			ExpansionVehicleWheel wheel = NULL;

			// Order matters, Axle must be added before the Wheel(s)

			if (wheelCount == 1)
			{
				axle = new ExpansionVehicleOneWheelAxle(this, axleName, i);
				AddModule(axle);

				wheel = axle.AddWheel("Center");
				AddModule(wheel);
			}
			else if (wheelCount == 2)
			{
				axle = new ExpansionVehicleTwoWheelAxle(this, axleName, i);
				AddModule(axle);

				wheel = axle.AddWheel("Left");
				AddModule(wheel);

				wheel = axle.AddWheel("Right");
				AddModule(wheel);
			}
			else if (wheelCount != 0)
			{
				axle = new ExpansionVehicleAxle(this, axleName, i);
				AddModule(axle);

				for (int j = 0; j < wheelCount; j++)
				{
					string wheelName;
					GetGame().ConfigGetChildName(wheelPath, j, wheelName);

					wheel = axle.AddWheel(wheelName);
					AddModule(wheel);
				}
			}
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Aerofoils";
		count = GetGame().ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string aerofoilName;
			GetGame().ConfigGetChildName(path, i, aerofoilName);

			AddModule(new ExpansionVehicleAerofoil(this, aerofoilName));
		}

		path = "CfgVehicles " + GetType() + " Crew";
		count = GetGame().ConfigGetChildrenCount(path);
		for (i = 0; i < count; i++)
		{
			string crewName;
			GetGame().ConfigGetChildName(path, i, crewName);

			m_Crew.Insert(new ExpansionVehicleCrew(this, crewName));
		}

		m_SoundVariables = new array<float>();
		for (i = 0; i < ExpansionVehicleSoundManager.s_SoundShaderParameters.Count(); i++)
			m_SoundVariables.Insert(0);

		m_SoundControllers = new array<ref ExpansionVehicleSound>();

		if (IsMissionClient())
		{
			array<string> soundSetNames = new array<string>();

			path = "CfgVehicles " + GetType() + " Sounds soundSetsFilter";
			GetGame().ConfigGetTextArray(path, soundSetNames);

			for (i = 0; i < soundSetNames.Count(); i++)
			{
				m_SoundControllers.Insert(new ExpansionVehicleSound(this, soundSetNames[i]));
			}

			path = "CfgVehicles " + GetType() + " Sounds soundSetsInt";
			GetGame().ConfigGetTextArray(path, soundSetNames);

			for (i = 0; i < soundSetNames.Count(); i++)
			{
				m_SoundControllers.Insert(new ExpansionVehicleSound(this, soundSetNames[i]));
			}
		}

		m_VehicleLockedState = ExpansionVehicleLockState.NOLOCK;

		m_Doors = new TStringArray;
		ConfigGetTextArray("doors", m_Doors);

		m_CanHaveLock = m_Doors.Count() > 0;

		string cHSSE = "hornSoundSetEXT";
		if (ConfigIsExisting(cHSSE))
		{
			m_HornSoundSetEXT = ConfigGetString(cHSSE);
		}
		else
		{
			m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		}

		string cHSSI = "hornSoundSetINT";
		if (ConfigIsExisting(cHSSI))
		{
			m_HornSoundSetINT = ConfigGetString(cHSSI);
		}
		else
		{
			m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
		}

		m_NetworkMode = ExpansionVehicleNetworkMode.SERVER;

		m_State.RegisterSync("m_State");

		m_FluidCapacities = new array<float>();
		m_FluidCapacities.Insert(ConfigGetFloat("fuelCapacity"));
		m_FluidCapacities.Insert(ConfigGetFloat("oilCapacity"));
		m_FluidCapacities.Insert(ConfigGetFloat("brakeFluidCapacity"));
		m_FluidCapacities.Insert(ConfigGetFloat("coolantCapacity"));

		m_Lights = new array<ExpansionPointLight>;
		m_Particles = new array<Particle>;

		ExpansionSettings.SI_Vehicle.Insert(OnSettingsUpdated);

		if (GetGame().IsServer())
			m_Event_SettingsChanged.SettingsChanged();

		LoadConstantVariables();

		Fill(CarFluid.FUEL, GetFluidCapacity(CarFluid.FUEL));

		if (IsMissionHost())
			SetAllowDamage(CanBeDamaged());

		//! This exists so it can be overridden (e.g.) by server owners who don't have access to unbinarized models
		path = "CfgVehicles " + GetType() + " mass";
		if (GetGame().ConfigIsExisting(path))
			m_Expansion_Mass = GetGame().ConfigGetFloat(path);
		else
			m_Expansion_Mass = dBodyGetMass(this);

		if (GetGame().IsServer())
		{
			foreach (ExpansionVehiclesConfig vehcfg : GetExpansionSettings().GetVehicle().VehiclesConfig)
			{
				if (IsKindOf(vehcfg.ClassName))
				{
					m_Expansion_CanPlayerAttach = vehcfg.CanPlayerAttach;
					break;
				}
			}
		}
	}

	void ~ExpansionVehicleBase()
	{
#ifdef CF_DebugUI
		CF_Debug.Destroy(this);
#endif

		ExpansionSettings.SI_Vehicle.Remove(OnSettingsUpdated);

		int i;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();
#endif

		delete m_DebugShapes;

		i = m_allVehicles.Find(this);
		if (i >= 0)
		{
			m_allVehicles.Remove(i);
		}
	}

	static set<ExpansionVehicleBase> GetAll()
	{
		return m_allVehicles;
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			if (SEffectManager.IsEffectExist(m_coolantPtcFx))
				SEffectManager.Stop(m_coolantPtcFx);

			if (SEffectManager.IsEffectExist(m_exhaustPtcFx))
				SEffectManager.Stop(m_exhaustPtcFx);

			if (SEffectManager.IsEffectExist(m_enginePtcFx))
				SEffectManager.Stop(m_enginePtcFx);

			if (m_Headlight)
				m_Headlight.Destroy();

			if (m_RearLight)
				m_RearLight.Destroy();
		}
		
#ifdef SERVER
		if (GetExpansionSettings().GetLog().VehicleDeleted)
		{
			GetExpansionSettings().GetLog().PrintLog("[VehicleDeleted] " + GetType() + " (id=" + GetVehiclePersistentIDString() + " pos=" + GetPosition().ToString() + ")");
		}
#endif
	}

	void LoadConstantVariables()
	{
	}

	void OnAfterLoadConstantVariables()
	{
	}

	override void DeferredInit()
	{
		super.DeferredInit();

#ifdef CF_DebugUI
		CF_Debug.Create(this);
#endif

		int i;

		array<ref ExpansionVehicleModule> tempModules = new array<ref ExpansionVehicleModule>();
		for (i = 0; i < m_Modules.Count(); i++)
			tempModules.Insert(m_Modules[i]);

		m_Modules.Clear();

		for (i = 0; i < m_Engines.Count(); i++)
			InitModule(m_Engines[i]);

		for (i = 0; i < m_Gearboxes.Count(); i++)
			InitModule(m_Gearboxes[i]);

		for (i = 0; i < m_Aerofoils.Count(); i++)
			InitModule(m_Aerofoils[i]);

		for (i = 0; i < m_Axles.Count(); i++)
			InitModule(m_Axles[i]);

		for (i = 0; i < m_Wheels.Count(); i++)
			InitModule(m_Wheels[i]);

		for (i = 0; i < tempModules.Count(); i++)
			if (m_Modules.Find(tempModules[i]) == -1)
				InitModule(tempModules[i]);

		m_BoundingRadius = ClippingInfo(m_BoundingBox);

		if (m_AirDragArea == -1)
		{
			float w = Math.AbsFloat(m_BoundingBox[0][0]) + Math.AbsFloat(m_BoundingBox[1][0]);
			float h = Math.AbsFloat(m_BoundingBox[0][1]) + Math.AbsFloat(m_BoundingBox[1][1]);
			m_AirDragArea = w * h;
		}

		m_AirDragConstant = m_AirDragArea * m_AirDragCoefficient * 0.5;

		m_State.m_MaxSpeed = m_MaxSpeed;
		m_State.m_AltitudeFullForce = m_AltitudeFullForce;
		m_State.m_AltitudeNoForce = m_AltitudeNoForce;

		m_State.Init();

		m_BoundingRadius = m_State.m_BoundingRadius;
		m_BoundingBox[0] = m_State.m_BoundingBox[0];
		m_BoundingBox[1] = m_State.m_BoundingBox[1];
		m_MaxSpeedMS = m_State.m_MaxSpeedMS;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnAfterLoadConstantVariables, 100, false);

		if (GetGame().IsClient())
		{
			auto rpc = ExpansionScriptRPC.Create();
			rpc.Send(this, ExpansionVehicleRPC.RequestCrewSync, true, NULL);
		}

		float minHeight = 0;
		for (i = 0; i < m_Wheels.Count(); i++)
		{
			float height = m_Wheels[i].m_InitialTransform[3][1] - 0.5;
			if (height < minHeight)
			{
				minHeight = height;
			}
		}

		vector position = GetOrigin();
		position[1] = GetGame().SurfaceY(position[0], position[2]) - minHeight;
		SetOrigin(position);
	}

	void InitModule(ExpansionVehicleModule module)
	{
		m_Modules.Insert(module);

		module.Init();

		if (module.m_TEMP_DeferredInit)
			m_Event_TEMP_DeferredInit.Add(module);

		if (module.m_SettingsChanged)
			m_Event_SettingsChanged.Add(module);

		if (module.m_Control)
			m_Event_Control.Add(module);

		if (module.m_PreSimulate)
			m_Event_PreSimulate.Add(module);

		// Simulate happens in reverse order for transmission of acceleration up the drive shafts from the wheel sources
		if (module.m_Simulate)
			m_Event_Simulate.AddStart(module);

		if (module.m_PostSimulate)
			m_Event_PostSimulate.AddStart(module);

		if (module.m_Animate)
			m_Event_Animate.Add(module);

		if (module.m_Network)
		{
			m_Event_NetworkSend.Add(module);
			m_Event_NetworkRecieve.Add(module);
		}
	}

	void OnSettingsUpdated()
	{
		//m_NetworkMode = GetExpansionSettings().GetVehicle().VehicleSync;

		m_Event_SettingsChanged.SettingsChanged();
	}

	override void OnCreatePhysics()
	{
	}

	override void EOnContact(IEntity other, Contact extra) //!EntityEvent.CONTACT
	{
		array<string> damageZones = new array<string>;
		GetDamageZones(damageZones);

		string zoneName = "";
		vector zonePosition;
		float largestDistance = 32000.0;
		vector contactMS = WorldToModel(extra.Position);

		for (int i = 0; i < damageZones.Count(); i++)
		{
			vector pos = GetDamageZonePos(damageZones[i]);
			if (vector.Distance(pos, contactMS) < largestDistance)
			{
				zonePosition = pos;
				zoneName = damageZones[i];
			}
		}

		auto item = ItemBase.Cast(other);
		if (item && !dBodyIsDynamic(item) && item.Expansion_CarContactActivates() && !item.GetHierarchyParent())
		{
			item.ExpansionCreateDynamicFromContact(this, extra.Position, extra);
		}

		if (s_ExpansionPhysicsStructure_Enabled)
		{
			auto bakedObject = ExpansionBakedMapObject.Cast(other);
			if (bakedObject)
			{
				bakedObject.Create(this, extra.Position, extra);
				return;
			}

			auto staticObject = ExpansionStaticMapObject.Cast(other);
			if (staticObject)
			{
				staticObject.Create(this, extra.Position, extra);
				return;
			}
		}

		if (zoneName != "")
			OnContact(zoneName, contactMS, other, extra);
	}

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		if (CanUpdateCarLock(timeSlice))
		{
			UpdateCarLock(timeSlice);
		}

		m_Time += timeSlice;

		//! move it to constants.c const float CAR_UPDATE_INTERVAL = 1.0
		if (m_Time >= GameConstants.CARS_FLUIDS_TICK)
		{
			float time = m_Time;
			m_Time = 0;

			CarPartsHealthCheck();

			//First of all check if the car should stop the engine
			if (GetGame().IsServer() && EnginesOn() > 0)
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
				//CheckVitalItem( IsVitalEngineBelt(), "EngineBelt" );
			}

			float fuel = GetFluidFraction(CarFluid.FUEL);
			float coolant = GetFluidFraction(CarFluid.COOLANT);
			float oil = GetFluidFraction(CarFluid.OIL);
			float brake = GetFluidFraction(CarFluid.BRAKE);
			if (!IsVitalRadiator())
				coolant = 1.0;

			float fuelConsumption;
			if (GetGame().IsServer() && m_Engines.Count() > 0)
			{
				m_EngineHealth = 0;
				for (int i = 0; i < m_Engines.Count(); i++)
				{
					m_Engines[i].ProcessHealth(time, fuel, coolant, oil, brake, m_EngineHealth, fuelConsumption);
				}
				m_EngineHealth = m_EngineHealth / m_Engines.Count();
			}

			//! actions runned when the engine on
			if (EnginesOn() > 0)
			{
				if (GetGame().IsServer())
				{
					//! leaking of coolant from radiator when damaged
					if (IsVitalRadiator() && coolant > 0.0 && m_RadiatorHealth < 0.5) //CARS_LEAK_THRESHOLD
						LeakFluid(CarFluid.COOLANT);

					if (fuel > 0.0 && m_FuelTankHealth < 0.5)
						LeakFluid(CarFluid.FUEL);

					if (brake > 0.0 && m_EngineHealth < 0.5)
						LeakFluid(CarFluid.BRAKE);

					if (oil > 0.0 && m_EngineHealth < 0.5)
						LeakFluid(CarFluid.OIL);

					if (fuelConsumption > 0.0)
						Leak(CarFluid.FUEL, fuelConsumption);
				}

				//FX only on Client and in Single
				if (!GetGame().IsMultiplayer() || GetGame().IsClient())
				{
					if (!SEffectManager.IsEffectExist(m_exhaustPtcFx))
					{
						m_exhaustFx = new EffExhaustSmoke();
						m_exhaustPtcFx = SEffectManager.PlayOnObject(m_exhaustFx, this, m_exhaustPtcPos, m_exhaustPtcDir);
					}

					m_exhaustFx.SetParticleStateLight();

					if (IsVitalRadiator() && SEffectManager.IsEffectExist(m_coolantPtcFx))
						SEffectManager.Stop(m_coolantPtcFx);

					if (IsVitalRadiator() && GetFluidFraction(CarFluid.COOLANT) < 0.5)
					{
						if (!SEffectManager.IsEffectExist(m_coolantPtcFx))
						{
							m_coolantFx = new EffCoolantSteam();
							m_coolantPtcFx = SEffectManager.PlayOnObject(m_coolantFx, this, m_coolantPtcPos, Vector(0, 0, 0), true);
						}

						if (GetFluidFraction(CarFluid.COOLANT) > 0)
							m_coolantFx.SetParticleStateLight();
						else
							m_coolantFx.SetParticleStateHeavy();
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
				//FX only on Client and in Single
				if (!GetGame().IsMultiplayer() || GetGame().IsClient())
				{
					if (SEffectManager.IsEffectExist(m_exhaustPtcFx))
						SEffectManager.Stop(m_exhaustPtcFx);

					if (SEffectManager.IsEffectExist(m_coolantPtcFx))
						SEffectManager.Stop(m_coolantPtcFx);
				}
			}
		}

		if (!GetGame().IsClient() && m_NetworkMode == ExpansionVehicleNetworkMode.CLIENT)
		{
			return;
		}
	}

	override void EOnPostFrame(IEntity other, int extra)
	{
		OnAnimationUpdate(m_RenderFrameTimeSlice);
	}

	override void EOnFrame(IEntity other, float timeSlice)
	{
		m_RenderFrameTimeSlice = timeSlice;
		OnAnimationUpdate(timeSlice);
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

	bool IsPhysicsHost()
	{
		return m_IsPhysicsHost;
	}

	bool Expansion_ShouldDisableSimulation()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_ShouldDisableSimulation");
#endif

		vector velocity = GetVelocity(this);
		if (velocity.LengthSq() > 0.01)
			return false;

		return true;
	}

	void HandleController(DayZPlayerImplement driver, float dt)
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

	void OnPreSimulation(float pDt)
	{
		if (IsMissionClient())
		{
			OnParticleUpdate(pDt);
		}

		OnUpdate(pDt);
	}

	void OnNoSimulation(float pDt)
	{
	}

	void OnSimulation(ExpansionPhysicsState pState)
	{
		float speed = pState.m_LinearVelocityMS[2];
		float drag = m_AirDragConstant * -Expansion_GetDensity(GetPosition()) * speed * speed;
		pState.m_Force += Vector(0, 0, drag).Multiply3(pState.m_Transform);

		vector gravity = dGetGravity(this);
		pState.m_Torque += gravity * pState.m_Mass;
	}

	void OnPostSimulation(float pDt)
	{
		OnAnimationUpdate(pDt);

		float rpm = 0;
		float engineOn = 0;

		if (m_Engines.Count() > 0)
		{
			rpm = EngineGetRPM();
			engineOn = EngineIsOn();
		}

		//rpm
		m_SoundVariables[0] = OnSound(CarSoundCtrl.RPM, rpm);

		//engineOn
		m_SoundVariables[1] = OnSound(CarSoundCtrl.ENGINE, engineOn);

		//campos
		m_SoundVariables[2] = 1;
		auto player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			if (!player.IsInThirdPerson())
			{
				if (player.GetParent() == this)
				{
					m_SoundVariables[2] = 0;
				}
			}
		}

		//doors
		m_SoundVariables[3] = OnSound(CarSoundCtrl.DOORS, 0);

		//speed
		m_SoundVariables[4] = OnSound(CarSoundCtrl.SPEED, Math.AbsFloat(GetSpeedometer()));

		//thrust
		m_SoundVariables[5] = m_Controller.m_Throttle[0];

		//water
		m_SoundVariables[6] = 0;

		//rock
		m_SoundVariables[7] = 0;

		//grass
		m_SoundVariables[8] = 0;

		//gravel
		m_SoundVariables[9] = 0;

		//asphalt
		m_SoundVariables[10] = 0;

		//latSlipDrive
		m_SoundVariables[11] = 0;

		//steerdelta
		m_SoundVariables[12] = m_Controller.m_Yaw - m_PreviousYaw;
		m_PreviousYaw = m_Controller.m_Yaw;

		//rain
		m_SoundVariables[13] = 0;

		//damperLeft
		m_SoundVariables[14] = 0;

		//damperRight
		m_SoundVariables[15] = 0;

		for (int i = 0; i < m_SoundControllers.Count(); i++)
			m_SoundControllers[i].Update(pDt, m_SoundVariables);
	}

	void OnParticleUpdate(float pDt)
	{
	}

	void OnAnimationUpdate(float pDt)
	{
		m_State.m_DeltaTime = pDt;

		m_Event_Animate.Animate(m_State);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		int i;

		instance.Add(m_Controller);
		instance.Add(m_State);

		instance.Add("Physics Host", m_IsPhysicsHost);

		instance.Add("Num Modules", m_Modules.Count());
		for (i = 0; i < m_Modules.Count(); i++)
			if (!m_Modules[i].m_SelfDebugWindow)
				instance.Add(m_Modules[i]);

		instance.Add("Num Sounds", m_SoundControllers.Count());
		for (i = 0; i < m_SoundControllers.Count(); i++)
			instance.Add(m_SoundControllers[i]);

		return true;
	}
#endif

	override void EOnSimulate(IEntity other, float dt)
	{
		int i;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();

		//Expansion_DBGTowing();

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

		//m_State.DBGDrawSphereMS("0 0 0", 0.5, 0x44AA00FF);
		//m_State.DBGDrawSphereMS(dBodyGetCenterOfMass(this), 0.3, 0x44FF0022);

#endif
		if (!dBodyIsDynamic(this))
		{
			m_State.CreateDynamic();
			return;
		}

		m_State.m_DeltaTime = dt;

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
				EngineStop();  //! Stop engine. Heli will autorotate to ground.
			}
		}

		if (GetGame().IsServer())
		{
			m_IsPhysicsHost = true;

			if (GetGame().IsMultiplayer() && vector.Distance(m_LastCheckedNetworkPosition, GetPosition()) > 30.0)
			{
				m_LastCheckedNetworkPosition = GetPosition();

				//for (i = 0; i < m_Crew.Count(); i++)
				//	m_Crew[i].NetworkBubbleFix();
			}

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

			if (!driver)
			{
				GetTransform(m_State.m_TargetTransform);

				m_State.m_DeltaTime = dt;

				m_Event_Control.Control(m_State, null);

				Expansion_OnHandleController(null, dt);

				if (dBodyIsActive(this) && m_Expansion_TowConnectionMask == 0 && Expansion_ShouldDisableSimulation())
				{
				//	dBodyActive(this, ActiveState.INACTIVE);
				//	return;
				}
			}
		}
		else if (GetGame().IsClient())
		{
			m_IsPhysicsHost = driver == GetGame().GetPlayer();
		}

		m_HasDriver = driver && m_IsPhysicsHost;

		dBodyEnableGravity(this, false);

		if (m_Expansion_TowConnectionMask == 0 && !dBodyIsActive(this))
		{
			if (GetGame().IsServer())
			{
				OnNoSimulation(dt);

				SetSynchDirty();
			}

			return;
		}
		
		dBodyActive(this, ActiveState.ACTIVE);

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

		m_State.ApplySimulation(dt);

		OnPostSimulation(dt);

		if (GetGame().IsMultiplayer())
		{
			SetSynchDirty();
		}
	}

	void SetHasPilot(bool state)
	{
		//! So we are able to detect if pilot got disconnected or got out on own accord
		m_Expansion_HasPilot = state;
	}

	float m_q1[4];
	float m_q2[4];
	float m_q3[4];

	string QuatToString(float q[4])
	{
		return "" + q[0] + ", " + q[1] + ", " + q[2] + ", " + q[3];
	}

	void CopyVecToQuat(vector v, out float q[4])
	{
		q[0] = v[0];
		q[1] = v[1];
		q[2] = v[2];
		q[3] = 0;
	}

	void HandleLerp(float pDt)
	{
		if (!m_RecievedInitialSync)
			return;

		vector t1[4];
		vector t2[4];
		vector t3[4];
		GetTransform(t2);

		m_State.m_TimeSince += pDt;

		vector linearVelocity;
		vector angularVelocity;

		ExpansionPhysics.IntegrateTransform(m_State.m_TargetTransform, m_State.m_SyncLinearVelocity, m_State.m_SyncAngularVelocity, m_State.m_TimeSince, t1);
		ExpansionPhysics.CalculateVelocity(t2, t1, pDt, linearVelocity, angularVelocity);

		//DGBDrawBoundingBox(t2, 0x1f00AA00);
		//DGBDrawBoundingBox(m_State.m_TargetTransform, 0x1fAA0000);
		//DGBDrawBoundingBox(t1, 0x1f0000AA);

		//m_State.m_AngularVelocity = "0 0 0";
		//angularVelocity = "0 0 0";
		//ExpansionPhysics.IntegrateTransform(t2, linearVelocity, angularVelocity, pDt, t3);
		//DGBDrawBoundingBox(t3, 0x1f005555);

		//t3[0] = t1[0];
		//t3[1] = t1[1];
		//t3[2] = t1[2];

		//dBodySetTargetMatrix(this, t3, pDt);

		float strength = Math.Clamp(0.001 * vector.DistanceSq(t2[3], t1[3]), 0, 1.0);

		linearVelocity = vector.Lerp(m_State.m_LinearVelocity, linearVelocity, strength * pDt);

		SetVelocity(this, linearVelocity);
		dBodySetAngularVelocity(this, angularVelocity);
	}

	override bool OnNetworkTransformUpdate(out vector pos, out vector ypr)
	{
		if (m_HasDriver)
		{
			m_RecievedInitialSync = true;

			Math3D.YawPitchRollMatrix(ypr * Math.RAD2DEG, m_State.m_TargetTransform);
			m_State.m_TargetTransform[3] = pos;

			pos = GetPosition();
			ypr = GetOrientation() * Math.DEG2RAD;

			return true;
		}

		return false;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		int crewIdx;

		switch (rpc_type)
		{
		case ExpansionVehicleRPC.ControllerSync:
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;	
				
			if (IsCrew(DayZPlayerConstants.VEHICLESEAT_DRIVER, sender))
			{
				m_Event_NetworkRecieve.NetworkRecieve(ctx);
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
		case ExpansionVehicleRPC.ClientSync:
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;	

			if (GetGame().IsClient() || m_NetworkMode != ExpansionVehicleNetworkMode.CLIENT)
				return;

			if (IsCrew(DayZPlayerConstants.VEHICLESEAT_DRIVER, sender))
			{
				m_RecievedInitialSync = true;
				m_State.OnRPC(ctx);
			}

			return;
		}
		case ExpansionVehicleRPC.CrewSync:
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;	
				
			ctx.Read(crewIdx);

			m_Crew[crewIdx].OnRead(ctx);

			return;
		}
		case ExpansionVehicleRPC.CrewSyncInit:
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;	

			for (crewIdx = 0; crewIdx < m_Crew.Count(); crewIdx++)
				m_Crew[crewIdx].OnRead(ctx);

			return;
		}
		case ExpansionVehicleRPC.RequestCrewSync:
		{
			if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
				return;	

			auto rpc = ExpansionScriptRPC.Create();
			for (crewIdx = 0; crewIdx < m_Crew.Count(); crewIdx++)
				m_Crew[crewIdx].OnSend(rpc);

			rpc.Send(this, ExpansionVehicleRPC.CrewSyncInit, true, sender);

			return;
		}
		}
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
				rpc.Send(NULL, ExpansionVehicleRPC.ControllerSync, true, human.GetIdentity());
		}
		else
		{
			rpc.Send(NULL, ExpansionVehicleRPC.ControllerSync, true, NULL);
		}
	}

	/**
	 * Called only server side
	 */
	override void OnEnterZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE)
			return;

		m_SafeZone = true;

		if (GetExpansionSettings().GetSafeZone().DisableVehicleDamageInSafeZone)
			SetAllowDamage(false);
	}

	/**
	 * Called only server side
	 */
	override void OnExitZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE)
			return;

		m_SafeZone = false;

		if (CanBeDamaged())
			SetAllowDamage(true);
	}

	override void EEInit()
	{
		super.EEInit();

		if (IsMissionHost())
			m_Expansion_SafeZoneInstance.Update();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (m_Controller.m_State[0] != m_EngineSync0)
		{
			m_Controller.m_State[0] = m_EngineSync0;

			if (m_EngineSync0)
			{
				OnEngineStart(0);
			}
			else
			{
				OnEngineStop(0);
			}
		}

		if (m_Controller.m_State[1] != m_EngineSync1)
		{
			m_Controller.m_State[1] = m_EngineSync1;

			if (m_EngineSync1)
			{
				OnEngineStart(1);
			}
			else
			{
				OnEngineStop(1);
			}
		}

		if (m_Controller.m_State[2] != m_EngineSync2)
		{
			m_Controller.m_State[2] = m_EngineSync2;

			if (m_EngineSync2)
			{
				OnEngineStart(2);
			}
			else
			{
				OnEngineStop(2);
			}
		}

		if (m_Controller.m_State[3] != m_EngineSync3)
		{
			m_Controller.m_State[3] = m_EngineSync3;

			if (m_EngineSync3)
			{
				OnEngineStart(3);
			}
			else
			{
				OnEngineStop(3);
			}
		}

		if (GetCrashHeavySound())
			PlayCrashHeavySound();
		else if (GetCrashLightSound())
			PlayCrashLightSound();

		UpdateLights();

		m_State.OnVariablesSynchronized(m_IsPhysicsHost);

		if (m_Expansion_SynchLastDriverUID != m_Expansion_LastDriverUIDSynched)
		{
			m_Expansion_LastDriverUIDSynched = m_Expansion_SynchLastDriverUID;

			if (!m_Expansion_SynchLastDriverUID)
				return;

			//! Reset m_Expansion_LastDriverUID client-side if vehicle has driver and it is not the player
			Human driver = CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
			Man player = GetGame().GetPlayer();
			if (driver && player && driver != player)
			{
				m_Expansion_LastDriverUID = "";
			}
		}
	}

	void ExpansionSetLastDriverUID(PlayerBase player)
	{
		m_Expansion_LastDriverUID = player.GetIdentityUID();

		if (!IsMissionHost())
			return;

		m_Expansion_SynchLastDriverUID = true;

		SetSynchDirty();
	}

	void ExpansionResetLastDriverUIDSynch()
	{
		m_Expansion_SynchLastDriverUID = false;

		SetSynchDirty();
	}

	string ExpansionGetLastDriverUID()
	{
		return m_Expansion_LastDriverUID;
	}

	ExpansionVehicleAxle GetAxle(int axle)
	{
		return m_Axles[axle];
	}

	float GetAxleCount()
	{
		return m_Axles.Count();
	}

	float GetNumWheels()
	{
		return m_Wheels.Count();
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
			engine.m_EngineIndex = m_Engines.Count();
			RegisterNetSyncVariableBool("m_EngineSync" + engine.m_EngineIndex);
			if (engine.m_EngineIndex >= 4)
			{
				Error(GetType() + ": " + engine.m_EngineIndex + " engines added were added, max is 4.");
			}

			m_Engines.Insert(engine);
			return;
		}

		if (module.IsInherited(ExpansionVehicleGearbox))
		{
			auto gearbox = ExpansionVehicleGearbox.Cast(module);
			gearbox.m_GearIndex = m_Gearboxes.Count();
			if (gearbox.m_GearIndex >= 4)
			{
				Error(GetType() + ": " + gearbox.m_GearIndex + " gearboxes added were added, max is 4.");
			}

			m_Gearboxes.Insert(gearbox);
			return;
		}
	}

	void CalculateAltitudeLimiter()
	{
		float altitude = GetPosition()[1];

		if (altitude > m_AltitudeFullForce)
		{
			if (altitude > m_AltitudeNoForce)
			{
				m_AltitudeLimiter = 0;
			}
			else
			{
				m_AltitudeLimiter = 1 - (altitude - m_AltitudeFullForce) * (1 / (m_AltitudeNoForce - m_AltitudeFullForce));
			}
		}
		else
		{
			m_AltitudeLimiter = 1;
		}
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		if (GetGame().IsServer())
		{
			if (slot_name == "Reflector_1_1")
				SetHealth("Reflector_1_1", "Health", item.GetHealth());

			if (slot_name == "Reflector_2_1")
				SetHealth("Reflector_2_1", "Health", item.GetHealth());

			if (slot_name == "CarBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "TruckBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "SparkPlug")
				m_PlugHealth = item.GetHealth01();

			if (slot_name == "GlowPlug")
				m_PlugHealth = item.GetHealth01();

			Synchronize();
		}
		UpdateHeadlightState();
		UpdateLights();

		ExpansionWheelBase wheel;
		if (!Class.CastTo(wheel, item))
			return;

		string slot = slot_name;
		slot.ToLower();

		for (int i = 0; i < m_Wheels.Count(); i++)
		{
			string inv_slot = m_Wheels[i].m_InventorySlot;
			inv_slot.ToLower();

			if (inv_slot == slot)
			{
				m_Wheels[i].OnWheelAttach(wheel);
				return;
			}
		}
	}

	// Updates state of attached headlight bulbs for faster access
	void UpdateHeadlightState()
	{
		EntityAI bulb_L = FindAttachmentBySlotName("Reflector_1_1");
		EntityAI bulb_R = FindAttachmentBySlotName("Reflector_2_1");

		if (bulb_L && !bulb_L.IsRuined() && bulb_R && !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.BOTH;
		}
		else if (bulb_L && !bulb_L.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.LEFT;
		}
		else if (bulb_R && !bulb_R.IsRuined())
		{
			m_HeadlightsState = CarHeadlightBulbsState.RIGHT;
		}
		else if ((!bulb_L || bulb_L.IsRuined()) && (!bulb_R || bulb_R.IsRuined()))
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

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		if (GetGame().IsServer())
		{
			//int slot_id = InventorySlots.GetSlotIdFromString(slot_name);
			if (IsScriptedLightsOn())
			{
				if (slot_name == "CarBattery" || slot_name == "TruckBattery")
				{
					ToggleHeadlights();
				}
			}

			if (EngineIsOn())
			{
				//EngineBelt
				if (slot_name == "GlowPlug" || slot_name == "SparkPlug" || slot_name == "CarBattery" || slot_name == "TruckBattery")
					EngineStop();
			}

			if (slot_name == "CarBattery")
				m_BatteryHealth = -1;

			if (slot_name == "TruckBattery")
				m_BatteryHealth = -1;

			if (slot_name == "SparkPlug")
				m_PlugHealth = -1;

			if (slot_name == "GlowPlug")
				m_PlugHealth = -1;

			if (slot_name == "CarRadiator")
			{
				LeakAll(CarFluid.COOLANT);
				SetHealth("Radiator", "Health", 0);
			}

			Synchronize();
		}

		UpdateHeadlightState();
		UpdateLights();

		ExpansionWheelBase wheel;
		if (!Class.CastTo(wheel, item))
			return;

		string slot = slot_name;
		slot.ToLower();

		for (int i = 0; i < m_Wheels.Count(); i++)
		{
			string inv_slot = m_Wheels[i].m_InventorySlot;
			inv_slot.ToLower();

			if (inv_slot == slot)
			{
				m_Wheels[i].OnWheelDetach();
				return;
			}
		}
	}

	ExpansionController GetExpansionController()
	{
		return m_Controller;
	}

	ExpansionController GetController()
	{
		return m_Controller;
	}

	void SetAttachmentHealth(string itemName, float health)
	{
		EntityAI item = FindAttachmentBySlotName(itemName);

		if (item)
			item.SetHealth("", "", health);
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

	//! Returns crew capacity of this vehicle.
	int CrewSize()
	{
		return m_Crew.Count();
	}

	//! Returns crew member index based on action component index.
	//! -1 is returned when no crew position corresponds to given component index.
	int CrewPositionIndex(int componentIdx)
	{
		//Print( "CrewPositionIndex" );
		//Print( componentIdx );
		for (int i = 0; i < m_Crew.Count(); i++)
		{
			//Print( m_Crew[i].GetComponentIndex() );
			if (m_Crew[i].GetComponentIndex() == componentIdx)
			{
				return i;
			}
		}

		return -1;
	}

	int CrewPositionIndexSelection(int componentIdx)
	{
		array<string> selections = new array<string>();
		GetActionComponentNameList(componentIdx, selections);

		for (int i = 0; i < selections.Count(); i++)
		{
			for (int j = 0; j < m_Crew.Count(); j++)
			{
				if (m_Crew[j].GetComponentSelection() == selections[i])
				{
					return j;
				}
			}
		}
		return -1;
	}

	//! Returns crew member index based on player's instance.
	//! -1 is returned when the player is not isnide.
	int CrewMemberIndex(Human player)
	{
		if (player == NULL)
			return -1;

		for (int i = 0; i < m_Crew.Count(); i++)
		{
			if (m_Crew[i].GetPlayer() == player)
			{
				return i;
			}
		}

		return -1;
	}

	//! Returns crew member based on position index.
	//! Null can be returned if no Human is present on the given position.
	Human CrewMember(int posIdx)
	{
		if (posIdx >= m_Crew.Count())
			return NULL;

		if (posIdx < 0)
			return NULL;

		return m_Crew[posIdx].GetPlayer();
	}

	bool IsCrew(int posIdx, PlayerIdentity identity)
	{
		if (posIdx >= m_Crew.Count())
			return false;

		if (posIdx < 0)
			return false;

		if (!m_Crew[posIdx].GetPlayer())
			return false;

		//! @note we cannot check identity instances for equality directly, they are NEVER the same instance.
		//! Use the ID to check whether identity instances point to the same underlying identity.
		return m_Crew[posIdx].GetPlayer().GetIdentity().GetId() == identity.GetId();
	}

	//! Performs transfer of player from world into vehicle on given position.
	void CrewGetIn(Human player, int posIdx)
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].SetPlayer(DayZPlayerImplement.Cast(player));

		if (GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			auto rpc = ExpansionScriptRPC.Create();
			m_Crew[posIdx].OnSend(rpc);
			rpc.Send(this, ExpansionVehicleRPC.CrewSync, true, null);
		}
	}

	//! Performs transfer of player from vehicle into world from given position.
	Human CrewGetOut(int posIdx)
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return null;

		Human human = m_Crew[posIdx].GetPlayer();

		m_Crew[posIdx].SetPlayer(null);

		if (GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			auto rpc = ExpansionScriptRPC.Create();
			m_Crew[posIdx].OnSend(rpc);
			rpc.Send(this, ExpansionVehicleRPC.CrewSync, true, null);
		}
		else
		{
			if (human == GetGame().GetPlayer())
			{
			}
		}

		return human;
	}

	void CrewDeath(int posIdx)
	{
	}

	void CrewEntry(int posIdx, out vector pos, out vector dir)
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetIn(pos, dir);
	}

	void CrewEntryWS(int posIdx, out vector pos, out vector dir)
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetIn(pos, dir);

		pos = ModelToWorld(pos);
		dir = vector.Direction(GetPosition(), ModelToWorld(dir));
	}

	void CrewTransform(int posIdx, out vector mat[4])
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetTransform(mat);
	}

	void CrewTransformWS(int posIdx, out vector mat[4])
	{
		if (posIdx < 0 || posIdx >= m_Crew.Count())
			return;

		m_Crew[posIdx].GetTransform(mat);

		mat[3] = ModelToWorld(mat[3]);
	}

	int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
	}

	int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}

	int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	int GetCrewIndex(string selection)
	{
		return -1;
	}

	bool CanReachSeatFromSeat(int currentSeat, int nextSeat)
	{
		return true;
	}

	bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "CanReachSeatFromDoors").Add(pSeatSelection).Add(pFromPos).Add(pDistance);
#endif

		string conPointName = GetDoorConditionPointFromSelection(pSeatSelection);
		if (conPointName.Length() > 0)
		{
			if (MemoryPointExists(conPointName))
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

	bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return true;
	}

	int GetSeatIndexFromDoor(string pDoorSelection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatIndexFromDoor").Add(pDoorSelection);
#endif

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

	bool IsIgnoredObject(Object o)
	{
		if (!o)
			return false;

		EntityAI e = EntityAI.Cast(o);
		// CanBeSkinned means it is a dead entity which should not block the door
		return ((e && e.IsZombie()) || o.CanBeSkinned() || o.IsBush() || o.IsTree());
	}

	bool IsAreaAtDoorFree( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "IsAreaAtDoorFree").Add(currentSeat).Add(maxAllowedObjHeight ).Add(horizontalExtents ).Add(playerHeight );
#endif

		vector crewPos;
		vector crewDir;
		CrewEntryWS(currentSeat, crewPos, crewDir);
		crewPos[1] = crewPos[1] + maxAllowedObjHeight + playerHeight * 0.5;
		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		excluded.Insert(this);
		excluded.Insert(GetGame().GetPlayer());
		GetGame().IsBoxColliding(crewPos, crewDir, Vector(horizontalExtents, playerHeight, horizontalExtents), excluded, collided);
		vector minmax[2];
		for (int i = 0; i < collided.Count(); i++)
		{
			if (collided[i].GetCollisionBox(minmax))
				return false;
		}
		return true;
	}

	Shape DebugFreeAreaAtDoor(int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7)
	{
		vector crewPos;
		vector crewDir;
		CrewEntryWS(currentSeat, crewPos, crewDir);
		crewPos[1] = crewPos[1] + maxAllowedObjHeight + playerHeight * 0.5;
		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		excluded.Insert(this);
		excluded.Insert(GetGame().GetPlayer());
		GetGame().IsBoxColliding(crewPos, crewDir, Vector(horizontalExtents, playerHeight, horizontalExtents), excluded, collided);
		int color = ARGB(100, 0, 255, 0);
		vector minmax[2];
		for (int i = 0; i < collided.Count(); i++)
		{
			if (collided[i].GetCollisionBox(minmax))
				color = ARGB(100, 255, 0, 0);
		}

		return Debug.DrawCylinder(crewPos, horizontalExtents, playerHeight, color);
	}

	void Synchronize()
	{
	}

	override void SetActions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetActions");
#endif

		AddAction(ExpansionActionFillFuel);
		AddAction(ExpansionActionSwitchLights);

		AddAction(ExpansionActionGetInExpansionVehicle);
		AddAction(ExpansionActionOpenVehicleDoor);
		AddAction(ExpansionActionCloseVehicleDoor);
	}

	vector GetEnginePosWS()
	{
		return ModelToWorld(m_DrownEnginePos);
	}

	vector GetCoolantPtcPosWS()
	{
		return ModelToWorld(m_coolantPtcPos);
	}

	vector GetRefillPointPosWS()
	{
		return ModelToWorld(m_fuelPos);
	}

	vector GetEnginePointPosWS()
	{
		return ModelToWorld(m_enginePos);
	}

	vector GetFrontPointPosWS()
	{
		return ModelToWorld(m_frontPos);
	}

	vector GetBackPointPosWS()
	{
		return ModelToWorld(m_backPos);
	}

	vector Get_1_1PointPosWS()
	{
		return ModelToWorld(m_side_1_1Pos);
	}

	vector Get_1_2PointPosWS()
	{
		return ModelToWorld(m_side_1_2Pos);
	}

	vector Get_2_1PointPosWS()
	{
		return ModelToWorld(m_side_2_1Pos);
	}

	vector Get_2_2PointPosWS()
	{
		return ModelToWorld(m_side_2_2Pos);
	}

	void OnBrakesPressed()
	{
		UpdateLights();
	}

	void OnBrakesReleased()
	{
		UpdateLights();
	}

	void OnUpdate(float dt)
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

		if (GetGame().IsServer())
		{
			if (GetGame().GetWaterDepth(GetEnginePosWS()) > 0)
			{
				m_DrownTime += dt;
				if (m_DrownTime > DROWN_ENGINE_THRESHOLD)
				{
					// *dt to get damage per second
					AddHealth("Engine", "Health", -DROWN_ENGINE_DAMAGE * dt);
				}
				//Debug.DrawSphere(GetCoolantPtcPosWS(), 0.2, COLOR_RED, ShapeFlags.NOZBUFFER | ShapeFlags.NOZWRITE);
			}
			else
			{
				m_DrownTime = 0;
			}
		}

		//FX only on Client and in Single
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			if (IsDamageDestroyed() && GetGame().GetWaterDepth(GetEnginePosWS()) <= 0)
			{
				if (!SEffectManager.IsEffectExist(m_enginePtcFx))
				{
					m_engineFx = new EffEngineSmoke();
					m_enginePtcFx = SEffectManager.PlayOnObject(m_engineFx, this, m_enginePtcPos, Vector(0, 0, 0), true);
					//m_engineFx.SetParticleStateLight();
					m_engineFx.SetParticleStateHeavy();
				}
			}
		}

		// Visualisation of brake lights for all players
		float brake_coef = GetController().GetBrake();

		if (brake_coef > 0)
		{
			if (!m_BrakesArePressed)
			{
				m_BrakesArePressed = true;
				SetSynchDirty();
				OnBrakesPressed();
			}
		}
		else
		{
			if (m_BrakesArePressed)
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
		return GetVelocity(this).Length() * 3.6;
	}

	/*!
		Returns tank capacity for the specified vehicle's fluid.

		\param fluid the specified fluid type
	*/
	float GetFluidCapacity(CarFluid fluid)
	{
		return m_FluidCapacities.Get(fluid);
	}

	/*!
		Returns fraction value (in range <0, 1>)
		of the current state of the specified vehicle's fluid.

		\param[in] fluid the specified fluid type
	*/
	float GetFluidFraction(CarFluid fluid)
	{
		float amount;
		switch (fluid)
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

		return amount / GetFluidCapacity(fluid);
	}

	//! Removes from the specified fluid the specified amount.
	void Leak(CarFluid fluid, float amount)
	{
		float previousAmt;
		float newAmt;

		switch (fluid)
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

		OnFluidChanged(fluid, previousAmt, newAmt);
	}

	//! Removes all the specified fluid from vehicle.
	void LeakAll(CarFluid fluid)
	{
		float previousAmt;
		float newAmt;

		switch (fluid)
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

		OnFluidChanged(fluid, previousAmt, 0);
	}

	//! Adds to the specified fluid the specified amount.
	void Fill(CarFluid fluid, float amount)
	{
		float previousAmt;
		float newAmt;

		switch (fluid)
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

		OnFluidChanged(fluid, previousAmt, newAmt);
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
	void OnFluidChanged(CarFluid fluid, float newValue, float oldValue)
	{
	}

	//!
	bool IsMoving()
	{
		if (Math.AbsFloat(GetSpeedometer()) > 3.5)
			return true;

		return false;
	}

	string EngineGetName()
	{
		switch (m_CurrentEngine)
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

	int EngineStartAnimation()
	{
		return EngineStartAnimation(m_CurrentEngine);
	}

	int EngineStopAnimation()
	{
		return EngineStopAnimation(m_CurrentEngine);
	}

	int EngineGetCurrent()
	{
		if (m_CurrentEngine < 0)
			m_CurrentEngine = 0;
		if (m_CurrentEngine >= m_Engines.Count())
			m_CurrentEngine = 0;

		return m_CurrentEngine;
	}

	int EngineGetCount()
	{
		return m_Engines.Count();
	}

	void EngineSetNext()
	{
		m_CurrentEngine++;
		if (m_CurrentEngine >= m_Engines.Count())
			m_CurrentEngine = 0;

		SetSynchDirty();
	}

	float EngineGetRPMMax()
	{
		return EngineGetRPMMax(m_CurrentEngine);
	}

	//! Returns engine's maximal working rpm without damaging the engine.
	float EngineGetRPMRedline()
	{
		return EngineGetRPMRedline(m_CurrentEngine);
	}

	//! Returns engine's rpm value.
	float EngineGetRPM()
	{
		return EngineGetRPM(m_CurrentEngine);
	}

	//! Returns true when engine is running, false otherwise.
	bool EngineIsOn()
	{
		return EngineIsOn(m_CurrentEngine);
	}

	//! Starts the engine.
	void EngineStart()
	{
		EngineStart(m_CurrentEngine);
	}

	//! Stops the engine.
	void EngineStop()
	{
		EngineStop(m_CurrentEngine);
	}

	//! Returns engine's max rpm before engine blows up.
	float EngineGetRPMMax(int index)
	{
		return m_Engines[index].m_RPMMax;
	}

	//! Returns engine's maximal working rpm without damaging the engine.
	float EngineGetRPMRedline(int index)
	{
		return m_Engines[index].m_RPMRedline;
	}

	//! Returns engine's rpm value.
	float EngineGetRPM(int index)
	{
		return m_Engines[index].m_RPM;
	}

	//! Returns true when engine is running, false otherwise.
	bool EngineIsOn(int index)
	{
		return m_Controller.m_State[index];
	}

	//! Starts the engine.
	void EngineStart(int index)
	{
		if (!m_Controller.m_State[index] && OnBeforeEngineStart(index))
		{
			m_Controller.m_State[index] = true;

			if (index == 0)
				m_EngineSync0 = true;
			else if (index == 1)
				m_EngineSync1 = true;
			else if (index == 2)
				m_EngineSync2 = true;
			else if (index == 3)
				m_EngineSync3 = true;

			OnEngineStart(index);
		}

		SetSynchDirty();
	}

	/*!
		Is called every time the game wants to start the engine.

		\return true if the engine can start, false otherwise.
	*/
	bool OnBeforeEngineStart(int index)
	{
		// engine can start by default
		return true;
	}

	//! Is called every time the engine starts.
	void OnEngineStart(int index)
	{
		m_EnginesOn++;

		UpdateLights();
	}

	//! Stops the engine.
	void EngineStop(int index)
	{
		if (m_Controller.m_State[index])
		{
			m_Controller.m_State[index] = false;

			if (index == 0)
				m_EngineSync0 = false;
			else if (index == 1)
				m_EngineSync1 = false;
			else if (index == 2)
				m_EngineSync2 = false;
			else if (index == 3)
				m_EngineSync3 = false;

			OnEngineStop(index);
		}

		SetSynchDirty();
	}

	//! Is called every time the engine stops.
	void OnEngineStop(int index)
	{
		m_EnginesOn--;

		UpdateLights();
	}

	int EnginesOn()
	{
		return m_EnginesOn;
	}

	int EngineStartAnimation(int index)
	{
		if (IsPlane())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		if (IsBoat())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;
		if (IsHelicopter())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_TUNE;

		return DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
	}

	int EngineStopAnimation(int index)
	{
		if (IsPlane())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		if (IsBoat())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;
		if (IsHelicopter())
			return DayZPlayerConstants.CMD_ACTIONMOD_ITEM_OFF;

		return DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
	}

	/*!
		Is called every time when the simulation changed gear.

		\param[in] newGear new gear level
		\param[in] oldGear previous gear level before gear shift
	*/
	void OnGearChanged(int newGear, int oldGear)
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
	void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "EOnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif

		if (zoneName == "")
		{
			Print("CarScript >> ERROR >> OnContact dmg zone not defined!");
			return;
		}

		switch (zoneName)
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
			if (GetGame().IsServer() && zoneName != "")
			{
				if (CanBeDamaged())
				{
					float dmgMin = 150.0;
					float dmgThreshold = 750.0;
					float dmgKillCrew = 3000.0;
					float dmg = data.Impulse * m_dmgContactCoef;

					if (dmg < dmgThreshold)
					{
						if (dmg > dmgMin)
						{
							//Print( GetType() + " >>> " + " smlHit >>> " + "zoneName: "  + zoneName + " >>> - " + dmg.ToString() + " HP >>> in " + GetSpeedometer() + " km/h");
							AddHealth(zoneName, "Health", -dmg);
							//m_PlayCrashSoundLight = true;
							SynchCrashLightSound(true);
						}
					}
					else
					{
						//Print( GetType() + " >>> " + " BIGHit >>> " + "zoneName: " + zoneName + " >>> - " + dmg.ToString() + " HP >>> in " + GetSpeedometer() + " km/h" );
						for (int i = 0; i < CrewSize(); i++)
						{
							Human crew = CrewMember(i);
							if (!crew)
								continue;

							PlayerBase player;
							if (Class.CastTo(player, crew))
							{
								if (dmg > dmgKillCrew)
								{
									player.SetHealth(0.0);
								}
								else
								{
									//deal shock to player
									float shockTemp = Math.InverseLerp(dmgThreshold, dmgKillCrew, dmg);
									float shock = Math.Lerp(50, 100, shockTemp);
									float hp = Math.Lerp(2, 60, shockTemp);

									player.AddHealth("", "Shock", -shock);
									player.AddHealth("", "Health", -hp);
									//Print( "SHOCK..........." + shock );
									//Print( "HEALTH..........." + hp );
								}
							}
						}

						//m_PlayCrashSoundHeavy = true;
						SynchCrashHeavySound(true);
						ProcessDirectDamage(DT_CUSTOM, null, zoneName, "EnviroDmg", "0 0 0", dmg);
					}
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
	float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		if (GetGame().IsServer())
		{
			if (GetCrashLightSound())
				m_PlayCrashSoundLight = false;

			if (GetCrashHeavySound())
				m_PlayCrashSoundHeavy = false;
		}

		switch (ctrl)
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "EEHealthLevelChanged").Add(oldLevel).Add(newLevel).Add(zone);
#endif

		super.EEHealthLevelChanged(oldLevel, newLevel, zone);

		if (newLevel == GameConstants.STATE_RUINED)
		{
			EffectSound sound_plug;
			switch (zone)
			{
			case "WindowLR":
			case "WindowRR":
				PlaySoundSet(sound_plug, "offroad_hit_window_small_SoundSet", 0, 0);
				break;

			case "WindowFront":
			case "WindowBack":
			case "WindowFrontLeft":
			case "WindowFrontRight":
				PlaySoundSet(sound_plug, "offroad_hit_window_large_SoundSet", 0, 0);
				break;
			}
		}
	}

	void Expansion_DBGTowing()
	{
		vector transform[4];
		GetTransform(transform);

		for (int i = 0; i < Expansion_NumberTowConnections(); i++)
		{
			vector position;
			vector size;
			Expansion_GetTowConnection(i, position, size);

			vector minMax[2];

			minMax[0] = position - size;
			minMax[1] = position + size;

			DGBDrawBoundingBox(transform, minMax);
		}

		DBGDrawSphere(ModelToWorld(Expansion_GetTowPosition()), Expansion_GetTowLength());
	}

	override int Expansion_NumberTowConnections()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAutoHoverTargetHeight");
#endif

		return 2;
	}

	override void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_GetTowConnection").Add(index);
#endif

		vector minMax[2];
		GetCollisionBox(minMax);

		if (index == 0)
		{
			position = Vector(0.0, -minMax[0][1], minMax[1][2] + Expansion_GetTowLength());
		}

		if (index == 1)
		{
			position = Vector(0.0, -minMax[0][1], minMax[0][2] - Expansion_GetTowLength());
		}

		position[0] = dBodyGetCenterOfMass(this)[0];
		position[2] = position[2] - dBodyGetCenterOfMass(this)[2];

		size = "0.5 0.5 0.5";
	}

	override float Expansion_GetTowLength()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowLength");
#endif

		return 0.4;
	}

	override bool Expansion_CanConnectTow(notnull Object other)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_CanConnectTow").Add(other);
#endif

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

		return false;
	}

	ExpansionVehicleLockState GetLockedState()
	{
		return m_VehicleLockedState;
	}

	void KeyMessage(string message)
	{
		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", message, ""));
	}

	override bool IsLocked()
	{
		if (HasKey())
		{
			return m_VehicleLockedState == ExpansionVehicleLockState.LOCKED;
		}

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

	// ------------------------------------------------------------
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
		if (!key)
			return;

		key.PairToVehicle(this);

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;

		SetSynchDirty();
	}

	/**
	 * Only call this after all keys have been confirmed to be removed
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
			return false;

		if (!key.IsPairedTo(this))
			return false;

		return true;
	}

	void LockCar(ExpansionCarKey key = NULL)
	{
		if (key && !IsCarKeys(key))
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.READY_TO_LOCK;

		SetSynchDirty();
	}

	void UnlockCar(ExpansionCarKey key = NULL)
	{
		if (key && !IsCarKeys(key))
			return;

		m_VehicleLockedState = ExpansionVehicleLockState.UNLOCKED;
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
	}

	void OnCarUnlocked()
	{
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

			OnCarLocked();
			SetSynchDirty();
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

	override bool IsSoundSynchRemote()
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
	 * Explodes the vehicle on the server, telling the client that they should explode by setting the sync boolean.
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

		SetHealth("", "", 0.0);

		SetAttachmentHealth("ExpansionHelicopterBattery", 0.0);
		SetAttachmentHealth("ExpansionAircraftBattery", 0.0);
		SetAttachmentHealth("CarBattery", 0.0);
		SetAttachmentHealth("TruckBattery", 0.0);

		SetAttachmentHealth("GlowPlug", 0.0);
		SetAttachmentHealth("SparkPlug", 0.0);

		SetAttachmentHealth("Reflector_1_1", 0.0);
		SetAttachmentHealth("Reflector_2_1", 0.0);

		SetAttachmentHealth("EngineBelt", 0.0);
		SetAttachmentHealth("CarRadiator", 0.0);

		for (int i = 0; i < CrewSize(); i++)
		{
			Human crew = CrewMember(i);
			if (!crew)
				continue;

			crew.SetHealth(0.0);
		}

		vector force = GetVelocity(this) * m_State.m_Mass;
		force[1] = m_State.m_Mass * 10 + force[1];
		dBodyApplyForce(this, force * 0.1);
	}

	/**
	 * Explodes the vehicle on the client, setting that the client has already exploded
	 */
	void ExpansionOnExplodeClient(int damageType, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "ExpansionOnExplodeClient").Add(damageType).Add(ammoType);
#endif

		StopSounds();
		EffectSound sound = SEffectManager.PlaySound("Expansion_Explosive_Vehicle_SoundSet", GetPosition());
		sound.SetSoundAutodestroy(true);

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
	}

	/**
	 * The entity was already exploded but the client never knew about it (i.e. outside of network bubble)
	 */
	void ExpansionOnSpawnExploded()
	{
		m_Exploded = true;

		// m_SmokeParticle = Particle.PlayOnObject( ParticleList.SMOKING_HELI_WRECK, this, GetMemoryPointPos( "velka osa" ) );
		// m_SmokeParticle.SetOrientation( n );
	}

	/**
	 * Does nothing, vanilla code shall not run
	 */
	override void OnExplodeClient()
	{
	}

	/**
	 * Does nothing, vanilla code shall not run
	 */
	override void SynchExplosion()
	{
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		return m_VehicleLockedState != ExpansionVehicleLockState.LOCKED;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		if (EnginesOn() > 0 && IsMoving())
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
			if (!ehcv || ehcv.GetObject() == this)
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

		if (!m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();

		m_Expansion_GlobalID.OnStoreSave(ctx);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;
		
		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx)
			return true;

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

		if (ctx.GetVersion() < 41)
			return true;

		if (!m_Expansion_GlobalID.OnStoreLoad(ctx))
			return false;
		
		return true;
	}
#endif

	//! Called when entity is being created as new by CE/ Debug
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		float maxVolume = GetFluidCapacity(CarFluid.FUEL);
		float amount = Math.RandomFloat(0.0, maxVolume * 0.35);

		Fill(CarFluid.FUEL, amount);

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
	}

	bool Expansion_CoverVehicle(EntityAI cover = null, out ExpansionEntityStoragePlaceholder placeholder = null)
	{
		//! TODO
		return false;
	}

	string Expansion_GetPlaceholderType(string coverType)
	{
		//! TODO
		return "Expansion_Generic_Vehicle_Cover";
	}

	override bool CanPutAsAttachment(EntityAI parent)
	{
		return false;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanRemoveFromCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	void StopSounds()
	{
	}

	bool IsExploded()
	{
		return m_Exploded;
	}

	string GetAnimSourceFromSelection(string selection)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetAnimSourceFromSelection").Add(selection);
#endif

		return "";
	}

	string GetDoorConditionPointFromSelection(string selection)
	{
		return "";
	}

	string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorSelectionNameFromSeatPos").Add(posIdx);
#endif

		return "";
	}

	string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetDoorInvSlotNameFromSeatPos").Add(posIdx);
#endif

		return "";
	}

	int GetCarDoorsState(string slotType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetCarDoorsState").Add(slotType);
#endif

		return CarDoorState.DOORS_MISSING;
	}

	string GetActionCompNameCoolant()
	{
		return "radiator";
	}

	float GetActionDistanceCoolant()
	{
		return 2.0;
	}

	string GetActionCompNameFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionCompNameFuel");
#endif

		return "refill";
	}

	float GetActionDistanceFuel()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetActionDistanceFuel");
#endif

		return 1.5;
	}

	string GetActionCompNameOil()
	{
		//return "refill";
		return "carradiator";
	}

	float GetActionDistanceOil()
	{
		return 2.0;
	}

	string GetActionCompNameBrakes()
	{
		return "carradiator";
	}

	float GetActionDistanceBrakes()
	{
		return 2.0;
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

	ItemBase GetBattery()
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

	bool HasRoof()
	{
		return true;
	}

	bool IsVitalAircraftBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalAircraftBattery");
#endif

		return false;
	}

	bool IsVitalHelicopterBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
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

	bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
	}

	bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return true;
	}

	bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return false;
	}

	bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return false;
	}

	bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return false;
	}

	bool IsScriptedLightsOn()
	{
		return m_HeadlightsOn;
	}

	void ToggleHeadlights()
	{
		m_HeadlightsOn = !m_HeadlightsOn;
		SetSynchDirty();

		UpdateLights();
	}

	bool IsReversing(int new_gear = -1)
	{
		return false;
	}

	void UpdateLights(int new_gear = -1) // -1 is invalid gear.
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer()) // client side
		{
			ItemBase battery;

			if (IsVitalCarBattery())
				battery = ItemBase.Cast(FindAttachmentBySlotName("CarBattery"));

			if (battery)
			{
				// HEADLIGHTS

				if (m_HeadlightsOn)
				{
					DashboardShineOn();

					if (!m_Headlight && m_HeadlightsState != CarHeadlightBulbsState.NONE)
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
						m_Headlight.AttachOnMemoryPoint(this, CarScript.m_RightHeadlightPoint, CarScript.m_RightHeadlightTargetPoint);
						m_Headlight.SegregateLight();
						RightFrontLightShineOn();
						LeftFrontLightShineOff();
					}
					else if (m_HeadlightsState == CarHeadlightBulbsState.BOTH)
					{
						vector local_pos_left = GetMemoryPointPos(CarScript.m_LeftHeadlightPoint);
						vector local_pos_right = GetMemoryPointPos(CarScript.m_RightHeadlightPoint);

						vector local_pos_middle = (local_pos_left + local_pos_right) * 0.5;
						m_Headlight.AttachOnObject(this, local_pos_middle);
						m_Headlight.AggregateLight();
						LeftFrontLightShineOn();
						RightFrontLightShineOn();
					}

					if (m_Headlight && m_HeadlightsState == CarHeadlightBulbsState.NONE)
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

				if (EnginesOn() > 0)
				{
					int reverse_light_state = CarRearLightType.NONE;

					// reverse

					if (IsReversing(new_gear))
					{
						reverse_light_state = CarRearLightType.REVERSE_ONLY;
					}

					// brakes

					if (m_BrakesArePressed)
					{
						if (reverse_light_state == CarRearLightType.REVERSE_ONLY)
						{
							reverse_light_state = CarRearLightType.BRAKES_AND_REVERSE;
						}
						else
						{
							reverse_light_state = CarRearLightType.BRAKES_ONLY;
						}
					}

					if (reverse_light_state != CarRearLightType.NONE && m_HeadlightsState != CarHeadlightBulbsState.NONE && !m_RearLight)
					{
						m_RearLight = CarRearLightBase.Cast(CreateRearLight());
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

		if (material != "" && material_off != "")
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

	CarLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif

		return NULL;
	}

	CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return NULL;
	}

	void CheckVitalItem(bool isVital, string itemName)
	{
		if (!isVital)
			return;

		EntityAI item = FindAttachmentBySlotName(itemName);

		if (!item)
			EngineStop();
		else if (item.IsRuined())
			EngineStop();
	}

	void LeakFluid(CarFluid fluid)
	{
		float ammount = 0;

		switch (fluid)
		{
		case CarFluid.COOLANT:
			ammount = (1 - m_RadiatorHealth) * Math.RandomFloat(0.02, 0.05); //CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
			Leak(fluid, ammount);
			break;

		case CarFluid.OIL:
			ammount = (1 - m_EngineHealth) * Math.RandomFloat(0.02, 1.0); //CARS_LEAK_OIL_MIN; CARS_LEAK_OIL_MAX
			Leak(fluid, ammount);
			break;

		case CarFluid.FUEL:
			ammount = (1 - m_FuelTankHealth) * Math.RandomFloat(0.02, 0.05); //CARS_LEAK_TICK_MIN; CARS_LEAK_TICK_MAX
			Leak(fluid, ammount);
			break;
		}
	}

	void CarPartsHealthCheck()
	{
		if (GetGame().IsServer())
		{
			ItemBase radiator;
			Class.CastTo(radiator, FindAttachmentBySlotName("CarRadiator"));
			if (radiator)
			{
				m_RadiatorHealth = radiator.GetHealth01("", "");
			}
			else
			{
				m_RadiatorHealth = 0;
			}

			m_FuelTankHealth = GetHealth01("FuelTank", "");
		}
	}

	bool GetCrashLightSound()
	{
		return m_PlayCrashSoundLight;
	}

	void SynchCrashLightSound(bool play)
	{
		m_PlayCrashSoundLight = play;
		SetSynchDirty();
	}

	void PlayCrashLightSound()
	{
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			EffectSound sound = SEffectManager.PlaySound("offroad_hit_light_SoundSet", GetPosition());
			sound.SetSoundAutodestroy(true);
			m_PlayCrashSoundLight = false;
		}
	}

	bool GetCrashHeavySound()
	{
		return m_PlayCrashSoundHeavy;
	}

	void SynchCrashHeavySound(bool play)
	{
		m_PlayCrashSoundHeavy = play;
		SetSynchDirty();
	}

	void PlayCrashHeavySound()
	{
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			EffectSound sound = SEffectManager.PlaySound("offroad_hit_heavy_SoundSet", GetPosition());
			sound.SetSoundAutodestroy(true);
			m_PlayCrashSoundHeavy = false;
		}
	}

	bool IsPlane()
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

	bool IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return false;
	}

	bool CanObjectAttach(Object obj)
	{
		return false;
	}

	override bool Expansion_IsPlane()
	{
		return IsPlane();
	}

	override bool Expansion_IsBoat()
	{
		return IsBoat();
	}

	override bool Expansion_IsHelicopter()
	{
		return IsHelicopter();
	}

	override bool Expansion_IsCar()
	{
		return IsCar();
	}

	bool LeavingSeatDoesAttachment(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "LeavingSeatDoesAttachment").Add(posIdx);
#endif

		// only called if CanObjectAttach returns true.

		return true;
	}

	override bool CanBeDamaged()
	{
		if (GetExpansionSettings().GetVehicle().DisableVehicleDamage)
		{
			return false;
		}

		if (GetExpansionSettings().GetSafeZone().Enabled && IsInSafeZone())
		{
			return !GetExpansionSettings().GetSafeZone().DisableVehicleDamageInSafeZone;
		}

		return true;
	}

	float GetModelZeroPointDistanceFromGround()
	{
		if (m_ModelZeroPointDistanceFromGround < 0)
		{
			//string path = "CfgVehicles " + GetType() + " modelZeroPointDistanceFromGround";
			//if (GetGame().ConfigIsExisting(path))
			//{
				//m_ModelZeroPointDistanceFromGround = GetGame().ConfigGetFloat(path);
			//}
			//else
			//{
				vector minMax[2];
				GetCollisionBox(minMax);
				float diff = -minMax[0][1];
				if (diff > 0)
					m_ModelZeroPointDistanceFromGround = diff;
				else
					m_ModelZeroPointDistanceFromGround = 0;
			//}
		}

		return m_ModelZeroPointDistanceFromGround;
	}

	float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 6.0;
	}

	vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 3.2 0";
	}

	bool IsFreeLook()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsFreeLook");
#endif

		return false;
	}

	override bool IsTransport()
	{
		return true;
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
		if (GetGame().ConfigIsExisting("CfgVehicles " + GetType() + " wreck"))
			return GetGame().ConfigGetTextOut("CfgVehicles " + GetType() + " wreck");
		return GetType() + "Wreck";
	}

	string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUniversalWheel"; //this should never happen
	}

	bool IsInSafeZone()
	{
		return m_SafeZone;
	}
	
	set<Human> Expansion_GetVehicleCrew(bool playersOnly = true)
	{
		set<Human> players = new set<Human>;
		Human crew;
		
		//! Seated players
		for (int i = 0; i < CrewSize(); i++)
		{
			crew = CrewMember(i);
			if (!crew)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}

		//! Attached players
		IEntity child = GetChildren();
		while (child)
		{
			crew = Human.Cast(child);

			child = child.GetSibling();

			if (!crew)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}
		
		return players;
	}
};
