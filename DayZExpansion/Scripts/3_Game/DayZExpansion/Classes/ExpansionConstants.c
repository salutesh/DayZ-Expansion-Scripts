/**
 * ExpansionConstants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * Expansion chat channels
 * @{
 */
enum ExpansionChatChannels 
{
	CCSystem = 1,
	CCAdmin = 2,
	CCDirect = 4,
	CCMegaphone = 8,
	CCTransmitter = 16,
	CCPublicAddressSystem = 32,
	CCTransport = 64,
	CCGlobal = 128,
	CCTeam = 256
};

const float EXP_FADE_TIMEOUT = 2.5;
const float EXP_FADE_OUT_DURATION = 0.25;
const float EXP_FADE_IN_DURATION = 0.25;

static const int EXP_DEFAULT_COLOUR = ARGB(255, 255, 255, 255);
static const int EXP_GAME_TEXT_COLOUR = ARGB(255, 167, 67, 192);
static const int EXP_ADMIN_TEXT_COLOUR = ARGB(255, 192, 57, 43);
static const int EXP_DIRECT_TEXT_COLOUR = ARGB(255, 255, 255, 255);
static const int EXP_GLOBAL_TEXT_COLOUR = ARGB(255, 88, 195, 247);
static const int EXP_PAS_TEXT_COLOUR = ARGB(255, 249, 255, 73);
static const int EXP_TRANSPORT_COLOUR = ARGB(255, 255, 206, 9);
static const int EXP_TEAM_TEXT_COLOUR = ARGB(255, 10, 250, 122);

/**
 * Expansion liquids types
 * @{
 */
const int LIQUID_MILK = 65536;

/**
 * Expansion Input Type
 * @{
 */
const int INPUT_EXCLUDE_CHAT_EXPANSION		= 50;		// Create with ai

/**
 * Expansion Missing type
 * @{
 */
const int ECE_ENABLEAI		= 0x800;		// Create with ai

/**
 * Expansion UI Menu IDs
 * @{
 */
const int MENU_EXPANSION_MENU_START = 1000;
const int MENU_EXPANSION_BOOK_MENU = MENU_EXPANSION_MENU_START + 0;
const int MENU_EXPANSION_MAIN_MENU = MENU_EXPANSION_MENU_START + 1;
const int MENU_EXPANSION_CODELOCK_MENU = MENU_EXPANSION_MENU_START + 2;
const int MENU_EXPANSION_MARKET_MENU = MENU_EXPANSION_MENU_START + 3;
const int MENU_EXPANSION_SPAWN_SELECTION_MENU = MENU_EXPANSION_MENU_START + 4;
const int MENU_EXPANSION_MISSION_MENU = MENU_EXPANSION_MENU_START + 5;
const int MENU_EXPANSION_LOCKER_MENU = MENU_EXPANSION_MENU_START + 6;
const int MENU_EXPANSION_NUMPAD_MENU = MENU_EXPANSION_MENU_START + 7;
const int MENU_EXPANSION_MAP = MENU_EXPANSION_MENU_START + 8;
const int MENU_EXPANSION_FLAG_MENU = MENU_EXPANSION_MENU_START + 9;
const int MENU_EXPANSION_PLAYER_LIST_MENU = MENU_EXPANSION_MENU_START + 10;

/**
 * Expansion UI Colors
 * @{
 */
static const int COLOR_EXPANSION_ITEM_HIGHLIGHT_TEXT 	= ARGB( 255, 0, 0, 0 );
static const int COLOR_EXPANSION_ITEM_NORMAL_TEXT = ARGB( 255, 0, 0, 0 );
static const int COLOR_EXPANSION_ITEM_HIGHLIGHT_ELEMENT = ARGB( 130, 89, 46, 26 );
static const int COLOR_EXPANSION_ITEM_NORMAL_ELEMENT = ARGB( 255, 0, 0, 0 );

static const int COLOR_EXPANSION_NOTIFICATION_INFO = ARGB( 255, 52, 152, 219 );
static const int COLOR_EXPANSION_NOTIFICATION_ERROR = ARGB( 255, 231, 76, 60 );
static const int COLOR_EXPANSION_NOTIFICATION_SUCCSESS = ARGB( 255, 46, 204, 113 );
static const int COLOR_EXPANSION_NOTIFICATION_ORANGE = ARGB( 255, 243, 156, 18 );
static const int COLOR_EXPANSION_NOTIFICATION_ASPHALT = ARGB( 255, 52, 73, 94 );
static const int COLOR_EXPANSION_NOTIFICATION_AMETHYST = ARGB( 255, 155, 89, 182 );
static const int COLOR_EXPANSION_NOTIFICATION_TURQUOISE = ARGB( 255, 26, 188, 156 );
static const int COLOR_EXPANSION_NOTIFICATION_ORANGEVILLE = ARGB( 255, 225, 112, 85 );
static const int COLOR_EXPANSION_NOTIFICATION_EXPANSION = ARGB( 255, 226, 65, 66 );
static const int COLOR_EXPANSION_NOTIFICATION_MISSION = ARGB(255, 235, 59, 90);

