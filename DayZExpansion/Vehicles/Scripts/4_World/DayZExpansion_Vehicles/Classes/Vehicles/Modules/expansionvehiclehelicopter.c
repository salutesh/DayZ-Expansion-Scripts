#ifndef DAYZ_1_27
//! 1.28+
class ExpansionHelicopterScriptMove : CarScriptMove
{
	float m_CollectiveTarget;
	float m_AntiTorqueTarget;
	
	float m_CyclicForwardTarget;
	float m_CyclicSideTarget;
	
	float m_AutoHoverSpeedTargetX;
	float m_AutoHoverAltitude;
	float m_AutoHoverSpeedTargetZ;

	int m_Packed;
	
	protected override event void Write(PawnMoveWriter ctx, PawnMove prev)
	{
		super.Write(ctx, prev);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Write(m_CollectiveTarget);
		ctx.Write(m_AntiTorqueTarget);
		ctx.Write(m_CyclicForwardTarget);
		ctx.Write(m_CyclicSideTarget);

		ctx.Write(m_AutoHoverSpeedTargetX);
		ctx.Write(m_AutoHoverAltitude);
		ctx.Write(m_AutoHoverSpeedTargetZ);

		ctx.Write(m_Packed);
	}

	protected override event void Read(PawnMoveReader ctx, PawnMove prev)
	{
		super.Read(ctx, prev);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Read(m_CollectiveTarget);
		ctx.Read(m_AntiTorqueTarget);
		ctx.Read(m_CyclicForwardTarget);
		ctx.Read(m_CyclicSideTarget);

		ctx.Read(m_AutoHoverSpeedTargetX);
		ctx.Read(m_AutoHoverAltitude);
		ctx.Read(m_AutoHoverSpeedTargetZ);

		ctx.Read(m_Packed);
	}

	protected override event int EstimateMaximumSize()
	{
		int size = super.EstimateMaximumSize();
		size += 8 * 4;  // num variables multiplied by size of variables
		return size;
	}
};

class ExpansionHelicopterScriptOwnerState : CarScriptOwnerState
{
	float m_RotorSpeed;
	bool m_EngineState;
	
	float m_Collective;
	float m_AntiTorque;
	
	float m_Hydraulic;
	
	float m_CyclicForward;
	float m_CyclicSide;
	
	int m_Packed;
	
	protected override event void Write(PawnStateWriter ctx)
	{
		super.Write(ctx);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Write(m_RotorSpeed);
		ctx.Write(m_EngineState);
		ctx.Write(m_Collective);
		ctx.Write(m_AntiTorque);
		ctx.Write(m_Hydraulic);
		ctx.Write(m_CyclicForward);
		ctx.Write(m_CyclicSide);
		ctx.Write(m_Packed);
	}

	protected override event void Read(PawnStateReader ctx)
	{
		super.Read(ctx);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Read(m_RotorSpeed);
		ctx.Read(m_EngineState);
		ctx.Read(m_Collective);
		ctx.Read(m_AntiTorque);
		ctx.Read(m_Hydraulic);
		ctx.Read(m_CyclicForward);
		ctx.Read(m_CyclicSide);
		ctx.Read(m_Packed);
	}

	protected override event int EstimateMaximumSize()
	{
		int size = super.EstimateMaximumSize();
		size += 8 * 4; // num variables multiplied by size of variables
		return size;
	}
};
#endif

class ExpansionHelicopterScriptRotor : CollisionOverlapCallback
{
	EntityAI m_Vehicle;

	bool m_Setup = false;

	string m_InventorySlot;
	float m_ContactDamage;

	float m_Radius;
	float m_Height;

	EntityAI m_Entity;

	void ExpansionHelicopterScriptRotor(EntityAI vehicle, string rotor)
	{
		m_Vehicle = vehicle;

		string path;
		string rootPath = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotors " + rotor;
		m_Setup = g_Game.ConfigIsExisting(rootPath);

		if (m_Setup)
		{
			path = rootPath + " inventorySlot";
			if (g_Game.ConfigIsExisting(path))
				m_InventorySlot = g_Game.ConfigGetTextOut(path);

			path = rootPath + " contactDamage";
			if (g_Game.ConfigIsExisting(path))
				m_ContactDamage = g_Game.ConfigGetFloat(path);

			path = rootPath + " radius";
			if (g_Game.ConfigIsExisting(path))
				m_Radius = g_Game.ConfigGetFloat(path);

			path = rootPath + " height";
			if (g_Game.ConfigIsExisting(path))
				m_Height = g_Game.ConfigGetFloat(path);

			m_InventorySlot.ToLower();
		}
	}

	void OnAttach(string slot, EntityAI entity)
	{
		if (slot != m_InventorySlot)
		{
			return;
		}

		m_Entity = entity;
	}

	void OnDetach(string slot, EntityAI entity)
	{
		if (slot != m_InventorySlot)
		{
			return;
		}
		
		m_Entity = null;
	}

	void Update(float time)
	{
		if (g_Game.IsClient() || !m_Entity || !m_Setup)
		{
			return;
		}

		if (!m_Entity.IsDamageDestroyed())
		{
			vector transform[4];
			m_Entity.GetTransform(transform);

			int layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.TERRAIN;

			bool collided = DayZPhysics.CylinderOverlapBullet(transform, Vector(m_Radius, m_Height, m_Radius), layers, this);

			if (collided)
			{
				m_Entity.DecreaseHealth(m_ContactDamage * time, false);
			}
		}
	}

	override bool OnContact(IEntity other, Contact contact)
	{
		if (other == m_Vehicle)
		{
			return false;
		}

		return true;
	}

	int GetHealthLevel()
	{
		if (m_Entity)
			return m_Entity.GetHealthLevel();
		else
			return m_Vehicle.GetHealthLevel();
	}
};

class ExpansionVehicleHelicopter : ExpansionVehicleModule
{
	static int HELICOPTER_CONTROLLER_INDEX = 1;
	const int MAX_CYCLIC_CONTROL_POINTS = 5;

	static float REFERENCE_MASS = 1700;  //! MH6
	static float REFERENCE_BOUNDING_RADIUS = 8.07054;  //! MH6

	static bool s_UseLegacyFlightModel;
	static ExpansionHelicopterSimulationMode s_SimulationMode = ExpansionHelicopterSimulationMode.RotorDisk;  //! Client only
	ExpansionHelicopterSimulationMode m_SimulationMode;  //! Client & server
	static ExpansionHelicopterSimulationAirFrictionMode s_AirFrictionMode = ExpansionHelicopterSimulationAirFrictionMode.Balanced;  //! Client only
	ExpansionHelicopterSimulationAirFrictionMode m_AirFrictionMode;  //! Client & server
	static vector s_AirFriction = "0.9 0.12 0.35";
	vector m_AirFriction;
	static bool s_RBS;  //! Client only
	bool m_RBS;  //! Client & server
	static bool s_AutoCollective;  //! Client only
	bool m_AutoCollective;  //! Client & server
	static bool s_AutoTrim = true;  //! Client only
	bool m_AutoTrim = true;  //! Client & server
	static bool s_TranslatingTendency;  //! Client only
	bool m_TranslatingTendency;  //! Client & server
	static bool s_RotorDiskInertia = true;  //! Client only
	bool m_RotorDiskInertia = true;  //! Client & server

	ExpansionHelicopterScript m_Helicopter;

	bool m_Initialized;

	// the rotor speeds for the drag of the helicopter when going down while the engine is turned off
	float m_MinAutoRotateSpeed; // (m/s)
	float m_MaxAutoRotateSpeed; // (m/s)

	// Coefficient for cyclic control
	float m_BodyFrictionCoef;
	float m_LiftForceCoef;

	// Coefficient for cyclic control
	float m_CyclicForceCoef;

	// the rotation at which the forward cyclic moves (pitch)
	int   m_CyclicForwardControlCount;
	float m_CyclicForwardControlFluid[MAX_CYCLIC_CONTROL_POINTS];
	float m_CyclicForwardControlDelta[MAX_CYCLIC_CONTROL_POINTS];
	float m_CyclicForwardMax;	// (m) per tick
	float m_CyclicForwardCoef;	// Coefficient for forward cyclic

	// the rotation at which the side cyclic moves (roll)
	int   m_CyclicSideControlCount;
	float m_CyclicSideControlFluid[MAX_CYCLIC_CONTROL_POINTS];
	float m_CyclicSideControlDelta[MAX_CYCLIC_CONTROL_POINTS];
	float m_CyclicSideMax;	 // (m)
	float m_CyclicSideCoef;	 // Coefficient for side cyclic

	// the rotation at which the anti torque moves (yaw)
	float m_AntiTorqueSpeed; // (m/s)
	float m_AntiTorqueMax;	 // (m)

	float m_BankForceCoef = 0.7;
	float m_TailForceCoef = 8.0;

	// the time it takes for the engine to startup
	float m_EngineStartDuration; // (s)

	vector m_LinearFrictionCoef;
	float m_AngularFrictionCoef;

	// Lift factor (taken from Arma 2 config)
	int m_MaxFlightEnvelope = 15;
	float m_FlightEnvelope[15] = {0.0, 0.2, 0.9, 2.1, 2.5, 3.3, 3.5, 3.6, 3.7, 3.8, 3.8, 3.0, 0.9, 0.7, 0.5};

	//! A3-style ETL (effective translational lift) curve: hover (low) -> transitional (16-24kt) -> full ETL -> high-speed parasite drag. Scaled to match original magnitude.
	float m_FlightEnvelope_ETL[15] = {0.2, 0.5, 1.1, 1.8, 2.2, 2.8, 3.2, 3.5, 3.6, 3.7, 3.8, 3.2, 2.4, 1.8, 1.2};

	float m_RotorSpeed;
	float m_RotorSpeedTarget;

	float m_CollectiveInput;
	float m_Collective;
	float m_CollectiveTarget;
	float m_CollectiveDelta;
	float m_CollectiveDeltaInterpolated;
	float m_CollectiveInterpolated;

	float m_AntiTorque;
	float m_AntiTorqueTarget;

	float m_TailRotateFactor;

	float m_Bank;

	float m_Hydraulic = 1.0;
	float m_HydraulicPrev = -1.0; //! Set to -1 to ensure the coefficients are updated before first simulation

	float m_CyclicForward;
	float m_CyclicForwardTarget;
	float m_CyclicForwardHydraulicCoef;
	float m_CyclicForwardInput;

	float m_CyclicSide;
	float m_CyclicSideTarget;
	float m_CyclicSideHydraulicCoef;
	float m_CyclicSideInput;

	float m_CyclicForwardTrim;
	float m_CyclicBackwardTrim;
	float m_CyclicLeftTrim;
	float m_CyclicRightTrim;
	float m_AntiTorqueLeftTrim;
	float m_AntiTorqueRightTrim;

	bool m_CyclicForwardTrimTransitional;
	bool m_CyclicBackwardTrimTransitional;
	bool m_CyclicLeftTrimTransitional;
	bool m_CyclicRightTrimTransitional;
	bool m_AntiTorqueLeftTrimTransitional;
	bool m_AntiTorqueRightTrimTransitional;

	float m_AutoHoverAltitude;
	bool m_AutoHover;
	bool m_OverrideTailForce;

	bool m_IsFreeLook = true;
	bool m_WasFreeLookPressed;
	float m_VertSens = 1.0;
	float m_HorzSens = 1.0;

	vector m_AutoHoverSpeed;
	vector m_AutoHoverSpeedTarget;

	bool m_Hit;
	Object m_HitObject;
	vector m_HitPosition;
	vector m_HitNormal;
	float m_HitFraction;

	vector m_WindSpeedSync;

	bool m_EnableWind;
	bool m_EnableMainRotorDamage;
	bool m_EnableTailRotorDamage;
	bool m_EnableHelicopterExplosions;

	float m_RotorAnimationPosition;

	float m_WaterVolume;
	float m_TotalVolume;

	autoptr NoiseParams m_NoiseParams;
	Particle m_DustParticle;
	Particle m_WaterParticle;

	ref ExpansionHelicopterScriptRotor m_Main;
	ref ExpansionHelicopterScriptRotor m_Tail;

	ref ExpansionInterpolatedInput m_Input_CollectiveUp;
	ref ExpansionInterpolatedInput m_Input_CollectiveDown;
	ref ExpansionInterpolatedInput m_Input_CollectiveAnalogueUp;
	ref ExpansionInterpolatedInput m_Input_CollectiveAnalogueDown;
	bool m_Input_UseAnalogueCollective;

	ref ExpansionInterpolatedInput m_Input_AntiTorqueLeft;
	ref ExpansionInterpolatedInput m_Input_AntiTorqueRight;

	ref ExpansionInterpolatedInput m_Input_CyclicForward;
	ref ExpansionInterpolatedInput m_Input_CyclicBackward;

	ref ExpansionInterpolatedInput m_Input_CyclicLeft;
	ref ExpansionInterpolatedInput m_Input_CyclicRight;

	float m_RoughLandingVerticalSpeedThreshold;

	//! Vortex Ring State (VRS) - thrust loss when descending into own downwash
	float m_VRSDescentThreshold = 3.0;   //! m/s (~600 fpm) - VRS begins
	float m_VRSDescentDeep = 6.0;        //! m/s (~1200 fpm) - deep VRS
	float m_VRSAirspeedThreshold = 8.0;  //! m/s (~16kt) - below ETL, VRS possible
	float m_VRSThrustLossMax = 0.55;    //! Max thrust reduction in deep VRS (55% loss)
	float m_VRSSeverity;
	float m_VRSBeginAltitude;

	//! Ground Effect - within ~1 rotor diameter (MH-6 ~8m dia, use bounding radius)
	float m_GroundEffectRadius = 1.2;    //! Multiplier of bounding radius for ground effect zone
	float m_GroundEffectMax = 1.3;     //! Max lift boost in ground effect

	//! Translating Tendency - lateral drift in hover (tail rotor thrust)
	float m_TranslatingTendencyCoef = 0.27;  //! Rightward drift when hovering

	//! Retreating Blade Stall (RBS) - lift loss and instability driven by advance ratio
	float m_RetreatingBladeStallLiftLoss = 0.35;  //! Max lift reduction at extreme overspeed
	float m_RetreatingBladeStallRollCoef = 0.22;  //! Roll toward retreating (left for CCW main rotor, right for CW) side - instability
	float m_RetreatingBladeStallPitchCoef = 0.18;  //! Nose pitch-up tendency
	float m_RetreatingBladeStallDragCoef = 0.6;   //! Extra parasite drag when stalled (form drag from separated flow, 0-60% at full RBS)
	float m_RBSSeverity;

	//! Rotor-disk / blade-element model (A3 RotorLib-style)
	float m_MainRotorRadius = 4.9;        //! MH-6 rotor radius [m]
	float m_MainRotorBlades = 4;
	float m_MainRotorTipSpeed = 205.0;   //! m/s at 100% RPM
	float m_MainRotorChord = 0.35;       //! Blade chord [m]
	float m_BladeLiftSlope = 5.7;        //! Effective Cl/rad (2*pi with tip loss)
	float m_BladeStallAngle = 0.2;      //! rad (~11 deg)
	float m_BladeStallDrag = 0.05;       //! Extra Cd when stalled
	float m_RotorDiskThrustScale = 0.13; //! Tune to match hover (blade-element magnitude)
	//! Collective pitch values tweaked for MH6 to give up to ~15 m/s ascent/decsent rate
	float m_CollectivePitchAtFull = 0.15;   //! Collective pitch [rad] at m_Collective = 1.0
	float m_CollectivePitchAtMin = 0.0015; //! Collective pitch [rad] at m_Collective <= 0
	float m_RotorDiskPitchMomentGain = 0.04;
	float m_RotorDiskRollMomentGain = 0.03;

	float m_InducedPitchMoment;
	float m_InducedRollMoment;
	bool m_UseCyclicDiskMoments = true;
	float m_CyclicPitchMoment;
	float m_CyclicRollMoment;

	float m_RotorDiskForwardInertia;
	float m_RotorDiskForwardInertiaVel[1];
	float m_RotorDiskSideInertia;
	float m_RotorDiskSideInertiaVel[1];

	float m_ThrustTiltCoef = 0.2;          //! Thrust vector tilt with disk lead
	vector m_ThrustDir;

	//! Disk loading (power droop at high collective)
	float m_CollectiveLoadCoef = 0.05;

	//! When Auto-Trim is disabled, main rotor torque needs to be counteracted by anti-torque input
	float m_MainRotorTorque;

	//! Wobble during hover/low speed flight
	float m_WobbleFrequencyScale = 2.15;
	float m_WobbleIntensityScale = 0.3;
	float m_HoverTimeTracker = 0.0;

