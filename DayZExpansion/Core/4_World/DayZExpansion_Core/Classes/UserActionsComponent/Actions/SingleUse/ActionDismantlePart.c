/**
 * ActionDismantlePart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionDismantlePart
{
	override protected bool DismantleCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		//! Check if nothing is wrong with the basic checks, will skip only if DismantleAnywhere is enabled
		if ( !super.DismantleCondition( player, target, item, camera_check ) )
			return false;

		//! Now it will check if we are in a safezone
		if ( player.IsInSafeZone() )
			return false;

		return true;
	}
}