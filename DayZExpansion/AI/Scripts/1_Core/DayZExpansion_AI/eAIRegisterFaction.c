class eAIRegisterFaction
{
	static ref array<typename> s_FactionTypes = new array<typename>;

	void eAIRegisterFaction(typename factionType)
	{
		Print("Registering faction type " + factionType + " ID " + s_FactionTypes.Count());

		s_FactionTypes.Insert(factionType);
	}
}
