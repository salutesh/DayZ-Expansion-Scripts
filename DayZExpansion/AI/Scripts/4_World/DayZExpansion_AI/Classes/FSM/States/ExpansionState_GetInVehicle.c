class ExpansionState_GetInVehicle : ExpansionState_GoToVehicle
{
	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		if (transport)
		{
			unit.Notify_Transport(transport, seat);
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		auto vehCmd = unit.GetCommand_VehicleAI();
		if (vehCmd && vehCmd.IsGettingIn())
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
