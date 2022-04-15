/**
 * ExpansionSnappingPosition.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.BASEBUILDING, this, "GenerateDirections");
#endif

		int idx = 0;

		while ( Target.MemoryPointExists( "ex_snap_pos_" + Index + "_dir_" + idx ) )
		{
			ExpansionSnappingDirection direction = new ExpansionSnappingDirection;
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
				CF_Log.Warn( "[EXPANSION] \"" + allowPath + "\" and \"" + offsetPath + "\" don't have the same count!" );

				direction.Allow.Clear();
				direction.Offset.Clear();
			}

			Directions.Insert( direction );

			idx++;
		}
	}

	void DebugPrint()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Position Index: " + Index );

		EXLogPrint( "  Target: " + Target );

		EXLogPrint( "  Type: " + Type );
		EXLogPrint( "  Position: " + Position );

		for ( int i = 0; i < Directions.Count(); i++ )
		{
			EXLogPrint( "  Direction Index: " + Directions[i].Index );
			EXLogPrint( "	Position: " + Directions[i].Position );

			for ( int j = 0; j < Directions[i].Allow.Count(); j++ )
			{
				EXLogPrint( "	  Allow: " + Directions[i].Allow[j] );
				EXLogPrint( "	  Offset: " + Directions[i].Offset[j] );
			}
		}

		#endif
	}
};