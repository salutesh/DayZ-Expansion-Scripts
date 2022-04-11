class eAIRoadNodeJoinMap
{
	eAIRoadNode node;
	ref array<int> indices = new array<int>();

	void Add(int index)
	{
		indices.Insert(index);
	}
};

class eAIRoadConnection
{
	eAIRoadNode m_Node;
	vector m_Position;
	bool m_Completed;
};