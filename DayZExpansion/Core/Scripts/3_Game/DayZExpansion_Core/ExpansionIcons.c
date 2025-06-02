/**
 * ExpansionIcons.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionIcons
{
	protected static ref ExpansionIcons s_Icons = new ExpansionIcons;
	protected autoptr map<string, ref ExpansionIcon> m_IconMap;
	protected autoptr array<ExpansionIcon> m_Icons;

	// ------------------------------------------------------------
	// ExpansionIcons Constructor
	// ------------------------------------------------------------
	private void ExpansionIcons()
	{
		m_IconMap = new map<string, ref ExpansionIcon>();
		m_Icons = new array<ExpansionIcon>();

		Generate();
	}

	// ------------------------------------------------------------
	// ExpansionIcons Destructor
	// ------------------------------------------------------------
	void ~ExpansionIcons()
	{
	}

	// ------------------------------------------------------------
	// ExpansionIcons Generate
	// ------------------------------------------------------------
	void Generate()
	{
		AddIcon("Ambush", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Ambush_256x256.edds");
		AddIcon("Animal Skull", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Animal_Skull_256x256.edds");
		AddIcon("Apple", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Apple_256x256.edds");
		AddIcon("Apple Core", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Apple_Core_256x256.edds");
		AddIcon("Arrows", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Arrows_256x256.edds");
		AddIcon("Axe", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Axe_256x256.edds");
		AddIcon("Arrow", "DayZExpansion\\Core\\GUI\\icons\\hud\\arrow_64x64.edds");
		AddIcon("Airdrop", "set:expansion_notification_iconset image:icon_airdrop");
		AddIcon("Anomaly", "set:expansion_inventory image:anomaly");
		
		AddIcon("Backpack", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_BagPack_256x256.edds");
		AddIcon("Bandage", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bandage_256x256.edds");
		AddIcon("Bandit", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bandit_256x256.edds");
		AddIcon("Batteries", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Batteries_256x256.edds");
		AddIcon("Berries", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Berries_256x256.edds");
		AddIcon("Binoculars", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Binoculars_256x256.edds");
		AddIcon("Bolt", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bolt_256x256.edds");
		AddIcon("Bonfire", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bonfire_256x256.edds");
		AddIcon("Bottle", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bottle_256x256.edds");
		AddIcon("Bow", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Bow_256x256.edds");
		AddIcon("Base", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_home.paa");
		AddIcon("Boat", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Boat_256x256.edds");
		AddIcon("Book 1", "DayZExpansion\\Core\\GUI\\icons\\hud\\book_1_64x64.edds");
		AddIcon("Book 2", "DayZExpansion\\Core\\GUI\\icons\\hud\\book_2_64x64.edds");
		AddIcon("Broken Lighter", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_BrokenLighter_256x256.edds");
		AddIcon("Bear", "DayZExpansion\\Core\\GUI\\icons\\hud\\bear_64x64.edds");
		
		AddIcon("Car", "set:expansion_notification_iconset image:icon_car");
		AddIcon("Car 2", "DayZExpansion\\Core\\GUI\\icons\\hud\\car_64x64.edds");
		AddIcon("Craft", "DayZExpansion\\Core\\GUI\\icons\\hud\\craft_64x64.edds");
		AddIcon("Can Of Beans Big", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Can_Of_Beans_Big_256x256.edds");
		AddIcon("Can Of Beans Small", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Can_Of_Beans_Small_256x256.edds");
		AddIcon("Car Keys", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Car_Keys_256x256.edds");
		AddIcon("Carrot", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Carrot_256x256.edds");
		AddIcon("Chain Saw", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Chain_Saw_256x256.edds");
		AddIcon("Chicken", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Chicken_256x256.edds");
		AddIcon("Chocolate", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Chocolate_bar_256x256.edds");
		AddIcon("Cigarets", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cigarets_256x256.edds");
		AddIcon("Circuit Board", "set:expansion_inventory image:circuit_board");
		AddIcon("Cloth", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cloth_256x256.edds");
		AddIcon("Compass", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Compass_256x256.edds");
		AddIcon("Corn", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Corn_256x256.edds");
		AddIcon("Crowbar", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Crowbar_256x256.edds");
		AddIcon("Cow", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cow_256x256.edds");
		AddIcon("Claw", "DayZExpansion\\Core\\GUI\\icons\\hud\\claw_64x64.edds");
		AddIcon("Coins", "DayZExpansion\\Core\\GUI\\icons\\misc\\coinstack_64x64.edds");
		AddIcon("Coins 2", "DayZExpansion\\Core\\GUI\\icons\\misc\\coinstack2_64x64.edds");
		AddIcon("Codelock", "set:expansion_inventory image:codelock");
		
		AddIcon("Deliver", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_deliver.paa");
		AddIcon("Dinosaur Skull", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Dinosaur_Skull_256x256.edds");
		AddIcon("Dry Wood", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Dry_Wood_256x256.edds");
		AddIcon("Drip", "DayZExpansion\\Core\\GUI\\icons\\hud\\drip_64x64.edds");
		
		AddIcon("Ear", "DayZExpansion\\Core\\GUI\\icons\\hud\\ear_64x64.edds");
		AddIcon("Eye", "DayZExpansion\\Core\\GUI\\icons\\hud\\eye_64x64.edds");
		AddIcon("Error", "set:expansion_notification_iconset image:icon_error");
		AddIcon("Exclamationmark", "DayZExpansion\\Core\\GUI\\icons\\hud\\exclamationmark_64x64.edds");
		AddIcon("Ellipse", "DayZExpansion\\Core\\GUI\\icons\\hud\\ellipse_64x64.edds");
		AddIcon("Eatable Flowers", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_EatableFlowers_256x256.edds");
		AddIcon("Electrical Tape", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_ElectricalTape_256x256.edds");
		AddIcon("Empty Can", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Empty_Can_256x256.edds");
		
		AddIcon("Fishing", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_fishing.paa");
		AddIcon("Fireplace", "DayZExpansion\\Core\\GUI\\icons\\hud\\fireplace_64x64.edds");
		AddIcon("Fish", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Fish_256x256.edds");
		AddIcon("Flare", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Flare_256x256.edds");
		AddIcon("Flare Gun", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_FlareGun_256x256.edds");
		AddIcon("Flare Gun Ammo", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_FlareGunAmmo_256x256.edds");
		AddIcon("Flashlight", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Flashlight_256x256.edds");
		AddIcon("Fox", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Fox_256x256.edds");
		AddIcon("Frying Pan", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Frying_Pan_256x256.edds");
		
		AddIcon("Grab", "DayZExpansion\\Core\\GUI\\icons\\hud\\grab_64x64.edds");
		AddIcon("Group", "set:expansion_notification_iconset image:icon_group");
		AddIcon("Gas", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Gas_256x256.edds");
		AddIcon("Gas Mask", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_GasMask_256x256.edds");
		AddIcon("Golf Club", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Golf_club_256x256.edds");
		AddIcon("Goose", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Goose_256x256.edds");
		AddIcon("Grenade", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Grenade_256x256.edds");
		AddIcon("Guitar", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Guitar_256x256.edds");
		AddIcon("Gun", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Gun_256x256.edds");
		AddIcon("Gun Bullets", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Gun_Bullets_256x256.edds");
		
		AddIcon("Heart", "DayZExpansion\\Core\\GUI\\icons\\hud\\heart_64x64.edds");
		AddIcon("Hook", "DayZExpansion\\Core\\GUI\\icons\\hud\\hook_64x64.edds");
		AddIcon("Helicopter", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Helicopter_256x256.edds");
		AddIcon("Hammer", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Hammer_256x256.edds");
		AddIcon("Herbal Medicine", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_HerbalMedicine_256x256.edds");
		AddIcon("Home Made Grenade", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_HomeMadeGrenade_256x256.edds");
		AddIcon("Human Skull", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_HumanSkull_256x256.edds");
		
		AddIcon("Info", "DayZExpansion\\Core\\GUI\\icons\\hud\\info_64x64.edds");
		AddIcon("Infected 1", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_zombie.paa");
		AddIcon("Infected 2", "DayZExpansion\\Core\\GUI\\icons\\marker\\zombie.paa");
		AddIcon("Insect", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Insect_256x256.edds");
		
		AddIcon("Knife", "DayZExpansion\\Core\\GUI\\icons\\hud\\knife_64x64.edds");
		AddIcon("Kitchen Knife", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Kitchen_Knife_256x256.edds");
		AddIcon("Kitchen Knife Big", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Big_Kitchen_Knife_256x256.edds");
		
		AddIcon("Ladder", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Ladder_256x256.edds");
		AddIcon("Lantern", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Lantern_256x256.edds");
		AddIcon("Lighter", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Lighter_256x256.edds");
		
		AddIcon("Menu", "DayZExpansion\\Core\\GUI\\icons\\hud\\menu_64x64.edds");
		AddIcon("Marker", "DayZExpansion\\Core\\GUI\\icons\\hud\\marker_64x64.edds");
		AddIcon("Map", "DayZExpansion\\Core\\GUI\\icons\\hud\\map_64x64.edds");
		AddIcon("Map Marker", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_mapmarker.paa");
		AddIcon("Moon", "DayZExpansion\\Core\\GUI\\icons\\hud\\moon_64x64.edds");
		AddIcon("Machette", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Machette_256x256.edds");
		AddIcon("Matches", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Matches_256x256.edds");
		AddIcon("Medic Box", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Medic_Box_256x256.edds");
		AddIcon("Mushrooms", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Mushrooms_256x256.edds");
		
		AddIcon("Note", "DayZExpansion\\Core\\GUI\\icons\\hud\\note_64x64.edds");
		AddIcon("Nails", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Nails_256x256.edds");
		
		AddIcon("Open Hand", "DayZExpansion\\Core\\GUI\\icons\\hud\\hand_open_64x64.edds");
		AddIcon("Orientation", "DayZExpansion\\Core\\GUI\\icons\\hud\\orientation_64x64.edds");
		AddIcon("Options", "set:expansion_iconset image:icon_options");
		
		AddIcon("Pen", "DayZExpansion\\Core\\GUI\\icons\\hud\\pen_64x64.edds");
		AddIcon("Persona", "DayZExpansion\\Core\\GUI\\icons\\hud\\persona_64x64.edds");
		AddIcon("Pill", "DayZExpansion\\Core\\GUI\\icons\\hud\\pill_64x64.edds");
		AddIcon("Paper Map", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Map_256x256.edds");
		AddIcon("Paper", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Paper_256x256.edds");
		AddIcon("Pills", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Pills_256x256.edds");
		AddIcon("Pipe Wrench", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Pipe_wrench_256x256.edds");
		AddIcon("Powder", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Powder_256x256.edds");
		AddIcon("Pumpkin", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Pumpkin_256x256.edds");
		
		AddIcon("Questionmark", "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_64x64.edds");
		AddIcon("Questionmark 2", "DayZExpansion\\Core\\GUI\\icons\\hud\\questionmark_2_64x64.edds");
		
		AddIcon("Radiation", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_radiation.paa");
		AddIcon("Radio", "DayZExpansion\\Core\\GUI\\icons\\hud\\radio_64x64.edds");
		AddIcon("Rabbit", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Rabbit_256x256.edds");
		AddIcon("Racoon", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Racoon_256x256.edds");
		AddIcon("Radio", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Radio_256x256.edds");
		AddIcon("Rat", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Rat_256x256.edds");
		AddIcon("Rock 1", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Rock_01_256x256.edds");
		AddIcon("Rock 2", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Rock_02_256x256.edds");
		AddIcon("Rope", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Rope_256x256.edds");
		
		AddIcon("Skull 1", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_skull.paa");
		AddIcon("Skull 2", "DayZExpansion\\Core\\GUI\\icons\\hud\\skull_1_64x64.edds");
		AddIcon("Skull 3", "DayZExpansion\\Core\\GUI\\icons\\hud\\skull_2_64x64.edds");
		AddIcon("Star", "DayZExpansion\\Core\\GUI\\icons\\hud\\star_64x64.edds");
		AddIcon("Sun", "DayZExpansion\\Core\\GUI\\icons\\hud\\sun_64x64.edds");
		AddIcon("Snow", "DayZExpansion\\Core\\GUI\\icons\\hud\\snow_64x64.edds");
		AddIcon("Square", "DayZExpansion\\Core\\GUI\\icons\\hud\\square_64x64.edds");
		AddIcon("Shield", "DayZExpansion\\Core\\GUI\\icons\\hud\\shield_64x64.edds");
		AddIcon("Saw", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Saw_256x256.edds");
		AddIcon("Scrap Metal", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Scrap_Metal_256x256.edds");
		AddIcon("Screwdriver", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Screwdriver_256x256.edds");
		AddIcon("Shotgun", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Shotgun_256x256.edds");
		AddIcon("Shotgun Bullets", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Shotgun_Bullets_256x256.edds");
		AddIcon("Shovel", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Shovel_256x256.edds");
		AddIcon("Soda", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Soda_256x256.edds");
		AddIcon("Soldier", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Soldier_256x256.edds");
		AddIcon("Status", "set:expansion_iconset image:icon_status");
		
		AddIcon("Territory", "set:expansion_iconset image:icon_territory");
		AddIcon("Trader", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_trader.paa");
		AddIcon("Tent", "DayZExpansion\\Core\\GUI\\icons\\hud\\tent_64x64.edds");
		AddIcon("Thermometer", "DayZExpansion\\Core\\GUI\\icons\\hud\\thermometer_1_64x64.edds");
		AddIcon("Tent", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Tent_256x256.edds");
		
		AddIcon("Vehicle Crash", "DayZExpansion\\Core\\GUI\\icons\\marker\\crash.paa");
		
		AddIcon("Water 1", "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_water.paa");
		AddIcon("Water 2", "DayZExpansion\\Core\\GUI\\icons\\hud\\water_64x64.edds");
		AddIcon("Walkie Talkie", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Wakie-Talkie_256x256.edds");
		AddIcon("Water Jug", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_WaterJug_256x256.edds");
		AddIcon("Wild Pork", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_WildPork_256x256.edds");
		AddIcon("Worms", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Worms_256x256.edds");

		//! Dont use yet
		//AddIcon("Three Stick", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Three_Stick_256x256.edds");
		//AddIcon("Sleeping Bag", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Sleeping_Bag_256x256.edds");
		//AddIcon("Slingshot", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Slingshot_256x256.edds");
		//AddIcon("Molotov", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Molotov_256x256.edds");
		//AddIcon("Lizard", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Lizard_256x256.edds");
		//AddIcon("Cooked Meat 1", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cooked_Meat_01_256x256.edds");
		//AddIcon("Cooked Meat 2", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cooked_Meat_02_256x256.edds");
		//AddIcon("Cooked Meat 3", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Cooked_Meat_03_256x256.edds");
		//AddIcon("Collection Of Bolts", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Collection_of_bolts_256x256.edds");
		//AddIcon("Collection Of Sticks 1", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Collection_Of_Sticks_01_256x256.edds");
		//AddIcon("Collection Of Sticks 2", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Collection_Of_Sticks_02_256x256.edds");
		//AddIcon("Collection Of Sticks 3", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Collection_Of_Sticks_03_256x256.edds");
		//AddIcon("Collection Of Sticks 4", "DayZExpansion\\Core\\GUI\\icons\\misc\\T_Collection_Of_Sticks_04_256x256.edds");

		//! Social Media Icons
		AddIcon("Discord", "set:expansion_iconset image:icon_discord");
		AddIcon("Reddit", "set:expansion_iconset image:icon_reddit");
		AddIcon("Steam", "set:expansion_iconset image:icon_steam");
		AddIcon("GitHub", "set:expansion_iconset image:icon_github");
		AddIcon("Homepage", "set:expansion_iconset image:icon_home");
		AddIcon("Forums", "set:expansion_iconset image:icon_forums");
#ifdef DabsFramework
		AddIcon("Twitter", "set:brand image:x_twitter");
#else
		AddIcon("Twitter", "set:expansion_iconset image:icon_twitter");
#endif
		AddIcon("YouTube", "set:expansion_iconset image:icon_youtube");
		AddIcon("Patreon", "set:expansion_iconset image:icon_patreon");
		AddIcon("Guilded", "set:expansion_iconset image:icon_guilded");
		AddIcon("PayPal", "set:expansion_iconset image:icon_paypal");
		
		//! Misc
		AddIcon("Cross", "DayZExpansion\\Core\\GUI\\icons\\hud\\cross_64x64.edds");
		AddIcon("Battery Full", "DayZExpansion\\Core\\GUI\\icons\\hud\\battery_high_64x64.edds");
		AddIcon("Battery Med", "DayZExpansion\\Core\\GUI\\icons\\hud\\battery_med_64x64.edds");
		AddIcon("Battery Low", "DayZExpansion\\Core\\GUI\\icons\\hud\\battery_low_64x64.edds");
		AddIcon("Battery Empty", "DayZExpansion\\Core\\GUI\\icons\\hud\\battery_empty_64x64.edds");
		AddIcon("Clock Blank", "set:expansion_iconset image:clock_blank");
		
		//! Expansion logos
		AddIcon("Expansion", "set:expansion_iconset image:icon_expansion_col");
		AddIcon("Expansion B/W", "set:expansion_iconset image:icon_expansion_bw");
		//AddIcon("Expansion White", "set:expansion_iconset image:icon_expansion_white");  //! Same as B/W
	}

	// ------------------------------------------------------------
	// ExpansionIcons AddIcon
	// ------------------------------------------------------------
	protected void AddIcon(string name, string localisation, string path)
	{
		if (localisation == "")
			localisation = name;

		ExpansionIcon icon = m_IconMap.Get(name);
		if (icon == NULL)
		{
			icon = new ExpansionIcon(name, localisation, path);
			m_IconMap.Insert(name, icon);
			m_Icons.Insert(icon);
		} else
		{
			icon.Name = name;
			icon.Localisation = localisation;
			icon.IconPath = path;
		}
	}

	protected void AddIcon(string name, string path)
	{
		AddIcon(name, name, path);
	}

	// ------------------------------------------------------------
	// ExpansionIcons Count
	// ------------------------------------------------------------
	static int Count()
	{
		return s_Icons.m_Icons.Count();
	}

	// ------------------------------------------------------------
	// ExpansionIcons Get
	// ------------------------------------------------------------
	static ExpansionIcon Get(int index)
	{
		return s_Icons.m_Icons.Get(index);
	}

	// ------------------------------------------------------------
	// ExpansionIcons Get
	// ------------------------------------------------------------
	static ExpansionIcon Get(string name)
	{
		return s_Icons.m_IconMap.Get(name);
	}

	// ------------------------------------------------------------
	// ExpansionIcons GetPath
	// ------------------------------------------------------------
	static string GetPath( string name )
	{
		ExpansionIcon icon = Get(name);
		if (!icon)
			return string.Empty;

		return icon.IconPath;
	}

	static array<ExpansionIcon> Sorted()
	{
		TStringArray namesSorted = {};

		foreach (ExpansionIcon icon: s_Icons.m_Icons)
		{
			namesSorted.Insert(icon.Name);
		}

		namesSorted.Sort();

		array<ExpansionIcon> iconsSorted();

		foreach (string name: namesSorted)
		{
			iconsSorted.Insert(Get(name));
		}

		return iconsSorted;
	}
}
