/**
 * ExpansionZoneCircle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	float m_RadiusSq;

	void ExpansionZoneCircle(ExpansionZoneType type, vector position, float radius)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneCircle::ExpansionZoneCircle start");
#endif

		m_Position = position;
		m_RadiusSq = radius * radius;

#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneCircle::ExpansionZoneCircle end");
#endif
	}

	override void Check(vector position)
	{
#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionZoneCircle::Check");
#endif

		if (s_InsideBuffer[m_Type])
			return;

		bool isInside = vector.DistanceSq(position, m_Position) <= m_RadiusSq;

		s_InsideBuffer[m_Type] = isInside;
	}

	override string ToStr()
	{
		return super.ToStr() + " position=" + m_Position + " radius=" + m_RadiusSq;
	}
};