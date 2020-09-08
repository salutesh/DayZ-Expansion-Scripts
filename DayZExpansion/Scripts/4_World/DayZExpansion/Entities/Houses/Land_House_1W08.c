/**
 * Land_House_1W08.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W08
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		/*
		SpawnInteriorObject( "bldr_washing_machine", "5.450486 -1.879997 4.002013", "0.013444 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropPetCarrier", "6.452236 0.339996 3.596090", "97.000015 0.000000 0.000000", -1 );
		SpawnInteriorObject( "ExpansionPropBin", "5.034920 -1.690000 3.290000", "-74.000000 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "ExpansionPropBleach", "4.869251 -1.790001 3.994313", "-46.999992 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "ExpansionPropClampLight1", "-0.948960 0.250076 4.225716", "179.999924 0.000000 -179.999998" );
		SpawnInteriorObject( "bldr_switch_lights", "-0.524806 -0.680008 4.345000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_light_kitchen_03", "5.685676 -0.079971 2.425008", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "5.027799 -0.580002 0.175000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropMiniFridge", "6.426839 -1.379997 2.560578", "91.013565 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropCooler1", "6.476843 -0.759003 2.502398", "68.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropRakeset", "6.615000 -0.904999 0.602125", "0.000000 -1.000000 -77.000008", 0.6 );
		SpawnInteriorObject( "ExpansionPropShelf3", "6.447513 -0.899994 1.475894", "0.000000 0.000000 0.000000", 0.6 );
		SpawnInteriorObject( "ExpansionPropTackleBox2", "6.020502 -1.769997 3.395480", "167.000015 0.000000 0.000000", 0.65 );
		SpawnInteriorObject( "bldr_carpet_big", "0.414586 -1.880081 2.628098", "92.000031 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "ExpansionPropClampLight2", "1.701040 0.220001 4.225716", "179.999924 0.000000 -179.999998" );
		SpawnInteriorObject( "bldr_light_kitchen_03", "-0.373348 -0.079971 -0.800035", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "4.485000 -0.580002 0.965262", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "0.700000 -1.900009 -0.113271", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropStool1", "0.461775 -1.630066 0.446086", "15.000003 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "ExpansionPropStool1", "1.309284 -1.690002 0.696086", "-144.999954 -87.000038 179.999985", 0.85 );
		SpawnInteriorObject( "bldr_radio_b", "1.250000 -1.019997 -0.173864", "-88.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_picture_f", "1.340000 -1.230045 -0.160000", "-90.000000 0.000000 -0.000005", 0.75 );
		SpawnInteriorObject( "bldr_postel_manz_kov", "-0.625510 -2.080093 2.530237", "0.000000 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "bldr_picture_a_05", "-0.975743 -1.330048 4.350000", "179.998993 0.000000 -2.000000" );
		SpawnInteriorObject( "bldr_picture_e_02", "2.293639 -2.134003 4.000000", "179.998993 17.000002 0.000000" );
		SpawnInteriorObject( "bldr_BookBible", "2.217650 -1.390488 3.947868", "-135.000046 -90.000000 179.999985", 0.75 );
		SpawnInteriorObject( "bldr_switch_lights", "-3.495000 -0.529999 0.988131", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_Misc_WoodPile1", "-5.240204 -1.715187 3.430020", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_Misc_WoodPile1", "-5.236024 -1.715187 2.339580", "0.000000 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_flowers_02", "4.076983 -1.889999 0.690000", "135.013092 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropWateringCan", "3.776436 -1.667999 0.504563", "32.000015 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropPot", "3.242878 -1.764999 0.470656", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "2.723795 -1.880081 0.422746", "0.000000 0.000005 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "4.540000 -1.130042 3.537862", "-90.000000 0.000000 -0.000005" );

		
		if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.650174 -1.860001 0.884726", "81.013565 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "5.414528 -1.871002 1.670190", "-14.999996 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.753397 -1.839996 3.915024", "66.036804 -2.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.184708 -1.870003 3.283114", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.931610 -1.853699 -1.142116", "74.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.081610 -1.873001 -0.846036", "65.008049 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.431610 -1.853699 0.324902", "-178.999954 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.394180 -1.853699 0.833721", "88.000000 0.000005 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.204712 -1.873001 0.776683", "-0.396906 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.674734 -1.866745 0.107484", "-91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.766621 -1.866745 1.239827", "174.339813 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.716620 -1.889999 2.494856", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_l_long_pile_b", "-5.225860 -1.797279 0.765927", "-70.000145 0.000000 0.000000", 0.65 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-0.307970 -0.050003 4.488506", "90.000000 0.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-1.600000 0.000000 4.500002", "-90.000000 89.000046 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-1.200000 -0.200012 -1.966152", "90.000000 -22.999998 -90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "3.549998 -0.300018 -0.050001", "-90.000000 0.000000 -90.000000", -1 );
		*/

	}

    override bool HasIvys()
    {
        return true;
    }
    
    override void SpawnIvys()
    {
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.150000 3.900238 2.149999", "179.999985 -32.000008 178.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "1.737886 3.771080 2.141673", "-38.999992 -44.000027 119.000633" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "6.100008 3.000061 1.280000", "147.000061 -36.000011 -162.000153" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.099998 4.026451 2.229999", "0.000000 -37.999996 -179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.658439 3.721077 2.423907", "0.000000 -46.999992 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-5.872385 0.957863 2.890089", "-85.999962 -83.000015 171.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.846422 2.007927 0.403568", "-91.000038 -32.999989 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "5.155207 3.571068 2.563438", "0.000000 -40.999996 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.955190 1.350082 4.760669", "179.999985 -89.000046 -0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.252675 -1.650101 4.710669", "179.999985 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.005290 1.400085 4.510668", "92.000031 0.000000 89.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "4.990194 -1.400085 4.689829", "90.000038 -89.000046 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.633178 1.200073 4.710669", "0.000000 -89.000046 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "3.403596 -2.549850 4.610668", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "6.980488 -1.550095 4.699473", "179.999985 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_D", "-5.684559 0.407829 2.571080", "173.000031 68.999992 -97.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "2.923996 2.721016 3.663420", "179.999985 40.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "7.059760 1.500092 2.336418", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "7.050000 -1.450089 -0.049999", "-9.000004 0.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "7.159760 -1.450089 2.439713", "90.000038 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "7.109760 -1.650101 0.460391", "89.999992 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "3.972625 1.166664 -0.193122", "-179.999939 -89.000046 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "0.263451 -0.479179 -2.153188", "90.000015 -1.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-0.193683 1.400085 -2.147052", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.579382 -1.450089 -2.258708", "-0.000009 86.000015 -179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-2.132418 -1.550095 -2.147052", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-2.475186 -2.150131 -2.097052", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-3.689900 -1.800110 -1.472761", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-5.693120 0.407829 0.644833", "-90.000000 -89.000046 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-5.934958 0.557838 4.572236", "-179.999939 0.000000 89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-5.688090 -1.600098 -1.119318", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-4.138145 -2.650162 4.560668", "0.000000 -1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-0.121279 3.736031 1.798211", "179.999985 -24.000006 179.999985" );
    }

}