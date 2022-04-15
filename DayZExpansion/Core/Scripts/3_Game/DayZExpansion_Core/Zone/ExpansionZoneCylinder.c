/**
 * ExpansionZoneCylinder.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionZoneCylinder
 * @brief
 **/
class ExpansionZoneCylinder : ExpansionZone
{
	vector m_Position;
	float m_Radius;
	float m_Height;

	void ExpansionZoneCylinder(ExpansionZoneType type, vector position, float radius, float height)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "ExpansionZoneCylinder");
#endif

		m_Position = position;
		m_Radius = radius;
		m_Height = height;
	}

	override void Check(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "Check");
#endif

		if (s_InsideBuffer[m_Type])
			return;

		if (position[1] < m_Position[1])
			return;

		if (position[1] > m_Position[1] + m_Height)
			return;

		bool isInside = Math.IsPointInCircle(position, m_Radius, m_Position);

		s_InsideBuffer[m_Type] = isInside;
	}

	override string ToStr()
	{
		return super.ToStr() + " position=" + m_Position + " radius=" + m_Radius;
	}
};