/**
 * ExpansionZoneCircle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionZoneCircle
 * @brief
 **/
class ExpansionZoneCircle : ExpansionZone
{
	vector m_Position;
	float m_Radius;

	void ExpansionZoneCircle(ExpansionZoneType type, vector position, float radius)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "ExpansionZoneCircle");
#endif

		m_Position = position;
		m_Radius = radius;
	}

	override void Check(vector position)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.ZONES, this, "Check");
#endif

		if (s_InsideBuffer[m_Type])
			return;

		bool isInside = Math.IsPointInCircle(position, m_Radius, m_Position);

		s_InsideBuffer[m_Type] = isInside;
	}

	override string ToStr()
	{
		return super.ToStr() + " position=" + m_Position + " radius=" + m_Radius;
	}
};
