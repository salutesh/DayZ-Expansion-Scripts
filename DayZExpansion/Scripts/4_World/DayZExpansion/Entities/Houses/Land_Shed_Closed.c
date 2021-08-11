/**
 * Land_Shed_Closed.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


class Land_Shed_Closed extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_misc_boardspack2", "-2.591732 -3.959999 5.239711", "-1.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_boardspack1", "-0.896565 -3.616547 5.323089", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_boardspack3", "2.553434 -3.922501 5.697881", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_boardspack3", "2.295122 -3.922501 1.876872", "93.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallets3", "-0.971886 -3.920998 2.888283", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "-1.697853 -3.974998 7.246000", "-3.500000 -67.000008 -3.000000", -1 );
		SpawnInteriorObject( "bldr_misc_pallet", "2.917167 -4.531441 -1.100595", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "1.390589 -4.531441 -1.081229", "93.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_Misc_BoxWooden", "2.583620 -3.959999 -1.098085", "2.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_Misc_BoxWooden", "1.514587 -3.959999 -1.078641", "93.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_Misc_BoxWooden", "1.509460 -2.986656 -1.070000", "2.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "1.476100 -4.531441 -3.628132", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "3.054248 -4.531441 -3.679946", "-179.000061 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "3.047094 -4.531441 -5.143027", "1.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_misc_pallet", "1.487252 -4.531441 -5.183509", "179.999924 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "1.792993 -3.939384 -3.905439", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "2.800571 -3.939384 -3.899000", "90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "2.800650 -3.939384 -4.939643", "-179.000061 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "1.623189 -3.939384 -5.041698", "13.000002 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "1.790000 -2.967003 -3.906312", "1.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "2.805000 -2.967003 -3.919529", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_tent_boxwooden", "4.196986 -4.050003 -4.957102", "-6.000000 3.000000 106.999985" );
		SpawnInteriorObject( "bldr_Rubble_Glass", "0.205928 -4.669998 -1.772746", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropShelf4", "-2.854000 -3.502998 -0.147762", "90.000038 0.000000 0.000000", 0.80 );
		SpawnInteriorObject( "ExpansionPropShelf4", "-2.854000 -3.502998 -1.960000", "89.999977 0.000000 0.000000", 0.80 );
		SpawnInteriorObject( "bldr_debris_small_house", "1.541226 -4.680809 7.162521", "39.000004 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "-2.037628 -4.680809 7.480845", "-41.000008 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "-0.223521 -4.680809 3.629865", "0.000000 0.000002 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "2.913821 -4.680809 4.197881", "33.000000 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "-2.758601 -4.680809 1.702230", "-114.999969 0.000002 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_debris_small_house", "2.715288 -4.680809 -1.556262", "-64.000008 0.000000 0.000000", 0.85 );
		SpawnInteriorObject( "bldr_Garbage_Pile3", "-2.443853 -4.406860 -4.438186", "155.000031 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "bldr_misc_pallets2", "-0.263907 -4.259857 -1.107293", "-82.999992 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-2.699999 -4.690002 -6.274389", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-1.250000 -4.639999 -5.300002", "-154.468613 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_locker_closed_v1", "0.313713 -1.290001 12.380000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_locker_closed_v2", "-0.087100 -1.290001 12.379509", "0.000000 -0.071194 0.000000" );
		SpawnInteriorObject( "bldr_locker_open_v1", "-0.500000 -1.290001 12.377720", "0.000000 -0.071194 0.000000" );
		SpawnInteriorObject( "bldr_locker_closed_v3", "-0.907236 -1.205002 12.376356", "0.000000 -0.071194 -26.000004" );
		SpawnInteriorObject( "bldr_locker_closed_v1", "-2.113752 -1.290001 12.380508", "0.000000 -0.071194 0.000000" );
		SpawnInteriorObject( "bldr_office_chair", "0.166169 -0.889999 11.196272", "114.999969 -0.071194 0.000000", 0.8 );
		SpawnInteriorObject( "bldr_office_chair", "-0.991976 -0.889999 10.983395", "176.000000 -0.075386 0.001768", 0.8 );
	}
}
