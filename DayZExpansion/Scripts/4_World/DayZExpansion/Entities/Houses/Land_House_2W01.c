/**
 * Land_House_2W01.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_2W01
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject("bldr_box_c_multi", "-0.171661 -3.199997 -2.045304", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_ch_mod_h", "-0.714760 -1.861992 -2.419890", "102.999977 -88.000015 -0.000009");
		SpawnInteriorObject("bldr_bucket", "-0.976474 -2.010002 -3.099478", "21.999998 -82.599983 0.000000");
		SpawnInteriorObject("bldr_ch_office_b", "-0.597700 -1.631699 -2.030000", "-97.000000 -45.000019 179.999985");
		SpawnInteriorObject("bldr_class_case_b_closed", "-2.088891 -3.000061 -2.345405", "-87.000008 0.000000 -22.000013");
		SpawnInteriorObject("bldr_picture_f_02", "-0.002378 -3.165001 -2.783068", "0.000000 33.200001 2.000000");
		SpawnInteriorObject("bldr_picture_d", "0.091437 -2.531748 -1.710000", "-179.998993 0.000000 -0.000009");
		SpawnInteriorObject("bldr_picture_e", "-1.307119 -2.531744 -1.710000", "179.999924 0.000000 -8.000001");
		SpawnInteriorObject("bldr_box_c_multi", "6.071687 -3.231796 1.116919", "95.000023 0.000000 0.000000");
		SpawnInteriorObject("bldr_case_cans_b", "6.796581 -2.750000 -2.033713", "-90.000015 0.000000 53.999996");
		SpawnInteriorObject("bldr_box_c", "6.835146 -2.931786 -0.568054", "-11.000007 0.000000 0.000000");
		SpawnInteriorObject("bldr_debris_small_house", "5.425579 -3.231804 -0.783962", "162.000015 0.000000 0.000000", 0.25);
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "5.718567 -3.205421 -1.741680", "93.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "6.731217 -3.205421 -1.023939", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "6.217906 -3.205414 2.481661", "58.999989 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "5.437127 -3.205414 2.455477", "-105.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "1.080371 -3.205421 -2.746254", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "1.792059 -3.205421 -0.423417", "107.999969 0.000000 0.000000");
		SpawnInteriorObject("bldr_sofa_leather_old", "0.055798 -2.800049 0.194811", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-1.295551 -2.900002 0.093165", "-11.000002 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_c_02", "-0.953339 -2.981781 -0.280000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_f", "-1.252268 -3.531815 -0.280000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_b", "-1.176650 -1.531755 -0.295000", "179.999985 -90.000000 -0.001006");
		SpawnInteriorObject("bldr_picture_b_02", "-1.708053 -2.131729 -0.280000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_a", "-1.670000 -2.660004 -0.280000", "-0.000009 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_e_02", "-2.020000 -3.231796 -0.280000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_picture_c", "-2.314797 -2.599998 -0.280000", "-0.000009 0.000000 -0.000009");
		SpawnInteriorObject("bldr_picture_g", "-2.154006 -1.989999 -0.280000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_carpet_big", "-0.083990 -3.181793 1.138397", "88.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_lobby_table", "0.083101 -3.231796 1.468267", "7.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "1.584909 -0.950058 4.455818", "90.000122 -58.000011 -89.999969", 0.33);
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "-0.978634 -1.300018 4.404035", "-89.999977 -79.000023 89.999992", 0.75);
		SpawnInteriorObject("bldr_fridge", "-2.392848 -3.231796 3.925924", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_kitchenstove_elec", "-1.828869 -3.231796 3.925247", "0.000000 0.000000 0.000000");
		// SpawnInteriorObject("bldr_BoxCerealCrunchin", "0.141940 -2.724998 1.430822", "-100.000038 -90.000000 179.999985");
		SpawnInteriorObject("bldr_radio_b", "0.465215 -2.440002 3.817086", "147.000122 0.000000 0.000000");
		// SpawnInteriorObject("bldr_Pot", "-1.940318 -2.385002 4.057125", "-172.000031 0.000000 0.000000");
		// SpawnInteriorObject("bldr_Vodka", "-2.352177 -1.694672 3.796089", "93.298775 0.000000 0.000000");
		// SpawnInteriorObject("bldr_Vodka", "-2.332465 -1.795998 3.901054", "71.608154 90.000000 179.999985");
		// SpawnInteriorObject("bldr_WaterBottle", "0.688354 -2.404999 3.648003", "106.833122 0.000000 90.000000");
		SpawnInteriorObject("bldr_case_wall_unit_part_c", "3.790951 -3.231796 3.850000", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_case_wall_unit_part_d", "3.790950 -3.231796 3.048000", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_tv_a", "3.713000 -2.002998 3.829270", "71.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_01", "3.802173 -2.000000 2.937410", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "3.383445 -3.205414 0.544376", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "2.491874 -3.205414 1.056335", "96.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_radiator", "2.710855 -3.131790 4.313807", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_debris_small_house", "2.445206 -3.250000 1.129363", "0.000000 0.000000 0.000000", 0.45);
		SpawnInteriorObject("bldr_BookBible", "3.813521 -2.391998 4.200000", "-0.000009 0.000000 -0.000009");
		SpawnInteriorObject("bldr_BookTheWarOfTheWorlds", "3.800000 -2.391998 4.145000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_BookAroundTheWorldIn80Days", "3.805000 -2.391998 4.090000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_BookCrimeAndPunishment", "3.813521 -2.400002 3.967711", "0.000000 -34.999992 0.000000");
		SpawnInteriorObject("bldr_BookTheMetamorphosis", "3.813521 -2.504997 3.683172", "103.000046 90.000000 -0.000009");
		SpawnInteriorObject("bldr_BookTheRaven", "3.763103 -2.465721 3.080000", "9.000001 -28.999990 0.000000");
		SpawnInteriorObject("bldr_BookRobinsonCrusoe", "3.763103 -2.574997 3.242116", "94.000000 89.965729 0.000000");
		SpawnInteriorObject("bldr_BookRussian", "3.770000 -2.519997 3.240000", "-91.000023 -90.000000 179.999985");
		SpawnInteriorObject("bldr_BookTheCallOfCthulhu", "3.763103 -2.465721 3.237533", "-81.000023 -90.000000 179.999985");
		// SpawnInteriorObject("bldr_BakedBeansCan_Opened", "-1.774000 -2.338997 4.075006", "-164.371490 0.000000 11.000003");
		// SpawnInteriorObject("bldr_SardinesCan_Opened", "-1.955353 -2.370003 3.850000", "-179.999985 9.000000 0.000000");
		// SpawnInteriorObject("bldr_lekarnicka", "-3.569052 -1.931717 4.580000", "0.000000 0.000000 0.000000");
		// SpawnInteriorObject("FirstAidKit", "-3.951769 -1.129997 4.150000", "17.999990 -89.965729 0.000000");
		// SpawnInteriorObject("bldr_VitaminBottle", "-3.741039 -3.165001 4.069036", "0.000000 0.000000 0.000000");
		// SpawnInteriorObject("bldr_CharcoalTablets", "-3.645676 -3.181793 4.069489", "37.999928 90.000000 -0.000009");
		// SpawnInteriorObject("bldr_TetracyclineAntibiotics", "-3.834510 -3.204002 3.958847", "89.999992 -90.000000 179.999985");
		// SpawnInteriorObject("bldr_PainkillerTablets", "-4.036809 -3.190002 4.127872", "53.999985 -90.000000 179.999985");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-5.119484 -3.190002 3.825468", "58.999989 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_02", "-6.445415 -2.650002 2.328866", "32.000004 0.000000 0.000000");
		SpawnInteriorObject("bldr_washing_machine", "-5.348947 -3.231804 2.188208", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_bag_dz", "-4.640105 -2.814674 2.214945", "160.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-6.729455 -2.900002 3.783989", "-85.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "-7.217557 -1.650101 3.121490", "179.999939 42.999992 -91.000015", 0.5);
		SpawnInteriorObject("bldr_bucket", "-5.957071 -3.209999 2.135035", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_radiator", "-4.082159 -3.131798 1.662876", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_desk_globe", "-5.632412 -2.440002 1.262940", "78.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_flag_chernarus_wall", "-4.893365 -1.131676 1.615000", "0.000000 0.000000 -2.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-4.139909 -3.205421 0.210179", "105.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-5.253272 -3.205421 0.772262", "-38.000004 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-5.582652 -3.205399 3.639540", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_vaulting_box_small", "-6.782113 -3.209999 -1.142496", "-3.000004 0.000000 0.000000");
		SpawnInteriorObject("bldr_radio_b", "-6.650246 -2.655998 -1.020056", "104.999962 90.000000 -0.000009");
		// SpawnInteriorObject("Bear_Beige", "-6.981912 -2.547432 -2.136333", "-87.388206 0.000000 0.000000");
		SpawnInteriorObject("bldr_school_map", "-4.031754 -1.389992 -3.497000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_debris_small_house", "-5.496915 -3.230003 -0.838481", "0.000000 0.000000 0.000000", 0.65);
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-6.499646 -3.205399 -2.176625", "-154.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_plnt_c_leafs_spread", "-4.894399 -3.205399 -2.491909", "82.999992 0.000000 0.000000");
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "-7.186003 -0.800049 0.429648", "0.000000 -89.000046 90.000000", 0.25);
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "-7.216307 -1.350082 -1.939120", "179.999985 90.000000 89.999992", 0.85);
		SpawnInteriorObject("bldr_mil_radar_tall_planks", "-5.904712 -1.750107 -3.688349", "89.999992 0.000000 -90.000038", 0.15);
		SpawnInteriorObject("bldr_case_bedroom_b", "-2.801807 0.017311 -3.312750", "97.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "0.415190 0.317329 -3.103529", "91.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "0.408805 0.317001 -2.197574", "-90.000038 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-0.100000 0.317001 -2.156804", "89.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-0.154929 0.317001 -3.061389", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "0.338032 0.917366 -2.176708", "97.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "0.382752 0.917000 -3.110000", "-88.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-0.690926 0.317001 -3.054811", "-97.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-0.612845 0.317001 -2.079093", "-87.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c", "-0.351012 0.917000 -2.854754", "-68.000015 0.000000 0.000000");
		// SpawnInteriorObject("WindscreenBox", "-3.977978 0.000000 -1.815508", "91.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_library_a_open", "-4.291743 0.017311 1.731978", "91.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_postel_manz_kov", "-1.070041 1.760002 3.180000", "98.000015 0.000000 90.000038");
		SpawnInteriorObject("bldr_foldingbed_open", "-2.434162 -0.241997 2.566868", "86.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_box_c_multi", "-0.836477 0.017311 0.031301", "-8.000002 0.000000 0.000000");
		SpawnInteriorObject("bldr_sink", "-1.739112 0.017311 0.904695", "173.000031 16.999996 16.000017");
	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-7.469485 2.000122 -0.188296", "0.000000 -23.999992 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.462268 3.879128 0.174754", "90.000000 0.000000 -144.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.147229 3.840271 0.153956", "179.999985 -42.000015 178.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-4.257028 3.268600 -0.403867", "81.000008 6.000003 -53.999996" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.628523 3.644554 -2.305281", "-90.000000 -33.999992 177.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.997157 3.610550 -1.599379", "96.000023 -30.999987 175.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "1.468503 2.468552 -1.719212", "179.999939 -39.999996 176.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "0.825625 2.252670 -3.083473", "89.999969 -89.000046 -179.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.108070 2.350143 -1.851508", "60.000233 20.999996 -149.000580" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-1.202253 3.450211 -4.015891", "-82.999992 64.000015 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_S", "-3.481007 2.117439 -3.560146", "-90.000046 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "-2.969202 2.817482 -2.869771", "179.999985 0.000000 -156.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.122411 -0.381622 -4.011884", "-0.000009 -82.000008 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "4.111961 -0.081604 -4.061884", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-5.516379 -1.381683 -3.861884", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-0.284989 -0.481628 -3.911884", "0.000000 0.000000 -92.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "1.163592 -3.231796 -3.961884", "0.000000 0.000000 -68.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.208895 -3.131790 -4.161885", "-90.000000 0.000000 -90.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-4.813201 -1.881714 -3.940000", "0.000000 0.000000 110.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.189407 1.718498 1.258287", "0.000000 -26.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "4.335896 3.703621 0.232581", "0.000000 -53.000008 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.002250 3.817543 0.483173", "0.000000 -47.000000 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.143906 3.817543 0.451010", "-94.000031 0.000000 -143.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-2.429413 3.767540 0.512922", "0.000000 -35.999992 -179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.085388 1.868515 3.092106", "0.000000 -38.999992 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "4.164356 -3.300201 4.740986", "178.999969 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "6.695879 -0.500031 -0.410000", "-120.999977 12.000002 173.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "1.471833 -2.100128 4.690986", "90.000038 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.230433 0.250015 4.690986", "-0.000005 -89.000046 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.218759 0.368423 4.703027", "0.999995 -89.000046 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.098732 -0.079033 4.640986", "-0.000005 -89.000046 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.343800 -3.150192 4.640986", "-0.000005 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-2.809893 -2.100128 4.690986", "-0.000005 -88.000015 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.469485 3.550217 0.585772", "179.999985 -51.000008 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-7.469485 0.400024 -1.127254", "90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.403586 -3.300201 4.647583", "-90.000000 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-7.519485 0.350021 4.327151", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-7.369485 0.250015 0.669374", "90.000000 0.000000 -83.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-7.403586 -3.650223 2.263661", "89.999969 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-7.348026 -2.481758 1.869042", "85.000008 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.585026 -0.931656 3.141552", "-90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "6.455249 -1.000061 3.237286", "0.000000 -89.000046 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "4.250616 -3.531815 3.681266", "90.000038 0.000000 0.000000" );
	}

}