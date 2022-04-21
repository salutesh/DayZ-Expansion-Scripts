/**
 * ExpansionMissionEventSoldierObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODAI
class ExpansionMissionEventSoldierObject
{
	string ObjectName;
	vector Position;
	vector Orientation;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionMissionEventSoldierObject( string name, vector position, vector orientation )
	{
		ObjectName = name;
		Position = position;
		Orientation = orientation;
	}
};
#endif