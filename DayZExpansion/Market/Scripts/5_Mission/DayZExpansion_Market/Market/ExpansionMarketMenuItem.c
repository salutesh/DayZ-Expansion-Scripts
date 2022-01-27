/**
 * ExpansionMarketMenuItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItem: ExpansionScriptView
{
	ref ExpansionMarketMenuItemController m_ItemController;
	ref ExpansionMarketItem m_Item;
	ref ExpansionMarketItem m_Variant;
	ref ExpansionMarketModule m_MarketModule;
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuItemTooltip m_Tooltip;
	ref ExpansionItemTooltip m_ItemTooltip;
	
	EntityAI m_Object;
	int m_CurrentSelectedSkinIndex = -1;

	protected ButtonWidget market_item_button;
	protected TextWidget market_item_header_text;
	protected TextWidget market_item_header_text_small;
	protected TextWidget market_item_header_text_verysmall;
	protected TextWidget market_item_header_text_smallerthensmall;
	protected TextWidget market_item_info_stock;
	protected TextWidget market_item_info_player_stock;
	protected Widget market_item_background;
	protected Widget market_item_info_content;
	protected ButtonWidget market_item_info_button;
	protected ImageWidget market_item_info_icon;
	protected ImageWidget market_item_info_buy_price_icon;
	protected ImageWidget market_item_header_background;
	protected ImageWidget market_item_info_stock_background;
	protected ImageWidget market_item_info_price_background;
	protected TextWidget market_item_info_buy_price;
	protected Widget market_item_fastbuy;
	protected Widget market_item_fastsell;
	protected Widget market_item_info_sell_price_panel;
	
	protected vector m_ItemPreviewOrientation = vector.Zero;
	protected int m_ItemPreviewRotationX = 0;
	protected int m_ItemPreviewRotationY = 0;
	protected bool m_IncludeAttachments = true;  //! Include attachments for price calc & purchase
	
	int m_PlayerStock = 0;
	int m_ItemStock = 0;
	int m_BuyPrice = 0;
	int m_SellPrice = 0;
	
	bool m_CanBuy;
	bool m_CanSell;

	// ------------------------------------------------------------
	// ExpansionMarketMenuItem Constructor
	// ------------------------------------------------------------	
	void ExpansionMarketMenuItem(ExpansionMarketMenu menu, ExpansionMarketItem item)
	{
		m_Item = item;
		m_MarketMenu = menu;
		
		if (!m_ItemController)
			m_ItemController = ExpansionMarketMenuItemController.Cast(GetController());
		
		if (!m_MarketModule)
			m_MarketModule = ExpansionMarketModule.Cast(GetModuleManager().GetModule(ExpansionMarketModule));
		
		market_item_header_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_small.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_verysmall.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_smallerthensmall.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_info_stock.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		market_item_header_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
		market_item_info_stock_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		market_item_info_price_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		
		if (GetExpansionSettings().GetMarket().CurrencyIcon != "")
		{
			m_ItemController.CurrencyIcon = GetExpansionSettings().GetMarket().CurrencyIcon;
			m_ItemController.NotifyPropertyChanged("CurrencyIcon");
		}
		
		SetView();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem Destructor
	// ------------------------------------------------------------
	void ~ExpansionMarketMenuItem()
	{	
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
		
		DestroyTooltip();
		DestroyItemTooltip();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetLayoutFile
	// ------------------------------------------------------------	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_item_element.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetControllerType
	// ------------------------------------------------------------		
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemController;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem SetVariant
	// ------------------------------------------------------------	
	void SetVariant(string className = "")
	{
		TStringArray currentAttachments = GetMarketItem().SpawnAttachments;

		if (className)
			m_Variant = GetExpansionSettings().GetMarket().GetItem(className);
		else
			m_Variant = NULL;

		//! If item manager is shown, set attachments of the current item to previous attachments
		if (m_MarketMenu.GetMarketMenuItemManager().IsVisible())
			GetMarketItem().SpawnAttachments = currentAttachments;

		DestroyTooltip();
		SetView();

		m_MarketMenu.SetItemInfo(this, false);

		if (m_MarketMenu.GetMarketMenuItemManager().IsVisible())
			m_MarketMenu.GetMarketMenuItemManager().RefreshPreview();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem SetExpansionSkin
	// ------------------------------------------------------------
	void SetExpansionSkin(int skinIndex)
	{
		m_CurrentSelectedSkinIndex = skinIndex;
		
		if (skinIndex > -1 && m_Object)
		{
			if (m_Object.IsInherited(CarScript))
			{
				CarScript car = CarScript.Cast(m_Object);
				if (car)
					car.ExpansionSetSkin(skinIndex);
			}
			else if (m_Object.IsInherited(ItemBase))
			{
				ItemBase item = ItemBase.Cast(m_Object);
				if (item)
					item.ExpansionSetSkin(skinIndex);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateSelectedVariantOrSkin
	// ------------------------------------------------------------
	void UpdateSelectedVariantOrSkin(string className, int skinIndex)
	{
		if (className)
		{
			SetVariant(className);
			SetExpansionSkin(-1);
		}
		else
		{
			if (m_Variant)
				SetVariant("");
			SetExpansionSkin(skinIndex);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem SetView
	// ------------------------------------------------------------		
	void SetView()
	{	
		if (!m_ItemController)
			return;
		
		if (!m_MarketModule)
			return;
		
		market_item_info_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));

		string previewClassName = m_MarketMenu.GetPreviewClassName(GetMarketItem().ClassName, true);
		string itemDisplayName = m_MarketMenu.GetDisplayName(previewClassName);
		string translate = Widget.TranslateString(itemDisplayName);
		int nameLength = translate.Length();
		
		if (nameLength > 15)
		{
			market_item_header_text_small.Show(true);
			market_item_header_text.Show(false);
		}
		else if (nameLength >= 20)
		{
			market_item_header_text_verysmall.Show(true);
			market_item_header_text.Show(false);
		}
		else if (nameLength >= 25)
		{
			market_item_header_text_smallerthensmall.Show(true);
			market_item_header_text.Show(false);
		}
		
		m_ItemController.ItemName = itemDisplayName;
		m_ItemController.NotifyPropertyChanged("ItemName");
		
		UpdatePreviewObject();
		
		m_CanBuy = GetMarketMenu().GetMarketTrader().CanBuyItem(GetMarketItem().ClassName);
		m_CanSell = GetMarketMenu().GetMarketTrader().CanSellItem(GetMarketItem().ClassName);

		UpdateView();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem UpdatePreviewObject
	// ------------------------------------------------------------		
	void UpdatePreviewObject()
	{
		string previewClassName = m_MarketMenu.GetPreviewClassName(GetMarketItem().ClassName);

		if (m_Object)
		{
			GetGame().ObjectDelete(m_Object);
		}
		
		if (!GetGame().IsKindOf(previewClassName, "DZ_LightAI"))
			m_Object = EntityAI.Cast(GetGame().CreateObjectEx(previewClassName, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
		else
			m_Object = NULL;
		
		if (m_Object)
		{
			if (m_Object.IsInherited(TentBase))
			{
				TentBase tent;
				Class.CastTo(tent, m_Object);
				tent.Pack(false);
			}
			
			Transport transportEntity;
			if (Class.CastTo(transportEntity, m_Object))
			{
				dBodyActive(m_Object, ActiveState.INACTIVE);
				dBodyDynamic(m_Object, false);
				transportEntity.DisableSimulation(true);
			}
			
			if (m_IncludeAttachments)
			{
				#ifdef EXPANSIONMODMARKET_DEBUG
				EXLogPrint("ExpansionMarketMenuItem::UpdatePreviewObject - Attachments count:" + GetMarketItem().SpawnAttachments.Count() + " for item " + previewClassName);
				#endif
				SpawnAttachments(GetMarketItem(), m_Object);
			}

			if (m_Object.HasSelection("antiwater"))
				m_Object.HideSelection("antiwater");

			BaseBuildingBase baseBuilding = BaseBuildingBase.Cast(m_Object);
			if (baseBuilding && baseBuilding.CanUseConstruction())
			{
				bool isSupportedBB = baseBuilding.GetType() == "Fence" || baseBuilding.GetType() == "Watchtower" || baseBuilding.GetType() == "TerritoryFlag";
				#ifdef EXPANSIONMOD
				isSupportedBB |= baseBuilding.IsInherited(ExpansionBaseBuilding);
				#endif
				if (isSupportedBB)
				{
					Construction construction = baseBuilding.GetConstruction();
					construction.Init();
					construction.ExpansionBuildFull();
				}
			}

			if (!m_Variant)
				SetExpansionSkin(m_CurrentSelectedSkinIndex);
		}
		
		m_ItemController.Preview = m_Object;
		m_ItemController.NotifyPropertyChanged("Preview");
	}
	
	//! Spawn attachments and attachments on attachments
	void SpawnAttachments(ExpansionMarketItem item, EntityAI parent, int level = 0)
	{
		foreach (string attachmentName: item.SpawnAttachments)
		{
			EntityAI attachmentEntity = ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, m_CurrentSelectedSkinIndex);
			if (attachmentEntity && level < 3)
			{
				ExpansionMarketItem attachment = GetExpansionSettings().GetMarket().GetItem(attachmentName, false);
				if (attachment)
					SpawnAttachments(attachment, attachmentEntity, level + 1);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuItem UpdateView
	// ------------------------------------------------------------		
	void UpdateView()
	{
		#ifdef EXPANSIONMODMARKET_DEBUG
		EXLogPrint("ExpansionMarketMenuItem::UpdateView - Start");
		#endif
		
		if (!m_ItemController)
			return;
		
		if (!m_MarketModule)
			return;
				
		if (GetMarketItem().IsStaticStock())
			m_ItemStock = 1;
		else
			m_ItemStock = m_MarketModule.GetClientZone().GetStock(GetMarketItem().ClassName);
		m_PlayerStock = m_MarketModule.GetAmountInInventory(GetMarketItem(), m_MarketModule.LocalGetEntityInventory());
		
		UpdatePrices();
			
		if (m_CanBuy)
		{		
			if (m_ItemStock > 0)
			{
				if (GetMarketItem().IsStaticStock())
					m_ItemController.ItemMarketStock = "∞";
				else 
					m_ItemController.ItemMarketStock = m_ItemStock.ToString() + " IN STOCK";
			}
			else
			{
				if (GetMarketItem().MaxStockThreshold > 0)
					m_ItemController.ItemMarketStock = "OUT OF STOCK";
				else
					m_ItemController.ItemMarketStock = "N/A";
			}
		}
		else
		{
			m_ItemController.ItemMarketStock = "CAN'T BUY";
		}
		
		m_ItemController.NotifyPropertyChanged("ItemMarketStock");
		
		if (m_CanSell)
		{
			if (m_PlayerStock >= 0)
				m_ItemController.ItemPlayerStock = m_PlayerStock.ToString() + " ON YOU";
			else
				m_ItemController.ItemPlayerStock = "CAN'T SELL";
		}
		else
		{
			m_ItemController.ItemPlayerStock = "CAN'T SELL";
		}

		m_ItemController.NotifyPropertyChanged("ItemPlayerStock");
				
		UpdateButtons();

		SetMarketStockColor();
		SetPlayerStockColor();
		
		if (m_Tooltip)
			DestroyTooltip();
		
		CreateTooltip();
		
		GetMarketItem().m_UpdateView = false;

		#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("ExpansionMarketMenuItem::UpdateView - End");
		#endif
	}
	
	void UpdateButtons()
	{
		market_item_fastbuy.Show(m_CanBuy && m_ItemStock > 0 && m_BuyPrice > -1 && m_MarketModule.GetPlayerWorth() >= m_BuyPrice);
		market_item_fastsell.Show(m_CanSell && m_PlayerStock > 0 && m_SellPrice > -1);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuItem UpdatePrices
	// ------------------------------------------------------------	
	void UpdatePrices()
	{
		//! Buy price
		if (m_CanBuy)
		{
			int price = 0;
			//! Can't pass in GetMarketItem() to FindPriceOfPurchase directly, causes NULL pointer. Fuck you EnforceScript.
			ExpansionMarketItem item = GetMarketItem();
			m_MarketModule.FindPriceOfPurchase(item, m_MarketModule.GetClientZone(), GetMarketMenu().GetMarketTrader(), 1, price, GetIncludeAttachments());
			m_BuyPrice = price;
			m_ItemController.ItemBuyPrice = ExpansionStatic.IntToCurrencyString(m_BuyPrice, ",", true);
		}
		else
		{
			m_BuyPrice = -1;
			m_ItemController.ItemBuyPrice = "";
		}

		market_item_info_buy_price_icon.Show(m_BuyPrice > -1);

		//! Sell price
		if (m_CanSell)
		{
			array<EntityAI> items;
			if (m_PlayerStock != 0)
			{
				//! Player has the item
				items = m_MarketModule.LocalGetEntityInventory();
			}
			else
			{
				//! Player doesn't have the item. Use preview item.
				items = new array<EntityAI>;
				EntityAI previewEntity;
				if (Class.CastTo(previewEntity, m_Object))
					items.Insert(previewEntity);
			}

			ExpansionMarketSell marketSell = new ExpansionMarketSell;
			marketSell.Item = GetMarketItem();
			marketSell.Trader = GetMarketMenu().GetTraderObject();
			m_MarketModule.FindSellPrice(PlayerBase.Cast(GetGame().GetPlayer()), items, m_ItemStock, 1, marketSell, m_PlayerStock != 0 || m_IncludeAttachments);
			m_SellPrice = marketSell.Price;

			m_ItemController.ItemSellPrice = ExpansionStatic.IntToCurrencyString(m_SellPrice, ",", true);
		}
		else
		{
			m_SellPrice = -1;
			m_ItemController.ItemSellPrice = "";
		}

		m_ItemController.NotifyPropertiesChanged({"ItemSellPrice", "ItemBuyPrice"});
		
		market_item_info_sell_price_panel.Show(m_SellPrice > -1);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem SetMarketStockColor
	// ------------------------------------------------------------		
	void SetMarketStockColor()
	{
		market_item_info_stock.SetColor(GetMarketStockColor());
	}
	
	int GetMarketStockColor(bool check = true)
	{
		int percent = 100;
		if (GetMarketItem().MaxStockThreshold == 0)
		{
			percent = 0;
		}
		else if (!GetMarketItem().IsStaticStock())
		{
			int itemStock = m_MarketModule.GetClientZone().GetStock(GetMarketItem().ClassName);
			percent = Math.Round((itemStock / GetMarketItem().MaxStockThreshold) * 100);
		}

		if (!percent || !m_CanBuy || !check)
		{
			//! Color red
			return ARGB(255, 192, 57, 43);
		}
		else if (percent < 25)
		{
			//! Color orange
			return ARGB(255, 243, 156, 18);
		}
		else if (percent < 50)
		{
			//! Color yellow
			return ARGB(255, 243, 223, 0);
		}
		else if (percent < 75)
		{
			//! Color yellowgreen
			return ARGB(255, 186, 223, 59);
		}

		//! Color green
		return ARGB(255, 39, 174, 96);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuItem SetPlayerStockColor
	// ------------------------------------------------------------	
	void SetPlayerStockColor()
	{
		market_item_info_player_stock.SetColor(GetPlayerStockColor());
	}
	
	int GetPlayerStockColor(bool check = true)
	{
		if (m_PlayerStock <= 0 || !m_CanSell || !check)
			return ARGB(255, 192, 57, 43);

		return ARGB(255, 41, 128, 185);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnItemButtonClick
	// ------------------------------------------------------------		
	void OnItemButtonClick()
	{
		m_MarketMenu.SetItemInfo(this);
		if (!m_MarketMenu.IsLoading())
			m_MarketMenu.RequestSelectedItem(ExpansionMarketMenuState.REQUESTING_SELECTED_ITEM, GetMarketItem().ClassName);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem CreateTooltip
	// ------------------------------------------------------------	
	void CreateTooltip()
	{
		if (!m_Tooltip)
		{
			m_Tooltip = new ExpansionMarketMenuItemTooltip(this);
			m_Tooltip.Hide();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem ShowInfoButton
	// ------------------------------------------------------------	
	void ShowInfoButton(bool state)
	{
		market_item_info_content.Show(state);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem DestroyTooltip
	// ------------------------------------------------------------	
	void DestroyTooltip()
	{
		if (m_Tooltip)
		{
			ShowInfoButton(false);
			m_Tooltip.Hide();
			delete m_Tooltip;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem DestroyTooltip
	// ------------------------------------------------------------	
	void DestroyItemTooltip()
	{
		if (m_ItemTooltip)
		{
			m_ItemTooltip.Hide();
			delete m_ItemTooltip;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnHide
	// ------------------------------------------------------------	
	override void OnHide()
	{
		super.OnHide();
		
		DestroyTooltip();
		DestroyItemTooltip();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case market_item_button:
			market_item_button.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemButton"));
			if (!m_ItemTooltip && m_Object)
			{
				m_ItemTooltip = new ExpansionItemTooltip(m_Object);
				m_ItemTooltip.Show();
			}
			break;
		case market_item_info_button:
			if (m_Tooltip) m_Tooltip.Show();
			market_item_info_icon.SetColor(ARGB(255, 220, 220, 220));
			break;
		
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case market_item_button:
			market_item_button.SetColor(ARGB(220, 0, 0, 0));
			if (m_ItemTooltip) delete m_ItemTooltip;
			break;
		case market_item_info_button:
			if (m_Tooltip) m_Tooltip.Hide();
			market_item_info_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetPreviewObject
	// ------------------------------------------------------------		
	Object GetPreviewObject()
	{
		return m_Object;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetMarketItem
	// ------------------------------------------------------------
	//! Return variant (if set) or base item
	ExpansionMarketItem GetMarketItem()
	{
		if (m_Variant)
			return m_Variant;
		else
			return m_Item;
	}
	
	ExpansionMarketItem GetBaseItem()
	{
		return m_Item;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetMarketMenu
	// ------------------------------------------------------------
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetTooltip
	// ------------------------------------------------------------
	ExpansionMarketMenuItemTooltip GetTooltip()
	{
		return m_Tooltip;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem GetTooltip
	// ------------------------------------------------------------	
	ExpansionMarketItem GetCurrentVariant()
	{
		return m_Variant;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetIncludeAttachments
	// ------------------------------------------------------------
	void SetIncludeAttachments(bool state)
	{
		m_IncludeAttachments = state;
	}

	bool GetIncludeAttachments()
	{
		return m_IncludeAttachments;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnFastBuyButtonClick
	// ------------------------------------------------------------
	void OnFastBuyButtonClick()
	{
		m_MarketMenu.SetItemInfo(this);
		m_MarketMenu.OnConfirmBuyButtonClick();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuItem OnFastBuyButtonClick
	// ------------------------------------------------------------
	void OnFastSellButtonClick()
	{
		m_MarketMenu.SetItemInfo(this);
		m_MarketMenu.OnConfirmSellButtonClick();
	}
};

class ExpansionMarketMenuItemController: ExpansionViewController
{
	string ItemName;
	string ItemMarketStock;
	string ItemPlayerStock;
	string ItemBuyPrice;
	string ItemSellPrice;
	Object Preview;
	string CurrencyIcon;
};