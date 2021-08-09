/**
 * Land_House_2B01.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_2B01
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_kitchenstove_elec", "-5.100001 -4.170000 2.549999", "-89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchendesk_a", "-5.125000 -3.110000 1.300000", "0.000000 0.000000 0.000000" );

		
		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "2.740000 -4.185204 -0.617809", "-90.000015 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "2.740000 -4.185204 -0.617809", "-90.000015 0.000000 0.000000" );
		}

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "2.740000 -4.185204 0.287000", "-90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "2.740000 -4.185204 0.287000", "-90.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_luxury_lamp", "0.743000 -2.211052 0.069126", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "2.880000 -2.900177 1.001644", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "0.864743 -2.835121 -1.924000", "-90.000000 0.000000 0.000000" );
		if ( false )
		{
			//SpawnInteriorObject( "Land_House_2B01", "0.000000 0.000000 0.000000", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "-1.518206 -4.215000 0.798205", "178.999954 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_carpet_big", "-1.495404 -4.160000 2.251707", "-179.000061 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_chest_dz", "-1.278499 -4.420000 4.586817", "91.000015 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_tv_a", "-1.298150 -3.540768 4.600974", "-2.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_bar_shelves_dz", "2.868677 -4.210288 4.013774", "90.000000 0.000000 0.000000" );
		}
		else
		{
			//SpawnInteriorObject( "Land_House_2B01", "0.000000 0.000000 0.000000", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_SofaCorner", "1.550000 -3.760000 3.980000", "-90.000038 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_carpet_big", "1.000000 -4.160000 3.499998", "-92.000031 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_lobby_table", "0.850000 -4.170000 3.349998", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_bar_shelves_dz", "-1.649999 -4.170000 0.400000", "179.999939 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_chest_dz", "-0.990215 -4.424000 4.569054", "88.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_flowers_01", "-1.450000 -3.545000 4.700000", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "ExpansionPropRadio", "-0.568495 -3.479000 4.684999", "110.999969 0.000000 -90.000000" );
		}
		
		SpawnInteriorObject( "bldr_washing_machine", "0.447248 -4.185204 -2.490000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropShelf3", "-0.003656 -3.184998 -4.753203", "90.000038 0.000000 0.000000", 0.65 );
		SpawnInteriorObject( "ExpansionPropShelf1", "-1.705272 -3.081238 -2.539876", "-2.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropTackleBox1", "-1.325132 -3.219002 -2.578878", "13.000005 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBin", "-1.141713 -3.417999 -4.744457", "21.000227 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropPropaneTank", "-1.236888 -3.800003 -2.506244", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropMiniFridge", "1.085450 -3.685173 -4.685550", "-178.999969 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropGlue", "-1.500829 -3.897003 -2.500046", "37.000004 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropClamp7", "-1.459487 -3.079002 -2.514108", "-7.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropChair1", "2.713712 -3.535164 -4.364838", "0.000000 0.000000 16.000006", 0.85 );
		SpawnInteriorObject( "ExpansionPropChainsaw3", "-1.795790 -2.519997 -2.557465", "7.000000 0.000000 0.000000", 0.6 );
		SpawnInteriorObject( "ExpansionPropCase1", "-1.700279 -3.334465 -2.534434", "-3.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "-1.895289 -1.699997 -2.509501", "-10.000004 0.000000 0.000000", 0.6 );
		SpawnInteriorObject( "bldr_lekarnicka", "3.222000 -2.885124 -3.700000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "0.904202 -2.785118 -2.190000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-3.635000 -2.785118 -4.780014", "0.000000 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_class_case_a_closed", "-5.010000 -4.185204 4.139998", "-90.000015 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_class_case_a_open", "-5.010000 -4.185204 4.140000", "-90.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "ExpansionPropTrashBarrel1", "-0.405428 -3.808998 -2.595276", "29.000008 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_e_02", "-3.728860 -3.385155 -1.940000", "0.000000 0.000024 0.000000" );
		SpawnInteriorObject( "bldr_postel_manz_kov", "0.905674 -1.176048 3.960339", "91.000015 -0.000003 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "2.211682 -0.816254 4.069431", "0.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_case_bedroom_a", "2.214162 -0.816254 1.760292", "0.000000 0.000000 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_carpet_2_dz", "0.016212 -0.949997 3.059099", "85.000008 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_box_c_multi", "-1.433491 -1.040001 0.691877", "-8.000002 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_chair_a", "-4.202799 -0.839996 2.320776", "179.999924 -64.000015 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_chair_a", "-3.099095 -1.026039 1.991786", "-69.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "-1.831666 -0.726021 3.457557", "55.999992 -0.000003 0.000000" );
		SpawnInteriorObject( "bldr_picture_b_02", "-0.843240 0.059998 2.663648", "8.999957 90.000000 -0.000009" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.430358 -2.844032 0.138000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "2.051586 -1.035011 -1.471730", "-164.000046 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-4.139736 -3.953789 -1.633605", "178.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-4.430358 -2.844032 0.138000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "2.051586 -1.035011 -1.471730", "-164.000046 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_fridge", "-3.893411 -4.185204 4.726618", "179.990005 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "-4.139736 -3.953789 -1.633605", "178.000000 0.000000 0.000000" );

		if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.838000 -4.152000 -3.384195", "-30.999994 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.003518 -4.152000 -4.517037", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.101892 -4.158821 -3.596252", "22.020298 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.909774 -4.158821 -4.289657", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.646862 -4.158821 -4.285820", "-52.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.679503 -4.158821 4.620475", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.629502 -4.158821 4.329971", "171.689499 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.929502 -4.158821 3.473531", "26.000011 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.740958 -4.158821 2.910813", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.102321 -4.158821 3.754541", "3.993680 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.963519 -4.158821 4.438551", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.890832 -4.158821 1.919896", "-136.901138 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.016644 -4.158821 2.524979", "0.000000 0.000000 0.000000" );
		}
	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.636841 4.923363 1.498337", "90.000038 -42.999989 -179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "0.149878 4.323326 -1.736701", "49.000008 -31.999987 -156.000443" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.139758 4.974312 1.306858", "-90.000015 -39.999996 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.166952 4.874306 -3.210580", "-90.000000 -35.999992 178.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.262710 4.915337 -0.519480", "0.000000 -41.999996 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.817163 4.849991 -0.993940", "179.999985 -44.000027 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-3.211171 3.274208 2.543723", "179.999954 0.000000 -57.999981" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-1.367709 3.299896 -5.456716", "89.999969 -25.999994 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.043865 2.299835 -5.396230", "0.000000 0.000000 -89.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-1.451305 -3.400208 -5.441741", "-90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-1.246441 -0.900055 -5.441741", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-0.547148 -0.950058 -5.441741", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-0.817523 -1.050064 -5.391741", "0.000000 0.000000 88.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-0.609458 -0.350021 -5.391741", "0.000000 0.000000 -22.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-3.130355 -3.950241 -5.391741", "-179.999985 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.055203 4.000244 -5.456700", "-90.000038 -50.000008 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.438125 -4.150253 -5.234539", "-91.000015 86.000015 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.338125 0.900055 1.841504", "-90.000000 -83.000015 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "3.188125 0.150009 -2.099849", "0.000000 7.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "3.629963 1.224083 -3.770973", "-90.000000 -83.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "3.260000 -3.700226 -2.305686", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "3.350955 -2.800171 4.440157", "179.999985 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "3.260000 -3.750229 3.244959", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "1.143392 1.950119 5.355700", "-90.000015 54.000004 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "2.658561 0.000000 5.354351", "-90.000046 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-3.505211 1.800110 5.505700", "90.000000 -82.000008 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-0.716494 3.050186 5.355700", "89.999969 74.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.657745 -0.850052 5.558158", "179.999954 -87.000038 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-4.283467 -0.100006 5.502291", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "0.190563 -3.250198 5.473322", "90.000038 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-0.047353 -4.150253 5.552291", "179.999924 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-5.037319 -4.850296 5.425000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-0.707284 -2.100128 5.410000", "0.000000 0.000000 38.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.771646 1.150070 0.187404", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-5.771646 -3.200195 2.221364", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "-5.771646 0.550034 3.803508", "0.000000 0.000000 -90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-5.721646 -0.850052 4.503400", "-90.000038 0.000000 -91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-5.721646 -4.700287 2.301301", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-4.830000 0.950058 -0.647431", "177.999939 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-4.940000 -0.350021 -1.875401", "11.000006 0.000000 88.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "-5.022024 -3.700226 -0.698428", "-90.000038 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-5.127677 -4.000244 0.020000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_F", "-4.805948 -3.700226 -2.220000", "0.000000 0.000000 0.000000" );
		
	}

}
