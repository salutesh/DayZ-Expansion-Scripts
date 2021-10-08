/**
 * ExpansionMarketTaraHarborZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTaraHarborZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "TaraHarbor";
		m_DisplayName = "Tara Harbor Trading Zone";
		m_FileName = m_ZoneName;

		Position = "8043.48 2.65 7593.5";
		Radius = 400;

		Update();
	}
};