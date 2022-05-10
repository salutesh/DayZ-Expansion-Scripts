class eAIFactionRaiders : eAIFaction
{
	void eAIFactionRaiders()
	{
		m_Name = "Raiders";
		m_Loadout = "BanditLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		return false;
	}
};