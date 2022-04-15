/*
 * ExpansionHelicopterLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionRearHelicopterLights extends CarRearLightBase
{
	void ExpansionRearHelicopterLights()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "ExpansionRearHelicopterLights");
#endif

		m_SegregatedBrakeBrightness = 0;
		m_SegregatedBrakeRadius = 0;
		m_SegregatedBrakeAngle = 180;
		m_SegregatedBrakeColorRGB = Vector(1.0, 1.0, 1.0);

		m_SegregatedBrightness = 0;
		m_SegregatedRadius = 0;
		m_SegregatedAngle = 180;
		m_SegregatedColorRGB = Vector(1.0, 1.0, 1.0);

		m_AggregatedBrightness = 0;
		m_AggregatedRadius = 0;
		m_AggregatedAngle = 180;
		m_AggregatedColorRGB = Vector(1.0, 1.0, 1.0);

		FadeIn(0.1);
		SetFadeOutTime(0.1);
		SetVisibleDuringDaylight(false);
		SetCastShadow(false);
		SetFlareVisible(false);

		SegregateLight();
	}
};

class ExpansionHelicopterFrontLight extends CarLightBase
{
	void ExpansionHelicopterFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "ExpansionHelicopterFrontLight");
#endif

		m_SegregatedBrightness = 5;
		m_SegregatedRadius = 220;
		m_SegregatedAngle = 110;
		m_SegregatedColorRGB = Vector(1.0, 0.8, 0.6);

		m_AggregatedBrightness = 10;
		m_AggregatedRadius = 90;
		m_AggregatedAngle = 120;
		m_AggregatedColorRGB = Vector(1.0, 0.8, 0.6);

		FadeIn(0.3);
		SetFadeOutTime(0.25);

		SegregateLight();
	}
};
