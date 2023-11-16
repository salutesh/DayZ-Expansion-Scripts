[eAIRegisterFaction(eAIFactionLunatics)]
class eAIFactionLunatics: eAIFaction
{
	void eAIFactionLunatics()
	{
		m_Loadout = "Humanloadout";
		AddModifier(eModifiers.MDF_BRAIN);
	}
};
