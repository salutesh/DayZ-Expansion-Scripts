/**
 * ExpansionLoot.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLootDefaults
{
	static array < ref ExpansionLoot > Regular()
	{
		array < ref ExpansionLoot > Loot;
		
		//! General Variants
		TStringArray att_UniversalLight = { "UniversalLight" };
		TStringArray att_TLRLight = { "TLRLight" };
		TStringArray att_PistolSuppressor = { "PistolSuppressor" };
		
		TStringArray att_TLRLightAndPistolSuppressor = { "TLRLight","PistolSuppressor" };

		TStringArray att_PUScopeOptic = { "att_PUScopeOpticOptic" };
		TStringArray att_HuntingOptic = { "HuntingOptic" };
		//! End of General Variants
		
		TStringArray ak74u_1 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_WoodBttstck", "AK_WoodHndgrd" };

		TStringArray sks_1 = { "SKS_Bayonet" };

		array< ref ExpansionLootVariant > sksVariants = {
			new ExpansionLootVariant( "SKS", sks_1, 0.2 ),
			new ExpansionLootVariant( "SKS", NULL, 0.6 ),
		};

		array< ref ExpansionLootVariant > umpVariants = {
			new ExpansionLootVariant( "UMP45", NULL, 0.6 ),
		};

		TStringArray mosin_1 = { "Mosin_Bayonet" };
		TStringArray mosin_2 = { "Mosin_Compensator" };
		TStringArray mosin_3 = { "Mosin_Compensator","att_PUScopeOpticOptic" };

		array< ref ExpansionLootVariant > mosinVariants = {
			new ExpansionLootVariant( "Mosin9130", mosin_3, 0.25 ),
			new ExpansionLootVariant( "Mosin9130", mosin_2, 0.25 ),
			new ExpansionLootVariant( "Mosin9130", mosin_1, 0.25 ),
			new ExpansionLootVariant( "Mosin9130", NULL, 0.25 ),
		};

		array< ref ExpansionLootVariant > b95Variants = {
			new ExpansionLootVariant( "B95", NULL, 0.666667 ),
		};

		array< ref ExpansionLootVariant > cz527Variants = {
			new ExpansionLootVariant( "CZ527", NULL, 0.4 ),
		};

		array< ref ExpansionLootVariant > cz75Variants = {
			new ExpansionLootVariant( "CZ75", att_TLRLightAndPistolSuppressor, 0.1 ),
			new ExpansionLootVariant( "CZ75", att_PistolSuppressor, 0.2 ),
			new ExpansionLootVariant( "CZ75", NULL, 0.5 ),
		};

		array< ref ExpansionLootVariant > fnxVariants = {
			new ExpansionLootVariant( "FNX45", att_TLRLightAndPistolSuppressor, 0.1 ),
			new ExpansionLootVariant( "FNX45", att_PistolSuppressor, 0.15 ),
			new ExpansionLootVariant( "FNX45", NULL, 0.5 ),
		};
		
		#ifdef EXPANSIONMODWEAPONS
		array< ref ExpansionLootVariant > kedrVariants = {
			new ExpansionLootVariant( "Expansion_Kedr", NULL, 0.6 ),
		};
		#endif

		TStringArray winchester70_1 = { "HuntingOptic" };

		array< ref ExpansionLootVariant > winchester70Variants = {
			new ExpansionLootVariant( "Winchester70", NULL, 0.8 ),
		};

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };
		TStringArray visor = { "DirtBikeHelmet_Visor" };
		
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","SalineBagIV" };
		TStringArray firstaidkit_3 = { "CharcoalTablets","Morphine" };
		TStringArray firstaidkit_4 = { "Epinephrine","StartKitIV" };
		TStringArray firstaidkit_5 = { "Morphine","SalineBagIV" };
		TStringArray firstaidkit_6 = { "PainkillerTablets","Epinephrine" };
		TStringArray firstaidkit_7 = { "TetracyclineAntiBiotics","Morphine" };
		TStringArray firstaidkit_8 = { "BandageDressing","VitaminBottle" };

		array< ref ExpansionLootVariant > firstaidkitVariants = {
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_2, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_3, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_4, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_5, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_6, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_7, 0.131579 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_8, 0.131579 ),
		};

		Loot = {
			new ExpansionLoot( "AKS74U", ak74u_1, 0.08 ),
			
			new ExpansionLoot( "AKM", akm_1, 0.05 ),

			new ExpansionLoot( "SKS", att_PUScopeOptic, 0.5, -1, sksVariants ),

			new ExpansionLoot( "UMP45", att_UniversalLight, 0.25, -1, umpVariants ),
 
			new ExpansionLoot( "Mosin9130", att_PUScopeOptic, 0.4, -1, mosinVariants ),
 
			new ExpansionLoot( "B95", att_HuntingOptic, 0.3, -1, b95Variants ),

			new ExpansionLoot( "CZ527", att_HuntingOptic, 0.5, -1, cz527Variants ),

			new ExpansionLoot( "CZ75", att_TLRLight, 0.2, -1, cz75Variants ),

			new ExpansionLoot( "FNX45", att_TLRLight, 0.4, -1, fnxVariants ),

			#ifdef EXPANSIONMODWEAPONS
			new ExpansionLoot( "Expansion_Kedr", att_PistolSuppressor, 0.5, -1, kedrVariants ),

			new ExpansionLoot( "Expansion_DT11", NULL, 0.3 ),
			#endif

   			new ExpansionLoot( "Mp133Shotgun", NULL, 0.8 ),

			new ExpansionLoot( "Winchester70", winchester70_1, 0.5, -1, winchester70Variants ),
				
			
			new ExpansionLoot( "Binoculars", NULL, 0.3 ),
			
			#ifdef NAMALSK_SURVIVAL
			new ExpansionLoot( "dzn_map_namalsk", NULL, 0.3 ),
			#else
			new ExpansionLoot( "ChernarusMap", NULL, 0.3 ),
			#endif
			new ExpansionLoot( "Rangefinder", battery, 0.05 ),
			new ExpansionLoot( "ExpansionGPS", NULL, 0.05 ),
			
			new ExpansionLoot( "BoxCerealCrunchin", NULL, 0.05 ),
			new ExpansionLoot( "PeachesCan", NULL, 0.1 ),
			new ExpansionLoot( "BakedBeansCan", NULL, 0.1 ),
			new ExpansionLoot( "SpaghettiCan", NULL, 0.1 ),
			new ExpansionLoot( "SardinesCan", NULL, 0.1 ),
			new ExpansionLoot( "TunaCan", NULL, 0.1 ),
			new ExpansionLoot( "WaterBottle", NULL, 0.5 ),
			
			new ExpansionLoot( "CanOpener", NULL, 0.5 ),
			new ExpansionLoot( "KitchenKnife", NULL, 0.3 ),
			
			new ExpansionLoot( "BallisticHelmet_UN", NULL, 0.08 ),
			new ExpansionLoot( "DirtBikeHelmet_Chernarus", visor, 0.3 ),
			
			new ExpansionLoot( "SewingKit", NULL, 0.25 ),
			new ExpansionLoot( "LeatherSewingKit", NULL, 0.25 ),
			new ExpansionLoot( "WeaponCleaningKit", NULL, 0.05 ),
			new ExpansionLoot( "Lockpick", NULL, 0.05 ),
			
			new ExpansionLoot( "GhillieAtt_Mossy", NULL, 0.05 ),

			new ExpansionLoot( "Mag_Expansion_Kedr_20Rnd", NULL, 0.8 ),
			new ExpansionLoot( "Mag_CZ527_5rnd", NULL, 0.9 ),
			new ExpansionLoot( "Mag_att_TLRLight5Rnd", NULL, 1 ),
			new ExpansionLoot( "Mag_FNX45_15Rnd", NULL, 1 ),
			new ExpansionLoot( "Mag_UMP_25Rnd", NULL, 0.5 ),

			new ExpansionLoot( "AmmoBox_9x39_20Rnd", NULL, 0.5 ),
			new ExpansionLoot( "AmmoBox_9x19_25Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_762x39Tracer_20Rnd", NULL, 0.5 ),
			new ExpansionLoot( "AmmoBox_762x39_20Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_45ACP_25Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_308WinTracer_20Rnd", NULL, 0.5 ),
			new ExpansionLoot( "AmmoBox_308Win_20Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_12gaRubberSlug_10Rnd", NULL, 1 ),
			new ExpansionLoot( "AmmoBox_12gaPellets_10Rnd", NULL, 1 ),
			
			new ExpansionLoot( "Ammo_9x39", NULL, 0.5 ),
			new ExpansionLoot( "Ammo_762x39Tracer", NULL, 0.5 ),
			new ExpansionLoot( "Ammo_762x39", NULL, 1 ),
			new ExpansionLoot( "Ammo_45ACP", NULL, 1 ),
			new ExpansionLoot( "Ammo_308WinTracer", NULL, 0.5 ),
			new ExpansionLoot( "Ammo_308Win", NULL, 1 ),
			new ExpansionLoot( "Ammo_12gaSlug", NULL, 1 ),
			new ExpansionLoot( "Ammo_12gaRubberSlug", NULL, 1 ),
			new ExpansionLoot( "Ammo_12gaPellets", NULL,  0.5 ),
			
			new ExpansionLoot( "FirstAidKit", firstaidkit_1, 0.38, -1, firstaidkitVariants ),
		};

		return Loot;
	}

	
	static array < ref ExpansionLoot > Medical()
	{
		array < ref ExpansionLoot > Loot;
		
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
		TStringArray firstaidkit_3 = { "Morphine","BandageDressing" };
		TStringArray firstaidkit_4 = { "BandageDressing","SalineBagIV" };
		TStringArray firstaidkit_5 = { "PainkillerTablets","DisinfectantAlcohol" };
		TStringArray firstaidkit_6 = { "CharcoalTablets","Morphine" };
		TStringArray firstaidkit_7 = { "Epinephrine","StartKitIV" };
		TStringArray firstaidkit_8 = { "TetracyclineAntiBiotics","VitaminBottle" };
		TStringArray firstaidkit_9 = { "Morphine","SalineBagIV" };
		TStringArray firstaidkit_10 = { "PainkillerTablets","Epinephrine" };
		TStringArray firstaidkit_11 = { "TetracyclineAntiBiotics","Morphine" };
		TStringArray firstaidkit_12 = { "VitaminBottle","SurgicalGloves_Blue" };
		TStringArray firstaidkit_13 = { "BandageDressing","VitaminBottle" };
		TStringArray firstaidkit_14 = { "CharcoalTablets","DisinfectantAlcohol" };
		TStringArray firstaidkit_15 = { "IodineTincture","PainkillerTablets" };
		TStringArray firstaidkit_16 = { "CharcoalTablets","AntiChemInjector", "BandageDressing" };
		
		TStringArray att_gasmask = { "GasMask_Filter" };

		array< ref ExpansionLootVariant > medicalScrubsHatsVariants = {
			new ExpansionLootVariant( "MedicalScrubsHat_White" ),
			new ExpansionLootVariant( "MedicalScrubsHat_Green" ),
		};

		array< ref ExpansionLootVariant > medicalScrubsPantsVariants = {
			new ExpansionLootVariant( "MedicalScrubsPants_White" ),
			new ExpansionLootVariant( "MedicalScrubsPants_Green" ),
		};

		array< ref ExpansionLootVariant > medicalScrubsShirtsVariants = {
			new ExpansionLootVariant( "MedicalScrubsShirt_White" ),
			new ExpansionLootVariant( "MedicalScrubsShirt_Green" ),
		};

		array< ref ExpansionLootVariant > surgicalGlovesVariants = {
			new ExpansionLootVariant( "SurgicalGloves_LightBlue" ),
			new ExpansionLootVariant( "SurgicalGloves_Green" ),
		};

		array< ref ExpansionLootVariant > firstaidkitVariants = {
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_2, 0.040984 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_3, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_4, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_5, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_6, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_7, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_8, 0.073770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_9, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_10, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_11, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_12, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_13, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_14, 0.063770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_15, 0.033770 ),
			new ExpansionLootVariant( "FirstAidKit", firstaidkit_16, 0.023770 ),
		};

		array< ref ExpansionLootVariant > airborneMaskVariants = {
			new ExpansionLootVariant( "AirborneMask", NULL, 0.80 ),
		};

		array< ref ExpansionLootVariant > gp5GasMaskVariants = {
			new ExpansionLootVariant( "GP5GasMask", NULL, 0.80 ),
		};

		Loot = {
			new ExpansionLoot( "BandageDressing", NULL , 0.15),

			new ExpansionLoot( "Morphine", NULL , 0.1),
			new ExpansionLoot( "Epinephrine", NULL , 0.1),

			new ExpansionLoot( "TacticalBaconCan", NULL , 0.05),

			new ExpansionLoot( "PainkillerTablets", NULL , 0.1),
			new ExpansionLoot( "CharcoalTablets", NULL , 0.1),
			new ExpansionLoot( "TetracyclineAntiBiotics", NULL , 0.1),

			new ExpansionLoot( "DisinfectantSpray", NULL , 0.08),
			new ExpansionLoot( "DisinfectantAlcohol", NULL , 0.1),

			new ExpansionLoot( "VitaminBottle", NULL , 0.2),

			new ExpansionLoot( "StartKitIV", NULL , 0.3),
			new ExpansionLoot( "SalineBagIV", NULL , 0.1),
			new ExpansionLoot( "SalineBag", NULL , 0.1),

			new ExpansionLoot( "BloodBagEmpty", NULL , 0.15),
			new ExpansionLoot( "BloodBagIV", NULL , 0.1),
			new ExpansionLoot( "BloodTestKit", NULL , 0.2),
			new ExpansionLoot( "BloodTestKit", NULL , 0.2),
			
			new ExpansionLoot( "IodineTincture", NULL , 0.2),
			new ExpansionLoot( "AntiChemInjector", NULL , 0.02),

			new ExpansionLoot( "MedicalScrubsHat_Blue", NULL, 0.24, -1, medicalScrubsHatsVariants ),
			new ExpansionLoot( "MedicalScrubsPants_Blue", NULL, 0.24, -1, medicalScrubsPantsVariants ),
			new ExpansionLoot( "MedicalScrubsShirt_Blue", NULL, 0.24, -1, medicalScrubsShirtsVariants ),

			new ExpansionLoot( "NioshFaceMask", NULL , 0.08),

			new ExpansionLoot( "SurgicalGloves_Blue", NULL, 0.24, -1, surgicalGlovesVariants ),

			new ExpansionLoot( "FirstAidKit", firstaidkit_1, 2.44, -1, firstaidkitVariants ),

			new ExpansionLoot( "NBCBootsGray", NULL,  0.05 ),
			new ExpansionLoot( "NBCBootsYellow", NULL,  0.1 ),
			new ExpansionLoot( "NBCGlovesGray", NULL,  0.05 ),
			new ExpansionLoot( "NBCGlovesYellow", NULL,  0.1 ),
			new ExpansionLoot( "NBCHoodGray", NULL,  0.05 ),
			new ExpansionLoot( "NBCHoodYellow", NULL,  0.1 ),
			new ExpansionLoot( "NBCJacketGray", NULL,  0.05 ),
			new ExpansionLoot( "NBCJacketYellow", NULL,  0.1 ),
			new ExpansionLoot( "NBCPantsGray", NULL,  0.05 ),
			new ExpansionLoot( "NBCPantsYellow", NULL,  0.1 ),
			
			new ExpansionLoot( "AirborneMask", att_gasmask,  0.1, -1, airborneMaskVariants ),
			new ExpansionLoot( "GP5GasMask", att_gasmask,  0.1, -1, gp5GasMaskVariants ),
			new ExpansionLoot( "GasMask", NULL,  0.05 ),
		};

		return Loot;
	}

	static array < ref ExpansionLoot > BaseBuilding()
	{
		array < ref ExpansionLoot > Loot;

		Loot = {
			new ExpansionLoot( "NailBox", NULL , 0.8),
			new ExpansionLoot( "DuctTape", NULL , 0.2),
			new ExpansionLoot( "MetalPlate", NULL , 0.25),
			new ExpansionLoot( "WoodenPlank", NULL , 0.5),
			new ExpansionLoot( "Fabric", NULL , 0.5),
			new ExpansionLoot( "Hammer", NULL , 0.4),
			new ExpansionLoot( "Shovel", NULL , 0.1),
			new ExpansionLoot( "Pliers", NULL , 0.1),
			new ExpansionLoot( "WoodAxe", NULL , 0.15),
			new ExpansionLoot( "Crowbar", NULL , 0.3),
			new ExpansionLoot( "Hacksaw", NULL , 0.3),
			new ExpansionLoot( "Handsaw", NULL , 0.1),
			new ExpansionLoot( "SledgeHammer", NULL , 0.15),
			new ExpansionLoot( "Hatchet", NULL , 0.2),
			new ExpansionLoot( "BarbedWire", NULL , 0.15),
			new ExpansionLoot( "MetalWire", NULL , 0.2),
			#ifdef EXPANSIONMODWEAPONS
			new ExpansionLoot( "ExpansionCodeLock", NULL , 0.1),

			new ExpansionLoot( "ExpansionHescoKit", NULL , 0.05),
			new ExpansionLoot( "ExpansionBarbedWireKit", NULL , 0.1),
			new ExpansionLoot( "ExpansionCamoBoxKit", NULL , 0.18),
			new ExpansionLoot( "ExpansionGunrack", NULL , 0.05),

			new ExpansionLoot( "ExpansionCone", NULL , 0.2),
			new ExpansionLoot( "ExpansionSignDanger", NULL , 0.2),
			new ExpansionLoot( "ExpansionSignDanger2", NULL , 0.2),
			new ExpansionLoot( "ExpansionSignDanger3", NULL , 0.2),
			new ExpansionLoot( "ExpansionSignRoadBarrier", NULL , 0.15),

			new ExpansionLoot( "ExpansionSafeMini", NULL , 0.2),
			new ExpansionLoot( "ExpansionSafeMedium", NULL , 0.15),
			new ExpansionLoot( "ExpansionSafeLarge", NULL , 0.1),

			new ExpansionLoot( "ExpansionCamoTentKit", NULL , 0.25),
			#endif
			new ExpansionLoot( "MediumTent", NULL , 0.2),
			new ExpansionLoot( "LargeTent", NULL , 0.1),
			new ExpansionLoot( "CarTent", NULL , 0.15),
			new ExpansionLoot( "CamoNet", NULL , 0.3),
		};

		return Loot;
	}

	static array < ref ExpansionLoot > Military()
	{
		array < ref ExpansionLoot > Loot;

		//! General Variants
		TStringArray att_UniversalLight 				= { "UniversalLight" };

		TStringArray att_TLRLight 						= { "TLRLight" };
		TStringArray att_PistolSuppressor 				= { "PistolSuppressor" };
		TStringArray att_TLRLightAndPistolSuppressor 	= { "TLRLight","PistolSuppressor" };
		
		TStringArray att_PUScopeOptic 					= { "att_PUScopeOpticOptic" };
		TStringArray att_HuntingOptic 					= { "HuntingOptic" };

		TStringArray ak_1 								= { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak_2 								= { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak_3 								= { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };
		TStringArray ak_4 								= { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray ak_5 								= { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray ak_6 								= { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray ak_7 								= { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};
		//! End of General Variants

		TStringArray att_AKPlastic 						= { "AK_PlasticBttstck", "AK_PlasticHndgrd" };
		TStringArray att_AK74 							= { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray att_AKM 							= { "AK_WoodBttstck", "AK_WoodHndgrd" };

		array< ref ExpansionLootVariant > ak74Variants = {
			new ExpansionLootVariant( "AK74", ak_1, 0.1 ),
			new ExpansionLootVariant( "AK74", ak_2, 0.05 ),
			new ExpansionLootVariant( "AK74", ak_3, 0.05 ),
			new ExpansionLootVariant( "AK74", ak_4, 0.1 ),
			new ExpansionLootVariant( "AK74", ak_5, 0.05 ),
			new ExpansionLootVariant( "AK74", ak_6, 0.05 ),
			new ExpansionLootVariant( "AK74", ak_7, 0.05 ),
		};
		array< ref ExpansionLootVariant > akmVariants = {
			new ExpansionLootVariant( "AKM", ak_1, 0.1 ),
			new ExpansionLootVariant( "AKM", ak_2, 0.05 ),
			new ExpansionLootVariant( "AKM", ak_3, 0.05 ),
			new ExpansionLootVariant( "AKM", ak_4, 0.1 ),
			new ExpansionLootVariant( "AKM", ak_5, 0.05 ),
			new ExpansionLootVariant( "AKM", ak_6, 0.05 ),
			new ExpansionLootVariant( "AKM", ak_7, 0.05 ),
		};
		array< ref ExpansionLootVariant > ak101Variants = {
			new ExpansionLootVariant( "AK101", ak_1, 0.1 ),
			new ExpansionLootVariant( "AK101", ak_2, 0.05 ),
			new ExpansionLootVariant( "AK101", ak_3, 0.05 ),
			new ExpansionLootVariant( "AK101", ak_4, 0.1 ),
			new ExpansionLootVariant( "AK101", ak_5, 0.05 ),
			new ExpansionLootVariant( "AK101", ak_6, 0.05 ),
			new ExpansionLootVariant( "AK101", ak_7, 0.05 ),
		};

		TStringArray ak74u_1 = { "AK74_WoodBttstck"};
		TStringArray ak74u_2 = { "AKS74U_Bttstck" };

		array< ref ExpansionLootVariant > ak74uVariants = {
			new ExpansionLootVariant( "AKS74U", ak74u_2, 0.545454 ),
		};

		TStringArray m16_1 = { "ACOGOptic" };

		array< ref ExpansionLootVariant > m16Variants = {
			new ExpansionLootVariant( "Expansion_M16", att_UniversalLight, 0.288889 ),
			new ExpansionLootVariant( "Expansion_M16", NULL, 0.4 ),
		};

		TStringArray m4a1_1 = { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 = { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		array< ref ExpansionLootVariant > m4a1Variants = {
			new ExpansionLootVariant( "M4A1", m4a1_2, 0.220339 ),
			new ExpansionLootVariant( "M4A1", m4a1_3, 0.237288 ),
			new ExpansionLootVariant( "M4A1", m4a1_4, 0.305085 ),
		};

		TStringArray fal_1 = { "Fal_OeBttstck" };
		TStringArray fal_2 = { "Fal_FoldingBttstck" };

		array< ref ExpansionLootVariant > falVariants = {
			new ExpansionLootVariant( "FAL", fal_2, 0.5 ),
		};

		TStringArray svd_1 = { "PSO1Optic"};

		array< ref ExpansionLootVariant > svdVariants = {
			new ExpansionLootVariant( "SVD", NULL, 0.642857 ),
		};

		TStringArray saiga_1 = { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 = { "Saiga_Bttstck" };

		array< ref ExpansionLootVariant > saigaVariants = {
			new ExpansionLootVariant( "Saiga", saiga_2, 0.545454 ),
		};

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };

		Loot = {
			new ExpansionLoot( "AK101", att_AKPlastic , 0.12, -1, ak101Variants),

			new ExpansionLoot( "AK74", att_AK74, 0.495, -1, ak74Variants ),

			new ExpansionLoot( "AKS74U", ak74u_1, 0.55, -1, ak74uVariants ),
			
			new ExpansionLoot( "AKM", att_AKM, 1.3, -1, akmVariants ),

			#ifdef EXPANSIONMODWEAPONS
			new ExpansionLoot( "Expansion_M16", m16_1, 0.45, -1, m16Variants ),
			#endif

			new ExpansionLoot( "M4A1", m4a1_1, 0.59, -1, m4a1Variants ),

			new ExpansionLoot( "FAL", fal_1, 0.24, -1, falVariants ),

			new ExpansionLoot( "SVD", svd_1, 0.28, -1, svdVariants ),

			new ExpansionLoot( "Saiga", saiga_1, 0.22, -1, saigaVariants ),

			new ExpansionLoot( "M79", NULL , 0.1 ),

			#ifdef EXPANSIONMODWEAPONS
			new ExpansionLoot( "ExpansionRPG7", NULL , 0.1 ),
			new ExpansionLoot( "ExpansionLAW", NULL , 0.1 ),
			#endif

			new ExpansionLoot( "AK_RailHndgrd", NULL , 0.1 ),
			new ExpansionLoot( "AK_Bayonet", NULL , 0.1 ),
			new ExpansionLoot( "KobraOptic", NULL , 0.08 ),
			new ExpansionLoot( "UniversalLight", NULL , 0.08),
			new ExpansionLoot( "KobraOptic", battery , 0.08),
			new ExpansionLoot( "UniversalLight", battery , 0.08),
			new ExpansionLoot( "ACOGOptic", NULL , 0.08),
			new ExpansionLoot( "M4_RisHndgrd", NULL , 0.1),
			new ExpansionLoot( "M9A1_Bayonet", NULL , 0.1),
			new ExpansionLoot( "Mosin_Bayonet", NULL , 0.1),
			new ExpansionLoot( "att_PUScopeOpticOptic", NULL , 0.1),
			new ExpansionLoot( "TLRLight", NULL , 0.1),
			new ExpansionLoot( "TLRLight", battery , 0.08),
			new ExpansionLoot( "SKS_Bayonet", NULL , 0.1),

			new ExpansionLoot( "M68Optic", NULL , 0.1),
			new ExpansionLoot( "M4_T3NRDSOptic", NULL , 0.1),
			new ExpansionLoot( "FNP45_MRDSOptic", NULL , 0.1),
			new ExpansionLoot( "ExpansionReflexMRSOptic", NULL , 0.1),
			new ExpansionLoot( "PSO1Optic", NULL , 0.1),
			new ExpansionLoot( "PSO11Optic", NULL , 0.1),
			new ExpansionLoot( "M68Optic", battery , 0.08),
			new ExpansionLoot( "M4_T3NRDSOptic", battery , 0.08),
			new ExpansionLoot( "ExpansionReflexMRSOptic", battery , 0.08),
			new ExpansionLoot( "PSO1Optic", battery , 0.08),
			new ExpansionLoot( "PSO11Optic", battery , 0.08),

			new ExpansionLoot( "RGD5Grenade", NULL , 0.08),
			new ExpansionLoot( "M67Grenade", NULL , 0.05),
			new ExpansionLoot( "M18SmokeGrenade_Red", NULL , 0.1),
			new ExpansionLoot( "M18SmokeGrenade_Green", NULL , 0.1),
			new ExpansionLoot( "M18SmokeGrenade_Yellow", NULL , 0.1),
			new ExpansionLoot( "M18SmokeGrenade_Purple", NULL , 0.1),
			new ExpansionLoot( "M18SmokeGrenade_White", NULL , 0.1),
			new ExpansionLoot( "RDG2SmokeGrenade_Black", NULL , 0.1),
			new ExpansionLoot( "RDG2SmokeGrenade_White", NULL , 0.1),
			new ExpansionLoot( "LandMineTrap", NULL , 0.1),

			new ExpansionLoot( "Mag_AK101_30Rnd", NULL , 0.10),
			new ExpansionLoot( "Mag_AK74_30Rnd", NULL , 0.1),
			new ExpansionLoot( "Mag_AKM_30Rnd", NULL , 0.2),
			new ExpansionLoot( "Mag_AKM_Drum75Rnd", NULL , 0.06),
			new ExpansionLoot( "Mag_AKM_Palm30Rnd", NULL , 0.15),
			new ExpansionLoot( "Mag_CMAG_20Rnd", NULL , 0.3),
			new ExpansionLoot( "Mag_CMAG_30Rnd", NULL , 0.15),
			new ExpansionLoot( "Mag_CMAG_40Rnd", NULL , 0.1),
			new ExpansionLoot( "Mag_FAL_20Rnd", NULL , 0.1),
			new ExpansionLoot( "Mag_STANAGCoupled_30Rnd", NULL , 0.08),
			new ExpansionLoot( "Mag_STANAG_30Rnd", NULL , 0.1),
			new ExpansionLoot( "Mag_SVD_10Rnd", NULL , 0.08),
			new ExpansionLoot( "Mag_Saiga_5Rnd", NULL , 0.15),
			new ExpansionLoot( "Mag_Saiga_8Rnd", NULL , 0.1),
			new ExpansionLoot( "Mag_Saiga_Drum20Rnd", NULL , 0.08),

			new ExpansionLoot( "AmmoBox_762x54Tracer_20Rnd", NULL , 0.08),
			new ExpansionLoot( "AmmoBox_762x54_20Rnd", NULL , 0.1),
			new ExpansionLoot( "AmmoBox_762x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionLoot( "AmmoBox_762x39_20Rnd", NULL , 0.09),
			new ExpansionLoot( "AmmoBox_556x45Tracer_20Rnd", NULL , 0.05),
			new ExpansionLoot( "AmmoBox_556x45_20Rnd", NULL , 0.08),
			new ExpansionLoot( "AmmoBox_545x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionLoot( "AmmoBox_545x39_20Rnd", NULL , 0.08),
			new ExpansionLoot( "Ammo_762x54Tracer", NULL , 0.08),
			new ExpansionLoot( "Ammo_762x54", NULL , 0.1),
			new ExpansionLoot( "Ammo_762x39Tracer", NULL , 0.1),
			new ExpansionLoot( "Ammo_762x39", NULL , 0.12),
			new ExpansionLoot( "Ammo_556x45Tracer", NULL , 0.07),
			new ExpansionLoot( "Ammo_556x45", NULL , 0.11),
			new ExpansionLoot( "Ammo_545x39Tracer", NULL , 0.07),
			new ExpansionLoot( "Ammo_545x39", NULL , 0.10),

			#ifdef EXPANSIONMODWEAPONS
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_White", NULL , 0.08),
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Red", NULL , 0.08),
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Green", NULL , 0.08),
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Yellow", NULL , 0.08),
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Purple", NULL , 0.08),
			new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Teargas", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_White", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Red", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Green", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Yellow", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Purple", NULL , 0.05),
			new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Teargas", NULL , 0.02),
			new ExpansionLoot( "Ammo_Expansion_M203_HE", NULL , 0.7),

			new ExpansionLoot( "ExpansionAmmoRPG", NULL , 0.05),
			#endif

			new ExpansionLoot( "UKAssVest_Black", NULL , 0.1),
			new ExpansionLoot( "UKAssVest_Camo", NULL , 0.1),
			new ExpansionLoot( "UKAssVest_Khaki", NULL , 0.1),
			new ExpansionLoot( "UKAssVest_Olive", NULL , 0.1),

			new ExpansionLoot( "HighCapacityVest_Black", NULL , 0.1),
			new ExpansionLoot( "HighCapacityVest_Olive", NULL , 0.1),

			new ExpansionLoot( "PlateCarrierVest", NULL , 0.08),
			new ExpansionLoot( "PlateCarrierHolster", NULL , 0.1),
			new ExpansionLoot( "PlateCarrierPouches", NULL , 0.1),
			new ExpansionLoot( "PlateCarrierVest", vest , 0.05),

			new ExpansionLoot( "TacticalBaconCan", NULL , 0.1),

			new ExpansionLoot( "Bear_Pink", NULL , 0.1),
			
			new ExpansionLoot( "GhillieAtt_Mossy", NULL , 0.2),
			new ExpansionLoot( "GhillieHood_Mossy", NULL , 0.1),
			new ExpansionLoot( "GhillieBushrag_Mossy", NULL , 0.05),
			new ExpansionLoot( "GhillieSuit_Mossy", NULL , 0.05),
		};

		return Loot;
	}
};
