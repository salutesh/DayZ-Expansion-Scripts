class ExpansionState_Fighting_Evade: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		unit.eAI_ForceSideStep(Math.RandomFloat(0.3, 0.5));
		unit.OverrideMovementSpeed(true, 3);
		auto target = unit.GetTarget();
		if (target)
			unit.OverrideTargetPosition(target);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementSpeed(false, 0);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsSideStepping())
			return CONTINUE;
		
		return EXIT;
	}
};
