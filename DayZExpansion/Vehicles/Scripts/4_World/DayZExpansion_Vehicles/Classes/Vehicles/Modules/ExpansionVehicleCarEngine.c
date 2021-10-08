/*
 * ExpansionVehicleCarEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleCarEngine : ExpansionVehicleEngineBase
{
	float m_TorqueMax;
	float m_TorqueRPM;

	float m_Steepness;

	float m_PowerMax;
	float m_PowerRPM;

	void ExpansionVehicleCarEngine(EntityAI vehicle, string rootPath)
	{
		string path;

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
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		return true;
	}
#endif

	override float CalculateTorque()
	{
		if (m_RPM >= m_PowerRPM && m_RPM <= m_RPMMax)
		{
			return SmoothStep(1.0 - ((m_RPM - m_PowerRPM) / (m_RPMMax - m_PowerRPM))) * (1000.0 * m_PowerMax) / FromRPM(m_PowerRPM);
		}
		else if (m_RPM >= m_TorqueRPM && m_RPM < m_PowerRPM)
		{
			return Lerp(m_TorqueMax, 1000.0 * m_PowerMax / FromRPM(m_PowerRPM), (m_RPM - m_TorqueRPM) / (m_PowerRPM - m_TorqueRPM));
		}
		else if (m_RPM >= m_RPMIdle && m_RPM < m_TorqueRPM)
		{
			float a = m_Steepness * FromRPM(m_RPM) / FromRPM(m_TorqueRPM);
			return m_TorqueMax * a * ((1.0 + (1.0 / m_Steepness)) / (1.0 + a));
		}

		return 0;
	}
};
