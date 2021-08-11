/**
 * Land_House_2B02.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_2B02 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		if ( false )
		{
			SpawnInteriorObject( "bldr_case_a", "8.900000 -5.582123 1.730000", "90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_case_a_open", "8.900000 -5.582123 1.730000", "90.000000 0.000000 0.000000" );
		}
		SpawnInteriorObject( "bldr_kitchendesk_a", "6.862147 -4.519272 -2.186000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchendesk_b", "5.425000 -4.519203 -2.234000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_fridge", "4.581802 -5.582123 -2.209174", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tac_board_posters_level1", "1.989000 -4.382050 1.304659", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "2.402809 -5.582123 1.253062", "-91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "2.482841 -5.400711 2.613073", "179.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "2.439605 -5.300705 -0.127444", "-173.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "3.045141 -5.540001 1.301198", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "-1.195651 -5.582123 1.434685", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "1.088424 -5.582123 1.519128", "91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_library_a", "9.056000 -5.582123 4.140000", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_library_a_open", "9.056000 -5.582123 4.140000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_b", "6.837713 -5.775002 7.040418", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_b", "6.228000 -5.775002 7.040420", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_b", "5.618900 -5.775002 7.040420", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "6.043848 -4.620003 6.875196", "69.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_chair", "7.140469 -5.582123 5.044887", "139.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_chair", "5.330465 -5.582123 5.053340", "-151.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "5.558964 -1.970474 2.491516", "0.000000 -0.000024 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "5.549999 -1.970200 -0.109999", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "5.549999 -1.971001 4.979378", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "2.031000 -4.182037 -0.598726", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "2.031000 -4.199997 3.156478", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_office_desk", "-6.689494 -5.160004 2.766088", "-88.999985 1.000000 0.000000" );
		SpawnInteriorObject( "bldr_ch_office_b", "-6.983786 -5.573997 2.698826", "-73.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_pc", "-6.675475 -4.742828 2.702715", "0.000305 0.017449 0.999848" );
		SpawnInteriorObject( "bldr_Hanged_civil", "-7.643503 -1.289803 3.742987", "-157.000031 0.000000 0.000000", 0.1 );

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "-8.990000 -5.574997 1.696181", "90.000015 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "-8.990000 -5.574997 1.691800", "90.000000 0.000000 0.000000" );
		}

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a_open", "-8.990000 -5.574997 4.198301", "90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a", "-8.990000 -5.532120 4.198300", "90.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_lobby_chair", "-5.187646 -5.582123 1.973768", "112.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_chair", "-5.316109 -5.582123 3.335465", "65.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "-6.768241 -5.582123 6.836785", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "-4.054613 -5.582123 6.825537", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-5.347810 -5.400711 6.748425", "-86.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookCrimeAndPunishment", "-5.304736 -5.091713 6.702518", "-16.999979 -90.000000 -0.000009" );
		SpawnInteriorObject( "bldr_box_c_multi", "-7.641609 -5.582123 -1.208131", "-169.999969 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_flowers_01", "-7.980471 -5.582123 6.783949", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c_02", "-6.179180 -4.882080 7.199000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-2.194000 -3.457962 2.242558", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-2.194000 -3.458000 0.157662", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "-5.833985 -2.200996 3.764234", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "-5.933517 -2.200996 1.262458", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.050000 -4.232040 3.097571", "179.990005 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.050000 -4.232002 -0.624849", "179.990005 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d", "1.505000 -4.480000 1.380000", "-90.000000 0.000000 -0.000009" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "1.097705 -5.400711 2.667096", "0.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_case_bedroom_a", "1.123598 -5.450714 0.342408", "-4.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-1.134017 -5.450714 2.624170", "178.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-1.107397 -5.450714 0.274668", "179.999954 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "-1.221557 -5.182098 0.217133", "0.000000 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.360000 -3.557968 -2.901085", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "1.345000 -3.557999 -2.900000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.345000 -3.557999 -0.285330", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "1.348000 -3.557999 -0.280000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "1.347000 -3.557968 2.505860", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.370000 -3.557999 2.500000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.864433 -4.804405 6.349000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "0.859452 -4.805161 -6.471000", "-90.000000 0.000000 0.000000" );


	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-9.684803 1.700104 7.735931", "-0.000005 -89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.852374 1.800110 7.885933", "179.999985 -89.000046 0.000050" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-5.745567 -0.900055 7.735932", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-8.174976 -4.600281 7.735932", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-5.960055 0.600037 7.535932", "-90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-2.720020 -5.900360 7.715000", "-1.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-2.705423 -5.700348 7.690000", "0.038892 0.000000 11.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_A", "-9.662953 -6.450394 7.677674", "177.999954 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-8.143600 -1.600098 7.635932", "0.000000 0.000000 -23.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.683439 -5.950363 7.735932", "90.000000 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.373780 -5.950363 7.735932", "179.999985 85.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "4.154539 -4.750290 7.685932", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "6.538748 -3.150192 7.535932", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "6.602987 -1.500092 7.685932", "-0.000005 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.568430 -1.900116 7.585932", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "3.945481 0.200012 7.585932", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.373601 -6.450394 7.685932", "-179.999985 0.000000 4.999999" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "6.797737 -1.400085 7.635932", "0.000000 0.000000 -20.000021" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-1.214841 5.418549 1.000985", "-33.000084 -21.999998 -167.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.884882 5.050003 1.923884", "53.999985 -22.999985 163.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "-0.806900 4.499969 3.016300", "0.000000 -20.999996 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-1.785824 3.949936 4.286274", "-90.000000 -5.000000 -66.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "2.788366 4.889252 2.277801", "-65.000214 -12.000000 -159.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "2.585932 4.099945 3.822428", "-90.000000 0.000000 -67.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.992429 4.168472 4.170081", "-46.999977 -21.999985 -165.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-3.852715 5.145546 -0.088266", "-179.976410 -2.944072 -157.316650" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-6.028496 4.250565 3.630493", "-138.000061 -26.000008 -164.999863" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-1.255330 5.268539 -1.040525", "74.000008 7.000002 -61.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.206027 5.868576 -0.073105", "-128.999939 -16.000002 -18.000011" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.109777 5.818573 -0.383305", "90.000015 0.000000 -155.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-2.756772 5.050308 -1.932541", "93.000031 -2.999997 -159.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "2.867386 5.748993 0.095083", "-45.995853 20.010023 18.527864" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "6.160424 4.039253 -0.331140", "0.000000 0.000000 -64.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "3.300739 2.645325 -8.135799", "179.999985 -80.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-9.762954 -6.000366 7.514058", "177.999954 89.000046 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-9.712954 -2.500153 0.188112", "0.000000 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-9.862954 -2.900177 3.428867", "0.000000 15.000002 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "-9.712954 -2.400146 6.171294", "90.000038 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-9.512953 -0.300018 6.519411", "0.000000 -2.000000 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-9.662953 -5.950058 1.126270", "90.000038 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-9.712954 -0.900055 -0.257125", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-9.662953 -4.600281 4.651752", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-9.612953 -1.800110 0.772091", "-90.000000 0.000000 99.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-9.612953 -6.250076 -2.521821", "90.000000 0.000000 -8.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-9.708695 1.650101 4.032195", "-90.000038 88.000015 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-9.712954 -4.450027 -2.529405", "0.024020 -1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-9.612953 -1.900116 5.457938", "-90.000046 0.000000 -36.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-12.621868 0.200012 -3.913001", "-89.999992 -85.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-12.296451 -0.050003 -2.564512", "176.999954 0.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_T", "-10.012955 2.300140 8.106575", "123.999947 -50.000008 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-12.146450 -2.550156 -5.161480", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-12.146450 -2.450150 -4.975474", "90.000000 0.000000 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-12.096450 -3.150192 -6.226044", "90.000000 0.000000 -68.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_A", "-12.146451 -6.800110 -2.623494", "163.999939 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-11.995707 -5.450027 -2.496578", "0.000000 0.000000 10.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-9.378922 -6.150375 -7.577059", "-90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-9.831476 -5.550034 -7.477058", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-9.496017 1.800110 -7.820243", "90.000000 0.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.399083 1.500092 -7.770243", "179.999985 -88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-6.236209 -2.250137 -7.770243", "89.999977 -81.000031 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-6.379148 0.000000 -7.570242", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-1.666157 -0.850052 -7.770243", "-90.000038 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-4.147164 -4.750290 -7.770243", "-90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-6.327990 -3.950241 -7.620243", "90.000015 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-7.914136 -2.200134 -7.820243", "1.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-8.955141 -5.950363 -7.770243", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-8.779586 -5.650345 -7.730000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.340586 -5.950363 -7.720243", "179.999985 89.000046 -0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.712848 -6.000366 -7.770243", "-89.999977 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "3.662567 -2.150131 -7.765000", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "10.274812 -1.850113 -7.570242", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.132790 -1.427383 -7.770243", "179.999985 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-1.771265 -6.100128 -7.720243", "-179.999985 0.000000 15.000003" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "8.190660 -1.000061 -7.770243", "-90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.938810 -0.650040 -7.620243", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "5.977549 -2.350143 -7.620243", "90.000015 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.581799 -7.300446 -7.720243", "0.000000 0.000000 12.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "7.784378 -2.000122 -7.720243", "-179.999985 0.000000 -29.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "9.077857 -3.600220 -7.720243", "-179.999954 0.000000 6.000006" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "12.221550 -5.850357 -6.104679", "90.000038 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "12.171550 -5.900116 -5.648870", "-90.000000 0.000000 15.000003" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "12.171550 -7.350082 -4.802492", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.771852 -6.100372 -2.347788", "-90.000046 88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.671852 1.750107 7.673682", "90.000107 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "9.721852 1.550095 0.763426", "-90.000046 -86.000015 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "9.721852 -0.849747 -0.365808", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "9.721852 -4.750290 6.200560", "-179.999985 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "9.671852 -3.750229 2.218719", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "9.671852 -5.850357 0.681562", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "9.571852 -2.150131 4.399799", "1.000000 13.999999 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "9.671852 -6.500397 6.725977", "-90.000000 0.000000 6.000001" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "9.671852 -3.800232 -1.231755", "-90.000000 0.000000 32.000004" );
	}

}