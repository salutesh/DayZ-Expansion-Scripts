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
	ref array < ref ExpansionAIMissionContainer > Containers;
	int ItemCount;
	ref array < ref ExpansionAirdropLoot > Loot;

	// ------------------------------------------------------------
	// Expansion ExpansionLootLocation
	// ------------------------------------------------------------
	void ExpansionLootLocation(array < ref ExpansionAIMissionContainer > containers, int itemcount, array < ref ExpansionAirdropLoot > loot)
	{
		Containers = containers;
		ItemCount = itemcount;
		Loot = loot;
	}
}
#endif