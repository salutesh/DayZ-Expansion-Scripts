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

	string QuestCooldownTitle;
	string QuestCooldownText;

#ifdef EXPANSIONMODGROUPS
	string QuestNotInGroupTitle;
	string QuestNotInGroupText;

	string QuestNotGroupOwnerTitle;
	string QuestNotGroupOwnerText;
	int GroupQuestMode = 0; //! 0 - Only group owners can accept and turn-in quests. | 1 - Only group owner can turn-in quest but all group members can accept them. | 2 - All group members can accept and turn-in quests.
#endif
};

class ExpansionQuestSettingsV5Base: ExpansionQuestSettingsBase
{
	string AchivementCompletedTitle;
	string AchivementCompletedText;
};

class ExpansionQuestSettingsV5: ExpansionQuestSettingsV5Base
{
	string WeeklyQuestResetDay;
	int WeeklyQuestResetHour;
	int WeeklyQuestResteMinute;
	int DailyQuestResetHour;
	int DailyQuestResetMinute;
	bool UseUTCTime;
};

/**@class		ExpansionQuestSettings
 * @brief		Vehicle settings class
 **/
class ExpansionQuestSettings: ExpansionQuestSettingsBase
{
	static const int VERSION = 8;

	[NonSerialized()]
	protected bool m_IsLoaded;

	string AchievementCompletedTitle;
	string AchievementCompletedText;

	string WeeklyResetDay;
	int WeeklyResetMinute;
	int WeeklyResetHour;
	int DailyResetHour;
	int DailyResetMinute;
	bool UseUTCTime;

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

	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		ExpansionQuestSettings thisSetting = this;
		ctx.Write(thisSetting);
	}

	override int Send( PlayerIdentity identity )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
	#endif

		if (!IsMissionHost())
		{
			return 0;
		}

		auto rpc = ExpansionScriptRPC.Create();
		OnSend(rpc);
		rpc.Send(null, ExpansionSettingsRPC.Quest, true, identity);

		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
	#endif

		ExpansionQuestSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal( s );
		return true;
	}

	protected void CopyInternal( ExpansionQuestSettingsBase s )
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

		//! Version 3
		QuestCooldownTitle = s.QuestCooldownTitle;
		QuestCooldownText = s.QuestCooldownText;

	#ifdef EXPANSIONMODGROUPS
		QuestNotInGroupTitle = s.QuestNotInGroupTitle;
		QuestNotInGroupText = s.QuestNotInGroupText;

		QuestNotGroupOwnerTitle = s.QuestNotGroupOwnerTitle;
		QuestNotGroupOwnerText = s.QuestNotGroupOwnerText;

		GroupQuestMode = s.GroupQuestMode;
	#endif
	}

	protected void CopyInternal( ExpansionQuestSettings s )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		AchievementCompletedTitle = s.AchievementCompletedTitle;
		AchievementCompletedText = s.AchievementCompletedText;

		WeeklyResetDay = s.WeeklyResetDay;
		WeeklyResetMinute = s.WeeklyResetMinute;
		WeeklyResetHour = s.WeeklyResetHour;
		DailyResetHour = s.DailyResetHour;
		DailyResetMinute = s.DailyResetMinute;
		UseUTCTime = s.UseUTCTime;

		ExpansionQuestSettingsBase sb = s;
		CopyInternal( sb );
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
		auto trace = EXTrace.Start(ExpansionTracing.SETTINGS, this);

		m_IsLoaded = true;

		bool save;
		bool questSettingsExist = FileExist(EXPANSION_QUEST_SETTINGS);
		if (questSettingsExist)
		{
			ExpansionQuestSettings settingsDefault = new ExpansionQuestSettings;
			settingsDefault.Defaults();

			ExpansionQuestSettingsBase settingsBase;
			if (!ExpansionJsonFileParser<ExpansionQuestSettingsBase>.Load(EXPANSION_QUEST_SETTINGS, settingsBase))
				return false;

			if (settingsBase.m_Version < VERSION)
			{
				EXTrace.Print(EXTrace.SETTINGS, this, "Convert quest settings version " + settingsBase.m_Version + " to version " + VERSION);

				if (!ExpansionJsonFileParser<ExpansionQuestSettings>.Load(EXPANSION_QUEST_SETTINGS, this))
					return false;

			#ifdef EXPANSIONMODGROUPS
				if (settingsBase.m_Version < 4)
				{
					GroupQuestMode = settingsDefault.GroupQuestMode;
				}
			#endif

				if (settingsBase.m_Version < 6)
				{
					ExpansionQuestSettingsV5 settingsV5;
					if (!ExpansionJsonFileParser<ExpansionQuestSettingsV5>.Load(EXPANSION_QUEST_SETTINGS, settingsV5))
						return false;

					DailyResetHour = settingsV5.DailyQuestResetHour;
					DailyResetMinute = settingsV5.DailyQuestResetMinute;
					WeeklyResetDay = settingsV5.WeeklyQuestResetDay;
					WeeklyResetMinute = settingsV5.WeeklyQuestResteMinute;
					WeeklyResetHour = settingsV5.WeeklyQuestResetHour;
				}

				if (settingsBase.m_Version < 7)
				{
					ExpansionQuestSettingsV5Base settingsV5Base;
					if (!ExpansionJsonFileParser<ExpansionQuestSettingsV5Base>.Load(EXPANSION_QUEST_SETTINGS, settingsV5Base))
						return false;

					AchievementCompletedTitle = settingsV5Base.AchivementCompletedTitle;
					AchievementCompletedText = settingsV5Base.AchivementCompletedText;
				}

				m_Version = VERSION;
				save = true;
			}
			else
			{
				if (!ExpansionJsonFileParser<ExpansionQuestSettings>.Load(EXPANSION_QUEST_SETTINGS, this))
					return false;
			}
		}
		else
		{
			EXTrace.Print(EXTrace.SETTINGS, this, "No existing setting file:" + EXPANSION_QUEST_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}

		if (save)
		{
			Save();
		}

		return questSettingsExist;
	}

	override bool OnSave()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
	#endif

		ExpansionJsonFileParser<ExpansionQuestSettings>.Save(EXPANSION_QUEST_SETTINGS, this);

		return true;
	}

	override void Update( ExpansionSettingBase setting )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
	#endif

		super.Update( setting );

		ExpansionSettings.SI_Quest.Invoke();
	}

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

		AchievementCompletedTitle = "Achievement \"%1\" completed!";
		AchievementCompletedText = "You have completed the achievement %1";

		QuestCooldownTitle = "Quest Cooldown";
		QuestCooldownText = "This quest is still on cooldown! Come back in %1";

	#ifdef EXPANSIONMODGROUPS
		QuestNotInGroupTitle = "Group Quest";
		QuestNotInGroupText = "Group quests can only accepted while in a group!";

		QuestNotGroupOwnerTitle = "Group Quest";
		QuestNotGroupOwnerText = "Only a group owner can accept and turn-in group quest!";
	#endif

		WeeklyResetDay = "Wednesday";
		WeeklyResetHour = 8;
		WeeklyResetMinute = 0;
		DailyResetHour = 8;
		DailyResetMinute = 0;

		UseUTCTime = false;

	#ifdef EXPANSIONMODGROUPS
		GroupQuestMode = 0;
	#endif
	}

	override string SettingName()
	{
		return "Quest Settings";
	}
};