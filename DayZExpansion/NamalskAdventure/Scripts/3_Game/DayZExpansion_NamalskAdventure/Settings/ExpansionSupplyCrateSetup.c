/**
 * ExpansionSupplyCrateSetup.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSupplyCrateSetupBase
{
	string ClassName;
	vector Position;
	vector Orientation;
}

class ExpansionSupplyCrateSetupV7: ExpansionSupplyCrateSetupBase
{
	ref array<ref ExpansionLootV1> CrateLoot;

	ExpansionSupplyCrateSetup Convert()
	{
		auto crate = new ExpansionSupplyCrateSetup(ClassName, Position, Orientation);

		foreach (ExpansionLootV1 lootV1: CrateLoot)
		{
			crate.CrateLoot.Insert(lootV1.Convert());
		}

		return crate;
	}
}

class ExpansionSupplyCrateSetup: ExpansionSupplyCrateSetupBase
{
	ref array<ref ExpansionLoot> CrateLoot;

	void ExpansionSupplyCrateSetup(string className, vector pos, vector ori)
	{
		ClassName = className;
		Position = pos;
		Orientation = ori;
		CrateLoot = new array<ref ExpansionLoot>;
	}

	void AddLoot(string name, TStringArray attachments = NULL, float chance = 1, int quantityPercent = -1, array<ref ExpansionLootVariant> variants = NULL, int max = -1, int min = 0)
	{
		array<ref ExpansionLootVariant> attachmentLoot;
		if (attachments && attachments.Count())
		{
			attachmentLoot = new array<ref ExpansionLootVariant>;
			foreach (string attachment: attachments)
			{
				attachmentLoot.Insert(new ExpansionLootVariant(attachment));
			}
		}
		ExpansionLoot loot = new ExpansionLoot(name, attachmentLoot, chance, quantityPercent, variants , max, min);
		CrateLoot.Insert(loot);
	}

	void InsertLoot(array<ref ExpansionLoot> loots)
	{
		foreach (ExpansionLoot loot: loots)
		{
			CrateLoot.Insert(loot);
		}
	}

	array<ref ExpansionLoot> GetLoot()
	{
		return CrateLoot;
	}
};