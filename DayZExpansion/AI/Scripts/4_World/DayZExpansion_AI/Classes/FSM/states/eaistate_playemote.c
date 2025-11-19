class eAIState_PlayEmote: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		unit.Expansion_PlayEmote();
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		return EXIT;
	}

	int Guard()
	{
		if (!unit.m_Expansion_EmoteID) return eAITransition.FAIL;
		if (unit.GetEmoteManager().IsEmotePlaying() || unit.GetEmoteManager().Expansion_GetCurrentGesture() == unit.m_Expansion_EmoteID) return eAITransition.FAIL;
		
		if (unit.IsRaised()) return eAITransition.FAIL;
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsClimbing()) return eAITransition.FAIL;
		if (unit.IsFalling()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		if (!unit.GetCommand_Move()) return eAITransition.FAIL;
		if (unit.eAI_IsChangingStance()) return eAITransition.FAIL;
		
		if (unit.GetThreatToSelf(true) > 0.2) return eAITransition.FAIL;
		if (unit.IsFighting()) return eAITransition.FAIL;
		if (unit.GetWeaponManager().IsRunning()) return eAITransition.FAIL;
		if (unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		auto hands = unit.GetItemInHands();
		if (hands && hands.Expansion_CanBeUsedToBandage())
			return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
