#define _ARMA_

class CfgPatches
{
	class BuilderItems_StaticItems_Weapons
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_akm: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\AKM\akm.p3d";
	};
	class bldr_AK_RailHndgrd_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_rail.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\DZ\weapons\attachments\data\akm_accessories_black_co.paa"};
	};
	class bldr_AK_RailHndgrd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_rail.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\DZ\weapons\attachments\data\akm_accessories_green_co.paa"};
	};
	class bldr_UniversalLight: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\light\weaponlight_universal.p3d";
	};
	class bldr_AK_Suppressor: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\muzzle\suppressor_545.p3d";
	};
	class bldr_Mag_AKM_Palm30Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_akm_palm30.p3d";
	};
	class bldr_ReflexOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_reflex.p3d";
	};
	class bldr_AK_FoldingBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_ak_folding.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_accessories_black_co.paa"};
	};
	class bldr_AK_FoldingBttstck_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_ak_folding.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_accessories_green_co.paa"};
	};
	class bldr_Crossbow_RedpointOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_crossbow.p3d";
	};
	class bldr_Crossbow_Base: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\archery\crossbow\crossbow.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_leaves_co.paa"};
	};
	class bldr_Crossbow_Black: bldr_Crossbow_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_black_co.paa"};
	};
	class bldr_Crossbow_Green: bldr_Crossbow_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_green_co.paa"};
	};
	class bldr_Crossbow_Wood: bldr_Crossbow_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_wood_co.paa"};
	};
	class bldr_Crossbow_Summer: bldr_Crossbow_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_summer_co.paa"};
	};
	class bldr_Crossbow_Camo: bldr_Crossbow_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dz\weapons\archery\crossbow\data\crossbow_leaves_co.paa"};
	};
	class bldr_RecurveBow: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\archery\bow_recurve\bow_recurve.p3d";
	};
	class bldr_AK_WoodHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_wood_co.paa"};
	};
	class bldr_AK74_Hndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\ak74_wood_co.paa"};
	};
	class bldr_AK_PlasticHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_plastic.p3d";
	};
	class bldr_AK_WoodBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_wood_co.paa"};
	};
	class bldr_AK74_WoodBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\ak74_wood_co.paa"};
	};
	class bldr_AK74_WoodBttstck_Camo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_wood_camo_co.paa"};
	};
	class bldr_AK_WoodHndgrd_Camo: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\akm_wood_camo_co.paa"};
	};
	class bldr_AK_PlasticBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_ak_plastic.p3d";
	};
	class bldr_AK101: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\AK101\ak101.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\AK101\data\ak101_co.paa"};
	};
	class bldr_AK101_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\AK101\ak101.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_AKS74U_Bttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_aks.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\AK74\data\aks74u_co.paa"};
	};
	class bldr_AKS74U_Bttstck_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_aks.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.37,0.28,1.0,CO)"};
	};
	class bldr_SKS: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\SKS\data\sks_co.paa"};
	};
	class bldr_SKS_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\SKS\data\sks_black_co.paa"};
	};
	class bldr_SKS_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\SKS\data\sks_green_co.paa"};
	};
	class bldr_RPG7: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\launchers\rpg7\rpg7.p3d";
	};
	class bldr_LAW: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\launchers\law\law.p3d";
	};
	class bldr_M203: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\launchers\m203\m203.p3d";
	};
	class bldr_SVD: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\svd\svd.p3d";
	};
	class bldr_PSO1Optic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_pso1.p3d";
	};
	class bldr_Mosin9130: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin9130.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\mosin9130\data\mosin_9130_co.paa"};
	};
	class bldr_Mosin9130_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin9130.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_SawedoffMosin9130: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\mosin9130\data\mosin_sawn_co.paa"};
	};
	class bldr_SawedoffMosin9130_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_SawedoffMosin9130_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_SawedoffMosin9130_Camo: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\mosin9130\data\mosin_nagant_camo_co.paa"};
	};
	class bldr_M4_MPBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_mp.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_M4_MPBttstck_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_mp.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_M4_RISHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_m4_mp_ris.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\DZ\weapons\attachments\data\m4_ris_co.paa"};
	};
	class bldr_M4_RISHndgrd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_m4_mp_ris.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\DZ\weapons\attachments\data\m4_ris_green_co.paa"};
	};
	class bldr_M4A1: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\m4\data\m4_body_co.paa"};
	};
	class bldr_M4_Suppressor: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\muzzle\suppressor_556.p3d";
	};
	class bldr_M68Optic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_m68_cco.p3d";
	};
	class bldr_CMAG_100Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_cmag100.p3d";
	};
	class bldr_Mag_CMAG_10Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_10.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_10Rnd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_10.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_co.paa"};
	};
	class bldr_Mag_CMAG_20Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_20.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_20Rnd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_20.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_30Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_30.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_30Rnd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_30.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_40Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_40.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_CMAG_40Rnd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pmag_40.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\pmag_black_co.paa"};
	};
	class bldr_Mag_STANAG_30Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_stanag30.p3d";
	};
	class bldr_Ammo_556x45: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\ammunition\556_LooseRounds.p3d";
	};
	class bldr_M67Grenade: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\grenade.p3d";
	};
	class bldr_FAL: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\fal\fal.p3d";
	};
	class bldr_Fal_OeBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_fal_oe.p3d";
	};
	class bldr_Fal_FoldingBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_fal_folding.p3d";
	};
	class bldr_VSS: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\VSS\VSS.p3d";
	};
	class bldr_PSO11Optic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_pso1.p3d";
	};
	class bldr_M4_PlasticHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\handguard_co.paa"};
	};
	class bldr_M4_PlasticHndgrd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\attachments\data\handguard_green_co.paa"};
	};
	class bldr_M4_MPHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_m4_mp.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_M4_MPHndgrd_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_m4_mp.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_M4_OEBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_oe.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_M4_OEBttstck_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_oe.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_M4_CQBBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_cqb.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_M4_CQBBttstck_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_m4_cqb.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_UMP45: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\UMP45\ump.p3d";
	};
	class bldr_Mp133Shotgun: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\shotguns\Mp133\mp133.p3d";
	};
	class bldr_CZ61: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\cz61\CZ61.p3d";
	};
	class bldr_Izh18: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\Izh18\Izh18_sawedoff.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\weapons\firearms\Izh18\Izh18.p3d"};
	};
	class bldr_SawedoffIzh18: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\Izh18\Izh18.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\Izh18\data\Izh18_co.paa"};
	};
	class bldr_PM73Rak: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\pm73rak\pm73rak.p3d";
	};
	class bldr_Winchester70: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\winchester70\data\winchester70_CO.paa"};
	};
	class bldr_Winchester70_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"};
	};
	class bldr_Winchester70_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_MakarovIJ70: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\pistols\pmm\ij70.p3d";
	};
	class bldr_CZ75: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\pistols\CZ75\cz75.p3d";
	};
	class bldr_Mag_FNX45_15Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_fnp45.p3d";
	};
	class bldr_Mag_PM73_15Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pm73_15rnd.p3d";
	};
	class bldr_Mag_PM73_25Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_pm73_25rnd.p3d";
	};
	class bldr_Mag_AKM_30Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_akm_30rnd.p3d";
	};
	class bldr_Mag_AKM_Drum75Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_akm_drum.p3d";
	};
	class bldr_AmmoBox_762x39_20Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\ammunition\762x39_20RoundBox.p3d";
	};
	class bldr_CZ527: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\cz527\cz527.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\cz527\data\cz527_co.paa"};
	};
	class bldr_CZ527_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\weapons\firearms\cz527\cz527.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"};
	};
	class bldr_MP5K: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\firearms\MP5\MP5K.p3d";
	};
	class bldr_MP5_Compensator: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\muzzle\MP5k_compensator.p3d";
	};
	class bldr_MP5k_StockBttstck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\buttstock_mp5K_stock.p3d";
	};
	class bldr_MP5_PlasticHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_mp5k_plastic.p3d";
	};
	class bldr_MP5_RailHndgrd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\support\handguard_mp5k_rail.p3d";
	};
	class bldr_PistolSuppressor: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\muzzle\suppressor_45acp.p3d";
	};
	class bldr_DartGun: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\nonlethal\dartgun\dartgun.p3d";
	};
	class bldr_Shockpistol: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\nonlethal\shockpistol\shockpistol.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\weapons\nonlethal\shockpistol\data\shockpistol_main_yel_co.paa"};
	};
	class bldr_Shockpistol_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\nonlethal\shockpistol\shockpistol.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\weapons\nonlethal\shockpistol\data\shockpistol_main_bk_co.paa"};
	};
	class bldr_Mag_UMP_25Rnd: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\magazine\magazine_ump45_25rnd.p3d";
	};
	class bldr_LongrangeOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\opticview_longrange.p3d";
	};
	class bldr_HuntingOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_hunting.p3d";
	};
	class bldr_PistolOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_pistol.p3d";
	};
	class bldr_KazuarOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\1PN51_opticview.p3d";
	};
	class bldr_KashtanOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_kashtan.p3d";
	};
	class bldr_KobraOptic: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\attachments\optics\optic_kobra.p3d";
	};
	class bldr_Ammo_LAW_HE: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\launchers\law\law_rocket.p3d";
	};
	class bldr_Ammo_RPG7_HE: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\ammunition\rocket_rpg7.p3d";
	};
	class bldr_RGD5Grenade: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\frag_RGD5.p3d";
	};
	class bldr_M18SmokeGrenade_Red: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_red_co.paa"};
	};
	class bldr_M18SmokeGrenade_Green: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_green_co.paa"};
	};
	class bldr_M18SmokeGrenade_Yellow: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_yellow_co.paa"};
	};
	class bldr_M18SmokeGrenade_White: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_white_co.paa"};
	};
	class bldr_RDG2SmokeGrenade_Black: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_rdg2_black_co.paa"};
	};
	class bldr_RDG2SmokeGrenade_White: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\weapons\explosives\data\smokegrenade_rdg2_white_co.paa"};
	};
	class bldr_FlashGrenade: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\explosives\flashbang.p3d";
	};
	class bldr_LandMineTrap: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\gear\traps\beartrap.p3d";
	};
	class bldr_BearTrap: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\gear\traps\beartrap.p3d";
	};
	class bldr_Ammo_762x39: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\ammunition\762x39_LooseRounds.p3d";
	};
	class bldr_Ammo_12GaBuck: HouseNoDestruct
	{
		scope = 1;
		model = "\DZ\weapons\ammunition\12ga_00buck.p3d";
	};
};
