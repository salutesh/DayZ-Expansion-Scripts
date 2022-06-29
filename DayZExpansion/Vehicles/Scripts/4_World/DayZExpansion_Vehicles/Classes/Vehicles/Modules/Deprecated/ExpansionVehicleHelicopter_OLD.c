
class ExpansionVehicleHelicopter_OLD : ExpansionVehicleModule
{
	static int HELICOPTER_CONTROLLER_INDEX = 1;

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
	float m_CyclicForwardSpeed; // (m/s) change per tick
	float m_CyclicForwardMax;	// (m) per tick
	float m_CyclicForwardCoef;	// Coefficient for forward cyclic

	// the rotation at which the side cyclic moves (roll)
	float m_CyclicSideSpeed; // (m/s)
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

	float m_RotorSpeed;
	float m_RotorSpeedTarget;

	float m_MainRotorSpeed;
	float m_MainRotorSpeedTarget;

	float m_BackRotorSpeed;
	float m_BackRotorSpeedTarget;

	float m_TailRotateFactor;

	float m_Bank;

	float m_CyclicForward;
	float m_CyclicForwardTarget;

	float m_CyclicSide;
	float m_CyclicSideTarget;

	float m_AutoHoverAltitude;
	bool m_AutoHover;

	bool m_IsFreeLook = true;
	bool m_WasFreeLookPressed;
	float m_MouseVertSens;
	float m_MouseHorzSens;

	vector m_AutoHoverSpeed;
	vector m_AutoHoverSpeedTarget;

	bool m_Hit;
	Object m_HitObject;
	vector m_HitPosition;
	vector m_HitNormal;
	float m_HitFraction;

	vector m_WindSpeedSync;

	bool m_EnableWind;
	bool m_EnableTailRotorDamage;
	bool m_EnableHelicopterExplosions;

	float m_RotorAnimationPosition;

	float m_WaterVolume;
	float m_TotalVolume;

	autoptr NoiseParams m_NoiseParams;
	Particle m_DustParticle;
	Particle m_WaterParticle;

	void ExpansionVehicleHelicopter_OLD(EntityAI pVehicle)
	{
		m_NoiseParams = new NoiseParams();
		m_NoiseParams.Load("HeliExpansionNoise");

		m_SelfDebugWindow = true;
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

		ExpansionHelicopterScript heli;
		if (!Class.CastTo(heli, m_Vehicle))
			return;

		m_MinAutoRotateSpeed = heli.m_MinAutoRotateSpeed;
		m_MaxAutoRotateSpeed = heli.m_MaxAutoRotateSpeed;
		m_BodyFrictionCoef = heli.m_BodyFrictionCoef;
		m_LiftForceCoef = heli.m_LiftForceCoef;
		m_CyclicForceCoef = heli.m_CyclicForceCoef;
		m_CyclicForwardSpeed = heli.m_CyclicForwardSpeed;
		m_CyclicForwardMax = heli.m_CyclicForwardMax;
		m_CyclicForwardCoef = heli.m_CyclicForwardCoef;
		m_CyclicSideSpeed = heli.m_CyclicSideSpeed;
		m_CyclicSideMax = heli.m_CyclicSideMax;
		m_CyclicSideCoef = heli.m_CyclicSideCoef;
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
	}

