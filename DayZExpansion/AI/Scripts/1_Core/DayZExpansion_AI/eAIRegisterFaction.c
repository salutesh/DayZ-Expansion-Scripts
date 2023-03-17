class eAIRegisterFaction
{
	static ref map<int, typename> s_FactionTypes = new map<int, typename>;
	static ref map<typename, int> s_FactionIDs = new map<typename, int>;

	void eAIRegisterFaction(typename factionType)
	{
		string cls = factionType.ToString();

		int id = ExpansionString.BetterHash(cls.Substring(10, cls.Length() - 10));

		if (s_FactionTypes[id])
		{
			Error("Hash collision " + factionType + " " + s_FactionTypes[id] + " " + id);
			return;
		}

		Print("Registering faction type " + factionType + " ID " + id);

		s_FactionTypes.Insert(id, factionType);
		s_FactionIDs.Insert(factionType, id);
	}
}