	void ExpansionVehicleHelicopter(EntityAI vehicle)
	{
		Class.CastTo(m_Helicopter, vehicle);

		m_NoiseParams = new NoiseParams();
		m_NoiseParams.Load("HeliExpansionNoise");

		m_SelfDebugWindow = true;

		m_Main = new ExpansionHelicopterScriptRotor(vehicle, "Main");
		m_Tail = new ExpansionHelicopterScriptRotor(vehicle, "Tail");

	#ifdef COMPONENT_SYSTEM
		m_Input_CollectiveUp = new ExpansionInterpolatedInput("UAExpansionHeliCollectiveUp");
		m_Input_CollectiveDown = new ExpansionInterpolatedInput("UAExpansionHeliCollectiveDown");
		m_Input_CollectiveAnalogueUp = new ExpansionInterpolatedInput("UAExpansionHeliCollectiveAnalogueUp");
		m_Input_CollectiveAnalogueDown = new ExpansionInterpolatedInput("UAExpansionHeliCollectiveAnalogueDown");
		
		m_Input_AntiTorqueLeft = new ExpansionInterpolatedInput("UAExpansionHeliAntiTorqueLeft");
		m_Input_AntiTorqueRight = new ExpansionInterpolatedInput("UAExpansionHeliAntiTorqueRight");
		
		m_Input_CyclicForward = new ExpansionInterpolatedInput("UAExpansionHeliCyclicForward");
		m_Input_CyclicBackward = new ExpansionInterpolatedInput("UAExpansionHeliCyclicBackward");
		
		m_Input_CyclicLeft = new ExpansionInterpolatedInput("UAExpansionHeliCyclicLeft");
		m_Input_CyclicRight = new ExpansionInterpolatedInput("UAExpansionHeliCyclicRight");
	#else
		m_Input_CollectiveUp = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveUp);
		m_Input_CollectiveDown = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveDown);
		m_Input_CollectiveAnalogueUp = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveAnalogueUp);
		m_Input_CollectiveAnalogueDown = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveAnalogueDown);
		
		m_Input_AntiTorqueLeft = new ExpansionInterpolatedInput(UAExpansionHeliAntiTorqueLeft);
		m_Input_AntiTorqueRight = new ExpansionInterpolatedInput(UAExpansionHeliAntiTorqueRight);
		
		m_Input_CyclicForward = new ExpansionInterpolatedInput(UAExpansionHeliCyclicForward);
		m_Input_CyclicBackward = new ExpansionInterpolatedInput(UAExpansionHeliCyclicBackward);
		
		m_Input_CyclicLeft = new ExpansionInterpolatedInput(UAExpansionHeliCyclicLeft);
		m_Input_CyclicRight = new ExpansionInterpolatedInput(UAExpansionHeliCyclicRight);
	#endif
	}

	void ~ExpansionVehicleHelicopter()
	{
		if (IsMissionClient())
		{
			if (m_DustParticle)
			{
				m_DustParticle.Stop();
			}

			if (m_WaterParticle)
			{
				m_WaterParticle.Stop();
			}
		}
	}

	override void Init()
	{
		super.Init();

		ExpansionHelicopterScript heli = m_Helicopter;
		if (!heli)
			return;

		m_MinAutoRotateSpeed = heli.m_MinAutoRotateSpeed;
		m_MaxAutoRotateSpeed = heli.m_MaxAutoRotateSpeed;
		m_BodyFrictionCoef = heli.m_BodyFrictionCoef;
		m_LiftForceCoef = heli.m_LiftForceCoef;

		m_CyclicForceCoef = heli.m_CyclicForceCoef;

		m_CyclicForwardMax = heli.m_CyclicForwardMax;
		m_CyclicForwardCoef = heli.m_CyclicForwardCoef;
		m_CyclicForwardControlCount = 2;
		m_CyclicForwardControlFluid[0] = 0.0;
		m_CyclicForwardControlDelta[0] = 0.0;
		m_CyclicForwardControlFluid[1] = 1.0;
		m_CyclicForwardControlDelta[1] = heli.m_CyclicForwardSpeed;

		m_CyclicSideMax = heli.m_CyclicSideMax;
		m_CyclicSideCoef = heli.m_CyclicSideCoef;
		m_CyclicSideControlCount = 2;
		m_CyclicSideControlFluid[0] = 0.0;
		m_CyclicSideControlDelta[0] = 0.0;
		m_CyclicSideControlFluid[1] = 1.0;
		m_CyclicSideControlDelta[1] = heli.m_CyclicSideSpeed;

		m_AntiTorqueSpeed = heli.m_AntiTorqueSpeed;
		m_AntiTorqueMax = heli.m_AntiTorqueMax;

		m_BankForceCoef = heli.m_BankForceCoef;
		m_TailForceCoef = heli.m_TailForceCoef;

		m_EngineStartDuration = heli.m_EngineStartDuration;
		m_LinearFrictionCoef = heli.m_LinearFrictionCoef;
		m_AngularFrictionCoef = heli.m_AngularFrictionCoef;

		m_TEMP_DeferredInit = true;
		m_SettingsChanged = true;
		m_Control = true;
		m_PreSimulate = true;
		m_Simulate = true;
		m_Animate = true;
		m_Network = true;

		m_RotorAnimationPosition = m_Vehicle.GetAnimationPhase("rotor");

	#ifndef SERVER
		auto settings = GetExpansionClientSettings();

		if (settings.AutoCollectiveMode == ExpansionHelicopterAutoCollectiveMode.AlwaysOn)
			s_AutoCollective = true;

		settings.SI_UpdateSetting.Insert(OnClientSettingsChanged);
	#endif
	}

	override void TEMP_DeferredInit()
	{
		m_Initialized = true;

		super.TEMP_DeferredInit();

		array<float> curve();
		int i;
		int index;

		string path;

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule bodyFrictionCoef";
		if (g_Game.ConfigIsExisting(path))
			m_BodyFrictionCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule liftForceCoef";
		if (g_Game.ConfigIsExisting(path))
			m_LiftForceCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule bankForceCoef";
		if (g_Game.ConfigIsExisting(path))
			m_BankForceCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule tailForceCoef";
		if (g_Game.ConfigIsExisting(path))
			m_TailForceCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule linearFrictionCoef";
		if (g_Game.ConfigIsExisting(path))
			m_LinearFrictionCoef = g_Game.ConfigGetVector(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule angularFrictionCoef";
		if (g_Game.ConfigIsExisting(path))
			m_AngularFrictionCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor minAutoRotateSpeed";
		if (g_Game.ConfigIsExisting(path))
			m_MinAutoRotateSpeed = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor maxAutoRotateSpeed";
		if (g_Game.ConfigIsExisting(path))
			m_MaxAutoRotateSpeed = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor startUpTime";
		if (g_Game.ConfigIsExisting(path))
			m_EngineStartDuration = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule AntiTorque speed";
		if (g_Game.ConfigIsExisting(path))
			m_AntiTorqueSpeed = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule AntiTorque max";
		if (g_Game.ConfigIsExisting(path))
			m_AntiTorqueMax = g_Game.ConfigGetFloat(path);

		m_AntiTorqueMax = Math.Clamp(m_AntiTorqueMax, 0.0, 1.0);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic forceCoefficient";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicForceCoef = g_Game.ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward speed";
		if (g_Game.ConfigIsExisting(path))
		{
			m_CyclicForwardControlCount = 2;
			m_CyclicForwardControlFluid[0] = 0.0;
			m_CyclicForwardControlDelta[0] = 0.0;
			m_CyclicForwardControlFluid[1] = 1.0;
			m_CyclicForwardControlDelta[1] = g_Game.ConfigGetFloat(path);
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward hydraulic";
		if (g_Game.ConfigIsExisting(path))
		{
			g_Game.ConfigGetFloatArray(path, curve);

			m_CyclicForwardControlCount = (curve.Count() / 2);

			if (m_CyclicForwardControlCount > MAX_CYCLIC_CONTROL_POINTS)
			{
				Error("Maximum points in 'hydraulic' exceeded. Count is '" + m_CyclicForwardControlCount + "' and maximum is '" + MAX_CYCLIC_CONTROL_POINTS + "'.");

				m_CyclicForwardControlCount = MAX_CYCLIC_CONTROL_POINTS;
			}

			for (i = 0; i < m_CyclicForwardControlCount; i++)
			{
				index = i * 2;
				m_CyclicForwardControlFluid[i] = curve[index + 0];
				m_CyclicForwardControlDelta[i] = curve[index + 1];
			#ifdef DIAG_DEVELOPER
				PrintFormat("float m_CyclicForwardControlFluid[%1] = %2", i, m_CyclicForwardControlFluid[i]);
				PrintFormat("float m_CyclicForwardControlDelta[%1] = %2", i, m_CyclicForwardControlDelta[i]);
			#endif
			}
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward max";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicForwardMax = g_Game.ConfigGetFloat(path);

		m_CyclicForwardMax = Math.Clamp(m_CyclicForwardMax, 0.0, 1.0);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward coefficient";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicForwardCoef = g_Game.ConfigGetFloat(path);

		//path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward animation";
		//if ( g_Game.ConfigIsExisting( path ) )
		//	m_Inertia = g_Game.ConfigGetTextOut( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side speed";
		if (g_Game.ConfigIsExisting(path))
		{
			m_CyclicSideControlCount = 2;
			m_CyclicSideControlFluid[0] = 0.0;
			m_CyclicSideControlDelta[0] = 0.0;
			m_CyclicSideControlFluid[1] = 1.0;
			m_CyclicSideControlDelta[1] = g_Game.ConfigGetFloat(path);
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side hydraulic";
		if (g_Game.ConfigIsExisting(path))
		{
			g_Game.ConfigGetFloatArray(path, curve);

			m_CyclicSideControlCount = (curve.Count() / 2);

			if (m_CyclicSideControlCount > MAX_CYCLIC_CONTROL_POINTS)
			{
				Error("Maximum points in 'hydraulic' exceeded. Count is '" + m_CyclicSideControlCount + "' and maximum is '" + MAX_CYCLIC_CONTROL_POINTS + "'.");

				m_CyclicSideControlCount = MAX_CYCLIC_CONTROL_POINTS;
			}

			for (i = 0; i < m_CyclicSideControlCount; i++)
			{
				index = i * 2;
				m_CyclicSideControlFluid[i] = curve[index + 0];
				m_CyclicSideControlDelta[i] = curve[index + 1];
			#ifdef DIAG_DEVELOPER
				PrintFormat("float m_CyclicSideControlFluid[%1] = %2", i, m_CyclicSideControlFluid[i]);
				PrintFormat("float m_CyclicSideControlDelta[%1] = %2", i, m_CyclicSideControlDelta[i]);
			#endif
			}
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side max";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicSideMax = g_Game.ConfigGetFloat(path);

		m_CyclicSideMax = Math.Clamp(m_CyclicSideMax, 0.0, 1.0);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side coefficient";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicSideCoef = g_Game.ConfigGetFloat(path);

		//path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side animation";
		//if ( g_Game.ConfigIsExisting( path ) )
		//	m_Inertia = g_Game.ConfigGetTextOut( path );

		m_CyclicSideCoef = 0.6 * m_CyclicSideCoef;
		m_CyclicForwardCoef = -1.6 * m_CyclicForwardCoef;

		if (m_EngineStartDuration <= 0)
		{
			m_EngineStartDuration = 0.0;
		}
		else
		{
			m_EngineStartDuration = 1.0 / m_EngineStartDuration;
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule translatingTendencyCoef";
		if (g_Game.ConfigIsExisting(path))
			m_TranslatingTendencyCoef = g_Game.ConfigGetFloat(path);
		else
		{
			//! Placeholder until A2 Helicopters Remastered is updated
			switch (m_Vehicle.GetType())
			{
				case "EXT_CH_47F":
				case "EXT_CH_47F_BAF":
				case "EXT_CH_47F_USMC":
				case "EXT_KA52":
				case "EXT_KA52_Black":
				case "EXT_KA52_Woodland":
					m_TranslatingTendencyCoef = 0.0;
					break;
			}
		}

		float vrsScale = Math.Max(m_Helicopter.m_State.m_BoundingRadius / REFERENCE_BOUNDING_RADIUS, 1.0);
		m_VRSDescentThreshold *= vrsScale;
		m_VRSDescentDeep *= vrsScale;
		m_VRSAirspeedThreshold *= vrsScale;

		float wobbleFrequencyMult = Math.Pow(Math.Clamp(REFERENCE_BOUNDING_RADIUS / m_Helicopter.m_State.m_BoundingRadius, 0.4, 1.0), 1.75);
		m_WobbleFrequencyScale *= wobbleFrequencyMult;

	#ifdef DIAG_DEVELOPER
		Print(vrsScale);
		Print(m_VRSDescentThreshold);
		Print(m_VRSDescentDeep);
		Print(m_VRSAirspeedThreshold);

		Print(wobbleFrequencyMult);
		Print(m_WobbleFrequencyScale);
	#endif
	}

	override void SettingsChanged()
	{
		super.SettingsChanged();

		auto settings = GetExpansionSettings().GetVehicle();
		m_EnableWind = settings.EnableWindAerodynamics;
		m_EnableMainRotorDamage = settings.EnableMainRotorDamage;
		m_EnableTailRotorDamage = settings.EnableTailRotorDamage;
		m_EnableHelicopterExplosions = settings.EnableHelicopterExplosions;

		if (m_RoughLandingVerticalSpeedThreshold <= 0)
		{
			m_RoughLandingVerticalSpeedThreshold = settings.RoughLandingVerticalSpeedThreshold;

			float vrsVerticalSpeedScale = Math.Max(m_RoughLandingVerticalSpeedThreshold / ExpansionVehicleSettings.ROUGH_LANDING_VERTICAL_SPEED_THRESHOLD_DEFAULT, 1.0);
			m_VRSDescentThreshold *= vrsVerticalSpeedScale;
			m_VRSDescentDeep *= vrsVerticalSpeedScale;
		#ifdef DIAG_DEVELOPER
			Print(vrsVerticalSpeedScale);
		#endif
		}
	}

	void OnClientSettingsChanged()
	{
		auto settings = GetExpansionClientSettings();

		switch (settings.AutoCollectiveMode)
		{
			case ExpansionHelicopterAutoCollectiveMode.AlwaysOn:
				s_AutoCollective = true;
				break;

			case ExpansionHelicopterAutoCollectiveMode.AlwaysOff:
				s_AutoCollective = false;
				break;
		}
	}

#ifndef DAYZ_1_27
	//! Client
	override event void ObtainMove(PawnMove pMove)
	{
		super.ObtainMove(pMove);
		
		ExpansionHelicopterScriptMove move = ExpansionHelicopterScriptMove.Cast(pMove);
		
		//move.m_EngineOn = m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX];
		
		move.m_CollectiveTarget = m_CollectiveTarget;
		move.m_AntiTorqueTarget = m_AntiTorqueTarget;
		move.m_CyclicForwardTarget = m_CyclicForwardTarget;
		move.m_CyclicSideTarget = m_CyclicSideTarget;
		
		move.m_AutoHoverSpeedTargetX = m_AutoHoverSpeedTarget[0];
		move.m_AutoHoverAltitude = m_AutoHoverAltitude;
		move.m_AutoHoverSpeedTargetZ = m_AutoHoverSpeedTarget[2];

		move.m_Packed = GetPackedNetworkVariables();
	}
	
	//! Server
	override event void ConsumeMove(PawnMove pMove)
	{
		super.ConsumeMove(pMove);

		ExpansionHelicopterScriptMove move = ExpansionHelicopterScriptMove.Cast(pMove);

		if (move)
		{
			//if (move.m_EngineOn >= 0)
			//	m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX] = move.m_EngineOn;
			
			m_CollectiveTarget = move.m_CollectiveTarget;
			m_AntiTorqueTarget = move.m_AntiTorqueTarget;
			m_CyclicForwardTarget = move.m_CyclicForwardTarget;
			m_CyclicSideTarget = move.m_CyclicSideTarget;
			
			m_AutoHoverSpeedTarget[0] = move.m_AutoHoverSpeedTargetX;
			m_AutoHoverAltitude = move.m_AutoHoverAltitude;
			m_AutoHoverSpeedTarget[2] = move.m_AutoHoverSpeedTargetZ;
			
			UnpackNetworkVariables(move.m_Packed);

			UpdateController();
		}
	}

	//! Client
	override event bool ReplayMove(PawnMove pMove)
	{
		ExpansionHelicopterScriptMove move = ExpansionHelicopterScriptMove.Cast(pMove);
		
		//if (move.m_EngineOn >= 0)
		//	m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX] = move.m_EngineOn;
		
		m_CollectiveTarget = move.m_CollectiveTarget;
		m_AntiTorqueTarget = move.m_AntiTorqueTarget;
		m_CyclicForwardTarget = move.m_CyclicForwardTarget;
		m_CyclicSideTarget = move.m_CyclicSideTarget;
			
		m_AutoHoverSpeedTarget[0] = move.m_AutoHoverSpeedTargetX;
		m_AutoHoverAltitude = move.m_AutoHoverAltitude;
		m_AutoHoverSpeedTarget[2] = move.m_AutoHoverSpeedTargetZ;
			
		UnpackNetworkVariables(move.m_Packed, false);

		return true;
	}

	//! Server
	override event void ObtainState(/*inout*/ PawnOwnerState pState)
	{
		ExpansionHelicopterScriptOwnerState state = ExpansionHelicopterScriptOwnerState.Cast(pState);
		
		state.m_RotorSpeed = m_RotorSpeed;
		state.m_EngineState = m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX];
		
		state.m_Collective = m_Collective;
		state.m_AntiTorque = m_AntiTorque;
		
		state.m_Hydraulic = m_Hydraulic;
		
		state.m_CyclicForward = m_CyclicForward;
		state.m_CyclicSide = m_CyclicSide;
		
		state.m_Packed = GetPackedNetworkVariables();
	}

	//! Client
	override event void RewindState(PawnOwnerState pState, /*inout*/ PawnMove pMove, inout NetworkRewindType pRewindType)
	{
		// possibly in dynamic collision, reverts to something like the old correction system
		if (pRewindType == NetworkRewindType.ADDITIVE)
			return;

		ExpansionHelicopterScriptOwnerState state = ExpansionHelicopterScriptOwnerState.Cast(pState);
		ExpansionHelicopterScriptMove move = ExpansionHelicopterScriptMove.Cast(pMove);
		
		m_RotorSpeed = state.m_RotorSpeed;
		m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX] = state.m_EngineState;
		
		m_Collective = state.m_Collective;
		m_AntiTorque = state.m_AntiTorque;
		
		m_Hydraulic = state.m_Hydraulic;
		
		// set to -1 so hydraulic coefs are forced to be recalculated
		m_HydraulicPrev = -1.0;
		
		m_CyclicForward = state.m_CyclicForward;
		m_CyclicSide = state.m_CyclicSide;
		
		UnpackNetworkVariables(state.m_Packed, false);
	}
#endif

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		if (!m_Initialized || !pDriver || pState.m_HaltPhysics)
			return;

		float pDt = pState.m_DeltaTime;

		UAInterface inputInterface;

	#ifndef DAYZ_1_27
		//! 1.28+
		if (m_Transport.GetNetworkMoveStrategy() == NetworkMoveStrategy.PHYSICS)
		{
		#ifdef SERVER
			return;
		#endif
		}
		else
	#endif
		{
			inputInterface = pDriver.GetInputInterface();
		}

		if (IsMissionClient())
		{
			if (GetExpansionClientSettings().UseHelicopterMouseControl)
			{
			#ifdef COMPONENT_SYSTEM
				bool isFreelook = InputValue("UAExpansionHeliFreeLook", inputInterface);
			#else
				bool isFreelook = InputValue_ID(UAExpansionHeliFreeLook, inputInterface);
			#endif

				if (isFreelook && !m_WasFreeLookPressed)
				{
					m_WasFreeLookPressed = true;
					m_IsFreeLook = !m_IsFreeLook;
				}
				else if (!isFreelook)
				{
					m_WasFreeLookPressed = false;
				}
			}
			else
			{
				m_IsFreeLook = true;
			}

			//! @note sensitivity is used for all vertical/horizontal inputs

			m_VertSens = GetExpansionClientSettings().HelicopterVerticalSensitivity;
			if (GetExpansionClientSettings().UseInvertedMouseControl)
				m_VertSens = -m_VertSens;

			m_HorzSens = GetExpansionClientSettings().HelicopterHorizontalSensitivity;
		}

	#ifndef SERVER
		float c_up = m_Input_CollectiveUp.GetValue(pState.m_DeltaTime, 0.02) * 0.02;
		float c_down = m_Input_CollectiveDown.GetValue(pState.m_DeltaTime, 0.02) * 0.02;
		float c_analogue_up = m_Input_CollectiveAnalogueUp.GetValue(pState.m_DeltaTime, 1.0);
		float c_analogue_down = m_Input_CollectiveAnalogueDown.GetValue(pState.m_DeltaTime, 1.0);

		if (c_analogue_up > 0 || c_analogue_down > 0)
			m_Input_UseAnalogueCollective = true;
		else if (c_up > 0 || c_down > 0)
			m_Input_UseAnalogueCollective = false;
	#endif

		float at_left = m_Input_AntiTorqueLeft.GetValue(pState.m_DeltaTime, m_HorzSens, inputInterface);
		float at_right = m_Input_AntiTorqueRight.GetValue(pState.m_DeltaTime, m_HorzSens, inputInterface);

		float vSens = Math.AbsFloat(m_VertSens);
		float c_forward = m_Input_CyclicForward.GetValue(pState.m_DeltaTime, vSens, inputInterface);
		float c_backward = m_Input_CyclicBackward.GetValue(pState.m_DeltaTime, vSens, inputInterface);

		float c_left = m_Input_CyclicLeft.GetValue(pState.m_DeltaTime, m_HorzSens, inputInterface);
		float c_right = m_Input_CyclicRight.GetValue(pState.m_DeltaTime, m_HorzSens, inputInterface);

		if (!IsFreeLook())
		{
			c_forward += InputValue_ID(UAAimDown, inputInterface) * m_VertSens;
			c_backward += InputValue_ID(UAAimUp, inputInterface) * m_VertSens;

			c_left += InputValue_ID(UAAimLeft, inputInterface) * m_HorzSens;
			c_right += InputValue_ID(UAAimRight, inputInterface) * m_HorzSens;
		}
		
		if (IsAutoHover() && pDriver && !g_Game.IsDedicatedServer() && GetExpansionClientSettings().TurnOffAutoHoverDuringFlight)
		{
			//! Automatically deactivate autohover if forward/backward/left/right input is detected (20% sensitivity)
			if (c_forward > 0.2 || c_backward > 0.2 || c_right > 0.2 || c_left > 0.2)
				SwitchAutoHover();
		}

	#ifdef COMPONENT_SYSTEM
		bool trimRelease = InputPress("UAExpansionHeliTrimRelease", inputInterface);
		bool trimSet = InputPress("UAExpansionHeliTrimSet", inputInterface);
		bool trimCyclic = InputPress("UAExpansionHeliTrimCyclic", inputInterface);
		bool trimAntiTorque = InputPress("UAExpansionHeliTrimAntiTorque", inputInterface);
	#else
		bool trimRelease = InputPress_ID(UAExpansionHeliTrimRelease, inputInterface);
		bool trimSet = InputPress_ID(UAExpansionHeliTrimSet, inputInterface);
		bool trimCyclic = InputPress_ID(UAExpansionHeliTrimCyclic, inputInterface);
		bool trimAntiTorque = InputPress_ID(UAExpansionHeliTrimAntiTorque, inputInterface);
	#endif

		if (trimRelease)
		{
			ReleaseTrim();
		}
		else if (!m_AutoTrim)
		{
			if (trimSet)
			{
				trimCyclic = true;
				trimAntiTorque = true;
			}

			ApplyTrim(c_forward, m_CyclicForwardTrim, m_CyclicForwardTrimTransitional);
			ApplyTrim(c_backward, m_CyclicBackwardTrim, m_CyclicBackwardTrimTransitional);
			ApplyTrim(c_left, m_CyclicLeftTrim, m_CyclicLeftTrimTransitional);
			ApplyTrim(c_right, m_CyclicRightTrim, m_CyclicRightTrimTransitional);

			if (trimCyclic)
			{
				SetTrim(c_forward, m_CyclicForwardTrim, m_CyclicForwardTrimTransitional);
				SetTrim(c_backward, m_CyclicBackwardTrim, m_CyclicBackwardTrimTransitional);
				SetTrim(c_left, m_CyclicLeftTrim, m_CyclicLeftTrimTransitional);
				SetTrim(c_right, m_CyclicRightTrim, m_CyclicRightTrimTransitional);
			}

			ApplyTrim(at_left, m_AntiTorqueLeftTrim, m_AntiTorqueLeftTrimTransitional);
			ApplyTrim(at_right, m_AntiTorqueRightTrim, m_AntiTorqueRightTrimTransitional);

			if (trimAntiTorque)
			{
				SetTrim(at_left, m_AntiTorqueLeftTrim, m_AntiTorqueLeftTrimTransitional);
				SetTrim(at_right, m_AntiTorqueRightTrim, m_AntiTorqueRightTrimTransitional);
			}
		}

		m_CyclicForwardInput = c_forward - c_backward;
		m_CyclicSideInput = c_left - c_right;

		m_AntiTorqueTarget = at_left  - at_right;

		if (IsAutoHover())
		{
		#ifndef SERVER
			if (IsMissionClient())
			{
				float autoHoverHeight = m_AutoHoverAltitude;
				float autoHoverChange;

				//! @note max return value from interpolated non-analogue input when going from 0 to 1 with interpolation speed 50 and
				//! sensitivity 0.02 is 0.0183583
				//! 0.0183583 * 54.471274 = ~1.0

				if (m_Input_UseAnalogueCollective)
					autoHoverChange = c_analogue_up - c_analogue_down;
				else
					autoHoverChange = (c_up - c_down) * 54.471274;

				autoHoverHeight += autoHoverChange;

				float surfaceY = g_Game.SurfaceY(pState.m_Transform[3][0], pState.m_Transform[3][2]);
				if (m_Hit)
					surfaceY = m_HitPosition[1];

				autoHoverHeight -= surfaceY;

				if (pState.m_LinearVelocity[2] > 11)
					if (autoHoverHeight < 11)
						autoHoverHeight = 11;

				if (autoHoverHeight < -0.2)
					autoHoverHeight = 0.2;

				autoHoverHeight += surfaceY;

				if (m_RotorSpeedTarget > 0 || m_RotorSpeed == 0)
					m_AutoHoverAltitude = autoHoverHeight;

				float fSpd = c_forward - c_backward;
				float sSpd = c_right - c_left;

				float sSpdMult = pState.m_MaxSpeedMS * 0.7;
				float fSpdMult = pState.m_MaxSpeedMS * 0.9;

				m_AutoHoverSpeedTarget[0] = sSpd * sSpdMult;
				m_AutoHoverSpeedTarget[2] = fSpd * fSpdMult;
			}
		#endif

			m_CyclicForwardTarget = 0.0;
			m_CyclicSideTarget = 0.0;
		}
		else
		{
			m_CyclicForwardTarget = c_forward - c_backward;
			m_CyclicSideTarget = c_left - c_right;
		}

	#ifndef SERVER
		if (m_Input_UseAnalogueCollective)
		{
			m_CollectiveInput = c_analogue_up - c_analogue_down;

			//! Allow full collective authority in autohover during autorotation
			if ((!m_AutoHover && !m_AutoCollective) || (m_RotorSpeedTarget == 0 && pState.m_HasDriver))
				m_CollectiveTarget = Math.Clamp(0.5 + m_CollectiveInput * 0.5, 0, 1);
		}
		else
		{
			m_CollectiveInput = c_up - c_down;

			//! Allow full collective authority in autohover during autorotation
			if ((!m_AutoHover && !m_AutoCollective) || (m_RotorSpeedTarget == 0 && pState.m_HasDriver))
				m_CollectiveTarget = Math.Clamp(m_CollectiveInput + m_CollectiveTarget, 0, 1);
		}
	#endif

		UpdateController();

		if (m_VRSSeverity > 0)
		{
			//! Stick shake
			m_CyclicForwardInput += Math.RandomFloatInclusive(-1, 1) * pDt * 4 * m_VRSSeverity;
			m_CyclicSideInput += Math.RandomFloatInclusive(-1, 1) * pDt * 4 * m_VRSSeverity;
		}
	}

	void UpdateController()
	{
		//! Not used ATM
		m_Controller.SetYaw(m_AntiTorqueTarget);
		m_Controller.SetRoll(m_CyclicSideTarget);
		m_Controller.SetPitch(m_CyclicForwardTarget);

		m_Controller.SetThrottle(m_CollectiveTarget, HELICOPTER_CONTROLLER_INDEX);
		m_Controller.SetBrake(0, HELICOPTER_CONTROLLER_INDEX);
	}

	float InputValue(string action, UAInterface inputInterface = null)
	{
		if (inputInterface)
			return inputInterface.SyncedValue(action);

		return GetUApi().GetInputByName(action).LocalValue();
	}

	float InputValue_ID(int action, UAInterface inputInterface = null)
	{
		if (inputInterface)
			return inputInterface.SyncedValue_ID(action);

		return GetUApi().GetInputByID(action).LocalValue();
	}

	float InputPress(string action, UAInterface inputInterface = null)
	{
		if (inputInterface)
			return inputInterface.SyncedClick(action);

		return GetUApi().GetInputByName(action).LocalClick();
	}

	float InputPress_ID(int action, UAInterface inputInterface = null)
	{
		if (inputInterface)
			return inputInterface.SyncedClick_ID(action);

		return GetUApi().GetInputByID(action).LocalClick();
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		if (!m_Initialized)
			return;

		if (!g_Game.IsDedicatedServer())
		{
			auto settings = GetExpansionClientSettings();

			s_RBS = settings.EnableRetreatingBladeStall;
			s_AutoTrim = !settings.DisableAutoTrim;
			s_TranslatingTendency = settings.EnableTranslatingTendency;

		#ifdef DIAG_DEVELOPER
			if (!g_Game.IsClient() && (!m_Vehicle.GetIsSimulationDisabled() || pState.m_HasDriver))
			{
				//! Offline/SP
				if (s_SimulationMode != m_SimulationMode)
					Message(typename.EnumToString(ExpansionHelicopterSimulationMode, s_SimulationMode));

				if (s_AirFrictionMode != m_AirFrictionMode)
					Message(typename.EnumToString(ExpansionHelicopterSimulationAirFrictionMode, s_AirFrictionMode));

				if (s_AirFriction[0] != m_AirFriction[0])
					Message("Air Friction X " + s_AirFriction[0]);

				if (s_AirFriction[1] != m_AirFriction[1])
					Message("Air Friction Y " + s_AirFriction[1]);

				if (s_AirFriction[2] != m_AirFriction[2])
					Message("Air Friction Z " + s_AirFriction[2]);

				if (s_RBS != m_RBS)
					Message("Retreating Blade Stall " + s_RBS.ToString());

				if (s_AutoCollective != m_AutoCollective)
					Message("Auto Collective " + s_AutoCollective.ToString());

				if (s_AutoTrim != m_AutoTrim)
					Message("Auto-Trim " + s_AutoTrim.ToString());

				if (s_TranslatingTendency != m_TranslatingTendency)
					Message("Translating Tendency " + s_TranslatingTendency.ToString());

				if (s_RotorDiskInertia != m_RotorDiskInertia)
					Message("Rotor Disk Inertia " + s_RotorDiskInertia.ToString());
			}

		#endif

			//! @note On diag client, these values are synced to server
			m_SimulationMode = s_SimulationMode;
			m_AirFrictionMode = s_AirFrictionMode;
			m_AirFriction[0] = s_AirFriction[0];
			m_AirFriction[1] = s_AirFriction[1];
			m_AirFriction[2] = s_AirFriction[2];

			m_RBS = s_RBS;
			m_AutoCollective = s_AutoCollective;
			m_AutoTrim = s_AutoTrim;
			m_TranslatingTendency = s_TranslatingTendency;

		#ifdef DIAG_DEVELOPER
			m_RotorDiskInertia = s_RotorDiskInertia;
		#endif
		}
	#ifndef DIAG_DEVELOPER
		else
		{
			//! @note On non-diag server, these values are not synced from client
			m_SimulationMode = s_SimulationMode;
			m_AirFrictionMode = s_AirFrictionMode;
			m_AirFriction[0] = s_AirFriction[0];
			m_AirFriction[1] = s_AirFriction[1];
			m_AirFriction[2] = s_AirFriction[2];
		}
	#endif

		float modelSize = pState.m_BoundingRadius * 1.5;
		modelSize += 15.0;

		vector start = pState.m_Transform[3];
		vector end = pState.m_Transform[3] - Vector(0, modelSize, 0);

		int component;
		set<Object> results = new set<Object>;
		m_Hit = DayZPhysics.RaycastRV(start, end, m_HitPosition, m_HitNormal, component, results, null, m_Vehicle, false, false, ObjIntersectView, 0.0);
		if (m_Hit)
			m_HitFraction = ExpansionMath.LinearConversion(start[1], end[1], m_HitPosition[1], 0.0, 1.0);
		else
			m_HitFraction = 0;

	#ifndef SERVER
		if (m_EnableWind)
		{
			m_WindSpeedSync = g_Game.GetWeather().GetWind();

			if (m_Hit)
			{

				float distance = pState.m_Transform[3][1] - m_HitPosition[1];

				vector bbox[2];
				distance /= ((pState.m_BoundingRadius * 1.5) + 10.0);
				m_WindSpeedSync = m_WindSpeedSync * distance;
			}
			//else
			//{
				//float rnd = fad.m_Player.GetRandomGeneratorSyncManager().GetRandom01(RandomGeneratorSyncUsage.RGSGeneric);

				//m_HasDriver

				// turbulence?
			//}
		}
		else
		{
			m_WindSpeedSync = vector.Zero;
		}
	#endif

		m_Main.Update(pState.m_DeltaTime);
		m_Tail.Update(pState.m_DeltaTime);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Profile(EXTrace.VEHICLES, this, "Simulate");
	#endif

		if (!m_Initialized)
			return;

		vector force;
		vector torque;
		float pDt = pState.m_DeltaTime;

		bool isAboveWater;
		float buoyancyForce;
		float horiSpeed = Vector(pState.m_LinearVelocityMS[0], 0, pState.m_LinearVelocityMS[2]).Length();
		float descentRate = -pState.m_LinearVelocityMS[1];
		float autoDescentLimiter = 1.0;
		//if (pState.m_AltitudeLimiter < 1.0)
			//autoDescentLimiter = 0.25;

		float change;

		m_RotorSpeedTarget = 0;
		if (pState.m_Exploded)
		{
			// if the heli isn't over water no force will be applied and the game will clean up physics for us
			if (m_WaterVolume < m_TotalVolume)
			{
				buoyancyForce = ExpansionPhysics.CalculateBuoyancyAtPosition(pState.m_Transform[3], 2.0, pState.m_Mass, 2.0, pState.m_LinearVelocity, isAboveWater);

				if (!isAboveWater)
				{
					// slowly sink helicopter
					m_WaterVolume += 0.05 * pDt * m_WaterVolume;

					force += Vector(0, buoyancyForce * (m_TotalVolume - m_WaterVolume) / m_TotalVolume, 0);
				}
			}

			dBodySetDamping(m_Vehicle, 0.0, 0.0);

			pState.m_Force += force;
			pState.m_Torque += torque;
		}
		else
		{
			if (m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX])
				m_RotorSpeedTarget = 1;

			if (IsMissionHost() && m_NoiseParams)
			{
				g_Game.GetNoiseSystem().AddNoise(m_Vehicle, m_NoiseParams);
			}

			if (!m_Hit && !pState.m_HasDriver && !IsAutoHover())
			{
			#ifndef SERVER
				m_CollectiveTarget = m_Transport.RandomFloat(-1, 1);
				m_AntiTorqueTarget = m_Transport.RandomFloat(-1, 1);
			#endif

				m_CyclicSideTarget = m_Transport.RandomFloat(-1, 1);
				m_CyclicForwardTarget = m_Transport.RandomFloat(-1, 1);
			}
			else if (IsAutoHover())
			{
				if (!pState.m_HasDriver)
				{
					m_AutoHoverSpeed = "0 0 0";
					m_AutoHoverSpeedTarget = "0 0 0";
					m_CyclicForwardTarget = 0;
					m_CyclicSideTarget = 0;
					m_AntiTorqueTarget = 0;
				}
				else
				{
					float sSpdMult = pState.m_MaxSpeedMS * 0.7;
					float fSpdMult = pState.m_MaxSpeedMS;

					change = Math.Clamp(m_AutoHoverSpeedTarget[0] - m_AutoHoverSpeed[0], -sSpdMult * 0.5 * pState.m_DeltaTime, sSpdMult * 0.5 * pState.m_DeltaTime);
					m_AutoHoverSpeed[0] = Math.Clamp(m_AutoHoverSpeed[0] + change, -sSpdMult, sSpdMult);

					change = Math.Clamp(m_AutoHoverSpeedTarget[2] - m_AutoHoverSpeed[2], -fSpdMult * 0.5 * pState.m_DeltaTime, fSpdMult * 0.5 * pState.m_DeltaTime);
					m_AutoHoverSpeed[2] = Math.Clamp(m_AutoHoverSpeed[2] + change, -fSpdMult, fSpdMult);
				}

				//! One second ahead
				vector estimatedPosition = pState.EstimatePosition(1.0);
				vector estimatedOrientation = pState.EstimateOrientation(1.0);
				vector targetOrientation = vector.Zero;

				if (m_RotorSpeedTarget > 0 || !pState.m_HasDriver)
				{
					change = Math.Clamp(m_AutoHoverAltitude - estimatedPosition[1], -0.25 * pDt * autoDescentLimiter, 0.25 * pDt);
					m_CollectiveTarget = Math.Clamp(m_Collective + change, 0, 1);
				}

				if (m_RotorSpeedTarget > 0)
				{
					float fwd = pState.m_LinearVelocityMS[2];
					float side = pState.m_LinearVelocityMS[0];

					if (m_VRSSeverity > 0.1 || (m_VRSSeverity > 0 && m_VRSBeginAltitude != 0))
					{
						//! Attempt automatic recovery if in VRS
						
						if (Math.AbsFloat(fwd) < m_VRSAirspeedThreshold * 0.9)
						{
							//! If fwd speed is less than 90% of VRS airspeed threshold, initiate Vuichard recovery
							m_OverrideTailForce = true;  //! Engage tail force override for clean bank
							m_AutoHoverSpeed[0] = 20.0 * Math.Sign(side);
							m_CollectiveTarget = 1.0;
						}
						else
						{
							m_AutoHoverSpeed[2] = m_VRSAirspeedThreshold * 2 * Math.Sign(fwd);
							m_CollectiveTarget = 0.01;  //! lower collective if fwd/bwd recovery
						}

						if (m_VRSBeginAltitude == 0)
							m_VRSBeginAltitude = pState.m_Transform[3][1];
					}
					else
					{
						//! As soon as the recovery is complete or there is any cyclic/antitorque input, disengage tail force override
						if (m_OverrideTailForce && ((Math.AbsFloat(m_Bank) < 0.01 && Math.AbsFloat(side) < 1.388) || Math.AbsFloat(m_CyclicForwardInput) > 0.01 || Math.AbsFloat(m_CyclicSideInput) > 0.01 || Math.AbsFloat(m_AntiTorqueTarget) > 0.01))
							m_OverrideTailForce = false;

						if (m_VRSBeginAltitude != 0)
						{
						#ifdef DIAG_DEVELOPER
							float vrsAltitudeLoss = m_VRSBeginAltitude - pState.m_Transform[3][1];
							EXPrint(m_Helicopter, "VRS altitude loss: " + vrsAltitudeLoss);
						#endif
							m_VRSBeginAltitude = 0;
						}
					}
				}
				else
				{
					m_OverrideTailForce = false;
				}

				if (m_CyclicForwardTarget == 0 && m_CyclicSideTarget == 0)
				{
					float forwardSpeed = pState.m_LinearVelocityMS[2] - m_AutoHoverSpeed[2];
					float sideSpeed = pState.m_LinearVelocityMS[0] - m_AutoHoverSpeed[0];

					//! Compensate for translating tendency
					if (m_TranslatingTendency && m_TranslatingTendencyCoef != 0.0)
					{
						float sideSpeedTarget = m_TranslatingTendencyCoef * 5.925925;
						//! Full effect at 0-5 km/h, no effect at ETL
						sideSpeedTarget *= ExpansionMath.LinearConversion(1.388888, 8.0, Math.AbsFloat(pState.m_LinearVelocityMS[2]), 1.0, 0);

						if (sideSpeedTarget < 0)
						{
							if (sideSpeed < 0 && sideSpeed > sideSpeedTarget * 2)
								sideSpeed += sideSpeedTarget;
						}
						else
						{
							if (sideSpeed > 0 && sideSpeed < sideSpeedTarget * 2)
								sideSpeed += sideSpeedTarget;
						}
					}

					forwardSpeed = Math.Clamp(forwardSpeed, -25.0, 25.0);
					sideSpeed = Math.Clamp(sideSpeed, -45.0, 45.0);

					float heightDiff = estimatedPosition[1] - m_AutoHoverAltitude;
					if (heightDiff >= 20.0 && forwardSpeed < 0)
						forwardSpeed *= 2.0;

					targetOrientation[1] = forwardSpeed;
					targetOrientation[2] = -sideSpeed;

					float targetPitchDiff = estimatedOrientation[1] - targetOrientation[1];
					float targetRollDiff = estimatedOrientation[2] - targetOrientation[2];

					//! Anti-wobble (below 10 km/h)
					float absFwd = Math.AbsFloat(forwardSpeed);
					float absSide = Math.AbsFloat(sideSpeed);
					if (absFwd < 2.7778)
						targetPitchDiff *= absFwd / 2.7778;
					if (absSide < 2.7778)
						targetRollDiff *= absSide / 2.7778;

					m_CyclicForwardTarget = Math.Clamp(targetPitchDiff * 0.25, -1.0, 1.0);
					m_CyclicSideTarget = Math.Clamp(targetRollDiff * 0.25, -1.0, 1.0);
				}
			}
			else
			{
				m_OverrideTailForce = false;

				m_AutoHoverSpeed = "0 0 0";

			#ifndef SERVER
				m_AutoHoverAltitude = pState.m_Transform[3][1];
			#endif

				if (m_AutoCollective && (m_RotorSpeedTarget > 0 || !pState.m_HasDriver))
				{
					float autoCollectiveChange;

					//! @note max return value from interpolated non-analogue input when going from 0 to 1 with interpolation speed 50 and
					//! sensitivity 0.02 is 0.0183583
					//! 0.0183583 * 54.471274 = ~1.0

					if (m_Input_UseAnalogueCollective)
						autoCollectiveChange = m_CollectiveInput;
					else
						autoCollectiveChange = m_CollectiveInput * 54.471274;

					m_AutoHoverAltitude += autoCollectiveChange * 16.5;

					//! One second ahead
					vector estimatedPosition2 = pState.EstimatePosition(1.0);

					change = Math.Clamp(m_AutoHoverAltitude - estimatedPosition2[1], -0.25 * pDt * autoDescentLimiter, 0.25 * pDt);
					m_CollectiveTarget = Math.Clamp(m_Collective + change, 0, 1);
				}
			}

			if (m_AutoHover || m_AutoCollective)
			{
				if (m_RotorSpeedTarget > 0)
				{
					if (m_VRSSeverity == 0 || m_VRSBeginAltitude == 0)
					{
						float descentAccelRate = -pState.m_LinearAccelerationMS[1];
						if (horiSpeed < m_VRSAirspeedThreshold * 1.5 && ((descentRate > 0 && descentAccelRate > m_VRSDescentThreshold * 0.5 * pDt) || descentRate > m_VRSDescentThreshold * 0.5))
						{
							float collectiveMin;

						#ifdef DIAG_DEVELOPER
							if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
								collectiveMin = ExpansionMath.LinearConversion(3, 12.9, m_VRSDescentThreshold, 0.346, 0.0);
							else if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy_Extended)
								collectiveMin = ExpansionMath.LinearConversion(3, 12.811, m_VRSDescentThreshold, 0.34, 0.0);
							else
						#endif
							{
								collectiveMin = ExpansionMath.LinearConversion(3, 15, m_VRSDescentThreshold, 0.403, 0.0);

								if (m_LiftForceCoef < 1.3)
								{
									//! With lift force coef < 1.3 nonlinear collective segment may start earlier,
									//! so need to convert from linear response to actual response
									float theta0 = GetTheta0(collectiveMin, 1.3);
									collectiveMin = GetCollectiveInput(theta0, m_LiftForceCoef);
								}
							}

							//if (pState.m_AltitudeLimiter < 1.0)
								//collectiveMin = Math.Lerp(collectiveMin, m_Collective, descentRate / (m_VRSDescentThreshold - 0.15) * (2.0 - pState.m_AltitudeLimiter));
							//else
								collectiveMin *= Math.Max(descentRate / m_VRSDescentThreshold, 1.0);  //! VRSProtecPLUS(tm) technology

							collectiveMin = Math.Clamp(collectiveMin, 0.0, 1.0);

							collectiveMin = ExpansionMath.LinearConversion(m_VRSAirspeedThreshold, m_VRSAirspeedThreshold * 1.5, horiSpeed, collectiveMin, m_CollectiveTarget);

							if (m_CollectiveTarget < collectiveMin)
								m_CollectiveTarget = collectiveMin;  //! Prevent entering VRS by limiting rate of descent
						}
					}
				}
			}
		}

		// see the speed at which we are free falling
		vector up = pState.m_Transform[1];
		float upSign = Math.Sign(up[1]);
		float goingDown = Math.Clamp((descentRate * upSign - m_MinAutoRotateSpeed) / (m_MaxAutoRotateSpeed - m_MinAutoRotateSpeed), 0, 1);
		float brakeRotor = Math.Max(Math.Max(m_Collective * 0.2, 0), -goingDown);

		float collectiveLoad;

		//! Disk loading: High collective, high horizontal speed = high load
		if (m_RotorSpeedTarget > 0 && m_Collective > 0.25)
		{
			float loadFactor;

		#ifdef DIAG_DEVELOPER
			if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
				loadFactor = 0.9;
			else
		#endif
				loadFactor = 1.0;

			float bucketSpeed = 31 * loadFactor * Math.Sqrt(pState.m_Mass / REFERENCE_MASS);
			float speedFactor;
			float collectiveLoadCoef;

			if (horiSpeed <= bucketSpeed)
			{
				speedFactor = horiSpeed / bucketSpeed;
				collectiveLoadCoef = Math.Lerp(m_CollectiveLoadCoef, 0, speedFactor);

				float collectiveFactor = ExpansionMath.LinearConversion(10.0, bucketSpeed, horiSpeed, 0.5, 0.25);
				collectiveLoad = ExpansionMath.LinearConversion(collectiveFactor, 1.0, m_Collective, 0, collectiveLoadCoef);
			}
			else
			{
				float excessSpeed = horiSpeed - bucketSpeed;
				speedFactor = excessSpeed / bucketSpeed;
				collectiveLoadCoef = Math.Lerp(0, m_CollectiveLoadCoef, speedFactor);
				collectiveLoad = ExpansionMath.LinearConversion(0.25, 1.0, m_Collective, 0, collectiveLoadCoef);
			}
		}

		// https://en.wikipedia.org/wiki/Autorotation (https://en.wikipedia.org/wiki/Autorotation#/media/File:Airflow_in_auto-2.jpg)
		// only in effect if the heli is falling down and the rotor is turn off
		if (goingDown > 0 && m_RotorSpeedTarget < 0.1)
		{
			change = (Math.Min(goingDown, 1.0) - m_RotorSpeed) * 0.08 * pDt;
		}
		else
		{
			change = m_RotorSpeedTarget - m_RotorSpeed;
		}

		change = Math.Clamp(change, (-0.025 - (brakeRotor * 0.35)) * pDt, m_EngineStartDuration * pDt);
		m_RotorSpeed = Math.Clamp(m_RotorSpeed + change, 0, 1.0 - collectiveLoad);

		m_CollectiveDelta = (m_CollectiveTarget - m_Collective) * m_RotorSpeed * m_RotorSpeed;
		if (Math.AbsFloat(m_CollectiveDelta) > 0)
			m_CollectiveDeltaInterpolated = Math.Lerp(m_CollectiveDeltaInterpolated, m_CollectiveDelta, pDt);
		else
			m_CollectiveDeltaInterpolated = Math.Lerp(m_CollectiveDeltaInterpolated, m_CollectiveDelta, 0.25 * pDt);

		change = Math.Clamp(Math.Min(m_CollectiveTarget, m_RotorSpeed + collectiveLoad) - m_Collective, -0.25 * pDt, 0.25 * pDt);
		m_Collective = Math.Clamp(m_Collective + change, 0.0, m_RotorSpeed + collectiveLoad);

		change = Math.Clamp(m_AntiTorqueTarget * m_AntiTorqueMax - m_AntiTorque, -m_AntiTorqueSpeed * pDt, m_AntiTorqueSpeed * pDt);
		m_AntiTorque = Math.Clamp(m_AntiTorque + change, -m_AntiTorqueMax, m_AntiTorqueMax);

		if (pState.m_Exploded)
		{
			return;
		}

		m_Hydraulic = m_Helicopter.GetFluidFraction(CarFluid.OIL);  //! Hydraulic fluid

		//! Don't be super responsive with the updating - better for performance like this and there is little use to looking up EVERY simulation step
		if (m_Hydraulic != m_HydraulicPrev)
		{
			m_Hydraulic = Math.Clamp(m_Hydraulic, 0.0, 1.0);
			m_HydraulicPrev = m_Hydraulic;

			m_CyclicForwardHydraulicCoef	= ExpansionMath.LookUp(m_Hydraulic, m_CyclicForwardControlCount, m_CyclicForwardControlFluid, m_CyclicForwardControlDelta);
			m_CyclicSideHydraulicCoef		= ExpansionMath.LookUp(m_Hydraulic, m_CyclicSideControlCount, m_CyclicSideControlFluid, m_CyclicSideControlDelta);
		}

		if (m_RotorSpeed != 0 || (pState.m_LinearVelocityMS.LengthSq() > 0.0025 && !m_Helicopter.m_IsLanded))
		{
			change = Math.Clamp(Math.Clamp(m_CyclicForwardTarget * m_CyclicForwardMax, -2, 2) - m_CyclicForward, -m_CyclicForwardHydraulicCoef * pDt, m_CyclicForwardHydraulicCoef * pDt);
			m_CyclicForward = Math.Clamp(m_CyclicForward + change, -m_CyclicForwardMax, m_CyclicForwardMax);

			change = Math.Clamp(Math.Clamp(m_CyclicSideTarget * m_CyclicSideMax, -2, 2) - m_CyclicSide, -m_CyclicSideHydraulicCoef * pDt, m_CyclicSideHydraulicCoef * pDt);
			m_CyclicSide = Math.Clamp(m_CyclicSide + change, -m_CyclicSideMax, m_CyclicSideMax);

			//! 0 if heli is facing direction of movement, 1 if perpendicular
			vector vehDir = pState.m_Transform[2];
			vector velDir = pState.m_LinearVelocity.Normalized();
			float perpendicular = Math.Acos(vector.Dot(vehDir, velDir)) / Math.PI_HALF;
			if (perpendicular > 1.0)
				perpendicular = 2.0 - perpendicular;

			float rbsSeverity;

			switch (m_SimulationMode)
			{
			#ifdef DIAG_DEVELOPER
				case ExpansionHelicopterSimulationMode.Legacy:
				case ExpansionHelicopterSimulationMode.Legacy_Extended:
					Simulate_Legacy(pState, force, torque, horiSpeed, rbsSeverity);
					break;
			#endif

				case ExpansionHelicopterSimulationMode.RotorDisk:
				default:
					Simulate_RotorDisk(pState, force, torque, horiSpeed, perpendicular, rbsSeverity);
					break;
			}

			Simulate_Common(pState, force, torque, horiSpeed, perpendicular, rbsSeverity);
		}

		PreventSinkingInOcean(pState, force, torque);

		pState.m_Force += force;
		pState.m_Torque += torque;
	}

	void Simulate_Legacy(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, out float rbsSeverity)
	{
		float pDt = pState.m_DeltaTime;

		//! collective
		{
			//! rotorSpeed^2
			//! so rotorSpeed=0.0, 0.0*0.0 = 0.0 | rotorSpeed=0.5, 0.5*0.5 = 0.25 | rotorSpeed=1.0, 1.0*1.0 = 1.0
			//! rotorSpeed is always clamped between 0.0-1.0

			float nearGround = 1;
			if (m_Hit)
			{
				nearGround = (pState.m_Transform[3][1] - m_HitPosition[1]) / pState.m_BoundingRadius;
				nearGround = Math.Clamp(1.5 - nearGround, 0.0, 1.0);
				nearGround = (nearGround * nearGround * 0.25) + 1.0;
			}

			float flightEnvelope[15];

			//! VRS
			float descentRate = -pState.m_LinearVelocityMS[1];  //! positive = descending

			ComputeVRSSeverity(descentRate, horiSpeed, nearGround, pDt);

			float vrsThrustMult = 1.0 - (m_VRSSeverity * m_VRSThrustLossMax);

			float rbsThrustMult = 1.0;

		#ifdef DIAG_DEVELOPER
			if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy_Extended)
				flightEnvelope = m_FlightEnvelope_ETL;
			else
		#endif
				flightEnvelope = m_FlightEnvelope;

			float liftFactor = 0;

			int iMaxFlightEnvelope = m_MaxFlightEnvelope - 1;
			float fHoriSpeedRel = horiSpeed / (pState.m_MaxSpeedMS * (iMaxFlightEnvelope / 10.0));
			float fCurrentEnvelope = iMaxFlightEnvelope * fHoriSpeedRel;
			int iCurrentEnvelopeFloor = Math.Floor(fCurrentEnvelope);

			if (iCurrentEnvelopeFloor >= iMaxFlightEnvelope)
				liftFactor = flightEnvelope[iMaxFlightEnvelope];
			else if (iCurrentEnvelopeFloor >= 0)
			{
				float fCurrentEnvelopeFloor = flightEnvelope[iCurrentEnvelopeFloor];
				float fCurrentEnvelopeFloorNext = flightEnvelope[iCurrentEnvelopeFloor + 1];
				liftFactor = Math.Lerp(fCurrentEnvelopeFloor, fCurrentEnvelopeFloorNext, fCurrentEnvelope - iCurrentEnvelopeFloor);
			}
			else
				liftFactor = flightEnvelope[0];

			float collective = -0.2 + m_Collective * 1.2;
			float targetVelocity = (pState.m_LinearVelocityMS[1] + 3.0) - (18.0 * collective * m_RotorSpeed * nearGround);
			if (targetVelocity < -5)
				targetVelocity = -5;

			targetVelocity *= pDt;
			float collectiveCoef = Math.Max((1.3 * liftFactor) - ((Math.SquareSign(targetVelocity) * 5.0) + (targetVelocity * 80.0)), 0);

			float totalThrust;

			if (m_VRSSeverity > 0 && descentRate > m_VRSDescentDeep)
				totalThrust = pState.m_Mass * Physics.STANDARD_GRAVITY * (descentRate / m_VRSDescentDeep) * vrsThrustMult;
			else
				totalThrust = collectiveCoef * vrsThrustMult * rbsThrustMult * pState.m_AltitudeLimiter * m_RotorSpeed * m_RotorSpeed * m_LiftForceCoef * pState.m_Mass;

			force += Vector(0, 1, 0) * totalThrust;
		}
	}

	void Simulate_Common(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, float perpendicular, float rbsSeverity)
	{
		float pDt = pState.m_DeltaTime;

		//! Translating Tendency: lateral drift in hover (tail rotor pushes heli right for CCW main rotor, left for CW)
		if (m_TranslatingTendency)
		{
			float translatingForce = m_TranslatingTendencyCoef * m_RotorSpeed * m_RotorSpeed * pState.m_Mass * pState.m_AltitudeLimiter;
			//! Full effect at 0-5 km/h, no effect at ETL
			translatingForce *= ExpansionMath.LinearConversion(1.388888, 8.0, Math.AbsFloat(pState.m_LinearVelocityMS[2]), 1.0, 0);
			force += Vector(1, 0, 0) * translatingForce;  //! +X in model space = right
		}

		float radiusScale = pState.m_BoundingRadius / REFERENCE_BOUNDING_RADIUS;

		//! Cyclic
		float cyclicForce = m_CyclicForceCoef * pState.m_Mass * m_RotorSpeed * m_RotorSpeed * pState.m_AltitudeLimiter;
		float cyclicForward;
		float cyclicSide;
		float cyclicPitchTorque;
		float cyclicRollTorque;	
	#ifdef DIAG_DEVELOPER
		if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
		{
			cyclicForward = m_CyclicForward;
			cyclicSide = m_CyclicSide;
			cyclicPitchTorque = cyclicForce * cyclicForward * m_CyclicForwardCoef * pState.m_BoundingRadius;
			cyclicRollTorque = cyclicForce * cyclicSide * m_CyclicSideCoef * pState.m_BoundingRadius;
		}
		else
	#endif
		{
			//! Use angular velocity target to calculate torque for cyclic (this is what makes the controls feel crisp)
			float pitchBlend;
			float rollBlend;
			float cyclicSideAbs = Math.AbsFloat(m_CyclicSide);
			float cyclicSideFrac = cyclicSideAbs / m_CyclicSideMax;
			float rollScaleMin;
			float rollScaleMax;

			if (!m_AutoTrim)
			{
				float cyclicForwardAbs = Math.AbsFloat(m_CyclicForward);
				float cyclicForwardFrac = cyclicForwardAbs / m_CyclicForwardMax;
				float pitchScale = Math.Min(0.8 * radiusScale, 1.0);
				rollScaleMin = Math.Min(0.3 * radiusScale, 1.0);
				rollScaleMax = Math.Min(0.5 * radiusScale, 1.0);
				pitchBlend = Math.Lerp(pitchScale, 1.0, cyclicForwardFrac);
				rollBlend = Math.Lerp(rollScaleMin, rollScaleMax, cyclicSideFrac);
			}
			else
			{
				pitchBlend = 1.0;
				rollBlend = Math.Min(0.5 * radiusScale, 1.0);
			}

		#ifdef DIAG_DEVELOPER
			if (!m_UseCyclicDiskMoments || m_SimulationMode != ExpansionHelicopterSimulationMode.RotorDisk)
			{
				cyclicForward = m_CyclicForward * Math.PI_HALF - pState.m_AngularVelocityMS[0] * pitchBlend;
				cyclicSide = m_CyclicSide * Math.PI_HALF - pState.m_AngularVelocityMS[2] * rollBlend;
				cyclicPitchTorque = cyclicForce * cyclicForward * m_CyclicForwardCoef * pState.m_BoundingRadius;
				cyclicRollTorque = cyclicForce * cyclicSide * m_CyclicSideCoef * pState.m_BoundingRadius;
			}
			else
		#endif
			{
				//! Apply cyclic disk moments with small gain (raw moments are very large)
				cyclicPitchTorque = (m_CyclicPitchMoment - m_InducedPitchMoment) * m_RotorDiskPitchMomentGain * radiusScale;
				cyclicRollTorque = (m_CyclicRollMoment - m_InducedRollMoment) * m_RotorDiskRollMomentGain * radiusScale;

				float maxCyclicTorque = cyclicForce * pState.m_BoundingRadius;
				cyclicPitchTorque -= pState.m_AngularVelocityMS[0] * maxCyclicTorque * m_CyclicForwardCoef * pitchBlend;
				cyclicRollTorque -= pState.m_AngularVelocityMS[2] * maxCyclicTorque * m_CyclicSideCoef * rollBlend;
			}
		}

		torque[0] = torque[0] - cyclicPitchTorque;
		torque[2] = torque[2] + cyclicRollTorque;

		//! Retreating Blade Stall - nose pitch-up and roll toward retreating (left for CCW, right for CW) side at high speed
		if (rbsSeverity > 0.0)
		{
			float rbsTorque = rbsSeverity * cyclicForce * pState.m_BoundingRadius;
			torque[0] = torque[0] - (rbsTorque * m_RetreatingBladeStallPitchCoef);   //! Nose pitch-up
			torque[2] = torque[2] + (rbsTorque * m_RetreatingBladeStallRollCoef * Math.Sign(m_TranslatingTendencyCoef));  //! Roll (retreating side down)
		}

		//! bank
		{
			const float tailRotateSqCoef = 0.00048;
			const float tailRotateCoef = 0.012;
			m_TailRotateFactor = (pState.m_LinearVelocityMS[2] * pState.m_LinearVelocityMS[2] * tailRotateSqCoef) + (Math.AbsFloat(pState.m_LinearVelocityMS[2]) * tailRotateCoef);

			float forwardX = pState.m_Transform[2][0];
			float sideY = pState.m_Transform[0][1];
			float forwardZ = pState.m_Transform[2][2];

			if (forwardX == 0.0 && forwardZ == 0.0)
			{
				m_Bank = Math.Sign(sideY);
			}
			else
			{
				m_Bank = sideY / ((forwardX * forwardX) + (forwardZ * forwardZ));
			}
		}

		float tailRotorForce;
		float tailForce;

		float yawVelocity = pState.m_AngularVelocityMS[1];

		//! tail rotor
		{
			float tailRotorMalfunction = 0.0;
			if (m_EnableTailRotorDamage)
			{
				tailRotorMalfunction = Math.Max(m_Tail.GetHealthLevel() - 1, 0) / 5.0;
			}

			float tailRotorMalfunctionNeg = 1.0 - tailRotorMalfunction;
			float tailRotorMalfunctionTorque = 0.5 * tailRotorMalfunction * m_RotorSpeed * (m_RotorSpeed + 0.1);

			tailRotorForce = m_AntiTorque * m_TailForceCoef * tailRotorMalfunctionNeg;
			tailRotorForce *= m_RotorSpeed * m_RotorSpeed;
		}

		//! @note when the heli has no tail rotor, there is no need to counteract main rotor torque (tandem or coaxial main rotor configuration)
		//! we can use m_TranslatingTendencyCoef to detect that (will be zero if no tail rotor)
		if (m_TranslatingTendencyCoef)
		{
			//! Torque from main rotor (CCW)
			float mainRotorTorqueTarget;

			if (!m_AutoTrim && !m_AutoHover)
				mainRotorTorqueTarget = Math.Max(m_Collective, 0.0);

			m_MainRotorTorque = Math.Lerp(m_MainRotorTorque, mainRotorTorqueTarget, pDt);
			float torqueSpeedFactor = 1.0 - Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.5), 0.0, 1.0);
			tailRotorForce -= m_TranslatingTendencyCoef * m_TranslatingTendencyCoef * m_MainRotorTorque * m_RotorSpeed * m_RotorSpeed * torqueSpeedFactor;
		}

		float bankForceCoef;

	#ifdef DIAG_DEVELOPER
		if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
			bankForceCoef = m_BankForceCoef;
		else
	#endif
		{
			if (pState.m_MaxSpeed > 120.0)
				bankForceCoef = Math.Lerp(0.05, 0.001 * radiusScale, horiSpeed / pState.m_MaxSpeedMS);
			else
				bankForceCoef = Math.Lerp(0.5, 0.1, horiSpeed / pState.m_MaxSpeedMS);
		}

		float bankForce = Math.Asin(m_Bank) * bankForceCoef * m_TailRotateFactor;

		//! tail aerodynamic forces
		{
			//! Directional Stability (Aerodynamic)
			//! This is the force of the wind hitting the vertical fin.
			float forwardVel = pState.m_LinearVelocityMS[2];
			float rightVel   = pState.m_LinearVelocityMS[0];
			float absForwardVel = Math.AbsFloat(forwardVel);
			float tailArmLength = pState.m_BoundingRadius * 0.85;
			float effectiveRightVel = rightVel + (yawVelocity * tailArmLength);

			//! Angle of attack
			float beta = Math.Atan2(effectiveRightVel, absForwardVel + 0.1);  //! +0.1 prevents division by zero

			float airDensity = 1.225;
			float finArea = 1.2;
			//! Lerp fin effectiveness with speed - near zero at hover so low-speed corrections feel smooth,
			//! full strength at cruise so heading tracks naturally. This is what gives A3 the feeling that
			//! fin resistance gradually releases as you slow down from an air brake.
			float speedBlend = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.25), 0.0, 1.0);
			float finEffectivenessCoef = Math.Lerp(0.12, 0.45, speedBlend);
			//finEffectivenessCoef *= Math.Min((REFERENCE_MASS * REFERENCE_BOUNDING_RADIUS) / (pState.m_Mass * pState.m_BoundingRadius), 1.0);

			float pedalAbs = Math.AbsFloat(m_AntiTorque);
			float pedalFrac = pedalAbs / m_AntiTorqueMax;
			//finEffectivenessCoef = Math.Lerp(1.0, finEffectivenessCoef, pedalFrac);
			float pedalActivity = Math.Clamp(pedalFrac * 2.0, 0.0, 1.0);
			float speedFactor = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.4), 0.0, 1.0);
			//! At high speed pedals suppress only 10% of fin - fin wins, enforcing the ~90 deg wall like A3
			//! At low speed pedals suppress up to 100% - free to pirouette in hover
			float maxSuppression = 1.0 - (speedFactor * 0.90);
			float activeFinCoef = finEffectivenessCoef * (1.0 - pedalActivity * maxSuppression);

			//! Rate-limit the fin force - keeps orbit corrections gradual at low speed.
			//! The ceiling is driven by ANGLE not speed - so approaching 90 deg keeps the wall strong
			//! even as the air brake scrubs speed. This is what makes the wall feel like it's
			//! actively resisting rather than fading away as you slow down.
			float angleWall = perpendicular * perpendicular;  //! 0 at forward, 1 at fully perpendicular
			float horiSpeedClamped = Math.Min(horiSpeed, pState.m_MaxSpeedMS);
			float speedBudget = Math.Clamp(horiSpeedClamped * horiSpeedClamped * 0.006 * pDt, 0.0, 1.2);
			float angleBudget = Math.Clamp(angleWall * 1.5 * pDt, 0.0, 1.2);
			float maxFinForce = Math.Max(speedBudget, angleBudget) * pState.m_Mass;
			float rawDirStabilityForce = -beta * horiSpeedClamped * airDensity * finArea * activeFinCoef * pDt;
			rawDirStabilityForce *= ExpansionMath.LinearConversion(10.0, 15.0, horiSpeed, 0.0, 1.0);  //! Only weathervane if above 36 km/h
			float dirStabilityForce = Math.Clamp(rawDirStabilityForce, -maxFinForce, maxFinForce);

			//! Preserve yaw speed "feel" of legacy simulation
			float tailRotorForceAdjusted = tailRotorForce * Math.Clamp(radiusScale, 1.0, 2.0);

			//! Use angular velocity target to calculate torque for antitorque (this is what makes the controls feel crisp)
			float antiTorqueYawTarget = (bankForce + tailRotorForceAdjusted - tailRotorMalfunctionTorque) * Math.PI;

			float yawScaleMax = Math.Min(0.8 * radiusScale, 1.0);
			float pedalBlend;

			if (!m_AutoTrim)
			{
				float yawScaleMin = Math.Min(0.5 * radiusScale, 1.0);
				pedalBlend = Math.Lerp(yawScaleMin, yawScaleMax, pedalFrac);
			}
			else
			{
				pedalBlend = Math.Lerp(yawScaleMax, 1.0, pedalFrac);
			}

			float yawBlend = Math.Lerp(pedalBlend, 1.0, speedFactor);

			float antiTorqueYawDiff = antiTorqueYawTarget + yawVelocity * yawBlend;

			float t = perpendicular * speedFactor;
		}

		float swingStrength;

	#ifdef DIAG_DEVELOPER
		if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
		{
			float maxSpeedTailEffect = 0.0125;  //! at ~200 km/h, tail rotor has 30% effect, at 288 km/h, it has no effect
			float tailEffectMin;
			float scaledSpeedFactor = 1.0 - Math.Min(Math.AbsFloat(pState.m_LinearVelocityMS[2] * maxSpeedTailEffect), 1.0 - tailEffectMin);

			tailForce = (bankForce + tailRotorForce - tailRotorMalfunctionTorque) * 0.5 * scaledSpeedFactor;

			antiTorqueYawDiff = ExpansionMath.LinearConversion(0, 0.25, t, tailForce, antiTorqueYawDiff);
		}
		else
	#endif
		{
			float maxSwingStrength = (2.75 - speedFactor) * (2.0 - t) * Math.Clamp(1.0 / radiusScale, 0.0, 1.0);
			swingStrength = ExpansionMath.LinearConversion(0.0, 1.0, pedalFrac, maxSwingStrength, 0.0);
			//swingStrength = (1.0 - pedalFrac);
		}

		{
			vector ori = m_Vehicle.GetOrientation();
			if (ori[1] < -70)
			{
				//! Oscillation protection when pitched down excessively
				float pitchFactor = ExpansionMath.LinearConversion(-70, -90, ori[1], 1, 0);
				t *= pitchFactor;
				swingStrength *= pitchFactor;
			}
			if (m_OverrideTailForce)
				t *= 0.2;
			tailForce = Math.Lerp(antiTorqueYawDiff + dirStabilityForce * swingStrength, dirStabilityForce + bankForce * Math.PI_HALF, t);
		}

		//! apply tail rotor + aerodynamic forces
		{
			tailForce *= pState.m_BoundingRadius * pState.m_Mass;

			//! apply torque to change the heading of the heli
			torque[1] = torque[1] - (pState.m_BoundingRadius * tailForce);

			//! apply a little bit of torque on the side of the heli to simulate some roll
			torque[2] = torque[2] - (pState.m_BoundingRadius * 0.01 * tailForce);
		}

		float airFrictionScale = 3.0;
		float airFrictionScaleInv = (airFrictionScale / (pDt * REFERENCE_MASS));

		//! Linear Friction
		{
			vector friction;

			vector frictionSpeed = pState.m_LinearVelocityMS;
			vector windSpeed = m_WindSpeedSync.InvMultiply3(pState.m_Transform);
			frictionSpeed -= windSpeed;

			float linearFrictionCoefX;
			float linearFrictionCoefY;
			float linearFrictionCoefZ;

			switch (m_AirFrictionMode)
			{
			#ifdef DIAG_DEVELOPER
				case ExpansionHelicopterSimulationAirFrictionMode.Legacy:
					//! @note because the legacy lateral friction value of 16.0 is extremely high relative to the forward value of 0.04,
					//! this can have the undesired side-effect of strongly affecting the direction of movement!
					//! I.e. when being in a forward hover motion with heli leveled and then applying yaw perpendicular
					//! to the movement direction, the direction of movement will change to the direction the heli is facing
					//! very rapidly (because the heli is "pushed" by the air resistance) which is *not* correct.

					//! if the helicopter is turned on then more force is applied to create pseudo-friction
					float stabilizeXY = 0.4 + (m_RotorSpeed * m_RotorSpeed * 0.6);

					//linearFrictionCoefX = m_LinearFrictionCoef[0] * stabilizeXY;  //! <-- original legacy lateral friction
					linearFrictionCoefX = m_LinearFrictionCoef[2] * stabilizeXY * (m_AirFriction[0] / m_AirFriction[2]);
					linearFrictionCoefY = m_LinearFrictionCoef[1] * stabilizeXY;
					linearFrictionCoefZ = m_LinearFrictionCoef[2];
					break;
			#endif

				case ExpansionHelicopterSimulationAirFrictionMode.Balanced:
				default:
					linearFrictionCoefX = m_AirFriction[0] * airFrictionScaleInv;
					linearFrictionCoefY = m_AirFriction[1] * airFrictionScaleInv;
					linearFrictionCoefZ = m_AirFriction[2] * airFrictionScaleInv;
					break;
			}

			friction[0] = Math.SquareSign(frictionSpeed[0]) * pDt * linearFrictionCoefX * pState.m_Mass;
			friction[1] = Math.SquareSign(frictionSpeed[1]) * pDt * linearFrictionCoefY * pState.m_Mass;
			friction[2] = Math.SquareSign(frictionSpeed[2]) * pDt * linearFrictionCoefZ * pState.m_Mass;

			//! Retreating Blade Stall - extra parasite drag from stalled blade (form drag opposes forward motion)
			if (rbsSeverity > 0.0)
			{
				float rbsDragMult = 1.0 + (rbsSeverity * m_RetreatingBladeStallDragCoef);
				friction[2] = friction[2] * rbsDragMult;  //! Forward axis - main parasite drag from stalled retreating blade
			}

			force -= friction * m_BodyFrictionCoef;
		}

		ApplyHoverInstability(pState, force, torque, horiSpeed);

		//! convert forces to worldspace
		{
			force = force.Multiply3(pState.m_Transform);
			torque = torque.Multiply3(pState.m_Transform);
		}

		//! Tail rotor lateral push - applied in worldspace using heli's right axis flattened to horizontal
		//! so pitch attitude never bleeds into vertical. Creates curved flight path toward nose when holding yaw at speed (A3 behaviour)
	#ifdef DIAG_DEVELOPER
		if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
	#endif
		{
			float tailLateralCoef = 0.004;
			float tailLateralForce = -(m_AntiTorque * m_TailForceCoef * tailLateralCoef * m_RotorSpeed * pState.m_Mass);
			//! Use heli right axis (transform column 0) but zero out Y so it's always horizontal
			vector rightAxis = Vector(pState.m_Transform[0][0], 0, pState.m_Transform[0][2]);
			float rightLen = rightAxis.Length();
			if (rightLen > 0.001)
				rightAxis = rightAxis * (1.0 / rightLen);
			force = force + rightAxis * tailLateralForce;
		}

		//! Sideslip drag applied in worldspace - purely horizontal so pitched attitude can't bleed into vertical
		//! This is what slows you down when presenting the heli's side to the airflow (pedal air-braking)
	#ifdef DIAG_DEVELOPER
		if (m_AirFrictionMode != ExpansionHelicopterSimulationAirFrictionMode.Legacy)
	#endif
		{
			vector horiVelWS = Vector(pState.m_LinearVelocity[0], 0, pState.m_LinearVelocity[2]);
			float horiSpeedWSSq = horiVelWS.LengthSq();
			if (horiSpeedWSSq > 0.01)
			{
				float sideslipDragMult = perpendicular * perpendicular * 4.5;
				float sdForce = horiSpeedWSSq * sideslipDragMult * pDt * m_AirFriction[0] * airFrictionScaleInv * pState.m_Mass * m_BodyFrictionCoef;
				force -= horiVelWS.Normalized() * sdForce;
			}
		}

		//! Angular Friction
		{
			vector t_friction;

			float tailRotateFactor = m_TailRotateFactor;

		#ifdef DIAG_DEVELOPER
			if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
				tailRotateFactor *= 0.5;
			else
		#endif
				tailRotateFactor *= 0.15;

			t_friction = pState.m_AngularVelocity * pState.m_Mass * (m_RotorSpeed + 0.2) * (m_AngularFrictionCoef + tailRotateFactor);

			torque -= t_friction;
		}
	}

	void PreventSinkingInOcean(ExpansionPhysicsState pState, inout vector force, inout vector torque)
	{
		bool isAboveWater;
		float buoyancyForce = ExpansionPhysics.CalculateBuoyancyAtPosition(pState.m_Transform[3], 2.0, pState.m_Mass, 2.0, pState.m_LinearVelocity, isAboveWater);

		if (!isAboveWater && IsMissionHost())
		{
			float buoyancyAcceleration = buoyancyForce * pState.m_InvMass;
			if (buoyancyAcceleration > 1.0)
			{
				CarScript car;
				ExpansionVehicleBase vehicle;
				bool canBeDamaged;
				if (Class.CastTo(car, m_Vehicle))
					canBeDamaged = car.CanBeDamaged();
				else if (Class.CastTo(vehicle, m_Vehicle))
					canBeDamaged = vehicle.CanBeDamaged();

				if (canBeDamaged)
					m_Vehicle.AddHealth("", "", -0.001 * buoyancyForce);
			}
		}

		force += Vector(0, buoyancyForce, 0);
	}

	//! Rotor-disk / blade-element model (A3 RotorLib-style)
	void Simulate_RotorDisk(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, float perpendicular, out float rbsSeverity)
	{
		float pDt = pState.m_DeltaTime;

		float vh = pState.m_LinearVelocityMS[2];   //! Forward velocity (model +Z)
		float vx = pState.m_LinearVelocityMS[0];   //! Lateral velocity
		float vv = pState.m_LinearVelocityMS[1];   //! Vertical (up positive)

		float omegaR = m_MainRotorTipSpeed * m_RotorSpeed;  //! Tip speed [m/s]
		float rotorAuthority = m_RotorSpeed * m_RotorSpeed; //! Powered rotor authority [0..1]
		float mu = 0.0;
		if (omegaR > 1.0)
			mu = horiSpeed / omegaR;  //! Advance ratio

		//! Ground effect
		float groundEffect = 1.0;
		if (m_Hit)
		{
			float heightAboveGround = pState.m_Transform[3][1] - m_HitPosition[1];
			float groundEffectZone = pState.m_BoundingRadius * m_GroundEffectRadius;
			if (heightAboveGround < groundEffectZone && heightAboveGround > 0)
			{
				float groundProximity = 1.0 - (heightAboveGround / groundEffectZone);
				groundEffect = 1.0 + (groundProximity * groundProximity * (m_GroundEffectMax - 1.0));
			}
		}

		 //! Ideal induced velocity for hover, from momentum theory: vi = sqrt(T / (2 * rho * A))
		 //! T = weight = mass * gravity. A = rotor disk area = pi * r^2. rho = air density.
		float viHover = Math.Sqrt((REFERENCE_MASS * Physics.STANDARD_GRAVITY) / (2.0 * 1.225 * Math.PI * m_MainRotorRadius * m_MainRotorRadius));
		float vi = viHover * rotorAuthority;

		//! Correction factor for ETL to match A3
		//! Nose pitched down 5° -> ~180 km/h top speed, ~1 m/s ascent at 50% collective (MH-6)
		//! Adjustment region 12.34666 m/s (44.448 km/h or 24 knots) to 45 m/s (162 km/h)
		float horiSpeedCorr = ExpansionMath.LinearConversion(12.346666, 45, horiSpeed, horiSpeed, horiSpeed * 0.41);

		//! Effective translational lift, from momentum theory, reduces induced velocity
		for (int j = 0; j < 8; ++j)
		{
			float denom = Math.Sqrt(horiSpeedCorr * horiSpeedCorr + vi * vi);
			vi = (viHover * viHover) / denom;
		}

		vi = vi / groundEffect;  //! Ground effect reduces induced velocity

		//! VRS
		float descentRate = -vv;

		ComputeVRSSeverity(descentRate, horiSpeed, groundEffect, pDt);

		float vrsThrustLoss = m_VRSSeverity * m_VRSThrustLossMax;
		float vrsMult = 1.0 - vrsThrustLoss;

		vi *= 1.0 + vrsThrustLoss;  //! VRS increases induced velocity

		//! Collective -> blade pitch: m_Collective 0 = zero thrust, 1 = full
		float theta0 = GetTheta0(m_Collective, m_LiftForceCoef);
		theta0 *= vrsMult;
		float theta1s = m_CyclicForward * m_CyclicForwardCoef * 0.096;   //! Longitudinal cyclic
		float theta1c = m_CyclicSide * m_CyclicSideCoef * 0.127;     //! Lateral cyclic

		//! Blade-element sampling: 8 azimuth sectors. cos/sin lookup [0..7]
		float azCos[8] = { 1.0, 0.707, 0.0, -0.707, -1.0, -0.707, 0.0, 0.707 };
		float azSin[8] = { 0.0, 0.707, 1.0, 0.707, 0.0, -0.707, -1.0, -0.707 };

		float dpsi = 0.785398;  //! 2*PI/8
		float rho = 1.225 * pState.m_AltitudeLimiter;

		float totalThrust = 0.0;
		m_InducedPitchMoment = 0.0;
		m_InducedRollMoment = 0.0;
		m_CyclicPitchMoment = 0.0;
		m_CyclicRollMoment = 0.0;

		if (m_RBS)
		{
			//! Our helis are slower in game than the real-world counterparts, so we have to adjust for this
			float speedCorr = 1.174090;  //! maxSafeAdvanceRatio / (VNE / tipSpeed) = 0.35 / (61.111111 / 205)
			rbsSeverity = (mu - 0.35) / 0.10;
		}

		for (int i = 0; i < 8; ++i)
		{
			float cpsi = azCos[i];
			float spsi = azSin[i];
			float ut = omegaR + vh * spsi + vx * cpsi;  //! Tangential velocity
			float up = vv + vi;                          //! Perpendicular through disk

			float u = Math.Sqrt(ut * ut + up * up);

			if (u < 0.5)
				u = 0.5;
			float utSafe;
			if (Math.AbsFloat(ut) < 1.0)
			{
				if (ut >= 0)
					utSafe = 1.0;
				else
					utSafe = -1.0;
			}
			else
			{
				utSafe = ut;
			}
			float phi = up / utSafe;  //! Inflow angle

			bool isRetreating = ut < omegaR;

			//! Approximate flapping effect
			float muLocal = 0.0;
			if (omegaR > 1.0)
				muLocal = vx / omegaR;

			float flapAngle = 0.0;

			if (isRetreating) //! retreating side
				flapAngle = 0.10 * Math.Clamp(muLocal, 0.0, 0.45) * -cpsi;

			float alpha = theta0 - phi + flapAngle;
			float cl = m_BladeLiftSlope * alpha;

			//! Retreating blade stall: low UT (retreating side) -> high alpha -> stall
			float stallFactor = 0;
			if (m_RBS && alpha > m_BladeStallAngle && isRetreating)
			{
				stallFactor = Math.Clamp((alpha - m_BladeStallAngle) / 0.25, 0.0, 1.0);
				cl = m_BladeLiftSlope * m_BladeStallAngle * (1.0 - stallFactor * 0.8);
			}
			else if (alpha < -0.1)
			{
				cl = cl * 0.5;
			}

			float dL = 0.5 * rho * m_MainRotorChord * (m_MainRotorRadius * 0.7) * cl * u * u * dpsi * rotorAuthority;

		#ifdef DIAG_DEVELOPER
			if (!m_AutoTrim || m_UseCyclicDiskMoments)
		#endif
			{
				m_InducedPitchMoment = m_InducedPitchMoment + (dL * spsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
				m_InducedRollMoment = m_InducedRollMoment + (dL * cpsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
			}

		#ifdef DIAG_DEVELOPER
			if (m_UseCyclicDiskMoments)
		#endif
			{
				float theta = theta0 + theta1s * spsi + theta1c * cpsi;
				alpha = theta - phi + flapAngle;
				cl = m_BladeLiftSlope * alpha;
				if (m_RBS && alpha > m_BladeStallAngle && isRetreating)
				{
					stallFactor = Math.Clamp((alpha - m_BladeStallAngle) / 0.25, 0.0, 1.0);
					cl = m_BladeLiftSlope * m_BladeStallAngle * (1.0 - stallFactor * 0.8);
				}
				else if (alpha < -0.1)
				{
					cl = cl * 0.5;
				}

				dL = 0.5 * rho * m_MainRotorChord * (m_MainRotorRadius * 0.7) * cl * u * u * dpsi * rotorAuthority;

				m_CyclicPitchMoment = m_CyclicPitchMoment + (dL * spsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
				m_CyclicRollMoment = m_CyclicRollMoment + (dL * cpsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
			}

			totalThrust += dL * m_MainRotorBlades;

			rbsSeverity += stallFactor;

		#ifdef DIAG_DEVELOPER
			if (stallFactor > 0.0)
				PrintFormat("%1 stallFactor %2 cl %3 ut %4 omegaR %5 muLocal %6 alpha %7 flapAngle %8", i, stallFactor, cl, ut, omegaR, muLocal, theta0 - phi, flapAngle);
		#endif
		}

		rbsSeverity = Math.Clamp(rbsSeverity, 0.0, 1.0);

		m_RBSSeverity = rbsSeverity;

		totalThrust *= m_RotorDiskThrustScale;
		totalThrust *= (1.0 - rbsSeverity * m_RetreatingBladeStallLiftLoss);

		//! Sideslip rotor efficiency loss - high perpendicular airflow disturbs disk inflow,
		//! reducing effective thrust. Scales with both sideslip angle and speed so it has
		//! no effect in hover (speed=0) but progressively counters the climb during air braking.
		//! This matches A3 behaviour where you do gain altitude but slowly, not a skyrocket.
		float speedFactor = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.5), 0.0, 1.0);
		float sideslipEfficiencyLoss = perpendicular * perpendicular * speedFactor * 0.35;
		totalThrust *= (1.0 - sideslipEfficiencyLoss);

		float massScale = pState.m_Mass / REFERENCE_MASS;

		//! Clamp thrust to negative mass
		//! (prevents wheels of large, heavy helis like the Merlin getting pushed through ground)
		if (totalThrust < -REFERENCE_MASS)
			totalThrust = -pState.m_Mass;
		else
			totalThrust *= massScale;  //! Scale by reference mass (1.0 = MH6)

		//if (m_VRSSeverity > 0 && descentRate > m_VRSDescentDeep)
			//totalThrust = pState.m_Mass * Physics.STANDARD_GRAVITY * (descentRate / m_VRSDescentDeep) * vrsMult;

		vector ori = m_Helicopter.GetOrientation();

		//! Thrust along tilted disk normal (disk leads, fuselage hangs below)
		float cyclicForwardThrustTarget = (m_CyclicForward - m_RotorDiskForwardInertia) / m_CyclicForwardMax;
		float cyclicForwardThrustTargetAbs = Math.AbsFloat(cyclicForwardThrustTarget) / m_CyclicSideMax;
		float cyclicSideThrustTarget = (m_CyclicSide - m_RotorDiskSideInertia);
		float cyclicSideThrustTargetAbs = Math.AbsFloat(cyclicSideThrustTarget);

		float accelMaxSpeed = 50;  //! m/s (~180 km/h)
		float accelTargetSpeed = Math.Clamp(vh, 0, accelMaxSpeed * (Math.Min(pState.m_MaxSpeed, 220.0) / 220.0));
		//! 0 to 100 km/h in ~36 s at 5° fwd pitch like A3 MH-6
		float pitchFactor = ExpansionMath.LinearConversion(0, -5, ori[1], 0.0, 1.0);
		float fwdThrustFactor = Math.Pow(accelTargetSpeed / accelMaxSpeed, 1.5) * pitchFactor;
		//PrintFormat("accel %1 pitch %2° pitchFactor %3 fwdThrustFactor %4", pState.m_LinearAccelerationMS[2], ori[1], pitchFactor, fwdThrustFactor);
		float collectiveSideThrustMoment;

		float liftComp = 1;
		float rotorRotationDirection = Math.Sign(m_TranslatingTendencyCoef);  //! Main rotor 1 = CCW -1 = CW

		//! Rotor disk dynamic lag/inertia and cross-axis coupling
	#ifdef DIAG_DEVELOPER
		if (m_RotorDiskInertia)
	#endif
		{
			//! "Swing" and resistance effect like in A3 (both fade away quickly while there's input and gets inverted when input is released)
			float swingForceScale = Math.Min(pState.m_BoundingRadius / REFERENCE_BOUNDING_RADIUS, 1.0);
			float swingForce = rotorAuthority * pState.m_Mass * pState.m_BoundingRadius * 0.5 * swingForceScale * pState.m_AltitudeLimiter;
			float resistancePitch;
			float resistanceRoll;
			float yawSwing;
			float rollSwing;

			if (cyclicForwardThrustTarget < 0)  //! Pitch up (cyclic bwd), resistance up
				resistancePitch = Math.Lerp(0, swingForce, cyclicForwardThrustTargetAbs);
			else  //! Pitch down (cyclic fwd), resistance down
				resistancePitch = Math.Lerp(0, -swingForce, cyclicForwardThrustTargetAbs);

			//! Swing in opposite direction of main rotor rotation
			if (m_CyclicForward < 0)
				yawSwing += resistancePitch * rotorRotationDirection;
			else
				yawSwing -= resistancePitch * rotorRotationDirection;

			float collectiveSwingFactor;

			if (m_CollectiveDelta > 0.0 || (m_CollectiveDelta == 0.0 && m_CollectiveDeltaInterpolated > 0.0))
				collectiveSwingFactor = 2.0;
			else
				collectiveSwingFactor = 1.0;

			float collectiveSpeedFactor = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.7), 0.5, 1.0);

			//! Collective up = yaw opposite to rotor rotation dir, collective down = yaw in rotor rotation dir
			float collectiveYawSwing = m_CollectiveDeltaInterpolated * swingForce * rotorRotationDirection;
			yawSwing += collectiveYawSwing * collectiveSwingFactor * 5.0 * collectiveSpeedFactor;

			//! Collective up = roll in rotor rotation dir, collective down = roll opposite to rotor rotation dir
			rollSwing += m_CollectiveDelta * swingForce * rotorRotationDirection * collectiveSwingFactor * 0.125 * (1.2 - collectiveSpeedFactor);

			//! Collective up = pitch up, collective down = pitch down
			float pitchSwing = m_CollectiveDeltaInterpolated * swingForce * collectiveSpeedFactor;

			collectiveSideThrustMoment = m_CollectiveDeltaInterpolated * rotorRotationDirection * 0.1 * collectiveSpeedFactor;

			rollSwing -= resistancePitch * 0.2;

			if (cyclicSideThrustTarget < 0)  //! Roll left (cyclic left), resistance left
				resistanceRoll = Math.Lerp(0, swingForce, cyclicSideThrustTargetAbs);
			else  //! Roll right (cyclic right), resistance right
				resistanceRoll = Math.Lerp(0, -swingForce, cyclicSideThrustTargetAbs);

			yawSwing += resistanceRoll * 2;

			torque[0] = torque[0] + resistancePitch * 2 - pitchSwing;
			torque[1] = torque[1] + yawSwing;
			torque[2] = torque[2] + resistanceRoll + rollSwing;
		}

		m_ThrustDir[0] = -cyclicSideThrustTarget * m_ThrustTiltCoef + collectiveSideThrustMoment;
		m_ThrustDir[1] = 1.0;
		m_ThrustDir[2] = (cyclicForwardThrustTarget + fwdThrustFactor) * m_ThrustTiltCoef;
		m_ThrustDir.Normalize();

		vector thrustForce = m_ThrustDir * totalThrust * liftComp;

		force += thrustForce;

		m_RotorDiskForwardInertia = Math.SmoothCD(m_RotorDiskForwardInertia, m_CyclicForward, m_RotorDiskForwardInertiaVel, 0.2, 1000, pDt);
		m_RotorDiskSideInertia = Math.SmoothCD(m_RotorDiskSideInertia, m_CyclicSide, m_RotorDiskSideInertiaVel, 0.2, 1000, pDt);

		m_InducedPitchMoment *= massScale;
		m_InducedRollMoment *= massScale;
		m_CyclicPitchMoment *= massScale;
		m_CyclicRollMoment *= massScale;

		//! Apply induced disk moments with small gain (raw moments are very large)
		if (!m_AutoTrim)
		{
			float rollAngle = ori[2];
			float rollFactor = ExpansionMath.LinearConversion(0, 30 * rotorRotationDirection, rollAngle, 1.0, 0.0);

			torque[0] = torque[0] - (m_InducedPitchMoment * m_RotorDiskPitchMomentGain);

			float pedalAbs = Math.AbsFloat(m_AntiTorque);
			float pedalFrac = pedalAbs / m_AntiTorqueMax;
			torque[2] = torque[2] + (m_InducedRollMoment * m_RotorDiskRollMomentGain * pedalFrac);

			torque[2] = torque[2] - (Math.AbsFloat(m_InducedPitchMoment) * 0.1 * m_RotorDiskRollMomentGain * rollFactor * rotorRotationDirection);
		}
	}

	void ComputeVRSSeverity(float descentRate, float horiSpeed, float groundEffect, float pDt)
	{
		float vrsSeverity;

		if (m_RotorSpeedTarget > 0 && descentRate > m_VRSDescentThreshold && horiSpeed < m_VRSAirspeedThreshold && m_RotorSpeed > 0.3 && (m_VRSSeverity > 0 || (groundEffect == 1.0 && descentRate < m_VRSDescentDeep + (m_VRSDescentDeep - m_VRSDescentThreshold))))
		{
			if (m_Collective > 0)
			{
				vrsSeverity = Math.Clamp((descentRate - m_VRSDescentThreshold) / (m_VRSDescentDeep - m_VRSDescentThreshold), 0.0, 1.0);
				vrsSeverity *= 1.0 - (horiSpeed / m_VRSAirspeedThreshold);  //! Less VRS at higher airspeed
				//vrsSeverity *= 1.0 - Math.Min(Math.AbsFloat(vx) / (pState.m_BoundingRadius * 0.6125), 1.0);  //! Less VRS when moving laterally towards vortex upwind

				if (m_Collective <= 0.5)
					vrsSeverity *= Math.Lerp(1.0, 0.75 / m_VRSThrustLossMax, m_Collective);  //! higher collective = worse VRS (up to 36%)
				else
					vrsSeverity *= Math.Lerp(0.75 / m_VRSThrustLossMax, 0.99 / m_VRSThrustLossMax, m_Collective);  //! higher collective = worse VRS (up to 80%)
			}
			else
			{
				//! If no or negative collective, full VRS reaches zero in 2 s
				vrsSeverity = Math.Max(vrsSeverity - pDt * 0.5, 0);
			}
		}

		m_VRSSeverity = vrsSeverity;
	}

	//! Given collective input in interval [0,1], return pitch angle theta0
	float GetTheta0(float collective, float liftForceCoef)
	{
		float theta0 = m_CollectivePitchAtMin;
		float collectivePitchRange = m_CollectivePitchAtFull - m_CollectivePitchAtMin;
		float collectiveTransition = ExpansionMath.LinearConversion(1.0, 1.3, liftForceCoef, 0.0, 0.5);

		if (collective <= collectiveTransition)
		{
			theta0 += collective * collectivePitchRange;
		}
		else
		{
			float collectiveRemaining = 1.0 - collectiveTransition;
			float t = (collective - collectiveTransition) / collectiveRemaining;
			float scaleFactor;

			if (liftForceCoef < 1.1)
				scaleFactor = ExpansionMath.LinearConversion(0.4, 1.1, liftForceCoef, -1.9, -1.2);
			else
				scaleFactor = ExpansionMath.LinearConversion(1.1, 1.3, liftForceCoef, -1.2, 1.0);

			t = (0.155 * scaleFactor * t * t) + (collectiveRemaining * t) + collectiveTransition;
			theta0 += t * collectivePitchRange;  //! Gradually ramp up to max ascent rate to match A3 (20 m/s for MH-6)
		}

		return theta0;
	}

	//! Given collective pitch angle theta0, return collective input in interval [0,1]
	float GetCollectiveInput(float theta0, float liftForceCoef)
	{
		float collectivePitchRange = m_CollectivePitchAtFull - m_CollectivePitchAtMin;
		float collectiveTransition = ExpansionMath.LinearConversion(1.0, 1.3, liftForceCoef, 0.0, 0.5);
		float collectivePitchRel = (theta0 - m_CollectivePitchAtMin) / collectivePitchRange;

		if (theta0 <= m_CollectivePitchAtMin + collectiveTransition * collectivePitchRange)
			return collectivePitchRel;  //! Linear segment maps directly to collective input

		float collectiveRemaining = 1.0 - collectiveTransition;
		float scaleFactor;
		if (liftForceCoef < 1.1)
			scaleFactor = ExpansionMath.LinearConversion(0.4, 1.1, liftForceCoef, -1.9, -1.2);
		else
			scaleFactor = ExpansionMath.LinearConversion(1.1, 1.3, liftForceCoef, -1.2, 1.0);

		//! Solve: collectivePitchRel = (0.155 * scaleFactor * t * t) + (collectiveRemaining * t) + collectiveTransition
		float a = 0.155 * scaleFactor;  //! Quadratic coefficient
		float b = collectiveRemaining;  //! Linear coefficient
		float c = collectiveTransition - collectivePitchRel;  //! Constant term
		float t;
		
		if (Math.AbsFloat(a) < 1e-8)
		{
			t = -c / b;
		}
		else
		{
			float disc = b * b - 4 * a * c;  //! Discriminant

			//! Safeguard against tiny floating-point errors (e.g. -1e-15 instead of 0)
			disc = Math.Max(disc, 0.0);

			float discSqrt = Math.Sqrt(disc);

			float t1 = (-b + discSqrt) / (2 * a);
			float t2 = (-b - discSqrt) / (2 * a);

			if (t1 >= -1e-12 && t1 <= 1.0 + 1e-12)
				t = t1;
			else
				t = t2;
		}

		return collectiveTransition + Math.Clamp(t, 0.0, 1.0) * collectiveRemaining;
	}

	void ApplyHoverInstability(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed)
	{
		//! Stabilize automatically as forward speed increases
		float speedThresh = 11.111111;  //! 40 km/h

		if (horiSpeed > speedThresh)
			return;

		//! Ground effect
		//! Drastically amplifies the wobble when hovering within ground effect
		float groundEffectMultiplier = 1.0;
		if (m_Hit)
		{
			float heightAboveGround = pState.m_Transform[3][1] - m_HitPosition[1];
			float groundEffectZone = pState.m_BoundingRadius * m_GroundEffectRadius;
			float groundEffectRadiusMin = m_GroundEffectRadius - 1.0;

			groundEffectMultiplier = Math.Lerp(2.5, 1.0, (Math.Clamp(heightAboveGround, 2.0, groundEffectZone) - 2.0) / (groundEffectZone - 2.0));

			//! Pull to zero when very close to ground (i.e. landed)
			if (heightAboveGround < 0.1)
				groundEffectMultiplier *= Math.InverseLerp(0.0, 0.1, heightAboveGround);
		}

		float wobbleFrequencyScale = m_WobbleFrequencyScale;
		float wobbleIntensityScale = m_WobbleIntensityScale;

		//! More violent shaking in VRS
		if (m_VRSSeverity > 0)
		{
			wobbleIntensityScale *= Math.Lerp(1.0, 3.0, m_VRSSeverity);
		}

		m_HoverTimeTracker += pState.m_DeltaTime * wobbleFrequencyScale;

		float time = m_HoverTimeTracker;

		float wavePitch = (Math.Sin(time * 1.2) * 0.6) + (Math.Cos(time * 2.7) * 0.3) + (Math.Sin(time * 0.4) * 0.1);
		float waveRoll  = (Math.Cos(time * 1.5) * 0.5) + (Math.Sin(time * 3.1) * 0.3) + (Math.Cos(time * 0.5) * 0.2);
		float waveYaw   = (Math.Sin(time * 0.8) * 0.4) + (Math.Cos(time * 1.9) * 0.2);  //! Yaw usually drifts slower

		float speedFactor = Math.InverseLerp(speedThresh, 0.0, horiSpeed);
		float forceMultiplier = wobbleIntensityScale * pState.m_Mass * speedFactor * groundEffectMultiplier * m_RotorSpeed * m_RotorSpeed;

		if (m_AutoHover)
			forceMultiplier *= 0.2;
		
		torque[0] = torque[0] + wavePitch * forceMultiplier * 0.7;
		torque[2] = torque[2] + waveRoll * forceMultiplier * 1.0;
		torque[1] = torque[1] + waveYaw * forceMultiplier * 0.3;

		//! Subtle lift variations
		float noiseLift = ExpansionPerlinNoiseGenerator.GetValue(time + 300.0) - 0.5;

		if (m_VRSSeverity == 0.0)
			force[1] = force[1] + noiseLift * forceMultiplier;

		//PrintFormat("%1 %2 %3 %4", wavePitch, waveRoll, waveYaw, noiseLift);
	}

	override void Animate(ExpansionPhysicsState pState, float deltaTime)
	{
		if (!m_Initialized)
			return;

		if (m_RotorSpeed > 0)
			m_RotorAnimationPosition += m_RotorSpeed * deltaTime * 20.0 / (Math.PI * 2.0);

		AnimateRotors();

		m_Vehicle.SetAnimationPhase("compasspointer", m_Vehicle.GetOrientation()[0] * Math.DEG2RAD);
		m_Vehicle.SetAnimationPhase("attitudeDiveRTD", m_Vehicle.GetDirection()[1]);
		m_Vehicle.SetAnimationPhase("attitudeBankRTD", m_Vehicle.GetOrientation()[2] / 360);

		float change;

		if (m_CollectiveInterpolated < m_CollectiveTarget)
			change = deltaTime;
		else if (m_CollectiveInterpolated > m_CollectiveTarget)
			change = -deltaTime;

		float absDiff = Math.AbsFloat(m_CollectiveTarget - m_CollectiveInterpolated);
		change = Math.Clamp(change, -absDiff, absDiff);
		m_CollectiveInterpolated += change;

		m_Vehicle.SetAnimationPhase("CollectiveRTD", m_CollectiveInterpolated);
		m_Vehicle.SetAnimationPhase("rudderRTD", m_AntiTorque / m_AntiTorqueMax);

		//! Particles, only client-side
		if (!IsMissionClient())
			return;

		if (m_Hit && m_RotorSpeed > 0)
		{
			float size = (1 - m_HitFraction) * m_RotorSpeed * 4.0 * pState.m_BoundingRadius;

			if (ExpansionStatic.SurfaceIsWater(m_HitPosition))
			{
				m_HitPosition[1] = g_Game.SurfaceGetSeaLevel();

				if (!m_WaterParticle)
				{
					if (size > 10 || !pState.m_Exploded)
					{
						m_WaterParticle = Particle.CreateInWorld(ParticleList.EXPANSION_HELICOPTER_WATER, m_HitPosition);
					}

					if (m_WaterParticle)
					{
						m_WaterParticle.PlayParticle();
					}
				}
				else
				{
					if (m_WaterParticle)
					{
						m_WaterParticle.SetPosition(m_HitPosition);
					}
				}

				if (m_DustParticle)
				{
					m_DustParticle.Stop();
				}
			}
			else
			{
				if (!m_DustParticle)
				{
					if (size > 10 || !pState.m_Exploded)
					{
						m_DustParticle = Particle.CreateInWorld(ParticleList.EXPANSION_HELICOPTER_GROUND, m_HitPosition);
					}

					if (m_DustParticle)
					{
						m_DustParticle.PlayParticle();
					}
				}
				else
				{
					if (m_DustParticle)
					{
						m_DustParticle.SetPosition(m_HitPosition);
					}
				}

				if (m_WaterParticle)
				{
					m_WaterParticle.Stop();
				}
			}

			float rotorRadius = pState.m_BoundingRadius * 0.6125;
			float maxEffectHeight = rotorRadius * 1.5;
			float altitude = pState.m_Transform[3][1];
			float heightAGL = altitude - m_HitPosition[1]; 

			if (heightAGL < maxEffectHeight)
			{
				float heightFactor = heightAGL / maxEffectHeight;
				float intensity = (1.0 - (heightFactor * heightFactor)) * m_RotorSpeed;
				float radius = rotorRadius * (1.35 + (heightFactor * 0.8));

				g_Game.GetWorld().FlattenGrassSphere(m_HitPosition[0], m_HitPosition[2], radius, 0, 1, intensity);
			}
		}
		else
		{
			if (m_DustParticle)
			{
				m_DustParticle.Stop();
			}

			if (m_WaterParticle)
			{
				m_WaterParticle.Stop();
			}
		}

		if (size < 10 || pState.m_Exploded)
		{
			if (m_DustParticle)
			{
				m_DustParticle.Stop();
			}

			if (m_WaterParticle)
			{
				m_WaterParticle.Stop();
			}
		}

	#ifdef DIAG_DEVELOPER
		vector rotorHubOffset = m_Helicopter.GetMemoryPointPos("rotor");
		vector origin = rotorHubOffset.Multiply4(pState.m_Transform);
		vector thrustDirWS = m_ThrustDir.Multiply3(pState.m_Transform);
		Debug.DrawArrow(origin, origin + pState.m_Transform[1] * 2, 0.5, COLOR_BLUE, ShapeFlags.ONCE | ShapeFlags.NOZBUFFER);
		Debug.DrawArrow(origin, origin + thrustDirWS * 2, 0.5, COLOR_GREEN, ShapeFlags.ONCE | ShapeFlags.NOZBUFFER);
	#endif
	}

	void AnimateCyclic()
	{
		float cyclicForwardInput = Math.Clamp(m_CyclicForwardInput, -1.0, 1.0);
		float cyclicSideInput = Math.Clamp(m_CyclicSideInput, -1.0, 1.0);

		m_Vehicle.SetAnimationPhase("cyclicForward", cyclicForwardInput);
		m_Vehicle.SetAnimationPhase("cyclicAside", -cyclicSideInput);
	}

	void AnimateRotors()
	{
		if (m_RotorSpeed > 0.7)
		{
			m_Vehicle.ShowSelection("hiderotorblur");
			m_Vehicle.HideSelection("hiderotor");
		}
		else
		{
			m_Vehicle.ShowSelection("hiderotor");
			m_Vehicle.HideSelection("hiderotorblur");
		}

		if (m_RotorAnimationPosition >= 1)
			m_RotorAnimationPosition -= 1;

		m_Vehicle.SetAnimationPhase("rotor", m_RotorAnimationPosition);
		m_Vehicle.SetAnimationPhase("rearrotor", m_RotorAnimationPosition);
	}

	void Message(string msg)
	{
		msg = string.Format("%1 %2", m_Helicopter.GetType(), msg);
	#ifdef SERVER
		ExpansionStatic.MessageNearPlayers(m_Helicopter.GetPosition(), m_Helicopter.m_State.m_BoundingRadius * 1.5 + 15, msg);
	#else
		//! @note g_Game.Chat doesn't work in offline/SP
		g_Game.GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(ChatChannelType.System, "", msg, "colorAction"));
	#endif
	}

	int GetPackedNetworkVariables()
	{
		int packed;

		packed |= (m_AutoHover & 0x1);
		packed |= (m_IsFreeLook & 0x1) << 1;

	#ifdef DIAG_DEVELOPER
		packed |= (m_SimulationMode & 0x3) << 2;
		packed |= (m_AirFrictionMode & 0x3) << 4;
	#endif

		packed |= (m_RBS & 0x1) << 6;
		packed |= (m_AutoCollective & 0x1) << 7;
		packed |= (m_AutoTrim & 0x1) << 8;
		packed |= (m_TranslatingTendency & 0x1) << 9;

	#ifdef DIAG_DEVELOPER
		packed |= (m_RotorDiskInertia & 0x1) << 10;

		int airFrictionX = Math.Round(m_AirFriction[0] * 100);
		int airFrictionY = Math.Round(m_AirFriction[1] * 100);
		int airFrictionZ = Math.Round(m_AirFriction[2] * 100);
		packed |= (airFrictionX & 0x7f) << 11;
		packed |= (airFrictionY & 0x7f) << 18;
		packed |= (airFrictionZ & 0x7f) << 25;
	#endif

		return packed;
	}

	//! Client
	override void NetworkSend(ParamsWriteContext ctx)
	{
		int packed = GetPackedNetworkVariables();

		ctx.Write(packed);

		ctx.Write(m_CollectiveTarget);

		if (m_AutoHover)
		{
			ctx.Write(m_AutoHoverAltitude);
			ctx.Write(m_AutoHoverSpeedTarget[0]);
			ctx.Write(m_AutoHoverSpeedTarget[2]);
		}
		else if (m_AutoCollective)
		{
			ctx.Write(m_AutoHoverAltitude);
			ctx.Write(m_CollectiveInput);
		}

		ctx.Write(m_VertSens);
		ctx.Write(m_HorzSens);

		ctx.Write(m_RotorAnimationPosition);

		if (m_EnableWind)
		{
			ctx.Write(m_WindSpeedSync[0]);
			ctx.Write(m_WindSpeedSync[2]);
		}
	}

	void UnpackNetworkVariables(int packed, bool updateFreeLook = true)
	{
		int autoHover = packed & 0x1;
		int isFreeLook = (packed >> 1) & 0x1;

	#ifdef DIAG_DEVELOPER
		int simulationMode = (packed >> 2) & 0x3;
		int airFrictionMode = (packed >> 4) & 0x3;
	#endif

		int rbs = (packed >> 6) & 0x1;
		int autoCollective = (packed >> 7) & 0x1;
		int autoTrim = (packed >> 8) & 0x1;
		int translatingTendency = (packed >> 9) & 0x1;

	#ifdef DIAG_DEVELOPER
		int rotorDiskInertia = (packed >> 10) & 0x1;

		int airFrictionX100 = (packed >> 11) & 0x7f;
		int airFrictionY100 = (packed >> 18) & 0x7f;
		int airFrictionZ100 = (packed >> 25) & 0x7f;
		float airFrictionX = airFrictionX100 * 0.01;
		float airFrictionY = airFrictionY100 * 0.01;
		float airFrictionZ = airFrictionZ100 * 0.01;
	#endif

		m_AutoHover = autoHover;

	#ifdef DIAG_DEVELOPER
		Man player = g_Game.GetPlayerByIndex(0);

		if (simulationMode != m_SimulationMode)
			Message(typename.EnumToString(ExpansionHelicopterSimulationMode, simulationMode));

		if (airFrictionMode != m_AirFrictionMode)
			Message(typename.EnumToString(ExpansionHelicopterSimulationAirFrictionMode, airFrictionMode));

		if (airFrictionX != m_AirFriction[0])
			Message("Air Friction X " + airFrictionX);

		if (airFrictionY != m_AirFriction[1])
			Message("Air Friction Y " + airFrictionY);

		if (airFrictionZ != m_AirFriction[2])
			Message("Air Friction Z " + airFrictionZ);

		if (rbs != m_RBS)
			Message("Retreating Blade Stall " + rbs.ToString());

		if (autoCollective != m_AutoCollective)
			Message("Auto Collective " + autoCollective.ToString());

		if (autoTrim != m_AutoTrim)
			Message("Auto-Trim " + autoTrim.ToString());

		if (translatingTendency != m_TranslatingTendency)
			Message("Translating Tendency " + translatingTendency.ToString());

		if (rotorDiskInertia != m_RotorDiskInertia)
			Message("Rotor Disk Inertia " + rotorDiskInertia.ToString());

		m_AirFriction[0] = airFrictionX;
		m_AirFriction[1] = airFrictionY;
		m_AirFriction[2] = airFrictionZ;
	#endif

		if (updateFreeLook)
			m_IsFreeLook = isFreeLook;

	#ifdef DIAG_DEVELOPER
		m_SimulationMode = simulationMode;
		m_AirFrictionMode = airFrictionMode;
	#endif

		m_RBS = rbs;
		m_AutoCollective = autoCollective;
		m_AutoTrim = autoTrim;
		m_TranslatingTendency = translatingTendency;

	#ifdef DIAG_DEVELOPER
		m_RotorDiskInertia = rotorDiskInertia;
	#endif
	}

	//! Server
	override void NetworkRecieve(ParamsReadContext ctx)
	{
		int packed;

		ctx.Read(packed);

		UnpackNetworkVariables(packed);

		float collectiveTgt;
		ctx.Read(collectiveTgt);
		
		if (m_AutoHover)
		{
			ctx.Read(m_AutoHoverAltitude);
			float autoHoverSpeedTargetX;
			float autoHoverSpeedTargetZ;
			ctx.Read(autoHoverSpeedTargetX);
			ctx.Read(autoHoverSpeedTargetZ);
			m_AutoHoverSpeedTarget[0] = autoHoverSpeedTargetX;
			m_AutoHoverSpeedTarget[2] = autoHoverSpeedTargetZ;

			if (m_RotorSpeedTarget == 0 && m_Helicopter.m_State.m_HasDriver)
				m_CollectiveTarget = collectiveTgt;
		}
		else if (m_AutoCollective)
		{
			ctx.Read(m_AutoHoverAltitude);
			ctx.Read(m_CollectiveInput);

			if (m_RotorSpeedTarget == 0 && m_Helicopter.m_State.m_HasDriver)
				m_CollectiveTarget = collectiveTgt;
		}
		else
		{
			m_CollectiveTarget = collectiveTgt;
		}

		ctx.Read(m_VertSens);
		ctx.Read(m_HorzSens);

		ctx.Read(m_RotorAnimationPosition);

		//! Sync rotor animation to state received from client. This fixes brief animation glitches
		//! when passengers enter heli while rotor isn't spinning and pilot is present.
		if (m_RotorSpeed == 0.0)
			AnimateRotors();

		if (m_EnableWind)
		{
			float windSpeedX;
			float windSpeedZ;
			ctx.Read(windSpeedX);
			ctx.Read(windSpeedZ);
			m_WindSpeedSync[0] = windSpeedX;
			m_WindSpeedSync[2] = windSpeedZ;
		}
	}

	bool IsAutoHover()
	{
		return m_AutoHover;
	}

	void SwitchAutoHover()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SwitchAutoHover");
