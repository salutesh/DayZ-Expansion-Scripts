[eAIRegisterFaction(eAIFactionObservers)]
class eAIFactionObservers : eAIFaction
{
	void eAIFactionObservers()
	{
		m_Loadout = "SurvivorLoadout";
		m_IsObserver = true;
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		return true;
	}

	override bool IsFriendlyEntity(EntityAI other, DayZPlayer factionMember = null)
	{
		return other.IsInherited(DayZCreatureAI);
	}
};

[eAIRegisterFaction(eAIFactionInvincibleObservers)]
class eAIFactionInvincibleObservers : eAIFactionObservers
{
	void eAIFactionInvincibleObservers()
	{
		m_IsInvincible = true;
	}

	override string GetDisplayName()
	{
		return "#STR_EXPANSION_AI_FACTION_OBSERVERS";
	}
};
