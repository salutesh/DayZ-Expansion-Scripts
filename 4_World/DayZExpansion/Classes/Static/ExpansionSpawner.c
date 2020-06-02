/**
 * SpawnBerezinoWrecks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

// ------------------------------------------------------------
// Expansion CreateExpansionTrader
// ------------------------------------------------------------
static ExpansionTraderBase CreateExpansionTrader( string type, vector position, vector orientation, array<string> gear )
{
	Object obj;
	ExpansionTraderBase trader;
	
	#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::CreateExpansionTrader - Start");
		#endif
	obj = GetGame().CreateObject( type, position, false, false, true );
	trader = ExpansionTraderBase.Cast( obj );

	if ( !trader && obj ) 
	{
		GetGame().ObjectDelete( obj ); 
		return NULL;
	}

	if ( trader )
	{
		trader.SetPosition( position );
		trader.SetOrientation( orientation );
		
		if ( gear )
		{
			for( int i = 0; i < gear.Count(); i++ )
			{
				trader.GetInventory().CreateAttachment( gear[i] );
			}
		}
	}
	#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::CreateExpansionTrader - End");
		#endif
	return trader;
}