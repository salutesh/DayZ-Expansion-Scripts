/**
 * ExpansionAirdropLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAirdropLocation
 * @brief		This class handle places where airdrop will drop
 **/
class ExpansionAirdropLocation
{
	float x, y;
	string Name;
	float Radius;

	// ------------------------------------------------------------
	// Expansion ExpansionAirdropLocation
	// ------------------------------------------------------------
	void ExpansionAirdropLocation(float xx, float yy, float radius, string name)
	{
		x = xx;
		y = yy;
		Radius = radius;
		Name = name;
	}
}