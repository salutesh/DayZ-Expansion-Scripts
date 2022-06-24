/**
 * ExpansionMarketNeviHoffZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNeviHoffZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_DisplayName = "NeviHoff Trading Zone";
		m_FileName = "NeviHoff";

		Position = "6244.14013671875 43.900001525878909 7498.0"; 	
		Radius = 300;

		Update();
	}
};