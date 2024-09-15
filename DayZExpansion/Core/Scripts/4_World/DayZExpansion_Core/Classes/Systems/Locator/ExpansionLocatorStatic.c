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

enum ExpansionLocationType
{
	CAMP = 1,
	HILL = 2,
	LOCAL = 4,
	LOCALOFFICE = 8,
	MARINE = 16,
	RAILROADSTATION = 32,
	RUIN = 64,
	SETTLEMENT = 128,
	VIEWPOINT = 256
}

/**@class		ExpansionLocatorStatic
 * @brief		This class handle expansion locator system
 **/
class ExpansionLocatorStatic
{
	static const ref TStringArray SETTLEMENTS = {"Capital", "City", "Village"};

	static int GetRadius( string type )
	{
		//! Most maps use these types.
		//! One exception is Rostow which prepends "Name", e.g. "NameCapital", "NameCity" etc.

		if ( type.Contains( "Capital" ) )
			return 1000;

		if ( type.Contains( "City" ) )
			return 500;

		if ( type.Contains( "Village" ) || type.Contains( "Suburb" ) )
			return 200;

		if ( type.Contains( "Camp" ) )
			return 100;

		if ( type.Contains( "Local" ) )
			return 100;
		
		return 100;
	}

	static array<ref ExpansionLocatorArray> GetWorldLocations(int include = 0)
	{
		if (!include)
		{
			include |= ExpansionLocationType.CAMP;
			include |= ExpansionLocationType.HILL;
			include |= ExpansionLocationType.LOCAL;
			include |= ExpansionLocationType.MARINE;
			include |= ExpansionLocationType.RUIN;
			include |= ExpansionLocationType.SETTLEMENT;
		}

		array<ref ExpansionLocatorArray> areaArray = new array< ref ExpansionLocatorArray >;

		string worldName = GetGame().GetWorldName();
		string location_config_path = "CfgWorlds " + worldName + " Names";
		int classNamesCount = GetGame().ConfigGetChildrenCount( location_config_path );
		
		worldName.ToLower();

		int index;
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

			if ( (include & ExpansionLocationType.CAMP) == 0 && location_type.Contains( "Camp" ) )
				continue;
			
			if ( (include & ExpansionLocationType.HILL) == 0 && location_type.Contains( "Hill" ) )
				continue;
			
			if ( (include & ExpansionLocationType.LOCAL) == 0 && location_type.Contains( "Local" ) )
				continue;

			if ( (include & ExpansionLocationType.LOCALOFFICE) == 0 && location_type.Contains( "LocalOffice" ) )
				continue;
			
			if ( (include & ExpansionLocationType.MARINE) == 0 && location_type.Contains( "Marine" ) )
				continue;
			
			if ( (include & ExpansionLocationType.RAILROADSTATION) == 0 && location_type.Contains( "RailroadStation" ) )
				continue;
			
			if ( (include & ExpansionLocationType.RUIN) == 0 && location_type.Contains( "Ruin" ) )
				continue;

			if ( (include & ExpansionLocationType.SETTLEMENT) == 0 && ExpansionString.ContainsAny(location_type, ExpansionLocatorStatic.SETTLEMENTS) )
				continue;

			if ( (include & ExpansionLocationType.VIEWPOINT) == 0 && location_type.Contains( "ViewPoint" ) )
				continue;

			if (worldName == "chernarusplus")
			{
				if ( location_class_name.Contains( "ZalivGuba" ) )
					continue;
				
				if ( location_class_name.Contains( "Dubovo" ) )
					location_type = "Suburb";
				
				if ( location_class_name.Contains( "Novoselki" ) )
					location_type = "Suburb";
				
				if ( location_class_name.Contains( "Vysotovo" ) )
					location_type = "Suburb";
			}

			areaArray.Insert( new ExpansionLocatorArray( Vector( location_position[0], 0, location_position[1] ), location_class_name, location_name, location_type, null, index++ ) );
		}

		return areaArray;
	}
}
