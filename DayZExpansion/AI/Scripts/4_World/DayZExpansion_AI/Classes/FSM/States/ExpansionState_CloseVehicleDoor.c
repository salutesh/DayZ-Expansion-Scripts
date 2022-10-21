class ExpansionState_CloseVehicleDoor : ExpansionState_GoToVehicle
{
	string source;
	float time;

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		bool hasDoor;

		ExpansionFSMHelper.DoorAnimationSource(entity, seat, hasDoor, source);
		if (hasDoor)
		{
			entity.SetAnimationPhase(source, 0.0);
		}

		time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		time += DeltaTime;

		if (time > 0.5 || entity.GetAnimationPhase(source) < 0.5)
		{
			return EXIT;
		}

		return CONTINUE;
	}
};
