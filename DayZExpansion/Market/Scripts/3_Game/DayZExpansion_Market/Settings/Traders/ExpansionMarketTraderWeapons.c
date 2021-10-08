/**
 * ExpansionMarketTraderWeapons.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderWeapons: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Weapons";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_WEAPONS";
		m_FileName = "Weapons";
		TraderIcon = "Shotgun";

		// ASSAULT RIFLES
		AddItem("FAL");
		AddItem("AKM");
		AddItem("AK101");
		AddItem("AK74");
		AddItem("M4A1");
		AddItem("M16A2");
		AddItem("FAMAS");
		
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_M16");
		AddItem("Expansion_G36");
	#endif
		
		// SUBMACHINE GUNS
		AddItem("CZ61");
		AddItem("UMP45");
		AddItem("MP5K");
		AddItem("AKS74U");

	#ifdef EXPANSIONMOD	
		AddItem("Expansion_Kedr");
		AddItem("Expansion_MPX");
		AddItem("Expansion_MP5");
		AddItem("Expansion_MP5SD");
		AddItem("Expansion_MP7");
		AddItem("Expansion_VityazSN");
	#endif
		
		// RIFLES
		AddItem("Izh18");
		AddItem("Ruger1022");
		AddItem("Repeater");
		AddItem("Mosin9130");
		AddItem("CZ527");
		AddItem("Winchester70");
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Kar98");
		AddItem("Expansion_M14");
		AddItem("Expansion_M1A");
	#endif
		
		// PISTOLS
		AddItem("MakarovIJ70");
		AddItem("CZ75");
		AddItem("FNX45");
		AddItem("Glock19");
		AddItem("MKII");
		AddItem("Colt1911");
		AddItem("Engraved1911");
		AddItem("Magnum");
		AddItem("Deagle");
		AddItem("Deagle_Gold");
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_M9");
		AddItem("Expansion_Longhorn");
	#endif

		// SNIPER RIFLES
		AddItem("VSS");
		AddItem("ASVAL");
		AddItem("B95");
		AddItem("SVD");

		if (IsMapEnoch())
		{
			AddItem("Scout_Livonia");
		}
		else if (IsMapChernarus())
		{
			AddItem("Scout_Chernarus");
		}
		else
		{
			AddItem("Scout");
		}
	#ifdef EXPANSIONMOD
		AddItem("Expansion_AWM");
	#endif
		
		// SHOTGUNS
		AddItem("Mp133Shotgun");
		AddItem("Izh43Shotgun");
		AddItem("Saiga");
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_BenelliM4");
		AddItem("Expansion_DT11");
	#endif
		
		// LAUNCHERS
		AddItem("Flaregun");
	
	#ifdef EXPANSIONMOD	
		// CROSSBOWS
		AddItem("Expansion_Crossbow");
	#endif
		
		// MEELE
		AddItem("BrassKnuckles_Dull");
		AddItem("BrassKnuckles_Shiny");
		AddItem("StunBaton");
		AddItem("Broom");
		AddItem("Broom_Birch");
		AddItem("Pipe");
		AddItem("CattleProd");
		AddItem("BaseballBat");
		AddItem("NailedBaseballBat");
		
		// AMMO
		AddItem("Ammo_12gaPellets");
		AddItem("Ammo_12gaRubberSlug");
		AddItem("Ammo_12gaSlug");
		
		AddItem("Ammo_22");
		
		AddItem("Ammo_308Win");
		AddItem("Ammo_308WinTracer");
		
		AddItem("Ammo_357");
		AddItem("Ammo_380");
		AddItem("Ammo_45ACP");
		
		AddItem("Ammo_545x39");
		AddItem("Ammo_545x39Tracer");
		
		AddItem("Ammo_556x45");
		AddItem("Ammo_556x45Tracer");
		
		AddItem("Ammo_762x39");
		AddItem("Ammo_762x39Tracer");
		
		AddItem("Ammo_762x54");
		AddItem("Ammo_762x54Tracer");
		
		AddItem("Ammo_9x19");
		AddItem("Ammo_9x39");
		AddItem("Ammo_9x39AP");
		
		AddItem("Ammo_Flare");
		AddItem("Ammo_FlareBlue");
		AddItem("Ammo_FlareGreen");
		AddItem("Ammo_FlareRed");
	
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Ammo_ArrowBolt");
		
		//AddItem("ExpansionAmmoFlare");
		//AddItem("ExpansionAmmoFlareLight");
			
		AddItem("Ammo_Expansion_46x30");
		AddItem("Ammo_Expansion_338");
		AddItem("Expansion_Ammo_8mm");
	#endif
		
		// AMMO BOXES
		AddItem("AmmoBox_00buck_10rnd");
		AddItem("AmmoBox_12gaSlug_10Rnd");
		AddItem("AmmoBox_12gaRubberSlug_10Rnd");
		
		AddItem("AmmoBox_22_50Rnd");
		
		AddItem("AmmoBox_308Win_20Rnd");
		AddItem("AmmoBox_308WinTracer_20Rnd");
		
		AddItem("AmmoBox_357_20Rnd");
		AddItem("AmmoBox_380_35rnd");
		AddItem("AmmoBox_45ACP_25rnd");
		
		AddItem("AmmoBox_545x39_20Rnd");
		AddItem("AmmoBox_545x39Tracer_20Rnd");
		
		AddItem("AmmoBox_556x45_20Rnd");
		AddItem("AmmoBox_556x45Tracer_20Rnd");
		
		AddItem("AmmoBox_762x39_20Rnd");
		AddItem("AmmoBox_762x39Tracer_20Rnd");
		
		AddItem("AmmoBox_762x54_20Rnd");
		AddItem("AmmoBox_762x54Tracer_20Rnd");
		
		AddItem("AmmoBox_9x19_25rnd");
		AddItem("AmmoBox_9x39AP_20rnd");
		AddItem("AmmoBox_9x39_20rnd");
	#ifdef EXPANSIONMOD	
		AddItem("AmmoBox_Expansion_46x30_25rnd");
		AddItem("AmmoBox_Expansion_338_15rnd");
		AddItem("Expansion_AmmoBox_8mm_15rnd");
	#endif

		// KNIFES
		AddItem("KitchenKnife");
		AddItem("SteakKnife");
		AddItem("HuntingKnife");
		AddItem("CombatKnife");
		AddItem("KukriKnife");
		AddItem("FangeKnife");
		AddItem("Machete");
		AddItem("CrudeMachete");
		AddItem("OrientalMachete");
		// MAGAZINES
		AddItem("Mag_IJ70_8Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_CZ75_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Glock_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_MKII_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_1911_7Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_FNX45_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Deagle_9Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_CZ527_5rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_CZ61_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_UMP_25Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_MP5_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_MP5_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_FAL_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Saiga_5Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_Saiga_8Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_Saiga_Drum20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_AKM_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_AKM_Palm30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_AKM_Drum75Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("Mag_AK101_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_AK74_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_AK74_45Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_STANAG_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_STANAGCoupled_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_STANAG_60Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_CMAG_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_CMAG_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_CMAG_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_CMAG_40Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	
		AddItem("Mag_VSS_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_VAL_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		//AddItem("Mag_Ruger1022_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_Ruger1022_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_Ruger1022_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_SVD_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Scout_5Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		//AddItem("Mag_CLIP762x54_5Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		//AddItem("Mag_CLIP762x39_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("Mag_FAMAS_25Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#ifdef EXPANSIONMOD	
		AddItem("Mag_Expansion_Taser", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Expansion_M9_15Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("Mag_Expansion_Kedr_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Expansion_MP7_40Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("Mag_Expansion_Vityaz_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Expansion_MPX_50Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);			
		
		AddItem("Mag_Expansion_M14_10Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mag_Expansion_M14_20Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("Mag_Expansion_G36_30Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Mag_Expansion_AWM_5Rnd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#endif
		
		// COMPENSATORS
		AddItem("Mosin_Compensator", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("MP5_Compensator", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		// SUPRESSORS
		AddItem("M4_Suppressor", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("AK_Suppressor", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("PistolSuppressor", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	
		// BAYONETS
		AddItem("AK_Bayonet", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M9A1_Bayonet", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Mosin_Bayonet", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("SKS_Bayonet", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Kar98_Bayonet", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#endif
		
		// OPTICS
		AddItem("PistolOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ReflexOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_CarryHandleOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("BUISOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("FNP45_MRDSOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ACOGOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ACOGOptic_6x", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M68Optic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_T3NRDSOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("KobraOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("KashtanOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("PUScopeOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("HuntingOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("PSO1Optic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("PSO11Optic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("KazuarOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("StarlightOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
	#ifdef EXPANSIONMOD	
		AddItem("Expansion_M1A_RailAtt", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Expansion_MP5_RailAtt", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionReflexMRSOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionDeltapointOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionReflexMRSOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionEXPS3HoloOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionHAMROptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Expansion_PMII25Optic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionKar98ScopeOptic", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		//! Doesn't work in Multiplayer
		//AddItem("Expansion_ANPEQ15_Red", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		//AddItem("Expansion_ANPEQ15_Green", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#endif

		// BUTTSTOCKS
		AddItem("MP5k_StockBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Fal_OeBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Fal_FoldingBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("Saiga_Bttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("AKS74U_Bttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("AK74_WoodBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("AK_PlasticBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("AK_WoodBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("AK_FoldingBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("M4_OEBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_MPBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_CQBBttstck", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
			
		AddItem("GhillieAtt_Tan", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("GhillieAtt_Woodland", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("GhillieAtt_Mossy", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		// HANDGUARDS
		AddItem("MP5_PlasticHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("MP5_RailHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("AK74_Hndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);

		AddItem("AK_WoodHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("AK_RailHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("AK_PlasticHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		AddItem("M4_PlasticHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_RISHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("M4_MPHndgrd", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		// LIGHTS
		AddItem("UniversalLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("TLRLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	}
};