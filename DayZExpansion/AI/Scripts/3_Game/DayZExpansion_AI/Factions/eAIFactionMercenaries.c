[eAIRegisterFaction(eAIFactionMercenaries)]
class eAIFactionMercenaries : eAIFaction
{
	void eAIFactionMercenaries()
	{
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionMercenaries)) return true;
		if (other.IsPassive()) return true;
		return false;
	}
};