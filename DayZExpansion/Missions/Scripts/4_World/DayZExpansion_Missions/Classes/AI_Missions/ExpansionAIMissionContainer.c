/**
 * ExpansionAIMissionContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAIMissionContainer
 * @brief		This class handle ai missions meta
 **/
class ExpansionAIMissionContainer
{
	ref TStringArray Classnames;
	bool IsLocked;

	vector Position;
	vector Orientation;
	
	// ------------------------------------------------------------
	// Expansion ExpansionAIMissionContainer
	// ------------------------------------------------------------
	void ExpansionAIMissionContainer(TStringArray classnames, vector position, vector orientation = "0 0 0", bool islocked = false)
	{
		Classnames = classnames;
		IsLocked = islocked;
		Position = position;
		Orientation = orientation;
	}
};
