/**
 * Land_House_1W04_Yellow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class Land_House_1W04_Yellow extends BuildingSuper
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
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.409184 -0.134506 0.354783", "-90.000015 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.400000 -2.550156 -3.100001", "-89.999954 84.000008 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.649999 0.250015 -9.250010", "0.000000 0.000000 89.999977" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "1.491185 -1.182076 -5.901031", "179.999954 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.967063 0.834297 -9.324474", "91.000031 -27.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.000003 -2.600037 -9.500001", "0.000000 89.000046 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "0.750000 -1.650101 -9.500001", "-90.000000 0.000000 94.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.250005 0.049942 -9.400009", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-5.150001 -0.100006 -2.450001", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-5.159508 0.151886 1.354815", "8.000011 1.000000 89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-4.299997 -2.600098 7.050000", "90.000038 88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-5.213439 -0.213188 4.122277", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.300000 -0.149948 6.649999", "179.999954 -84.000008 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.126521 0.201477 -9.462601", "-94.000031 34.000031 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.649999 2.931259 -6.100004", "89.999992 -50.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.503403 2.711403 -1.277132", "90.000000 -38.000015 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.400000 1.700043 -0.500000", "0.000000 -3.000000 -55.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.733271 2.920593 2.150462", "90.000038 -50.000004 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-2.949998 0.750107 4.999992", "-90.000000 -25.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.938738 3.019592 5.432559", "-90.000000 -32.000008 -2.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.000001 2.850052 -0.150000", "-90.000015 -38.000015 178.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.896445 3.065399 -5.564150", "-92.000031 -54.000004 179.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-5.833691 0.167999 -0.023561", "179.999985 -86.000015 -96.999931" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "1.351174 -3.811775 3.310491", "90.000000 0.000000 0.000000" );
	}
}