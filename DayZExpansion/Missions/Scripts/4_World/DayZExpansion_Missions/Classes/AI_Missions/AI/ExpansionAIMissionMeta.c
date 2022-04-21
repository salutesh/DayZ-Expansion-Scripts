/**
 * ExpansionAIMissionMeta.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAIMissionMeta
 * @brief		This class handle ai missions meta
 **/
#ifdef EXPANSIONMODAI
class ExpansionAIMissionMeta
{
	string Name;
	string Description;
	
	// ------------------------------------------------------------
	// Expansion ExpansionAIMissionMeta
	// ------------------------------------------------------------
	void ExpansionAIMissionMeta(string name, string description)
	{
		Name = name;
		Description = description;
	}
}
#endif