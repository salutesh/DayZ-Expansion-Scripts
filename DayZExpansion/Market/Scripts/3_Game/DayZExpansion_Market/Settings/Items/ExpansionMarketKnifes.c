/**
 * ExpansionMarketKnifes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
		
		AddItem("kitchenknife", 	370, 	615,	1,100);
		AddItem("steakknife", 	580, 	960,	1,100);
		AddItem("huntingknife", 	855, 	1425,	1,100);
		AddItem("combatknife", 	1385, 	2310,	1,100);
		AddItem("kukriknife", 	370, 	615,	1,100);
		AddItem("fangeknife", 	910, 	1515,	1,100);
		AddItem("machete", 	2180, 	3630,	1,100);
		AddItem("crudemachete", 	3715, 	6190,	1,100);
		AddItem("orientalmachete", 	355, 	590,	1,100);
		AddItem("cleaver", 	285, 	475,	1,100);
		//! 1.21+
		AddItem("sword", 	355, 	595,	1,100);
	}
};