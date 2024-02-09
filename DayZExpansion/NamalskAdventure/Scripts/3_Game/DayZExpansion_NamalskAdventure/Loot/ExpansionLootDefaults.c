modded class ExpansionLootDefaults
{
	static void Expansion_NA_Weapons_Civilian(out array < ref ExpansionLoot > Loot)
	{
		//! General Variants
		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));

		array<ref ExpansionLootVariant> att_UniversalLight = {};
			att_UniversalLight.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> att_TLRLight = {};
			att_TLRLight.Insert(new ExpansionLootVariant("TLRLight"));
		array<ref ExpansionLootVariant> att_PistolSuppressor = {};
			att_PistolSuppressor.Insert(new ExpansionLootVariant("PistolSuppressor"));
		array<ref ExpansionLootVariant> att_TLRLightAndPistolSuppressor = {};
			att_TLRLightAndPistolSuppressor.Insert(new ExpansionLootVariant("TLRLight"));
			att_TLRLightAndPistolSuppressor.Insert(new ExpansionLootVariant("PistolSuppressor"));

		array<ref ExpansionLootVariant> att_PUScopeOptic = {};
			att_PUScopeOptic.Insert(new ExpansionLootVariant("PUScopeOptic"));
		array<ref ExpansionLootVariant> att_HuntingOptic = {};
			att_HuntingOptic.Insert(new ExpansionLootVariant("HuntingOptic"));

		array<ref ExpansionLootVariant> att_PSO1Optic = {};
			att_PSO1Optic.Insert(new ExpansionLootVariant("PSO1Optic"));
		array<ref ExpansionLootVariant> att_PSO11Optic = {};
			att_PSO11Optic.Insert(new ExpansionLootVariant("PSO11Optic"));
		array<ref ExpansionLootVariant> att_KazuarOptic = {};
			att_KazuarOptic.Insert(new ExpansionLootVariant("KazuarOptic"));
		array<ref ExpansionLootVariant> att_KobraOptic = {};
			att_KobraOptic.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> att_KashtanOptic = {};
			att_KashtanOptic.Insert(new ExpansionLootVariant("KashtanOptic"));

		array<ref ExpansionLootVariant> att_ReflexOptic = {};
			att_ReflexOptic.Insert(new ExpansionLootVariant("ReflexOptic"));
		array<ref ExpansionLootVariant> att_ACOGOptic = {};
			att_ACOGOptic.Insert(new ExpansionLootVariant("ACOGOptic"));
		array<ref ExpansionLootVariant> att_ACOGOptic_6x = {};
			att_ACOGOptic_6x.Insert(new ExpansionLootVariant("ACOGOptic_6x"));
		array<ref ExpansionLootVariant> att_M68Optic = {};
			att_M68Optic.Insert(new ExpansionLootVariant("M68Optic"));
		array<ref ExpansionLootVariant> att_M4_T3NRDSOptic = {};
			att_M4_T3NRDSOptic.Insert(new ExpansionLootVariant("M4_T3NRDSOptic"));
		array<ref ExpansionLootVariant> att_StarlightOptic = {};
			att_StarlightOptic.Insert(new ExpansionLootVariant("StarlightOptic"));
		#ifdef EXPANSIONMODWEAPONS
		array<ref ExpansionLootVariant> att_ExpansionReflexMRSOptic = {};
			att_ExpansionReflexMRSOptic.Insert(new ExpansionLootVariant("ExpansionReflexMRSOptic"));
		array<ref ExpansionLootVariant> att_ExpansionEXPS3HoloOptic = {};
			att_ExpansionEXPS3HoloOptic.Insert(new ExpansionLootVariant("ExpansionEXPS3HoloOptic"));
		array<ref ExpansionLootVariant> att_ExpansionHAMROptic = {};
			att_ExpansionHAMROptic.Insert(new ExpansionLootVariant("ExpansionHAMROptic"));
		#endif

		array<ref ExpansionLootVariant> att_AKPlastic = {};
			att_AKPlastic.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			att_AKPlastic.Insert(new ExpansionLootVariant("AK_PlasticHndgrd"));
		array<ref ExpansionLootVariant> att_AK74 = {};
			att_AK74.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
			att_AK74.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
		array<ref ExpansionLootVariant> att_AKM = {};
			att_AKM.Insert(new ExpansionLootVariant("AK_WoodBttstck"));
			att_AKM.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
		//! End of General Variants

		array<ref ExpansionLootVariant> ak_1 = {};
			ak_1.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_1.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
		array<ref ExpansionLootVariant> ak_2 = {};
			ak_2.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_2.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
			ak_2.Insert(new ExpansionLootVariant("AK_Bayonet"));
		array<ref ExpansionLootVariant> ak_3 = {};
			ak_3.Insert(new ExpansionLootVariant("KobraOptic"));
			ak_3.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
			ak_3.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
		array<ref ExpansionLootVariant> ak_4 = {};
			ak_4.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_4.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
		array<ref ExpansionLootVariant> ak_5 = {};
			ak_5.Insert(new ExpansionLootVariant("AK_WoodBttstck"));
			ak_5.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
			ak_5.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> ak_6 = {};
			ak_6.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			ak_6.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
		array<ref ExpansionLootVariant> ak_7 = {};
			ak_7.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			ak_7.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
			ak_7.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> ak74u_1 = {};
			ak74u_1.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
		array<ref ExpansionLootVariant> ak74u_2 = {};
			ak74u_2.Insert(new ExpansionLootVariant("AKS74U_Bttstck"));

		array<ref ExpansionLootVariant> expansionm16_1 = {};
			expansionm16_1.Insert(new ExpansionLootVariant("ExpansionEXPS3HoloOptic"));
			expansionm16_1.Insert(new ExpansionLootVariant("UniversalLight"));
		array<ref ExpansionLootVariant> expansionm16_2 = {};
			expansionm16_2.Insert(new ExpansionLootVariant("M68Optic"));
			expansionm16_2.Insert(new ExpansionLootVariant("UniversalLight"));
		array<ref ExpansionLootVariant> expansionm16_3 = {};
			expansionm16_3.Insert(new ExpansionLootVariant("ReflexOptic"));
			expansionm16_3.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> m4a1_1 = {};
			m4a1_1.Insert(new ExpansionLootVariant("M4_CarryHandleOptic"));
			m4a1_1.Insert(new ExpansionLootVariant("M4_OEBttstck"));
			m4a1_1.Insert(new ExpansionLootVariant("M4_PlasticHndgrd"));
		array<ref ExpansionLootVariant> m4a1_2 = {};
			m4a1_2.Insert(new ExpansionLootVariant("BUISOptic"));
			m4a1_2.Insert(new ExpansionLootVariant("M4_CQBBttstck"));
			m4a1_2.Insert(new ExpansionLootVariant("M4_RisHndgrd"));
		array<ref ExpansionLootVariant> m4a1_3 = {};
			m4a1_3.Insert(new ExpansionLootVariant("ACOGOptic"));
			m4a1_3.Insert(new ExpansionLootVariant("M4_MPBttstck"));
			m4a1_3.Insert(new ExpansionLootVariant("M4_MPHndgrd"));
		array<ref ExpansionLootVariant> m4a1_4 = {};
			m4a1_4.Insert(new ExpansionLootVariant("BUISOptic"));
			m4a1_4.Insert(new ExpansionLootVariant("M4_CQBBttstck"));
			m4a1_4.Insert(new ExpansionLootVariant("M4_RisHndgrd"));
			m4a1_4.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> fal_1 = {};
			fal_1.Insert(new ExpansionLootVariant("Fal_OeBttstck"));
		array<ref ExpansionLootVariant> fal_2 = {};
			fal_2.Insert(new ExpansionLootVariant("Fal_FoldingBttstck"));
		array<ref ExpansionLootVariant> fal_3 = {};
			fal_3.Insert(new ExpansionLootVariant("Fal_OeBttstck"));
			fal_3.Insert(new ExpansionLootVariant("ACOGOptic"));
		array<ref ExpansionLootVariant> fal_4 = {};
			fal_4.Insert(new ExpansionLootVariant("Fal_FoldingBttstck"));
			fal_4.Insert(new ExpansionLootVariant("ReflexOptic"));

		array<ref ExpansionLootVariant> saiga_1 = {};
			saiga_1.Insert(new ExpansionLootVariant("Saiga_Bttstck"));
			saiga_1.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> saiga_2 = {};
			saiga_2.Insert(new ExpansionLootVariant("Saiga_Bttstck"));

		array<ref ExpansionLootVariant> sks_1 = {};
			sks_1.Insert(new ExpansionLootVariant("SKS_Bayonet"));

		array<ref ExpansionLootVariant> mosin_1 = {};
			mosin_1.Insert(new ExpansionLootVariant("Mosin_Bayonet"));
		array<ref ExpansionLootVariant> mosin_2 = {};
			mosin_2.Insert(new ExpansionLootVariant("Mosin_Compensator"));
		array<ref ExpansionLootVariant> mosin_3 = {};
			mosin_3.Insert(new ExpansionLootVariant("Mosin_Compensator"));
			mosin_3.Insert(new ExpansionLootVariant("PUScopeOptic"));

		array<ref ExpansionLootVariant> winchester70_1 = {};
			winchester70_1.Insert(new ExpansionLootVariant("HuntingOptic"));

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
		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));

		array<ref ExpansionLootVariant> att_UniversalLight = {};
			att_UniversalLight.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> att_TLRLight = {};
			att_TLRLight.Insert(new ExpansionLootVariant("TLRLight"));
		array<ref ExpansionLootVariant> att_PistolSuppressor = {};
			att_PistolSuppressor.Insert(new ExpansionLootVariant("PistolSuppressor"));
		array<ref ExpansionLootVariant> att_TLRLightAndPistolSuppressor = {};
			att_TLRLightAndPistolSuppressor.Insert(new ExpansionLootVariant("TLRLight"));
			att_TLRLightAndPistolSuppressor.Insert(new ExpansionLootVariant("PistolSuppressor"));

		array<ref ExpansionLootVariant> att_PUScopeOptic = {};
			att_PUScopeOptic.Insert(new ExpansionLootVariant("PUScopeOptic"));
		array<ref ExpansionLootVariant> att_HuntingOptic = {};
			att_HuntingOptic.Insert(new ExpansionLootVariant("HuntingOptic"));

		array<ref ExpansionLootVariant> att_PSO1Optic = {};
			att_PSO1Optic.Insert(new ExpansionLootVariant("PSO1Optic"));
		array<ref ExpansionLootVariant> att_PSO11Optic = {};
			att_PSO11Optic.Insert(new ExpansionLootVariant("PSO11Optic"));
		array<ref ExpansionLootVariant> att_KazuarOptic = {};
			att_KazuarOptic.Insert(new ExpansionLootVariant("KazuarOptic"));
		array<ref ExpansionLootVariant> att_KobraOptic = {};
			att_KobraOptic.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> att_KashtanOptic = {};
			att_KashtanOptic.Insert(new ExpansionLootVariant("KashtanOptic"));

		array<ref ExpansionLootVariant> att_ReflexOptic = {};
			att_ReflexOptic.Insert(new ExpansionLootVariant("ReflexOptic"));
		array<ref ExpansionLootVariant> att_ACOGOptic = {};
			att_ACOGOptic.Insert(new ExpansionLootVariant("ACOGOptic"));
		array<ref ExpansionLootVariant> att_ACOGOptic_6x = {};
			att_ACOGOptic_6x.Insert(new ExpansionLootVariant("ACOGOptic_6x"));
		array<ref ExpansionLootVariant> att_M68Optic = {};
			att_M68Optic.Insert(new ExpansionLootVariant("M68Optic"));
		array<ref ExpansionLootVariant> att_M4_T3NRDSOptic = {};
			att_M4_T3NRDSOptic.Insert(new ExpansionLootVariant("M4_T3NRDSOptic"));
		array<ref ExpansionLootVariant> att_StarlightOptic = {};
			att_StarlightOptic.Insert(new ExpansionLootVariant("StarlightOptic"));
	#ifdef EXPANSIONMODWEAPONS
		array<ref ExpansionLootVariant> att_ExpansionReflexMRSOptic = {};
			att_ExpansionReflexMRSOptic.Insert(new ExpansionLootVariant("ExpansionReflexMRSOptic"));
		array<ref ExpansionLootVariant> att_ExpansionEXPS3HoloOptic = {};
			att_ExpansionEXPS3HoloOptic.Insert(new ExpansionLootVariant("ExpansionEXPS3HoloOptic"));
		array<ref ExpansionLootVariant> att_ExpansionHAMROptic = {};
			att_ExpansionHAMROptic.Insert(new ExpansionLootVariant("ExpansionHAMROptic"));
	#endif

		array<ref ExpansionLootVariant> att_AKPlastic = {};
			att_AKPlastic.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			att_AKPlastic.Insert(new ExpansionLootVariant("AK_PlasticHndgrd"));
		array<ref ExpansionLootVariant> att_AK74 = {};
			att_AK74.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
			att_AK74.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
		array<ref ExpansionLootVariant> att_AKM = {};
			att_AKM.Insert(new ExpansionLootVariant("AK_WoodBttstck"));
			att_AKM.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
		//! End of General Variants

		array<ref ExpansionLootVariant> ak_1 = {};
			ak_1.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_1.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
		array<ref ExpansionLootVariant> ak_2 = {};
			ak_2.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_2.Insert(new ExpansionLootVariant("AK74_Hndgrd"));
			ak_2.Insert(new ExpansionLootVariant("AK_Bayonet"));
		array<ref ExpansionLootVariant> ak_3 = {};
			ak_3.Insert(new ExpansionLootVariant("KobraOptic"));
			ak_3.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
			ak_3.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
		array<ref ExpansionLootVariant> ak_4 = {};
			ak_4.Insert(new ExpansionLootVariant("AK_FoldingBttstck"));
			ak_4.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
		array<ref ExpansionLootVariant> ak_5 = {};
			ak_5.Insert(new ExpansionLootVariant("AK_WoodBttstck"));
			ak_5.Insert(new ExpansionLootVariant("AK_WoodHndgrd"));
			ak_5.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> ak_6 = {};
			ak_6.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			ak_6.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
		array<ref ExpansionLootVariant> ak_7 = {};
			ak_7.Insert(new ExpansionLootVariant("AK_PlasticBttstck"));
			ak_7.Insert(new ExpansionLootVariant("AK_RailHndgrd"));
			ak_7.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> ak74u_1 = {};
			ak74u_1.Insert(new ExpansionLootVariant("AK74_WoodBttstck"));
		array<ref ExpansionLootVariant> ak74u_2 = {};
			ak74u_2.Insert(new ExpansionLootVariant("AKS74U_Bttstck"));

		array<ref ExpansionLootVariant> expansionm16_1 = {};
			expansionm16_1.Insert(new ExpansionLootVariant("ExpansionEXPS3HoloOptic"));
			expansionm16_1.Insert(new ExpansionLootVariant("UniversalLight"));
		array<ref ExpansionLootVariant> expansionm16_2 = {};
			expansionm16_2.Insert(new ExpansionLootVariant("M68Optic"));
			expansionm16_2.Insert(new ExpansionLootVariant("UniversalLight"));
		array<ref ExpansionLootVariant> expansionm16_3 = {};
			expansionm16_3.Insert(new ExpansionLootVariant("ReflexOptic"));
			expansionm16_3.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> m4a1_1 = {};
			m4a1_1.Insert(new ExpansionLootVariant("M4_CarryHandleOptic"));
			m4a1_1.Insert(new ExpansionLootVariant("M4_OEBttstck"));
			m4a1_1.Insert(new ExpansionLootVariant("M4_PlasticHndgrd"));
		array<ref ExpansionLootVariant> m4a1_2 = {};
			m4a1_2.Insert(new ExpansionLootVariant("BUISOptic"));
			m4a1_2.Insert(new ExpansionLootVariant("M4_CQBBttstck"));
			m4a1_2.Insert(new ExpansionLootVariant("M4_RisHndgrd"));
		array<ref ExpansionLootVariant> m4a1_3 = {};
			m4a1_3.Insert(new ExpansionLootVariant("ACOGOptic"));
			m4a1_3.Insert(new ExpansionLootVariant("M4_MPBttstck"));
			m4a1_3.Insert(new ExpansionLootVariant("M4_MPHndgrd"));
		array<ref ExpansionLootVariant> m4a1_4 = {};
			m4a1_4.Insert(new ExpansionLootVariant("BUISOptic"));
			m4a1_4.Insert(new ExpansionLootVariant("M4_CQBBttstck"));
			m4a1_4.Insert(new ExpansionLootVariant("M4_RisHndgrd"));
			m4a1_4.Insert(new ExpansionLootVariant("UniversalLight"));

		array<ref ExpansionLootVariant> fal_1 = {};
			fal_1.Insert(new ExpansionLootVariant("Fal_OeBttstck"));
		array<ref ExpansionLootVariant> fal_2 = {};
			fal_2.Insert(new ExpansionLootVariant("Fal_FoldingBttstck"));
		array<ref ExpansionLootVariant> fal_3 = {};
			fal_3.Insert(new ExpansionLootVariant("Fal_OeBttstck"));
			fal_3.Insert(new ExpansionLootVariant("ACOGOptic"));
		array<ref ExpansionLootVariant> fal_4 = {};
			fal_4.Insert(new ExpansionLootVariant("Fal_FoldingBttstck"));
			fal_4.Insert(new ExpansionLootVariant("ReflexOptic"));

		array<ref ExpansionLootVariant> saiga_1 = {};
			saiga_1.Insert(new ExpansionLootVariant("Saiga_Bttstck"));
			saiga_1.Insert(new ExpansionLootVariant("KobraOptic"));
		array<ref ExpansionLootVariant> saiga_2 = {};
			saiga_2.Insert(new ExpansionLootVariant("Saiga_Bttstck"));

		array<ref ExpansionLootVariant> sks_1 = {};
			sks_1.Insert(new ExpansionLootVariant("SKS_Bayonet"));

		array<ref ExpansionLootVariant> mosin_1 = {};
			mosin_1.Insert(new ExpansionLootVariant("Mosin_Bayonet"));
		array<ref ExpansionLootVariant> mosin_2 = {};
			mosin_2.Insert(new ExpansionLootVariant("Mosin_Compensator"));
		array<ref ExpansionLootVariant> mosin_3 = {};
			mosin_3.Insert(new ExpansionLootVariant("Mosin_Compensator"));
			mosin_3.Insert(new ExpansionLootVariant("PUScopeOptic"));

		array<ref ExpansionLootVariant> winchester70_1 = {};
			winchester70_1.Insert(new ExpansionLootVariant("HuntingOptic"));

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
		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));

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
		array<ref ExpansionLootVariant> att_gasmask = {};
			att_gasmask.Insert(new ExpansionLootVariant("GasMask_Filter"));

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
		array<ref ExpansionLootVariant> firstaidkit_1 = {};
			firstaidkit_1.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_1.Insert(new ExpansionLootVariant("BandageDressing"));
		array<ref ExpansionLootVariant> firstaidkit_2 = {};
			firstaidkit_2.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_2.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_2.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_2.Insert(new ExpansionLootVariant("BandageDressing"));
		array<ref ExpansionLootVariant> firstaidkit_3 = {};
			firstaidkit_3.Insert(new ExpansionLootVariant("Morphine"));
			firstaidkit_3.Insert(new ExpansionLootVariant("BandageDressing"));
		array<ref ExpansionLootVariant> firstaidkit_4 = {};
			firstaidkit_4.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_4.Insert(new ExpansionLootVariant("SalineBagIV"));
		array<ref ExpansionLootVariant> firstaidkit_5 = {};
			firstaidkit_5.Insert(new ExpansionLootVariant("PainkillerTablets"));
			firstaidkit_5.Insert(new ExpansionLootVariant("DisinfectantAlcohol"));
		array<ref ExpansionLootVariant> firstaidkit_6 = {};
			firstaidkit_6.Insert(new ExpansionLootVariant("CharcoalTablets"));
			firstaidkit_6.Insert(new ExpansionLootVariant("Morphine"));
		array<ref ExpansionLootVariant> firstaidkit_7 = {};
			firstaidkit_7.Insert(new ExpansionLootVariant("Epinephrine"));
			firstaidkit_7.Insert(new ExpansionLootVariant("StartKitIV"));
		array<ref ExpansionLootVariant> firstaidkit_8 = {};
			firstaidkit_8.Insert(new ExpansionLootVariant("TetracyclineAntiBiotics"));
			firstaidkit_8.Insert(new ExpansionLootVariant("VitaminBottle"));
		array<ref ExpansionLootVariant> firstaidkit_9 = {};
			firstaidkit_9.Insert(new ExpansionLootVariant("Morphine"));
			firstaidkit_9.Insert(new ExpansionLootVariant("SalineBagIV"));
		array<ref ExpansionLootVariant> firstaidkit_10 = {};
			firstaidkit_10.Insert(new ExpansionLootVariant("PainkillerTablets"));
			firstaidkit_10.Insert(new ExpansionLootVariant("Epinephrine"));
		array<ref ExpansionLootVariant> firstaidkit_11 = {};
			firstaidkit_11.Insert(new ExpansionLootVariant("TetracyclineAntiBiotics"));
			firstaidkit_11.Insert(new ExpansionLootVariant("Morphine"));
		array<ref ExpansionLootVariant> firstaidkit_12 = {};
			firstaidkit_12.Insert(new ExpansionLootVariant("VitaminBottle"));
			firstaidkit_12.Insert(new ExpansionLootVariant("SurgicalGloves_Blue"));
		array<ref ExpansionLootVariant> firstaidkit_13 = {};
			firstaidkit_13.Insert(new ExpansionLootVariant("BandageDressing"));
			firstaidkit_13.Insert(new ExpansionLootVariant("VitaminBottle"));
		array<ref ExpansionLootVariant> firstaidkit_14 = {};
			firstaidkit_14.Insert(new ExpansionLootVariant("CharcoalTablets"));
			firstaidkit_14.Insert(new ExpansionLootVariant("DisinfectantAlcohol"));
		array<ref ExpansionLootVariant> firstaidkit_15 = {};
			firstaidkit_15.Insert(new ExpansionLootVariant("IodineTincture"));
			firstaidkit_15.Insert(new ExpansionLootVariant("PainkillerTablets"));
		array<ref ExpansionLootVariant> firstaidkit_16 = {};
			firstaidkit_16.Insert(new ExpansionLootVariant("CharcoalTablets"));
			firstaidkit_16.Insert(new ExpansionLootVariant("AntiChemInjector"));
			firstaidkit_16.Insert(new ExpansionLootVariant("BandageDressing"));
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

		array<ref ExpansionLootVariant> visor = {};
			visor.Insert(new ExpansionLootVariant("DirtBikeHelmet_Visor"));
		Loot.Insert(new ExpansionLoot( "DirtBikeHelmet_Chernarus", visor, 0.3 ));
	}

	static void Expansion_NA_Clothing_Military(out array < ref ExpansionLoot > Loot)
	{
		array<ref ExpansionLootVariant> vest = {};
			vest.Insert(new ExpansionLootVariant("PlateCarrierHolster"));
			vest.Insert(new ExpansionLootVariant("PlateCarrierPouches"));

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

		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));

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

		array<ref ExpansionLootVariant> battery = {};
			battery.Insert(new ExpansionLootVariant("Battery9V"));

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
