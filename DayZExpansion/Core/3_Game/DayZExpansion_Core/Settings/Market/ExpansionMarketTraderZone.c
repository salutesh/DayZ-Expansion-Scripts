/**
 * ExpansionMarketTraderZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderZone
{
	int m_Version = 3;

	string m_FileName;

	string m_ZoneName;
	string m_DisplayName;

	vector Position;
	float Radius;

	ref map< string, int > Stock;
	ref map< string, int > ReservedStock;
	
	// ------------------------------------------------------------
	// ExpansionMarketTraderZone Constructor
	// ------------------------------------------------------------
	void ExpansionMarketTraderZone()
	{
		Stock = new map< string, int >;
		ReservedStock = new map< string, int >;
	}

	void DebugPrint()
	{
		//! Print( "DebugPrint Count: " + Stock.Count() );
		for ( int i = 0 ; i < Stock.Count(); i++ )
		{
			string clsName = Stock.GetKey( i );
			clsName.ToLower();
			
			//! Print( "Item " + clsName );
			//! Print( " Stock " + Stock.Get( clsName ) );
			//! Print( " ReservedStock " + ReservedStock.Get( clsName ) );
		}
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketTraderZone Load
	// ------------------------------------------------------------
	static ref ExpansionMarketTraderZone Load( string name )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[ExpansionMarketTraderZone] Loading settings file " + name );
		#endif

		ExpansionMarketTraderZone settings = new ExpansionMarketTraderZone;
		settings.m_FileName = name;

		FileSerializer file = new FileSerializer();

		string path = EXPANSION_TRADER_ZONES_FOLDER + name + ".bin";

		if ( FileExist( path ) && file.Open( path, FileMode.READ ) )
		{
			// if we make any changes to how the data is stored, 
			// use the version to specify how we should modify the data
			file.Read( settings.m_Version );

			int minversion = 1;
			int maxversion = 3;

			if ( settings.m_Version < minversion || settings.m_Version > maxversion )
			{
				file.Close();

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[" + settings + "] Failed to load settings file " + path + ": Invalid version; got " + settings.m_Version + ", expected [" + minversion + ", " + maxversion + "]" );
				#endif
				return settings;
			}

			file.Read( settings.m_ZoneName );
			file.Read( settings.m_DisplayName );
			file.Read( settings.Position );
			file.Read( settings.Radius );

			if ( settings.m_Version >= 3 )
			{
				int stockMapLength;
				file.Read( stockMapLength );

				for ( int i = 0; i < stockMapLength; i++ )
				{
					string stockClassName;
					int stockLevel;
					file.Read( stockClassName );
					file.Read( stockLevel );

					stockClassName.ToLower();

					settings.Stock.Insert( stockClassName, stockLevel );
					settings.ReservedStock.Insert( stockClassName, 0 );
				}
			} else
			{
				file.Read( settings.Stock );
			}

			file.Close();
			if ( settings.m_Version != maxversion )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "[" + settings + "] Old settings file, updating to newer version" );
				#endif

				settings.Save();
			}

			DebugPrint();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[" + settings + "] Loaded settings file " + path );
			#endif

			return settings;
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[" + settings + "] Failed to load settings file " + path + ": file not found");
		#endif

		return settings;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[" + this + "] Saving settings");
		#endif
		
		FileSerializer file = new FileSerializer();
		
		string path = EXPANSION_TRADER_ZONES_FOLDER + m_FileName + ".bin";
		
		if ( file.Open( path, FileMode.WRITE ) )
		{
			// always overwrite with the latest version.
			file.Write( m_Version );
			
			file.Write( m_ZoneName );
			file.Write( m_DisplayName );
			file.Write( Position );
			file.Write( Radius );

			file.Write( Stock.Count() );

			for ( int i = 0; i < Stock.Count(); i++ )
			{
				file.Write( Stock.GetKey( i ) );
				file.Write( Stock.GetElement( i ) );
			}

			file.Close();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[" + this + "] Saved settings");
			#endif
		} 
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[" + this + "] Failed to save settings file " + m_FileName );
			#endif
		}
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[" + ClassName() + "] Loading default settings");
		#endif
		
		m_ZoneName = "NA";
		m_DisplayName = "NA";
		m_FileName = "INVALID-FILE-NAME";
	}
	
	// ------------------------------------------------------------
	// Expansion GetNetworkSerialization
	// ------------------------------------------------------------
	void GetNetworkSerialization( ExpansionMarketTrader trader, out array< ref ExpansionMarketNetworkItem > list )
	{
		if ( !list )
			list = new array< ref ExpansionMarketNetworkItem >;

		for ( int i = 0 ; i < Stock.Count(); i++ )
		{
			string className = Stock.GetKey( i );
			className.ToLower();

			if ( trader.ItemExists( className ) )
			{
				ExpansionMarketItem mItem;
				if ( !GetExpansionSettings().GetMarket().GetItem( className, mItem ) )
					continue;

				ExpansionMarketNetworkItem item = new ExpansionMarketNetworkItem;

				if ( mItem.StaticStock )
				{
					if ( Stock.Get( className ) == 1 )
					{
						item.Stock = -1; //! -1 is in stock
					} 
					else
					{
						item.Stock = -2; //! -2 is not in stock
					}
				} 
				else
				{
					item.Stock = Stock.Get( className );// - ReservedStock.Get( className );
				}

				item.CategoryID = mItem.CategoryID;
				item.ClassName = className;
				list.Insert( item );
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion SetStock
	// ------------------------------------------------------------
	void SetStock( string className, int stock )
	{
		className.ToLower();

		ExpansionMarketItem marketItem;
		if ( !GetExpansionSettings().GetMarket().GetItem( className, marketItem ) )
			return;

		if ( marketItem.StaticStock )
			stock = 1;

		//! Can't set stock higher than the max stock
		if ( stock > marketItem.MaxStockThreshold && !marketItem.StaticStock )
			stock = marketItem.MaxStockThreshold;
		
		if ( Stock.Contains( className ) )
		{
			Stock.Set( className, stock );
		} 
		else 
		{
			Stock.Insert( className, stock );
			ReservedStock.Insert( className, 0 );
		}
	}

	// ------------------------------------------------------------
	// Expansion ClearReservedStock
	// ------------------------------------------------------------
	void ClearReservedStock( string className, int reserved )
	{
		className.ToLower();

		ExpansionMarketItem marketItem;
		if ( !GetExpansionSettings().GetMarket().GetItem( className, marketItem ) )
			return;

		if ( !marketItem.StaticStock )
		{
			if ( ReservedStock.Contains( className ) )
			{
				int new_stock = ReservedStock.Get( className ) - reserved;

				ReservedStock.Set( className, new_stock );
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion AddStock
	// Adds the stock level for the item
	// ------------------------------------------------------------
	void AddStock( string className, int stock )
	{
		className.ToLower();

		ExpansionMarketItem marketItem;
		if ( !GetExpansionSettings().GetMarket().GetItem( className, marketItem ) )
			return;

		if ( marketItem.StaticStock )
			stock = 1;

		if ( Stock.Contains( className ) )
		{
			Stock.Set( className, Stock.Get( className ) + stock );
			// don't update reserved stock when adding to it
		} else 
		{
			Stock.Insert( className, stock );
			ReservedStock.Insert( className, 0 );
		}
	}

	// ------------------------------------------------------------
	// Expansion RemoveStock
	// Remove the stock level for the item
	// ------------------------------------------------------------
	void RemoveStock( string className, int stock, bool inReserve = false )
	{
		className.ToLower();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionMarketTraderZone] RemoveStock | " + m_ZoneName + " | " + className + " | " + stock);
		#endif

		ExpansionMarketItem marketItem;
		if ( !GetExpansionSettings().GetMarket().GetItem( className, marketItem ) )
			return;

		if ( Stock.Contains( className ) )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[ExpansionMarketTraderZone] RemoveStock contains " + className );
			#endif

			int new_stock = 0;

			if ( inReserve )
			{
				if ( !marketItem.StaticStock )
				{
					new_stock = ReservedStock.Get( className ) + stock;
					ReservedStock.Set( className, new_stock );
				}

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("[ExpansionMarketTraderZone] RemoveStock reserved set " + className + " new_stock : " + new_stock);
				#endif
			} 
			else
			{
				if ( !marketItem.StaticStock )
				{
					new_stock = Stock.Get( className ) - stock;
					
					if ( new_stock < 0 )
						new_stock = 0;
				}

				Stock.Set( className, new_stock );

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("[ExpansionMarketTraderZone] RemoveStock set " + className + " new_stock : " + new_stock);
				#endif
			}
		} 
		else 
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[ExpansionMarketTraderZone] RemoveStock does not contain " + className);
			#endif

			Stock.Insert( className, 0 );
			ReservedStock.Insert( className, 0 );
		}
	}

	// ------------------------------------------------------------
	// Expansion ItemExists
	// ------------------------------------------------------------
	bool ItemExists( string className )
	{
		className.ToLower();
		
		return Stock.Contains( className );
	}

	// ------------------------------------------------------------
	// Expansion GetStock
	// Gets the stock of an item within a trading zone
	// ------------------------------------------------------------
	int GetStock( string className, bool actual = false )
	{
		className.ToLower();

		if ( !ItemExists( className ) )
			return -3; // -3 means the item does not exist in this trader zone

		int stock = Stock.Get( className );

		if ( !actual )
		{
			int reservedStock = 0;
			if ( !ReservedStock.Find( className, reservedStock ) )
			{
				ReservedStock.Insert( className, 0 );
			} 
			else
			{
				stock = stock - reservedStock;
			}

			if ( stock < 0 )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "WARNING: ReservedStock for " + className + " is " + reservedStock + " which brings the total stock to " + stock );
				#endif
			}
		}

		return stock;
	}
}