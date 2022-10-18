class ExpansionState_FindVehicle : eAIState
{
	EntityAI entity;
	int seat;

	override void OnEntry(string Event, ExpansionState From)
	{
		float minDistance = float.MAX;
		float distance;

		auto cars = CarScript.GetAll();
		foreach (CarScript car : cars)
		{
			distance = vector.Distance(unit.GetPosition(), car.GetPosition());
			if (distance < minDistance)
			{
				entity = car;
				minDistance = distance;
			}
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
