#ifndef DAYZ_1_27
//! 1.28+
class ExpansionHelicopterScriptMove : CarScriptMove
{
	float m_MainRotorSpeedTarget;
	float m_BackRotorSpeedTarget;
	
	float m_CyclicForwardTarget;
	float m_CyclicSideTarget;
	
	float m_AutoHoverSpeedX;
	float m_AutoHoverAltitude;
	float m_AutoHoverSpeedZ;

	int m_Packed;
	
	protected override event void Write(PawnMoveWriter ctx, PawnMove prev)
	{
		super.Write(ctx, prev);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Write(m_MainRotorSpeedTarget);
		ctx.Write(m_BackRotorSpeedTarget);
		ctx.Write(m_CyclicForwardTarget);
		ctx.Write(m_CyclicSideTarget);

		ctx.Write(m_AutoHoverSpeedX);
		ctx.Write(m_AutoHoverAltitude);
		ctx.Write(m_AutoHoverSpeedZ);

		ctx.Write(m_Packed);
	}

	protected override event void Read(PawnMoveReader ctx, PawnMove prev)
	{
		super.Read(ctx, prev);

		// DO NOT USE 'vector' TYPE, EXPAND TO THREE FLOATS
		
		ctx.Read(m_MainRotorSpeedTarget);
		ctx.Read(m_BackRotorSpeedTarget);
		ctx.Read(m_CyclicForwardTarget);
		ctx.Read(m_CyclicSideTarget);

		ctx.Read(m_AutoHoverSpeedX);
		ctx.Read(m_AutoHoverAltitude);
		ctx.Read(m_AutoHoverSpeedZ);

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
	
	float m_MainRotorSpeed;
	float m_BackRotorSpeed;
	
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
		ctx.Write(m_MainRotorSpeed);
		ctx.Write(m_BackRotorSpeed);
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
		ctx.Read(m_MainRotorSpeed);
		ctx.Read(m_BackRotorSpeed);
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

//! @note order matters because we rely on it in ExpansionVehicleHelicopter_OLD::Simulate_Common
enum ExpansionHelicopterSimulationMode
{
	RotorDisk,
	Legacy_Extended,
	Legacy
}

enum ExpansionHelicopterSimulationAirFrictionMode
{
	Balanced,
	Legacy
}

class ExpansionVehicleHelicopter_OLD : ExpansionVehicleModule
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
	static bool s_CollectiveDecay;  //! Client only
	bool m_CollectiveDecay;  //! Client & server

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

	float m_MainRotorSpeed;
	float m_MainRotorSpeedTarget;

	float m_BackRotorSpeed;
	float m_BackRotorSpeedTarget;

	float m_TailRotateFactor;

	float m_Bank;

	float m_Hydraulic = 1.0;
	float m_HydraulicPrev = -1.0; //! Set to -1 to ensure the coefficients are updated before first simulation

	float m_CyclicForward;
	float m_CyclicForwardTarget;
	float m_CyclicForwardHydraulicCoef;
	float m_CyclicForwardInputVal;

	float m_CyclicSide;
	float m_CyclicSideTarget;
	float m_CyclicSideHydraulicCoef;
	float m_CyclicSideInputVal;

	float m_AutoHoverAltitude;
	bool m_AutoHover;

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

	ref ExpansionInterpolatedInput m_Input_AntiTorqueLeft;
	ref ExpansionInterpolatedInput m_Input_AntiTorqueRight;

	ref ExpansionInterpolatedInput m_Input_CyclicForward;
	ref ExpansionInterpolatedInput m_Input_CyclicBackward;

	ref ExpansionInterpolatedInput m_Input_CyclicLeft;
	ref ExpansionInterpolatedInput m_Input_CyclicRight;

	//! Vortex Ring State (VRS) - thrust loss when descending into own downwash
	float m_VRSDescentThreshold = 1.5;   //! m/s (~300 fpm) - VRS begins
	float m_VRSDescentDeep = 3.0;        //! m/s (~600 fpm) - deep VRS
	float m_VRSAirspeedThreshold = 8.0;  //! m/s (~16kt) - below ETL, VRS possible
	float m_VRSThrustLossMax = 0.55;    //! Max thrust reduction in deep VRS (55% loss)

	//! Ground Effect - within ~1 rotor diameter (MH-6 ~8m dia, use bounding radius)
	float m_GroundEffectRadius = 1.2;    //! Multiplier of bounding radius for ground effect zone
	float m_GroundEffectMax = 1.15;     //! Max lift boost in ground effect (~15%)

	//! Translating Tendency - lateral drift in hover (tail rotor thrust)
	float m_TranslatingTendencyCoef = 0.018;  //! Rightward drift when hovering

	//! Retreating Blade Stall (RBS) - lift loss and instability at high forward speed (VNE limit)
	float m_RetreatingBladeStallSpeed = 0.72;   //! Fraction of m_MaxSpeedMS where stall begins (~VNE)
	float m_RetreatingBladeStallLiftLoss = 0.35;  //! Max lift reduction at extreme overspeed
	float m_RetreatingBladeStallRollCoef = 0.22;  //! Roll toward retreating (left) side - instability
	float m_RetreatingBladeStallPitchCoef = 0.18;  //! Nose pitch-up tendency
	float m_RetreatingBladeStallDragCoef = 0.6;   //! Extra parasite drag when stalled (form drag from separated flow, 0-60% at full RBS)

	//! Rotor-disk / blade-element model (A3 RotorLib-style)
	float m_MainRotorRadius = 4.9;        //! MH-6 rotor radius [m]
	float m_MainRotorBlades = 4;
	float m_MainRotorTipSpeed = 205.0;   //! m/s at 100% RPM
	float m_MainRotorChord = 0.35;       //! Blade chord [m]
	float m_BladeLiftSlope = 5.7;        //! Effective Cl/rad (2*pi with tip loss)
	float m_BladeStallAngle = 0.35;      //! rad (~20 deg)
	float m_BladeStallDrag = 0.05;       //! Extra Cd when stalled
	//float m_RotorDiskThrustScale = 0.85; //! Tune to match hover (blade-element magnitude)
	float m_RotorDiskThrustScale = 0.13;  //! Retuned after fixing cyclic-induced thrust asymmetry in blade element loop
	//! Collective pitch values tweaked for MH6 to give up to ~20 m/s upward and ~8 m/s downward thrust
	float m_CollectivePitchAtFull = 0.12744;   //! Collective pitch [rad] at m_MainRotorSpeed = 1.0
	float m_CollectivePitchAtMin = 0.0323; //! Collective pitch [rad] at m_MainRotorSpeed <= 0
	float m_RotorDiskCyclicForwardGain = 0.04;
	float m_RotorDiskCyclicSideGain = 0.03;

	void ExpansionVehicleHelicopter_OLD(EntityAI vehicle)
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
		
		m_Input_AntiTorqueLeft = new ExpansionInterpolatedInput("UAExpansionHeliAntiTorqueLeft");
		m_Input_AntiTorqueRight = new ExpansionInterpolatedInput("UAExpansionHeliAntiTorqueRight");
		
		m_Input_CyclicForward = new ExpansionInterpolatedInput("UAExpansionHeliCyclicForward");
		m_Input_CyclicBackward = new ExpansionInterpolatedInput("UAExpansionHeliCyclicBackward");
		
		m_Input_CyclicLeft = new ExpansionInterpolatedInput("UAExpansionHeliCyclicLeft");
		m_Input_CyclicRight = new ExpansionInterpolatedInput("UAExpansionHeliCyclicRight");
	#else
		m_Input_CollectiveUp = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveUp);
		m_Input_CollectiveDown = new ExpansionInterpolatedInput(UAExpansionHeliCollectiveDown);
		
