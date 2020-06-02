/**
 * Land_House_2B03.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_2B03 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_couch_dz", "-2.552167 -5.485001 -0.620409", "179.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_02", "1.671173 -4.819923 -0.956104", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_table", "-2.245295 -5.470047 0.746041", "6.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c_multi", "0.212439 -5.470047 5.646420", "96.000023 -0.000021 0.000000", 0.6 );
		SpawnInteriorObject( "bldr_box_c_multi", "2.285537 -2.230003 2.671303", "-99.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_box_c", "2.107005 -1.930000 5.969967", "2.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "-2.282951 -2.230003 -6.072443", "0.000000 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_class_case_a_closed", "-0.234107 -2.214455 -7.342526", "179.990005 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_class_case_a_open", "-0.234100 -2.214455 -7.342500", "179.990005 0.000000 0.000000" );
		}
		
		SpawnInteriorObject( "bldr_radiator", "-2.856544 -2.100067 -2.222446", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-1.140921 -0.964378 -1.173000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_bar_shelves_dz", "2.680000 -2.214455 -2.320362", "90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lobby_chair", "1.330001 -2.214455 -1.556650", "-23.999994 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_b", "-1.492763 -2.314461 -7.565033", "91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d_02", "3.980000 -4.819923 0.609728", "-90.000000 0.000000 0.000000" );

		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.851947 -5.600342 -6.801993", "-92.000031 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "7.752257 -2.300140 -1.829239", "-179.999985 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.064503 5.667435 3.613914", "90.000000 -38.999992 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.505523 4.307419 -0.254612", "179.706833 0.000000 153.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.059573 3.435547 -1.107232", "0.000000 -1.000000 -68.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.274144 4.700287 8.010305", "157.999847 -24.000019 154.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "1.557792 4.326759 8.870703", "-10.000004 -31.109472 8.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.083449 3.599915 10.035361", "-46.000027 -23.999985 -160.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "8.134166 1.853340 8.435431", "89.999992 -79.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.367050 5.680481 3.576304", "-89.999969 -42.000015 178.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-5.521535 3.199890 0.015350", "176.000000 0.000000 -158.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-8.478611 2.000122 1.837322", "-89.999969 -79.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-2.694124 4.479408 -1.535130", "-55.999985 -22.999998 168.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-0.385831 5.679482 0.625564", "-87.999977 -31.000006 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "7.666617 -5.100006 -6.419358", "-90.000038 0.000000 9.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "7.716617 -4.800293 -4.252023", "90.000000 0.000000 -17.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "7.700000 -3.350204 -3.943597", "-90.000000 0.000000 -14.000007" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "7.670000 -1.450089 -4.440429", "90.000000 0.000000 168.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "7.802257 1.250076 9.048598", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "7.752257 -0.500031 6.782072", "0.000000 29.999989 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "7.816617 -4.699982 10.052026", "-179.999985 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "7.752257 -1.200073 8.387487", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "7.740442 0.800049 7.852345", "90.000015 -4.000000 -94.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "7.660000 -2.650162 8.920713", "90.000000 0.000000 -67.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "7.710000 -3.300201 10.298261", "90.000000 0.000000 -27.000010" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "2.833337 -5.550034 13.028799", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "7.666842 -5.750046 12.928801", "-92.999985 0.000000 -92.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_C", "4.910802 -1.700104 13.178799", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "7.201644 -5.600342 12.928800", "-179.999985 0.000000 6.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "3.165359 -5.600342 12.928800", "-179.999985 0.000000 85.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "2.973166 -4.899994 12.928000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_I", "2.735385 -3.449905 13.078799", "0.000000 0.000000 -17.000006" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_I", "2.565000 -2.349838 12.928795", "0.000000 0.000000 12.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-4.208510 -2.249832 13.028795", "-90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-5.571379 -4.749985 13.028801", "90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.601053 -2.699921 12.990000", "0.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-2.985229 -3.299896 12.928797", "0.000000 0.000000 19.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_I", "-1.813421 -4.449966 12.928797", "0.000000 0.000000 -57.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-8.200745 1.150375 12.989735", "179.999954 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-8.247473 1.150375 7.881395", "90.000000 -87.000038 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-8.150744 0.250320 7.102193", "179.999985 -55.000019 -89.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-8.150744 -2.300140 8.607142", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_P", "-8.100744 -2.799866 0.470502", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-8.150744 -1.149765 -1.701970", "90.000046 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-8.100754 -4.649979 -3.377492", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-8.100700 -5.600037 11.108506", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-8.266221 1.100372 0.719611", "-90.000038 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-8.100744 -2.899872 -1.384998", "-90.000000 0.000000 27.000008" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "-8.050754 -3.899933 -0.638704", "-90.000000 0.000000 -52.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "-8.100754 -5.000000 -0.760494", "91.000015 0.000000 -10.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "-8.050754 -3.749924 -3.332835", "90.000000 0.000000 12.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-8.030000 -5.850052 10.656552", "90.000000 0.000000 -41.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-8.060000 -5.850052 8.516308", "90.000046 0.000000 8.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-8.050000 -4.299957 7.854674", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-8.055000 -2.999878 7.668620", "90.000000 0.000000 11.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-8.060000 -2.899872 1.053102", "90.000000 0.000000 -56.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-8.050744 -2.449844 0.424455", "95.000000 0.000000 37.000004" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.058680 1.200378 -6.931450", "0.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-4.118345 -2.049820 -6.881451", "-90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "3.230430 0.450333 -6.781450", "90.000015 7.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.289585 0.745659 -7.078074", "0.000000 75.000038 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "7.594521 1.100372 -6.931451", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_F", "4.007565 -2.099823 -7.081451", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "3.306358 -1.449783 -6.731450", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "5.593826 -2.349838 -6.838050", "-90.000000 -85.000008 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-6.818958 -2.499847 -6.881450", "-90.000015 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-6.215210 -2.649918 -6.781450", "-179.999985 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-4.686710 -2.449844 -6.781450", "0.000000 0.000000 134.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-2.743083 -4.799988 -6.790000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-2.890000 -3.349899 -6.790000", "-179.999985 0.000000 22.000011" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_H", "-1.315832 -5.950058 -6.790000", "-179.999985 0.000000 31.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "1.724753 0.550339 -6.780000", "0.000000 0.000000 157.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "3.922992 -5.000000 -6.790000", "0.000000 0.000000 -16.000011" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_J", "1.913479 -5.550034 -6.790000", "-179.999985 0.000000 -33.000004" );
	}
}