/**
 * Expansion Option Acces Types
 * @{
 */
const int AT_LIGHT_DETAIL = 63;
const int AT_MARKERS_SETTINGS = 64;

/**
 * Expansion Profile Name Types
 * @{
 */
const string EXPANSION_CAST_SHADOW_PROFILE_NAME = "EXPANSION_CAST_SHADOW_PROFILE";
const string EXPANSION_MARKERS_SETTINGS_PROFILE_NAME = "EXPANSION_MARKERS_PROFILE";

/**
 * Expansion Path
 * @{
 */
static const string EXPANSION_FOLDER = "$profile:ExpansionMod\\";

static const string EXPANSION_SETTINGS_FOLDER = EXPANSION_FOLDER + "Settings\\";
static const string EXPANSION_MARKET_FOLDER = EXPANSION_FOLDER + "Market\\";
static const string EXPANSION_TRADER_ZONES_FOLDER = EXPANSION_FOLDER + "TraderZones\\";
static const string EXPANSION_TRADER_FOLDER = EXPANSION_FOLDER + "Traders\\";
static const string EXPANSION_MISSIONS_FOLDER = EXPANSION_FOLDER + "Missions\\";
static const string EXPANSION_GROUPS_FOLDER = EXPANSION_FOLDER + "Groups\\";

static const string EXPANSION_MAPPING_FOLDER = "DayZExpansion/Scripts/Data/Mapping/";
static const string EXPANSION_MAPPING_EXT = ".map";

static const string EXPANSION_GENERAL_SETTINGS = EXPANSION_SETTINGS_FOLDER + "GeneralSettings.json";
static const string EXPANSION_BOOK_SETTINGS = EXPANSION_SETTINGS_FOLDER + "BookSettings.json";
static const string EXPANSION_BASE_BUILDING_SETTINGS = EXPANSION_SETTINGS_FOLDER + "BaseBuildingSettings.json";
static const string EXPANSION_SPAWN_SETTINGS = EXPANSION_SETTINGS_FOLDER + "SpawnSettings.json";
static const string EXPANSION_AIRDROP_SETTINGS = EXPANSION_SETTINGS_FOLDER + "AirdropSettings.json";
static const string EXPANSION_BOT_SETTINGS = EXPANSION_SETTINGS_FOLDER + "BotSettings.json";
static const string EXPANSION_MAP_SETTINGS = EXPANSION_SETTINGS_FOLDER + "MapSettings.json";
static const string EXPANSION_PARTY_SETTINGS = EXPANSION_SETTINGS_FOLDER + "PartySettings.json";
static const string EXPANSION_SAFE_ZONES_SETTINGS = EXPANSION_SETTINGS_FOLDER + "SafeZoneSettings.json";
static const string EXPANSION_MISSION_SETTINGS = EXPANSION_SETTINGS_FOLDER + "MissionSettings.json";
static const string EXPANSION_MARKET_SETTINGS = EXPANSION_SETTINGS_FOLDER + "MarketSettings.json";
static const string EXPANSION_NOTIFICATION_SETTINGS = EXPANSION_SETTINGS_FOLDER + "NotificationSettings.json";
static const string EXPANSION_RAID_SETTINGS = EXPANSION_SETTINGS_FOLDER + "RaidSettings.json";
static const string EXPANSION_TERRITORY_SETTINGS = EXPANSION_SETTINGS_FOLDER + "TerritorySettings.json";
static const string EXPANSION_VEHICLE_SETTINGS = EXPANSION_SETTINGS_FOLDER + "VehicleSettings.json";
static const string EXPANSION_DEBUG_SETTINGS = EXPANSION_SETTINGS_FOLDER + "DebugSettings.json";

static const string EXPANSION_TEMP_INTERIORS = EXPANSION_FOLDER + "TempInteriors.bin";

//! On Client Only
static const string EXPANSION_CLIENT_SETTINGS = EXPANSION_FOLDER + "settings.bin";
static const string EXPANSION_CLIENT_MARKERS = EXPANSION_FOLDER + "markers.bin";

/**
 * InGame Menu
 * @{
 */
