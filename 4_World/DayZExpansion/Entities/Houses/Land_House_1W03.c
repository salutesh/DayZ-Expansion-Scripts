/**
 * Land_House_1W03.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W03
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_sofacorner", "-0.674823 -2.030746 2.615662", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "-0.600037 -2.443253 2.755787", "9.000003 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_lobby_table", "-1.229499 -2.442459 3.218066", "101.999977 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_case_sink_a", "-3.342555 -2.443253 4.440000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_cans_b", "-1.157288 -2.443253 -1.820000", "-2.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_chest_dz", "-0.421979 -2.714996 -5.840123", "92.000015 0.000000 0.000000", 0.2 );
		SpawnInteriorObject( "bldr_carpet_2_dz", "-0.430073 -1.945000 -7.101552", "-7.000000 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_lobby_chair", "0.287465 -2.443253 -4.339605", "138.999985 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_lobby_chair", "-1.340910 -2.443253 -4.325975", "-165.000015 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_office_table_a", "-3.584087 -2.443253 -5.185099", "1.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_chair_a", "-2.990120 -2.443253 -5.253413", "104.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_library_a", "2.190000 -2.443253 -6.926800", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_library_a_open", "2.190000 -2.443253 -6.024000", "-90.000000 0.000000 0.000000" );

		//if ( Math.RandomFloatInclusive(0, 1) <= 0.25 )
		{
			SpawnInteriorObject( "bldr_box_c_multi", "1.172155 -2.443253 -3.582774", "95.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_box_c", "2.024318 -1.509003 -2.716336", "4.000098 0.000000 -86.000008" );
		}


		SpawnInteriorObject( "bldr_picture_c", "-4.035000 -1.999944 -5.123661", "90.000000 0.500000 0.000000" );
		SpawnInteriorObject( "Carp", "-0.869814 -0.450615 -3.098000", "-3.000006 0.000000 90.000000", 0.35 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.159371 -2.416870 -1.166253", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.304961 -2.449997 -0.788165", "5.003724 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.747044 -2.459999 0.212906", "-68.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.418234 -2.449997 -0.918089", "-30.000006 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.463873 -2.449997 0.929435", "-159.000000 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.492394 -2.449997 -0.347758", "171.001678 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.266038 -2.416870 3.674526", "0.000000 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.418957 -2.449997 3.249241", "0.000000 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_debris_small_house", "-2.265800 -2.480003 1.132342", "0.000000 0.000000 0.000000", 0.5 );

		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.300001 0.350021 -8.500013", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.249996 0.150009 -2.050002", "90.000000 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-4.300001 0.000000 2.209999", "89.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.300001 -2.700165 8.770005", "-90.000038 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-4.100000 0.550034 8.600003", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.550000 2.600159 -4.350000", "-91.000031 -38.999992 -179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.532442 2.604607 0.304621", "-91.000038 -36.000011 -179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-1.450001 2.650162 0.350000", "-90.000000 -43.000011 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.500001 2.850052 8.850012", "-89.000000 -32.999989 -3.000001" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.799999 -2.699982 8.900003", "179.999985 81.999985 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.333602 -99.155983 8.574102", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.049999 -99.632385 8.750001", "90.000046 -58.000011 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.850002 -2.500092 8.550000", "-90.000000 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.800001 0.349960 -2.049999", "90.000015 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_E", "-3.479879 2.200035 0.969770", "69.785416 1.694371 -7.222174" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.862103 0.209206 4.322373", "-90.000000 -80.000008 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "0.850000 -2.450150 -2.149999", "84.000031 0.000000 -91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "0.971599 0.641396 -2.067057", "-84.374657 50.000004 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.113557 2.622612 4.917463", "89.000000 -48.000004 -179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.200000 2.650101 -1.899999", "90.000015 -44.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "2.749999 0.802307 -7.550010", "97.000038 -78.000015 173.999863" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.687283 0.724762 -2.256127", "-90.000015 22.999985 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-1.144691 2.692078 -4.008764", "95.000031 -36.999992 175.999908" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-1.220000 2.899994 -8.920002", "89.000046 -44.000008 174.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "2.660046 0.786980 -5.552777", "-170.999756 0.000000 89.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_D", "0.201669 1.233521 -8.653591", "90.000145 42.000015 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "0.349680 -1.127434 -8.700433", "-88.999977 0.000000 88.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.299999 -2.700226 -8.699991", "-0.000009 89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_C", "2.501377 0.419395 -2.993032", "-124.999939 -89.000046 179.999985" );
	}
}