/**
 * ActionLowerFlag.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionLowerFlag
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//! In this scenario the player don't need tools to dismantle. 
		if ( player.IsInTerritory() ) {
			if ( player.IsInsideOwnTerritory() ) {
				//! The player is in his own territory, let's parse the usual vanilla checks then
				return super.ActionCondition( player, target, item );
			}

			//! The player is not in his territory, so he can't interact
			return false;
		}

		//! This is not a territory, Everyone can interact with then
		return super.ActionCondition( player, target, item );
	}
	
};