/**
 * DayZExpansion_Hardline_Enums.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note Rarity is encoded in 4 bits in market module, so please limit to 16 rarities!
enum ExpansionHardlineItemRarity
{
	NONE,
	Poor,
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Mythic,
	Exotic,
	Quest,
	Collectable,
	Ingredient,
};
