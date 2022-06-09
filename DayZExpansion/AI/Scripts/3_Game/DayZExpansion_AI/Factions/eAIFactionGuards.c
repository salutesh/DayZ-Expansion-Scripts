class eAIFactionGuards : eAIFaction
{
	void eAIFactionGuards()
	{
		m_Name = "Guards";
		m_Loadout = "PoliceLoadout";
		m_IsGuard = true;
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionGuards)) return true;
		if (other.IsInherited(eAIFactionPassive)) return true;
		return false;
	}
};
