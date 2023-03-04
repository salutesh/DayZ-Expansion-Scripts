/**
 * ExpansionLighthouse.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLighthouse
 * @brief		
 **/
class ExpansionLighthouse: SpotLightBase
{
	float m_Val = 0;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionLighthouse()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "ExpansionLighthouse");
#endif
		
		SetVisibleDuringDaylight( false );
		SetRadius( 300 );
		SetCastShadow( true );	
		SetDiffuseColor( 1, 1, 0.8 );
		SetAmbientColor( 1, 1, 0.8 );
		SetAmbientAlpha( 1 );
		SetEnabled( true );
		SetFlareVisible( true );
		SetSpotLightAngle( 100 );
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionLighthouse()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "~ExpansionLighthouse");
#endif

	}

	// ------------------------------------------------------------
	// EOnFrame
	// ------------------------------------------------------------
	override void OnFrameLightSource(IEntity other, float timeSlice)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.LIGHTHOUSE, this, "OnFrameLightSource");
#endif
		
		super.OnFrameLightSource( other, timeSlice);
				
		if (m_Val < 360)
			m_Val += 25 * timeSlice;
		else
			m_Val = 0;

		this.SetOrientation(Vector(m_Val, 0, 0));
	}
	
	override void CheckIfParentIsInCargo()
	{
	}
};
