/*
 * ExpansionVehicleBikeBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

/**
 * @class		ExpansionVehicleBikeBase
 * @brief		This class handle bicycle simulation
 */
class ExpansionVehicleBikeBase extends ExpansionVehicleBase
{
	float m_AirControlForwardCoef;
	float m_AirControlRightCoef;

	bool m_AirControl;
	float m_AirControlForward;
	float m_AirControlRight;

	bool m_GroundControl;
	float m_GroundStabilizer;
	float m_FrontSuspFraction;
	float m_BackSuspFraction;

	void ExpansionVehicleBikeBase()
	{
		m_AirControlForwardCoef = 1.0;
		m_AirControlRightCoef = 1.0;

		string path;

		if (m_Engines.Count() == 0)
		{
			path = "CfgVehicles " + GetType() + " SimulationModule Engine";
			AddModule(Expansion_CreateEngine(this, path));
		}

		if (m_Gearboxes.Count() == 0)
		{
			path = "CfgVehicles " + GetType() + " SimulationModule Gearbox";
			AddModule(Expansion_CreateGearbox(this, path));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Throttle";
		AddModule(new ExpansionVehicleCarThrottle(this, path));

		path = "CfgVehicles " + GetType() + " SimulationModule Steering";
		AddModule(new ExpansionVehicleBikeSteering(this, path));
	}

	override ExpansionController GetExpansionController()
	{
		return m_Controller;
	}

	override ExpansionController GetController()
	{
		return m_Controller;
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType(int posIdx)
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

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return true;
	}

	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return false;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return false;
	}

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return false;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return false;
	}

	override bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return false;
	}

	override vector GetTransportCameraOffset()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraOffset");
#endif

		return "0 0 0";
	}

	override float GetTransportCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetTransportCameraDistance");
#endif

		return 3;
	}

	override bool IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return true;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Front Susp Fraction", m_FrontSuspFraction);
		instance.Add("Back Susp Fraction", m_BackSuspFraction);

		instance.Add("Air Control", m_AirControl);
		instance.Add("Ground Control", m_GroundControl);

		instance.Add("Ground Stabilizer", m_GroundStabilizer);

		return true;
	}
#endif

	override void OnPreSimulation(float pDt)
	{
		super.OnPreSimulation(pDt);

		float fMax = m_Axles[0].m_TravelMax;
		float bMax = m_Axles[1].m_TravelMax;
		m_FrontSuspFraction = (fMax - m_Wheels[0].m_SuspensionLength) - m_Axles[0].m_TravelMaxDown;
		m_BackSuspFraction = (bMax - m_Wheels[1].m_SuspensionLength) - m_Axles[1].m_TravelMaxDown;

		m_FrontSuspFraction = (fMax - m_FrontSuspFraction) * (fMax + m_FrontSuspFraction) / (fMax * fMax);
		m_BackSuspFraction = (bMax - m_BackSuspFraction) * (bMax + m_BackSuspFraction) / (bMax * bMax);

		float yUp = vector.Dot(m_State.m_Transform[1], "0 1 0");

		m_GroundStabilizer = Math.Max(m_FrontSuspFraction, m_BackSuspFraction); //! we can lean so long 1 wheel is on the ground

		m_AirControl = m_GroundStabilizer < 0.1;
		m_GroundControl = yUp > 0.5 && m_GroundStabilizer > 0.1;
	}

	override void OnAnimationUpdate(float pDt)
	{
		super.OnAnimationUpdate(pDt);

		SetAnimationPhase("steeringwheel", GetController().GetSteering());
	}

	override void OnSimulation(ExpansionPhysicsState pState)
	{
		super.OnSimulation(pState);

		float absForwardSpeed = Math.AbsFloat(pState.m_LinearVelocityMS[2]) + 0.1;

		if (m_GroundControl) //! we are on the ground
		{
			vector fWheelNormal = m_Wheels[0].m_ContactNormal * m_FrontSuspFraction;
			vector bWheelNormal = m_Wheels[1].m_ContactNormal * m_BackSuspFraction;
			vector terrainSurface = fWheelNormal + bWheelNormal;
			terrainSurface = terrainSurface.Normalized().Multiply3(pState.m_Transform);

			vector estDirUp = pState.EstimateDirection(0.1, 1);

			float leanAmount = Math.Clamp(GetController().GetSteering(), -0.16, 0.16);
			float leanOnX = leanAmount * absForwardSpeed * 0.001;
			leanOnX = Math.Clamp(leanOnX, -0.8, 0.8);

			vector upDirWanted = Vector(-terrainSurface[0], 1.0, -terrainSurface[2]).Normalized();

			vector upFromForward[4];
			Math3D.DirectionAndUpMatrix(GetDirection(), "0 1 0", upFromForward);
			upDirWanted = upDirWanted.InvMultiply3(upFromForward);
			upDirWanted[0] = leanOnX;
			upDirWanted.Normalize();
			upDirWanted = upDirWanted.Multiply3(upFromForward);

			vector applyPosition = Vector(0.0, 1.0, 0.0);

			vector stabilize = (upDirWanted - estDirUp);
			stabilize = stabilize.Normalized() * stabilize.LengthSq();
			const float maxStabCoef = 0.8;
			if (stabilize.LengthSq() > maxStabCoef * maxStabCoef)
				stabilize = stabilize.Normalized() * maxStabCoef;

			stabilize = stabilize * 50.0 * pState.m_Mass * m_GroundStabilizer;

			stabilize = stabilize.InvMultiply3(pState.m_Transform);
			stabilize[2] = 0;
			pState.m_Torque += (applyPosition * stabilize).Multiply3(pState.m_Transform);
		}
		else if (m_AirControl) //! we are in the air
		{
			//float airControlForward = m_AirControlForward * m_AirControlForwardCoef;
			//float airControlRight = m_AirControlRight * m_AirControlRightCoef;
			//
			//vector airControl = Vector( airControlRight, 0, -airControlForward ) * m_State.m_Mass * 0.25;
			//
			//torque += ( Vector( 0, 1, 0 ) * airControl ).Multiply3( m_State.m_Transform );
		}
	}
};
