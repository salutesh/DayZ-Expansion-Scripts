/**
 * ExpansionAIMissionInfected.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAIMissionInfected
 * @brief		This class handle ai missions meta
 **/
class ExpansionAIMissionInfected
{
	ref TStringArray Classnames;
	vector Position;
	float SpawnRadius;
	int Amount;
	
	// ------------------------------------------------------------
	// Expansion ExpansionAIMissionInfected
	// ------------------------------------------------------------
	void ExpansionAIMissionInfected(TStringArray classnames, vector position, float radius, int amount)
	{
		Classnames = classnames;
		Position = position;
		SpawnRadius = radius;
		Amount = amount;
	}
};
