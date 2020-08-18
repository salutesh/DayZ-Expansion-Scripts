/**
 * Land_House_1W05_Yellow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifdef DAYZ_1_09
class Land_House_1W05_Yellow extends Land_House_1W05
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "6.050589 -1.169098 4.436679", "89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.850003 -1.150070 1.000000", "-89.999954 81.000000 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_S", "5.317667 -0.550034 -2.761996", "-175.991928 1.027026 90.458321" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "5.535602 2.381119 -0.569723", "-1.000000 -67.000000 -89.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.750001 1.700104 0.650000", "-90.000038 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.201045 3.631195 1.052144", "0.000000 -35.999992 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.120932 3.631195 0.703651", "179.999985 -35.999992 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.818295 3.581192 0.740488", "179.999985 -29.999989 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.883352 -0.900055 4.307810", "86.999969 83.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.883965 1.397034 4.247283", "95.999969 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-0.950000 1.100067 4.300000", "0.000000 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.049999 -0.100006 -6.300004", "-2.000099 -74.000008 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.150000 -1.300079 -7.950004", "90.000046 8.000001 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "3.550000 -0.549911 4.849999", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.141939 -3.400208 -4.532451", "-90.000038 1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "2.869404 -1.550095 -3.904123", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "3.960444 -1.250076 -3.925628", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "2.158547 -2.550156 -5.022088", "-90.000038 1.000000 0.000000" );
	}
}
#endif