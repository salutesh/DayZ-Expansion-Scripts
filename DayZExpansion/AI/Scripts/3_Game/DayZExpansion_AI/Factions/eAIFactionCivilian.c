[eAIRegisterFaction(eAIFactionCivilian)]
class eAIFactionCivilian : eAIFaction
{
	void eAIFactionCivilian()
	{
		m_Loadout = "SurvivorLoadout";
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		return true;
	}
};