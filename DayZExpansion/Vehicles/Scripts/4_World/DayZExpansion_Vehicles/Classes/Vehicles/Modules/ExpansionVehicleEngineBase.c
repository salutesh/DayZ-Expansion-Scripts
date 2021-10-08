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

class ExpansionVehicleEngineBase : ExpansionVehicleModule
{
	int m_EngineIndex;
	int m_GearIndex;
	int m_ThrottleIndex;

	float m_Inertia;

	float m_RPMIdle;
	float m_RPMMin;
	float m_RPMClutch;
	float m_RPMRedline;
	float m_RPMMax;

	float m_RPM;
	float m_Torque;
	float m_Velocity;

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
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_RPM = m_Controller.m_RPM[m_EngineIndex];

		m_Torque = 0;
		if (m_Controller.m_State[m_EngineIndex])
		{
			if (m_RPM <= m_RPMMin)
				m_RPM += m_RPMMin * pState.m_DeltaTime / m_Inertia;

			m_Torque = CalculateTorque() * m_Controller.m_Throttle[m_ThrottleIndex] * m_Inertia * pState.m_DeltaTime;
			m_RPM += m_Torque * pState.m_DeltaTime;// * pState.m_Mass;
		}

		m_Velocity = m_RPM * Math.PI / 60.0;
		m_Velocity -= m_Velocity * pState.m_DeltaTime / m_Inertia;
		m_Velocity = Math.Max(m_Velocity, 0);
		m_RPM = (m_Velocity * 60.0) / Math.PI;

		m_Controller.m_RPM[m_EngineIndex] = m_RPM;
		m_Controller.m_Torque[m_EngineIndex] = m_Torque;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("RPM", m_RPM);
		instance.Add("Torque", m_Torque);
		instance.Add("Velocity", m_Velocity);

		return true;
	}
#endif

	static float Lerp(float a, float b, float t)
	{
		return (a * (1.0 - t)) + (b * t);
	}

	static float SmoothStep(float t)
	{
		return t * t * (3.0 - (2.0 * t));
	}

	static float FromRPM(float rpm)
	{
		return rpm * Math.PI / 30.0;
	}

	static float ToRPM(float rot)
	{
		return Math.AbsFloat(rot) * 30.0 / Math.PI;
	}

	float CalculateTorque()
	{
		return 0;
	}
};
