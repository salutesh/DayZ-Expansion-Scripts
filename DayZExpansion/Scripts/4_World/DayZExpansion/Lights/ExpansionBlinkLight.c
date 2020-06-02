/**
 * ExpansionBlinkLight.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBlinkingLight
 * @brief		
 **/
class ExpansionBlinkLight extends ExpansionPointLight
{
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionBlinkLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::ExpansionBlinkLight - Start");
		#endif
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			SetEnabled(true);

			m_Val = 0;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::ExpansionBlinkLight - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionBlinkLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::~ExpansionBlinkLight - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::~ExpansionBlinkLight - End");
		#endif
	}

	// ------------------------------------------------------------
	// EOnFrame
	// ------------------------------------------------------------
	override void EOnFrame(IEntity other, float timeSlice)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::EOnFrame - Start");
		#endif
		
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{
			if ( ExpansionGetEnabled() )
			{
				if (m_Val < 360)
				{	
					m_Val += 1;
				}	
				else
				{
					m_Val = 0;
				}			
	
				if (m_Val < 180)
				{
					SetEnabled(true);
				}
				else
				{
					SetEnabled(false);
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionBlinkLight::EOnFrame - End");
		#endif
	}
}