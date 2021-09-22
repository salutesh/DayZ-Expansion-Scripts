/**
 * ExpansionVehicleAxle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleAxle : ExpansionVehicleModule
{
	string m_Name;
	int m_Index;

	autoptr array<ref ExpansionVehicleWheel> m_Wheels = new array<ref ExpansionVehicleWheel>();

	float m_WheelRadius;

	float m_MaxSteeringAngle;

	int m_ControlIndex;
	int m_EngineIndex;

	float m_BrakeBias;
	float m_BrakeForce;
	float m_FinalRatio;
	float m_WheelHubMass;
	float m_WheelHubRadius;

	float m_SwayBar;
	float m_Stiffness;
	float m_Compression;
	float m_Damping;

	float m_TravelMaxUp;
	float m_TravelMaxDown;
	float m_TravelMax;

	void ExpansionVehicleAxle(EntityAI pVehicle, string pName, int pIndex)
	{
		m_Name = pName;
		m_Index = pIndex;

		string path = "";

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " controller";
		if (GetGame().ConfigIsExisting(path))
			m_ControlIndex = GetGame().ConfigGetInt(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " engine";
		if (GetGame().ConfigIsExisting(path))
			m_EngineIndex = GetGame().ConfigGetInt(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " maxSteeringAngle";
		m_MaxSteeringAngle = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " finalRatio";
		m_FinalRatio = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " brakeBias";
		m_BrakeBias = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " brakeForce";
		m_BrakeForce = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " wheelHubMass";
		m_WheelHubMass = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " wheelHubRadius";
		m_WheelHubRadius = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension swayBar";
		m_SwayBar = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension stiffness";
		m_Stiffness = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension compression";
		m_Compression = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension damping";
		m_Damping = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension travelMaxUp";
		m_TravelMaxUp = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " Suspension travelMaxDown";
		m_TravelMaxDown = GetGame().ConfigGetFloat(path);

		m_TravelMax = m_TravelMaxUp + m_TravelMaxDown;
	}

	override void Init()
	{
		super.Init();

		if (m_EngineIndex == -1)
		{
			m_FinalRatio = 0;
			return;
		}

		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);
		auto engineType = vehicle.m_Engines[m_EngineIndex].m_Type;

		switch (engineType)
		{
		case ExpansionVehicleEngineType.RWD:
			if (m_Index != vehicle.GetAxleCount() - 1)
				m_FinalRatio = 0;
			break;
		case ExpansionVehicleEngineType.FWD:
			if (m_Index != 0)
				m_FinalRatio = 0;
			break;
		}
	}

	ExpansionVehicleWheel AddWheel(string pName)
	{
		ExpansionVehicleWheel wheel = new ExpansionVehicleWheel(m_Vehicle, pName, this);

		m_Wheels.Insert(wheel);

		return wheel;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		return true;
	}
#endif

	void UpdateWheelRadius()
	{
		for (int i = 0; i < m_Wheels.Count(); ++i)
			m_WheelRadius += m_Wheels[i].m_Radius;

		m_WheelRadius = m_WheelRadius / m_Wheels.Count();
	}

	float GetRPM()
	{
		float highest = 0;

		for (int i = 0; i < m_Wheels.Count(); ++i)
			if (m_Wheels[i].m_RPM > highest)
				highest = m_Wheels[i].m_RPM;

		return highest;
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float torque = 0;
		if (m_EngineIndex != -1)
			torque = m_Controller.m_Torque[m_EngineIndex] * m_FinalRatio;

		float brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
		float steering = m_Controller.m_Yaw * m_MaxSteeringAngle;
		float ratio = 1.0 / m_Wheels.Count();
		for (int i = 0; i < m_Wheels.Count(); ++i)
		{
			m_Wheels[i].m_WheelTorque = torque * ratio;
			m_Wheels[i].m_BrakeTorque = brake;
			m_Wheels[i].m_Steering = steering;
		}
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		if (m_FinalRatio != 0.0)
			m_Controller.m_RPM[m_EngineIndex] = GetRPM() / m_FinalRatio;
	}
};

class ExpansionVehicleOneWheelAxle : ExpansionVehicleAxle
{
	ExpansionVehicleWheel m_Center;

	override ExpansionVehicleWheel AddWheel(string pName)
	{
		m_Center = super.AddWheel(pName);

		return m_Center;
	}

	override float GetRPM()
	{
		return m_Center.m_RPM;
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float torque = 0;
		if (m_EngineIndex != -1)
			torque = m_Controller.m_Torque[m_EngineIndex] * m_FinalRatio;

		m_Center.m_WheelTorque = torque;
		m_Center.m_BrakeTorque = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
		m_Center.m_Steering = m_Controller.m_Yaw * m_MaxSteeringAngle;
	}
};

class ExpansionVehicleTwoWheelAxle : ExpansionVehicleAxle
{
	float m_AntiRollForce;

	ExpansionVehicleWheel m_Left;
	ExpansionVehicleWheel m_Right;

	bool m_RawSteering = true;

	float m_TurnRadius;
	float m_WheelBase;
	float m_WheelOffset;

	float Sign(float number)
	{
		if (number < 0)
			return -1;
		return 1;
	}

	override void Init()
	{
		super.Init();

		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);

		int lastIndex = vehicle.GetAxleCount() - 1;

		if (m_Index != 0 && m_Index != lastIndex)
			return;

		m_TurnRadius = Math.AbsFloat(Math.DEG2RAD * m_MaxSteeringAngle) / (Math.PI2);
		if (m_TurnRadius == 0.0)
		{
			return;
		}

		vector frontWheel;
		vector backWheel;

		if (m_Index == 0)
		{
			frontWheel = m_Wheels[0].m_InitialTransform[3];
			backWheel = vehicle.GetAxle(lastIndex).m_Wheels[0].m_InitialTransform[3];
		}
		else
		{
			frontWheel = vehicle.GetAxle(lastIndex).m_Wheels[0].m_InitialTransform[3];
			backWheel = m_Wheels[0].m_InitialTransform[3];
		}

		m_WheelBase = Math.AbsFloat(backWheel[2] - frontWheel[2]);
		m_WheelOffset = Math.AbsFloat(backWheel[0]);

		m_RawSteering = false;
	}

	override ExpansionVehicleWheel AddWheel(string pName)
	{
		if (pName == "Left")
		{
			m_Left = super.AddWheel(pName);
			return m_Left;
		}
		else if (pName == "Right")
		{
			m_Right = super.AddWheel(pName);
			return m_Right;
		}

		return super.AddWheel(pName);
	}

	override float GetRPM()
	{
		return Math.Max(m_Left.m_RPM, m_Right.m_RPM);
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float sp = pState.GetModelVelocityAt("0 0 0")[2];

		float torque = 0;
		if (m_EngineIndex != -1)
			torque = m_Controller.m_Torque[m_EngineIndex] * m_FinalRatio;
			
		float brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
		float steering = m_Controller.m_Yaw;

		m_AntiRollForce = Math.Clamp(sp * steering * 0.01 * pState.m_DeltaTime, -1, 1) * m_SwayBar;

		m_Left.m_WheelTorque = torque * 0.5;
		m_Right.m_WheelTorque = torque * 0.5;
		m_Left.m_BrakeTorque = brake;
		m_Right.m_BrakeTorque = brake;

		m_Left.m_Steering = steering * m_MaxSteeringAngle;
		m_Right.m_Steering = steering * m_MaxSteeringAngle;

		if (!m_RawSteering)
		{
			float leftBackWheel = m_WheelOffset * -Sign(steering);
			float rightBackWheel = m_WheelOffset * Sign(steering);

			float leftFrontWheelSteer = Math.Atan2(m_WheelBase / (m_TurnRadius + leftBackWheel), 1);
			float rightFrontWheelSteer = Math.Atan2(m_WheelBase / (m_TurnRadius + rightBackWheel), 1);
			m_Left.m_Steering = Math.AbsFloat(Math.RAD2DEG * leftFrontWheelSteer * 2.0 / Math.PI) * steering * Sign(m_MaxSteeringAngle);
			m_Right.m_Steering = Math.AbsFloat(Math.RAD2DEG * rightFrontWheelSteer * 2.0 / Math.PI) * steering * Sign(m_MaxSteeringAngle);
		}
	}

	override void Simulate(ExpansionPhysicsState pState)
	{
		float coef = pState.m_DeltaTime * pState.m_Mass;

		float l = Math.Max(0, -m_AntiRollForce);
		float r = Math.Max(0, m_AntiRollForce);

		vector w1 = m_Left.m_ContactNormal * coef * l;
		vector w2 = m_Right.m_ContactNormal * coef * r;
		vector p1 = m_Left.m_ContactPosition;
		vector p2 = m_Right.m_ContactPosition;

		vector impulse;
		vector impulseTorque;

		impulse += w1;
		impulseTorque += p1 * w1;

		impulse += w2;
		impulseTorque += p2 * w2;

		//pImpulse += impulse.Multiply3( pState.m_Transform );
		//pImpulseTorque += impulseTorque.Multiply3( pState.m_Transform );

		super.Simulate(pState);
	}
};
