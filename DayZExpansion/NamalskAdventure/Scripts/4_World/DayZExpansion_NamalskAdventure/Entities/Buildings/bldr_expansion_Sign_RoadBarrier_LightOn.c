/**
 * bldr_expansion_Sign_RoadBarrier_LightOn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMAPASSETS
class ExpansionRoadBarrierLight extends PointLightBase
{
	void ExpansionRoadBarrierLight()
	{
		vector colorRGB = Vector(1.0, 0.5, 0);
		
		SetVisibleDuringDaylight(true);
		SetRadiusTo(20.0);
		SetBrightnessTo(1.0);
		SetCastShadow(true);
		SetFadeOutTime(1.0);
		SetDiffuseColor(colorRGB[0], colorRGB[1], colorRGB[2]);
		SetAmbientColor(colorRGB[0], colorRGB[1], colorRGB[2]);
		SetFlareVisible(false);
		SetFlickerAmplitude(1.0);
		SetFlickerSpeed(0.9);
		SetDancingShadowsMovementSpeed(0.010);
		SetDancingShadowsAmplitude(0.008);
	}
};

modded class bldr_expansion_Sign_RoadBarrier_LightOn
{
	protected ExpansionRoadBarrierLight m_RoadBarrierLight;

	void bldr_expansion_Sign_RoadBarrier_LightOn()
	{
		if (!GetGame().IsDedicatedServer())
		{
			if (m_Light)
				GetGame().ObjectDelete(m_Light);
			
			if (!m_RoadBarrierLight)
			{
				m_RoadBarrierLight = ExpansionRoadBarrierLight.Cast(ScriptedLightBase.CreateLight(ExpansionRoadBarrierLight, GetPosition()));
				m_RoadBarrierLight.AttachOnMemoryPoint(this, "zluty pozicni blik");
			}
		}
	}
};
#endif