const int EXPANSION_IDC_MAIN_OPTIONS	  	= 50000100;
const int EXPANSION_IDC_Int_RETRY			= 50000101;
const int EXPANSION_IDC_MAIN_QUIT		 	= 50000102;
const int EXPANSION_IDC_MAIN_CONTINUE		= 50000103;
const int EXPANSION_IDC_Int_DIRECT			= 50000104;

const int EXPANSION_INPUT_UDT_PLAYER_LINK	= 40003000;
const int EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK	= 40003001;
const int EXPANSION_INPUT_UDT_PERFORM_CLIMB	= 40003002;

const int EXPANSION_INPUT_RPC_PLAYER_LINK	= 40003003;

enum ExpansionPlayerLink
{
	NONE = 0,
	ATTACH,
	DETACH,
	CLIMB_START,
	CLIMB_FINISH
}

enum ExpansionPlayerRaycastResult
{
	FALSE = 0,
	DETECT,
	DETECT_FALL,
	DETACH
}

/**
 * Expansion URLs
 * @{
 */
const string EXPANSION_Rest_URL = "https://api.thurston.pw/expansion/";

/**
 * Expansion notification Icons
 * @{
 */
//! Old icons
static const string EXPANSION_NOTIFICATION_ICON_POSITION = "set:expansion_iconset image:icon_position";
static const string EXPANSION_NOTIFICATION_ICON_ERROR = "set:expansion_notification_iconset image:icon_error";
static const string EXPANSION_NOTIFICATION_ICON_CAR = "set:expansion_notification_iconset image:icon_car";
static const string EXPANSION_NOTIFICATION_ICON_GROUP = "set:expansion_notification_iconset image:icon_group";
static const string EXPANSION_NOTIFICATION_ICON_TERRITORY = "set:expansion_iconset image:icon_territory";
static const string EXPANSION_NOTIFICATION_ICON_AIRDROP = "set:expansion_notification_iconset image:icon_airdrop";
static const string EXPANSION_NOTIFICATION_ICON_DELIVER = "DayZExpansion\\GUI\\icons\\marker\\marker_deliver.paa";
static const string EXPANSION_NOTIFICATION_ICON_RADIATION = "DayZExpansion\\GUI\\icons\\marker\\marker_radiation.paa";
static const string EXPANSION_NOTIFICATION_ICON_TRADER = "DayZExpansion\\GUI\\icons\\marker\\marker_trader.paa";
static const string EXPANSION_NOTIFICATION_ICON_WATER_1 = "DayZExpansion\\GUI\\icons\\marker\\marker_water.paa";
static const string EXPANSION_NOTIFICATION_ICON_INFECTED_1 = "DayZExpansion\\GUI\\icons\\marker\\marker_zombie.paa";
static const string EXPANSION_NOTIFICATION_ICON_INFECTED_2 = "DayZExpansion\\GUI\\icons\\marker\\zombie.paa";
static const string EXPANSION_NOTIFICATION_ICON_SKULL_1 = "DayZExpansion\\GUI\\icons\\marker\\marker_skull.paa";
static const string EXPANSION_NOTIFICATION_ICON_HELI = "DayZExpansion\\GUI\\icons\\marker\\marker_helicopter.paa";
static const string EXPANSION_NOTIFICATION_ICON_HOME = "DayZExpansion\\GUI\\icons\\marker\\marker_home.paa";
static const string EXPANSION_NOTIFICATION_ICON_BOAT = "DayZExpansion\\GUI\\icons\\marker\\marker_boat.paa";
static const string EXPANSION_NOTIFICATION_ICON_FISHING = "DayZExpansion\\GUI\\icons\\marker\\marker_fishing.paa";
static const string EXPANSION_NOTIFICATION_ICON_VEHICLE_CRASH = "DayZExpansion\\GUI\\icons\\marker\\crash.paa";
static const string EXPANSION_NOTIFICATION_ICON_MAP_MARKER = "DayZExpansion\\GUI\\icons\\marker\\marker_mapmarker.paa";

