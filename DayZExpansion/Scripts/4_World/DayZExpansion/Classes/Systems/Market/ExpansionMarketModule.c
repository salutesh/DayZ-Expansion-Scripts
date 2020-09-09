/**
 * ExpansionMarketModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketModule: JMModuleBase
{	
	static ref ScriptInvoker SI_SetTraderInvoker = new ScriptInvoker();
	static ref ScriptInvoker SI_SelectedItemUpdatedInvoker = new ScriptInvoker();
	static ref ScriptInvoker SI_Callback = new ScriptInvoker();

	protected ref array< string >   m_LocalStringInventory;
	protected ref array< EntityAI > m_LocalEntityInventory;

	ref map< string, ExpansionMarketCurrency > m_MoneyTypes;
	ref array< string >	m_MoneyDenominations;

	protected ref array< ref ExpansionMarketNetworkItem > m_NetworkItems;
	protected ref ExpansionMarketTraderZone m_ClientMarketZone;
	
	protected ExpansionTraderBase m_OpenedClientTrader;
	
	// ------------------------------------------------------------
	// ExpansionMarketModule Constructor
	// ------------------------------------------------------------
	void ExpansionMarketModule()
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::ExpansionMarketModule - Start");
		#endif
		
		m_LocalStringInventory = new array< string >;
		m_LocalEntityInventory = new array< EntityAI >;

		m_MoneyTypes = new map< string, ExpansionMarketCurrency >;
		m_MoneyDenominations = new array< string >;

		m_NetworkItems = new array< ref ExpansionMarketNetworkItem >;

		m_ClientMarketZone = new ExpansionMarketClientTraderZone;
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::ExpansionMarketModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::OnMissionLoaded - Start");
		#endif
		
		super.OnMissionLoaded();

		LoadMoneyPrice();
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::OnMissionLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{	
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::OnMissionFinish - Start");
		#endif
		
		super.OnMissionFinish();

		m_MoneyTypes.Clear();
		m_MoneyDenominations.Clear();
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::OnMissionFinish - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetClientZone
	// ------------------------------------------------------------
	ref ExpansionMarketTraderZone GetClientZone()
	{
		return m_ClientMarketZone;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketCurrency GetMoneyPrice
	// ------------------------------------------------------------
	ExpansionMarketCurrency GetMoneyPrice( string type )
	{
		return m_MoneyTypes.Get( type );
	}
	
	// ------------------------------------------------------------
	// Expansion String GetMoneyType
	// ------------------------------------------------------------
	string GetMoneyType( int index )
	{
		return m_MoneyDenominations.Get( index );
	}

	// ------------------------------------------------------------
	// Expansion LoadMoneyPrice
	// ------------------------------------------------------------
	void LoadMoneyPrice()
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::LoadMoneyPrice - Start");
		#endif
		
		if ( !GetExpansionSettings() )
			return;

		ref ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		if ( !market )
			return;

		//! Id 0 is always the exchange
		ExpansionMarketCategory category = market.GetCategory( 0 ); 
		if ( !category )
			return;

		//! Loop through all the items in this category to get the different price denominations
		for ( int i = 0; i < category.Items.Count(); i++ )
		{
			ExpansionMarketCurrency worth = category.Items[i].StaticPrice;
			string name = category.Items[i].ClassName;
			name.ToLower();

			m_MoneyTypes.Insert( name, worth );
			m_MoneyDenominations.Insert( name );
		}

		for ( i = 0; i < m_MoneyDenominations.Count() - 1; i++ ) 
		{
			int min_idx = i;
			for ( int j = i + 1; j < m_MoneyDenominations.Count(); j++ ) 
			{
				ExpansionMarketCurrency jMoney = GetMoneyPrice( m_MoneyDenominations[ j ] );
				ExpansionMarketCurrency minIndexMoney = GetMoneyPrice( m_MoneyDenominations[ min_idx ] );
				if ( jMoney < minIndexMoney )
				{
					min_idx = j;
				}
			}

			m_MoneyDenominations.SwapItems( min_idx, i );
		}

		m_MoneyDenominations.Invert();
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::LoadMoneyPrice - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTrader
	// Client only
	// ------------------------------------------------------------
	void SetTrader( ExpansionTraderBase trader )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SetTrader - Start");
		#endif
		
		m_OpenedClientTrader = trader;
		SI_SetTraderInvoker.Invoke( trader );
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SetTrader - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetTrader
	// ------------------------------------------------------------
	ExpansionTraderBase GetTrader()
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::GetTrader - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::GetTrader - End and return!");
			#endif
			return m_OpenedClientTrader;
		}

		Error( "ExpansionMarketModule::GetTrader - Invalid operation" );
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::GetTrader - [ERROR]: End and NULL!");
		#endif
		
		return NULL;
	}
	
	// ------------------------------------------------------------
	// Expansion FindSellPrice
	// ------------------------------------------------------------
	bool FindSellPrice( notnull PlayerBase player, ExpansionMarketItem item, int stock, int amountWanted, out ExpansionMarketSell sell )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::FindSellPrice - Start");
		#endif
		
		if ( !player )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindSellPrice - [ERROR]: Player is NULL!");
			#endif
			return false;
		}
		
		if ( !item )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindSellPrice - [ERROR]: ExpansionMarketItem is NULL!");
			#endif
			return false;
		}
		
		if ( sell == NULL )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindSellPrice - [ERROR]: ExpansionMarketSell is NULL! GetMarketSell!");
			#endif
			sell = player.GetMarketSell();
		}

		array< EntityAI > items = new array< EntityAI >;
	   	player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		Magazine mag;

		int increaseStockBy = 0;

		for ( int i = 0; i < items.Count(); i++ )
		{
			EntityAI itementity = EntityAI.Cast( items[i] );
			string itemClassName = itementity.GetType();
			
			itemClassName.ToLower();
			
			if ( itemClassName == item.ClassName )
			{
				ItemBase itembase = ItemBase.Cast( items[i] );
				int quantityTaken;
				int quantityRemaining;
				bool canSell = true;

				if ( itembase.GetInventory() )
				{					
					if ( itembase.GetInventory().AttachmentCount() != 0 )
					{
						canSell = false;
					}

					if ( itembase.GetInventory().GetCargo() && itembase.GetInventory().GetCargo().GetItemCount() != 0 )
					{
						canSell = false;
					}
				}

				if ( canSell )
				{
					if ( itembase ) 
					{
						if ( IsLiquidItem( itembase ) || itembase.IsFood() || itembase.HasEnergyManager() )
						{
							quantityTaken = 1;
							quantityRemaining = 0;
						} 
						else if ( Class.CastTo( mag, itembase ) )
						{
							if ( itembase.IsKindOf( "Ammunition_Base" ) )
							{
								quantityTaken = mag.GetAmmoCount();
								quantityRemaining = 0;
							} 
							else
							{
								if ( mag.GetAmmoCount() > 0 )
								{
									canSell = false;
								} 
								else
								{
									quantityTaken = 1;
									quantityRemaining = 0;
								}
							}
						} 
						else if ( itembase.HasQuantity() )
						{
							quantityTaken = itembase.GetQuantity();
							quantityRemaining = 0;
						} 
						else
						{
							quantityTaken = 1;
							quantityRemaining = 0;
						}
					} 
					else
					{
						quantityTaken = 1;
						quantityRemaining = 0;
					}
				}

				if ( canSell )
				{
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("ExpansionMarketModule::FindSellPrice - Can sell!");
					#endif
					if ( amountWanted - quantityTaken <= 0 )
					{
						quantityRemaining = quantityTaken - amountWanted;
						quantityTaken = amountWanted;
					}

					amountWanted -= quantityTaken;
					increaseStockBy += quantityTaken;

					sell.Price += item.CalculatePrice( stock + increaseStockBy );
				}

				sell.AddItem( canSell, quantityRemaining, quantityTaken, items[i] );

				if ( amountWanted <= 0 )
				{
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("ExpansionMarketModule::FindSellPrice - End and return true!");
					#endif
					return true;
				}
			}
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::FindSellPrice - End and return false!");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion Bool FindPurchasePriceAndReserve
	// ------------------------------------------------------------
	bool FindPurchasePriceAndReserve( ExpansionMarketItem item, ExpansionMarketTraderZone zone, int amountWanted, out ExpansionMarketReserve reserved )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::FindPurchasePriceAndReserve - Start");
		#endif
				
		if ( !item )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindSellPrice - [ERROR]: ExpansionMarketItem is NULL!");
			#endif
			return false;
		}
		
		if ( !zone )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindSellPrice - [ERROR]: ExpansionMarketTraderZone is NULL!");
			#endif
			return false;
		}
		
		ExpansionMarketCurrency price;
		if ( !GetPriceOfPurchase( item, zone, amountWanted, price ) )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::FindPurchasePriceAndReserve - ExpansionMarketItem " + item.ClassName + " has no purchase price! End and return false!");
			#endif
			return false;
		}

		reserved.RootItem = item;
		reserved.AddReserved( zone, item.ClassName, amountWanted, price );

		for ( int i = 0; i < item.SpawnAttachments.Count(); i++ )
		{
			ExpansionMarketItem attachment;
			if ( GetExpansionSettings().GetMarket().GetItem( item.SpawnAttachments[i], attachment ) )
			{
				if ( !GetPriceOfPurchase( attachment, zone, 1, price ) )
				{						
					reserved.ClearReserved( zone );
					
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("ExpansionMarketModule::FindPurchasePriceAndReserve - ExpansionMarketItem " + attachment.ClassName + " has no purchase price! End and return false!");
					#endif
					
					return false;
				}

				reserved.AddReserved( zone, attachment.ClassName, 1, price );
			}
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::FindPurchasePriceAndReserve - End and return true!");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion Bool GetPriceOfPurchase
	// ------------------------------------------------------------
	private bool GetPriceOfPurchase( ExpansionMarketItem item, ExpansionMarketTraderZone zone, int amountWanted, out ExpansionMarketCurrency price )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::GetPriceOfPurchase - Start");
		#endif
		
		if ( item.StaticStock )
		{
			price = amountWanted * item.CalculatePrice( 0 );
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::GetPriceOfPurchase - End and return true!");
			#endif
			
			return true;
		}

		int stock = zone.GetStock( item.ClassName );
		if ( amountWanted > stock )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::GetPriceOfPurchase - End and return false!");
			#endif
			return false;
		}

		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("amountWanted " + amountWanted);
		#endif

		for ( int i = 0; i < amountWanted; i++ )
		{
			price += item.CalculatePrice( stock - i );
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::GetPriceOfPurchase - End and return true!");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion Array<Object> Spawn
	// ------------------------------------------------------------
	array< Object > Spawn( ExpansionTraderBase trader, EntityAI parent, ExpansionMarketReserveItem reserveItem )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::Spawn - Start");
		#endif
		
		array< Object > objs = new array< Object >;

		ExpansionMarketItem item;
		if ( !GetExpansionSettings().GetMarket().GetItem( reserveItem.ClassName, item ) )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::Spawn - End and return objects: " + objs.ToString() );
			#endif
			
			return objs;
		}

		int quantity = reserveItem.Amount;
		while ( quantity > 0 )
		{
			vector position = "0 0 0";
			if ( item.PurchaseType == 1 )
			{
				trader.HasVehicleSpawnPosition( item.ClassName, position );
			}

			objs.Insert( Spawn( trader.GetTraderMarket(), item, parent, position, quantity ) );
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::Spawn - End and return objects: " + objs.ToString() );
		#endif

		return objs;
	}
	
	// ------------------------------------------------------------
	// Expansion Object Spawn
	// ------------------------------------------------------------
	Object Spawn( ExpansionMarketTrader trader, ExpansionMarketItem item, EntityAI parent, vector position, out int quantity )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::Spawn - Start");
		#endif
		
		switch ( item.PurchaseType )
		{
			case 0:
			{
				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::Spawn - End and case 0!");
				#endif
				return SpawnDefault( trader, item, parent, position, quantity );
			}
			case 1:
			{
				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::Spawn - End and case 1!");
				#endif
				return SpawnVehicle( trader, item, parent, position, quantity );
			}
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::Spawn - End and return NULL!");
		#endif
		
		return NULL;
	}
	
	// ------------------------------------------------------------
	// Expansion Object SpawnDefault
	// ------------------------------------------------------------
	private Object SpawnDefault( ExpansionMarketTrader trader, ExpansionMarketItem mItem, EntityAI parent, vector position, out int quantity )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnDefault - Start");
		#endif
		
		Object obj = parent.GetInventory().CreateInInventory( mItem.ClassName );

		ItemBase item;
		if ( !Class.CastTo( item, obj ) )
		{
			if ( obj )
			{
				GetGame().ObjectDelete( obj );
				
				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return NULL!");
				#endif
				
				return NULL;
			}

			obj = GetGame().CreateObject( mItem.ClassName, parent.GetPosition() );
			if ( !Class.CastTo( item, obj ) )
			{
				if ( obj )
				{
					GetGame().ObjectDelete( obj );
					
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return NULL!");
					#endif
					
					return NULL;
				}
			}
		}

		if ( IsLiquidItem( item ) || item.IsFood() || item.HasEnergyManager() )
		{
			quantity--;
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
			#endif
			
			return obj;
		}

		int max;
		
		Magazine mag;
		if ( Class.CastTo( mag, item ) )
		{
			if ( item.IsKindOf( "Ammunition_Base" ) )
			{
				max = mag.GetAmmoMax();
				if ( quantity < max )
				{
					quantity -= quantity;
					mag.ServerSetAmmoCount( quantity );

					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
					#endif
					
					return obj;
				}

				quantity -= max;
				mag.ServerSetAmmoCount( max );

				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
				#endif
				
				return obj;
			}

			quantity--;
			mag.ServerSetAmmoCount( 0 );

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
			#endif
			
			return obj;
		}

		float stackable = item.ConfigGetFloat( "varStackMax" );
		max = item.GetQuantityMax();

		if ( stackable > 0 || max > 0 )
		{
			if ( stackable > 0 )
				max = stackable;

			if ( quantity < max )
			{
				quantity -= quantity;
				item.SetQuantity( quantity );

				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
				#endif
				
				return obj;
			}
			else
			{
				quantity -= max;
				item.SetQuantity( max );
				
				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
				#endif
				
				return obj;
			}
		}

		quantity--;
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnDefault - End and return obj: " + obj.ToString() );
		#endif
		
		return obj;
	}
	
	// ------------------------------------------------------------
	// Expansion Object SpawnVehicle
	// ------------------------------------------------------------
	private Object SpawnVehicle( ExpansionMarketTrader trader, ExpansionMarketItem item, EntityAI parent, vector position, out int quantity )
	{	
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnVehicle - Start");
		#endif
		Object obj = GetGame().CreateObject( item.ClassName, position );

		CarScript vehicle;
		if ( !Class.CastTo( vehicle, obj ) )
		{
			if ( obj )
			{
				GetGame().ObjectDelete( obj );
			}
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("ExpansionMarketModule::SpawnVehicle - End return false!");
			#endif
			
			return NULL;
		}

		if ( vehicle.GetInventory() )
		{
			for ( int i = 0; i < item.SpawnAttachments.Count(); ++i )
			{
				vehicle.GetInventory().CreateAttachment( item.SpawnAttachments[i] );
			}
		}

		vehicle.Fill( CarFluid.FUEL, vehicle.GetFluidCapacity( CarFluid.FUEL ) );
		vehicle.Fill( CarFluid.OIL, vehicle.GetFluidCapacity( CarFluid.OIL ) );
		vehicle.Fill( CarFluid.BRAKE, vehicle.GetFluidCapacity( CarFluid.BRAKE ) );
		vehicle.Fill( CarFluid.COOLANT, vehicle.GetFluidCapacity( CarFluid.COOLANT ) );

		if ( vehicle.CanBeLocked() )
		{
			ExpansionCarKey key;
			if ( !Class.CastTo( key, parent.GetInventory().CreateInInventory( "ExpansionCarKey" ) ) )
			{
				Class.CastTo( key, GetGame().CreateObject( "ExpansionCarKey", parent.GetPosition() ) );
			}

			if ( key )
			{
				vehicle.PairKeyTo( key );
				vehicle.LockCar( key );
			}
		}

		quantity--;
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnVehicle - End return obj:" + obj.ToString());
		#endif
		
		return obj;
	}
	
	// ------------------------------------------------------------
	// Expansion MarketMessageGM
	// ------------------------------------------------------------
	void MarketMessageGM( string message )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::MarketMessageGM - Start");
		#endif
		
		GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( CCDirect, "", message, "" ) );
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::MarketMessageGM - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion Array<ExpansionMoneyBase> SpawnMoney
	// ------------------------------------------------------------
	array< ExpansionMoneyBase > SpawnMoney( PlayerBase player, ExpansionMarketCurrency amount )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnMoney - Start");
		#endif
		
		array< ExpansionMoneyBase > monies = new array< ExpansionMoneyBase >;

		ExpansionMarketCurrency remainingAmount = amount;
		ExpansionMarketCurrency minAmount = GetMoneyPrice( m_MoneyDenominations[ m_MoneyDenominations.Count() - 1 ] );

		for ( int currentDenomination = 0; currentDenomination < m_MoneyDenominations.Count(); currentDenomination++ )
		{
			string type = m_MoneyDenominations[ currentDenomination ];

			ExpansionMarketCurrency denomPrice = GetMoneyPrice( type );
			ExpansionMarketCurrency divAmount = remainingAmount / denomPrice;

			int toSpawn = Math.Floor( divAmount );

			ExpansionMarketCurrency amountSpawned = denomPrice * toSpawn;

			if ( divAmount < 1 )
				continue;

			remainingAmount -= amountSpawned;

			while ( toSpawn > 0 )
			{
				EntityAI obj = EntityAI.Cast( player.GetInventory().CreateInInventory( type ) );
				if ( !obj )
				{
					obj = EntityAI.Cast( GetGame().CreateObject( type, player.GetPosition() ) );
				}

				ExpansionMoneyBase money;
				if ( Class.CastTo( money, obj ) )
				{
					int max = money.GetQuantityMax();

					if ( toSpawn <= max )
					{
						money.SetQuantity( toSpawn );

						toSpawn = 0;
					} 
					else
					{
						money.SetQuantity( max );

						toSpawn -= max;
					}

					monies.Insert( money );
				} 
				else
				{
					//! Force this loop to end
					toSpawn = 0;
				}
			}

			if ( remainingAmount < minAmount )
				break;
		}
		
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnMoney - ExpansionMoneyBase: " + monies.ToString());
		#endif

		return monies;
	}
	
	// ------------------------------------------------------------
	// Expansion Bool FindMoneyAndCountTypes
	// ------------------------------------------------------------
	bool FindMoneyAndCountTypes( PlayerBase player, ExpansionMarketCurrency amount, out array< int > monies )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("ExpansionMarketModule::SpawnMoney - FindMoneyAndCountTypes: " + monies.ToString());
		#endif
		
		if ( !monies )
			monies = new array< int >;

		if ( !player )
			return false;

		array< EntityAI > items = new array< EntityAI >;
	   	player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		ref array< ref array< ExpansionMoneyBase > > foundMoney = new array< ref array< ExpansionMoneyBase > >;
		for ( int i = 0; i < m_MoneyDenominations.Count(); i++ )
		{
			foundMoney.Insert( new array< ExpansionMoneyBase > );
			monies.Insert( 0 );
		}

		for ( i = 0; i < items.Count(); i++ )
		{
			ExpansionMoneyBase money;
			if ( Class.CastTo( money, items[i] ) )
			{
				string type = money.GetType();
				type.ToLower();

				int idx = m_MoneyDenominations.Find( type );
				foundMoney[ idx ].Insert( money );
			}
		}

		ExpansionMarketCurrency foundAmount = 0;
		ExpansionMarketCurrency minAmount = GetMoneyPrice( m_MoneyDenominations[ m_MoneyDenominations.Count() - 1 ] );

		for ( int currentDenomination = 0; currentDenomination < foundMoney.Count(); currentDenomination++ )
		{
			ExpansionMarketCurrency denomPrice = GetMoneyPrice( m_MoneyDenominations[ currentDenomination ] );
			ExpansionMarketCurrency remainingAmount = amount - foundAmount;
			ExpansionMarketCurrency divAmount = remainingAmount / denomPrice;
			int toReserve = Math.Floor( divAmount );
			ExpansionMarketCurrency amountReserve = toReserve * denomPrice;

			int countCurrentDenom = foundMoney[currentDenomination].Count();
			int checkedCurrentDenom = 0;

			while ( toReserve > 0 )
			{
				if ( checkedCurrentDenom >= countCurrentDenom )
					break;

				int stack = foundMoney[currentDenomination][checkedCurrentDenom].GetQuantity();
				int number = 0;

				if ( stack >= toReserve )
				{
					number = toReserve;
				} 
				else
				{
					number = stack;
				}

				monies[currentDenomination] = monies[currentDenomination] + number;

				toReserve -= number;
				checkedCurrentDenom++;
			}

			foundAmount += amountReserve;

			if ( foundAmount > amount - minAmount )
			{
				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion Float GetPlayerWorth
	// ------------------------------------------------------------
	ExpansionMarketCurrency GetPlayerWorth( PlayerBase player, out array< int > monies )
	{
		int i = 0;
		ExpansionMarketCurrency worth = 0;

		if ( !monies )
			monies = new array< int >;

		for ( i = 0; i < m_MoneyDenominations.Count(); i++ )
			monies.Insert( 0 );

		if ( !player )
			return worth;

		array< EntityAI > items = new array< EntityAI >;
	   	player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		for ( i = 0; i < items.Count(); i++ )
		{
			ExpansionMoneyBase money;
			if ( Class.CastTo( money, items[i] ) )
			{
				string type = money.GetType();
				type.ToLower();
				int idx = m_MoneyDenominations.Find( type );
				monies[ idx ] = monies[ idx ] + money.GetQuantity();

				worth += GetMoneyPrice( type ) * money.GetQuantity();
			}
		}

		return worth;
	}
	
	// ------------------------------------------------------------
	// Expansion Bool FindMoneyAndReserve
	// ------------------------------------------------------------
	// will handle reserving partial of the stack for 1 transaction. to reserve the same stack for 
	// another transaction the previous one must be cancelled.
	bool FindMoneyAndReserve( PlayerBase player, ExpansionMarketCurrency amount, out array< ExpansionMoneyBase > monies )
	{
		if ( !monies )
			monies = new array< ExpansionMoneyBase >;

		if ( !player )
			return false;

		array< EntityAI > items = new array< EntityAI >;
	   	player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		array< ref array< ExpansionMoneyBase > > foundMoney = new array< ref array< ExpansionMoneyBase > >;
		for ( int i = 0; i < m_MoneyDenominations.Count(); i++ )
		{
			foundMoney.Insert( new array< ExpansionMoneyBase > );
		}

		for ( i = 0; i < items.Count(); i++ )
		{
			ExpansionMoneyBase money;
			if ( Class.CastTo( money, items[i] ) )
			{
				string type = money.GetType();
				type.ToLower();

				int idx = m_MoneyDenominations.Find( type );
				foundMoney[idx].Insert( money );
			}
		}

		ExpansionMarketCurrency foundAmount = 0;
		ExpansionMarketCurrency minAmount = GetMoneyPrice( m_MoneyDenominations[ m_MoneyDenominations.Count() - 1 ] );

		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("FindMoneyAndReserve foundMoney " + foundMoney.Count());
		#endif

		for ( int currentDenomination = 0; currentDenomination < foundMoney.Count(); currentDenomination++ )
		{
			ExpansionMarketCurrency denomPrice = GetMoneyPrice( m_MoneyDenominations[ currentDenomination ] );
			ExpansionMarketCurrency remainingAmount = amount - foundAmount;
			ExpansionMarketCurrency divAmount = remainingAmount / denomPrice;
			int toReserve = Math.Floor( divAmount );
			int didReserve = 0;

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			//! EXLogPrint( "FindMoneyAndReserve 1 denomPrice: " + (int)denomPrice );
			#endif

			int remainCurrentDenom = foundMoney[currentDenomination].Count();
			int checkedCurrentDenom = 0;

			while ( toReserve > 0 && checkedCurrentDenom < remainCurrentDenom )
			{
				ExpansionMoneyBase moneyObj = foundMoney[currentDenomination][checkedCurrentDenom];

				int quantity = moneyObj.GetQuantity();

				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("FindMoneyAndReserve quanity " + quantity);
				#endif

				#ifdef EXPANSION_MARKET_MODULE_DEBUG
				EXLogPrint("FindMoneyAndReserve toReserve " + toReserve);
				#endif

				if ( quantity >= toReserve )
				{
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("FindMoneyAndReserve 0 ");
					#endif
					
					moneyObj.ReservedAmount = quantity;

					toReserve -= quantity;
					didReserve += quantity;
				} else
				{
					#ifdef EXPANSION_MARKET_MODULE_DEBUG
					EXLogPrint("FindMoneyAndReserve 1 ");
					#endif

					moneyObj.ReservedAmount = toReserve;

					toReserve = 0;
					didReserve += toReserve;
				}

				moneyObj.IsReserved = true;

				monies.Insert( moneyObj );

				checkedCurrentDenom++;
			}

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint( "Callback_FailedNotEnoughMoney didReserve: " + didReserve );
			#endif

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			//! EXLogPrint( "Callback_FailedNotEnoughMoney denomPrice: " + (int)denomPrice );
			#endif

			foundAmount += didReserve * denomPrice;

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint( "Callback_FailedNotEnoughMoney amountReserve: " + didReserve * denomPrice );
			#endif
						
			/*#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint( "Callback_FailedNotEnoughMoney foundAmount: " + foundAmount );
			#endif

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint( "Callback_FailedNotEnoughMoney amount: " + amount );
			#endif

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint( "Callback_FailedNotEnoughMoney minAmount: " + minAmount );
			#endif*/

			if ( foundAmount > amount - minAmount )
			{
				return true;
			}
		}

		// dereserve all money found
		for ( i = 0; i < monies.Count(); i++ )
		{
			monies[i].ReservedAmount = 0;
			monies[i].IsReserved = false;
		}

		monies.Clear();

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion UnlockMoney
	// ------------------------------------------------------------
	void UnlockMoney( PlayerBase player )
	{
		if ( !player )
			return;

		array< EntityAI > items = new array< EntityAI >;
	   	player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );

		for ( int i = 0; i < items.Count(); i++ )
		{
			ExpansionMoneyBase money;
			if ( Class.CastTo( money, items[i] ) )
			{
				money.ReservedAmount = 0;
				money.IsReserved = false;
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveMoney
	// ------------------------------------------------------------
	void RemoveMoney( array< ExpansionMoneyBase > monies )
	{
		for ( int i = 0; i < monies.Count(); i++ )
		{
			int max = monies[i].GetQuantityMax();

			if ( monies[i].ReservedAmount < max )
			{
				monies[i].SetQuantity( max - monies[i].ReservedAmount );

				monies[i].ReservedAmount = 0;
				monies[i].IsReserved = false;
			} 
			else
			{
				GetGame().ObjectDelete( monies[i] );
			}
		}
	}

	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionMarketModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionMarketModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
			case ExpansionMarketModuleRPC.Callback:
			{
				RPC_Callback( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.MoneyDenominations:
			{
				RPC_MoneyDenominations( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.RequestPurchase:
			{
				RPC_RequestPurchase( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.ConfirmPurchase:
			{
				RPC_ConfirmPurchase( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.CancelPurchase:
			{
				RPC_CancelPurchase( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.RequestSell:
			{
				RPC_RequestSell( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.ConfirmSell:
			{
				RPC_ConfirmSell( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.CancelSell:
			{
				RPC_CancelSell( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.RequestTraderData:
			{
				RPC_RequestTraderData( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.LoadTraderData:
			{
				RPC_LoadTraderData( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.RequestItemData:
			{
				RPC_RequestItemData( ctx, sender, target );
				break;
			}
			case ExpansionMarketModuleRPC.LoadItemData:
			{
				RPC_LoadItemData( ctx, sender, target );
				break;
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion RemoveReservedStock
	// ------------------------------------------------------------
	private void RemoveReservedStock( PlayerBase player, string itemClassName )
	{
		if ( !player )
			return;

		ExpansionMarketReserve reserve = player.GetMarketReserve();
		if ( reserve )
		{
			if ( player.IsMarketItemReserved( itemClassName ) )
			{
				UnlockMoney( player );
				
				reserve.ClearReserved( reserve.Trader.GetTraderZone() );
	
				Callback_FailedReserveTime( reserve.RootItem.ClassName, player.GetIdentity() );
	
				player.ClearMarketReserve();
			}
		}
	}

	/*
	 * Called Server Only: 
	*/
	
	// ------------------------------------------------------------
	// Expansion Callback_FailedUnknown
	// ------------------------------------------------------------
	void Callback_FailedUnknown( string itemClassName, PlayerIdentity playerIdent )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Failed: FailedUnknown");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( false );
			rpc.Write( -1 );
			rpc.Send( NULL, ExpansionMarketModuleRPC.Callback, true, playerIdent );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Callback_Success
	// ------------------------------------------------------------
	void Callback_Success( string itemClassName, PlayerIdentity playerIdent )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Failed: Success");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( true );
			rpc.Send( NULL, ExpansionMarketModuleRPC.Callback, true, playerIdent );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Callback_FailedStockChange
	// ------------------------------------------------------------
	void Callback_FailedStockChange( string itemClassName, PlayerIdentity playerIdent )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Failed: FailedStockChange");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( false );
			rpc.Write( 1 );
			rpc.Send( NULL, ExpansionMarketModuleRPC.Callback, true, playerIdent );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Callback_FailedNotEnoughMoney
	// ------------------------------------------------------------
	void Callback_FailedNotEnoughMoney( string itemClassName, PlayerIdentity playerIdent )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Failed: FailedNotEnoughMoney");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( false );
			rpc.Write( 2 );
			rpc.Send( NULL, ExpansionMarketModuleRPC.Callback, true, playerIdent );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Callback_FailedReserveTime
	// ------------------------------------------------------------
	void Callback_FailedReserveTime( string itemClassName, PlayerIdentity playerIdent )
	{
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Failed: ReserveTime");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( false );
			rpc.Write( 3 );
			rpc.Send( NULL, ExpansionMarketModuleRPC.Callback, true, playerIdent );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Callback
	// ------------------------------------------------------------
	private void RPC_Callback( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		bool success;
		if ( !ctx.Read( success ) )
			return;

		if ( !success )
		{
			int type;

			if ( !ctx.Read( type ) )
				return;

			SI_Callback.Invoke( false, type );
		} 
		else
		{
			SI_Callback.Invoke( true, 0 );
		}
	}

	// -----------------------------------------------------------
	// Expansion OnInvokeConnect
	// -----------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		SendMoneyDenominations( identity );
	}
	
	// -----------------------------------------------------------
	// Expansion SendMoneyDenominations
	// -----------------------------------------------------------
	private void SendMoneyDenominations( PlayerIdentity identity )
	{
		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
			return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( m_MoneyTypes.GetKeyArray() );
		rpc.Write( m_MoneyTypes.GetValueArray() );
		rpc.Send( NULL, ExpansionMarketModuleRPC.MoneyDenominations, true, identity );
	}
	
	// -----------------------------------------------------------
	// Expansion RPC_MoneyDenominations
	// -----------------------------------------------------------
	private void RPC_MoneyDenominations( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		array< string > keys = new array< string >;
		array< ExpansionMarketCurrency > values = new array< ExpansionMarketCurrency >;

		if ( !ctx.Read( keys ) || !ctx.Read( values ) )
			return;

		int count = keys.Count();
		if ( count != values.Count() )
			return;

		m_MoneyTypes.Clear();
		m_MoneyDenominations.Clear();

		for ( int i = 0; i < count; i++ )
		{
			m_MoneyDenominations.Insert( keys[i] );
			m_MoneyTypes.Insert( keys[i], values[i] );
		}
	}

	/*
	 * Called Client Only: The client would send this RPC to request a purchase 
	 * to be made and to lock in the price it is at. This lock will last
	 * 30 seconds and on any new clients will show the new price as if
	 * the stock of those items were released.
	 */
	
	// ------------------------------------------------------------
	// Expansion RequestPurchase
	// Client only
	// ------------------------------------------------------------
	void RequestPurchase( string itemClassName, int count, ExpansionMarketCurrency currentPrice, ExpansionTraderBase trader, PlayerBase player = NULL )
	{
		if ( !trader )
			return;

		if ( IsMissionHost() )
		{
			if ( IsMissionOffline() )
			{
				player = PlayerBase.Cast( GetGame().GetPlayer() );
			}

			Exec_RequestPurchase( player, itemClassName, count, currentPrice, trader );
		} 
		else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( count );
			rpc.Write( currentPrice );
			rpc.Send( trader, ExpansionMarketModuleRPC.RequestPurchase, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RequestPurchase
	//	Server only
	// ------------------------------------------------------------
	private void Exec_RequestPurchase( notnull PlayerBase player, string itemClassName, int count, ExpansionMarketCurrency currentPrice, ExpansionTraderBase trader )
	{		
		if ( !player )
		{
			return;
		}
		
		if ( !trader )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 1");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}
		
		//! Get current market Trader Zone from given trader
		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		if ( !market )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 2");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		ExpansionMarketTraderZone zone = trader.GetTraderZone();
		if ( !zone )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 3");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		//! Afterwards calculate the price of the items at that stock		
		ExpansionMarketReserve reservedList = player.GetMarketReserve();
		reservedList.Trader = trader;
		
		if ( !market.GetItem( itemClassName, reservedList.RootItem ) /* || reservedList.Valid*/)
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 4 itemClassName " + itemClassName);
			#endif

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 4");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		//! Compare that price to the one the player sent
		//! Issue with reservedList.Price, look like reservedList.Price is doing += price somewhere, aka price increases by its own value when ever you press buy
		if ( !FindPurchasePriceAndReserve( reservedList.RootItem, zone, count, reservedList ) /*|| reservedList.Price != currentPrice*/ )
		{
			/*
			if (!FindPurchasePriceAndReserve( reservedList.RootItem, zone, count, reservedList ))
			{
				EXLogPrint("Callback_FailedStockChange FindPurchasePriceAndReserve");
			}	
			if ( reservedList.Price != currentPrice )
			{
				EXLogPrint("Callback_FailedStockChange reservedList");
			}

			EXLogPrint( "Callback_FailedStockChange CurrentPrice: " + currentPrice );
			EXLogPrint( "Callback_FailedStockChange ReservedList: " + reservedList.Price );
			*/

			reservedList.Debug();

			reservedList.ClearReserved( zone );
			player.ClearMarketReserve();

			// If they don't match, call Callback_FailedNotEnoughMoney
			Callback_FailedStockChange( itemClassName, player.GetIdentity() );

			/*
			EXLogPrint( "Callback_FailedStockChange Item: " + reservedList.RootItem );
			EXLogPrint( "Callback_FailedStockChange Zone: " + zone.ToString() );
			EXLogPrint( "Callback_FailedStockChange Count: " + count );

			EXLogPrint("Callback_FailedStockChange: 1");
			EXLogPrint("Callback_FailedStockChange: itemClassName " + itemClassName);
			*/

			//! EXLogPrint("Callback_FailedStockChange: player " + player);
			//! EXLogPrint("Callback_FailedStockChange: identity " + player.GetIdentity());

			return;
		}
		
		//! Find the money that will be reserved (always of type ExpansionMoneyBase), change the IsReserved to true.
		//! Ignore the money that already has IsReserved as true.
		UnlockMoney( player );

		array< ExpansionMoneyBase > monies = new array< ExpansionMoneyBase >;
		if ( !FindMoneyAndReserve( player, /*reservedList.Price*/currentPrice, monies ) )
		{
			reservedList.ClearReserved( zone );
			player.ClearMarketReserve();

			Callback_FailedNotEnoughMoney( itemClassName, player.GetIdentity() );
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedNotEnoughMoney: 1");
			#endif

			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedNotEnoughMoney: player " + player);
			#endif
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			//! EXLogPrint("Callback_FailedNotEnoughMoney: price" + reservedList.Price);
			#endif
			
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			//! EXLogPrint("Callback_FailedNotEnoughMoney: currentPrice" + currentPrice);
			#endif

			return;
		}

		reservedList.Valid = true;
		reservedList.Time = GetGame().GetTime();

		player.SetMoney( monies );

		// !TODO: Finish method RemoveReservedStock in PlayerBase
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( RemoveReservedStock, 30000, false, player, itemClassName );
		
		Callback_Success( itemClassName, player.GetIdentity() );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestPurchase
	// Server only
	// ------------------------------------------------------------
	private void RPC_RequestPurchase( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		int count;
		if ( !ctx.Read( count ) )
			return;
			
		ExpansionMarketCurrency currentPrice;
		if ( !ctx.Read( currentPrice ) )
			return;

		ExpansionTraderBase trader = ExpansionTraderBase.Cast( target );
		if ( !trader )
			return;
			
		PlayerBase player = PlayerBase.Cast( GetPlayerObjectByIdentity( senderRPC ) );
		if ( !player )
			return;
			
		Exec_RequestPurchase( player, itemClassName, count, currentPrice, trader );
	}

	/*
	 * Called Client Only: The server will finalize the transaction with the 
	 * details that were stored in RequestPurchase. This also finalizes the stock values.
	 */
	
	// ------------------------------------------------------------
	// Expansion ConfirmPurchase
	// ------------------------------------------------------------
	void ConfirmPurchase( string itemClassName, PlayerBase player = NULL )
	{
		if ( IsMissionHost() )
		{
			Exec_ConfirmPurchase( player, itemClassName );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Send( NULL, ExpansionMarketModuleRPC.ConfirmPurchase, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_ConfirmPurchase
	// ------------------------------------------------------------
	private void Exec_ConfirmPurchase( notnull PlayerBase player, string itemClassName )
	{
		ExpansionMarketReserve reserve = player.GetMarketReserve();
		if ( !reserve || !reserve.Trader || !reserve.Trader.GetTraderZone() )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 5");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		ExpansionMarketTraderZone zone = reserve.Trader.GetTraderZone();

		if ( !player.IsMarketItemReserved( itemClassName ) )
		{
			reserve.ClearReserved( zone );
			player.ClearMarketReserve();

			Callback_FailedReserveTime( itemClassName, player.GetIdentity() );
			return;
		}

		RemoveMoney( player.GetMoney() );
		
		array< Object > objs = Spawn( reserve.Trader, player, reserve.Reserved[0] );

		zone.ClearReservedStock( reserve.Reserved[0].ClassName, reserve.Reserved[0].Amount );

		//! TODO: Fix stock bug here
		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("objs : " + objs);
		#endif

		#ifdef EXPANSION_MARKET_MODULE_DEBUG
		EXLogPrint("_Exec_ConfirmPurchase " + reserve.Reserved[0].ClassName + " " + reserve.Reserved[0].Amount + " " + reserve.Reserved.Count());
		#endif

		zone.RemoveStock( reserve.Reserved[0].ClassName, reserve.Reserved[0].Amount, false );

		if ( reserve.Reserved.Count() > 1 )
		{
			for ( int j = 0; j < objs.Count(); j++ )
			{
				reserve.SpawnObjects( zone, this, objs[j] );
			}
		}

		reserve.SpawnObjects( zone, this, player );

		Callback_Success( itemClassName, player.GetIdentity() );
		
		//Need to clear reserved after a bought
		reserve.ClearReserved( zone );

		zone.Save();
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_ConfirmPurchase
	// ------------------------------------------------------------
	private void RPC_ConfirmPurchase( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		PlayerBase player;
		if ( !Class.CastTo( player, GetPlayerObjectByIdentity( senderRPC ) ) )
			return;

		Exec_ConfirmPurchase( player, itemClassName );
	}

	/*
	 * Called Client Only: The server will clear the reserved stock that the player made
	 */
	
	// ------------------------------------------------------------
	// Expansion CancelPurchase
	// ------------------------------------------------------------
	void CancelPurchase( string itemClassName, PlayerBase player = NULL )
	{
		if ( IsMissionHost() )
		{
			Exec_CancelPurchase( player, itemClassName );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Send( NULL, ExpansionMarketModuleRPC.CancelPurchase, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_CancelPurchase
	// ------------------------------------------------------------
	private void Exec_CancelPurchase( notnull PlayerBase player, string itemClassName )
	{
		RemoveReservedStock( player, itemClassName );
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_CancelPurchase
	// ------------------------------------------------------------
	private void RPC_CancelPurchase( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		PlayerBase player;
		if ( !Class.CastTo( player, GetPlayerObjectByIdentity( senderRPC ) ) )
			return;

		Exec_CancelPurchase( player, itemClassName );
	}

	// ------------------------------------------------------------
	// Expansion RequestSell
	// ------------------------------------------------------------
	void RequestSell( string itemClassName, int count, ExpansionMarketCurrency currentPrice, ExpansionTraderBase trader, PlayerBase player = NULL )
	{
		if ( !trader )
		{
			return;
		}
			
		if ( IsMissionHost() )
		{
			if ( IsMissionOffline() )
				player = PlayerBase.Cast( GetGame().GetPlayer() );
				
			Exec_RequestSell( player, itemClassName, count, currentPrice, trader );
		} 
		else if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Write( count );
			rpc.Write( currentPrice );
			rpc.Send( trader, ExpansionMarketModuleRPC.RequestSell, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RequestSell
	// ------------------------------------------------------------
	private void Exec_RequestSell( notnull PlayerBase player, string itemClassName, int count, ExpansionMarketCurrency currentPrice, ExpansionTraderBase trader )
	{
		if ( !player )
		{
			return;
		}
		
		ExpansionMarketTraderZone zone = trader.GetTraderZone();
		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		
		if ( !market )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 6");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}
		
		if ( !zone )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 7");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		// Afterwards calculate the price of the items at that stock		
		ExpansionMarketSell sellList = player.GetMarketSell();
		sellList.Trader = trader;

		if ( !market.GetItem( itemClassName, sellList.Item ) /*|| sellList.Valid*/ )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 8");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}		

		// Compare that price to the one the player sent
		// Look up into Exec_RequestBuy
		if ( !FindSellPrice( player, sellList.Item, zone.GetStock( itemClassName ), count, sellList ) /*|| sellList.Price != currentPrice*/ )
		{
			sellList.Debug();

			// If they don't match, call Callback_FailedStockChange
			Callback_FailedStockChange( itemClassName, player.GetIdentity() );
			
			return;
		}

		sellList.Valid = true;
		sellList.Time = GetGame().GetTime();

		Callback_Success( itemClassName, player.GetIdentity() );

		zone.Save();
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestSell
	// ------------------------------------------------------------
	private void RPC_RequestSell( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		int count;
		if ( !ctx.Read( count ) )
			return;
			
		ExpansionMarketCurrency currentPrice;
		if ( !ctx.Read( currentPrice ) )
			return;

		ExpansionTraderBase trader;
		if ( !Class.CastTo( trader, target ) )
			return;

		PlayerBase player;
		if ( !Class.CastTo( player, GetPlayerObjectByIdentity( senderRPC ) ) ) return;

		Exec_RequestSell( player, itemClassName, count, currentPrice, trader );
	}

	// ------------------------------------------------------------
	// Expansion ConfirmSell
	// ------------------------------------------------------------
	void ConfirmSell( string itemClassName, PlayerBase player = NULL )
	{
		if ( IsMissionHost() )
		{
			Exec_ConfirmSell( player, itemClassName );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Send( NULL, ExpansionMarketModuleRPC.ConfirmSell, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_ConfirmSell
	// ------------------------------------------------------------
	private void Exec_ConfirmSell( notnull PlayerBase player, string itemClassName )
	{
		if ( !player )
		{
			return;
		}
		
		ExpansionMarketSell sell = player.GetMarketSell();
		if ( !sell || !sell.Valid )
		{
			Callback_FailedReserveTime( itemClassName, player.GetIdentity() );
			return;
		}

		ExpansionMarketTraderZone zone = sell.Trader.GetTraderZone();
		if ( !zone )
		{
			#ifdef EXPANSION_MARKET_MODULE_DEBUG
			EXLogPrint("Callback_FailedUnknown: 9");
			#endif

			Callback_FailedUnknown( itemClassName, player.GetIdentity() );
			return;
		}

		for ( int j = 0; j < sell.Sell.Count(); j++ )
		{
			GetGame().ObjectDelete( sell.Sell[j].ItemRep );
		}

		SpawnMoney( player, sell.Price );
		
		zone.AddStock( itemClassName, sell.TotalAmount );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_ConfirmSell
	// ------------------------------------------------------------
	private void RPC_ConfirmSell( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		PlayerBase player;
		if ( !Class.CastTo( player, GetPlayerObjectByIdentity( senderRPC ) ) )
			return;

		Exec_ConfirmSell( player, itemClassName );
	}

	// ------------------------------------------------------------
	// Expansion CancelSell
	// ------------------------------------------------------------
	void CancelSell( string itemClassName, PlayerBase player = NULL )
	{
		if ( IsMissionHost() )
		{
			Exec_CancelSell( player, itemClassName );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Send( NULL, ExpansionMarketModuleRPC.CancelSell, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_CancelSell
	// ------------------------------------------------------------
	private void Exec_CancelSell( notnull PlayerBase player, string itemClassName )
	{
		player.ClearMarketSell();
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_CancelSell
	// ------------------------------------------------------------
	private void RPC_CancelSell( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		PlayerBase player;
		if ( !Class.CastTo( player, GetPlayerObjectByIdentity( senderRPC ) ) )
			return;

		Exec_CancelSell( player, itemClassName );
	}

	// ------------------------------------------------------------
	// Expansion RequestTraderData
	// ------------------------------------------------------------
	void RequestTraderData( ExpansionTraderBase trader )
	{
		if ( IsMissionOffline() )
		{
			Exec_RequestTraderData( trader, NULL );
		} else if ( !IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( trader, ExpansionMarketModuleRPC.RequestTraderData, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RequestTraderData
	// ------------------------------------------------------------
	private void Exec_RequestTraderData( ExpansionTraderBase trader, PlayerIdentity ident )
	{
		LoadTraderData( trader, ident );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestTraderData
	// ------------------------------------------------------------
	private void RPC_RequestTraderData( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ExpansionTraderBase trader;
		if ( !Class.CastTo( trader, target ) )
			return;

		Exec_RequestTraderData( trader, senderRPC );
	}

	// ------------------------------------------------------------
	// Expansion LoadTraderData
	// ------------------------------------------------------------
	void LoadTraderData( ExpansionTraderBase trader, PlayerIdentity ident )
	{
		m_NetworkItems.Clear();
		trader.GetNetworkSerialization( m_NetworkItems );

		if ( IsMissionOffline() )
		{
			Exec_LoadTraderData( trader, ident );

			SetTrader( trader );
		} else if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_NetworkItems );
			rpc.Send( trader, ExpansionMarketModuleRPC.LoadTraderData, true, ident );
		}
	}

	// ------------------------------------------------------------
	// Expansion Exec_LoadTraderData
	// ------------------------------------------------------------
	private void Exec_LoadTraderData( ExpansionTraderBase trader, PlayerIdentity ident )
	{
		for ( int i = 0; i < m_NetworkItems.Count(); i++ )
		{
			if ( !IsMissionOffline() )
				GetExpansionSettings().GetMarket().UpdateMarketItem_Client( m_NetworkItems[i] );

			m_ClientMarketZone.SetStock( m_NetworkItems[i].ClassName, m_NetworkItems[i].Stock );
		}

		SetTrader( trader );
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_LoadTraderData
	// ------------------------------------------------------------
	private void RPC_LoadTraderData( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ExpansionTraderBase trader;
		if ( !Class.CastTo( trader, target ) )
			return;

		m_NetworkItems.Clear();
		if ( !ctx.Read( m_NetworkItems ) )
			return;

		Exec_LoadTraderData( trader, senderRPC );
	}

	// ------------------------------------------------------------
	// Expansion RequestItemData
	// ------------------------------------------------------------
	void RequestItemData( ExpansionTraderBase trader, string itemClassName )
	{
		if ( IsMissionOffline() )
		{
			Exec_RequestItemData( trader, itemClassName, NULL );
		} else if ( !IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( itemClassName );
			rpc.Send( trader, ExpansionMarketModuleRPC.RequestItemData, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_RequestItemData
	// ------------------------------------------------------------
	private void Exec_RequestItemData( ExpansionTraderBase trader, string itemClassName, PlayerIdentity ident )
	{
		ExpansionMarketSettings market = GetExpansionSettings().GetMarket();
		
		ExpansionMarketItem item;
		if ( !market.GetItem( itemClassName, item ) )
		{
			return;
		}

		LoadItemData( trader, item, ident );

		for ( int i = 0; i < item.SpawnAttachments.Count(); i++ )
		{
			ExpansionMarketItem attItem;
			if ( !market.GetItem( item.SpawnAttachments[ i ], attItem ) )
			{
				continue;
			}

			LoadItemData( trader, attItem, ident );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestItemData
	// ------------------------------------------------------------
	private void RPC_RequestItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ExpansionTraderBase trader;
		if ( !Class.CastTo( trader, target ) )
			return;

		string itemClassName;
		if ( !ctx.Read( itemClassName ) )
			return;

		Exec_RequestItemData( trader, itemClassName, senderRPC );
	}

	// ------------------------------------------------------------
	// Expansion LoadItemData
	// ------------------------------------------------------------
	void LoadItemData( ExpansionTraderBase trader, ExpansionMarketItem item, PlayerIdentity ident )
	{
		if ( IsMissionOffline() )
		{
			SI_SelectedItemUpdatedInvoker.Invoke();
		} else if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( item );
			rpc.Send( trader, ExpansionMarketModuleRPC.LoadItemData, true, ident );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Exec_LoadItemData
	// ------------------------------------------------------------
	private void Exec_LoadItemData( ExpansionTraderBase trader, ExpansionMarketItem item )
	{
		ExpansionMarketItem clientItem;
		if ( GetExpansionSettings().GetMarket().GetItem( item.ClassName, clientItem ) )
		{
			clientItem.MaxPriceThreshold = item.MaxPriceThreshold;
			clientItem.MinPriceThreshold = item.MinPriceThreshold;

			clientItem.MaxStockThreshold = item.MaxStockThreshold;
			clientItem.MinStockThreshold = item.MinStockThreshold;
		}

		SI_SelectedItemUpdatedInvoker.Invoke();
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_LoadItemData
	// ------------------------------------------------------------
	private void RPC_LoadItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		ExpansionTraderBase trader;
		if ( !Class.CastTo( trader, target ) )
			return;

		ExpansionMarketItem item;
		if ( !ctx.Read( item ) )
			return;

		Exec_LoadItemData( trader, item );
	}

	// ------------------------------------------------------------
	// Expansion Bool IsLiquidItem
	// Return true if the item contains liquid. Liquid container + soda are liquid container for current update of DayZ (1.04)
	// ------------------------------------------------------------
	bool IsLiquidItem( ItemBase item )
	{
		return ( item && ( item.IsLiquidContainer() || item.IsKindOf("SodaCan_ColorBase") ) );
	}

	// -----------------------------------------------------------
	// Expansion array< EntityAI > LocalGetEntityInventory
	// -----------------------------------------------------------
	ref array< EntityAI > LocalGetEntityInventory()
	{
		return m_LocalEntityInventory;
	}

	// -----------------------------------------------------------
	// Expansion array< EntityAI > GetPlayerInventory
	// -----------------------------------------------------------
	ref array< EntityAI > GetPlayerInventory( PlayerBase player )
	{
		ref array<EntityAI> itemsArray = new array<EntityAI>;
		
		if ( player )
		{
	   		player.GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, itemsArray );

			m_LocalStringInventory.Clear();
			m_LocalEntityInventory.Clear();

			for ( int i = 0; i < itemsArray.Count(); i++ )
			{
				EntityAI itemEntity = itemsArray.Get(i);
				
				m_LocalStringInventory.Insert( itemEntity.GetType() );
				m_LocalEntityInventory.Insert( itemEntity );
			}
		}

		return itemsArray;
	}
	
	// ------------------------------------------------------------
	// Expansion Int GetAmountInInventory
	// Gets the amount of market items the player has in his inventroy
	// ------------------------------------------------------------
	int GetAmountInInventory( ExpansionMarketItem item, array< EntityAI > entitys )
	{
		int totalAmount = 0;

		for ( int i = 0; i < entitys.Count(); i++ )
		{
			EntityAI entity = entitys.Get(i);
			if ( entity == NULL )
				continue;

			string entName = entity.GetType();
			entName.ToLower();
			if ( entName != item.ClassName )
				continue;

			//! Check if the item has a container and any items in it
			if ( entity.HasAnyCargo() || entity.GetInventory().AttachmentCount() > 0 )
			{
				totalAmount = -1;
				continue;
			}

			float health = entity.GetHealthLevel("") + 1;

			float health_ratio = 1.0;

			if ( health != 0 ) health_ratio = 1 / health;
			if ( health_ratio == 0 || entity.IsRuined() )
				continue;

			ItemBase itemBase = ItemBase.Cast( entity );
			int quantitymax = 1;
			int quantity;
			bool liquidItem = false;

			if ( itemBase )
			{
				quantitymax = itemBase.GetQuantityMax();
				liquidItem = IsLiquidItem( itemBase );
			}

			//! We want to have only ammo and not magazine, and we don't want to detect quantity for energy item or liquid item or food item
			if ( ( quantitymax > 1 && !liquidItem && !entity.HasEnergyManager() && !entity.IsFood() ) || itemBase.IsKindOf("Ammunition_Base") )
			{
				//! We get the quantity of the item, a little exception for ammo, because it's using a custom function
				if ( itemBase.IsKindOf("Ammunition_Base") )
				{
					Magazine ammo = Magazine.Cast( itemBase );
					if ( ammo )
					{
						quantity = ammo.GetAmmoCount();
					}
					else
					{
						quantity = 1;
					}
				}
				else
				{
					quantity = itemBase.GetQuantity();
				}

				//! We add the quantity directly to the total amount of item
				totalAmount += quantity;
			}
			else
			//! Here it's item that don't need to calcul quantity, it's will be automaticaly 1
			{
				totalAmount++;
			}
		}

		return totalAmount;
	}

	// ------------------------------------------------------------
	// Expansion Bool CanOpenTraderMenu
	// ------------------------------------------------------------
	bool CanOpenTraderMenu()
	{
		if ( IsMissionClient() )
		{
			if ( GetGame().GetUIManager().GetMenu() )
			{
				return false;
			}
		}

		return true;
	}

	// ------------------------------------------------------------
	// Expansion OpenTraderMenu
	// ------------------------------------------------------------
	void OpenTraderMenu()
	{
		GetGame().GetUIManager().EnterScriptedMenu( MENU_EXPANSION_MARKET_MENU, NULL );
	}
}