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
};

class ExpansionQuestPersistentServerData: ExpansionQuestPersistentServerDataBase
{
	static const int CONFIGVERSION = 1;
#ifdef EXPANSIONMODMARKET
	ref array<ref ExpansionQuestItemForMarket> m_QuestMarketItems;
#endif

	void ExpansionQuestPersistentServerData()
	{
		ConfigVersion = 1;

	#ifdef EXPANSIONMODMARKET
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		ExpansionJsonFileParser<ExpansionQuestPersistentServerData>.Save(EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE, this);
	}

	void CopyConfig(ExpansionQuestPersistentServerDataBase dataBase)
	{
	}
};
