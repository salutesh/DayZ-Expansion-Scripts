/**
 * ModStructure.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

#ifdef EXPANSION_MODSTORAGE
class DZ_Expansion : ModStructure
{
	override void LoadData()
	{
		super.LoadData();

		SetStorageVersion(EXPANSION_VERSION_CURRENT_SAVE);
	}
};
#endif
