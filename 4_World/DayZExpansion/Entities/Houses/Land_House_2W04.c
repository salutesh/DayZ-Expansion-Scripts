/**
 * Land_House_2W04.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_2W04 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-0.110000 -0.797401 4.128298", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_sink_a", "-0.040951 -0.735001 2.881175", "179.899979 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_postel_manz_kov", "1.031076 -0.947411 -0.334273", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "2.011207 -0.769028 0.866889", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchendesk_a", "6.553815 0.265450 1.480000", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "2.038327 0.602684 4.808000", "90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "4.070000 0.602684 5.164487", "0.000000 -0.000002 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.185590 -3.899933 -0.431000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "-1.496232 -2.000969 -0.352506", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight2", "-5.140000 -3.060791 0.383008", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "-5.140000 -3.070000 3.612909", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "Broom", "-2.710567 -3.994888 4.560000", "-52.999996 0.000000 -170.999954" );
		SpawnInteriorObject( "ExpansionPropTrashBarrel1", "-4.958586 -4.809998 3.188791", "45.000145 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_wall_board_02", "-5.258430 -5.168098 0.399763", "90.000015 0.000000 -2.000000" );
		SpawnInteriorObject( "bldr_box_c_multi", "-4.541504 -0.797401 4.345828", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.571212 -5.150192 0.133630", "-100.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.558117 -5.150200 0.264589", "6.003724 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.550347 -5.141716 0.236281", "-67.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.796433 -5.141716 1.399742", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.732806 -5.141716 1.216803", "115.999962 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.421600 -0.771019 6.428697", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-5.402020 -0.771019 6.639713", "65.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.222171 -0.771019 6.246327", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.512010 -0.771019 6.440610", "-78.994560 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.683939 -0.771019 5.363534", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.273058 -0.771019 6.483808", "52.000210 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.859836 -0.771019 6.892837", "-100.017715 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "7.674060 -0.771019 4.818557", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "7.246522 -0.771019 3.763286", "83.003708 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "7.524065 -0.771019 2.498942", "52.030415 0.000000 0.000000" );

		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "7.337287 1.300079 -1.277194", "89.999969 -60.000011 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "4.011000 2.050125 -1.327194", "-0.000005 -87.000038 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.537092 2.200134 -1.427194", "-0.000009 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-0.394264 0.750046 -1.077194", "90.000015 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "1.475266 -0.650040 -1.127194", "90.000015 9.000001 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.236116 -1.350082 -1.227100", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.168407 -4.950302 -1.276884", "-90.000038 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.617856 -4.950302 -1.326884", "-0.000009 79.000015 -179.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "7.776059 -4.500275 -1.176884", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.475326 -5.000305 -1.196884", "0.000000 0.000000 8.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "4.633968 -4.550034 -1.176884", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-6.716579 1.633919 -1.077194", "89.000000 7.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-2.122861 -4.800293 -1.127194", "90.000000 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-1.636382 -4.499969 -1.185000", "-179.999985 0.000000 -79.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-8.264177 0.150009 -1.226884", "-90.000076 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-10.453244 -4.850296 -1.226884", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-10.122670 1.000061 -1.276884", "89.999992 33.000011 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-9.012114 2.250137 -1.226884", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-10.844278 -5.000000 -1.176884", "0.000000 0.000000 7.000001" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-9.560163 -5.200012 -1.176884", "0.000000 0.000000 84.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_C", "-5.654091 -1.549789 -2.073054", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-5.232570 -6.150131 -1.185000", "0.000000 0.000000 -1.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_F", "-7.241625 0.200012 -1.177194", "0.000000 0.000000 97.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-2.532109 1.111450 -1.227100", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "0.443726 -0.650040 -1.177194", "-179.999954 0.000000 83.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "0.407409 -0.500031 -1.177194", "-179.999985 0.000000 162.999802" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.300375 6.064087 2.947865", "179.999985 -47.000011 178.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "2.108615 5.202965 1.865640", "-135.999969 -29.999989 152.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.885923 6.002419 2.833632", "179.999985 -39.000008 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-7.257667 5.000000 1.597363", "119.999969 -40.000011 -149.000076" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-8.023220 4.649979 1.356318", "-25.000008 -25.000008 151.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-6.496709 4.186249 0.846588", "90.000000 -2.000000 -46.999996" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.909010 -0.128914 -1.177194", "0.000000 0.000000 -92.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-2.057914 2.632813 -1.801430", "0.000000 -89.000046 3.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-1.630158 4.220749 0.724805", "75.000000 9.000002 -54.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.761025 5.045555 1.863728", "90.000015 0.000000 -49.999973" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.229168 2.400146 -1.821210", "-0.000009 -87.000038 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-9.164844 2.200134 -1.826884", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "8.106958 -1.150070 -1.246884", "90.000122 82.000008 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "11.772293 -1.350082 0.205061", "89.999992 -66.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.537071 2.250137 -1.150649", "90.000015 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.587071 2.300140 7.631250", "-90.000038 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "8.540896 1.900116 2.731147", "-90.000000 -88.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.587071 -5.250320 7.562932", "179.999985 0.000000 90.000122" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "8.437071 0.600037 6.263742", "-0.000005 -75.000008 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "8.687072 -1.100067 3.516053", "0.000000 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "11.289701 -2.100128 2.025415", "-90.000038 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "11.289701 -1.900116 0.590414", "-89.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "9.866003 -2.650162 3.533116", "-90.000000 14.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.487071 -5.600342 6.957433", "90.000015 0.000000 11.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_T", "8.860168 0.150009 5.430695", "0.000000 -44.999996 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "8.494818 1.600098 -0.699772", "-90.000015 0.000000 95.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "4.652759 4.902946 1.623610", "22.000015 -22.999996 -154.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "5.372730 4.449127 3.501074", "22.000023 -12.000000 26.825193" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "4.059150 5.011597 4.728841", "-52.999992 -25.999994 -152.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.911019 6.124184 3.359254", "0.000000 -47.000011 178.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-6.264077 4.784485 5.106054", "19.000029 -38.999996 167.999695" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-3.673819 6.132591 3.384133", "0.000000 -38.999992 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-1.671477 3.750488 6.093119", "-90.000000 0.000000 -53.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-11.320301 2.450150 -1.186460", "-90.000015 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-11.270301 2.150131 4.276051", "89.999992 -89.000046 -0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-11.370001 -0.900055 4.147601", "0.000000 68.000008 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-11.370301 -1.600098 1.420237", "90.000015 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-11.320301 -2.000122 0.550380", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-11.320302 -5.000305 5.158978", "90.000038 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-11.370302 -5.000305 7.634863", "90.000046 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-11.270302 -5.500031 5.296067", "-90.000000 0.000000 8.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-11.270301 -2.300140 0.230301", "-90.000000 0.000000 -26.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-11.160737 2.400146 7.693117", "179.999985 -89.000046 -0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-11.270301 2.850174 7.605939", "-90.000000 -1.000000 -162.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-11.225000 2.100433 6.809743", "-90.000000 0.000000 -175.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.881216 2.200134 7.687117", "179.999985 -86.000015 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-0.294160 0.000000 7.687117", "90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "-0.652110 -1.750107 7.687117", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.325948 2.150131 7.687117", "-0.000005 -89.000046 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.144535 2.050125 7.687117", "179.999985 -89.000046 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "6.728394 0.050003 7.587116", "-89.999992 -22.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.351477 -5.050308 7.693117", "-0.000005 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-6.350352 -4.900299 7.643116", "-179.999954 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.934917 -1.800110 7.693117", "-0.000009 -89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-10.571458 -1.200073 7.493116", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-10.926935 -6.000366 7.643116", "-179.999985 0.000000 -6.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.528612 -1.950119 7.637116", "-179.999985 0.000000 82.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-0.464675 -2.599854 7.637116", "-179.999985 0.000000 19.000006" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "5.895384 0.100006 7.637116", "0.000000 0.000000 -112.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_F", "0.230273 -2.700165 7.630000", "0.000000 0.000000 -179.999985" );
	}
}