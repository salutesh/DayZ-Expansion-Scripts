/**
 * Expansion_PlayerList_Constants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

const int EXPANSION_MENU_MARKET = EXPANSION_MENU_START + 3;
const int EXPANSION_MENU_ATM = EXPANSION_MENU_START + 4;

//! Server
static const string EXPANSION_MARKET_FOLDER = EXPANSION_FOLDER + "Market\\";
static const string EXPANSION_TRADER_ZONES_FOLDER = EXPANSION_MISSION_FOLDER + "traderzones\\";
static const string EXPANSION_TRADER_FOLDER = EXPANSION_FOLDER + "Traders\\";
static const string EXPANSION_ATM_FOLDER = EXPANSION_FOLDER + "ATM\\";
static const string EXPANSION_MARKET_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "MarketSettings.json";

//! Client
static const string EXPANSION_MARKET_PRESETS_FOLDER = EXPANSION_FOLDER + "MarketPresets\\";
static const string EXPANSION_MARKET_WEAPON_PRESETS_FOLDER = EXPANSION_MARKET_PRESETS_FOLDER + "Weapons\\";
static const string EXPANSION_MARKET_CLOTHING_PRESETS_FOLDER = EXPANSION_MARKET_PRESETS_FOLDER + "Clothing\\";
static const string EXPANSION_MARKET_VESTS_PRESETS_FOLDER = EXPANSION_MARKET_CLOTHING_PRESETS_FOLDER + "Vests\\";