//! New icons
static const string EXPANSION_NOTIFICATION_ICON_BEAR = "DayZExpansion\\GUI\\icons\\hud\\bear_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_CLAW = "DayZExpansion\\GUI\\icons\\hud\\claw_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_CROSS = "DayZExpansion\\GUI\\icons\\hud\\cross_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_DRIP = "DayZExpansion\\GUI\\icons\\hud\\drip_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_EAR = "DayZExpansion\\GUI\\icons\\hud\\ear_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_EYE = "DayZExpansion\\GUI\\icons\\hud\\eye_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_FIREPLACE = "DayZExpansion\\GUI\\icons\\hud\\fireplace_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_HEART = "DayZExpansion\\GUI\\icons\\hud\\heart_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_HOOK = "DayZExpansion\\GUI\\icons\\hud\\hook_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_INFO = "DayZExpansion\\GUI\\icons\\hud\\info_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_KNIFE = "DayZExpansion\\GUI\\icons\\hud\\knife_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_MARKER = "DayZExpansion\\GUI\\icons\\hud\\marker_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_MENU = "DayZExpansion\\GUI\\icons\\hud\\menu_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_MOON = "DayZExpansion\\GUI\\icons\\hud\\moon_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_PEN = "DayZExpansion\\GUI\\icons\\hud\\pen_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_PERSONA = "DayZExpansion\\GUI\\icons\\hud\\persona_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_PILL = "DayZExpansion\\GUI\\icons\\hud\\pill_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_QUESTIONMARK = "DayZExpansion\\GUI\\icons\\hud\\questionmark_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_SKULL_2 = "DayZExpansion\\GUI\\icons\\hud\\skull_1_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_SKULL_3 = "DayZExpansion\\GUI\\icons\\hud\\skull_2_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_STAR = "DayZExpansion\\GUI\\icons\\hud\\star_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_SUN = "DayZExpansion\\GUI\\icons\\hud\\sun_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_TENT = "DayZExpansion\\GUI\\icons\\hud\\tent_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_THERMOMETER = "DayZExpansion\\GUI\\icons\\hud\\thermometer_1_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_WATER_2 = "DayZExpansion\\GUI\\icons\\hud\\water_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_BOOK_1 = "DayZExpansion\\GUI\\icons\\hud\\book_1_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_BOOK_2 = "DayZExpansion\\GUI\\icons\\hud\\book_2_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_ELLIPSE = "DayZExpansion\\GUI\\icons\\hud\\ellipse_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_GRAB = "DayZExpansion\\GUI\\icons\\hud\\grab_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_HAND_OPEN = "DayZExpansion\\GUI\\icons\\hud\\hand_open_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_MAP = "DayZExpansion\\GUI\\icons\\hud\\map_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_NOTE = "DayZExpansion\\GUI\\icons\\hud\\note_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_ORIENTATION = "DayZExpansion\\GUI\\icons\\hud\\orientation_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_RADIO = "DayZExpansion\\GUI\\icons\\hud\\radio_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_SHIELD = "DayZExpansion\\GUI\\icons\\hud\\shield_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_SNOW = "DayZExpansion\\GUI\\icons\\hud\\snow_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_ARROW = "DayZExpansion\\GUI\\icons\\hud\\arrow_64x64.edds";

static const string EXPANSION_NOTIFICATION_ICON_BATTERY_FULL = "DayZExpansion\\GUI\\icons\\hud\\battery_high_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_BATTERY_MED = "DayZExpansion\\GUI\\icons\\hud\\battery_med_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_BATTERY_LOW = "DayZExpansion\\GUI\\icons\\hud\\battery_low_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_BATTERY_EMPTY = "DayZExpansion\\GUI\\icons\\hud\\battery_empty_64x64.edds";

static const string EXPANSION_NOTIFICATION_ICON_3D_ON = "DayZExpansion\\GUI\\icons\\hud\\3D_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_3D_OFF = "DayZExpansion\\GUI\\icons\\hud\\3D_off_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_2D_ON = "DayZExpansion\\GUI\\icons\\hud\\2D_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_2D_OFF = "DayZExpansion\\GUI\\icons\\hud\\2D_off_64x64.edds";

