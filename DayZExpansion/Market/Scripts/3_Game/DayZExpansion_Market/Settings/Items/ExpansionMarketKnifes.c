/**
 * ExpansionMarketKnifes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketKnifes: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 32;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_KNIFES";
		m_FileName = "Knifes";
		
		AddItem("KitchenKnife", 				8,		16,		1,		100,	0);
		AddItem("SteakKnife", 					9,		18,		1,		100,	0);
		AddItem("HuntingKnife", 				9,		18,		1,		100,	0);
		AddItem("CombatKnife", 					10,		20,		1,		100,	0);
		AddItem("KukriKnife", 					10,		20,		1,		100,	0);
		AddItem("FangeKnife", 					10,		20,		1,		100,	0);
		AddItem("Machete", 						12,		24,		1,		100,	0);
		AddItem("CrudeMachete", 				12,		24,		1,		100,	0);
		AddItem("OrientalMachete", 				12,		24,		1,		100,	0);
	}
};