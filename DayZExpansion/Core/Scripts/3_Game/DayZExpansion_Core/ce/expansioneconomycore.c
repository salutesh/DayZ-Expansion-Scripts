/**
 * ExpansionEconomyCore.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

enum ExpansionCEFileType
{
	TYPES,
	SPAWNABLETYPES,
	EVENTS
}

class ExpansionEconomyCoreRootClass
{
	string Name;
	string Act;
	string ReportMemoryLOD;

	void ExpansionEconomyCoreRootClass(string name, string act, string reportMemoryLOD)
	{
		Name = name;
		Act = act;
		ReportMemoryLOD = reportMemoryLOD;
	}
}

class ExpansionEconomyCoreDefault
{
	string Name;
	string Value;

	void ExpansionEconomyCoreDefault(string name, string value)
	{
		Name = name;
		Value = value;
	}
}

class ExpansionCEFolder
{
	string Folder;
	ref array<ref ExpansionCEFile> Files = {};

	[NonSerialized()]
	protected ref map<ExpansionCEFileType, ref array<ExpansionCEFile>> m_FilesByType = new map<ExpansionCEFileType, ref array<ExpansionCEFile>>;

	void ExpansionCEFolder(string folder)
	{
		Folder = folder;
	}

	void AddFile(string name, ExpansionCEFileType type)
	{
		auto file = new ExpansionCEFile(name, type);

		Files.Insert(file);

		array<ExpansionCEFile> files;

		if (!m_FilesByType.Find(type, files))
		{
			files = {};
			m_FilesByType[type] = files;
		}

		files.Insert(file);
	}

	void Load(ExpansionCE ce)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this, Folder);
	#endif

		foreach (ExpansionCEFile file: Files)
			file.Load(ce, Folder);
	}

	void Load(ExpansionCE ce, ExpansionCEFileType type)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this, Folder, typename.EnumToString(ExpansionCEFileType, type));
	#endif

		auto files = m_FilesByType[type];

		if (files)
			foreach (ExpansionCEFile file: files)
				file.Load(ce, Folder);
	}
}

class ExpansionCEFile
{
	string Name;
	string Type;

	[NonSerialized()]
	protected ExpansionCEFileType m_Type;

	[NonSerialized()]
	protected bool m_IsLoaded;

	void ExpansionCEFile(string name, ExpansionCEFileType type)
	{
		Name = name;
		Type = typename.EnumToString(ExpansionCEFileType, type);
		Type.ToLower();
		m_Type = type;
	}

	void Load(ExpansionCE ce, string folder)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.CE, this, Name);
	#endif

		if (m_IsLoaded)
			return;

		string path = string.Format("$mission:%1\\%2", folder, Name);

		switch (m_Type)
		{
			case ExpansionCEFileType.TYPES:
				ce.Types.ReadTypes(path);
				break;

			case ExpansionCEFileType.SPAWNABLETYPES:
				ce.SpawnableTypes.ReadTypes(path);
				break;

			case ExpansionCEFileType.EVENTS:
				ce.Events.ReadEvents(path);
				break;
		}

		m_IsLoaded = true;
	}
}

/**@class       ExpansionEconomyCore
 * @brief       structure for EconomyCore.xml
 **/
class ExpansionEconomyCore
{
	ref array<ref ExpansionEconomyCoreRootClass> Classes = {};
	ref array<ref ExpansionEconomyCoreDefault> Defaults = {};
	ref array<ref ExpansionCEFolder> CE = {};

	void Load(ExpansionCE ce, string fileName)
	{
		CF_XML_Document document = CF_XML.ReadDocumentEx(fileName);
		if (document)
		{
			auto root = document.Get("economycore")[0];
			if (!root)
			{
				CF.FormatError("No root tag 'economycore' in '%1'", fileName);
				return;
			}

			auto classes = root.GetTag("classes")[0];
			if (classes)
			{
				auto rootClasses = classes.GetTag("rootclass");
				foreach (CF_XML_Tag rootClass: rootClasses)
				{
					string rootName = ExpansionXML.GetAttributeString(rootClass, "name");
					string act = ExpansionXML.GetAttributeString(rootClass, "act");
					string reportMemoryLOD = ExpansionXML.GetAttributeString(rootClass, "reportMemoryLOD");

					Classes.Insert(new ExpansionEconomyCoreRootClass(rootName, act, reportMemoryLOD));
				}
			}

			auto defaults = root.GetTag("defaults")[0];
			if (defaults)
			{
				auto dflts = defaults.GetTag("default");
				foreach (CF_XML_Tag dflt: dflts)
				{
					string defaultName = ExpansionXML.GetAttributeString(dflt, "name");
					string defaultValue = ExpansionXML.GetAttributeString(dflt, "value");

					Defaults.Insert(new ExpansionEconomyCoreDefault(defaultName, defaultValue));
				}
			}

			auto folders = root.GetTag("ce");
			foreach (CF_XML_Tag folder: folders)
			{
				auto ceFolder = new ExpansionCEFolder(ExpansionXML.GetAttributeString(folder, "folder"));

				auto files = folder.GetTag("file");
				foreach (CF_XML_Tag file: files)
				{
					string name = ExpansionXML.GetAttributeString(file, "name");
					string type = ExpansionXML.GetAttributeString(file, "type");
					type.ToUpper();

					ceFolder.AddFile(name, typename.StringToEnum(ExpansionCEFileType, type));
				}

				if (ce)
					ceFolder.Load(ce);

				CE.Insert(ceFolder);
			}
		}
	}

	void LoadDB(ExpansionCE ce)
	{
		auto ceFolder = new ExpansionCEFolder("");
		ceFolder.AddFile("db/types.xml", ExpansionCEFileType.TYPES);
		ceFolder.AddFile("db/events.xml", ExpansionCEFileType.EVENTS);
		ceFolder.AddFile("cfgspawnabletypes.xml", ExpansionCEFileType.SPAWNABLETYPES);

		if (ce)
			ceFolder.Load(ce);

		CE.Insert(ceFolder);
	}

	void Clear()
	{
		Classes.Clear();
		Defaults.Clear();
		CE.Clear();
	}
}
