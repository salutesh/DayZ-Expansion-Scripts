/**
 * Land_House_1W03_Brown.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class Land_House_1W03_Brown extends BuildingWithFireplace
{
	override bool HasInterior()
	{
		return false;
	}

	    override bool HasIvys()
    {
        return true;
    }
    
	
	override void SpawnIvys()
	{
		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.300001 0.350021 -8.500013", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.249996 0.150009 -2.050002", "90.000000 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-4.300001 0.000000 2.209999", "89.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.300001 -2.700165 8.770005", "-90.000038 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-4.100000 0.550034 8.600003", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.550000 2.600159 -4.350000", "-91.000031 -38.999992 -179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.532442 2.604607 0.304621", "-91.000038 -36.000011 -179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-1.450001 2.650162 0.350000", "-90.000000 -43.000011 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.500001 2.850052 8.850012", "-89.000000 -32.999989 -3.000001" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.799999 -2.699982 8.900003", "179.999985 81.999985 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.333602 -99.155983 8.574102", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.049999 -99.632385 8.750001", "90.000046 -58.000011 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.850002 -2.500092 8.550000", "-90.000000 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "0.800001 0.349960 -2.049999", "90.000015 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_E", "-3.479879 2.200035 0.969770", "69.785416 1.694371 -7.222174" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.862103 0.209206 4.322373", "-90.000000 -80.000008 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "0.850000 -2.450150 -2.149999", "84.000031 0.000000 -91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "0.971599 0.641396 -2.067057", "-84.374657 50.000004 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.113557 2.622612 4.917463", "89.000000 -48.000004 -179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.200000 2.650101 -1.899999", "90.000015 -44.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "2.749999 0.802307 -7.550010", "97.000038 -78.000015 173.999863" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.687283 0.724762 -2.256127", "-90.000015 22.999985 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-1.144691 2.692078 -4.008764", "95.000031 -36.999992 175.999908" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-1.220000 2.899994 -8.920002", "89.000046 -44.000008 174.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "2.660046 0.786980 -5.552777", "-170.999756 0.000000 89.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_D", "0.201669 1.233521 -8.653591", "90.000145 42.000015 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "0.349680 -1.127434 -8.700433", "-88.999977 0.000000 88.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.299999 -2.700226 -8.699991", "-0.000009 89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_C", "2.501377 0.419395 -2.993032", "-124.999939 -89.000046 179.999985" );
	}
}