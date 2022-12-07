/**
 * AreaExposureMdfr.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @brief disable effects of exposure to contaminated area in safezones
modded class AreaExposureMdfr
{
	override void OnActivate(PlayerBase player)
	{
		if (!player.Expansion_IsInSafeZone())
			super.OnActivate(player);
	}

	override void OnTick(PlayerBase player, float deltaT)
	{
		if (!player.Expansion_IsInSafeZone())
			super.OnTick(player, deltaT);
	}
}
