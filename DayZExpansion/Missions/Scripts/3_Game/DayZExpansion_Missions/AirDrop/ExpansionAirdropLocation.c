/**
 * ExpansionAirdropLocation.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	float x, z;
	string Name;
	float Radius;

	// ------------------------------------------------------------
	// Expansion ExpansionAirdropLocation
	// ------------------------------------------------------------
	void ExpansionAirdropLocation(float xx, float zz, float radius, string name)
	{
		x = xx;
		z = zz;
		Radius = radius;
		Name = name;
	}
};