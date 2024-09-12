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

	string QuestNotInGroupTitle;
	string QuestNotInGroupText;

	string QuestNotGroupOwnerTitle;
	string QuestNotGroupOwnerText;
	int GroupQuestMode = 0; //! 0 - Only group owners can accept and turn-in quests. | 1 - Only group owner can turn-in quest but all group members can accept them. | 2 - All group members can accept and turn-in quests.
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
	static const int VERSION = 10;

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
	bool UseQuestNPCIndicators;
	int MaxActiveQuests;

	override bool OnRecieve( ParamsReadContext ctx )
	{
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
		ExpansionQuestSettings thisSetting = this;
		ctx.Write(thisSetting);
	}

	override int Send( PlayerIdentity identity )
	{
		if (!IsMissionHost())
		{
			return 0;
		}

		auto rpc = CreateRPC();
		OnSend(rpc);
		rpc.Expansion_Send(true, identity);

		return 0;
	}

	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionQuestSettings s;
		if (!Class.CastTo(s, setting))
			return false;

		CopyInternal( s );
		return true;
	}

	protected void CopyInternal( ExpansionQuestSettingsBase s )
	{
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

		QuestNotInGroupTitle = s.QuestNotInGroupTitle;
		QuestNotInGroupText = s.QuestNotInGroupText;

		QuestNotGroupOwnerTitle = s.QuestNotGroupOwnerTitle;
		QuestNotGroupOwnerText = s.QuestNotGroupOwnerText;

		GroupQuestMode = s.GroupQuestMode;
	}

	protected void CopyInternal( ExpansionQuestSettings s )
	{
		AchievementCompletedTitle = s.AchievementCompletedTitle;
		AchievementCompletedText = s.AchievementCompletedText;

		WeeklyResetDay = s.WeeklyResetDay;
		WeeklyResetMinute = s.WeeklyResetMinute;
		WeeklyResetHour = s.WeeklyResetHour;
		DailyResetHour = s.DailyResetHour;
		DailyResetMinute = s.DailyResetMinute;
		UseUTCTime = s.UseUTCTime;
		UseQuestNPCIndicators = s.UseQuestNPCIndicators;
		MaxActiveQuests = s.MaxActiveQuests;

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.SETTINGS, this);
#endif
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

				if (settingsBase.m_Version < 4)
				{
					GroupQuestMode = settingsDefault.GroupQuestMode;
				}

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
				
				if (settingsBase.m_Version < 9)
				{
					UseQuestNPCIndicators = settingsDefault.UseQuestNPCIndicators;
				}
				
				if (settingsBase.m_Version < 10)
				{
					MaxActiveQuests = settingsDefault.MaxActiveQuests;
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
		ExpansionJsonFileParser<ExpansionQuestSettings>.Save(EXPANSION_QUEST_SETTINGS, this);

		return true;
	}

	override void Update( ExpansionSettingBase setting )
	{
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
		AchievementCompletedText = "%1";

		QuestCooldownTitle = "Quest Cooldown";
		QuestCooldownText = "This quest is still on cooldown! Come back in %1";

		QuestNotInGroupTitle = "Group Quest";
		QuestNotInGroupText = "Group quests can only be accepted while in a group!";

		QuestNotGroupOwnerTitle = "Group Quest";
		QuestNotGroupOwnerText = "Only a group owner can accept and turn-in a group quest!";

		WeeklyResetDay = "Wednesday";
		WeeklyResetHour = 8;
		WeeklyResetMinute = 0;
		DailyResetHour = 8;
		DailyResetMinute = 0;

		UseUTCTime = false;
		
		UseQuestNPCIndicators = true;
		MaxActiveQuests = -1;

		GroupQuestMode = 0;
	}

	override string SettingName()
	{
		return "Quest Settings";
	}
};