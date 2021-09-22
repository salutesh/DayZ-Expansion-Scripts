/**
 * SalineMdfr.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SalineMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		//! Allow blood regen in safezone
		if (player.IsAlive() && player.IsInSafeZone())
			player.SetAllowDamage(true);

		super.OnTick(player, deltaT);

		if (player.IsAlive() && player.IsInSafeZone())
			player.SetAllowDamage(false);
	}
}
