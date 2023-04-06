/**
 * ExpansionAnomalyData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionAnomalyPersistance
{
	NONE = 0,
	LIFETIME = 1
};

enum ExpansionAnomalyLootSpawnType
{
	STATIC = 0,
	DYNAMIC = 1
};

class ExpansionAnomalyStatic
{
	ref array<string> AnomalyTypes = new array<string>;
	vector CenterPosition;
	ref array <ref ExpansionLoot> Loot;
	int LootItemsMin = 1;
	int LootItemsMax = 1;
	ExpansionAnomalyLootSpawnType LootSpawnType = ExpansionAnomalyLootSpawnType.STATIC;

	void ExpansionAnomalyStatic(array<string> anomalyTypes, vector center)
	{
		AnomalyTypes = anomalyTypes;
		CenterPosition = center;
	}
	
	void SetLoot(array <ref ExpansionLoot> loot, int min, int max, ExpansionAnomalyLootSpawnType lootType)
	{
		Loot = new array<ref ExpansionLoot>;
		Loot = loot;
		
		LootItemsMin = min;
		LootItemsMax = max;
		LootSpawnType = lootType;
	}
};

class ExpansionAnomalyDynamic: ExpansionAnomalyStatic
{
	float SquareSize;
	int Amount;
	ExpansionAnomalyPersistance Persistance;

	void ExpansionAnomalyDynamic(array<string> anomalyTypes, vector center, float squareSize = 500, int amount = 1, ExpansionAnomalyPersistance persistance = ExpansionAnomalyPersistance.NONE)
	{
		AnomalyTypes = anomalyTypes;
		CenterPosition = center;
		SquareSize = squareSize;
		Amount = amount;
		Persistance = persistance;
	}
};