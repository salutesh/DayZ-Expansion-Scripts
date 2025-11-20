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
	ref TIntArray m_StaticNetworkRepresentation;

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
	ref set<ExpansionMarketItem> m_VariantsShownInMenu;

	[NonSerialized()]
	int m_Idx;

	[NonSerialized()]
	bool m_UpdateView;
	
	[NonSerialized()]
	EntityAI m_PreviewEntity;

	[NonSerialized()]
	int m_Rarity;

	[NonSerialized()]
	int m_RequiredRep;

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

		MinPriceThreshold = minPrice;
		MaxPriceThreshold = maxPrice;

		SellPricePercent = sellPricePercent;

		MinStockThreshold = minStock;
		MaxStockThreshold = maxStock;

		QuantityPercent = quantityPercent;

		SpawnAttachments = attachments;

		Variants = variants;

		m_AttachmentIDs = attachmentIDs;

		SanityCheckAndRepair();

	#ifndef SERVER
		m_VariantsShownInMenu = new set<ExpansionMarketItem>;
	#endif
	}
	
	void SanityCheckAndRepair()
	{
		//! Make sure item classnames are lowercase
		ClassName.ToLower();

		if ( MinPriceThreshold < 0 )
		{
			EXError.Error(null, "MARKET CONFIGURATION ERROR: The minimum price must be 0 or higher for '" + ClassName + "'", {});
			MinPriceThreshold = 0;
		}

		if ( MinStockThreshold < 0 )
		{
			EXError.Error(null, "MARKET CONFIGURATION ERROR: The minimum stock must be 0 or higher for '" + ClassName + "'", {});
			MinStockThreshold = 0;
		}

		if ( MinPriceThreshold > MaxPriceThreshold )
		{
			EXError.Error(null, "MARKET CONFIGURATION ERROR: The minimum price must be lower than or equal to the maximum price for '" + ClassName + "'", {});
			MaxPriceThreshold = MinPriceThreshold;
		}

		if ( MinStockThreshold > MaxStockThreshold )
		{
			EXError.Error(null, "MARKET CONFIGURATION ERROR: The minimum stock must be lower than or equal to the maximum stock for '" + ClassName + "'", {});
			MaxStockThreshold = MinStockThreshold;
		}

		if (MinStockThreshold > 655535)
		{
			EXError.Warn(null, "Market configuration warning: The minimum stock must be lower than or equal to 655535 for '" + ClassName + "'", {});
			MinStockThreshold = 655535;
		}

		if (MaxStockThreshold > 655535)
		{
			EXError.Warn(null, "Market configuration warning: The maximum stock must be lower than or equal to 655535 for '" + ClassName + "'", {});
			MaxStockThreshold = 655535;
		}

		//! Convert to integer representation of bfloat16
		m_SellPricePercent = CF_Cast<float, int>.Reinterpret(SellPricePercent) >> 16;
		//! Convert integer representation of bfloat16 back to float to make sure values used on server and client are the same
		SellPricePercent = CF_Cast<int, float>.Reinterpret(m_SellPricePercent << 16);

		if (EXTrace.MARKET && SellPricePercent != -1)
			EXTrace.Print(true, this, ClassName + " SellPricePercent " + SellPricePercent);

		SetAttachments(SpawnAttachments);

		SetVariants(Variants);

		if (g_Game.IsKindOf(ClassName, "Magazine_Base") && !g_Game.IsKindOf(ClassName, "Ammunition_Base"))
			m_IsMagazine = true;
	}

	void CreateStaticNetworkRepresentation()
	{
		//! Encoding on-the-fly when sending network items would (roughly) double the processing time due to string encoding,
		//! so we pre-fill some static encoded values

		auto serializer = new ExpansionArraySerializer;

		auto writer = new ExpansionBitStreamArrayWriter(serializer);

		//! @note Technically itemID is also static, but the 16-bit values for ItemID and stock
		//! make a nice aligned pair and are needed together in NetworkBaseItem

		writer.WriteUInt(CategoryID, 16);

		WriteClassNameLowerHashed(writer, ClassName);

		TIntArray attachmentIDs = {};
		foreach (string attClassName: SpawnAttachments)
		{
			ExpansionMarketItem attachment = ExpansionMarketCategory.GetGlobalItem(attClassName);
			if (attachment)
				attachmentIDs.Insert(attachment.ItemID);
			else
				EXError.Warn(null, "ExpansionMarketItem::WriteTo - WARNING: Attachment '" + attClassName + "' does not exist!", {});
		}

		writer.WriteUInt(attachmentIDs.Count(), 8);
		foreach (int attID: attachmentIDs)
		{
			writer.WriteUInt(attID, 16);
		}

		int count = Variants.Count();

		writer.WriteUInt(count, 8);

		if (count)
		{
			string prefix = ExpansionString.FindLongestCommonPrefix(ClassName, Variants, true);
			int substringLength = prefix.Length();
			bool isSubstring = (substringLength > 0 && prefix != ClassName);

			writer.WriteBool(isSubstring);

			string className;

			if (isSubstring)
			{
				writer.WriteUInt(substringLength, 7);
				className = prefix;
			}
			else
			{
				className = ClassName;
			}

			int classNameLength = className.Length();

			foreach (string variant: Variants)
			{
				int variantLength = variant.Length();

				isSubstring = false;
				if (variantLength > classNameLength && variant.IndexOf(className) == 0 && variantLength - classNameLength <= 2)
					isSubstring = true;

				writer.WriteBool(isSubstring);

				if (isSubstring)
				{
					variant = variant.Substring(classNameLength, variantLength - classNameLength);
					WriteClassNameLower(writer, variant);
				}
				else
				{
					WriteClassNameLowerHashed(writer, variant);
				}

			#ifdef DIAG_DEVELOPER
				if (isSubstring)
					EXTrace.Print(EXTrace.MARKET, this, className + "*" + variant);
				else
					EXTrace.Print(EXTrace.MARKET, this, variant);
			#endif
			}
		}

		writer.Flush();

		m_StaticNetworkRepresentation = serializer.m_Data;
	}

	void WriteClassNameLowerHashed(ExpansionBitStreamArrayWriter writer, string className)
	{
		if (className.Length() > 2)
		{
			int hashA;
			int hashB;
			ExpansionItemNameTable.Hash(className, hashA, hashB);

			if (ExpansionItemNameTable.GetTypeByHash(hashA, hashB) && !ExpansionItemNameTable.IsHashColliding(className))
			{
				//! 25 bits
				writer.WriteBool(true);
				ExpansionItemNameTable.WriteHash(writer, hashA, hashB);

				return;
			}
		}

		//! 1 bit + 7 bits string length + 6 * <string length> bits
		writer.WriteBool(false);
		WriteClassNameLower(writer, className);
	}

	void WriteClassNameLower(ExpansionBitStreamArrayWriter writer, string value)
	{
		int bits = 7;
		string alphabet = ExpansionBitStream.CLASSNAME_ALPHABET_LOWERCASE;
		int length = value.Length();

		if (bits < BIT_INT_SIZE)
		{
			int maxLength = Math.Pow(2, bits) - 1;

			if (length > maxLength)
			{
				EXError.MarketCfgError(null, string.Format("string length %1 exceeds allowed maximum %2: '%3'", length.ToString(), maxLength.ToString(), value));
				length = maxLength;
			}
		}

		writer.WriteUInt(length, bits);

		int alphabetBits = ExpansionBitStream.BitSize(alphabet.Length());

		for (int i = 0; i < length; ++i)
		{
			int v = alphabet.IndexOf(value[i]);

			if (v == -1)
			{
				string indicator = "";

				for (int j = 0; j < i; ++j)
				{
					indicator += "-";
				}

				indicator += "^";

				EXError.MarketCfgError(null, string.Format("Invalid character '%1' at position %2:\n'%3'\n %4", value[i], (i + 1).ToString(), value, indicator));

				v = 10;
			}

			writer.WriteUInt(v, alphabetBits);
		}
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
		SpawnAttachments = {};
		if ( attachments )
		{
			foreach (int i, string attClsName : attachments)
			{
				if (i == 255)
				{
					EXError.Error(null, "MARKET CONFIGURATION ERROR: The max allowed number of attachments per item is 255. Item: '" + ClassName + "'", {});
					break;
				}

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
		SpawnAttachments = {};
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

	void SetVariants(TStringArray variants)
	{
		Variants = {};
		if (variants)
		{
			foreach (int i, string variant: variants)
			{
				if (i == 255)
				{
					EXError.Error(null, "MARKET CONFIGURATION ERROR: The max allowed number of variants per item is 255. Item: '" + ClassName + "'", {});
					break;
				}

				variant.ToLower();
				Variants.Insert(variant);
			}
		}
	}

	/**
	 * @brief create derivative with same properties but different attachments
	 */
	ExpansionMarketItem CreateDerivative(TIntArray attachmentIDs)
	{
		ExpansionMarketItem item = new ExpansionMarketItem(CategoryID, ClassName, MinPriceThreshold, MaxPriceThreshold, MinStockThreshold, MaxStockThreshold, null, Variants, SellPricePercent, QuantityPercent, ItemID, attachmentIDs);

		item.m_SellPricePercent = m_SellPricePercent;
		item.m_IsMagazine = m_IsMagazine;

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
		EXPrint("ExpansionMarketItem::CalculatePrice - Start - " + ClassName + " - stock " + stock + " modifier " + modifier + " minstock " + MinStockThreshold + " maxstock " + MaxStockThreshold + " maxprice " + MaxPriceThreshold + " minprice " + MinPriceThreshold + " pct " + SellPricePercent);
		#endif

		float price;

		if (!IsStaticStock() && MaxStockThreshold != 0)
			price = ExpansionMath.PowerConversion(MinStockThreshold, MaxStockThreshold, stock, MaxPriceThreshold, MinPriceThreshold, 6.0);
		else
			price = MinPriceThreshold;

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
				if (isMag && g_Game.IsKindOf(attachmentName, "Ammunition_Base"))
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
		int magCapacity = g_Game.ConfigGetInt("CfgMagazines " + ClassName + " count");

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
			g_Game.ConfigGetTextArray("CfgMagazines " + ClassName + " ammoItems", ammoItems);
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
