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
class ExpansionSoldierLocation
{
	float x, y, z;
	string Name;
	ref TStringArray Clothes;

	// ------------------------------------------------------------
	// Expansion ExpansionSoldierLocation
	// ------------------------------------------------------------
	void ExpansionSoldierLocation(string name, float xx, float yy, float zz, ref TStringArray clothes)
	{
		x = xx;
		y = yy;
		z = zz;
		Name = name;
		Clothes = clothes;
	}
}