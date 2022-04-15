/**
 * ExpansionLocatorStatic.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLocatorStatic
 * @brief		This class handle expansion locator system
 **/
class ExpansionLocatorStatic
{
	static int GetRadius( string type )
	{
		//! Most maps use these types.
		//! One exception is Rostow which prepends "Name", e.g. "NameCapital", "NameCity" etc.

		if ( type.Contains( "Capital" ) )
			return 1000;

		if ( type.Contains( "City" ) )
			return 500;

		if ( type.Contains( "Village" ) )
			return 200;

		if ( type.Contains( "Camp" ) )
			return 100;

		if ( type.Contains( "Local" ) )
			return 100;
		
		return 100;
	}

	static array<ref ExpansionLocatorArray> GetWorldLocations()
	{
		array<ref ExpansionLocatorArray> areaArray = new array< ref ExpansionLocatorArray >;

		string location_config_path = "CfgWorlds " + GetGame().GetWorldName() + " Names";
		int classNamesCount = GetGame().ConfigGetChildrenCount( location_config_path );
		
		for ( int l = 0; l < classNamesCount; ++l ) 
		{
			string location_class_name;
			GetGame().ConfigGetChildName( location_config_path, l, location_class_name );

			string location_class_name_path = location_config_path + " " + location_class_name;

			string location_type_path = location_class_name_path + " type";
			string location_name_path = location_class_name_path + " name";
			string location_position_path = location_class_name_path + " position";

			string location_type;
			string location_name;
			GetGame().ConfigGetText( location_type_path, location_type );
			GetGame().ConfigGetText( location_name_path, location_name );

			TFloatArray location_position = new TFloatArray;
			GetGame().ConfigGetFloatArray( location_position_path, location_position );
			
			if (location_position == null || location_position.Count() != 2)
			{
				//Error("ExpansionLocatorStatic::GetWorldLocations location_position.Count() != 2 count : " + location_position.Count());
				continue;
			}

			if ( location_type.Contains( "LocalOffice" ) )
				continue;
			
			if ( location_type.Contains( "ViewPoint" ) )
				continue;
			
			if ( location_type.Contains( "RailroadStation" ) )
				continue;

			if ( location_class_name.Contains( "ZalivGuba" ) )
				continue;
			
			if ( location_class_name.Contains( "Dubovo" ) )
				location_type = "Village";
			
			if ( location_class_name.Contains( "Novoselki" ) )
				location_type = "Village";
			
			if ( location_class_name.Contains( "Vysotovo" ) )
				location_type = "Village";

			areaArray.Insert( new ExpansionLocatorArray( Vector( location_position[0], 0, location_position[1] ), location_name, location_type ) );
		}

		return areaArray;
	}
}
