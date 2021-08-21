/**
 * ActionDigGardenPlot.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		
		if ( ExpansionSafeZoneModule.IsInside( player.GetPosition() ) )
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

				//! Player is in enemy territory
				for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory.Count(); ++i)
				{
					if ( GetExpansionSettings().GetBaseBuilding().DeployableInsideAEnemyTerritory[i] == "GardenPlot" )
						return true;
				}
			}
			else
			{
				if ( GetExpansionSettings().GetBaseBuilding().AllowBuildingWithoutATerritory )
					return true;
					
				for (i = 0; i < GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory.Count(); ++i)
				{
					if ( GetExpansionSettings().GetBaseBuilding().DeployableOutsideATerritory[i] == "GardenPlot" )
						return true;
				}
			}
		}
		
		return false;
	}
};
