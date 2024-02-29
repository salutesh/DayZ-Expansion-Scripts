/**
 * ExpansionLighthouseLampLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLighthouseLampLight_Base: SpotLightBase
{
	void ExpansionLighthouseLampLight_Base()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.LIGHTHOUSE, this);
#endif
		
		SetVisibleDuringDaylight( false );
		SetCastShadow( false );	
		SetDiffuseColor( 1, 1, 0.8 );
		SetAmbientColor( 1, 1, 0.8 );
		SetAmbientAlpha( 1 );
		SetEnabled( true );
		SetFlareVisible( true );
		FadeIn(0.3);
	}

	void ~ExpansionLighthouseLampLight_Base()
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.LIGHTHOUSE, this);
#endif

	}
	
	override void CheckIfParentIsInCargo()
	{
	}
}

class ExpansionLighthouseLampLight_Inner: ExpansionLighthouseLampLight_Base
{
	void ExpansionLighthouseLampLight_Inner()
	{
		SetRadiusTo(2);
		SetSpotLightAngle(100);
		SetBrightnessTo(20);
	}
};

class ExpansionLighthouseLampLight_Outer: ExpansionLighthouseLampLight_Base
{
	void ExpansionLighthouseLampLight_Outer()
	{
		SetRadiusTo(500);
		SetSpotLightAngle(50);
		SetBrightnessTo(1.2);
	}
};