		m_Input_AntiTorqueLeft = new ExpansionInterpolatedInput(UAExpansionHeliAntiTorqueLeft);
		m_Input_AntiTorqueRight = new ExpansionInterpolatedInput(UAExpansionHeliAntiTorqueRight);
		
		m_Input_CyclicForward = new ExpansionInterpolatedInput(UAExpansionHeliCyclicForward);
		m_Input_CyclicBackward = new ExpansionInterpolatedInput(UAExpansionHeliCyclicBackward);
		
		m_Input_CyclicLeft = new ExpansionInterpolatedInput(UAExpansionHeliCyclicLeft);
		m_Input_CyclicRight = new ExpansionInterpolatedInput(UAExpansionHeliCyclicRight);
	#endif
	}

	void ~ExpansionVehicleHelicopter_OLD()
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

			m_CyclicForwardControlCount = (curve.Count() / 2) + 1;

			if (m_CyclicForwardControlCount > MAX_CYCLIC_CONTROL_POINTS)
			{
				Error("Maximum points in 'hydraulic' exceeded. Count is '" + m_CyclicForwardControlCount + "' and maximum is '" + MAX_CYCLIC_CONTROL_POINTS + "'.");

				m_CyclicForwardControlCount = MAX_CYCLIC_CONTROL_POINTS;
			}

			m_CyclicForwardControlFluid[0] = 0;
			m_CyclicForwardControlDelta[0] = 0;

			for (i = 1; i < m_CyclicForwardControlCount; i++)
			{
				index = (i - 1) * 2;
				m_CyclicForwardControlFluid[i] = curve[index + 0];
				m_CyclicForwardControlDelta[i] = curve[index + 1];
			}
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward max";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicForwardMax = g_Game.ConfigGetFloat(path);

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

			m_CyclicSideControlCount = (curve.Count() / 2) + 1;

			if (m_CyclicSideControlCount > MAX_CYCLIC_CONTROL_POINTS)
			{
				Error("Maximum points in 'hydraulic' exceeded. Count is '" + m_CyclicSideControlCount + "' and maximum is '" + MAX_CYCLIC_CONTROL_POINTS + "'.");

				m_CyclicSideControlCount = MAX_CYCLIC_CONTROL_POINTS;
			}

			m_CyclicSideControlFluid[0] = 0;
			m_CyclicSideControlDelta[0] = 0;

			for (i = 1; i < m_CyclicSideControlCount; i++)
			{
				index = (i - 1) * 2;
				m_CyclicSideControlFluid[i] = curve[index + 0];
				m_CyclicSideControlDelta[i] = curve[index + 1];
			}
		}

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side max";
		if (g_Game.ConfigIsExisting(path))
			m_CyclicSideMax = g_Game.ConfigGetFloat(path);

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
	}

	override void SettingsChanged()
	{
		super.SettingsChanged();

		auto settings = GetExpansionSettings().GetVehicle();
		m_EnableWind = settings.EnableWindAerodynamics;
		m_EnableMainRotorDamage = settings.EnableMainRotorDamage;
		m_EnableTailRotorDamage = settings.EnableTailRotorDamage;
		m_EnableHelicopterExplosions = settings.EnableHelicopterExplosions;
	}


