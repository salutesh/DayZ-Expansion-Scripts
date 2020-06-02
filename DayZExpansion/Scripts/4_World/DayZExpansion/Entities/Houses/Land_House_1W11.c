/**
 * Land_House_1W11.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_1W11 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_washing_machine", "-7.049802 -2.823242 -5.396524", "-178.009979 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_d", "-6.154066 -2.823242 -5.580000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c_multi", "-6.084473 -2.823242 1.099020", "96.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_sink_a", "-5.184223 -2.754997 -2.740691", "0.000000 0.000000 0.000019" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "-5.013886 -0.956192 -2.714652", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.927000 -1.449783 -3.871687", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "-6.415559 -1.875000 -5.546024", "-63.000011 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "ExpansionPropGrill2", "-7.312764 -2.260002 -2.984483", "-93.000031 0.000000 0.000000", 0.2 );
		SpawnInteriorObject( "ExpansionPropLawnMower2", "-7.382713 -2.309998 -1.691877", "179.000000 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropLeafBlower", "-7.484313 -2.723236 -3.887208", "-87.999977 90.000000 179.999985", 0.85 );
		SpawnInteriorObject( "ExpansionPropNailgun", "-5.992637 -2.320000 -5.553597", "-36.999996 -8.000001 -83.000008" );
		SpawnInteriorObject( "ExpansionPropPropaneTank", "-5.099422 -2.589996 -3.415290", "-125.000107 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "ExpansionPropBin", "-7.165731 -1.480003 -5.448231", "25.000011 0.000000 -2.000000", 0.75 );
		SpawnInteriorObject( "ExpansionPropSawset", "-6.112336 -1.955002 -5.575252", "-14.000003 0.000000 -2.000014" );
		SpawnInteriorObject( "ExpansionPropStrapleGun", "-6.318340 -2.320099 -5.558187", "171.057556 0.000000 -90.000000" );
		SpawnInteriorObject( "ExpansionPropTackleBox1", "-6.225676 -2.592003 -5.556704", "-173.000015 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropWacker", "-5.217970 -1.973190 -5.550000", "117.999954 77.000038 -0.000009" );
		SpawnInteriorObject( "ExpansionPropWateringCan", "-7.316982 -2.602997 -3.782598", "-156.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "1.651842 -2.637497 -4.165896", "0.000000 0.000000 0.000007", 0.8 );
		SpawnInteriorObject( "bldr_office_desk", "2.898573 -2.250015 -2.316430", "-89.991867 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_ch_office_b", "2.476169 -2.637497 -2.283519", "-68.000000 0.000000 0.000007" );
		SpawnInteriorObject( "bldr_pc", "2.943715 -1.834335 -2.455188", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_table_a", "-2.623588 -2.681686 -2.634609", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_office_chair", "-1.729379 -2.260002 -2.525363", "76.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_office_chair", "-2.594088 -2.260002 -3.247470", "172.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_chest_dz", "0.190973 -2.910004 -4.156224", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tv_a", "0.244793 -2.029999 -4.153681", "-86.000000 0.000000 0.000000" );

		//if ( Math.RandomFloatInclusive(0, 1) <= 0.65 )
		//{
		//	SpawnInteriorObject( "bldr_library_a", "-0.370000 -2.650002 -4.606670", "-90.000015 0.000000 0.000000" );
		//	SpawnInteriorObject( "bldr_library_a", "-0.370000 -2.650002 -3.704549", "-90.000000 0.000000 0.000007" );
		//}
		//else
		{
			SpawnInteriorObject( "bldr_library_a_open", "-0.370000 -2.650002 -4.606670", "-90.000015 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_library_a_open", "-0.370000 -2.650002 -3.704549", "-90.000000 0.000000 0.000007" );
		}

		SpawnInteriorObject( "bldr_case_bedroom_a", "0.891177 -2.450027 -5.902515", "88.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "-2.131421 -2.650002 -5.899922", "178.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "1.502338 -0.044998 -3.653153", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "-3.276864 -2.650002 -5.851659", "23.999994 0.000000 0.000007" );
		SpawnInteriorObject( "bldr_BookHeartOfDarkness", "0.881092 -2.141029 -5.792123", "-26.999964 90.000000 -0.000009", false );
		SpawnInteriorObject( "bldr_luxury_light", "-2.747698 -0.055000 -3.303000", "0.000000 0.000000 0.000007" );
		SpawnInteriorObject( "bldr_switch_lights", "2.571730 -1.237411 0.060000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_light_kitchen_03", "0.870027 -1.037399 1.120969", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d_02", "-0.547980 -1.937454 2.170000", "179.999878 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.139210 -1.350021 -0.260000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.630000 -1.423088 -3.846953", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.855982 -2.796860 1.432326", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-7.081194 -2.806999 0.109342", "134.004181 1.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.515946 -2.796860 -1.045606", "-12.424401 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-5.399986 -2.796860 -2.449511", "-69.580353 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-7.036565 -2.796860 -2.518798", "165.583481 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.812407 -2.806999 -3.675033", "-30.000006 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.993211 -2.796860 -4.913462", "-67.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.012120 -2.806999 -5.112040", "-121.977028 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "-7.433839 -2.727394 0.350130", "94.000031 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "-7.339090 -2.692009 -5.014409", "0.000000 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_a", "-5.747782 -2.692070 -0.826900", "99.000015 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "-5.583295 -2.727394 -2.254007", "-81.999992 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.817878 -2.611115 -5.535470", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.543154 -2.611115 -5.506649", "29.112072 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.371781 -2.611115 -5.791556", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.159611 -2.611115 -5.566742", "-14.898635 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.274729 -2.640999 -4.755585", "31.000006 0.000000 0.000007" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.993211 -2.806999 -3.675000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.342266 -2.629997 0.609269", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.292266 -2.629997 1.555526", "51.030415 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.092267 -2.629997 0.843319", "-85.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_debris_small_house", "1.542001 -2.680000 -1.327348", "119.999947 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "-2.962846 -2.675003 -4.660236", "-82.999992 0.000000 0.000000", -1 );

		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.491157 4.414261 -4.136718", "90.000000 -29.999987 177.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.556957 3.301254 2.925177", "90.000046 -52.000008 178.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.569092 3.271202 -3.834013", "90.000046 -57.999996 178.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.288625 3.556625 -3.533236", "-89.999992 25.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.882756 4.407173 -1.546357", "-89.000000 -31.999987 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-3.316393 2.418625 0.872479", "179.999954 0.000000 -37.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.874698 3.268677 -3.246453", "-89.999992 -60.000065 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "0.352940 1.650101 2.406607", "90.000015 -10.000000 -179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_C", "-0.014357 3.000183 2.856610", "-64.000008 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.330053 0.200012 2.578597", "179.999985 -89.000046 -0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.781804 0.400024 2.573687", "0.000000 -89.000046 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-1.647367 -1.900116 2.623687", "-90.000000 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.610445 -2.750168 2.628597", "179.999985 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "0.076852 -3.250198 2.528597", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_F", "2.946560 -2.899933 2.528597", "0.013445 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "3.250803 -1.649857 2.478597", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-4.082849 -0.399780 2.492972", "-179.999924 0.000000 -20.000006" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.827672 0.576965 -0.371107", "89.999992 50.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.727623 0.576965 2.609199", "-90.000038 -12.000002 -1.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-5.067611 0.426956 -3.270699", "-89.999992 -21.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.588803 2.450150 -6.583394", "90.000000 11.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.321198 0.800049 -6.634824", "179.999985 89.000046 -0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.897538 0.300018 -7.138738", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.590728 0.150009 -7.187829", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.922095 -2.800171 -6.649923", "179.999985 89.000046 0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.900001 -0.100006 -6.749997", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-7.713634 -0.900055 -6.165141", "-85.000008 67.000008 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-8.005194 -2.100128 -0.874596", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-8.105195 -0.700043 -2.902930", "90.000008 -75.000038 0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-5.041156 0.320503 2.736783", "-90.000000 0.000000 90.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.717176 -3.050186 -6.444015", "89.999969 89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.717176 -0.450027 -3.579130", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "3.667176 -3.101013 -0.446722", "90.000000 0.000000 0.000000" );
	}
}