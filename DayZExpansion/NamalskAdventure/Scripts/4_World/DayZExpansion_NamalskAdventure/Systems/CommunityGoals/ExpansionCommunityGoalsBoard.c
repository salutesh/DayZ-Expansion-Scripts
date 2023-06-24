/**
 * ExpansionCommunityGoalsBoard.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCommunityGoalsBoard
{
	string ClassName;
	vector Position;
	vector Orientation;
	ExpansionNamalskAdventureFaction FactionID;

	void ExpansionCommunityGoalsBoard(string className, vector pos, vector ori, ExpansionNamalskAdventureFaction factionID)
	{
		ClassName = className;
		Position = pos;
		Orientation = ori;
		FactionID = factionID;
	}
};