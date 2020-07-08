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
	CCGlobal = 128
}

class ExpansionChatInformation
{
	ExpansionChatChannels m_Channel;
	string m_Sender;
	string m_Message;

	void ExpansionChatInformation( ExpansionChatChannels c, string s, string m )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatInformation::ExpansionChatInformation - Start");
		#endif
		
		m_Channel = c;
		m_Sender = s;
		m_Message = m;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionChatInformation::ExpansionChatInformation - End");
		#endif
	}
}

const float EXP_FADE_TIMEOUT = 2.5;
const float EXP_FADE_OUT_DURATION = 0.25;
const float EXP_FADE_IN_DURATION = 0.25;

static const int EXP_DEFAULT_COLOUR = ARGB(255, 255, 255, 255);
static const int EXP_GAME_TEXT_COLOUR = ARGB(255, 122, 134, 156);
static const int EXP_ADMIN_TEXT_COLOUR = ARGB(255, 192, 57, 43);
static const int EXP_DIRECT_TEXT_COLOUR = ARGB(255, 255, 255, 255);
static const int EXP_GLOBAL_TEXT_COLOUR = ARGB(255, 88, 195, 247);
static const int EXP_PAS_TEXT_COLOUR = ARGB(255, 249, 255, 73);
static const int EXP_TRANSPORT_COLOUR = ARGB(255, 255, 206, 9);

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
const int EXPANSION_INPUT_RPC_PLAYER_LINK	= 40003003;

enum ExpansionPlayerLink
{
	NONE = 0,
	ATTACH,
	DETACH,
	CLIMB_START,
	CLIMB_FINISHED,
	REQUEST_LINK
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
static const string EXPANSION_NOTIFICATION_ICON_INFO = "DayZExpansion/GUI/icons/hud/info_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_ERROR = "set:expansion_notification_iconset image:icon_error";
static const string EXPANSION_NOTIFICATION_ICON_CHECK = "set:expansion_notification_iconset image:icon_check";
static const string EXPANSION_NOTIFICATION_ICON_MARKER = "DayZExpansion/GUI/icons/hud/marker_64x64.edds";
static const string EXPANSION_NOTIFICATION_ICON_CAR = "set:expansion_notification_iconset image:icon_car";
static const string EXPANSION_NOTIFICATION_ICON_GROUP = "set:expansion_notification_iconset image:icon_group";
static const string EXPANSION_NOTIFICATION_ICON_TERRITORY = "set:expansion_iconset image:icon_territory";
static const string EXPANSION_NOTIFICATION_ICON_AIRDROP = "set:expansion_notification_iconset image:icon_airdrop";

/**
 * Expansion Flags texture paths
 * @{
 */
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
static const string EXPANSION_FLAG_COUNTRIES_BIOHAZARD = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_biohazard_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_ANYONEINCHERNO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_anyoneincherno_co.paa";

/**
 * Expansion Object Serialization
 * @{
 */
static const int EXPANSION_VERSION_CURRENT_SAVE = 5;

/**
 * Expansion Debugging types
 * @{
 */
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_CAR = 0;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_CONTROLLER = 1;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_HELICOPTER = 10;
static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_PLANE = 15;

static const ExpansionDebuggerType EXPANSION_DEBUG_VEHICLE_WHEELS = 20;
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

static void Expansion_SetupDebugger()
{
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

	ExpansionDebugger.Enable( EXPANSION_DEBUG_AI );
}