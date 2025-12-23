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

class ExpansionCEFileType
{
	static string TYPES = "types";
	static string SPAWNABLETYPES = "spawnabletypes";
	static string EVENTS = "events";
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

	void ExpansionCEFolder(string folder)
	{
		Folder = folder;
	}

	void AddFile(string name, string type)
	{
		Files.Insert(new ExpansionCEFile(name, type));
	}

	void Load(ExpansionCE ce)
	{
		foreach (ExpansionCEFile file: Files)
			file.Load(ce, Folder);
	}
}

class ExpansionCEFile
{
	string Name;
	string Type;

	void ExpansionCEFile(string name, string type)
	{
		Name = name;
		Type = type;
	}

	void Load(ExpansionCE ce, string folder)
	{
		string path = string.Format("$mission:%1\\%2", folder, Name);

		switch (Type)
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

					ceFolder.AddFile(name, type);
				}

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
		ceFolder.Load(ce);

		CE.Insert(ceFolder);
	}
}
