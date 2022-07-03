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

	static typename GetType(string factionName)
	{
		return ("eAIFaction" + factionName).ToType();
	}

	static eAIFaction Create(string factionName)
	{
		typename faction = GetType(factionName);
		if (faction)
			//! @note w/o the cast to eAIFaction, the compiler warns about unsafe downcasting.
			//! Of course the compiler is wrong, because we're casting up, not down, so this cast here is just there to satisfy compiler shortcomings.
			//! Yes I wrote this comment for the sole reason that I'm annoyed by this.
			return eAIFaction.Cast(faction.Spawn());
		else
			Error("Invalid faction name " + factionName);

		return new eAIFactionRaiders();
	}
};
