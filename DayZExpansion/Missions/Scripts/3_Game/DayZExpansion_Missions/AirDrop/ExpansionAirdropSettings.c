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
class ExpansionAirdropSettingsBase: ExpansionSettingBase
{
	bool ServerMarkerOnDropLocation;
	bool Server3DMarkerOnDropLocation;
	bool ShowAirdropTypeOnMarker;
	bool HideCargoWhileParachuteIsDeployed;
	bool HeightIsRelativeToGroundLevel;

	float Height;
	float FollowTerrainFraction;
	float Speed;
	float Radius;
	float InfectedSpawnRadius;

	int InfectedSpawnInterval;  //! ms

	//! Only kept for backwards compatibility, should be set per container
	int ItemCount;
	
};

class ExpansionAirdropSettingsV4: ExpansionAirdropSettingsBase
{
	ref array < ref ExpansionLootContainerV1 > Containers;
};

class ExpansionAirdropSettings: ExpansionAirdropSettingsBase
{
	static const int VERSION = 6;

	string AirdropPlaneClassName;
	
	ref array < ref ExpansionLootContainer > Containers = {};

	[NonSerialized()]
	private bool m_IsLoaded;
	
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
		ExpansionArray<ExpansionLootContainer>.RefCopy(s.Containers, Containers);
		
		ExpansionAirdropSettingsBase sb = s;
		CopyInternal( sb );
	}

	private void CopyInternal(  ExpansionAirdropSettingsBase s )
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

			ExpansionAirdropSettingsBase configbase;
			JsonFileLoader<ExpansionAirdropSettingsBase>.JsonLoadFile( EXPANSION_AIRDROP_SETTINGS, configbase );

			if (configbase.m_Version < VERSION)
			{
				ExpansionAirdropSettings defaults = new ExpansionAirdropSettings;
				defaults.Defaults();

				if (configbase.m_Version < 5)
				{
					ExpansionAirdropSettingsV4 configV4;
					if (!ExpansionJsonFileParser<ExpansionAirdropSettingsV4>.Load(EXPANSION_AIRDROP_SETTINGS, configV4))
					{
						Defaults();
						return false;
					}

					CopyInternal(configbase);

					foreach(ExpansionLootContainerV1 containerV1: configV4.Containers)
					{
						if (configbase.m_Version < 2)
						{
							foreach (ExpansionLootV1 lootV1OrLower: containerV1.Loot)
							{
								if (!lootV1OrLower.QuantityPercent)
									lootV1OrLower.QuantityPercent = -1;

								if (!lootV1OrLower.Max)
									lootV1OrLower.Max = -1;
							}
						}

						if (configbase.m_Version < 3)
							containerV1.FallSpeed = 4.5;

						ExpansionLootContainer newcontainer = containerV1.Convert();
						Containers.Insert( newcontainer );
					}
				}
				else
				{
					if (!ExpansionJsonFileParser<ExpansionAirdropSettings>.Load(EXPANSION_AIRDROP_SETTINGS, this))
					{
						Defaults();
						return false;
					}

					if (m_Version < 6 && !AirdropPlaneClassName)
						AirdropPlaneClassName = defaults.AirdropPlaneClassName;
				}

				if (configbase.m_Version < 4)
					HideCargoWhileParachuteIsDeployed = defaults.HideCargoWhileParachuteIsDeployed;

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionAirdropSettings>.JsonLoadFile( EXPANSION_AIRDROP_SETTINGS, this );
			}
		}
		else
		{
			EXPrint("[ExpansionAirdropSettings] No existing setting file:" + EXPANSION_AIRDROP_SETTINGS + ". Creating defaults!");
			Defaults();
			DefaultRegular();
			DefaultMedical();
			DefaultBaseBuilding();
			DefaultMilitary();
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
		m_Version = VERSION;

		ServerMarkerOnDropLocation = true;
		Server3DMarkerOnDropLocation = true;
		ShowAirdropTypeOnMarker = true;
		HideCargoWhileParachuteIsDeployed = true;
		HeightIsRelativeToGroundLevel = true;

		Height = 450;
		FollowTerrainFraction = 0.5;
		Speed = 35;
		Radius = 1;
		InfectedSpawnRadius = 50;
		InfectedSpawnInterval = 250;

		ItemCount = 50;

		AirdropPlaneClassName = "";

		Containers.Clear();
	}

	void DefaultRegular()
	{
		array < ref ExpansionLoot > Loot = ExpansionLootDefaults.Airdrop_Regular();
		TStringArray Infected = {	
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

		Containers.Insert( new ExpansionLootContainer( "ExpansionAirdropContainer", 0, 5, Loot, Infected, itemCount, infectedCount ) );	
	}

	void DefaultMedical()
	{
		array < ref ExpansionLoot > Loot = ExpansionLootDefaults.Airdrop_Medical();
		TStringArray Infected = {	
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

		Containers.Insert( new ExpansionLootContainer( "ExpansionAirdropContainer_Medical", 0, 10, Loot, Infected, itemCount, infectedCount ) );
	}

	void DefaultBaseBuilding()
	{
		array < ref ExpansionLoot > Loot = ExpansionLootDefaults.Airdrop_BaseBuilding();
		TStringArray Infected = {	
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

		Containers.Insert( new ExpansionLootContainer( "ExpansionAirdropContainer_Basebuilding", 0, 15, Loot, Infected, itemCount, infectedCount ) );
	}

	void DefaultMilitary()
	{
		array < ref ExpansionLoot > Loot = ExpansionLootDefaults.Airdrop_Military();
		TStringArray Infected = {	
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

		//! Set defaults depending on map name
		switch (ExpansionStatic.GetCanonicalWorldName())
		{
			case "namalsk":
			case "sakhal":
				Containers.Insert( new ExpansionLootContainer( "ExpansionAirdropContainer_Military_WinterCamo", 0, 20, Loot, Infected, itemCount, infectedCount ) );
				break;
			default:
				Containers.Insert( new ExpansionLootContainer( "ExpansionAirdropContainer_Military", 0, 20, Loot, Infected, itemCount, infectedCount ) );
				break;
		}
	}
	
	// ------------------------------------------------------------	
	override string SettingName()
	{
		return "Airdrop Settings";
	}
};
