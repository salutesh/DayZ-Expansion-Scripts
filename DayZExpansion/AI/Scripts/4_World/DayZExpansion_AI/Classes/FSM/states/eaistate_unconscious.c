class eAIState_Unconscious: eAIState
{
	float m_Time;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;
		unit.OverrideTargetPosition(unit.GetPosition());
		unit.OverrideMovementSpeed(true, 0);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		if (EXTrace.AI)
			EXPrint(unit.ToString() + " left uncon state after " + m_Time + " seconds");
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (m_Time > 3)
		{
			return EXIT;
		}
		
		if (!unit.IsUnconscious())
		{
			m_Time += DeltaTime;  //! Allow time to stand up so we don't instantly start firing
			if (unit.Expansion_IsAnimationIdle()) return EXIT;
		}
		
		return CONTINUE;
	}

	int Guard()
	{
		if (!unit.IsUnconscious()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