#endif

		auto settings = GetExpansionClientSettings();

		if (m_AutoHover && !s_AutoCollective && settings.AutoCollectiveMode != ExpansionHelicopterAutoCollectiveMode.AlwaysOff)
		{
			s_AutoCollective = true;
			m_AutoHover = false;
		}
		else if (s_AutoCollective)
		{
			s_AutoCollective = false;

			if (settings.AutoCollectiveMode == ExpansionHelicopterAutoCollectiveMode.AlwaysOn)
				m_AutoHover = true;
		}
		else
		{
			m_AutoHover = !m_AutoHover;
		}
	}

	void ReleaseTrim()
	{
		m_CyclicForwardTrim = 0.0;
		m_CyclicBackwardTrim = 0.0;
		m_CyclicLeftTrim = 0.0;
		m_CyclicRightTrim = 0.0;
		m_AntiTorqueLeftTrim = 0.0;
		m_AntiTorqueRightTrim = 0.0;
	}

	void ApplyTrim(inout float input, float trim, inout bool transitional)
	{
		if (transitional)
		{
			if (input < 0.01)
				transitional = false;

			input = Math.Max(input, trim);
		}
		else
		{
			input += trim;
		}
	}

	void SetTrim(float input, inout float trim, inout bool transitional)
	{
		trim = input;
		transitional = true;
	}

	bool IsFreeLook()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsFreeLook");
