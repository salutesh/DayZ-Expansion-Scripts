[eAIRegisterFaction(eAIFactionPassive)]
class eAIFactionPassive : eAIFactionCivilian
{
	void eAIFactionPassive()
	{
		m_IsPassive = true;
	}

	override bool IsFriendlyEntity(EntityAI other, DayZPlayer factionMember = null)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};
