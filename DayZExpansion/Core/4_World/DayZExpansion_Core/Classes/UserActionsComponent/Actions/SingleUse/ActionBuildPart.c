/**
 * ActionBuildPart.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionBuildPart
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//! Now it will check if we are in a safezone
		if ( player.IsInSafeZone() )
			return false;

		return super.ActionCondition( player, target, item );
	}
}