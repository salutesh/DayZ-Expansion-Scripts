/**
 * ModStructure.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef CF_MODULE_MODSTORAGE
modded class ModStructure
{
	override bool OnLoad( string modName )
	{
		if ( modName != "DZ_Expansion_Vehicles" )
			return super.OnLoad( modName );

		//! Set the storage version for this mod
		SetStorageVersion( EXPANSION_VERSION_CURRENT_SAVE );

		return true;
	}
};
#endif