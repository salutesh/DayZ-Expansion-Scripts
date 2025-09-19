/**
 * Hud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Hud
{
	bool Expansion_CanShowHUDElements(DayZPlayer player = null)
	{
		return true;
	}

	bool Expansion_IsVisible()
	{
		return true;
	}
};