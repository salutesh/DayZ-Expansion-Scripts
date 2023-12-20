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
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if (!GetExpansionSettings().GetBaseBuilding(false).IsLoaded())
			return false;

		if ( player.IsInTerritory() )
		{
			if (GetExpansionSettings().GetBaseBuilding().DismantleInsideTerritory)
				return true;

			//! If it was deployable, it's also packable
			ItemBase targetItem = ItemBase.Cast( target.GetParentOrObject() );
			if (ActionDeployObject.CanDeployInTerritory(player, targetItem))
				return true;

			if (GetGame().IsServer() && player.GetIdentity())
				ExpansionNotification("STR_EXPANSION_TERRITORY_TITLE", "STR_EXPANSION_TERRITORY_ENEMY_TERRITORY").Error(player.GetIdentity());

			return false;
		}

		return true;
	}
};
