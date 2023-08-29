[eAIRegisterFaction(eAIFactionPassive)]
class eAIFactionPassive : eAIFactionCivilian
{
	void eAIFactionPassive()
	{
		m_IsPassive = true;
	}

	override bool IsFriendly(EntityAI other)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};
