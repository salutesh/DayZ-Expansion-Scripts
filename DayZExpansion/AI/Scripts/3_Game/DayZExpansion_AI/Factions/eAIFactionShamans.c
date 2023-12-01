[eAIRegisterFaction(eAIFactionShamans)]
class eAIFactionShamans : eAIFaction
{
	void eAIFactionShamans()
	{
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionShamans)) return true;
		if (other.IsPassive()) return true;
		return false;
	}

	override bool IsFriendlyEntity(EntityAI other, DayZPlayer factionMember = null)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};