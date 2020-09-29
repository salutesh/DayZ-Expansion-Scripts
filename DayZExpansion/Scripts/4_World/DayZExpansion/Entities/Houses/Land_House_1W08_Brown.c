/**
 * Land_House_1W08_Brown.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_1W08_Brown extends BuildingWithFireplace
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