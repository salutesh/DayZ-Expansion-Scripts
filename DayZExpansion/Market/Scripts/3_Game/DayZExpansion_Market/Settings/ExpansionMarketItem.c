/**
 * ExpansionMarketItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	static int m_CurrentItemId;

	[NonSerialized()]
	int ItemID;

	[NonSerialized()]
	int CategoryID;

	string ClassName;

	ExpansionMarketCurrency MaxPriceThreshold;
	ExpansionMarketCurrency MinPriceThreshold;

	int MaxStockThreshold;
	int MinStockThreshold;
	int PurchaseType;

	autoptr array< string > SpawnAttachments;
	autoptr array< string > Variants;

	[NonSerialized()]
	autoptr array< int > m_AttachmentIDs;

	[NonSerialized()]
	bool m_IsVariant;

	[NonSerialized()]
	ExpansionMarketItem m_Parent;

	[NonSerialized()]
	bool m_StockOnly;

	[NonSerialized()]
	bool m_ShowInMenu;

	[NonSerialized()]
	bool m_Idx;

	[NonSerialized()]
	bool m_UpdateView;
	
	// ------------------------------------------------------------
	// ExpansionMarketItem Constructor
	// ------------------------------------------------------------
	void ExpansionMarketItem( int catID, string className, ExpansionMarketCurrency minPrice, ExpansionMarketCurrency maxPrice, int minStock, int maxStock, int purchaseType, array<string> attachments = null, array<string> variants = null, int itemID = -1, array<int> attachmentIDs = null)
	{
		if (itemID == -1)
			ItemID = ++m_CurrentItemId;
		else
			ItemID = itemID;

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
			foreach ( string attClsName : attachments )
			{
				attClsName.ToLower();
				SpawnAttachments.Insert( attClsName );
			}
		}

		Variants = new array< string >;
		if ( variants )
		{
			foreach ( string variantClsName : variants )
			{
				variantClsName.ToLower();
				Variants.Insert( variantClsName );
			}
		}

		if ( attachmentIDs )
			m_AttachmentIDs = attachmentIDs;
	}
	
	void SetAttachmentsFromIDs()
	{
		SpawnAttachments.Clear();
		foreach (int attachmentID: m_AttachmentIDs)
		{
			ExpansionMarketItem attachment = GetExpansionSettings().GetMarket().GetItem(attachmentID, false);
			if (attachment)
				SpawnAttachments.Insert(attachment.ClassName);
			else
				EXPrint("ExpansionMarketItem::SetAttachmentsFromIDs - WARNING: Attachment ID " + attachmentID + " does not exist!");
		}
		m_AttachmentIDs = NULL;
	}

	bool IsStaticStock()
	{
		return MaxStockThreshold > 0 && MinStockThreshold == MaxStockThreshold;
	}
	
	// ------------------------------------------------------------
	// Expansion CalculatePrice
	// Calculates the current price of the item for one item at the current stock level
	// ------------------------------------------------------------
	ExpansionMarketCurrency CalculatePrice(int stock, float modifier = 1.0)
	{
		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - Start - " + ClassName + " - stock " + stock);
		#endif

		float price;

		if (!IsStaticStock() && MaxStockThreshold != 0)
			price = ExpansionMath.PowerConversion(MinStockThreshold, MaxStockThreshold, stock, MaxPriceThreshold, MinPriceThreshold, 6.0);
		else
			price = MinPriceThreshold;

		price = Math.Round(price * modifier);

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - End and return calculated price: " + price);
		#endif
		
		return (ExpansionMarketCurrency) price;
	}
}