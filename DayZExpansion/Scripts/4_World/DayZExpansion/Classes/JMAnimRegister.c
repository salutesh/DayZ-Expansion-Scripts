modded class JMAnimRegister
{
	override void OnRegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterEmptyHanded( pType, pBehavior );
	}

	override void OnRegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterTwoHanded( pType, pBehavior );
		
		//! Small Placables
		pType.AddItemInHandsProfileIK("ExpansionCone", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/spotlight.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignDanger2", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/shovel.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignDanger3", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/spotlight.anm");
		
		//! Basebuilding Kits
		pType.AddItemInHandsProfileIK("ExpansionCamoTentKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
		
		//! Vehicle parts
		pType.AddItemInHandsProfileIK("ExpansionHelicopterBattery", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/batterytruck.anm");
		pType.AddItemInHandsProfileIK("ExpansionAircraftBattery", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/batterytruck.anm");
		
		//! Basebuilding Components
		pType.AddItemInHandsProfileIK("ExpansionLumber3", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber1_5", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber1", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber0_5", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
	}

	override void OnRegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterOneHanded( pType, pBehavior );

		//! Basebuilding Kits
		pType.AddItemInHandsProfileIK("ExpansionBarbedWireKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/barbed_wire.anm");
		
		//! Weapons
		pType.AddItemInHandsProfileIK("ExpansionExplosive", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/fence_kit_folded.anm");
		//pType.AddItemInHandsProfileIK("ExpansionAmmoRPG", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionM67Grenade", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", pBehavior, "dz/anims/anm/player/ik/gear/soda_can.anm");
		pType.AddItemInHandsProfileIK("ExpansionRGD5Grenade","dz/anims/workspaces/player/player_main/weapons/player_main_1h_knife.asi", pBehavior, "dz/anims/anm/player/ik/gear/hammer_ik.anm");
		pType.AddItemInHandsProfileIK("ExpansionM18SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/explosives/smokegrenade.anm");
		
		//! Consumables
		pType.AddItemInHandsProfileIK("ExpansionBread1", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionBread2", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionBread3", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/rice.anm");
		
		//! Magazines
		pType.AddItemInHandsProfileIK("Mag_Expansion_MPX_50Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_ump45.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_Vityaz_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_ump45.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_MP7_40Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_G36_30Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_AWM_5rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_M14_10Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_M14_20Rnd", "dz/anims/workspaces/player/player_main/props/player_main_STANAG_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_stanag30.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_Kedr_20Rnd", "dz/anims/workspaces/player/player_main/props/player_main_mp5k_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_ump45.anm");
		pType.AddItemInHandsProfileIK("Mag_Expansion_M9_15Rnd", "dz/anims/workspaces/player/player_main/props/player_main_fnp45_mag.asi", pBehavior, "dz/anims/anm/player/ik/attachments/magazines/magazine_CZ75.anm");
		
		//! Weapon Attachments
		pType.AddItemInHandsProfileIK("ExpansionEXPS3HoloOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_m68_cco.anm");
		pType.AddItemInHandsProfileIK("Expansion_PMII25Optic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("ExpansionHAMROptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("ExpansionKar98ScopeOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("ExpansionReflexMRSOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_reflex.anm");
		pType.AddItemInHandsProfileIK("ExpansionDeltapointOptic", "dz/anims/workspaces/player/player_main/props/player_main_1h_scope_kobra.asi", pBehavior, "dz/anims/anm/player/ik/attachments/optic/optic_fnp45_mrd.anm");
		pType.AddItemInHandsProfileIK("ExpansionHydraulicHoses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/banana.anm");
		
		//! Misc Items
		pType.AddItemInHandsProfileIK("ExpansionCarKey", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", pBehavior, "dz/anims/anm/player/ik/gear/handcuff_keys.anm");	
		pType.AddItemInHandsProfileIK("ExpansionPowerSwitch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/fence_kit_folded.anm");
	
		//! Props
		pType.AddItemInHandsProfileIK("Expansion_Guitar", "DayZExpansion/Animations/Player/Props/player_main_guitar.asi", pBehavior, "dz/anims/anm/player/ik/gear/LongWoodenStick.anm");
	}

	override void OnRegisterRestrained( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterRestrained( pType, pBehavior );
	}

	override void OnRegisterSurrender( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterSurrender( pType, pBehavior );
	}

	override void OnRegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterHeavy( pType, pBehavior );

		//! Misc Placables
		pType.AddItemInHandsProfileIK("ExpansionSignDanger", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignRoadBarrier", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
	
		//! Basebuilding Placables
		pType.AddItemInHandsProfileIK("ExpansionHescoKit", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("ExpansionCamoBoxKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/tent_medium_packed.anm");
		pType.AddItemInHandsProfileIK("ExpansionBarrierGateKit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");
		pType.AddItemInHandsProfileIK("ExpansionSafeLarge", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionSafeMini", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionSafeMedium", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionStove", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionGunrack", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/sea_chest.anm");
		
		#ifdef EXPANSION_CHICKEN_DISABLE
		pType.AddItemInHandsProfileIK("ExpansionBreader", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		#endif
		
		//! Vehicle Parts
		pType.AddItemInHandsProfileIK("ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("TransitBusWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("TransitBusWheelDouble", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorFrontWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorFrontWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/v3s_wheel.anm");
	}

	override void OnRegisterPistol( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterPistol( pType, pBehavior );
		
		// pType.AddItemInHandsProfileIK("ExpansionFlaregunBase", "DayZExpansion/Animations/Player/Weapons/player_main_flaregun.asi", pBehavior, "dz/anims/anm/player/ik/weapons/fnp45.anm", "dz/anims/anm/player/reloads/FNP45/w_fnp45_states.anm" );		
		pType.AddItemInHandsProfileIK("Expansion_M9_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz75.asi", pBehavior, "dz/anims/anm/player/ik/weapons/CZ75.anm", "dz/anims/anm/player/reloads/CZ75/w_CZ75_states.anm");	
		pType.AddItemInHandsProfileIK("Expansion_Longhorn_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_flaregun.asi", pBehavior, "dz/anims/anm/player/ik/weapons/flaregun.anm", "dz/anims/anm/player/reloads/Flaregun/w_Flaregun_states.anm");
		
	}

	override void OnRegisterFireArms( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterFireArms( pType, pBehavior );
		
		pType.AddItemInHandsProfileIK("Expansion_M79_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh18.asi", pBehavior, "dz/anims/anm/player/ik/weapons/izh18.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		pType.AddItemInHandsProfileIK("ExpansionRPG7Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh18.asi", pBehavior, "dz/anims/anm/player/ik/weapons/izh18.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_MPX_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", pBehavior, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");	   
		pType.AddItemInHandsProfileIK("Expansion_AWM_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz527.asi", pBehavior, "dz/anims/anm/player/ik/weapons/cz527.anm", "dz/anims/anm/player/reloads/cz527/w_CZ527_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_Kar98_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", pBehavior, "dz/anims/anm/player/ik/weapons/mosin9130.anm","dz/anims/anm/player/reloads/Mosin_1903/w_mosin1903_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_M1A_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_svd.asi", pBehavior, "dz/anims/anm/player/ik/weapons/svd.anm","dz/anims/anm/player/reloads/SVD/w_SVD_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_M14_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_svd.asi", pBehavior, "dz/anims/anm/player/ik/weapons/svd.anm","dz/anims/anm/player/reloads/SVD/w_SVD_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_M16_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", pBehavior, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");	   
		pType.AddItemInHandsProfileIK("Expansion_G36_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", pBehavior, "dz/anims/anm/player/ik/weapons/m4a1_ik.anm", "dz/anims/anm/player/reloads/M4A1/w_M4A1_states.anm");	   
		pType.AddItemInHandsProfileIK("Expansion_Kedr_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz61.asi", pBehavior, "dz/anims/anm/player/ik/weapons/CZ61.anm", "dz/anims/anm/player/reloads/cz61/w_cz61_states.anm");	   
		pType.AddItemInHandsProfileIK("Expansion_MP5_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ump45.asi", pBehavior, "dz/anims/anm/player/ik/weapons/ump.anm", "dz/anims/anm/player/reloads/UMP45/w_UMP45_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_BenelliM4_Base", "DayZExpansion/Animations/Player/Weapons/SemiAutomaticShotgun.asi", pBehavior, "dz/anims/anm/player/ik/weapons/mp133.anm", "dz/anims/anm/player/reloads/MP133/w_mp133_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_DT11_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_b95.asi", pBehavior, "dz/anims/anm/player/ik/weapons/b95.anm", "dz/anims/anm/player/reloads/B95/w_B95_states.anm"); 
		pType.AddItemInHandsProfileIK("ExpansionCrossbow_Base", "DayZExpansion/Animations/Player/weapons/player_main_Expansion_Crossbow.asi", pBehavior, "dz/anims/anm/player/ik/weapons/izh18.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		pType.AddItemInHandsProfileIK("Expansion_Vityaz_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_akm.asi", pBehavior, "dz/anims/anm/player/ik/weapons/akm.anm", "dz/anims/anm/player/reloads/AKM/w_akm_states.anm");
			
		// pType.AddItemInHandsProfileIK("ExpansionRPG7Base", "DayZExpansion/Animations/Player/Weapons/player_main_rpg.asi", pBehavior, "dz/anims/anm/player/ik/weapons/RPG.anm", "dz/anims/anm/player/reloads/IZH18/w_izh18_states.anm");
		// pType.AddItemInHandsProfileIK("ExpansionLAWBase", "DayZExpansion/Animations/Player/Weapons/player_main_law.asi", pBehavior, "dz/anims/anm/player/ik/weapons/SKS.anm", "dz/anims/anm/player/reloads/SKS/w_SKS_states.anm");
	}

	override void OnRegisterArcheryItem( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterArcheryItem( pType, pBehavior );
	}

	override void OnRegisterLauncher( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.OnRegisterLauncher( pType, pBehavior );
	}

	override void OnBoneRemapping( DayZPlayerType pType )
	{
		super.OnBoneRemapping( pType );

		array<string> DT11BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", "bullet2", "Weapon_Bone_04", "snaploader", "Weapon_Bone_05" };
		pType.AddItemBoneRemap("Expansion_DT11_Base", DT11BoneRemap);
		
		array<string> CrossbowBoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", };
		pType.AddItemBoneRemap("ExpansionCrossbow_Base", CrossbowBoneRemap);
		
		array<string> m79BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", };
		pType.AddItemBoneRemap("Expansion_M79_Base", m79BoneRemap);		
		
		array<string> RPG7BoneRemap = { "release", "Weapon_Bone_02", "barrel", "Weapon_Bone_01", "trigger", "Weapon_Trigger","ejector","Weapon_Bone_03","bullet", "Weapon_Bullet", };
		pType.AddItemBoneRemap("ExpansionRPG7Base", RPG7BoneRemap);
		
		array<string> WorkingLongHornBoneRemap = { "hammer", "Weapon_Bone_02", "reload", "Weapon_Bone_01", "trigger", "Weapon_Trigger" ,"bullet", "Weapon_Bullet" };
		pType.AddItemBoneRemap("Expansion_Longhorn_Base", WorkingLongHornBoneRemap);

		#ifndef EXPANSION_CHICKEN_DISABLE
		{
			DayzPlayerItemBehaviorCfg chickenBehaviour = new DayzPlayerItemBehaviorCfg;
			chickenBehaviour.SetChicken();
		
			pType.AddItemInHandsProfileIK("ExpansionChicken", "dz/anims/workspaces/player/player_main/props/player_main_2h_pot.asi", chickenBehaviour, "dz/anims/anm/player/ik/two_handed/CookingPot.anm");
			pType.AddItemInHandsProfileIK("ExpansionChickenFemale", "dz/anims/workspaces/player/player_main/props/player_main_2h_pot.asi", chickenBehaviour, "dz/anims/anm/player/ik/two_handed/CookingPot.anm");
		}
		#endif
	}
}