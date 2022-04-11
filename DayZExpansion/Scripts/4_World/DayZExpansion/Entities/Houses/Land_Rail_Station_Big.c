/**
 * Land_Rail_Station_Big.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_Rail_Station_Big extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_hospital_bench", "-4.95164 -5.00142 -0.100008", "3 0 0" );
		SpawnInteriorObject( "bldr_hospital_bench", "-3.62062 -5.00142 -0.107163", "-3 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-4.97775 -5.00142 0.46", "-180 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-3.73493 -5.00142 0.482813", "178 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-5.01503 -5.00142 2.60301", "0 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-3.78895 -5.00142 2.58398", "3 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-5.00315 -5.00142 3.2045", "-178 0 0" );		
		SpawnInteriorObject( "bldr_hospital_bench", "-3.77527 -5.00142 3.22581", "179 0 0" );		
		SpawnInteriorObject( "bldr_wall_board", "-5.63972 -4.40138 4.08521", "90 0 0" );		
		SpawnInteriorObject( "bldr_wall_board_02", "-5.63972 -4.48734 1.3869", "90 0 -2" );
		
		if ( Math.RandomFloatInclusive(0, 1) <= 0.5 )
		{
			SpawnInteriorObject( "bldr_vending_machine", "1.48786 -3.94592 5.76696", "4 0 0" );	
		}
		else if ( Math.RandomFloatInclusive(0, 1) <= 0.5 )
		{
			SpawnInteriorObject( "bldr_vending_machine", "-0.211032 -4.54596 2.56835", "45 -90 180" );	
		}
			
		SpawnInteriorObject( "bldr_Rubble_Glass", "0.298956 -4.96838 1.78483", "0 5e-06 2.02656e-06" );		
		SpawnInteriorObject( "bldr_office_chair", "2.65351 -4.59132 -0.226282", "146 0 0" );		
		SpawnInteriorObject( "bldr_fluor_lamp", "-3.62779 -1.96 3.84408", "0 0 1.10269e-05" );		
		SpawnInteriorObject( "bldr_fluor_lamp", "-3.62 -1.96 -1.02338", "1 0 0" );		
		SpawnInteriorObject( "bldr_fluor_lamp", "0.024506 -1.9872 3.84659", "0 0 1.10269e-05" );		
		SpawnInteriorObject( "bldr_fluor_lamp", "0.495381 -4.95142 -0.850793", "30 0 180" );		
		SpawnInteriorObject( "bldr_flag_chernarus_pole", "-0.441229 -1.77363 6.89", "-90 0.000149 0" );		
		SpawnInteriorObject( "bldr_flag_chernarus_pole", "-3.00116 -1.78 6.89", "-90 5e-05 0" );		
		SpawnInteriorObject( "bldr_school_clock", "3.125 -2.63716 0.994094", "90 0 0" );		
		SpawnInteriorObject( "bldr_tac_board_posters_level2", "-5.65488 -3.95135 -1.78847", "-90 0 -1" );
	}
}