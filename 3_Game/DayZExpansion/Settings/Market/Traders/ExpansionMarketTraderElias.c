/**
 * ExpansionMarketTraderElias.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderElias: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderElias";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_VEHICLES";
		m_FileName = "Vehicles_Trader_Elias";

		AddItem("ExpansionTractor");
		AddItem("CivilianSedan");
		AddItem("Sedan_02");
		AddItem("Hatchback_02");
		AddItem("OffroadHatchback");
		AddItem("ExpansionBus");
		AddItem("ExpansionUAZ");
		AddItem("ExpansionUAZRoofless");
		AddItem("ExpansionUAZCargo");
		AddItem("ExpansionUAZCargoRoofless");
		AddItem("ExpansionVodnik");
		AddItem("ExpansionSpraycanOffroadHatchbackGreen");
		AddItem("ExpansionSpraycanOffroadHatchbackBlue");
		AddItem("ExpansionSpraycanOffroadHatchbackWhite");
		AddItem("ExpansionSpraycanHatchback_02Red");
		AddItem("ExpansionSpraycanHatchback_02Blue");
		AddItem("ExpansionSpraycanHatchback_02Black");  
		AddItem("ExpansionSpraycanCivilianSedanWhite");
		AddItem("ExpansionSpraycanCivilianSedanGrey");
		AddItem("ExpansionSpraycanCivilianSedanWine");
		AddItem("ExpansionSpraycanCivilianSedanLime");
		AddItem("ExpansionSpraycanCivilianSedanBlack");
		AddItem("ExpansionSpraycanSedan_02Blue");
		AddItem("ExpansionSpraycanSedan_02Grey");
		AddItem("ExpansionSpraycanSedan_02Red");
		AddItem("ExpansionSpraycanSedan_02Yellow");
		AddItem("ExpansionSpraycanSedan_02YellowBright");
		AddItem("ExpansionSpraycanBusDefault");
		AddItem("ExpansionSpraycanBusWeeb");
	}
}