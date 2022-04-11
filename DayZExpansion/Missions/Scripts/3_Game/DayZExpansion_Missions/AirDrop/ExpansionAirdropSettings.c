/**
 * ExpansionAirdropSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAirdropSettings
 * @brief		Airdrop settings class
 **/
class ExpansionAirdropSettings: ExpansionSettingBase
{
	static const int VERSION = 2;
	
	bool ServerMarkerOnDropLocation;
	bool Server3DMarkerOnDropLocation;
	bool ShowAirdropTypeOnMarker;
	bool HeightIsRelativeToGroundLevel;

	float Height;
	float FollowTerrainFraction;
	float Speed;
	float Radius;
	float InfectedSpawnRadius;

	int InfectedSpawnInterval;  //! ms

	//! Only kept for backwards compatibility, should be set per container
	int ItemCount;

	ref array < ref ExpansionAirdropLootContainer > Containers;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionAirdropSettings()
	{
		Containers =  new array< ref ExpansionAirdropLootContainer >;
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		//Not sent to client under normal operation
		m_IsLoaded = true;

		ExpansionAirdropSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionAirdropSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Airdrop.Invoke();
		
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionAirdropSettings thisSetting = this;

		ctx.Write( thisSetting );
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		//Not sent to client
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionAirdropSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionAirdropSettings s )
	{
		ServerMarkerOnDropLocation = s.ServerMarkerOnDropLocation;
		Server3DMarkerOnDropLocation = s.Server3DMarkerOnDropLocation;
		ShowAirdropTypeOnMarker = s.ShowAirdropTypeOnMarker;
		HeightIsRelativeToGroundLevel = s.HeightIsRelativeToGroundLevel;
		Height = s.Height;
		FollowTerrainFraction = s.FollowTerrainFraction;
		Speed = s.Speed;
		Radius = s.Radius;
		InfectedSpawnRadius = s.InfectedSpawnRadius;
		InfectedSpawnInterval = s.InfectedSpawnInterval;
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;
		
		bool save;

		bool airdropSettingsExist = FileExist(EXPANSION_AIRDROP_SETTINGS);
		
		if (airdropSettingsExist)
		{
			EXPrint("[ExpansionAirdropSettings] Load existing setting file:" + EXPANSION_AIRDROP_SETTINGS);

			JsonFileLoader<ExpansionAirdropSettings>.JsonLoadFile( EXPANSION_AIRDROP_SETTINGS, this );

			if (m_Version < VERSION)
			{
				if (m_Version < 2)
				{
					foreach (ExpansionAirdropLootContainer containerV1OrLower : Containers)
					{
						foreach (ExpansionAirdropLoot lootV1OrLower: containerV1OrLower.Loot)
						{
							if (!lootV1OrLower.QuantityPercent)
								lootV1OrLower.QuantityPercent = -1;

							if (!lootV1OrLower.Max)
								lootV1OrLower.Max = -1;
						}
					}
				}

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionAirdropSettings] No existing setting file:" + EXPANSION_AIRDROP_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();

		return airdropSettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionAirdropSettings] Saving settings");

		JsonFileLoader<ExpansionAirdropSettings>.JsonSaveFile( EXPANSION_AIRDROP_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Airdrop.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		ServerMarkerOnDropLocation = true;
		Server3DMarkerOnDropLocation = true;
		ShowAirdropTypeOnMarker = true;
		HeightIsRelativeToGroundLevel = true;

		Height = 450;
		FollowTerrainFraction = 0.5;
		Speed = 1500;
		Radius = 1;
		InfectedSpawnRadius = 50;
		InfectedSpawnInterval = 250;

		ItemCount = 50;

		DefaultRegular();
		DefaultMedical();
		DefaultBaseBuilding();
		DefaultMilitary();
	}

	void DefaultRegular()
	{
		array < ref ExpansionAirdropLoot > Loot;
		TStringArray Infected;
		
		TStringArray ak74u_1 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_WoodBttstck", "AK_WoodHndgrd" };

		TStringArray sks_1 = { "PUScopeOptic" };
		TStringArray sks_2 = { "SKS_Bayonet" };

		array< ref ExpansionAirdropLootVariant > sksVariants = {
			new ExpansionAirdropLootVariant( "SKS", sks_2, 0.2 ),
			new ExpansionAirdropLootVariant( "SKS", NULL, 0.6 ),
		};

		TStringArray ump_1 = { "UniversalLight" };

		array< ref ExpansionAirdropLootVariant > umpVariants = {
			new ExpansionAirdropLootVariant( "UMP45", NULL, 0.6 ),
		};

		TStringArray mosin_1 = { "PUScopeOptic" };

		array< ref ExpansionAirdropLootVariant > mosinVariants = {
			new ExpansionAirdropLootVariant( "Mosin9130", NULL, 0.75 ),
		};

		TStringArray b95_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > b95Variants = {
			new ExpansionAirdropLootVariant( "B95", NULL, 0.666667 ),
		};

		TStringArray cz527_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > cz527Variants = {
			new ExpansionAirdropLootVariant( "CZ527", NULL, 0.4 ),
		};

		TStringArray cz75_1 = { "TLRLight" };

		array< ref ExpansionAirdropLootVariant > cz75Variants = {
			new ExpansionAirdropLootVariant( "CZ75", NULL, 0.5 ),
		};

		TStringArray fnx1_1 = { "TLRLight" };

		array< ref ExpansionAirdropLootVariant > fnxVariants = {
			new ExpansionAirdropLootVariant( "FNX45", NULL, 0.75 ),
		};
		
		TStringArray kedr_1 = { "PistolSuppressor" };  

		array< ref ExpansionAirdropLootVariant > kedrVariants = {
			new ExpansionAirdropLootVariant( "Expansion_Kedr", NULL, 0.6 ),
		};

		TStringArray winchester70_1 = { "HuntingOptic" };

		array< ref ExpansionAirdropLootVariant > winchester70Variants = {
			new ExpansionAirdropLootVariant( "Winchester70", NULL, 0.8 ),
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

		array< ref ExpansionAirdropLootVariant > firstaidkitVariants = {
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_2, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_3, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_4, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_5, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_6, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_7, 0.131579 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_8, 0.131579 ),
		};

		Loot = {
			new ExpansionAirdropLoot( "AKS74U", ak74u_1, 0.08 ),
			
			new ExpansionAirdropLoot( "AKM", akm_1, 0.05 ),

			new ExpansionAirdropLoot( "SKS", sks_1, 0.5, -1, sksVariants ),

			new ExpansionAirdropLoot( "UMP45", ump_1, 0.25, -1, umpVariants ),
 
			new ExpansionAirdropLoot( "Mosin9130", mosin_1, 0.4, -1, mosinVariants ),
 
			new ExpansionAirdropLoot( "B95", b95_1, 0.3, -1, b95Variants ),

			new ExpansionAirdropLoot( "CZ527", cz527_1, 0.5, -1, cz527Variants ),

			new ExpansionAirdropLoot( "CZ75", cz75_1, 0.2, -1, cz75Variants ),

			new ExpansionAirdropLoot( "FNX45", fnx1_1, 0.4, -1, fnxVariants ),

			new ExpansionAirdropLoot( "Expansion_Kedr", kedr_1, 0.5, -1, kedrVariants ),

   			new ExpansionAirdropLoot( "Mp133Shotgun", NULL, 0.8 ),

			new ExpansionAirdropLoot( "Winchester70", winchester70_1, 0.5, -1, winchester70Variants ),
				
			new ExpansionAirdropLoot( "Expansion_DT11", NULL, 0.3 ),
			
			new ExpansionAirdropLoot( "Binoculars", NULL, 0.3 ),
			
			#ifdef NAMALSK_SURVIVAL
			new ExpansionAirdropLoot( "dzn_map_namalsk", NULL, 0.3 ),
			#else
			new ExpansionAirdropLoot( "ChernarusMap", NULL, 0.3 ),
			#endif
			new ExpansionAirdropLoot( "Rangefinder", battery, 0.05 ),
			new ExpansionAirdropLoot( "ExpansionGPS", NULL, 0.05 ),
			
			new ExpansionAirdropLoot( "BoxCerealCrunchin", NULL, 0.05 ),
			new ExpansionAirdropLoot( "PeachesCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "BakedBeansCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "SpaghettiCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "SardinesCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "TunaCan", NULL, 0.1 ),
			new ExpansionAirdropLoot( "WaterBottle", NULL, 0.5 ),
			
			new ExpansionAirdropLoot( "CanOpener", NULL, 0.5 ),
			new ExpansionAirdropLoot( "KitchenKnife", NULL, 0.3 ),
			
			new ExpansionAirdropLoot( "BallisticHelmet_UN", NULL, 0.08 ),
			new ExpansionAirdropLoot( "DirtBikeHelmet_Chernarus", visor, 0.3 ),
			
			new ExpansionAirdropLoot( "SewingKit", NULL, 0.25 ),
			new ExpansionAirdropLoot( "LeatherSewingKit", NULL, 0.25 ),
			new ExpansionAirdropLoot( "WeaponCleaningKit", NULL, 0.05 ),
			new ExpansionAirdropLoot( "Lockpick", NULL, 0.05 ),
			
			new ExpansionAirdropLoot( "GhillieAtt_Mossy", NULL, 0.05 ),

			new ExpansionAirdropLoot( "Mag_Expansion_Kedr_20Rnd", NULL, 0.8 ),
			new ExpansionAirdropLoot( "Mag_CZ527_5rnd", NULL, 0.9 ),
			new ExpansionAirdropLoot( "Mag_CZ75_15Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "Mag_FNX45_15Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "Mag_UMP_25Rnd", NULL, 0.5 ),

			new ExpansionAirdropLoot( "AmmoBox_9x39_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_9x19_25Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_762x39Tracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_762x39_20Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_45ACP_25Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_308WinTracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLoot( "AmmoBox_308Win_20Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaRubberSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLoot( "AmmoBox_12gaPellets_10Rnd", NULL, 1 ),
			
			new ExpansionAirdropLoot( "Ammo_9x39", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_762x39Tracer", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_762x39", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_45ACP", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_308WinTracer", NULL, 0.5 ),
			new ExpansionAirdropLoot( "Ammo_308Win", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaSlug", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaRubberSlug", NULL, 1 ),
			new ExpansionAirdropLoot( "Ammo_12gaPellets", NULL,  0.5 ),
			
			new ExpansionAirdropLoot( "FirstAidKit", firstaidkit_1, 0.38, -1, firstaidkitVariants ),
		};

		Infected = {
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Blue",
			"ZmbF_CitizenANormal_Brown",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HikerSkinny_Blue",
			"ZmbM_HikerSkinny_Green",
			"ZmbM_HikerSkinny_Yellow",
			"ZmbF_HikerSkinny_Blue",
			"ZmbF_HikerSkinny_Grey",
			"ZmbF_HikerSkinny_Green",
			"ZmbF_HikerSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotobikerFat_Beige",
			"ZmbM_MotobikerFat_Black",
			"ZmbM_MotobikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_greenchecks",
			"ZmbM_Jacket_grey",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
		};

		int itemCount = 30;
		int infectedCount = 25;

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainer( "ExpansionAirdropContainer", 0, 5, Loot, Infected, itemCount, infectedCount ) );	
	}

	void DefaultMedical()
	{
		array < ref ExpansionAirdropLoot > Loot;
		TStringArray Infected;
		
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

		array< ref ExpansionAirdropLootVariant > medicalScrubsHatsVariants = {
			new ExpansionAirdropLootVariant( "MedicalScrubsHat_White" ),
			new ExpansionAirdropLootVariant( "MedicalScrubsHat_Green" ),
		};

		array< ref ExpansionAirdropLootVariant > medicalScrubsPantsVariants = {
			new ExpansionAirdropLootVariant( "MedicalScrubsPants_White" ),
			new ExpansionAirdropLootVariant( "MedicalScrubsPants_Green" ),
		};

		array< ref ExpansionAirdropLootVariant > medicalScrubsShirtsVariants = {
			new ExpansionAirdropLootVariant( "MedicalScrubsShirt_White" ),
			new ExpansionAirdropLootVariant( "MedicalScrubsShirt_Green" ),
		};

		array< ref ExpansionAirdropLootVariant > surgicalGlovesVariants = {
			new ExpansionAirdropLootVariant( "SurgicalGloves_LightBlue" ),
			new ExpansionAirdropLootVariant( "SurgicalGloves_Green" ),
		};

		array< ref ExpansionAirdropLootVariant > firstaidkitVariants = {
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_2, 0.040984 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_3, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_4, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_5, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_6, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_7, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_8, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_9, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_10, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_11, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_12, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_13, 0.073770 ),
			new ExpansionAirdropLootVariant( "FirstAidKit", firstaidkit_14, 0.073770 ),
		};

		Loot = {
			new ExpansionAirdropLoot( "BandageDressing", NULL , 0.15),

			new ExpansionAirdropLoot( "Morphine", NULL , 0.1),
			new ExpansionAirdropLoot( "Epinephrine", NULL , 0.1),

			new ExpansionAirdropLoot( "TacticalBaconCan", NULL , 0.05),

			new ExpansionAirdropLoot( "PainkillerTablets", NULL , 0.1),
			new ExpansionAirdropLoot( "CharcoalTablets", NULL , 0.1),
			new ExpansionAirdropLoot( "TetracyclineAntiBiotics", NULL , 0.1),

			new ExpansionAirdropLoot( "DisinfectantSpray", NULL , 0.08),
			new ExpansionAirdropLoot( "DisinfectantAlcohol", NULL , 0.1),

			new ExpansionAirdropLoot( "VitaminBottle", NULL , 0.2),

			new ExpansionAirdropLoot( "StartKitIV", NULL , 0.3),
			new ExpansionAirdropLoot( "SalineBagIV", NULL , 0.1),
			new ExpansionAirdropLoot( "SalineBag", NULL , 0.1),

			new ExpansionAirdropLoot( "BloodBagEmpty", NULL , 0.15),
			new ExpansionAirdropLoot( "BloodBagIV", NULL , 0.1),
			new ExpansionAirdropLoot( "BloodTestKit", NULL , 0.2),
			new ExpansionAirdropLoot( "BloodTestKit", NULL , 0.2),

			new ExpansionAirdropLoot( "MedicalScrubsHat_Blue", NULL, 0.24, -1, medicalScrubsHatsVariants ),
			new ExpansionAirdropLoot( "MedicalScrubsPants_Blue", NULL, 0.24, -1, medicalScrubsPantsVariants ),
			new ExpansionAirdropLoot( "MedicalScrubsShirt_Blue", NULL, 0.24, -1, medicalScrubsShirtsVariants ),

			new ExpansionAirdropLoot( "NioshFaceMask", NULL , 0.08),

			new ExpansionAirdropLoot( "SurgicalGloves_Blue", NULL, 0.24, -1, surgicalGlovesVariants ),

			new ExpansionAirdropLoot( "FirstAidKit", firstaidkit_1, 2.44, -1, firstaidkitVariants ),
		};

		Infected = {
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Blue",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HikerSkinny_Blue",
			"ZmbM_HikerSkinny_Green",
			"ZmbM_HikerSkinny_Yellow",
			"ZmbF_HikerSkinny_Blue",
			"ZmbF_HikerSkinny_Grey",
			"ZmbF_HikerSkinny_Green",
			"ZmbF_HikerSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotobikerFat_Beige",
			"ZmbM_MotobikerFat_Black",
			"ZmbM_MotobikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_greenchecks",
			"ZmbM_Jacket_grey",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
			"ZmbF_DoctorSkinny",
			"ZmbF_NurseFat",
			"ZmbM_DoctorFat",
			"ZmbF_PatientOld",
			"ZmbM_PatientSkinny",
		};

		int itemCount = 25;
		int infectedCount = 15;

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainer( "ExpansionAirdropContainer_Medical", 0, 10, Loot, Infected, itemCount, infectedCount ) );
	}

	void DefaultBaseBuilding()
	{
		array < ref ExpansionAirdropLoot > Loot;
		TStringArray Infected;

		Loot = {
			new ExpansionAirdropLoot( "NailBox", NULL , 0.8),
			new ExpansionAirdropLoot( "DuctTape", NULL , 0.2),
			new ExpansionAirdropLoot( "MetalPlate", NULL , 0.25),
			new ExpansionAirdropLoot( "WoodenPlank", NULL , 0.5),
			new ExpansionAirdropLoot( "Hammer", NULL , 0.4),
			new ExpansionAirdropLoot( "Shovel", NULL , 0.1),
			new ExpansionAirdropLoot( "Pliers", NULL , 0.1),
			new ExpansionAirdropLoot( "WoodAxe", NULL , 0.15),
			new ExpansionAirdropLoot( "Crowbar", NULL , 0.3),
			new ExpansionAirdropLoot( "Hacksaw", NULL , 0.3),
			new ExpansionAirdropLoot( "Handsaw", NULL , 0.1),
			new ExpansionAirdropLoot( "SledgeHammer", NULL , 0.15),
			new ExpansionAirdropLoot( "Hatchet", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionCodeLock", NULL , 0.1),
			new ExpansionAirdropLoot( "BarbedWire", NULL , 0.15),
			new ExpansionAirdropLoot( "MetalWire", NULL , 0.2),

			new ExpansionAirdropLoot( "ExpansionHescoKit", NULL , 0.05),
			new ExpansionAirdropLoot( "ExpansionBarbedWireKit", NULL , 0.1),
			new ExpansionAirdropLoot( "ExpansionCamoBoxKit", NULL , 0.18),
			new ExpansionAirdropLoot( "ExpansionGunrack", NULL , 0.05),

			new ExpansionAirdropLoot( "ExpansionCone", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionSignDanger", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionSignDanger2", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionSignDanger3", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionSignRoadBarrier", NULL , 0.15),

			new ExpansionAirdropLoot( "ExpansionSafeMini", NULL , 0.2),
			new ExpansionAirdropLoot( "ExpansionSafeMedium", NULL , 0.15),
			new ExpansionAirdropLoot( "ExpansionSafeLarge", NULL , 0.1),

			new ExpansionAirdropLoot( "ExpansionCamoTentKit", NULL , 0.25),
			new ExpansionAirdropLoot( "MediumTent", NULL , 0.2),
			new ExpansionAirdropLoot( "LargeTent", NULL , 0.1),
			new ExpansionAirdropLoot( "CarTent", NULL , 0.15),
			new ExpansionAirdropLoot( "CamoNet", NULL , 0.3),
		};

		Infected = {
			"ZmbM_HermitSkinny_Beige",
			"ZmbM_HermitSkinny_Black",
			"ZmbM_HermitSkinny_Green",
			"ZmbM_HermitSkinny_Red",
			"ZmbM_FarmerFat_Beige",
			"ZmbM_FarmerFat_Blue",
			"ZmbM_FarmerFat_Brown",
			"ZmbM_FarmerFat_Green",
			"ZmbF_CitizenANormal_Beige",
			"ZmbF_CitizenANormal_Brown",
			"ZmbF_CitizenANormal_Blue",
			"ZmbM_CitizenBFat_Blue",
			"ZmbM_CitizenBFat_Red",
			"ZmbM_CitizenBFat_Green",
			"ZmbF_CitizenBSkinny",
			"ZmbM_FishermanOld_Blue",
			"ZmbM_FishermanOld_Green",
			"ZmbM_FishermanOld_Grey",
			"ZmbM_FishermanOld_Red",
			"ZmbM_JournalistSkinny",
			"ZmbF_JournalistNormal_Blue",
			"ZmbF_JournalistNormal_Green",
			"ZmbF_JournalistNormal_Red",
			"ZmbF_JournalistNormal_White",
			"ZmbM_HikerSkinny_Blue",
			"ZmbM_HikerSkinny_Green",
			"ZmbM_HikerSkinny_Yellow",
			"ZmbF_HikerSkinny_Blue",
			"ZmbF_HikerSkinny_Grey",
			"ZmbF_HikerSkinny_Green",
			"ZmbF_HikerSkinny_Red",
			"ZmbF_SurvivorNormal_Blue",
			"ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red",
			"ZmbF_SurvivorNormal_White",
			"ZmbM_CommercialPilotOld_Blue",
			"ZmbM_CommercialPilotOld_Olive",
			"ZmbM_CommercialPilotOld_Brown",
			"ZmbM_CommercialPilotOld_Grey",
			"ZmbM_JoggerSkinny_Blue",
			"ZmbM_JoggerSkinny_Green",
			"ZmbM_JoggerSkinny_Red",
			"ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown",
			"ZmbF_JoggerSkinny_Green",
			"ZmbF_JoggerSkinny_Red",
			"ZmbM_MotobikerFat_Beige",
			"ZmbM_MotobikerFat_Black",
			"ZmbM_MotobikerFat_Blue",
			"ZmbM_VillagerOld_Blue",
			"ZmbM_VillagerOld_Green",
			"ZmbM_VillagerOld_White",
			"ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown",
			"ZmbM_SkaterYoung_Green",
			"ZmbM_SkaterYoung_Grey",
			"ZmbF_SkaterYoung_Brown",
			"ZmbF_SkaterYoung_Striped",
			"ZmbF_SkaterYoung_Violet",
			"ZmbM_OffshoreWorker_Green",
			"ZmbM_OffshoreWorker_Orange",
			"ZmbM_OffshoreWorker_Red",
			"ZmbM_OffshoreWorker_Yellow",
			"ZmbM_Jacket_beige",
			"ZmbM_Jacket_black",
			"ZmbM_Jacket_blue",
			"ZmbM_Jacket_bluechecks",
			"ZmbM_Jacket_brown",
			"ZmbM_Jacket_greenchecks",
			"ZmbM_Jacket_grey",
			"ZmbM_Jacket_khaki",
			"ZmbM_Jacket_magenta",
			"ZmbM_Jacket_stripes",
			"ZmbF_ShortSkirt_beige",
			"ZmbF_ShortSkirt_black",
			"ZmbF_ShortSkirt_brown",
			"ZmbF_ShortSkirt_green",
			"ZmbF_ShortSkirt_grey",
			"ZmbF_ShortSkirt_checks",
			"ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes",
			"ZmbF_ShortSkirt_white",
			"ZmbF_ShortSkirt_yellow",
			"ZmbF_VillagerOld_Blue",
			"ZmbF_VillagerOld_Green",
			"ZmbF_VillagerOld_Red",
			"ZmbF_VillagerOld_White",
			"ZmbF_MilkMaidOld_Beige",
			"ZmbF_MilkMaidOld_Black",
			"ZmbF_MilkMaidOld_Green",
			"ZmbF_MilkMaidOld_Grey",
			"ZmbF_BlueCollarFat_Blue",
			"ZmbF_BlueCollarFat_Green",
			"ZmbF_BlueCollarFat_Red",
			"ZmbF_BlueCollarFat_White",
			"ZmbF_MechanicNormal_Beige",
			"ZmbF_MechanicNormal_Green",
			"ZmbF_MechanicNormal_Grey",
			"ZmbF_MechanicNormal_Orange",
			"ZmbM_MechanicSkinny_Blue",
			"ZmbM_MechanicSkinny_Grey",
			"ZmbM_MechanicSkinny_Green",
			"ZmbM_MechanicSkinny_Red",
			"ZmbM_ConstrWorkerNormal_Beige",
			"ZmbM_ConstrWorkerNormal_Black",
			"ZmbM_ConstrWorkerNormal_Green",
			"ZmbM_ConstrWorkerNormal_Grey",
			"ZmbM_HeavyIndustryWorker",
		};

		int itemCount = 50;
		int infectedCount = 10;

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainer( "ExpansionAirdropContainer_Basebuilding", 0, 15, Loot, Infected, itemCount, infectedCount ) );
	}

	void DefaultMilitary()
	{
		array < ref ExpansionAirdropLoot > Loot;
		TStringArray Infected;

		TStringArray ak101_1 = { "AK_PlasticBttstck", "AK_PlasticHndgrd" };

		TStringArray ak74_1 = { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray ak74_2 = { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak74_3 = { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak74_4 = { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };

		array< ref ExpansionAirdropLootVariant > ak74Variants = {
			new ExpansionAirdropLootVariant( "AK74", ak74_2, 0.232323 ),
			new ExpansionAirdropLootVariant( "AK74", ak74_3, 0.252525 ),
			new ExpansionAirdropLootVariant( "AK74", ak74_4, 0.242424 ),
		};

		TStringArray ak74u_1 = { "AK74_WoodBttstck"};
		TStringArray ak74u_2 = { "AKS74U_Bttstck" };

		array< ref ExpansionAirdropLootVariant > ak74uVariants = {
			new ExpansionAirdropLootVariant( "AKS74U", ak74u_2, 0.545454 ),
		};

		TStringArray akm_1 = { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray akm_2 = { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray akm_3 = { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray akm_4 = { "AK74_WoodBttstck", "AK74_Hndgrd"};
		TStringArray akm_5 = { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};

		array< ref ExpansionAirdropLootVariant > akmVariants = {
			new ExpansionAirdropLootVariant( "AKM", akm_2, 0.2 ),
			new ExpansionAirdropLootVariant( "AKM", akm_3, 0.2 ),
			new ExpansionAirdropLootVariant( "AKM", akm_4, 0.207692 ),
			new ExpansionAirdropLootVariant( "AKM", akm_5, 0.2 ),
		};

		TStringArray m16_1 = { "ACOGOptic" };
		TStringArray m16_2 = { "UniversalLight" };

		array< ref ExpansionAirdropLootVariant > m16Variants = {
			new ExpansionAirdropLootVariant( "Expansion_M16", m16_2, 0.288889 ),
			new ExpansionAirdropLootVariant( "Expansion_M16", NULL, 0.4 ),
		};

		TStringArray m4a1_1 = { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 = { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		array< ref ExpansionAirdropLootVariant > m4a1Variants = {
			new ExpansionAirdropLootVariant( "M4A1", m4a1_2, 0.220339 ),
			new ExpansionAirdropLootVariant( "M4A1", m4a1_3, 0.237288 ),
			new ExpansionAirdropLootVariant( "M4A1", m4a1_4, 0.305085 ),
		};

		TStringArray fal_1 = { "Fal_OeBttstck" };
		TStringArray fal_2 = { "Fal_FoldingBttstck" };

		array< ref ExpansionAirdropLootVariant > falVariants = {
			new ExpansionAirdropLootVariant( "FAL", fal_2, 0.5 ),
		};

		TStringArray svd_1 = { "PSO1Optic"};

		array< ref ExpansionAirdropLootVariant > svdVariants = {
			new ExpansionAirdropLootVariant( "SVD", NULL, 0.642857 ),
		};

		TStringArray saiga_1 = { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 = { "Saiga_Bttstck" };

		array< ref ExpansionAirdropLootVariant > saigaVariants = {
			new ExpansionAirdropLootVariant( "Saiga", saiga_2, 0.545454 ),
		};

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };

		Loot = {
			new ExpansionAirdropLoot( "AK101", ak101_1 , 0.12),

			new ExpansionAirdropLoot( "AK74", ak74_1, 0.495, -1, ak74Variants ),

			new ExpansionAirdropLoot( "AKS74U", ak74u_1, 0.55, -1, ak74uVariants ),
			
			new ExpansionAirdropLoot( "AKM", akm_1, 1.3, -1, akmVariants ),

			new ExpansionAirdropLoot( "Expansion_M16", m16_1, 0.45, -1, m16Variants ),

			new ExpansionAirdropLoot( "M4A1", m4a1_1, 0.59, -1, m4a1Variants ),

			new ExpansionAirdropLoot( "FAL", fal_1, 0.24, -1, falVariants ),

			new ExpansionAirdropLoot( "SVD", svd_1, 0.28, -1, svdVariants ),

			new ExpansionAirdropLoot( "Saiga", saiga_1, 0.22, -1, saigaVariants ),

			new ExpansionAirdropLoot( "Expansion_M79", NULL , 0.1 ),

			new ExpansionAirdropLoot( "ExpansionRPG7", NULL , 0.1 ),
			new ExpansionAirdropLoot( "ExpansionLAW", NULL , 0.1 ),

			new ExpansionAirdropLoot( "AK_RailHndgrd", NULL , 0.1 ),
			new ExpansionAirdropLoot( "AK_Bayonet", NULL , 0.1 ),
			new ExpansionAirdropLoot( "KobraOptic", NULL , 0.08 ),
			new ExpansionAirdropLoot( "UniversalLight", NULL , 0.08),
			new ExpansionAirdropLoot( "KobraOptic", battery , 0.08),
			new ExpansionAirdropLoot( "UniversalLight", battery , 0.08),
			new ExpansionAirdropLoot( "ACOGOptic", NULL , 0.08),
			new ExpansionAirdropLoot( "M4_RisHndgrd", NULL , 0.1),
			new ExpansionAirdropLoot( "M9A1_Bayonet", NULL , 0.1),
			new ExpansionAirdropLoot( "Mosin_Bayonet", NULL , 0.1),
			new ExpansionAirdropLoot( "PUScopeOptic", NULL , 0.1),
			new ExpansionAirdropLoot( "TLRLight", NULL , 0.1),
			new ExpansionAirdropLoot( "TLRLight", battery , 0.08),
			new ExpansionAirdropLoot( "SKS_Bayonet", NULL , 0.1),

			new ExpansionAirdropLoot( "M68Optic", NULL , 0.1),
			new ExpansionAirdropLoot( "M4_T3NRDSOptic", NULL , 0.1),
			new ExpansionAirdropLoot( "FNP45_MRDSOptic", NULL , 0.1),
			new ExpansionAirdropLoot( "ExpansionReflexMRSOptic", NULL , 0.1),
			new ExpansionAirdropLoot( "PSO1Optic", NULL , 0.1),
			new ExpansionAirdropLoot( "PSO11Optic", NULL , 0.1),
			new ExpansionAirdropLoot( "M68Optic", battery , 0.08),
			new ExpansionAirdropLoot( "M4_T3NRDSOptic", battery , 0.08),
			new ExpansionAirdropLoot( "ExpansionReflexMRSOptic", battery , 0.08),
			new ExpansionAirdropLoot( "PSO1Optic", battery , 0.08),
			new ExpansionAirdropLoot( "PSO11Optic", battery , 0.08),

			new ExpansionAirdropLoot( "RGD5Grenade", NULL , 0.08),
			new ExpansionAirdropLoot( "M67Grenade", NULL , 0.05),
			new ExpansionAirdropLoot( "M18SmokeGrenade_Red", NULL , 0.1),
			new ExpansionAirdropLoot( "M18SmokeGrenade_Green", NULL , 0.1),
			new ExpansionAirdropLoot( "M18SmokeGrenade_Yellow", NULL , 0.1),
			new ExpansionAirdropLoot( "M18SmokeGrenade_Purple", NULL , 0.1),
			new ExpansionAirdropLoot( "M18SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLoot( "RDG2SmokeGrenade_Black", NULL , 0.1),
			new ExpansionAirdropLoot( "RDG2SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLoot( "LandMineTrap", NULL , 0.1),

			new ExpansionAirdropLoot( "Mag_AK101_30Rnd", NULL , 0.10),
			new ExpansionAirdropLoot( "Mag_AK74_30Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "Mag_AKM_30Rnd", NULL , 0.2),
			new ExpansionAirdropLoot( "Mag_AKM_Drum75Rnd", NULL , 0.06),
			new ExpansionAirdropLoot( "Mag_AKM_Palm30Rnd", NULL , 0.15),
			new ExpansionAirdropLoot( "Mag_CMAG_20Rnd", NULL , 0.3),
			new ExpansionAirdropLoot( "Mag_CMAG_30Rnd", NULL , 0.15),
			new ExpansionAirdropLoot( "Mag_CMAG_40Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "Mag_FAL_20Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "Mag_STANAGCoupled_30Rnd", NULL , 0.08),
			new ExpansionAirdropLoot( "Mag_STANAG_30Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "Mag_SVD_10Rnd", NULL , 0.08),
			new ExpansionAirdropLoot( "Mag_Saiga_5Rnd", NULL , 0.15),
			new ExpansionAirdropLoot( "Mag_Saiga_8Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "Mag_Saiga_Drum20Rnd", NULL , 0.08),

			new ExpansionAirdropLoot( "AmmoBox_762x54Tracer_20Rnd", NULL , 0.08),
			new ExpansionAirdropLoot( "AmmoBox_762x54_20Rnd", NULL , 0.1),
			new ExpansionAirdropLoot( "AmmoBox_762x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLoot( "AmmoBox_762x39_20Rnd", NULL , 0.09),
			new ExpansionAirdropLoot( "AmmoBox_556x45Tracer_20Rnd", NULL , 0.05),
			new ExpansionAirdropLoot( "AmmoBox_556x45_20Rnd", NULL , 0.08),
			new ExpansionAirdropLoot( "AmmoBox_545x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLoot( "AmmoBox_545x39_20Rnd", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_762x54Tracer", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_762x54", NULL , 0.1),
			new ExpansionAirdropLoot( "Ammo_762x39Tracer", NULL , 0.1),
			new ExpansionAirdropLoot( "Ammo_762x39", NULL , 0.12),
			new ExpansionAirdropLoot( "Ammo_556x45Tracer", NULL , 0.07),
			new ExpansionAirdropLoot( "Ammo_556x45", NULL , 0.11),
			new ExpansionAirdropLoot( "Ammo_545x39Tracer", NULL , 0.07),
			new ExpansionAirdropLoot( "Ammo_545x39", NULL , 0.10),

			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_White", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_Red", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_Green", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_Yellow", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_Purple", NULL , 0.08),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Smoke_Teargas", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_White", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_Red", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_Green", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_Yellow", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_Purple", NULL , 0.05),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_Sticky_Smoke_Teargas", NULL , 0.02),
			new ExpansionAirdropLoot( "Ammo_Expansion_M203_HE", NULL , 0.7),

			new ExpansionAirdropLoot( "ExpansionAmmoRPG", NULL , 0.05),

			new ExpansionAirdropLoot( "UKAssVest_Black", NULL , 0.1),
			new ExpansionAirdropLoot( "UKAssVest_Camo", NULL , 0.1),
			new ExpansionAirdropLoot( "UKAssVest_Khaki", NULL , 0.1),
			new ExpansionAirdropLoot( "UKAssVest_Olive", NULL , 0.1),

			new ExpansionAirdropLoot( "HighCapacityVest_Black", NULL , 0.1),
			new ExpansionAirdropLoot( "HighCapacityVest_Olive", NULL , 0.1),

			new ExpansionAirdropLoot( "PlateCarrierVest", NULL , 0.08),
			new ExpansionAirdropLoot( "PlateCarrierHolster", NULL , 0.1),
			new ExpansionAirdropLoot( "PlateCarrierPouches", NULL , 0.1),
			new ExpansionAirdropLoot( "PlateCarrierVest", vest , 0.05),

			new ExpansionAirdropLoot( "TacticalBaconCan", NULL , 0.1),

			new ExpansionAirdropLoot( "Bear_Pink", NULL , 0.1),
			
			new ExpansionAirdropLoot( "GhillieAtt_Mossy", NULL , 0.2),
			new ExpansionAirdropLoot( "GhillieHood_Mossy", NULL , 0.1),
			new ExpansionAirdropLoot( "GhillieBushrag_Mossy", NULL , 0.05),
			new ExpansionAirdropLoot( "GhillieSuit_Mossy", NULL , 0.05),
		};

		Infected = {	
			"ZmbM_usSoldier_normal_Woodland",
			"ZmbM_SoldierNormal",
			"ZmbM_usSoldier_normal_Desert",
			"ZmbM_PatrolNormal_PautRev",
			"ZmbM_PatrolNormal_Autumn",
			"ZmbM_PatrolNormal_Flat",
			"ZmbM_PatrolNormal_Summer",
		};

		int itemCount = 50;
		int infectedCount = 50;

		if( Containers )
		{
			//! Set default markers depending on map name
			string world_name = "empty";
			GetGame().GetWorldName(world_name);
			world_name.ToLower();
			
			if ( world_name.Contains( "namalsk" ) || world_name.Contains( "namalskgloom" ) )
			{
				Containers.Insert( new ExpansionAirdropLootContainer( "ExpansionAirdropContainer_Military_WinterCamo", 0, 20, Loot, Infected, itemCount, infectedCount ) );
			} else {
				Containers.Insert( new ExpansionAirdropLootContainer( "ExpansionAirdropContainer_Military", 0, 20, Loot, Infected, itemCount, infectedCount ) );
			}
		}
	}
	
	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Airdrop Settings";
	}
};
