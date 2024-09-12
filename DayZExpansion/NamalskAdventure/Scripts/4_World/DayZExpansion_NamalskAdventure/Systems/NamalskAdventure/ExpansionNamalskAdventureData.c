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

class ExpansionNamalskAdventureDataBase
{
	int Version;
	
	ref array<int> LastMerchantItemSets = new array<int>;
	ref array<int> LastMerchantPositions = new array<int>;
};

class ExpansionNamalskAdventureData: ExpansionNamalskAdventureDataBase
{
	[NonSerialized()];
	static const int VERSION = 1;

	void ExpansionNamalskAdventureData()
	{
		Version = VERSION;
	}

	void CopyFromBaseClass(ExpansionNamalskAdventureDataBase base)
	{
		LastMerchantItemSets = base.LastMerchantItemSets;
		LastMerchantPositions = base.LastMerchantPositions;
	}

	static ExpansionNamalskAdventureData Load(string fileName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionNamalskAdventureData - Load");
#endif

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", this);
	}

	static void Save(ExpansionNamalskAdventureData data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, null, "::ExpansionNamalskAdventureData - Save");
#endif

		ExpansionJsonFileParser<ExpansionNamalskAdventureData>.Save(ExpansionNamalskModule.s_dataFolderPath + "ServerData.json", data);
	}

	void ResetUsedItemSets()
	{
		LastMerchantItemSets.Clear();
	}

	void ResetUsedPositions()
	{
		LastMerchantPositions.Clear();
	}

	array<int> GetUsedMerchantItemSets()
	{
		return LastMerchantItemSets:
	}

	array<int> GetUsedMerchantPositions()
	{
		return LastMerchantPositions:
	}
};
