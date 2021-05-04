/**
 * ActionWorldFlagActionSwitch.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionWorldFlagActionSwitch
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//! In this scenario the player don't need tools to dismantle. 
		if ( player.IsInTerritory() )
		{
			if ( player.IsInsideOwnTerritory() )
			{
				//! The player is in his own territory, let's parse the usual vanilla checks then
				return super.ActionCondition( player, target, item );
			}
			
			TerritoryFlag totem = TerritoryFlag.Cast( target.GetObject() );
			if ( !totem )
				return false;
			
			float state = totem.GetAnimationPhase( "flag_mast" );

			//! Allow player to interact with the flag since he need to lower the flag to dismantle with tools
			if ( GetExpansionSettings().GetBaseBuilding().DismantleFlagRequireTools == DismantleFlagMode.WithTools && state < 0.1 )
				return super.ActionCondition( player, target, item );
				
			//! The player is not in his territory, so he can't interact
			return false;
		}

		//! This is not a territory, Everyone can interact with then
		return super.ActionCondition( player, target, item );
	}
	
};