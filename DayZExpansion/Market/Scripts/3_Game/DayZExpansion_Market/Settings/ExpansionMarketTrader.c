/**
 * ExpansionMarketTrader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Whether the player can only buy, buy and sell, or only sell at this trader
enum ExpansionMarketTraderBuySell
{
	CanOnlyBuy = 0,
	CanBuyAndSell,
	CanOnlySell,
	CanBuyAndSellAsAttachmentOnly  //! Item should not be shown in menu, but can be sold/purchased as attachment on another item. For internal use only
}

class ExpansionMarketTraderBase
{
	int m_Version;

	[NonSerialized()]
	string m_FileName;

	string DisplayName;
}

class ExpansionMarketTraderV3 : ExpansionMarketTraderBase
{
	autoptr array< string > Items;
}

class ExpansionMarketTraderV9: ExpansionMarketTraderBase
{
	int MinRequiredHumanity;
	int MaxRequiredHumanity
}

class ExpansionMarketTrader : ExpansionMarketTraderBase
{
	static const int VERSION = 13;

	int MinRequiredReputation;
	int MaxRequiredReputation;

	string RequiredFaction;

	int RequiredCompletedQuestID;

	string TraderIcon;

	autoptr TStringArray Currencies;
	int DisplayCurrencyValue;
	string DisplayCurrencyName;

	bool UseCategoryOrder;

	autoptr TStringArray Categories;
	
	ref map<string, ExpansionMarketTraderBuySell> Items;

	[NonSerialized()]
	ref map<int, ExpansionMarketTraderBuySell> m_Categories;

	[NonSerialized()]
	ref TIntArray m_CategoryIDs = {};  //! Ordered

	[NonSerialized()]
	ref array<ref ExpansionMarketTraderItem> m_Items;

	//! Client only!
	[NonSerialized()]
	bool m_StockOnly;

	[NonSerialized()]
	int m_DisplayCurrencyPrecision;
	
	// ------------------------------------------------------------
	// ExpansionMarketTrader Constructor
	// ------------------------------------------------------------
	void ExpansionMarketTrader()
	{
		m_Version = VERSION;

		Items = new map<string, ExpansionMarketTraderBuySell>;
		Currencies = new TStringArray; 
		Categories = new TStringArray;
		m_Categories = new map<int, ExpansionMarketTraderBuySell>;
		m_Items = new array<ref ExpansionMarketTraderItem>;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketTrader Load
	// ------------------------------------------------------------	
	static ExpansionMarketTrader Load(string name)
	{
		ExpansionMarketTrader settingsDefault = new ExpansionMarketTrader;
		settingsDefault.Defaults();
		
		ExpansionMarketTrader settings = new ExpansionMarketTrader;
		
		ExpansionMarketTraderBase settingsBase;
		
		if (!ExpansionJsonFileParser<ExpansionMarketTraderBase>.Load( EXPANSION_TRADER_FOLDER + name + ".json", settingsBase ))
			return NULL;
		
		//! Automatically convert outdated trader files to current version
		if (settingsBase.m_Version < VERSION)
		{
			settings.DisplayName = settingsBase.DisplayName;

			EXPrint("ExpansionMarketTrader::Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_TRADER_FOLDER + name + "\" to v" + VERSION);

			if (settingsBase.m_Version == 3)
			{
				ExpansionMarketTraderV3 settings_v3;
		
				if (!ExpansionJsonFileParser<ExpansionMarketTraderV3>.Load( EXPANSION_TRADER_FOLDER + name + ".json", settings_v3 ))
					return NULL;

				foreach (string item : settings_v3.Items)
				{
					item.ToLower();
					settings.AddItem(item);
				}
			}
			else if (settingsBase.m_Version >= 4)
			{
				if (!ExpansionJsonFileParser<ExpansionMarketTrader>.Load( EXPANSION_TRADER_FOLDER + name + ".json", settings ))
					return NULL;
			}

			if (settingsBase.m_Version < 5)
				settings.DefaultCurrencies();

			if (settingsBase.m_Version < 6)
				settings.TraderIcon = settingsDefault.TraderIcon;

			if (settingsBase.m_Version < 10)
			{
				ExpansionMarketTraderV9 settings_v9;
				if (!ExpansionJsonFileParser<ExpansionMarketTraderV9>.Load( EXPANSION_TRADER_FOLDER + name + ".json", settings_v9 ))
					return NULL;
				
				settings.MinRequiredReputation = settings_v9.MinRequiredHumanity;
				settings.MaxRequiredReputation = settings_v9.MaxRequiredHumanity;
			}

			if (settingsBase.m_Version < 11)
			{	
				settings.RequiredFaction = settingsDefault.RequiredFaction;

				settings.RequiredCompletedQuestID = settingsDefault.RequiredCompletedQuestID;
			}
			
			if (settingsBase.m_Version < 12 && !settings.DisplayCurrencyValue)
				settings.DisplayCurrencyValue = settingsDefault.DisplayCurrencyValue;

			settings.m_Version = VERSION;
			settings.m_FileName = name;
			
			settings.Save();
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionMarketTrader>.Load( EXPANSION_TRADER_FOLDER + name + ".json", settings ))
				return NULL;
			settings.m_FileName = name;
		}
		
		if (settingsBase.m_Version >= 5)
		{
			//! Make sure currencies are lowercase
			settings.Currencies = ExpansionMarketSettings.StringArrayToLower(settings.Currencies);
		}
			
		if (settingsBase.m_Version >= 4)
		{
			//! Make sure item classnames are lowercase
			map<string, ExpansionMarketTraderBuySell> items = settings.Items;
			settings.Items = new map<string, ExpansionMarketTraderBuySell>;
			foreach (string className, ExpansionMarketTraderBuySell buySell : items)
			{
				className.ToLower();
				settings.AddItem(className, buySell);
			}
		}

		settings.m_DisplayCurrencyPrecision = ExpansionStatic.GetPrecision(settings.DisplayCurrencyValue);
		
		settings.Finalize();
		
		return settings;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save()
	{
		JsonFileLoader<ExpansionMarketTrader>.JsonSaveFile( EXPANSION_TRADER_FOLDER + m_FileName + ".json", this );
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		DisplayName = "NA";
		m_FileName = "INVALID-FILE-NAME";
		TraderIcon = "Trader";
		
		MinRequiredReputation = 0;
		MaxRequiredReputation = int.MAX;

		RequiredFaction = "";

		RequiredCompletedQuestID = -1;
		
		DefaultCurrencies();

		DisplayCurrencyValue = 1;
		DisplayCurrencyName = "";
	}

	void DefaultCurrencies()
	{
		Currencies = {"expansionbanknotehryvnia"};
	}

	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	ExpansionMarketTraderItem AddItem( string item, ExpansionMarketTraderBuySell buySell = ExpansionMarketTraderBuySell.CanBuyAndSell )
	{
		item.ToLower();
		if (Items.Contains(item))
			return NULL;  //! Already added, possibly implicitly by adding a variant before the parent (which will add the parent first)

		ExpansionMarketItem marketItem = ExpansionMarketCategory.GetGlobalItem( item );
		if ( marketItem )
		{
			return AddItemInternal( marketItem, buySell );
		}

		EXError.MarketCfgError(null, "Can't add " + item + " to trader " + m_FileName + " - item does not exist in market");

		return NULL;
	}

	ExpansionMarketTraderItem AddItemInternal( ExpansionMarketItem marketItem, ExpansionMarketTraderBuySell buySell )
	{
		ExpansionMarketTraderItem item = new ExpansionMarketTraderItem( marketItem, buySell );
		AddItemInternal( item );
		return item;
	}

	void AddItemInternal( ExpansionMarketTraderItem item )
	{
		if (Items.Contains(item.MarketItem.ClassName))
		{
			CF.FormatError("Item %1 has already been added to trader %2", item.MarketItem.ClassName, m_FileName);
			return;
		}

		Items.Insert( item.MarketItem.ClassName, item.BuySell );

		int count = m_Items.Count();
		int i;

		int id = item.MarketItem.ItemID;
		int lastID;
		if (count)
			lastID = m_Items[count - 1].MarketItem.ItemID;

		//! Inserting ordered by ID ensures same order of IDs as given to items by categories (only required on server for correct netsynch)
		if (!count || g_Game.IsClient() || id >= lastID)
		{
			m_Items.Insert( item );
		}
		else
		{
			int nextID = m_Items[0].MarketItem.ItemID;
			if (id > nextID)
			{
				for (i = 1; i < count; i++)
				{
					nextID = m_Items[i].MarketItem.ItemID;
					if (id <= nextID)
						break
				}
			}

			m_Items.InsertAt( item, i );
		}

		CF_Log.Debug("ExpansionMarketTrader::AddItemInternal - Added item " + item.MarketItem.ClassName + " (ID " + item.MarketItem.ItemID + ") to trader " + m_FileName + " items array at index " + i + " | count: " + m_Items.Count());
	}

	void AddCategory(string fileName, ExpansionMarketTraderBuySell buySell)
	{
		Categories.Insert(fileName + ":" + buySell);
	}

	//! Adds any missing items, variants and attachments
	void Finalize()
	{
		//! Add any missing items from categories
		ExpansionMarketCategory cat;
		if (g_Game.IsServer())
		{
			foreach (string fileName : Categories)
			{
				ExpansionMarketTraderBuySell catBuySell = ExpansionMarketTraderBuySell.CanBuyAndSell;

				TStringArray parts = new TStringArray;
				fileName.Split(":", parts);
				if (parts.Count() == 2)
				{
					fileName = parts[0];
					catBuySell = parts[1].ToInt();
				}

				cat = GetExpansionSettings().GetMarket().GetCategory(fileName);

				if (!cat)
				{
					EXError.Error(null, "TRADER CONFIGURATION ERROR: Category " + fileName + " in " + m_FileName + " does not exist!", {});
					continue;
				}

				if (!m_Categories.Contains(cat.CategoryID))
					m_CategoryIDs.Insert(cat.CategoryID);

				m_Categories.Insert(cat.CategoryID, catBuySell);

				AddCategoryItems(cat, catBuySell);
			}
		}
		else
		{
			foreach (int id, ExpansionMarketTraderBuySell buySell: m_Categories)
			{
				cat = GetExpansionSettings().GetMarket().GetCategory(id);

				if (!cat)
				{
					EXError.Error(null, "TRADER CONFIGURATION ERROR: Category " + id + " in " + m_FileName + " does not exist!", {});
					continue;
				}

				AddCategoryItems(cat, buySell);
			}
		}

		//! Add any missing variants and attachments
		AddAttachmentsAndVariants(m_Items);
	}

	protected void AddCategoryItems(ExpansionMarketCategory cat, ExpansionMarketTraderBuySell buySell)
	{
		foreach (ExpansionMarketItem marketItem : cat.Items)
		{
			if (!Items.Contains(marketItem.ClassName))
			{
				AddItemInternal(marketItem, buySell);
			}
		}
	}

	void AddAttachmentsAndVariants(array<ref ExpansionMarketTraderItem> items)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MARKET, this, m_FileName);
	#endif

		//! @note can't add to m_Items directly because it would alter the array during iteration...

		map<string, ExpansionMarketTraderBuySell> toAdd = new map<string, ExpansionMarketTraderBuySell>;

		foreach (ExpansionMarketTraderItem item : items)
		{
			ExpansionMarketItem marketItem = item.MarketItem;

			foreach ( string attachment : marketItem.SpawnAttachments )
			{
				if ( !Items.Contains( attachment ) && !toAdd.Contains( attachment ) )
				{
				#ifdef DIAG_DEVELOPER
					CF_Log.Debug("ExpansionMarketTrader::AddAttachmentsAndVariants - Adding attachment " + attachment + " to trader " + m_FileName);
				#endif
					toAdd.Insert( attachment, ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly );
				}
			}

			if (marketItem.m_IsVariant && marketItem.m_Parent)
			{
				string parentClassName = marketItem.m_Parent.ClassName;

				if ( !Items.Contains( parentClassName ) && !toAdd.Contains( parentClassName ) )
				{
				#ifdef DIAG_DEVELOPER
					CF_Log.Debug("ExpansionMarketTrader::AddAttachmentsAndVariants - Adding parent " + parentClassName + " to trader " + m_FileName);
				#endif
					toAdd.Insert(parentClassName, item.BuySell);
				}
			}

			foreach ( string variant : marketItem.Variants )
			{
				if ( !Items.Contains( variant ) && !toAdd.Contains( variant ) )
				{
				#ifdef DIAG_DEVELOPER
					CF_Log.Debug("ExpansionMarketTrader::AddAttachmentsAndVariants - Adding variant " + variant + " to trader " + m_FileName);
				#endif
					toAdd.Insert( variant, item.BuySell );
				}
			}
		}

		array<ref ExpansionMarketTraderItem> addedItems = new array<ref ExpansionMarketTraderItem>;

		foreach (string className, ExpansionMarketTraderBuySell buySell : toAdd)
		{
			ExpansionMarketTraderItem addedItem = AddItem( className, buySell );
			if (addedItem)
				addedItems.Insert(addedItem);
		}

		//! We need to do this recursively, since attachments and variants can themselves have attachments and variants...
		if (addedItems.Count())
			AddAttachmentsAndVariants(addedItems);
	}

	// ------------------------------------------------------------
	// Expansion ItemExists
	// ------------------------------------------------------------
	bool ItemExists( string item )
	{
		item.ToLower();
		return Items.Contains( item );
	}
	
	//! Whether the player can sell this item to this specific trader
	bool CanSellItem( string item )
	{
		item.ToLower();
		return Items.Get( item ) != ExpansionMarketTraderBuySell.CanOnlyBuy;
	}
	
	//! Whether the player can buy this item at this specific trader
	bool CanBuyItem( string item )
	{
		item.ToLower();
		return Items.Get( item ) != ExpansionMarketTraderBuySell.CanOnlySell;
	}
	
	//! Whether this item can only be bought & sold as attachment on another item
	bool IsAttachmentBuySell( string item )
	{
		item.ToLower();
		return Items.Get( item ) == ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly;
	}
}

class ExpansionMarketTraderItem
{
	ExpansionMarketItem MarketItem;
	ExpansionMarketTraderBuySell BuySell;

	void ExpansionMarketTraderItem(ExpansionMarketItem marketItem, ExpansionMarketTraderBuySell buySell)
	{
		MarketItem = marketItem;
		BuySell = buySell;
	}
}
