// TODO: optimize for Enforce Script

ref ExpansionNavMesh g_ExpansionNavMesh;

class ExpansionNavMesh
{
	autoptr array<ref ExpansionNavMeshPolygon> m_Polygons = new array<ref ExpansionNavMeshPolygon>();

	autoptr array<Shape> m_DebugShapes = new array<Shape>();

	ref array<vector> m_LastFoundPath;

	void ExpansionNavMesh()
	{
		g_ExpansionNavMesh = this;
	}

	void ~ExpansionNavMesh()
	{
		DestroyDebugShapes();
	}

	void Generate(LOD lod)
	{
		array<Selection> selections();
		lod.GetSelections(selections);

		foreach (auto selection : selections)
		{
			int count = selection.GetVertexCount();
			if (count != 4)
			{
				return;
			}

			ExpansionNavMeshPolygon polygon = new ExpansionNavMeshPolygon(lod, selection);
			m_Polygons.Insert(polygon);
		}
		
		for (int i = 0; i < m_Polygons.Count(); i++)
        {
            for (int j = i + 1; j < m_Polygons.Count(); j++)
            {
                auto pg0 = m_Polygons[i];
                auto pg1 = m_Polygons[j];

				if (!pg0.ShouldBeConnecting(pg1))
				{
					continue;
				}

				pg0.Add(pg1);
			}
		}
	}

	/**
	 * @brief Finds a path within the mesh
	 * 
	 * @param start A positional value relative to the navmesh
	 * @param end The target position for the path to reach
	 * @param path [out] An array of vectors
	 */
	void FindPath(vector start, vector end, out array<vector> path)
	{
		ExpansionNavMeshPolygon startPoly = SamplePolygon(start, start);
		ExpansionNavMeshPolygon endPoly = SamplePolygon(end, end);
		
		if (startPoly == endPoly)
		{
			// No need to perform a search, we know the start and end already match
			path.Resize(2);
			path[0] = start;
			path[1] = end;
		}
		else
		{
			array<ExpansionNavMeshPolygon> polyPath();

			AStar<ExpansionNavMeshPolygon>.Perform(startPoly, endPoly, polyPath);

			int count = polyPath.Count();
			path.Resize(count);

			// AStar produces the path in reverse order
			for (int i = 0; i < count; i++)
			{				
				int j = count - (i + 1);
				path[j] = polyPath[i].m_Position;
			}

			// Update the first position in the path with the start input
			path[0] = start;

			// Check if a path was found to reach the destination
			ExpansionNavMeshPolygon finalPoly = polyPath[0];
			if (finalPoly == endPoly)
			{
				// Set the final position to the real end if it is within the polygon
				path[count - 1] = end;
			}
		}

		// Debug viewing
		m_LastFoundPath = path;
	}

	/**
	 * @brief Finds a position and polygon on the navmesh nearest to the input
	 *  
	 * @param position Input position
	 * @param sampledPosition The position found
	 * @return The polygon the position was found on
	 */
	ExpansionNavMeshPolygon SamplePolygon(vector position, out vector sampledPosition)
	{
		ExpansionNavMeshPolygon result;
		sampledPosition = position;

		float minDistance = float.MAX;
		foreach (auto polygon : m_Polygons)
		{
			vector otherPos = polygon.SamplePosition(position);

			float distance = vector.DistanceSq(position, otherPos);
			if (minDistance < distance)
			{
				continue;
			}

			minDistance = distance;

			sampledPosition = otherPos;
			result = polygon;
		}

		return result;
	}

	/**
	 * @brief Finds a position within the navmesh nearest to the input
	 *  
	 * @param position Input position
	 * @param sampledPosition The position found
	 * @return If a position was found
	 */
	bool SamplePosition(vector position, out vector sampledPosition)
	{
		sampledPosition = position;

		float minDistance = float.MAX;
		foreach (auto polygon : m_Polygons)
		{
			vector otherPos = polygon.SamplePosition(position);

			float distance = vector.DistanceSq(position, otherPos);
			if (minDistance < distance)
			{
				continue;
			}

			minDistance = distance;

			sampledPosition = otherPos;
		}

		return true;
	}

