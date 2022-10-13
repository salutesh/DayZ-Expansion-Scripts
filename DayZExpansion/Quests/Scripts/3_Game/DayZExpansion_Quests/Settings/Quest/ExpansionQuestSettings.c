/**
 * ExpansionQuestSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestSettingsBase: ExpansionSettingBase
{
	bool EnableQuests;
	bool EnableQuestLogTab;
	bool CreateQuestNPCMarkers;
	string QuestAcceptedTitle;
	string QuestAcceptedText;
	string QuestCompletedTitle;
	string QuestCompletedText;
	string QuestFailedTitle;
	string QuestFailedText;
	string QuestCanceledTitle;
	string QuestCanceledText;
	string QuestTurnInTitle;
	string QuestTurnInText;
	string QuestObjectiveCompletedTitle;
	string QuestObjectiveCompletedText;
	string AchivementCompletedTitle;
	string AchivementCompletedText;
	string WeeklyQuestResetDay;
	int WeeklyQuestResetHour;
	int WeeklyQuestResteMinute;
	int DailyQuestResetHour;
	int DailyQuestResetMinute;
	bool UseUTCTime;
}

/**@class		ExpansionQuestSettings
 * @brief		Vehicle settings class
 **/
class ExpansionQuestSettings: ExpansionQuestSettingsBase
{
	static const int VERSION = 3;

	string QuestCooldownTitle;
	string QuestCooldownText;

	string QuestNotInGroupTitle;
	string QuestNotInGroupText;

	string QuestNotGroupOwnerTitle;
	string QuestNotGroupOwnerText;

	[NonSerialized()]
	private bool m_IsLoaded;

	void ExpansionQuestSettings()
	{

	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings OnRecieve
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ExpansionQuestSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionQuestSettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Quest.Invoke();

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings OnSend
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
#endif

		ExpansionQuestSettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings Send
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( !IsMissionHost() )
		{
			return 0;
		}

		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Quest, true, identity );

		return 0;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings Copy
	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionQuestSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionQuestSettingsBase s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		EnableQuests = s.EnableQuests;
		EnableQuestLogTab = s.EnableQuestLogTab;
		CreateQuestNPCMarkers = s.CreateQuestNPCMarkers;

		QuestAcceptedTitle = s.QuestAcceptedTitle;
		QuestAcceptedText = s.QuestAcceptedText;

		QuestCompletedTitle = s.QuestCompletedTitle;
		QuestCompletedText = s.QuestCompletedText;

		QuestFailedTitle = s.QuestFailedTitle;
		QuestFailedText = s.QuestFailedText;

		QuestCanceledTitle = s.QuestCanceledTitle;
		QuestCanceledText = s.QuestCanceledText;

		QuestTurnInTitle = s.QuestTurnInTitle;
		QuestTurnInText = s.QuestTurnInText;

		QuestObjectiveCompletedTitle = s.QuestObjectiveCompletedTitle;
		QuestObjectiveCompletedText = s.QuestObjectiveCompletedText;

		AchivementCompletedTitle = s.AchivementCompletedTitle;
		AchivementCompletedText = s.AchivementCompletedText;

		//! Version 1
		WeeklyQuestResetDay = s.WeeklyQuestResetDay;
		WeeklyQuestResetHour = s.WeeklyQuestResetHour;
		WeeklyQuestResteMinute = s.WeeklyQuestResteMinute;
		DailyQuestResetHour = s.DailyQuestResetHour;
		DailyQuestResetMinute = s.DailyQuestResetMinute;

		//! Version 2
		UseUTCTime = s.UseUTCTime;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionQuestSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		QuestCooldownTitle = s.QuestCooldownTitle;
		QuestCooldownText = s.QuestCooldownText;

		QuestNotInGroupTitle = s.QuestNotInGroupTitle;
		QuestNotInGroupText = s.QuestNotInGroupText;

		QuestNotGroupOwnerTitle = s.QuestNotGroupOwnerTitle;
		QuestNotGroupOwnerText = s.QuestNotGroupOwnerText;

