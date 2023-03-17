[eAIRegisterFaction(eAIFactionCivilian)]
class eAIFactionCivilian : eAIFaction
{
	void eAIFactionCivilian()
	{
		m_Loadout = "SurvivorLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		return true;
	}
};