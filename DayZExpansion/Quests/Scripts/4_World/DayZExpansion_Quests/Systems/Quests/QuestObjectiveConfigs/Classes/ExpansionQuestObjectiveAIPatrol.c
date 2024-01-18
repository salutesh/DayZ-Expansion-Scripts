/**
 * ExpansionQuestObjectiveTreasureHunt.c
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
#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIPatrol
{
	int NPCUnits;
	ref array<vector> Waypoints = new array<vector>;
	string NPCSpeed;
	string NPCMode;
	string NPCFaction;
	string NPCFormation;
	string NPCLoadoutFile;
	float NPCAccuracyMin;
	float NPCAccuracyMax;
	autoptr array<string> ClassNames = new array<string>;
	autoptr array<string> AllowedWeapons = new array<string>;
};
#endif