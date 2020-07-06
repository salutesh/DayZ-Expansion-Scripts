/**
 * Land_House_1W12.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W12
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_kitchendesk_a", "2.584462 -1.450150 -4.470000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchenstove_elec", "4.425000 -2.513000 -5.769924", "90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "1.611134 -2.220001 -1.824355", "3.008216 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_radiator", "-0.760000 -2.412994 -7.601094", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.280382 -1.312927 -4.191000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_radio_b", "1.975053 -1.538002 -6.149156", "32.999943 90.000000 -0.000009", 0.85 );
		SpawnInteriorObject( "bldr_class_case_a_open", "0.478142 -2.513000 -8.397341", "-179.990005 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_conference_table_a", "4.260106 -2.160004 -3.543045", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_chair_a", "4.024138 -2.513000 -3.256124", "-10.000004 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_pc", "4.094769 -1.805000 -3.530536", "80.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "2.870000 -2.212982 -3.671311", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "2.929679 -1.608002 -3.670000", "3.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "5.309482 -2.513000 -3.406790", "103.000130 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_dz", "4.897027 -2.513000 -0.176094", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_vaulting_box_large", "-0.555051 -2.682999 -5.751089", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "4.945172 -1.750000 0.155000", "179.998993 0.000000 -0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.547000 -1.162918 -1.789685", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClampLight1", "1.792214 -0.595947 -3.831100", "0.000000 0.000000 -179.999998" );
		SpawnInteriorObject( "bldr_library_a_open", "8.230000 -2.513000 -0.490173", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_metalcrate_02", "8.090000 -2.625000 -1.635202", "4.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lekarnicka", "2.045000 -1.262924 -1.077381", "-89.999992 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_switch_lights", "2.390000 -1.149948 -3.574078", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "5.892594 -1.230011 0.140000", "-90.000000 0.000000 179.999985" );
		SpawnInteriorObject("bldr_picture_a", "8.250013 -3.220000 -1.649999", "-85.999977 4.000000 0.000000" );
		SpawnInteriorObject("bldr_picture_e_02", "8.140002 -3.240000 -1.600001", "-86.000008 7.990001 0.000000" );
		SpawnInteriorObject("bldr_picture_f", "7.893000 -3.150000 -1.700000", "-86.000000 19.000000 0.000000" );
		SpawnInteriorObject("bldr_picture_d_02", "7.399000 -3.062000 -1.570000", "-86.000000 41.999996 0.000000" );
		SpawnInteriorObject( "bldr_washing_machine", "8.013866 -2.563004 1.570499", "91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBag1", "5.305633 -1.940002 1.007426", "92.000015 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "5.372234 -0.815002 0.962972", "137.365479 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "5.421375 -0.815002 1.196102", "-59.020889 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBleach", "5.306304 -0.859001 0.801661", "22.999971 90.000000 176.999954" );
		SpawnInteriorObject( "ExpansionPropLawnMower1", "5.971201 -1.968002 3.890332", "-105.000076 0.000000 46.999992", 0.85 );
		SpawnInteriorObject( "ExpansionPropBin", "8.041769 -2.334999 2.270848", "20.000006 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropTrashBarrel1", "8.076779 -2.162979 3.222854", "-15.553958 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBattery2", "5.317580 -1.580002 1.169709", "76.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBucket2", "5.390973 -2.414001 1.586323", "30.013454 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropCase2", "5.323837 -1.580002 0.838920", "167.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropCase1", "5.330200 -1.640999 0.841000", "167.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropCooler2", "5.320428 -2.362991 1.233014", "-79.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_fluor_lamp", "6.911319 0.087158 2.069870", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "6.530795 -1.312927 3.992000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_debris_small_house", "6.275026 -2.513000 -1.675421", "-42.000000 -0.000005 0.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "0.836229 -2.513000 -6.901093", "37.999996 0.000000 0.000000", -1 );

		if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.049109 -2.495003 -2.738803", "-32.000004 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.925090 -2.503998 -2.259783", "-118.999855 0.000000 0.000000" );
		}

		if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "4.106231 -2.486618 -8.241795", "-110.999969 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.629013 -2.486618 -8.127385", "-172.989929 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.347440 -2.486618 -8.148737", "141.003052 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.988006 -2.503403 -7.934298", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.446815 -2.486618 -7.216006", "126.999931 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.132357 -2.486618 -7.077030", "-22.000011 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.832687 -2.486618 -6.736261", "0.000000 0.000000 0.000000" );
		}

		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.249397 4.599846 -2.048816", "91.000015 -45.999992 178.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "3.536237 4.631638 -2.778364", "-91.000015 -43.999989 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.033077 3.379906 1.174461", "-151.000076 -29.999989 -146.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.905101 3.125595 1.610770", "62.000004 -29.999987 146.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.389829 3.694786 -4.244061", "0.209472 -0.255324 -39.366241" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.683118 2.262848 -7.345118", "-30.000006 -23.999985 147.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.825775 3.187347 -6.599743", "113.000023 -30.000006 -145.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "3.935647 4.787445 -4.432101", "-32.000122 -40.000050 119.000130" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.783759 4.431328 -0.196419", "-58.999985 -29.999989 -149.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "5.375822 3.555153 -0.671678", "166.000031 -11.000002 -38.000011" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.996798 4.000000 -4.952877", "31.000002 -27.999992 -146.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-1.983188 0.350021 2.459854", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.760308 2.511963 2.401227", "-0.352706 -38.060596 179.613922" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "5.092026 2.700165 -6.753773", "125.999931 -25.999994 -60.999981" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.103289 -2.450150 -9.360670", "-4.000009 89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.288680 -0.250015 -9.210670", "0.000000 -85.000008 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "5.715554 -0.200012 -9.260670", "-3.000004 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.821516 -0.100006 -9.210670", "1.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.961796 -2.300140 -8.971944", "87.000046 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "8.861796 -2.250137 -1.889773", "-89.999992 86.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.811795 -0.100006 4.233508", "-90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "8.861796 -0.300018 -1.629885", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.208204 -2.250137 4.553281", "-92.000015 88.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.308204 -0.200012 1.442573", "90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.258204 -0.150009 -9.210316", "92.000031 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.022013 -0.200012 -3.410948", "-89.999954 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.908204 -0.100006 -5.075114", "4.000000 0.000000 91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_D", "-1.800599 0.000000 -1.663023", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_U", "-2.089858 -0.150009 -3.985945", "0.000000 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_V", "-3.492629 -1.700104 -1.605647", "0.000000 37.000008 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.212873 -0.050003 4.819332", "-177.000015 -85.000008 0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.469091 -0.150009 4.619331", "0.013450 -88.319603 -179.541260" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.262696 -2.300140 4.569331", "179.999985 81.000000 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-0.808204 -0.950058 -1.497338", "-179.999954 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.008204 -3.900238 -4.347733", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-1.008204 -3.200195 -5.551357", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "0.604361 -2.650162 -9.080000", "0.007812 0.980125 0.456656" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.739905 -2.700165 -9.110669", "0.000000 0.000000 -74.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.711796 -2.200134 -8.048508", "90.000000 0.000000 -76.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.711796 -3.550217 -7.244067", "90.000015 0.000000 25.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.711796 -3.550217 0.317703", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "4.149137 -2.900177 4.419331", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.988876 -3.900238 4.419331", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.010724 -0.100006 -6.963589", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.928497 0.350021 -5.431330", "89.999992 36.000027 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.040532 0.000000 5.581701", "179.999985 -87.000038 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.838353 0.250015 5.436375", "179.999985 33.000011 178.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "9.761800 -0.045708 0.790404", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "9.706438 -0.150009 -3.948161", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "9.644018 0.350021 -2.867815", "-89.999992 33.000011 179.999985" );
	}
}