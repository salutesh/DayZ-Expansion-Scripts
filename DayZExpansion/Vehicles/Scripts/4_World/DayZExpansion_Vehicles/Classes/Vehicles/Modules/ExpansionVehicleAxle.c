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

enum ExpansionVehicleDifferentialType
{
	RWD,
	FWD,
	AWD
};

class ExpansionVehicleAxle : ExpansionVehicleRotational
{
	string m_Name;
	int m_Index;

	autoptr array<ref ExpansionVehicleWheel> m_Wheels = new array<ref ExpansionVehicleWheel>();

	float m_MaxSteeringAngle;

	int m_ControlIndex;

	float m_EffectiveRatio;

	float m_Brake;

	float m_BrakeBias;
	float m_BrakeForce;
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

		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " maxSteeringAngle";
		m_MaxSteeringAngle = GetGame().ConfigGetFloat(path);
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " finalRatio";
		m_Ratio = GetGame().ConfigGetFloat(path);
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

		int count;
		int i;

		count = m_Wheels.Count();
		for (i = 0; i < count; i++)
		{
			m_Position = m_Position + m_Wheels[i].m_Position;
		}

		m_Position = m_Position * (1.0 / count);

		if (m_ControlIndex == -1)
		{
			m_Ratio = 0;
			return;
		}

		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);
		auto engineType = vehicle.m_DifferentialType;

		switch (engineType)
		{
		case ExpansionVehicleDifferentialType.RWD:
			if (m_Index != vehicle.GetAxleCount() - 1)
				m_Ratio = 0;
			break;
		case ExpansionVehicleDifferentialType.FWD:
			if (m_Index != 0)
				m_Ratio = 0;
			break;
		}

		if (m_Ratio == 0)
		{
			m_ControlIndex = -1;
		}

		if (m_ControlIndex >= 0)
		{
			m_Parent = vehicle.m_Gearboxes[m_ControlIndex];
			m_Parent.m_Component = this;
		}
	}

	ExpansionVehicleWheel AddWheel(string pName)
	{
		ExpansionVehicleWheel wheel = new ExpansionVehicleWheel(m_Vehicle, pName, this);

		wheel.m_Parent = this;

		m_Wheels.Insert(wheel);

		return wheel;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Index", m_ControlIndex);

		return true;
	}
#endif

	/*
	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float brake = 0.0;
		float gear = 1.0;
		if (m_ControlIndex != -1)
		{
			brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
			gear = m_Controller.m_Ratio[m_ControlIndex];
		}

		float torque = 0.0;
		if (m_EngineIndex != -1)
		{
			torque = m_Controller.m_Torque[m_EngineIndex] * m_Ratio * gear;
		}

		float steering = m_Controller.m_Yaw * m_MaxSteeringAngle;
		for (int i = 0; i < m_Wheels.Count(); ++i)
		{
			m_Wheels[i].m_WheelTorque = torque;
			m_Wheels[i].m_BrakeTorque = brake;
			m_Wheels[i].m_Steering = steering;
		}
	}
*/
};

class ExpansionVehicleOneWheelAxle : ExpansionVehicleAxle
{
	ExpansionVehicleWheel m_Center;

	override ExpansionVehicleWheel AddWheel(string pName)
	{
		m_Center = super.AddWheel(pName);

		return m_Center;
	}
	/*
	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float brake = 0.0;
		float gear = 1.0;
		if (m_ControlIndex != -1)
		{
			brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;
			gear = m_Controller.m_Ratio[m_ControlIndex];
		}

		float torque = 0.0;
		if (m_EngineIndex != -1)
		{
			torque = m_Controller.m_Torque[m_EngineIndex] * m_Ratio * gear;
		}

		m_Center.m_WheelTorque = torque;
		m_Center.m_BrakeTorque = brake;
		m_Center.m_Steering = m_Controller.m_Yaw * m_MaxSteeringAngle;
	}
*/
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

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float sp = pState.GetModelVelocityAt("0 0 0")[2];

		m_Brake = 0.0;
		if (m_ControlIndex != -1)
			m_Brake = m_Controller.m_Brake[m_ControlIndex] * m_BrakeForce * m_BrakeBias;

		float steering = m_Controller.m_Yaw;

		m_AntiRollForce = Math.Clamp(sp * steering * 0.01 * pState.m_DeltaTime, -1, 1) * m_SwayBar;

		float j0 = 0;
		float j1 = 1;
		float j2 = 1;

		if (m_Parent)
			j0 = m_Parent.m_Inertia;

		float jt = j0 + j1 + j2;

		float m0 = m_Torque * m_Ratio;
		float m1 = m_Left.m_TractionTorque;
		float m2 = m_Right.m_TractionTorque;

		float mt = m0 + m1 + m2;

		// axles must be running at the same rpm
		m_Acceleration = mt / jt;

		m_Left.m_Torque = m_Acceleration;
		m_Right.m_Torque = m_Acceleration;

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
};
