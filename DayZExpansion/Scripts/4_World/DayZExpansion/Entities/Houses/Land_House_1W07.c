/**
 * Land_House_1W07.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_1W07 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "ExpansionPropLawnMower2", "5.870012 -2.580002 -3.045567", "-88.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropLeafBlower", "5.162772 -2.989998 -3.342390", "0.008217 0.000000 -15.000003" );
		SpawnInteriorObject( "ExpansionPropBin", "6.527078 -2.919998 -2.291005", "-79.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropSawset", "6.529241 -2.860001 -2.115000", "98.000015 4.000000 61.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "4.389848 -1.790161 -3.450000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_fluor_lamp", "3.386456 -0.279999 0.306172", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_fluor_lamp", "5.257717 -0.279999 0.305000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropAirtank", "6.400435 -2.750107 -1.360611", "84.999977 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBag1", "0.475634 -1.79742 2.08378", "93.000031 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBattery", "1.586007 -2.167999 0.983615", "-63.000011 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "1.586005 -2.144997 0.601953", "-53.999996 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "1.600974 -2.180000 0.397689", "-51.000008 -59.000000 90.000000" );
		SpawnInteriorObject( "ExpansionPropBoltCutters1", "1.775000 -2.688004 1.274830", "0.000000 0.000000 74.000000" );
		SpawnInteriorObject( "ExpansionPropCase1", "1.599760 -2.214996 1.423388", "-20.000006 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropCircularSaw3", "1.536700 -2.198997 1.268423", "179.999985 78.000015 179.999985" );
		SpawnInteriorObject( "ExpansionPropDrill", "1.568118 -2.584999 1.016960", "64.000008 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropGrill2", "2.040000 -2.533997 3.340000", "-4.000000 0.000000 0.000000", 0.15 );
		SpawnInteriorObject( "ExpansionPropGrinder", "1.555648 -2.535004 0.379609", "79.999992 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropHammer", "1.560724 -2.584000 1.159254", "81.999992 0.000000 80.000053" );
		SpawnInteriorObject( "ExpansionPropTackleBox1", "1.603455 -2.480003 1.961201", "-87.000008 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropTackleBox2", "1.562922 -2.879997 0.465879", "-8.000002 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropTrimmer", "3.642164 -2.500000 3.459501", "6.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropWelder", "4.475175 -2.110001 3.469958", "62.000011 0.000000 0.000000", 0.25 );
		SpawnInteriorObject( "ExpansionPropWorkbench3", "6.458562 -2.404999 0.869839", "2.000001 0.000000 0.000000", 0.35 );
		SpawnInteriorObject( "bldr_library_a", "-5.660000 -3.089996 2.803140", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_05", "-2.738966 -2.490204 2.010000", "0.000000 0.000000 -1.000000" );
		SpawnInteriorObject( "bldr_picture_c_02", "-1.510137 -2.499969 2.010000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d_02", "-1.661957 -2.640213 3.670000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_wall_unit_part_c", "-5.518693 -3.200134 -7.840000", "-135.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_debris_small_house", "5.074069 -3.089996 -0.340183", "52.000000 0.000000 0.000000", 0.30 );
		SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "-0.737158 -3.139999 -0.295512", "89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_table", "-2.123947 -3.100128 -0.305677", "94.000031 0.000000 0.000000", 0.55 );
		SpawnInteriorObject( "bldr_light_kitchen_03", "-3.995324 -1.529640 -1.070378", "0.000000 -0.000004 0.000000" );
		SpawnInteriorObject( "bldr_office_desk", "-7.018543 -2.723152 0.573463", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_ch_office_b", "-6.765910 -3.093002 0.617581", "80.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_pc", "-6.996204 -2.307472 0.623518", "168.000015 0.000000 0.000000", 0.70 );
		SpawnInteriorObject( "bldr_flowers_01", "-7.082703 -2.309898 0.085236", "-0.000029 -0.004193 0.396783", 0.70 );
		SpawnInteriorObject( "bldr_picture_a_05", "-5.441800 -2.290192 1.820000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-5.074082 -1.790161 1.805000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-3.414485 -1.066086 3.510000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.106453 -1.070000 3.510000", "0.036726 1.679372 1.252985" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.004241 -1.790161 3.651000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_03", "-7.460000 -2.640213 -0.853320", "90.000000 0.000000 1.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-7.301000 -1.019997 -0.850000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-7.301000 -1.019997 1.170000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-7.445000 -1.940170 1.226120", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_02", "-0.320000 -2.490204 -0.781434", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "-0.315000 -2.290191 1.540000", "-90.000000 0.000000 -55.000107" );
		SpawnInteriorObject( "bldr_washing_machine", "1.679874 -3.090240 -0.656053", "-88.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_library_a", "0.023906 -3.099884 -3.300872", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.020848 -1.740158 -3.450000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "0.708436 -1.750000 -1.445000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookTheArtOfWar", "-5.380000 -2.831306 -7.850000", "133.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookRobinsonCrusoe", "-5.462000 -2.839996 -7.855569", "134.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookRussianCheatSheet", "-5.554734 -2.828003 -7.783885", "134.000015 13.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookTheTimeMachine", "-5.601782 -2.579002 -7.738451", "95.999954 -90.000000 -0.000009" );
		SpawnInteriorObject( "bldr_BookYouth", "-5.311110 -2.359001 -8.119151", "134.000000 -2.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookTheHoundOfTheBaskervilles", "-5.363134 -2.360001 -8.090000", "134.000000 -3.000000 0.000000" );
		SpawnInteriorObject( "bldr_BookAndersen", "-5.428695 -2.362000 -7.997000", "134.000000 -24.999985 0.000000" );
		SpawnInteriorObject( "bldr_BookTheCosmicComputer", "-5.657079 -2.175003 -7.735052", "-0.000009 -90.000000 -0.000009" );
		SpawnInteriorObject( "bldr_chest_dz", "-3.413929 -3.250076 -7.896431", "90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tv_a", "-3.518229 -2.370003 -7.879685", "-177.999969 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_flowers_02", "-4.549439 -3.089996 -7.745287", "32.020256 0.979769 1.184370" );
		SpawnInteriorObject( "bldr_flowers_01", "-2.873704 -2.370003 -7.958917", "0.000000 0.000000 0.000000", 0.70 );
		SpawnInteriorObject( "bldr_carpet_2_dz", "-3.704025 -2.600098 -4.877588", "169.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-2.580294 -1.120003 -3.881000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-4.795206 -1.120003 -3.881000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-2.578564 -2.850113 -4.174925", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-4.925101 -2.849998 -4.165121", "-87.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "-3.692464 -2.390210 -3.725000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "-5.028728 -2.570000 -4.020710", "0.000000 0.000000 0.000000", 0.70 );
		SpawnInteriorObject( "bldr_switch_lights", "-2.039794 -1.890167 -3.741000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.719440 -2.349998 -3.741000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.720052 -2.349998 -3.741000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_light", "-2.530934 -0.528656 -6.677582", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a", "1.100000 -2.390207 -7.625536", "-90.000015 0.000000 -0.001006" );
		SpawnInteriorObject( "bldr_picture_e", "1.100000 -2.390207 -6.618151", "-90.000000 0.000000 -0.000005" );
		SpawnInteriorObject( "bldr_carpet_big", "-0.741666 -3.080002 -6.691559", "0.000000 0.000000 0.000000", 0.70 );
		SpawnInteriorObject( "ExpansionPropRadio", "-5.509931 -1.806999 -7.783000", "-40.000004 0.000000 0.000000", 0.40 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.333678 -3.070000 -2.869570", "71.008293 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "4.178169 -3.081001 -2.167770", "103.078224 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.889157 -3.081001 -1.908588", "160.026825 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-6.225200 -3.070000 -0.891000", "-74.999992 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-5.852793 -3.070000 -2.069479", "144.033340 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.831553 -3.063858 2.962284", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.781551 -3.080002 2.923225", "-74.999992 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.368447 -3.063858 3.021060", "0.000000 0.000000 0.000000" );

		if ( Math.RandomFloatInclusive(0, 1) <= 0.5 )
		{
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-7.591629 -1.350082 -2.188192", "0.000000 0.000000 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-7.600002 -1.660019 -0.100001", "0.000000 0.000000 -90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-6.055678 -1.750107 -6.221278", "0.000000 63.000011 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-4.111975 -1.760002 -8.400013", "-90.000000 0.000000 -90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-2.550001 -1.409981 -8.400002", "90.000000 0.000000 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "0.100000 -1.409981 -8.400002", "-90.000000 0.000000 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "6.852333 -1.760002 -1.990071", "0.000000 0.000000 -90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "6.900000 -1.860008 0.049999", "-0.000005 89.000046 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "6.849999 -2.100006 2.099999", "0.000000 62.000004 -90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "2.699990 -2.000000 3.799999", "-90.000000 0.000000 90.000000", 0.80 );
			SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-0.349999 -2.000000 3.800000", "90.000000 0.000000 90.000000", 0.80 );
		}
	}
	
	override void SpawnIvys()
	{
		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.399999 -0.300018 -8.600015", "-0.000009 -87.000038 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-1.150000 -0.350021 -8.500002", "179.999924 89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.960561 -2.840225 -8.597109", "89.999977 89.000046 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-6.376336 -2.900177 -8.591384", "0.000000 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-6.300001 -2.100128 -6.749997", "-0.000063 0.000000 89.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-6.475924 -0.350021 -3.657249", "-178.999924 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-6.849999 -4.350266 -3.649999", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.800001 -0.150009 -3.499998", "-90.000000 -89.000046 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-3.849999 -0.100067 -8.399998", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-7.850000 -0.050003 1.550000", "89.999969 89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_T", "-7.782883 -0.100006 -4.101577", "0.000000 -39.999985 -90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.200001 3.060135 -3.149998", "89.999992 -38.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.500000 3.110138 -3.300000", "-85.000008 -37.999981 176.999908" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.450001 3.160141 -9.300007", "-90.000038 -36.000011 -178.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_V", "-5.150003 1.260025 -2.750001", "-23.999996 57.000061 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.987360 2.750046 -0.310328", "179.999985 -34.999992 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-8.568520 3.010132 -0.148279", "88.000000 0.000000 -146.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_D", "-3.951692 2.660110 -1.082750", "-137.999985 -34.999992 -160.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.602659 3.010132 0.389933", "0.000000 -36.999992 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.093529 2.960129 0.205959", "0.000000 -30.999987 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "3.173145 0.159958 4.501530", "120.999969 -89.000046 61.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.760014 3.109375 0.240001", "0.000000 -32.999989 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.057254 2.924164 0.335179", "0.000000 -36.999992 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "7.750014 0.200012 4.550002", "179.999985 44.000027 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-7.721955 -2.800171 3.750351", "173.999939 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.706645 -0.250015 3.979857", "-0.000005 -88.000015 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.580365 -0.300018 4.029857", "100.000008 0.000000 91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.552512 -0.300018 3.979857", "0.000000 -89.000046 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "1.947094 -2.200134 4.009757", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "3.235792 -3.349899 3.909757", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-7.721955 -4.200256 1.308802", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "7.130863 2.550156 -0.120141", "1.000014 65.000015 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.055703 3.059380 -0.138406", "179.999985 -40.000011 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-1.407244 2.560104 -0.731072", "-51.000103 -62.000011 97.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "4.104457 1.860062 2.067760", "-89.000153 0.000000 -143.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "4.206161 1.650101 -2.200000", "-167.999939 -34.000011 170.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "1.660000 -0.150009 -5.409999", "94.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "1.412135 -3.999939 -4.552687", "90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.350002 -0.179558 -3.600000", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.150008 -0.300018 -3.750000", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.150001 -2.900177 -3.649999", "90.000000 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "7.050000 -0.300018 0.150000", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "7.050000 -0.600037 -1.050000", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "6.999999 -3.049820 3.709998", "90.000000 0.000000 6.000001" );
	}
}