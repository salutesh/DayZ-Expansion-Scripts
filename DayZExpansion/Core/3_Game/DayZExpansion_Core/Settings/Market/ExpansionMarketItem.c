/**
 * ExpansionMarketItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMarketItems
 * @brief		Item settings class
 **/
class ExpansionMarketItem
{
	[NonSerialized()]
	int CategoryID;

	string ClassName;

	ExpansionMarketCurrency MaxPriceThreshold;
	ExpansionMarketCurrency MinPriceThreshold;

	int MaxStockThreshold;
	int MinStockThreshold;

	int PurchaseType;

	[NonSerialized()]
	int StaticPrice;
	
	[NonSerialized()]
	bool StaticStock;

	autoptr array< string > SpawnAttachments;
	
	// ------------------------------------------------------------
	// ExpansionMarketItem Constructor
	// ------------------------------------------------------------
	void ExpansionMarketItem( int catID, string className, ExpansionMarketCurrency minPrice, ExpansionMarketCurrency maxPrice, int minStock, int maxStock, int purchaseType, ref array<string> attachments = null )
	{
		CategoryID = catID;
		
		ClassName = className;
		ClassName.ToLower();

		MinPriceThreshold = minPrice;
		MaxPriceThreshold = maxPrice;

		MinStockThreshold = minStock;
		MaxStockThreshold = maxStock;

		PurchaseType = purchaseType;

		SpawnAttachments = new array< string >;
		if ( attachments )
		{
			for ( int i = 0; i < attachments.Count(); i++ )
			{
				string attClsName = attachments[i];
				attClsName.ToLower();
				SpawnAttachments.Insert( attClsName );
			}
		}

		UpdateStatics();
	}

	// ------------------------------------------------------------
	// Expansion UpdateStatics
	// Updates the static price and static stock
	// ------------------------------------------------------------
	void UpdateStatics()
	{
		if ( MinPriceThreshold == MaxPriceThreshold )
		{
			StaticPrice = MinPriceThreshold;
		}

		if ( MinStockThreshold == MaxStockThreshold )
		{
			StaticStock = true;
			MinStockThreshold = 1;
			MaxStockThreshold = 1;
		}
		else 
		{
			StaticStock = false;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion CalculatePrice
	// Calculates the current price of the item for one item at the current stock level
	// ------------------------------------------------------------
	ExpansionMarketCurrency CalculatePrice( int stock )
	{
		if ( StaticPrice > 0 )
			return StaticPrice;

		if ( StaticStock )
			stock = 1;

		if ( stock <= 0 )
			return 0;

		if ( stock >= MaxStockThreshold )
			return MinPriceThreshold;
			
		if ( stock <= MinStockThreshold )
			return MaxPriceThreshold;
		
		//return LinearConversion( MinStockThreshold, MaxStockThreshold, stock, MaxPriceThreshold, MinPriceThreshold, true );

		float minStock = MinStockThreshold;
		float maxStock = MaxStockThreshold;
		float cStock = stock;
		float pStock = ( cStock - minStock ) / ( maxStock - minStock );

		float nPriceDiff = ( 1.0 - pStock ) / ( MaxPriceThreshold - MinPriceThreshold );
		
		return MinPriceThreshold + ( ExpansionMarketCurrency ) nPriceDiff;
	}
}