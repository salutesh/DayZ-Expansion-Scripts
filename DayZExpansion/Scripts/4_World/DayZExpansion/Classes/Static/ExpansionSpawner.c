/**
 * ExpansionSpawner.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// ------------------------------------------------------------
// Expansion SpawnObject
// ------------------------------------------------------------
static Object SpawnObjectEx( string type, vector position, vector orientation, bool update = true )
{
	auto obj = GetGame().CreateObject( type, position, false, false, true );
	if (obj) 
	{
		obj.SetPosition( position );
		obj.SetOrientation( orientation );
		vector roll = obj.GetOrientation();
		roll [ 2 ] = roll [ 2 ] - 1;
		obj.SetOrientation( roll );
		roll [ 2 ] = roll [ 2 ] + 1;
		obj.SetOrientation( roll );

		if (update)
		{
			if ( obj.CanAffectPathgraph() )
			{
				obj.SetAffectPathgraph( true, false );
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
			}
		}
	}
	
	return obj;
}

// ------------------------------------------------------------
// Expansion SpawnClientObject
// ------------------------------------------------------------
static Object SpawnClientObjectEx( string type, vector position, vector orientation, bool update = true )
{
	if (IsMissionClient())
	{
		auto obj = GetGame().CreateObject( type, position, true, false, true );
		if (obj) 
		{
			obj.SetPosition( position );
			obj.SetOrientation( orientation );
			vector roll = obj.GetOrientation();
			roll [ 2 ] = roll [ 2 ] - 1;
			obj.SetOrientation( roll );
			roll [ 2 ] = roll [ 2 ] + 1;
			obj.SetOrientation( roll );

			if (update)
			{
				if ( obj.CanAffectPathgraph() )
				{
					obj.SetAffectPathgraph( true, false );
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
				}
			}
		}

		return obj;
	}
	
	return NULL;
}
