class ExpansionAIPatrolLoadBalancing
{
	int MinPlayers;
	int MaxPlayers;
	int MaxPatrols;

	[NonSerialized()]
	ref ExpansionAIPatrolLoadBalancingTracker m_PatrolCountTracker;

	void ExpansionAIPatrolLoadBalancing(int minPlayers = 1, int maxPlayers = 255, int maxPatrols = -1)
	{
		MinPlayers = minPlayers;
		MaxPlayers = maxPlayers;
		MaxPatrols = maxPatrols;
	}
}
