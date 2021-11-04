/**
 * ExpansionMarketTraderSpecial.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderSpecial: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Special";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_SPECIAL";
		m_FileName = "Special";
		TraderIcon = "Grenade";
		
		Categories.Insert("Explosives_And_Grenades");
		
	#ifdef EXPANSIONMOD
		// LAUNCHERS
		AddItem("ExpansionLAW");
		AddItem("ExpansionRPG7");
		AddItem("Expansion_M79");

		// LAUNCHER AMMO
		AddItem("ExpansionAmmoLAW");
		AddItem("ExpansionAmmoRPG");
		
		AddItem("Ammo_Expansion_M203_Smoke_White");
		//AddItem("Ammo_Expansion_M203_Smoke_Red");
		//AddItem("Ammo_Expansion_M203_Smoke_Green");
		//AddItem("Ammo_Expansion_M203_Smoke_Yellow");
		//AddItem("Ammo_Expansion_M203_Smoke_Purple");

		AddItem("Ammo_Expansion_M203_Smoke_Teargas");

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White");
		//AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red");
		//AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green");
		//AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow");
		//AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple");

		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas");

		AddItem("Ammo_Expansion_M203_HE");
	#endif
		// MISC
		AddItem("Rangefinder");
		AddItem("NVGoggles");
		AddItem("NVGHeadstrap");
		AddItem("Battery9V");
	}
}