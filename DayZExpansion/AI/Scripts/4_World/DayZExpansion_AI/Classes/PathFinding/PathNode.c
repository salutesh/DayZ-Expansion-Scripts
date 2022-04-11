class PathNode
{
	vector m_Position;
	float m_Radius;
	bool m_Valid;

	ref set<PathNode> m_Neighbours = new set<PathNode>();

	int Count()
	{
		return m_Neighbours.Count();
	}

	PathNode Get(int index)
	{
		return m_Neighbours[index];
	}

	int Find(PathNode node)
	{
		return m_Neighbours.Find(node);
	}

	bool Contains(PathNode node)
	{
		return m_Neighbours.Find(node) != -1;
	}

	void Add(PathNode node)
	{
		if (node == this) return;
		
		m_Neighbours.Insert(node);
		node.m_Neighbours.Insert(this);
	}

	void Remove(PathNode node)
	{
		if (node == this) return;
		
		int idx = m_Neighbours.Find(node);
		if (idx != -1) m_Neighbours.Remove(idx);

		idx = node.m_Neighbours.Find(this);
		if (idx != -1) node.m_Neighbours.Remove(idx);
	}
};