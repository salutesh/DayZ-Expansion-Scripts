class eAIRoadNodeSection extends eAIRoadNodeBase
{
	eAIRoadNode m_Node;

	void PathTo(eAIRoadNodeSection target, inout array<PathNode> path)
	{
		eAIRoadSection section = m_Sections[m_Neighbours.Find(target)];
		
		if (target.m_Node == section.m_Tail)
		{
			m_Node.PathTo(section.m_Head, path);

			return;
		}
		
		if (target.m_Node == section.m_Head)
		{
			m_Node.PathTo(section.m_Tail, path);

			return;
		}
	}
};