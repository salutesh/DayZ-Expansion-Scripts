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

	override float CalculateTorque()
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

/*
class ExpansionVehicleEnginePoint
{
	static const float s_TENSION = 0.0;

	vector m_Position;
	vector m_Tangent;

	float m_Start;
	float m_End;
	float m_Diff;
	float m_Length;

	ref ExpansionVehicleEnginePoint m_Next;
	ExpansionVehicleEnginePoint m_Prev;

	void Init()
	{
		if (m_Next)
			m_Length = vector.Distance(m_Position, m_Next.m_Position);

		if (m_Prev)
			m_Start = m_Prev.m_End;

		m_End = m_Start + m_Length;

		float diff = m_End;
		if (m_Prev)
			diff -= m_Prev.m_Start;

		vector pv = vector.Zero;
		vector nv = vector.Zero;

		if (m_Next)
			nv = vector.Direction(m_Next.m_Position, m_Position);
		if (m_Prev)
			pv = vector.Direction(m_Position, m_Prev.m_Position);

		if (!m_Next && m_Prev)
			nv = vector.Direction(m_Prev.m_Position, m_Position);
		if (!m_Prev && m_Next)
			pv = vector.Direction(m_Position, m_Next.m_Position);

		m_Tangent = (pv + nv) * -(1.0 - s_TENSION) * (1.0 / Math.Max(0.0001, diff));

		m_Diff = m_Length;
		if (m_Prev)
			m_Diff = m_Start - m_Prev.m_Start;

	}

	vector CubicInterp(vector p0, vector p1, vector m0, vector m1, float t)
	{
		float t2 = t * t;
		float t3 = t * t * t;

		vector res = vector.Zero;
		res += ((2.0 * t3) - (3.0 * t2) + 1.0) * p0;
		res += ((t3) - (2.0 * t2) + t) * m0;
		res += ((-2.0 * t3) + (3.0 * t2)) * p1;
		res += (t3 - t2) * m1;
		return res;
	}

	vector _Calculate(float rpm)
	{
		if (!m_Next)
			return vector.Zero;

		float delta = rpm - m_Position[0];
		delta = delta / m_Length;
		return CubicInterp(m_Position, m_Next.m_Position, m_Tangent * m_Diff, m_Next.m_Tangent * m_Diff, delta);
	}

	float Calculate(float rpm)
	{
		if (!m_Next)
			return 0;

		float delta = rpm - m_Position[0];
		delta = delta / m_Length;
		return CubicInterp(m_Position, m_Next.m_Position, m_Tangent * m_Diff, m_Next.m_Tangent * m_Diff, delta)[1];
	}
};

class ExpansionVehicleEngine : ExpansionVehicleEngineBase
{
	ref ExpansionVehicleEnginePoint m_Root;

	void ExpansionVehicleEngine(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " points";
		array<float> points = new array<float>();
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, points);

		m_Root = new ExpansionVehicleEnginePoint();
		ExpansionVehicleEnginePoint current = m_Root;
		current.m_Position[0] = 0;
		current.m_Position[1] = 0;

		for (int i = 0; i < points.Count() / 2; i++)
		{
			current.m_Next = new ExpansionVehicleEnginePoint();
			current.m_Next.m_Prev = current.m_Next;
			current = current.m_Next;

			current.m_Position[0] = points[(i * 2) + 0];
			current.m_Position[1] = points[(i * 2) + 1];
		}

		current.m_Next = new ExpansionVehicleEnginePoint();
		current.m_Next.m_Prev = current.m_Next;
		current = current.m_Next;

		current.m_Position[0] = m_RPMMax;
		current.m_Position[1] = 0;

		current = m_Root;
		while (current)
		{
			current.Init();
			current = current.m_Next;
		}

		for (int rpm = 0; rpm < m_RPMMax; rpm++)
		{
			m_RPM = rpm;
			vector calc = _CalculateTorque();

			Print("" + calc[0] + ", " + calc[1]);
		}
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		return true;
	}
#endif

	vector _CalculateTorque()
	{
		ExpansionVehicleEnginePoint current = m_Root;
		while (current)
		{
			if (current.m_Next && m_RPM > current.m_Position[0] && m_RPM <= current.m_Next.m_Position[0])
			{
				return current._Calculate(m_RPM);
			}

			current = current.m_Next;
		}

		return vector.Zero;
	}

	override float CalculateTorque()
	{
		ExpansionVehicleEnginePoint current = m_Root;
		while (current)
		{
			if (current.m_Next && m_RPM > current.m_Position[0] && m_RPM <= current.m_Next.m_Position[0])
			{
				return current.Calculate(m_RPM);
			}

			current = current.m_Next;
		}

		return 0;
	}
};
*/
