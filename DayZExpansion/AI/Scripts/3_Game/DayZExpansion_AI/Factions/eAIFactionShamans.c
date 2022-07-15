class eAIFactionShamans : eAIFaction
{
	void eAIFactionShamans()
	{
		m_Name = "Shamans";
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionShamans)) return true;
		if (other.IsInherited(eAIFactionPassive)) return true;
		return false;
	}

	override bool IsFriendly(EntityAI other)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};