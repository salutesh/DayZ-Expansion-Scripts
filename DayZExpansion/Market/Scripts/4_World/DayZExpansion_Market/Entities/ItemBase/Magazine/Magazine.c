/**
 * Magazine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MagazineStorage
{
	//! Fix NULL pointer when giving trader NPCs guns with mags
	override void OnInventoryEnter(Man player)
	{
		PlayerBase p = PlayerBase.Cast(player);
		if (p)
			super.OnInventoryEnter(player);
	}
}
