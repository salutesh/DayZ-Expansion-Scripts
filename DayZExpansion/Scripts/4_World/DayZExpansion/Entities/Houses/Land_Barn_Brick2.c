/**
 * Land_Barn_Brick2.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_Barn_Brick2 extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}

	override void SpawnInterior()
	{
		if ( Math.RandomFloatInclusive(0, 1) < 0.75 )
		{
			SpawnInteriorObject( "bldr_misc_haybale", "-3.01352 -3.2999 6.25135", "0 0 0" );		
			SpawnInteriorObject( "bldr_misc_haybale", "-1.75369 -3.3 6.33637", "0 1.6809 1.25245" );		
			SpawnInteriorObject( "bldr_misc_haybale", "-0.420499 -3.3 6.27878", "0 0 65" );		
			SpawnInteriorObject( "bldr_misc_haybale", "0.941654 -3.3 6.38422", "0 1.6809 0.458367" );		
			SpawnInteriorObject( "bldr_misc_haybale", "-2.33558 -2.04993 6.26988", "0 4 13.4964" );		
			SpawnInteriorObject( "bldr_misc_haybale", "-1.11461 -1.9997 6.13935", "-1 6 -2" );	
		}
			
		SpawnInteriorObject( "bldr_tent_boxwooden", "2.08201 0.93 2.176", "14 -0.018598 0" );		
		SpawnInteriorObject( "bldr_foldingbed_open", "2.08082 0.4786 4.03713", "-9 0 0" );		
		SpawnInteriorObject( "bldr_candle", "1.49839 0.521004 3.35474", "0 0 0" );		
		SpawnInteriorObject( "bldr_bucket", "1.1684 0.467003 4.41587", "0 0.981402 -1" );

		if ( Math.RandomFloatInclusive(0, 1) < 0.65 )
		{
			SpawnInteriorObject( "bldr_misc_haybale", "2.59998 -3.3 -6.39831", "86 1.02737 0.45825" );		
			SpawnInteriorObject( "bldr_misc_haybale", "2.59862 -3.3 -5.01938", "-90 0 0" );		
			SpawnInteriorObject( "bldr_misc_haybale", "2.50919 -3.36848 -3.55845", "-92 0 -54.4391" );		
			SpawnInteriorObject( "bldr_misc_haybale", "2.45919 -2.06857 -5.72088", "90 3 77.7809" );	
		}

		SpawnInteriorObject( "bldr_misc_boardspack1", "2.60599 -3.04988 5.58747", "1 0 0", 0.50 );		
		SpawnInteriorObject( "bldr_misc_pallets3", "-2.98192 -3.3 2.96587", "92.0369 0 0", 0.50 );		
		SpawnInteriorObject( "bldr_misc_through_trailer", "1.31846 -3.70853 -2.20904", "-1.6e-05 0 -1", 0.65 );
	}
}