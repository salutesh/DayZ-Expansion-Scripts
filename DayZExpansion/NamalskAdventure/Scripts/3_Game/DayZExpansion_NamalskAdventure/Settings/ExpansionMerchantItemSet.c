/**
 * ExpansionMerchantItemSet.c
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
class ExpansionMerchantItem
{
	string ClassName;
	int Amount;
	ExpansionMarketTraderBuySell BuySell;

	void ExpansionMerchantItem(string name, int amount, ExpansionMarketTraderBuySell buySell)
	{
		ClassName = name;
		Amount = amount;
		BuySell = buySell;
	}
};

class ExpansionMerchantItemSet
{
	int ID = -1;
	ref array<ref ExpansionMerchantItem> Items;

	void ExpansionMerchantItemSet()
	{
		Items = new array<ref ExpansionMerchantItem>;
	}
	
	void AddItem(string name, int amount, ExpansionMarketTraderBuySell buySell)
	{
		ExpansionMerchantItem item = new ExpansionMerchantItem(name, amount, buySell);
		Items.Insert(item);
	}
};
#endif