/**
 * ExpansionMarketTrader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	string TraderName;
	string DisplayName;
}

class ExpansionMarketTraderV3 : ExpansionMarketTraderBase
{
	autoptr array< string > Items;
}

class ExpansionMarketTrader : ExpansionMarketTraderBase
{
	static const int VERSION = 7;

	string TraderIcon;

	autoptr TStringArray Currencies;

	autoptr TStringArray Categories;
	
	ref map<string, ExpansionMarketTraderBuySell> Items;

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
		
		JsonFileLoader<ExpansionMarketTraderBase>.JsonLoadFile( EXPANSION_TRADER_FOLDER + name + ".json", settingsBase );
		
		//! Automatically convert outdated trader files to current version
		if (settingsBase.m_Version < VERSION)
		{
			settings.TraderName = settingsBase.TraderName;
			settings.DisplayName = settingsBase.DisplayName;

			EXPrint("ExpansionMarketTrader::Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_TRADER_FOLDER + name + "\" to v" + VERSION);

			if (settingsBase.m_Version == 3)
			{
				ExpansionMarketTraderV3 settings_v3;
		
				JsonFileLoader<ExpansionMarketTraderV3>.JsonLoadFile( EXPANSION_TRADER_FOLDER + name + ".json", settings_v3 );

				foreach (string item : settings_v3.Items)
				{
					item.ToLower();
					settings.AddItem(item);
				}
			}
			else if (settingsBase.m_Version >= 4)
			{				
				JsonFileLoader<ExpansionMarketTrader>.JsonLoadFile( EXPANSION_TRADER_FOLDER + name + ".json", settings );
			}

			if (settingsBase.m_Version < 5)
				settings.DefaultCurrencies();

			if (settingsBase.m_Version < 6)
				settings.TraderIcon = settingsDefault.TraderIcon;

			settings.m_Version = VERSION;
			settings.m_FileName = name;
			
			settings.Save();
		}
		else
		{
			JsonFileLoader<ExpansionMarketTrader>.JsonLoadFile( EXPANSION_TRADER_FOLDER + name + ".json", settings );
			settings.m_FileName = name;
		}
		
		if (settingsBase.m_Version >= 5)
		{
			//! Make sure currencies are lowercase
			TStringArray currencies = settings.Currencies;
			settings.Currencies = new TStringArray;
			foreach (string currency : currencies)
			{
				currency.ToLower();
				settings.Currencies.Insert(currency);
			}
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
		TraderName = "NA";
		DisplayName = "NA";
		m_FileName = "INVALID-FILE-NAME";
		TraderIcon = "Questionmark";

		DefaultCurrencies();
	}

	void DefaultCurrencies()
	{
		Currencies = {"expansiongoldbar", "expansiongoldnugget", "expansionsilverbar", "expansionsilvernugget"};
	}

	// ------------------------------------------------------------
	// Expansion AddItem
	// ------------------------------------------------------------
	void AddItem( string item, ExpansionMarketTraderBuySell buySell = ExpansionMarketTraderBuySell.CanBuyAndSell )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketTrader::AddItem - Start");
		#endif
		
		item.ToLower();
		if (Items.Contains(item))
			return;  //! Already added, possibly implicitly by adding a variant before the parent (which will add the parent first)

		ExpansionMarketItem marketItem = GetExpansionSettings().GetMarket().GetItem( item );
		if ( marketItem )
		{
			AddItemInternal( marketItem, buySell );
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMarketTrader::AddItem - Added item " + item + " to trader " + TraderName + " items array");
			#endif

			return;
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[ExpansionMarketTrader] Error: The \"" + item + "\" does not exist in the market!" );
		#endif
	}

	void AddItemInternal( ExpansionMarketItem marketItem, ExpansionMarketTraderBuySell buySell )
	{
		//! If this is a variant, check if parent is present, otherwise we need to add it first or it will fuck with network sync
		if (marketItem.m_IsVariant && marketItem.m_StockOnly && !Items.Contains(marketItem.m_Parent.ClassName))
			AddItemInternal( new ExpansionMarketTraderItem( marketItem.m_Parent, buySell ) );
		AddItemInternal( new ExpansionMarketTraderItem( marketItem, buySell ) );
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

	//! Adds any missing items, variants and attachments
	void Finalize()
	{
		//! Add any missing items from categories
		foreach (string fileName : Categories)
		{
			ExpansionMarketCategory cat = GetExpansionSettings().GetMarket().GetCategory(fileName);

			if (!cat)
				continue;

			foreach (ExpansionMarketItem marketItem : cat.Items)
			{
				if (!Items.Contains(marketItem.ClassName))
				{
					AddItemInternal(marketItem, ExpansionMarketTraderBuySell.CanBuyAndSell);
				}
			}
		}

		//! Add any missing variants and attachments
		map<string, ExpansionMarketTraderBuySell> toAdd = new map<string, ExpansionMarketTraderBuySell>;

		foreach (ExpansionMarketTraderItem item : m_Items)
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

		foreach (string className, ExpansionMarketTraderBuySell buySell : toAdd)
		{
			AddItem( className, buySell );
		}
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

	// ------------------------------------------------------------
	// Expansion IsMapEnoch
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
	// Expansion IsMapChernarus
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
	// Expansion IsMapNamalsk
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
