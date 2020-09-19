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
		if ( !super.DismantleCondition( player, target, item, camera_check ) )
			if ( !GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
				return false;

		if ( player.IsInSafeZone() )
			return false;

		TerritoryFlag flag;
		if ( Class.CastTo( flag, target.GetObject() ) && flag.HasExpansionTerritoryInformation() )
		{
			if ( player.IsInsideOwnTerritory() )
				return true;

			return !GetExpansionSettings().GetBaseBuilding().SimpleTerritory;
		} else if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();

		if ( GetExpansionSettings().GetBaseBuilding().DismantleAnywhere )
			return DismantleAnywhereCondition( player, target, item, camera_check );
				
		return GetExpansionSettings().GetBaseBuilding().DismantleOutsideTerritory;
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
				ConstructionPart construction_part = construction.GetConstructionPartToDismantle( part_name, item );
				
				if ( construction_part )
				{
					//camera and position checks
					if (!player.GetInputController().CameraIsFreeLook() )
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