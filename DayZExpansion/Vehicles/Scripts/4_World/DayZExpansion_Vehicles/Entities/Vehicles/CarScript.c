/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
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
	static float EXPANSION_COLLISION_DMG_MIN_SPEED_TO_DMG_MULT = 97.2;

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
	float m_Expansion_AnimationTickTime;

	int m_Expansion_EnginesOn;

	// ------------------------------------------------------------ //
	// Constant Values - Set in Constructor, Errors occur if not.   //
	// ------------------------------------------------------------ //
	protected float m_MaxSpeed; // (km/h)
	protected float m_MaxSpeedMS; // (m/s)

	protected float m_AltitudeFullForce; // (m)
	protected float m_AltitudeNoForce; // (m)

	protected float m_Expansion_Mass;
	protected float m_Expansion_ApproxMass;

	// ------------------------------------------------------------ //
	// Member values                                                //
	// ------------------------------------------------------------ //
	private int m_ExpansionSaveVersion;

	protected int m_Expansion_MinimumEngine;
	protected int m_Expansion_MaximumEngine;
	protected int m_Expansion_CurrentEngine;

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

	protected bool m_Expansion_EngineSync1;
	protected bool m_Expansion_EngineSync2;
	protected bool m_Expansion_EngineSync3;

	protected bool m_Expansion_WasMissionLoadedAtVehicleInstantiation;

	bool m_Expansion_CollisionDamageIfEngineOff;
	float m_Expansion_CollisionDamageMinSpeed; 

	ref map<string, int> m_Expansion_WheelsToAdd = new map<string, int>();
	int m_Expansion_AttachedWheelsCount;

	ref TIntArray m_Expansion_NonGUIAttachmentSlotIds = {};

	protected ref EffVehicleSmoke m_Expansion_ExhaustFx[3];
	protected vector m_Expansion_ExhaustPtcPos[3];
	protected vector m_Expansion_ExhaustPtcDir[3];
	protected int m_Expansion_ExhaustPtcFx[3];

	#ifndef SERVER
	protected ref ExpansionEngineStartSounds m_Expansion_EngineStartSounds = new ExpansionEngineStartSounds(this);
	#endif

	static int s_Expansion_ControllerSync_RPCID;
	static int s_Expansion_ClientPing_RPCID;

	EntityAI m_Expansion_HydraulicHoses;
	float m_Expansion_HydraulicHosesHealth = -1.0;

	void CarScript()
	{
	#ifndef SERVER
		SetEventMask(EntityEvent.POSTSIMULATE | EntityEvent.FRAME);
	#endif

		g_Expansion_Car = this;

		RegisterNetSyncVariableInt("m_Expansion_CurrentEngine");

	#ifdef DAYZ_1_25
		RegisterNetSyncVariableBool("m_Expansion_AcceptingAttachment");
	#endif

#ifndef EXPANSION_VEHICLE_DESYNC_PROTECTION_DISABLE
		m_State.RegisterSync_CarScript("m_State");
#endif

		m_DebugShapes = new array<Shape>();

		m_Lights = new array<ExpansionPointLight>;
		m_Particles = new array<Particle>;

		int i;
		int count;
		string path;

		LoadConstantVariables();

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(LongDeferredInit, 1000);

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
		count = g_Game.ConfigGetChildrenCount(path);

		AddModule(new ExpansionVehicleEngine_CarScript(this, "CfgVehicles " + GetType() + " SimulationModule Engine"));
		AddModule(new ExpansionVehicleGearbox_CarScript(this, "CfgVehicles " + GetType() + " SimulationModule Gearbox"));

		for (i = 0; i < count; i++)
		{
			string engineName;
			g_Game.ConfigGetChildName(path, i, engineName);

			string enginePath = path + " " + engineName;
			AddModule(Expansion_CreateEngine(this, enginePath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Gearboxes";
		count = g_Game.ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string gearName;
			g_Game.ConfigGetChildName(path, i, gearName);

			string gearPath = path + " " + gearName;
			AddModule(Expansion_CreateGearbox(this, gearPath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Props";
		count = g_Game.ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string propName;
			g_Game.ConfigGetChildName(path, i, propName);

			string propPath = path + " " + propName;
			AddModule(new ExpansionVehicleProp(this, propPath));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Buoyancy";
		count = g_Game.ConfigGetChildrenCount(path);

		for (i = 0; i < count; i++)
		{
			string buoyancyName;
			g_Game.ConfigGetChildName(path, i, buoyancyName);

			string buoyancyPath = path + " " + buoyancyName;
			AddModule(new ExpansionVehicleBuoyantPoint(this, buoyancyPath));
		}

		//! This exists so it can be overridden (e.g.) by server owners who don't have access to unbinarized models
		path = "CfgVehicles " + GetType() + " mass";
		if (g_Game.ConfigIsExisting(path))
			m_Expansion_Mass = g_Game.ConfigGetFloat(path);
		else
			m_Expansion_Mass = dBodyGetMass(this);

	#ifdef DIAG_DEVELOPER
		PrintFormat("%1 mass %2", this, m_Expansion_Mass);
	#endif

		//! Approx. mass used for tree collisions
		if (Expansion_IsHelicopter())
		{
			//! Ok to just use actual mass
			m_Expansion_ApproxMass = m_Expansion_Mass;
		}
		else
		{
			//! 3rd party modded vehicle physical mass is all over the place, so calculate approx. mass based on ext volume and avg bulk density
			vector minMax[2];
			GetCollisionBox(minMax);
			float w = minMax[1][0] - minMax[0][0];
			float h = minMax[1][1] - minMax[0][1];
			float d = minMax[1][2] - minMax[0][2];
			float volume = w * h * d;
			//! Based on real-world counterparts dimensions and mass:
			//! Avg bulk density (mass divided by external volume) for Ada, Olga, Sarka and Gunther is around 108 kg/m3
			//! Avg bulk density for typical motorcycles is around 100 kg/m3
			float density = 100.0;
			m_Expansion_ApproxMass = volume * density;

		#ifdef DIAG_DEVELOPER
			PrintFormat("%1 dim. %2 x %3 x %4 volume %5 approx. mass %6", this, w, h, d, volume, m_Expansion_ApproxMass);
		#endif
		}

		path = "CfgVehicles " + GetType() + " hornLongSoundSet";
		if (g_Game.ConfigIsExisting(path))
			m_CarHornLongSoundName = g_Game.ConfigGetTextOut(path);

		path = "CfgVehicles " + GetType() + " hornShortSoundSet";
		if (g_Game.ConfigIsExisting(path))
			m_CarHornShortSoundName = g_Game.ConfigGetTextOut(path);

		TStringArray attachmentSlotNames = {};
		ConfigGetTextArray("attachments", attachmentSlotNames);

		if (g_Game.IsServer())
		{
			auto settings = GetExpansionSettings().GetVehicle();

			m_Expansion_CollisionDamageIfEngineOff = settings.CollisionDamageIfEngineOff;
			m_Expansion_CollisionDamageMinSpeed = settings.CollisionDamageMinSpeedKmh / 3.6;  //! Converted to m/s

			TStringArray wheels = new TStringArray();
		#ifdef DAYZ_1_19
			ConfigGetTextArray("wheels", wheels);
		#endif

			if ((IsHelicopter() || IsBoat()) && wheels.Count() < 4)
			{
				//! Make sure we have at least four wheels
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

		path = CFG_VEHICLESPATH + " " + GetType() + " GUIInventoryAttachmentsProps";
		if (g_Game.ConfigIsExisting(path))
		{
			count = g_Game.ConfigGetChildrenCount(path);

			if (count > 0)
			{
				TStringArray guiAtts = {};

				for (i = 0; i < count; ++i)
				{
					string attCatName;
					g_Game.ConfigGetChildName(path, i, attCatName);

					string attSlotsPath = path + " " + attCatName + " attachmentSlots";
					if (g_Game.ConfigIsExisting(attSlotsPath))
					{
						TStringArray attSlots = {};
						g_Game.ConfigGetTextArray(attSlotsPath, attSlots);

						guiAtts.InsertAll(attSlots);
					}
				}

				foreach (string attSlotName: attachmentSlotNames)
				{
					if (!ExpansionStatic.StringArrayContainsIgnoreCase(guiAtts, attSlotName))
						m_Expansion_NonGUIAttachmentSlotIds.Insert(InventorySlots.GetSlotIdFromString(attSlotName));
				}
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

		if (!s_Expansion_ControllerSync_RPCID)
			s_Expansion_ControllerSync_RPCID = m_Expansion_RPCManager.RegisterBoth("RPC_Expansion_ControllerSync");
		if (!s_Expansion_ClientPing_RPCID)
			s_Expansion_ClientPing_RPCID = m_Expansion_RPCManager.RegisterServer("RPC_Expansion_ClientPing");
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

				g_Game.ObjectDelete(m_Lights[i]);
			}

			for (i = 0; i < m_Particles.Count(); i++)
			{
				if (!m_Particles[i])
					continue;

				m_Particles[i].Stop();

				g_Game.ObjectDelete(m_Particles[i]);
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
#endif 
		
		super.EEInit();

		if (g_Game.IsServer() && (IsHelicopter() || IsBoat()))
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_AddWheels);
	}

	override bool NameOverride(out string output)
	{
		//! @note normally m_ExpansionVehicle cannot be NULL since it's set on CarScript instantiation,
		//! but EntityAI initializes damagezones before we can set it and uses GetDisplayName if there are no damagezones configured
		if (m_ExpansionVehicle)
			return m_ExpansionVehicle.NameOverride(output);

		return false;
	}

	void Expansion_AssignOwner(notnull Man player, bool send = true)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().AssignOwner");
		m_ExpansionVehicle.AssignOwner(player, send);
	}

	void Expansion_AssignOwner(notnull PlayerIdentity owner, bool send = true)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().AssignOwner");
		m_ExpansionVehicle.AssignOwner(owner, send);
	}

	void Expansion_AssignOwner(string ownerUID, string ownerName, bool send = true)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().AssignOwner");
		m_ExpansionVehicle.AssignOwner(ownerUID, ownerName, send);
	}

	bool Expansion_IsOwner(notnull Man player)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsOwner");
		return m_ExpansionVehicle.IsOwner(player);
	}

	bool Expansion_IsOwner(notnull PlayerIdentity identity)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsOwner");
		return m_ExpansionVehicle.IsOwner(identity);
	}

	bool Expansion_IsOwner(string playerUID)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsOwner");
		return m_ExpansionVehicle.IsOwner(playerUID);
	}

	void Expansion_ResetOwner()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().ResetOwner");
		m_ExpansionVehicle.ResetOwner();
	}

	string Expansion_GetOwnerName()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetOwnerName");
		return m_ExpansionVehicle.GetOwnerName();
	}

	string Expansion_GetOwnerUID()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetOwnerUID");
		return m_ExpansionVehicle.GetOwnerUID();
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

		if (g_Game.IsServer())
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

		#ifndef SERVER
			if (m_Modules[i].m_Animate)
				m_Event_Animate.Add(m_Modules[i]);
		#endif

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

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnAfterLoadConstantVariables, 100, false);

		if (g_Game.IsServer())
		{
			if (dBodyIsActive(this) && m_Expansion_IsStoreLoaded && !m_Expansion_WasMissionLoadedAtVehicleInstantiation)
			{
				//! Setting state to inactive fixes issues with vehicles being simulated at server start (jumpy helis, boats being always active when in water, not needed for cars)
				if (IsInherited(ExpansionHelicopterScript) || (IsInherited(ExpansionBoatScript) && ExpansionStatic.SurfaceIsWater(GetPosition())))
				{
					EXTrace.Print(EXTrace.VEHICLES, this, "DeferredInit - isStoreLoaded - missionLoaded - setting ActiveState.INACTIVE");
					dBodyActive(this, ActiveState.INACTIVE);
				}
			}

			SetSynchDirty();
		}

		if (GetExpansionSettings().GetVehicle(false).IsLoaded())
			OnSettingsUpdated();
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
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetLockComplexity");
		return m_ExpansionVehicle.GetLockComplexity();
	}

	protected void KeyMessage(string message)
	{
		EXError.Error(this, "DEPRECATED");
	}

	bool IsLocked()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsLocked");
		return m_ExpansionVehicle.IsLocked();
	}

	bool Expansion_IsReadyToLock()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsReadyToLock");
		return m_ExpansionVehicle.IsReadyToLock();
	}

	int GetPersistentIDA()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetMasterKeyPersistentID");
		return 0;
	}

	int GetPersistentIDB()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetMasterKeyPersistentID");
		return 0;
	}

	int GetPersistentIDC()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetMasterKeyPersistentID");
		return 0;
	}

	int GetPersistentIDD()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetMasterKeyPersistentID");
		return 0;
	}

	//! ID of the paired master key
	string GetPersistentIDString()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetMasterKeyPersistentIDString");
		return m_ExpansionVehicle.GetMasterKeyPersistentIDString();
	}

	//! ID of the vehicle itself
	string GetVehiclePersistentIDString()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetPersistentIDString");
		return m_ExpansionVehicle.GetPersistentIDString();
	}

	void SetPersistentIDA(int newIDA)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().SetMasterKeyPersistentID");
	}

	void SetPersistentIDB(int newIDB)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().SetMasterKeyPersistentID");
	}

	void SetPersistentIDC(int newIDC)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().SetMasterKeyPersistentID");
	}

	void SetPersistentIDD(int newIDD)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().SetMasterKeyPersistentID");
	}

	bool HasKey()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().HasKey");
		return m_ExpansionVehicle.HasKey();
	}

	void PairKeyTo(ExpansionCarKey key)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().PairKey");
		m_ExpansionVehicle.PairKey(key);
	}

	/**
	 * @brief Only call this after all keys have been confirmed to be removed
	 */
	void ResetKeyPairing()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().ResetKeyPairing");
		m_ExpansionVehicle.ResetKeyPairing();
	}

	bool CanBeLocked()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().CanBeLocked");
		return m_ExpansionVehicle.CanBeLocked();
	}

	void OnCarDoorOpened(string source)
	{
		EXError.Error(this, "DEPRECATED, use Expansion_OnDoorOpened");
		m_ExpansionVehicle.OnDoorOpened(source);
	}

	void OnCarDoorClosed(string source)
	{
		EXError.Error(this, "DEPRECATED, use Expansion_OnDoorClosed");
		m_ExpansionVehicle.OnDoorClosed(source);
	}

	void Expansion_OnDoorOpened(string selection)
	{
	}

	void Expansion_OnDoorClosed(string selection)
	{
	}

	void Expansion_OnGotOut(PlayerBase player, int seatIdx)
	{
	}

	bool IsCarKeys(ExpansionCarKey key)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsPairedTo");
		return m_ExpansionVehicle.IsPairedTo(key);
	}

	void LockCar(ExpansionCarKey key)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().Lock");
		m_ExpansionVehicle.Lock(key);
	}

	void UnlockCar(ExpansionCarKey key)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().Unlock");
		m_ExpansionVehicle.Unlock(key);
	}

	void UnlockCarWithoutKey(ExpansionVehicleLockState lockState = ExpansionVehicleLockState.FORCEDUNLOCKED)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().ForceUnlock");
		m_ExpansionVehicle.ForceUnlock(lockState);
	}

	void OnCarLocked()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().OnLocked");
	}

	void OnCarUnlocked()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().OnUnlocked");
	}

	override void Explode(int damageType, string ammoType = "")
	{
		if (g_Game.IsServer() && !m_Exploded)
		{
			if (ammoType == "")
				ammoType = this.ConfigGetString("ammoType");

			if (ammoType == "")
				ammoType = "Dummy_Heavy";

			if (g_Game.IsServer() && !g_Game.IsMultiplayer())
			{
				ExpansionOnExplodeClient(damageType, ammoType);
			}
			else
			{
				g_Game.RPCSingleParam(this, ERPCs.RPC_EXPLODE_EVENT, new Param2<int, string>(damageType, ammoType), true);
			}

			ExpansionOnExplodeServer(damageType, ammoType);
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

			if (g_Game.IsMultiplayer())
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

				g_Game.ObjectDelete(m_Particles[z]);
			}

			for (z = 0; z < m_Lights.Count(); z++)
			{
				m_Lights[z].ExpansionSetEnabled(false);

				g_Game.ObjectDelete(m_Lights[z]);
			}

			if (g_Game.GetWaterDepth(GetPosition()) < 1)
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

		float distance_to_player = vector.Distance(GetPosition(), g_Game.GetPlayer().GetPosition());

		float strength_factor = Math.InverseLerp(GameConstants.CAMERA_SHAKE_GRENADE_DISTANCE, 0, distance_to_player);

		// g_Game.GetPlayer().GetCurrentCamera().SpawnCameraShake( strength_factor * 16.0 );
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
			//! @note vanilla RPC, so not using Expansion RPC manager
			case ERPCs.RPC_EXPLODE_EVENT:
			{
				if (g_Game.IsClient())
				{
					Param2<int, string> params;
	
					if (ctx.Read(params))
					{
						ExpansionOnExplodeClient(params.param1, params.param2);
					}
				}

				return;
			}
		}

		super.OnRPC(sender, rpc_type, ctx);
	}

	void RPC_Expansion_ControllerSync(PlayerIdentity sender, ParamsReadContext ctx)
	{
	#ifndef DAYZ_1_27
		//! 1.28+
		if (GetNetworkMoveStrategy() == NetworkMoveStrategy.PHYSICS)
		{
			debug;
			return;
		}
	#endif
		
		if (m_Controller)
		{
			PlayerBase driverBase;
			//! @note sender and driverBase.GetIdentity() will NOT be the same object even if they point to the same player identity (same ID)!
			if (Class.CastTo(driverBase, CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER)) && driverBase.GetIdentityUID() == sender.GetId())
			{
				m_Event_NetworkRecieve.NetworkRecieve(ctx);
			}
		}
	}

	void RPC_Expansion_ClientPing(PlayerIdentity sender, ParamsReadContext ctx)
	{
		m_State.OnPing(ctx);
	}

	void NetworkSend()
	{
		if (IsMissionOffline())
			return;
		
	#ifndef DAYZ_1_27
		//! 1.28+
		// sent part of 'PawnMove'
		if (GetNetworkMoveStrategy() == NetworkMoveStrategy.PHYSICS)
			return;
	#endif

		auto rpc = m_Expansion_RPCManager.CreateRPC(s_Expansion_ControllerSync_RPCID);

		m_Event_NetworkSend.NetworkSend(rpc);

		if (IsMissionHost())
		{
			Human human = CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
			if (human != NULL)
				rpc.Expansion_Send(true, human.GetIdentity());
		}
		else
		{
			rpc.Expansion_Send(true);
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
			return ItemBase.Cast(FindAttachmentBySlotName("ExpansionHelicopterBattery"));
		if (IsVitalAircraftBattery())
			return ItemBase.Cast(FindAttachmentBySlotName("ExpansionAircraftBattery"));

		return super.GetBattery();
	}
	
	int Expansion_GetBatteryEnergy()
	{
		ItemBase battery = GetBattery();
		if (battery)
			return Math.Ceil(battery.GetQuantity() / battery.GetQuantityMax() * 100);

		return 0;
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
		return Expansion_IsCar();
	}

	bool IsDuck()
	{
		return Expansion_IsDuck();
	}

	bool IsBoat()
	{
		return Expansion_IsBoat();
	}

	bool IsHelicopter()
	{
		return Expansion_IsHelicopter();
	}

	bool IsPlane()
	{
		return Expansion_IsPlane();
	}

	bool IsExploded()
	{
		return Expansion_IsExploded();
	}

	bool Expansion_IsExploded()
	{
		return m_Exploded;
	}

	bool CanObjectAttach(Object obj)
	{
		return Expansion_CanObjectAttach(obj);
	}

	float Expansion_GetThrottle(int index)
	{
		if (index == 0)
			return GetThrottle();

		return m_Controller.GetThrottle(index);
	}

	override bool Expansion_IsVehicleFunctional(bool checkOptionalParts = false, set<typename> missingComponents = null)
	{
		if (!super.Expansion_IsVehicleFunctional(checkOptionalParts, missingComponents))
			return false;

		EntityAI item;
		
		if (IsVitalHelicopterBattery() || IsVitalAircraftBattery())
		{
			item = GetBattery();
			if (!item || item.IsRuined() || item.GetCompEM().GetEnergy() < m_BatteryEnergyStartMin)
			{
				if (missingComponents)
					missingComponents.Insert(VehicleBattery);
				return false;
			}
		}
	
		if (IsVitalHydraulicHoses())
		{
			item = FindAttachmentBySlotName("ExpansionHydraulicHoses");
			if (!item || item.IsRuined())
			{
				if (missingComponents)
					missingComponents.Insert(ExpansionHydraulicHoses);
				return false;
			}
		}
	
		if (IsVitalIgniterPlug())
		{
			item = FindAttachmentBySlotName("ExpansionIgniterPlug");
			if (!item || item.IsRuined())
			{
				if (missingComponents)
					missingComponents.Insert(SparkPlug);
				return false;
			}
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
		if (g_Game.IsServer())
		{
			if (slot_name == "ExpansionHelicopterBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "ExpansionAircraftBattery")
				m_BatteryHealth = item.GetHealth01();

			if (slot_name == "ExpansionHydraulicHoses")
			{
				m_Expansion_HydraulicHoses = item;
				m_Expansion_HydraulicHosesHealth = item.GetHealth01();
			}

			if (item.IsInherited(CarWheel))
			{
				EXTrace.Print(EXTrace.VEHICLES, this, string.Format("Attached %1 (type=%2)", item.ToString(), item.GetType()));
				m_Expansion_AttachedWheelsCount++;
				string wheelType = item.GetType();
				wheelType.ToLower();
				if (m_Expansion_WheelsToAdd[wheelType])
				{
					int wheelCount = m_Expansion_WheelsToAdd[wheelType];
					if (wheelCount > 0)
						m_Expansion_WheelsToAdd[wheelType] = wheelCount - 1;
				}
			}
		}

		if (item.GetType() == "HatchbackWheel" && !IsCar() && !IsDuck())
		{
			if (!g_Game.IsDedicatedServer())
				item.SetInvisible(true);
		#ifndef DAYZ_1_19
			//! DayZ 1.20+
			if (g_Game.IsServer())
				item.SetAllowDamage(false);
		#endif
		}

		int slotID = InventorySlots.GetSlotIdFromString(slot_name);
		if (!InventorySlots.GetShowForSlotId(slotID) || m_Expansion_NonGUIAttachmentSlotIds.Find(slotID) > -1)
		{
			ItemBase ib;
			if (Class.CastTo(ib, item))
				ib.Expansion_SetCanPhysicsDrop(false);
		}

		super.EEItemAttached(item, slot_name);

		m_ExpansionVehicle.OnItemAttached(item, slot_name);
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

			if (slot_name == "ExpansionHydraulicHoses")
			{
				m_Expansion_HydraulicHoses = null;
				m_Expansion_HydraulicHosesHealth = -1.0;
			}

			if (item.IsInherited(CarWheel))
				m_Expansion_AttachedWheelsCount--;
		}

		super.EEItemDetached(item, slot_name);

		m_ExpansionVehicle.OnItemDetached(item, slot_name);
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
		EXError.Error(this, "DEPRECATED, do not use!");
		return m_ExpansionVehicle.CanUpdateLock(pDt);
	}

	int DoorCount()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().DoorCount");
		return m_ExpansionVehicle.DoorCount();
	}

	bool AllDoorsClosed()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().AllDoorsClosed");
		return m_ExpansionVehicle.AllDoorsClosed();
	}

	void UpdateCarLock(float pDt)
	{
		EXError.Error(this, "DEPRECATED, do not use!");
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

		if (g_Game.IsServer())
		{
			if (HasRadiator())
			{
				m_RadiatorHealth = GetRadiator().GetHealth01("", "");
			}
			else
			{
				m_RadiatorHealth = 0;
			}

			if (m_Expansion_HydraulicHoses)
				m_Expansion_HydraulicHosesHealth = m_Expansion_HydraulicHoses.GetHealth01();
			else
				m_Expansion_HydraulicHosesHealth = 0;

			m_FuelTankHealth = GetHealth01("FuelTank", "");

			if (EngineIsOn() || Expansion_EnginesOn())
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

				if (oil > 0.0 && (m_EngineHealth < 0.25 || (m_Expansion_HydraulicHoses && m_Expansion_HydraulicHosesHealth <= 0.5)))
					LeakFluid(CarFluid.OIL);

				m_ExpansionVehicle.ConsumeFuel(fuelConsumption);

				if (GetFluidFraction(CarFluid.FUEL) <= 0)
					Expansion_EngineStop();
			}
		}

		//FX only on Client and in Single
		if (!g_Game.IsDedicatedServer())
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

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		//! Prevent vanilla fluid checks from running
		m_Time = -1;

		if (CanUpdateHealth(timeSlice))
		{
			UpdateHealth(timeSlice);
		}

		m_ExpansionVehicle.OnPostSimulate(timeSlice);
	}

	override void OnVehicleJumpOutServer(GetOutTransportActionData gotActionData)
	{
		//! Jump out damage should only apply for land vehicles, not helis, planes or boats, and only if player is not in godmode
		//! For air vehicles, damage is dealt separately elsewhere after the fall
		if (gotActionData.m_Player.Expansion_CanBeDamaged("FallDamage"))
		{
			if (Expansion_IsCar() || Expansion_IsBike() || gotActionData.m_Player.m_Expansion_QueuedJumpOutVehicleDamageActionData)
				super.OnVehicleJumpOutServer(gotActionData);
			else if (Expansion_IsHelicopter())
			{
				gotActionData.m_Speed = GetVelocity(this).Length() * 3.6;
				gotActionData.m_Player.m_Expansion_QueuedJumpOutVehicleDamageActionData = gotActionData;
			}
		}
	}

	//! @note only called for driver on client
	override void OnUpdate(float dt)
	{
		if (g_Game.IsServer() && m_DrownTime > 0 && !CanBeDamaged())
			m_DrownTime = 0;

		super.OnUpdate(dt);

		m_ExpansionVehicle.OnUpdate(dt);

	#ifndef SERVER
		OnAnimationUpdate(dt);
	#endif
	}

	void Expansion_AddWheels()
	{
		if (ToDelete())
			return;

		// CreateAttachment will de-reference on a nullptr when spawning in for some ItemPreviewWidget as the PhysicsComponent doesn't exist (ECE_CREATEPHYSICS not used)
	#ifndef DAYZ_1_27
		//! 1.28+
		if (!GetPhysics())
	#else
		if (!dBodyIsSet(this))
	#endif
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

		#ifdef DAYZ_1_28
			if (m_HeadlightsOn)
		#else
			//! 1.29+
			if (LightIsOn())
		#endif
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

		#ifdef DAYZ_1_28
			if (m_HeadlightsOn)
		#else
			//! 1.29+
			if (LightIsOn())
		#endif
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

#ifndef DAYZ_1_27
	//! 1.28+
	override void OnInput(float dt)
	{
		super.OnInput(dt);

		// only if car is using new networking
		if (GetNetworkMoveStrategy() == NetworkMoveStrategy.PHYSICS)
		{
			Human driver = CrewDriver();
			DayZPlayerImplement player;
			if (IsAuthority() && !driver)
			{
				// reset inputs
			}
			else if (IsOwner() && Class.CastTo(player, driver))
			{
				// this looks stupid, what?
				// -- because it is
				m_State.m_DeltaTime = Math.Min(dt, 0.025);  //! Prevent lag resulting in huge input due to high dt
		
				// damn i had bad ideas
				m_Event_Control.Control(m_State, player);
		
				// sort of sensible
				Expansion_OnHandleController(player, dt);
			}
		}
	}
#endif

	void Expansion_HandleController(DayZPlayerImplement driver, float dt)
	{
		if (CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER) != driver)
			return;

		if (!driver.GetInputInterface())
			return;

		m_State.m_DeltaTime = Math.Min(dt, 0.033334);  //! Prevent lag resulting in huge input due to high dt

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
#ifndef SERVER
		OnParticleUpdate(pDt);
#endif
	}

	protected void OnNoSimulation(float pDt)
	{
#ifndef SERVER
		m_State.SetupSimulation(pDt);
		m_Event_PreSimulate.PreSimulate(m_State);
		OnParticleUpdate(pDt);
#endif
	}

	protected void OnSimulation(ExpansionPhysicsState pState)
	{
	}

	protected void OnPostSimulation(float pDt)
	{
	}

	protected void OnParticleUpdate(float pDt)
	{
	}

	protected void OnAnimationUpdate(float pDt)
	{
		//float tickTime = g_Game.GetTickTime();
		//float deltaTimeAccurate = tickTime - m_Expansion_AnimationTickTime;
		//m_Expansion_AnimationTickTime = tickTime;
		m_Event_Animate.Animate(m_State, pDt);
	}