//! Newer icons
static const string EXPANSION_NOTIFICATION_ICON_T_Animal_Skull = "DayZExpansion\\GUI\\icons\\misc\\T_Animal_Skull_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Apple = "DayZExpansion\\GUI\\icons\\misc\\T_Apple_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Apple_Core = "DayZExpansion\\GUI\\icons\\misc\\T_Apple_Core_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Arrows = "DayZExpansion\\GUI\\icons\\misc\\T_Arrows_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Axe = "DayZExpansion\\GUI\\icons\\misc\\T_Axe_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_BagPack = "DayZExpansion\\GUI\\icons\\misc\\T_BagPack_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Bandage = "DayZExpansion\\GUI\\icons\\misc\\T_Bandage_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Batteries = "DayZExpansion\\GUI\\icons\\misc\\T_Batteries_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Berries = "DayZExpansion\\GUI\\icons\\misc\\T_Berries_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Big_Kitchen_Knife = "DayZExpansion\\GUI\\icons\\misc\\T_Big_Kitchen_Knife_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Binoculars = "DayZExpansion\\GUI\\icons\\misc\\T_Binoculars_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Bolt = "DayZExpansion\\GUI\\icons\\misc\\T_Bolt_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Bonfire = "DayZExpansion\\GUI\\icons\\misc\\T_Bonfire_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Bottle = "DayZExpansion\\GUI\\icons\\misc\\T_Bottle_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Bow = "DayZExpansion\\GUI\\icons\\misc\\T_Bow_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_BrokenLighter = "DayZExpansion\\GUI\\icons\\misc\\T_BrokenLighter_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Can_Of_Beans_Big = "DayZExpansion\\GUI\\icons\\misc\\T_Can_Of_Beans_Big_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Can_Of_Beans_Small = "DayZExpansion\\GUI\\icons\\misc\\T_Can_Of_Beans_Small_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Car_Keys = "DayZExpansion\\GUI\\icons\\misc\\T_Car_Keys_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Carrot = "DayZExpansion\\GUI\\icons\\misc\\T_Carrot_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Chain_Saw = "DayZExpansion\\GUI\\icons\\misc\\T_Chain_Saw_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Chicken = "DayZExpansion\\GUI\\icons\\misc\\T_Chicken_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Chocolate = "DayZExpansion\\GUI\\icons\\misc\\T_Chocolate_bar_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cigarets = "DayZExpansion\\GUI\\icons\\misc\\T_Cigarets_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cloth = "DayZExpansion\\GUI\\icons\\misc\\T_Cloth_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Collection_of_bolts = "DayZExpansion\\GUI\\icons\\misc\\T_Collection_of_bolts_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_01 = "DayZExpansion\\GUI\\icons\\misc\\T_Collection_Of_Sticks_01_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_02 = "DayZExpansion\\GUI\\icons\\misc\\T_Collection_Of_Sticks_02_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_03 = "DayZExpansion\\GUI\\icons\\misc\\T_Collection_Of_Sticks_03_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Collection_Of_Sticks_04 = "DayZExpansion\\GUI\\icons\\misc\\T_Collection_Of_Sticks_04_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Compass = "DayZExpansion\\GUI\\icons\\misc\\T_Compass_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_01 = "DayZExpansion\\GUI\\icons\\misc\\T_Cooked_Meat_01_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_02 = "DayZExpansion\\GUI\\icons\\misc\\T_Cooked_Meat_02_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cooked_Meat_03 = "DayZExpansion\\GUI\\icons\\misc\\T_Cooked_Meat_03_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Corn = "DayZExpansion\\GUI\\icons\\misc\\T_Corn_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Crowbar = "DayZExpansion\\GUI\\icons\\misc\\T_Crowbar_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Cow = "DayZExpansion\\GUI\\icons\\misc\\T_Cow_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Dinosaur_Skull = "DayZExpansion\\GUI\\icons\\misc\\T_Dinosaur_Skull_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Dry_Wood = "DayZExpansion\\GUI\\icons\\misc\\T_Dry_Wood_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_EatableFlowers = "DayZExpansion\\GUI\\icons\\misc\\T_EatableFlowers_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_ElectricalTape = "DayZExpansion\\GUI\\icons\\misc\\T_ElectricalTape_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Empty_Can = "DayZExpansion\\GUI\\icons\\misc\\T_Empty_Can_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Fish = "DayZExpansion\\GUI\\icons\\misc\\T_Fish_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Flare = "DayZExpansion\\GUI\\icons\\misc\\T_Flare_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_FlareGun = "DayZExpansion\\GUI\\icons\\misc\\T_FlareGun_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_FlareGunAmmo = "DayZExpansion\\GUI\\icons\\misc\\T_FlareGunAmmo_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Flashlight = "DayZExpansion\\GUI\\icons\\misc\\T_Flashlight_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Fox = "DayZExpansion\\GUI\\icons\\misc\\T_Fox_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Frying_Pan = "DayZExpansion\\GUI\\icons\\misc\\T_Frying_Pan_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Gas = "DayZExpansion\\GUI\\icons\\misc\\T_Gas_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_GasMask = "DayZExpansion\\GUI\\icons\\misc\\T_GasMask_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Golf_club = "DayZExpansion\\GUI\\icons\\misc\\T_Golf_club_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Goose = "DayZExpansion\\GUI\\icons\\misc\\T_Goose_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Grenade = "DayZExpansion\\GUI\\icons\\misc\\T_Grenade_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Guitar = "DayZExpansion\\GUI\\icons\\misc\\T_Guitar_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Gun = "DayZExpansion\\GUI\\icons\\misc\\T_Gun_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Gun_Bullets = "DayZExpansion\\GUI\\icons\\misc\\T_Gun_Bullets_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Hammer = "DayZExpansion\\GUI\\icons\\misc\\T_Hammer_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_HerbalMedicine = "DayZExpansion\\GUI\\icons\\misc\\T_HerbalMedicine_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_HomeMadeGrenade = "DayZExpansion\\GUI\\icons\\misc\\T_HomeMadeGrenade_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_HumanSkull = "DayZExpansion\\GUI\\icons\\misc\\T_HumanSkull_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Insect = "DayZExpansion\\GUI\\icons\\misc\\T_Insect_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Kitchen_Knife = "DayZExpansion\\GUI\\icons\\misc\\T_Kitchen_Knife_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Ladder = "DayZExpansion\\GUI\\icons\\misc\\T_Ladder_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Lantern = "DayZExpansion\\GUI\\icons\\misc\\T_Lantern_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Lighter = "DayZExpansion\\GUI\\icons\\misc\\T_Lighter_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Lizard = "DayZExpansion\\GUI\\icons\\misc\\T_Lizard_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Machette = "DayZExpansion\\GUI\\icons\\misc\\T_Machette_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Map = "DayZExpansion\\GUI\\icons\\misc\\T_Map_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Matches = "DayZExpansion\\GUI\\icons\\misc\\T_Matches_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Medic_Box = "DayZExpansion\\GUI\\icons\\misc\\T_Medic_Box_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Molotov = "DayZExpansion\\GUI\\icons\\misc\\T_Molotov_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Mushrooms = "DayZExpansion\\GUI\\icons\\misc\\T_Mushrooms_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Nails = "DayZExpansion\\GUI\\icons\\misc\\T_Nails_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Paper = "DayZExpansion\\GUI\\icons\\misc\\T_Paper_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Pills = "DayZExpansion\\GUI\\icons\\misc\\T_Pills_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Pipe_wrench = "DayZExpansion\\GUI\\icons\\misc\\T_Pipe_wrench_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Powder = "DayZExpansion\\GUI\\icons\\misc\\T_Powder_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Pumpkin = "DayZExpansion\\GUI\\icons\\misc\\T_Pumpkin_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Rabbit = "DayZExpansion\\GUI\\icons\\misc\\T_Rabbit_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Raccon = "DayZExpansion\\GUI\\icons\\misc\\T_Raccon_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Radio = "DayZExpansion\\GUI\\icons\\misc\\T_Radio_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Rat = "DayZExpansion\\GUI\\icons\\misc\\T_Rat_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Rock_01 = "DayZExpansion\\GUI\\icons\\misc\\T_Rock_01_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Rock_02 = "DayZExpansion\\GUI\\icons\\misc\\T_Rock_02_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Rope = "DayZExpansion\\GUI\\icons\\misc\\T_Rope_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Saw = "DayZExpansion\\GUI\\icons\\misc\\T_Saw_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Scrap_Metal = "DayZExpansion\\GUI\\icons\\misc\\T_Scrap_Metal_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Screwdriver = "DayZExpansion\\GUI\\icons\\misc\\T_Screwdriver_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Shotgun = "DayZExpansion\\GUI\\icons\\misc\\T_Shotgun_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Shotgun_Bullets = "DayZExpansion\\GUI\\icons\\misc\\T_Shotgun_Bullets_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Shovel = "DayZExpansion\\GUI\\icons\\misc\\T_Shovel_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Sleeping_Bag = "DayZExpansion\\GUI\\icons\\misc\\T_Sleeping_Bag_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Slingshot = "DayZExpansion\\GUI\\icons\\misc\\T_Slingshot_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Soda = "DayZExpansion\\GUI\\icons\\misc\\T_Soda_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Tent = "DayZExpansion\\GUI\\icons\\misc\\T_Tent_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Three_Stick = "DayZExpansion\\GUI\\icons\\misc\\T_Three_Stick_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie = "DayZExpansion\\GUI\\icons\\misc\\T_Walkie_Talkie_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_WaterJug = "DayZExpansion\\GUI\\icons\\misc\\T_WaterJug_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_WildPork = "DayZExpansion\\GUI\\icons\\misc\\T_WildPork_256x256.edds";
static const string EXPANSION_NOTIFICATION_ICON_T_Worms = "DayZExpansion\\GUI\\icons\\misc\\T_Worms_256x256.edds";


