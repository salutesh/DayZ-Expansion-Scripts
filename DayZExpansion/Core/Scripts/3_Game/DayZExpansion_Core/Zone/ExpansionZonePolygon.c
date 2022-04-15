/**
 * ExpansionZonePolygon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionZonePolygon
 * @brief
 **/
class ExpansionZonePolygon : ExpansionZone
{
	vector m_Position;
	float m_Radius;

	int m_Count;
	float m_Positions_X[32];
	float m_Positions_Z[32];

	static float s_PosX;
	static float s_PosZ;
	static float s_PosX0;
	static float s_PosZ0;
	static float s_PosX1;
	static float s_PosZ1;
	static int s_ItrK;
	static int s_ItrJ;

	void ExpansionZonePolygon(ExpansionZoneType type, TVectorArray positions)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "ExpansionZonePolygon");
#endif

		int i, j;

		m_Count = positions.Count();
		if (m_Count <= 2 || m_Count >= 32)
		{
			Error("Invalid number of positions supplied for polygon zone, got " + m_Count + ", expected 2<n<32");
		}

		for (i = 0; i < m_Count; i++)
		{
			m_Positions_X[i] = positions[i][0];
			m_Positions_Z[i] = positions[i][2];
		}

		float accumulatedArea = 0.0;
		j = m_Count - 1;
		for (i = 0; i < m_Count; ++i)
		{
			float area = m_Positions_X[i] * m_Positions_Z[j] - m_Positions_X[j] * m_Positions_Z[i];
			accumulatedArea += area;
			m_Position[0] = m_Position[0] + ((m_Positions_X[i] + m_Positions_X[j]) * area);
			m_Position[2] = m_Position[2] + ((m_Positions_Z[i] + m_Positions_Z[j]) * area);
			j = i;
		}

		accumulatedArea *= 3.0;
		if (accumulatedArea == 0.0)
			return;

		m_Position[0] = m_Position[0] / accumulatedArea;
		m_Position[2] = m_Position[2] / accumulatedArea;

		for (i = 0; i < m_Count; ++i)
		{
			float distance = vector.Distance(m_Position, positions[i]);
			if (distance > m_Radius)
			{
				m_Radius = distance;
			}
		}
	}

	override void Check(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "Check");
#endif

		m_Position[1] = position[1];

		if (s_InsideBuffer[m_Type] || !Math.IsPointInCircle(position, m_Radius, m_Position))
			return;

		bool ins = false;

		s_PosX = position[0];
		s_PosZ = position[2];

		s_ItrK = 0;
		s_ItrJ = m_Count - 1;

		while (s_ItrK < m_Count)
		{
			s_PosX0 = m_Positions_X[s_ItrK];
			s_PosZ0 = m_Positions_Z[s_ItrK];
			s_PosX1 = m_Positions_X[s_ItrJ];
			s_PosZ1 = m_Positions_Z[s_ItrJ];

			if ((s_PosZ0 > s_PosZ) != (s_PosZ1 > s_PosZ) && s_PosX < (s_PosX1 - s_PosX0) * (s_PosZ - s_PosZ0) / (s_PosZ1 - s_PosZ0) + s_PosX0)
				ins = !ins;

			s_ItrJ = s_ItrK;
			s_ItrK++;
		}

		s_InsideBuffer[m_Type] = ins;
	}

	override string ToStr()
	{
		return super.ToStr() + " position=" + m_Position + " radius=" + m_Radius;
	}
};
