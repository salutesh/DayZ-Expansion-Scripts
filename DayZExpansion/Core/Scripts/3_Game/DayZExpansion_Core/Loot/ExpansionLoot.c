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

class ExpansionLootVariant
{
	string Name;
	ref TStringArray Attachments;
	float Chance;

	[NonSerialized()]
	float m_RemainingChance;

	void ExpansionLootVariant( string name, TStringArray attachments = NULL, float chance = 1 )
	{
		Name = name;
		Attachments = attachments;
		Chance = chance;
	}
};

class ExpansionLoot : ExpansionLootVariant
{
	int QuantityPercent;
	int Max;
	int Min;
	ref array< ref ExpansionLootVariant > Variants;

	[NonSerialized()]
	int m_Remaining;

	void ExpansionLoot( string name, TStringArray attachments = NULL, float chance = 1, int quantityPercent = -1, array< ref ExpansionLootVariant > variants = NULL, int max = -1, int min = 0 )
	{
		QuantityPercent = quantityPercent;
		Max = max;
		Min = min;
		Variants = variants;
	}
};
