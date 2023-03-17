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

#ifdef EXPANSIONMODMARKET
class ExpansionQuestItemForMarket
{
	protected vector ZonePosition;
	protected string ClassName;
	protected int Amount;

	void ExpansionQuestItemForMarket(vector pos, string className, int amount)
	{
		ZonePosition = pos;
		ClassName = className;
		Amount = amount;
	}

	vector GetZonePosition()
	{
		return ZonePosition;
	}

	string GetClassName()
	{
		return ClassName;
	}

	int GetAmount()
	{
		return Amount;
	}

	void SetAmount(int amount)
	{
		Amount = amount;
	}
};
#endif

class ExpansionQuestPersistentServerDataBase
{
	int ConfigVersion;
	int LastWeeklyReset;
	int LastDailyReset;
};

class ExpansionQuestPersistentServerData: ExpansionQuestPersistentServerDataBase
{
	static const int CONFIGVERSION = 1;
#ifdef EXPANSIONMODMARKET
	ref array<ref ExpansionQuestItemForMarket> m_QuestMarketItems;
#endif

	[NonSerialized()]
	static int DAY_TIME = 86400; //! Day in seconds
	[NonSerialized()]
	static int WEEK_TIME = 604800; //! Week in seconds

	void ExpansionQuestPersistentServerData()
	{
		ConfigVersion = 1;

	#ifdef EXPANSIONMODMARKET
		if (!m_QuestMarketItems)
			m_QuestMarketItems = new array<ref ExpansionQuestItemForMarket>;
	#endif
	}

#ifdef EXPANSIONMODMARKET
	void AddQuestMarketItem(vector pos, string className, int amount)
	{
		bool added;
		foreach (ExpansionQuestItemForMarket current: m_QuestMarketItems)
		{
			if (current.GetZonePosition() == pos)
			{
				if (current.GetClassName() == className)
				{
					int currentAmount = current.GetAmount();
					int newAmount = currentAmount + amount;
					current.SetAmount(newAmount);
					added = true;
				}
			}
		}

		if (!added)
		{
			ExpansionQuestItemForMarket questMarketItem = new ExpansionQuestItemForMarket(pos, className, amount);
			m_QuestMarketItems.Insert(questMarketItem);
		}
	}

	void AddQuestMarketItemsToZones()
	{
		auto marketSettings = GetExpansionSettings().GetMarket();
		array<ExpansionMarketTraderZone> zonesToUpdate = new array<ExpansionMarketTraderZone>;
		foreach (ExpansionQuestItemForMarket marketItem: m_QuestMarketItems)
		{
			vector zonePos = marketItem.GetZonePosition();
			ExpansionMarketTraderZone traderZone = marketSettings.GetTraderZoneByPosition(zonePos);
			if (!traderZone)
				return;

			string name = marketItem.GetClassName();
			int amount = marketItem.GetAmount();
			traderZone.AddStock(name, amount);

			if (zonesToUpdate.Find(traderZone) == -1)
				zonesToUpdate.Insert(traderZone);
		}

		foreach (ExpansionMarketTraderZone zone: zonesToUpdate)
		{
			zone.Save();
		}

		m_QuestMarketItems.Clear();
		Save();
	}
#endif

	void SetWeeklyResetTime()
	{
		int year, month, day, hour, minute, second;
		int weeklyResetHour = GetExpansionSettings().GetQuest().WeeklyResetHour;
		int weeklyResetMinute = GetExpansionSettings().GetQuest().WeeklyResetMinute;
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
		int dailyResetHour = GetExpansionSettings().GetQuest().DailyResetHour;
		int dailyResetMinute = GetExpansionSettings().GetQuest().DailyResetMinute;
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
		Print("[ExpansionQuestPersistentServerData] Load existing configuration file:" + EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE);

		ExpansionQuestPersistentServerData data;
		ExpansionQuestPersistentServerDataBase dataBase;

		if (!ExpansionJsonFileParser<ExpansionQuestPersistentServerDataBase>.Load(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, dataBase))
			return NULL;

		if (dataBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestPersistentServerData] Convert existing configuration file:" + EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE + " to version " + CONFIGVERSION);
			data = new ExpansionQuestPersistentServerData();
			//! Copy over old configuration that haven't changed
			data.CopyConfig(dataBase);
			data.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestPersistentServerData>.Load(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, data))
				return NULL;
		}

		if (save)
		{
			data.Save();
		}

		return data;
	}

	void Save()
	{
		ExpansionJsonFileParser<ExpansionQuestPersistentServerData>.Save(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, this);
	}

	void CopyConfig(ExpansionQuestPersistentServerDataBase dataBase)
	{
		LastWeeklyReset = dataBase.LastWeeklyReset;
		LastDailyReset = dataBase.LastDailyReset;
	}
};