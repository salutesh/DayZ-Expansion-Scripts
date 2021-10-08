/**
 * ExpansionMarketSpraycans.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSpraycans: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 61;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SPRAYCANS";
		m_FileName = "Spraycans";
	
	#ifdef EXPANSIONMODVEHICLE
		//! COLORS
		//AddItem("ExpansionSpraycanBlue",			100,	200,	1,		100,	0);	//! NOT READY
		//AddItem("ExpansionSpraycanBlueRust",			100,	200,	1,		100,	0);	//! NOT READY
		
		AddItem("ExpansionSpraycanRed",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanRedRust"});
		//AddItem("ExpansionSpraycanRedRust",			100,	200,	1,		100,	0);
		
		AddItem("ExpansionSpraycanGreen",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanGreenRust", "ExpansionSpraycanLightGreen"});
		/*AddItem("ExpansionSpraycanGreenRust",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanLightGreen",			100,	200,	1,		100,	0);*/
		
		AddItem("ExpansionSpraycanBlack",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanBlackRust"});
		//AddItem("ExpansionSpraycanBlackRust",			100,	200,	1,		100,	0);
		
		AddItem("ExpansionSpraycanOrange",			100,	200,	1,		100,	0);
		
		AddItem("ExpansionSpraycanWhite",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanWhiteRust"});
		//AddItem("ExpansionSpraycanWhiteRust",			100,	200,	1,		100,	0);
		
		AddItem("ExpansionSpraycanGrey",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanGreyRust"});
		//AddItem( "ExpansionSpraycanGreyRust",			100,	200,	1,		100,	0);
		
		AddItem("ExpansionSpraycanYellow",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanYellowRust"});
		//AddItem("ExpansionSpraycanYellowRust",			100,	200,	1,		100,	0);
		
		//AddItem("ExpansionSpraycanYellowBright",			100,	200,	1,		100,	0);	//! NOT READY
		//AddItem("ExpansionSpraycanYellowBrightRust",			100,	200,	1,		100,	0);	//! NOT READY
		
		AddItem("ExpansionSpraycanWine",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanWineRust"});
		//AddItem("ExpansionSpraycanWineRust",			100,	200,	1,		100,	0);
		
		//AddItem("ExpansionSpraycanLime",			100,	200,	1,		100,	0);	//! NOT READY
		
		//! SKINS
		//AddItem("ExpansionSpraycanPolice",			100,	200,	1,		100,	0);	//! NOT READY
		
		//! HATCHBACK 02 ONLY
		AddItem("ExpansionSpraycanBanditKitty",			100,	200,	1,		100,	0);
		
		//! VODNIC
		AddItem("ExpansionSpraycanCamo",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanCamoMedical",			100,	200,	1,		100,	0);
				
		//! NOT READY
		/*AddItem("ExpansionSpraycanWeeb",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanBlueline",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanDigital",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanFurious",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanGrayWatcher",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanJeans",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanResistance",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanShadow",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanSheriff",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanSpeedy",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanSunset",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanVrana",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanWasp",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanWave",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMedical",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanUber",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanDahoman",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanIon",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanPresident",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanElliptical",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanCivilian",			100,	200,	1,		100,	0);*/
		
		//! LEGACY
		
		//! MH6
		AddItem("ExpansionSpraycanMh6Blueline",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanMh6Digital", "ExpansionSpraycanMh6Elliptical", "ExpansionSpraycanMh6Furious", "ExpansionSpraycanMh6GrayWatcher", "ExpansionSpraycanMh6Jeans", "ExpansionSpraycanMh6Resistance", "ExpansionSpraycanMh6Shadow", "ExpansionSpraycanMh6Sheriff", "ExpansionSpraycanMh6Speedy", "ExpansionSpraycanMh6Sunset", "ExpansionSpraycanMh6Vrana", "ExpansionSpraycanMh6Wasp", "ExpansionSpraycanMh6Wave"});
		/*AddItem("ExpansionSpraycanMh6Digital",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Elliptical",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Furious",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6GrayWatcher",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Jeans",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Resistance",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Shadow",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Sheriff",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Speedy",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Sunset",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Vrana",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Wasp",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMh6Wave",			100,	200,	1,		100,	0);*/
		
		//! UH1H
		AddItem("ExpansionSpraycanUh1hMedical",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanUh1hUber", "ExpansionSpraycanUh1hResistance", "ExpansionSpraycanUh1hCivilian"});
		/*AddItem("ExpansionSpraycanUh1hUber",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanUh1hResistance",			100,	200,	1,		100,	0);*/
		
		//! MERLIN
		AddItem("ExpansionSpraycanMerlinDahoman",			100,	200,	1,		100,	0, null, {"ExpansionSpraycanMerlinIon", "ExpansionSpraycanMerlinPresident", "ExpansionSpraycanMerlinVrana", "ExpansionSpraycanMerlinWave"});
		/*AddItem("ExpansionSpraycanMerlinIon",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMerlinPresident",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMerlinVrana",			100,	200,	1,		100,	0);
		AddItem("ExpansionSpraycanMerlinWave",			100,	200,	1,		100,	0);*/
		
		//! BUS
		AddItem("ExpansionSpraycanBusWeeb",			100,	200,	1,		100,	0);
	#endif
	}
};