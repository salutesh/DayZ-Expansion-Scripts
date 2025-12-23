class eAIState_Flank: eAIState
{
	eAITarget m_Target;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.OverrideMovementDirection(false, 0);
		unit.OverrideMovementSpeed(false, 0);
		
		unit.eAI_SetShouldTakeCover(true);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.eAI_SetShouldTakeCover(false);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (m_Target)
			unit.OverrideTargetPosition(m_Target);
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.GetThreatToSelf() >= 0.4) return eAITransition.FAIL;
				
		if (unit.IsInTransport()) return eAITransition.FAIL;
		
		auto group = unit.GetGroup();
		if (!group) return eAITransition.FAIL;
		if (!unit.m_eAI_EnableFlankingOutsideCombat && group.GetFormationState() != eAIGroupFormationState.FLANK && !group.IsInCombat())
			return eAITransition.FAIL;
		
		m_Target = unit.GetTarget();
		if (!m_Target || m_Target.IsItem() || unit.GetThreatToSelf(true) <= 0.152)
			return eAITransition.FAIL;
		
		//if (!unit.eAI_IsInFlankRange(m_Target)) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		auto group = unit.GetGroup();
		if (group && (unit.m_eAI_EnableFlankingOutsideCombat || group.GetFormationState() == eAIGroupFormationState.FLANK || group.IsInCombat()) && m_Target && !m_Target.IsItem() && unit.GetThreatToSelf(true) > 0.152 /*&& unit.eAI_IsInFlankRange(m_Target)*/) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
