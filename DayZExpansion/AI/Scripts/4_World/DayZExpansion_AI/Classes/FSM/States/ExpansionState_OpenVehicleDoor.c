class ExpansionState_OpenVehicleDoor : ExpansionState_GoToVehicle
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
			entity.SetAnimationPhase(source, 1.0);
		}

		time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		time += DeltaTime;

		if (time > 1.0 || !source || entity.GetAnimationPhase(source) > 0.95)
		{
			return EXIT;
		}

		return CONTINUE;
	}
};
