/**
 * ExpansionMarketTraderManua.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderManua: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderManua";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_SPRAYCANS";
		m_FileName = "Spraycan_Trader_Manua";

		AddItem("ExpansionSpraycanGreen");
		AddItem("ExpansionSpraycanBlue");
		AddItem("ExpansionSpraycanWhite");
		AddItem("ExpansionSpraycanRed");
		AddItem("ExpansionSpraycanBlack");
		AddItem("ExpansionSpraycanGrey");
		AddItem("ExpansionSpraycanWine");
		AddItem("ExpansionSpraycanYellow");
		AddItem("ExpansionSpraycanWeeb");
		AddItem("ExpansionSpraycanDahoman");
		AddItem("ExpansionSpraycanIon");
		AddItem("ExpansionSpraycanPresident");
		AddItem("ExpansionSpraycanVrana");
		AddItem("ExpansionSpraycanWave");
		AddItem("ExpansionSpraycanCivilian");
		AddItem("ExpansionSpraycanMedical");
		AddItem("ExpansionSpraycanUber");
		AddItem("ExpansionSpraycanBlueline");
		AddItem("ExpansionSpraycanDigital");
		AddItem("ExpansionSpraycanElliptical");
		AddItem("ExpansionSpraycanFurious");
		AddItem("ExpansionSpraycanGrayWatcher");
		AddItem("ExpansionSpraycanJeans");
		AddItem("ExpansionSpraycanShadow");
		AddItem("ExpansionSpraycanSpeedy");
		AddItem("ExpansionSpraycanSunset");
		AddItem("ExpansionSpraycanWasp");
	}
}