		ExpansionQuestSettingsBase sb = s;
		CopyInternal( sb );
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings IsLoaded
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings Unload
	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings OnLoad
	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;
		bool questSettingsExist = FileExist(EXPANSION_QUEST_SETTINGS);
		if (questSettingsExist)
		{
			ExpansionQuestSettings settingsDefault = new ExpansionQuestSettings;
			settingsDefault.Defaults();

			ExpansionQuestSettingsBase settingsBase;
			JsonFileLoader<ExpansionQuestSettingsBase>.JsonLoadFile(EXPANSION_QUEST_SETTINGS, settingsBase);
			if (settingsBase.m_Version < VERSION)
			{
				if (settingsBase.m_Version < 1)
				{
					WeeklyQuestResetDay = settingsDefault.WeeklyQuestResetDay;
					WeeklyQuestResetHour = settingsDefault.WeeklyQuestResetHour;
					WeeklyQuestResteMinute = settingsDefault.WeeklyQuestResteMinute;
					DailyQuestResetHour = settingsDefault.DailyQuestResetHour;
					DailyQuestResetMinute = settingsDefault.DailyQuestResetMinute;
				}

				if (settingsBase.m_Version < 3)
				{
					QuestCooldownTitle = settingsDefault.QuestCooldownTitle;
					QuestCooldownText = settingsDefault.QuestCooldownText;

					QuestNotInGroupTitle = settingsDefault.QuestNotInGroupTitle;
					QuestNotInGroupText = settingsDefault.QuestNotInGroupText;

					QuestNotGroupOwnerTitle = settingsDefault.QuestNotGroupOwnerTitle;
					QuestNotGroupOwnerText = settingsDefault.QuestNotGroupOwnerText;
				}
				else
				{
					JsonFileLoader<ExpansionQuestSettings>.JsonLoadFile(EXPANSION_QUEST_SETTINGS, this);
				}

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionQuestSettings>.JsonLoadFile(EXPANSION_QUEST_SETTINGS, this);
			}
		}
		else
		{
			CF_Log.Info("[ExpansionQuestSettings] No existing setting file:" + EXPANSION_QUEST_SETTINGS + ". Creating defaults!");

			Defaults();
			save = true;
		}

		if (save)
		{
			Save();
		}

		return questSettingsExist;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings OnSave
	// ------------------------------------------------------------
	override bool OnSave()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
#endif

		JsonFileLoader<ExpansionQuestSettings>.JsonSaveFile(EXPANSION_QUEST_SETTINGS, this);

		return true;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings Update
	// ------------------------------------------------------------
override void Update( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
#endif

		super.Update( setting );

		ExpansionSettings.SI_Quest.Invoke();
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

		EnableQuests = true;
		EnableQuestLogTab = true;
		CreateQuestNPCMarkers = true;

		QuestAcceptedTitle = "Quest Accepted";
		QuestAcceptedText = "The quest %1 has been accepted!";

		QuestCompletedTitle = "Quest Completed";
		QuestCompletedText = "All objectives of the quest %1 have been completed";

		QuestFailedTitle = "Quest Failed";
		QuestFailedText = "The quest %1 failed!";

		QuestCanceledTitle = "Quest Canceled";
		QuestCanceledText = "The quest %1 has been canceled!";

		QuestTurnInTitle = "Quest Turn-In";
		QuestTurnInText = "The quest %1 has been completed!";

		QuestObjectiveCompletedTitle = "Objective Completed";
		QuestObjectiveCompletedText = "You have completed the objective %1 of the quest %2.";

		AchivementCompletedTitle = "Achievement \"%1\" completed!";
		AchivementCompletedText = "You have completed the achievement %1";

		QuestCooldownTitle = "Quest Cooldown";
		QuestCooldownText = "This quest is still on cooldown! Come back in %1";

		QuestNotInGroupTitle = "Group Quest";
		QuestNotInGroupText = "Group quests can only accepted while in a group!";

		QuestNotGroupOwnerTitle = "Group Quest";
		QuestNotGroupOwnerText = "Only a group owner can accept and turn-in group quest!";

		WeeklyQuestResetDay = "Wednesday";
		WeeklyQuestResetHour = 8;
		WeeklyQuestResteMinute = 0;
		DailyQuestResetHour = 8;
		DailyQuestResetMinute = 0;

		UseUTCTime = false;
	}

	// ------------------------------------------------------------
	// ExpansionQuestSettings SettingName
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Quest Settings";
	}
};