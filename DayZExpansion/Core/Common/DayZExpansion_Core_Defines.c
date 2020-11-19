/**
 * DayZExpansion_Core_Defines.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Enable it if you want to enable logging with EXPrint
//#define EXPANSIONEXPRINT

//! Enable it if you want to enable logging with EXLogPrint
//#define EXPANSIONEXLOGPRINT

//! Enable it if you want to enable item attaching logic 
#define EXPANSION_ITEM_ATTACHING_DISABLE
//#define EXPANSION_ITEM_ATTACHING_LOGGING

//! Enable it if you want to DISABLE market
#define EXPANSION_MARKET_DISABLE

//! Enable it if you want to DISABLE chicken stuff
#define EXPANSION_CHICKEN_DISABLE

#define EXPANSION_HELI_TEMP
#define EXPANSION_HELI_WIND
//#define EXPANSION_HELI_WIND_DEBUG
#define EXPANSION_HELI_CONTACT_NORMAL_DISABLE
//#define EXPANSION_USING_TRANSPORT_BASE

//#define EXPANSION_DEBUG_SHAPES_DISABLE
//#define EXPANSION_WHEEL_DEBUG_DISABLE

#define EXPANSION_UNIVERSAL_WHEEL_DISABLE

#define EXPANSION_PLAYER_ATTACHMENT
//#define EXPANSION_PLAYER_ATTACHMENT_LOG

//! Enable to check player count mins for missions
#define EXPANSION_MISSION_PLAYERCOUNT_CHECK
//#define EXPANSION_MISSION_HORDE

//! Enableif you want to enable vehicle towing
#define EXPANSION_VEHICLE_TOWING

// Enable it if you want mission event debug logs enabled
//#define EXPANSION_MISSION_EVENT_DEBUG

// Enable it if you want market debug logs enabled
//#define EXPANSION_MARKET_MODULE_DEBUG

//! Enable it if you want party module debug logs enabled
//#define EXPANSION_PARTY_MODULE_DEBUG

//! Enable it if you want to enable kill-feed module debug logs
//#define EXPANSION_KILLFEED_MODULE_DEBUG

//! Enable it if you want to marker module debug logs
//#define EXPANSION_MARKER_MODULE_DEBUG

//! Enable it if you want to enable mapping module debug logs
//#define EXPANSION_MAPPING_MODULE_DEBUG

//! Enable it if you want to enable territry module debug logs
//#define EXPANSION_TERRITORY_MODULE_DEBUG

//! Enable it if you want to enable territry dev debug
//#define EXPANSION_TERRITORY_DEV_DEBUG

//! Enable it if you want to enable COT territory module debug logs
//#define EXPANSION_COT_TERRITORY_MODULE_DEBUG

//! Enable it if you want to enable map menu debug logs
//#define EXPANSION_MAP_MENU_DEBUG

//! Enable it if you want to enable client settings debug logs
//#define EXPANSION_CLIENT_SETTINGS_DEBUG

//! Enable it if you want to enable skinings system debug logs
//#define EXPANSION_SKIN_LOGGING

//! Enable it if you want to enable car key debug logs
//#define EXPANSION_CARKEY_LOGGING

//! Enable it if you want to disable skinings system skin replacement on object creation
#define EXPANSION_SKIN_REPLACEMENT_DISABLE

//! Enable if if you want to enable car debug logs
//#define EXPANSION_CARSCRIPT_LOGGING

//! JMVehicleSpawnerSettings override for 'Vehicle_'
//#define EXPANSION_COT_USE_VEHICLE

//#define EXPANSION_WEAPON_DEBUG_OBJECTS
//#define EXPANSION_WEAPON_BULLET_TRACE

#define EXPANSION_DISABLE_FALL

#define EXPANSION_PARTICLE_VOLUMETRIC_CLOUDS_DISABLE

//#define EXPANSION_INTERIOR_RANDONMIZATION

//! Enable to disable character szene main menu changes
#define EXPANSION_MAINMENU_NEW_DISABLE

#define EXPANSION_COT_VEHICLE_FORCE_DEFAULTS

//! Used for third party mods, if they want to know if expansion is loaded
#define EXPANSIONMOD

//! Enable or disable bunnyhopping 
//#define EXPANSION_BUNNY_HOPPING

#ifdef SERVER //! Don't comment these, diag exe server crashes
#define EXPANSION_DEBUG_SHAPES_DISABLE
#define EXPANSION_WHEEL_DEBUG_DISABLE
#endif