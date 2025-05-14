modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		if (super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack))
		{
			//! TODO: Measure actual values based on attack type and weapon
			switch (m_HitType)
			{
				case EMeleeHitType.LIGHT:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				case EMeleeHitType.WPN_STAB:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				case EMeleeHitType.HEAVY:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				case EMeleeHitType.SPRINT:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				case EMeleeHitType.KICK:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				case EMeleeHitType.WPN_HIT:
				case EMeleeHitType.WPN_HIT_BUTTSTOCK:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;

				default:
					m_Player.m_eAI_AttackCooldown = 1.0;
					break;
			}

			return true;
		}

		return false;
	}
}
