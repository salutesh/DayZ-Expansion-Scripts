class eAIFactionCivilian : eAIFaction
{
	void eAIFactionCivilian()
	{
		m_Name = "Civilian";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		return true;
	}
};