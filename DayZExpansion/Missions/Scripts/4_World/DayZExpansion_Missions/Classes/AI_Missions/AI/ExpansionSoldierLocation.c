/**
 * ExpansionSoldierLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSoldierLocation
 * @brief		This class handle places where soldier will spawn
 **/
#ifdef EXPANSIONMODAI
class ExpansionSoldierLocation
{
	vector StartPosition;
	array<vector> Waypoints;
	string Loadout;
	string Faction;
	int GroupSize;

	// ------------------------------------------------------------
	// Expansion ExpansionSoldierLocation
	// ------------------------------------------------------------
	void ExpansionSoldierLocation(vector startpos, array<vector> waypoints, string loadout, string faction, int size = 1)
	{
		StartPosition = startpos;
		Waypoints = waypoints;
		Loadout = loadout;
		Faction = faction;
		GroupSize = size;
	}
}
#endif