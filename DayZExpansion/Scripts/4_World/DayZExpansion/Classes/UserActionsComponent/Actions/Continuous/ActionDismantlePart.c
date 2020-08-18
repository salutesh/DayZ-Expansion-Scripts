modded class ActionDismantlePart
{
	/*
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "Player: " + player );
		ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "Target: " + target );
		ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "Item: " + item );

		if ( target )
		{
			Object tobj = target.GetObject();
			int ci = target.GetComponentIndex();
			ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "Target Obj: " + tobj );
				string part_name = tobj.GetActionComponentName( ci );
			ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "ci: " + ci );
			ExpansionDebugger.Display( EXPANSION_DEBUG_BASEBUILDING, "part_name: " + part_name );
		}

		return super.ActionCondition( player, target, item );
	}
	*/

	override protected bool DismantleCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		ExpansionDebugger.Push( EXPANSION_DEBUG_BASEBUILDING );
		bool dismantleWithoutTerritory 	= GetExpansionSettings().GetBaseBuilding().DismantleOutsideTerritory;
		bool DismantleAnywhere			= GetExpansionSettings().GetBaseBuilding().DismantleAnywhere;

		if ( !super.DismantleCondition( player, target, item, camera_check ) )
			return false;
			
		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();

		if ( player.IsInSafeZone() )
			return false;

		if ( DismantleAnywhere )
			return DismantleAnywhereCondition( player, target, item, camera_check );
		
		return dismantleWithoutTerritory;
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