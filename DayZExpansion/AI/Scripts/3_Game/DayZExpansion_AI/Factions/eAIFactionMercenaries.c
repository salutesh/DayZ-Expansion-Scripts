[eAIRegisterFaction(eAIFactionMercenaries)]
class eAIFactionMercenaries : eAIFaction
{
	void eAIFactionMercenaries()
	{
		m_Name = "Mercenaries";
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionMercenaries)) return true;
		if (other.IsInherited(eAIFactionPassive)) return true;
		return false;
	}
};