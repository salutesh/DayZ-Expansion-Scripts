/**
 * ExpansionMarketJaloviskoZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketJaloviskoZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "Jalovisko";
		m_DisplayName = "Jalovisko Trading Zone";
		m_FileName = m_ZoneName;

		Position = "8583.67 24.55 10515.0";
		Radius = 400;

		Update();
	}
};