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
	autoptr TStringArray ExcludedSlots = {};
	bool AllowAttachmentCargo;

	void ExpansionPersonalStorageLevel(int repReq, TStringArray excludedSlots = null, bool allowAttCargo = false)
	{
		ReputationRequirement = repReq;
		if (excludedSlots)
			ExcludedSlots = excludedSlots;
		AllowAttachmentCargo = allowAttCargo;
	}
}

class ExpansionPersonalStorageNewSettings: ExpansionSettingBase
{
	static const int VERSION = 2;
	static const string SETTINGS_PATH = EXPANSION_SETTINGS_FOLDER + "PersonalStorageNewSettings.json";

	bool UseCategoryMenu;

	ref map<int, ref ExpansionPersonalStorageLevel> StorageLevels = new map<int, ref ExpansionPersonalStorageLevel>;

	[NonSerialized()]
	private bool m_IsLoaded;

	override int Send( PlayerIdentity identity )
	{
		auto rpc = ExpansionScriptRPC.Create();
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.PersonalStorageNew, true, identity );

		return 0;
	}
	
	override void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(UseCategoryMenu);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(UseCategoryMenu))
		{
			Error("ExpansionPersonalStorageNewSettings::OnRecieve UseCategoryMenu");
			return false;
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

		TStringArray defaultExcludedSlots = {"Vest", "Body", "Hips", "Legs", "Back"};

		for (int lvl = 1; lvl <= 10; lvl++)
		{
			switch (lvl)
			{
				case 6:
				case 7:
				case 8:
				case 9:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(-1));
					break;
				case 10:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(-1, null, true));
					break;
				default:
					StorageLevels.Insert(lvl, new ExpansionPersonalStorageLevel(-1, defaultExcludedSlots, false));
					break;
			}
		}
	}

	override string SettingName()
	{
		return "Personal Storage New Settings";
	}
};
