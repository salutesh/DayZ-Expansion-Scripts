/**
 * ExpansionFlagKits_Colors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionFlagKitExpansion
 * @brief		
 **/
class ExpansionFlagKitExpansion: ExpansionFlagKitBase
{
	override string GetTexturePath()
	{
		return EXPANSION_FLAG_LOGO_EXPANSION;
	}
}

/**@class		ExpansionFlagKitBiohazard
 * @brief		
 **/
class ExpansionFlagKitBiohazard: ExpansionFlagKitBase
{
	override string GetTexturePath()
	{
		return EXPANSION_FLAG_COUNTRIES_BIOHAZARD;
	}
}

/**@class		ExpansionFlagKitAnyoneInCherno
 * @brief		
 **/
class ExpansionFlagKitAnyoneInCherno: ExpansionFlagKitBase
{
	override string GetTexturePath()
	{
		return EXPANSION_FLAG_COUNTRIES_ANYONEINCHERNO;
	}
}