/**
 * Expansion Flags texture paths
 * @{
 */
//! Vanilla flags
static const string EXPANSION_FLAG_DAYZ_CHERNARUS = "dz\\gear\\camping\\Data\\Flag_Chern_co.paa";
static const string EXPANSION_FLAG_DAYZ_CHEDAKI = "dz\\gear\\camping\\Data\\Flag_Ched_co.paa";
static const string EXPANSION_FLAG_DAYZ_NAPA = "dz\\gear\\camping\\Data\\Flag_NAPA_co.paa";
static const string EXPANSION_FLAG_DAYZ_CDF = "dz\\gear\\camping\\Data\\Flag_CDF_co.paa";
static const string EXPANSION_FLAG_DAYZ_LIVONIA = "dz\\gear\\camping\\Data\\Flag_Livo_co.paa";
static const string EXPANSION_FLAG_DAYZ_ALTIS = "dz\\gear\\camping\\Data\\Flag_Alti_co.paa";
static const string EXPANSION_FLAG_DAYZ_SSAHRANI = "dz\\gear\\camping\\Data\\Flag_KoS_co.paa";
static const string EXPANSION_FLAG_DAYZ_NSAHRANI = "dz\\gear\\camping\\Data\\Flag_DRoS_co.paa";
static const string EXPANSION_FLAG_DAYZ_DAYZ = "dz\\gear\\camping\\Data\\Flag_DAYZ_co.paa";
static const string EXPANSION_FLAG_DAYZ_LIVONIA_ARMY = "dz\\gear\\camping\\Data\\Flag_LDF_co.paa";
static const string EXPANSION_FLAG_DAYZ_WHITE = "dz\\gear\\camping\\Data\\Flag_white_co.paa";
static const string EXPANSION_FLAG_DAYZ_BOHEMIA = "dz\\gear\\camping\\Data\\Flag_BI_co.paa";
static const string EXPANSION_FLAG_DAYZ_APA = "dz\\gear\\camping\\Data\\Flag_APA_co.paa";
static const string EXPANSION_FLAG_DAYZ_UEC = "dz\\gear\\camping\\Data\\Flag_UEC_co.paa";
static const string EXPANSION_FLAG_DAYZ_PIRATES = "dz\\gear\\camping\\Data\\Flag_jolly_co.paa";
static const string EXPANSION_FLAG_DAYZ_CANNIBALS = "dz\\gear\\camping\\Data\\Flag_jolly_c_co.paa";
static const string EXPANSION_FLAG_DAYZ_BEAR = "dz\\gear\\camping\\Data\\Flag_bear_co.paa";
static const string EXPANSION_FLAG_DAYZ_WOLF = "dz\\gear\\camping\\Data\\Flag_wolf_co.paa";
static const string EXPANSION_FLAG_DAYZ_BABY_DEER = "dz\\gear\\camping\\Data\\Flag_Fawn_co.paa";
static const string EXPANSION_FLAG_DAYZ_ROOSTER = "dz\\gear\\camping\\Data\\Flag_cock_co.paa";
static const string EXPANSION_FLAG_DAYZ_LIVONIA_POLICE = "dz\\gear\\camping\\Data\\Flag_police_co.paa";
static const string EXPANSION_FLAG_DAYZ_CMC = "dz\\gear\\camping\\Data\\Flag_CMC_co.paa";
static const string EXPANSION_FLAG_DAYZ_TEC = "dz\\gear\\camping\\Data\\Flag_TEC_co.paa";
static const string EXPANSION_FLAG_DAYZ_CHEL = "dz\\gear\\camping\\Data\\Flag_CHEL_co.paa";
static const string EXPANSION_FLAG_DAYZ_ZENIT = "dz\\gear\\camping\\Data\\Flag_Zenit_co.paa";
static const string EXPANSION_FLAG_DAYZ_HUNTERZ = "dz\\gear\\camping\\Data\\Flag_zhunters_co.paa";
static const string EXPANSION_FLAG_DAYZ_BRAINZ = "dz\\gear\\camping\\Data\\Flag_brain_co.paa";

