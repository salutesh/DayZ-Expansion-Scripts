/**
 * ExpansionPointLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPointLight
 * @brief		
 **/
class ExpansionPointLight extends PointLightBase
{
	float m_Val;
	
	bool m_Enabled;	

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionPointLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPointLight::ExpansionPointLight - Start");
		#endif
		
		SetVisibleDuringDaylight(false);
		SetCastShadow(false);
		SetFlareVisible(true);
		SetRadiusTo( 10 );
		SetBrightnessTo(1);
		SetDiffuseColor(1,1,0.5);	
		
		EnableSpecular(false);
		EnableLinear(true);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPointLight::ExpansionPointLight - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionPointLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPointLight::~ExpansionPointLight - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPointLight::~ExpansionPointLight - End");
		#endif
	}

	bool ExpansionGetEnabled()
	{
		return m_Enabled;
	}

	void ExpansionSetEnabled( bool enabled )
	{
		m_Enabled = enabled;

		SetEnabled( enabled );
	}
}