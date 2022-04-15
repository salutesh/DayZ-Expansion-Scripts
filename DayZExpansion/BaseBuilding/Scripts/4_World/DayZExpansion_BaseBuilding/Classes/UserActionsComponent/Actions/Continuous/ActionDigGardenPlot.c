/**
 * ActionDigGardenPlot.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionDigGardenPlot
{	
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;
		
		if ( player.IsInSafeZone() )
			return false;

		if ( GetExpansionSettings().GetTerritory() && !GetExpansionSettings().GetTerritory().EnableTerritories )
			return super.ActionCondition( player, target, item );
			
		if ( GetExpansionSettings().GetBaseBuilding() )
		{
			int i;

			//! Garden plot can be placed if not in enemy territory or if item is whitelisted

			if ( player.IsInTerritory() )
			{
				if ( player.IsInsideOwnTerritory() )
					return true;

				//! Items cannot be checked on client because they are not synced (network optimization). Server will do the check for us
				if ( !GetGame().IsServer() )
					return true;

				//! Player is in enemy territory
				foreach (string deployableInEnemyTerritory: GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory)
				{
					if ( deployableInEnemyTerritory == "GardenPlot" )
						return true;
				}
			}
			else
			{
				if ( GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory )
					return true;

				//! Items cannot be checked on client because they are not synced (network optimization). Server will do the check for us
				if ( !GetGame().IsServer() )
					return true;
					
				foreach (string deployableOutsideTerritory: GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory)
				{
					if ( deployableOutsideTerritory == "GardenPlot" )
						return true;
				}
			}
		}
		
		return false;
	}
};
