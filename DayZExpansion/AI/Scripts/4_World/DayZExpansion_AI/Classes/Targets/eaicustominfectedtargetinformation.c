class eAICustomInfectedTargetInformation: eAICustomCreatureTargetInformation
{
	private DayZInfected m_Infected;

	void eAICustomInfectedTargetInformation(EntityAI target)
	{
		Class.CastTo(m_Infected, target);
	}

	override bool IsFighting()
	{
		if (m_Infected.GetCommand_Attack())
			return true;

		return false;
	}
};
