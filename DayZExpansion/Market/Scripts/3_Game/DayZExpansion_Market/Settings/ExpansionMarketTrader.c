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

#ifdef EXPANSIONMODHARDLINE
class ExpansionMarketTraderV9: ExpansionMarketTraderBase
{
	int MinRequiredHumanity;
	int MaxRequiredHumanity
}
#endif

class ExpansionMarketTrader : ExpansionMarketTraderBase
{
	static const int VERSION = 10;

	#ifdef EXPANSIONMODHARDLINE
	int MinRequiredReputation;
	int MaxRequiredReputation;
	#endif

	string TraderIcon;

	autoptr TStringArray Currencies;

	autoptr TStringArray Categories;
	
	ref map<string, ExpansionMarketTraderBuySell> Items;

	[NonSerialized()]
	ref map<int, ExpansionMarketTraderBuySell> m_Categories;

	[NonSerialized()]
	ref array<ref ExpansionMarketTraderItem> m_Items;

	//! Client only!
	[NonSerialized()]
	bool m_StockOnly;
	
	// ------------------------------------------------------------
	// ExpansionMarketTrader Constructor
	// ------------------------------------------------------------
	void ExpansionMarketTrader()
	{
		m_Version = VERSION;

		Items = new map<string, ExpansionMarketTraderBuySell>;
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
				
				#ifdef EXPANSIONMODHARDLINE
				settings.MinRequiredReputation = settings_v9.MinRequiredHumanity;
				settings.MaxRequiredReputation = settings_v9.MaxRequiredHumanity;
				#endif
			}
			
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
		
		#ifdef EXPANSIONMODHARDLINE
		MinRequiredReputation = 0;
		MaxRequiredReputation = int.MAX;
		#endif

		DefaultCurrencies();
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MARKET, this, "AddItem");
#endif
	
		item.ToLower();
		if (Items.Contains(item))
			return NULL;  //! Already added, possibly implicitly by adding a variant before the parent (which will add the parent first)

		ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem( item );
		if ( marketItem )
		{
			CF_Log.Debug("ExpansionMarketTrader::AddItem - Added item " + item + " to trader " + m_FileName + " items array");

			return AddItemInternal( marketItem, buySell );
		}

		CF_Log.Warn( "[ExpansionMarketTrader] Error: The \"" + item + "\" does not exist in the market!" );

		return NULL;
	}

	ExpansionMarketTraderItem AddItemInternal( ExpansionMarketItem marketItem, ExpansionMarketTraderBuySell buySell )
	{
		//! If this is a variant, check if parent is present, otherwise we need to add it first or it will fuck with network sync
		if (marketItem.m_IsVariant && marketItem.m_StockOnly && !Items.Contains(marketItem.m_Parent.ClassName))
			AddItemInternal( new ExpansionMarketTraderItem( marketItem.m_Parent, buySell ) );
		ExpansionMarketTraderItem item = new ExpansionMarketTraderItem( marketItem, buySell );
		AddItemInternal( item );
		return item;
	}

	void AddItemInternal( ExpansionMarketTraderItem item )
	{
		Items.Insert( item.MarketItem.ClassName, item.BuySell );

		//! Inserting ordered by ID ensures same order of IDs as given to items by categories (only required on server for correct netsynch)
		int count = m_Items.Count();
		int i;
		if (!count || GetGame().IsClient() || item.MarketItem.ItemID >= m_Items[count - 1].MarketItem.ItemID)
		{
			m_Items.Insert( item );
		}
		else
		{
			if (item.MarketItem.ItemID > m_Items[0].MarketItem.ItemID)
			{
				for (i = 1; i < count; i++)
				{
					if (item.MarketItem.ItemID <= m_Items[i].MarketItem.ItemID)
						break
				}
			}

			m_Items.InsertAt( item, i );
		}
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
		if (GetGame().IsServer())
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
					EXPrint("[ExpansionMarketTrader] Error: Category " + fileName + " does not exist!");
					continue;
				}

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
					EXPrint("[ExpansionMarketTrader] Error: Category " + id + " does not exist!");
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
		map<string, ExpansionMarketTraderBuySell> toAdd = new map<string, ExpansionMarketTraderBuySell>;

		foreach (ExpansionMarketTraderItem item : items)
		{
			foreach ( string attachment : item.MarketItem.SpawnAttachments )
			{
				if ( !Items.Contains( attachment ) && !toAdd.Contains( attachment ) )
					toAdd.Insert( attachment, ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly );
			}

			foreach ( string variant : item.MarketItem.Variants )
			{
				if ( !Items.Contains( variant ) && !toAdd.Contains( variant ) )
					toAdd.Insert( variant, item.BuySell );
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
