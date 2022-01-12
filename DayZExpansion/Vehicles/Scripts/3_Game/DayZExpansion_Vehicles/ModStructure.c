/**
 * ModStructure.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

#ifdef CF_MODULE_MODSTORAGE
modded class ModStructure
{
	const static CF_String DZ_Expansion_Vehicles = "DZ_Expansion_Vehicles";

	override bool CF_OnLoad(CF_String modName)
	{
		if (!CF_String.EqualsIgnoreCase(modName, DZ_Expansion_Vehicles))
		{
			return super.CF_OnLoad(modName);
		}

		SetStorageVersion(EXPANSION_VERSION_CURRENT_SAVE);
		return true;
	}
};
#endif