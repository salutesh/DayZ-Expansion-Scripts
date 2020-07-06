#define _ARMA_

class CfgPatches
{
	class BuilderItems_Parts
	{
		units[] = {"bldr_rds_asf1_0_2000","bldr_rds_asf1_1_1000","bldr_rds_asf1_6","bldr_rds_asf1_6_crosswalk","bldr_rds_asf1_6konec","bldr_rds_asf1_7_100","bldr_rds_asf1_10_25","bldr_rds_asf1_10_50","bldr_rds_asf1_10_75","bldr_rds_asf1_10_100","bldr_rds_asf1_12","bldr_rds_asf1_15_75","bldr_rds_asf1_22_50","bldr_rds_asf1_25","bldr_rds_asf1_30_25","bldr_rds_asf1_60_10","bldr_rds_asf2_0_2000","bldr_rds_asf2_1_1000","bldr_rds_asf2_6","bldr_rds_asf2_6_crosswalk","bldr_rds_asf2_6konec","bldr_rds_asf2_7_100","bldr_rds_asf2_10_25","bldr_rds_asf2_10_50","bldr_rds_asf2_10_75","bldr_rds_asf2_10_100","bldr_rds_asf2_12","bldr_rds_asf2_15_75","bldr_rds_asf2_22_50","bldr_rds_asf2_25","bldr_rds_asf2_30_25","bldr_rds_asf2_60_10","bldr_rds_asf3_0_2000","bldr_rds_asf3_1_1000","bldr_rds_asf3_6","bldr_rds_asf3_6konec","bldr_rds_asf3_7_100","bldr_rds_asf3_10_25","bldr_rds_asf3_10_50","bldr_rds_asf3_10_75","bldr_rds_asf3_10_100","bldr_rds_asf3_12","bldr_rds_asf3_15_75","bldr_rds_asf3_22_50","bldr_rds_asf3_25","bldr_rds_asf3_30_25","bldr_rds_asf3_60_10","bldr_rds_city_0_2000","bldr_rds_city_1_1000","bldr_rds_city_6","bldr_rds_city_6_crosswalk","bldr_rds_city_6konec","bldr_rds_city_7_100","bldr_rds_city_10_25","bldr_rds_city_10_50","bldr_rds_city_10_75","bldr_rds_city_10_100","bldr_rds_city_12","bldr_rds_city_15_75","bldr_rds_city_22_50","bldr_rds_city_25","bldr_rds_city_30_25","bldr_rds_city_60_10","bldr_rds_grav_0_2000","bldr_rds_grav_1_1000","bldr_rds_grav_6","bldr_rds_grav_6konec","bldr_rds_grav_7_100","bldr_rds_grav_10_25","bldr_rds_grav_10_50","bldr_rds_grav_10_75","bldr_rds_grav_10_100","bldr_rds_grav_12","bldr_rds_grav_15_75","bldr_rds_grav_22_50","bldr_rds_grav_25","bldr_rds_grav_30_25","bldr_rds_grav_60_10","bldr_rds_kr_t_asf1_asf2","bldr_rds_kr_t_asf1_asf3","bldr_rds_kr_t_asf1_city","bldr_rds_kr_t_asf2_asf2","bldr_rds_kr_t_asf2_asf3","bldr_rds_kr_t_asf3_asf2","bldr_rds_kr_t_asf3_asf3","bldr_rds_kr_t_asf3_mud","bldr_rds_kr_t_city_asf3","bldr_rds_kr_t_city_city","bldr_rds_kr_t_mud_mud","bldr_rds_kr_x_asf1_asf3","bldr_rds_kr_x_asf1_city","bldr_rds_kr_x_asf2_asf3","bldr_rds_kr_x_city_city","bldr_rds_kr_x_city_city_asf3","bldr_rds_mud_0_2000","bldr_rds_mud_1_1000","bldr_rds_mud_6","bldr_rds_mud_6konec","bldr_rds_mud_7_100","bldr_rds_mud_10_25","bldr_rds_mud_10_50","bldr_rds_mud_10_75","bldr_rds_mud_10_100","bldr_rds_mud_12","bldr_rds_mud_15_75","bldr_rds_mud_22_50","bldr_rds_mud_25","bldr_rds_mud_30_25","bldr_rds_mud_60_10","bldr_rds_path_dirt_10","bldr_rds_path_dirt_30","bldr_rds_path_dirt_curve","bldr_rds_path_rock_10","bldr_rds_path_rock_30","bldr_rds_path_rock_curve","bldr_rds_runway_beton","bldr_rds_runway_beton_end1","bldr_rds_runway_beton_end2","bldr_rds_runway_dirt_1","bldr_rds_runway_dirt_2","bldr_rds_runway_dirt_3","bldr_rds_runway_end15","bldr_rds_runway_end33","bldr_rds_runway_main","bldr_rds_runway_main_40","bldr_rds_runway_poj_draha","bldr_rds_runway_poj_l_1","bldr_rds_runway_poj_l_1_end","bldr_rds_runway_poj_l_2","bldr_rds_runway_poj_l_2_end","bldr_rds_runway_poj_spoj","bldr_rds_runway_poj_spoj_2","bldr_rds_runway_poj_t_1","bldr_rds_runway_poj_t_2","bldr_rds_runwayold_40_main","bldr_Decal_Heli_Army"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Roads"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_rds_asf1_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_0 2000.p3d";
	};
	class bldr_rds_asf1_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_1 1000.p3d";
	};
	class bldr_rds_asf1_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_6.p3d";
	};
	class bldr_rds_asf1_6_crosswalk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_6_crosswalk.p3d";
	};
	class bldr_rds_asf1_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_6konec.p3d";
	};
	class bldr_rds_asf1_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_7 100.p3d";
	};
	class bldr_rds_asf1_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_10 25.p3d";
	};
	class bldr_rds_asf1_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_10 50.p3d";
	};
	class bldr_rds_asf1_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_10 75.p3d";
	};
	class bldr_rds_asf1_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_10 100.p3d";
	};
	class bldr_rds_asf1_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_12.p3d";
	};
	class bldr_rds_asf1_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_15 75.p3d";
	};
	class bldr_rds_asf1_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_22 50.p3d";
	};
	class bldr_rds_asf1_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_25.p3d";
	};
	class bldr_rds_asf1_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_30 25.p3d";
	};
	class bldr_rds_asf1_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf1_60 10.p3d";
	};
	class bldr_rds_asf2_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_0 2000.p3d";
	};
	class bldr_rds_asf2_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_1 1000.p3d";
	};
	class bldr_rds_asf2_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_6.p3d";
	};
	class bldr_rds_asf2_6_crosswalk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_6_crosswalk.p3d";
	};
	class bldr_rds_asf2_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_6konec.p3d";
	};
	class bldr_rds_asf2_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_7 100.p3d";
	};
	class bldr_rds_asf2_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_10 25.p3d";
	};
	class bldr_rds_asf2_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_10 50.p3d";
	};
	class bldr_rds_asf2_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_10 75.p3d";
	};
	class bldr_rds_asf2_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_10 100.p3d";
	};
	class bldr_rds_asf2_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_12.p3d";
	};
	class bldr_rds_asf2_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_15 75.p3d";
	};
	class bldr_rds_asf2_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_22 50.p3d";
	};
	class bldr_rds_asf2_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_25.p3d";
	};
	class bldr_rds_asf2_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_30 25.p3d";
	};
	class bldr_rds_asf2_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf2_60 10.p3d";
	};
	class bldr_rds_asf3_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_0 2000.p3d";
	};
	class bldr_rds_asf3_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_1 1000.p3d";
	};
	class bldr_rds_asf3_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_6.p3d";
	};
	class bldr_rds_asf3_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_6konec.p3d";
	};
	class bldr_rds_asf3_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_7 100.p3d";
	};
	class bldr_rds_asf3_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_10 25.p3d";
	};
	class bldr_rds_asf3_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_10 50.p3d";
	};
	class bldr_rds_asf3_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_10 75.p3d";
	};
	class bldr_rds_asf3_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_10 100.p3d";
	};
	class bldr_rds_asf3_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_12.p3d";
	};
	class bldr_rds_asf3_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_15 75.p3d";
	};
	class bldr_rds_asf3_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_22 50.p3d";
	};
	class bldr_rds_asf3_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_25.p3d";
	};
	class bldr_rds_asf3_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_30 25.p3d";
	};
	class bldr_rds_asf3_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\asf3_60 10.p3d";
	};
	class bldr_rds_city_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_0 2000.p3d";
	};
	class bldr_rds_city_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_1 1000.p3d";
	};
	class bldr_rds_city_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_6.p3d";
	};
	class bldr_rds_city_6_crosswalk: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_6_crosswalk.p3d";
	};
	class bldr_rds_city_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_6konec.p3d";
	};
	class bldr_rds_city_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_7 100.p3d";
	};
	class bldr_rds_city_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_10 25.p3d";
	};
	class bldr_rds_city_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_10 50.p3d";
	};
	class bldr_rds_city_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_10 75.p3d";
	};
	class bldr_rds_city_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_10 100.p3d";
	};
	class bldr_rds_city_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_12.p3d";
	};
	class bldr_rds_city_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_15 75.p3d";
	};
	class bldr_rds_city_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_22 50.p3d";
	};
	class bldr_rds_city_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_25.p3d";
	};
	class bldr_rds_city_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_30 25.p3d";
	};
	class bldr_rds_city_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\city_60 10.p3d";
	};
	class bldr_rds_grav_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_0 2000.p3d";
	};
	class bldr_rds_grav_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_1 1000.p3d";
	};
	class bldr_rds_grav_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_6.p3d";
	};
	class bldr_rds_grav_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_6konec.p3d";
	};
	class bldr_rds_grav_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_7 100.p3d";
	};
	class bldr_rds_grav_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_10 25.p3d";
	};
	class bldr_rds_grav_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_10 50.p3d";
	};
	class bldr_rds_grav_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_10 75.p3d";
	};
	class bldr_rds_grav_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_10 100.p3d";
	};
	class bldr_rds_grav_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_12.p3d";
	};
	class bldr_rds_grav_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_15 75.p3d";
	};
	class bldr_rds_grav_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_22 50.p3d";
	};
	class bldr_rds_grav_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_25.p3d";
	};
	class bldr_rds_grav_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_30 25.p3d";
	};
	class bldr_rds_grav_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\grav_60 10.p3d";
	};
	class bldr_rds_kr_t_asf1_asf2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf1_asf2.p3d";
	};
	class bldr_rds_kr_t_asf1_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf1_asf3.p3d";
	};
	class bldr_rds_kr_t_asf1_city: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf1_city.p3d";
	};
	class bldr_rds_kr_t_asf2_asf2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf2_asf2.p3d";
	};
	class bldr_rds_kr_t_asf2_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf2_asf3.p3d";
	};
	class bldr_rds_kr_t_asf3_asf2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf3_asf2.p3d";
	};
	class bldr_rds_kr_t_asf3_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf3_asf3.p3d";
	};
	class bldr_rds_kr_t_asf3_mud: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_asf3_mud.p3d";
	};
	class bldr_rds_kr_t_city_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_city_asf3.p3d";
	};
	class bldr_rds_kr_t_city_city: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_city_city.p3d";
	};
	class bldr_rds_kr_t_mud_mud: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_t_mud_mud.p3d";
	};
	class bldr_rds_kr_x_asf1_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_x_asf1_asf3.p3d";
	};
	class bldr_rds_kr_x_asf1_city: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_x_asf1_city.p3d";
	};
	class bldr_rds_kr_x_asf2_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_x_asf2_asf3.p3d";
	};
	class bldr_rds_kr_x_city_city: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_x_city_city.p3d";
	};
	class bldr_rds_kr_x_city_city_asf3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\kr_x_city_city_asf3.p3d";
	};
	class bldr_rds_mud_0_2000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_0 2000.p3d";
	};
	class bldr_rds_mud_1_1000: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_1 1000.p3d";
	};
	class bldr_rds_mud_6: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_6.p3d";
	};
	class bldr_rds_mud_6konec: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_6konec.p3d";
	};
	class bldr_rds_mud_7_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_7 100.p3d";
	};
	class bldr_rds_mud_10_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_10 25.p3d";
	};
	class bldr_rds_mud_10_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_10 50.p3d";
	};
	class bldr_rds_mud_10_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_10 75.p3d";
	};
	class bldr_rds_mud_10_100: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_10 100.p3d";
	};
	class bldr_rds_mud_12: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_12.p3d";
	};
	class bldr_rds_mud_15_75: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_15 75.p3d";
	};
	class bldr_rds_mud_22_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_22 50.p3d";
	};
	class bldr_rds_mud_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_25.p3d";
	};
	class bldr_rds_mud_30_25: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_30 25.p3d";
	};
	class bldr_rds_mud_60_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\mud_60 10.p3d";
	};
	class bldr_rds_path_dirt_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_dirt_10.p3d";
	};
	class bldr_rds_path_dirt_30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_dirt_30.p3d";
	};
	class bldr_rds_path_dirt_curve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_dirt_curve.p3d";
	};
	class bldr_rds_path_rock_10: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_rock_10.p3d";
	};
	class bldr_rds_path_rock_30: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_rock_30.p3d";
	};
	class bldr_rds_path_rock_curve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\path_rock_curve.p3d";
	};
	class bldr_rds_runway_beton: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_beton.p3d";
	};
	class bldr_rds_runway_beton_end1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_beton_end1.p3d";
	};
	class bldr_rds_runway_beton_end2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_beton_end2.p3d";
	};
	class bldr_rds_runway_dirt_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_dirt_1.p3d";
	};
	class bldr_rds_runway_dirt_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_dirt_2.p3d";
	};
	class bldr_rds_runway_dirt_3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_dirt_3.p3d";
	};
	class bldr_rds_runway_end15: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_end15.p3d";
	};
	class bldr_rds_runway_end33: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_end33.p3d";
	};
	class bldr_rds_runway_main: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_main.p3d";
	};
	class bldr_rds_runway_main_40: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_main_40.p3d";
	};
	class bldr_rds_runway_poj_draha: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_draha.p3d";
	};
	class bldr_rds_runway_poj_l_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_l_1.p3d";
	};
	class bldr_rds_runway_poj_l_1_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_l_1_end.p3d";
	};
	class bldr_rds_runway_poj_l_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_l_2.p3d";
	};
	class bldr_rds_runway_poj_l_2_end: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_l_2_end.p3d";
	};
	class bldr_rds_runway_poj_spoj: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_spoj.p3d";
	};
	class bldr_rds_runway_poj_spoj_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_spoj_2.p3d";
	};
	class bldr_rds_runway_poj_t_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_t_1.p3d";
	};
	class bldr_rds_runway_poj_t_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runway_poj_t_2.p3d";
	};
	class bldr_rds_runwayold_40_main: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\parts\runwayold_40_main.p3d";
	};
	class bldr_Decal_Heli_Army: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures\roads\decals\Decal_Heli_Army.p3d";
	};
};
