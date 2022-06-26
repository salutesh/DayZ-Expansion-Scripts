class eAIFaction
{
	protected string m_Name;
	protected string m_Loadout = "HumanLoadout";
	protected bool m_IsGuard;

	string GetName()
	{
		return m_Name;
	}

	string GetDefaultLoadout()
	{
		return m_Loadout;
	}

	bool IsFriendly(notnull eAIFaction other)
	{
		return true;
	}

	bool IsGuard()
	{
		return m_IsGuard;
	}

	static eAIFaction Create(string factionName)
	{
		typename faction = ("eAIFaction" + factionName).ToType();
		if (faction)
			return faction.Spawn();
		else
			Error("Invalid faction name " + factionName);

		return new eAIFactionRaiders();
	}
};
