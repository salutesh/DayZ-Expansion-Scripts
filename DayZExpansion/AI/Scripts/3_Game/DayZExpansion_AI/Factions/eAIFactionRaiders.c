[eAIRegisterFaction(eAIFactionRaiders)]
class eAIFactionRaiders : eAIFaction
{
	void eAIFactionRaiders()
	{
		m_Loadout = "BanditLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsPassive()) return true;
		return false;
	}
};