/**
 * ExpansionConstants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

const float NAN = "nan".ToFloat();

/**
 * Expansion chat channels
 * @{
 */
enum ExpansionChatChannels
{
	//! Vanilla
	CCSystem = 1,
	CCAdmin = 2,
	CCDirect = 4,
	CCMegaphone = 8,
	CCTransmitter = 16,
	CCPublicAddressSystem = 32,
	CCBattlEye = 64,

	//! Expansion
	CCTransport = 128,
	CCGlobal = 256,
	CCTeam = 512
};

/**
 * Expansion liquids types
 * @{
 */
const int EXPANSION_LIQUID_MILK = 65536;

/**
 * Expansion Input Type
 * @{
 */
const int INPUT_EXCLUDE_CHAT_EXPANSION		= 50;

/**
 * Expansion UI Menu IDs
 * @{
 */
const int MENU_EXPANSION_MENU_START = 1000;
const int MENU_EXPANSION_CODELOCK_MENU = MENU_EXPANSION_MENU_START + 0;
const int MENU_EXPANSION_NUMPAD_MENU = MENU_EXPANSION_MENU_START + 1;
const int MENU_EXPANSION_MAP = MENU_EXPANSION_MENU_START + 2;
const int MENU_EXPANSION_FLAG_MENU = MENU_EXPANSION_MENU_START + 3;

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
 * Expansion Path
 * @{
 */
static const string EXPANSION_FOLDER = "$profile:ExpansionMod\\";
static const string EXPANSION_MISSION_FOLDER = "$mission:expansion\\";

static const string EXPANSION_SETTINGS_FOLDER = EXPANSION_FOLDER + "Settings\\";
static const string EXPANSION_LOADOUT_FOLDER = EXPANSION_FOLDER + "Loadouts\\";
static const string EXPANSION_GROUPS_FOLDER = EXPANSION_FOLDER + "Groups\\";
static const string EXPANSION_LOG_FOLDER = EXPANSION_FOLDER + "Logs\\";
static const string EXPANSION_AI_FOLDER = EXPANSION_FOLDER + "AI\\";
static const string EXPANSION_AI_PATROLS_FOLDER = EXPANSION_AI_FOLDER + "Patrols\\";
static const string EXPANSION_AI_UTILS_FOLDER = EXPANSION_AI_FOLDER + "Utils\\";
static const string EXPANSION_MISSION_SETTINGS_FOLDER = EXPANSION_MISSION_FOLDER + "settings\\";
static const string EXPANSION_MISSIONS_FOLDER = EXPANSION_MISSION_FOLDER + "missions\\";
static const string EXPANSION_MISSIONS_OBJECTS_FOLDER = EXPANSION_MISSION_FOLDER + "missionobjects\\";

static const string EXPANSION_MAPPING_FOLDER = "DayZExpansion/Scripts/Data/Mapping/";
static const string EXPANSION_MAPPING_EXT = ".map";

static const string EXPANSION_GENERAL_SETTINGS = EXPANSION_SETTINGS_FOLDER + "GeneralSettings.json";
static const string EXPANSION_BASE_BUILDING_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "BaseBuildingSettings.json";
static const string EXPANSION_AIRDROP_SETTINGS = EXPANSION_SETTINGS_FOLDER + "AirdropSettings.json";
static const string EXPANSION_AI_SETTINGS = EXPANSION_SETTINGS_FOLDER + "AISettings.json";
static const string EXPANSION_AIPATROL_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "AIPatrolSettings.json";
static const string EXPANSION_MAP_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "MapSettings.json";
static const string EXPANSION_PARTY_SETTINGS = EXPANSION_SETTINGS_FOLDER + "PartySettings.json";
static const string EXPANSION_SAFE_ZONES_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "SafeZoneSettings.json";
static const string EXPANSION_MISSION_SETTINGS = EXPANSION_SETTINGS_FOLDER + "MissionSettings.json";
static const string EXPANSION_NOTIFICATION_SETTINGS = EXPANSION_SETTINGS_FOLDER + "NotificationSettings.json";
static const string EXPANSION_RAID_SETTINGS = EXPANSION_SETTINGS_FOLDER + "RaidSettings.json";
static const string EXPANSION_TERRITORY_SETTINGS = EXPANSION_SETTINGS_FOLDER + "TerritorySettings.json";
static const string EXPANSION_VEHICLE_SETTINGS = EXPANSION_SETTINGS_FOLDER + "VehicleSettings.json";
static const string EXPANSION_DEBUG_SETTINGS = EXPANSION_SETTINGS_FOLDER + "DebugSettings.json";
static const string EXPANSION_PLAYERLIST_SETTINGS = EXPANSION_SETTINGS_FOLDER + "PlayerListSettings.json";
static const string EXPANSION_LOG_SETTINGS = EXPANSION_SETTINGS_FOLDER + "LogsSettings.json";
static const string EXPANSION_SOCIALMEDIA_SETTINGS = EXPANSION_SETTINGS_FOLDER + "SocialMediaSettings.json";
static const string EXPANSION_CHAT_SETTINGS = EXPANSION_SETTINGS_FOLDER + "ChatSettings.json";
static const string EXPANSION_MONITORING_SETTINGS = EXPANSION_SETTINGS_FOLDER + "MonitoringSettings.json";
static const string EXPANSION_DAMAGESYSTEM_SETTINGS = EXPANSION_SETTINGS_FOLDER + "DamageSystemSettings.json";
static const string EXPANSION_GARAGE_SETTINGS = EXPANSION_SETTINGS_FOLDER + "GarageSettings.json";

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

enum ExpansionItemQuantityType
{
	PC = 1,
	PERCENTAGE = 2,
	GRAM = 4,
	MILLILITER = 8,
	MAGAZINE = 16,
	POWER = 32,
	HEATISOLATION = 64
}

/**
 * Expansion URLs
 * @{
 */
const string EXPANSION_Rest_URL = "https://api.thurston.pw/expansion/";

/**
 * Expansion icons (internal use only)
 * @{
 */
static const string EXPANSION_ICON_POSITION = "set:expansion_iconset image:icon_position";

static const string EXPANSION_ICON_3D_ON = "DayZExpansion\\Core\\GUI\\icons\\hud\\3D_64x64.edds";
static const string EXPANSION_ICON_3D_OFF = "DayZExpansion\\Core\\GUI\\icons\\hud\\3D_off_64x64.edds";
static const string EXPANSION_ICON_2D_ON = "DayZExpansion\\Core\\GUI\\icons\\hud\\2D_64x64.edds";
static const string EXPANSION_ICON_2D_OFF = "DayZExpansion\\Core\\GUI\\icons\\hud\\2D_off_64x64.edds";

/**
 * Expansion notification icons (internal use only)
 * @{
 */
//! Old icons
static const string EXPANSION_NOTIFICATION_ICON_ERROR = "Error";
static const string EXPANSION_NOTIFICATION_ICON_CAR = "Car";
static const string EXPANSION_NOTIFICATION_ICON_TERRITORY = "Territory";
static const string EXPANSION_NOTIFICATION_ICON_AIRDROP = "Airdrop";
static const string EXPANSION_NOTIFICATION_ICON_TRADER = "Trader";
static const string EXPANSION_NOTIFICATION_ICON_HOME = "Base";
static const string EXPANSION_NOTIFICATION_ICON_VEHICLE_CRASH = "Vehicle Crash";
static const string EXPANSION_NOTIFICATION_ICON_AI_MISSION = EXPANSION_NOTIFICATION_ICON_VEHICLE_CRASH;	// PLACEHOLDER

//! New icons
static const string EXPANSION_NOTIFICATION_ICON_INFO = "Info";
static const string EXPANSION_NOTIFICATION_ICON_MARKER = "Marker";
static const string EXPANSION_NOTIFICATION_ICON_PERSONA = "Persona";
static const string EXPANSION_NOTIFICATION_ICON_WATER_2 = "Water 2";
static const string EXPANSION_NOTIFICATION_ICON_MAP = "Map";
static const string EXPANSION_NOTIFICATION_ICON_SHIELD = "Shield";

//! Newer icons
static const string EXPANSION_NOTIFICATION_ICON_T_Batteries = "Batteries";
static const string EXPANSION_NOTIFICATION_ICON_T_Empty_Can = "Empty Can";
static const string EXPANSION_NOTIFICATION_ICON_T_Gun = "Gun";
static const string EXPANSION_NOTIFICATION_ICON_T_Walkie_Talkie = "Walkie Talkie";

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
static const string EXPANSION_FLAG_DAYZ_REFUGE = "dz\\gear\\camping\\Data\\Flag_refuge_co.paa";
static const string EXPANSION_FLAG_DAYZ_RSTA = "dz\\gear\\camping\\Data\\Flag_rsta_co.paa";
static const string EXPANSION_FLAG_DAYZ_SNAKE = "dz\\gear\\camping\\Data\\Flag_snake_co.paa";

//! Expansion flags
static const string EXPANSION_FLAG_LOGO_EXPANSION = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_expansion_co.paa";
static const string EXPANSION_FLAG_COLOR_WHITE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\colors\\flag_white_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_ARGENTINA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_argentina_co.paa";
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
static const string EXPANSION_FLAG_COUNTRIES_TURKEY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_turkey_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_UK = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_uk_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_UKRAINE = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_ukraine_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_USA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_usa_co.paa";

static const string EXPANSION_FLAG_COUNTRIES_SCOTLAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_scotland_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SWEDEN = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_sweden_co.paa";
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
static const string EXPANSION_FLAG_COUNTRIES_BORDURIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_borduria_co.paa";
static const string EXPANSION_FLAG_LOGO_BIOHAZARD = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_biohazard_co.paa";
static const string EXPANSION_FLAG_LOGO_ANYONEINCHERNO = "DayZExpansion\\Objects\\Structures\\Flags\\data\\logos\\flag_anyoneincherno_co.paa";

static const string EXPANSION_FLAG_COUNTRIES_IRELAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_ireland_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_ITALY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_italy_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_WALES = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_wales_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SWITZERLAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_switzerland_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SKILAND = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_skilanka_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SOUTHAFRICA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_southafrica_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_SICILY = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_sicily_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_OFFWITHHEAD = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_offwithhead_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_GIBRALTAR = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_gibraltar_co.paa";
static const string EXPANSION_FLAG_COUNTRIES_CZECHIA = "DayZExpansion\\Objects\\Structures\\Flags\\data\\countries\\flag_czechia_co.paa";
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

//! Storage save version
static const int EXPANSION_VERSION_CURRENT_SAVE = 48;

//! Starting with client settings v38, client and map marker save version are no longer linked to storage save version
static const int EXPANSION_VERSION_CLIENT_SETTING_SAVE = 44;
static const int EXPANSION_VERSION_MAP_MARKER_SAVE = 39;  //! Also used for groups

//! Expansion social media URLs
static const string EXPANSION_DISCORD_URL = "https://discord.gg/WabhFUa";
static const string EXPANSION_TWITTER_URL  = "https://twitter.com/DayZExpansion";
static const string EXPANSION_WIKI_URL = "https://github.com/salutesh/DayZ-Expansion-Scripts/wiki";
