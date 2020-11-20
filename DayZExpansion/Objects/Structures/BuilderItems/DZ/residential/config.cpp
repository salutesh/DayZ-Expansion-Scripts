#define _ARMA_

class CfgPatches
{
	class BuilderItems_Residential
	{
		units[] = {"bldr_BusStation_corner","bldr_BusStation_roof_big","bldr_BusStation_roof_long","bldr_BusStation_wall","bldr_BusStation_wall_bench","bldr_BusStop_City_Sign","bldr_BusStop_Village","bldr_BusStop_Village_Sign","bldr_City_FireStation_hoses","bldr_City_FireStation_Int","bldr_windows","bldr_Misc_DogHouse","bldr_Lamp_City1","bldr_Lamp_City2","bldr_Lamp_City3","bldr_Misc_AdvertColumn","bldr_Misc_Barricade","bldr_Misc_Bench1","bldr_Misc_Bench2","bldr_Misc_Bench3","bldr_Misc_Bench4","bldr_Misc_Bench5","bldr_Misc_Boogieman","bldr_Misc_BoundaryStone","bldr_Misc_BoundaryStone_Plastic","bldr_Misc_BoundaryStone_Tube","bldr_Misc_BoxWooden","bldr_Misc_Carousel","bldr_Misc_Carousel_Small","bldr_Misc_Chair_Camp1","bldr_Misc_Chair_Camp2","bldr_Misc_ChickenCoop","bldr_Misc_CrashBarrier","bldr_Misc_FeedRack","bldr_Misc_Football_Gate","bldr_Misc_Football_Line","bldr_Misc_Football_Line_Corner","bldr_Misc_Football_Line_HCircle","bldr_Misc_Fountain","bldr_Misc_HayStack","bldr_Misc_Hutch","bldr_Misc_NoticeBoard1","bldr_Misc_NoticeBoard2","bldr_Misc_PostBox1","bldr_Misc_PostBox2","bldr_Misc_RoadBarrier","bldr_Misc_Sandbox","bldr_Misc_Seesaw","bldr_Misc_StadiumBenches","bldr_Misc_Sunshade","bldr_Misc_Table","bldr_Misc_Table_Camp","bldr_Misc_Table_Camp_Small","bldr_Misc_Toilet_Dry","bldr_Misc_TrafficLights","bldr_Misc_TrailRoof_Big","bldr_Misc_TrailRoof_Small","bldr_Misc_Tree_Pavement1","bldr_Misc_Tree_Pavement2","bldr_Misc_Tree_Pavement3","bldr_Misc_Tree_Protector","bldr_Misc_Well","bldr_Misc_WindBreak","bldr_Misc_WoodBlock","bldr_Misc_WoodPile1","bldr_Misc_WoodPile2","bldr_Misc_WoodReserve","bldr_Misc_WoodTable_Indoor","bldr_Misc_WoodTable_Outdoor","bldr_Office_Municipal2_Archway","bldr_A_Office01_metalbars","bldr_A_Office02_grnd_proxy","bldr_A_Office02_metalbars","bldr_A_Office02_windows","bldr_Black","bldr_DoorGlass","bldr_DoorInterier","bldr_DoorInterier_Closed","bldr_DoorSmallGlass","bldr_WindowHall","bldr_WindowLarge","bldr_WindowSmall","bldr_Window_A","bldr_Window_A_broken","bldr_Window_B","bldr_Window_B_broken","bldr_Zarivky","bldr_Platform1_Angle","bldr_Platform1_Block","bldr_Platform1_Ramp","bldr_Platform1_Stairs_20","bldr_Platform1_Stairs_30","bldr_Platform1_Stairs_30_WallL","bldr_Platform1_Stairs_30_WallR","bldr_Platform1_Staris_Block","bldr_Platform1_Wall","bldr_Platform2_Angle","bldr_Platform2_Block","bldr_Platform2_Enter","bldr_Platform2_Stairs_20","bldr_Platform2_Stairs_30","bldr_Platform2_Stairs_30_WallL","bldr_Platform2_Stairs_30_WallR","bldr_Platform2_Staris_Block","bldr_Platform2_Wall","bldr_school_details","bldr_school_exterior_proxy","bldr_school_interior","bldr_school_sign","bldr_Tenement_Small_Entry","bldr_Balcony_Rail","bldr_TB_Big_Doorricade","bldr_TB_Big_DoorsFrame_extract","bldr_TB_Big_HandRail","bldr_TB_Big_Ladders","bldr_TB_Big_Roof_Items","bldr_TB_Doors_A","bldr_TB_Doors_B","bldr_TB_Doors_C","bldr_TB_Elevator_Stairs","bldr_TB_Elevator_winch","bldr_TB_Lobby","bldr_TB_small_barrier_proxy","bldr_TB_small_doors_proxy","bldr_TB_Small_Roof_Items","bldr_TB_Windows","bldr_Tenement_Big_win","bldr_Tenement_Big_win_Single","bldr_Tenement_small_win_Back","bldr_Tenement_small_win_Front"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Residential"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_BusStation_corner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstation_corner.p3d";
	};
	class bldr_BusStation_roof_big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstation_roof_big.p3d";
	};
	class bldr_BusStation_roof_long: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstation_roof_long.p3d";
	};
	class bldr_BusStation_wall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstation_wall.p3d";
	};
	class bldr_BusStation_wall_bench: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstation_wall_bench.p3d";
	};
	class bldr_BusStop_City_Sign: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstop_city_sign.p3d";
	};
	class bldr_BusStop_Village: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstop_village.p3d";
	};
	class bldr_BusStop_Village_Sign: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\bus\busstop_village_sign.p3d";
	};
	class bldr_City_FireStation_hoses: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\firedep\proxy\city_firestation_hoses.p3d";
	};
	class bldr_City_FireStation_Int: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\firedep\proxy\city_firestation_int.p3d";
	};
	class bldr_windows: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\firedep\proxy\windows.p3d";
	};
	class bldr_Misc_DogHouse: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\Residential\Misc\Misc_DogHouse.p3d";
	};
	class bldr_Lamp_City1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\lamp_city1.p3d";
	};
	class bldr_Lamp_City2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\lamp_city2.p3d";
	};
	class bldr_Lamp_City3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\lamp_city3.p3d";
	};
	class bldr_Misc_AdvertColumn: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_advertcolumn.p3d";
	};
	class bldr_Misc_Barricade: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_barricade.p3d";
	};
	class bldr_Misc_Bench1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_bench1.p3d";
	};
	class bldr_Misc_Bench2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_bench2.p3d";
	};
	class bldr_Misc_Bench3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_bench3.p3d";
	};
	class bldr_Misc_Bench4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_bench4.p3d";
	};
	class bldr_Misc_Bench5: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_bench5.p3d";
	};
	class bldr_Misc_Boogieman: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_boogieman.p3d";
	};
	class bldr_Misc_BoundaryStone: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_boundarystone.p3d";
	};
	class bldr_Misc_BoundaryStone_Plastic: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_boundarystone_plastic.p3d";
	};
	class bldr_Misc_BoundaryStone_Tube: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_boundarystone_tube.p3d";
	};
	class bldr_Misc_BoxWooden: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_boxwooden.p3d";
	};
	class bldr_Misc_Carousel: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_carousel.p3d";
	};
	class bldr_Misc_Carousel_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_carousel_small.p3d";
	};
	class bldr_Misc_Chair_Camp1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_chair_camp1.p3d";
	};
	class bldr_Misc_Chair_Camp2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_chair_camp2.p3d";
	};
	class bldr_Misc_ChickenCoop: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_chickencoop.p3d";
	};
	class bldr_Misc_CrashBarrier: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_crashbarrier.p3d";
	};
	class bldr_Misc_FeedRack: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_feedrack.p3d";
	};
	class bldr_Misc_Football_Gate: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_football_gate.p3d";
	};
	class bldr_Misc_Football_Line: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_football_line.p3d";
	};
	class bldr_Misc_Football_Line_Corner: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_football_line_corner.p3d";
	};
	class bldr_Misc_Football_Line_HCircle: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_football_line_hcircle.p3d";
	};
	class bldr_Misc_Fountain: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_fountain.p3d";
	};
	class bldr_Misc_HayStack: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_haystack.p3d";
	};
	class bldr_Misc_Hutch: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_hutch.p3d";
	};
	class bldr_Misc_NoticeBoard1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_noticeboard1.p3d";
	};
	class bldr_Misc_NoticeBoard2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_noticeboard2.p3d";
	};
	class bldr_Misc_PostBox1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_postbox1.p3d";
	};
	class bldr_Misc_PostBox2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_postbox2.p3d";
	};
	class bldr_Misc_RoadBarrier: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_roadbarrier.p3d";
	};
	class bldr_Misc_Sandbox: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_sandbox.p3d";
	};
	class bldr_Misc_Seesaw: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_seesaw.p3d";
	};
	class bldr_Misc_StadiumBenches: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_stadiumbenches.p3d";
	};
	class bldr_Misc_Sunshade: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_sunshade.p3d";
	};
	class bldr_Misc_Table: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_table.p3d";
	};
	class bldr_Misc_Table_Camp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_table_camp.p3d";
	};
	class bldr_Misc_Table_Camp_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_table_camp_small.p3d";
	};
	class bldr_Misc_Toilet_Dry: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_toilet_dry.p3d";
	};
	class bldr_Misc_TrafficLights: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_trafficlights.p3d";
	};
	class bldr_Misc_TrailRoof_Big: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_trailroof_big.p3d";
	};
	class bldr_Misc_TrailRoof_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_trailroof_small.p3d";
	};
	class bldr_Misc_Tree_Pavement1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_tree_pavement1.p3d";
	};
	class bldr_Misc_Tree_Pavement2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_tree_pavement2.p3d";
	};
	class bldr_Misc_Tree_Pavement3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_tree_pavement3.p3d";
	};
	class bldr_Misc_Tree_Protector: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_tree_protector.p3d";
	};
	class bldr_Misc_Well: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_well.p3d";
	};
	class bldr_Misc_WindBreak: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_windbreak.p3d";
	};
	class bldr_Misc_WoodBlock: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodblock.p3d";
	};
	class bldr_Misc_WoodPile1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodpile1.p3d";
	};
	class bldr_Misc_WoodPile2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodpile2.p3d";
	};
	class bldr_Misc_WoodReserve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodreserve.p3d";
	};
	class bldr_Misc_WoodTable_Indoor: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodtable_indoor.p3d";
	};
	class bldr_Misc_WoodTable_Outdoor: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\misc\misc_woodtable_outdoor.p3d";
	};
	class bldr_Office_Municipal2_Archway: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\office_municipal2_archway.p3d";
	};
	class bldr_A_Office01_metalbars: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\a_office01_metalbars.p3d";
	};
	class bldr_A_Office02_grnd_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\a_office02_grnd_proxy.p3d";
	};
	class bldr_A_Office02_metalbars: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\a_office02_metalbars.p3d";
	};
	class bldr_A_Office02_windows: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\a_office02_windows.p3d";
	};
	class bldr_Black: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\black.p3d";
	};
	class bldr_DoorGlass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\doorglass.p3d";
	};
	class bldr_DoorInterier: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\doorinterier.p3d";
	};
	class bldr_DoorInterier_Closed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\doorinterier_closed.p3d";
	};
	class bldr_DoorSmallGlass: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\doorsmallglass.p3d";
	};
	class bldr_WindowHall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\windowhall.p3d";
	};
	class bldr_WindowLarge: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\windowlarge.p3d";
	};
	class bldr_WindowSmall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\windowsmall.p3d";
	};
	class bldr_Window_A: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\window_a.p3d";
	};
	class bldr_Window_A_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\window_a_broken.p3d";
	};
	class bldr_Window_B: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\window_b.p3d";
	};
	class bldr_Window_B_broken: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\window_b_broken.p3d";
	};
	class bldr_Zarivky: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\offices\proxy\zarivky.p3d";
	};
	class bldr_Platform1_Angle: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_angle.p3d";
	};
	class bldr_Platform1_Block: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_block.p3d";
	};
	class bldr_Platform1_Ramp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_ramp.p3d";
	};
	class bldr_Platform1_Stairs_20: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_stairs_20.p3d";
	};
	class bldr_Platform1_Stairs_30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_stairs_30.p3d";
	};
	class bldr_Platform1_Stairs_30_WallL: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_stairs_30_walll.p3d";
	};
	class bldr_Platform1_Stairs_30_WallR: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_stairs_30_wallr.p3d";
	};
	class bldr_Platform1_Staris_Block: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_staris_block.p3d";
	};
	class bldr_Platform1_Wall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform1_wall.p3d";
	};
	class bldr_Platform2_Angle: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_angle.p3d";
	};
	class bldr_Platform2_Block: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_block.p3d";
	};
	class bldr_Platform2_Enter: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_enter.p3d";
	};
	class bldr_Platform2_Stairs_20: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_stairs_20.p3d";
	};
	class bldr_Platform2_Stairs_30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_stairs_30.p3d";
	};
	class bldr_Platform2_Stairs_30_WallL: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_stairs_30_walll.p3d";
	};
	class bldr_Platform2_Stairs_30_WallR: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_stairs_30_wallr.p3d";
	};
	class bldr_Platform2_Staris_Block: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_staris_block.p3d";
	};
	class bldr_Platform2_Wall: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\platforms\platform2_wall.p3d";
	};
	class bldr_school_details: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\schools\proxy\school_details.p3d";
	};
	class bldr_school_exterior_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\schools\proxy\school_exterior_proxy.p3d";
	};
	class bldr_school_interior: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\schools\proxy\school_interior.p3d";
	};
	class bldr_school_sign: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\schools\proxy\school_sign.p3d";
	};
	class bldr_Tenement_Small_Entry: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\tenement_small_entry.p3d";
	};
	class bldr_Balcony_Rail: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\balcony_rail.p3d";
	};
	class bldr_TB_Big_Doorricade: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_big_doorricade.p3d";
	};
	class bldr_TB_Big_DoorsFrame_extract: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_big_doorsframe_extract.p3d";
	};
	class bldr_TB_Big_HandRail: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_big_handrail.p3d";
	};
	class bldr_TB_Big_Ladders: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_big_ladders.p3d";
	};
	class bldr_TB_Big_Roof_Items: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_big_roof_items.p3d";
	};
	class bldr_TB_Doors_A: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_doors_a.p3d";
	};
	class bldr_TB_Doors_B: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_doors_b.p3d";
	};
	class bldr_TB_Doors_C: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_doors_c.p3d";
	};
	class bldr_TB_Elevator_Stairs: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_elevator_stairs.p3d";
	};
	class bldr_TB_Elevator_winch: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_elevator_winch.p3d";
	};
	class bldr_TB_Lobby: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_lobby.p3d";
	};
	class bldr_TB_small_barrier_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_small_barrier_proxy.p3d";
	};
	class bldr_TB_small_doors_proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_small_doors_proxy.p3d";
	};
	class bldr_TB_Small_Roof_Items: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_small_roof_items.p3d";
	};
	class bldr_TB_Windows: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tb_windows.p3d";
	};
	class bldr_Tenement_Big_win: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tenement_big_win.p3d";
	};
	class bldr_Tenement_Big_win_Single: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tenement_big_win_single.p3d";
	};
	class bldr_Tenement_small_win_Back: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tenement_small_win_back.p3d";
	};
	class bldr_Tenement_small_win_Front: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\residential\tenements\proxy\tenement_small_win_front.p3d";
	};
};
