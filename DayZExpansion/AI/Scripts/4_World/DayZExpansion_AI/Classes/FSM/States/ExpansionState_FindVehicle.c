class ExpansionState_FindVehicle : eAIState
{
	EntityAI entity;
	int seat;

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
				entity = node.m_Value;
				minDistance = distance;
			}
			node = node.m_Next;
		}
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		if (To && entity)
		{
			EnScript.SetClassVar(To, "entity", 0, entity);
			EnScript.SetClassVar(To, "seat", 0, seat);
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (entity)
		{
			unit.OverrideTargetPosition(entity.GetPosition());
		}

		return CONTINUE;
	}
};
