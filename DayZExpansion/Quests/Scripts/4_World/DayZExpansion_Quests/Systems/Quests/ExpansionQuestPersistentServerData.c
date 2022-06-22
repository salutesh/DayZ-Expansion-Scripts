/**
 * ExpansionQuestPersistentServerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestPersistentServerDataBase
{
	int ConfigVersion;
	int LastWeeklyReset;
	int LastDailyReset;
};

class ExpansionQuestPersistentServerData: ExpansionQuestPersistentServerDataBase
{
	[NonSerialized()]
	static int CONFIGVERSION = 0;
	[NonSerialized()]
	static int DAY_TIME = 86400; //! Day in seconds
	[NonSerialized()]
	static int WEEK_TIME = 604800; //! Week in seconds

	void SetWeeklyResetTime()
	{
		int year, month, day, hour, minute, second;
		int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyQuestResetHour;
		int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyQuestResteMinute;
		int current = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		CF_Date.TimestampToDate(current, year, month, day, hour, minute, second);

		hour = weeklyResetHour;
		minute = weeklyResetMinute;
		second = 0;

		LastWeeklyReset = CF_Date.CreateDateTime(year, month, day, hour, minute, second).GetTimestamp();
	}

	void SetDailyResetTime()
	{
		int year, month, day, hour, minute, second;
		int dailyResetHour = GetExpansionSettings().GetQuest().DailyQuestResetHour;
		int dailyResetMinute = GetExpansionSettings().GetQuest().DailyQuestResetMinute;
		int current = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		CF_Date.TimestampToDate(current, year, month, day, hour, minute, second);

		hour = dailyResetHour;
		minute = dailyResetMinute;
		second = 0;

		LastDailyReset = CF_Date.CreateDateTime(year, month, day, hour, minute, second).GetTimestamp();
	}

	int GetLastWeeklyResetTimeDiff()
	{
		int currentTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		return (currentTime - LastWeeklyReset);
	}

	int GetLastDailyResetTimeDiff()
	{
		int currentTime = CF_Date.Now(GetExpansionSettings().GetQuest().UseUTCTime).GetTimestamp();
		return (currentTime - LastDailyReset);
	}

	bool HasWeeklyResetCooldown()
	{
		int timedif = GetLastWeeklyResetTimeDiff();
		if (timedif < WEEK_TIME)
			return true;

		return false;
	}

	bool HasDailyResetCooldown()
	{
		int timedif = GetLastDailyResetTimeDiff();
		if (timedif < DAY_TIME)
			return true;

		return false;
	}

	static ExpansionQuestPersistentServerData Load()
	{
		bool save;
		CF_Log.Info("[ExpansionQuestPersistentServerData] Load existing configuration file:" + EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE);

		ExpansionQuestPersistentServerData data;
		ExpansionQuestPersistentServerDataBase dataBase;
		JsonFileLoader<ExpansionQuestPersistentServerDataBase>.JsonLoadFile(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, dataBase);

		if (dataBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestPersistentServerData] Convert existing configuration file:" + EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE + " to version " + CONFIGVERSION);
			data = new ExpansionQuestPersistentServerData();
			//! Copy over old configuration that haven't changed
			data.CopyConfig(dataBase);
			data.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestPersistentServerData>.JsonLoadFile(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, data);
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestPersistentServerData>.JsonSaveFile(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, data);
		}

		return data;
	}

	void Save()
	{
		JsonFileLoader<ExpansionQuestPersistentServerData>.JsonSaveFile(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, this);
	}

	void CopyConfig(ExpansionQuestPersistentServerDataBase dataBase)
	{
		LastWeeklyReset = dataBase.LastWeeklyReset;
		LastDailyReset = dataBase.LastDailyReset;
	}
};