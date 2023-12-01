[eAIRegisterFaction(eAIFactionInfected)]
class eAIFactionInfected : eAIFaction
{
	void eAIFactionInfected()
	{
		m_Loadout = "Humanloadout";
		AddModifier(eModifiers.MDF_BRAIN);
		AddModifier(eModifiers.MDF_FLIES);
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionInfected)) return true;
		return false;
	}
};
