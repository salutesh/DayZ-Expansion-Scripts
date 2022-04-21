/**
 * ExpansionLootLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionLootLocation
 * @brief		This class handle places where soldier will spawn
 **/
#ifdef EXPANSIONMODAI
class ExpansionLootLocation
{
	string Name;
	vector Position;
	int Items;
	ref TStringArray Loot;

	// ------------------------------------------------------------
	// Expansion ExpansionLootLocation
	// ------------------------------------------------------------
	void ExpansionLootLocation(string name, vector position, int items, ref TStringArray loot)
	{
		Name = name;
		Position = position;
		Items = items;
		Loot = loot;
	}
}
#endif