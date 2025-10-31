class eAIState_FindVehicle : eAIState
{
	EntityAI m_Entity;
	int m_Seat;

	override void OnEntry(string Event, ExpansionState From)
	{
		float minDistance = float.MAX;
		float distance;

		auto node = CarScript.s_Expansion_AllVehicles.m_Head;
		while (node)
		{
			distance = vector.Distance(unit.GetPosition(), node.m_Value.GetPosition());
			if (distance < minDistance)
			{
				m_Entity = node.m_Value;
				minDistance = distance;
			}
			node = node.m_Next;
		}
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		if (To && m_Entity)
		{
			EnScript.SetClassVar(To, "m_Entity", 0, m_Entity);
			EnScript.SetClassVar(To, "m_Seat", 0, m_Seat);
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (m_Entity)
		{
			unit.OverrideTargetPosition(m_Entity.GetPosition());
		}

		return CONTINUE;
	}
};
