[eAIRegisterFaction(eAIFactionBrawlers)]
class eAIFactionBrawlers: eAIFaction
{
	void eAIFactionBrawlers()
	{
		m_Loadout = "SurvivorLoadout";
		m_MeleeDamageMultiplier = 10.0;
		m_MeleeYeetForce = 5.0;
		m_MeleeYeetFactors = "0.5 0.5 0.5";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionBrawlers)) return true;
		if (other.IsInherited(eAIFactionCivilian)) return true;
		return false;
	}
};
