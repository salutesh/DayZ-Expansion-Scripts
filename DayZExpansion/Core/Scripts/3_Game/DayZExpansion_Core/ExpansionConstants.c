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

/**
 * Expansion chat channels
 * @{
 */
enum ExpansionChatChannels
{
	//! Vanilla
	CCLocal = 0, // https://www.youtube.com/watch?v=nN66uAbiJJw
	CCSystem = 1,
	CCAdmin = 2, // https://www.youtube.com/watch?v=W3q8Od5qJio
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

//! DPRECATED, constant is no longer used and is only kept for legacy reasons. Liquid types are set in CfgLiquidDefinitions,
//! and liquids can be accesed via vanilla Liquid EnfScript class, e.g. use Liquid::Expansion_GetDisplayName to get display name for UI
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
const int MENU_EXPANSION_MENU_END = MENU_EXPANSION_MENU_START + 4;

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
static const int COLOR_EXPANSION_NOTIFICATION_SUCCESS = ARGB( 255, 46, 204, 113 );
static const int COLOR_EXPANSION_NOTIFICATION_SUCCSESS = ARGB( 255, 46, 204, 113 );  //! Legacy typo
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
static const string EXPANSION_AI_FSM_FOLDER = EXPANSION_AI_FOLDER + "FSM\\";
static const string EXPANSION_MISSION_SETTINGS_FOLDER = EXPANSION_MISSION_FOLDER + "settings\\";
static const string EXPANSION_MISSIONS_FOLDER = EXPANSION_MISSION_FOLDER + "missions\\";
static const string EXPANSION_MISSION_MISSIONOBJECTS_FOLDER = EXPANSION_MISSION_FOLDER + "missionobjects\\";

static const string EXPANSION_MISSION_OBJECTS_FOLDER = EXPANSION_MISSION_FOLDER + "objects\\";
static const string EXPANSION_MISSION_TRADERS_FOLDER = EXPANSION_MISSION_FOLDER + "traders\\";

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
static const string EXPANSION_CORE_SETTINGS = EXPANSION_SETTINGS_FOLDER + "CoreSettings.json";
static const string EXPANSION_DEBUG_SETTINGS = EXPANSION_SETTINGS_FOLDER + "DebugSettings.json";  //! Legacy
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
 * Server Browser Menu
 * @{
 */
const int EXPANSION_IDC_Int_DIRECT			= 50000104;

/**
 * Player attachment
 * @{
 */
const int EXPANSION_INPUT_UDT_PLAYER_LINK	= 40003000;
const int EXPANSION_INPUT_UDT_GET_IN_TRANSPORT_UNLINK	= 40003001;
const int EXPANSION_INPUT_UDT_PERFORM_CLIMB	= 40003002;

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
 * Item tooltips
 * @{
 */
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
 * Expansion social media URLs
 * @{
 */
static const string EXPANSION_DISCORD_URL = "https://discord.gg/WabhFUa";
static const string EXPANSION_TWITTER_URL  = "https://twitter.com/DayZExpansion";
static const string EXPANSION_WIKI_URL = "https://github.com/salutesh/DayZ-Expansion-Scripts/wiki";

/**
 * Storage save version - DEPRECATED, NO LONGER USED, set individual storage version per mod in DZ_* ModStructure
 * @{
 */
static const int EXPANSION_VERSION_CURRENT_SAVE = 49;  //! DEPRECATED, NO LONGER USED, set individual storage version per mod in DZ_* ModStructure

/**
 * Client settings save version
 */
//! Starting with client settings v38, client and map marker save version are no longer linked to storage save version
static const int EXPANSION_VERSION_CLIENT_SETTING_SAVE = 56;
static const int EXPANSION_VERSION_MAP_MARKER_SAVE = 42;  //! Also used for groups since group data stores group marker data
