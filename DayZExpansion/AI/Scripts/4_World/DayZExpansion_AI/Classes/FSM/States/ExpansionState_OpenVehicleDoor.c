class ExpansionState_OpenVehicleDoor : ExpansionState_GoToVehicle
{
	string source;
	float time;

	bool PerformTest(EntityAI entity, int seat)
	{
		bool hasDoor;
		string source;

		ExpansionFSMHelper.DoorAnimationSource(entity, seat, hasDoor, source);
		if (hasDoor)
		{
			return entity.GetAnimationPhase(source) > 0.5;
		}

		return false;
	}

	override void OnEntry(string Event, ExpansionState From)
	{
		super.OnEntry(Event, From);

		bool hasDoor;

		ExpansionFSMHelper.DoorAnimationSource(entity, seat, hasDoor, source);
		if (hasDoor)
		{
			vehicle.SetAnimationPhase(source, 1.0);
		}

		time = 0;
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		super.OnUpdate(DeltaTime, SimulationPrecision);

		time += DeltaTime;

		if (time > 0.5 || entity.GetAnimationPhase(source) > 0.5)
		{
			return EXIT;
		}

		return CONTINUE;
	}
};
