/**
 * Land_House_1W09_Yellow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_1W09_Yellow extends BuildingWithFireplace
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
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.383186 3.245811 -1.459863", "89.999969 -40.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-3.425065 3.518639 4.104299", "90.000000 -40.000015 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.107292 3.145805 2.226408", "88.000023 -40.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.660125 3.318665 -4.719347", "-90.000038 -34.999992 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.763582 3.338005 1.553750", "-90.000000 -40.999996 178.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-7.427958 0.912537 4.836975", "142.000061 -68.000053 -93.000229" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-6.246202 1.570137 1.583366", "179.999954 -5.000000 -54.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.839552 1.750107 2.174736", "-12.000016 4.000000 -79.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-0.889199 1.795723 -0.595681", "57.000214 -8.000001 -173.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.678373 1.895729 -4.836409", "0.000000 0.000000 -167.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "0.383100 1.645714 3.597863", "-13.000230 0.000000 -168.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.562963 -1.850113 4.708426", "89.000015 86.000015 -178.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-7.312962 0.500031 1.123248", "90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-7.212961 -2.600159 1.625574", "90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-7.212961 -3.400208 3.286091", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-7.312962 0.450027 -4.828626", "10.000006 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-7.212961 -2.149826 -3.861436", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.638582 0.450027 -1.228045", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-3.590033 1.500092 -4.865680", "-90.000038 -28.999990 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.051785 0.550034 -4.915680", "-89.000000 -83.000015 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-5.935234 0.650040 -4.732983", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "3.210000 -3.300201 -4.042555", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "3.238024 -3.200195 -3.312565", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "2.600138 -2.850174 -4.800000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.226426 0.700043 4.223977", "-174.999939 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-1.661875 0.700043 4.901750", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.814565 1.100067 5.031040", "-89.999954 -79.000015 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-6.533463 -2.800171 4.751749", "179.999954 0.000000 0.000000" );
	}
}