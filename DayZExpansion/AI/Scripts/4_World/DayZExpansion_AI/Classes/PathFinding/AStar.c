class AStar<Class NodeType>
{
	static float Heuristic(vector a, vector b)
	{
		return Math.AbsFloat(a[0] - b[0]) + Math.AbsFloat(a[1] - b[1]) + Math.AbsFloat(a[2] - b[2]);
	}

	static void Perform(NodeType start, NodeType goal, inout array<NodeType> path)
	{
		PriorityQueue<NodeType> queue = new PriorityQueue<NodeType>();
		queue.Enqueue(start, 0);

		map<NodeType, NodeType> mappedPath();
		map<NodeType, float> cost();

		NodeType current = null;

		mappedPath[start] = null;
		cost[start] = 0.0;

		int idx = 0;

		while (queue.Count() > 0)
		{
			idx++;
			if (idx > 5)
			{
				//Sleep(1);
				idx = 0;
			}

			current = queue.Dequeue();

			if (current == goal)
				break;

			foreach (NodeType next : current.m_Neighbours)
			{
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
			current = mappedPath[current];
		}
	}
};
