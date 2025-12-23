/**
 * ExpansionCE.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionCE 
{
	static const int NOT_SET = int.MIN;

	protected static ref ExpansionCE s_Instance;

	ref ExpansionEconomyCore EconomyCore = new ExpansionEconomyCore();
	ref ExpansionCETypes Types = new ExpansionCETypes();
	ref ExpansionCESpawnableTypes SpawnableTypes = new ExpansionCESpawnableTypes();
	ref ExpansionCEEvents Events = new ExpansionCEEvents();
	ref map<string, ref ExpansionCETerritories> Territories = new map<string, ref ExpansionCETerritories>();

	static ExpansionCE GetInstance()
	{
		if (!s_Instance)
			s_Instance = new ExpansionCE();

		return s_Instance;
	}

	private void ExpansionCE()
	{
		Init();
	}

	void Init()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		TStringArray territories = ExpansionStatic.FindFilesInLocation("$mission:env\\", ".xml");
		foreach (string territory: territories) 
		{
			string territoryName = territory;
			territoryName.Replace("_territories.xml", "");
			Territories[territoryName] = ExpansionCETerritories.LoadTerritories(string.Format("$mission:env\\%1", territory));
		}

		EconomyCore.LoadDB(this);
		EconomyCore.Load(this, "$mission:cfgeconomycore.xml");

	#ifdef DIAG_DEVELOPER
		string errorMsg;
		if (!JsonFileLoader<ExpansionCE>.SaveFile("$mission:ExpansionCE.json", this, errorMsg))
			Error(errorMsg);
	#endif
	}

	ExpansionCEType GetType(string name)
	{
		name.ToLower();
		return Types[name];
	}

	ExpansionCESpawnableType GetSpawnableType(string name)
	{
		name.ToLower();
		return SpawnableTypes[name];
	}

	ExpansionCEEvent GetEvent(string name)
	{
		name.ToLower();
		return Events[name];
	}
}
