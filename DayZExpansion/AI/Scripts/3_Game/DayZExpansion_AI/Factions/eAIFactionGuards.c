[eAIRegisterFaction(eAIFactionGuards)]
class eAIFactionGuards : eAIFaction
{
	void eAIFactionGuards()
	{
		m_Loadout = "PoliceLoadout";
		m_IsGuard = true;
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsGuard()) return true;
		if (other.IsPassive()) return true;
		return false;
	}
};

[eAIRegisterFaction(eAIFactionInvincibleGuards)]
class eAIFactionInvincibleGuards : eAIFactionGuards
{
	void eAIFactionInvincibleGuards()
	{
		m_IsInvincible = true;
	}

	override string GetDisplayName()
	{
		return "#STR_EXPANSION_AI_FACTION_GUARDS";
	}
};
