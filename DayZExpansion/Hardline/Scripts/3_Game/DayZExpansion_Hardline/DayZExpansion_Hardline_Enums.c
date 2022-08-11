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

enum ExpansionHardlineItemRarity
{
	NONE = 0
	Poor,
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary,
	Mythic,
	Exotic
};

enum ExpansionHardlineItemTier
{
	TIER_1 = 1,
	TIER_2,
	TIER_3,
	TIER_4,
	TIER_5,
	TIER_6,
    TIER_7,
    TIER_8
};

enum ExpansionHardlineRank
{
	INVALID = 0,

	Bambi,
	Survivor,

	//! Hero ranks
	Scout,
	Pathfinder,
	Hero,
	Superhero,
	Legend,

	//! Bandit ranks
	Kleptomaniac,
	Bully,
	Bandit,
	Killer,
	Madman
};
