#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Roads
	{
		units[] = {"bldr_Bridge_Concrete1_25","bldr_Bridge_Concrete1_Left","bldr_Bridge_Concrete1_Right","bldr_Bridge_Concrete2_25","bldr_Bridge_Concrete2_Left","bldr_Bridge_Concrete2_Right","bldr_Decal_asphalt_fix1","bldr_Decal_asphalt_fix2","bldr_Decal_asphalt_fix3","bldr_Decal_asphalt_fix4","bldr_Decal_asphalt_fix5","bldr_Decal_asphalt_fix6","bldr_Decal_asphalt_patch01","bldr_Decal_asphalt_patch02","bldr_Decal_asphalt_patch03","bldr_Decal_asphalt_patch04","bldr_Decal_asphalt_patch05","bldr_Decal_asphalt_patch06","bldr_Decal_asphalt_patch07","bldr_Decal_asphalt_patch08","bldr_Decal_asphalt_patch09","bldr_Decal_asphalt_patch10","bldr_Decal_asphalt_patch11","bldr_Decal_asphalt_patch12","bldr_Decal_damage_long10","bldr_Decal_damage_long6","bldr_Decal_damage_long7","bldr_Decal_damage_long8","bldr_Decal_damage_long9","bldr_Decal_damage_medium10","bldr_Decal_damage_medium4","bldr_Decal_damage_medium5","bldr_Decal_damage_medium6","bldr_Decal_damage_medium7","bldr_Decal_damage_medium8","bldr_Decal_damage_medium9","bldr_Decal_dirt_damage_long1","bldr_Decal_dirt_damage_long2","bldr_Decal_dirt_damage_long3","bldr_Decal_dirt_rocks1","bldr_Decal_dirt_rocks2","bldr_Decal_oilstain1","bldr_Decal_oilstain2","bldr_Decal_oilstain3","bldr_Decal_oilstain4","bldr_Misc_BoundaryStone_Big","bldr_Misc_BoundaryStone_Small","bldr_ConcretePanels_02_four_F","bldr_Panels_Concrete_Small","bldr_Panel_Concrete_Small_1","bldr_Panel_Concrete_Small_2","bldr_Panel_Concrete_Small_Dam","bldr_asf1enoch_0_2000","bldr_asf1enoch_1_1000","bldr_asf1enoch_10_100","bldr_asf1enoch_10_25","bldr_asf1enoch_10_50","bldr_asf1enoch_10_75","bldr_asf1enoch_12","bldr_asf1enoch_15_75","bldr_asf1enoch_22_50","bldr_asf1enoch_25","bldr_asf1enoch_30_25","bldr_asf1enoch_6","bldr_asf1enoch_60_10","bldr_asf1enoch_6konec","bldr_asf1enoch_7_100","bldr_asf2enoch_0_2000","bldr_asf2enoch_1_1000","bldr_asf2enoch_10_100","bldr_asf2enoch_10_25","bldr_asf2enoch_10_50","bldr_asf2enoch_10_75","bldr_asf2enoch_12","bldr_asf2enoch_15_75","bldr_asf2enoch_22_50","bldr_asf2enoch_25","bldr_asf2enoch_30_25","bldr_asf2enoch_6","bldr_asf2enoch_60_10","bldr_asf2enoch_6konec","bldr_asf2enoch_7_100","bldr_mudenoch_0_2000","bldr_mudenoch_1_1000","bldr_mudenoch_10_100","bldr_mudenoch_10_25","bldr_mudenoch_10_50","bldr_mudenoch_10_75","bldr_mudenoch_12","bldr_mudenoch_15_75","bldr_mudenoch_22_50","bldr_mudenoch_25","bldr_mudenoch_30_25","bldr_mudenoch_6","bldr_mudenoch_60_10","bldr_mudenoch_6konec","bldr_mudenoch_7_100","bldr_Sidewalk4_4m","bldr_Sidewalk4_8m","bldr_Sidewalk4_Border_2m","bldr_Sidewalk4_Border_4m","bldr_Sidewalk4_Border_8m","bldr_Sidewalk4_Corner","bldr_Sidewalk4_Narrow_2m","bldr_Sidewalk4_Narrow_4m","bldr_Sidewalk4_Narrow_8m","bldr_Sidewalk5_Narrow","bldr_Sidewalk5_Narrow_Corner","bldr_Sidewalk5_Wide","bldr_Sidewalk5_Wide_Corner","bldr_Sidewalk_Stairs","bldr_sinkhole_sila"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Bliss_Roads"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Bridge_Concrete1_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete1_25.p3d";
	};
	class bldr_Bridge_Concrete1_Left: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete1_Left.p3d";
	};
	class bldr_Bridge_Concrete1_Right: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete1_Right.p3d";
	};
	class bldr_Bridge_Concrete2_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete2_25.p3d";
	};
	class bldr_Bridge_Concrete2_Left: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete2_Left.p3d";
	};
	class bldr_Bridge_Concrete2_Right: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Bridges\Bridge_Concrete2_Right.p3d";
	};
	class bldr_decal: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_Decal_asphalt_fix1: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix1.p3d";
	};
	class bldr_Decal_asphalt_fix2: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix2.p3d";
	};
	class bldr_Decal_asphalt_fix3: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix3.p3d";
	};
	class bldr_Decal_asphalt_fix4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix4.p3d";
	};
	class bldr_Decal_asphalt_fix5: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix5.p3d";
	};
	class bldr_Decal_asphalt_fix6: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_fix6.p3d";
	};
	class bldr_Decal_asphalt_patch01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch01.p3d";
	};
	class bldr_Decal_asphalt_patch02: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch02.p3d";
	};
	class bldr_Decal_asphalt_patch03: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch03.p3d";
	};
	class bldr_Decal_asphalt_patch04: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch04.p3d";
	};
	class bldr_Decal_asphalt_patch05: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch05.p3d";
	};
	class bldr_Decal_asphalt_patch06: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch06.p3d";
	};
	class bldr_Decal_asphalt_patch07: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch07.p3d";
	};
	class bldr_Decal_asphalt_patch08: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch08.p3d";
	};
	class bldr_Decal_asphalt_patch09: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch09.p3d";
	};
	class bldr_Decal_asphalt_patch10: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch10.p3d";
	};
	class bldr_Decal_asphalt_patch11: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch11.p3d";
	};
	class bldr_Decal_asphalt_patch12: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_asphalt_patch12.p3d";
	};
	class bldr_Decal_damage_long10: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_long10.p3d";
	};
	class bldr_Decal_damage_long6: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_long6.p3d";
	};
	class bldr_Decal_damage_long7: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_long7.p3d";
	};
	class bldr_Decal_damage_long8: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_long8.p3d";
	};
	class bldr_Decal_damage_long9: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_long9.p3d";
	};
	class bldr_Decal_damage_medium10: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium10.p3d";
	};
	class bldr_Decal_damage_medium4: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium4.p3d";
	};
	class bldr_Decal_damage_medium5: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium5.p3d";
	};
	class bldr_Decal_damage_medium6: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium6.p3d";
	};
	class bldr_Decal_damage_medium7: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium7.p3d";
	};
	class bldr_Decal_damage_medium8: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium8.p3d";
	};
	class bldr_Decal_damage_medium9: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_damage_medium9.p3d";
	};
	class bldr_Decal_dirt_damage_long1: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_dirt_damage_long1.p3d";
	};
	class bldr_Decal_dirt_damage_long2: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_dirt_damage_long2.p3d";
	};
	class bldr_Decal_dirt_damage_long3: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_dirt_damage_long3.p3d";
	};
	class bldr_Decal_dirt_rocks1: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_dirt_rocks1.p3d";
	};
	class bldr_Decal_dirt_rocks2: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_dirt_rocks2.p3d";
	};
	class bldr_Decal_oilstain1: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_oilstain1.p3d";
	};
	class bldr_Decal_oilstain2: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_oilstain2.p3d";
	};
	class bldr_Decal_oilstain3: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_oilstain3.p3d";
	};
	class bldr_Decal_oilstain4: bldr_decal
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Decals\Decal_oilstain4.p3d";
	};
	class bldr_Misc_BoundaryStone_Big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Misc\Misc_BoundaryStone_Big.p3d";
	};
	class bldr_Misc_BoundaryStone_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Misc\Misc_BoundaryStone_Small.p3d";
	};
	class bldr_ConcretePanels_02_four_F: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Panels\ConcretePanels_02_four_F.p3d";
	};
	class bldr_Panels_Concrete_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Panels\Panels_Concrete_Small.p3d";
	};
	class bldr_Panel_Concrete_Small_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Panels\Panel_Concrete_Small_1.p3d";
	};
	class bldr_Panel_Concrete_Small_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Panels\Panel_Concrete_Small_2.p3d";
	};
	class bldr_Panel_Concrete_Small_Dam: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Panels\Panel_Concrete_Small_Dam.p3d";
	};
	class bldr_asf1enoch_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_0 2000.p3d";
	};
	class bldr_asf1enoch_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_1 1000.p3d";
	};
	class bldr_asf1enoch_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_10 100.p3d";
	};
	class bldr_asf1enoch_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_10 25.p3d";
	};
	class bldr_asf1enoch_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_10 50.p3d";
	};
	class bldr_asf1enoch_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_10 75.p3d";
	};
	class bldr_asf1enoch_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_12.p3d";
	};
	class bldr_asf1enoch_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_15 75.p3d";
	};
	class bldr_asf1enoch_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_22 50.p3d";
	};
	class bldr_asf1enoch_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_25.p3d";
	};
	class bldr_asf1enoch_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_30 25.p3d";
	};
	class bldr_asf1enoch_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_6.p3d";
	};
	class bldr_asf1enoch_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_60 10.p3d";
	};
	class bldr_asf1enoch_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_6konec.p3d";
	};
	class bldr_asf1enoch_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf1enoch_7 100.p3d";
	};
	class bldr_asf2enoch_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_0 2000.p3d";
	};
	class bldr_asf2enoch_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_1 1000.p3d";
	};
	class bldr_asf2enoch_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_10 100.p3d";
	};
	class bldr_asf2enoch_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_10 25.p3d";
	};
	class bldr_asf2enoch_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_10 50.p3d";
	};
	class bldr_asf2enoch_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_10 75.p3d";
	};
	class bldr_asf2enoch_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_12.p3d";
	};
	class bldr_asf2enoch_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_15 75.p3d";
	};
	class bldr_asf2enoch_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_22 50.p3d";
	};
	class bldr_asf2enoch_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_25.p3d";
	};
	class bldr_asf2enoch_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_30 25.p3d";
	};
	class bldr_asf2enoch_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_6.p3d";
	};
	class bldr_asf2enoch_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_60 10.p3d";
	};
	class bldr_asf2enoch_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_6konec.p3d";
	};
	class bldr_asf2enoch_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\asf2enoch_7 100.p3d";
	};
	class bldr_mudenoch_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_0 2000.p3d";
	};
	class bldr_mudenoch_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_1 1000.p3d";
	};
	class bldr_mudenoch_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_10 100.p3d";
	};
	class bldr_mudenoch_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_10 25.p3d";
	};
	class bldr_mudenoch_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_10 50.p3d";
	};
	class bldr_mudenoch_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_10 75.p3d";
	};
	class bldr_mudenoch_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_12.p3d";
	};
	class bldr_mudenoch_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_15 75.p3d";
	};
	class bldr_mudenoch_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_22 50.p3d";
	};
	class bldr_mudenoch_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_25.p3d";
	};
	class bldr_mudenoch_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_30 25.p3d";
	};
	class bldr_mudenoch_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_6.p3d";
	};
	class bldr_mudenoch_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_60 10.p3d";
	};
	class bldr_mudenoch_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_6konec.p3d";
	};
	class bldr_mudenoch_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Parts\mudenoch_7 100.p3d";
	};
	class bldr_Sidewalk4_4m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_4m.p3d";
	};
	class bldr_Sidewalk4_8m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_8m.p3d";
	};
	class bldr_Sidewalk4_Border_2m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Border_2m.p3d";
	};
	class bldr_Sidewalk4_Border_4m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Border_4m.p3d";
	};
	class bldr_Sidewalk4_Border_8m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Border_8m.p3d";
	};
	class bldr_Sidewalk4_Corner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Corner.p3d";
	};
	class bldr_Sidewalk4_Narrow_2m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Narrow_2m.p3d";
	};
	class bldr_Sidewalk4_Narrow_4m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Narrow_4m.p3d";
	};
	class bldr_Sidewalk4_Narrow_8m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk4_Narrow_8m.p3d";
	};
	class bldr_Sidewalk5_Narrow: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk5_Narrow.p3d";
	};
	class bldr_Sidewalk5_Narrow_Corner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk5_Narrow_Corner.p3d";
	};
	class bldr_Sidewalk5_Wide: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk5_Wide.p3d";
	};
	class bldr_Sidewalk5_Wide_Corner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk5_Wide_Corner.p3d";
	};
	class bldr_Sidewalk_Stairs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sidewalks\Sidewalk_Stairs.p3d";
	};
	class bldr_sinkhole_sila: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\roads\Sinkhole\sinkhole_sila.p3d";
	};
};
