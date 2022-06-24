/**
 * ExpansionMarketKamenkaBoatsZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKamenkaBoatsZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_DisplayName = "Kamenka Boats Trading Zone";
		m_FileName = "KamenkaBoats";

		Position = "1756.1 2.2 2025.47"; 	
		Radius = 100;

		Update();
	}
};