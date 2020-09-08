/**
 * Land_House_1W02.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W02
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_switch_lights", "2.626000 -1.050728 -1.890000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_almara", "2.040000 -2.350822 0.859450", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropPetCarrier", "2.239837 0.046997 0.847720", "150.000031 0.000000 0.000000", 0.35 );
		SpawnInteriorObject( "bldr_luxury_lamp", "1.443000 -0.376671 -1.470000", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchendesk_a", "5.010000 -1.287971 -0.810000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchenstove_elec", "3.284248 -2.350822 0.840224", "-88.000046 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "3.178041 -2.350822 -1.538439", "-50.000004 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "bldr_fridge", "5.032772 -2.350822 1.331490", "-89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "2.980000 -1.100746 0.275000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_b_02", "2.930000 -1.600777 3.012859", "90.000038 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "bldr_picture_c_02", "3.954208 -1.750786 3.730000", "179.999939 0.000000 2.000000", 0.7 );
		SpawnInteriorObject( "ExpansionPropRadio", "3.437104 -1.415001 3.451519", "68.000000 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "bldr_case_a", "-1.040000 -2.350822 1.041775", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_conference_table_a", "0.730674 -2.008499 1.594642", "-91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_office_chair", "0.445010 -1.954765 1.308887", "-81.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_pc", "0.709576 -1.654999 1.419725", "-14.000003 0.000000 0.000000", 0.4 );
		SpawnInteriorObject( "bldr_BookTonyAndTheBeetles", "0.686501 -1.626076 1.968860", "120.000061 90.000000 179.999985", 0.4 );
		SpawnInteriorObject( "bldr_radio_b", "-0.047975 -1.752998 3.219602", "-101.999954 90.000000 -0.000009", 0.6 );
		SpawnInteriorObject( "bldr_flowers_02", "0.581824 -2.350822 3.196714", "-145.000031 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_picture_a_05", "-0.814122 -1.750786 3.730000", "-179.999985 0.000000 -1.000000" );
		SpawnInteriorObject( "bldr_picture_d_02", "-1.360000 -1.850792 2.667355", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_washing_machine", "-3.446941 -2.350685 0.660292", "-89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_bag_dz", "-1.896132 -1.933586 -1.130610", "103.000000 0.000000 0.001228" );
		SpawnInteriorObject( "ExpansionPropCooler1", "-2.573280 -2.211998 -1.304808", "10.000004 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropLawnMower1", "-3.580000 -1.800644 2.168647", "-179.999939 0.000000 51.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropBin", "-1.717258 -2.160004 1.139920", "84.000008 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropTackleBox2", "-3.514262 -2.239998 1.524618", "0.000000 0.000000 0.001228", 0.85 );
		SpawnInteriorObject( "ExpansionPropTool2", "-1.578240 -1.570000 1.670299", "92.000031 81.000031 0.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropWateringCan", "-1.623539 -2.138000 2.066170", "109.036865 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_luxury_lamp", "-0.795691 -0.126656 3.576000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "1.073000 -1.200752 -1.162000", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "0.960000 -0.126656 1.032856", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "-0.882445 -2.050804 -1.421983", "3.000007 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-0.180138 -0.600037 -1.845000", "-90.000000 0.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "5.400002 -0.749985 2.399999", "0.000000 0.000000 -90.000000", -1 );


	}

    override bool HasIvys()
    {
        return true;
    }
    
    override void SpawnIvys()
    {
		SpawnInteriorIvy( "bldr_plnt_ivy_S", "-1.100000 -0.143333 -1.999999", "90.000107 7.000001 -89.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.550003 0.750046 -2.049999", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "-0.450000 -0.899994 -2.000002", "0.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.099033 -2.000000 -1.955983", "79.999992 0.000000 -89.999832" );
		SpawnInteriorIvy( "bldr_plnt_ivy_T", "1.078985 0.129967 -3.005127", "106.999985 -45.000019 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_U", "0.898959 -0.100006 -2.702205", "0.000000 0.000000 89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_U", "3.118001 -0.050003 -2.588995", "179.999924 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.500000 0.350021 1.400000", "-90.000015 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "1.289997 1.700104 -0.950000", "-46.999992 19.000008 161.000107" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.100000 1.800110 -0.700000", "123.999985 -30.000063 -156.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.018001 1.878250 -0.321818", "-45.492138 -21.013626 161.482834" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-3.949997 0.177376 0.550000", "-88.000130 -67.000000 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.973038 0.904831 0.206119", "90.000107 24.000019 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.950000 -2.200134 0.700000", "90.000038 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "-3.899999 -1.000061 1.969999", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "-3.050000 -1.550095 -1.700000", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.623306 -2.334740 -1.895051", "-90.000084 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "3.967469 1.899437 2.223722", "136.999985 -21.999981 161.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.058193 1.717232 3.064634", "-55.000019 -21.999998 -160.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.786352 0.418663 4.641064", "0.000000 -30.897200 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "0.254960 -1.250076 5.931941", "90.036858 4.000000 91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "2.750000 -1.113556 5.910001", "90.000000 -81.000031 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.656842 -3.450027 3.483109", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-1.550000 -2.920212 5.100000", "-90.000015 0.000000 0.000000" );
    }

}