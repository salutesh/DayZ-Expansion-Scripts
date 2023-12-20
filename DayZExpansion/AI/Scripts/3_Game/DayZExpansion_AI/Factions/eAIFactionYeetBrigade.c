[eAIRegisterFaction(eAIFactionYeetBrigade)]
class eAIFactionYeetBrigade: eAIFaction
{
	void eAIFactionYeetBrigade()
	{
		m_Loadout = "YeetBrigadeLoadout";
		m_MeleeDamageMultiplier = 100.0;
		m_MeleeYeetForce = 15.0;
		m_MeleeYeetFactors = "0.5 1 0.5";
		m_DisableWeaponPickup = true;
		m_IsGuard = true;
		m_HasUnlimitedStamina = true;
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (other.IsInherited(eAIFactionYeetBrigade)) return true;
		return false;
	}
};

[eAIRegisterFaction(eAIFactionInvincibleYeetBrigade)]
class eAIFactionInvincibleYeetBrigade : eAIFactionYeetBrigade
{
	void eAIFactionInvincibleYeetBrigade()
	{
		m_IsInvincible = true;
	}

	override string GetDisplayName()
	{
		return "#STR_EXPANSION_AI_FACTION_YEETBRIGADE";
	}
};
