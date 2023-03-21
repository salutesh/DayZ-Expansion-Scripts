/**
 * ExpansionNamalskAdventureData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_NAMALSK_ADVENTURE
#ifdef NAMALSK_TERRAIN
class ExpansionNamalskAdventureDataBase
{
	int Version;
};

class ExpansionNamalskAdventureData: ExpansionNamalskAdventureDataBase
{
	[NonSerialized()];
	static const int VERSION = 0;

	protected ref array<int> m_LastMerchantItemSets = new array<int>;
	protected ref array<int> m_LastMerchantPositions = new array<int>;

	void ExpansionNamalskAdventureData()
	{
		Version = VERSION;
	}

	void CopyFromBaseClass(ExpansionNamalskAdventureDataBase base)
	{
		//! Nothing to do here yet
	}

	static ExpansionNamalskAdventureData Load(string fileName)
	{
		CF_Log.Info("[ExpansionNamalskAdventureData] Load existing namalsk adventure server data file:" + fileName);
		ExpansionNamalskAdventureDataBase dataBase;
		ExpansionJsonFileParser<ExpansionNamalskAdventureDataBase>.Load(fileName, dataBase);

		bool save;
		ExpansionNamalskAdventureData data = new ExpansionNamalskAdventureData();
		if (dataBase.Version < VERSION)
		{
			save = true;
			data.CopyFromBaseClass(dataBase); //! Copy over old data that has not changed.
			data.Version = VERSION;

			if (save)
				Save(data);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Load(fileName, data))
				return NULL;
		}

		return data;
	}

	void Save()
	{
		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", this);
	}

	static void Save(ExpansionNamalskAdventureData data)
	{
		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", data);
	}

	void ResetUsedItemSets()
	{
		m_LastMerchantItemSets.Clear();
	}

	void ResetUsedPositions()
	{
		m_LastMerchantPositions.Clear();
	}
	
	array<int> GetUsedMerchantItemSets()
	{
		return m_LastMerchantItemSets:
	}
	
	array<int> GetUsedMerchantPositions()
	{
		return m_LastMerchantPositions:
	}
};
#endif
#endif