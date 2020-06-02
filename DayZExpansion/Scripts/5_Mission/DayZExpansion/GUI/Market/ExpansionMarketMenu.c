/**
 * ExpansionMarketMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionMarketMenuState
{
	NONE, 
	REQUESTING_PURCHASE, 
	REQUESTING_SELL
}

class ExpansionMarketMenu extends UIScriptedMenu
{
	protected autoptr map< string, ref ExpansionMarketUIItem >	m_MarketItemsList;
	protected autoptr map< int, ref Widget > m_MarketCategoryList;

	protected ScrollWidget								m_MarketScroller;
	protected GridSpacerWidget							m_MarketItems;

	protected TextWidget								m_TitleWidget;
	protected TextWidget								m_WorthWidget;

	protected Widget									m_LoadingWidget;

	protected ExpansionTraderBase						m_Trader;
	protected ExpansionMarketTrader						m_TraderMarket;

	protected ExpansionMarketUIItem						m_SelectedItem;

	protected bool										m_IsLoading;

	protected Mission									m_Mission;
	protected IngameHud									m_InGameHUD;

	protected autoptr array< TextWidget	>				m_PlayerInfoPriceTypes;
	protected TextWidget								m_PlayerInfoTotal;

	protected TextWidget								m_MarketCurrentPrice;
	protected TextWidget								m_MarketItemStock;
	protected TextWidget								m_MarketItemStockPlayer;
	protected TextWidget								m_MarketItemType;
	protected TextWidget								m_MarketItemState;

	protected Widget									m_MarketInfoContent;
	protected TextWidget								m_MarketItemHeader;
	protected Widget									m_MarketItemDescriptionContent;
	protected RichTextWidget							m_MarketItemDescription;
	protected ItemPreviewWidget							m_ItemPreview;

	protected ButtonWidget								m_MarketBuy;
	protected TextWidget								m_MarketBuyPrice;
	protected autoptr array< TextWidget	>				m_MarketBuyPriceTypes;

	protected ButtonWidget								m_MarketSell;
	protected TextWidget								m_MarketSellPrice;
	protected autoptr array< TextWidget	>				m_MarketSellPriceTypes;

	protected ButtonWidget								m_MarketQuantityDecrease;
	protected ImageWidget								m_MarketQuantityDecreaseIcon;
	protected ButtonWidget								m_MarketQuantityIncrease;
	protected ImageWidget								m_MarketQuantityIncreaseIcon;
	protected ButtonWidget								m_MarketQuantityManual;
	protected TextWidget								m_MarketQuantity;

	protected Widget									m_QuantityDialogWindow;
	protected EditBoxWidget								m_QuantityEditBox;
	protected ButtonWidget								m_QuantityApplyButton;
	protected ButtonWidget								m_QuantityCancelButton;

	protected EditBoxWidget								m_SearchEditBox;
	protected ButtonWidget								m_ClearSearchEditBoxButton;
	protected ImageWidget								m_ClearSearchEditIcon;
	protected float										m_LastTimeTypedInSearch;
	protected bool 										m_TypingInSearchbox;
	
	protected Object									m_ItemEntity;

	protected int										m_Quantity;
	protected int										m_TraderStock;
	protected int										m_PlayerStock;
	protected float										m_BuyPrice;
	protected float										m_SellPrice;

	protected int 										m_ItemRotationX;
	protected int 										m_ItemRotationY;
	protected vector 									m_ItemOrientation;
	protected int										m_ItemScaleDelta;

	protected float 									m_RequestMarketTraderDataAccum;

	protected bool										m_WasGPSVisible;

	protected Widget									m_PopupDialog;
	protected TextWidget								m_PopupDialogTitle;
	protected RichTextWidget							m_PopupDialogText;
	protected ButtonWidget								m_PopupDialogConfirmButton;
	
	protected ExpansionMarketModule						m_MarketModule;

	protected ExpansionMarketMenuState					m_CurrentState;

	// ------------------------------------------------------------
	// Expansion ExpansionMarketMenu Constructor
	// ------------------------------------------------------------
	void ExpansionMarketMenu()
	{
		m_MarketItemsList = new map< string, ref ExpansionMarketUIItem >;

		m_MarketCategoryList = new map< int, ref Widget >;
		
		m_MarketModule = ExpansionMarketModule.Cast( GetModuleManager().GetModule( ExpansionMarketModule ) );
		
		ExpansionMarketModule.SI_SetTraderInvoker.Insert( SetTrader );
		ExpansionMarketModule.SI_SelectedItemUpdatedInvoker.Insert( OnNetworkItemUpdate );

		ExpansionMarketModule.SI_Callback.Insert( Menu_Callback );
	}

	// -----------------------------------------------------------
	// Expansion ExpansionMarketMenu Destructor
	// -----------------------------------------------------------
	void ~ExpansionMarketMenu()
	{
		ExpansionMarketModule.SI_SetTraderInvoker.Remove( SetTrader );
		ExpansionMarketModule.SI_SelectedItemUpdatedInvoker.Remove( OnNetworkItemUpdate );

		if ( m_ItemEntity )
		{
			GetGame().ObjectDelete( m_ItemEntity );
		}
	}

	// -----------------------------------------------------------
	// Expansion Widget Init
	// -----------------------------------------------------------
	override Widget Init()
	{
		layoutRoot 							= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/traders/expansion_market_menu.layout" );

		m_MarketScroller 					= ScrollWidget.Cast( layoutRoot.FindAnyWidget( "market_items_scroller" ) );
		m_MarketItems						= GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "market_items_container" ) );

		m_TitleWidget						= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_text" ) );

		m_WorthWidget						= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_money_text" ) );
		m_PlayerInfoTotal					= TextWidget.Cast( layoutRoot.FindAnyWidget( "player_total" ) );

		m_PlayerInfoPriceTypes				= new array< TextWidget >;
		m_PlayerInfoPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "player_info_goldbar" ) ) );
		m_PlayerInfoPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "player_info_silverbar" ) ) );
		m_PlayerInfoPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "player_info_goldnugget" ) ) );
		m_PlayerInfoPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "player_info_silvernugget" ) ) );

		m_MarketCurrentPrice				= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_current_price" ) );
		m_MarketItemStock					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_stock" ) );
		m_MarketItemStockPlayer				= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_stock_player" ) );
		m_MarketItemType					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_type" ) );
		m_MarketItemState					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_state" ) );

		m_MarketInfoContent					= Widget.Cast( layoutRoot.FindAnyWidget( "menu_info_content" ) );
		m_MarketItemHeader					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_header_text" ) );
		
		m_MarketItemDescriptionContent		= Widget.Cast( layoutRoot.FindAnyWidget( "market_item_description_container" ) );
		m_MarketItemDescription				= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_description" ) );
		m_ItemPreview 						= ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "market_item_preview" ) );

		m_MarketBuy							= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_item_buy" ) );
		m_MarketBuyPrice					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_buy_price_text" ) );
		
		m_MarketBuyPriceTypes				= new array< TextWidget >;
		m_MarketBuyPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_goldbar_buy_price" ) ) );
		m_MarketBuyPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_silverbar_buy_price" ) ) );
		m_MarketBuyPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_goldnugget_buy_price" ) ) );
		m_MarketBuyPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_silvernugget_buy_price" ) ) );

		m_MarketSell						= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_item_sell" ) );
		m_MarketSellPrice					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_sell_price_text" ) );
		
		m_MarketSellPriceTypes				= new array< TextWidget >;
		m_MarketSellPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_goldbar_sell_price" ) ) );
		m_MarketSellPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_silverbar_sell_price" ) ) );
		m_MarketSellPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_goldnugget_sell_price" ) ) );
		m_MarketSellPriceTypes.Insert( TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_info_silvernugget_sell_price" ) ) );

		m_MarketQuantityDecrease			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_button_left" ) );
		m_MarketQuantityDecreaseIcon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_left_icon" ) );
		m_MarketQuantityIncrease			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_button_right" ) );
		m_MarketQuantityIncreaseIcon		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_right_icon" ) );
		m_MarketQuantityManual				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_button" ) );
		m_MarketQuantity					= TextWidget.Cast( layoutRoot.FindAnyWidget( "market_item_amount_text" ) );

		m_LoadingWidget						= layoutRoot.FindAnyWidget( "market_loading" );

		m_QuantityDialogWindow				= Widget.Cast( layoutRoot.FindAnyWidget( "count_dialog_panel" ) );
		m_QuantityEditBox					= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "count_dialog_editbox" ) );
		m_QuantityApplyButton				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "count_dialog_apply_button" ) );
		m_QuantityCancelButton				= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "count_dialog_cancel_button" ) );

		m_SearchEditBox						= EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "market_filter_box" ) );
		m_ClearSearchEditBoxButton			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "market_filter_clear" ) );
		m_ClearSearchEditIcon				= ImageWidget.Cast( layoutRoot.FindAnyWidget( "market_filter_clear_icon" ) );

		m_PopupDialog						= Widget.Cast( layoutRoot.FindAnyWidget( "popup_dialog_panel" ) );
		m_PopupDialogTitle					= TextWidget.Cast( layoutRoot.FindAnyWidget( "popup_dialog_title_text" ) );
		m_PopupDialogText					= RichTextWidget.Cast( layoutRoot.FindAnyWidget( "popup_dialog_text" ) );
		m_PopupDialogConfirmButton			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "popup_dialog_confirm_button" ) );
		
		m_LastTimeTypedInSearch = 0;
		m_TypingInSearchbox = false;

		SetIsLoading( true );

		SetTrader( m_MarketModule.GetTrader() );

		return layoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion ShowPopupDialog
	// ------------------------------------------------------------
	void ShowPopupDialog( string title, string text )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::ShowPopupDialog - Start");
		#endif
		
		m_PopupDialog.Show( true );

		m_PopupDialogTitle.SetText( title );
		m_PopupDialogText.SetText( text );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::ShowPopupDialog - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion HidePopupDialog
	// ------------------------------------------------------------
	void HidePopupDialog()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::HidePopupDialog - Start");
		#endif
		
		m_PopupDialog.Show( false );
		m_PopupDialogTitle.SetText( "" );
		m_PopupDialogText.SetText( "" );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::HidePopupDialog - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SearchItems
	// ------------------------------------------------------------
	void SearchItems( string search )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::SearchItems - Start");
		#endif
		
		SetIsLoading( true );

		DeleteAllMarketItems();

		m_MarketItemsList.Clear();
		m_MarketCategoryList.Clear();

		GenerateMarketCategoriesUI( search );

		m_SelectedItem = NULL;

		UpdateItemFieldFromBasicNetSync();

		UpdateUIValues();

		if ( m_ItemEntity )
		{
			GetGame().ObjectDelete( m_ItemEntity );
		}

		m_ItemPreview.SetItem( NULL );

		m_MarketScroller.VScrollToPos( 0 );

		SetIsLoading( false );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::SearchItems - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteAllMarketItems
	// ------------------------------------------------------------
	void DeleteAllMarketItems()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::DeleteAllMarketItems - Start");
		#endif
		
		if ( m_MarketItems )
		{
			Widget child = m_MarketItems.GetChildren();
		
			while ( child )
			{
				Widget child_child = child.GetChildren();

				while ( child_child )
				{
					child_child.Unlink();
					child_child = child.GetSibling();
				}

				child.Unlink();
				child = m_MarketItems.GetSibling();
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::DeleteAllMarketItems - End");
		#endif
	}

	void Menu_Callback( bool ret, int type )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::Menu_Callback - Start");
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Ret: " + ret );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Type: " + type );
		#endif
		
		if ( ret && type == 0 )
		{
			ExpansionMarketMenuState state = m_CurrentState;
			m_CurrentState = ExpansionMarketMenuState.NONE;
			
			switch ( state )
			{
				case ExpansionMarketMenuState.REQUESTING_PURCHASE:
				{
					m_MarketModule.ConfirmPurchase( m_SelectedItem.GetMarketItem().ClassName, PlayerBase.Cast( GetGame().GetPlayer() ) );
					break;
				}
				case ExpansionMarketMenuState.REQUESTING_SELL:
				{
					m_MarketModule.ConfirmSell( m_SelectedItem.GetMarketItem().ClassName, PlayerBase.Cast( GetGame().GetPlayer() ) );
					break;
				}
			}
		} 
		else
		{
			if (type == 1)
			{
				ShowPopupDialog( "Error", "the stock changed. show a popup in the market menu" );
			} 
			else if (type == 2)
			{
				ShowPopupDialog( "Error", "player did not have enough money. show a popup in the market menu" );
			} 
			else if (type == 3) 
			{ 
				ShowPopupDialog( "Error", "30 seconds has passed." );
			} 
			else
			{
				ShowPopupDialog( "Error", "unknown error" );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::Menu_Callback - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateUIValues
	// ------------------------------------------------------------
	void UpdateUIValues()
	{
		// Print("ExpansionMarketMenu::UpdateUIValues - Start");
		
		ExpansionMarketItem item;
		
		if ( m_SelectedItem && Class.CastTo( item, m_SelectedItem.GetMarketItem() ) )
		{
			m_MarketItemHeader.SetText( ExpansionGetItemDisplayNameWithType( item.ClassName ) );

			string item_desc = ExpansionGetItemDescriptionWithType( item.ClassName );
			if ( item_desc != "")
			{
				m_MarketItemDescription.SetText( item_desc );
				m_MarketItemDescriptionContent.Show(true);
			} 
			else
			{
				m_MarketItemDescriptionContent.Show(false);
			}
			

			int limit = m_TraderStock;
			
			if ( limit < m_PlayerStock )
			{
				limit = m_PlayerStock;
			}

			if ( limit == 0 )
			{
				m_MarketQuantity.SetText( "N/A" );
			} 
			else 
			{
				if ( m_Quantity > limit )
				{
					m_Quantity = limit;
				} 
				else if ( m_Quantity < 1 )
				{
					m_Quantity = 1;
				}

				m_MarketQuantity.SetText( "" + m_Quantity );
			}

			m_MarketInfoContent.Show(true);
		} 
		else
		{
			m_MarketItemHeader.SetText( "" );
			m_MarketItemDescription.SetText( "" );
			m_MarketQuantity.SetText( "" );
			m_MarketInfoContent.Show(false);
		}
		
		UpdatePlayerCurrency();
		
		// Print("ExpansionMarketMenu::UpdateUIValues - End");
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePlayerCurrency
	// ------------------------------------------------------------
	void UpdatePlayerCurrency()
	{
		// Print("ExpansionMarketMenu::UpdatePlayerCurrency - Start");
		
		array< int > monies = new array< int >;
		float worth = m_MarketModule.GetPlayerWorth( PlayerBase.Cast( GetGame().GetPlayer() ), monies );

		m_PlayerInfoTotal.SetText( worth.ToString() + " #STR_EXPANSION_MARKET_CURRENCY_TOTAL" );

		string name;
		int i;

		for ( i = 0; i < monies.Count(); i++ )
		{
			if ( i >= m_PlayerInfoPriceTypes.Count() )
				break;

			//name = "#" + ExpansionGetItemDisplayNameWithType( m_MarketModule.GetMoneyType( i ) );
			name = m_MarketModule.GetMoneyType( i ).Substring( 9, m_MarketModule.GetMoneyType( i ).Length() - 9 );

			m_PlayerInfoPriceTypes[i].SetText( "" + monies[i] + " " + name );
		}
		
		// Print("ExpansionMarketMenu::UpdatePlayerCurrency - End");
	}
	
	// ------------------------------------------------------------
	// Expansion SetSelectedItem
	// ------------------------------------------------------------
	void SetSelectedItem( ExpansionMarketUIItem uiItem )
	{
		// Print("ExpansionMarketMenu::SetSelectedItem - Start");
		
		m_SelectedItem = uiItem;

		ExpansionMarketItem item;
		if ( m_SelectedItem && Class.CastTo( item, m_SelectedItem.GetMarketItem() ) )
		{
			m_MarketModule.RequestItemData( m_Trader, item.ClassName );

			m_MarketItemHeader.SetText( ExpansionGetItemDisplayNameWithType( item.ClassName ) );
			m_MarketItemDescription.SetText( "<html><body><p>" + ExpansionGetItemDescriptionWithType( item.ClassName ) + "</p></body></html>" );
			
			UpdateItemFieldFromBasicNetSync();

			if ( m_ItemEntity )
			{
				GetGame().ObjectDelete( m_ItemEntity );
			}

			m_ItemEntity = GetGame().CreateObject( item.ClassName, vector.Zero, true, false );

			Transport transportEntity;
			if ( Class.CastTo( transportEntity, m_ItemEntity ) )
			{
				dBodyActive( m_ItemEntity, ActiveState.INACTIVE );
				dBodyDynamic( m_ItemEntity, false );

				transportEntity.DisableSimulation( true );
			}

			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.UpdateItemRender, 100.0 );
		}
		
		// !TODO: After rpc sending, add this back in
		SetIsLoading( true );
		
		// Print("ExpansionMarketMenu::SetSelectedItem - End");
	}
	
	// ------------------------------------------------------------
	// Expansion OnNetworkItemUpdate
	// ------------------------------------------------------------
	protected void OnNetworkItemUpdate()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::OnNetworkItemUpdate - Start");
		#endif
		
		UpdateUIValues();

		UpdateItemFieldFromBasicNetSync();

		SetIsLoading( false );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::OnNetworkItemUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateItemRender
	// ------------------------------------------------------------
	protected void UpdateItemRender()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::UpdateItemRender - Start");
		#endif
		
		if ( m_ItemEntity && m_ItemPreview )
		{
			m_ItemPreview.SetItem( EntityAI.Cast( m_ItemEntity ) );
			m_ItemPreview.SetModelPosition( Vector( 0, 0, 0.5 ) );
			m_ItemPreview.SetModelOrientation( vector.Zero );
			m_ItemPreview.SetView( EntityAI.Cast(m_ItemEntity).GetViewIndex() );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::UpdateItemRender - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateItemFieldFromBasicNetSync
	// ------------------------------------------------------------
	protected void UpdateItemFieldFromBasicNetSync()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::UpdateItemFieldFromBasicNetSync - Start");
		#endif
		
		int i;

		ExpansionMarketItem item;
		if ( !m_SelectedItem || !Class.CastTo( item, m_SelectedItem.GetMarketItem() ) )
		{
			m_MarketSellPrice.SetText( "" );
			m_MarketBuyPrice.SetText( "" );

			m_MarketItemStock.SetText( "" );
			m_MarketItemStockPlayer.SetText( "" );

			for ( i = 0; i < m_MarketSellPriceTypes.Count(); i++ )
			{
				m_MarketSellPriceTypes[i].SetText( "" );
			}

			for ( i = 0; i < m_MarketBuyPriceTypes.Count(); i++ )
			{
				m_MarketBuyPriceTypes[i].SetText( "" );
			}

			return;
		}

		m_TraderStock = m_SelectedItem.GetTraderStock();

		array<EntityAI> toRemove = new array<EntityAI>;
		int toSpawn = 0;
		bool canPurchase = false;
		
		ExpansionMarketSell marketSell = new ExpansionMarketSell;
		// marketSell.Trader = trader;
		if ( m_MarketModule.FindSellPrice( PlayerBase.Cast( GetGame().GetPlayer() ), item, m_TraderStock, m_Quantity, marketSell ) )
		{
			m_SellPrice = marketSell.Price;

			m_MarketSellPrice.SetText( "" + m_SellPrice );
		} else
		{
			m_SellPrice = -1;

			m_MarketSellPrice.SetText( "ERROR" );
		}

		ExpansionMarketReserve reserved = new ExpansionMarketReserve;
		if ( m_MarketModule.FindPurchasePriceAndReserve( item, m_MarketModule.GetClientZone(), m_Quantity, reserved ) )
		{
			m_BuyPrice = reserved.Price;

			m_MarketBuyPrice.SetText( "" + m_BuyPrice );
		} 
		else
		{
			m_BuyPrice = -1;

			m_MarketBuyPrice.SetText( "ERROR" );
		}
		
		m_PlayerStock = m_MarketModule.GetAmountInInventory( item, m_MarketModule.LocalGetEntityInventory() );
		
		if ( m_TraderStock >= 0 )
		{
			if ( m_TraderStock == 0 )
			{
				m_MarketItemStock.SetText( "Not In Stock" );
				m_MarketBuy.Show(false);
			}
			else
			{
				m_MarketItemStock.SetText( "In Stock: " + m_TraderStock.ToString() );
				m_MarketBuy.Show(true);
			}
		} 
		else
		{
			if ( m_TraderStock == -1 )
			{
				m_MarketItemStock.SetText( "In Stock" );
				m_MarketBuy.Show(true);
			} 
			else
			{
				m_MarketItemStock.SetText( "Not In Stock" );
				m_MarketBuy.Show(false);
			}
		}
		
		if ( m_PlayerStock >= 0 )
		{
			if (m_PlayerStock == 0)
			{
				m_MarketItemStockPlayer.SetText( "You Have: No Items" );
				m_MarketSell.Show(false);
			} 
			else
			{
				m_MarketItemStockPlayer.SetText( "You Have: " + m_PlayerStock );
				m_MarketSell.Show(true);
			}
		}

		array< int > monies = new array< int >;

		string name;
		m_MarketModule.FindMoneyAndCountTypes( PlayerBase.Cast(GetGame().GetPlayer()), m_SellPrice, monies );
		for ( i = 0; i < monies.Count(); i++ )
		{
			if ( i >= m_MarketSellPriceTypes.Count() )
				break;

			//name = "#" + ExpansionGetItemDisplayNameWithType( m_MarketModule.GetMoneyType( i ) );
			name = m_MarketModule.GetMoneyType( i ).Substring( 9, m_MarketModule.GetMoneyType( i ).Length() - 9 );

			m_MarketSellPriceTypes[i].SetText( "" + monies[i] + " " + name );
		}

		monies.Clear();
		m_MarketModule.FindMoneyAndCountTypes( PlayerBase.Cast(GetGame().GetPlayer()), m_BuyPrice, monies );
		for ( i = 0; i < monies.Count(); i++ )
		{
			if ( i >= m_MarketBuyPriceTypes.Count() )
				break;

			//name = "#" + ExpansionGetItemDisplayNameWithType( m_MarketModule.GetMoneyType( i ) );
			name = m_MarketModule.GetMoneyType( i ).Substring( 9, m_MarketModule.GetMoneyType( i ).Length() - 9 );

			m_MarketBuyPriceTypes[i].SetText( "" + monies[i] + " " + name );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::UpdateItemFieldFromBasicNetSync - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetIsLoading
	// ------------------------------------------------------------
	void SetIsLoading( bool state )
	{
		// Print("ExpansionMarketMenu::SetIsLoading - Start");
		
		m_IsLoading = state;

		m_LoadingWidget.Show( m_IsLoading );
		
		// Print("ExpansionMarketMenu::SetIsLoading - End");
	}
	
	// ------------------------------------------------------------
	// Expansion IsLoading
	// ------------------------------------------------------------
	bool IsLoading()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::SetIsLoading - Return: " + m_IsLoading.ToString() );
		#endif
		return m_IsLoading;
	}
	
	// ------------------------------------------------------------
	// Expansion GenerateMarketCategoriesUI
	// ------------------------------------------------------------
	protected void GenerateMarketCategoriesUI( string searchString = "" )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::GenerateMarketCategoriesUI - Start");
		#endif
		
		map< int, ref ExpansionMarketCategory > categories = GetExpansionSettings().GetMarket().GetCategories();
		array< ref ExpansionMarketItem > tempItems = new array< ref ExpansionMarketItem >;

		searchString.ToLower();

		for ( int i = 0; i < categories.Count(); i++ )
		{
			if ( categories.GetElement( i ).Items.Count() == 0 )
				continue;

			for ( int j = 0; j < categories.GetElement( i ).Items.Count(); j++ )
			{
				if ( !m_TraderMarket.ItemExists( categories.GetElement( i ).Items[j].ClassName ) )
					continue;

				if ( searchString != "" )
				{
					//Do this instead everything inside one condition because ExpansionGetItemDisplayNameWithType function, could be heavy with a lot of items to compute
					string displayName = ExpansionGetItemDisplayNameWithType( categories.GetElement( i ).Items[j].ClassName );
					displayName.ToLower();

					if ( displayName.IndexOf( searchString ) == -1 )
						continue;
				}
					

				tempItems.Insert( categories.GetElement( i ).Items[j] );
			}

			if ( tempItems.Count() == 0 )
				continue;

			Widget category;
			if ( !m_MarketCategoryList.Find( categories.GetElement( i ).CategoryID, category ) )
			{
				category = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/traders/expansion_market_category.layout", m_MarketItems ) );

				TextWidget catTitle = TextWidget.Cast( category.FindAnyWidget( "category_title" ) );

				if ( catTitle )
				{
					catTitle.SetText( categories.GetElement( i ).DisplayName );
				}

				m_MarketCategoryList.Insert( categories.GetElement( i ).CategoryID, category );
			}

			Widget catParent = category.FindAnyWidget( "category_items" );

			for ( int k = 0; k < tempItems.Count(); k++ )
			{
				GenerateMarketItemUI( tempItems[k], catParent );
			}

			tempItems.Clear();
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarketMenu::GenerateMarketCategoriesUI - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GenerateMarketItemUI
	// ------------------------------------------------------------
	protected void GenerateMarketItemUI( ExpansionMarketItem item, Widget catParent )
	{
		// Print("ExpansionMarketMenu::GenerateMarketItemUI - Start");
		
		ExpansionMarketUIItem marketItem;

		if ( !m_MarketItemsList.Find( item.ClassName, marketItem ) )
		{
			Widget parent = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/traders/expansion_market_item_300.layout", catParent ) );

			Widget marketItemWidget = Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/traders/expansion_market_item.layout", parent ) );

			marketItemWidget.SetRotation( 0, 0, ( 0.5 - Math.RandomFloat01() ) * 0.001 );

			marketItemWidget.GetScript( marketItem );

			marketItem.SetMarketItemData( item );
			marketItem.SetMarketMenu( this );
			marketItem.SetTrader( m_Trader );

			m_MarketItemsList.Insert( item.ClassName, marketItem );

			parent.Show( true );
			marketItemWidget.Show( true );
		}

		if ( marketItem )
		{
			marketItem.UpdateFields();
		}
		
		// Print("ExpansionMarketMenu::GenerateMarketItemUI - End");
	}

	// ------------------------------------------------------------
	// Expansion SetTrader
	// For security we will load from RPC later on
	// ------------------------------------------------------------
	void SetTrader( ExpansionTraderBase trader )
	{
		// Print("ExpansionMarketMenu::SetTrader - Start");

		if (!trader)
		{
			// Print("ExpansionMarketMenu::SetTrader - Error: Trader is NULL!");
			return;
		}
		
		m_Trader = trader;

		if ( m_Trader == NULL )
		{
			//Error("[ExpansionMarketMenu] m_Trader is NULL!");
			// Print("ExpansionMarketMenu::SetTrader Error");
			return;
		}

		m_TraderMarket = trader.GetTraderMarket();

		if ( m_TraderMarket == NULL )
		{
			//Error("[ExpansionMarketMenu] m_TraderMarket is NULL!");
			// Print("ExpansionMarketMenu::SetTrader Error");

			trader.RequestTraderObject();
			return;
		}

		SetIsLoading( false );

		m_MarketModule.GetPlayerInventory( GetPlayer() );

		m_TitleWidget.SetText( m_TraderMarket.DisplayName );

		GenerateMarketCategoriesUI();

		UpdateItemFieldFromBasicNetSync();

		UpdateUIValues();

		// Print("ExpansionMarketMenu::SetTrader - End");
	}

	// ------------------------------------------------------------
	// Expansion IncrementQuantity
	// ------------------------------------------------------------
	void IncrementQuantity()
	{		
		// Print("ExpansionMarketMenu::IncrementQuantity - Start");
		
		m_Quantity++;
		
		UpdateUIValues();

		UpdateItemFieldFromBasicNetSync();
		
		// Print("ExpansionMarketMenu::IncrementQuantity - End");
	}

	// ------------------------------------------------------------
	// Expansion DecrementQuantity
	// ------------------------------------------------------------
	void DecrementQuantity()
	{
		// Print("ExpansionMarketMenu::DecrementQuantity - Start");
		
		m_Quantity--;

		UpdateUIValues();

		UpdateItemFieldFromBasicNetSync();
		
		// Print("ExpansionMarketMenu::DecrementQuantity - End");
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateRotation
	//-------------------------------------------------------------
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		// Print("ExpansionMarketMenu::UpdateRotation - Start");
		
		vector o = m_ItemOrientation;
		o[0] = o[0] + (m_ItemRotationY - mouse_y);
		o[1] = o[1] - (m_ItemRotationX - mouse_x);
		
		m_ItemPreview.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_ItemOrientation = o;
		}
		
		// Print("ExpansionMarketMenu::UpdateRotation - End");
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateScale
	//-------------------------------------------------------------
	void UpdateScale()
	{
		// Print("ExpansionMarketMenu::UpdateScale - Start");
		
		float w, h, x, y;		
		m_ItemPreview.GetPos(x, y);
		m_ItemPreview.GetSize(w,h);
		w = w + ( m_ItemScaleDelta / 4);
		h = h + ( m_ItemScaleDelta / 4 );
		if ( w > 0.5 && w < 3 )
		{
			m_ItemPreview.SetSize( w, h );
		}
		
		// Print("ExpansionMarketMenu::UpdateScale - End");
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleQuatityDialog
	//-------------------------------------------------------------
	void ToggleQuantityDialog()
	{
		// Print("ExpansionMarketMenu::ToggleQuantityDialog - Start");
		
		string quantityText;
		
		if ( !m_QuantityDialogWindow.IsVisible() )
		{
			quantityText = m_Quantity.ToString();
			m_QuantityEditBox.SetText( quantityText );
			
			m_QuantityDialogWindow.Show( true );
			m_ItemPreview.Show( false );
		}
		else
		{
			m_QuantityDialogWindow.Show( false );
			m_ItemPreview.Show( true );
		}
		
		// Print("ExpansionMarketMenu::ToggleQuantityDialog - End");
	}
	
	// ------------------------------------------------------------
	// Expansion SetQuantity
	//-------------------------------------------------------------
	void SetQuantity()
	{
		// Print("ExpansionMarketMenu::SetQuantity - Start");
		
		string quantityText;
		int quantity;
		TStringArray allowedCharacters = {"0","1","2","3","4","5","6","7","8","9"}
		
		quantityText = m_QuantityEditBox.GetText();
		
		//Check if they are using a character that is not allowed
		for ( int i = 0; i < quantityText.Length(); i++ )
		{
			if ( allowedCharacters.Find( quantityText.Get(i) ) == -1 )
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_MARKET_TITLE" ), new StringLocaliser( "STR_EXPANSION_MARKET_QUANTITY" ), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7 );
				return;
			}
		}
		
		// Convert the string to int for use in m_Quantity!
		quantity = quantityText.ToInt();
		m_Quantity = quantity;

		UpdateItemFieldFromBasicNetSync();
		UpdateUIValues();
		
		ToggleQuantityDialog();
		
		// Print("ExpansionMarketMenu::SetQuantity - End");
	}
	
	// ------------------------------------------------------------
	// Expansion RequestPurchase
	//-------------------------------------------------------------
	void RequestPurchase()
	{
		// Print("ExpansionMarketMenu::RequestPurchase - Start");
		
		ExpansionMarketItem item = m_SelectedItem.GetMarketItem();
		if ( !item )
			return;
		
		m_MarketModule = ExpansionMarketModule.Cast( GetModuleManager().GetModule(ExpansionMarketModule) );
		if ( !m_MarketModule )
			return;
		
		m_CurrentState = ExpansionMarketMenuState.REQUESTING_PURCHASE;

		m_MarketModule.RequestPurchase( item.ClassName, m_Quantity, m_BuyPrice, m_Trader );
		
		// Print("ExpansionMarketMenu::RequestPurchase - End");
	}
	
	// ------------------------------------------------------------
	// Expansion RequestSell
	//-------------------------------------------------------------
	void RequestSell()
	{
		// Print("ExpansionMarketMenu::RequestSell - Start");
		
		ExpansionMarketItem item = m_SelectedItem.GetMarketItem();
		if ( !item )
			return;

		m_MarketModule = ExpansionMarketModule.Cast( GetModuleManager().GetModule(ExpansionMarketModule) );
		if ( !m_MarketModule )
			return;

		m_CurrentState = ExpansionMarketMenuState.REQUESTING_SELL;
		
		m_MarketModule.RequestSell( item.ClassName, m_Quantity, m_BuyPrice, m_Trader );
		
		// Print("ExpansionMarketMenu::RequestSell - End");
	}
	
	// ------------------------------------------------------------
	// Override Bool UseKeyboard
	// ------------------------------------------------------------
	override bool UseKeyboard()
	{
		return true;
	}

	// ------------------------------------------------------------
	// Override Bool UseMouse
	// ------------------------------------------------------------
	override bool UseMouse()
	{
		return true;
	}

	// ------------------------------------------------------------
	// Override OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		// Print("ExpansionMarketMenu::OnShow - Start");
		
		super.OnShow();
		
		SetFocus( layoutRoot );
		PPEffects.SetBlurMenu( 1 );

		GetGame().GetMission().PlayerControlDisable( INPUT_EXCLUDE_ALL );
		
		// Print("ExpansionMarketMenu::OnShow - End");
	}

	// ------------------------------------------------------------
	// Override OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		// Print("ExpansionMarketMenu::OnHide - Start");
		
		super.OnHide();

		PPEffects.SetBlurMenu( 0 );

		MissionGameplay.Cast( GetGame().GetMission() ).PlayerControlEnable(true);
		
		// Print("ExpansionMarketMenu::OnHide - End");
	}

	// ------------------------------------------------------------
	// Override Update
	// ------------------------------------------------------------
	override void Update( float timeslice )
	{
		// Print("ExpansionMarketMenu::Update - Start");
		
		if ( GetUApi().GetInputByName( "UAUIBack" ).LocalPress() )
		{
			if ( m_PopupDialog.IsVisible() )
			{
				HidePopupDialog();
			} 
			else
			{
				Close();
			}

			return;
		}

		m_RequestMarketTraderDataAccum += timeslice;
		if ( m_RequestMarketTraderDataAccum > 0.5 )
		{
			m_RequestMarketTraderDataAccum = 0;

			m_MarketModule.RequestTraderData( m_Trader );
		}

		UpdateUIValues();

		//When typing in search box
		if ( m_TypingInSearchbox )
		{
			if ( m_LastTimeTypedInSearch >= 0.25 )
			{
				m_TypingInSearchbox = false;
				SearchItems( m_SearchEditBox.GetText() );
				m_LastTimeTypedInSearch = 0;
			} 
			else
			{
				m_LastTimeTypedInSearch += timeslice;
			}
		}
		
		// Print("ExpansionMarketMenu::Update - End");
	}
	
	// ------------------------------------------------------------
	// Override bool OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if ( IsLoading() )
		{
			return false;
		}

		if ( w == m_ClearSearchEditBoxButton )
		{
			m_SearchEditBox.SetText( "" );
			SearchItems( "" );
			
			return true;
		}

		ExpansionMarketItem item;
		if ( !m_SelectedItem || !Class.CastTo( item, m_SelectedItem.GetMarketItem() ) )
		{
			return false;
		}

		if ( w == m_MarketQuantityDecrease )
		{
			DecrementQuantity();
			
			return true;
		}

		if ( w == m_MarketQuantityIncrease )
		{
			IncrementQuantity();
			
			return true;
		}

		if ( w == m_MarketQuantityManual )
		{
			// show quantity textbox popup
			ToggleQuantityDialog();
			
			return true;
		}

		if ( w == m_MarketBuy )
		{
			RequestPurchase();
			
			return true;
		}

		if ( w == m_MarketSell )
		{
			RequestSell();
			
			return true;
		}
		
		if (w == m_QuantityApplyButton )
		{
			// Apply quantity in popop
			SetQuantity();
			
			return true;
		}
		
		if (w == m_QuantityCancelButton )
		{
			ToggleQuantityDialog();
			
			return true;
		}
		
		if (w == m_PopupDialogConfirmButton)
		{
			HidePopupDialog();
			
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonDown
	//-------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w == m_ItemPreview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_ItemRotationX, m_ItemRotationY);
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnKeyPress
	//-------------------------------------------------------------
	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		super.OnKeyPress( w, x, y, key );

		if ( w == m_SearchEditBox )
		{
			m_TypingInSearchbox = true;
			m_LastTimeTypedInSearch = 0;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseWheel
	//-------------------------------------------------------------
	override bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		super.OnMouseWheel(w, x, y, wheel);
		
		if ( w == m_ItemPreview )
		{
			m_ItemScaleDelta = wheel;
			UpdateScale();
		}
		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{	
		if (w == m_MarketQuantityDecrease || w == m_MarketQuantityDecreaseIcon)
		{
			m_MarketQuantityDecreaseIcon.SetColor(ARGB(255,192,57,43));
			
			return true;
		}
		else if (w == m_MarketQuantityIncrease || w == m_MarketQuantityIncreaseIcon)
		{
			m_MarketQuantityIncreaseIcon.SetColor(ARGB(255,39,174,96));
			
			return true;
		}
		else if (w == m_MarketQuantityManual)
		{
			m_MarketQuantity.SetColor(ARGB(255,127,140,141));
			
			return true;
		}
		else if (w == m_ClearSearchEditBoxButton || w == m_ClearSearchEditIcon )
		{
			m_ClearSearchEditIcon.SetColor(ARGB(255,243,156,18));
			
			return true;
		}		

	 	return super.OnMouseEnter(w, x, y);
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{		
		if (w == m_MarketQuantityDecrease || w == m_MarketQuantityDecreaseIcon)
		{
			m_MarketQuantityDecreaseIcon.SetColor(ARGB(255,231,76,60));
			
			return true;
		}
		else if (w == m_MarketQuantityIncrease || w == m_MarketQuantityIncreaseIcon)
		{
			m_MarketQuantityIncreaseIcon.SetColor(ARGB(255,46,204,113));
			
			return true;
		}
		else if (w == m_MarketQuantityManual)
		{
			m_MarketQuantity.SetColor(ARGB(255,255,255,255));
			
			return true;
		}
		else if (w == m_ClearSearchEditBoxButton || w == m_ClearSearchEditIcon)
		{
			m_ClearSearchEditIcon.SetColor(ARGB(255,241,196,15));
			
			return true;
		}		

	 	return super.OnMouseLeave(w, enterW, x, y);
	}
}
