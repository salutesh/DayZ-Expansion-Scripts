class ExpansionPathPoint
{
	vector Position;
	Object Parent;

	int Flags;

	ExpansionNavMesh NavMesh;

	ExpansionPathPoint Next;

	autoptr PGFilter m_Filter;

	void ExpansionPathPoint()
	{
		m_Filter = new PGFilter();
	}

	vector GetPosition()
	{
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			return Position.Multiply4(transform);
		}

		return Position;
	}

	void OnParentUpdate()
	{
		if (Parent)
		{
			NavMesh = ExpansionNavMeshes.Get(Parent);

			if (!NavMesh)
			{
				Parent = null;
			}
		}

		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);

			Position = Position.InvMultiply4(transform);
		}
		else
		{
			NavMesh = null;
		}
	}

	void UpdateFlags(ExpansionPathHandler handler)
	{
		float distance = 1.0;

		Flags = 0;
		Flags |= Sample(distance, PGPolyFlags.WALK);
		Flags |= Sample(distance, PGPolyFlags.DISABLED);
		Flags |= Sample(distance, PGPolyFlags.DOOR);
		Flags |= Sample(distance, PGPolyFlags.INSIDE);
		Flags |= Sample(distance, PGPolyFlags.SWIM);
		Flags |= Sample(distance, PGPolyFlags.SWIM_SEA);
		Flags |= Sample(distance, PGPolyFlags.LADDER);
		Flags |= Sample(distance, PGPolyFlags.JUMP_OVER);
		Flags |= Sample(distance, PGPolyFlags.JUMP_DOWN);
		Flags |= Sample(distance, PGPolyFlags.CLIMB);
		Flags |= Sample(distance, PGPolyFlags.CRAWL);
		Flags |= Sample(distance, PGPolyFlags.CROUCH);
		Flags |= Sample(distance, PGPolyFlags.UNREACHABLE);
	}

	int Sample(float distance, PGPolyFlags flag)
	{
		if (!Next)
		{
			return 0;
		}
		
		m_Filter.SetFlags(flag, PGPolyFlags.NONE, PGPolyFlags.NONE);
		
		m_Filter.SetCost(PGAreaType.NONE, 10.0);
		m_Filter.SetCost(PGAreaType.TERRAIN, 10.0);
		m_Filter.SetCost(PGAreaType.DOOR_OPENED, 10.0);
		m_Filter.SetCost(PGAreaType.DOOR_CLOSED, 10.0);
		m_Filter.SetCost(PGAreaType.OBJECTS, 10.0);
		m_Filter.SetCost(PGAreaType.BUILDING, 10.0);
		m_Filter.SetCost(PGAreaType.ROADWAY, 10.0);
		m_Filter.SetCost(PGAreaType.ROADWAY_BUILDING, 10.0);
		m_Filter.SetCost(PGAreaType.LADDER, 10.0);
		m_Filter.SetCost(PGAreaType.CRAWL, 10.0);
		m_Filter.SetCost(PGAreaType.CROUCH, 10.0);
		m_Filter.SetCost(PGAreaType.FENCE_WALL, 10.0);
		m_Filter.SetCost(PGAreaType.JUMP, 10.0);
		
		vector direction = vector.Direction(Position, Next.Position).Normalized();
		
		vector p0 = Position - (direction * distance);
		vector p1 = Position + (direction * distance);
		vector hitPos;
		vector hitNor;
		
		bool hit = GetGame().GetWorld().GetAIWorld().RaycastNavMesh(p0, p1, m_Filter, hitPos, hitNor);
				
		if (hit)
		{
			return flag;
		}

		return 0;
	}

	void Copy(ExpansionPathPoint other)
	{
		Position = other.Position;
		Parent = other.Parent;
		NavMesh = other.NavMesh;
	}

	void FindPath(ExpansionPathHandler pathFinding, inout array<vector> path)
	{
		vector transform[4];
		pathFinding.m_Unit.GetTransform(transform);
		
		FindPathFrom(transform[3], pathFinding, path);
	}

	void FindPathFrom(vector startPos, ExpansionPathHandler pathFinding, inout array<vector> path)
	{
		int i;
		
		PGFilter filter = pathFinding.GetFilter();
		
		if (Parent)
		{
			vector transform[4];
			Parent.GetTransform(transform);
			
			startPos = startPos.InvMultiply4(transform);

			NavMesh.FindPath(startPos, Position, filter, path);
			
			for (i = 0; i < path.Count(); i++)
			{
				path[i] = path[i].Multiply4(transform);
			}
		}
		else
		{
			pathFinding.m_AIWorld.FindPath(startPos, Position, filter, path);
		}

	#ifdef DIAG
		pathFinding.m_Unit.Expansion_DebugObject(11110, startPos, "ExpansionDebugNoticeMe_Blue", pathFinding.m_Unit.GetDirection());
		//EXTrace.Print(EXTrace.AI, pathFinding.m_Unit, "FindPath " + path.Count() + " points");
	#endif
	}
};
