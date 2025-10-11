class ExpansionPathNode
{
	vector m_Position;
	float m_Radius;

	int m_Flags;

	ref set<ExpansionPathNode> m_Neighbours = new set<ExpansionPathNode>();

	int Count()
	{
		return m_Neighbours.Count();
	}

	ExpansionPathNode Get(int index)
	{
		return m_Neighbours[index];
	}

	int Find(ExpansionPathNode node)
	{
		return m_Neighbours.Find(node);
	}

	bool Contains(ExpansionPathNode node)
	{
		return m_Neighbours.Find(node) != -1;
	}

	void Add(ExpansionPathNode node)
	{
		if (node == this) return;
		
		m_Neighbours.Insert(node);
		node.m_Neighbours.Insert(this);
	}

	void Remove(ExpansionPathNode node)
	{
		if (node == this) return;
		
		int idx = m_Neighbours.Find(node);
		if (idx != -1) m_Neighbours.Remove(idx);

		idx = node.m_Neighbours.Find(this);
		if (idx != -1) node.m_Neighbours.Remove(idx);
	}
};
