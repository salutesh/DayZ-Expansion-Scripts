/**
 * ExpansionMarketNamalskAirstripZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNamalskAirstripZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "NamalskAirstrip";
		m_DisplayName = "Airstrip Trading Zone";
		m_FileName = m_ZoneName;

		Position = "6302.0 21.21 9513.0";
		Radius = 400;

		Update();
	}
};