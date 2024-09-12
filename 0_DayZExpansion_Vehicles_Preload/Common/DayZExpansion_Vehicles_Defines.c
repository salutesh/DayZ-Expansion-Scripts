/**
 * DayZExpansion_Vehicles_Defines.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#define EXPANSIONMODVEHICLE

//#define EXPANSION_VEHICLE_DOOR_JOINTS

#define EXPANSION_HELI_WIND

//#define EXPANSION_WHEEL_DEBUG_DISABLE

#define EXPANSION_UNIVERSAL_WHEEL_DISABLE

//! Enableif you want to enable vehicle towing
#define EXPANSION_VEHICLE_TOWING

//#define EXPANSION_SOUND_CRASH_IDENTIFY

//! Enable it if you want to enable car key debug logs
//#define EXPANSION_CARKEY_LOGGING

//! Enable if if you want to enable car debug logs
//#define EXPANSION_CARSCRIPT_LOGGING

#ifndef DIAG_DEVELOPER
#define EXPANSION_VEHICLES_CONFIG_DISPLAYNAME_TEMP
#endif

//#define EXPANSION_COT_VEHICLE_FORCE_DEFAULTS

#ifdef CF_MVVM
#define EXPANSION_VEHICLE_NEW_HUD
#endif

//! Enable it if you want to enable skinning system debug logs
//#define EXPANSION_VEHICLE_SKIN_LOGGING

#ifndef EXPANSIONCOTESPMODULE
#define EXPANSIONCOTESPMODULE
#endif

#ifdef SERVER //! Don't comment these, diag exe server crashes
#define EXPANSION_WHEEL_DEBUG_DISABLE
#endif

#define EXPANSION_OBSOLETE_CAMERA
