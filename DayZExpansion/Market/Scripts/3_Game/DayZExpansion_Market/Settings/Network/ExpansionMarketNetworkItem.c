/**
 * ExpansionMarketNetworkItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNetworkBaseItem
{
	int ItemID;

	int Stock;

	void ExpansionMarketNetworkBaseItem(int itemID, int stock)
	{
		ItemID = itemID;
		Stock = stock;
	}
};

class ExpansionMarketNetworkItem: ExpansionMarketNetworkBaseItem
{
	int CategoryID;

	string ClassName;

	ExpansionMarketCurrency MaxPriceThreshold;
	ExpansionMarketCurrency MinPriceThreshold;

	int MaxStockThreshold;
	int MinStockThreshold;

	autoptr array< int > AttachmentIDs;
	autoptr array< string > Variants;

	ExpansionMarketTraderBuySell BuySell;

	[NonSerialized()]
	bool m_StockOnly;
};