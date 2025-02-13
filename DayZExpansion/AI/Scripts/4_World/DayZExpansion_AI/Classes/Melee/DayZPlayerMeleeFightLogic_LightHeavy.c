modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		if (!m_Player.Expansion_IsAI() || m_Player.GetIdentity())
			return super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack);

		return false;
	}

	void eAI_EvaluateHit(InventoryItem weapon)
	{
		EvaluateHit(weapon);
	}

	float eAI_GetAttackTypeByDistanceToTarget(EntityAI pTarget, EMeleeTargetType pTargetType = EMeleeTargetType.ALIGNABLE)
	{
		return GetAttackTypeByDistanceToTarget(pTarget, pTargetType);
	}
};