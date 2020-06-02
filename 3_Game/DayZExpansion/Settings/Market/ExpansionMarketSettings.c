/**
 * ExpansionMarketSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

typedef int ExpansionMarketCurrency;

/**@class		ExpansionMarketSettings
 * @brief		Market settings class
 **/
class ExpansionMarketSettings: ExpansionSettingBase
{
	autoptr array< vector > LandSpawnPositions;
	autoptr array< vector > AirSpawnPositions;
	autoptr array< vector > WaterSpawnPositions;

	autoptr array< ref ExpansionMarketNetworkCategory > NetworkCategories;

	[NonSerialized()]
	protected autoptr map< int, ref ExpansionMarketCategory > m_Categories;

	[NonSerialized()]
	protected autoptr array< ref ExpansionMarketTraderZone > m_TraderZones;

	[NonSerialized()]
	protected autoptr array< ref ExpansionMarketTrader > m_Traders;

	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionMarketSettings()
	{
		//TraderPrint("ExpansionMarketSettings::ExpansionMarketSettings - Start");
		
		LandSpawnPositions = new array< vector >;
		AirSpawnPositions = new array< vector >;
		WaterSpawnPositions = new array< vector >;

		NetworkCategories = new array< ref ExpansionMarketNetworkCategory >;

		m_Categories = new map< int, ref ExpansionMarketCategory >;

		m_TraderZones = new array< ref ExpansionMarketTraderZone >;
		m_Traders = new array< ref ExpansionMarketTrader >;
		
		//TraderPrint("ExpansionMarketSettings::ExpansionMarketSettings - End");
	}
	
	// ------------------------------------------------------------
	protected void LoadCategories()
	{
		//TraderPrint("ExpansionMarketSettings::LoadCategories - Start");
		
		if ( !FileExist( EXPANSION_MARKET_FOLDER ) )
		{
			//TraderPrint( "[EXPANSION MARKET] ERROR: Folder not found! Generating Market Categores");

			MakeDirectory( EXPANSION_MARKET_FOLDER );
			
			DefaultCategories();

			return;
		}

		array< string > files = FindFilesInLocation( EXPANSION_MARKET_FOLDER );

		ExpansionMarketCategory matchingCategory;
		
		for ( int i = 0; i < files.Count(); i++ )
		{
			string fileName;
			string fileType;
			int pos = files[i].IndexOf( "." );
			
			if ( pos > -1 )
			{
				fileName = files[i].Substring( 0, pos );

				ExpansionMarketCategory category = ExpansionMarketCategory.Load( fileName );

				if ( !m_Categories.Find( category.CategoryID, matchingCategory ) )
				{
					m_Categories.Set( category.CategoryID, category );

					NetworkCategories.Insert( new ExpansionMarketNetworkCategory( category ) );
				} else
				{
					//TraderPrint( "[EXPANSION MARKET] DUPLICATE: Category ID is already set elsewhere (" + files[i] + ")" );
				}
			}
		}
		
		//TraderPrint("ExpansionMarketSettings::LoadCategories - End");
	}

	// ------------------------------------------------------------
	protected void LoadTraderZones()
	{
		//TraderPrint("ExpansionMarketSettings::LoadTraderZones - Start");
		
		if ( !FileExist( EXPANSION_TRADER_ZONES_FOLDER ) )
		{
			//TraderPrint( "[EXPANSION MARKET] ERROR: Folder not found! Generating Trader Zones");

			MakeDirectory( EXPANSION_TRADER_ZONES_FOLDER );

			DefaultTraderZones();

			return;
		}

		array< string > files = FindFilesInLocation( EXPANSION_TRADER_ZONES_FOLDER );
		
		for ( int i = 0; i < files.Count(); i++ )
		{
			string fileName;
			string fileType;
			int pos = files[i].IndexOf( "." );
			
			if ( pos > -1 )
			{
				fileName = files[i].Substring( 0, pos );

				m_TraderZones.Insert( ExpansionMarketTraderZone.Load( fileName ) );
			}
		}
		
		//TraderPrint("ExpansionMarketSettings::LoadTraderZones - End");
	}

