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

		category.CategoryID = m_CategoryIDs.Get(name);
		
		//! Make sure we have no duplicates
		array<ref ExpansionMarketItem> items = new array<ref ExpansionMarketItem>;
		foreach (ExpansionMarketItem currentItem : category.Items)
		{
			//! Make sure item classnames are lowercase
			currentItem.ClassName.ToLower();

			if (!category.CheckDuplicate(currentItem.ClassName))
				items.Insert(currentItem);
		}

		category.Items.Clear();

		foreach (ExpansionMarketItem item : items)
		{
			//! NOTE: ItemID is not serialized
			item.ItemID = ++ExpansionMarketItem.m_CurrentItemId;

			//! NOTE: CategoryID is not serialized for the item, so always assign it from containing category!
			item.CategoryID = category.CategoryID;

			//! Make sure attachment classnames are lowercase
			TStringArray attachments = item.SpawnAttachments;
			item.SpawnAttachments = new TStringArray;
			foreach (string attachment : attachments)
			{
				attachment.ToLower();
				//! Check if attachment is not same classname as parent to prevent infinite recursion (user error)
				if (attachment == item.ClassName)
					Error("[ExpansionMarketItem] Trying to add " + item.ClassName + " as attachment to itself!");
				else
					item.SpawnAttachments.Insert(attachment);
			}

			item.SanityCheckAndRepair();

			category.AddItemInternal( item );
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
		CategoryID = -1;
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
		className.ToLower();

		if (ExpansionGame.IsServerOrOffline() && CheckDuplicate(className))
			return NULL;

		ExpansionMarketItem item = new ExpansionMarketItem( CategoryID, className, minPrice, maxPrice, minStock, maxStock, attachments, variants, sellPricePercent, quantityPercent, itemID, attachmentIDs );

		AddItemInternal( item );

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
	
	bool CheckDuplicate(string className)
	{
		ExpansionMarketItem item;
		if (s_GlobalItems.Find(className, item))
		{
			ExpansionMarketCategory cat = GetExpansionSettings().GetMarket().GetCategory(item.CategoryID);
			string catInfo;
			if (cat)
				catInfo = cat.m_FileName + " (ID " + item.CategoryID + ")";
			else
				catInfo = "ID " + item.CategoryID;
			Error("Item " + className + " has already been added to category " + catInfo);
			return true;
		}

		return false;
	}

	void AddItemInternal(ExpansionMarketItem item, bool addToList = true)
	{
		item.Category = this;
		if (addToList)
			Items.Insert( item );
		m_Items.Insert(item.ClassName, item);
		m_ItemsByID.Insert(item.ItemID, item);
		s_GlobalItems.Insert(item.ClassName, item);
		s_GlobalItemsByID.Insert(item.ItemID, item);
		m_HasItems = true;
		m_Finalized = false;
	}
	
	//! Adds all variants to m_Items and finalizes the category for use
	void Finalize(bool addVariantsAndDefaultAttachments = true)
	{
		if (addVariantsAndDefaultAttachments)
		{
			foreach (ExpansionMarketItem item : Items)
			{
				item.AddDefaultAttachments();
				AddVariants(item);
			}
		}

		m_Finalized = true;

		CF_Log.Debug("Finalized category ID " + CategoryID + " (" + m_FileName + "), " + m_Items.Count() + " items");
	}
	
	void AddVariants(ExpansionMarketItem item, TIntArray variantIds = NULL, out int variantIdIdx = -1)
	{
		if (item.Variants.Count())
		{
			int variantId = -1;

			TStringArray variants = item.Variants;
			item.Variants = new TStringArray;
			foreach (string className : variants)
			{
				className.ToLower();
				ExpansionMarketItem variant;
				if (!m_Items.Find(className, variant))
				{
					if (ExpansionGame.IsServerOrOffline() && CheckDuplicate(className))
						continue;

					if (variantIds)
						variantId = variantIds[variantIdIdx];
					variant = new ExpansionMarketItem( CategoryID, className, item.MinPriceThreshold, item.MaxPriceThreshold, item.MinStockThreshold, item.MaxStockThreshold, item.SpawnAttachments, NULL, item.SellPricePercent, item.QuantityPercent, variantId, item.m_AttachmentIDs );
					//! Variants that do not already have an entry only need to synch stock, they will be automatically added on client
					variant.m_StockOnly = true;
					AddItemInternal(variant, false);

					CF_Log.Debug("Added variant " + className + " (ID " + variant.ItemID + ", idx " + variantIdIdx + ")");

					if (variantIds)
						variantIdIdx++;
				}
				else
				{
					CF_Log.Debug("Setting variant " + className);
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
			Error("[WARNING] GetItem called on unfinalized category ID " + CategoryID + " (" + m_FileName + "), " + m_Items.Count() + " items");
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