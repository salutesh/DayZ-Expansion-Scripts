/**
 * ExpansionMarketWorldTraderZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketWorldTraderZone: ExpansionMarketTraderZone
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		m_ZoneName = "World";
		m_DisplayName = "World Trading Zone";
		m_FileName = m_ZoneName;

		Position = "7500 0 7500"; 	
		Radius = 50000;

		map< int, ref ExpansionMarketCategory > cats = GetExpansionSettings().GetMarket().GetCategories();
		for ( int i = 0; i < cats.Count(); i++ )
		{
			ExpansionMarketCategory cat = cats.GetElement( i );
			for ( int j = 0; j < cat.Items.Count(); j++ )
			{
				SetStock( cat.Items[j].ClassName, 250 );
			}
		}
		
		//! Default vehicle stock values
		TStringArray vehicles = 
		{
			"ExpansionTractor", 
			"ExpansionBus", 
			"CivilianSedan",
			"Sedan_02",
			"Hatchback_02", 
			"OffroadHatchback",
			"ExpansionUAZ", 
			"ExpansionUAZRoofless", 
			"ExpansionUAZCargo", 
			"ExpansionUAZCargoRoofless", 
			"ExpansionVodnik", 
			"ExpansionMh6", 
			"ExpansionUh1h",  
			"ExpansionMerlin", 
			//! "ExpansionAmphibiaBoat", 
			"ExpansionUtilityBoat",
			"ExpansionZodiacBoat"
		};

		for ( int v = 0; v < vehicles.Count(); v++ )
		{
			SetStock( vehicles[v], 5 );
		}		
	}
}