#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Ruins
	{
		units[] = {"bldr_bed_large_metal","bldr_bed_large_metal_broken","bldr_bed_large_metal_crushed","bldr_carpet_big_dirty","bldr_carpet_big_dirty2","bldr_case_bedroom_a","bldr_case_bedroom_b","bldr_chair","bldr_chair_broken","bldr_chair_crushed","bldr_chair_leisure","bldr_chair_leisure_broken","bldr_chair_leisure_crushed","bldr_classroom_case_a","bldr_classroom_case_a_broken","bldr_classroom_case_a_crushed","bldr_classroom_case_b","bldr_classroom_case_b_broken","bldr_classroom_case_b_crushed","bldr_drapes_long_torn","bldr_drapes_torn","bldr_flowerpot","bldr_lobby_table","bldr_piano_broken","bldr_picture_a","bldr_picture_b","bldr_picture_c","bldr_picture_d","bldr_picture_e","bldr_picture_f","bldr_picture_g","bldr_racks","bldr_racks_broken","bldr_racks_crushed","bldr_Ruin_Church2_01_Proxy","bldr_Ruin_Church2_02_Proxy","bldr_Ruin_Church2_03_Proxy","bldr_Ruin_Church2_04_Proxy","bldr_Ruin_Church2_Proxy_overgrowth","bldr_Ruin_HouseBig1_Half_Proxy_01","bldr_Ruin_HouseBig1_Half_Proxy_overgrowth","bldr_Ruin_HouseBig1_Proxy_01","bldr_Ruin_HouseBig1_Proxy_overgrowth","bldr_Ruin_HouseBig1_Proxy_overgrowth_02","bldr_Ruin_HouseBig1_Proxy_overgrowth_02_template","bldr_Ruin_HouseBig1_Proxy_overgrowth_template","bldr_Ruin_HouseBig2_Half_Proxy_01","bldr_Ruin_HouseBig2_Half_Proxy_overgrowth","bldr_Ruin_HouseBig2_Proxy_01","bldr_Ruin_HouseBig2_Proxy_02","bldr_Ruin_HouseBig2_Proxy_overgrowth","bldr_Ruin_HouseBig3_Half_Proxy_01","bldr_Ruin_HouseBig3_Half_Proxy_overgrowth","bldr_Ruin_HouseBig3_Proxy_01","bldr_Ruin_HouseBig3_Proxy_overgrowth","bldr_Ruin_HouseSmall1_Half_Proxy_01","bldr_Ruin_HouseSmall1_Half_Proxy_overgrowth","bldr_Ruin_HouseSmall1_Proxy_01","bldr_Ruin_HouseSmall1_Proxy_02","bldr_Ruin_HouseSmall1_Proxy_overgrowth","bldr_Ruin_HouseSmall2_Proxy","bldr_Ruin_HouseSmall2_Proxy_bricks","bldr_Ruin_HouseSmall2_Proxy_overgrowth","bldr_Ruin_House_1B01_1_Proxy","bldr_Ruin_House_1B01_2_Proxy","bldr_Ruin_House_1W02_Proxy","bldr_Ruin_House_1W03_Proxy","bldr_Ruin_House_1W04_Proxy","bldr_Ruin_House_1W05_Proxy","bldr_Ruin_House_1W06_Proxy","bldr_Ruin_House_1W07_Proxy","bldr_Ruin_House_1W09_Proxy","bldr_Ruin_House_1W11_Proxy","bldr_Ruin_House_1W12_Proxy","bldr_Ruin_House_2B01_Proxy","bldr_Ruin_House_2B02_Proxy","bldr_Ruin_House_2B03_Proxy","bldr_Ruin_House_2B04_Proxy","bldr_Ruin_House_2W01_Proxy","bldr_Ruin_House_2W02_Proxy","bldr_Ruin_House_2W03_Proxy","bldr_Ruin_House_2W04_Proxy","bldr_Ruin_House_2W05_Proxy","bldr_Ruin_Mil_ControlTower_Proxy","bldr_shelfs_small","bldr_shelfs_small_broken","bldr_shelfs_small_crushed","bldr_skrin_bar","bldr_SofaCorner","bldr_table","bldr_table_broken","bldr_table_crushed","bldr_tools_racking","bldr_wardrobe_a","bldr_wardrobe_b"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Bliss_Ruins"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_bed_large_metal: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\bed_large_metal.p3d";
	};
	class bldr_bed_large_metal_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\bed_large_metal_broken.p3d";
	};
	class bldr_bed_large_metal_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\bed_large_metal_crushed.p3d";
	};
	class bldr_carpet_big_dirty: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\carpet_big_dirty.p3d";
	};
	class bldr_carpet_big_dirty2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\carpet_big_dirty2.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_case_bedroom_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\case_bedroom_a.p3d";
	};
	class bldr_case_bedroom_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\case_bedroom_b.p3d";
	};
	class bldr_chair: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair.p3d";
	};
	class bldr_chair_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair_broken.p3d";
	};
	class bldr_chair_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair_crushed.p3d";
	};
	class bldr_chair_leisure: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair_leisure.p3d";
	};
	class bldr_chair_leisure_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair_leisure_broken.p3d";
	};
	class bldr_chair_leisure_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\chair_leisure_crushed.p3d";
	};
	class bldr_classroom_case_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_a.p3d";
	};
	class bldr_classroom_case_a_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_a_broken.p3d";
	};
	class bldr_classroom_case_a_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_a_crushed.p3d";
	};
	class bldr_classroom_case_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_b.p3d";
	};
	class bldr_classroom_case_b_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_b_broken.p3d";
	};
	class bldr_classroom_case_b_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\classroom_case_b_crushed.p3d";
	};
	class bldr_drapes_long_torn: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\drapes_long_torn.p3d";
	};
	class bldr_drapes_torn: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\drapes_torn.p3d";
	};
	class bldr_flowerpot: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\flowerpot.p3d";
	};
	class bldr_lobby_table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\lobby_table.p3d";
	};
	class bldr_piano_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\piano_broken.p3d";
	};
	class bldr_picture: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_picture_a: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_a.p3d";
	};
	class bldr_picture_b: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_b.p3d";
	};
	class bldr_picture_c: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_c.p3d";
	};
	class bldr_picture_d: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_d.p3d";
	};
	class bldr_picture_e: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_e.p3d";
	};
	class bldr_picture_f: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_f.p3d";
	};
	class bldr_picture_g: bldr_picture
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\picture_g.p3d";
	};
	class bldr_racks: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\racks.p3d";
	};
	class bldr_racks_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\racks_broken.p3d";
	};
	class bldr_racks_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\racks_crushed.p3d";
	};
	class bldr_Ruin_Church2_01_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Church2_01_Proxy.p3d";
	};
	class bldr_Ruin_Church2_02_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Church2_02_Proxy.p3d";
	};
	class bldr_Ruin_Church2_03_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Church2_03_Proxy.p3d";
	};
	class bldr_Ruin_Church2_04_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Church2_04_Proxy.p3d";
	};
	class bldr_Ruin_Church2_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Church2_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig1_Half_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Half_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig1_Half_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Half_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig1_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig1_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig1_Proxy_overgrowth_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Proxy_overgrowth_02.p3d";
	};
	class bldr_Ruin_HouseBig1_Proxy_overgrowth_02_template: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Proxy_overgrowth_02_template.p3d";
	};
	class bldr_Ruin_HouseBig1_Proxy_overgrowth_template: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig1_Proxy_overgrowth_template.p3d";
	};
	class bldr_Ruin_HouseBig2_Half_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig2_Half_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig2_Half_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig2_Half_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig2_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig2_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig2_Proxy_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig2_Proxy_02.p3d";
	};
	class bldr_Ruin_HouseBig2_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig2_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig3_Half_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig3_Half_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig3_Half_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig3_Half_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseBig3_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig3_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseBig3_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseBig3_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseSmall1_Half_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall1_Half_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseSmall1_Half_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall1_Half_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseSmall1_Proxy_01: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall1_Proxy_01.p3d";
	};
	class bldr_Ruin_HouseSmall1_Proxy_02: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall1_Proxy_02.p3d";
	};
	class bldr_Ruin_HouseSmall1_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall1_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_HouseSmall2_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall2_Proxy.p3d";
	};
	class bldr_Ruin_HouseSmall2_Proxy_bricks: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall2_Proxy_bricks.p3d";
	};
	class bldr_Ruin_HouseSmall2_Proxy_overgrowth: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_HouseSmall2_Proxy_overgrowth.p3d";
	};
	class bldr_Ruin_House_1B01_1_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1B01_1_Proxy.p3d";
	};
	class bldr_Ruin_House_1B01_2_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1B01_2_Proxy.p3d";
	};
	class bldr_Ruin_House_1W02_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W02_Proxy.p3d";
	};
	class bldr_Ruin_House_1W03_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W03_Proxy.p3d";
	};
	class bldr_Ruin_House_1W04_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W04_Proxy.p3d";
	};
	class bldr_Ruin_House_1W05_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W05_Proxy.p3d";
	};
	class bldr_Ruin_House_1W06_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W06_Proxy.p3d";
	};
	class bldr_Ruin_House_1W07_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W07_Proxy.p3d";
	};
	class bldr_Ruin_House_1W09_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W09_Proxy.p3d";
	};
	class bldr_Ruin_House_1W11_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W11_Proxy.p3d";
	};
	class bldr_Ruin_House_1W12_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_1W12_Proxy.p3d";
	};
	class bldr_Ruin_House_2B01_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2B01_Proxy.p3d";
	};
	class bldr_Ruin_House_2B02_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2B02_Proxy.p3d";
	};
	class bldr_Ruin_House_2B03_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2B03_Proxy.p3d";
	};
	class bldr_Ruin_House_2B04_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2B04_Proxy.p3d";
	};
	class bldr_Ruin_House_2W01_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2W01_Proxy.p3d";
	};
	class bldr_Ruin_House_2W02_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2W02_Proxy.p3d";
	};
	class bldr_Ruin_House_2W03_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2W03_Proxy.p3d";
	};
	class bldr_Ruin_House_2W04_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2W04_Proxy.p3d";
	};
	class bldr_Ruin_House_2W05_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_House_2W05_Proxy.p3d";
	};
	class bldr_Ruin_Mil_ControlTower_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\Ruin_Mil_ControlTower_Proxy.p3d";
	};
	class bldr_shelfs_small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\shelfs_small.p3d";
	};
	class bldr_shelfs_small_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\shelfs_small_broken.p3d";
	};
	class bldr_shelfs_small_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\shelfs_small_crushed.p3d";
	};
	class bldr_skrin_bar: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\skrin_bar.p3d";
	};
	class bldr_SofaCorner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\SofaCorner.p3d";
	};
	class bldr_table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\table.p3d";
	};
	class bldr_table_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\table_broken.p3d";
	};
	class bldr_table_crushed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\table_crushed.p3d";
	};
	class bldr_tools_racking: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\tools_racking.p3d";
	};
	class bldr_wardrobe_a: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\wardrobe_a.p3d";
	};
	class bldr_wardrobe_b: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\ruins\Proxy\wardrobe_b.p3d";
	};
};
