class eAIFactionEast : eAIFaction
{
	void eAIFactionEast()
	{
		m_Name = "East";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionEast)) return true;
		if (other.IsInherited(eAIFactionCivilian)) return true;
		return false;
	}
};