	// ------------------------------------------------------------
	protected void LoadTraders()
	{
		//TraderPrint("ExpansionMarketSettings::LoadTraders - Start");
		
		if ( !FileExist( EXPANSION_TRADER_FOLDER ) )
		{
			//TraderPrint( "[EXPANSION MARKET] ERROR: Folder not found! Generating Traders");

			MakeDirectory( EXPANSION_TRADER_FOLDER );

			DefaultTraders();

			return;
		}

		array< string > files = FindFilesInLocation( EXPANSION_TRADER_FOLDER );
		
		for ( int i = 0; i < files.Count(); i++ )
		{
			string fileName;
			string fileType;
			int pos = files[i].IndexOf( "." );
			
			if ( pos > -1 )
			{
				fileName = files[i].Substring( 0, pos );

				m_Traders.Insert( ExpansionMarketTrader.Load( fileName ) );
			}
		}
		
		//TraderPrint("ExpansionMarketSettings::LoadTraders - End");
	}	
	
	// ------------------------------------------------------------
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		//TraderPrint("ExpansionMarketSettings::HandleRPC - Start");
		
		ExpansionMarketSettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionMarketSettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;
		
		ExpansionSettings.SI_Market.Invoke();
		
		//TraderPrint("ExpansionMarketSettings::HandleRPC - End");
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
		//TraderPrint("ExpansionMarketSettings::Send - Start");
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionMarketSettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Market, true, identity );
		
		//TraderPrint("ExpansionMarketSettings::Send - End and return");
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionMarketSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal( ref ExpansionMarketSettings s )
	{		
		LandSpawnPositions.Clear();
		AirSpawnPositions.Clear();
		WaterSpawnPositions.Clear();

		LandSpawnPositions.Copy( s.LandSpawnPositions );
		AirSpawnPositions.Copy( s.AirSpawnPositions );
		WaterSpawnPositions.Copy( s.WaterSpawnPositions );

		for ( int i = 0; i < s.NetworkCategories.Count(); i++ )
		{
			ref ExpansionMarketCategory category = new ref ExpansionMarketCategory;
			category.DisplayName = s.NetworkCategories[i].Name;
			category.CategoryID = s.NetworkCategories[i].CategoryID;
			m_Categories.Insert( category.CategoryID, category );
		}
	}
	
	// ------------------------------------------------------------
	ExpansionMarketItem UpdateMarketItem_Client( ExpansionMarketNetworkItem networkItem )
	{
		//TraderPrint("ExpansionMarketSettings::UpdateMarketItem_Client - Start");

		string clsName = networkItem.ClassName;
		clsName.ToLower();

		ExpansionMarketItem item;
		if ( !GetItem( clsName, item ) )
		{
			item = new ExpansionMarketItem( networkItem.CategoryID, clsName, -1, -1, -1, -1, -1 );

			m_Categories.Get( item.CategoryID ).Items.Insert( item );
		}

		//TraderPrint("ExpansionMarketSettings::UpdateMarketItem_Client - End");

		return item;
	}
	
	// ------------------------------------------------------------
	private void AddDefaultCategory( ExpansionMarketCategory category )
	{
		category.Defaults();

		category.Save();

		m_Categories.Set( category.CategoryID, category );
		
		NetworkCategories.Insert( new ExpansionMarketNetworkCategory( category ) );
	}

	// ------------------------------------------------------------
	protected void DefaultCategories()
	{
		//TraderPrint("ExpansionMarketSettings::DefaultCategories - Start");
		
		AddDefaultCategory( new ExpansionMarketExchange );
		AddDefaultCategory( new ExpansionMarketAmmo );
		AddDefaultCategory( new ExpansionMarketAmmoBoxes );
		AddDefaultCategory( new ExpansionMarketArmbands );
		AddDefaultCategory( new ExpansionMarketAssaultRifles );
		AddDefaultCategory( new ExpansionMarketBackpacks );
		AddDefaultCategory( new ExpansionMarketBandanas );
		AddDefaultCategory( new ExpansionMarketBatteries );
		AddDefaultCategory( new ExpansionMarketBelts );
		AddDefaultCategory( new ExpansionMarketBlousesAndSuits );
		AddDefaultCategory( new ExpansionMarketBoats );
		AddDefaultCategory( new ExpansionMarketBootsAndShoes );
		AddDefaultCategory( new ExpansionMarketButtstocks );
		AddDefaultCategory( new ExpansionMarketCaps );
		AddDefaultCategory( new ExpansionMarketCars );
		AddDefaultCategory( new ExpansionMarketCoatsAndJackets );
		AddDefaultCategory( new ExpansionMarketContainers );
		AddDefaultCategory( new ExpansionMarketDrinks );
		AddDefaultCategory( new ExpansionMarketElectronics );
		AddDefaultCategory( new ExpansionMarketExplosives );
		AddDefaultCategory( new ExpansionMarketEyewear );
		AddDefaultCategory( new ExpansionMarketFish );
		AddDefaultCategory( new ExpansionMarketFishing );
		AddDefaultCategory( new ExpansionMarketFood );
		AddDefaultCategory( new ExpansionMarketGhillies );
		AddDefaultCategory( new ExpansionMarketGloves );
		AddDefaultCategory( new ExpansionMarketHandguards );
		AddDefaultCategory( new ExpansionMarketHatsAndHoods );
		AddDefaultCategory( new ExpansionMarketHelicopters );
		AddDefaultCategory( new ExpansionMarketHelmets );
		AddDefaultCategory( new ExpansionMarketKits );
		AddDefaultCategory( new ExpansionMarketLaunchers );
		AddDefaultCategory( new ExpansionMarketKnifes );
		AddDefaultCategory( new ExpansionMarketLights );
		AddDefaultCategory( new ExpansionMarketLiquids );
		AddDefaultCategory( new ExpansionMarketLocks );
		AddDefaultCategory( new ExpansionMarketMagazines );
		AddDefaultCategory( new ExpansionMarketMeat );
		AddDefaultCategory( new ExpansionMarketMasks );
		AddDefaultCategory( new ExpansionMarketMedical );
		AddDefaultCategory( new ExpansionMarketMelee );
		AddDefaultCategory( new ExpansionMarketMuzzles );
		AddDefaultCategory( new ExpansionMarketNavigation );
		AddDefaultCategory( new ExpansionMarketOptics );
		AddDefaultCategory( new ExpansionMarketPants );
		AddDefaultCategory( new ExpansionMarketRifles );
		AddDefaultCategory( new ExpansionMarketShirtsAndTShirts );
		AddDefaultCategory( new ExpansionMarketShotguns );
		AddDefaultCategory( new ExpansionMarketPistols );
		AddDefaultCategory( new ExpansionMarketSkirtsAndDresses );
		AddDefaultCategory( new ExpansionMarketSniperRifles );
		AddDefaultCategory( new ExpansionMarketSubmachineGuns );
		AddDefaultCategory( new ExpansionMarketSupplies );
		AddDefaultCategory( new ExpansionMarketSweatersAndHoodies );
		AddDefaultCategory( new ExpansionMarketTents );
		AddDefaultCategory( new ExpansionMarketTools );
		AddDefaultCategory( new ExpansionMarketVehicleParts );
		AddDefaultCategory( new ExpansionMarketVests );
		AddDefaultCategory( new ExpansionMarketVegetables );
		AddDefaultCategory( new ExpansionMarketSpraycans );
		AddDefaultCategory( new ExpansionMarketCrossbows );
		
		//TraderPrint("ExpansionMarketSettings::DefaultCategories - End");
	}

	// ------------------------------------------------------------
	protected void DefaultTraderZones()
	{
		//TraderPrint("ExpansionMarketSettings::DefaultTraderZones - Start");
		
		m_TraderZones.Insert( new ExpansionMarketWorldTraderZone );

		for ( int i = 0; i < m_TraderZones.Count(); i++ )
		{
			m_TraderZones[i].Defaults();
			m_TraderZones[i].Save();
		}
		
		//TraderPrint("ExpansionMarketSettings::DefaultTraderZones - End");
	}

	// ------------------------------------------------------------
	protected void DefaultTraders()
	{
		//TraderPrint("ExpansionMarketSettings::DefaultTraders - Start");
		
		// Male Traders
		m_Traders.Insert( new ExpansionMarketTraderDenis );
		m_Traders.Insert( new ExpansionMarketTraderMirek );
		m_Traders.Insert( new ExpansionMarketTraderBoris );
		m_Traders.Insert( new ExpansionMarketTraderCyril );
		m_Traders.Insert( new ExpansionMarketTraderElias );
		m_Traders.Insert( new ExpansionMarketTraderFrancis );
		m_Traders.Insert( new ExpansionMarketTraderGuo );
		m_Traders.Insert( new ExpansionMarketTraderHassan );
		m_Traders.Insert( new ExpansionMarketTraderIndar );
		m_Traders.Insert( new ExpansionMarketTraderJose );
		m_Traders.Insert( new ExpansionMarketTraderKaito );
		m_Traders.Insert( new ExpansionMarketTraderLewis );
		m_Traders.Insert( new ExpansionMarketTraderManua );
		m_Traders.Insert( new ExpansionMarketTraderNiki );
		m_Traders.Insert( new ExpansionMarketTraderOliver );
		m_Traders.Insert( new ExpansionMarketTraderPeter );
		m_Traders.Insert( new ExpansionMarketTraderQuinn );
		m_Traders.Insert( new ExpansionMarketTraderRolf );
		m_Traders.Insert( new ExpansionMarketTraderSeth );
		m_Traders.Insert( new ExpansionMarketTraderTaiki );

		// Female Traders
		m_Traders.Insert( new ExpansionMarketTraderLinda );
		m_Traders.Insert( new ExpansionMarketTraderMaria );
		m_Traders.Insert( new ExpansionMarketTraderFrida );
		m_Traders.Insert( new ExpansionMarketTraderGabi );
		m_Traders.Insert( new ExpansionMarketTraderHelga );
		m_Traders.Insert( new ExpansionMarketTraderIrena );
		m_Traders.Insert( new ExpansionMarketTraderJudy );
		m_Traders.Insert( new ExpansionMarketTraderKeiko );
		m_Traders.Insert( new ExpansionMarketTraderLina );
		m_Traders.Insert( new ExpansionMarketTraderNaomi );

		for ( int i = 0; i < m_Traders.Count(); i++ )
		{
			m_Traders[i].Defaults();
			m_Traders[i].Save();
		}
		
		//TraderPrint("ExpansionMarketSettings::DefaultTraders - End");
	}
	
	// ------------------------------------------------------------
	protected void DefaultTraderSpawnAreas()
	{
		//TraderPrint("ExpansionMarketSettings::DefaultTraderSpawnAreas - Start");
		
		LandSpawnPositions.Insert( "11885 140 12465" );
		LandSpawnPositions.Insert( "1150 6 2405" );

		AirSpawnPositions.Insert( "12173 140 12640" );
		AirSpawnPositions.Insert( "4977 9 2457" );

		WaterSpawnPositions.Insert( "1759 0 1994" );
		WaterSpawnPositions.Insert( "14347 0 13235" );

		//TraderPrint("ExpansionMarketSettings::DefaultTraderSpawnAreas - End");
	}
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	// ------------------------------------------------------------
	override bool Load()
	{
		return OnLoad();
	}
	
	// ------------------------------------------------------------
	override bool OnLoad()
	{
		//TraderPrint("ExpansionMarketSettings::Load - Start");

		m_IsLoaded = true;

		LoadCategories();
		LoadTraders();
		LoadTraderZones();

		if ( FileExist( EXPANSION_MARKET_SETTINGS ) )
		{
			JsonFileLoader<ExpansionMarketSettings>.JsonLoadFile( EXPANSION_MARKET_SETTINGS, this );
			
			return true;
		}
			
		DefaultTraderSpawnAreas();
		Save();
			
		//TraderPrint("ExpansionMarketSettings::Load - End");
		
		return false;
	}

	// ------------------------------------------------------------
	override bool Save()
	{
		return OnSave();
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		//TraderPrint("ExpansionMarketSettings::Save - Start");
		
		JsonFileLoader< ExpansionMarketSettings >.JsonSaveFile( EXPANSION_MARKET_SETTINGS, this );

		for ( int i = 0; i < m_Categories.Count(); i++ )
		{
			if ( m_Categories.GetElement( i ) )
				m_Categories.GetElement( i ).Save();
		}

		for ( int j = 0; j < m_TraderZones.Count(); j++ )
		{
			m_TraderZones[j].Save();
		}

		for ( int k = 0; k < m_Traders.Count(); k++ )
		{
			m_Traders[k].Save();
		}
		
		//TraderPrint("ExpansionMarketSettings::Save - End");

		return true;
	}

	// ------------------------------------------------------------
	bool GetItem( string clsName, out ExpansionMarketItem item )
	{
		//! TraderPrint("ExpansionMarketSettings::GetItem - Start");
		
		clsName.ToLower();

		//! TraderPrint( "ExpansionMarketSettings::GetItem - Given class name is is: " + clsName );
		//! TraderPrint( "ExpansionMarketSettings::GetItem - m_Categories count is: " + m_Categories.Count().ToString() );
		
		//! Print("_GetItem:: m_Categories count " + m_Categories.Count());
		//! Print("_GetItem:: clsName " + clsName);

		for ( int i = 0; i < m_Categories.Count(); i++ )
		{
			item = m_Categories.GetElement( i ).GetItem( clsName );
			//! Print("_GetItem:: item " + item);
			if ( item )
			{
				//! Print("_GetItem:: true");
				//! TraderPrint("ExpansionMarketSettings::GetItem - Return true!");
				return true;
			}
		}

		//! Print("_GetItem:: false");

		//! TraderPrint("ExpansionMarketSettings::GetItem - Return false!");
		return false;
	}

	// ------------------------------------------------------------
	bool ExistsItem( string clsName )
	{
		ExpansionMarketItem item;
		if ( GetItem( clsName, item ) )
			return true;

		return false;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionMarketCategory GetCategory
	// ------------------------------------------------------------
	ref map< int, ref ExpansionMarketCategory > GetCategories()
	{
		return m_Categories;
	}

	// ------------------------------------------------------------
	ref ExpansionMarketCategory GetCategory( int id )
	{
		return m_Categories.Get( id );
	}

	// ------------------------------------------------------------
	ref ExpansionMarketTraderZone GetTraderZoneByPosition( vector position )
	{
		//TraderPrint("ExpansionMarketSettings::GetTraderZoneByPosition - Start");
		
		if ( m_TraderZones.Count() != 0 )
		{
			for ( int i = 0; i < m_TraderZones.Count(); i++ )
			{
				ref ExpansionMarketTraderZone currentZone = ExpansionMarketTraderZone.Cast( m_TraderZones.Get( i ) );
				if ( vector.Distance( currentZone.Position, position ) <= currentZone.Radius )
				{
					//TraderPrint("ExpansionMarketSettings::GetTraderZoneByPosition - End - " + currentZone);
					return currentZone;
				}
			}
		}

		//TraderPrint("ExpansionMarketSettings::GetTraderZoneByPosition - End - Null");
		return NULL;
	}

	// ------------------------------------------------------------
	ref ExpansionMarketTrader GetMarketTrader( string name )
	{
		//TraderPrint( "GetMarketTrader " + name );
		for ( int i = 0; i < m_Traders.Count(); i++ )
		{
			if ( m_Traders[i].TraderName == name )
			{
				return m_Traders[i];
			}
		}
		return NULL;
	}

	// ------------------------------------------------------------
	void TraderPrint( void text )
	{
		//! Print( text );
	}
}