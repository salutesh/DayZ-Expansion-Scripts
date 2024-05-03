modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	void eAI_EvaluateHit(InventoryItem weapon)
	{
		EvaluateHit(weapon);
	}

	float eAI_GetAttackTypeByDistanceToTarget(EntityAI pTarget, EMeleeTargetType pTargetType = EMeleeTargetType.ALIGNABLE)
	{
		return GetAttackTypeByDistanceToTarget(pTarget, pTargetType);
	}
};