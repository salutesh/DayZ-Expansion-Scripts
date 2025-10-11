class eAIState_Interacting: eAIState
{
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.OverrideTargetPosition(unit.GetPosition() + unit.GetDirection() * unit.Expansion_GetMovementSpeed() * 0.333333);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.IsUnconscious()) return EXIT;
		
		if (unit.eAI_HasInteractingPlayers()) return CONTINUE;

		return EXIT;
	}

	int Guard()
	{
		if (!unit.eAI_HasInteractingPlayers()) return eAITransition.FAIL;
		
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		if (unit.eAI_HasInteractingPlayers()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
