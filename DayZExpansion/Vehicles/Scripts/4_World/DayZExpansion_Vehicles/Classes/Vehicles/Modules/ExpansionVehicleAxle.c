/**
 * ExpansionVehicleAxle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	float m_BrakeBias;  //! 1.18 and lower
	float m_BrakeForce;  //! 1.18 and lower
	float m_MaxBrakeTorque;  //! 1.19
	float m_MaxHandbrakeTorque;  //! 1.19
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
		if (GetGame().ConfigIsExisting(path))
		{
			m_MaxSteeringAngle = GetGame().ConfigGetFloat(path);
		}
		else
		{
			path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Steering maxSteeringAngle";  //! 1.19
			m_MaxSteeringAngle = GetGame().ConfigGetFloat(path);
		}
		path = "CfgVehicles " + m_Vehicle.GetType() + " SimulationModule Axles " + pName + " maxBrakeTorque";
		m_MaxBrakeTorque = GetGame().ConfigGetFloat(path);
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
			{
				m_Ratio = 0;
			}
			break;
		case ExpansionVehicleDifferentialType.FWD:
			if (m_Index != 0)
			{
				m_Ratio = 0;
			}
			break;
		}

		// TODO: control link could be another differential
		if (m_Ratio != 0.0 && m_ControlIndex >= 0)
		{
			vehicle.m_Gearboxes[m_ControlIndex].AddComponent(this);
		}
	}

	ExpansionVehicleWheel AddWheel(string pName)
	{
		ExpansionVehicleWheel wheel = new ExpansionVehicleWheel(m_Vehicle, pName, this);

		AddComponent(wheel);

		m_Wheels.Insert(wheel);

		return wheel;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Control Index", m_ControlIndex);

		return true;
	}
#endif
};
