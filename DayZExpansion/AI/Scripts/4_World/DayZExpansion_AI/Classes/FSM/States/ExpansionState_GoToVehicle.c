class ExpansionState_GoToVehicle : eAIState
{
	EntityAI entity;

	int seat;
	vector position;

	Transport transport;
	ExpansionVehicleBase vehicle;

	override void OnEntry(string Event, ExpansionState From)
	{
		if (From)
		{
			EnScript.GetClassVar(From, "entity", 0, entity);
			EnScript.GetClassVar(From, "seat", 0, seat);
		}

		auto vehCmd = unit.GetCommand_VehicleAI();
		if (vehCmd)
		{
			seat = vehCmd.GetVehicleSeat();
		}

		Class.CastTo(transport, entity);
		Class.CastTo(vehicle, entity);
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
		if (transport)
		{
			vector direction;
			transport.CrewEntryWS(seat, position, direction);
				
			unit.OverrideTargetPosition(position);
		}

		return CONTINUE;
	}
};
