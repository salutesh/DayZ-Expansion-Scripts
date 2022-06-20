/**
 * DayZExpansion_Core_Defines.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Enable it if you want to enable logging with EXPrint
//#define EXPANSIONEXPRINT
//#define EXPANSIONTRACE

//! Enable it if you want to enable logging with EXLogPrint
//#define EXPANSIONEXLOGPRINT

//! Enable mod load order debug prints
//#define EXPANSION_LOADORDER_DEBUG

//! Enable storage/modstorage debug prints
//#define EXPANSION_STORAGE_DEBUG

//! Enable it if you want to enable item attaching logic
#define EXPANSION_ITEM_ATTACHING_DISABLE
//#define EXPANSION_ITEM_ATTACHING_LOGGING

#define EXPANSION_PLAYER_ATTACHMENT
//#define EXPANSION_PLAYER_ATTACHMENT_LOG

//#define EXPANSION_PLAYER_ATTACHMENT_CANATTACH_OVERRIDE

//! Enable it if you want to enable skinning system debug logs
//#define EXPANSION_SKIN_LOGGING

//! Used for third party mods, if they want to know if expansion is loaded
#define EXPANSIONMODCORE

#ifdef CF_DebugUI
#define EXPANSION_DEBUG_UI_AI
#define EXPANSION_DEBUG_UI_HOLOGRAM
#define EXPANSION_DEBUG_UI_FALLCOMMAND
#define EXPANSION_DEBUG_UI_VEHICLE
#define EXPANSION_DEBUG_UI_CAMERA
#endif

#ifdef SERVER //! Don't comment these, diag exe server crashes
#define EXPANSION_DEBUG_SHAPES_DISABLE
#endif
