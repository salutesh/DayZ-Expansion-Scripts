modded class ExpansionLootDefaults
{
	static void Expansion_NA_Weapons_Civilian(out array < ref ExpansionLoot > Loot)
	{
		//! General Variants
		TStringArray battery = { "Battery9V" };

		TStringArray att_UniversalLight 				= { "UniversalLight" };

		TStringArray att_TLRLight 						= { "TLRLight" };
		TStringArray att_PistolSuppressor 				= { "PistolSuppressor" };
		TStringArray att_TLRLightAndPistolSuppressor 	= { "TLRLight","PistolSuppressor" };

		TStringArray att_PUScopeOptic 					= { "PUScopeOptic" };
		TStringArray att_HuntingOptic 					= { "HuntingOptic" };

		TStringArray att_PSO1Optic 						= { "PSO1Optic" };
		TStringArray att_PSO11Optic 					= { "PSO11Optic" };
		TStringArray att_KazuarOptic 					= { "KazuarOptic" };
		TStringArray att_KobraOptic 					= { "KobraOptic" };
		TStringArray att_KashtanOptic 					= { "KashtanOptic" };

		TStringArray att_ReflexOptic 					= { "ReflexOptic" };
		TStringArray att_ACOGOptic 						= { "ACOGOptic" };
		TStringArray att_ACOGOptic_6x 					= { "ACOGOptic_6x" };
		TStringArray att_M68Optic 						= { "M68Optic" };
		TStringArray att_M4_T3NRDSOptic 				= { "M4_T3NRDSOptic" };
		TStringArray att_StarlightOptic 				= { "StarlightOptic" };
		#ifdef EXPANSIONMODWEAPONS
		TStringArray att_ExpansionReflexMRSOptic 		= { "ExpansionReflexMRSOptic" };
		TStringArray att_ExpansionEXPS3HoloOptic 		= { "ExpansionEXPS3HoloOptic" };
		TStringArray att_ExpansionHAMROptic 			= { "ExpansionHAMROptic" };
		#endif

		TStringArray att_AKPlastic 						= { "AK_PlasticBttstck", "AK_PlasticHndgrd" };
		TStringArray att_AK74 							= { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray att_AKM 							= { "AK_WoodBttstck", "AK_WoodHndgrd" };
		//! End of General Variants

		TStringArray ak_1 								= { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak_2 								= { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak_3 								= { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };
		TStringArray ak_4 								= { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray ak_5 								= { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray ak_6 								= { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray ak_7 								= { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};

		TStringArray ak74u_1 							= { "AK74_WoodBttstck"};
		TStringArray ak74u_2 							= { "AKS74U_Bttstck" };

		TStringArray expansionm16_1 					= { "ExpansionEXPS3HoloOptic","UniversalLight" };
		TStringArray expansionm16_2 					= { "M68Optic","UniversalLight" };
		TStringArray expansionm16_3 					= { "ReflexOptic","UniversalLight" };

		TStringArray m4a1_1 							= { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 							= { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 							= { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 							= { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		TStringArray fal_1 								= { "Fal_OeBttstck" };
		TStringArray fal_2 								= { "Fal_FoldingBttstck" };
		TStringArray fal_3 								= { "Fal_OeBttstck", "ACOGOptic" };
		TStringArray fal_4 								= { "Fal_FoldingBttstck", "ReflexOptic" };

		TStringArray saiga_1 							= { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 							= { "Saiga_Bttstck" };

		TStringArray sks_1 								= { "SKS_Bayonet" };

		TStringArray mosin_1 							= { "Mosin_Bayonet" };
		TStringArray mosin_2 							= { "Mosin_Compensator" };
		TStringArray mosin_3 							= { "Mosin_Compensator","PUScopeOptic" };

		TStringArray winchester70_1 					= { "HuntingOptic" };

		array< ref ExpansionLootVariant > sksVariants = {
			new ExpansionLootVariant( "SKS", sks_1, 0.2 ),
			new ExpansionLootVariant( "SKS", NULL, 0.6 ),
		};

		array< ref ExpansionLootVariant > umpVariants = {
			new ExpansionLootVariant( "UMP45", NULL, 0.6 ),
		};

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

		array< ref ExpansionLootVariant > kedrVariants = {
			new ExpansionLootVariant( "Expansion_Kedr", NULL, 0.6 ),
		};

		array< ref ExpansionLootVariant > winchester70Variants = {
			new ExpansionLootVariant( "Winchester70", NULL, 0.8 ),
		};

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

		array< ref ExpansionLootVariant > ak74uVariants = {
			new ExpansionLootVariant( "AKS74U", ak74u_2, 0.545454 ),
		};

		array< ref ExpansionLootVariant > expansionm16Variants = {
			new ExpansionLootVariant( "Expansion_M16", expansionm16_1, 0.1 ),
			new ExpansionLootVariant( "Expansion_M16", expansionm16_2, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", expansionm16_3, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", att_ACOGOptic_6x, 0.1 ),
			new ExpansionLootVariant( "Expansion_M16", att_StarlightOptic, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", NULL, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", att_UniversalLight, 0.05 ),
		};

		array< ref ExpansionLootVariant > m4a1Variants = {
			new ExpansionLootVariant( "M4A1", m4a1_2, 0.220339 ),
			new ExpansionLootVariant( "M4A1", m4a1_3, 0.237288 ),
			new ExpansionLootVariant( "M4A1", m4a1_4, 0.305085 ),
		};

		array< ref ExpansionLootVariant > falVariants = {
			new ExpansionLootVariant( "FAL", fal_2, 0.40 ),
			new ExpansionLootVariant( "FAL", fal_3, 0.20 ),
			new ExpansionLootVariant( "FAL", fal_4, 0.20 ),
		};

		array< ref ExpansionLootVariant > scoutVariants = {
			new ExpansionLootVariant( "Scout", NULL, 0.642857 ),
			new ExpansionLootVariant( "Scout", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "Scout", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "Scout", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "Scout", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > expansionAWMVariants = {
			new ExpansionLootVariant( "Expansion_AWM", NULL, 0.642857 ),
			new ExpansionLootVariant( "Expansion_AWM", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "Expansion_AWM", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "Expansion_AWM", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "Expansion_AWM", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > vssVariants = {
			new ExpansionLootVariant( "VSS", NULL, 0.642857 ),
			new ExpansionLootVariant( "VSS", att_KazuarOptic, 0.082857 ),
		};

		array< ref ExpansionLootVariant > asvalVariants = {
			new ExpansionLootVariant( "ASVAL", NULL, 0.642857 ),
			new ExpansionLootVariant( "ASVAL", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "ASVAL", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "ASVAL", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "ASVAL", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > svdVariants = {
			new ExpansionLootVariant( "SVD", NULL, 0.642857 ),
			new ExpansionLootVariant( "SVD", att_KazuarOptic, 0.082857 ),
		};

		array< ref ExpansionLootVariant > saigaVariants = {
			new ExpansionLootVariant( "Saiga", saiga_2, 0.545454 ),
		};

		Loot.Insert(new ExpansionLoot( "SKS", att_PUScopeOptic, 0.5, -1, sksVariants ));
		Loot.Insert(new ExpansionLoot( "UMP45", att_UniversalLight, 0.25, -1, umpVariants ));
		Loot.Insert(new ExpansionLoot( "Mosin9130", att_PUScopeOptic, 0.4, -1, mosinVariants ));
		Loot.Insert(new ExpansionLoot( "B95", att_HuntingOptic, 0.3, -1, b95Variants ));
		Loot.Insert(new ExpansionLoot( "CZ527", att_HuntingOptic, 0.5, -1, cz527Variants ));
		Loot.Insert(new ExpansionLoot( "CZ75", att_TLRLight, 0.2, -1, cz75Variants ));
		Loot.Insert(new ExpansionLoot( "FNX45", att_TLRLight, 0.4, -1, fnxVariants ));
		Loot.Insert(new ExpansionLoot( "Glock19", att_TLRLight, 0.4, -1, fnxVariants ));
		Loot.Insert(new ExpansionLoot( "MKII", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "Magnum", NULL , 0.05 ));
		Loot.Insert(new ExpansionLoot( "Deagle", NULL , 0.125 ));
		Loot.Insert(new ExpansionLoot( "P1", NULL , 0.175 ));
		Loot.Insert(new ExpansionLoot( "Longhorn", NULL , 0.125 ));
   		Loot.Insert(new ExpansionLoot( "Mp133Shotgun", NULL, 0.8 ));
		Loot.Insert(new ExpansionLoot( "Winchester70", winchester70_1, 0.5, -1, winchester70Variants ));
		Loot.Insert(new ExpansionLoot( "AK101", att_AKPlastic , 0.12, -1, ak101Variants));
		Loot.Insert(new ExpansionLoot( "AK74", att_AK74, 0.495, -1, ak74Variants ));
		Loot.Insert(new ExpansionLoot( "AKS74U", ak74u_1, 0.55, -1, ak74uVariants ));
		Loot.Insert(new ExpansionLoot( "AKM", att_AKM, 0.6, -1, akmVariants ));
		Loot.Insert(new ExpansionLoot( "M4A1", m4a1_1, 0.375, -1, m4a1Variants ));
		Loot.Insert(new ExpansionLoot( "M16A2", NULL , 0.35 ));
		Loot.Insert(new ExpansionLoot( "FAMAS", NULL , 0.2 ));
		Loot.Insert(new ExpansionLoot( "Aug", NULL , 0.3 ));

		Loot.Insert(new ExpansionLoot( "RGD5Grenade", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "M67Grenade", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Red", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Green", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Yellow", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Purple", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_White", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "RDG2SmokeGrenade_Black", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "RDG2SmokeGrenade_White", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "LandMineTrap", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "Mag_AK101_30Rnd", NULL , 0.10));
		Loot.Insert(new ExpansionLoot( "Mag_AK74_30Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_AKM_30Rnd", NULL , 0.2));
		Loot.Insert(new ExpansionLoot( "Mag_AKM_Palm30Rnd", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "Mag_CMAG_20Rnd", NULL , 0.3));
		Loot.Insert(new ExpansionLoot( "Mag_CMAG_30Rnd", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "Mag_STANAG_30Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_CZ527_5rnd", NULL, 0.9 ));
		Loot.Insert(new ExpansionLoot( "Mag_Deagle_9Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Mag_FNX45_15Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Mag_UMP_25Rnd", NULL, 0.5 ));

			#ifdef EXPANSIONMODWEAPONS
		Loot.Insert(new ExpansionLoot( "Expansion_Kedr", att_PistolSuppressor, 0.5, -1, kedrVariants ));
		Loot.Insert(new ExpansionLoot( "Expansion_DT11", NULL, 0.3 ));
		Loot.Insert(new ExpansionLoot( "Expansion_M9", att_TLRLight, 0.4, -1, fnxVariants ));

		Loot.Insert(new ExpansionLoot( "Mag_Expansion_M9_15Rnd", NULL, 0.2 ));
		Loot.Insert(new ExpansionLoot( "Mag_Expansion_Kedr_20Rnd", NULL, 0.4 ));
			#endif
	}

	static void Expansion_NA_Weapons_Military(out array < ref ExpansionLoot > Loot)
	{
		//! General Variants
		TStringArray battery = { "Battery9V" };

		TStringArray att_UniversalLight 				= { "UniversalLight" };

		TStringArray att_TLRLight 						= { "TLRLight" };
		TStringArray att_PistolSuppressor 				= { "PistolSuppressor" };
		TStringArray att_TLRLightAndPistolSuppressor 	= { "TLRLight","PistolSuppressor" };

		TStringArray att_PUScopeOptic 					= { "PUScopeOptic" };
		TStringArray att_HuntingOptic 					= { "HuntingOptic" };

		TStringArray att_PSO1Optic 						= { "PSO1Optic" };
		TStringArray att_PSO11Optic 					= { "PSO11Optic" };
		TStringArray att_KazuarOptic 					= { "KazuarOptic" };
		TStringArray att_KobraOptic 					= { "KobraOptic" };
		TStringArray att_KashtanOptic 					= { "KashtanOptic" };

		TStringArray att_ReflexOptic 					= { "ReflexOptic" };
		TStringArray att_ACOGOptic 						= { "ACOGOptic" };
		TStringArray att_ACOGOptic_6x 					= { "ACOGOptic_6x" };
		TStringArray att_M68Optic 						= { "M68Optic" };
		TStringArray att_M4_T3NRDSOptic 				= { "M4_T3NRDSOptic" };
		TStringArray att_StarlightOptic 				= { "StarlightOptic" };
	#ifdef EXPANSIONMODWEAPONS
		TStringArray att_ExpansionReflexMRSOptic 		= { "ExpansionReflexMRSOptic" };
		TStringArray att_ExpansionEXPS3HoloOptic 		= { "ExpansionEXPS3HoloOptic" };
		TStringArray att_ExpansionHAMROptic 			= { "ExpansionHAMROptic" };
	#endif

		TStringArray att_AKPlastic 						= { "AK_PlasticBttstck", "AK_PlasticHndgrd" };
		TStringArray att_AK74 							= { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray att_AKM 							= { "AK_WoodBttstck", "AK_WoodHndgrd" };
		//! End of General Variants

		TStringArray ak_1 								= { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak_2 								= { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak_3 								= { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };
		TStringArray ak_4 								= { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray ak_5 								= { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray ak_6 								= { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray ak_7 								= { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};

		TStringArray ak74u_1 							= { "AK74_WoodBttstck"};
		TStringArray ak74u_2 							= { "AKS74U_Bttstck" };

		TStringArray expansionm16_1 					= { "ExpansionEXPS3HoloOptic","UniversalLight" };
		TStringArray expansionm16_2 					= { "M68Optic","UniversalLight" };
		TStringArray expansionm16_3 					= { "ReflexOptic","UniversalLight" };

		TStringArray m4a1_1 							= { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 							= { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 							= { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 							= { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		TStringArray fal_1 								= { "Fal_OeBttstck" };
		TStringArray fal_2 								= { "Fal_FoldingBttstck" };
		TStringArray fal_3 								= { "Fal_OeBttstck", "ACOGOptic" };
		TStringArray fal_4 								= { "Fal_FoldingBttstck", "ReflexOptic" };

		TStringArray saiga_1 							= { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 							= { "Saiga_Bttstck" };

		TStringArray sks_1 								= { "SKS_Bayonet" };

		TStringArray mosin_1 							= { "Mosin_Bayonet" };
		TStringArray mosin_2 							= { "Mosin_Compensator" };
		TStringArray mosin_3 							= { "Mosin_Compensator","PUScopeOptic" };

		TStringArray winchester70_1 					= { "HuntingOptic" };

		array< ref ExpansionLootVariant > sksVariants = {
			new ExpansionLootVariant( "SKS", sks_1, 0.2 ),
			new ExpansionLootVariant( "SKS", NULL, 0.6 ),
		};

		array< ref ExpansionLootVariant > umpVariants = {
			new ExpansionLootVariant( "UMP45", NULL, 0.6 ),
		};

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

		array< ref ExpansionLootVariant > kedrVariants = {
			new ExpansionLootVariant( "Expansion_Kedr", NULL, 0.6 ),
		};

		array< ref ExpansionLootVariant > winchester70Variants = {
			new ExpansionLootVariant( "Winchester70", NULL, 0.8 ),
		};

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

		array< ref ExpansionLootVariant > ak74uVariants = {
			new ExpansionLootVariant( "AKS74U", ak74u_2, 0.545454 ),
		};

		array< ref ExpansionLootVariant > expansionm16Variants = {
			new ExpansionLootVariant( "Expansion_M16", expansionm16_1, 0.1 ),
			new ExpansionLootVariant( "Expansion_M16", expansionm16_2, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", expansionm16_3, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", att_ACOGOptic_6x, 0.1 ),
			new ExpansionLootVariant( "Expansion_M16", att_StarlightOptic, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", NULL, 0.05 ),
			new ExpansionLootVariant( "Expansion_M16", att_UniversalLight, 0.05 ),
		};

		array< ref ExpansionLootVariant > m4a1Variants = {
			new ExpansionLootVariant( "M4A1", m4a1_2, 0.220339 ),
			new ExpansionLootVariant( "M4A1", m4a1_3, 0.237288 ),
			new ExpansionLootVariant( "M4A1", m4a1_4, 0.305085 ),
		};

		array< ref ExpansionLootVariant > falVariants = {
			new ExpansionLootVariant( "FAL", fal_2, 0.40 ),
			new ExpansionLootVariant( "FAL", fal_3, 0.20 ),
			new ExpansionLootVariant( "FAL", fal_4, 0.20 ),
		};

		array< ref ExpansionLootVariant > scoutVariants = {
			new ExpansionLootVariant( "Scout", NULL, 0.642857 ),
			new ExpansionLootVariant( "Scout", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "Scout", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "Scout", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "Scout", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > expansionAWMVariants = {
			new ExpansionLootVariant( "Expansion_AWM", NULL, 0.642857 ),
			new ExpansionLootVariant( "Expansion_AWM", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "Expansion_AWM", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "Expansion_AWM", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "Expansion_AWM", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > vssVariants = {
			new ExpansionLootVariant( "VSS", NULL, 0.642857 ),
			new ExpansionLootVariant( "VSS", att_KazuarOptic, 0.082857 ),
		};

		array< ref ExpansionLootVariant > asvalVariants = {
			new ExpansionLootVariant( "ASVAL", NULL, 0.642857 ),
			new ExpansionLootVariant( "ASVAL", att_ReflexOptic, 0.2 ),
			new ExpansionLootVariant( "ASVAL", att_ACOGOptic, 0.1 ),
			new ExpansionLootVariant( "ASVAL", att_M68Optic, 0.15 ),
			new ExpansionLootVariant( "ASVAL", att_M4_T3NRDSOptic, 0.1 ),
		};

		array< ref ExpansionLootVariant > svdVariants = {
			new ExpansionLootVariant( "SVD", NULL, 0.642857 ),
			new ExpansionLootVariant( "SVD", att_KazuarOptic, 0.082857 ),
		};

		array< ref ExpansionLootVariant > saigaVariants = {
			new ExpansionLootVariant( "Saiga", saiga_2, 0.545454 ),
		};

		Loot.Insert(new ExpansionLoot( "SKS", att_PUScopeOptic, 0.5, -1, sksVariants ));
		Loot.Insert(new ExpansionLoot( "UMP45", att_UniversalLight, 0.25, -1, umpVariants ));
		Loot.Insert(new ExpansionLoot( "Mosin9130", att_PUScopeOptic, 0.4, -1, mosinVariants ));
		Loot.Insert(new ExpansionLoot( "B95", att_HuntingOptic, 0.3, -1, b95Variants ));
		Loot.Insert(new ExpansionLoot( "CZ527", att_HuntingOptic, 0.5, -1, cz527Variants ));
		Loot.Insert(new ExpansionLoot( "CZ75", att_TLRLight, 0.2, -1, cz75Variants ));
		Loot.Insert(new ExpansionLoot( "FNX45", att_TLRLight, 0.4, -1, fnxVariants ));
		Loot.Insert(new ExpansionLoot( "Glock19", att_TLRLight, 0.4, -1, fnxVariants ));
		Loot.Insert(new ExpansionLoot( "MKII", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "Magnum", NULL , 0.05 ));
		Loot.Insert(new ExpansionLoot( "Deagle", NULL , 0.125 ));
		Loot.Insert(new ExpansionLoot( "P1", NULL , 0.175 ));
		Loot.Insert(new ExpansionLoot( "Longhorn", NULL , 0.125 ));
   		Loot.Insert(new ExpansionLoot( "Mp133Shotgun", NULL, 0.8 ));
		Loot.Insert(new ExpansionLoot( "Winchester70", winchester70_1, 0.5, -1, winchester70Variants ));
		Loot.Insert(new ExpansionLoot( "AK101", att_AKPlastic , 0.12, -1, ak101Variants));
		Loot.Insert(new ExpansionLoot( "AK74", att_AK74, 0.495, -1, ak74Variants ));
		Loot.Insert(new ExpansionLoot( "AKS74U", ak74u_1, 0.55, -1, ak74uVariants ));
		Loot.Insert(new ExpansionLoot( "AKM", att_AKM, 1.3, -1, akmVariants ));
		Loot.Insert(new ExpansionLoot( "M4A1", m4a1_1, 0.59, -1, m4a1Variants ));
		Loot.Insert(new ExpansionLoot( "FAL", fal_1, 0.24, -1, falVariants ));
		Loot.Insert(new ExpansionLoot( "SVD", att_PSO1Optic, 0.28, -1, svdVariants ));
		Loot.Insert(new ExpansionLoot( "VSS", att_PSO11Optic, 0.10, -1, vssVariants ));
		Loot.Insert(new ExpansionLoot( "ASVAL", att_ACOGOptic_6x, 0.05, -1, asvalVariants ));
		Loot.Insert(new ExpansionLoot( "Saiga", saiga_1, 0.22, -1, saigaVariants ));
		Loot.Insert(new ExpansionLoot( "M79", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "M16A2", NULL , 0.60 ));
		Loot.Insert(new ExpansionLoot( "FAMAS", NULL , 0.55 ));
		Loot.Insert(new ExpansionLoot( "Aug", NULL , 0.60 ));
		Loot.Insert(new ExpansionLoot( "AugShort", NULL , 0.45 ));
		Loot.Insert(new ExpansionLoot( "Scout", att_ACOGOptic_6x, 0.05, -1, scoutVariants ));

		Loot.Insert(new ExpansionLoot( "RGD5Grenade", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "M67Grenade", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Red", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Green", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Yellow", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_Purple", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M18SmokeGrenade_White", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "RDG2SmokeGrenade_Black", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "RDG2SmokeGrenade_White", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "LandMineTrap", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "Mag_AK101_30Rnd", NULL , 0.10));
		Loot.Insert(new ExpansionLoot( "Mag_AK74_30Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_AKM_30Rnd", NULL , 0.2));
		Loot.Insert(new ExpansionLoot( "Mag_AKM_Drum75Rnd", NULL , 0.06));
		Loot.Insert(new ExpansionLoot( "Mag_AKM_Palm30Rnd", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "Mag_CMAG_20Rnd", NULL , 0.3));
		Loot.Insert(new ExpansionLoot( "Mag_CMAG_30Rnd", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "Mag_CMAG_40Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_FAL_20Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_STANAGCoupled_30Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Mag_STANAG_30Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_SVD_10Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Mag_Saiga_5Rnd", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "Mag_Saiga_8Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mag_Saiga_Drum20Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Mag_CZ527_5rnd", NULL, 0.9 ));
		Loot.Insert(new ExpansionLoot( "Mag_Deagle_9Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Mag_FNX45_15Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Mag_UMP_25Rnd", NULL, 0.5 ));

	#ifdef EXPANSIONMODWEAPONS
		Loot.Insert(new ExpansionLoot( "Expansion_Kedr", att_PistolSuppressor, 0.5, -1, kedrVariants ));
		Loot.Insert(new ExpansionLoot( "Expansion_DT11", NULL, 0.3 ));
		Loot.Insert(new ExpansionLoot( "Expansion_M16", att_M4_T3NRDSOptic, 0.45, -1, expansionm16Variants ));
		Loot.Insert(new ExpansionLoot( "ExpansionRPG7", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "ExpansionLAW", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "Expansion_M9", att_TLRLight, 0.4, -1, fnxVariants ));
		Loot.Insert(new ExpansionLoot( "Expansion_AWM", att_ACOGOptic_6x, 0.015, -1, expansionAWMVariants ));

		Loot.Insert(new ExpansionLoot( "Mag_Expansion_AWM_5Rnd", NULL, 0.08 ));
		Loot.Insert(new ExpansionLoot( "Mag_Expansion_M9_15Rnd", NULL, 0.25 ));
		Loot.Insert(new ExpansionLoot( "Mag_Expansion_Kedr_20Rnd", NULL, 0.3 ));
	#endif
	}

	static void Expansion_NA_Ammo_Civilian(out array < ref ExpansionLoot > Loot)
	{
		Loot.Insert(new ExpansionLoot( "AmmoBox_9x39_20Rnd", NULL, 0.5 ));
		Loot.Insert(new ExpansionLoot( "AmmoBox_9x19_25Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "AmmoBox_762x39_20Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "AmmoBox_45ACP_25Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "AmmoBox_308Win_20Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_12gaPellets", NULL, 1 ));

		Loot.Insert(new ExpansionLoot( "Ammo_9x39", NULL, 0.5 ));
		Loot.Insert(new ExpansionLoot( "Ammo_762x39", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_45ACP", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_308Win", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_12gaSlug", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_12gaRubberSlug", NULL, 1 ));
		Loot.Insert(new ExpansionLoot( "Ammo_12gaPellets", NULL,  0.5 ));
	}

	static void Expansion_NA_Ammo_Military(out array < ref ExpansionLoot > Loot)
	{
		Loot.Insert(new ExpansionLoot( "AmmoBox_762x54Tracer_20Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "AmmoBox_762x54_20Rnd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "AmmoBox_762x39Tracer_20Rnd", NULL , 0.06));
		Loot.Insert(new ExpansionLoot( "AmmoBox_762x39_20Rnd", NULL , 0.09));
		Loot.Insert(new ExpansionLoot( "AmmoBox_556x45Tracer_20Rnd", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "AmmoBox_556x45_20Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "AmmoBox_545x39Tracer_20Rnd", NULL , 0.06));
		Loot.Insert(new ExpansionLoot( "AmmoBox_545x39_20Rnd", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_762x54Tracer", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_762x54", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Ammo_762x39Tracer", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Ammo_762x39", NULL , 0.12));
		Loot.Insert(new ExpansionLoot( "Ammo_556x45Tracer", NULL , 0.07));
		Loot.Insert(new ExpansionLoot( "Ammo_556x45", NULL , 0.11));
		Loot.Insert(new ExpansionLoot( "Ammo_545x39Tracer", NULL , 0.07));
		Loot.Insert(new ExpansionLoot( "Ammo_545x39", NULL , 0.10));

	#ifdef EXPANSIONMODWEAPONS
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_White", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Red", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Green", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Yellow", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Purple", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Smoke_Teargas", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_White", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Red", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Green", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Yellow", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Purple", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "Ammo_Expansion_M203_Sticky_Smoke_Teargas", NULL , 0.02));

		Loot.Insert(new ExpansionLoot( "ExpansionAmmoRPG", NULL , 0.05));
	#endif
	}

	static void Expansion_NA_Attachments(out array < ref ExpansionLoot > Loot)
	{
		TStringArray battery = { "Battery9V" };

		Loot.Insert(new ExpansionLoot( "AK_RailHndgrd", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "AK_Bayonet", NULL , 0.1 ));
		Loot.Insert(new ExpansionLoot( "KobraOptic", NULL , 0.08 ));
		Loot.Insert(new ExpansionLoot( "UniversalLight", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "KobraOptic", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "UniversalLight", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "ACOGOptic", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "ACOGOptic_6x", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "M4_RisHndgrd", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M9A1_Bayonet", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Mosin_Bayonet", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "PUScopeOptic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "TLRLight", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "TLRLight", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "SKS_Bayonet", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "M68Optic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M4_T3NRDSOptic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "FNP45_MRDSOptic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "PSO1Optic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "PSO11Optic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "M68Optic", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "M4_T3NRDSOptic", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "PSO1Optic", battery , 0.08));
		Loot.Insert(new ExpansionLoot( "PSO11Optic", battery , 0.08));

	#ifdef EXPANSIONMODWEAPONS
		Loot.Insert(new ExpansionLoot( "ExpansionReflexMRSOptic", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "ExpansionReflexMRSOptic", battery , 0.08));
	#endif
	}

	static void Expansion_NA_Food(out array < ref ExpansionLoot > Loot)
	{
		Loot.Insert(new ExpansionLoot( "BoxCerealCrunchin", NULL, 0.05 ));
		Loot.Insert(new ExpansionLoot( "dzn_PeachesCan", NULL, 0.1 ));
		Loot.Insert(new ExpansionLoot( "dzn_BakedBeansCan", NULL, 0.1 ));
		Loot.Insert(new ExpansionLoot( "dzn_SpaghettiCan", NULL, 0.1 ));
		Loot.Insert(new ExpansionLoot( "dzn_SardinesCan", NULL, 0.1 ));
		Loot.Insert(new ExpansionLoot( "dzn_TunaCan", NULL, 0.1 ));
		Loot.Insert(new ExpansionLoot( "dzn_WaterBottle", NULL, 0.5 ));

		Loot.Insert(new ExpansionLoot( "TacticalBaconCan", NULL , 0.025));

		Loot.Insert(new ExpansionLoot( "CanOpener", NULL, 0.5 ));
		Loot.Insert(new ExpansionLoot( "KitchenKnife", NULL, 0.3 ));
	}

	static void Expansion_NA_NBC(out array < ref ExpansionLoot > Loot)
	{
		TStringArray att_gasmask = { "GasMask_Filter" };

		array< ref ExpansionLootVariant > airborneMaskVariants = {
			new ExpansionLootVariant( "AirborneMask", NULL, 0.80 ),
		};

		array< ref ExpansionLootVariant > gp5GasMaskVariants = {
			new ExpansionLootVariant( "GP5GasMask", NULL, 0.80 ),
		};

		Loot.Insert(new ExpansionLoot( "NBCBootsGray", NULL,  0.05 ));
		Loot.Insert(new ExpansionLoot( "NBCBootsYellow", NULL,  0.1 ));
		Loot.Insert(new ExpansionLoot( "NBCGlovesGray", NULL,  0.05 ));
		Loot.Insert(new ExpansionLoot( "NBCGlovesYellow", NULL,  0.1 ));
		Loot.Insert(new ExpansionLoot( "NBCHoodGray", NULL,  0.05 ));
		Loot.Insert(new ExpansionLoot( "NBCHoodYellow", NULL,  0.1 ));
		Loot.Insert(new ExpansionLoot( "NBCJacketGray", NULL,  0.05 ));
		Loot.Insert(new ExpansionLoot( "NBCJacketYellow", NULL,  0.1 ));
		Loot.Insert(new ExpansionLoot( "NBCPantsGray", NULL,  0.05 ));
		Loot.Insert(new ExpansionLoot( "NBCPantsYellow", NULL,  0.1 ));

		Loot.Insert(new ExpansionLoot( "AirborneMask", att_gasmask,  0.1, -1, airborneMaskVariants ));
		Loot.Insert(new ExpansionLoot( "GP5GasMask", att_gasmask,  0.1, -1, gp5GasMaskVariants ));
		Loot.Insert(new ExpansionLoot( "GasMask", NULL,  0.05 ));
	}

	static void Expansion_NA_Gear_Medical(out array < ref ExpansionLoot > Loot)
	{
		TStringArray firstaidkit_1 = { "BandageDressing","BandageDressing" };
		TStringArray firstaidkit_2 = { "BandageDressing","BandageDressing","BandageDressing","BandageDressing" };
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

		Loot.Insert(new ExpansionLoot( "FirstAidKit", firstaidkit_1, 2.44, -1, firstaidkitVariants ));

		Loot.Insert(new ExpansionLoot( "AntiChemInjector", NULL , 0.02));

		Loot.Insert(new ExpansionLoot( "BandageDressing", NULL , 0.15));

		Loot.Insert(new ExpansionLoot( "Morphine", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "Epinephrine", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "PainkillerTablets", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "CharcoalTablets", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "TetracyclineAntiBiotics", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "DisinfectantSpray", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "DisinfectantAlcohol", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "VitaminBottle", NULL , 0.2));

		Loot.Insert(new ExpansionLoot( "StartKitIV", NULL , 0.3));
		Loot.Insert(new ExpansionLoot( "SalineBagIV", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "SalineBag", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "BloodBagEmpty", NULL , 0.15));
		Loot.Insert(new ExpansionLoot( "BloodBagIV", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "BloodTestKit", NULL , 0.2));
		Loot.Insert(new ExpansionLoot( "BloodTestKit", NULL , 0.2));

		Loot.Insert(new ExpansionLoot( "IodineTincture", NULL , 0.2));
	}

	static void Expansion_NA_Clothing_Medical(out array < ref ExpansionLoot > Loot)
	{
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

		Loot.Insert(new ExpansionLoot( "MedicalScrubsHat_Blue", NULL, 0.24, -1, medicalScrubsHatsVariants ));
		Loot.Insert(new ExpansionLoot( "MedicalScrubsPants_Blue", NULL, 0.24, -1, medicalScrubsPantsVariants ));
		Loot.Insert(new ExpansionLoot( "MedicalScrubsShirt_Blue", NULL, 0.24, -1, medicalScrubsShirtsVariants ));

		Loot.Insert(new ExpansionLoot( "NioshFaceMask", NULL , 0.08));

		Loot.Insert(new ExpansionLoot( "SurgicalGloves_Blue", NULL, 0.24, -1, surgicalGlovesVariants ));
	}

	static void Expansion_NA_Clothing_Civilian(out array < ref ExpansionLoot > Loot)
	{
		array < ref ExpansionLoot > Loot;

		TStringArray visor = { "DirtBikeHelmet_Visor" };
		Loot.Insert(new ExpansionLoot( "DirtBikeHelmet_Chernarus", visor, 0.3 ));
	}

	static void Expansion_NA_Clothing_Military(out array < ref ExpansionLoot > Loot)
	{
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };

		Loot.Insert(new ExpansionLoot( "UKAssVest_Black", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "UKAssVest_Camo", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "UKAssVest_Khaki", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "UKAssVest_Olive", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "HighCapacityVest_Black", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "HighCapacityVest_Olive", NULL , 0.1));

		Loot.Insert(new ExpansionLoot( "PlateCarrierVest", NULL , 0.08));
		Loot.Insert(new ExpansionLoot( "PlateCarrierHolster", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "PlateCarrierPouches", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "PlateCarrierVest", vest , 0.05));

		Loot.Insert(new ExpansionLoot( "GhillieAtt_Mossy", NULL , 0.2));
		Loot.Insert(new ExpansionLoot( "GhillieHood_Mossy", NULL , 0.1));
		Loot.Insert(new ExpansionLoot( "GhillieBushrag_Mossy", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "GhillieSuit_Mossy", NULL , 0.05));
		Loot.Insert(new ExpansionLoot( "BallisticHelmet_UN", NULL, 0.08 ));
	}

	static array < ref ExpansionLoot > SupplyCrate_Regular()
	{
		array < ref ExpansionLoot > Loot = new array < ref ExpansionLoot >;

		TStringArray battery = { "Battery9V" };

		Loot.Insert(new ExpansionLoot( "SewingKit", NULL, 0.25 ));
		Loot.Insert(new ExpansionLoot( "LeatherSewingKit", NULL, 0.25 ));
		Loot.Insert(new ExpansionLoot( "WeaponCleaningKit", NULL, 0.05 ));
		Loot.Insert(new ExpansionLoot( "Lockpick", NULL, 0.05 ));
		Loot.Insert(new ExpansionLoot( "Binoculars", NULL, 0.3 ));
		Loot.Insert(new ExpansionLoot( "Rangefinder", battery, 0.05 ));
		Loot.Insert(new ExpansionLoot( "WeaponCleaningKit", NULL, 0.05 ));
	#ifdef NAMALSK_SURVIVAL
		Loot.Insert(new ExpansionLoot( "dzn_map_namalsk", NULL, 0.3 ));
	#else
		Loot.Insert(new ExpansionLoot( "ChernarusMap", NULL, 0.3 ));
	#endif
	#ifdef EXPANSIONMODNAVIGATION
		Loot.Insert(new ExpansionLoot( "ExpansionGPS", NULL, 0.05 ));
	#endif

		Expansion_NA_Weapons_Civilian(Loot);
		Expansion_NA_Ammo_Civilian(Loot);
		Expansion_NA_Gear_Medical(Loot);
		Expansion_NA_Clothing_Civilian(Loot);
		Expansion_NA_Clothing_Military(Loot);
		Expansion_NA_Food(Loot);

		return Loot;
	}

	static array <ref ExpansionLoot> SupplyCrate_Medical()
	{
		array < ref ExpansionLoot > Loot = new array < ref ExpansionLoot >;

		Loot.Insert(new ExpansionLoot( "Bear_Pink", NULL , 0.1));

		Expansion_NA_Gear_Medical(Loot);
		Expansion_NA_Clothing_Medical(Loot);

		return Loot;
	}

	static array <ref ExpansionLoot> SupplyCrate_Military()
	{
		array <ref ExpansionLoot> Loot = new array < ref ExpansionLoot >;

		TStringArray battery = { "Battery9V" };

		Loot.Insert(new ExpansionLoot( "Binoculars", NULL, 0.3 ));
		Loot.Insert(new ExpansionLoot( "Rangefinder", battery, 0.05 ));
		Loot.Insert(new ExpansionLoot( "GhillieAtt_Mossy", NULL, 0.05 ));
		Loot.Insert(new ExpansionLoot( "WeaponCleaningKit", NULL, 0.05 ));

	#ifdef NAMALSK_SURVIVAL
		Loot.Insert(new ExpansionLoot( "dzn_map_namalsk", NULL, 0.3 ));
	#else
		Loot.Insert(new ExpansionLoot( "ChernarusMap", NULL, 0.3 ));
	#endif

	#ifdef EXPANSIONMODNAVIGATION
		Loot.Insert(new ExpansionLoot( "ExpansionGPS", NULL, 0.05 ));
	#endif

		Expansion_NA_Weapons_Military(Loot);
		Expansion_NA_Ammo_Military(Loot);
		Expansion_NA_Attachments(Loot);
		Expansion_NA_Clothing_Military(Loot);
		Expansion_NA_Food(Loot);

		return Loot;
	}
};