	override void TEMP_DeferredInit()
	{
		m_Initialized = true;

		super.TEMP_DeferredInit();

		string path;

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule bodyFrictionCoef";
		if (GetGame().ConfigIsExisting(path))
			m_BodyFrictionCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule liftForceCoef";
		if (GetGame().ConfigIsExisting(path))
			m_LiftForceCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule bankForceCoef";
		if (GetGame().ConfigIsExisting(path))
			m_BankForceCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule tailForceCoef";
		if (GetGame().ConfigIsExisting(path))
			m_TailForceCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule linearFrictionCoef";
		if (GetGame().ConfigIsExisting(path))
			m_LinearFrictionCoef = GetGame().ConfigGetVector(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule angularFrictionCoef";
		if (GetGame().ConfigIsExisting(path))
			m_AngularFrictionCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor minAutoRotateSpeed";
		if (GetGame().ConfigIsExisting(path))
			m_MinAutoRotateSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor maxAutoRotateSpeed";
		if (GetGame().ConfigIsExisting(path))
			m_MaxAutoRotateSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Rotor startUpTime";
		if (GetGame().ConfigIsExisting(path))
			m_EngineStartDuration = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule AntiTorque speed";
		if (GetGame().ConfigIsExisting(path))
			m_AntiTorqueSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule AntiTorque max";
		if (GetGame().ConfigIsExisting(path))
			m_AntiTorqueMax = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic forceCoefficient";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicForceCoef = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward speed";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicForwardSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward max";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicForwardMax = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward coefficient";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicForwardCoef = GetGame().ConfigGetFloat(path);

		//path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Forward animation";
		//if ( GetGame().ConfigIsExisting( path ) )
		//	m_Inertia = GetGame().ConfigGetTextOut( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side speed";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicSideSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side max";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicSideMax = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side coefficient";
		if (GetGame().ConfigIsExisting(path))
			m_CyclicSideCoef = GetGame().ConfigGetFloat(path);

		//path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Cyclic Side animation";
		//if ( GetGame().ConfigIsExisting( path ) )
		//	m_Inertia = GetGame().ConfigGetTextOut( path );

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

		m_EnableWind = GetExpansionSettings().GetVehicle().EnableWindAerodynamics;
		m_EnableTailRotorDamage = GetExpansionSettings().GetVehicle().EnableTailRotorDamage;
		m_EnableHelicopterExplosions = GetExpansionSettings().GetVehicle().EnableHelicopterExplosions;
	}

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		if (!m_Initialized || !pDriver)
			return;

		UAInterface input = pDriver.GetInputInterface();

#ifdef COMPONENT_SYSTEM
		if (IsMissionClient())
		{
			if (GetExpansionClientSettings().UseHelicopterMouseControl)
			{
				bool isFreelook = input.SyncedValue("UAExpansionHeliFreeLook");

				if (isFreelook && !m_WasFreeLookPressed)
				{
					m_WasFreeLookPressed = true;
					m_IsFreeLook = !m_IsFreeLook;
				}
				else if (!isFreelook)
				{
					m_WasFreeLookPressed = false;
				}

				m_MouseVertSens = GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;
				if (GetExpansionClientSettings().UseInvertedMouseControl)
					m_MouseVertSens = -m_MouseVertSens;

				m_MouseHorzSens = GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
			}
			else
			{
				m_IsFreeLook = true;
			}
		}

		float c_up = input.SyncedValue("UAExpansionHeliCollectiveUp");
		float c_down = input.SyncedValue("UAExpansionHeliCollectiveDown");

		float at_left = input.SyncedValue("UAExpansionHeliAntiTorqueLeft");
		float at_right = input.SyncedValue("UAExpansionHeliAntiTorqueRight");

		float c_forward = input.SyncedValue("UAExpansionHeliCyclicForward");
		float c_backward = input.SyncedValue("UAExpansionHeliCyclicBackward");

		float c_left = input.SyncedValue("UAExpansionHeliCyclicLeft");
		float c_right = input.SyncedValue("UAExpansionHeliCyclicRight");
#else
		if (IsMissionClient())
		{
			if (GetExpansionClientSettings().UseHelicopterMouseControl)
			{
				bool isFreelook = input.SyncedValue_ID(UAExpansionHeliFreeLook);

				if (isFreelook && !m_WasFreeLookPressed)
				{
					m_WasFreeLookPressed = true;
					m_IsFreeLook = !m_IsFreeLook;
				}
				else if (!isFreelook)
				{
					m_WasFreeLookPressed = false;
				}

				m_MouseVertSens = GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;
				if (GetExpansionClientSettings().UseInvertedMouseControl)
					m_MouseVertSens = -m_MouseVertSens;

				m_MouseHorzSens = GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
			}
			else
			{
				m_IsFreeLook = true;
			}
		}

		float c_up = input.SyncedValue_ID(UAExpansionHeliCollectiveUp);
		float c_down = input.SyncedValue_ID(UAExpansionHeliCollectiveDown);

		float at_left = input.SyncedValue_ID(UAExpansionHeliAntiTorqueLeft);
		float at_right = input.SyncedValue_ID(UAExpansionHeliAntiTorqueRight);

		float c_forward = input.SyncedValue_ID(UAExpansionHeliCyclicForward);
		float c_backward = input.SyncedValue_ID(UAExpansionHeliCyclicBackward);

		float c_left = input.SyncedValue_ID(UAExpansionHeliCyclicLeft);
		float c_right = input.SyncedValue_ID(UAExpansionHeliCyclicRight);
#endif

		if (!IsFreeLook())
		{
			c_forward += input.SyncedValue_ID(UAAimDown) * m_MouseVertSens;
			c_backward += input.SyncedValue_ID(UAAimUp) * m_MouseVertSens;

			c_left += input.SyncedValue_ID(UAAimLeft) * m_MouseHorzSens;
			c_right += input.SyncedValue_ID(UAAimRight) * m_MouseHorzSens;
		}
		
		if (IsAutoHover() && pDriver && !GetGame().IsDedicatedServer() && GetExpansionClientSettings().TurnOffAutoHoverDuringFlight)
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

				float surfaceY = GetGame().SurfaceY(pState.m_Transform[3][0], pState.m_Transform[3][2]);
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
			m_CyclicForwardTarget = c_forward - c_backward;
			m_CyclicSideTarget = c_left - c_right;

			m_AutoHoverAltitude = pState.m_Transform[3][1];
			m_AutoHoverSpeed = "0 0 0";

			float mainRotorInput = c_up - c_down;
			m_MainRotorSpeedTarget = Math.Clamp(mainRotorInput, -0.25 * pState.m_DeltaTime, 0.25 * pState.m_DeltaTime) + m_MainRotorSpeed;
		}

