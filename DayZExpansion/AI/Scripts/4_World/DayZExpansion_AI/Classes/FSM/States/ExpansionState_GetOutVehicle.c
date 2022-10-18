class ExpansionState_GetOutVehicle : ExpansionState_GoToVehicle
{
	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		auto vehCmd = unit.GetCommand_VehicleAI();
		if (vehCmd && vehCmd.IsGettingIn())
		{
			vehCmd.GetOutVehicle();
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		auto vehCmd = unit.GetCommand_VehicleAI();
		if (vehCmd && vehCmd.IsGettingOut())
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
