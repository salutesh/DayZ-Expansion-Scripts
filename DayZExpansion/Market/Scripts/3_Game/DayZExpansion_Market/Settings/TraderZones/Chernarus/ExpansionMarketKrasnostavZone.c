/**
 * ExpansionMarketKrasnostavZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKrasnostavZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "Krasnostav";
		m_DisplayName = "Krasnostav Trading Zone";
		m_FileName = m_ZoneName;

		Position = "11924.7 140.7 12527.0"; 	
		Radius = 1000;

		Update();
	}
};