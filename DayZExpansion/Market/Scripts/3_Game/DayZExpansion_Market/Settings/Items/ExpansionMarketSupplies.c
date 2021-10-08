/**
 * ExpansionMarketSupplies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSupplies: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 52;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SUPPLIES";
		m_FileName = "Supplies";
		
		AddItem("Paper", 						2,		4,		1,		100,	0);
		AddItem("Pen_Black", 					2,		4,		1,		100,	0, null, {"Pen_Blue", "Pen_Green", "Pen_Red"});
		/*AddItem("Pen_Blue", 					2,		4,		1,		100,	0);
		AddItem("Pen_Green", 					2,		4,		1,		100,	0);
		AddItem("Pen_Red",	 					2,		4,		1,		100,	0);*/
		AddItem("Rope", 						4,		8,		1,		100,	0);
		AddItem("TannedLeather", 				4,		8,		1,		100,	0);
		AddItem("AntiPestsSpray", 				8,		16,		1,		100,	0);
		AddItem("MetalWire", 					15,		30,		1,		100,	0);
		AddItem("EpoxyPutty", 					15,		30,		1,		100,	0);
		AddItem("DuctTape", 					20,		40,		1,		100,	0);
		AddItem("Pot", 							25,	 	50,		1,		100,	0);
		AddItem("HandcuffKeys", 				35,		70,		1,		100,	0);
		AddItem("Handcuffs", 					50,		100,	1,		100,	0);
		AddItem("Netting", 						50,		100,	1,		100,	0);
		AddItem("BurlapSack", 					50,		100,	1,		100,	0);
		AddItem("WoodenPlank", 					50,		100,	1,		100,	0);
		AddItem("MetalPlate", 					100,	200,	1,		100,	0);
		AddItem("SmallGasCanister", 			80,		160,	1,		100,	0);
		AddItem("MediumGasCanister", 			100,	200,	1,		100,	0);
		AddItem("LargeGasCanister", 			200,	400,	1,		100,	0);
		//----------------------------------------------------------------------------------------------------------
		//! Keep in mind here that players can buy the Nail boxes and get 70 nails from 1 box!
		// They should not get more or the same amount of the nail box price back when they sell these 70 nails!
		//----------------------------------------------------------------------------------------------------------
		AddItem("NailBox", 						300,	600,	1,		100,	0);
		AddItem("Nail", 						3,		6,		1,		100,	0);
		//----------------------------------------------------------------------------------------------------------
		AddItem("BarbedWire", 					450,	900,	1,		100,	0);
		AddItem("Fabric", 						600,	1200,	1,		100,	0);
		AddItem("Camonet", 						600,	1200,	1,		100,	0);
		AddItem("HescoBox", 					650,	1300,	1,		100,	0);
		AddItem("PowerGenerator", 				3000,	6000,	1,		100,	0);

	#ifdef EXPANSIONMOD
		//AddItem("ExpansionLumber0_5", 		50,		100,	1,		100,	0);
		AddItem("ExpansionLumber1", 			50,		100,	1,		100,	0);
		AddItem("ExpansionLumber1_5", 			50,		100,	1,		100,	0);
		AddItem("ExpansionLumber3", 			50,		100,	1,		100,	0);
		
		//! For the BaseBuilding update
		//AddItem("ExpansionCement", 			150,	300,	1,		100,	0);
		//AddItem("ExpansionBricks", 			500,	1000,	1,		100,	0);
		
		//AddItem("ExpansionMixerKit", 			400,	800,	1,		100,	0);
		//AddItem("ExpansionMixerTank", 		400,	800,	1,		100,	0);
		//AddItem("ExpansionMixerMotor", 		400,	800,	1,		100,	0);
		//AddItem("ExpansionMixerHandle", 		400,	800,	1,		100,	0);
	#endif
	}
};