	void DrawDebug(Object parent)
	{
#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DrawDebug");
#endif

		DestroyDebugShapes();

		foreach (auto polygon : m_Polygons)
		{
			polygon.DrawDebug(parent, this);
		}

		if (false && m_LastFoundPath && m_LastFoundPath.Count() > 0)
		{			
			vector points[2];
			vector offset = "0 0.01 0";
			
			for (int i = 0; i < m_LastFoundPath.Count() - 1; i++)
			{
				points[0] = parent.ModelToWorld(m_LastFoundPath[i] + offset);
				points[1] = parent.ModelToWorld(m_LastFoundPath[i + 1] + offset);

				AddDebugShape(Shape.CreateLines(0xffff0000, ShapeFlags.NOZBUFFER, points, 2));
				
				AddDebugShape(Shape.CreateSphere(0x1fff5500, ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE, points[0], 0.05));
			}
			
			AddDebugShape(Shape.CreateSphere(0x1fff5500, ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE, points[1], 0.05));
		}
	}

	vector GetPositionAtCursor(Object ignore)
	{
		vector dir = GetGame().GetCurrentCameraDirection();
		vector from = GetGame().GetCurrentCameraPosition();

		PhxInteractionLayers layerMask;

		layerMask |= PhxInteractionLayers.DEFAULT;
		layerMask |= PhxInteractionLayers.VEHICLE;
		layerMask |= PhxInteractionLayers.CHARACTER;
		layerMask |= PhxInteractionLayers.BUILDING;
		layerMask |= PhxInteractionLayers.ROADWAY;
		layerMask |= PhxInteractionLayers.TERRAIN;
		layerMask |= PhxInteractionLayers.WATERLAYER;  //! Doesn't seem to work?
		layerMask |= PhxInteractionLayers.ITEM_LARGE;

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		float hitFraction;
		DayZPhysics.RayCastBullet(from, from + (dir * 20.0), layerMask, ignore, hitObject, hitPosition, hitNormal, hitFraction);
		return hitPosition;
	}

	void DebugScripts()
	{
		return;
		
		DayZPlayerImplement player;
		if (!Class.CastTo(player, GetGame().GetPlayer()))
			return;

		vector cursorPosition = g_Expansion_Car.WorldToModel(GetPositionAtCursor(player));
		vector snappedPosition = vector.Zero;
		vector playerPosition = g_Expansion_Car.WorldToModel(player.GetPosition());

		array<vector> path();
		FindPath(playerPosition, cursorPosition, path);

		SamplePosition(playerPosition, snappedPosition);

		DrawDebug(g_Expansion_Car);

		AddDebugShape(Shape.CreateSphere(0x1F0000FF, ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE, g_Expansion_Car.ModelToWorld(cursorPosition), 0.1));
		AddDebugShape(Shape.CreateSphere(0x5f00ff00, ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE, g_Expansion_Car.ModelToWorld(snappedPosition), 0.1));
		AddDebugShape(Shape.CreateSphere(0x5fff0000, ShapeFlags.NOZBUFFER | ShapeFlags.TRANSP | ShapeFlags.NOOUTLINE, g_Expansion_Car.ModelToWorld(playerPosition), 0.05));
	}

	void AddDebugShape(Shape shape)
	{
		m_DebugShapes.Insert(shape);
	}

	void DestroyDebugShapes()
	{
		for (int i = 0; i < m_DebugShapes.Count(); ++i)
		{
			if (!m_DebugShapes[i])
			{
				continue;
			}
			
			m_DebugShapes[i].Destroy();
		}

		m_DebugShapes.Clear();
	}
};
