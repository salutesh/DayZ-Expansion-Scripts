/**
 * ExpansionLoot.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLootBase
{
	string Name;
	float Chance;
}

class ExpansionLootVariantV1: ExpansionLootBase
{
	ref TStringArray Attachments;

	void ExpansionLootVariantV1( string name = "", TStringArray attachments = NULL, float chance = 1 )
	{
		Name = name;
		Attachments = attachments;
		Chance = chance;
	}

	ExpansionLootVariant ConvertVariant()
	{
		array<ref ExpansionLootVariant> new_Att = new array<ref ExpansionLootVariant>;
		foreach(string att: Attachments)
		{
			new_Att.Insert(new ExpansionLootVariant(att, null, 1));
		}
		
		return new ExpansionLootVariant(Name, new_Att, Chance);
	}
};

class ExpansionLootV1 : ExpansionLootVariantV1
{
	float QuantityPercent;
	int Max;
	int Min;
	ref array< ref ExpansionLootVariantV1 > Variants;

	void ExpansionLoot( string name = "", TStringArray attachments = NULL, float chance = 1, int quantityPercent = -1, array< ref ExpansionLootVariantV1 > variants = NULL, int max = -1, int min = 0 )
	{
		QuantityPercent = quantityPercent;
		Max = max;
		Min = min;
		Variants = variants;
	}

	ExpansionLoot Convert()
	{
		auto loot = ConvertVariant();
		auto newLoot = new ExpansionLoot(Name, loot.Attachments, Chance);

		array< ref ExpansionLootVariant > newVariants = new array< ref ExpansionLootVariant >;
		foreach(ExpansionLootVariantV1 variant: Variants)
		{
			newVariants.Insert(variant.ConvertVariant());
		}
		newLoot.Variants 		= newVariants;
		newLoot.QuantityPercent = QuantityPercent;
		newLoot.Max 			= Max;
		newLoot.Min 			= Min;

		return newLoot;
	}
};

class ExpansionLootVariant: ExpansionLootBase
{
	ref array<ref ExpansionLootVariant> Attachments;

	[NonSerialized()]
	float m_RemainingChance;

	void ExpansionLootVariant( string name = "", array<ref ExpansionLootVariant> attachments = NULL, float chance = 1 )
	{
		Name = name;
		Attachments = attachments;
		Chance = chance;
	}
};

class ExpansionLoot : ExpansionLootVariant
{
	float QuantityPercent;
	int Max;
	int Min;
	ref array< ref ExpansionLootVariant > Variants;

	[NonSerialized()]
	int m_Remaining;

	void ExpansionLoot( string name = "", array<ref ExpansionLootVariant> attachments = NULL, float chance = 1, int quantityPercent = -1, array< ref ExpansionLootVariant > variants = NULL, int max = -1, int min = 0 )
	{
		QuantityPercent = quantityPercent;
		Max = max;
		Min = min;
		Variants = variants;
	}
};
