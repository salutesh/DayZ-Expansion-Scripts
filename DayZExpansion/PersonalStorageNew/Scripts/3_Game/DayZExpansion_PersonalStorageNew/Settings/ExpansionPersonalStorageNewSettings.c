/**
 * ExpansionPersonalStorageNewSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageLevel
{
	int ReputationRequirement;
	int QuestID;
	autoptr TStringArray ExcludedSlots = {};
	bool AllowAttachmentCargo;

	[NonSerialized()]
	int m_Level;

	void ExpansionPersonalStorageLevel(int lvl, int repReq, int questID, TStringArray excludedSlots = null, bool allowAttCargo = false)
	{
		m_Level = lvl;
		ReputationRequirement = repReq;
		QuestID = questID;
		if (excludedSlots)
			ExcludedSlots = excludedSlots;
		AllowAttachmentCargo = allowAttCargo;
	}
}

class ExpansionPersonalStorageNewSettings: ExpansionSettingBase
{
	static const int VERSION = 4;
	static const string SETTINGS_PATH = EXPANSION_SETTINGS_FOLDER + "PersonalStorageNewSettings.json";

	bool UseCategoryMenu;

	ref TStringArray ExcludedItems = {};

	//! Server only, all levels
	ref map<int, ref ExpansionPersonalStorageLevel> StorageLevels = new map<int, ref ExpansionPersonalStorageLevel>;

	//! Server + client, only levels w/ requirements
	[NonSerialized()]
	ref map<int, ref ExpansionPersonalStorageLevel> m_StorageLevelsRequirements = new map<int, ref ExpansionPersonalStorageLevel>;

	//! Server + client, only levels w/ requirements, sorted by level (asc)
	[NonSerialized()]
	ref array<ref ExpansionPersonalStorageLevel> m_StorageLevelsRequirements_Sorted = new array<ref ExpansionPersonalStorageLevel>;

	[NonSerialized()]
	private bool m_IsLoaded;

	override int Send( PlayerIdentity identity )
	{
		auto rpc = CreateRPC();
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);

		return 0;
	}
	
	override void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(UseCategoryMenu);

		ctx.Write(m_StorageLevelsRequirements_Sorted.Count());
		foreach (auto storageLevelConfig: m_StorageLevelsRequirements_Sorted)
		{
			ctx.Write(storageLevelConfig.m_Level);
			ctx.Write(storageLevelConfig.ReputationRequirement);
			ctx.Write(storageLevelConfig.QuestID);
		}
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(UseCategoryMenu))
		{
			Error("ExpansionPersonalStorageNewSettings::OnRecieve Couldn't read UseCategoryMenu");
			return false;
		}

		int count;
		if (!ctx.Read(count))
		{
			Error("ExpansionPersonalStorageNewSettings::OnRecieve Couldn't read storage levels reputation requirements count");
			return false;
		}

		m_StorageLevelsRequirements.Clear();
		m_StorageLevelsRequirements_Sorted.Clear();

		while (count--)
		{
			int lvl, repReq, questID;
			if (!ctx.Read(lvl) || !ctx.Read(repReq) || !ctx.Read(questID))
			{
				Error("ExpansionPersonalStorageNewSettings::OnRecieve Couldn't read storage level, reputation or questID requirements");
				return false;
			}
			auto storageLevelConfig = new ExpansionPersonalStorageLevel(lvl, repReq, questID);
			m_StorageLevelsRequirements[lvl] = storageLevelConfig;
			m_StorageLevelsRequirements_Sorted.Insert(storageLevelConfig);
			EXTrace.Print(EXTrace.PERSONALSTORAGE, this, "Received personal storage lvl " + lvl + " rep req " + repReq + " quest ID " + questID);
		}

		m_IsLoaded = true;

		ExpansionSettings.SI_PersonalStorageNew.Invoke();
		
		return true;
	}

	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;
	
		bool save;

		bool settingsExist = FileExist(SETTINGS_PATH);

		if (settingsExist)
		{
			EXPrint("[ExpansionPersonalStorageNewSettings] Load existing setting file:" + SETTINGS_PATH);

			JsonFileLoader<ExpansionPersonalStorageNewSettings>.JsonLoadFile(SETTINGS_PATH, this);

			if (m_Version < VERSION)
			{
				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionPersonalStorageNewSettings] No existing setting file:" + SETTINGS_PATH + ". Creating defaults!");
			Defaults();
			save = true;
		}

		m_StorageLevelsRequirements.Clear();
		m_StorageLevelsRequirements_Sorted.Clear();

		foreach (int storageLevel, auto storageLevelConfig: StorageLevels)
		{
			int repReq = storageLevelConfig.ReputationRequirement;
			int questID = storageLevelConfig.QuestID;

			if (repReq == -1 && !questID)
				continue;

			storageLevelConfig.m_Level = storageLevel;
			m_StorageLevelsRequirements[storageLevel] = storageLevelConfig;
			m_StorageLevelsRequirements_Sorted.Insert(storageLevelConfig);
			EXTrace.Print(EXTrace.PERSONALSTORAGE, this, "Personal storage lvl " + storageLevel + " rep req " + repReq + " quest ID " + questID);
		}

		if (save)
			Save();

		return settingsExist;
	}

	override bool OnSave()
	{
		Print("[ExpansionPersonalStorageNewSettings] Saving settings");

		JsonFileLoader<ExpansionPersonalStorageNewSettings>.JsonSaveFile(SETTINGS_PATH, this);

		return true;
	}

	override void Defaults()
	{
		m_Version = VERSION;

		UseCategoryMenu = false;
		ExcludedItems.Clear();
		StorageLevels.Clear();

		TStringArray defaultExcludedSlots = {"Vest", "Body", "Hips", "Legs", "Back"};

		for (int lvl = 1; lvl <= 10; lvl++)
		{
			switch (lvl)
			{
				case 6:
				case 7:
				case 8:
				case 9:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(lvl, -1, 0));
					break;
				case 10:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(lvl, -1, 0, null, true));
					break;
				default:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(lvl, -1, 0, defaultExcludedSlots, false));
					break;
			}
		}
	}

	override string SettingName()
	{
		return "Personal Storage New Settings";
	}
};
