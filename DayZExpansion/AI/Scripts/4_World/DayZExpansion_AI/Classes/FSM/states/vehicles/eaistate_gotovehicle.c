class eAIState_GoToVehicle : eAIState
{
	EntityAI m_Entity;

	int m_Seat;
	vector m_Position;

	Transport m_Transport;
	ExpansionVehicle m_Vehicle;

	override void OnEntry(string Event, ExpansionState From)
	{
		if (From)
		{
			EnScript.GetClassVar(From, "m_Entity", 0, m_Entity);
			EnScript.GetClassVar(From, "m_Seat", 0, m_Seat);
		}

		auto vehCmd = unit.GetCommand_Vehicle();
		if (vehCmd)
		{
			m_Seat = vehCmd.GetVehicleSeat();
		}

		Class.CastTo(m_Transport, m_Entity);
		ExpansionVehicle.Get(m_Vehicle, m_Entity);
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
		if (m_Transport)
		{
			if (m_Transport.CrewMember(m_Seat))
			{
				if (m_Vehicle)
				{
					m_Vehicle.ReserveSeat(m_Seat, null);
					return EXIT;
				}
			}

			vector direction;
			m_Transport.CrewEntryWS(m_Seat, m_Position, direction);
				
			unit.OverrideTargetPosition(m_Position);
		}

		return EXIT;
	}
};
