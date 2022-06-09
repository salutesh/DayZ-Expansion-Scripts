/**
 * ExpansionMapping.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapping
{
	bool BuildingInteriors;				// Interior inside building mapping switch
	autoptr TStringArray Interiors;
	bool BuildingIvys;					// Exterior Ivies mapping switch

	// ------------------------------------------------------------
	// Expansion ExpansionMapping
	// ------------------------------------------------------------
	void ExpansionMapping()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionMapping");
#endif

		Interiors = new TStringArray;
	}

	// ------------------------------------------------------------
	// Expansion Copy
	// ------------------------------------------------------------
	void Copy( ExpansionMapping s )
	{
		Interiors.Copy( s.Interiors );

		BuildingInteriors = s.BuildingInteriors;
		BuildingIvys = s.BuildingIvys;
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Defaults");
#endif
		
		//! Set default markers depending on map name
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}

		//! Vanilla Maps		
		switch (world_name)
		{
			case "chernarusplus":
				ExpansionBuildingInterior();
			break;
			case "enoch":
				ExpansionBuildingInterior();
			break;
			case "deerisle":
				ExpansionBuildingInterior();
			break;
			case "namalsk":
			break;
			case "chiemsee":
				ExpansionBuildingInterior();
			break;
			case "rostow":
				ExpansionBuildingInterior();
			break;
			case "esseker":
				ExpansionBuildingInterior();
			break;
			case "valning":
				ExpansionBuildingInterior();
			break;
			case "banov":
				ExpansionBuildingInterior();
			break;
			case "sandbox":
			break;
			case "expansiontest":
				ExpansionBuildingInterior();
			break;
		}

		//! Keep at false so it will create less troubles for new server starting to setup everything
		BuildingInteriors 				= false;
		BuildingIvys					= false;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionBuildingInterior
	// ------------------------------------------------------------
	void ExpansionBuildingInterior()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionBuildingInterior");
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
	}
};
