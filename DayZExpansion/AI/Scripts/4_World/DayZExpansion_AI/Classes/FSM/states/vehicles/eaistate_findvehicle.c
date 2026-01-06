class eAIState_FindVehicle : eAIState
{
	EntityAI m_Entity;
	int m_Seat;

	override void OnEntry(string Event, ExpansionState From)
	{
		float minDistance = float.MAX;
		float distance;

		auto node = ExpansionVehicle.s_All.m_Head;
		while (node)
		{
			EntityAI entity = node.m_Value.GetEntity();
			distance = vector.Distance(unit.GetPosition(), entity.GetPosition());
			if (distance < minDistance)
			{
				m_Entity = entity;
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
