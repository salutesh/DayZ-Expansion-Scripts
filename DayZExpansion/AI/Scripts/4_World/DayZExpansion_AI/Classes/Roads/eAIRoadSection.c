class eAIRoadSection: eAIRoadNodeBase
{
	ref array<eAIRoadNode> m_Nodes = new array<eAIRoadNode>();

	eAIRoadNode m_Head;
	eAIRoadNode m_HeadNext;
	eAIRoadNode m_Tail;
	eAIRoadNode m_TailNext;

	ref array<vector> m_Points = new array<vector>();

	void Init()
	{
		int i;
		int j;

		Print("  COUNT: " + m_Nodes.Count());
		for (j = 0; j < m_Nodes.Count(); j++)
		{
			Print("    m_Nodes[" + j + "]: " + m_Nodes[j]);
			Print("    m_Nodes[" + j + "].m_Index: " + m_Nodes[j].m_Index);
			Print("    m_Nodes[" + j + "].m_SectionNode: " + m_Nodes[j].m_SectionNode);
		}

		if (m_Nodes.Count() < 3)
		{
			Error("Sections must have 3 or more nodes.");
			return;
		}

		m_Radius = 0;

		m_Head = null;
		m_Tail = null;

		m_Position = "0 0 0";
		int count = m_Nodes.Count();
		for (i = 0; i < count; i++)
		{
			if (!m_Head && m_Nodes[i].m_SectionNode)
			{
				m_Head = m_Nodes[i];

				int idx;
				for (j = 0; j < m_Head.Count(); j++)
				{
					idx = m_Nodes.Find(eAIRoadNode.Cast(m_Head[j]));
					if (idx != -1)
					{
						m_HeadNext = eAIRoadNode.Cast(m_Head[j]);
						break;
					}
				}
			}
			
			for (j = 0; j < count; j++)
			{
				float dist = vector.Distance(m_Nodes[i].m_Position, m_Nodes[j].m_Position) * 0.5;
				if (dist > m_Radius)
				{
					m_Position = (m_Nodes[i].m_Position + m_Nodes[j].m_Position) * 0.5;
					m_Radius = dist;
				}
			}
		}
		
		m_Radius *= 1.1;
		
		Print("  m_Head: " + m_Head);
		Print("  m_HeadNext: " + m_HeadNext);
		
		m_Nodes.Clear();

		m_Nodes.Insert(m_Head);
		m_Nodes.Insert(m_HeadNext);

		int INDEX = 0;
		eAIRoadNode current;
		
		current = m_Nodes[INDEX];
		Print("    m_Nodes[" + INDEX + "]: " + current);
		Print("    m_Nodes[" + INDEX + "].m_Index: " + current.m_Index);
		Print("    m_Nodes[" + INDEX + "].m_SectionNode: " + current.m_SectionNode);
		INDEX++;
		
		current = m_Nodes[INDEX];
		Print("    m_Nodes[" + INDEX + "]: " + current);
		Print("    m_Nodes[" + INDEX + "].m_Index: " + current.m_Index);
		Print("    m_Nodes[" + INDEX + "].m_SectionNode: " + current.m_SectionNode);
		INDEX++;

		while (true)
		{
			eAIRoadNode current0 = eAIRoadNode.Cast(current[0]);
			eAIRoadNode current1 = eAIRoadNode.Cast(current[1]);
			if (m_Nodes.Find(current0) == -1) current = current0;
			else if (m_Nodes.Find(current1) == -1) current = current1;
			else break;

			m_Nodes.Insert(current);

			Print("    m_Nodes[" + INDEX + "]: " + current);
			Print("    m_Nodes[" + INDEX + "].m_Index: " + current.m_Index);
			Print("    m_Nodes[" + INDEX + "].m_SectionNode: " + current.m_SectionNode);
			INDEX++;

			if (current.Count() != 2) break;
		}

		Print("  NEW COUNT: " + m_Nodes.Count());

		m_Tail = m_Nodes[m_Nodes.Count() - 1];
		m_TailNext = m_Nodes[m_Nodes.Count() - 2];

		Print("  m_Tail: " + m_Tail);
		Print("  m_TailNext: " + m_TailNext);
		
		Print("  m_Head.m_SectionNode: " + m_Head.m_SectionNode);
		Print("  m_Tail.m_SectionNode: " + m_Tail.m_SectionNode);

		m_Head.m_SectionNode.Add(m_Tail.m_SectionNode);
	}

	bool ContainsPath(eAIRoadNode node)
	{
		return m_Nodes.Find(node) != -1;
	}

	void UpdateNodeClosest(vector position, inout array<eAIRoadNode> foundNodes)
	{
		eAIRoadNode node = null;
		float distance = m_Radius * 2.0;
		for (int i = 1; i < m_Nodes.Count() - 1; i++)
		{
			float dist = vector.Distance(m_Nodes[i].m_Position, position);
			if (dist < distance)
			{
				node = m_Nodes[i];
				distance = dist;
			}
		}
		if (node)
		{
			foundNodes.Insert(node);
		}
	}
};