[eAIRegisterFaction(eAIFactionWest)]
class eAIFactionWest : eAIFaction
{
	void eAIFactionWest()
	{
		m_Loadout = "WestLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionWest)) return true;
		if (other.IsInherited(eAIFactionCivilian)) return true;
		return false;
	}
};
