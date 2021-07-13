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
		AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_VEHICLE_CRASH	);
		
		//! Newer icons
		AddIcon( "Animal Skull",	"Animal Skull",	EXPANSION_NOTIFICATION_ICON_T_Animal_Skull );
		AddIcon( "Apple",	"Apple",	EXPANSION_NOTIFICATION_ICON_T_Apple );
		AddIcon( "Apple Core",	"Apple Core",	EXPANSION_NOTIFICATION_ICON_T_Apple_Core );
		AddIcon( "Arrows",	"Arrows",	EXPANSION_NOTIFICATION_ICON_T_Arrows );
		AddIcon( "Axe",	"Axe",	EXPANSION_NOTIFICATION_ICON_T_Axe );
		AddIcon( "Backpack",	"Backpack",	EXPANSION_NOTIFICATION_ICON_T_BagPack );
		AddIcon( "Bandage",	"Bandage",	EXPANSION_NOTIFICATION_ICON_T_Bandage );
		AddIcon( "Batteries",	"Batteries",	EXPANSION_NOTIFICATION_ICON_T_Batteries );
		AddIcon( "Berries",	"Berries",	EXPANSION_NOTIFICATION_ICON_T_Berries );
		AddIcon( "Kitchen Knife Big",	"Kitchen Knife Big",	EXPANSION_NOTIFICATION_ICON_T_Big_Kitchen_Knife );
		AddIcon( "Binoculars",	"Binoculars",	EXPANSION_NOTIFICATION_ICON_T_Binoculars );
		AddIcon( "Bolt",	"Bolt",	EXPANSION_NOTIFICATION_ICON_T_Bolt );
		AddIcon( "Bonfire",	"Bornfire",	EXPANSION_NOTIFICATION_ICON_T_Bonfire );
		AddIcon( "Bottle",	"Bottle",	EXPANSION_NOTIFICATION_ICON_T_Bottle );
		AddIcon( "Bow",	"Bow",	EXPANSION_NOTIFICATION_ICON_T_Bow );
		AddIcon( "Broken Lighter",	"Broken Lighter",	EXPANSION_NOTIFICATION_ICON_T_BrokenLighter );
		AddIcon( "Can Of Beans Big",	"Can Of Beans Big",	EXPANSION_NOTIFICATION_ICON_T_Can_Of_Beans_Big );
		AddIcon( "Can Of Beans Small",	"Can Of Beans Small",	EXPANSION_NOTIFICATION_ICON_T_Can_Of_Beans_Small );
		AddIcon( "Car Keys",	"Car Keys",	EXPANSION_NOTIFICATION_ICON_T_Car_Keys );
		AddIcon( "Carrot",	"Carrot",	EXPANSION_NOTIFICATION_ICON_T_Carrot );
		AddIcon( "Chain Saw",	"Chain Saw",	EXPANSION_NOTIFICATION_ICON_T_Chain_Saw );
		AddIcon( "Chicken",	"Chicken",	EXPANSION_NOTIFICATION_ICON_T_Chicken );
		AddIcon( "Chocolate",	"Chocolate",	EXPANSION_NOTIFICATION_ICON_T_Chocolate );
		AddIcon( "Cigarets",	"Cigarets",	EXPANSION_NOTIFICATION_ICON_T_Cigarets );
		AddIcon( "Cloth",	"Cloth ",	EXPANSION_NOTIFICATION_ICON_T_Cloth );
		AddIcon( "Compass",	"Compass",	EXPANSION_NOTIFICATION_ICON_T_Compass );
		AddIcon( "Corn",	"Corn",	EXPANSION_NOTIFICATION_ICON_T_Corn );
		AddIcon( "Crowbar",	"Crowbar",	EXPANSION_NOTIFICATION_ICON_T_Crowbar );
		AddIcon( "Cow",	"Cow",	EXPANSION_NOTIFICATION_ICON_T_Cow );
		AddIcon( "Dinosaur Skull",	"Dinosaur Skull ",	EXPANSION_NOTIFICATION_ICON_T_Dinosaur_Skull );
		AddIcon( "Dry Wood",	"Dry Wood",	EXPANSION_NOTIFICATION_ICON_T_Dry_Wood );
		AddIcon( "Eatable Flowers",	"Eatable Flowers",	EXPANSION_NOTIFICATION_ICON_T_EatableFlowers );
		AddIcon( "Electrical Tape",	"Electrical Tape",	EXPANSION_NOTIFICATION_ICON_T_ElectricalTape );
		AddIcon( "Empty Can",	"Empty Can",	EXPANSION_NOTIFICATION_ICON_T_Empty_Can );
		AddIcon( "Fish",	"Fish",	EXPANSION_NOTIFICATION_ICON_T_Fish );
		AddIcon( "Flare",	"Flare",	EXPANSION_NOTIFICATION_ICON_T_Flare );
		AddIcon( "Flare Gun",	"Flare Gun",	EXPANSION_NOTIFICATION_ICON_T_FlareGun );
		AddIcon( "Flare Gun Ammo",	"Flare Gun Ammo",	EXPANSION_NOTIFICATION_ICON_T_FlareGunAmmo );
		AddIcon( "Flashlight",	"Flashlight",	EXPANSION_NOTIFICATION_ICON_T_Flashlight );
		AddIcon( "Fox",	"Fox",	EXPANSION_NOTIFICATION_ICON_T_Fox );
		AddIcon( "Frying Pan",	"Frying Pan",	EXPANSION_NOTIFICATION_ICON_T_Frying_Pan );
		AddIcon( "Gas",	"Gas",	EXPANSION_NOTIFICATION_ICON_T_Gas );
		AddIcon( "Gas Mask",	"Gas Mask",	EXPANSION_NOTIFICATION_ICON_T_GasMask );
		AddIcon( "Golf Club",	"Golf Club",	EXPANSION_NOTIFICATION_ICON_T_Golf_club );
		AddIcon( "Goose",	"Goose",	EXPANSION_NOTIFICATION_ICON_T_Goose );
		AddIcon( "Grenade",	"Grenade",	EXPANSION_NOTIFICATION_ICON_T_Grenade );
		AddIcon( "Guitar",	"Guitar",	EXPANSION_NOTIFICATION_ICON_T_Guitar );
		AddIcon( "Gun",	"Gun",	EXPANSION_NOTIFICATION_ICON_T_Gun );
		AddIcon( "Gun Bullets",	"Gun Bullets",	EXPANSION_NOTIFICATION_ICON_T_Gun_Bullets );
		AddIcon( "Hammer",	"Hammer",	EXPANSION_NOTIFICATION_ICON_T_Hammer );
		AddIcon( "Herbal Medicine",	"Herbal Medicine",	EXPANSION_NOTIFICATION_ICON_T_HerbalMedicine );
		AddIcon( "Home Made Grenade",	"Home Made Grenade",	EXPANSION_NOTIFICATION_ICON_T_HomeMadeGrenade );
		AddIcon( "Human Skull",	"Human Skull",	EXPANSION_NOTIFICATION_ICON_T_HumanSkull );
		AddIcon( "Insect",	"Insect",	EXPANSION_NOTIFICATION_ICON_T_Insect );
		AddIcon( "Kitchen Knife",	"Kitchen Knife",	EXPANSION_NOTIFICATION_ICON_T_Kitchen_Knife );
		AddIcon( "Ladder",	"Ladder",	EXPANSION_NOTIFICATION_ICON_T_Ladder );
		AddIcon( "Lantern",	"Lantern",	EXPANSION_NOTIFICATION_ICON_T_Lantern );
		AddIcon( "Lighter",	"Lighter",	EXPANSION_NOTIFICATION_ICON_T_Lighter );
		AddIcon( "Machette",	"Machette",	EXPANSION_NOTIFICATION_ICON_T_Machette );
		AddIcon( "Paper Map",	"Paper Map",	EXPANSION_NOTIFICATION_ICON_T_Map );
		AddIcon( "Matches",	"Matches",	EXPANSION_NOTIFICATION_ICON_T_Matches );
		AddIcon( "Medic Box",	"Medic Box",	EXPANSION_NOTIFICATION_ICON_T_Medic_Box );
		AddIcon( "Mushrooms",	"Mushrooms",	EXPANSION_NOTIFICATION_ICON_T_Mushrooms );
		AddIcon( "Nails",	"Nails",	EXPANSION_NOTIFICATION_ICON_T_Nails );
		AddIcon( "Paper",	"Paper",	EXPANSION_NOTIFICATION_ICON_T_Paper );
		AddIcon( "Pills",	"Pills",	EXPANSION_NOTIFICATION_ICON_T_Pills );
		AddIcon( "Pipe Wrench",	"Pipe Wrench",	EXPANSION_NOTIFICATION_ICON_T_Pipe_wrench );
		AddIcon( "Powder",	"Powder",	EXPANSION_NOTIFICATION_ICON_T_Powder );
		AddIcon( "Pumpkin",	"Pumpkin",	EXPANSION_NOTIFICATION_ICON_T_Pumpkin );
		AddIcon( "Rabbit",	"Rabbit",	EXPANSION_NOTIFICATION_ICON_T_Rabbit );
		AddIcon( "Racoon",	"Racoon",	EXPANSION_NOTIFICATION_ICON_T_Racoon );
		AddIcon( "Radio",	"Radio",	EXPANSION_NOTIFICATION_ICON_T_Radio );
		AddIcon( "Rat",	"Rat",	EXPANSION_NOTIFICATION_ICON_T_Rat );
		AddIcon( "Rock 1",	"Rock 1",	EXPANSION_NOTIFICATION_ICON_T_Rock_01 );
		AddIcon( "Rock 2",	"Rock 2",	EXPANSION_NOTIFICATION_ICON_T_Rock_02 );
		AddIcon( "Rope",	"Rope",	EXPANSION_NOTIFICATION_ICON_T_Rope );
		AddIcon( "Saw",	"Saw",	EXPANSION_NOTIFICATION_ICON_T_Saw );
		AddIcon( "Scrap Metal",	"Scrap Metal",	EXPANSION_NOTIFICATION_ICON_T_Scrap_Metal );
		AddIcon( "Screwdriver",	"Screwdriver",	EXPANSION_NOTIFICATION_ICON_T_Screwdriver );
		AddIcon( "Shotgun",	"Shotgun",	EXPANSION_NOTIFICATION_ICON_T_Shotgun );
		AddIcon( "Shotgun Bullets",	"Shotgun Bullets",	EXPANSION_NOTIFICATION_ICON_T_Shotgun_Bullets );
		AddIcon( "Shovel",	"Shovel",	EXPANSION_NOTIFICATION_ICON_T_Shovel );
		AddIcon( "Soda",	"Soda",	EXPANSION_NOTIFICATION_ICON_T_Soda );
		AddIcon( "Tent Small",	"Tent Small",	EXPANSION_NOTIFICATION_ICON_T_Tent );
		AddIcon( "Walkie Talkie",	"Walkie Talkie",	EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie );
		AddIcon( "Water Jug",	"Water Jug",	EXPANSION_NOTIFICATION_ICON_T_WaterJug );
		AddIcon( "Wild Pork",	"Wild Pork",	EXPANSION_NOTIFICATION_ICON_T_WildPork );
		AddIcon( "Worms",	"Worms",	EXPANSION_NOTIFICATION_ICON_T_Worms );
		
		//! Dont use yet
		//AddIcon( "Three Stick ",	"Three Stick ",	EXPANSION_NOTIFICATION_ICON_T_Three_Stick );
		//AddIcon( "Sleeping Bag",	"Sleeping Bag",	EXPANSION_NOTIFICATION_ICON_T_Sleeping_Bag );
		//AddIcon( "Slingshot",	"Slingshot",	EXPANSION_NOTIFICATION_ICON_T_Slingshot );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Molotov );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Lizard );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_01 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_02 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_03 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Collection_of_bolts );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_01 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_02 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_03 );
		//AddIcon( "Vehicle Crash",	"Vehicle Crash",	EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_04 );
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