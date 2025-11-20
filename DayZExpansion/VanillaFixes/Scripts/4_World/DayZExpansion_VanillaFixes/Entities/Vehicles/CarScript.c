modded class CarScript
{
	//! Prevent being able to glitch through base walls etc when getting out vehicle
	//! Mostly a verbatim copy of vanilla IsAreaAtDoorFree, but stretches the extents so that the whole area from door to
	//! get out position is checked, and has exception for our BB because it has no collision box (TODO: why?! we should fix this)
	override bool IsAreaAtDoorFree( int currentSeat, float maxAllowedObjHeight, inout vector extents, out vector transform[4] )
	{
		GetTransform(transform);
		
		vector crewPos;
		vector crewDir;
		CrewEntry( currentSeat, crewPos, crewDir );

		//! Adjust bbox depth so it stretches to the vehicle
		extents[2] = extents[2] + crewDir.Length();

		vector entry[4];
		entry[2] = crewDir;
		entry[0] = vector.Up * crewDir;
		entry[1] = entry[2] * entry[0];
		entry[3] = crewPos + crewDir * 0.5;  //! Adjust bbox position for altered depth
		
		Math3D.MatrixMultiply4( transform, entry, transform );
		
		vector position = transform[3];
		vector orientation = Math3D.MatrixToAngles(transform);
		
		position[1] = position[1] + maxAllowedObjHeight + (extents[1] * 0.5);
		
		array<Object> excluded = new array<Object>;
		array<Object> collided = new array<Object>;
		
		excluded.Insert(this);
		
		g_Game.IsBoxColliding(position, orientation, extents, excluded, collided);
		
		orientation.RotationMatrixFromAngles(transform);
		transform[3] = position;
		
		foreach (Object o : collided)
		{
			EntityAI e = EntityAI.Cast(o);			
			if (IsIgnoredObject(o))
				continue;
			
			vector minmax[2];
			if (o.GetCollisionBox(minmax))
				return false;

			if (o.IsInherited(ExpansionBaseBuilding))
				return false;
		}

		return true;
	}

	override bool IsAreaAtDoorFree( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
	{
		vector transform[4];
		vector extents;
		
		extents[0] = horizontalExtents;
		if (playerHeight > maxAllowedObjHeight)
			extents[1] = playerHeight - maxAllowedObjHeight;  //! extents height needs to be adjusted for maxAllowedObjHeight
		else
			extents[1] = playerHeight;
		extents[2] = horizontalExtents;
		
		return IsAreaAtDoorFree( currentSeat, maxAllowedObjHeight, extents, transform );
	}
	
	override Shape DebugFreeAreaAtDoor( int currentSeat, float maxAllowedObjHeight = 0.5, float horizontalExtents = 0.5, float playerHeight = 1.7 )
	{
		int color = ARGB(20, 0, 255, 0);
		
		vector transform[4];
		vector extents;
		
		extents[0] = horizontalExtents;
		if (playerHeight > maxAllowedObjHeight)
			extents[1] = playerHeight - maxAllowedObjHeight;  //! extents height needs to be adjusted for maxAllowedObjHeight
		else
			extents[1] = playerHeight;
		extents[2] = horizontalExtents;
		
		if (!IsAreaAtDoorFree( currentSeat, maxAllowedObjHeight, extents, transform ))
		{
			color = ARGB(20, 255, 0, 0);
		}
		
		Shape shape = Debug.DrawBox(-extents * 0.5, extents * 0.5, color);
		shape.SetMatrix(transform);
		return shape;
	}
}
