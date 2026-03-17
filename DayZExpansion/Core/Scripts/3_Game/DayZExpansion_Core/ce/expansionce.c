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

/**
 * @brief Central Economy
 * \n usage:
 * @code
 *  ExpansionCE ce = ExpansionCE.GetInstance();
 * 
 *  ce.LoadTypes();
 *  ExpansionCESpawnableType hatchet = ce.GetType("Hatchet");
 *  Print(hatchet.Categories[0]);
 *
 *  >> "tools"
 * 
 *  ce.LoadSpawnableTypes();
 *  ExpansionCEType vss = ce.GetSpawnableType("VSS");
 *  Print(vss.AttachmentGroups[0].Items[0].ItemName);
 *
 *  >> "PSO11Optic"
 * 
 *  ce.LoadEvents();
 *  ExpansionCEEvent helicrash = ce.GetEvent("StaticHeliCrash");
 *  Print(helicrash.Children["wreck_mi8_crashed"].LootMin);
 *
 *  >> 10
 * 
 *  ce.LoadTerritories("wolf");
 *  ExpansionCETerritories wolf = ce.Territories["wolf"];
 *  Print(wolf.Zones[0].Name);
 *
 *  >> "Water"
 * @endcode
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
	ref ExpansionCEPlayerSpawnpoints PlayerSpawnPoints = new ExpansionCEPlayerSpawnpoints();

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

	/**
	 * @brief Initialize instance. WARNING: Should only be called on fresh instance or after calling Clear()
	 */
	void Init()
	{
		EconomyCore.LoadDB(null);
		EconomyCore.Load(null, "$mission:cfgeconomycore.xml");
	}

	/**
	 * @brief Load player spawnpoints
	 */
	void LoadPlayerSpawnpoints()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		if (!PlayerSpawnPoints.IsLoaded())
			PlayerSpawnPoints.Load("$mission:cfgplayerspawnpoints.xml");
	}

	/**
	 * @brief Load all territories
	 */
	void LoadTerritories()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		if (Territories.Count() > 0)
			return;

		TStringArray territories = ExpansionStatic.FindFilesInLocation("$mission:env\\", ".xml");
		foreach (string territory: territories) 
		{
			string territoryName = territory;
			territoryName.Replace("_territories.xml", "");
			LoadTerritories(territoryName);
		}
	}

	/**
	 * @brief Load specific territories
	 */
	void LoadTerritories(string territoryName)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		if (!Territories.Contains(territoryName))
			Territories[territoryName] = ExpansionCETerritories.LoadTerritories(string.Format("$mission:env\\%1_territories.xml", territoryName));
	}

	/**
	 * @brief Load all suppported CE filetypes (types, spawnabletypes and events)
	 */
	void LoadCE()
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		foreach (ExpansionCEFolder folder: EconomyCore.CE)
		{
			folder.Load(this);
		}
	}

	/**
	 * @brief Load specific CE filetype (types, spawnabletypes or events)
	 */
	void LoadCE(ExpansionCEFileType type)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this, typename.EnumToString(ExpansionCEFileType, type));
	#endif

		foreach (ExpansionCEFolder folder: EconomyCore.CE)
		{
			folder.Load(this, type);
		}
	}

	/**
	 * @brief Load types (equivalent to LoadCE(ExpansionCEFileType.TYPES)
	 */
	void LoadTypes()
	{
		LoadCE(ExpansionCEFileType.TYPES);
	}

	/**
	 * @brief Load spawnabletypes (equivalent to LoadCE(ExpansionCEFileType.SPAWNABLETYPES)
	 */
	void LoadSpawnableTypes()
	{
		LoadCE(ExpansionCEFileType.SPAWNABLETYPES);
	}

	/**
	 * @brief Load events (equivalent to LoadCE(ExpansionCEFileType.EVENTS)
	 */
	void LoadEvents()
	{
		LoadCE(ExpansionCEFileType.EVENTS);
	}

	void SaveJSON(string fileName = "$mission:ExpansionCE.json")
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this);
	#endif

		string errorMsg;
		if (!JsonFileLoader<ExpansionCE>.SaveFile(fileName, this, errorMsg))
			Error(errorMsg);
	}

	/**
	 * @brief Clear all loaded data
	 */
	void Clear()
	{
		EconomyCore.Clear();
		Types.Clear();
		SpawnableTypes.Clear();
		Events.Clear();
		Territories.Clear();
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
