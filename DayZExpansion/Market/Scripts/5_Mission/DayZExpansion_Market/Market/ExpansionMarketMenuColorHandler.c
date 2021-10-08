/**
 * ExpansionMarketMenuColorHandler.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuColorHandler
{
	protected Widget m_LayooutRoot;
	protected ImageWidget vignette;
	protected Widget market_header;
	protected Widget market_menu_info_header;	
	protected Widget market_footer;
	protected ImageWidget market_filter_background;
	protected ImageWidget player_info_background;
	protected ImageWidget market_footer_categories_background;
	protected ImageWidget market_icon;
	protected Widget menu_info_background;
	protected TextWidget market_item_header_text;
	protected ImageWidget market_item_info_attachments_icon;
	protected ImageWidget market_item_header_panel;
	protected ButtonWidget market_item_amount_button_left;
	protected ImageWidget market_item_amount_left_icon;
	protected ButtonWidget market_item_amount_button;
	protected ButtonWidget market_item_amount_button_right;
	protected ImageWidget market_item_amount_right_icon;
	protected Widget market_item_sell_panel;
	protected ButtonWidget market_item_sell_button;
	protected Widget market_item_buy_panel;
	protected ButtonWidget market_item_buy_button;

	protected EditBoxWidget market_filter_box;
	protected TextWidget player_info_header_text;
	protected TextWidget player_total;
	protected RichTextWidget market_item_description;
	protected TextWidget market_item_info_stock_player;
	protected TextWidget market_item_info_stock;
	protected TextWidget market_item_amount_text;
	protected TextWidget market_item_sell_price_text;
	protected ImageWidget market_item_sell_price_icon;
	protected TextWidget market_item_buy_price_text;
	protected ImageWidget market_item_buy_price_icon;
	protected TextWidget market_item_sell_text;
	protected TextWidget market_item_buy_text;
	protected TextWidget market_item_info_attachments_lable;
	protected CheckBoxWidget market_item_info_attachments_checkbox;
	protected TextWidget market_footer_categories_label;
	protected Widget market_header_options;
	protected Widget dropdown_selector;
	protected Widget skin_selector;
	protected ScrollWidget skin_selector_container;
	
	void ExpansionMarketMenuColorHandler(Widget layoutRoot)
	{
		m_LayooutRoot = layoutRoot;
		
		SetColors();
	}
	
	void SetColors()
	{
		//! Menu vignette
		vignette = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("vignette"));
		vignette.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorVignette));
		
		//! Menu header
		market_header = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_header"));
		market_header.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
		
		//! Trader type icon
		market_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_icon"));
		market_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorMarketIcon));
		
		//!  Search filter text
		market_filter_box = EditBoxWidget.Cast(m_LayooutRoot.FindAnyWidget("market_filter_box"));
		market_filter_box.SetTextColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Menu footer
		market_footer = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_footer"));
		market_footer.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
		
		//! Search bar background
		market_filter_background = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_filter_background"));
		market_filter_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
			
		//! Toggle categories button lable background
		market_footer_categories_background = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_footer_categories_background"));
		market_footer_categories_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
		
		//! Player info header lable background
		player_info_background = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("player_info_background"));
		player_info_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
				
		//! Info section background
		menu_info_background = Widget.Cast(m_LayooutRoot.FindAnyWidget("menu_info_background"));
		menu_info_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorInfoSectionBackground));
		
		//! Info section header
		market_menu_info_header = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_menu_info_header"));
		market_menu_info_header.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
		
		//! Item info section label background
		market_item_header_panel = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_header_panel"));
		market_item_header_panel.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
			
		//!  Item info section description text
		market_item_header_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_header_text"));
		market_item_header_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Attackments option text
		market_item_info_attachments_lable = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_info_attachments_lable"));
		market_item_info_attachments_lable.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Attackments option checkbox
		market_item_info_attachments_checkbox = CheckBoxWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_info_attachments_checkbox"));
		market_item_info_attachments_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorCheckboxes));
				
		//! Player currecy description text
		player_info_header_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("player_info_header_text"));
		player_info_header_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Player currecy value text
		player_total = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("player_total"));
		player_total.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Attachments info icon
		market_item_info_attachments_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_info_attachments_icon"));
		market_item_info_attachments_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoAttachments));
		
		//! Item description text
		market_item_description = RichTextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_description"));
		market_item_description.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Player stock text
		market_item_info_stock_player = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_info_stock_player"));
		market_item_info_stock_player.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Quantity text
		market_item_amount_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_text"));
		market_item_amount_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Trader stock text
		market_item_info_stock = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_info_stock"));
		market_item_info_stock.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Decrease quantity button
		market_item_amount_button_left = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_button_left"));
		market_item_amount_button_left.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorDecreaseQuantityButton));
		
		//! Decrease quantity icon
		market_item_amount_left_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_left_icon"));
		market_item_amount_left_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorDecreaseQuantityIcon));
		
		//! Quantity button
		market_item_amount_button = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_button"));
		market_item_amount_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSetQuantityButton));
		
		//! Increase quantity button
		market_item_amount_button_right = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_button_right"));
		market_item_amount_button_right.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorIncreaseQuantityButton));
		
		//! Increase quantity icon
		market_item_amount_right_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_amount_right_icon"));
		market_item_amount_right_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorIncreaseQuantityIcon));
		
		//! Sell panel
		market_item_sell_panel = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_item_sell_panel"));
		market_item_sell_panel.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSellPanel));
		
		//! Sell button
		market_item_sell_button = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_sell_button"));
		market_item_sell_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSellButton));
		
		//! Sell button text
		market_item_sell_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_sell_text"));
		market_item_sell_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText)); 
		
		//! Buy panel
		market_item_buy_panel = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_item_buy_panel"));
		market_item_buy_panel.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorBuyPanel));
		
		//! Buy button
		market_item_buy_button = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_buy_button"));
		market_item_buy_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorBuyButton));
		
		//! Buy button text
		market_item_buy_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_buy_text"));
		market_item_buy_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText)); 
		
		//! Sell price text
		market_item_sell_price_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_sell_price_text"));
		market_item_sell_price_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Sell price icon
		market_item_sell_price_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_sell_price_icon"));
		market_item_sell_price_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Buy price text
		market_item_buy_price_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_buy_price_text"));
		market_item_buy_price_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Buy price icon
		market_item_buy_price_icon = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("market_item_buy_price_icon"));
		market_item_buy_price_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Toggle categories button text
		market_footer_categories_label = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("market_footer_categories_label"));
		market_footer_categories_label.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		
		//! Menu options
		market_header_options = Widget.Cast(m_LayooutRoot.FindAnyWidget("market_header_options"));
		market_header_options.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
		
		//! Dropdown options selector
		dropdown_selector = Widget.Cast(m_LayooutRoot.FindAnyWidget("dropdown_selector"));
		dropdown_selector.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
		
		//! Skin selector
		skin_selector = Widget.Cast(m_LayooutRoot.FindAnyWidget("skin_selector"));
		skin_selector.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
		skin_selector_container = ScrollWidget.Cast(m_LayooutRoot.FindAnyWidget("skin_selector_container"));
		skin_selector_container.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorHeaders));
	}
};