/**
 * ExpansionMarketCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCategory
{
	static const int VERSION = 8;

	int m_Version;

	protected static ref map<string, int> m_CategoryIDs = new map<string, int>;
	protected static ref map<string, int> s_GlobalItems = new map<string, int>;

	[NonSerialized()]
	string m_FileName;

	[NonSerialized()]
	int CategoryID;

	string DisplayName;
	string Icon;
	string Color;
	float InitStockPercent;

	//! NOTE: Normally we don't want to iterate over this, use GetItems() instead (contains all variants)
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
	bool m_Idx;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketCategory Load
	// ------------------------------------------------------------
	static ExpansionMarketCategory Load(string name)
	{
		ExpansionMarketCategory category = new ExpansionMarketCategory;
		
		JsonFileLoader<ExpansionMarketCategory>.JsonLoadFile( EXPANSION_MARKET_FOLDER + name + ".json", category );

		category.m_FileName = name;

		if (category.m_Version < VERSION)
		{
			if (category.m_Version < 5)
			{
				category.Icon = "Deliver";
				category.Color = GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText;
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
				category.InitStockPercent = 75;

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
		CategoryID = -1;
		DisplayName = "N/A";
		m_FileName = "INVALID-FILE-NAME";
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem AddItem
	// 'className' the name of the item
	// 'minPrice' the lowest the item will sell at when it has reached 'maxStock'
	// 'maxPrice' the highest the item will sell at when it has reached 'minStock'
	// ------------------------------------------------------------
	ExpansionMarketItem AddItem( string className, ExpansionMarketCurrency minPrice, ExpansionMarketCurrency maxPrice, int minStock, int maxStock, array< string > attachments = NULL, array< string > variants = NULL, int sellPricePercent = -1, int itemID = -1, array<int> attachmentIDs = NULL )
	{
		className.ToLower();

		if (ExpansionGame.IsServerOrOffline() && CheckDuplicate(className))
			return NULL;

		ExpansionMarketItem item = new ExpansionMarketItem( CategoryID, className, minPrice, maxPrice, minStock, maxStock, attachments, variants, sellPricePercent, itemID, attachmentIDs );

		AddItemInternal( item );

		return item;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem AddStaticItem
	// 'className' the name of the item
	// 'minPrice' the lowest the item will sell at when it has reached 'maxStock'
	// 'maxPrice' the highest the item will sell at when it has reached 'maxStock'
	// ------------------------------------------------------------
	ExpansionMarketItem AddStaticItem( string className, ExpansionMarketCurrency staticPrice, array< string > attachments = NULL, array< string > variants = NULL, int sellPricePercent = -1 )
	{
		return AddItem( className, staticPrice, staticPrice, 1, 1, attachments, variants, sellPricePercent );
	}
	
	bool CheckDuplicate(string className)
	{
		int categoryID;
		if (s_GlobalItems.Find(className, categoryID))
		{
			ExpansionMarketCategory cat = GetExpansionSettings().GetMarket().GetCategory(categoryID);
			string catInfo;
			if (cat)
				catInfo = cat.m_FileName + " (ID " + categoryID + ")";
			else
				catInfo = "ID " + categoryID;
			Error("Item " + className + " has already been added to category " + catInfo);
			return true;
		}
		else
		{
			s_GlobalItems.Insert(className, CategoryID);
		}

		return false;
	}

	void AddItemInternal(ExpansionMarketItem item, bool addToList = true)
	{
		if (addToList)
			Items.Insert( item );
		m_Items.Insert(item.ClassName, item);
		m_ItemsByID.Insert(item.ItemID, item);
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

		#ifdef EXPANSIONEXPRINT
		EXPrint("Finalized category ID " + CategoryID + " (" + m_FileName + "), " + m_Items.Count() + " items");
		#endif
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
					variant = new ExpansionMarketItem( CategoryID, className, item.MinPriceThreshold, item.MaxPriceThreshold, item.MinStockThreshold, item.MaxStockThreshold, item.SpawnAttachments, NULL, item.SellPricePercent, variantId, item.m_AttachmentIDs );
					//! Variants that do not already have an entry only need to synch stock, they will be automatically added on client
					variant.m_StockOnly = true;
					AddItemInternal(variant, false);
					#ifdef EXPANSIONEXPRINT
					EXPrint("Added variant " + className + " (ID " + variant.ItemID + ", idx " + variantIdIdx + ")");
					#endif
					if (variantIds)
						variantIdIdx++;
				}
				#ifdef EXPANSIONEXPRINT
				else
				{
					EXPrint("Setting variant " + className);
				}
				#endif
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

		CheckFinalized(checkFinalized);

		return m_Items.Get(clsName);
	}

	ExpansionMarketItem GetItem( int itemID, bool checkFinalized = true )
	{
		if (!m_HasItems)
			return NULL;

		CheckFinalized(checkFinalized);

		return m_ItemsByID.Get(itemID);
	}

	protected void CheckFinalized(bool checkFinalized)
	{
		if (checkFinalized && !m_Finalized)
			Error("[WARNING] GetItem called on unfinalized category ID " + CategoryID + " (" + m_FileName + "), " + m_Items.Count() + " items");
	}

	array<ref ExpansionMarketItem> GetItems()
	{
		array<ref ExpansionMarketItem> items = new array< ref ExpansionMarketItem >;
		foreach (ExpansionMarketItem item : m_Items)
		{
			items.Insert(item);
		}
		return items;
	}

	array<string> GetClassNames()
	{
		return m_Items.GetKeyArray();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem IsMapEnoch
	// ------------------------------------------------------------
	bool IsMapEnoch()
	{
		string world_name = "default";

		if ( GetGame() )
			GetGame().GetWorldName(world_name);

		world_name.ToLower();
			
		if ( world_name == "enochgloom" ||  world_name == "enoch" )
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem IsMapChernarus
	// ------------------------------------------------------------
	bool IsMapChernarus()
	{
		string world_name = "default";

		if ( GetGame() )
			GetGame().GetWorldName(world_name);

		world_name.ToLower();
			
		if ( world_name == "chernarusplusgloom" ||  world_name == "chernarusplus" )
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem IsMapNamalsk
	// ------------------------------------------------------------
	bool IsMapNamalsk()
	{
		string world_name = "default";

		if ( GetGame() )
			GetGame().GetWorldName(world_name);

		world_name.ToLower();
			
		if ( world_name == "namalskgloom" ||  world_name == "namalsk" )
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem IsExchange
	// ------------------------------------------------------------
	bool IsExchange()
	{
		string fileNameLower = m_FileName;
		fileNameLower.ToLower();
		return fileNameLower.IndexOf("exchange") == 0;
	}

	void Copy(ExpansionMarketNetworkCategory cat)
	{
		DisplayName = cat.Name;
		Icon = cat.Icon;
		Color = ExpansionColor.ARGBToHex(cat.Color);
		CategoryID = cat.CategoryID;
		m_FileName = cat.m_FileName;
	}
}