#ifndef DAYZ_1_27
	//! Client
	override event void ObtainMove(PawnMove pMove)
	{
		super.ObtainMove(pMove);
		
		ExpansionHelicopterScriptMove move = ExpansionHelicopterScriptMove.Cast(pMove);
		
		//move.m_EngineOn = m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX];
		
		move.m_MainRotorSpeedTarget = m_MainRotorSpeedTarget;
		move.m_BackRotorSpeedTarget = m_BackRotorSpeedTarget;
		move.m_CyclicForwardTarget = m_CyclicForwardTarget;
		move.m_CyclicSideTarget = m_CyclicSideTarget;
		
		move.m_AutoHoverSpeedX = m_AutoHoverSpeed[0];
		move.m_AutoHoverAltitude = m_AutoHoverAltitude;
		move.m_AutoHoverSpeedZ = m_AutoHoverSpeed[2];

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
			
			m_MainRotorSpeedTarget = move.m_MainRotorSpeedTarget;
			m_BackRotorSpeedTarget = move.m_BackRotorSpeedTarget;
			m_CyclicForwardTarget = move.m_CyclicForwardTarget;
			m_CyclicSideTarget = move.m_CyclicSideTarget;
			
			m_AutoHoverSpeed[0] = move.m_AutoHoverSpeedX;
			m_AutoHoverAltitude = move.m_AutoHoverAltitude;
			m_AutoHoverSpeed[2] = move.m_AutoHoverSpeedZ;
			
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
		
		m_MainRotorSpeedTarget = move.m_MainRotorSpeedTarget;
		m_BackRotorSpeedTarget = move.m_BackRotorSpeedTarget;
		m_CyclicForwardTarget = move.m_CyclicForwardTarget;
		m_CyclicSideTarget = move.m_CyclicSideTarget;
			
		m_AutoHoverSpeed[0] = move.m_AutoHoverSpeedX;
		m_AutoHoverAltitude = move.m_AutoHoverAltitude;
		m_AutoHoverSpeed[2] = move.m_AutoHoverSpeedZ;
			
		UnpackNetworkVariables(move.m_Packed, false);

		return true;
	}

	//! Server
	override event void ObtainState(/*inout*/ PawnOwnerState pState)
	{
		ExpansionHelicopterScriptOwnerState state = ExpansionHelicopterScriptOwnerState.Cast(pState);
		
		state.m_RotorSpeed = m_RotorSpeed;
		state.m_EngineState = m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX];
		
		state.m_MainRotorSpeed = m_MainRotorSpeed;
		state.m_BackRotorSpeed = m_BackRotorSpeed;
		
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
		
		m_MainRotorSpeed = state.m_MainRotorSpeed;
		m_BackRotorSpeed = state.m_BackRotorSpeed;
		
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

		float c_up = m_Input_CollectiveUp.GetValue(pState.m_DeltaTime, 1.0, inputInterface);
		float c_down = m_Input_CollectiveDown.GetValue(pState.m_DeltaTime, 1.0, inputInterface);

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

		m_CyclicForwardInputVal = c_forward - c_backward;
		m_CyclicSideInputVal = c_left - c_right;
		
		if (IsAutoHover() && pDriver && !g_Game.IsDedicatedServer() && GetExpansionClientSettings().TurnOffAutoHoverDuringFlight)
		{
			//! Automatically deactivate autohover if forward/backward/left/right input is detected (20% sensitivity)
			if (c_forward > 0.2 || c_backward > 0.2 || c_right > 0.2 || c_left > 0.2)
				SwitchAutoHover();
		}

		m_BackRotorSpeedTarget = at_left - at_right;

		if (IsAutoHover())
		{
			if (IsMissionClient())
			{
				float autoHoverHeight = m_AutoHoverAltitude;
				float autoHoverChange = c_up - c_down;

				autoHoverHeight += autoHoverChange * 0.3;

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

				m_AutoHoverAltitude = autoHoverHeight;

				float change;
				float fSpd = c_forward - c_backward;
				float sSpd = c_right - c_left;

				float sSpdMult = pState.m_MaxSpeedMS * 0.7;
				float fSpdMult = pState.m_MaxSpeedMS;

				m_AutoHoverSpeedTarget[0] = sSpd * sSpdMult;
				m_AutoHoverSpeedTarget[2] = fSpd * fSpdMult;

				change = Math.Clamp(m_AutoHoverSpeedTarget[0] - m_AutoHoverSpeed[0], -sSpdMult * 0.5 * pState.m_DeltaTime, sSpdMult * 0.5 * pState.m_DeltaTime);
				m_AutoHoverSpeed[0] = Math.Clamp(m_AutoHoverSpeed[0] + change, -sSpdMult, sSpdMult);

				change = Math.Clamp(m_AutoHoverSpeedTarget[2] - m_AutoHoverSpeed[2], -fSpdMult * 0.5 * pState.m_DeltaTime, fSpdMult * 0.5 * pState.m_DeltaTime);
				m_AutoHoverSpeed[2] = Math.Clamp(m_AutoHoverSpeed[2] + change, -fSpdMult, fSpdMult);
			}

			m_CyclicForwardTarget = 0.0;
			m_CyclicSideTarget = 0.0;
		}
		else
		{
			m_CyclicForwardTarget = m_CyclicForwardInputVal;
			m_CyclicSideTarget = m_CyclicSideInputVal;

			m_AutoHoverAltitude = pState.m_Transform[3][1];
			m_AutoHoverSpeed = "0 0 0";

			float mainRotorInput = c_up - c_down;

			if (m_CollectiveDecay && Math.AbsFloat(mainRotorInput) < 0.005)
			{
				float collectiveDecay = 0.125 * pState.m_DeltaTime;  //! Decay at half the normal rate
				if (m_MainRotorSpeed > 0.0)
					mainRotorInput -= collectiveDecay;
				else if (m_MainRotorSpeed < 0.0)
					mainRotorInput += collectiveDecay;
			}

			m_MainRotorSpeedTarget = Math.Clamp(mainRotorInput, -0.25 * pState.m_DeltaTime, 0.25 * pState.m_DeltaTime) + m_MainRotorSpeed;
		}

		UpdateController();
	}

	void UpdateController()
	{
		//! Not used ATM
		m_Controller.SetYaw(m_BackRotorSpeedTarget);
		m_Controller.SetRoll(m_CyclicSideTarget);
		m_Controller.SetPitch(m_CyclicForwardTarget);

		m_Controller.SetThrottle(m_MainRotorSpeedTarget, HELICOPTER_CONTROLLER_INDEX);
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

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		if (!m_Initialized)
			return;

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

		if (IsMissionClient() && m_EnableWind)
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
			m_WindSpeedSync = "0 0 0";
		}

		m_Main.Update(pState.m_DeltaTime);
		m_Tail.Update(pState.m_DeltaTime);
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		if (!m_Initialized)
			return;

		vector force;
		vector torque;
		float pDt = pState.m_DeltaTime;

		bool isAboveWater;
		float buoyancyForce;

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
		else if (m_Controller.m_State[HELICOPTER_CONTROLLER_INDEX])
		{
			m_RotorSpeedTarget = 1;
			if (IsMissionHost() && m_NoiseParams)
			{
				g_Game.GetNoiseSystem().AddNoise(m_Vehicle, m_NoiseParams);
			}

			if (!m_Hit && !pState.m_HasDriver && !IsAutoHover())
			{
			#ifdef DAYZ_1_27
				m_MainRotorSpeedTarget = Math.RandomFloatInclusive(-1, 1);
				m_BackRotorSpeedTarget = Math.RandomFloatInclusive(-1, 1);

				m_CyclicSideTarget = Math.RandomFloatInclusive(-1, 1);
				m_CyclicForwardTarget = Math.RandomFloatInclusive(-1, 1);
			#else
				//! 1.28+
				m_MainRotorSpeedTarget = m_Transport.RandomFloat(-1, 1);
				m_BackRotorSpeedTarget = m_Transport.RandomFloat(-1, 1);

				m_CyclicSideTarget = m_Transport.RandomFloat(-1, 1);
				m_CyclicForwardTarget = m_Transport.RandomFloat(-1, 1);
			#endif
			}
			else if (IsAutoHover())
			{
				if (!pState.m_HasDriver)
				{
					m_AutoHoverSpeed = "0 0 0";
					m_AutoHoverSpeedTarget = "0 0 0";
					m_CyclicForwardTarget = 0;
					m_CyclicSideTarget = 0;
					m_BackRotorSpeedTarget = 0;
				}

				//! Two seconds ahead - usually
				float estT = 80.0 * pDt;
				vector estimatedPosition = pState.EstimatePosition(estT);
				vector estimatedOrientation = pState.EstimateOrientation(estT);
				vector targetOrientation = vector.Zero;

				m_MainRotorSpeedTarget = Math.Clamp(m_AutoHoverAltitude - estimatedPosition[1], -0.25 * pDt, 0.25 * pDt) + m_MainRotorSpeed;

				if (m_CyclicForwardTarget == 0 && m_CyclicSideTarget == 0)
				{
					float forwardSpeed = pState.m_LinearVelocityMS[2] - m_AutoHoverSpeed[2];
					float sideSpeed = pState.m_LinearVelocityMS[0] - m_AutoHoverSpeed[0];

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
		}
		else
		{
			m_CyclicForwardTarget = 0;
			m_CyclicSideTarget = 0;
		}

		float change;

		// see the speed at which we are free falling
		float goingDown = Math.Clamp((-pState.m_LinearVelocityMS[1] - m_MinAutoRotateSpeed) / (m_MaxAutoRotateSpeed - m_MinAutoRotateSpeed), 0, 1);
		float brakeRotor = Math.Max(Math.Max(m_MainRotorSpeed * 0.2, 0), -goingDown);

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
		m_RotorSpeed = Math.Clamp(m_RotorSpeed + change, 0, 1);

		if (m_RotorSpeed == 1.0)
		{
			change = Math.Clamp(Math.Min(m_MainRotorSpeedTarget, m_RotorSpeed) - m_MainRotorSpeed, -0.25 * pDt, 0.25 * pDt);
			m_MainRotorSpeed = Math.Clamp(m_MainRotorSpeed + change, -0.2, m_RotorSpeed);
		}
		else if (goingDown)
		{
			m_MainRotorSpeed = Math.Min(m_MainRotorSpeed, m_RotorSpeed);
		}
		else
		{
			m_MainRotorSpeed *= m_RotorSpeed;
		}

		change = Math.Clamp(m_BackRotorSpeedTarget - m_BackRotorSpeed, -m_AntiTorqueSpeed * pDt, m_AntiTorqueSpeed * pDt);
		m_BackRotorSpeed = Math.Clamp(m_BackRotorSpeed + change, -m_AntiTorqueMax, m_AntiTorqueMax);

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

		if (pState.m_LinearVelocityMS.Length() > 0.05 || m_RotorSpeed != 0)
		{
			change = Math.Clamp(Math.Clamp(m_CyclicForwardTarget, -2, 2) - m_CyclicForward, -m_CyclicForwardHydraulicCoef * pDt, m_CyclicForwardHydraulicCoef * pDt);
			m_CyclicForward = Math.Clamp(m_CyclicForward + change, -m_CyclicForwardMax, m_CyclicForwardMax);

			change = Math.Clamp(Math.Clamp(m_CyclicSideTarget, -2, 2) - m_CyclicSide, -m_CyclicSideHydraulicCoef * pDt, m_CyclicSideHydraulicCoef * pDt);
			m_CyclicSide = Math.Clamp(m_CyclicSide + change, -m_CyclicSideMax, m_CyclicSideMax);

			if (!g_Game.IsDedicatedServer())
			{
				auto settings = GetExpansionClientSettings();

				if (settings.UseLegacyHelicopterFlightModel && !s_UseLegacyFlightModel)
				{
					s_SimulationMode = ExpansionHelicopterSimulationMode.Legacy;
					s_AirFrictionMode = ExpansionHelicopterSimulationAirFrictionMode.Legacy;
					s_UseLegacyFlightModel = true;
				}
				else if (!settings.UseLegacyHelicopterFlightModel && s_UseLegacyFlightModel)
				{
					s_SimulationMode = ExpansionHelicopterSimulationMode.RotorDisk;
					s_AirFrictionMode = ExpansionHelicopterSimulationAirFrictionMode.Balanced;
					s_UseLegacyFlightModel = false;
				}

				s_RBS = settings.EnableRetreatingBladeStall;

			#ifdef DIAG_DEVELOPER
				if (!g_Game.IsClient())
				{
					//! Offline/SP
					if (s_SimulationMode != m_SimulationMode)
						g_Game.Chat(typename.EnumToString(ExpansionHelicopterSimulationMode, s_SimulationMode), "colorAction");

					if (s_AirFrictionMode != m_AirFrictionMode)
						g_Game.Chat(typename.EnumToString(ExpansionHelicopterSimulationAirFrictionMode, s_AirFrictionMode), "colorAction");

					if (s_AirFriction[0] != m_AirFriction[0])
						g_Game.Chat("Air Friction X " + s_AirFriction[0], "colorAction");

					if (s_AirFriction[1] != m_AirFriction[1])
						g_Game.Chat("Air Friction Y " + s_AirFriction[1], "colorAction");

					if (s_AirFriction[2] != m_AirFriction[2])
						g_Game.Chat("Air Friction Z " + s_AirFriction[2], "colorAction");

					if (s_RBS != m_RBS)
						g_Game.Chat("Retreating Blade Stall " + s_RBS.ToString(), "colorAction");

					if (s_CollectiveDecay != m_CollectiveDecay)
						g_Game.Chat("Collective Decay " + s_CollectiveDecay.ToString(), "colorAction");
				}

			#endif

				m_SimulationMode = s_SimulationMode;
				m_AirFrictionMode = s_AirFrictionMode;
				//! On diag client, air friction values are synced to server
				m_AirFriction[0] = s_AirFriction[0];
				m_AirFriction[1] = s_AirFriction[1];
				m_AirFriction[2] = s_AirFriction[2];
				m_RBS = s_RBS;
				m_CollectiveDecay = s_CollectiveDecay;
			}
		#ifndef DIAG_DEVELOPER
			else
			{
				//! On non-diag server, air friction values are not synced from client
				m_AirFriction[0] = s_AirFriction[0];
				m_AirFriction[1] = s_AirFriction[1];
				m_AirFriction[2] = s_AirFriction[2];
			}
		#endif

			float horiSpeed = Vector(pState.m_LinearVelocityMS[0], 0, pState.m_LinearVelocityMS[2]).Length();
			float stallSpeedThreshold = pState.m_MaxSpeedMS * m_RetreatingBladeStallSpeed;
			float rbsSeverity;

			switch (m_SimulationMode)
			{
				case ExpansionHelicopterSimulationMode.Legacy:
				case ExpansionHelicopterSimulationMode.Legacy_Extended:
					Simulate_Legacy(pState, force, torque, horiSpeed, stallSpeedThreshold, rbsSeverity);
					break;

				case ExpansionHelicopterSimulationMode.RotorDisk:
				default:
					Simulate_RotorDisk(pState, force, torque, horiSpeed, stallSpeedThreshold, rbsSeverity);
					break;
			}

			Simulate_Common(pState, force, torque, horiSpeed, stallSpeedThreshold, rbsSeverity);
		}

		PreventSinkingInOcean(pState, force, torque);

		pState.m_Force += force;
		pState.m_Torque += torque;
	}

	void Simulate_Legacy(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, float stallSpeedThreshold, out float rbsSeverity)
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

			float vrsThrustMult = 1.0;
			float rbsThrustMult = 1.0;

			if (m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy_Extended)
			{
				flightEnvelope = m_FlightEnvelope_ETL;

				//! Vortex Ring State (VRS): thrust loss when descending into own downwash at low airspeed
				float descentRate = -pState.m_LinearVelocityMS[1];  //! positive = descending
				if (descentRate > m_VRSDescentThreshold && horiSpeed < m_VRSAirspeedThreshold && m_MainRotorSpeed > 0.3)
				{
					float vrsSeverity = Math.Clamp((descentRate - m_VRSDescentThreshold) / (m_VRSDescentDeep - m_VRSDescentThreshold), 0.0, 1.0);
					vrsSeverity *= 1.0 - (horiSpeed / m_VRSAirspeedThreshold);  //! Less VRS at higher airspeed
					vrsThrustMult = 1.0 - (vrsSeverity * m_VRSThrustLossMax);
				}

				//! Retreating Blade Stall - lift loss at high forward speed (VNE). Worse at low RPM and high altitude.
				if (m_RBS && horiSpeed > stallSpeedThreshold && m_RotorSpeed > 0.5)
				{
					rbsSeverity = Math.Clamp((horiSpeed - stallSpeedThreshold) / (pState.m_MaxSpeedMS - stallSpeedThreshold), 0.0, 1.0);
					rbsSeverity *= (1.2 - m_RotorSpeed);  //! Worse at low rotor RPM
					rbsSeverity *= (1.5 - pState.m_AltitudeLimiter * 0.5);  //! Worse at high altitude
					rbsSeverity = Math.Clamp(rbsSeverity, 0.0, 1.0);
					rbsThrustMult = 1.0 - (rbsSeverity * m_RetreatingBladeStallLiftLoss);
				}
			}
			else
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

			float targetVelocity = (pState.m_LinearVelocityMS[1] + 3.0) - (18.0 * m_MainRotorSpeed * m_RotorSpeed * nearGround);
			if (targetVelocity < -5)
				targetVelocity = -5;

			targetVelocity *= pDt;
			float collectiveCoef = Math.Max((1.3 * liftFactor) - ((Math.SquareSign(targetVelocity) * 5.0) + (targetVelocity * 80.0)), 0);

			force += Vector(0, 1, 0) * collectiveCoef * vrsThrustMult * rbsThrustMult * pState.m_AltitudeLimiter * m_RotorSpeed * m_RotorSpeed * m_LiftForceCoef * pState.m_Mass;
		}
	}

	void Simulate_Common(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, float stallSpeedThreshold, float rbsSeverity)
	{
		float pDt = pState.m_DeltaTime;

		//! Translating Tendency: lateral drift in hover (tail rotor pushes heli right)
		if (horiSpeed < 5.0 && m_RotorSpeed > 0.5)
		{
			float translatingForce = m_TranslatingTendencyCoef * m_RotorSpeed * m_RotorSpeed * pState.m_Mass * pState.m_AltitudeLimiter;
			force += Vector(1, 0, 0) * translatingForce;  //! +X in model space = right
		}

		//! Cyclic
		float cyclicForce = m_CyclicForceCoef * pState.m_Mass * m_RotorSpeed * m_RotorSpeed * pState.m_AltitudeLimiter;
		if (IsAutoHover() || m_SimulationMode == ExpansionHelicopterSimulationMode.Legacy)
		{
			//! Don't use angular velocity target in autohover or they will fight over cyclic authority
			//! (autover already adjusts cyclic input to meet target)
			torque[0] = torque[0] - (cyclicForce * m_CyclicForward * m_CyclicForwardCoef * pState.m_BoundingRadius);
			torque[2] = torque[2] + (cyclicForce * m_CyclicSide * m_CyclicSideCoef * pState.m_BoundingRadius);
		}
		else
		{
			//! Use angular velocity target to calculate torque for cyclic (this is what makes the controls feel crisp)
			vector cyclicVelocityTarget;
			cyclicVelocityTarget[0] = (m_CyclicForward * Math.PI_HALF);
			cyclicVelocityTarget[2] = (m_CyclicSide * Math.PI_HALF);
			vector cyclicError = cyclicVelocityTarget - pState.m_AngularVelocityMS;
			torque[0] = torque[0] - (cyclicForce * cyclicError[0] * m_CyclicForwardCoef * pState.m_BoundingRadius);
			torque[2] = torque[2] + (cyclicForce * cyclicError[2] * m_CyclicSideCoef * pState.m_BoundingRadius);
		}

		//! Retreating Blade Stall - nose pitch-up and roll toward retreating (left) side at high speed
		//! @note only for Legacy_Extended and RotorDisk simulation
		if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
		{
			if (rbsSeverity > 0.01)
			{
				float rbsTorque = rbsSeverity * cyclicForce * pState.m_BoundingRadius;
				torque[0] = torque[0] - (rbsTorque * m_RetreatingBladeStallPitchCoef);   //! Nose pitch-up
				torque[2] = torque[2] + (rbsTorque * m_RetreatingBladeStallRollCoef);    //! Roll left (retreating side down)
			}
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

		//! 0 if heli is facing direction of movement, 1 if perpendicular
		vector vehDir = pState.m_Transform[2];
		vector velDir = pState.m_LinearVelocity.Normalized();
		float perpendicular = Math.Acos(vector.Dot(vehDir, velDir)) / Math.PI_HALF;
		if (perpendicular > 1.0)
			perpendicular = 2.0 - perpendicular;

		float tailRotorForce;
		float tailForce;

		float maxSpeedTailEffect = 0.0125;  //! at ~200 km/h, tail rotor has 30% effect, at 288 km/h, it has no effect
		float tailEffectMin;
		float scaledSpeedFactor = 1.0 - Math.Min(Math.AbsFloat(pState.m_LinearVelocityMS[2] * maxSpeedTailEffect), 1.0 - tailEffectMin);

		float yawVelocity = pState.m_AngularVelocityMS[1];

		//! tail rotor
		{
			float tailRotorMalfunction = 0.0;
			if (m_EnableTailRotorDamage)
			{
				tailRotorMalfunction = m_Tail.GetHealthLevel() / 5.0;
			}

			float tailRotorMalfunctionNeg = 1.0 - tailRotorMalfunction;
			float tailRotorMalfunctionTorque = 0.5 * tailRotorMalfunction * m_RotorSpeed * (m_RotorSpeed + 0.1);

			tailRotorForce = m_BackRotorSpeed * m_TailForceCoef * tailRotorMalfunctionNeg;
		}

		float bankForce = Math.Asin(m_Bank) * m_BankForceCoef * m_TailRotateFactor;

		//! tail aerodynamic forces
		if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
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
			//! Lerp fin effectiveness with speed — near zero at hover so low-speed corrections feel smooth,
			//! full strength at cruise so heading tracks naturally. This is what gives A3 the feeling that
			//! fin resistance gradually releases as you slow down from an air brake.
			float speedBlend = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.25), 0.0, 1.0);
			float finEffectivenessCoef = Math.Lerp(0.12, 0.45, speedBlend);
			//finEffectivenessCoef *= Math.Min((REFERENCE_MASS * REFERENCE_BOUNDING_RADIUS) / (pState.m_Mass * pState.m_BoundingRadius), 1.0);

			float pedalActivity = Math.Clamp(Math.AbsFloat(m_BackRotorSpeed) / m_AntiTorqueMax * 2.0, 0.0, 1.0);
			float speedFactor = Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.4), 0.0, 1.0);
			//! At high speed pedals suppress only 10% of fin — fin wins, enforcing the ~90° wall like A3
			//! At low speed pedals suppress up to 100% — free to pirouette in hover
			float maxSuppression = 1.0 - (speedFactor * 0.90);
			float activeFinCoef = finEffectivenessCoef * (1.0 - pedalActivity * maxSuppression);

			//! Rate-limit the fin force — keeps orbit corrections gradual at low speed.
			//! The ceiling is driven by ANGLE not speed — so approaching 90° keeps the wall strong
			//! even as the air brake scrubs speed. This is what makes the wall feel like it's
			//! actively resisting rather than fading away as you slow down.
			float angleWall = perpendicular * perpendicular;  //! 0 at forward, 1 at fully perpendicular
			float speedBudget = Math.Clamp(horiSpeed * horiSpeed * 0.006 * pDt, 0.0, 1.2);
			float angleBudget = Math.Clamp(angleWall * 1.5 * pDt, 0.0, 1.2);
			float maxFinForce = Math.Max(speedBudget, angleBudget) * pState.m_Mass;
			float rawDirStabilityForce = -beta * horiSpeed * airDensity * finArea * activeFinCoef * pDt;
			float dirStabilityForce = Math.Clamp(rawDirStabilityForce, -maxFinForce, maxFinForce);

			//! Preserve yaw speed "feel" of legacy simulation
			tailRotorForce *= Math.Min(Math.Max(pState.m_BoundingRadius / REFERENCE_BOUNDING_RADIUS, 1.0), 2.0);

			//! Use angular velocity target to calculate torque for antitorque (this is what makes the controls feel crisp)
			float antiTorqueYawTarget = -(bankForce + tailRotorForce - tailRotorMalfunctionTorque) * Math.PI;
			float antiTorqueYawDiff = antiTorqueYawTarget - yawVelocity;

			float t = perpendicular * speedFactor;
			if (m_BackRotorSpeed == 0.0)
				t = Easing.EaseOutQuad(t);
			tailForce = Math.Lerp(-antiTorqueYawDiff * scaledSpeedFactor, dirStabilityForce + bankForce * Math.PI_HALF, t);
		}
		else
		{
			tailForce = (bankForce + tailRotorForce - tailRotorMalfunctionTorque) * 0.5 * scaledSpeedFactor;
		}

		//! apply tail rotor + aerodynamic forces
		{
			tailForce *= m_RotorSpeed * m_RotorSpeed * pState.m_BoundingRadius * pState.m_Mass;

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
#ifdef EXPANSION_HELI_WIND
			vector windSpeed = m_WindSpeedSync.InvMultiply3(pState.m_Transform);
			frictionSpeed += windSpeed;
#endif

			float linearFrictionCoefX;
			float linearFrictionCoefY;
			float linearFrictionCoefZ;

			switch (m_AirFrictionMode)
			{
				case ExpansionHelicopterSimulationAirFrictionMode.Legacy:
					//! @note because the legacy lateral friction value of 16.0 is extremely high relative to the forward value of 0.04,
					//! this can have the undesired side-effect of strongly affecting the direction of movement!
					//! I.e. when being in a forward hover motion with heli leveled and then applying yaw perpendicular
					//! to the movement direction, the direction of movement will change to the direction the heli is facing
					//! very rapidly (because the heli is "pushed" by the air resistance) which is *not* correct.

					//! if the helicopter is turned on then more force is applied to create pseudo-friction
					float stabilizeXY = 0.4 + (m_RotorSpeed * m_RotorSpeed * 0.6);

					linearFrictionCoefX = m_LinearFrictionCoef[0] * stabilizeXY;
					linearFrictionCoefY = m_LinearFrictionCoef[1] * stabilizeXY;
					linearFrictionCoefZ = m_LinearFrictionCoef[2];
					break;

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
			//! @note only for Legacy_Extended and RotorDisk simulation
			if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
			{
				if (rbsSeverity > 0.01)
				{
					float rbsDragMult = 1.0 + (rbsSeverity * m_RetreatingBladeStallDragCoef);
					friction[2] = friction[2] * rbsDragMult;  //! Forward axis - main parasite drag from stalled retreating blade
				}
			}

			force -= friction * m_BodyFrictionCoef;
		}

		//! convert forces to worldspace
		{
			force = force.Multiply3(pState.m_Transform);
			torque = torque.Multiply3(pState.m_Transform);
		}

		//! Tail rotor lateral push - applied in worldspace using heli's right axis flattened to horizontal
		//! so pitch attitude never bleeds into vertical. Creates curved flight path toward nose when holding yaw at speed (A3 behaviour)
		if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
		{
			float tailLateralCoef = 0.004;
			float tailLateralForce = -(m_BackRotorSpeed * m_TailForceCoef * tailLateralCoef * m_RotorSpeed * pState.m_Mass);
			//! Use heli right axis (transform column 0) but zero out Y so it's always horizontal
			vector rightAxis = Vector(pState.m_Transform[0][0], 0, pState.m_Transform[0][2]);
			float rightLen = rightAxis.Length();
			if (rightLen > 0.001)
				rightAxis = rightAxis * (1.0 / rightLen);
			force = force + rightAxis * tailLateralForce;
		}

		//! Sideslip drag applied in worldspace — purely horizontal so pitched attitude can't bleed into vertical
		//! This is what slows you down when presenting the heli's side to the airflow (pedal air-braking)
		if (m_AirFrictionMode != ExpansionHelicopterSimulationAirFrictionMode.Legacy)
		{
			vector horizVel = Vector(pState.m_LinearVelocity[0], 0, pState.m_LinearVelocity[2]);
			float horizSpeed = horizVel.Length();
			if (horizSpeed > 0.1)
			{
				float sideslipDragMult = perpendicular * perpendicular * 4.5;
				float sdForce = horizSpeed * horizSpeed * sideslipDragMult * pDt * m_AirFriction[0] * airFrictionScaleInv * pState.m_Mass * m_BodyFrictionCoef;
				force -= horizVel.Normalized() * sdForce;
			}
		}

		//! Angular Friction
		{
			vector t_friction;

			float tailRotateFactor = m_TailRotateFactor;

			if (m_SimulationMode != ExpansionHelicopterSimulationMode.Legacy)
				tailRotateFactor *= 0.15;
			else
				tailRotateFactor *= 0.5;

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
	void Simulate_RotorDisk(ExpansionPhysicsState pState, inout vector force, inout vector torque, float horiSpeed, float stallSpeedThreshold, out float rbsSeverity)
	{
		float pDt = pState.m_DeltaTime;

		float vh = pState.m_LinearVelocityMS[2];   //! Forward velocity (model +Z)
		float vx = pState.m_LinearVelocityMS[0];   //! Lateral velocity
		float vv = pState.m_LinearVelocityMS[1];   //! Vertical (up positive)

		float omegaR = m_MainRotorTipSpeed * m_RotorSpeed;  //! Tip speed [m/s]
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

		//! A3-style ETL bell curve.
		//! At hover: high induced inflow (inefficient). As speed builds, inflow drops (more efficient).
		//! The bell curve creates a pronounced lift surge at ~6-12 m/s before tapering off at cruise.
		//! ETL peak speed in m/s — ~16-24 knots, matches A3 feel
		float etlPeakSpeed = 9.0; //Center of the bump in m/s, lower for earlier and more pronounced, higher for later and less pronounced
		float etlPeakWidth = 5.0; //How wide the bump is in m/s, Smaller = sharper/more sudden, larger = more gradual spread
		float speedDiff = horiSpeed - etlPeakSpeed;
		float etlBump = 1.0 + 0.40 * Math.Exp(-(speedDiff * speedDiff) / (2.0 * etlPeakWidth * etlPeakWidth)); //! ETL bump multiplier 0.40 means 40% more thrust at the peak of the bump

		 //! Ideal induced velocity for hover, from momentum theory: vi = sqrt(T / (2 * rho * A))
		 //! T = weight = mass * gravity. A = rotor disk area = pi * r^2. rho = air density.
		float viHover = Math.Sqrt((REFERENCE_MASS * 9.81) / (2.0 * 1.225 * Math.PI * m_MainRotorRadius * m_MainRotorRadius));
		float vi = viHover * (1.0 - Math.Clamp(mu * 0.5, 0.0, 0.6));
		vi = vi / (groundEffect * etlBump);  //! ETL bump reduces induced inflow = more thrust at transition speed

		//! VRS
		float vrsMult = 1.0;
		float descentRate = -vv;
		if (descentRate > m_VRSDescentThreshold && horiSpeed < m_VRSAirspeedThreshold && m_MainRotorSpeed > 0.3)
		{
			float vrsSeverity = Math.Clamp((descentRate - m_VRSDescentThreshold) / (m_VRSDescentDeep - m_VRSDescentThreshold), 0.0, 1.0);
			vrsSeverity *= 1.0 - (horiSpeed / m_VRSAirspeedThreshold);
			vrsMult = 1.0 - (vrsSeverity * m_VRSThrustLossMax);
		}

		//! Collective → blade pitch: m_MainRotorSpeed 0 = zero thrust, 1 = full
		float theta0 = m_CollectivePitchAtMin;
		float collective = m_MainRotorSpeed * m_RotorSpeed;
		if (collective > 0.0)
			theta0 = theta0 * 1.2 + collective * m_CollectivePitchAtFull;
		else
			theta0 = ExpansionMath.LinearConversion(-0.2, 0.0, collective, theta0, theta0 * 1.2);
		float theta1s = m_CyclicForward * 0.04;   //! Longitudinal cyclic
		float theta1c = m_CyclicSide * 0.04;     //! Lateral cyclic

		//! Blade-element sampling: 8 azimuth sectors. cos/sin lookup [0..7]
		float azCos[8] = { 1.0, 0.707, 0.0, -0.707, -1.0, -0.707, 0.0, 0.707 };
		float azSin[8] = { 0.0, 0.707, 1.0, 0.707, 0.0, -0.707, -1.0, -0.707 };

		float dpsi = 0.785398;  //! 2*PI/8
		float rho = 1.225 * pState.m_AltitudeLimiter;

		float totalThrust = 0.0;
		float totalPitchMoment = 0.0;
		float totalRollMoment = 0.0;

		if (m_RBS && horiSpeed > stallSpeedThreshold && m_RotorSpeed > 0.5)
		{
			rbsSeverity = Math.Clamp((horiSpeed - stallSpeedThreshold) / (pState.m_MaxSpeedMS - stallSpeedThreshold), 0.0, 1.0);
			rbsSeverity *= (1.2 - m_RotorSpeed);
			rbsSeverity *= (1.5 - pState.m_AltitudeLimiter * 0.5);
			rbsSeverity = Math.Clamp(rbsSeverity, 0.0, 1.0);
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

			//! Thrust uses collective pitch ONLY — cyclic is intentionally excluded here.
			//! Including cyclic in thrust breaks azimuthal symmetry via the nonlinear stall/negative-alpha
			//! clamps, causing cyclic inputs to spuriously increase total thrust (skyrocket on bank+yaw).
			float alphaThrust = theta0 - phi;
			float clThrust = m_BladeLiftSlope * alphaThrust;
			bool isRetreating = ut < omegaR * 0.6;
			if (m_RBS && alphaThrust > m_BladeStallAngle && isRetreating)
			{
				float stallFactor = Math.Clamp((alphaThrust - m_BladeStallAngle) / 0.25, 0.0, 1.0);
				clThrust = m_BladeLiftSlope * m_BladeStallAngle * (1.0 - stallFactor * 0.8);
			}
			else if (alphaThrust < -0.1)
			{
				clThrust = clThrust * 0.5;
			}

			float dL = 0.5 * rho * m_MainRotorChord * (m_MainRotorRadius * 0.7) * clThrust * u * u * dpsi;
			totalThrust += dL * m_MainRotorBlades;

			//! TODO: Could use this but much harder to fly than old cyclic controller
			//float theta = theta0 + theta1s * spsi + theta1c * cpsi;
			//float alpha = theta - phi;
			//totalPitchMoment = totalPitchMoment - (dL * spsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
			//totalRollMoment = totalRollMoment + (dL * cpsi * m_MainRotorRadius * 0.7 * m_MainRotorBlades);
		}

		totalThrust *= vrsMult * groundEffect * m_RotorDiskThrustScale;
		totalThrust *= (1.0 - rbsSeverity * m_RetreatingBladeStallLiftLoss);

		//! Sideslip rotor efficiency loss — high perpendicular airflow disturbs disk inflow,
		//! reducing effective thrust. Scales with both sideslip angle and speed so it has
		//! no effect in hover (speed=0) but progressively counters the climb during air braking.
		//! This matches A3 behaviour where you do gain altitude but slowly, not a skyrocket.
		vector vehDir = pState.m_Transform[2];
		vector velDir = pState.m_LinearVelocity.Normalized();
		float perpendicular = Math.Acos(vector.Dot(vehDir, velDir)) / Math.PI_HALF;
		if (perpendicular > 1.0)
			perpendicular = 2.0 - perpendicular;
		float sideslipEfficiencyLoss = perpendicular * perpendicular * Math.Clamp(horiSpeed / (pState.m_MaxSpeedMS * 0.5), 0.0, 1.0) * 0.35;
		totalThrust *= (1.0 - sideslipEfficiencyLoss);

		//! Clamp thrust to negative mass
		//! (prevents wheels of large, heavy helis like the Merlin getting pushed through ground)
		if (totalThrust < -REFERENCE_MASS)
			totalThrust = -pState.m_Mass;
		else
			totalThrust *= pState.m_Mass / REFERENCE_MASS;  //! Scale by reference mass (1.0 = MH6)

		force += Vector(0, 1, 0) * totalThrust;

		//! Apply disk moments with small gain so cyclic feels stable (raw moments are very large)
		//! TODO: Could use this but much harder to fly than old cyclic controller
		//torque[0] = torque[0] - (totalPitchMoment * m_RotorDiskCyclicForwardGain);
		//torque[2] = torque[2] + (totalRollMoment * m_RotorDiskCyclicSideGain);
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
	}

	void AnimateCyclic()
	{
		m_Vehicle.SetAnimationPhase("cyclicForward", m_CyclicForwardInputVal);
		m_Vehicle.SetAnimationPhase("cyclicAside", -m_CyclicSideInputVal);
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

	int GetPackedNetworkVariables()
	{
		int packed;

		packed |= (m_AutoHover & 0x1);
		packed |= (m_IsFreeLook & 0x1) << 1;
		packed |= (m_SimulationMode & 0x3) << 2;
		packed |= (m_AirFrictionMode & 0x3) << 4;
		packed |= (m_RBS & 0x1) << 6;
		packed |= (m_CollectiveDecay & 0x1) << 7;

	#ifdef DIAG_DEVELOPER
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

		if (m_AutoHover)
		{
			ctx.Write(m_AutoHoverAltitude);
			ctx.Write(m_AutoHoverSpeed);
		}

		ctx.Write(m_VertSens);
		ctx.Write(m_HorzSens);

		ctx.Write(m_RotorAnimationPosition);

		if (m_EnableWind)
			ctx.Write(m_WindSpeedSync);
	}

	void UnpackNetworkVariables(int packed, bool updateFreeLook = true)
	{
		int autoHover = packed & 0x1;
		int isFreeLook = (packed >> 1) & 0x1;
		int simulationMode = (packed >> 2) & 0x3;
		int airFrictionMode = (packed >> 4) & 0x3;
		int rbs = (packed >> 6) & 0x1;
		int collectiveDecay = (packed >> 7) & 0x1;

	#ifdef DIAG_DEVELOPER
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
			g_Game.ChatMP(player, typename.EnumToString(ExpansionHelicopterSimulationMode, simulationMode), "colorAction");

		if (airFrictionMode != m_AirFrictionMode)
			g_Game.ChatMP(player, typename.EnumToString(ExpansionHelicopterSimulationAirFrictionMode, airFrictionMode), "colorAction");

		if (airFrictionX != m_AirFriction[0])
			g_Game.ChatMP(player, "Air Friction X " + airFrictionX, "colorAction");

		if (airFrictionY != m_AirFriction[1])
			g_Game.ChatMP(player, "Air Friction Y " + airFrictionY, "colorAction");

		if (airFrictionZ != m_AirFriction[2])
			g_Game.ChatMP(player, "Air Friction Z " + airFrictionZ, "colorAction");

		if (rbs != m_RBS)
			g_Game.ChatMP(player, "Retreating Blade Stall " + rbs.ToString(), "colorAction");

		m_AirFriction[0] = airFrictionX;
		m_AirFriction[1] = airFrictionY;
		m_AirFriction[2] = airFrictionZ;
	#endif

		if (updateFreeLook)
			m_IsFreeLook = isFreeLook;
		m_SimulationMode = simulationMode;
		m_AirFrictionMode = airFrictionMode;
		m_RBS = rbs;
		m_CollectiveDecay = collectiveDecay;
	}

	//! Server
	override void NetworkRecieve(ParamsReadContext ctx)
	{
		int packed;

		ctx.Read(packed);

		UnpackNetworkVariables(packed);

		if (m_AutoHover)
		{
			ctx.Read(m_AutoHoverAltitude);
			ctx.Read(m_AutoHoverSpeed);
		}

		ctx.Read(m_VertSens);
		ctx.Read(m_HorzSens);

		ctx.Read(m_RotorAnimationPosition);

		//! Sync rotor animation to state received from client. This fixes brief animation glitches
		//! when passengers enter heli while rotor isn't spinning and pilot is present.
		if (m_RotorSpeed == 0.0)
			AnimateRotors();

		if (m_EnableWind)
			ctx.Read(m_WindSpeedSync);
	}

	bool IsAutoHover()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsAutoHover");
#endif

		return m_AutoHover;
	}

	void SwitchAutoHover()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SwitchAutoHover");
#endif

		m_AutoHover = !m_AutoHover;
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

		instance.Add("Main Rotor Speed", m_MainRotorSpeed);
		instance.Add("Main Rotor Speed Target", m_MainRotorSpeedTarget);

		instance.Add("Back Rotor Speed", m_BackRotorSpeed);
		instance.Add("Back Rotor Speed Target", m_BackRotorSpeedTarget);

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
