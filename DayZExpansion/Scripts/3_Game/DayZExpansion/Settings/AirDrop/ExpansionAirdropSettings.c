/**
 * ExpansionAirdropSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	float Height;
	float Speed;

	int ItemCount;

	ref array < ref ExpansionAirdropLootContainers > Containers;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionAirdropSettings()
	{
		Containers =  new array< ref ExpansionAirdropLootContainers >;
	}
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::HandleRPC - Start");
		#endif

		ExpansionAirdropSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionAirdropSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Airdrop.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::HandleRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionAirdropSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.AirDrop, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Send - End and return");
		#endif
		return 0;
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
	private void CopyInternal( ref ExpansionAirdropSettings s )
	{
		Height = s.Height;
		Speed = s.Speed;
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Load - Start");
		#endif
		
		m_IsLoaded = true;
		
		if ( FileExist( EXPANSION_AIRDROP_SETTINGS ) )
		{
			JsonFileLoader<ExpansionAirdropSettings>.JsonLoadFile( EXPANSION_AIRDROP_SETTINGS, this );
	
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionAirdropSettings::Load - End - Loaded");
			#endif

			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Load - End - Not Loaded");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Save - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			JsonFileLoader<ExpansionAirdropSettings>.JsonSaveFile( EXPANSION_AIRDROP_SETTINGS, this );
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionAirdropSettings::Save - Settings saved!");
			#endif

			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionAirdropSettings::Save - End");
		#endif

		return false;
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("[ExpansionAirdropSettings] Loading default settings");
		#endif

		Height = 750;
		Speed = 1500;

		ItemCount = 50;

		DefaultRegular();
		DefaultMedical();
		DefaultBaseBuilding();
		DefaultMilitary();
	}

	void DefaultRegular()
	{
		ref array < ref ExpansionAirdropLootAttachments > Loot;
		
		TStringArray ak74u_1 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_WoodBttstck", "AK_WoodHndgrd" };

		TStringArray sks_1 = { "PUScopeOptic" };
		TStringArray sks_2 = { "SKS_Bayonet" };

		TStringArray ump_1 = { "UniversalLight" };

		TStringArray mosin_1 = { "PUScopeOptic" };

		TStringArray b95_1 = { "HuntingOptic" };

		TStringArray cz527_1 = { "HuntingOptic" };

		TStringArray cz75_1 = { "TLRLight" };

		TStringArray fnx1_1 = { "TLRLight" };
		
		TStringArray kedr_1 = { "PistolSuppressor" };  

		TStringArray winchester70_1 = { "HuntingOptic" };

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

		Loot = {
			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_1, 0.08 ),
			
			new ExpansionAirdropLootAttachments( "AKM", akm_1, 0.05 ),

			new ExpansionAirdropLootAttachments( "SKS", sks_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "SKS", sks_2, 0.1  ),
			new ExpansionAirdropLootAttachments( "SKS", NULL, 0.3 ),

			new ExpansionAirdropLootAttachments( "UMP45", ump_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "UMP45", NULL, 0.15 ),
 
			new ExpansionAirdropLootAttachments( "Mosin9130", mosin_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "Mosin9130", NULL, 0.3 ),
 
			new ExpansionAirdropLootAttachments( "B95", b95_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "B95", NULL, 0.2 ),

			new ExpansionAirdropLootAttachments( "CZ527", cz527_1, 0.3 ),
			new ExpansionAirdropLootAttachments( "CZ527", NULL, 0.2 ),

			new ExpansionAirdropLootAttachments( "CZ75", cz75_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "CZ75", NULL, 0.1 ),

			new ExpansionAirdropLootAttachments( "FNX45", fnx1_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "FNX45", NULL, 0.3 ),

			new ExpansionAirdropLootAttachments( "Expansion_Kedr", kedr_1, 0.2 ),
			new ExpansionAirdropLootAttachments( "Expansion_Kedr", NULL, 0.3 ),

   			new ExpansionAirdropLootAttachments( "Mp133Shotgun", NULL, 0.8 ),

			new ExpansionAirdropLootAttachments( "Winchester70", winchester70_1, 0.1 ),
			new ExpansionAirdropLootAttachments( "Winchester70", NULL, 0.4 ),
				
			new ExpansionAirdropLootAttachments( "Expansion_DT11", NULL, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "Binoculars", NULL, 0.3 ),
			new ExpansionAirdropLootAttachments( "ChernarusMap", NULL, 0.3 ),
			new ExpansionAirdropLootAttachments( "Rangefinder", battery, 0.05 ),
			new ExpansionAirdropLootAttachments( "ExpansionGPS", NULL, 0.05 ),
			
			new ExpansionAirdropLootAttachments( "BoxCerealCrunchin", NULL, 0.05 ),
			new ExpansionAirdropLootAttachments( "PeachesCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "BakedBeansCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "SpaghettiCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "SardinesCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "TunaCan", NULL, 0.1 ),
			new ExpansionAirdropLootAttachments( "WaterBottle", NULL, 0.5 ),
			
			new ExpansionAirdropLootAttachments( "CanOpener", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "KitchenKnife", NULL, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "BallisticHelmet_UN", NULL, 0.08 ),
			new ExpansionAirdropLootAttachments( "DirtBikeHelmet_Chernarus", visor, 0.3 ),
			
			new ExpansionAirdropLootAttachments( "SewingKit", NULL, 0.25 ),
			new ExpansionAirdropLootAttachments( "LeatherSewingKit", NULL, 0.25 ),
			new ExpansionAirdropLootAttachments( "WeaponCleaningKit", NULL, 0.05 ),
			new ExpansionAirdropLootAttachments( "Lockpick", NULL, 0.05 ),
			
			new ExpansionAirdropLootAttachments( "GhillieAtt_Mossy", NULL, 0.05 ),

			new ExpansionAirdropLootAttachments( "Mag_Expansion_Kedr_20Rnd", NULL, 0.8 ),
			new ExpansionAirdropLootAttachments( "Mag_CZ527_5rnd", NULL, 0.9 ),
			new ExpansionAirdropLootAttachments( "Mag_CZ75_15Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Mag_FNX45_15Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Mag_UMP_25Rnd", NULL, 0.5 ),

			new ExpansionAirdropLootAttachments( "AmmoBox_9x39_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_9x19_25Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39Tracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39_20Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_45ACP_25Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_308WinTracer_20Rnd", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_308Win_20Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaRubberSlug_10Rnd", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "AmmoBox_12gaPellets_10Rnd", NULL, 1 ),
			
			new ExpansionAirdropLootAttachments( "Ammo_9x39", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_762x39Tracer", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_762x39", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_45ACP", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_308WinTracer", NULL, 0.5 ),
			new ExpansionAirdropLootAttachments( "Ammo_308Win", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaSlug", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaRubberSlug", NULL, 1 ),
			new ExpansionAirdropLootAttachments( "Ammo_12gaPellets", NULL,  0.5 ),
			
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_1 , 0.03),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_2 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_3 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_4 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_5 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_6 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_7 , 0.05),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_8 , 0.05),
		};

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainers( "ExpansionAirdropContainer", Loot ) );	
	}

	void DefaultMedical()
	{
		ref array < ref ExpansionAirdropLootAttachments > Loot;

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };
		
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

		Loot = {
			new ExpansionAirdropLootAttachments( "BandageDressing", NULL , 0.15),

			new ExpansionAirdropLootAttachments( "Morphine", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Epinephrine", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "TacticalBaconCan", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "PainkillerTablets", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "CharcoalTablets", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TetracyclineAntiBiotics", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "DisinfectantSpray", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "DisinfectantAlcohol", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "VitaminBottle", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "StartKitIV", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "SalineBagIV", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "SalineBag", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "BloodBagEmpty", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "BloodBagIV", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "BloodTestKit", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "BloodTestKit", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsHat_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsPants_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "MedicalScrubsShirt_Green", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "NioshFaceMask", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "SurgicalGloves_Blue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "SurgicalGloves_LightBlue", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "SurgicalGloves_Green", NULL , 0.08),		

			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_1 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_2 , 0.1),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_3 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_4 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_5 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_6 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_7 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_8 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_9 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_10 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_11 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_12 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_13 , 0.18),
			new ExpansionAirdropLootAttachments( "FirstAidKit", firstaidkit_14 , 0.18),
		};

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainers( "ExpansionAirdropContainer_Medical", Loot ) );
	}

	void DefaultBaseBuilding()
	{
		ref array < ref ExpansionAirdropLootAttachments > Loot;

		Loot = {
			new ExpansionAirdropLootAttachments( "NailBox", NULL , 0.8),
			new ExpansionAirdropLootAttachments( "DuctTape", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "MetalPlate", NULL , 0.25),
			new ExpansionAirdropLootAttachments( "WoodenPlank", NULL , 0.5),
			new ExpansionAirdropLootAttachments( "Hammer", NULL , 0.4),
			new ExpansionAirdropLootAttachments( "Shovel", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Pliers", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "WoodAxe", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Crowbar", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Hacksaw", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Handsaw", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "SledgeHammer", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Hatchet", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionCodeLock", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "BarbedWire", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "MetalWire", NULL , 0.2),

			new ExpansionAirdropLootAttachments( "ExpansionHescoKit", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "EspansionBarbedWireKit", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "ExpansionCamoBoxKit", NULL , 0.18),
			new ExpansionAirdropLootAttachments( "ExpansionGunrack", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "ExpansionCone", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger2", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignDanger3", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSignRoadBarrier", NULL , 0.15),

			new ExpansionAirdropLootAttachments( "ExpansionSafeMini", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "ExpansionSafeMedium", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "ExpansionSafeLarge", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "ExpansionCamoTentKit", NULL , 0.25),
			new ExpansionAirdropLootAttachments( "MediumTent", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "LargeTent", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "CarTent", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "CamoNet", NULL , 0.3),
		};

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainers( "ExpansionAirdropContainer_Basebuilding", Loot ) );
	}

	void DefaultMilitary()
	{
		ref array < ref ExpansionAirdropLootAttachments > Loot;

		TStringArray ak101_1 = { "AK_PlasticBttstck", "AK_PlasticHndgrd" };

		TStringArray ak74_1 = { "AK74_WoodBttstck", "AK74_Hndgrd" };
		TStringArray ak74_2 = { "AK_FoldingBttstck", "AK74_Hndgrd" };
		TStringArray ak74_3 = { "AK_FoldingBttstck", "AK74_Hndgrd", "AK_Bayonet" };
		TStringArray ak74_4 = { "KobraOptic", "AK_WoodHndgrd", "AK74_WoodBttstck" };

		TStringArray ak74u_1 = { "AK74_WoodBttstck"};
		TStringArray ak74u_2 = { "AKS74U_Bttstck" };

		TStringArray akm_1 = { "AK_FoldingBttstck", "AK_RailHndgrd" };
		TStringArray akm_2 = { "AK_WoodBttstck", "AK_WoodHndgrd", "KobraOptic" };
		TStringArray akm_3 = { "AK_PlasticBttstck", "AK_RailHndgrd"};
		TStringArray akm_4 = { "AK74_WoodBttstck", "AK74_Hndgrd"};
		TStringArray akm_5 = { "AK_PlasticBttstck", "AK_RailHndgrd", "UniversalLight"};

		TStringArray m16_1 = { "ACOGOptic" };
		TStringArray m16_2 = { "UniversalLight" };

		TStringArray m4a1_1 = { "M4_CarryHandleOptic", "M4_OEBttstck", "M4_PlasticHndgrd" };
		TStringArray m4a1_2 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd" };
		TStringArray m4a1_3 = { "ACOGOptic", "M4_MPBttstck", "M4_MPHndgrd" };
		TStringArray m4a1_4 = { "BUISOptic", "M4_CQBBttstck", "M4_RisHndgrd", "UniversalLight"};

		TStringArray fal_1 = { "Fal_OeBttstck" };
		TStringArray fal_2 = { "Fal_FoldingBttstck" };

		TStringArray svd_1 = { "PSO1Optic"};

		TStringArray saiga_1 = { "Saiga_Bttstck", "KobraOptic" };
		TStringArray saiga_2 = { "Saiga_Bttstck" };

		TStringArray colt1911_1 = { "TLRLight" };

		TStringArray glock19_1 = { "TLRLight" };

		TStringArray benelim4 = { "UniversalLight" };

		TStringArray winchester70_1 = { "HuntingOptic" };

		TStringArray battery = { "Battery9V" };
		TStringArray vest = { "PlateCarrierHolster","PlateCarrierPouches" };

		Loot = {
			new ExpansionAirdropLootAttachments( "AK101", ak101_1 , 0.12),

			new ExpansionAirdropLootAttachments( "AK74", ak74_1 , 0.135),
			new ExpansionAirdropLootAttachments( "AK74", ak74_2 , 0.115),
			new ExpansionAirdropLootAttachments( "AK74", ak74_3 , 0.125),
			new ExpansionAirdropLootAttachments( "AK74", ak74_4 , 0.12),

			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_1 , 0.25),
			new ExpansionAirdropLootAttachments( "AKS74U", ak74u_2 , 0.3),
			
			new ExpansionAirdropLootAttachments( "AKM", akm_1 , 0.25),
			new ExpansionAirdropLootAttachments( "AKM", akm_2 , 0.26),
			new ExpansionAirdropLootAttachments( "AKM", akm_3 , 0.26),
			new ExpansionAirdropLootAttachments( "AKM", akm_4 , 0.27),
			new ExpansionAirdropLootAttachments( "AKM", akm_5 , 0.26),

			new ExpansionAirdropLootAttachments( "Expansion_M16", m16_1 , 0.14),
			new ExpansionAirdropLootAttachments( "Expansion_M16", m16_2 , 0.13),
			new ExpansionAirdropLootAttachments( "Expansion_M16", NULL , 0.18),

			new ExpansionAirdropLootAttachments( "M4A1", m4a1_1 , 0.14),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_2 , 0.13),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_3 , 0.14),
			new ExpansionAirdropLootAttachments( "M4A1", m4a1_4 , 0.18),

			new ExpansionAirdropLootAttachments( "FAL", fal_1 , 0.12),
			new ExpansionAirdropLootAttachments( "FAL", fal_2 , 0.12),

			new ExpansionAirdropLootAttachments( "SVD", svd_1 , 0.1),
			new ExpansionAirdropLootAttachments( "SVD", NULL , 0.18),

			new ExpansionAirdropLootAttachments( "Saiga", saiga_1 , 0.10),
			new ExpansionAirdropLootAttachments( "Saiga", saiga_2 , 0.12),

			new ExpansionAirdropLootAttachments( "Expansion_M79", NULL , 0.1 ),

			new ExpansionAirdropLootAttachments( "ExpansionRPG7", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "ExpansionLAW", NULL , 0.1 ),

			new ExpansionAirdropLootAttachments( "AK_RailHndgrd", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "AK_Bayonet", NULL , 0.1 ),
			new ExpansionAirdropLootAttachments( "KobraOptic", NULL , 0.08 ),
			new ExpansionAirdropLootAttachments( "UniversalLight", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "KobraOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "UniversalLight", battery , 0.08),
			new ExpansionAirdropLootAttachments( "ACOGOptic", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "M4_RisHndgrd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M9A1_Bayonet", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mosin_Bayonet", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PUScopeOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TLRLight", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "TLRLight", battery , 0.08),
			new ExpansionAirdropLootAttachments( "SKS_Bayonet", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "M68Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M4_T3NRDSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "FNP45_MRDSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "ExpansionReflexMRSOptic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PSO1Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PSO11Optic", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M68Optic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "M4_T3NRDSOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "ExpansionReflexMRSOptic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "PSO1Optic", battery , 0.08),
			new ExpansionAirdropLootAttachments( "PSO11Optic", battery , 0.08),

			new ExpansionAirdropLootAttachments( "RGD5Grenade", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "M67Grenade", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Red", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Green", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Yellow", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_Purple", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "M18SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "RDG2SmokeGrenade_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "RDG2SmokeGrenade_White", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "LandMineTrap", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "Mag_AK101_30Rnd", NULL , 0.10),
			new ExpansionAirdropLootAttachments( "Mag_AK74_30Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_AKM_30Rnd", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "Mag_AKM_Drum75Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "Mag_AKM_Palm30Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_20Rnd", NULL , 0.3),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_30Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_CMAG_40Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_FAL_20Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_STANAGCoupled_30Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Mag_STANAG_30Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_SVD_10Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_5Rnd", NULL , 0.15),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_8Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Mag_Saiga_Drum20Rnd", NULL , 0.08),

			new ExpansionAirdropLootAttachments( "AmmoBox_762x54Tracer_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x54_20Rnd", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "AmmoBox_762x39_20Rnd", NULL , 0.09),
			new ExpansionAirdropLootAttachments( "AmmoBox_556x45Tracer_20Rnd", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "AmmoBox_556x45_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "AmmoBox_545x39Tracer_20Rnd", NULL , 0.06),
			new ExpansionAirdropLootAttachments( "AmmoBox_545x39_20Rnd", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_762x54Tracer", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_762x54", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Ammo_762x39Tracer", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "Ammo_762x39", NULL , 0.12),
			new ExpansionAirdropLootAttachments( "Ammo_556x45Tracer", NULL , 0.07),
			new ExpansionAirdropLootAttachments( "Ammo_556x45", NULL , 0.11),
			new ExpansionAirdropLootAttachments( "Ammo_545x39Tracer", NULL , 0.07),
			new ExpansionAirdropLootAttachments( "Ammo_545x39", NULL , 0.10),

			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_White", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Red", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Green", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Yellow", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Purple", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Smoke_Teargas", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_White", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Red", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Green", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Yellow", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Purple", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_Sticky_Smoke_Teargas", NULL , 0.02),
			new ExpansionAirdropLootAttachments( "Ammo_Expansion_M203_HE", NULL , 0.7),

			new ExpansionAirdropLootAttachments( "ExpansionAmmoRPG", NULL , 0.05),

			new ExpansionAirdropLootAttachments( "UKAssVest_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Camo", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Khaki", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "UKAssVest_Olive", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "HighCapacityVest_Black", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "HighCapacityVest_Olive", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "PlateCarrierVest", NULL , 0.08),
			new ExpansionAirdropLootAttachments( "PlateCarrierHolster", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PlateCarrierPouches", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "PlateCarrierVest", vest , 0.05),

			new ExpansionAirdropLootAttachments( "TacticalBaconCan", NULL , 0.1),

			new ExpansionAirdropLootAttachments( "Bear_Pink", NULL , 0.1),
			
			new ExpansionAirdropLootAttachments( "GhillieAtt_Mossy", NULL , 0.2),
			new ExpansionAirdropLootAttachments( "GhillieHood_Mossy", NULL , 0.1),
			new ExpansionAirdropLootAttachments( "GhillieBushrag_Mossy", NULL , 0.05),
			new ExpansionAirdropLootAttachments( "GhillieSuit_Mossy", NULL , 0.05),
		};

		if(Containers)
			Containers.Insert( new ExpansionAirdropLootContainers( "ExpansionAirdropContainer_Military", Loot ) );
	}
}