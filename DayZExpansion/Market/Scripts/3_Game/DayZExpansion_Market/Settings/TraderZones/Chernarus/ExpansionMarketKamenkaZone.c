/**
 * ExpansionMarketKamenkaZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKamenkaZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "Kamenka";
		m_DisplayName = "Kamenka Trading Zone";
		m_FileName = m_ZoneName;

		Position = "1143.14 6.9 2429.27"; 	
		Radius = 700;

		Update();
	}
};