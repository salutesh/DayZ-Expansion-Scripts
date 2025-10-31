class eAIState_Struggle: eAIState
{
	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (Guard() == eAITransition.FAIL) return EXIT;

		unit.StartActionObject(ActionUnrestrainSelf, null);
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (!unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.m_eAI_IsInventoryVisible) return eAITransition.FAIL;
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
}
