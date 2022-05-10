class eAIFaction
{
	protected string m_Name;
	protected string m_Loadout = "HumanLoadout";

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
};