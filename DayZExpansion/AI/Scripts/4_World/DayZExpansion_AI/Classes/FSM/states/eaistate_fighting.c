class eAIState_Fighting: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		if (unit.GetEmoteManager().IsEmotePlaying())
			unit.GetEmoteManager().ServerRequestEmoteCancel();
		
		unit.eAI_SetIsFightingFSM(true);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.eAI_SetIsFightingFSM(false);
	}

	int Guard()
	{
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		
		if (unit.GetThreatToSelf() < 0.4) return eAITransition.FAIL;
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		if (unit.IsInTransport()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}

	int GuardIdle()
	{
		if (unit.GetThreatToSelf() > 0.2) return eAITransition.FAIL;
		
		if (unit.IsFighting()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
