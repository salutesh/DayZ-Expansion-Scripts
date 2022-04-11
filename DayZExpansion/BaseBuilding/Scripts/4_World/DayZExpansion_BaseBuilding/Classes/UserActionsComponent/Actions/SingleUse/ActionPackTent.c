/**
 * ActionPackTent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionPackTent
{	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetExpansionSettings().GetBaseBuilding().DismantleAnywhere && player.IsInTerritory() )
			return player.IsInsideOwnTerritory();

		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();
            
		return true;
	}
};