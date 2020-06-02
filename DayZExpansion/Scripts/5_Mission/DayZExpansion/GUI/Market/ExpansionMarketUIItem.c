/**
 * ExpansionMarketUIItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketUIItem: ScriptedWidgetEventHandler
{	
	//! Widgets
	protected Widget							m_LayoutRoot;
	protected ImageWidget						m_ItemBackground;
	protected ItemPreviewWidget					m_ItemPreview;
	protected Widget							m_ItemPreviewContent;
	protected TextWidget						m_ItemNameText;
	protected TextWidget						m_ItemStockText;
	protected TextWidget						m_PlayerStockText;
	protected ButtonWidget						m_MainButton;
	protected Widget							m_ItemHighlightPanel;
	protected Widget							m_ItemInfoPanel;
	protected ButtonWidget						m_ItemInfoButton;
	protected ImageWidget						m_ItemInfoButtonIcon;

	//! Market details
	protected ExpansionMarketItem 				m_MarketItem;
	protected ExpansionMarketTraderZone 		m_MarketZone;

	protected ExpansionMarketMenu				m_MarketMenu;
	protected ExpansionMarketModule				m_MarketModule;

	protected ExpansionTraderBase				m_Trader;

	//! Metadata
	protected int								m_Stock;
	protected int								m_AmountInInventory;

	protected float								m_BuyPrice;
	protected float								m_SellPrice;
	
	protected Object							m_ItemEntity;
	
	protected int								MARKET_NO_STOCK_COLOR = ARGB(255, 192, 57, 43);
	protected int								MARKET_STOCK_LOW_COLOR = ARGB(255, 211, 84, 0);
	protected int								MARKET_STOCK_NORMAL_COLOR = ARGB(255, 41, 128, 185);
	protected int								MARKET_STOCK_HIGH_COLOR = ARGB(255, 39, 174, 96);
	protected int								MARKET_CANT_SELL_COLOR = ARGB(255, 243, 156, 18);
	
	protected ref ExpansionUITooltip			m_ItemTooltip;
	
	// ------------------------------------------------------------
	// ExpansionMarketUIItem Constructor
	// ------------------------------------------------------------
	void ExpansionMarketUIItem()
	{
		Class.CastTo( m_MarketModule, GetModuleManager().GetModule( ExpansionMarketModule ) );
		
		m_ItemTooltip = new ExpansionUITooltip("#STR_EXPANSION_MARKET_ITEM_TOOLTIP_TEXT", "#STR_EXPANSION_MARKET_ITEM_TOOLTIP_TITLE");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketUIItem Destructor
	// ------------------------------------------------------------
	void ~ExpansionMarketUIItem()
	{
		if ( m_ItemEntity )
		{
			GetGame().ObjectDelete( m_ItemEntity );
		}

		if ( m_ItemTooltip )
			delete m_ItemTooltip;
	}
	
	// ------------------------------------------------------------
	// Expansion OnWidgetScriptInit
	// ------------------------------------------------------------
	void OnWidgetScriptInit( Widget w )
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler( this );

		m_ItemBackground				= ImageWidget.Cast(m_LayoutRoot.FindAnyWidget( "market_item_background" ));
		m_ItemPreview 					= ItemPreviewWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_preview" ) );
		m_ItemPreviewContent			= Widget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_preview_container" ) );

		m_ItemNameText 					= TextWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_header_text" ) );

		m_ItemStockText 				= TextWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_info_stock" ) );
		m_PlayerStockText 				= TextWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_info_player_stock" ) );

		m_MainButton 					= ButtonWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_button" ) );
		m_ItemHighlightPanel			= Widget.Cast( m_LayoutRoot.FindAnyWidget( "market_highlight_panel" ) );

		m_ItemInfoPanel					= Widget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_info_content" ) );
		m_ItemInfoButton				= ButtonWidget.Cast( m_LayoutRoot.FindAnyWidget( "market_item_info_button" ) );
		m_ItemInfoButtonIcon			= ImageWidget.Cast(m_LayoutRoot.FindAnyWidget( "market_item_info_icon" ));
		
		m_LayoutRoot.Show( false );
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketItem GetMarketItem
	// ------------------------------------------------------------
	ExpansionMarketItem GetMarketItem()
	{
		return m_MarketItem;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketTraderZone GetMarketZone
	// ------------------------------------------------------------
	ExpansionMarketTraderZone GetMarketZone()
	{
		return m_MarketZone;
	}

	// ------------------------------------------------------------
	// Expansion SetMarketItemData
	// ------------------------------------------------------------
	void SetMarketItemData( ref ExpansionMarketItem item )
	{
		m_MarketItem = item;

		if ( !m_MarketItem )
			return;

		m_ItemEntity = GetGame().CreateObject( m_MarketItem.ClassName, vector.Zero, true, false );
		
		Transport transportEntity;
		if ( Class.CastTo( transportEntity, m_ItemEntity ) )
		{
			dBodyActive( transportEntity, ActiveState.INACTIVE );
			dBodyDynamic( transportEntity, false );

			transportEntity.DisableSimulation( true );
		}

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.UpdateItemRender, 100.0 );

		m_ItemNameText.SetText( ExpansionGetItemDisplayNameWithType( m_MarketItem.ClassName ) );
	}
	
	// ------------------------------------------------------------
	// Expansion UpdateItemRender
	// ------------------------------------------------------------
	protected void UpdateItemRender()
	{
		if ( m_ItemEntity && m_ItemPreview )
		{
			// m_ItemEntity.DisableSimulation( true );

			m_ItemPreview.SetItem( EntityAI.Cast(m_ItemEntity) );
			m_ItemPreview.SetModelPosition( Vector( 0, 0, 0.5 ) );
			m_ItemPreview.SetModelOrientation( vector.Zero );
			m_ItemPreview.SetView( EntityAI.Cast(m_ItemEntity).GetViewIndex() );
		}
	}

	// ------------------------------------------------------------
	// Expansion SetTrader
	// ------------------------------------------------------------
	void SetTrader( ExpansionTraderBase trader )
	{
		m_Trader = trader;
		m_MarketZone = m_Trader.GetTraderZone();
	}

	// ------------------------------------------------------------
	// Expansion UpdateFields
	// ------------------------------------------------------------
	void UpdateFields()
	{
		if ( !m_MarketItem )
			return;

		m_AmountInInventory = m_MarketModule.GetAmountInInventory( m_MarketItem, m_MarketModule.LocalGetEntityInventory() );
		string clsName = m_MarketItem.ClassName;
		clsName.ToLower();
		m_Stock = m_MarketModule.GetClientZone().GetStock( clsName, true );

		// Print( "ExpansionMarketUIItem::UpdateFields - m_Stock: " + m_Stock );
		
		m_ItemInfoPanel.Show( false );
		
		if ( m_Stock >= 0 )
		{
			if ( m_Stock == 0 )
			{
				m_ItemStockText.SetText( "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK" );
			} else
			{
				m_ItemStockText.SetText( "" + m_Stock + " #STR_EXPANSION_MARKET_ITEM_INSTOCK");
			}
		} else
		{			
			if ( m_Stock == -1 )
			{
				m_ItemStockText.SetText( "#STR_EXPANSION_MARKET_ITEM_INSTOCK" );
			} else
			{
				m_ItemStockText.SetText( "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK" );
			}
		}
		
		m_ItemStockText.SetText( "" + m_Stock + " #STR_EXPANSION_MARKET_ITEM_INSTOCK");
		
		// Market stock coloring
		if ( m_Stock >= 0) 
		{
			if ( m_Stock == 0 )
				m_ItemStockText.SetColor( MARKET_NO_STOCK_COLOR );
				
			if ( m_Stock >= 1 )
				m_ItemStockText.SetColor( MARKET_STOCK_LOW_COLOR );
				
			if ( m_Stock >= 50 )
				m_ItemStockText.SetColor( MARKET_STOCK_NORMAL_COLOR );
			
			//if ( m_Stock >= 100 )
			//	m_ItemStockText.SetColor( MARKET_STOCK_HIGH_COLOR );
		}

		m_PlayerStockText.SetText( m_AmountInInventory.ToString() + " #STR_EXPANSION_MARKET_ITEM_ONPLAYER" );
		
		// Player stock coloring
		if ( m_AmountInInventory >= 0 )
		{
			if ( m_AmountInInventory == 0 )
				m_PlayerStockText.SetColor( MARKET_NO_STOCK_COLOR );
				
			if ( m_AmountInInventory >= 1 )
				m_PlayerStockText.SetColor( MARKET_STOCK_LOW_COLOR );
				
			if ( m_AmountInInventory >= 50 )
				m_PlayerStockText.SetColor( MARKET_STOCK_NORMAL_COLOR );
			
			//if ( m_AmountInInventory >= 100 )
			//	m_PlayerStockText.SetColor(MARKET_STOCK_HIGH_COLOR);
		} else if (m_AmountInInventory == -1)
		{
			m_PlayerStockText.SetText( "CAN'T SELL" );
			m_PlayerStockText.SetColor( MARKET_CANT_SELL_COLOR );
			
			m_ItemInfoPanel.Show( true );
		}
	}

	// ------------------------------------------------------------
	// Expansion Widget GetLayoutRoot
	// ------------------------------------------------------------
	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}
	
	// ------------------------------------------------------------
	// Expansion void SetMarketMenu
	// ------------------------------------------------------------
	void SetMarketMenu( ref ExpansionMarketMenu menu )
	{
		m_MarketMenu = menu;
	}
	
	// ------------------------------------------------------------
	// Expansion String GetItemClassName
	// ------------------------------------------------------------
	string GetItemClassName()
	{
		if ( m_MarketItem )
		{
			return m_MarketItem.ClassName;
		}

		return "";
	}
	
	// ------------------------------------------------------------
	// Override bool OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( m_MarketMenu.IsLoading() )
		{
			return true;
		}

		if ( w == m_MainButton && m_MarketMenu )
		{
			m_MarketMenu.SetSelectedItem( this );
		}
		
		return super.OnClick(w, x, y, button);
	}

	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{	
		if (w == m_MainButton)
		{
			m_ItemBackground.SetColor(ARGB(255,77,85,86));
			m_ItemPreviewContent.SetColor(ARGB(140,26,26,26));
			m_ItemHighlightPanel.Show(true);
			
			return true;
		}
		
		if (m_ItemInfoPanel.IsVisible() && w == m_ItemInfoButton)
		{
			m_ItemTooltip.ShowTooltip();
			return true;
		}

	 	return super.OnMouseEnter(w, x, y);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		if (w == m_MainButton)
		{
			m_ItemBackground.SetColor(ARGB(255,127,140,141));
			m_ItemPreviewContent.SetColor(ARGB(140,0,0,0));
			m_ItemHighlightPanel.Show(false);

			return true;
		}
		
		if (m_ItemInfoPanel.IsVisible() && w == m_ItemInfoButton)
		{
			m_ItemTooltip.HideTooltip();
			return true;
		}
		
	 	return super.OnMouseLeave(w, enterW, x, y);
	}

	// ------------------------------------------------------------
	// Expansion Int GetTraderStock
	// ------------------------------------------------------------
	int GetTraderStock()
	{
		return m_Stock;
	}
	
	// ------------------------------------------------------------
	// Expansion Int GetPlayerStock
	// ------------------------------------------------------------
	int GetPlayerStock()
	{
		return m_AmountInInventory;
	}
	
	// ------------------------------------------------------------
	// Expansion Float GetSellPrice
	// ------------------------------------------------------------
	float GetSellPrice()
	{
		return m_SellPrice;
	}
	
	// ------------------------------------------------------------
	// Expansion Float GetBuyPrice
	// ------------------------------------------------------------
	float GetBuyPrice()
	{
		return m_BuyPrice;
	}
}