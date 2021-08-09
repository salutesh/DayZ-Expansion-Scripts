/*
 * ExpansionBusLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBusLights extends CarLightBase
{
	void ExpansionBusLights()
	{
		m_SegregatedBrightness = 1;
		m_SegregatedRadius = 2.5;
		m_SegregatedAngle = 110;
		m_SegregatedColorRGB = Vector(1.0, 0.8, 0.6);
		
		m_AggregatedBrightness = 1;
		m_AggregatedRadius = 2.5;
		m_AggregatedAngle = 120;
		m_AggregatedColorRGB = Vector(1.0, 0.8, 0.6);
		
		FadeIn(0.3);
		SetFadeOutTime(0.25);
		
		SegregateLight();

		SetLightType(LightSourceType.PointLight);
		//SetCastShadow(false);
	}
}