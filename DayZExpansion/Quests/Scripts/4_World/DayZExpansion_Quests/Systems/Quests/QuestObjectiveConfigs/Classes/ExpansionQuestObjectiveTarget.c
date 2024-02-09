/**
 * ExpansionQuestObjectiveTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! DEPRECATED BUT HERE FOR CONVERSION
class ExpansionQuestObjectiveTarget
{
	int Amount = -1;
	ref array<string> ClassNames = new array<string>;
	bool CountSelfKill;
	bool SpecialWeapon = false;
	ref array<string> AllowedWeapons = new array<string>;
	ref array<string> ExcludedClassNames = new array<string>;

	bool CountAIPlayers = false;
	ref array<string> AllowedTargetFactions = new array<string>;
};