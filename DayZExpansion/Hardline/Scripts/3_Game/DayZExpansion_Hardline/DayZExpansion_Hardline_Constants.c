/**
 * DayZExpansion_Hardline_Constants.c
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
 * Expansion hardline system folder paths
 * @{
 */
static const string EXPANSION_HARDLINE_FOLDER = EXPANSION_FOLDER + "Hardline\\";
static const string EXPANSION_HARDLINE_PLAYERDATA_FOLDER = EXPANSION_HARDLINE_FOLDER + "PlayerData\\";

static const string EXPANSION_HARDLINE_SETTINGS = EXPANSION_MISSION_SETTINGS_FOLDER + "HardlineSettings.json";

class ExpansionHardlineItemRarityColor
{
	static const int Poor = ARGB(30, 157, 157, 157);
	static const int Common = ARGB(30, 255, 255, 255);
	static const int Uncommon = ARGB(30, 30, 255, 0);
	static const int Rare = ARGB(30, 0, 112, 221);
	static const int Epic = ARGB(30, 163, 53, 238);
	static const int Legendary = ARGB(30, 255, 128, 0);
	static const int Mythic = ARGB(30, 230, 204, 128);
	static const int Exotic = ARGB(30, 199, 38, 81);
	static const int Quest = ARGB(30, 241, 196, 15);
}
