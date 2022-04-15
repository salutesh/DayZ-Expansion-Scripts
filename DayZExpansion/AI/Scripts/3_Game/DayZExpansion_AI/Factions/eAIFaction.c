class eAIFaction
{
	protected string m_Name;

	string GetName()
	{
		return m_Name;
	}

	bool IsFriendly(notnull eAIFaction other)
	{
		return true;
	}
};