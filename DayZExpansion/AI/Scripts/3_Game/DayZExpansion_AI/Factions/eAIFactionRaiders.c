class eAIFactionRaiders : eAIFaction
{
	void eAIFactionRaiders()
	{
		m_Name = "Raiders";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		return false;
	}
};