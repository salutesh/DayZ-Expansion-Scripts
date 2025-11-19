class eAIState_GetOutVehicle : eAIState_GoToVehicle
{
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		auto vehCmd = unit.GetCommand_Vehicle();
		if (vehCmd && !vehCmd.IsGettingIn())
		{
			vehCmd.GetOutVehicle();
			CarScript car;
			if (Class.CastTo(car, vehCmd.GetTransport()))
				car.Expansion_ReserveSeat(vehCmd.GetVehicleSeat(), null);

			unit.LookAtDirection("0 0 1");  //! Reset look direction to prevent head snapping to previous direction after getting out
		}

		m_Time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		m_Time += DeltaTime;

		auto vehCmd = unit.GetCommand_Vehicle();
		if ((vehCmd && vehCmd.IsGettingOut()) || m_Time < 2.0)
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