//! Expansion flags
static const string EXPANSION_FLAG_LOGO_EXPANSION = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_expansion_co.paa";
static const string EXPANSION_FLAG_COLOR_WHITE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_white_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_AUSTRALIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_australia_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CANADA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_canada_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CHERNARUS = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_chernarus_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_FRANCE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_france_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_GERMANY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_germany_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_LATVIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_latvia_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_LUXEMBOURG = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_luxembourg_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_MEXICO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_mexico_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NETHERLANDS = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_netherlands_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NEWZEALAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_new_zealand_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NORWAY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_norway_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_POLAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_poland_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_RUSSIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_russia_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_UK = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_uk_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_USA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_usa_co.paa";

static const string EXPANSION_FLAG_COUNTRIES_SCOTTISH = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_scottish_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_FINNISH = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_finnish_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SWEDISH = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_swedish_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SPAIN = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_spain_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_BRAZIL = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_brazil_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_PORTUGAL = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_portugal_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_BELGIUM = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_belgium_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_LIVONIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_livonia_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_JAPAN = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_japan_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CHINA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_china_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SOUTHKOREA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_southkorea_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_UN = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_un_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NATO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_nato_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_PIRATE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_pirate_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CHEDAKI = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_chedaki_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NAPA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_napa_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CDF = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_cdf_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_NUEVORICO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_nuevorico_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_BORDURIENS = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_borduriens_co.paa";
static const string EXPANSION_FLAG_LOGO_BIOHAZARD = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_biohazard_co.paa";
static const string EXPANSION_FLAG_LOGO_ANYONEINCHERNO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_anyoneincherno_co.paa";

