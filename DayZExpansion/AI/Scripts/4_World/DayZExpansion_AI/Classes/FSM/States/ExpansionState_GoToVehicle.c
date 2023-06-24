class ExpansionState_GoToVehicle : eAIState
{
	EntityAI entity;

	int seat;
	vector position;

	Transport transport;
#ifdef EXPANSIONMODVEHICLE
	ExpansionVehicleBase vehicle;
#endif

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
#ifdef EXPANSIONMODVEHICLE
		Class.CastTo(vehicle, entity);
#endif
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
			if (transport.CrewMember(seat))
			{
				CarScript cs;
				if (Class.CastTo(cs, transport))
				{
					cs.Expansion_ReserveSeat(seat, null);
					return EXIT;
				}
			}

			vector direction;
			transport.CrewEntryWS(seat, position, direction);
				
			unit.OverrideTargetPosition(position);
		}

		return EXIT;
	}
};
