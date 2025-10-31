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
	protected static ref ExpansionCE s_Instance;

	ref map<string, ExpansionCEType> Types = new map<string, ExpansionCEType>();
	ref ExpansionEconomyCore EconomyCore = new ExpansionEconomyCore();
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

		EconomyCore.LoadDB();
		EconomyCore.Load("$mission:cfgeconomycore.xml");

		MergeCE();

	#ifdef DIAG_DEVELOPER
		string errorMsg;
		if (!JsonFileLoader<ExpansionCE>.SaveFile("$mission:ExpansionCE.json", this, errorMsg))
			Error(errorMsg);
	#endif
	}

	void MergeCE() 
	{
		ExpansionCEType existing;
		foreach (ExpansionEconomyCoreCE ce: EconomyCore.CE) 
		{
			foreach (ExpansionEconomyCoreCEFile file: ce.Files) 
			{
				switch (file.Type)
				{
					case ExpansionEconomyCoreCEFileType.TYPES:
						foreach (ExpansionCEType type: file.Types) 
						{
							string name = type.Name;
							name.ToLower();

							if (Types.Find(name, existing))
								existing.Merge(type);
							else
								Types[name] = type;
						}
						break;

					case ExpansionEconomyCoreCEFileType.SPAWNABLETYPES:
						//! TODO
						break;

					case ExpansionEconomyCoreCEFileType.EVENTS:
						//! TODO
						break;
				}
			}
		}
	}

	ExpansionCEType GetType(string name)
	{
		name.ToLower();
		return Types[name];
	}
}
