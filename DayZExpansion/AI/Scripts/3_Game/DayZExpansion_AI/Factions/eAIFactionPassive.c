[eAIRegisterFaction(eAIFactionPassive)]
class eAIFactionPassive : eAIFactionCivilian
{
	void eAIFactionPassive()
	{
		m_IsPassive = true;
	}

	override bool IsFriendly(EntityAI other, DayZPlayer factionMember)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};
