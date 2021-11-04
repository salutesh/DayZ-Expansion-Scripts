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

#ifndef EXPANSION_SAFEZONE_BLOOD_TRANSFER_DISABLE
modded class HemolyticReactionMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		//! Don't block hemolytic reaction in safezones
		if (player.IsInSafeZone())
			player.SetAllowDamage(true);

		super.OnTick(player, deltaT);

		if (player.IsInSafeZone())
			player.SetAllowDamage(false);
	}
}
#endif
