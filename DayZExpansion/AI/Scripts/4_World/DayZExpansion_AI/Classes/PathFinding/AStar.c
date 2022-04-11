class AStar
{
	static float Heuristic(vector a, vector b)
	{
		return Math.AbsFloat(a[0] - b[0]) + Math.AbsFloat(a[1] - b[1]) + Math.AbsFloat(a[2] - b[2]);
	}

	static void Perform(PathNode start, PathNode goal, inout array<PathNode> path)
	{
		PriorityQueue<PathNode> queue = new PriorityQueue<PathNode>();
		queue.Enqueue(start, 0);

		map<PathNode, PathNode> mappedPath();
		map<PathNode, float> cost();

		PathNode current = null;

		mappedPath[start] = null;
		cost[start] = 0.0;
		
		int idx = 0;
		
		//Print(start);
		//Print(goal);

		while (queue.Count() > 0)
		{
			idx++;
			if (idx > 5)
			{
				Sleep(1);
				idx = 0;
			}
			
			current = queue.Dequeue();

			//Print(current);

			if (current == goal) break;

			foreach (PathNode next : current.m_Neighbours)
			{
				//Print(next);
				float newCost = cost[current] + vector.DistanceSq(goal.m_Position, next.m_Position);
				if ((!cost.Contains(next) || newCost < cost[next]))
				{
					cost[next] = newCost;
					float priority = newCost + Heuristic(next.m_Position, goal.m_Position);
					queue.Enqueue(next, priority);
					mappedPath[next] = current;
				}
			}
		}

		while (current)
		{
			path.Insert(current);
			
			if (!Class.CastTo(current, mappedPath[current])) return;
		}
	}
};