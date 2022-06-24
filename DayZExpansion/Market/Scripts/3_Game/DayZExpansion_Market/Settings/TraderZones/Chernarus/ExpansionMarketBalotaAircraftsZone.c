/**
 * ExpansionMarketBalotaAircraftsZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketBalotaAircraftsZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_DisplayName = "Balota Aircrafts Trading Zone";
		m_FileName = "BalotaAircrafts";

		Position = "4971.0 10.5 2436.36"; 	
		Radius = 100;

		Update();
	}
};