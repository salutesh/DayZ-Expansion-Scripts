class ExpansionState_GetInVehicle : ExpansionState_GoToVehicle
{
	float time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		auto vehCmd = unit.GetCommand_VehicleAI();

		if (!vehCmd && transport && !unit.IsInTransport() && !unit.eAI_GetTransport())
		{
			//! Wait until unit is idle to prevent animation glitches
			if (!unit.Expansion_IsAnimationIdle())
			{
				return CONTINUE;
			}

			unit.Notify_Transport(transport, seat);
		}

		time += DeltaTime;

		if (!vehCmd || vehCmd.IsGettingIn() || time < 1.5)
		{
			return CONTINUE;
		}

		return EXIT;
	}
};
