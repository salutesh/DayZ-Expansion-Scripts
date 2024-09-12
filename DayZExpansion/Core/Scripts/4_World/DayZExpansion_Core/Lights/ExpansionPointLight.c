/**
 * ExpansionPointLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPointLight
 * @brief		
 **/
class ExpansionPointLight: PointLightBase
{
	float m_Val;
	
	bool m_Enabled = true;	

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionPointLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTS, this, "ExpansionPointLight");
#endif
		
		SetVisibleDuringDaylight(false);
		SetCastShadow(false);
		SetFlareVisible(true);
		SetRadiusTo( 10 );
		SetBrightnessTo(1);
		SetDiffuseColor(1,1,0.5);	
		
		EnableSpecular(false);
		EnableLinear(true);
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionPointLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTS, this, "~ExpansionPointLight");
#endif
		
	}

	//! @note this exists because EntityLightSource::IsEnabled always return true...
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