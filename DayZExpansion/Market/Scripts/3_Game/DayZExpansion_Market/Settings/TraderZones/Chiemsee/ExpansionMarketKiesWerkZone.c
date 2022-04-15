/**
 * ExpansionMarketKiesWerkZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKiesWerkZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "KiesWerk";
		m_DisplayName = "KiesWerk Airfield Trading Zone";
		m_FileName = m_ZoneName;

		Position = "3264.6201171875 20.903200149536134 4740.669921875"; 	
		Radius = 400;

		Update();
	}
};