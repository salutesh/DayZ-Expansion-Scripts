/**
 * ExpansionMarketCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCategory
{
	static const int VERSION = 12;

	static int s_CurrentCategoryId;

	int m_Version;

	protected static ref map<string, int> m_CategoryIDs = new map<string, int>;
	protected static ref map<string, ref ExpansionMarketItem> s_GlobalItems = new map<string, ref ExpansionMarketItem>;
	protected static ref map<int, ref ExpansionMarketItem> s_GlobalItemsByID = new map<int, ref ExpansionMarketItem>;

	[NonSerialized()]
	string m_FileName;

	[NonSerialized()]
	int CategoryID;

	string DisplayName;
	string Icon;
	string Color;
	bool IsExchange;
	float InitStockPercent;

	//! NOTE: Normally we don't want to iterate over this, use m_Items instead (contains all variants)
	autoptr array< ref ExpansionMarketItem > Items = new array< ref ExpansionMarketItem >;

	//! ClassName -> Item
	[NonSerialized()]
	ref map<string, ref ExpansionMarketItem> m_Items = new map<string, ref ExpansionMarketItem>;

	//! ItemID -> Item
	[NonSerialized()]
	ref map<int, ref ExpansionMarketItem> m_ItemsByID = new map<int, ref ExpansionMarketItem>;

	[NonSerialized()]
	bool m_HasItems;

	[NonSerialized()]
	bool m_Finalized;

	[NonSerialized()]
	int m_Idx;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketCategory Load
	// ------------------------------------------------------------
	static ExpansionMarketCategory Load(string name)
	{
		ExpansionMarketCategory categoryDefault = new ExpansionMarketCategory;
		categoryDefault.Defaults();

		ExpansionMarketCategory category = new ExpansionMarketCategory;
		if (!ExpansionJsonFileParser<ExpansionMarketCategory>.Load( EXPANSION_MARKET_FOLDER + name + ".json", category ))
			return NULL;

		category.m_FileName = name;

		if (category.m_Version < VERSION)
		{
			EXPrint("[ExpansionMarketCategory] Load - Converting v" + category.m_Version + " \"" + name + ".json\" to v" + VERSION);

			if (category.m_Version < 5)
			{
				category.Icon = categoryDefault.Icon;
				category.Color = categoryDefault.Color;
			}

			if (category.m_Version < 7)
			{
				foreach (ExpansionMarketItem itemV6OrLower : category.Items)
				{
					if (!itemV6OrLower.SellPricePercent)
						itemV6OrLower.SellPricePercent = -1;
				}
			}

			if (category.m_Version < 8)
				category.InitStockPercent = categoryDefault.InitStockPercent;

			if (category.m_Version < 9)
			{
				foreach (ExpansionMarketItem itemV8OrLower : category.Items)
				{
					if (!itemV8OrLower.QuantityPercent)
						itemV8OrLower.QuantityPercent = -1;
				}
			}

			if (category.m_Version < 12)
			{
				string fileNameLower = name;
				fileNameLower.ToLower();
				category.IsExchange = fileNameLower.IndexOf("exchange") == 0;
			}

			category.m_Version = VERSION;
			category.Save();
		}

		if (!m_CategoryIDs.Contains(name))
			m_CategoryIDs.Insert(name, m_CategoryIDs.Count() + 1);
		else
			EXError.Warn(null, "Category " + name + " has already been loaded");

		category.CategoryID = m_CategoryIDs.Get(name);
		
		//! Rebuild the items array so we can force classnames to lowercase, check for duplicates and do other sanity checks
		array<ref ExpansionMarketItem> items = new array<ref ExpansionMarketItem>;
		items.Reserve(category.Items.Count());
		ExpansionArray<ExpansionMarketItem>.RefCopy(category.Items, items);

		category.Items.Clear();

		foreach (ExpansionMarketItem item : items)
		{
			//! NOTE: ItemID is not serialized
			item.ItemID = ++ExpansionMarketItem.m_CurrentItemId;

			//! NOTE: CategoryID is not serialized for the item, so always assign it from containing category!
			item.CategoryID = category.CategoryID;

			item.SanityCheckAndRepair();

			category.AddItemInternal(item, true, false);
		}

		category.Finalize();

		return category;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save()
	{
		JsonFileLoader<ExpansionMarketCategory>.JsonSaveFile( EXPANSION_MARKET_FOLDER + m_FileName + ".json", this );
	}
	
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		m_Version = VERSION;
		CategoryID = ++s_CurrentCategoryId;
		DisplayName = "N/A";
		Icon = "Deliver";
		Color = "FBFCFEFF";
		InitStockPercent = 75;
		m_FileName = "INVALID-FILE-NAME";
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem AddItem
	// 'className' the name of the item
	// 'minPrice' the lowest the item will sell at when it has reached 'maxStock'
	// 'maxPrice' the highest the item will sell at when it has reached 'minStock'
	// ------------------------------------------------------------
	ExpansionMarketItem AddItem( string className, int minPrice, int maxPrice, int minStock, int maxStock, array< string > attachments = NULL, array< string > variants = NULL, float sellPricePercent = -1, int quantityPercent = -1, int itemID = -1, array<int> attachmentIDs = NULL )
	{
		ExpansionMarketItem item = new ExpansionMarketItem( CategoryID, className, minPrice, maxPrice, minStock, maxStock, attachments, variants, sellPricePercent, quantityPercent, itemID, attachmentIDs );

		if (!AddItemInternal( item ))
			return null;

		return item;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem AddStaticItem
	// 'className' the name of the item
	// 'minPrice' the lowest the item will sell at when it has reached 'maxStock'
	// 'maxPrice' the highest the item will sell at when it has reached 'maxStock'
	// ------------------------------------------------------------
	ExpansionMarketItem AddStaticItem( string className, int staticPrice, array< string > attachments = NULL, array< string > variants = NULL, float sellPricePercent = -1, int quantityPercent = -1 )
	{
		return AddItem( className, staticPrice, staticPrice, 1, 1, attachments, variants, sellPricePercent, quantityPercent );
	}
	
	bool CheckDuplicate(ExpansionMarketItem newItem, bool stackTraceOnDuplicate = true)
	{
		ExpansionMarketItem item;
		if (s_GlobalItems.Find(newItem.ClassName, item) || s_GlobalItemsByID.Find(newItem.ItemID, item))
		{
			string catInfo = GetItemCategoryInfo(item);

			string errorMsg = string.Format("Item %1 (ID %2) in category %3 (ID %4) was already added in category %5 as %6 (ID %7)", newItem.ClassName, newItem.ItemID, GetCategoryName(), CategoryID, catInfo, item.ClassName, item.ItemID);

			if (stackTraceOnDuplicate)
				EXError.MarketCfgError(this, errorMsg, true);
			else
				EXError.MarketCfgError(null, errorMsg);

			return true;
		}

		return false;
	}

	string GetCategoryName()
	{
		if (m_FileName)
			return m_FileName;

		return Widget.TranslateString(DisplayName);
	}

	string GetItemCategoryInfo(ExpansionMarketItem item)
	{
		ExpansionMarketCategory cat = GetExpansionSettings().GetMarket().GetCategory(item.CategoryID);
		string catInfo;

		if (cat)
			catInfo = cat.GetCategoryName() + " (ID " + item.CategoryID + ")";
		else if (item.CategoryID == CategoryID && m_ItemsByID.Contains(item.ItemID))
			catInfo = GetCategoryName() + " (ID " + CategoryID + ")";
		else
			catInfo = "ID " + item.CategoryID;

		return catInfo;
	}

	bool AddItemInternal(ExpansionMarketItem item, bool addToList = true, bool stackTraceOnDuplicate = true)
	{
		if (!item.ClassName.Trim())
		{
			EXError.MarketCfgError(null, "Item with empty ClassName in category " + GetCategoryName() + " (ID " + CategoryID + ")");
			return false;
		}

		if (CheckDuplicate(item, stackTraceOnDuplicate))
			return false;

		item.Category = this;
		if (addToList)
			Items.Insert( item );
		m_Items.Insert(item.ClassName, item);
		m_ItemsByID.Insert(item.ItemID, item);
		s_GlobalItems.Insert(item.ClassName, item);
		s_GlobalItemsByID.Insert(item.ItemID, item);
		m_HasItems = true;
		m_Finalized = false;
		
		CF_Log.Debug("ExpansionMarketCategory::AddItemInternal - Added item " + item.ClassName + " (ID " + item.ItemID + ") to category " + GetCategoryName() + " (ID " + CategoryID + ")");

		return true;
	}
	
	//! Adds all variants to m_Items and finalizes the category for use
	void Finalize(bool addVariants = true)
	{
		if (addVariants)
		{
			foreach (ExpansionMarketItem item : Items)
			{
			#ifdef EXPANSIONMODHARDLINE
				item.SetRarityAndRepReq();
			#endif
				AddVariants(item);
			}
		}

		m_Finalized = true;

		CF_Log.Debug("Finalized category " + GetCategoryName() + " (ID " + CategoryID + "), " + m_Items.Count() + " items");
	}
	
	static void AddDefaultAttachments()
	{
		foreach (ExpansionMarketItem item: s_GlobalItems)
		{
			item.AddDefaultAttachments();
		}
	}

	static void CreateStaticNetworkRepresentations()
	{
		foreach (ExpansionMarketItem item: s_GlobalItems)
		{
			item.CreateStaticNetworkRepresentation();
		}
	}

	//! @note variantIds and variantIdIdx are client-only
	void AddVariants(ExpansionMarketItem item, TIntArray variantIds = NULL, inout int variantIdIdx = -1)
	{
		if (item.Variants.Count())
		{
			int variantId = -1;
			string catInfo;
			string errorMsg;

			TStringArray variants = item.Variants;
			item.Variants = new TStringArray;
			foreach (string className : variants)
			{
			#ifdef DIAG_DEVELOPER
				CF_Log.Debug("Processing variant " + className);
			#endif

				if (g_Game.IsServer())
				{
					if (!className.Trim())
					{
						errorMsg = string.Format("Empty variant for item %1 (ID %2) in category %3 (ID %4)", item.ClassName, item.ItemID, GetCategoryName(), CategoryID);

						EXError.MarketCfgError(null, errorMsg);
						continue;
					}

					if (className == item.ClassName)
					{
						errorMsg = string.Format("Item %1 (ID %2) in category %3 (ID %4) can't be added as variant of itself", className, item.ItemID, GetCategoryName(), CategoryID);

						EXError.MarketCfgError(null, errorMsg);
						continue;
					}

					if (item.m_IsVariant)
					{
						catInfo = GetItemCategoryInfo(item.m_Parent);

						if (className == item.m_Parent.ClassName)
						{
							errorMsg = string.Format("Item %1 (ID %2) in category %3 can't be added as variant of %4 (ID %5) in category %6 (ID %7)\nbecause %4 is a variant of %1", className, item.m_Parent.ItemID, catInfo, item.ClassName, item.ItemID, GetCategoryName(), CategoryID);
						}
						else
						{
							errorMsg = string.Format("%1 can't be added as variant of %2 (ID %3) in category %4 (ID %5)\nbecause %2 is a variant of %6 (ID %7) in category %8", className, item.ClassName, item.ItemID, GetCategoryName(), CategoryID, item.m_Parent.ClassName, item.m_Parent.ItemID, catInfo);
						}

						EXError.MarketCfgError(null, errorMsg);
						continue;
					}
				}

				ExpansionMarketItem variant;
				if (!m_Items.Find(className, variant))
				{
					if (variantIds)
					{
						if (variantIdIdx < variantIds.Count())
						{
							variantId = variantIds[variantIdIdx];
						}
						else
						{
							errorMsg = string.Format("Index %1 for variant %2 of %3 (ID %4) in category %5 (ID %6) is greater than or equal to variants count %7", variantIdIdx, className, item.ClassName, item.ItemID, GetCategoryName(), CategoryID, variantIds.Count());

							Error(errorMsg);
							continue;
						}
					}

					variant = new ExpansionMarketItem( CategoryID, className, item.MinPriceThreshold, item.MaxPriceThreshold, item.MinStockThreshold, item.MaxStockThreshold, item.SpawnAttachments, NULL, item.SellPricePercent, item.QuantityPercent, variantId, item.m_AttachmentIDs );

					//! Variants that do not already have an entry only need to synch stock, they will be automatically added on client
					variant.m_StockOnly = true;
				#ifdef EXPANSIONMODHARDLINE
					variant.m_Rarity = item.m_Rarity;
					variant.m_RequiredRep = item.m_RequiredRep;
				#endif

					CF_Log.Debug("Adding variant " + className + " (ID " + variant.ItemID + ", idx " + variantIdIdx + ") for item " + item.ClassName + " (ID " + item.ItemID + ")");

					bool added = AddItemInternal(variant, false);

					if (variantIds)
						variantIdIdx++;

					if (!added)
						continue;
				}
				else
				{
					if (g_Game.IsServer() && variant.m_IsVariant)
					{
						catInfo = GetItemCategoryInfo(variant.m_Parent);

						errorMsg = string.Format("Variant %1 of %2 (ID %3) in category %4 (ID %5) was already added in category %6 as variant of %7 (ID %8)", className, item.ClassName, item.ItemID, GetCategoryName(), CategoryID, catInfo, variant.m_Parent.ClassName, variant.m_Parent.ItemID);

						EXError.MarketCfgError(null, errorMsg);
						continue;
					}

					//! 2nd and subsequent visits to same trader in this session will have stockonly variants still cached,
					//! so we need to increment the index
					if (variantIds && variant.m_StockOnly)
					{
					#ifdef DIAG_DEVELOPER
						CF_Log.Debug("Variant " + className + " is stockonly, incrementing index");
					#endif
						variantIdIdx++;
					}

					CF_Log.Debug("Setting variant " + className + " (ID " + variant.ItemID + ") for item " + item.ClassName + " (ID " + item.ItemID + ")");
				}
				
				variant.m_IsVariant = true;
				variant.m_Parent = item;
				item.Variants.Insert(className);
			}
		}
	}

	void SetAttachmentsFromIDs()
	{
		foreach (string className, ExpansionMarketItem item : m_Items)
		{
			if (item.m_AttachmentIDs)
				item.SetAttachmentsFromIDs();
		}
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem GetItem
	// ------------------------------------------------------------
	ExpansionMarketItem GetItem( string clsName, bool checkFinalized = true )
	{
		if (!m_HasItems)
			return NULL;

		if (checkFinalized)
			CheckFinalized();

		return m_Items.Get(clsName);
	}

	ExpansionMarketItem GetItem( int itemID, bool checkFinalized = true )
	{
		if (!m_HasItems)
			return NULL;

		if (checkFinalized)
			CheckFinalized();

		return m_ItemsByID.Get(itemID);
	}

	void CheckFinalized()
	{
		if (!m_Finalized)
			Error("[WARNING] GetItem called on unfinalized category " + GetCategoryName() + " (ID " + CategoryID + "), " + m_Items.Count() + " items");
	}

	static ExpansionMarketItem GetGlobalItem(string className, bool checkCategoryFinalized = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Profile(EXTrace.MARKET, ExpansionMarketCategory);
#endif

		ExpansionMarketItem item = s_GlobalItems[className];
		if (item && checkCategoryFinalized)
			item.Category.CheckFinalized();

		return item;
	}

	static ExpansionMarketItem GetGlobalItem(int itemID, bool checkCategoryFinalized = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Profile(EXTrace.MARKET, ExpansionMarketCategory);
#endif
		
		ExpansionMarketItem item = s_GlobalItemsByID[itemID];
		if (item && checkCategoryFinalized)
			item.Category.CheckFinalized();

		return item;
	}

	static void ClearGlobalItems()
	{
		s_GlobalItems.Clear();
		s_GlobalItemsByID.Clear();
	}

	void Copy(ExpansionMarketNetworkCategory cat)
	{
		DisplayName = cat.DisplayName;
		Icon = cat.Icon;
		Color = ExpansionColor.ARGBToHex(cat.Color);
		CategoryID = cat.CategoryID;
		IsExchange = cat.IsExchange;
	}
}