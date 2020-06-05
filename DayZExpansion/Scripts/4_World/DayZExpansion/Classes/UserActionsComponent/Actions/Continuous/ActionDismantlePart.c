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
		bool dismantleWithoutTerritory = GetExpansionSettings().GetBaseBuilding().DismantleOutsideTerritory;

		if ( !super.DismantleCondition( player, target, item, camera_check ) )
			return false;

		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory();
		
		return dismantleWithoutTerritory;
	}
}