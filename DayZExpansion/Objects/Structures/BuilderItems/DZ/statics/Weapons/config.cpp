class CfgPatches
{
	class BuilderItems_StaticItems_Weapons
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Structures_Signs"
		};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_akm: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\AKM\akm.p3d";
	};
	class bldr_AK_RailHndgrd_Black: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_rail.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\DZ\weapons\attachments\data\akm_accessories_black_co.paa"
		};
	};
	class bldr_AK_RailHndgrd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_rail.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\DZ\weapons\attachments\data\akm_accessories_green_co.paa"
		};
	};
	class bldr_UniversalLight: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\light\weaponlight_universal.p3d";
	};
	class bldr_AK_Suppressor: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\muzzle\suppressor_545.p3d";
	};
	class bldr_Mag_AKM_Palm30Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_akm_palm30.p3d";
	};
	class bldr_ReflexOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_reflex.p3d";
	};
	class bldr_AK_FoldingBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_ak_folding.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_accessories_black_co.paa"
		};
	};
	class bldr_AK_FoldingBttstck_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_ak_folding.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_accessories_green_co.paa"
		};
	};
	class bldr_Crossbow_RedpointOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_crossbow.p3d";
	};
	class bldr_Crossbow_Base: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\archery\crossbow\crossbow.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_leaves_co.paa"
		};
	};
	class bldr_Crossbow_Black: bldr_Crossbow_Base
	{
		scope=1;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_black_co.paa"
		};
	};
	class bldr_Crossbow_Green: bldr_Crossbow_Base
	{
		scope=1;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_green_co.paa"
		};
	};
	class bldr_Crossbow_Wood: bldr_Crossbow_Base
	{
		scope=1;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_wood_co.paa"
		};
	};
	class bldr_Crossbow_Summer: bldr_Crossbow_Base
	{
		scope=1;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_summer_co.paa"
		};
	};
	class bldr_Crossbow_Camo: bldr_Crossbow_Base
	{
		scope=1;
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\archery\crossbow\data\crossbow_leaves_co.paa"
		};
	};
	class bldr_RecurveBow: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\archery\bow_recurve\bow_recurve.p3d";
	};
	class bldr_AK_WoodHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_wood_co.paa"
		};
	};
	class bldr_AK74_Hndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\ak74_wood_co.paa"
		};
	};
	class bldr_AK_PlasticHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_plastic.p3d";
	};
	class bldr_AK_WoodBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_wood_co.paa"
		};
	};
	class bldr_AK74_WoodBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\ak74_wood_co.paa"
		};
	};
	class bldr_AK74_WoodBttstck_Camo: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_wood_camo_co.paa"
		};
	};
	class bldr_AK_WoodHndgrd_Camo: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_ak_wood.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\akm_wood_camo_co.paa"
		};
	};
	class bldr_AK_PlasticBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_ak_plastic.p3d";
	};
	class bldr_AK101: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\AK101\ak101.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\AK101\data\ak101_co.paa"
		};
	};
	class bldr_AK101_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\AK101\ak101.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_AKS74U_Bttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_aks.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\AK74\data\aks74u_co.paa"
		};
	};
	class bldr_AKS74U_Bttstck_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_aks.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.37,0.28,1.0,CO)"
		};
	};
	class bldr_SKS: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\SKS\data\sks_co.paa"
		};
	};
	class bldr_SKS_Black: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\SKS\data\sks_black_co.paa"
		};
	};
	class bldr_SKS_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\SKS\SKS.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\SKS\data\sks_green_co.paa"
		};
	};
	class bldr_RPG7: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\launchers\rpg7\rpg7.p3d";
	};
	class bldr_LAW: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\launchers\law\law.p3d";
	};
	class bldr_M203: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\launchers\m203\m203.p3d";
	};
	class bldr_SVD: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\svd\svd.p3d";
	};
	class bldr_PSO1Optic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_pso1.p3d";
	};
	class bldr_Mosin9130: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin9130.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\mosin9130\data\mosin_9130_co.paa"
		};
	};
	class bldr_Mosin9130_Black: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin9130.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_SawedoffMosin9130: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\mosin9130\data\mosin_sawn_co.paa"
		};
	};
	class bldr_SawedoffMosin9130_Black: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_SawedoffMosin9130_Green: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_SawedoffMosin9130_Camo: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mosin9130\mosin_sawn.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\mosin9130\data\mosin_nagant_camo_co.paa"
		};
	};
	class bldr_M4_MPBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_mp.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_M4_MPBttstck_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_mp.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_M4_RISHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_m4_mp_ris.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\DZ\weapons\attachments\data\m4_ris_co.paa"
		};
	};
	class bldr_M4_RISHndgrd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_m4_mp_ris.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\DZ\weapons\attachments\data\m4_ris_green_co.paa"
		};
	};
	class bldr_M4A1: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\m4\data\m4_body_co.paa"
		};
	};
	class bldr_M4_Suppressor: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\muzzle\suppressor_556.p3d";
	};
	class bldr_M68Optic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_m68_cco.p3d";
	};
	class bldr_CMAG_100Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_cmag100.p3d";
	};
	class bldr_Mag_CMAG_10Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_10.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_10Rnd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_10.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_co.paa"
		};
	};
	class bldr_Mag_CMAG_20Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_20.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_20Rnd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_20.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_30Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_30.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_30Rnd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_30.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_40Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_40.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_CMAG_40Rnd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pmag_40.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\pmag_black_co.paa"
		};
	};
	class bldr_Mag_STANAG_30Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_stanag30.p3d";
	};
	class bldr_Ammo_556x45: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\ammunition\556_LooseRounds.p3d";
	};
	class bldr_M67Grenade: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\grenade.p3d";
	};
	class bldr_FAL: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\fal\fal.p3d";
	};
	class bldr_Fal_OeBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_fal_oe.p3d";
	};
	class bldr_Fal_FoldingBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_fal_folding.p3d";
	};
	class bldr_VSS: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\VSS\VSS.p3d";
	};
	class bldr_PSO11Optic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_pso1.p3d";
	};
	class bldr_M4_PlasticHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\handguard_co.paa"
		};
	};
	class bldr_M4_PlasticHndgrd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\m4\m4a1.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\attachments\data\handguard_green_co.paa"
		};
	};
	class bldr_M4_MPHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_m4_mp.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_M4_MPHndgrd_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_m4_mp.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_M4_OEBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_oe.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_M4_OEBttstck_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_oe.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_M4_CQBBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_cqb.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_M4_CQBBttstck_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_m4_cqb.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_UMP45: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\UMP45\ump.p3d";
	};
	class bldr_Mp133Shotgun: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\shotguns\Mp133\mp133.p3d";
	};
	class bldr_CZ61: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\cz61\CZ61.p3d";
	};
	class bldr_Izh18: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\Izh18\Izh18_sawedoff.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\dz\weapons\firearms\Izh18\Izh18.p3d"
		};
	};
	class bldr_SawedoffIzh18: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\Izh18\Izh18.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\Izh18\data\Izh18_co.paa"
		};
	};
	class bldr_PM73Rak: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\pm73rak\pm73rak.p3d";
	};
	class bldr_Winchester70: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\winchester70\data\winchester70_CO.paa"
		};
	};
	class bldr_Winchester70_Black: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.15,0.15,0.15,1.0,CO)"
		};
	};
	class bldr_Winchester70_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\winchester70\winchester70.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_MakarovIJ70: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\pistols\pmm\ij70.p3d";
	};
	class bldr_CZ75: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\pistols\CZ75\cz75.p3d";
	};
	class bldr_Mag_FNX45_15Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_fnp45.p3d";
	};
	class bldr_Mag_PM73_15Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pm73_15rnd.p3d";
	};
	class bldr_Mag_PM73_25Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_pm73_25rnd.p3d";
	};
	class bldr_Mag_AKM_30Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_akm_30rnd.p3d";
	};
	class bldr_Mag_AKM_Drum75Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_akm_drum.p3d";
	};
	class bldr_AmmoBox_762x39_20Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\ammunition\762x39_20RoundBox.p3d";
	};
	class bldr_CZ527: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\cz527\cz527.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\firearms\cz527\data\cz527_co.paa"
		};
	};
	class bldr_CZ527_Black: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\cz527\cz527.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.35,0.36,0.28,1.0,CO)"
		};
	};
	class bldr_MP5K: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\firearms\MP5\MP5K.p3d";
	};
	class bldr_MP5_Compensator: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\muzzle\MP5k_compensator.p3d";
	};
	class bldr_MP5k_StockBttstck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\buttstock_mp5K_stock.p3d";
	};
	class bldr_MP5_PlasticHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_mp5k_plastic.p3d";
	};
	class bldr_MP5_RailHndgrd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\support\handguard_mp5k_rail.p3d";
	};
	class bldr_PistolSuppressor: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\muzzle\suppressor_45acp.p3d";
	};
	class bldr_DartGun: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\nonlethal\dartgun\dartgun.p3d";
	};
	class bldr_Shockpistol: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\nonlethal\shockpistol\shockpistol.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\dz\weapons\nonlethal\shockpistol\data\shockpistol_main_yel_co.paa"
		};
	};
	class bldr_Shockpistol_Black: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\nonlethal\shockpistol\shockpistol.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"\dz\weapons\nonlethal\shockpistol\data\shockpistol_main_bk_co.paa"
		};
	};
	class bldr_Mag_UMP_25Rnd: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\magazine\magazine_ump45_25rnd.p3d";
	};
	class bldr_LongrangeOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\opticview_longrange.p3d";
	};
	class bldr_HuntingOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_hunting.p3d";
	};
	class bldr_PistolOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_pistol.p3d";
	};
	class bldr_KazuarOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\1PN51_opticview.p3d";
	};
	class bldr_KashtanOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_kashtan.p3d";
	};
	class bldr_KobraOptic: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\attachments\optics\optic_kobra.p3d";
	};
	class bldr_Ammo_LAW_HE: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\launchers\law\law_rocket.p3d";
	};
	class bldr_Ammo_RPG7_HE: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\ammunition\rocket_rpg7.p3d";
	};
	class bldr_RGD5Grenade: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\frag_RGD5.p3d";
	};
	class bldr_M18SmokeGrenade_Red: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_red_co.paa"
		};
	};
	class bldr_M18SmokeGrenade_Green: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_green_co.paa"
		};
	};
	class bldr_M18SmokeGrenade_Yellow: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_yellow_co.paa"
		};
	};
	class bldr_M18SmokeGrenade_White: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_white_co.paa"
		};
	};
	class bldr_RDG2SmokeGrenade_Black: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_rdg2_black_co.paa"
		};
	};
	class bldr_RDG2SmokeGrenade_White: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\smokegrenade.p3d";
		hiddenSelections[]=
		{
			"camoGround"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\weapons\explosives\data\smokegrenade_rdg2_white_co.paa"
		};
	};
	class bldr_FlashGrenade: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\explosives\flashbang.p3d";
	};
	class bldr_LandMineTrap: HouseNoDestruct
	{
		scope=1;
		model="\DZ\gear\traps\beartrap.p3d";
	};
	class bldr_BearTrap: HouseNoDestruct
	{
		scope=1;
		model="\DZ\gear\traps\beartrap.p3d";
	};
	class bldr_Ammo_762x39: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\ammunition\762x39_LooseRounds.p3d";
	};
	class bldr_Ammo_12GaBuck: HouseNoDestruct
	{
		scope=1;
		model="\DZ\weapons\ammunition\12ga_00buck.p3d";
	};
	class bldr_00buck_10roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\00buck_10roundbox.p3d";
	};
	class bldr_12ga_beanbag: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\12ga_beanbag.p3d";
	};
	class bldr_12ga_rifled_slug: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\12ga_rifled_slug.p3d";
	};
	class bldr_12ga_rubberrifled_slug: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\12ga_rubberrifled_slug.p3d";
	};
	class bldr_22_50roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\22_50roundbox.p3d";
	};
	class bldr_22_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\22_looserounds.p3d";
	};
	class bldr_22lr_looseround: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\22lr_looseround.p3d";
	};
	class bldr_308win_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\308win_20roundbox.p3d";
	};
	class bldr_308win_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\308win_looserounds.p3d";
	};
	class bldr_357_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\357_20roundbox.p3d";
	};
	class bldr_357_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\357_looserounds.p3d";
	};
	class bldr_380auto_35roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\380auto_35roundbox.p3d";
	};
	class bldr_380auto_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\380auto_looserounds.p3d";
	};
	class bldr_40mm_grenade: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade.p3d";
	};
	class bldr_40mm_grenade_chemgas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade_chemgas.p3d";
	};
	class bldr_40mm_grenade_smoke_b: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade_smoke_b.p3d";
	};
	class bldr_40mm_grenade_smoke_g: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade_smoke_g.p3d";
	};
	class bldr_40mm_grenade_smoke_r: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade_smoke_r.p3d";
	};
	class bldr_40mm_grenade_smoke_w: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\40mm_grenade_smoke_w.p3d";
	};
	class bldr_45acp_25rnd_box: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\45acp_25rnd_box.p3d";
	};
	class bldr_45acp_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\45acp_looserounds.p3d";
	};
	class bldr_545x39_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\545x39_20roundbox.p3d";
	};
	class bldr_545x39_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\545x39_looserounds.p3d";
	};
	class bldr_556_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\556_20roundbox.p3d";
	};
	class bldr_762_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\762_20roundbox.p3d";
	};
	class bldr_762_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\762_looserounds.p3d";
	};
	class bldr_762x39_box: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\762x39_box.p3d";
	};
	class bldr_9mm_25rnd_box: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\9mm_25rnd_box.p3d";
	};
	class bldr_9mm_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\9mm_looserounds.p3d";
	};
	class bldr_9x39_20roundbox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\9x39_20roundbox.p3d";
	};
	class bldr_9x39_looserounds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\9x39_looserounds.p3d";
	};
	class bldr_flare_singleround_blue: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\flare_singleround_blue.p3d";
	};
	class bldr_flare_singleround_green: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\flare_singleround_green.p3d";
	};
	class bldr_flare_singleround_red: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\flare_singleround_red.p3d";
	};
	class bldr_vog25: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\ammunition\vog25.p3d";
	};
	class bldr_bow_pvc: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\archery\bow_pvc\bow_pvc.p3d";
	};
	class bldr_bow_quickie: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\archery\bow_quickie\bow_quickie.p3d";
	};
	class bldr_weaponlight_peq2: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\light\weaponlight_peq2.p3d";
	};
	class bldr_weaponlight_tlr: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\light\weaponlight_tlr.p3d";
	};
	class bldr_357_speedloader: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\357_speedloader.p3d";
	};
	class bldr_clip_762_5rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\clip_762_5rnd.p3d";
	};
	class bldr_clip_762x39_10rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\clip_762x39_10rnd.p3d";
	};
	class bldr_clip_9mm_10rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\clip_9mm_10rnd.p3d";
	};
	class bldr_magazine_1911: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_1911.p3d";
	};
	class bldr_magazine_ak101_30rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ak101_30rnd.p3d";
	};
	class bldr_magazine_ak74_30rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ak74_30rnd.p3d";
	};
	class bldr_magazine_ak74_45rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ak74_45rnd.p3d";
	};
	class bldr_magazine_ammobox_pkm: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ammobox_pkm.p3d";
	};
	class bldr_magazine_aug30: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_aug30.p3d";
	};
	class bldr_magazine_cz527: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_cz527.p3d";
	};
	class bldr_magazine_cz550_10rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_cz550_10rnd.p3d";
	};
	class bldr_magazine_cz550_4rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_cz550_4rnd.p3d";
	};
	class bldr_magazine_cz61: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_cz61.p3d";
	};
	class bldr_magazine_cz75: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_cz75.p3d";
	};
	class bldr_magazine_dartgun_co2: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_dartgun_co2.p3d";
	};
	class bldr_magazine_de: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_de.p3d";
	};
	class bldr_magazine_fal: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_fal.p3d";
	};
	class bldr_magazine_famas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_famas.p3d";
	};
	class bldr_magazine_glock19: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_glock19.p3d";
	};
	class bldr_magazine_ij70: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ij70.p3d";
	};
	class bldr_magazine_m14_10rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_m14_10rnd.p3d";
	};
	class bldr_magazine_m14_20rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_m14_20rnd.p3d";
	};
	class bldr_magazine_m249_ammobox: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_m249_ammobox.p3d";
	};
	class bldr_magazine_mk2: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_mk2.p3d";
	};
	class bldr_magazine_mp5_15rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_mp5_15rnd.p3d";
	};
	class bldr_magazine_mp5_30rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_mp5_30rnd.p3d";
	};
	class bldr_magazine_ots14: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ots14.p3d";
	};
	class bldr_magazine_p1: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_p1.p3d";
	};
	class bldr_magazine_pb6p9: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_pb6p9.p3d";
	};
	class bldr_magazine_pp19: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_pp19.p3d";
	};
	class bldr_magazine_quiver: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_quiver.p3d";
	};
	class bldr_magazine_ruger1022_10: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ruger1022_10.p3d";
	};
	class bldr_magazine_ruger1022_15: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ruger1022_15.p3d";
	};
	class bldr_magazine_ruger1022_30: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ruger1022_30.p3d";
	};
	class bldr_magazine_saiga_drum: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_saiga_drum.p3d";
	};
	class bldr_magazine_saiga_mag5: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_saiga_mag5.p3d";
	};
	class bldr_magazine_saiga_mag8: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_saiga_mag8.p3d";
	};
	class bldr_magazine_scout: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_scout.p3d";
	};
	class bldr_magazine_ssg82: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_ssg82.p3d";
	};
	class bldr_magazine_stanag30_coupled: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_stanag30_coupled.p3d";
	};
	class bldr_magazine_stanag60: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_stanag60.p3d";
	};
	class bldr_magazine_sv98_10rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_sv98_10rnd.p3d";
	};
	class bldr_magazine_svd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_svd.p3d";
	};
	class bldr_magazine_uzi_10: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_uzi_10.p3d";
	};
	class bldr_magazine_uzi_22: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_uzi_22.p3d";
	};
	class bldr_magazine_val: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_val.p3d";
	};
	class bldr_magazine_vikhr_30rnd: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_vikhr_30rnd.p3d";
	};
	class bldr_magazine_vss: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\magazine_vss.p3d";
	};
	class bldr_shockpistol_battery: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\shockpistol_battery.p3d";
	};
	class bldr_shockpistol_cartridge: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\shockpistol_cartridge.p3d";
	};
	class bldr_snaploader: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\snaploader.p3d";
	};
	class bldr_snaploader_762: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\snaploader_762.p3d";
	};
	class bldr_snaploader_empty: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\magazine\snaploader_empty.p3d";
	};
	class bldr_bayonet_ak: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\bayonet_ak.p3d";
	};
	class bldr_bayonet_m9a1: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\bayonet_m9a1.p3d";
	};
	class bldr_bayonet_mosin: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\bayonet_mosin.p3d";
	};
	class bldr_bayonet_sks: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\bayonet_sks.p3d";
	};
	class bldr_bottle_suppressor: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\bottle_suppressor.p3d";
	};
	class bldr_mosin_compensator: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\mosin_compensator.p3d";
	};
	class bldr_suppressor_pb6p9: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\muzzle\suppressor_pb6p9.p3d";
	};
	class bldr_bow_sights: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\bow_sights.p3d";
	};
	class bldr_optic_1pn51: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_1pn51.p3d";
	};
	class bldr_optic_acog: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_acog.p3d";
	};
	class bldr_optic_acog6x: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_acog6x.p3d";
	};
	class bldr_optic_buis: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_buis.p3d";
	};
	class bldr_optic_fnp45_mrds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_fnp45_mrds.p3d";
	};
	class bldr_optic_holo_sight: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_holo_sight.p3d";
	};
	class bldr_optic_longrange: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_longrange.p3d";
	};
	class bldr_optic_m4_carryhandle: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_m4_carryhandle.p3d";
	};
	class bldr_optic_mark4: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_mark4.p3d";
	};
	class bldr_optic_ots14: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_ots14.p3d";
	};
	class bldr_optic_pso6: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_pso6.p3d";
	};
	class bldr_optic_puscope: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_puscope.p3d";
	};
	class bldr_optic_red_point: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_red_point.p3d";
	};
	class bldr_optic_rpg7: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_rpg7.p3d";
	};
	class bldr_optic_sporting: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_sporting.p3d";
	};
	class bldr_optic_starlight: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_starlight.p3d";
	};
	class bldr_optic_t3n_rds: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\optics\optic_t3n_rds.p3d";
	};
	class bldr_bipod_atlas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\bipod_atlas.p3d";
	};
	class bldr_bipod_m249: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\bipod_m249.p3d";
	};
	class bldr_bipod_rpg7: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\bipod_rpg7.p3d";
	};
	class bldr_buttstock_ak_wood: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_ak_wood.p3d";
	};
	class bldr_buttstock_m249: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_m249.p3d";
	};
	class bldr_buttstock_pkm: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_pkm.p3d";
	};
	class bldr_buttstock_pp19: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_pp19.p3d";
	};
	class bldr_buttstock_red9: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_red9.p3d";
	};
	class bldr_buttstock_saiga_folding: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\buttstock_saiga_folding.p3d";
	};
	class bldr_ghillie_wrap: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\ghillie_wrap.p3d";
	};
	class bldr_grip_m4_mp: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\grip_m4_mp.p3d";
	};
	class bldr_handguard_m249: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\handguard_m249.p3d";
	};
	class bldr_handguard_m249_ris: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\handguard_m249_ris.p3d";
	};
	class bldr_handguard_m4_plastic: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\handguard_m4_plastic.p3d";
	};
	class bldr_handguard_m4_ris: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\handguard_m4_ris.p3d";
	};
	class bldr_mosin_wrap: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\mosin_wrap.p3d";
	};
	class bldr_mosin_wrap_grass: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\mosin_wrap_grass.p3d";
	};
	class bldr_universal_rail: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\universal_rail.p3d";
	};
	class bldr_wrap_burlap: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\wrap_burlap.p3d";
	};
	class bldr_wrap_burlap_grass: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\support\wrap_burlap_grass.p3d";
	};
	class bldr_m203leafsight: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\underslung\m203leafsight.p3d";
	};
	class bldr_ubgl_gp25: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\attachments\underslung\ubgl_gp25.p3d";
	};
	class bldr_claymoremine: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\claymoremine.p3d";
	};
	class bldr_claymoremineplacing: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\claymoremineplacing.p3d";
	};
	class bldr_grenade_chemgas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\grenade_chemgas.p3d";
	};
	class bldr_improvisedexplosive: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\improvisedexplosive.p3d";
	};
	class bldr_improvisedexplosiveplacing: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\improvisedexplosiveplacing.p3d";
	};
	class bldr_plastic_explosive: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\plastic_explosive.p3d";
	};
	class bldr_smokegrenade_rdg2: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\explosives\smokegrenade_rdg2.p3d";
	};
	class bldr_aks74u: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ak74\aks74u.p3d";
	};
	class bldr_aug: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\aug\aug.p3d";
	};
	class bldr_aug_short: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\aug\aug_short.p3d";
	};
	class bldr_b95: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\b95\b95.p3d";
	};
	class bldr_b95_sawn: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\b95\b95_sawn.p3d";
	};
	class bldr_cz550: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\cz550\cz550.p3d";
	};
	class bldr_famas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\famas\famas.p3d";
	};
	class bldr_sawedofffamas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\famas\sawedofffamas.p3d";
	};
	class bldr_m14: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\m14\m14.p3d";
	};
	class bldr_m16a2: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\m16a2\m16a2.p3d";
	};
	class bldr_m249: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\m249\m249.p3d";
	};
	class bldr_mini_uzi: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\mini_uzi\mini_uzi.p3d";
	};
	class bldr_ots14: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14.p3d";
	};
	class bldr_ots14_barrel: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14_barrel.p3d";
	};
	class bldr_ots14_barrel_grip: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14_barrel_grip.p3d";
	};
	class bldr_ots14_grip: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14_grip.p3d";
	};
	class bldr_ots14_launcher: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14_launcher.p3d";
	};
	class bldr_ots14_silencer: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ots14\ots14_silencer.p3d";
	};
	class bldr_pkm: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\pkm\pkm.p3d";
	};
	class bldr_pp19: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\pp19\pp19.p3d";
	};
	class bldr_repeater: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\repeater\repeater.p3d";
	};
	class bldr_ruger1022: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ruger1022\ruger1022.p3d";
	};
	class bldr_scout: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\scout\scout.p3d";
	};
	class bldr_speargun: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\speargun\speargun.p3d";
	};
	class bldr_ssg82: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\ssg82\ssg82.p3d";
	};
	class bldr_sv98: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\sv98\sv98.p3d";
	};
	class bldr_svd_old: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\svd\svd_old.p3d";
	};
	class bldr_trumpet: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\trumpet\trumpet.p3d";
	};
	class bldr_trumpet_parts: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\trumpet\trumpet_parts.p3d";
	};
	class bldr_val: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\vss\val.p3d";
	};
	class bldr_vikhr: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\firearms\vss\vikhr.p3d";
	};
	class bldr_m79: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\launchers\m79\m79.p3d";
	};
	class bldr_combatknife: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\combatknife.p3d";
	};
	class bldr_fange_knife: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\fange_knife.p3d";
	};
	class bldr_firefighteraxe: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\firefighteraxe.p3d";
	};
	class bldr_hatchet: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\hatchet.p3d";
	};
	class bldr_hay_fork: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\hay_fork.p3d";
	};
	class bldr_hunting_knife: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\hunting_knife.p3d";
	};
	class bldr_kukri_knife: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\kukri_knife.p3d";
	};
	class bldr_machete: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\machete.p3d";
	};
	class bldr_machete_oriental: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\machete_oriental.p3d";
	};
	class bldr_machete_tactical: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\machete_tactical.p3d";
	};
	class bldr_meat_cleaver: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\meat_cleaver.p3d";
	};
	class bldr_medieval_sword: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\medieval_sword.p3d";
	};
	class bldr_pickaxe: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\pickaxe.p3d";
	};
	class bldr_woodaxe: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blade\woodaxe.p3d";
	};
	class bldr_baseballbat: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\baseballbat.p3d";
	};
	class bldr_baseballbat_barbed: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\baseballbat_barbed.p3d";
	};
	class bldr_baseballbat_nailed: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\baseballbat_nailed.p3d";
	};
	class bldr_baseballbat_xmas: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\baseballbat_xmas.p3d";
	};
	class bldr_brassknuckles: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\brassknuckles.p3d";
	};
	class bldr_hockey_stick: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\hockey_stick.p3d";
	};
	class bldr_hockey_stick_peters: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\hockey_stick_peters.p3d";
	};
	class bldr_pipewrench: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\pipewrench.p3d";
	};
	class bldr_police_baton: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\police_baton.p3d";
	};
	class bldr_telescopingbaton: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\telescopingbaton.p3d";
	};
	class bldr_telescopingbaton_retracted: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\blunt\telescopingbaton_retracted.p3d";
	};
	class bldr_chainsaw: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\powered\chainsaw.p3d";
	};
	class bldr_stun_baton: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\melee\powered\stun_baton.p3d";
	};
	class bldr_1911: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\1911\1911.p3d";
	};
	class bldr_de: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\de\de.p3d";
	};
	class bldr_derringer: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\derringer\derringer.p3d";
	};
	class bldr_flaregun: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\flaregun\flaregun.p3d";
	};
	class bldr_fnp45: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\fnx45\fnp45.p3d";
	};
	class bldr_glock19: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\glock\glock19.p3d";
	};
	class bldr_longhorn: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\longhorn\longhorn.p3d";
	};
	class bldr_magnum: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\magnum\magnum.p3d";
	};
	class bldr_magnum_sawn: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\magnum\magnum_sawn.p3d";
	};
	class bldr_rugermkii: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\mkii\rugermkii.p3d";
	};
	class bldr_p1: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\p1\p1.p3d";
	};
	class bldr_pb6p9: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\pb6p9\pb6p9.p3d";
	};
	class bldr_red9: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\pistols\red9\red9.p3d";
	};
	class bldr_arrow_composite: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\arrow_composite.p3d";
	};
	class bldr_arrow_crafted_simple: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\arrow_crafted_simple.p3d";
	};
	class bldr_arrow_crude_simple: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\arrow_crude_simple.p3d";
	};
	class bldr_bolt_biggame: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_biggame.p3d";
	};
	class bldr_bolt_crafted: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_crafted.p3d";
	};
	class bldr_bolt_crude: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_crude.p3d";
	};
	class bldr_bolt_cupids: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_cupids.p3d";
	};
	class bldr_bolt_smallgame: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\projectiles\bolt_smallgame.p3d";
	};
	class bldr_izh18shotgun: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\izh18shotgun\izh18shotgun.p3d";
	};
	class bldr_izh18shotgun_sawedoff: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\izh18shotgun\izh18shotgun_sawedoff.p3d";
	};
	class bldr_izh43: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\izh43\izh43.p3d";
	};
	class bldr_izh43_sawedoff: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\izh43\izh43_sawedoff.p3d";
	};
	class bldr_mp133_pistol_grip: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\mp133\mp133_pistol_grip.p3d";
	};
	class bldr_r12: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\remington_r12\r12.p3d";
	};
	class bldr_saiga: HouseNoDestruct
	{
		scope=1;
		model="\dz\weapons\shotguns\saiga\saiga.p3d";
	};
};
