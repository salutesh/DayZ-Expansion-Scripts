/**
 * ExpansionMarketMenuItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItem: ExpansionScriptView
{
	protected ref ExpansionMarketMenuItemController m_ItemController;
	protected ref ExpansionMarketItem m_Item;
	protected ref ExpansionMarketItem m_Variant;
	protected ref ExpansionMarketModule m_MarketModule;
	protected ref ExpansionMarketMenu m_MarketMenu;
	protected ref ExpansionMarketMenuItemTooltip m_Tooltip;
	protected ExpansionItemTooltip m_ItemTooltip;
	protected string m_ItemSortName;

	protected int m_MenuIdx;
	
	protected EntityAI m_Object;
	protected int m_CurrentSelectedSkinIndex = -1;

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
	protected ImageWidget market_item_info_sell_price_icon;
	protected TextWidget market_item_info_sell_price;
	
	protected Widget market_item_overlay;
	
	protected vector m_ItemPreviewOrientation = vector.Zero;
	protected int m_ItemPreviewRotationX = 0;
	protected int m_ItemPreviewRotationY = 0;
	protected bool m_IncludeAttachments = true;  //! Include attachments for price calc & purchase
	
	protected int m_PlayerStock = 0;
	protected int m_ItemStock = 0;
	protected int m_BuyPrice = 0;
	protected int m_SellPrice = 0;
	
	protected bool m_CanBuy;
	protected bool m_CanSell;
	protected bool m_HasRepBuy = true;
	protected bool m_HasRepSell = true;

	void ExpansionMarketMenuItem(ExpansionMarketMenu menu, ExpansionMarketItem item)
	{
		m_Item = item;
		m_MarketMenu = menu;
		
		if (!m_ItemController)
			m_ItemController = ExpansionMarketMenuItemController.Cast(GetController());
		
		if (!m_MarketModule)
			m_MarketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
		
		market_item_header_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_small.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_verysmall.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_header_text_smallerthensmall.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		market_item_info_stock.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		market_item_header_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
		market_item_info_stock_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		market_item_info_price_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		
		SetView();
	}

	void ~ExpansionMarketMenuItem()
	{	
		if (m_Object && IsLocalPreview())
			g_Game.ObjectDelete(m_Object);
		
		DestroyTooltip();
		DestroyItemTooltip();
	}

	bool IsLocalPreview()
	{
		int low, high;
		m_Object.GetNetworkID(low, high);
		if (low == 0 && high == 0)
			return true;

		return false;
	}

	bool IsPreview(EntityAI preview)
	{
		if (m_Object == preview)
			return true;

		return false;
	}

	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_item_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemController;
	}

	void SetVariant(string className = "")
	{
		TStringArray currentAttachments = GetMarketItem().SpawnAttachments;

		if (className)
			m_Variant = ExpansionMarketCategory.GetGlobalItem(className);
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

	void SetExpansionSkin(int skinIndex)
	{
		m_CurrentSelectedSkinIndex = skinIndex;
		
		if (skinIndex > -1 && m_Object)
		{
			auto vehicle = ExpansionVehicle.Get(m_Object);
			if (vehicle)
			{
				vehicle.SetSkin(skinIndex);
			}
			else if (m_Object.IsInherited(ItemBase))
			{
				ItemBase item = ItemBase.Cast(m_Object);
				if (item)
					item.ExpansionSetSkin(skinIndex);
			}
		}
	}

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

		itemDisplayName.ToLower();
		m_ItemSortName = itemDisplayName;
		
		UpdatePreviewObject();
		
		m_CanBuy = GetMarketMenu().GetMarketTrader().CanBuyItem(GetMarketItem().ClassName);
		m_CanSell = GetMarketMenu().GetMarketTrader().CanSellItem(GetMarketItem().ClassName);
	
		OnSetView();

		UpdateView();
	}

	//! This is here for overrides
	void OnSetView()
	{
	#ifdef EXPANSIONMODHARDLINE
		auto settings = GetExpansionSettings().GetHardline();
		if (settings.UseReputation && (settings.UseItemRarityForMarketPurchase || settings.UseItemRarityForMarketSell))
		{
			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			ExpansionMarketItem item = GetMarketItem();
			bool hasRep = m_MarketModule.HasRepForItemRarity(player, item);

			if (settings.UseItemRarityForMarketPurchase)
				m_HasRepBuy = hasRep;
			
			if (settings.UseItemRarityForMarketSell)
				m_HasRepSell = hasRep;
		}
		
	#ifdef EXPANSIONMODMARKET_DEBUG
		EXLogPrint(string.Format("%1::SetView - Has rep for item %2: Has rep to buy: %3 | Has rep to sell: %4 | Can buy: %5 | Can sell: %6", ToString(), GetMarketItem().ClassName, m_HasRepBuy.ToString(), m_HasRepSell.ToString(), m_CanBuy.ToString(), m_CanSell.ToString()));
	#endif
	#endif
	}
	
	void UpdatePreviewObject()
	{
		ExpansionMarketItem item = GetMarketItem();

		if (item.m_PreviewEntity && !item.m_PreviewEntity.IsSetForDeletion())
			m_Object = item.m_PreviewEntity;
		else
			CreatePreviewObject();

		m_ItemController.Preview = m_Object;
		m_ItemController.NotifyPropertyChanged("Preview");
	}
	
	void CreatePreviewObject()
	{
		ExpansionMarketItem item = GetMarketItem();

		if (m_Object && !IsLocalPreview())
			m_Object = null;

		m_MarketMenu.CreatePreviewObjectEx(item, m_Object);
		
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
				EXLogPrint("ExpansionMarketMenuItem::UpdatePreviewObject - Attachments count:" + item.SpawnAttachments.Count() + " for item " + item.ClassName);
			#endif
				SpawnAttachments(item, m_Object);
			}

			if (m_Object.HasSelection("antiwater"))
				m_Object.HideSelection("antiwater");

			Construction.ExpansionBuildFullIfSupported(m_Object);

			if (!m_Variant)
				SetExpansionSkin(m_CurrentSelectedSkinIndex);
		}
	}
	
	//! Spawn attachments and attachments on attachments
	void SpawnAttachments(ExpansionMarketItem item, EntityAI parent, int level = 0)
	{
		foreach (string attachmentName: item.SpawnAttachments)
		{
			ExpansionMarketItem attachment = ExpansionMarketCategory.GetGlobalItem(attachmentName, false);

			EntityAI attachmentEntity = ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, m_CurrentSelectedSkinIndex);
			if (attachmentEntity && level < 3)
			{
				if (attachment)
					SpawnAttachments(attachment, attachmentEntity, level + 1);
			}
		}
	}

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

		bool canOnlyBuy = m_CanBuy && !m_CanSell;
		bool canOnlySell = !m_CanBuy && m_CanSell;
		bool isOutOfStock;
		
		//! Market stock and buy
		if (m_CanBuy)
		{
			if (m_ItemStock > 0)
			{
				if (GetMarketItem().IsStaticStock())
					m_ItemController.ItemMarketStock = "∞";
				else 
					m_ItemController.ItemMarketStock = m_ItemStock.ToString() + " #STR_EXPANSION_MARKET_ITEM_INSTOCK";
			}
			else
			{
				if (GetMarketItem().MaxStockThreshold > 0) 
				{
					isOutOfStock = true;
					m_ItemController.ItemMarketStock = "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK";
				}
				else
					m_ItemController.ItemMarketStock = "";
			}
		}
		else
		{
			if (m_CanBuy && GetMarketItem().MaxStockThreshold > 0 && m_ItemStock < 1) 
				isOutOfStock = !GetMarketItem().IsStaticStock();

			if (canOnlySell)
				m_ItemController.ItemMarketStock = "";
			else
				m_ItemController.ItemMarketStock = "#STR_EXPANSION_MARKET_ITEM_CANT_BUY";
		}
		
		m_ItemController.NotifyPropertyChanged("ItemMarketStock");
		
		//! Player stock and sell
		if (m_CanSell)
		{
			if (m_PlayerStock >= 0)
				m_ItemController.ItemPlayerStock = m_PlayerStock.ToString() + " #STR_EXPANSION_MARKET_ITEM_ONPLAYER";
			else
				m_ItemController.ItemPlayerStock = "#STR_EXPANSION_MARKET_ITEM_CANT_SELL";
		}
		else
		{
			if (canOnlyBuy)
				m_ItemController.ItemPlayerStock = "";
			else
				m_ItemController.ItemPlayerStock = "#STR_EXPANSION_MARKET_ITEM_CANT_SELL";
		}

		m_ItemController.NotifyPropertyChanged("ItemPlayerStock");
		
		bool shouldDisplayOverlay;
		if (canOnlyBuy)
			shouldDisplayOverlay = !m_HasRepBuy || isOutOfStock;
		else if (canOnlySell)
			shouldDisplayOverlay = !m_HasRepSell;
		else if (!m_HasRepSell && !m_HasRepBuy)
			shouldDisplayOverlay = true;
		
		if (shouldDisplayOverlay)
		{
			market_item_overlay.Show(true);
			if (isOutOfStock && canOnlyBuy)
				m_ItemController.OverlayText = "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK";
		#ifdef EXPANSIONMODHARDLINE
			else
				m_ItemController.OverlayText = string.Format("#STR_EXPANSION_MARKET_ITEM_REP (%1)", ExpansionStatic.FormatInt(GetMarketItem().m_RequiredRep));
		#endif
			m_ItemController.NotifyPropertyChanged("OverlayText");
		}
				
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
		bool showFastBuy;
		if (m_CanBuy && m_HasRepBuy && m_ItemStock > 0 && m_BuyPrice > -1 && m_MarketModule.GetPlayerWorth() >= m_BuyPrice)
			showFastBuy = true;

		market_item_fastbuy.Show(showFastBuy);

		bool showFastSell;
		if (m_CanSell && m_HasRepSell && m_PlayerStock > 0 && m_SellPrice > -1)
			showFastSell = true;

		market_item_fastsell.Show(showFastSell);
	}

	void UpdatePrices()
	{
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());

		//! Buy price
		if (m_CanBuy && m_HasRepBuy)
		{
			int price = 0;
			//! Can't pass in GetMarketItem() to FindPriceOfPurchaseEx directly, causes NULL pointer. Fuck you EnforceScript.
			ExpansionMarketItem item = GetMarketItem();
			m_MarketModule.FindPriceOfPurchaseEx(item, m_MarketModule.GetClientZone(), GetMarketMenu().GetMarketTrader(), player, 1, price, GetIncludeAttachments());
			m_BuyPrice = price;
			m_ItemController.ItemBuyPrice = m_MarketMenu.GetDisplayPrice(m_BuyPrice, true);
			m_ItemController.CurrencyBuyIcon = GetExpansionSettings().GetMarket().CurrencyIcon;
		}
		else
		{
			m_BuyPrice = -1;

			//! If both hasrepbuy and hasrepsell are false, hide currency info & price since the required rep will be shown on the overlay.
			//! Otherwise, show required rep instead of price
			if (m_HasRepBuy || !m_HasRepSell)
			{
				m_ItemController.ItemBuyPrice = "";
			}
			else
			{
				m_ItemController.ItemBuyPrice = ExpansionStatic.FormatInt(GetMarketItem().m_RequiredRep, true);
				m_ItemController.CurrencyBuyIcon = ExpansionMarketMenu.s_RepIcon;
			}
		}

		market_item_info_buy_price_icon.Show(m_ItemController.ItemBuyPrice != "");

		//! Sell price
		if (m_CanSell && m_HasRepSell)
		{
			array<EntityAI> items;
			if (m_PlayerStock != 0)
			{
				//! Player has the item
				items = m_MarketModule.LocalGetEntityInventory();
			}
			else
			{
				//! Player doesn't have the item
				items = new array<EntityAI>;
			}

			ExpansionMarketSell marketSell = new ExpansionMarketSell(player);
			marketSell.Item = GetMarketItem();
			marketSell.Trader = GetMarketMenu().GetTraderObject();
			bool includeAttachments;
			//! https://feedback.bistudio.com/T193384
			if (m_PlayerStock != 0 || m_IncludeAttachments)
				includeAttachments = true;
			m_MarketModule.FindSellPrice(player, items, m_ItemStock, 1, marketSell, includeAttachments);
			m_SellPrice = marketSell.Price;

			m_ItemController.ItemSellPrice = m_MarketMenu.GetDisplayPrice(m_SellPrice, true);
			m_ItemController.CurrencySellIcon = GetExpansionSettings().GetMarket().CurrencyIcon;
		}
		else
		{
			m_SellPrice = -1;

			//! If both hasrepbuy and hasrepsell are false, hide currency info & price since the required rep will be shown on the overlay.
			//! Otherwise, show required rep instead of price
			if (m_HasRepSell || !m_HasRepBuy)
			{
				m_ItemController.ItemSellPrice = "";
			}
			else
			{
				m_ItemController.ItemSellPrice = ExpansionStatic.FormatInt(GetMarketItem().m_RequiredRep, true);
				m_ItemController.CurrencySellIcon = ExpansionMarketMenu.s_RepIcon;
			}
		}

		m_ItemController.NotifyPropertiesChanged({"ItemSellPrice", "ItemBuyPrice", "CurrencySellIcon", "CurrencyBuyIcon"});
		
		market_item_info_sell_price_panel.Show(m_ItemController.ItemSellPrice != "");
	}
	
	void SetMarketStockColor()
	{
		market_item_info_stock.SetColor(GetMarketStockColor());

		int color;
		if (!m_HasRepBuy || m_MarketModule.GetPlayerWorth() < m_BuyPrice)
			color = GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorRequirementsNotMet");
		else
			color = GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText");

		market_item_info_buy_price.SetColor(color);
		market_item_info_buy_price_icon.SetColor(color);
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

		if (!percent || !check)
		{
			//! Color red
			return GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorRequirementsNotMet");
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

	void SetPlayerStockColor()
	{
		market_item_info_player_stock.SetColor(GetPlayerStockColor());

		int color;
		if (!m_HasRepSell)
			color = GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorRequirementsNotMet");
		else
			color = GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText");

		market_item_info_sell_price.SetColor(color);
		market_item_info_sell_price_icon.SetColor(color);
	}
	
	int GetPlayerStockColor(bool check = true)
	{
		if (m_PlayerStock <= 0 || !m_CanSell || !check)
			return GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorRequirementsNotMet");

		return GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorPlayerStock");
	}
	
	void OnItemButtonClick(ButtonCommandArgs args)
	{
		int button = args.GetMouseButton();
		bool buttonState = args.GetButtonState();
		
		if (button == MouseState.LEFT && buttonState)
		{
			m_MarketMenu.SetItemInfo(this);
			if (!m_MarketMenu.IsLoading())
				m_MarketMenu.RequestSelectedItem(ExpansionMarketMenuState.REQUESTING_SELECTED_ITEM, GetMarketItem().ClassName);
		}
	}

	void CreateTooltip()
	{
		if (!m_Tooltip)
		{
			m_Tooltip = new ExpansionMarketMenuItemTooltip(this);
			m_Tooltip.Hide();
		}
	}

	void ShowInfoButton(bool state)
	{
		market_item_info_content.Show(state);
	}

	void DestroyTooltip()
	{
		if (m_Tooltip)
		{
			ShowInfoButton(false);
			m_Tooltip.Destroy();
		}
	}

	void DestroyItemTooltip()
	{
		if (m_ItemTooltip)
		{
			m_ItemTooltip.Destroy();
		}
	}

	override void OnShow()
	{
		super.OnShow();
	}

	override void OnHide()
	{
		super.OnHide();
		
		DestroyTooltip();
		DestroyItemTooltip();
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && w == market_item_button)
		{
			if (button == MouseState.MIDDLE && m_Object)
			{
				MissionGameplay.InspectItem(m_MarketMenu, m_Object);
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == market_item_button)
			{
				market_item_button.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemButton"));
				if (!m_ItemTooltip && m_Object)
				{
					m_ItemTooltip = MissionGameplay.SetItemTooltip(m_Object);
				}
				
				return true;
			}
			else if (w == market_item_info_button)
			{
				if (m_Tooltip) 
					m_Tooltip.Show();
				
				market_item_info_icon.SetColor(ARGB(255, 220, 220, 220));
				return true;
			}
		}
		
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == market_item_button)
			{
				market_item_button.SetColor(ARGB(220, 0, 0, 0));
				if (m_ItemTooltip)
				{
					MissionGameplay.Expansion_DestroyItemTooltip();
					m_ItemTooltip = null;
				}
				return true;
			}
			else if (w == market_item_info_button)
			{
				if (m_Tooltip) 
					m_Tooltip.Hide();

				market_item_info_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));
				return true;
			}
		}
		
		return false;
	}
	
	Object GetPreviewObject()
	{
		return m_Object;
	}

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

	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}

	ExpansionMarketMenuItemTooltip GetTooltip()
	{
		return m_Tooltip;
	}

	ExpansionMarketItem GetCurrentVariant()
	{
		return m_Variant;
	}

	string GetItemDisplayName()
	{
		return m_ItemController.ItemName;
	}

	string GetItemSortName()
	{
		return m_ItemSortName;
	}

	int GetBuyPrice()
	{
		return m_BuyPrice;
	}

	string GetSortKey(ExpansionMarketMenuSortPriority sortPriority)
	{
		string pricePadded = ExpansionString.JustifyRight(m_BuyPrice.ToString(), 10, " ");

		switch (sortPriority)
		{
			case ExpansionMarketMenuSortPriority.NAME:
				return m_ItemSortName + "\t" + pricePadded;

			case ExpansionMarketMenuSortPriority.PRICE:
				return pricePadded + "\t" + m_ItemSortName;
		}

		return string.Empty;
	}

	void SetMenuIdx(int idx)
	{
		m_MenuIdx = idx;
	}

	int GetMenuIdx()
	{
		return m_MenuIdx;
	}

	void SetIncludeAttachments(bool state)
	{
		m_IncludeAttachments = state;
	}

	bool GetIncludeAttachments()
	{
		return m_IncludeAttachments;
	}

	void OnFastBuyButtonClick()
	{
		m_MarketMenu.SetItemInfo(this);
		m_MarketMenu.OnConfirmBuyButtonClick();
	}

	void OnFastSellButtonClick()
	{
		ExpansionMarketPlayerItem playerItem = m_MarketMenu.GetPlayerItem(GetMarketItem().ClassName);
		if (!playerItem || !playerItem.Item)
			return;

		m_MarketMenu.SetItemInfo(this);
		//! Show confirmation if attached/equipped
		if (playerItem.IsAttachedOrEquipped())
			m_MarketMenu.OnSellButtonClick();
		else
			m_MarketMenu.OnConfirmSellButtonClick();
	}

	bool CanSell()
	{
		return m_CanSell;
	}
	
	bool CanBuy()
	{
		return m_CanBuy;
	}
	
	bool HasRepSell()
	{
		return m_HasRepSell;
	}
	
	bool HasRepBuy()
	{
		return m_HasRepBuy;
	}
	
	int GetCurrentSelectedSkinIndex()
	{
		return m_CurrentSelectedSkinIndex;
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
	string CurrencyIcon;  //! DEPRECATED
	string CurrencyBuyIcon;
	string CurrencySellIcon;
	string OverlayText;
};