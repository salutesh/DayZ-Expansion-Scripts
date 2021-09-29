/*
 * ExpansionVehicleEngine.c
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
	RWD,
	FWD,
	AWD
};

class ExpansionVehicleEngine : ExpansionVehicleModule
{
	int m_EngineIndex;
	int m_GearIndex;
	int m_ThrottleIndex;

	float m_Inertia;

	float m_TorqueMax;
	float m_TorqueRPM;

	float m_Steepness;

	float m_PowerMax;
	float m_PowerRPM;

	float m_RPMIdle;
	float m_RPMMin;
	float m_RPMClutch;
	float m_RPMRedline;
	float m_RPMMax;

	float m_RPM;
	float m_Torque;
	float m_Velocity;

	ExpansionVehicleEngineType m_Type;
	int m_Stage;

	void ExpansionVehicleEngine(EntityAI vehicle, string rootPath, ExpansionVehicleEngineType type = ExpansionVehicleEngineType.AWD)
	{
		m_Type = type;

		string path;

		path = rootPath + " type";
		if (GetGame().ConfigIsExisting(path))
		{
			int configType = GetGame().ConfigGetType(path);
			if (configType == CT_STRING)
			{
				string typeName;
				GetGame().ConfigGetText(path, typeName);
				typeName.ToUpper();
				m_Type = typename.StringToEnum(ExpansionVehicleEngineType, typeName);
			}
			else
			{
				m_Type = GetGame().ConfigGetInt(path);
			}
		}

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

		path = rootPath + " torqueMax";
		m_TorqueMax = 0;
		if (GetGame().ConfigIsExisting(path))
			m_TorqueMax = GetGame().ConfigGetFloat(path);

		path = rootPath + " torqueRpm";
		m_TorqueRPM = 0;
		if (GetGame().ConfigIsExisting(path))
			m_TorqueRPM = GetGame().ConfigGetFloat(path);

		path = rootPath + " steepness";
		m_Steepness = 1;
		if (GetGame().ConfigIsExisting(path))
			m_Steepness = GetGame().ConfigGetFloat(path);

		path = rootPath + " powerMax";
		m_PowerMax = 0;
		if (GetGame().ConfigIsExisting(path))
			m_PowerMax = GetGame().ConfigGetFloat(path);

		path = rootPath + " powerRpm";
		m_PowerRPM = 0;
		if (GetGame().ConfigIsExisting(path))
			m_PowerRPM = GetGame().ConfigGetFloat(path);

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
		if (!m_Controller.m_State[m_EngineIndex])
		{
			m_RPM = 0;
			m_Torque = 0;
			m_Controller.m_Torque[m_EngineIndex] = 0.0;
			return;
		}

		float gear = 1.0;
		float invGear = 0.0;
		if (m_GearIndex >= 0)
			gear = m_Controller.m_Ratio[m_GearIndex];

		if (gear != 0.0)
		{
			invGear = 1.0 / gear;

			m_RPM = Math.Max(m_Controller.m_RPM[m_EngineIndex] * gear, m_RPMIdle);
		}

		m_Velocity = m_RPM * Math.PI / 60.0;
		m_Torque = LookupTorque(m_RPM);

		if (gear == 0.0)
		{
			m_Velocity -= m_Inertia * m_Velocity * pState.m_DeltaTime;
			m_RPM = Math.Max((m_Velocity * 60.0) / Math.PI, m_RPMIdle);
		}

		m_Controller.m_Torque[m_EngineIndex] = m_Torque * m_Controller.m_Throttle[m_ThrottleIndex] * m_Inertia * gear * pState.m_DeltaTime;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		//instance.Add("Stage", m_Stage);
		//instance.Add("Power  Max", m_PowerMax);
		//instance.Add("Torque Max", m_TorqueMax);
		//instance.Add("Max    RPM", m_RPMMax);
		//instance.Add("Power  RPM", m_PowerRPM);
		//instance.Add("Torque RPM", m_TorqueRPM);
		//instance.Add("RPM    Idle", m_RPMIdle);

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

	float LookupTorque(float rpm)
	{
		if (rpm >= m_PowerRPM && rpm <= m_RPMMax)
		{
			m_Stage = 3;

			return SmoothStep(1.0 - ((rpm - m_PowerRPM) / (m_RPMMax - m_PowerRPM))) * (1000.0 * m_PowerMax) / FromRPM(m_PowerRPM);
		}
		else if (rpm >= m_TorqueRPM && rpm < m_PowerRPM)
		{
			m_Stage = 2;

			return Lerp(m_TorqueMax, 1000.0 * m_PowerMax / FromRPM(m_PowerRPM), (rpm - m_TorqueRPM) / (m_PowerRPM - m_TorqueRPM));
		}
		else if (rpm >= m_RPMIdle && rpm < m_TorqueRPM)
		{
			m_Stage = 1;

			float a = m_Steepness * FromRPM(rpm) / FromRPM(m_TorqueRPM);
			return m_TorqueMax * a * ((1.0 + (1.0 / m_Steepness)) / (1.0 + a));
		}

		m_Stage = 0;

		return 0;
	}
};
