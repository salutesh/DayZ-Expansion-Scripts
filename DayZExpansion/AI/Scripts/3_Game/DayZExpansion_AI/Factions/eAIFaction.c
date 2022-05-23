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
};