#endif

		return m_IsFreeLook;
	}

	void OnAttach(string slot, EntityAI entity)
	{
		slot.ToLower();

		m_Main.OnAttach(slot, entity);
		m_Tail.OnAttach(slot, entity);
	}

	void OnDetach(string slot, EntityAI entity)
	{
		slot.ToLower();
		
		m_Main.OnDetach(slot, entity);
		m_Tail.OnDetach(slot, entity);
	}

	//! Returns true if main or tail rotor are damaged (but not destroyed)
	bool IsRotorDamaged()
	{
		if (m_Main.m_Entity && !m_Main.m_Entity.IsDamageDestroyed() && m_Main.m_Entity.GetHealthLevel() != GameConstants.STATE_PRISTINE)
			return true;

		if (m_Tail.m_Entity && !m_Tail.m_Entity.IsDamageDestroyed() && m_Tail.m_Entity.GetHealthLevel() != GameConstants.STATE_PRISTINE)
			return true;

		return false;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Rotor Speed", m_RotorSpeed);
		instance.Add("Rotor Speed Target", m_RotorSpeedTarget);

		instance.Add("Collective", m_Collective);
		instance.Add("Collective Target", m_CollectiveTarget);

		instance.Add("Anti Torque", m_AntiTorque);
		instance.Add("Anti Torque Target", m_AntiTorqueTarget);

		instance.Add("Cyclic Forward", m_CyclicForward);
		instance.Add("Cyclic Forward Target", m_CyclicForwardTarget);

		instance.Add("Cyclic Side", m_CyclicSide);
		instance.Add("Cyclic Side Target", m_CyclicSideTarget);

		instance.Add("Auto-Hover Height", m_AutoHoverAltitude);
		instance.Add("Auto-Hover Target Speed", m_AutoHoverSpeedTarget);
		instance.Add("Auto-Hover Speed", m_AutoHoverSpeed);

		instance.Add("Lift Force Coef", m_LiftForceCoef);

		return true;
	}
#endif
};
