/**
 * ExpansionMarketTraderAttachments.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderAttachments: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Attachments";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_ATTACHMENTS";
		m_FileName = "Attachments";
		TraderIcon = "Scrap Metal";
		
		// MAGAZINES
		AddItem("Mag_IJ70_8Rnd");

		AddItem("Mag_CZ75_15Rnd");

		AddItem("Mag_Glock_15Rnd");

		AddItem("Mag_MKII_10Rnd");

		AddItem("Mag_1911_7Rnd");

		AddItem("Mag_FNX45_15Rnd");

		AddItem("Mag_Deagle_9Rnd");

		AddItem("Mag_CZ527_5rnd");

		AddItem("Mag_CZ61_20Rnd");

		AddItem("Mag_UMP_25Rnd");

		AddItem("Mag_MP5_15Rnd");
		AddItem("Mag_MP5_30Rnd");

		AddItem("Mag_FAL_20Rnd");

		AddItem("Mag_Saiga_5Rnd");
		AddItem("Mag_Saiga_8Rnd");
		AddItem("Mag_Saiga_Drum20Rnd");

		AddItem("Mag_AKM_30Rnd");
		AddItem("Mag_AKM_Palm30Rnd");
		AddItem("Mag_AKM_Drum75Rnd");
		
		AddItem("Mag_AK101_30Rnd");

		AddItem("Mag_AK74_30Rnd");
		AddItem("Mag_AK74_45Rnd");

		AddItem("Mag_STANAG_30Rnd");
		AddItem("Mag_STANAGCoupled_30Rnd");
		AddItem("Mag_STANAG_60Rnd");

		AddItem("Mag_CMAG_10Rnd");
		AddItem("Mag_CMAG_20Rnd");
		AddItem("Mag_CMAG_30Rnd");
		AddItem("Mag_CMAG_40Rnd");
	
		AddItem("Mag_VSS_10Rnd");
		AddItem("Mag_VAL_20Rnd");

		//AddItem("Mag_Ruger1022_10Rnd");
		AddItem("Mag_Ruger1022_15Rnd");
		AddItem("Mag_Ruger1022_30Rnd");

		AddItem("Mag_SVD_10Rnd");

		AddItem("Mag_Scout_5Rnd");

		//AddItem("Mag_CLIP762x54_5Rnd");
		//AddItem("Mag_CLIP762x39_10Rnd");
		
		AddItem("Mag_FAMAS_25Rnd");
	#ifdef EXPANSIONMOD	
		AddItem("Mag_Expansion_Taser");

		AddItem("Mag_Expansion_M9_15Rnd");
		
		AddItem("Mag_Expansion_Kedr_20Rnd");

		AddItem("Mag_Expansion_MP7_40Rnd");
		
		AddItem("Mag_Expansion_Vityaz_30Rnd");

		AddItem("Mag_Expansion_MPX_50Rnd");			
		
		AddItem("Mag_Expansion_M14_10Rnd");
		AddItem("Mag_Expansion_M14_20Rnd");
		
		AddItem("Mag_Expansion_G36_30Rnd");

		AddItem("Mag_Expansion_AWM_5Rnd");
	#endif
		
		// COMPENSATORS
		AddItem("Mosin_Compensator");
		AddItem("MP5_Compensator");
		
		// SUPRESSORS
		AddItem("M4_Suppressor");
		AddItem("AK_Suppressor");
		AddItem("PistolSuppressor");
	
		// BAYONETS
		AddItem("AK_Bayonet");
		AddItem("M9A1_Bayonet");
		AddItem("Mosin_Bayonet");
		AddItem("SKS_Bayonet");
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Kar98_Bayonet");
	#endif
		
		// OPTICS
		AddItem("PistolOptic");
		AddItem("ReflexOptic");
		AddItem("M4_CarryHandleOptic");
		AddItem("BUISOptic");
		AddItem("FNP45_MRDSOptic");
		AddItem("ACOGOptic");
		AddItem("ACOGOptic_6x");
		AddItem("M68Optic");
		AddItem("M4_T3NRDSOptic");
		AddItem("KobraOptic");
		AddItem("KashtanOptic");
		AddItem("PUScopeOptic");
		AddItem("HuntingOptic");
		AddItem("PSO1Optic");
		AddItem("PSO11Optic");
		AddItem("KazuarOptic");
		AddItem("StarlightOptic");
		
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_M1A_RailAtt");
		AddItem("Expansion_MP5_RailAtt");
		AddItem("ExpansionReflexMRSOptic");
		AddItem("ExpansionDeltapointOptic");
		AddItem("ExpansionReflexMRSOptic");
		AddItem("ExpansionEXPS3HoloOptic");
		AddItem("ExpansionHAMROptic");
		AddItem("Expansion_PMII25Optic");
		AddItem("ExpansionKar98ScopeOptic");
		
		//! Doesn't work in Multiplayer
		//AddItem("Expansion_ANPEQ15_Red");
		//AddItem("Expansion_ANPEQ15_Green");
	#endif

		// BUTTSTOCKS
		AddItem("MP5k_StockBttstck");

		AddItem("Fal_OeBttstck");
		AddItem("Fal_FoldingBttstck");

		AddItem("Saiga_Bttstck");

		AddItem("AKS74U_Bttstck");

		AddItem("AK74_WoodBttstck");

		AddItem("AK_PlasticBttstck");
		AddItem("AK_WoodBttstck");
		AddItem("AK_FoldingBttstck");

		AddItem("M4_OEBttstck");
		AddItem("M4_MPBttstck");
		AddItem("M4_CQBBttstck");
			
		AddItem("GhillieAtt_Tan");
		AddItem("GhillieAtt_Woodland");
		AddItem("GhillieAtt_Mossy");
		
		// HANDGUARDS
		AddItem("MP5_PlasticHndgrd");
		AddItem("MP5_RailHndgrd");

		AddItem("AK74_Hndgrd");

		AddItem("AK_WoodHndgrd");
		AddItem("AK_RailHndgrd");
		AddItem("AK_PlasticHndgrd");
		
		AddItem("M4_PlasticHndgrd");
		AddItem("M4_RISHndgrd");
		AddItem("M4_MPHndgrd");
		
		// LIGHTS
		AddItem("UniversalLight");
		AddItem("TLRLight");
	}
}