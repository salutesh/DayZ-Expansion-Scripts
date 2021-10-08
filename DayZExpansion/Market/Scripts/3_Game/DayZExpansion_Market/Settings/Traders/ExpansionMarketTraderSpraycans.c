/**
 * ExpansionMarketTraderSpraycans.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderSpraycans: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Spraycans";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_SPRAYCANS";
		m_FileName = "Spraycans";
		TraderIcon = "Bottle";
	
		#ifdef EXPANSIONMODVEHICLE
		//! COLORS
		//AddItem("ExpansionSpraycanBlue");	//! NOT READY
		//AddItem("ExpansionSpraycanBlueRust");	//! NOT READY
		
		AddItem("ExpansionSpraycanRed");
		AddItem("ExpansionSpraycanRedRust");
		
		AddItem("ExpansionSpraycanGreen");
		AddItem("ExpansionSpraycanGreenRust");
		AddItem("ExpansionSpraycanLightGreen");
		
		AddItem("ExpansionSpraycanBlack");
		AddItem("ExpansionSpraycanBlackRust");
		
		AddItem("ExpansionSpraycanOrange");
		
		AddItem("ExpansionSpraycanWhite");
		AddItem("ExpansionSpraycanWhiteRust");
		
		AddItem("ExpansionSpraycanGrey");
		AddItem( "ExpansionSpraycanGreyRust");
		
		AddItem("ExpansionSpraycanYellow");
		AddItem("ExpansionSpraycanYellowRust");
		
		//AddItem("ExpansionSpraycanYellowBright");	//! NOT READY
		//AddItem("ExpansionSpraycanYellowBrightRust");	//! NOT READY
		
		AddItem("ExpansionSpraycanWine");
		AddItem("ExpansionSpraycanWineRust");
		
		//AddItem("ExpansionSpraycanLime");	//! NOT READY
		
		//! SKINS
		//AddItem("ExpansionSpraycanPolice");	//! NOT READY
		
		//! HATCHBACK 02 ONLY
		AddItem("ExpansionSpraycanBanditKitty");
		
		//! VODNIC
		AddItem("ExpansionSpraycanCamo");
		AddItem("ExpansionSpraycanCamoMedical");
				
		//! Using specific spraycans instead
		/*AddItem("ExpansionSpraycanWeeb");
		AddItem("ExpansionSpraycanBlueline");
		AddItem("ExpansionSpraycanDigital");
		AddItem("ExpansionSpraycanElliptical");
		AddItem("ExpansionSpraycanFurious");
		AddItem("ExpansionSpraycanGrayWatcher");
		AddItem("ExpansionSpraycanJeans");
		AddItem("ExpansionSpraycanResistance");
		AddItem("ExpansionSpraycanShadow");
		AddItem("ExpansionSpraycanSheriff");
		AddItem("ExpansionSpraycanSpeedy");
		AddItem("ExpansionSpraycanSunset");
		AddItem("ExpansionSpraycanVrana");
		AddItem("ExpansionSpraycanWasp");
		AddItem("ExpansionSpraycanWave");
		AddItem("ExpansionSpraycanCivilian");
		AddItem("ExpansionSpraycanMedical");
		AddItem("ExpansionSpraycanUber");
		AddItem("ExpansionSpraycanDahoman");
		AddItem("ExpansionSpraycanIon");
		AddItem("ExpansionSpraycanPresident");
		AddItem("ExpansionSpraycanElliptical");
		AddItem("ExpansionSpraycanCivilian");*/
		
		//! LEGACY
		
		//! MH6
		AddItem("ExpansionSpraycanMh6Blueline");
		AddItem("ExpansionSpraycanMh6Digital");
		//AddItem("ExpansionSpraycanMh6Elliptical");		//! LEGACY NOT WORKING
		AddItem("ExpansionSpraycanMh6Furious");
		AddItem("ExpansionSpraycanMh6GrayWatcher");
		AddItem("ExpansionSpraycanMh6Jeans");
		AddItem("ExpansionSpraycanMh6Resistance");
		AddItem("ExpansionSpraycanMh6Shadow");
		AddItem("ExpansionSpraycanMh6Sheriff");
		AddItem("ExpansionSpraycanMh6Speedy");
		AddItem("ExpansionSpraycanMh6Sunset");
		AddItem("ExpansionSpraycanMh6Vrana");
		AddItem("ExpansionSpraycanMh6Wasp");
		AddItem("ExpansionSpraycanMh6Wave");
		
		//! UH1H
		AddItem("ExpansionSpraycanUh1hMedical");
		AddItem("ExpansionSpraycanUh1hUber");
		AddItem("ExpansionSpraycanUh1hResistance");
		
		//! MERLIN
		AddItem("ExpansionSpraycanMerlinDahoman");
		AddItem("ExpansionSpraycanMerlinIon");
		AddItem("ExpansionSpraycanMerlinPresident");
		AddItem("ExpansionSpraycanMerlinVrana");
		AddItem("ExpansionSpraycanMerlinWave");
		
		//! BUS
		AddItem("ExpansionSpraycanBusWeeb");
	#endif
	}
}