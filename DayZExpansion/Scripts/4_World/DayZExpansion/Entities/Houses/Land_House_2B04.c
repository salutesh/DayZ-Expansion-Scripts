/**
 * Land_House_2B04.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_2B04 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_couch_dz", "7.858236 -5.760002 -1.551729", "91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_b", "8.059421 -6.393219 -0.275359", "0.000000 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_couch_dz", "7.831921 -5.760002 1.092656", "90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_couch_dz", "6.863893 -5.525002 0.996979", "40.999992 -84.000008 0.000000" );
		}

		SpawnInteriorObject( "bldr_couch_dz", "7.817132 -5.760002 6.602267", "84.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "7.849232 -5.760002 4.110126", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "7.772810 -5.458954 5.308692", "0.000000 0.023906 0.000014" );
		SpawnInteriorObject( "bldr_flowers_02", "7.708210 -5.741486 2.534368", "133.999954 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "8.164967 -3.799927 -0.917621", "-89.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "8.164968 -3.800003 1.687030", "-90.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "ExpansionPropClampLight2", "8.164914 -3.800003 4.019087", "-90.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "8.164968 -3.800003 6.328601", "-90.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "bldr_switch_lights", "4.931149 -4.392883 -2.540000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "6.562960 -4.400002 7.555000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "3.975600 -2.541740 1.240000", "-90.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.096499 -1.691475 1.873713", "90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_almara", "3.090000 -5.790001 7.170000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tac_board_posters_level2", "1.924980 -4.791290 2.231000", "-179.999985 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_hospital_transport_bed", "1.880987 -5.760002 5.577057", "-72.000023 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_hospital_transport_bed", "1.793279 -5.385002 5.273361", "-34.999981 0.000000 93.000031" );
		}

		SpawnInteriorObject( "bldr_ch_office_b", "1.592054 -5.757004 3.252456", "-10.000004 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_debris_small_house", "6.352849 -5.750000 1.648614", "-27.000010 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "2.492734 -5.750000 5.315927", "-13.999990 0.018253 0.000000", -1 );
		SpawnInteriorObject( "bldr_case_d", "3.835136 -5.791290 2.895508", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "-4.839533 -5.041296 2.260000", "1-0.000009 0.000000 -0.000009" );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-4.852064 -5.760002 2.510000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_sink_a", "-3.034453 -5.741272 2.570000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_light_kitchen_03", "-3.133166 -3.840378 4.976767", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.570000 -4.490891 3.694890", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_sofa_leather_old", "-5.153320 -5.355003 7.072372", "179.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_table", "-5.175532 -5.789978 5.307296", "0.000000 0.018259 0.000000" );
		SpawnInteriorObject( "bldr_tv_a", "-5.374725 -5.310997 5.260661", "-176.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_radio_b", "-4.894532 -5.309998 5.335476", "81.000000 0.018259 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "-4.452822 -5.739967 5.538386", "98.000015 0.018259 0.000000", 0.75 );

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a_open", "-0.743982 -5.789925 6.940423", "-90.000015 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a", "-0.743982 -5.789925 6.940423", "-90.000015 0.000000 0.000000" );
		}

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a_open", "-0.743000 -5.790016 6.037000", "-90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a", "-0.743000 -5.790016 6.037000", "-90.000000 0.000000 0.000000" );
		}

		if ( Math.RandomFloatInclusive(0, 1) <= 0.1 )
		{
			SpawnInteriorObject( "bldr_kitchen_chair_a", "-2.408754 -5.529999 5.102157", "153.000031 -87.000038 -0.000009" );
			SpawnInteriorObject( "bldr_Hanged_doctor", "-1.919121 -1.551811 5.768857", "33.000004 0.000000 0.000000" );
		}

		if ( false )
		{
			SpawnInteriorObject( "bldr_case_a", "-0.895798 -5.750000 2.990505", "90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_case_a_open", "-0.895000 -5.750000 2.990000", "90.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "6.542438 -5.716621 -3.669066", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "6.937893 -5.716705 -4.655363", "152.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.661170 -5.716705 -3.555361", "-125.999931 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.098077 -5.716698 -3.555361", "174.023056 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "6.037092 -5.716698 -4.155361", "54.044987 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "7.277414 -5.716667 -5.890878", "-56.999989 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "4.753344 -5.648331 -3.889035", "-86.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "6.960003 -5.611847 -3.561886", "-72.000000 0.000000 0.000000" );

		if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.154728 -5.716591 -1.926661", "-98.000015 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "6.238522 -5.716591 -1.776661", "152.000031 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "4.827410 -5.716629 -0.940607", "100.999985 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.221295 -5.730003 7.018839", "8.000002 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "6.302547 -5.730003 6.955268", "80.000549 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.951083 -5.713509 7.033486", "45.999996 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.397326 -5.713524 6.435891", "0.000000 0.018253 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-5.537594 -5.713203 4.467727", "-34.000000 0.000000 0.000000" );
		}
	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-6.207916 -5.650345 -7.163248", "-0.000009 88.000015 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-3.157945 -3.849930 -7.113248", "-90.000000 -89.000046 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-1.413750 -5.650040 -7.113247", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-2.109950 -2.299835 -7.113248", "-0.000005 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "0.161128 -2.999939 -7.113248", "-90.000000 33.999992 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "6.925868 -2.949875 -7.113247", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.690191 -3.499908 -7.012582", "90.000000 16.000002 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "2.057259 -2.799866 -7.063248", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.431233 -0.300018 -7.163248", "-0.000005 -86.000015 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-5.794569 -5.000000 -7.020000", "0.000000 0.000000 4.999999" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-3.152614 -3.649918 -7.063200", "0.000000 0.000000 -36.999996" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-0.790664 -6.000061 -7.020000", "0.000000 0.000000 83.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-0.242197 -5.800049 -7.035000", "0.000000 0.000000 4.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "5.391171 -3.249893 -7.035000", "0.000000 0.000000 -63.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "7.617613 -3.599915 -7.030000", "0.000000 0.000000 -28.000006" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.342535 -6.000061 -7.030000", "-179.999985 0.000000 12.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.733975 -5.750046 -6.884946", "90.000046 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.735316 -0.249710 -6.925443", "0.000000 0.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "8.725360 -5.600037 -0.463151", "-89.999992 84.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "8.524417 -2.899872 -2.385541", "0.000000 -10.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "8.676102 -3.449905 1.241634", "0.000000 32.999989 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "8.679333 -2.749863 3.797196", "90.000046 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "8.584568 -2.449844 -4.505841", "0.000000 -9.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "8.625859 -0.749741 -0.521264", "90.000038 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.600000 -6.250076 -2.607317", "-90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.600000 -6.450089 -2.293427", "-90.000000 0.000000 12.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.628281 -2.899872 1.455165", "-90.000000 0.000000 86.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "8.623530 -5.800049 2.845184", "-90.000000 0.000000 24.000019" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "8.634855 -5.500031 -3.106940", "90.000000 0.000000 40.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "8.583583 -5.800049 -4.524868", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-6.405181 -0.299713 -6.979578", "89.000000 -88.000015 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-6.405181 -0.400024 -0.165995", "90.000046 -86.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-6.405181 -2.399841 1.331668", "-89.999992 -87.000038 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-6.255180 -3.449905 5.724836", "179.999954 10.000002 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-6.405181 -1.799805 5.638956", "90.000015 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-6.405181 -5.750046 4.311654", "89.000023 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-6.405181 -5.550034 7.146170", "-90.000038 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-6.205181 -3.749924 -1.173182", "179.999954 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-6.405181 -3.349899 2.473890", "179.999985 -21.000008 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-6.355181 -3.099884 -1.218780", "90.000000 0.000000 -170.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-6.310000 -6.900116 2.258986", "-90.000015 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_I", "-6.355181 -2.399841 2.756604", "-90.000000 0.000000 45.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-6.305181 -2.449844 -1.316916", "90.000038 0.000000 -13.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-6.242507 -5.750046 8.021053", "92.000015 0.000000 91.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.819005 -5.500031 7.971053", "179.999985 80.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-5.283878 -0.999756 8.021053", "-89.999992 -54.000015 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.446130 -0.399780 7.971053", "179.999954 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-3.891159 -3.149887 7.821053", "-90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-2.002380 -4.149948 7.971053", "90.000038 -30.999987 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "4.241073 -5.500031 7.950000", "179.999985 -89.000046 -0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.519701 -2.449905 7.921053", "-0.000050 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.285564 -6.800110 7.921053", "0.000000 0.000000 27.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.615098 5.050613 0.563179", "89.999992 -34.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-0.041409 4.499908 0.750060", "-107.000122 -32.999989 -171.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.044139 4.000549 -2.948169", "-30.000015 -20.999983 155.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.778130 2.899872 -4.568182", "128.000015 -32.000008 -153.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "1.028037 4.199951 -2.905129", "97.999985 -9.999996 -141.000076" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "1.558171 5.088310 -1.653742", "-153.999954 13.000019 151.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "3.243830 2.972183 -4.129673", "118.999962 -20.000000 -57.999996" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "1.391214 2.618645 -4.708323", "90.000015 0.000000 -54.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "4.910405 0.426666 -7.331431", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-2.881294 0.599731 -7.661814", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "6.616193 2.118027 0.097494", "0.000000 0.000000 -150.428940" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "7.820480 1.259132 -0.461634", "-12.000010 7.000002 -67.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "3.124242 4.018356 -2.676036", "-150.035675 11.062593 154.554977" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.992821 3.999939 3.731083", "-158.000031 -27.999992 -152.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.100270 5.159645 2.334157", "50.000004 -23.999992 -28.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-7.615669 -2.699860 -3.827634", "179.999924 -2.000000 -64.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-8.954621 -5.700043 -2.966330", "-90.000015 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-7.015580 0.660728 0.111938", "-89.999992 -82.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-6.837162 0.150009 1.228070", "-89.999992 -89.000046 179.999985" );
	}

}