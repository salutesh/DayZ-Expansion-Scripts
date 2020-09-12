/**
 * ExpansionIcons.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

private static ref ExpansionIcons g_exp_MarkerIcons = NULL;

class ExpansionIcons
{
	private ref map< string, ref ExpansionIcon > m_IconMap;
	private ref array< ExpansionIcon > m_Icons;

	private ref array< string > m_OldIconSystem;
	
	// ------------------------------------------------------------
	// ExpansionIcons Constructor
	// ------------------------------------------------------------	
	private void ExpansionIcons()
	{
		m_IconMap = new map< string, ref ExpansionIcon >();
		m_Icons = new array< ExpansionIcon >();

		m_OldIconSystem = new array< string >();

		Generate();
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons Destructor
	// ------------------------------------------------------------	
	void ~ExpansionIcons()
	{
		delete m_IconMap;
		delete m_Icons;
		delete m_OldIconSystem;
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons Generate
	// ------------------------------------------------------------	
	void Generate()
	{
		AddIcon( "Arrow",			"Arrow",			EXPANSION_NOTIFICATION_ICON_ARROW			);
		AddIcon( "Error",			"Error",			EXPANSION_NOTIFICATION_ICON_ERROR			);
		AddIcon( "Airdrop",			"Airdrop",			EXPANSION_NOTIFICATION_ICON_AIRDROP			);
		AddIcon( "Car",				"Car",				EXPANSION_NOTIFICATION_ICON_CAR				);
		AddIcon( "Deliver",			"Deliver",			EXPANSION_NOTIFICATION_ICON_DELIVER			);
		AddIcon( "Radiation",		"Radiation",		EXPANSION_NOTIFICATION_ICON_RADIATION		);
		AddIcon( "Trader",			"Trader",			EXPANSION_NOTIFICATION_ICON_TRADER			);
		AddIcon( "Water 1",			"Water",			EXPANSION_NOTIFICATION_ICON_WATER_1			);
		AddIcon( "Infected 1",		"Infected",			EXPANSION_NOTIFICATION_ICON_INFECTED_1		);
		AddIcon( "Infected 2",		"Infected",			EXPANSION_NOTIFICATION_ICON_INFECTED_2		);
		AddIcon( "Skull 1",			"Skull",			EXPANSION_NOTIFICATION_ICON_SKULL_1			);
		AddIcon( "Helicopter",		"Helicopter",		EXPANSION_NOTIFICATION_ICON_HELI			);
		AddIcon( "Base",			"Base",				EXPANSION_NOTIFICATION_ICON_HOME			);
		AddIcon( "Boat",			"Boat",				EXPANSION_NOTIFICATION_ICON_BOAT			);
		AddIcon( "Fishing",			"Fishing",			EXPANSION_NOTIFICATION_ICON_FISHING			);		
		AddIcon( "Territory",		"Territory",		EXPANSION_NOTIFICATION_ICON_TERRITORY		);
		AddIcon( "Bear",			"Bear",				EXPANSION_NOTIFICATION_ICON_BEAR			);
		AddIcon( "Claw",			"Claw",				EXPANSION_NOTIFICATION_ICON_CLAW			);
		AddIcon( "Drip",			"Drip",				EXPANSION_NOTIFICATION_ICON_DRIP			);
		AddIcon( "Ear",				"Ear",				EXPANSION_NOTIFICATION_ICON_EAR				);
		AddIcon( "Eye",				"Eye",				EXPANSION_NOTIFICATION_ICON_EYE				);
		AddIcon( "Fireplace",		"Fireplace",		EXPANSION_NOTIFICATION_ICON_FIREPLACE		);
		AddIcon( "Heart",			"Heart",			EXPANSION_NOTIFICATION_ICON_HEART			);
		AddIcon( "Hook",			"Hook",				EXPANSION_NOTIFICATION_ICON_HOOK			);
		AddIcon( "Info",			"Info",				EXPANSION_NOTIFICATION_ICON_INFO			);
		AddIcon( "Knife",			"Knife",			EXPANSION_NOTIFICATION_ICON_KNIFE			);
		AddIcon( "Marker",			"Marker",			EXPANSION_NOTIFICATION_ICON_MARKER			);
		AddIcon( "Map Marker",		"Map Marker",		EXPANSION_NOTIFICATION_ICON_MAP_MARKER		);
		AddIcon( "Menu",			"Menu",				EXPANSION_NOTIFICATION_ICON_MENU			);
		AddIcon( "Moon",			"Moon",				EXPANSION_NOTIFICATION_ICON_MOON			);
		AddIcon( "Pen",				"Pen",				EXPANSION_NOTIFICATION_ICON_PEN				);
		AddIcon( "Persona",			"Persona",			EXPANSION_NOTIFICATION_ICON_PERSONA			);
		AddIcon( "Pill",			"Pill",				EXPANSION_NOTIFICATION_ICON_PILL			);
		AddIcon( "Questionmark",	"Questionmark",		EXPANSION_NOTIFICATION_ICON_QUESTIONMARK	);
		AddIcon( "Skull 2",			"Skull",			EXPANSION_NOTIFICATION_ICON_SKULL_2			);
		AddIcon( "Skull 3",			"Skull",			EXPANSION_NOTIFICATION_ICON_SKULL_3			);
		AddIcon( "Star",			"Star",				EXPANSION_NOTIFICATION_ICON_STAR			);
		AddIcon( "Sun",				"Sun",				EXPANSION_NOTIFICATION_ICON_SUN				);
		AddIcon( "Tent",			"Tent",				EXPANSION_NOTIFICATION_ICON_TENT			);
		AddIcon( "Thermometer",		"Thermometer",		EXPANSION_NOTIFICATION_ICON_THERMOMETER		);
		AddIcon( "Water 2",			"Water",			EXPANSION_NOTIFICATION_ICON_WATER_2			);
		AddIcon( "Book 1",			"Book",				EXPANSION_NOTIFICATION_ICON_BOOK_1			);
		AddIcon( "Book 2",			"Book",				EXPANSION_NOTIFICATION_ICON_BOOK_2			);
		AddIcon( "Ellipse",			"Ellipse",			EXPANSION_NOTIFICATION_ICON_ELLIPSE			);
		AddIcon( "Grab",			"Grab",				EXPANSION_NOTIFICATION_ICON_GRAB			);
		AddIcon( "Open Hand",		"Open Hand",		EXPANSION_NOTIFICATION_ICON_HAND_OPEN		);
		AddIcon( "Map",				"Map",				EXPANSION_NOTIFICATION_ICON_MAP				);
		AddIcon( "Note",			"Note",				EXPANSION_NOTIFICATION_ICON_NOTE			);
		AddIcon( "Orientation",		"Orientation",		EXPANSION_NOTIFICATION_ICON_ORIENTATION		);
		AddIcon( "Radio",			"Radio",			EXPANSION_NOTIFICATION_ICON_RADIO			);
		AddIcon( "Shield",			"Shield",			EXPANSION_NOTIFICATION_ICON_SHIELD			);
		AddIcon( "Snow",			"Snow",				EXPANSION_NOTIFICATION_ICON_SNOW			);
		AddIcon( "Group",			"Group",			EXPANSION_NOTIFICATION_ICON_GROUP			);
		AddIcon( "Infected 1",		"Infected",			EXPANSION_NOTIFICATION_ICON_INFECTED_1		);
		AddIcon( "Infected 2",		"Infected",			EXPANSION_NOTIFICATION_ICON_INFECTED_2		);
		AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_VEHICLE_CRASH	);
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons AddIcon
	// ------------------------------------------------------------	
	protected void AddIcon( string name, string localisation, string path )
	{
		if ( localisation == "" )
			localisation = name;

		ExpansionIcon icon = m_IconMap.Get( name );
		if ( icon == NULL )
		{
			icon = new ExpansionIcon( name, localisation, path );
			m_IconMap.Insert( name, icon );
			m_Icons.Insert( icon );
		} else
		{
			icon.Name = name;
			icon.Localisation = localisation;
			icon.Path = path;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons Count
	// ------------------------------------------------------------	
	static int Count()
	{
		if ( g_exp_MarkerIcons == NULL )
			g_exp_MarkerIcons = new ExpansionIcons();

		return g_exp_MarkerIcons.m_Icons.Count();
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons Get
	// ------------------------------------------------------------	
	static ref ExpansionIcon Get( int index )
	{
		if ( g_exp_MarkerIcons == NULL )
			g_exp_MarkerIcons = new ExpansionIcons();
		
		return g_exp_MarkerIcons.m_Icons.Get( index );
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons Get
	// ------------------------------------------------------------
	static ref ExpansionIcon Get( string name )
	{
		if ( g_exp_MarkerIcons == NULL )
			g_exp_MarkerIcons = new ExpansionIcons();
		
		return g_exp_MarkerIcons.m_IconMap.Get( name );
	}
	
	// ------------------------------------------------------------
	// ExpansionIcons GetPath
	// ------------------------------------------------------------
	static string GetPath( string name )
	{
		if ( g_exp_MarkerIcons == NULL )
			g_exp_MarkerIcons = new ExpansionIcons();
	
		return g_exp_MarkerIcons.m_IconMap.Get( name ).Path;
	}
}