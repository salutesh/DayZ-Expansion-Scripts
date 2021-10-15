/*
 * ExpansionVehicleEngineBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleEngineType
{
	CAR,
	POINTS
};

static ExpansionVehicleEngineBase Expansion_CreateEngine(EntityAI vehicle, string path)
{
	int type = ExpansionVehicleEngineType.CAR;
	string typePath = path + " type";

	if (GetGame().ConfigIsExisting(typePath))
	{
		switch (GetGame().ConfigGetType(typePath))
		{
		case CT_STRING:
			string typeName;
			GetGame().ConfigGetText(typePath, typeName);
			typeName.ToUpper();
			type = typename.StringToEnum(ExpansionVehicleEngineType, typeName);
			break;
		default:
			type = GetGame().ConfigGetInt(typePath);
			break;
		}
	}

	switch (type)
	{
	case ExpansionVehicleEngineType.CAR:
		return new ExpansionVehicleCarEngine(vehicle, path);
	case ExpansionVehicleEngineType.POINTS:
		return new ExpansionVehicleEngine(vehicle, path);
	}

	return null;
}

class ExpansionVehicleEngineBase : ExpansionVehicleRotational
{
	int m_EngineIndex;
	int m_GearIndex;
	int m_ThrottleIndex;

	bool m_State;

	float m_RPMIdle;
	float m_RPMMin;
	float m_RPMClutch;
	float m_RPMRedline;
	float m_RPMMax;

	float m_Throttle;
	float m_ThrottleRange;
	float m_ThrottleIdle;

	ExpansionVehicleGearbox m_Gearbox;

	void ExpansionVehicleEngineBase(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " gear";
		if (GetGame().ConfigIsExisting(path))
			m_GearIndex = GetGame().ConfigGetInt(path);

		path = rootPath + " throttle";
		if (GetGame().ConfigIsExisting(path))
			m_ThrottleIndex = GetGame().ConfigGetInt(path);

		path = rootPath + " inertia";
		m_Inertia = 0.9;
		if (GetGame().ConfigIsExisting(path))
			m_Inertia = GetGame().ConfigGetFloat(path);

		path = rootPath + " rpmIdle";
		m_RPMIdle = 0;
		if (GetGame().ConfigIsExisting(path))
			m_RPMIdle = GetGame().ConfigGetFloat(path);

		path = rootPath + " rpmMin";
		m_RPMMin = 0;
		if (GetGame().ConfigIsExisting(path))
			m_RPMMin = GetGame().ConfigGetFloat(path);

		path = rootPath + " rpmClutch";
		m_RPMClutch = 0;
		if (GetGame().ConfigIsExisting(path))
			m_RPMClutch = GetGame().ConfigGetFloat(path);

		path = rootPath + " rpmRedline";
		m_RPMRedline = 0;
		if (GetGame().ConfigIsExisting(path))
			m_RPMRedline = GetGame().ConfigGetFloat(path);

		path = rootPath + " rpmMax";
		m_RPMMax = 0;
		if (GetGame().ConfigIsExisting(path))
			m_RPMMax = GetGame().ConfigGetFloat(path);

		m_Position = ExpansionVehiclesStatic.GetCenterSelection(m_Vehicle, "geometryView", "dmgzone_engine");
	}

	override void Init()
	{
		super.Init();

		if (m_Vehicle.IsInherited(CarScript))
			Init_CarScript();
		else
			Init_ExpansionVehicle();

		m_Component = m_Gearbox;
		m_Gearbox.m_Parent = this;

		m_ThrottleIdle = m_RPMMin / m_RPMMax;
		m_ThrottleRange = 1.0 - m_ThrottleIdle;
	}

	void Init_CarScript()
	{
		auto vehicle = CarScript.Cast(m_Vehicle);

		if (m_GearIndex >= 0)
			m_Gearbox = vehicle.m_Gearboxes[m_GearIndex];

		if (!m_Gearbox)
		{
			ExpansionVehicleGearbox gBox = new ExpansionVehicleGearbox(m_Vehicle, "");
			vehicle.AddModule(gBox);
			m_Gearbox = gBox;
			m_GearIndex = gBox.m_GearIndex;
		}
	}

	void Init_ExpansionVehicle()
	{
		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);

		if (m_GearIndex >= 0)
			m_Gearbox = vehicle.m_Gearboxes[m_GearIndex];

		if (!m_Gearbox)
		{
			ExpansionVehicleGearbox gBox = new ExpansionVehicleGearbox(m_Vehicle, "");
			vehicle.AddModule(gBox);
			m_Gearbox = gBox;
			m_GearIndex = gBox.m_GearIndex;
		}
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Torque = 0;

		m_Throttle = ((m_Controller.m_Throttle[m_ThrottleIndex] * m_ThrottleRange) + m_ThrottleIdle) * m_Controller.m_State[m_EngineIndex];
		m_Throttle = Math.Clamp(m_Throttle, 0, 1);

		float minTorque = -FromRPM(m_RPM);

		if (m_Throttle > 0 && m_RPM < m_RPMMax)
		{
			m_Torque = (CalculateTorque(pState) - minTorque) * m_Throttle;
		}

		m_Torque += minTorque;

		// apply a starting torque
		if (m_Controller.m_State[m_EngineIndex] && m_State != m_Controller.m_State[m_EngineIndex])
		{
			m_Torque += FromRPM(m_RPMIdle);

			if (m_RPM >= m_RPMIdle)
			{
				m_State = true;
			}
		}
		else if (!m_Controller.m_State[m_EngineIndex])
		{
			m_State = false;
		}

		m_Torque -= m_Gearbox.m_Clutch * m_Gearbox.m_MaxClutchTorque;

		if (m_Velocity + m_Torque < 0)
		{
			m_Torque = 0;
			m_Velocity = 0;
		}

		super.PreSimulate(pState);

		m_Controller.m_Torque[m_EngineIndex] = m_Torque;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Throttle", m_Throttle);
		instance.Add("ThrottleRange", m_ThrottleRange);
		instance.Add("ThrottleIdle", m_ThrottleIdle);

		return true;
	}
#endif

	static float SmoothStep(float t)
	{
		return t * t * (3.0 - (2.0 * t));
	}

	float CalculateTorque(ExpansionPhysicsState pState)
	{
		return 0;
	}
};
