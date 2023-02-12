class ExpansionState_GetOutVehicle : ExpansionState_GoToVehicle
{
	float time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		auto vehCmd = unit.GetCommand_VehicleAI();
		if (vehCmd && !vehCmd.IsGettingIn())
		{
			vehCmd.GetOutVehicle();
			CarScript car;
			if (Class.CastTo(car, vehCmd.GetTransport()))
				car.Expansion_ReserveSeat(vehCmd.GetVehicleSeat(), null);
		}

		time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		time += DeltaTime;

		auto vehCmd = unit.GetCommand_VehicleAI();
		if ((vehCmd && vehCmd.IsGettingOut()) || time < 2.0)
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
