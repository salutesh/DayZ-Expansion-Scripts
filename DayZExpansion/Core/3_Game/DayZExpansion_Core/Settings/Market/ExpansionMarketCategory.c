/**
 * ExpansionMarketCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketCategory
{
	int m_Version = 4;
	string m_FileName;

	int CategoryID;
	string DisplayName;

	autoptr array< ref ExpansionMarketItem > Items = new array< ref ExpansionMarketItem >;

	// ------------------------------------------------------------
	// ExpansionMarketCategory Constructor
	// ------------------------------------------------------------
	void ExpansionMarketCategory()
	{
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketCategory Load
	// ------------------------------------------------------------
	static ref ExpansionMarketCategory Load( string name )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[ExpansionMarketCategory] Loading settings file " + name );
		#endif

		ref ExpansionMarketCategory settings = new ExpansionMarketCategory;
		settings.m_FileName = name;

		FileSerializer file = new FileSerializer();

		if ( FileExist( EXPANSION_MARKET_FOLDER + name + ".bin" ) && file.Open( EXPANSION_MARKET_FOLDER + name + ".bin", FileMode.READ ) )
		{
			file.Read( settings.m_Version );

			// if we make any changes to how the data is stored, 
			// use the version to specify how we should modify the data
			
			file.Read( settings.CategoryID );
			file.Read( settings.DisplayName );

			if ( settings.m_Version >= 3 )
			{
				int count;
				file.Read( count );

				for ( int i = 0; i < count; i++ )
				{
					string clsName;
					ExpansionMarketCurrency minPrice;
					ExpansionMarketCurrency maxPrice;
					int minStock;
					int maxStock;
					int purchaseType;

					int attCount;
					TStringArray attachments = new TStringArray;

					file.Read( clsName );

					if ( settings.m_Version >= 4 )
					{
						file.Read( minPrice );
						file.Read( maxPrice );
					} else
					{
						float f_minPrice;
						float f_maxPrice;
						file.Read( f_minPrice );
						file.Read( f_maxPrice );

						minPrice = (ExpansionMarketCurrency) f_minPrice * 10.0;
						maxPrice = (ExpansionMarketCurrency) f_maxPrice * 10.0;
					}

					file.Read( minStock );
					file.Read( maxStock );
					file.Read( purchaseType );

					file.Read( attCount );
					for ( int j = 0; j < attCount; j++ )
					{
						string attClsName;
						file.Read( attClsName );
						attachments.Insert( attClsName );
					}

					settings.Items.Insert( new ExpansionMarketItem( settings.CategoryID, clsName, minPrice, maxPrice, minStock, maxStock, purchaseType, attachments ) );
				}
			} else if ( settings.m_Version == 1 )
			{
				file.Read( settings.Items );
			}

			file.Close();
			
			settings.Save();
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[ExpansionMarketCategory] Loaded settings file " + name );
			#endif
		} 
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[ExpansionMarketCategory] Failed to load settings file " + name );
			#endif
		}

		return settings;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	void Save()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionMarketCategory] Saving settings");
		#endif
		
		FileSerializer file = new FileSerializer();
		
		if ( file.Open( EXPANSION_MARKET_FOLDER + m_FileName + ".bin", FileMode.WRITE ) )
		{
			// always overwrite with the latest version.

			file.Write( m_Version );
			
			file.Write( CategoryID );
			file.Write( DisplayName );

			file.Write( Items.Count() );
			
			for ( int i = 0; i < Items.Count(); i++ )
			{
				file.Write( Items[i].ClassName );
				file.Write( Items[i].MinPriceThreshold );
				file.Write( Items[i].MaxPriceThreshold );
				file.Write( Items[i].MinStockThreshold );
				file.Write( Items[i].MaxStockThreshold );
				file.Write( Items[i].PurchaseType );

				array< string > atts = Items[i].SpawnAttachments;
				file.Write( atts.Count() );
				for ( int j = 0; j < atts.Count(); j++ )
				{
					file.Write( atts[j] );
				}
			}

			file.Close();

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[ExpansionMarketCategory] Saved settings");
			#endif
		} else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "[ExpansionMarketCategory] Failed to save settings file " + m_FileName );
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
	ref ExpansionMarketItem AddItem( string className, ExpansionMarketCurrency minPrice, ExpansionMarketCurrency maxPrice, int minStock, int maxStock, int purchaseType = 0, ref array< string > attachments = NULL )
	{
		className.ToLower();

		if ( minPrice <= 0 ) 
		{
			Error("[ExpansionMarketCategory] The minimum price must be higher than 0 for '" + className + "'");
			return NULL;
		}

		if ( minStock <= 0 ) 
		{
			Error("[ExpansionMarketCategory] The minimum stock must be higher than 0 for '" + className + "'");
			return NULL;
		}

		if ( minPrice > maxPrice ) 
		{
			Error("[ExpansionMarketCategory] The minimum price must be lower than the maximum price for '" + className + "'");
			return NULL;
		}

		if ( minStock >= maxStock ) 
		{
			Error("[ExpansionMarketCategory] The minimum stock must be lower than the maximum stock for '" + className + "'");
			return NULL;
		}

		ExpansionMarketItem item = new ExpansionMarketItem( CategoryID, className, minPrice, maxPrice, minStock, maxStock, purchaseType, attachments );

		Items.Insert( item );

		return item;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketItem AddStaticItem
	// 'className' the name of the item
	// 'minPrice' the lowest the item will sell at when it has reached 'maxStock'
	// 'maxPrice' the highest the item will sell at when it has reached 'maxStock'
	// ------------------------------------------------------------
	ref ExpansionMarketItem AddStaticItem( string className, ExpansionMarketCurrency staticPrice, int purchaseType = 0, ref array< string > attachments = NULL )
	{
		className.ToLower();

		if ( staticPrice <= 0 ) 
		{
			Error("[ExpansionMarketCategory] The static price must be higher than 0 for '" + className + "'");
			return NULL;
		}

		ExpansionMarketItem item = new ExpansionMarketItem( CategoryID, className, staticPrice, staticPrice, 0, 0, purchaseType, attachments );

		Items.Insert( item );

		return item;
	}

	ref ExpansionMarketItem GetItem( string clsName )
	{
		for ( int i = 0; i < Items.Count(); i++ )
		{
			if ( Items[i].ClassName == clsName )
			{
				return Items[i];
			}
		}

		return NULL;
	}
}