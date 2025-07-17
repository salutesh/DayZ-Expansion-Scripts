modded class ZombieBase
{
	protected ref ExpansionInfectedType m_Expansion_InfectedType;
	protected ref ExpansionZombieFightLogic m_Expansion_ZombieFightLogic;

	void ZombieBase()
	{
		if (GetGame().IsServer() && GetExpansionSettings().GetAI().EnableZombieVehicleAttackHandler)
		{
			m_Expansion_InfectedType = ExpansionInfectedType.Get(Type());
			m_Expansion_ZombieFightLogic = new ExpansionZombieFightLogic(this);
		}
	}

	override bool FightLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.FightLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		return super.FightLogic(pCurrentCommandID, pInputController, pDt);
	}

	override bool ChaseAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.ChaseAttackLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		return super.ChaseAttackLogic(pCurrentCommandID, pInputController, pDt);
	}
	
	override bool FightAttackLogic(int pCurrentCommandID, DayZInfectedInputController pInputController, float pDt)
	{
		if (m_Expansion_ZombieFightLogic && m_Expansion_ZombieFightLogic.FightAttackLogic(pCurrentCommandID, pInputController, pDt))
			return true;

		return super.FightAttackLogic(pCurrentCommandID, pInputController, pDt);
	}

	ExpansionInfectedType GetExpansionInfectedType()
	{
		return m_Expansion_InfectedType;
	}
}