#ifndef SERVER
	//! @note not called for driver on client
	override void EOnFrame(IEntity other, float timeSlice)
	{
		OnAnimationUpdate(timeSlice);
	}
#endif

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

		int i;

#ifndef EXPANSION_DEBUG_SHAPES_DISABLE
		for (i = 0; i < m_DebugShapes.Count(); i++)
			m_DebugShapes[i].Destroy();

		m_DebugShapes.Clear();
#endif

		DayZPlayerImplement driver = DayZPlayerImplement.Cast(CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER));

		bool isActive = dBodyIsActive(this);

		if (g_Game.IsClient())
		{
			m_IsPhysicsHost = IsOwner();
		}
		else if (g_Game.IsServer())
		{
			m_IsPhysicsHost = true;

		#ifdef DAYZ_1_25
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
		#endif

			if (!driver && Expansion_CanSimulate())
			{
				m_State.m_DeltaTime = Math.Min(dt, 0.025);  //! Prevent lag resulting in huge input due to high dt
				m_State.m_IsSync = true;

				m_Event_Control.Control(m_State, null);

				Expansion_OnHandleController(null, dt);

				if (Expansion_ShouldDisableSimulation())
				{
					if (isActive)
						EXTrace.Print(EXTrace.VEHICLES, this, "no driver - CanSimulate - ShouldDisableSimulation - setting ActiveState.INACTIVE");
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
			m_ExpansionVehicle.OnSimulate(dt);
		}

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
		//m_Controller.m_Throttle[0] = GetThrottle();
		m_Controller.m_Brake[0] = GetBrake();
		m_Controller.m_Gear[0] = GetGear();

		TFloatArray gears = new TFloatArray;
		g_Game.ConfigGetFloatArray("CfgVehicles " + GetType() + " SimulationModule Gearbox ratios", gears);

		//! 1.19
		if (GetGear() > 2)
		{
			m_Controller.m_Ratio[0] = gears[GetGear() - 2];
		}
		else if (GetGear() == 0)
		{
			m_Controller.m_Ratio[0] = g_Game.ConfigGetFloat("CfgVehicles " + GetType() + " SimulationModule Gearbox reverse");
		}
		else
		{
			m_Controller.m_Ratio[0] = 0;
		}

		OnPreSimulation(dt);

		//! https://feedback.bistudio.com/T173348
		if (driver && driver.IsAlive() && m_IsPhysicsHost)
			m_State.m_HasDriver = true;
		else
			m_State.m_HasDriver = false;
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
		
	#ifndef DAYZ_1_27
		//! 1.28+
		if (GetNetworkMoveStrategy() == NetworkMoveStrategy.PHYSICS)
		{
			m_State.ApplySimulation(dt);
		}
		else
	#endif
		{
			if (m_IsPhysicsHost && g_Game.IsClient())
			{
				NetworkSend();
			}

#ifndef EXPANSION_VEHICLE_DESYNC_PROTECTION_DISABLE
			m_State.ApplySimulation_CarScript(dt, m_IsPhysicsHost, driver);
#else
			m_State.ApplySimulation(dt);
#endif
		}
	
		OnPostSimulation(dt);

		if (g_Game.IsServer())
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

	[Obsolete("DEPRECATED, no replacement")]
	void SetHasPilot(bool state)
	{
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
			if (engine.m_EngineIndex > 0)
				RegisterNetSyncVariableBool("m_Expansion_EngineSync" + engine.m_EngineIndex);
			if (engine.m_EngineIndex >= 4)
			{
				Error(GetType() + ": " + engine.m_EngineIndex + " engines added were added, max is 4.");
			}

			m_Engines.Insert(engine);

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.VEHICLES, this, "Added " + engine + " index " + engine.m_EngineIndex);
		#endif

			m_Expansion_MaximumEngine = m_Engines.Count();
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

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.VEHICLES, this, "Added " + module);
	#endif
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

		if (m_Expansion_CurrentEngine > 0)
		{
			if (Expansion_IsBoat())
			{
				int gear = GetGear();
				if (gear > CarGear.FIRST)
					ShiftTo(CarGear.FIRST);
			}
		}

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

	float Expansion_EngineGetRPMIdle(int index)
	{
		if (index == 0)
			return EngineGetRPMIdle();

		return m_Engines[index].m_RPMIdle;
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.VEHICLES, this, index.ToString());
#endif 
		
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

		//! Vanilla does this check for us in OnBeforeEngineStart() if engine 0 is selected
		if (Expansion_EngineGetCurrent() > 0 && GetFluidFraction(CarFluid.FUEL) <= 0)
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

		if (!m_ExpansionVehicle.OnBeforeEngineStart(index))
			return false;

		return true;
	}

	void Expansion_HandleEngineSound(CarEngineSoundState state)
	{
		#ifndef SERVER
		switch (state)
		{
			case CarEngineSoundState.STARTING:
				m_Expansion_EngineStartSounds.Play();
				break;
		}
		#endif
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

		m_ExpansionVehicle.OnEngineStart(index);

		SetSynchDirty();

		Expansion_HandleEngineSound(CarEngineSoundState.STARTING);
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

#ifdef DAYZ_1_25
			m_EngineBeforeStart = false;
#endif
		}

		m_ExpansionVehicle.OnEngineStop(index);

		SetSynchDirty();
	}

	//! Get the number of Expansion engines that are on (does NOT include vanilla engine!)
	int Expansion_EnginesOn()
	{
		return m_Expansion_EnginesOn;
	}

	int Expansion_EngineStartAnimation(int index)
	{
		return DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
	}

	int Expansion_EngineStopAnimation(int index)
	{
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

		m_ExpansionVehicle.OnStoreSave(ctx);

		ctx.Write(m_Exploded);

		GetCurrentOrientation();
		ctx.Write(m_Orientation);

		GetCurrentPosition();
		ctx.Write(m_Position);

		ctx.Write(false);
		ctx.Write(false);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		m_ExpansionSaveVersion = ctx.GetVersion();

		if (!m_ExpansionVehicle.OnStoreLoad(ctx))
			return false;

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

		if (ctx.GetVersion() < 51)
			return true;

		if (ctx.GetVersion() < 53 && !m_ExpansionVehicle.OnStoreLoad_Owner(ctx))
			return false;

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

		m_ExpansionVehicle.OnHierarchyAfterLoad();
	}

	override void OnEngineStop()
	{
		super.OnEngineStop();

		//! Something (probably vanilla?) is calling OnEngineStop in a loop on client, EVEN IF ENGINE IS RUNNING WHYYY WTF
		//! Prevent this by checking var that tells us if engine is really stopped or not
		if (g_Game.IsClient() && m_Expansion_EngineIsOn)
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

	override string ExpansionGetCurrentSkinName()
	{
		return m_CurrentSkinName;
	}
	
	override int ExpansionGetCurrentSkinIndex()
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

		bool isGlobalOrEngineRuined;
		if ((!zone || zone == "GlobalHealth" || zone == "Engine") && newLevel == GameConstants.STATE_RUINED)
			isGlobalOrEngineRuined = true;

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
						m_enginePtcFx = SEffectManager.PlayOnObject(m_engineFx, this, fxPos, "0 0 0", false);
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

		if (m_CurrentSkin.HornLong != "")
		{
			m_CarHornLongSoundName = m_CurrentSkin.HornLong;
		}

		if (m_CurrentSkin.HornShort != "")
		{
			m_CarHornShortSoundName = m_CurrentSkin.HornShort;
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

		if (!m_ExpansionVehicle.CanReceiveAttachment(attachment, slotId))
			return false;

		return true;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CanReleaseAttachment").Add(attachment);
#endif

		if (!super.CanReleaseAttachment(attachment))
			return false;

		if (!m_ExpansionVehicle.CanReleaseAttachment(attachment))
			return false;

		return true;
	}

	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;

		if (!m_ExpansionVehicle.IsInventoryVisible())
			return false;

		return true;
	}

	bool HasGear()
	{
		EXError.WarnOnce(this, "DEPRECATED");
		return Expansion_HasGear();
	}

	bool Expansion_HasGear()
	{
		return false;
	}

	void SwitchGear()
	{
		EXError.WarnOnce(this, "DEPRECATED");
		Expansion_SwitchGear();
	}

	void Expansion_SwitchGear()
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

		m_ExpansionVehicle.OnCECreate();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		Expansion_OnCEUpdate();
	}

	void Expansion_OnCEUpdate()
	{
		m_ExpansionVehicle.AutoCover();
	}

	bool Expansion_CoverVehicle(EntityAI cover = null, out ExpansionEntityStoragePlaceholder placeholder = null)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().Cover");
		return m_ExpansionVehicle.Cover(cover, placeholder);
	}

	bool Expansion_CanCover()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().CanCover");
		return m_ExpansionVehicle.CanCover();
	}

	string Expansion_GetPlaceholderType(string coverType)
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetPlaceholderType");
		return m_ExpansionVehicle.GetPlaceholderType(coverType);
	}

	ExpansionPhysicsState Expansion_GetPhysicsState()
	{
		return m_State;
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
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().GetModelZeroPointDistanceFromGround");
		return m_ExpansionVehicle.GetModelZeroPointDistanceFromGround();
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
		if (g_Game.ConfigIsExisting(path))
			return g_Game.ConfigGetTextOut(path);

		string className = GetType() + "Wreck";

		if (g_Game.ConfigIsExisting("CfgVehicles " + className))
			return className;

		g_Game.ConfigGetBaseName("CfgVehicles " + GetType(), className);

		return className + "Wreck";
	}

	string ExpansionGetWheelType(int slot_id)
	{
		return "ExpansionUniversalWheel"; //this should never happen
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
#ifdef EXPANSION_ONCONTACT_TRACING
		auto trace = EXTrace.Start(ExpansionTracing.VEHICLES, this);
		EXTrace.Add(trace, zoneName);
		EXTrace.Add(trace, localPos);
		EXTrace.Add(trace, other);
		EXTrace.Add(trace, data);
#endif

		//! DayZ 1.28 broke this (segfault with certain items like wheels)
		//auto item = ItemBase.Cast(other);
		//if (item && !dBodyIsDynamic(item) && item.Expansion_CarContactActivates() && !item.GetHierarchyParent())
		//{
			//item.ExpansionCreateDynamicFromContact(this, data.Position, data);
		//}

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

		float velocity = data.RelativeVelocityBefore.Length();
		float momentum = m_Expansion_ApproxMass * velocity;

		ExpansionWorld.CheckTreeContact(other, momentum, true);

		if (g_Game.IsServer() && (!m_Expansion_CollisionDamageIfEngineOff || m_Expansion_CollisionDamageMinSpeed))
		{
			CarScript otherVehicle;
			bool otherVehicleEngineOn;
			if (Class.CastTo(otherVehicle, other) && otherVehicle.Expansion_EngineIsSpinning())
				otherVehicleEngineOn = true;

			if (!m_Expansion_CollisionDamageIfEngineOff)
			{
				if (!Expansion_EngineIsSpinning() && !otherVehicleEngineOn)
					return;
			}

			if (m_Expansion_CollisionDamageMinSpeed)
			{
				if (velocity < m_Expansion_CollisionDamageMinSpeed && !otherVehicleEngineOn)
					return;

				float dmg = data.Impulse * m_dmgContactCoef;

				if (dmg <= m_Expansion_CollisionDamageMinSpeed * EXPANSION_COLLISION_DMG_MIN_SPEED_TO_DMG_MULT)
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

		#ifdef DIAG_DEVELOPER
			CrashDebugData.m_CrashDataPoint = new CrashDebugData();
			CrashDebugData.m_CrashDataPoint.m_VehicleType = GetDisplayName();
			CrashDebugData.m_CrashDataPoint.m_Damage = dmg;
			CrashDebugData.m_CrashDataPoint.m_Zone = zoneName;
			CrashDebugData.m_CrashDataPoint.m_MomentumCurr = GetMomentum();
			CrashDebugData.m_CrashDataPoint.m_MomentumPrev = m_MomentumPrevTick;
			CrashDebugData.m_CrashDataPoint.m_MomentumDelta = data[0].impulse;
			CrashDebugData.m_CrashDataPoint.m_SpeedWorld = GetVelocity(this).Length() * 3.6;
			CrashDebugData.m_CrashDataPoint.m_SpeedWorldPrev = m_VelocityPrevTick.Length() * 3.6;
			CrashDebugData.m_CrashDataPoint.m_SpeedWorldDelta = (m_VelocityPrevTick.Length() - GetVelocity(this).Length()) * 3.6;
			CrashDebugData.m_CrashDataPoint.m_VelocityCur = GetVelocity(this);
			CrashDebugData.m_CrashDataPoint.m_VelocityPrev = m_VelocityPrevTick;
			CrashDebugData.m_CrashDataPoint.m_VelocityDot = vector.Dot(m_VelocityPrevTick.Normalized(), GetVelocity(this).Normalized());
			CrashDebugData.m_CrashDataPoint.m_Time = g_Game.GetTime();

			if (DEBUG_OUTPUT_TYPE & EVehicleDebugOutputType.DAMAGE_CONSIDERED)
			{
				Debug.Log("--------------------------------------------------");
				Debug.Log("Vehicle:" + GetDisplayName());
				Debug.Log("DMG: " + dmg);
				Debug.Log("zoneName : "+ zoneName);
				Debug.Log("momentumCurr : "+ GetMomentum());
				Debug.Log("momentumPrev : "+ m_MomentumPrevTick);
				Debug.Log("momentumDelta : "+ data[0].impulse);
				Debug.Log("speed(km/h): "+ GetVelocity(this).Length() * 3.6);
				Debug.Log("speedPrev(km/h): "+ m_VelocityPrevTick.Length() * 3.6);
				Debug.Log("speedDelta(km/h) : "+ (m_VelocityPrevTick.Length() - GetVelocity(this).Length()) * 3.6);
				Debug.Log("velocityCur.): "+ GetVelocity(this));
				Debug.Log("velocityPrev.): "+ m_VelocityPrevTick);
				Debug.Log("velocityDot): "+ vector.Dot(m_VelocityPrevTick.Normalized(), GetVelocity(this).Normalized()));
				Debug.Log("g_Game.GetTime(): "+ g_Game.GetTime());
				Debug.Log("--------------------------------------------------");
			}
		#endif

			if (dmg < GameConstants.CARS_CONTACT_DMG_MIN)
				continue;

			float crewDmgBase = Math.AbsInt((data[0].impulse / dBodyGetMass(this)) * 1000 * m_dmgContactCoef);// calculates damage as if the object's weight was 1000kg instead of its actual weight

			int pddfFlags;

		#ifdef DIAG_DEVELOPER
			CrashDebugData.m_CrashData.Insert(CrashDebugData.m_CrashDataPoint);
			CrashDebugData.m_CrashDataPoint.m_Speedometer =  GetSpeedometer();
			//Print("Crash data recorded");
		#endif

			if (dmg < GameConstants.CARS_CONTACT_DMG_THRESHOLD)
			{
			#ifdef DIAG_DEVELOPER
				CrashDebugData.m_CrashDataPoint.m_DamageType = "Small";
				if (DEBUG_OUTPUT_TYPE & EVehicleDebugOutputType.DAMAGE_APPLIED)
					Debug.Log(string.Format("[Vehiles:Damage]:: DMG %1 to the %2 zone is SMALL (threshold: %3), SPEEDOMETER: %4, TIME: %5", dmg, zoneName, GameConstants.CARS_CONTACT_DMG_MIN, GetSpeedometer(), g_Game.GetTime() ));
			#endif

				SynchCrashLightSound(true);
				pddfFlags = ProcessDirectDamageFlags.NO_TRANSFER;
			}
			else
			{
			#ifdef DIAG_DEVELOPER
				CrashDebugData.m_CrashDataPoint.m_DamageType = "Big";
				if (DEBUG_OUTPUT_TYPE & EVehicleDebugOutputType.DAMAGE_APPLIED)
					Debug.Log(string.Format("[Vehiles:Damage]:: DMG %1 to the %2 zone is BIG (threshold: %3), SPEED: %4, TIME: %5", dmg, zoneName, GameConstants.CARS_CONTACT_DMG_THRESHOLD, GetSpeedometer(), g_Game.GetTime() ));
			#endif

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

			Object targetEntity = Object.Cast(data[0].other);
			if (targetEntity && targetEntity.IsTree() && !targetEntity.IsDamageDestroyed())
			{
				SEffectManager.CreateParticleServer(targetEntity.GetPosition(), new TreeEffecterParameters("TreeEffecter", 1.0, 0.1));
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

		m_ExpansionVehicle.OnDestroyed(killer);
	}
};
