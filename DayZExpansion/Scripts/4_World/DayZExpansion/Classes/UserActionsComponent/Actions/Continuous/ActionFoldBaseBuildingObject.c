/**
 * ActionFoldBaseBuildingObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionFoldBaseBuildingObject
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) && !GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
			return false;

		if ( player.IsInSafeZone() )
			return false;

		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();
			
		return true;
	}
}