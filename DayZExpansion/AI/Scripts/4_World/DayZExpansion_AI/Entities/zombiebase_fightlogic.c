modded class ZombieBase
{
	protected ref ExpansionInfectedType m_Expansion_InfectedType;
	protected ref ExpansionZombieFightLogic m_Expansion_ZombieFightLogic;
	float m_eAI_AttackCooldown;  //! vanilla m_AttackCoolDownTime unfortunately only ticks down in FightAttackLogic, not ChaseAttackLogic, and is sped up
	EntityAI m_Expansion_Target;

	void ZombieBase()
	{
		if (g_Game.IsServer() && GetExpansionSettings().GetAI().EnableZombieVehicleAttackHandler)
		{
			m_Expansion_InfectedType = ExpansionInfectedType.Get(Type());
			m_Expansion_ZombieFightLogic = new ExpansionZombieFightLogic(this);
		}
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)
	{
		//! @note attack starts *after* cooldown reaches zero
		if (m_eAI_AttackCooldown > 0)
			m_eAI_AttackCooldown -= pDt;

		super.CommandHandler(pDt, pCurrentCommandID, pCurrentCommandFinished);
	}

	override bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		m_Expansion_Target = pInputController.GetTargetEntity();

		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.FightLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		return super.FightLogic(pCurrentCommandID, pInputController, pDt);
	}

	override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.ChaseAttackLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		if (super.ChaseAttackLogic(pCurrentCommandID, pInputController, pDt))
		{
			if (EXTrace.AI)
				PrintFormat("%1 ChaseAttackLogic distance=%2 pitch=%3 type=%4 subtype=%5 %6 heavy=%7 cooldown=%8 probability=%9", this, m_ActualAttackType.m_Distance, m_ActualAttackType.m_Pitch, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype, m_ActualAttackType.m_AmmoType, m_ActualAttackType.m_IsHeavy, m_ActualAttackType.m_Cooldown, m_ActualAttackType.m_Probability);

			m_eAI_AttackCooldown = m_ActualAttackType.m_Cooldown;
			return true;
		}

		return false;
	}
	
	override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.FightAttackLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		if (super.FightAttackLogic(pCurrentCommandID, pInputController, pDt))
		{
			if (EXTrace.AI)
				PrintFormat("%1 FightAttackLogic distance=%2 pitch=%3 type=%4 subtype=%5 %6 heavy=%7 cooldown=%8 probability=%9", this, m_ActualAttackType.m_Distance, m_ActualAttackType.m_Pitch, m_ActualAttackType.m_Type, m_ActualAttackType.m_Subtype, m_ActualAttackType.m_AmmoType, m_ActualAttackType.m_IsHeavy, m_ActualAttackType.m_Cooldown, m_ActualAttackType.m_Probability);

			m_eAI_AttackCooldown = m_ActualAttackType.m_Cooldown;
			return true;
		}

		return false;
	}

	ExpansionInfectedType GetExpansionInfectedType()
	{
		return m_Expansion_InfectedType;
	}

	/**
	 * @brief get target entity during chase/attack
	 */
	EntityAI Expansion_GetActualTarget()
	{
		return m_ActualTarget;   //! Vanilla variable, but no vanilla getter
	}

	/**
	 * @brief get current target entity irrespective of mindstate
	 */
	EntityAI Expansion_GetTarget()
	{
		return m_Expansion_Target;
	}
}
