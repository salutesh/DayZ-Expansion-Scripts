modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		if (super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack))
		{
			eAI_UpdateAttackCooldown();
			return true;
		}

		return false;
	}

	void eAI_UpdateAttackCooldown()
	{
		//! TODO: Measure actual values based on attack type and weapon
		float cooldown = Math.RandomFloat(0.0, 1.0);

		switch (m_HitType)
		{
			case EMeleeHitType.LIGHT:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			case EMeleeHitType.WPN_STAB:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			case EMeleeHitType.HEAVY:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			case EMeleeHitType.SPRINT:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			case EMeleeHitType.KICK:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			case EMeleeHitType.WPN_HIT:
			case EMeleeHitType.WPN_HIT_BUTTSTOCK:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;

			default:
				m_Player.m_eAI_AttackCooldown = cooldown;
				break;
		}
	}
}
