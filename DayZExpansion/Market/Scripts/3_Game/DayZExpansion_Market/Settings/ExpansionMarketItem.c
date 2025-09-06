/**
 * ExpansionMarketItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

	[NonSerialized()]
	ExpansionMarketCategory Category;

	string ClassName;

	int MaxPriceThreshold;
	int MinPriceThreshold;

	//! @note this is a workaround for a serious Enforce bug (MaxPriceThreshold gets randomly reset to zero on client after a few buys/sells)
	[NonSerialized()]
	private int m_MaxPriceThreshold;
	[NonSerialized()]
	private int m_MinPriceThreshold;

	//! @note not netsynced directly! Encoded to bfloat16 and sent packed together with BuySell and QuantityPercent
	float SellPricePercent;

	//! @note integer representation of bfloat16
	[NonSerialized()]
	int m_SellPricePercent;

	int MaxStockThreshold;
	int MinStockThreshold;

	int QuantityPercent;

	autoptr array< string > SpawnAttachments;
	autoptr array< string > Variants;

	[NonSerialized()]
	autoptr array< int > m_AttachmentIDs;

	[NonSerialized()]
	bool m_IsMagazine;

	[NonSerialized()]
	bool m_IsVariant;

	[NonSerialized()]
	ExpansionMarketItem m_Parent;

	[NonSerialized()]
	bool m_StockOnly;

	[NonSerialized()]
	bool m_ShowInMenu;

	[NonSerialized()]
	int m_Idx;

	[NonSerialized()]
	bool m_UpdateView;
	
	[NonSerialized()]
	EntityAI m_PreviewEntity;

#ifdef EXPANSIONMODHARDLINE
	[NonSerialized()]
	int m_Rarity;

	[NonSerialized()]
	int m_RequiredRep;
#endif

	// ------------------------------------------------------------
	// ExpansionMarketItem Constructor
	// ------------------------------------------------------------
	void ExpansionMarketItem( int catID, string className, int minPrice, int maxPrice, int minStock, int maxStock, array<string> attachments = null, array<string> variants = null, float sellPricePercent = -1, int quantityPercent = -1, int itemID = -1, array<int> attachmentIDs = null)
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

		SellPricePercent = sellPricePercent;

		MinStockThreshold = minStock;
		MaxStockThreshold = maxStock;

		QuantityPercent = quantityPercent;

		SpawnAttachments = attachments;

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

		SanityCheckAndRepair();
	}
	
	void SanityCheckAndRepair()
	{
		if ( MinPriceThreshold < 0 )
		{
			Error("[ExpansionMarketItem] The minimum price must be 0 or higher for '" + ClassName + "'");
			MinPriceThreshold = 0;
		}

		if ( MinStockThreshold < 0 )
		{
			Error("[ExpansionMarketItem] The minimum stock must be 0 or higher for '" + ClassName + "'");
			MinStockThreshold = 0;
		}

		if ( MinPriceThreshold > MaxPriceThreshold )
		{
			Error("[ExpansionMarketItem] The minimum price must be lower than or equal to the maximum price for '" + ClassName + "'");
			MaxPriceThreshold = MinPriceThreshold;
		}

		if ( MinStockThreshold > MaxStockThreshold )
		{
			Error("[ExpansionMarketItem] The minimum stock must be lower than or equal to the maximum stock for '" + ClassName + "'");
			MaxStockThreshold = MinStockThreshold;
		}

		m_MinPriceThreshold = MinPriceThreshold;
		m_MaxPriceThreshold = MaxPriceThreshold;

		//! Convert to integer representation of bfloat16
		m_SellPricePercent = CF_Cast<float, int>.Reinterpret(SellPricePercent) >> 16;
		//! Convert integer representation of bfloat16 back to float to make sure values used on server and client are the same
		SellPricePercent = CF_Cast<int, float>.Reinterpret(m_SellPricePercent << 16);

		if (EXTrace.MARKET && SellPricePercent != -1)
			EXTrace.Print(true, this, ClassName + " SellPricePercent " + SellPricePercent);

		SetAttachments(SpawnAttachments);

		if (GetGame().IsKindOf(ClassName, "Magazine_Base") && !GetGame().IsKindOf(ClassName, "Ammunition_Base"))
			m_IsMagazine = true;
	}

#ifdef EXPANSIONMODHARDLINE
	//! Server only!
	void SetRarityAndRepReq()
	{
		if (m_StockOnly)
		{
			EXError.Warn(this, string.Format("%1: rarity and reputation requirements for stockonly netsync variants are set in ExpansionMarketCategory::AddVariants", ClassName));
			return;
		}

		ExpansionHardlineSettings settings = GetExpansionSettings().GetHardline();
		m_Rarity = settings.GetItemRarityByType(ClassName);
		if (m_Rarity != ExpansionHardlineItemRarity.NONE)
			m_RequiredRep = settings.GetReputationForRarity(m_Rarity);
	}
#endif

	void SetAttachments(TStringArray attachments)
	{
		SpawnAttachments = new array< string >;
		if ( attachments )
		{
			foreach ( string attClsName : attachments )
			{
				attClsName.ToLower();
				//! Check if attachment is not same classname as parent to prevent infinite recursion (user error)
				if (attClsName == ClassName)
					EXError.Error(null, "MARKET CONFIGURATION ERROR: Trying to add " + ClassName + " as attachment to itself", {});
				else
					SpawnAttachments.Insert( attClsName );
			}
		}
	}

	void SetAttachmentsFromIDs()
	{
		SpawnAttachments.Clear();
		foreach (int attachmentID: m_AttachmentIDs)
		{
			ExpansionMarketItem attachment = ExpansionMarketCategory.GetGlobalItem(attachmentID, false);
			if (attachment)
				SpawnAttachments.Insert(attachment.ClassName);
			else
				EXError.Error(null, "MARKET CONFIGURATION ERROR: Attachment ID " + attachmentID + " does not exist for item " + ClassName + " (ID " + ItemID + ")", {});
		}
		m_AttachmentIDs = NULL;
	}

	/**
	 * @brief create derivative with same properties but different attachments
	 */
	ExpansionMarketItem CreateDerivative(TIntArray attachmentIDs)
	{
		ExpansionMarketItem item = new ExpansionMarketItem(CategoryID, ClassName, MinPriceThreshold, MaxPriceThreshold, MinStockThreshold, MaxStockThreshold, null, Variants, SellPricePercent, QuantityPercent, ItemID, attachmentIDs);

		item.SetAttachmentsFromIDs();

	#ifdef EXPANSIONMODHARDLINE
		item.m_Rarity = m_Rarity;
		item.m_RequiredRep = m_RequiredRep;
	#endif

		return item;
	}

	bool IsStaticStock()
	{
		return MaxStockThreshold > 0 && MinStockThreshold == MaxStockThreshold;
	}
	
	// ------------------------------------------------------------
	// Expansion CalculatePrice
	// Calculates the current price of the item for one item at the current stock level
	// ------------------------------------------------------------
	int CalculatePrice(int stock, float modifier = 1.0, bool round = false)
	{
		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - Start - " + ClassName + " - stock " + stock + " modifier " + modifier + " minstock " + MinStockThreshold + " maxstock " + MaxStockThreshold + " maxprice " + m_MaxPriceThreshold + " minprice " + m_MinPriceThreshold + " pct " + SellPricePercent);
		#endif

		float price;

		if (!IsStaticStock() && MaxStockThreshold != 0)
			price = ExpansionMath.PowerConversion(MinStockThreshold, MaxStockThreshold, stock, m_MaxPriceThreshold, m_MinPriceThreshold, 6.0);
		else
			price = m_MinPriceThreshold;

		price *= modifier;

		if (round)
			price = Math.Round(price);

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketItem::CalculatePrice - End and return calculated price: " + price);
		#endif
		
		return (int) price;
	}

	bool IsMagazine()
	{
		return m_IsMagazine;
	}

	map<string, bool> GetAttachmentTypes(out int magAmmoCount)
	{
		map<string, bool> attachmentTypes = new map<string, bool>;

		bool isMag = IsMagazine();

		foreach (string attachmentName: SpawnAttachments)
		{
			bool isMagAmmo = false;
			if (!attachmentTypes.Find(attachmentName, isMagAmmo))
			{
				if (isMag && GetGame().IsKindOf(attachmentName, "Ammunition_Base"))
					isMagAmmo = true;
				attachmentTypes.Insert(attachmentName, isMagAmmo);
			}
			if (isMagAmmo)
				magAmmoCount++;
		}

		return attachmentTypes;
	}

	map<string, int> GetMagAmmoQuantities(map<string, bool> attachmentTypes, int magAmmoCount)
	{
		if (!attachmentTypes.Count() || !magAmmoCount || !IsMagazine())
			return NULL;

		map<string, int> magAmmoCounts = new map<string, int>;
		int magCapacity = GetGame().ConfigGetInt("CfgMagazines " + ClassName + " count");

		int totalAmmo;
		int quantityPercent = QuantityPercent;
		if (quantityPercent < 0)
			quantityPercent = 100;
		int ammoMax = Math.Ceil(magCapacity * quantityPercent / 100);
		while (totalAmmo < ammoMax)
		{
			foreach (string attachmentName, bool isMagAmmo: attachmentTypes)
			{
				if (isMagAmmo)
				{
					int ammoQuantity;
					if (!magAmmoCounts.Find(attachmentName, ammoQuantity))
					{
						if (magAmmoCount == 1)
							ammoQuantity = ammoMax;
						else
							ammoQuantity = 1;
						magAmmoCounts.Insert(attachmentName, ammoQuantity);
						totalAmmo += ammoQuantity;
					}
					else
					{
						magAmmoCounts.Set(attachmentName, ammoQuantity + 1);
						totalAmmo++;
					}

					if (totalAmmo == ammoMax)
						break;
				}
			}
		}

		return magAmmoCounts;
	}

	void AddDefaultAttachments()
	{
		if (SpawnAttachments.Count())
			return;

		if (IsMagazine() && QuantityPercent)
		{
			//! Add ammo "attachment" (use 1st ammo item) if not yet present and quantity is not zero
			TStringArray ammoItems = new TStringArray;
			GetGame().ConfigGetTextArray("CfgMagazines " + ClassName + " ammoItems", ammoItems);
			foreach (string ammo: ammoItems)
			{
				ammo.ToLower();
				if (SpawnAttachments.Find(ammo) == -1 && ExpansionMarketCategory.GetGlobalItem(ammo, false))
				{
					SpawnAttachments.Insert(ammo);
					break;
				}
			}

			if (SpawnAttachments.Count() == 0)
			{
				EXError.MarketCfgWarn(null, string.Format("Magazine %1 has no specified ammo attachment, and the magazine's default ammo (%2) does not exist in market. The magazine will be empty when bought.", ClassName, ExpansionString.JoinStrings(ammoItems, " or ")));
			}
		}
	}

	bool IsVehicle()
	{
		return ExpansionStatic.IsVehicle(ClassName);
	}
}
