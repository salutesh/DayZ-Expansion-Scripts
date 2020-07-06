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
	
	bool InteriorBuilding;				// Interior inside building mapping switch. It's not linked to UseCustomMappingModule 
	bool Ivies;							// Ivies mapping switch

	// ------------------------------------------------------------
	// Expansion ExpansionMapping
	// ------------------------------------------------------------
	void ExpansionMapping()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMapping::ExpansionMapping - Start");
		#endif
		
		Mapping = new TStringArray;
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
		ObjectsToDelete.Copy( s.ObjectsToDelete );

		InteriorBuilding = s.InteriorBuilding;
		Ivies = s.Ivies;
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapping::Defaults - Start");
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
		Mapping.Insert( "NadezhdinoNoCollision" );
		Mapping.Insert( "Radom_StuffNoCollision" );
		Mapping.Insert( "RoadsNoCollision" );
		Mapping.Insert( "Solnechniy_GrassNoCollision" );
		Mapping.Insert( "StaroyeNoCollision" );
		Mapping.Insert( "StaryYarBkrNoCollision" );
		Mapping.Insert( "SvetloyarskNoCollision" );
		Mapping.Insert( "SvetRoadForestNoCollision" );
		Mapping.Insert( "TisyNoCollision" );
	
		#ifdef EXPANSION_DISABLE_MAPPING
		
		UseCustomMappingModule 			= false;
		InteriorBuilding 				= false;
		Ivies							= false;
		
		#else
		
		UseCustomMappingModule 			= true;
		InteriorBuilding 				= true;
		Ivies							= true;
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapping::Defaults - End");
		#endif
	}
}