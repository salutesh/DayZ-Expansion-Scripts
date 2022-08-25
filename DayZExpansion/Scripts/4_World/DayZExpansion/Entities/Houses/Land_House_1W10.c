/**
 * Land_House_1W10.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W10
{
	override bool HasInterior()
	{
		return false;
	}
	
	override void SpawnInterior()
	{
		/*
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-0.703067 -1.455147 -0.120000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_lekarnicka", "1.493512 -1.010002 -3.647858", "107.999962 -90.000000 -0.000009" );
		SpawnInteriorObject( "ExpansionPropScrewdriver1", "1.700579 -0.455086 -3.528016", "153.000031 0.000000 88.000046" );

		//if ( Math.RandomFloatInclusive(0, 1) <= 0.9 )
		//{
		//	SpawnInteriorObject( "bldr_case_a_open", "-3.797107 -1.450027 -0.152310", "0.000000 0.000000 0.000000" );
		//}
		//else
		{
			SpawnInteriorObject( "bldr_case_a", "-3.797110 -1.449997 -0.152310", "0.000000 0.000000 0.000000", 0.9 );
		}
		
		SpawnInteriorObject( "bldr_carpet_big", "-4.234338 -1.455147 -2.621881", "3.000001 0.000000 0.000000", 0.95 );
		SpawnInteriorObject( "bldr_lobby_chair", "-1.837407 -1.459999 -1.657410", "159.000031 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "-0.375936 -1.455147 -1.504080", "-43.000000 0.000000 0.000000", 0.35 );
		
		if ( Math.RandomFloatInclusive(0, 1) <= 0.7 )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.936434 -1.428764 -0.497685", "9.000001 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.086434 -1.449997 -0.673058", "-178.697968 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.686434 -1.449997 -1.793606", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.636434 -1.449997 -1.859962", "62.681744 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.264358 -1.428764 -3.497770", "81.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.113096 -1.449997 -3.476658", "-68.999969 0.000000 0.000000" );
		}
		
		SpawnInteriorObject( "bldr_bucket", "-2.411649 -1.455147 -3.744117", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "-0.290902 -1.459999 -3.749198", "-44.999989 0.000000 0.000000" );
		*/


	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.751482 3.744316 -2.088995", "179.999985 -48.000011 177.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.614371 3.895180 -2.013372", "179.999985 -39.000015 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-4.987113 2.295082 -4.038944", "-29.000006 35.000019 163.000397" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-4.129210 1.345024 1.270458", "179.999924 -86.000015 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.692300 3.745171 -1.665813", "0.000000 -46.999992 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-6.429515 3.295143 -1.131700", "179.999985 -21.999998 178.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "1.255244 3.245140 -1.228090", "-97.000015 7.000000 -51.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.122017 1.300079 -4.576165", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.788271 -1.300079 -4.575423", "179.999985 88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-4.641161 -1.200073 -4.575723", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-5.774588 1.400085 -1.012362", "-179.000061 58.000011 -179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.574752 1.250076 0.899821", "-178.000031 -86.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.900120 1.150070 0.649821", "-0.000005 -89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "4.016500 1.100067 -2.514243", "0.000000 1.000000 -73.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "2.869791 1.650101 -2.063877", "-89.999969 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.800478 0.800049 0.352483", "176.999969 -1.000009 165.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.000215 -1.400085 -4.443304", "179.999985 88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "3.785650 0.800049 0.290842", "179.999939 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.746162 -2.800171 0.243329", "179.999954 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-5.603276 -1.450089 -3.745330", "-90.000000 1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "5.829864 -3.250198 -0.487457", "-91.000031 -1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "5.982159 -0.950058 -3.279355", "0.000000 0.000000 90.000000" );
	}

}