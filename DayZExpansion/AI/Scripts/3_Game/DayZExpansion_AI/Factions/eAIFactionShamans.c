[eAIRegisterFaction(eAIFactionShamans)]
class eAIFactionShamans : eAIFaction
{
	void eAIFactionShamans()
	{
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionShamans)) return true;
		if (other.IsPassive()) return true;
		return false;
	}

	override bool IsFriendly(EntityAI other, DayZPlayer factionMember)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};