		//! Not used ATM
		m_Controller.SetYaw(m_BackRotorSpeedTarget);
		m_Controller.SetRoll(m_CyclicSideTarget);
		m_Controller.SetPitch(m_CyclicForwardTarget);

		m_Controller.SetThrottle(m_MainRotorSpeedTarget, HELICOPTER_CONTROLLER_INDEX);
		m_Controller.SetBrake(0, HELICOPTER_CONTROLLER_INDEX);
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		if (!m_Initialized)
			return;

		float modelSize = pState.m_BoundingRadius * 1.5;
		modelSize += 10.0;

		vector start = pState.m_Transform[3];
		vector end = pState.m_Transform[3] - Vector(0, modelSize, 0);

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;

		m_Hit = DayZPhysics.SphereCastBullet(start, end, 5.0, collisionLayerMask, m_Vehicle, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction);

		if (IsMissionClient() && m_EnableWind)
		{
			m_WindSpeedSync = GetGame().GetWeather().GetWind();

			if (m_Hit)
			{

				float distance = 0;

				if (ExpansionStatic.SurfaceIsWater(m_HitPosition))
				{
					distance = pState.m_Transform[3][1] - GetGame().SurfaceGetSeaLevel();
				}
				else
				{
					distance = pState.m_Transform[3][1] - m_HitPosition[1];
				}

				vector bbox[2];
				distance /= ((pState.m_BoundingRadius * 1.5) + 10.0);
				m_WindSpeedSync = m_WindSpeedSync * distance;
			}
			else
			{
				//float rnd = fad.m_Player.GetRandomGeneratorSyncManager().GetRandom01(RandomGeneratorSyncUsage.RGSGeneric);

				//m_HasDriver

				// turbulence?
			}
		}
		else
		{
			m_WindSpeedSync = "0 0 0";
		}
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
				GetGame().GetNoiseSystem().AddNoise(m_Vehicle, m_NoiseParams);
			}

			if (!m_Hit && !pState.m_HasDriver && !IsAutoHover())
			{
				m_MainRotorSpeedTarget = Math.RandomFloatInclusive(-1, 1);
				m_BackRotorSpeedTarget = Math.RandomFloatInclusive(-1, 1);

				m_CyclicSideTarget = Math.RandomFloatInclusive(-1, 1);
				m_CyclicForwardSpeed = Math.RandomFloatInclusive(-1, 1);
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
			if (pState.m_HasDriver)
			{
				m_MainRotorSpeedTarget = 0;
				m_BackRotorSpeedTarget = 0;

				m_CyclicForwardTarget *= 0.25;
				m_CyclicSideTarget *= 0.25;
			}
			else
			{
				m_MainRotorSpeedTarget = 0;
				m_BackRotorSpeedTarget = 0;

				m_CyclicForwardTarget = 0;
				m_CyclicSideTarget = 0;
			}
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

		change = Math.Clamp(Math.Min(m_MainRotorSpeedTarget, m_RotorSpeed) - m_MainRotorSpeed, -0.25 * pDt, 0.25 * pDt);
		m_MainRotorSpeed = Math.Clamp(m_MainRotorSpeed + change, -0.2, m_RotorSpeed);

		change = Math.Clamp(m_BackRotorSpeedTarget - m_BackRotorSpeed, -m_AntiTorqueSpeed * pDt, m_AntiTorqueSpeed * pDt);
		m_BackRotorSpeed = Math.Clamp(m_BackRotorSpeed + change, -m_AntiTorqueMax, m_AntiTorqueMax);

		if (pState.m_Exploded)
		{
			return;
		}

		if (pState.m_LinearVelocityMS.Length() > 0.05 || m_RotorSpeed != 0)
		{
			change = Math.Clamp(Math.Clamp(m_CyclicForwardTarget, -2, 2) - m_CyclicForward, -m_CyclicForwardSpeed * pDt, m_CyclicForwardSpeed * pDt);
			m_CyclicForward = Math.Clamp(m_CyclicForward + change, -m_CyclicForwardMax, m_CyclicForwardMax);

			change = Math.Clamp(Math.Clamp(m_CyclicSideTarget, -2, 2) - m_CyclicSide, -m_CyclicSideSpeed * pDt, m_CyclicSideSpeed * pDt);
			m_CyclicSide = Math.Clamp(m_CyclicSide + change, -m_CyclicSideMax, m_CyclicSideMax);

			// collective
			{
				// rotorSpeed^2
				// so rotorSpeed=0.0, 0.0*0.0 = 0.0 | rotorSpeed=0.5, 0.5*0.5 = 0.25 | rotorSpeed=1.0, 1.0*1.0 = 1.0
				// rotorSpeed is always clamped between 0.0-1.0

				float nearGround = 1;
				if (m_Hit)
				{
					nearGround = (pState.m_Transform[3][1] - m_HitPosition[1]) / pState.m_BoundingRadius;
					nearGround = Math.Clamp(1.5 - nearGround, 0.0, 1.0);
					nearGround = (nearGround * nearGround * 0.25) + 1.0;
				}

				float liftFactor = 0;

				int iMaxFlightEnvelope = m_MaxFlightEnvelope - 1;
				float fHoriSpeedRel = Vector(pState.m_LinearVelocityMS[0], 0, pState.m_LinearVelocityMS[2]).Length() / (pState.m_MaxSpeedMS * (iMaxFlightEnvelope / 10.0));
				float fCurrentEnvelope = iMaxFlightEnvelope * fHoriSpeedRel;
				int iCurrentEnvelopeFloor = Math.Floor(fCurrentEnvelope);

				if (iCurrentEnvelopeFloor >= iMaxFlightEnvelope)
					liftFactor = m_FlightEnvelope[iMaxFlightEnvelope];
				else if (iCurrentEnvelopeFloor >= 0)
				{
					float fCurrentEnvelopeFloor = m_FlightEnvelope[iCurrentEnvelopeFloor];
					float fCurrentEnvelopeFloorNext = m_FlightEnvelope[iCurrentEnvelopeFloor + 1];
					liftFactor = Math.Lerp(fCurrentEnvelopeFloor, fCurrentEnvelopeFloorNext, fCurrentEnvelope - iCurrentEnvelopeFloor);
				};

				float targetVelocity = (pState.m_LinearVelocityMS[1] + 3.0) - (18.0 * m_MainRotorSpeed * m_RotorSpeed * nearGround);
				if (targetVelocity < -5)
					targetVelocity = -5;

				targetVelocity *= pDt;
				float collectiveCoef = Math.Max((1.3 * liftFactor) - ((Math.SquareSign(targetVelocity) * 5.0) + (targetVelocity * 80.0)), 0);

				force += Vector(0, 1, 0) * collectiveCoef * pState.m_AltitudeLimiter * m_RotorSpeed * m_RotorSpeed * m_LiftForceCoef * pState.m_Mass;
			}

			// cyclic
			{
				float cyclicForce = m_CyclicForceCoef * pState.m_Mass * m_RotorSpeed * m_RotorSpeed * pState.m_AltitudeLimiter;

				torque[0] = torque[0] - (cyclicForce * m_CyclicForward * m_CyclicForwardCoef * pState.m_BoundingRadius);
				torque[2] = torque[2] + (cyclicForce * m_CyclicSide * m_CyclicSideCoef * pState.m_BoundingRadius);
			}

			// bank
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

			// tail
			{
				float tailRotorMalfunction = 0.0;
				if (m_EnableTailRotorDamage)
					tailRotorMalfunction = m_Vehicle.GetHealthLevel() / 5.0; // GetHealthLevel( "TailRotor" ) / 5.0;

				float tailRotorMalfunctionNeg = 1.0 - tailRotorMalfunction;
				float tailRotorMalfunctionTorque = 0.5 * tailRotorMalfunction * m_RotorSpeed * (m_RotorSpeed + 0.1);

				const float maxSpeedTailEffect = 1.0 / 80.0; // at ~200km/h, tail rotor has no effect
				float scaledSpeedFactor = 1.0 - Math.Min(Math.AbsFloat(pState.m_LinearVelocityMS[2] * maxSpeedTailEffect), 1.0);

				float bankForce = Math.Asin(m_Bank) * m_BankForceCoef * m_TailRotateFactor;
				float tailRotorForce = m_BackRotorSpeed * m_TailForceCoef * tailRotorMalfunctionNeg;

				float tailForce = (bankForce + tailRotorForce - tailRotorMalfunctionTorque) * 0.5 * scaledSpeedFactor * m_RotorSpeed * m_RotorSpeed * pState.m_BoundingRadius * pState.m_Mass;

				// apply torque to change the heading of the heli
				torque[1] = torque[1] - (pState.m_BoundingRadius * tailForce);

				// apply a little bit of torque on the side of the heli to simulate some roll
				torque[2] = torque[2] - (pState.m_BoundingRadius * 0.01 * tailForce);
			}

			// rotate to the direction of the speed
			//TODO: back to the drawing board...
			{
				float heliSpeedY = pState.m_LinearVelocityMS[1] * (pState.m_LinearVelocityMS[0] * 0.1);

				float rotateX = m_TailRotateFactor * ((pState.m_LinearVelocityMS[0] * -0.500) + (Math.SquareSign(pState.m_LinearVelocityMS[0]) * -0.010)) * pState.m_Mass;
				float rotateY = m_TailRotateFactor * ((heliSpeedY * -0.005) + (Math.SquareSign(heliSpeedY) * -0.0003)) * pState.m_Mass;

				vector heliRotateDir = Vector(0, 0, -pState.m_BoundingRadius) * Vector(rotateX, rotateY, 0);

				//torque[0] = torque[0] + heliRotateDir[0];
				//torque[1] = torque[1] + heliRotateDir[1];
				//torque[2] = torque[2] + heliRotateDir[2];
			}

			//! Linear Friction
			{
				vector friction;

				vector frictionSpeed = pState.m_LinearVelocityMS;
#ifdef EXPANSION_HELI_WIND
				vector windSpeed = m_WindSpeedSync.InvMultiply3(pState.m_Transform);
#else
				vector windSpeed = "0 0 0";
#endif

				frictionSpeed += windSpeed;

				// if the helicopter is turned on then more force is applied to create psuedo-friction
				float stabilizeXY = 0.4 + (m_RotorSpeed * m_RotorSpeed * 0.6);

				friction[0] = Math.SquareSign(frictionSpeed[0]) * pDt * m_LinearFrictionCoef[0] * pState.m_Mass * stabilizeXY;
				friction[1] = Math.SquareSign(frictionSpeed[1]) * pDt * m_LinearFrictionCoef[1] * pState.m_Mass * stabilizeXY;
				friction[2] = Math.SquareSign(frictionSpeed[2]) * pDt * m_LinearFrictionCoef[2] * pState.m_Mass;

				force -= friction * m_BodyFrictionCoef;
			}

			//! convert forces to worldspace
			{
				force = force.Multiply3(pState.m_Transform);
				torque = torque.Multiply3(pState.m_Transform);
			}

			//! Angular Friction
			{
				vector t_friction;

				t_friction = pState.m_AngularVelocity * pState.m_Mass * (m_RotorSpeed + 0.2) * ((m_AngularFrictionCoef) + (m_TailRotateFactor * 0.5));

				torque -= t_friction;
			}
		}

		// prevent helicopter from sinking in ocean
		{
			buoyancyForce = ExpansionPhysics.CalculateBuoyancyAtPosition(pState.m_Transform[3], 2.0, pState.m_Mass, 2.0, pState.m_LinearVelocity, isAboveWater);

			if (!isAboveWater && IsMissionHost())
			{
				float buoyancyAcceleration = buoyancyForce * pState.m_InvMass;
				if (buoyancyAcceleration > 1.0)
				{
					m_Vehicle.AddHealth("", "", -0.001 * buoyancyForce);
				}
			}

			force += Vector(0, buoyancyForce, 0);
		}

		pState.m_Force += force;
		pState.m_Torque += torque;
	}

	override void Animate(ExpansionPhysicsState pState)
	{
		if (!m_Initialized)
			return;

		float n;

		m_RotorAnimationPosition += m_RotorSpeed * pState.m_DeltaTime * 20.0 / (Math.PI * 2.0);

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

		if (m_RotorSpeed > 0.01)
		{
			m_Vehicle.SetAnimationPhase("rotor", m_RotorAnimationPosition);
			m_Vehicle.SetAnimationPhase("rearrotor", m_RotorAnimationPosition);
		}

		m_Vehicle.SetAnimationPhase("compasspointer", m_Vehicle.GetOrientation()[0] * Math.DEG2RAD);
		m_Vehicle.SetAnimationPhase("attitudeDiveRTD", m_Vehicle.GetDirection()[1]);
		m_Vehicle.SetAnimationPhase("attitudeBankRTD", m_Vehicle.GetOrientation()[2] / 360);

		m_Vehicle.SetAnimationPhase("cyclicForward", m_CyclicForwardTarget);
		m_Vehicle.SetAnimationPhase("cyclicAside", -m_CyclicSideTarget);

		//! Particles, only client-side
		if (!IsMissionClient())
			return;

		if (m_Hit && m_RotorSpeed > 0)
		{
			float size = (1 - m_HitFraction) * m_RotorSpeed * 4.0 * pState.m_BoundingRadius;

			if (ExpansionStatic.SurfaceIsWater(m_HitPosition))
			{
				m_HitPosition[1] = GetGame().SurfaceGetSeaLevel();

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

	override void NetworkSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_AutoHover);

		if (m_AutoHover)
		{
			ctx.Write(m_AutoHoverAltitude);
			ctx.Write(m_AutoHoverSpeed);
		}

		ctx.Write(m_IsFreeLook);

		ctx.Write(m_MouseVertSens);
		ctx.Write(m_MouseHorzSens);

		ctx.Write(m_WindSpeedSync);
	}

	override void NetworkRecieve(ParamsReadContext ctx)
	{
		ctx.Read(m_AutoHover);

		if (m_AutoHover)
		{
			ctx.Read(m_AutoHoverAltitude);
			ctx.Read(m_AutoHoverSpeed);
		}

		ctx.Read(m_IsFreeLook);

		ctx.Read(m_MouseVertSens);
		ctx.Read(m_MouseHorzSens);

		ctx.Read(m_WindSpeedSync);

		if (!m_EnableWind)
			m_WindSpeedSync = "0 0 0";
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
		instance.Add("Cyclic Forward Target", m_CyclicForwardSpeed);

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
