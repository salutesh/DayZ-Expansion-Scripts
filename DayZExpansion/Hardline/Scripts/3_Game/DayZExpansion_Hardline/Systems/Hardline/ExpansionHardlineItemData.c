/**
 * ExpansionHardlineItemData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Legacy, settings v1
class ExpansionHardlineItemData
{
	string ClassName;
	ExpansionHardlineItemRarity Rarity;

	void SetType(string type)
	{
		ClassName = type;
	}
	
	string GetType()
	{
		return ClassName;
	}
	
	void SetRatity(ExpansionHardlineItemRarity rarity)
	{
		Rarity = rarity;
	}

	ExpansionHardlineItemRarity GetRarity()
	{
		return Rarity;
	}
};