static const string EXPANSION_FLAG_COUNTRIES_IRISH = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_irish_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_WALES = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_wales_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SWITZERLAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_switzerland_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SKILAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_skilanka_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SOUTHAFRICA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_southafrica_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SICILY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_sicily_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_OFFWITHHEAD = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_offwithhead_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_GIBRALTAR = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_gibraltar_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CZECH = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_czech_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_FARI = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_fari_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_FINLAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_finland_co.paa";
static const string EXPANSION_FLAG_LOGO_DAYZWHITE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_dayzwhite_co.paa";
static const string EXPANSION_FLAG_LOGO_DAYZBLACK = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_dayzblack_co.paa";
static const string EXPANSION_FLAG_LOGO_DOUBLEAXE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_doubleaxe_co.paa";
static const string EXPANSION_FLAG_LOGO_GRENADE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_grenade_co.paa";
static const string EXPANSION_FLAG_COLOR_RED = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_red_co.paa";
static const string EXPANSION_FLAG_COLOR_BLUE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_blue_co.paa";
static const string EXPANSION_FLAG_COLOR_GREEN = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_green_co.paa";
static const string EXPANSION_FLAG_COLOR_YELLOW = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_yellow_co.paa";
static const string EXPANSION_FLAG_COLOR_ORANGE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_orange_co.paa";
static const string EXPANSION_FLAG_COLOR_PINK = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_pink_co.paa";
static const string EXPANSION_FLAG_COLOR_PURPLE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_purple_co.paa";
static const string EXPANSION_FLAG_COLOR_RAINBOW = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_rainbow_co.paa";
/**
 * Expansion Object Serialization
 * @{
 */
static const int EXPANSION_VERSION_CURRENT_SAVE = 13;
static const int EXPANSION_VERSION_CLIENT_SETTING_SAVE = EXPANSION_VERSION_CURRENT_SAVE;
static const int EXPANSION_VERSION_MAP_MARKER_SAVE = EXPANSION_VERSION_CURRENT_SAVE;

/**
 * Expansion Debugging types
 * @{
 */
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_CAR = 0;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_CONTROLLER = 1;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_HELICOPTER = 10;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_PLANE = 15;

static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_WHEELS = 20;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_ENGINE = 25;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_WINGS = 30;

static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER = 40;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_ATTACHMENT = 41;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_FALL_COMMAND = 42;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND = 43;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_VEHICLE_HORN = 44;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_PARACHUTES = 45;
static const ExpansionDebuggerType EXPANSION_DEBUG_PLAYER_DATA = 46;

static const ExpansionDebuggerType EXPANSION_DEBUG_HOLOGRAM_BASEBUILDING = 50;
static const ExpansionDebuggerType EXPANSION_DEBUG_BASEBUILDING = 51;

static const ExpansionDebuggerType EXPANSION_DEBUG_AI = 60; // reserve 20

static const ExpansionDebuggerType EXPANSION_DEBUG_XOB_OBJ = 80; // reserve 20

static bool EXPANSION_DEBUG_SETUP = false;

static void Expansion_SetupDebugger()
{
	if ( EXPANSION_DEBUG_SETUP )
		return;
	
	EXPANSION_DEBUG_SETUP = true;
	
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_CAR );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_CONTROLLER );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_HELICOPTER );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_PLANE );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_WHEELS );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_WINGS );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_ATTACHMENT );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_FALL_COMMAND );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_VEHICLE_HORN );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_PARACHUTES );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_PLAYER_DATA );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_HOLOGRAM_BASEBUILDING );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_BASEBUILDING );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_AI );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_XOB_OBJ );
	ExpansionDebugger.AddType( EXPANSION_DEBUG_VEHICLE_ENGINE );

	ExpansionDebugger.Enable( EXPANSION_DEBUG_VEHICLE_WHEELS );
	ExpansionDebugger.Enable( EXPANSION_DEBUG_VEHICLE_CAR );
	ExpansionDebugger.Enable( EXPANSION_DEBUG_VEHICLE_ENGINE );
}