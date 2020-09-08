/**
 * ExpansionMarketTraderHelga.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderHelga: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderHelga";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_SPECIAL";
		m_FileName = "Special_Trader_Helga";
		
		// Explosives
		AddItem("RGD5Grenade");
		AddItem("M67Grenade");
		AddItem("M18SmokeGrenade_Red");
		AddItem("M18SmokeGrenade_Green");
		AddItem("M18SmokeGrenade_Yellow");
		AddItem("M18SmokeGrenade_Purple");
		AddItem("M18SmokeGrenade_White");
		AddItem("RDG2SmokeGrenade_Black");
		AddItem("RDG2SmokeGrenade_White");
		AddItem("FlashGrenade");
		
		//Expansion Explosives
		AddItem("ExpansionSatchel");

		//Expansion Grenade Teargas
		AddItem("Expansion_M18SmokeGrenade_Teargas");
		
		AddItem("NVGoggles");
		AddItem("NVGHeadstrap");
		AddItem("Battery9V");
	}
}