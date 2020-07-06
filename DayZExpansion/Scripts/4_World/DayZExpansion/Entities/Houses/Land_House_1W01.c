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
		SpawnInteriorObject( "bldr_sofa_leather_old", "1.945863 -2.436615 0.290004", "-1.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_chair", "4.176582 -2.848717 2.360970", "87.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "4.196806 -2.848717 3.261189", "16.000004 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_bucket", "4.409515 -2.070000 3.889876", "41.000008 0.000000 -81.999992" );
		SpawnInteriorObject( "bldr_flowers_01", "4.388534 -1.448631 5.231056", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_radio_b", "1.707969 -2.125000 4.017194", "-135.000046 90.000000 179.999985" );
		SpawnInteriorObject( "bldr_library_a", "1.469173 -2.849998 5.470000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_washing_machine", "-0.733580 -2.848717 5.001147", "-89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-0.997506 -2.848717 0.380594", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchenstove_elec", "-0.950000 -2.848717 1.311314", "-89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lekarnicka", "5.110000 -1.250076 4.022698", "90.000000 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_flag_chernarus_wall", "4.736706 -1.150070 0.702699", "90.000000 0.000000 2.999999", -1 );
		SpawnInteriorObject( "bldr_pc", "-2.990678 -2.139000 0.166439", "76.000000 0.000000 0.000000", 0.65 );
		SpawnInteriorObject( "bldr_chest_dz", "-4.170191 -2.898720 0.743711", "3.000007 0.000000 0.000000", 0.65 );
		SpawnInteriorObject( "bldr_drapes", "-3.832000 -1.022003 5.602200", "0.000000 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "2.372681 -2.848717 2.320879", "0.000000 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.297297 -2.822334 0.597623", "0.000000 -0.000007 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.987701 -2.839996 1.658191", "-79.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.021866 -2.839996 1.771038", "-65.998642 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.819490 -2.839996 2.694687", "-34.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.213265 -2.822334 4.925344", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.831241 -2.839996 4.407481", "13.496431 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.440912 -2.839996 4.643891", "-78.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "2.149124 -1.499481 -0.352658", "90.000015 37.999996 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "4.850000 -1.499969 2.399999", "0.000000 0.000000 -90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "3.155049 -1.499969 5.688990", "-89.999992 -83.000015 -89.999992", -1 );
		SpawnInteriorObject( "bldr_switch_lights", "0.999077 -1.648643 -0.241000", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-1.599000 -1.648643 1.711730", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "-4.392620 -2.548698 2.137421", "92.000015 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_box_c", "-4.191232 -2.548698 3.082011", "-26.000011 0.000000 0.000000", -1 );

		//Ivy
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