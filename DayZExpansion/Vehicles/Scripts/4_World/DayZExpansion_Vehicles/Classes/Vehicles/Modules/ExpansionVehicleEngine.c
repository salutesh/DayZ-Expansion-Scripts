/*
 * ExpansionVehicleEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleEngine : ExpansionVehicleEngineBase
{
	ref array<float> m_RPMs = new array<float>();
	ref array<float> m_Torques = new array<float>();

	int m_Count;

	int m_Index;

	void ExpansionVehicleEngine(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " points";
		array<float> points = new array<float>();
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, points);

		m_Count = (points.Count() / 2) + 2;
		m_RPMs.Resize(m_Count);
		m_Torques.Resize(m_Count);

		m_RPMs[0] = 0;
		m_Torques[0] = 0;

		for (int i = 1; i < m_Count - 1; i++)
		{
			m_RPMs[i] = points[(i * 2) + 0];
			m_Torques[i] = points[(i * 2) + 1];
		}

		m_RPMs[m_Count - 1] = m_RPMMax;
		m_Torques[m_Count - 1] = 0;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Index", m_Index);
		for (int i = 0; i < m_Count; i++)
		{
			instance.Add("Data[" + i + "]", Vector(m_RPMs[i], m_Torques[i], 0));
		}

		return true;
	}
#endif

	override float CalculateTorque(ExpansionPhysicsState pState)
	{
		for (int i = 0; i < m_Count - 1; i++)
		{
			if (m_RPM > m_RPMs[i] && m_RPM < m_RPMs[i + 1])
			{
				m_Index = i;

				float delta = (m_RPM - m_RPMs[i]) / (m_RPMs[i + 1] - m_RPMs[i]);
				return Math.Lerp(m_Torques[i], m_Torques[i + 1], delta);
			}
		}

		m_Index = -1;

		return 0;
	}
};
