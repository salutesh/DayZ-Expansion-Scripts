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
		if ( !super.DismantleCondition( player, target, item, camera_check ) && !GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
			return false;

		//! Now it will check if we are in a safezone
		if ( player.IsInSafeZone() )
			return false;

		//! is this a territory flag
		TerritoryFlag flag;
		if ( Class.CastTo( flag, target.GetObject() ) && flag.HasExpansionTerritoryInformation() )
		{
			//! is it his territory ?
			if ( player.IsInsideOwnTerritory() )
			{
				//! If SimpleTerritory is enabled, the player can't dismantle parts to prevent ressources exploits.
				return !GetExpansionSettings().GetBaseBuilding().SimpleTerritory;
			}
				
			////! Allow the raider to dismantle with tools
			//if ( GetExpansionSettings().GetBaseBuilding().DismantleFlagRequireTools == 1 )
			//		return super.ActionCondition( player, target, item );

			//! Raider is not allowed to dismantle with tools
			return false;
		}
		//! It's not a territory flag, but his he in a territory
		else if ( player.IsInTerritory() )
		{
			//! It's his territory ?
			if ( player.IsInsideOwnTerritory() )
			{
				//! Can he dismantle anywhere ?
				if ( GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
					return DismantleAnywhereCondition( player, target, item, camera_check );

				//! Apply normal logic
				return true;
			}
		}

		//! Can you dismantle outside territoies ?
		if ( GetExpansionSettings().GetBaseBuilding().DismantleOutsideTerritory )
		{
			//! Can he dismantle anywhere and outside territoies ?
			if ( GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
				return DismantleAnywhereCondition( player, target, item, camera_check );

			return true;
		}
		
		//! Looks like nothing fit, so let's say no
		return false;
	}
	
	protected bool DismantleAnywhereCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		if ( player && !player.IsLeaning() )
		{
			Object target_object = target.GetObject();
			if ( target_object && target_object.CanUseConstruction() )
			{
				string part_name = target_object.GetActionComponentName( target.GetComponentIndex() );
				
				BaseBuildingBase base_building = BaseBuildingBase.Cast( target_object );
				Construction construction = base_building.GetConstruction();		
				ConstructionPart construction_part = construction.GetConstructionPartToDestroy( part_name );
				
				if ( construction_part )
				{
					//camera and position checks
					if ( IsInReach(player, target, UAMaxDistances.DEFAULT) && !player.GetInputController().CameraIsFreeLook() )
					{						
						ConstructionActionData construction_action_data = player.GetConstructionActionData();
						construction_action_data.SetTargetPart( construction_part );
						
						return true;				
					}
				}
			}
		}
		
		return false;
	}
}