/**
 * ExpansionStreetLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionStreetLight
 * @brief		
 **/
class ExpansionStreetLight extends SpotLightBase
{
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionStreetLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStreetLight::ExpansionStreetLight - Start");
		#endif
		
		SetVisibleDuringDaylight( false );

		SetSpotLightAngle(120);
		SetCastShadow( true );
		SetFlareVisible( true );
		SetFadeOutTime(0.15);

		SetRadiusTo( 15 );
		SetBrightnessTo( 2.5 );
		SetFadeOutTime( 0.15 );
		SetAmbientColor( 1.0, 0.85, 0.75 );
		SetDiffuseColor( 1, 1, 0.5 );	

		EnableSpecular( true );
		EnableLinear(true);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStreetLight::ExpansionStreetLight - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionStreetLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStreetLight::~ExpansionStreetLight - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStreetLight::~ExpansionStreetLight - End");
		#endif
	}
	
	override void CheckIfParentIsInCargo() {}
}