/**
 * ExpansionMapping.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapping
{
	bool UseCustomMappingModule;  		//Use per default to false, so not loading in main menu, only loading on server	 Same for InteriorBuilding
	
	autoptr TStringArray Mapping;
	autoptr TStringArray ObjectsToDelete;
	
	bool BuildingInteriors;				// Interior inside building mapping switch. It's not linked to UseCustomMappingModule
	autoptr TStringArray Interiors;
	bool BuildingIvys;					// Exterior Ivies mapping switch

	// ------------------------------------------------------------
	// Expansion ExpansionMapping
	// ------------------------------------------------------------
	void ExpansionMapping()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapping::ExpansionMapping - Start");
		#endif
		
		Mapping = new TStringArray;
		Interiors = new TStringArray;
		ObjectsToDelete = new TStringArray;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapping::ExpansionMapping - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Copy
	// ------------------------------------------------------------
	void Copy( ExpansionMapping s )
	{
		UseCustomMappingModule = s.UseCustomMappingModule;
		
		Mapping.Copy( s.Mapping );
		Interiors.Copy( s.Interiors );
		ObjectsToDelete.Copy( s.ObjectsToDelete );

		BuildingInteriors = s.BuildingInteriors;
		BuildingIvys = s.BuildingIvys;
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapping::Defaults - Start");
		#endif
		
		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		ExpansionBuildingInterior();

		//! Vanilla Maps
		if ( world_name.Contains( "chernarusplus" ) || world_name.Contains( "chernarusplusgloom" ) )
		{
			ExpansionMappingChernarus();
		}
		//! Only Chernarus have custom mapping so we will check only "chernarusplus" until we have custom mapping for other maps
		/*
		else if ( world_name.Contains( "enoch" ) || world_name.Contains( "enochgloom" ) )
		{
			ExpansionMappingLivonia();
		} else if ( world_name.Contains( "deerisle" ) || world_name.Contains( "deerislegloom" ) )
		{
			ExpansionMappingDeerIsle();
		} else if ( world_name.Contains( "namalsk" ) || world_name.Contains( "namalskgloom" ) )
		{
			ExpansionMappingNamalsk();
		}
		*/

		UseCustomMappingModule 			= true;
		BuildingInteriors 				= true;
		BuildingIvys					= true;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapping::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingChernarus
	// ------------------------------------------------------------
	void ExpansionMappingChernarus()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingChernarus::Defaults - Start");
		#endif
		
		Mapping.Insert( "BerezhkiCave" );
		Mapping.Insert( "Berezino" );
		Mapping.Insert( "Chernogorsk_Enhancement1" );
		Mapping.Insert( "Chernogorsk_Enhancement2" );
		Mapping.Insert( "Chernogorsk_Forest1" );
		Mapping.Insert( "Chernogorsk_Forest2" );
		Mapping.Insert( "Chernogorsk_Highschool" );
		Mapping.Insert( "DebugIsland" );
		Mapping.Insert( "Elektrozavodsk" );
		Mapping.Insert( "Evacuation" );
		Mapping.Insert( "Factory" );
		Mapping.Insert( "Gorka" );
		Mapping.Insert( "Kamenka" );
		Mapping.Insert( "Kamyshovo" );
		Mapping.Insert( "Krutoy_Cap" );
		Mapping.Insert( "Myshkino" );
		Mapping.Insert( "Nadezhdino" );
		Mapping.Insert( "Nizhnoye" );
		Mapping.Insert( "Novodmitrovsk" );
		Mapping.Insert( "Radom_Stuff" );
		Mapping.Insert( "Roads" );
		Mapping.Insert( "Sea_Platform" );
		Mapping.Insert( "Solnechniy" );
		Mapping.Insert( "Staroye" );
		Mapping.Insert( "StaryYarBkr" );
		Mapping.Insert( "Svetloyarsk" );
		Mapping.Insert( "Tisy" );
		
		Mapping.Insert( "BerezinoNoCollision" );
		Mapping.Insert( "Chernogorsk_Enhancement1NoCollision" );
		Mapping.Insert( "Chernogorsk_Enhancement2NoCollision" );
		Mapping.Insert( "Chernogorsk_Forest1NoCollision" );
		Mapping.Insert( "Chernogorsk_Forest2NoCollision" );
		Mapping.Insert( "Chernogorsk_Grass1NoCollision" );
		Mapping.Insert( "Chernogorsk_Grass2NoCollision" );
		Mapping.Insert( "Chernogorsk_HighschoolNoCollision" );
		Mapping.Insert( "DebugIslandNoCollision" );
		Mapping.Insert( "ElektrozavodskNoCollision" );
		Mapping.Insert( "EvacuationNoCollision" );
		Mapping.Insert( "GorkaNoCollision" );
		Mapping.Insert( "GrassNoCollision" );
		Mapping.Insert( "KamenkaNoCollision" );
		Mapping.Insert( "Kamyshovo_GrassNoCollision" );
		Mapping.Insert( "KamyshovoNoCollision" );
		Mapping.Insert( "Krutoy_CapNoCollision" );
		Mapping.Insert( "MyshkinoNoCollision" );
		Mapping.Insert( "NWAFenhanced" );
		Mapping.Insert( "NadezhdinoNoCollision" );
		Mapping.Insert( "Radom_StuffNoCollision" );
		Mapping.Insert( "RoadsNoCollision" );
		Mapping.Insert( "Solnechniy_GrassNoCollision" );
		Mapping.Insert( "StaroyeNoCollision" );
		Mapping.Insert( "StaryYarBkrNoCollision" );
		Mapping.Insert( "SvetloyarskNoCollision" );
		Mapping.Insert( "SvetRoadForestNoCollision" );
		Mapping.Insert( "TisyNoCollision" );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingChernarus::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingLivonia
	// ------------------------------------------------------------
	void ExpansionMappingLivonia()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingLivonia::Defaults - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingLivonia::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingDeerIsle
	// ------------------------------------------------------------
	void ExpansionMappingDeerIsle()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingDeerIsle::Defaults - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingDeerIsle::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMappingNamalsk
	// ------------------------------------------------------------
	void ExpansionMappingNamalsk()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingNamalsk::Defaults - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMappingNamalsk::Defaults - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion ExpansionBuildingInterior
	// ------------------------------------------------------------
	void ExpansionBuildingInterior()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBuildingInterior::Defaults - Start");
		#endif

		Interiors.Insert( "Land_Barn_Brick2" );
		Interiors.Insert( "Land_Barn_Metal_Big" );
		Interiors.Insert( "Land_Barn_Wood2" );
		Interiors.Insert( "Land_BarnMetal_Big_Grey" );

		Interiors.Insert( "Land_City_Hospital" );
		Interiors.Insert( "Land_City_School" );

		Interiors.Insert( "Land_Garage_Big" );
		Interiors.Insert( "Land_Garage_Row_Big" );
		Interiors.Insert( "Land_Shed_Closed" );

		Interiors.Insert( "Land_Village_Pub" );
		Interiors.Insert( "Land_House_1B01_Pub" );

		Interiors.Insert( "Land_House_1W01" );
		Interiors.Insert( "Land_House_1W02" );
		Interiors.Insert( "Land_House_1W03" );
		Interiors.Insert( "Land_House_1W04" );
		Interiors.Insert( "Land_House_1W05" );
		Interiors.Insert( "Land_House_1W06" );
		Interiors.Insert( "Land_House_1W07" );
		Interiors.Insert( "Land_House_1W08" );
		Interiors.Insert( "Land_House_1W09" );
		Interiors.Insert( "Land_House_1W10" );
		Interiors.Insert( "Land_House_1W11" );
		Interiors.Insert( "Land_House_1W12" );

		Interiors.Insert( "Land_House_1W03_Brown" );
		Interiors.Insert( "Land_House_1W04_Yellow" );
		Interiors.Insert( "Land_House_1W05_Yellow" );
		Interiors.Insert( "Land_House_1W08_Brown" );
		Interiors.Insert( "Land_House_1W09_Yellow" );
		Interiors.Insert( "Land_House_1W10_Brown" );
		Interiors.Insert( "Land_House_1W12_Brown" );

		Interiors.Insert( "Land_House_2B01" );
		Interiors.Insert( "Land_House_2B02" );
		Interiors.Insert( "Land_House_2B03" );
		Interiors.Insert( "Land_House_2B04" );

		Interiors.Insert( "Land_House_2W01" );
		Interiors.Insert( "Land_House_2W02" );
		Interiors.Insert( "Land_House_2W03" );
		Interiors.Insert( "Land_House_2W04" );
		
		Interiors.Insert( "Land_House_2W03_Brown" );
		Interiors.Insert( "Land_House_2W04_Yellow" );
		
		Interiors.Insert( "Land_Lighthouse" );
		Interiors.Insert( "Land_Power_Station" );
		Interiors.Insert( "Land_Rail_Station_Big" );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionBuildingInterior::Defaults - End");
		#endif
	}
};