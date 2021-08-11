/**
 * ExpansionLighthouse.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLighthouse
 * @brief		
 **/
class ExpansionLighthouse extends SpotLightBase
{
	float m_Val = 0;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionLighthouse()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::ExpansionLighthouse - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::ExpansionLighthouse - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionLighthouse()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::~ExpansionLighthouse - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::~ExpansionLighthouse - End");
		#endif
		
	}

	// ------------------------------------------------------------
	// EOnFrame
	// ------------------------------------------------------------
	override void OnFrameLightSource(IEntity other, float timeSlice)
	{
		super.OnFrameLightSource( other, timeSlice);
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::OnFrameLightSource - Start");
		#endif
		
		if (m_Val < 360)
			m_Val += 25 * timeSlice;
		else
			m_Val = 0;

		this.SetOrientation(Vector(m_Val, 0, 0));
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLighthouse::OnFrameLightSource - End");
		#endif
	}
	
	override void CheckIfParentIsInCargo() {}
}