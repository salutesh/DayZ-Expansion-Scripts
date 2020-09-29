/**
 * Land_House_1W01.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W01
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{		
		// SpawnInteriorObject( "bldr_sofa_leather_old", "1.945863 -2.436615 0.290004", "-1.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_lobby_chair", "4.176582 -2.848717 2.360970", "87.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_flowers_02", "4.196806 -2.848717 3.261189", "16.000004 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_bucket", "4.409515 -2.070000 3.889876", "41.000008 0.000000 -81.999992" );
		// SpawnInteriorObject( "bldr_flowers_01", "4.388534 -1.448631 5.231056", "0.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_radio_b", "1.707969 -2.125000 4.017194", "-135.000046 90.000000 179.999985" );
		// SpawnInteriorObject( "bldr_library_a", "1.469173 -2.849998 5.470000", "-179.999985 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_washing_machine", "-0.733580 -2.848717 5.001147", "-89.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-0.997506 -2.848717 0.380594", "-90.000015 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_kitchenstove_elec", "-0.950000 -2.848717 1.311314", "-89.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_lekarnicka", "5.110000 -1.250076 4.022698", "90.000000 0.000000 0.000000", -1 );
		// SpawnInteriorObject( "bldr_flag_chernarus_wall", "4.736706 -1.150070 0.702699", "90.000000 0.000000 2.999999", -1 );
		// SpawnInteriorObject( "bldr_pc", "-2.990678 -2.139000 0.166439", "76.000000 0.000000 0.000000", 0.65 );
		// SpawnInteriorObject( "bldr_chest_dz", "-4.170191 -2.898720 0.743711", "3.000007 0.000000 0.000000", 0.65 );
		// SpawnInteriorObject( "bldr_drapes", "-3.832000 -1.022003 5.602200", "0.000000 0.000000 0.000000", -1 );
		// SpawnInteriorObject( "bldr_debris_small_house", "2.372681 -2.848717 2.320879", "0.000000 0.000000 0.000000", -1 );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.297297 -2.822334 0.597623", "0.000000 -0.000007 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.987701 -2.839996 1.658191", "-79.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.021866 -2.839996 1.771038", "-65.998642 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.819490 -2.839996 2.694687", "-34.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.213265 -2.822334 4.925344", "0.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.831241 -2.839996 4.407481", "13.496431 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.440912 -2.839996 4.643891", "-78.000000 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_mil_radar_tall_planks", "2.149124 -1.499481 -0.352658", "90.000015 37.999996 90.000000", -1 );
		// SpawnInteriorObject( "bldr_mil_radar_tall_planks", "4.850000 -1.499969 2.399999", "0.000000 0.000000 -90.000000", -1 );
		// SpawnInteriorObject( "bldr_mil_radar_tall_planks", "3.155049 -1.499969 5.688990", "-89.999992 -83.000015 -89.999992", -1 );
		// SpawnInteriorObject( "bldr_switch_lights", "0.999077 -1.648643 -0.241000", "-90.000015 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_switch_lights", "-1.599000 -1.648643 1.711730", "-179.999985 0.000000 0.000000" );
		// SpawnInteriorObject( "bldr_box_c", "-4.392620 -2.548698 2.137421", "92.000015 0.000000 0.000000", -1 );
		// SpawnInteriorObject( "bldr_box_c", "-4.191232 -2.548698 3.082011", "-26.000011 0.000000 0.000000", -1 );

		SpawnInteriorObject( "bldr_sofa_leather_old", "1.955558 -2.436615  0.340000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject( "bldr_lobby_table", "1.933940 -2.848717 1.822599", "6.000000 0.000000 0.000000");
		SpawnInteriorObject( "bldr_carpet_big_dirty2", "2.143656 -2.830002 1.648908", "93.000031 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_library_a", "-1.098104 -2.848717 1.186980", "91.000015 -0.000007 0.000000");
		SpawnInteriorObject( "bldr_picture_d", "1.480411 -2.338692  5.630564", "179.999939 0.000000 5.000009"); // needs to be client sided 
		SpawnInteriorObject( "bldr_kitchendesk_b", "-1.015838 -1.785088 4.929969", "0.000000 0.000000 0.000000");
		SpawnInteriorObject( "ExpansionIndustrialProp_CartonGarbage7", "-1.897026 -0.457001 3.997701", "-7.000000 0.000000 0.000000");
		SpawnInteriorObject( "bldr_classroom_case_b_broken", "-4.371838 -2.848717 2.151436", "-83.999886 0.000000 0.000000");
		SpawnInteriorObject( "bldr_picture_g_02", "-2.729617 -3.168022 0.350677", "-88.999954 59.000004 -0.000009"); // needs to be client sided 
		SpawnInteriorObject( "bldr_desk_globe", "-3.916745 -2.705002 0.473046", "135.000015 74.000031 -1.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_flowers_02", "-4.232609 -2.848717 0.245393", "116.999947 0.000000 0.000000");
		SpawnInteriorObject( "ExpansionIndustrialProp_CartonGarbage8", "4.284019 -2.619995  2.464697", "-175.000000 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "ExpansionIndustrialProp_CartonGarbage3", "3.438589 -2.811996 2.931662", "0.000000 -1.000000 0.000000");
		SpawnInteriorObject( "bldr_debris_small_house", "0.668655 -2.848717 3.085153", "-120.999947 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "ExpansionPropTackleBox1", "4.563218 -2.029999 4.068072", "70.999992 0.000000 0.000000");
		SpawnInteriorObject( "ExpansionPropTool2", "-1.012975 -2.097 0.120622", "-90.000000 74.000008 179.999985"); // needs to be client sided 
		SpawnInteriorObject( "ExpansionPropWateringCan", "-1.096051 -2.635002 0.031651", "-70.000000 -0.000007 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "0.230416 -2.739998 -1.024931", "17.000025 0.027374 0.458250"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "0.233391 -2.753998 -0.007425", "0.000000 1.027257 0.458367"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "4.060386 -2.739998 2.617620", "-68.000015 2.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "-1.034934 -2.716782 1.041164", "-88.000046 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "0.033652 -2.753998  4.923003", "-16.000004 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "-0.896820 -2.716782 5.181221", "-178.999954 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_large_pile", "-2.301861 -2.690773 3.714873", "-61.000004 0.000000 1.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "-3.796751 -2.742882 3.965950", "108.000000 1.702589 1.230775"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "-1.632661 -2.716751 0.594320", "-73.000084 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "-3.667868 -2.742882 0.116238", "-179.999985 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_light_kitchen_03", "1.894963 -1.148613 2.659581", "0.000000 0.000000 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "ExpansionPropClampLight1", "-1.676478 -0.791679 0.706506", "-90.000000 0.000000 -179.999939"); // needs to be client sided 
		SpawnInteriorObject( "bldr_switch_lights", "0.864101 -1.726662 -0.239185", "-89.999977 -0.000007 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_switch_lights", "-1.598940 -1.726662 1.808377", "-179.999985 -0.000007 0.000000"); // needs to be client sided 
		SpawnInteriorObject( "bldr_picture_f", "-2.521151 -2.032677 5.630530", "-179.999985 0.000000 4.000000"); // needs to be client sided 


	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-4.099999 0.000000 -0.400000", "2.999870 -84.000008 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "4.849998 0.800049 -0.550000", "179.999939 -88.000015 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-4.700002 0.550034 -0.400000", "0.000000 -84.000008 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.400000 0.400024 -0.500000", "-179.999954 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-5.000004 -1.150070 -0.150000", "0.000000 0.000000 89.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-5.000001 0.450027 2.749998", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.149998 0.251472 5.620005", "-90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-4.800000 -1.200073 3.699999", "-179.999939 0.000000 3.000026" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-5.050001 -3.050186 1.800001", "90.000015 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-1.400000 -2.900177 -0.450000", "0.000000 83.000015 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.730000 -3.100189 -0.500000", "177.000031 0.000000 -93.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.099993 0.500031 5.550004", "90.000122 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.100001 -3.100189 -0.400000", "90.000015 89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "4.980000 -2.900177 1.590000", "0.000000 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "5.099998 0.250015 0.230000", "-86.999969 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "4.931616 -3.950241 1.108696", "-90.000000 1.680066 0.458564" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-4.317979 -3.300018 -0.483618", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "5.000001 -3.000000 4.399999", "90.000000 2.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.100000 -2.898720 5.940000", "179.999985 89.000046 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.500001 0.500031 5.900000", "0.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-3.000001 -0.498573 5.900007", "-90.000107 0.000000 -90.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.200000 -2.848717 5.949999", "179.999985 81.000000 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-1.788250 -0.348564 5.784756", "-89.999969 13.000002 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "1.370935 -3.198738 5.842519", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.728557 -4.700104 5.929074", "0.036726 1.679372 1.252985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-5.782194 0.836662 4.397884", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-5.346267 1.201530 2.017859", "90.000038 -89.000046 -1.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-1.726501 3.651680 2.827504", "47.000092 -44.000027 126.000130" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-3.320254 0.842903 -1.271719", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.505524 2.301598 0.548239", "-122.000069 -29.000008 147.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-0.053477 3.856827 2.398047", "177.205505 -38.000004 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.281546 3.836464 2.799860", "0.081482 -38.643150 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "1.928976 2.390198 4.601846", "-142.938019 33.999992 -72.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_E", "3.810831 1.250076 -0.722930", "0.008217 1.027026 0.458441" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "2.473495 2.901634 2.438649", "0.000000 0.000000 -54.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.420005 2.401604 0.884213", "23.000042 -20.000008 -149.999969" );
	}

}