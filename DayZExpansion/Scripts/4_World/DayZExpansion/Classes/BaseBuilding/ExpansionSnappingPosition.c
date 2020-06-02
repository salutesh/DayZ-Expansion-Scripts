/**
 * ExpansionSnappingPosition.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSnappingPosition
{
	Object Target;
	string Type;

	vector Position;
	int Index;

	bool IsHidden;

	ref array< ref ExpansionSnappingDirection > Directions = new array< ref ExpansionSnappingDirection >;

	void GenerateDirections()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSnappingPosition::GenerateDirections - Start");
		#endif
		int idx = 0;

		while ( Target.MemoryPointExists( "ex_snap_pos_" + Index + "_dir_" + idx ) )
		{
			ref ExpansionSnappingDirection direction = new ExpansionSnappingDirection;
			direction.Position = Target.GetMemoryPointPos( "ex_snap_pos_" + Index + "_dir_" + idx );
			direction.Index = idx;

			string allowPath = "cfgVehicles " + Target.GetType() + " ExpansionSnapping Pos_" + Index + " Dir_" + idx + " allow";
			if ( GetGame().ConfigIsExisting( allowPath ) )
			{
				GetGame().ConfigGetTextArray( allowPath, direction.Allow );
			}

			string offsetPath = "cfgVehicles " + Target.GetType() + " ExpansionSnapping Pos_" + Index + " Dir_" + idx + " offset";
			if ( GetGame().ConfigIsExisting( offsetPath ) )
			{
				GetGame().ConfigGetIntArray( offsetPath, direction.Offset );
			}

			if ( direction.Allow.Count() != direction.Offset.Count() )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[EXPANSION] \"" + allowPath + "\" and \"" + offsetPath + "\" don't have the same count!" );
				#endif

				direction.Allow.Clear();
				direction.Offset.Clear();
			}

			Directions.Insert( direction );

			idx++;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSnappingPosition::GenerateDirections - End");
		#endif
	}

	void DebugPrint()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Position Index: " + Index );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "  Target: " + Target );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "  Type: " + Type );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "  Position: " + Position );
		#endif

		for ( int i = 0; i < Directions.Count(); i++ )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "  Direction Index: " + Directions[i].Index );
			#endif

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "	Position: " + Directions[i].Position );
			#endif

			for ( int j = 0; j < Directions[i].Allow.Count(); j++ )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "	  Allow: " + Directions[i].Allow[j] );
				#endif

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "	  Offset: " + Directions[i].Offset[j] );
				#endif
			}
		}
	}
}