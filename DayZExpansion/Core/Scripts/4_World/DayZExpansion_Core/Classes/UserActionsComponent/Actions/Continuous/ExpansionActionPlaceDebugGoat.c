class ExpansionActionPlaceDebugGoat: ActionPlaceObject
{
	override void MoveEntityToFinalPosition(ActionData action_data, vector position, vector orientation)
	{
		EntityAI entity_for_placing = action_data.m_MainItem;
		vector rotation_matrix[3];
		float direction[4];
		InventoryLocation source = new InventoryLocation;
		InventoryLocation destination = new InventoryLocation;
		
		Math3D.YawPitchRollMatrix(orientation, rotation_matrix);
		Math3D.MatrixToQuat(rotation_matrix, direction);

		vector entityMinMax[2];
		if (!entity_for_placing.GetCollisionBox(entityMinMax))
			entity_for_placing.ClippingInfo(entityMinMax);

		float entityOffsetY = entityMinMax[0][1];
		position[1] = position[1] - entityOffsetY;

		if (entity_for_placing.GetInventory().GetCurrentInventoryLocation(source))
		{
			destination.SetGroundEx(entity_for_placing, position, direction);
			
			if (g_Game.IsMultiplayer())
				action_data.m_Player.ServerTakeToDst(source, destination);
			else // singleplayer
				MoveEntityToFinalPositionSinglePlayer(action_data, source, destination);
		
		}
	}
}
