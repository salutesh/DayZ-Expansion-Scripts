/**
 * ExpansionMarketMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenu: ExpansionScriptViewMenu
{
	protected static float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MIN = 1.5;
	protected static float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MAX = 4;
	protected static float POSITION_PLAYER_PREVIEW_Y_OFFSET = -0.125;
	protected static float POSITION_PLAYER_PREVIEW_ANIMATION_DURATION = 0.5;

	protected ref ExpansionMarketMenuController m_MarketMenuController;
	protected ref ExpansionMarketModule m_MarketModule;
	protected ref ExpansionTraderObjectBase m_TraderObject;
	protected ref ExpansionMarketTrader m_TraderMarket;
	protected ref TStringArray m_CategoryNames;
	protected ref map<string, ref array<ref ExpansionMarketCategory>> m_MarketCategories;
	protected ref array<ExpansionMarketItem> m_TraderItems;
	protected ref ExpansionMarketItem m_SelectedMarketItem;
	protected ref ExpansionMarketMenuItem m_SelectedMarketItemElement;
	protected ref ExpansionMenuDialog_MarketSetQuantity m_QuantityDialog;
	protected ref ExpansionMarketMenuTooltip m_AttachmentsTooltip; 
	protected ref ExpansionMenuDialog_MarketConfirmPurchase m_PurchaseDialog;
	protected ref ExpansionMenuDialog_MarketConfirmSell m_SellDialog;
	protected ref ExpansionMarketMenuTooltip m_CurrenciesTooltip;
	protected ref ExpansionMarketMenuTooltip m_SellDenomsTooltip;
	protected ref ExpansionMarketMenuTooltip m_BuyDenomsTooltip;
	protected ref ExpansionMarketMenuTooltip m_QuatityTooltip;	
	
	protected int m_SelectedItemSlot;
	protected float m_LastPositionPlayerPreviewW;
	protected float m_LastPositionPlayerPreviewY;
	protected int m_PlayerPosCurrentAnimID;
	protected bool m_PlayerPosAnimRunning;
	protected int m_Quantity = 1;
	protected int m_TraderItemStock;
	protected int m_PlayerStock;
	protected int m_BuyPrice;
	protected ref ExpansionMarketSell m_MarketSell;
	protected int m_SellPrice;
	protected ExpansionMarketMenuState m_CurrentState = ExpansionMarketMenuState.LOADING;
	protected float m_RequestMarketTraderDataAccum;
	protected bool m_CategoriesCreated = false;
	protected bool m_ShowPrimeMag = false;
	protected bool m_ShowSecondMag = false;
	protected bool m_ShowPrimeAttach = false;
	protected bool m_ShowSecondAttach = false;
	protected bool m_ShowHandAttach = false;
	protected bool m_ShowHandMag = false;
	protected bool m_ShowPrimeBullets = false;
	protected bool m_ShowSecondBullets = false;
	protected bool m_ShowHandBullets = false;
	protected bool m_ShowSellable = false;
	protected bool m_ShowPurchasables = false;
	protected ref array<ref ExpansionMarketPlayerItem> m_PlayerItems;
	protected ref ExpansionMarketFilters m_MarketFilters;
	protected ref TStringArray m_FilterOptionStrings;
	protected bool m_FilterUpdateInProgress;
	protected vector m_CharacterOrientation;
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;
	protected bool m_FirstCall;
	protected bool m_Complete;
	protected ref map<string, string> m_TypeDisplayNames;
	PlayerBase m_PlayerPreview;
	protected bool m_IncludeAttachmentsData = true;
	int CategoriesExpanded;
	protected ref ExpansionMarketMenuColorHandler m_ColorHandler;
	Object m_CurrentPreviewObject;
	bool m_KeyInput = false;
	bool m_IsDropdownExpanded = false;
	bool m_IsSkinSelectorExpanded = false;
	protected ref ExpansionMarketMenuItemManager m_MarketMenuItemManager;
	
	protected EditBoxWidget market_filter_box;
	protected Widget menu_info_content;
	protected ButtonWidget market_item_buy;
	protected ButtonWidget market_item_sell;
	protected ButtonWidget market_filter_clear;
	protected ImageWidget market_filter_clear_icon;
	protected ItemPreviewWidget market_item_preview;
	protected PlayerPreviewWidget market_player_preview;
	protected ScrollWidget market_categories_scroller;
	protected Widget market_loading;
	protected ImageWidget market_icon;
	protected Widget menu_item_info_content;
	protected Widget dropdown_selector;
	protected ButtonWidget dropdown_selector_button;
	protected ImageWidget dropdown_expand_image;
	protected ImageWidget dropdown_collapse_image;
	protected ScrollWidget dropdown_container;
	protected GridSpacerWidget dropdown_content;
	protected RichTextWidget market_item_info_buy_price_monies;
	protected TextWidget market_item_sell_price_text;
	protected ImageWidget market_item_sell_price_icon;
	protected RichTextWidget market_item_info_sell_price_monies;
	protected TextWidget market_item_buy_price_text;
	protected ImageWidget market_item_buy_price_icon;
	protected Widget market_item_info_attachments;
	protected ButtonWidget market_item_info_attachments_button;
	protected ImageWidget market_item_info_attachments_icon;
	protected Widget market_item_info_attachments_panel;
	protected Widget market_item_info_attachments_setup_panel;
	protected CheckBoxWidget market_item_info_attachments_checkbox;
	protected ButtonWidget player_info_content;
	protected Widget market_item_buy_price_panel;
	protected Widget market_item_sell_price_panel;
	protected ButtonWidget market_footer_categories_toggle;
	protected TextWidget market_footer_categories_label;
	protected ButtonWidget market_item_info_attachments_setup_button;
	protected Widget market_item_description_panel;
	protected Widget market_item_description_spacer;
	protected Widget market_item_description_scroll_indicator_header;
	protected Widget market_item_description_scroll_indicator_footer;
	protected RichTextWidget market_item_description;
	protected ScrollWidget market_item_description_scroller;
	protected Widget skin_selector;
	protected ButtonWidget skin_selector_button;
	protected ImageWidget skin_selector_collapse_image;
	protected ImageWidget skin_selector_expand_image;
	protected ScrollWidget skin_selector_container;
	protected WrapSpacerWidget skin_selector_content;
	protected CheckBoxWidget market_item_purchasables_checkbox;
	protected CheckBoxWidget market_item_sellables_checkbox;
	protected ButtonWidget market_item_info_quantity_button;
	protected Widget market_menu_content;
	protected Widget market_menu_attachments_content;
	protected Widget market_item_info_stock;
	protected Widget market_item_info_stock_player;
	protected Widget dropdown_background;
	protected Widget market_item_info_item_manager_panel;
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu Constructor
	// ------------------------------------------------------------
	void ExpansionMarketMenu()
	{
		SetIsLoading(true);

		if (!m_MarketMenuController)
			m_MarketMenuController = ExpansionMarketMenuController.Cast(GetController());
		
		if (!m_MarketModule)
			m_MarketModule = ExpansionMarketModule.Cast(GetModuleManager().GetModule(ExpansionMarketModule));
		
		if (!m_FilterOptionStrings)
			m_FilterOptionStrings = new TStringArray;
				
		if (!m_MarketFilters)
			m_MarketFilters = new ExpansionMarketFilters();
				
		if (!m_TypeDisplayNames)
			m_TypeDisplayNames = new map<string, string>;
		
		m_CategoryNames = new TStringArray;
		m_MarketCategories = new map<string, ref array<ref ExpansionMarketCategory>>;
		m_TraderItems = new array<ExpansionMarketItem>;
		
		ExpansionMarketModule.SI_SetTraderInvoker.Insert(SetTraderObject);
		ExpansionMarketModule.SI_SelectedItemUpdatedInvoker.Insert(OnNetworkItemUpdate);
		ExpansionMarketModule.SI_Callback.Insert(MenuCallback);
		
		if (!m_ColorHandler)
			m_ColorHandler = new ExpansionMarketMenuColorHandler(GetLayoutRoot());
		
		if (!m_QuatityTooltip)
		{
			m_QuatityTooltip = new ExpansionMarketMenuTooltip();
			m_QuatityTooltip.Hide();
			m_QuatityTooltip.SetTitle("#STR_EXPANSION_MARKET_TOOLTIP_QUANTITY_TITLE");
			m_QuatityTooltip.SetText("#STR_EXPANSION_MARKET_TOOLTIP_QUANTITY_DESC");
			m_QuatityTooltip.SetContentOffset(-0.273438, -0.104167);
		}
		
		m_MarketMenuController.ShowSellables = GetExpansionClientSettings().MarketMenuFilterSellableState;
		m_MarketMenuController.ShowPurchasables = GetExpansionClientSettings().MarketMenuFilterPurchasableState;

		market_item_sellables_checkbox.SetChecked(m_MarketMenuController.ShowSellables);
		market_item_purchasables_checkbox.SetChecked(m_MarketMenuController.ShowPurchasables);
		
		SetFilterSellables(GetExpansionClientSettings().MarketMenuFilterSellableState);
		SetFilterPurchasables(GetExpansionClientSettings().MarketMenuFilterPurchasableState);
		
		if (!m_MarketMenuItemManager)
		{
			m_MarketMenuItemManager = new ExpansionMarketMenuItemManager(this);
			m_MarketMenuItemManager.Hide();
		}
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdatePlayerItems
	// ------------------------------------------------------------
	void UpdatePlayerItems()
	{
		MarketPrint("UpdatePlayerItems - Start");
		
		if (!m_PlayerItems)
			m_PlayerItems = new array <ref ExpansionMarketPlayerItem>;
		else
			m_PlayerItems.Clear();
		
		array< EntityAI > items = m_MarketModule.LocalGetEntityInventory();

		foreach (EntityAI item: items)
		{
			string name = item.ClassName();
			bool added = false;
			
			MarketPrint("UpdatePlayerItems - Add player item: " + name + " to player items array");
			
			foreach (ExpansionMarketPlayerItem currentItem : m_PlayerItems)
			{
				if (currentItem.ClassName == name)
				{
					currentItem.Count++;
					currentItem.UpdateContainerItems();
					added = true;
					break;
				}
			}
			
			if (!added)
			{
				m_PlayerItems.Insert(new ExpansionMarketPlayerItem(item));
			}
		}

		MarketPrint("UpdatePlayerItems - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu HasPlayerItem
	// ------------------------------------------------------------
	bool HasPlayerItem(string name)
	{
		MarketPrint("HasPlayerItem - Start");
		
		for (int i = 0; i < m_PlayerItems.Count(); i++)
		{
			string itemName = m_PlayerItems[i].ClassName;
			itemName.ToLower();
			
			if (itemName == name)
			{
				MarketPrint("ExpansionMarketMenu::HasPlayerItem - End and return true" + name);
				return true;
			}
		}
		
		MarketPrint("HasPlayerItem - End and return false: " + name);
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu HasPlayerMarketItem
	// ------------------------------------------------------------	
	bool HasPlayerMarketItem(ExpansionMarketItem item)
	{
		return (m_MarketModule.GetAmountInInventory(item, m_MarketModule.LocalGetEntityInventory()) > 0);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetPlayerItem
	// ------------------------------------------------------------
	ExpansionMarketPlayerItem GetPlayerItem(string name)
	{		
		ExpansionMarketPlayerItem item;
		for (int i = 0; i < m_PlayerItems.Count(); i++)
		{
			string itemName = m_PlayerItems[i].ClassName;
			//itemName.ToLower();
			
			if (itemName == name)
			{
				item = m_PlayerItems[i];
			}
		}
		
		return item;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetPlayerItems
	// ------------------------------------------------------------
	array<ref ExpansionMarketPlayerItem> GetPlayerItems()
	{
		return m_PlayerItems;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetLayoutFile
	// ------------------------------------------------------------
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu.layout";
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetControllerType
	// ------------------------------------------------------------
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuController;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu CanShow
	// ------------------------------------------------------------
	override bool CanShow()
	{
		return true;
	}
		
	void InitMarketCategories()
	{
		map<int, ref ExpansionMarketCategory> categories = GetExpansionSettings().GetMarket().GetCategories();
		if (!categories)
		{
			Error("ExpansionMarketMenu::InitMarketCategories - Could not get market categories from expansion settings");
			SetIsLoading(false);
			return;
		}

		array<ref ExpansionMarketCategory> categoriesArray;

		foreach (ExpansionMarketCategory category : categories)
		{
			string displayNameTranslated = Widget.TranslateString(category.DisplayName);

			//! Deal with the case where different categories may have the same display name
			if (!m_MarketCategories.Find(displayNameTranslated, categoriesArray))
			{
				categoriesArray = new array<ref ExpansionMarketCategory>;
				m_MarketCategories.Insert(displayNameTranslated, categoriesArray);
			}
			categoriesArray.Insert(category);
		}

		m_CategoryNames = m_MarketCategories.GetKeyArray();
		m_CategoryNames.Sort();

		int idx;
		foreach (string categoryName : m_CategoryNames)
		{
			categoriesArray = m_MarketCategories[categoryName];
			foreach (ExpansionMarketCategory currentCategory: categoriesArray)
			{
				currentCategory.m_Idx = idx++;
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu CreateMenuCategories
	// ------------------------------------------------------------
	void CreateMenuCategories()
	{
		MarketPrint("CreateMenuCategories - Start");
		
		array<ref ExpansionMarketCategory> categoriesArray;

		string search = GetSearchText();
		search.ToLower();
				
		foreach (string categoryName : m_CategoryNames)
		{
			categoriesArray = m_MarketCategories[categoryName];
			CreateMenuCategories(categoriesArray, search);
		}

		int count = m_MarketMenuController.MarketCategories.Count();
		for (int i = 0; i < count; i++)
		{
			ExpansionMarketMenuCategory menuCategory = m_MarketMenuController.MarketCategories[i];
			if (GetExpansionClientSettings().MarketMenuCategoriesState || count == 1)
				menuCategory.ToggleCategory(true);
		}

		SetIsLoading(false);

		MarketPrint("CreateMenuCategories - End");
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu CreateMenuCategories
	// ------------------------------------------------------------
	protected void CreateMenuCategories( array<ref ExpansionMarketCategory> categories, string search = "")
	{
		foreach (ExpansionMarketCategory currentCategory : categories)
		{
			ExpansionMarketMenuCategory marketCategoryElement = NULL;
			map<string, ref array<ExpansionMarketItem>> tempItems = NULL;

			for (int i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
			{
				if (m_MarketMenuController.MarketCategories[i].m_Category.CategoryID == currentCategory.CategoryID)
				{
					marketCategoryElement = m_MarketMenuController.MarketCategories[i];
					tempItems = marketCategoryElement.m_TempItems;
				}
			}

			if (!tempItems)
				tempItems = new map<string, ref array<ExpansionMarketItem>>;

			MarketPrint("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------");
			MarketPrint("ExpansionMarketMenu::CreateMenuCategories - Add category: " + currentCategory.m_FileName + " | " + currentCategory.Items.Count());
			if (currentCategory.Items.Count() == 0)
				continue;
			
			int itemCount;
			int hiddenItemCount;
			array<ref ExpansionMarketItem> items = currentCategory.GetItems();
			foreach (ExpansionMarketItem currentItem : items)
			{
				MarketPrint("ExpansionMarketMenu::CreateMarketCategorys - Check item: " + currentItem.ClassName);
				if (!m_TraderMarket.ItemExists(currentItem.ClassName) || m_TraderMarket.IsAttachmentBuySell(currentItem.ClassName))
					continue;
				
				string displayName = GetDisplayName(GetPreviewClassName(currentItem.ClassName, true));
				
				currentItem.m_ShowInMenu = (!currentItem.m_IsVariant || ShowSellables() || search) && ShouldShowItem(currentItem, displayName, search);
				
				TempInsertItem(displayName, currentItem, tempItems);
				
				itemCount++;
				
				if (!currentItem.m_ShowInMenu)
					hiddenItemCount++;
			}
			
			if ( tempItems.Count() == 0 )
				continue;
			
			if (!marketCategoryElement)
			{
				//! Create the category with its items here
				marketCategoryElement = new ExpansionMarketMenuCategory(this, currentCategory, tempItems);
				AddMenuCategory(marketCategoryElement);
			}
			else
			{
				marketCategoryElement.UpdateMarketItems();
			}

			if (hiddenItemCount == itemCount)
				marketCategoryElement.Hide();
		}
	}
	
	bool ShouldShowItem(ExpansionMarketItem currentItem, string displayName, string search = "")
	{
		if (search != "")
		{
			string displayNameLower = displayName;
			displayNameLower.ToLower();

			if (!displayNameLower.Contains(search))
			{
				MarketPrint("ExpansionMarketMenu::ShouldShowItem - Search filter is active. Skip item: " + currentItem.ClassName);
				return false;
			}
		}
		
		if (ShowPrimeMag() || ShowSecondMag() || ShowPrimeAttach() || ShowSecondAttach() || ShowHandMag() || ShowHandAttach() || ShowPrimeBullet() || ShowSecondBullet() || ShowHandBullet() || ShowSellables() || ShowPurchasables())
		{
			foreach (string option_filter : m_FilterOptionStrings)
			{
				option_filter.ToLower();

				if (currentItem.ClassName == option_filter)
				{
					if (ShowSellables() && !HasPlayerItem(currentItem.ClassName))
						return false;

					MarketPrint("ExpansionMarketMenu::ShouldShowItem - A option filter is active. Add item: " + currentItem.ClassName);
					return true;
				}
			}

			//! Skip item if no filter matches
			return false;
		}

		MarketPrint("ExpansionMarketMenu::ShouldShowItem - No option filter is active. Add item: " + currentItem.ClassName);
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu TempInsertItem
	// ------------------------------------------------------------
	void TempInsertItem(string displayName, ExpansionMarketItem currentItem, map<string, ref array<ExpansionMarketItem>> tempItems)
	{
		//! This deals with the case where different class names have the same display name
		array<ExpansionMarketItem> itemsArray;
		if (!tempItems.Find(displayName, itemsArray)) 
		{
			itemsArray = new array<ExpansionMarketItem>;
			tempItems.Insert(displayName, itemsArray);
		}
		else
		{
			foreach (ExpansionMarketItem item: itemsArray)
			{
				if (item.ItemID == currentItem.ItemID)
					return;
			}
		}
		itemsArray.Insert(currentItem);
		m_TraderItems.Insert(currentItem);
	}
	
	void AddMenuCategory(ExpansionMarketMenuCategory marketCategoryElement)
	{
		int count = m_MarketMenuController.MarketCategories.Count();

		if (!count || marketCategoryElement.m_Category.m_Idx > m_MarketMenuController.MarketCategories[count - 1].m_Category.m_Idx)
		{
			m_MarketMenuController.MarketCategories.Insert(marketCategoryElement);
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				if (marketCategoryElement.m_Category.m_Idx < m_MarketMenuController.MarketCategories[i].m_Category.m_Idx)
					break;
			}

			m_MarketMenuController.MarketCategories.InsertAt(marketCategoryElement, i);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateMarketCategories
	// ------------------------------------------------------------
	void UpdateMarketCategories(bool updateItemViews = false)
	{
		MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Start");

		string search = GetSearchText();
		search.ToLower();
		
		bool isFiltered = IsFiltered();

		//! This deals with the case where filters are toggled or search text is entered while the initial drawing of menu items hasn't yet completed,
		//! and sets whether items should be shown or not
		foreach (ExpansionMarketItem item : m_TraderItems)
		{
			item.m_ShowInMenu = !isFiltered && !item.m_IsVariant;
			if (!item.m_ShowInMenu && (!item.m_IsVariant || ShowSellables() || search))
			{
				string displayName = GetDisplayName(GetPreviewClassName(item.ClassName, true));
				item.m_ShowInMenu = ShouldShowItem(item, displayName, search);
			}
		}
		
		bool includeAttachments = m_SelectedMarketItemElement && m_SelectedMarketItemElement.GetIncludeAttachments() && m_SelectedMarketItem.SpawnAttachments.Count() > 0;

		for (int i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
		{
			int filteredItemCount = 0;
			ExpansionMarketMenuCategory menuCategory = m_MarketMenuController.MarketCategories[i];
			
			MarketPrint("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------");
			MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Update category: " + menuCategory.m_Category.m_FileName + " | " +menuCategory.GetItems().Count());
			
			if (isFiltered)
			{
				foreach (ExpansionMarketItem tempItem: menuCategory.m_MarketItems)
				{
					if (!tempItem.m_ShowInMenu)
						filteredItemCount++;
				}
			}

			int itemCount = menuCategory.GetItems().Count();
			for (int j = 0; j < itemCount; j++)
			{
				ExpansionMarketMenuItem menuItem =  menuCategory.GetItems()[j];
				ExpansionMarketItem currentBaseItem = menuItem.GetBaseItem();
				ExpansionMarketItem currentItem = menuItem.GetMarketItem();

				if (updateItemViews)
				{
					if (!currentItem.m_UpdateView && m_SelectedMarketItem)
					{
						if (includeAttachments)
						{
							//! The selected item was bought/sold with attachments,
							//! update all attachments, all items that have one of the same attachments,
							//! and items that have the selected item as attachment
							foreach (string selectedAttachment: m_SelectedMarketItem.SpawnAttachments)
							{
								if (currentItem.ClassName == selectedAttachment)
								{
									currentItem.m_UpdateView = true;
									break;
								}

								foreach (string attachment: currentItem.SpawnAttachments)
								{
									if (attachment == selectedAttachment || attachment == m_SelectedMarketItem.ClassName)
									{
										currentItem.m_UpdateView = true;
										break;
									}
								}

								if (currentItem.m_UpdateView)
									break;
							}
						}
						else
						{
							//! Update items that have the selected item as attachment
							foreach (string currentAttachment: currentItem.SpawnAttachments)
							{
								if (currentAttachment == m_SelectedMarketItem.ClassName)
								{
									currentItem.m_UpdateView = true;
									break;
								}
							}
						}
					}

					if (currentItem.m_UpdateView)
						menuItem.UpdateView();
				}
		
				if (!currentBaseItem.m_ShowInMenu)
				{
					menuItem.Hide();
					MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Hide item: " + currentBaseItem.ClassName);
				}
				else
				{
					menuItem.Show();
					MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Show item: " + currentBaseItem.ClassName);
				}
			}
			
			if (isFiltered && filteredItemCount == menuCategory.m_MarketItems.Count())
			{
				menuCategory.Hide();
				MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Hide category: " + menuCategory.m_CategoryController.CategoryName);
			}
			else
			{
				menuCategory.Show();
				MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - Show category: " + menuCategory.m_CategoryController.CategoryName);
				if (!menuCategory.IsUpdateTimerRunning() || !menuCategory.m_UpdateItemCount)
				{
					int show;
					if (isFiltered)
						show = menuCategory.m_MarketItems.Count() - filteredItemCount;
					else
						show = menuCategory.GetShowItemsCount();
					menuCategory.UpdateItemCount(show);
				}
			}
		}
		
		MarketPrint("ExpansionMarketMenu::UpdateMarketCategories - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateOptionFilterStrings
	// ------------------------------------------------------------
	void UpdateOptionFilterStrings()
	{
		MarketPrint("ExpansionMarketMenu::UpdateOptionFilterStrings - Start");

		if (m_FilterOptionStrings.Count() > 0)
			m_FilterOptionStrings.Clear();
		
		if (ShowPrimeMag())
		{
			TStringArray primaryMags = m_MarketFilters.GetPrimaryWeaponMagazines();
			foreach (string primaryMag : primaryMags)
			{
				if (!IsInFilter(primaryMag))
					m_FilterOptionStrings.Insert(primaryMag);
			}
		}
		
		if (ShowSecondMag())
		{
			TStringArray secondaryMags = m_MarketFilters.GetSecondaryWeaponMagazines();
			foreach (string secondaryMag : secondaryMags)
			{
				if (!IsInFilter(secondaryMag))
					m_FilterOptionStrings.Insert(secondaryMag);
			}
		}
		
		if (ShowHandMag())
		{
			TStringArray handMags = m_MarketFilters.GetHandWeaponMagazines();
			foreach (string handMag : handMags)
			{
				if (!IsInFilter(handMag))
					m_FilterOptionStrings.Insert(handMag);
			}
		}
		
		if (ShowPrimeAttach())
		{
			TStringArray primaryAttachments = m_MarketFilters.GetPrimaryWeaponAttachments();
			foreach (string primaryAttachment : primaryAttachments)
			{
				if (!IsInFilter(primaryAttachment))
					m_FilterOptionStrings.Insert(primaryAttachment);
			}
		}
		
		if (ShowSecondAttach())
		{
			TStringArray secondaryAttachments = m_MarketFilters.GetSecondaryWeaponAttachments();
			foreach (string secondaryAttachment : secondaryAttachments)
			{
				if (!IsInFilter(secondaryAttachment))
					m_FilterOptionStrings.Insert(secondaryAttachment);
			}
		}
		
		if (ShowHandAttach())
		{
			TStringArray handAttachments = m_MarketFilters.GetHandWeaponAttachments();
			foreach (string handAttachment : handAttachments)
			{
				if (!IsInFilter(handAttachment))
					m_FilterOptionStrings.Insert(handAttachment);
			}
		}
		
		if (ShowPrimeBullet())
		{
			TStringArray primaryBullets = m_MarketFilters.GetPrimaryWeaponBullets();
			foreach (string primaryBullet : primaryBullets)
			{
				if (!IsInFilter(primaryBullet))
					m_FilterOptionStrings.Insert(primaryBullet);
			}
		}
		
		if (ShowSecondBullet())
		{
			TStringArray secondaryBullets = m_MarketFilters.GetSecondaryWeaponBullets();
			foreach (string secondaryBullet : secondaryBullets)
			{
				if (!IsInFilter(secondaryBullet))
					m_FilterOptionStrings.Insert(secondaryBullet);
			}
		}
		
		if (ShowHandBullet())
		{
			TStringArray handBullets = m_MarketFilters.GetHandWeaponBullets();
			foreach (string handBullet : handBullets)
			{
				if (!IsInFilter(handBullet))
					m_FilterOptionStrings.Insert(handBullet);
			}
		}
		
		int i;

		if (m_FilterOptionStrings.Count() > 0 && (ShowSellables() || ShowPurchasables()))
		{
			//! Apply sellables/purchasables filter to selected filter strings
			for (i = m_FilterOptionStrings.Count() - 1; i >= 0; i--)
			{
				string item = m_FilterOptionStrings[i];
				if ((ShowSellables() && !GetMarketTrader().CanSellItem(item)) || (ShowPurchasables() && !GetMarketTrader().CanBuyItem(item)))
					m_FilterOptionStrings.Remove(i);
			}
		}
		else
		{
			if (ShowSellables())
			{
				TStringArray sellables = m_MarketFilters.GetSellables();
				foreach (string sellable : sellables)
				{
					//! If purchasables filter is also active, show only items that can be sold AND bought, not either-or
					if ((!ShowPurchasables() || GetMarketTrader().CanBuyItem(sellable)) && !IsInFilter(sellable))
						m_FilterOptionStrings.Insert(sellable);
				}
			}
			else if (ShowPurchasables())
			{
				TStringArray purchasables = m_MarketFilters.GetPurchasables();
				foreach (string purchasable : purchasables)
				{
					if (!IsInFilter(purchasable))
						m_FilterOptionStrings.Insert(purchasable);
				}
			}
		}
		
		MarketPrint("ExpansionMarketMenu::UpdateOptionFilterStrings - End");
	}
	
	bool IsFiltered()
	{
		//! NOTE: ShowSellables/ShowPurchasables needs to be checked separately because it can result in zero option filter strings
		return m_FilterOptionStrings.Count() > 0 || GetSearchText() != "" || ShowSellables() || ShowPurchasables();
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu IsInFilter
	// ------------------------------------------------------------
	bool IsInFilter(string className)
	{
		int index = m_FilterOptionStrings.Find(className);
		if (index > -1)
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu IsInFilter
	// ------------------------------------------------------------
	void RemoveFromFilter(string className)
	{
		int index = m_FilterOptionStrings.Find(className);
		if (index > -1)
			m_FilterOptionStrings.Remove(index);
	}
	
	string GetSearchText()
	{
		return market_filter_box.GetText();
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu ClearCategories
	// ------------------------------------------------------------
	void ClearCategories()
	{
		for (int i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
		{
			ExpansionMarketMenuCategory categoryElement = m_MarketMenuController.MarketCategories[i];
			categoryElement.Hide();
			delete categoryElement;
		}
		
		m_MarketMenuController.MarketCategories.Clear();
		CategoriesExpanded = 0;
		ExpansionMarketMenuCategory.UpdateCategoryID = -1;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu ClearCategoryItems
	// ------------------------------------------------------------
	void ClearCategoryItems()
	{
		for (int i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
		{
			ExpansionMarketMenuCategory categoryElement = m_MarketMenuController.MarketCategories[i];
			categoryElement.ClearCategoryItems();
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetTraderObject
	// ------------------------------------------------------------
	//! Gets called from market module SI_SetTraderInvoker invoker and sets the current intracted market trader and its market class
	void SetTraderObject( ExpansionTraderObjectBase trader, bool complete)
	{
		MarketPrint("SetTraderObject - Start");
		
		m_TraderObject = trader;
		if (!m_TraderObject)
		{
			Error("ExpansionMarketMenu::SetTraderObject - Could not get trader!");
			Hide();
			return;
		}
		
		m_TraderMarket = trader.GetTraderMarket();
		if (!m_TraderMarket)
		{
			Error("ExpansionMarketMenu::SetTraderObject - Could not get trader market!");
			Hide();
			return;
		}
		
		m_KeyInput = false;

		if (m_CurrentState != ExpansionMarketMenuState.REQUESTING_SELECTED_ITEM)
		{
			if (!m_FirstCall || (m_Complete && complete))
			{
				m_MarketModule.EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));

				UpdatePlayerCurrency();
				UpdatePlayerItems();
			}

			UpdateFilterView();
		}

		if (!m_FirstCall)
		{
			EXPrint("SetTraderObject() - !m_FirstCall");
			InitMarketCategories();
			CreateMenuCategories();
	
			m_MarketMenuController.MarketName = m_TraderMarket.DisplayName;
			m_MarketMenuController.NotifyPropertyChanged("MarketName");
			
			if (m_TraderMarket.TraderIcon != "")
			{
				m_MarketMenuController.MarketIcon = ExpansionIcons.GetPath(m_TraderMarket.TraderIcon);
				if (m_MarketMenuController.MarketIcon == "")
					m_MarketMenuController.MarketIcon = m_TraderMarket.TraderIcon;
				
				m_MarketMenuController.NotifyPropertyChanged("MarketIcon");
			}
			
			if (GetExpansionSettings().GetMarket().CurrencyIcon != "")
			{
				m_MarketMenuController.CurrencySellIcon = GetExpansionSettings().GetMarket().CurrencyIcon;
				m_MarketMenuController.NotifyPropertyChanged("CurrencySellIcon");
				m_MarketMenuController.CurrencyBuyIcon = GetExpansionSettings().GetMarket().CurrencyIcon;
				m_MarketMenuController.NotifyPropertyChanged("CurrencyBuyIcon");
			}
			
			m_FirstCall = true;
		}
		else if (!m_Complete)
		{
			EXPrint("SetTraderObject() - !m_Complete");
			//! Create remaining categories as items arrive
			CreateMenuCategories();
		}
		else
		{
			if (m_CurrentState != ExpansionMarketMenuState.REQUESTING_SELECTED_ITEM)
			{
				EXPrint("SetTraderObject() - m_Complete -> UpdateMarketCategories(true)");
				UpdateMarketCategories(true);
			}
			else
			{
				EXPrint("SetTraderObject() - m_Complete -> GetSelectedMarketItemElement().UpdateView()");
				GetSelectedMarketItemElement().UpdateView();
			}
			
			UpdateItemFieldFromBasicNetSync();
		}

		if (!m_Complete && complete)
			m_Complete = complete;

		SetIsLoading(false);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateFilterView
	// ------------------------------------------------------------
	void UpdateFilterView()
	{
		if (m_FilterOptionStrings.Count() > 0)
		{
			//! Filter(s) active, need to update complete view
			menu_info_content.Show(false);

			//! Store state of selected filters
			int i;
			ExpansionMarketMenuDropdownElement dropdownElement;
			map<string, bool> dropdownCheckedStates = new map<string, bool>;
			for (i = 0; i < m_MarketMenuController.DropdownElements.Count(); i++)
			{
				dropdownElement = m_MarketMenuController.DropdownElements[i];
				dropdownCheckedStates.Insert(dropdownElement.ClassName(), dropdownElement.IsChecked());
			}
			
			ResetDropdownFilterStates();

			UpdateDropdownFilterElements();

			//! Restore state of selected filters
			for (i = 0; i < m_MarketMenuController.DropdownElements.Count(); i++)
			{
				dropdownElement = m_MarketMenuController.DropdownElements[i];
				bool storedState;
				if (dropdownCheckedStates.Find(dropdownElement.ClassName(), storedState))
				{
					dropdownElement.SetChecked(storedState, false);
				}
			}
		}
		else
		{
			UpdateDropdownFilterElements();
		}

		UpdateOptionFilterStrings();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ClearDropdownElements
	// ------------------------------------------------------------
	void ClearDropdownElements()
	{
		for (int i = 0; i < m_MarketMenuController.DropdownElements.Count(); i++)
		{
			ExpansionMarketMenuDropdownElement element = m_MarketMenuController.DropdownElements[i];
			element.Hide();
			delete element;
		}
		
		m_MarketMenuController.DropdownElements.Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateDropdownFilterElements
	// ------------------------------------------------------------
	void UpdateDropdownFilterElements()
	{
		EXPrint("UpdateDropdownFilterElements - START");

		EntityAI shoulder_left = GetGame().GetPlayer().FindAttachmentBySlotName("Shoulder");
		EntityAI shoulder_right = GetGame().GetPlayer().FindAttachmentBySlotName("Melee");
		EntityAI hands = GetGame().GetPlayer().GetHumanInventory().GetEntityInHands();
		
		ClearDropdownElements();
		
		if (shoulder_left && shoulder_left.IsWeapon())
		{
			m_MarketFilters.GenerateWeaponAttachmentDenom(shoulder_left.GetType());

			if (m_MarketFilters.GetWeaponMagazines(shoulder_left).Count() > 0)
			{
				StringLocaliser primary_mag = new StringLocaliser(GetDisplayName(shoulder_left.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_MAGAZINES");
				ExpansionMarketMenuDropdownElement_FilterPrimeMag primemag_filter = new ExpansionMarketMenuDropdownElement_FilterPrimeMag(this, primary_mag);
				m_MarketMenuController.DropdownElements.Insert(primemag_filter);
			}

			if (m_MarketFilters.GetWeaponAttachments(shoulder_left).Count() > 0)
			{
				StringLocaliser primary_attach = new StringLocaliser(GetDisplayName(shoulder_left.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_ATTACHMENTS");
				ExpansionMarketMenuDropdownElement_FilterPrimeAttach primeattach_filter = new ExpansionMarketMenuDropdownElement_FilterPrimeAttach(this, primary_attach);
				m_MarketMenuController.DropdownElements.Insert(primeattach_filter);
			}

			if (m_MarketFilters.GetWeaponBullets(shoulder_left).Count() > 0)
			{
				StringLocaliser primary_bullets = new StringLocaliser(GetDisplayName(shoulder_left.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_BULLETS");
				ExpansionMarketMenuDropdownElement_FilterPrimeBullet primebullet_filter = new ExpansionMarketMenuDropdownElement_FilterPrimeBullet(this, primary_bullets);
				m_MarketMenuController.DropdownElements.Insert(primebullet_filter);
			}
		}

		if (shoulder_right && shoulder_right.IsWeapon())
		{
			m_MarketFilters.GenerateWeaponAttachmentDenom(shoulder_right.GetType());

			if (m_MarketFilters.GetWeaponMagazines(shoulder_right).Count() > 0)
			{
				StringLocaliser second_mag = new StringLocaliser(GetDisplayName(shoulder_right.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_MAGAZINES");
				ExpansionMarketMenuDropdownElement_FilterSecondMag secondmag_filter = new ExpansionMarketMenuDropdownElement_FilterSecondMag(this, second_mag);
				m_MarketMenuController.DropdownElements.Insert(secondmag_filter);
			}

			if (m_MarketFilters.GetWeaponAttachments(shoulder_right).Count() > 0)
			{
				StringLocaliser second_attach = new StringLocaliser(GetDisplayName(shoulder_right.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_ATTACHMENTS");
				ExpansionMarketMenuDropdownElement_FilterSecondAttach secondattach_filter = new ExpansionMarketMenuDropdownElement_FilterSecondAttach(this, second_attach);
				m_MarketMenuController.DropdownElements.Insert(secondattach_filter);
			}

			if (m_MarketFilters.GetWeaponBullets(shoulder_right).Count() > 0)
			{
				StringLocaliser second_bullets = new StringLocaliser(GetDisplayName(shoulder_right.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_BULLETS");
				ExpansionMarketMenuDropdownElement_FilterSecondBullet secondbullet_filter = new ExpansionMarketMenuDropdownElement_FilterSecondBullet(this, second_bullets);
				m_MarketMenuController.DropdownElements.Insert(secondbullet_filter);
			}
		}

		if (hands && hands.IsWeapon())
		{
			m_MarketFilters.GenerateWeaponAttachmentDenom(hands.GetType());

			if (m_MarketFilters.GetWeaponMagazines(hands).Count() > 0)
			{
				StringLocaliser hand_mag = new StringLocaliser(GetDisplayName(hands.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_MAGAZINES");		
				ExpansionMarketMenuDropdownElement_FilterHandMag handmag_filter = new ExpansionMarketMenuDropdownElement_FilterHandMag(this, hand_mag);
				m_MarketMenuController.DropdownElements.Insert(handmag_filter);
			}

			if (m_MarketFilters.GetWeaponAttachments(hands).Count() > 0)
			{
				StringLocaliser hand_attach = new StringLocaliser(GetDisplayName(hands.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_ATTACHMENTS");
				ExpansionMarketMenuDropdownElement_FilterHandAttach handattach_filter = new ExpansionMarketMenuDropdownElement_FilterHandAttach(this, hand_attach);
				m_MarketMenuController.DropdownElements.Insert(handattach_filter);
			}

			if (m_MarketFilters.GetWeaponBullets(hands).Count() > 0)
			{
				StringLocaliser hand_bullets = new StringLocaliser(GetDisplayName(hands.GetType()) + " #STR_EXPANSION_MARKET_OPTION_FILTER_BULLETS");
				ExpansionMarketMenuDropdownElement_FilterHandBullet handbullet_filter = new ExpansionMarketMenuDropdownElement_FilterHandBullet(this, hand_bullets);
				m_MarketMenuController.DropdownElements.Insert(handbullet_filter);
			}
		}
		
		if (m_MarketMenuController.DropdownElements.Count() == 0)
		{
			dropdown_selector.Show(false);
		}
		else if (m_MarketMenuController.DropdownElements.Count() > 0)
		{
			dropdown_selector.Show(true);
		}

		EXPrint("UpdateDropdownFilterElements - END");
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdatePlayerCurrency
	// ------------------------------------------------------------
	void UpdatePlayerCurrency()
	{
		array<int> monies = new array<int>;
		
		ExpansionMarketCurrency worth = m_MarketModule.GetPlayerWorth(PlayerBase.Cast(GetGame().GetPlayer()), monies, m_TraderMarket);
		
		TStringArray descriptions = new TStringArray;
		TStringArray excludedCurrencys = new TStringArray;
		for (int i = 0; i < monies.Count(); i++)
		{
			if (!monies[i])
				continue;
			string type = m_MarketModule.GetMoneyDenomination(i);
			int total_monies = monies[i] * m_MarketModule.GetMoneyPrice(type);
			if (m_TraderMarket.Currencies.Find(type) == -1)
			{
				excludedCurrencys.Insert(GetDisplayName(type));
			}
			else
			{
				string currencyString = ExpansionStatic.IntToCurrencyString(total_monies, ",");
				descriptions.Insert(ExpansionStatic.IntToCurrencyString(monies[i], ",") + " × " + GetDisplayName(type) + " (" + currencyString + ")");
			}
		}
		
		monies.Clear();
		
		string currencyPlayerTotalMoneyString = ExpansionStatic.IntToCurrencyString(worth, ",");
		m_MarketMenuController.PlayerTotalMoney = currencyPlayerTotalMoneyString + " #STR_EXPANSION_MARKET_CURRENCY_TOTAL";
		m_MarketMenuController.NotifyPropertyChanged("PlayerTotalMoney");
		
		if (!m_CurrenciesTooltip)
		{
			m_CurrenciesTooltip = new ExpansionMarketMenuTooltip();
			m_CurrenciesTooltip.SetTitle("#STR_EXPANSION_MARKET_CURRENCY_DENOMS");
			m_CurrenciesTooltip.SetContentOffset(-0.234375, 0.006944);
		}
		else
		{
			m_CurrenciesTooltip.ClearEntries();
		}

		foreach (string desc: descriptions)
		{
			m_CurrenciesTooltip.AddEntry(desc);
		}

		if (excludedCurrencys.Count())
		{
			//! Show currencies this trader does not accept
			m_CurrenciesTooltip.AddEntry("");
			m_CurrenciesTooltip.AddEntry("This trader does not accept:");  //! TODO: localize this
			foreach (string exclDesc: excludedCurrencys)
			{
				m_CurrenciesTooltip.AddEntry(exclDesc);
			}
		}
		
		m_CurrenciesTooltip.Hide();
		
		descriptions.Clear();
		excludedCurrencys.Clear();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetItemInfo
	// ------------------------------------------------------------
	void SetItemInfo( ExpansionMarketMenuItem itemElement, bool resetQuantity = true)
	{
		m_CurrentPreviewObject = NULL;
		
		m_SelectedMarketItemElement = itemElement;
		m_SelectedMarketItem = itemElement.GetMarketItem();
		
		menu_info_content.Show(true);

		string itemClassName = GetPreviewClassName(GetSelectedMarketItem().ClassName, true);
		
		m_MarketMenuController.MarketItemName = GetDisplayName(itemClassName);
		m_MarketMenuController.NotifyPropertyChanged("MarketItemName");
		
		m_MarketMenuController.MarketItemDesc = ExpansionStatic.GetItemDescriptionWithType(itemClassName);
		m_MarketMenuController.NotifyPropertyChanged("MarketItemDesc");
		
		UpdatePreview();
		
		if (resetQuantity)
			m_Quantity = 1;
		
		UpdateItemFieldFromBasicNetSync();
		SetItemAttachmentsInfo(itemElement);
		PlayMarketSound();
		UpdateItemDescriptionPanel();
		UpdateItemSkins();
		
		bool hasAttachments = false;
		if (itemElement.GetPreviewObject().IsWeapon())
		{
			m_MarketFilters.GenerateWeaponAttachmentDenom(itemElement.GetPreviewObject().GetType());
			hasAttachments = m_MarketFilters.HasWeaponAttachments(itemElement.GetPreviewObject().GetType());
		}
		else if (itemElement.GetPreviewObject().IsClothing())
		{
			if (ExpansionMarketFilters.IsCustomizableClothing(itemElement.GetPreviewObject().GetType()))
			{
				m_MarketFilters.GenerateClothingAttachmentDenom(itemElement.GetPreviewObject().GetType());
				hasAttachments = m_MarketFilters.HasClothingAttachments(itemElement.GetPreviewObject().GetType());
			}
		}
		
		bool hasSpawnAttachments = itemElement.GetMarketItem().SpawnAttachments.Count() > 0;
		
		market_item_info_item_manager_panel.Show(hasAttachments);
		market_item_info_attachments_panel.Show(hasSpawnAttachments && !hasAttachments);
		market_item_info_attachments_checkbox.SetChecked(GetSelectedMarketItemElement().GetIncludeAttachments());
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetItemInfo
	// ------------------------------------------------------------
	void UpdatePreview()
	{
		string previewClassName = GetPreviewClassName(GetSelectedMarketItem().ClassName);
		
		if (!UsePlayerPreview())
		{
			market_item_preview.Show(true);
			market_player_preview.Show(false);
			m_MarketMenuController.MarketItemPreview = GetSelectedMarketItemElement().GetPreviewObject();
			m_MarketMenuController.NotifyPropertyChanged("MarketItemPreview");
			
			m_CurrentPreviewObject = GetSelectedMarketItemElement().GetPreviewObject();
		}
		else
		{
			market_item_preview.Show(false);
			market_player_preview.Show(true);

			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			int i;
			
			if (!m_PlayerPreview)
			{
				//! Oh fml... of all the things I tried to get the player preview face texture to render,
				//! it turns out the preview entity needs to be close to the player's viewport. Go figure.
				//! Just spawn it behind the player camera so it won't be seen under normal circumstances.
				vector dir = GetGame().GetCurrentCameraDirection();
				dir.Normalize();
				vector pos = GetGame().GetCurrentCameraPosition() - dir * 0.5;
				pos[1] = player.GetPosition()[1];
				m_PlayerPreview = PlayerBase.Cast(GetGame().CreateObjectEx(player.GetType(), pos, ECE_LOCAL|ECE_NOLIFETIME));
				m_PlayerPreview.SetOrientation(player.GetOrientation());

				m_MarketMenuController.MarketPlayerPreview = m_PlayerPreview;
				m_MarketMenuController.NotifyPropertyChanged("MarketPlayerPreview");
			}
			else
			{
				//! Remove existing attachments on player preview
				array<ItemBase> previewAttachments = new array<ItemBase>;
				for (i = 0; i < m_PlayerPreview.GetInventory().AttachmentCount(); i++)
				{
					ItemBase previewItem = ItemBase.Cast(m_PlayerPreview.GetInventory().GetAttachmentFromIndex(i));
					//! Removing the attachment here would change attachment count, so add it to a temp array for later removal
					if (previewItem)
						previewAttachments.Insert(previewItem);
				}
				foreach (ItemBase previewAttachment: previewAttachments)
				{
					m_PlayerPreview.GetInventory().LocalDestroyEntity(previewAttachment);
				}
			}

			//! Add selected item first to override any current player attachment on same slot
			EntityAI item = m_PlayerPreview.GetInventory().CreateAttachment(previewClassName);
			if (GetSelectedMarketItem().SpawnAttachments.Count() && GetSelectedMarketItemElement().GetIncludeAttachments())
				GetSelectedMarketItemElement().SpawnAttachments(GetSelectedMarketItem(), item);

			m_CurrentPreviewObject = item;
			
			//! Get slot (for dynamic preview focus)
			m_SelectedItemSlot = -1;
			if (item)
			{
				InventoryLocation loc = new InventoryLocation;
				item.GetInventory().GetCurrentInventoryLocation(loc);
				if (loc)
					m_SelectedItemSlot = loc.GetSlot();
			}

			//! Add player's current attachments to player preview
			CreateAttachments(player, m_PlayerPreview);

			if (!m_LastPositionPlayerPreviewW)
				UpdateLastPositionPlayerPreview(1.0);
			else
				m_LastPositionPlayerPreviewY = market_player_preview.GetModelPosition()[1];

			AnimatePositionPlayerPreview();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu CreateAttachments
	// ------------------------------------------------------------
	//! Create all attachments from src in dst
	void CreateAttachments(EntityAI src, EntityAI dst)
	{
		if (!src.GetInventory())
			return;

		for (int i = 0; i < src.GetInventory().AttachmentCount(); i++)
		{
			ItemBase srcAttachment = ItemBase.Cast(src.GetInventory().GetAttachmentFromIndex(i));
			if (srcAttachment)
			{
				//! Create attachment
				EntityAI dstAttachment;
				if (dst.IsInherited(Weapon_Base))
					dstAttachment = Weapon_Base.Cast(dst).ExpansionCreateInInventory(srcAttachment.GetType());
				else
					dstAttachment = dst.GetInventory().CreateAttachment(srcAttachment.GetType());
				if (dstAttachment)
					CreateAttachments(srcAttachment, dstAttachment);  //! Create attachments of attachment
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateItemDescriptionPanel
	// ------------------------------------------------------------
	void UpdateItemDescriptionPanel()
	{		
		string text = m_MarketMenuController.MarketItemDesc;
		int length = text.Length();
		if (length > 220)
		{
			market_item_description_scroll_indicator_header.Show(false);
			market_item_description_scroll_indicator_footer.Show(true);
		}
		else
		{
			market_item_description_scroll_indicator_footer.Show(false);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ClearSkins
	// ------------------------------------------------------------
	void ClearSkins()
	{
		m_MarketMenuController.SkinsDropdownElements.Clear();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateItemSkins
	// ------------------------------------------------------------
	void UpdateItemSkins()
	{
		ClearSkins();

		if (!GetSelectedMarketItemElement())
			return;
		
		ExpansionMarketItem baseItem = GetSelectedMarketItemElement().GetBaseItem();

		map<string, ref array<ref ExpansionMarketMenuSkinsDropdownElement>> dropdownElements = new map<string, ref array<ref ExpansionMarketMenuSkinsDropdownElement>>;
		ExpansionMarketMenuSkinsDropdownElement dropdownElement;
		array<ref ExpansionMarketMenuSkinsDropdownElement> currentElements;

		ExpansionSkinModule skinModule = ExpansionSkinModule.Cast(GetModuleManager().GetModule(ExpansionSkinModule));
		if (skinModule)
		{
			array<ExpansionSkin> skins = new array<ExpansionSkin>;
			string defaultSkin;
			int currentSkinIndex;
			string currentSkinName;
			
			skinModule.RetrieveSkins(baseItem.ClassName, skins, defaultSkin);
			
			for (int i = 0; i < skins.Count(); i++)
			{
				currentSkinIndex = i;
				currentSkinName = skinModule.GetSkinName(baseItem.ClassName, currentSkinIndex);

				//! If skin is added as variant, skip it
				currentSkinName.ToLower();
				if (baseItem.Variants.Find(baseItem.ClassName + "_" + currentSkinName) > -1)
					continue;

				currentSkinName.ToUpper();
				dropdownElement = new ExpansionMarketMenuSkinsDropdownElement(this, i, currentSkinName);
				if (!dropdownElements.Find(currentSkinName, currentElements))
				{
					currentElements = new array<ref ExpansionMarketMenuSkinsDropdownElement>;
					dropdownElements.Insert(currentSkinName, currentElements);
				}
				currentElements.Insert(dropdownElement);
			}
		}

		TStringArray variants = new TStringArray;

		//! If we have skins, the base class is already included, otherwise we have to add it
		if (dropdownElements.Count() == 0)
			variants.Insert(baseItem.ClassName);

		variants.InsertAll(baseItem.Variants);

		string baseDisplayName = ExpansionStatic.GetItemDisplayNameWithType(baseItem.ClassName, m_TypeDisplayNames);

		map<string, string> displayNames = new map<string, string>;
		string displayName;
		map<string, ref TStringArray> displayNameClasses = new map<string, ref TStringArray>;
		TStringArray classNames;

		foreach (string className : variants)
		{
			displayName = "";

			//! Check if this is a Expansion skin type - e.g. <ClassName>_<SkinName> - and use skin name in that case
			if (skinModule)
				displayName = skinModule.GetSkinName( className );

			//! Fall back to display name
			if (!displayName)
				displayName = ExpansionStatic.GetItemDisplayNameWithType(className, m_TypeDisplayNames);

			displayNames.Insert(className, displayName);
			if (!displayNameClasses.Find(displayName, classNames))
			{
				classNames = new TStringArray;
				displayNameClasses.Insert(displayName, classNames);
			}
			classNames.Insert(className);
		}

		map<string, int> seenDisplayNames = new map<string, int>;
		int count;

		foreach (string variant : variants)
		{
			displayName = displayNames.Get(variant);

			//! Check if it's the same display name as base item and if so, try to derive a suitable diff string from the classname
			classNames = displayNameClasses.Get(displayName);
			if (classNames.Count() > 1)
			{
				string diff = "";
				//! Split base item classname and variant classname using "_" delimiter
				//! Use part with lowest amount of occurences
				//! E.g. given these two classnames (base, variant):
				//! TrackSuitJacket_Black, TrackSuitJacket_Blue -> TrackSuitJacket: 2, Black: 1, Blue: 1 -> Blue will get used
				TStringArray tempPartsVariant = new TStringArray;
				variant.Split("_", tempPartsVariant);
				if (tempPartsVariant.Count() > 1)
				{
					TStringArray tempParts = new TStringArray;
					classNames[0].Split("_", tempParts);
					tempParts.InsertAll(tempPartsVariant);
					TStringArray partNames = new TStringArray;
					map<string, int> parts = new map<string, int>;
					foreach (string tempPart : tempParts)
					{
						if (classNames[0].IndexOf(tempPart) == 0 || variant.IndexOf(tempPart) == 0)
							continue;
						if (!parts.Contains(tempPart))
						{
							partNames.Insert(tempPart);
							parts.Insert(tempPart, 1);
						}
						else
						{
							parts.Set(tempPart, parts.Get(tempPart) + 1);
						}
					}
					int lowestCount = -1;
					foreach (string part : partNames)
					{
						count = parts.Get(part);
						if (lowestCount == -1 || count <= lowestCount)
						{
							lowestCount = count;
							diff = part;
						}
					}
					displayName = diff;
				}
				else if (variant == baseItem.ClassName)
				{
					displayName = "Default";
				}
				else
				{
					//! Fallback
					displayName = variant;
				}
			}

			if (seenDisplayNames.Contains(displayName))
			{
				count = seenDisplayNames.Get(displayName) + 1;
				seenDisplayNames.Set(displayName, count);
				//! Add sequential number
				displayName += " (" + count + ")";
			}
			else
			{
				seenDisplayNames.Insert(displayName, 1);
			}

			displayName.ToUpper();
			dropdownElement = new ExpansionMarketMenuSkinsDropdownElement(this, -1, displayName, variant);
			if (!dropdownElements.Find(displayName, currentElements))
			{
				currentElements = new array<ref ExpansionMarketMenuSkinsDropdownElement>;
				dropdownElements.Insert(displayName, currentElements);
			}
			currentElements.Insert(dropdownElement);
		}

		TStringArray names = dropdownElements.GetKeyArray();
		names.Sort();

		foreach (string name : names)
		{
			currentElements = dropdownElements.Get(name);
			foreach (ExpansionMarketMenuSkinsDropdownElement currentElement : currentElements)
			{
				m_MarketMenuController.SkinsDropdownElements.Insert(currentElement);
			}
		}

		skin_selector.Show(dropdownElements.Count() > 1);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetItemAttachmentsInfo
	// ------------------------------------------------------------
	void SetItemAttachmentsInfo( ExpansionMarketMenuItem itemElement)
	{
		if (m_AttachmentsTooltip)
		{
			if (market_item_info_attachments.IsVisible()) 
				market_item_info_attachments.Show(false);
			
			delete m_AttachmentsTooltip;
		}
		
		if (itemElement)
		{
			if (itemElement.GetMarketItem().SpawnAttachments.Count() > 0 && itemElement.GetIncludeAttachments())
			{
				m_IncludeAttachmentsData = true;
				
				m_AttachmentsTooltip = new ExpansionMarketMenuTooltip();
				m_AttachmentsTooltip.Hide();
				m_AttachmentsTooltip.SetContentOffset(-0.234375, 0.006944);
				m_AttachmentsTooltip.SetTitle(ExpansionStatic.GetItemDisplayNameWithType(GetSelectedMarketItemElement().GetMarketItem().ClassName) + " #STR_EXPANSION_MARKET_TOOLTIP_ATTACHMENTS_TITLE");
				m_AttachmentsTooltip.SetText("#STR_EXPANSION_MARKET_TOOLTIP_ATTACHMENTS_TEXT");
				m_AttachmentsTooltip.ClearEntries();
				foreach (string attachment: GetSelectedMarketItemElement().GetMarketItem().SpawnAttachments)
				{
					m_AttachmentsTooltip.AddEntry(attachment);
				}
				
				market_item_info_attachments.Show(true);
				market_item_info_attachments_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoAttachments));
			}
			else if (itemElement.GetMarketItem().SpawnAttachments.Count() == 0)
			{
				m_IncludeAttachmentsData = false;
			}
			//market_item_info_attachments_panel.Show(itemElement.GetMarketItem().SpawnAttachments.Count() > 0);
			//market_item_info_attachments_setup_panel.Show(itemElement.GetMarketItem().SpawnAttachments.Count() > 0);
		
			if (!itemElement.GetIncludeAttachments())
			{
				if (market_item_info_attachments.IsVisible()) 
					market_item_info_attachments.Show(false);
			}
		}
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateItemFieldFromBasicNetSync
	// ------------------------------------------------------------
	void UpdateItemFieldFromBasicNetSync()
	{
		MarketPrint("UpdateItemFieldFromBasicNetSync - Start");
		
		if (!GetSelectedMarketItem())
			return;

		if (GetSelectedMarketItem().IsStaticStock())
			m_TraderItemStock = 1;
		else
			m_TraderItemStock = m_MarketModule.GetClientZone().GetStock(GetSelectedMarketItem().ClassName, true);
		m_PlayerStock = m_MarketModule.GetAmountInInventory(GetSelectedMarketItem(), m_MarketModule.LocalGetEntityInventory());
		
		MarketPrint("UpdateItemFieldFromBasicNetSync - GetSelectedMarketItem().ClassName: " + GetSelectedMarketItem().ClassName);
		MarketPrint("UpdateItemFieldFromBasicNetSync - m_TraderItemStock: " + m_TraderItemStock);
		
		m_MarketSell = new ExpansionMarketSell;
		m_MarketSell.Item = GetSelectedMarketItem();
		m_MarketSell.Trader = m_TraderObject;
		
		if (m_PlayerStock > 0)
		{
			if (m_MarketModule.FindSellPrice(PlayerBase.Cast(GetGame().GetPlayer()), m_MarketModule.LocalGetEntityInventory(), m_TraderItemStock, m_Quantity, m_MarketSell))
			{
				m_SellPrice = m_MarketSell.Price;
				m_MarketMenuController.MarketItemTotalSellPrice = ExpansionStatic.IntToCurrencyString(m_SellPrice, ",");
			}
			else
			{
				m_SellPrice = -1;
				m_MarketMenuController.MarketItemTotalSellPrice = "";
			}
			market_item_sell_price_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText)); 
			market_item_sell_price_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		}
		else
		{
			float sellPricePct = GetSelectedMarketItem().SellPricePercent;
			if (sellPricePct < 0)
				sellPricePct = m_MarketModule.GetClientZone().SellPricePercent;
			if (sellPricePct < 0)
				sellPricePct = GetExpansionSettings().GetMarket().SellPricePercent;
			m_SellPrice = GetSelectedMarketItem().CalculatePrice(m_TraderItemStock + m_Quantity, sellPricePct / 100);
			m_MarketMenuController.MarketItemTotalSellPrice = ExpansionStatic.IntToCurrencyString(m_SellPrice, ",");
			market_item_sell_price_text.SetColor(COLOR_EXPANSION_NOTIFICATION_EXPANSION);
			market_item_sell_price_icon.SetColor(COLOR_EXPANSION_NOTIFICATION_EXPANSION);
		}
		
		m_MarketMenuController.NotifyPropertyChanged("MarketItemTotalSellPrice");
		market_item_sell_price_icon.Show(m_SellPrice > -1);

		ExpansionMarketCurrency price = 0;
		if (m_MarketModule.FindPriceOfPurchase(GetSelectedMarketItem(), m_MarketModule.GetClientZone(), m_TraderMarket, m_Quantity, price, GetSelectedMarketItemElement().GetIncludeAttachments()))
		{
			m_BuyPrice = price;
			m_MarketMenuController.MarketItemTotalBuyPrice = ExpansionStatic.IntToCurrencyString(m_BuyPrice, ",");
		}
		else
		{
			m_BuyPrice = -1;
			m_MarketMenuController.MarketItemTotalBuyPrice = "";
		}
				
		m_MarketMenuController.NotifyPropertyChanged("MarketItemTotalBuyPrice");
		market_item_buy_price_icon.Show(m_BuyPrice > -1);
		
		if (GetMarketTrader().CanBuyItem(GetSelectedMarketItem().ClassName))
		{
			if (m_TraderItemStock == 0)
			{
				if (GetSelectedMarketItem().MaxStockThreshold > 0)
				{
					m_MarketMenuController.MarketItemStockTrader = "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK";
					market_item_info_stock.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
				}
				else
				{
					m_MarketMenuController.MarketItemStockTrader = "#STR_EXPANSION_MARKET_ITEM_NOTINSTOCK";
					market_item_info_stock.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
				}
				market_item_buy.Show(false);
			}
			else
			{
				if (GetSelectedMarketItem().IsStaticStock())
				{
					m_MarketMenuController.MarketItemStockTrader = "#STR_EXPANSION_MARKET_ITEM_INSTOCK";
					market_item_info_stock.SetColor(COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
				}
				else
				{
					m_MarketMenuController.MarketItemStockTrader = m_TraderItemStock.ToString() + " #STR_EXPANSION_MARKET_ITEM_INSTOCK";
					market_item_info_stock.SetColor(COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
				}
				market_item_buy.Show(m_Quantity != 0 && m_BuyPrice > -1);
			}
		}
		else
		{
			m_MarketMenuController.MarketItemStockTrader = "#STR_EXPANSION_MARKET_ITEM_CANT_BUY";
			market_item_info_stock.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
		}

		m_MarketMenuController.MarketQuantity = m_Quantity.ToString();

		m_MarketMenuController.NotifyPropertyChanged("MarketItemStockTrader");
		m_MarketMenuController.NotifyPropertyChanged("MarketQuantity");
		
		if (GetMarketTrader().CanSellItem(GetSelectedMarketItem().ClassName))
		{
			if (m_PlayerStock >= 0)
			{
				if (m_PlayerStock == 0)
				{
					m_MarketMenuController.MarketItemStockPlayer = "0 #STR_EXPANSION_MARKET_ITEM_ONPLAYER";
					market_item_info_stock_player.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
					market_item_sell.Show(false);
				}
				else if (m_Quantity > m_PlayerStock)
				{
					m_MarketMenuController.MarketItemStockPlayer = "#STR_EXPANSION_MARKET_ITEM_NOT_ENOUGH";
					market_item_info_stock_player.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
					market_item_sell.Show(false);
				}
				else
				{
					m_MarketMenuController.MarketItemStockPlayer = m_PlayerStock.ToString() + " #STR_EXPANSION_MARKET_ITEM_ONPLAYER";
					market_item_info_stock_player.SetColor(COLOR_EXPANSION_NOTIFICATION_SUCCSESS);
					market_item_sell.Show(m_Quantity != 0 && m_SellPrice > -1);
				}
			}
			else
			{
				m_MarketMenuController.MarketItemStockPlayer = "#STR_EXPANSION_MARKET_ITEM_CANT_SELL";
				market_item_info_stock_player.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
				market_item_sell.Show(false);
			}
		}
		else
		{
			m_MarketMenuController.MarketItemStockPlayer = "#STR_EXPANSION_MARKET_ITEM_CANT_SELL";
			market_item_info_stock_player.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
			market_item_sell.Show(false);
		}

		m_MarketMenuController.NotifyPropertyChanged("MarketItemStockPlayer");
		
		UpdateMonieDenominations();
		
		MarketPrint("UpdateItemFieldFromBasicNetSync - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateMonieDenominations
	// Should update and display the current needed monie denominations
	// for the selected and currently displayed market item
	// ------------------------------------------------------------
	void UpdateMonieDenominations()
	{
		array<string> denoms = m_MarketModule.GetMoneyDenominations();
		array<int> monies = new array<int>;
		int i;
		string prefix;
		
		TStringArray descriptions;
		TStringArray sellDescriptions = new TStringArray;
		TStringArray sellChangeDescriptions = new TStringArray;
		TStringArray buyDescriptions = new TStringArray;
		TStringArray buyChangeDescriptions = new TStringArray;
		
		m_MarketModule.FindMoneyAndCountTypes(NULL, m_SellPrice, monies, false, GetSelectedMarketItem(), m_TraderMarket);
		for (i = 0; i < monies.Count(); i++)
		{
			if (monies[i] > 0)
			{
				//! Player will get during sell transaction
				prefix = "+";
				descriptions = sellDescriptions;
			}
			else if (monies[i] < 0)
			{
				//! Player will give during sell transaction (change)
				prefix = "-";
				descriptions = sellChangeDescriptions;
			}
			else
			{
				prefix = "";
			}
			
			#ifdef EXPANSIONMODMARKET_DEBUG
			MarketPrint("UpdateMonieDenominations - Sell - " + prefix + monies[i].ToString() + " " + denoms[i] + " " + GetDisplayName(denoms[i]));
			#endif
			
			if (monies[i] != 0)
				descriptions.Insert(prefix + Math.AbsInt(monies[i]).ToString() + " × " + GetDisplayName(denoms[i]));
		}
		
		monies.Clear();
		
		if (!m_SellDenomsTooltip)
		{
			m_SellDenomsTooltip = new ExpansionMarketMenuTooltip();
			m_SellDenomsTooltip.Hide();
			m_SellDenomsTooltip.SetTitle("#STR_EXPANSION_MARKET_YOU_GET");
			m_SellDenomsTooltip.SetContentOffset(-0.273438, -0.277778);
		}
		else
		{
			m_SellDenomsTooltip.ClearEntries();
		}

		foreach (string sellDesc: sellDescriptions)
		{
			m_SellDenomsTooltip.AddEntry(sellDesc);
		}
		
		sellDescriptions.Clear();

		if (sellChangeDescriptions.Count())
		{
			m_SellDenomsTooltip.AddEntry("");
			m_SellDenomsTooltip.AddEntry("#STR_EXPANSION_MARKET_YOU_PAY");
			foreach (string sellChangeDesc: sellChangeDescriptions)
			{
				m_SellDenomsTooltip.AddEntry(sellChangeDesc);
			}
		}
		
		sellChangeDescriptions.Clear();
		
		m_MarketModule.FindMoneyAndCountTypes(PlayerBase.Cast(GetGame().GetPlayer()), m_BuyPrice, monies, false, GetSelectedMarketItem(), m_TraderMarket);
		for (i = 0; i < monies.Count(); i++)
		{
			if (monies[i] < 0)
			{
				//! Player will get during buy transaction (change)
				prefix = "+";
				descriptions = buyChangeDescriptions;
			}
			else if (monies[i] > 0)
			{
				//! Player will give during buy transaction
				prefix = "-";
				descriptions = buyDescriptions;
			}
			else
			{
				prefix = "";
			}
			
			#ifdef EXPANSIONMODMARKET_DEBUG
			MarketPrint("UpdateMonieDenominations - Buy - " + prefix + monies[i].ToString() + " " + denoms[i] + " " + GetDisplayName(denoms[i]));
			#endif
		
			if (monies[i] != 0)
				descriptions.Insert(prefix + Math.AbsInt(monies[i]).ToString() + " × " + GetDisplayName(denoms[i]));
		}
		
		monies.Clear();
		
		if (!m_BuyDenomsTooltip)
		{
			m_BuyDenomsTooltip = new ExpansionMarketMenuTooltip();
			m_BuyDenomsTooltip.Hide();
			m_BuyDenomsTooltip.SetTitle("#STR_EXPANSION_MARKET_YOU_PAY");
			m_BuyDenomsTooltip.SetContentOffset(-0.390625, -0.277778);
		}
		else
		{
			m_BuyDenomsTooltip.ClearEntries();
		}

		foreach (string buyDesc: buyDescriptions)
		{
			m_BuyDenomsTooltip.AddEntry(buyDesc);
		}
		
		buyDescriptions.Clear();

		if (buyChangeDescriptions.Count())
		{
			m_BuyDenomsTooltip.AddEntry("");
			m_BuyDenomsTooltip.AddEntry("#STR_EXPANSION_MARKET_YOU_GET");
			foreach (string buyChangeDesc: buyChangeDescriptions)
			{
				m_BuyDenomsTooltip.AddEntry(buyChangeDesc);
			}
		}
		
		buyChangeDescriptions.Clear();
	}

	string GetDisplayName(string type)
	{
		//! Uses a cache to speed up repeat lookups
		type.ToLower();
		return ExpansionStatic.GetItemDisplayNameWithType(type, m_TypeDisplayNames);
	}

	string GetPreviewClassName(string className, bool ignoreBaseBuildingKits = false)
	{
		if (GetGame().ConfigIsExisting("CfgVehicles " + className + "_ExpansionMarketPreview"))
		{
			return className + "_ExpansionMarketPreview";
		}
		else if (!ignoreBaseBuildingKits && className.IndexOf("kit") == className.Length() - 3)
		{
			//! Special handling for Expansion
			if (GetGame().IsKindOf(className, "ExpansionKitLarge"))
			{
				string path = "CfgVehicles " + className + " placingTypes";
				if (GetGame().ConfigIsExisting(path))
				{
					TStringArray placingTypes = new TStringArray;
					GetGame().ConfigGetTextArray(path, placingTypes);
					foreach (string placingType : placingTypes)
					{
						path = "CfgVehicles " + placingType + " deployType";
						if (GetGame().ConfigIsExisting(path))
						{
							return GetGame().ConfigGetTextOut(path);
						}
					}
				}
			}

			if (className == "fencekit" || className == "watchtowerkit" || className == "territoryflagkit")
			{
				//! Item name is kit name without "kit" at the end
				string previewClassName = className.Substring(0, className.Length() - 3);
				if (GetGame().ConfigIsExisting("CfgVehicles " + previewClassName))
					return previewClassName;
			}
		}
		return className;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnIncrementQuantityButtonClick
	// ------------------------------------------------------------
	void OnIncrementQuantityButtonClick()
	{
		if (ExpansionStatic.Key_CTRL())
		{
			m_Quantity = m_Quantity + 10;
		}
		else if (ExpansionStatic.Key_SHIFT())
		{
			m_Quantity = m_Quantity + 100;
		}
		else if (ExpansionStatic.Key_ALT())
		{
			int playerQuantity = m_MarketModule.GetAmountInInventory(GetSelectedMarketItem(), m_MarketModule.LocalGetEntityInventory());
			if (playerQuantity > 0)
			{
				m_Quantity = playerQuantity;
			}
			else
			{
				m_Quantity = 1;
			}
		}
		else
		{
			m_Quantity++;
		}
		
		UpdateQuantity();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnDecrementQuantityButtonClick
	// ------------------------------------------------------------
	void OnDecrementQuantityButtonClick()
	{
		if (ExpansionStatic.Key_CTRL())
		{
			if ( m_Quantity - 10 > 0 )
				m_Quantity = m_Quantity - 10;
		}
		else if (ExpansionStatic.Key_SHIFT())
		{
			if ( m_Quantity - 100 > 0 )
				m_Quantity = m_Quantity - 100;
		}
		else if (ExpansionStatic.Key_ALT())
		{
			m_Quantity = 1;
		}
		else
		{
			if ( m_Quantity > 1 )
				m_Quantity--;
		}

		UpdateQuantity();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateQuantity
	// ------------------------------------------------------------
	void UpdateQuantity()
	{
		if (!GetSelectedMarketItem())
			return;
		
		int limit = m_TraderItemStock;
		int absPlayerStock = Math.AbsInt(m_PlayerStock);
		if (limit < absPlayerStock)
		{
			limit = absPlayerStock;
		}

		if (limit > 0)
		{
			if ( m_Quantity > limit && !GetSelectedMarketItem().IsStaticStock() )
			{
				m_Quantity = limit;
			} 
		}
		
		UpdateItemFieldFromBasicNetSync();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu MenuCallback
	// ------------------------------------------------------------
	void MenuCallback(ExpansionMarketResult result, int option = -1, Object object = NULL)
	{
		MarketPrint("MenuCallback - Start - " + typename.EnumToString(ExpansionMarketResult, result));
		
		if (result == ExpansionMarketResult.Success)
		{
			ExpansionMarketMenuState state = m_CurrentState;
			
			switch (state)
			{
				case ExpansionMarketMenuState.REQUESTING_PURCHASE:
				{
					SetMenuState(ExpansionMarketMenuState.CONFIRMING_PURCHASE);
					m_MarketModule.ConfirmPurchase(GetSelectedMarketItem().ClassName, PlayerBase.Cast(GetGame().GetPlayer()), GetSelectedMarketItem().SpawnAttachments.Count() > 0 && GetSelectedMarketItemElement().GetIncludeAttachments(), GetSelectedMarketItemElement().m_CurrentSelectedSkinIndex);
					
					break;
				}
				case ExpansionMarketMenuState.REQUESTING_SELL:
				{
					SetMenuState(ExpansionMarketMenuState.CONFIRMING_SELL);
					m_MarketModule.ConfirmSell(GetSelectedMarketItem().ClassName, PlayerBase.Cast(GetGame().GetPlayer()));
					
					break;
				}
				default:
				{
					//! Transaction went through
					PlayMarketSound();
					
					AfterNetworkCallback();
				}
			}
		} 
		else
		{
			string title;
			string text;
			string tmp;
			StringLocaliser loc;

			if (result >= ExpansionMarketResult.FailedNoVehicleSpawnPositions && result <= ExpansionMarketResult.FailedVehicleSpawnOccupied)
			{
				tmp = typename.EnumToString(ExpansionMarketVehicleSpawnType, option);
			}

			if (result == ExpansionMarketResult.FailedStockChange)
			{
				MarketPrint("MenuCallback - The stock has changed meanwhile");
				
				EXPrint("MenuCallback - Current stock: " + m_MarketModule.GetClientZone().GetStock(GetSelectedMarketItem().ClassName, true));
				if (m_CurrentState == ExpansionMarketMenuState.REQUESTING_SELL || m_CurrentState == ExpansionMarketMenuState.CONFIRMING_SELL)
				{
					if (m_MarketSell)
						m_MarketSell.Debug();
				}
				else
				{
					//! TODO: Add purchase debug?
				}
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_STOCK_CHANGED";
			} 
			else if (result == ExpansionMarketResult.FailedOutOfStock)
			{
				MarketPrint("MenuCallback - The item is out of stock");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_OUT_OF_STOCK";
			}
			else if (result == ExpansionMarketResult.FailedAttachmentOutOfStock)
			{
				MarketPrint("MenuCallback - The item is out of stock");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_ATTACHMENT_OUT_OF_STOCK";
			}
			else if (result == ExpansionMarketResult.FailedNotEnoughMoney)
			{
				MarketPrint("MenuCallback - player did not have enough money");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_NOT_ENOGH_MONEY";
			}
			else if (result == ExpansionMarketResult.FailedReserveTime) 
			{ 
				MarketPrint("MenuCallback - 30 seconds have passed");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_RESERVATION_FAILED";
			} 
			else if (result == ExpansionMarketResult.FailedNoCount) 
			{ 
				MarketPrint("MenuCallback - player did not set item count");
				
				title = "STR_EXPANSION_TRADER_TRANSACTION_FAILED";
				text = "STR_EXPANSION_TRADER_NONZERO";
			} 
			else if (result == ExpansionMarketResult.FailedNoVehicleSpawnPositions)
			{
				MarketPrint("MenuCallback - no vehicle spawn positions");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_NO_" + tmp + "_SPAWN_POSITIONS";
			}
			else if (result == ExpansionMarketResult.FailedNotEnoughVehicleSpawnPositionsNear)
			{
				MarketPrint("MenuCallback - not enough vehicle spawn positions near trader");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_NOT_ENOUGH_" + tmp + "_SPAWN_POSITIONS_NEAR";
				float minDistance = GetExpansionSettings().GetMarket().GetMinVehicleDistanceToTrader(GetSelectedMarketItem().ClassName);
				float maxDistance = GetExpansionSettings().GetMarket().GetMaxVehicleDistanceToTrader(GetSelectedMarketItem().ClassName);
				loc = new StringLocaliser(text, minDistance.ToString(), maxDistance.ToString());
				text = loc.Format();
				
				MarketPrint("MenuCallback - " + text);
			}
			else if (result == ExpansionMarketResult.FailedVehicleSpawnOccupied)
			{
				MarketPrint("MenuCallback - vehicle spawn occupied");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_ANOTHER_VEHICLE_ON_SPAWN";

				if (object)
				{
					loc = new StringLocaliser(text);
					string displayName;
					PlayerBase player = PlayerBase.Cast(object);
					if (player && player.GetIdentity())
						displayName = player.GetIdentityName();  //! So you can call 'em out in chat - unless it's yourself...
					else
						displayName = object.GetDisplayName();
					text = loc.Format() + " (" + displayName + ")";
				}
			}
			else if (result == ExpansionMarketResult.FailedTooFarAway)
			{
				MarketPrint("MenuCallback - player is too far from trader");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_TOO_FAR_AWAY";
			}
			else if (result == ExpansionMarketResult.FailedCannotSell)
			{
				MarketPrint("MenuCallback - cannot sell this item to this trader");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_CANNOTSELL";
			}
			else if (result == ExpansionMarketResult.FailedCannotBuy)
			{
				MarketPrint("MenuCallback - cannot buy this item at this trader");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_CANNOTBUY";
			}
			else if (result == ExpansionMarketResult.FailedNotInPlayerPossession)
			{
				MarketPrint("MenuCallback - cannot sell something that is not in player's possession");
				
				title = "STR_EXPANSION_MARKET_TITLE";
				text = "STR_EXPANSION_TRADER_NOT_IN_PLAYER_POSSESSION";
			}
			else
			{
				MarketPrint("MenuCallback - unknown error");

				title = "STR_EXPANSION_MARKET_TITLE";
				text = "UNKNOWN ERROR";
			}

			ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 3, ExpansionNotificationType.MARKET).Create();
		
			AfterNetworkCallback();
		}
				
		MarketPrint("MenuCallback - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu AfterNetworkCallback
	// ------------------------------------------------------------	
	void AfterNetworkCallback()
	{
		if (GetSelectedMarketItem())
		{
			EXPrint("AfterNetworkCallback - has market item - request selected item");
			RequestSelectedItem(ExpansionMarketMenuState.LOADING);
		}
		else
		{
			EXPrint("AfterNetworkCallback - no market item - request trader data");
			SetIsLoading(true);
			m_MarketModule.RequestTraderData(m_TraderObject);
		}
	}

	void RequestSelectedItem(ExpansionMarketMenuState menuState)
	{
		EXPrint(ToString() + "::RequestSelectedItem - " + typename.EnumToString(ExpansionMarketMenuState, menuState));
		TIntArray itemIDs = new TIntArray;
		if (m_CurrentState == ExpansionMarketMenuState.CONFIRMING_SELL && m_MarketSell)
		{
			//! Last action was a sale. Request stock info for sale item and its attachments
			foreach (ExpansionMarketSellItem sellItem: m_MarketSell.Sell)
			{
				string className = sellItem.ClassName;
				className.ToLower();
				ExpansionMarketItem item = GetExpansionSettings().GetMarket().GetItem(className, false);
				if (item && itemIDs.Find(item.ItemID) == -1)
					itemIDs.Insert(item.ItemID);
			}
		}
		else
		{
			//! Last action was not a sale. Request stock info for selected item and its attachments
			itemIDs.Insert(GetSelectedMarketItem().ItemID);
			TIntArray attachmentIDs = GetCurrentSelectedAttachmentIDs(true);
			foreach (int attachmentID: attachmentIDs)
			{
				if (itemIDs.Find(attachmentID) == -1)
					itemIDs.Insert(attachmentID);
			}
		}
		SetMenuState(menuState);
		m_MarketModule.RequestTraderItems(m_TraderObject, 0, m_TraderMarket.m_StockOnly, itemIDs);
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu OnBuyButtonClick
	// ------------------------------------------------------------
	void OnBuyButtonClick()
	{		
		if (!GetSelectedMarketItem())
			return;
		
		if (!m_MarketModule)
			return;
		
		if (GetExpansionClientSettings().MarketMenuSkipConfirmations)
		{
			OnConfirmBuyButtonClick();
		}
		else
		{
			SetMenuState(ExpansionMarketMenuState.DIALOG);
			ExpansionMarketMenuDialogData dialogData = new ExpansionMarketMenuDialogData;
			dialogData.ClassName = GetSelectedMarketItem().ClassName;
			dialogData.Amount = m_Quantity;
			dialogData.Price = m_BuyPrice;
			dialogData.IncludeAttachments = m_IncludeAttachmentsData;
			
			m_PurchaseDialog = new ExpansionMenuDialog_MarketConfirmPurchase(this, dialogData);
			m_PurchaseDialog.Show();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnConfirmBuyButtonClick
	// ------------------------------------------------------------
	void OnConfirmBuyButtonClick()
	{
		if (!GetSelectedMarketItem())
			return;
		
		if (!m_MarketModule)
			return;
		
		SetMenuState(ExpansionMarketMenuState.REQUESTING_PURCHASE);
		m_MarketModule.RequestPurchase(GetSelectedMarketItem().ClassName, m_Quantity, m_BuyPrice, m_TraderObject, NULL, GetSelectedMarketItem().SpawnAttachments.Count() > 0 && GetSelectedMarketItemElement().GetIncludeAttachments(), GetSelectedMarketItemElement().m_CurrentSelectedSkinIndex, GetCurrentSelectedAttachmentIDs());
		if (m_PurchaseDialog)
			m_PurchaseDialog.Hide();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnSellButtonClick
	// ------------------------------------------------------------
	void OnSellButtonClick()
	{
		if (!GetSelectedMarketItem())
			return;

		if (!m_MarketModule)
			return;
		
		if (GetExpansionClientSettings().MarketMenuSkipConfirmations)
		{
			OnConfirmSellButtonClick();
		}
		else
		{
			SetMenuState(ExpansionMarketMenuState.DIALOG);
			ExpansionMarketMenuDialogData dialogData = new ExpansionMarketMenuDialogData;
			dialogData.ClassName = m_SelectedMarketItem.ClassName;
			dialogData.Amount = m_Quantity;
			dialogData.Price = m_SellPrice;
			dialogData.IncludeAttachments = m_IncludeAttachmentsData;
			
			m_SellDialog = new ExpansionMenuDialog_MarketConfirmSell(this, dialogData);
			m_SellDialog.Show();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnConfirmSellButtonClick
	// ------------------------------------------------------------
	void OnConfirmSellButtonClick()
	{
		if (!GetSelectedMarketItem())
			return;

		if (!m_MarketModule)
			return;
		
		if (m_PlayerStock == 0)
		{
			ExpansionNotification("STR_EXPANSION_MARKET_TITLE", "STR_EXPANSION_MARKET_DONT_HAVE_ITEM", EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 3, ExpansionNotificationType.MARKET).Create();
			return;
		}
		
		SetMenuState(ExpansionMarketMenuState.REQUESTING_SELL);
		m_MarketModule.RequestSell(GetSelectedMarketItem().ClassName, m_Quantity, m_SellPrice, m_TraderObject);
		if (m_SellDialog)
			m_SellDialog.Hide();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnQuantityButtonClick
	// ------------------------------------------------------------
	void OnQuantityButtonClick()
	{
		if (!m_QuantityDialog)
			m_QuantityDialog = new ExpansionMenuDialog_MarketSetQuantity(this);
		
		SetMenuState(ExpansionMarketMenuState.DIALOG);	
		m_QuantityDialog.Show();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetQuantity
	// ------------------------------------------------------------
	void SetQuantity(string quantity)
	{
		m_Quantity = quantity.ToInt();
		
		if (m_QuantityDialog)
		{
			m_QuantityDialog.Hide();
			delete m_QuantityDialog;
		}
		
		UpdateItemFieldFromBasicNetSync();
		SetMenuState(ExpansionMarketMenuState.NONE);	
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetQuantity
	// ------------------------------------------------------------
	int GetQuantity()
	{
		return m_Quantity;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnFilterButtonClick
	// ------------------------------------------------------------
	void OnFilterButtonClick()
	{		
		market_filter_box.SetText("");		
		UpdateMarketCategories();
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		EXPrint("ExpansionMarketMenu::OnHide - Start");
				
		ExpansionMarketModule.SI_SetTraderInvoker.Remove(SetTraderObject);
		ExpansionMarketModule.SI_SelectedItemUpdatedInvoker.Remove(OnNetworkItemUpdate);
		ExpansionMarketModule.SI_Callback.Remove(MenuCallback);

		if (m_PlayerPreview)
			GetGame().ObjectDelete(m_PlayerPreview);
	
		m_SelectedMarketItem = NULL;
		
		ClearCategoryItems();
		ClearCategories();
		ClearDropdownElements();
		ClearSkins();
		
		if (m_FilterOptionStrings)
		{
			m_FilterOptionStrings.Clear();
			delete m_FilterOptionStrings;
		}
		
		if (m_MarketFilters)
		{
			delete m_MarketFilters;
		}
		
		if (m_PlayerItems)
		{
			m_PlayerItems.Clear();
			delete m_PlayerItems;
		}
		
		if (m_QuantityDialog)
		{
			m_QuantityDialog.Hide();
			delete m_QuantityDialog;
		}
		
		if (m_AttachmentsTooltip)
		{
			m_AttachmentsTooltip.Hide();
			delete m_AttachmentsTooltip;
		}

		if (m_PurchaseDialog)
		{
			m_PurchaseDialog.Hide();
			delete m_PurchaseDialog;
		}
		
		if (m_SellDialog)
		{
			m_SellDialog.Hide();
			delete m_SellDialog;
		}
		
		if (m_CurrenciesTooltip)
		{
			m_CurrenciesTooltip.Hide();
			delete m_CurrenciesTooltip;
		}
		
		if (m_BuyDenomsTooltip)
		{
			m_BuyDenomsTooltip.Hide();
			delete m_BuyDenomsTooltip;
		}
		
		if (m_SellDenomsTooltip)
		{
			m_SellDenomsTooltip.Hide();
			delete m_SellDenomsTooltip;
		}
		
		if (m_QuatityTooltip)
		{
			m_QuatityTooltip.Hide();
			delete m_QuatityTooltip;
		}
		
		if (m_MarketMenuItemManager)
		{
			m_MarketMenuItemManager.Hide();
			delete m_MarketMenuItemManager;
		}
		
		m_FirstCall = false;
		
		super.OnHide();
		
		MarketPrint("ExpansionMarketMenu::OnHide - End");
	}
	
	void OnAttachmentsCheckboxStateChange()
	{
		if (!GetSelectedMarketItemElement())
			return;

		bool wasIncludingAttachments = GetSelectedMarketItemElement().GetIncludeAttachments();
		m_IncludeAttachmentsData = GetMarketMenuController().IncludeAttachments;
		GetSelectedMarketItemElement().SetIncludeAttachments(m_IncludeAttachmentsData);
		UpdateItemFieldFromBasicNetSync();
		SetItemAttachmentsInfo(GetSelectedMarketItemElement());
		if (m_IncludeAttachmentsData != wasIncludingAttachments)
		{
			EXPrint("OnAttachmentsCheckboxStateChange() -> GetSelectedMarketItemElement().UpdateView()");
			GetSelectedMarketItemElement().UpdateView();
			GetSelectedMarketItemElement().UpdatePreviewObject();
		}
		
		UpdatePreview();
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnChange
	// ------------------------------------------------------------
	override bool OnChange(Widget w, int x, int y, bool finished)
	{				
		if (w == market_filter_box)
		{
			UpdateMarketCategories();
		}
		
		return false;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			case market_filter_clear:
				market_filter_clear_icon.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSearchFilterButton));
				break;
			case dropdown_selector:
				dropdown_selector.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsButton));
				dropdown_collapse_image.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsIcon));
				dropdown_expand_image.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsIcon));
				break;
			case market_item_info_attachments_button:
				if (m_AttachmentsTooltip) m_AttachmentsTooltip.Show();
				break;
			case player_info_content:
				if (m_CurrenciesTooltip) m_CurrenciesTooltip.Show();
				break;
			case market_item_buy_price_panel:
				if (m_BuyDenomsTooltip) m_BuyDenomsTooltip.Show();
				break;
			case market_item_sell_price_panel:
				if (m_SellDenomsTooltip) m_SellDenomsTooltip.Show();
				break;
			case market_footer_categories_toggle:
				market_footer_categories_label.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorToggleCategoriesText));
				break;
			case market_item_info_attachments_checkbox:
				market_item_info_attachments_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSetQuantityButton));
				break;
			case skin_selector:
				skin_selector.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsButton));
				skin_selector_expand_image.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsIcon));
				skin_selector_collapse_image.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorFilterOptionsIcon));
				break;
			case market_item_info_quantity_button:
				if (m_QuatityTooltip) m_QuatityTooltip.Show();
				break;
			case market_item_info_attachments_setup_button:
				market_item_info_attachments_setup_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSetQuantityButton));
				break;
		}		
		return false;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case market_filter_clear:
				market_filter_clear_icon.SetColor(ARGB(140, 251, 252, 254));
				break;
			case dropdown_selector:
				dropdown_selector.SetColor(ARGB(0, 0, 0, 0));
				dropdown_collapse_image.SetColor(ARGB(140, 251, 252, 254));
				dropdown_expand_image.SetColor(ARGB(140, 251, 252, 254));
				break;
			case market_item_info_attachments_button:
				if (m_AttachmentsTooltip) m_AttachmentsTooltip.Hide();
				break;
			case player_info_content:
				if (m_CurrenciesTooltip) m_CurrenciesTooltip.Hide();
				break;
			case market_item_buy_price_panel:
				if (m_BuyDenomsTooltip) m_BuyDenomsTooltip.Hide();
				break;
			case market_item_sell_price_panel:
				if (m_SellDenomsTooltip) m_SellDenomsTooltip.Hide();
				break;
			case market_footer_categories_toggle:
				market_footer_categories_label.SetColor(ARGB(255, 255, 255, 255));
				break;
			case market_item_info_attachments_checkbox:
				market_item_info_attachments_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
				break;
			case skin_selector:
				skin_selector.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
				skin_selector_expand_image.SetColor(ARGB(240, 251, 252, 254));
				skin_selector_collapse_image.SetColor(ARGB(240, 251, 252, 254));
				break;
			case market_item_info_quantity_button:
				if (m_QuatityTooltip) m_QuatityTooltip.Hide();
				break;
			case market_item_info_attachments_setup_button:
				market_item_info_attachments_setup_button.SetColor(ARGB(0, 0, 0, 0));
				break;
		}
			
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseButtonDown
	// ------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		if (w == market_item_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}
		else if (w == market_player_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotationPlayerPreview");
			g_Game.GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseButtonUp
	// ------------------------------------------------------------	
	override bool OnMouseButtonUp( Widget w, int x, int y, int button )
	{
		if (w == dropdown_selector && button == MouseState.LEFT)
		{
			SetDropdownState();
			return true;
		}
		else if (w == skin_selector && button == MouseState.LEFT)
		{
			SetSkinSelectorState();
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnMouseWheel
	// ------------------------------------------------------------
	override bool OnMouseWheel(Widget w, int x, int y, int wheel) 
	{
		if (w == market_item_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_CharacterScaleDelta = wheel ;
		}
		else if (w == market_player_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateScalePlayerPreview");
			m_CharacterScaleDelta = wheel ;
		}
		
		return false;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu OnItemSelected
	// ------------------------------------------------------------
	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		float itemx, itemy;
		if (w == market_item_preview)
		{
			m_CharacterOrientation = vector.Zero;
			
			market_item_preview.SetModelPosition(Vector(0,0,0.5));
			market_item_preview.SetModelOrientation(m_CharacterOrientation);
		
			market_item_preview.GetPos(itemx, itemy);

			market_item_preview.SetSize(1.5, 1.5);

			// align to center 
			market_item_preview.SetPos(-0.225, -0.225);
		}
		else if (w == market_player_preview)
		{
			m_CharacterOrientation = vector.Zero;
			
			market_player_preview.SetModelPosition(Vector(0,0,0.5));
			market_player_preview.SetModelOrientation(m_CharacterOrientation);
	
			market_player_preview.GetPos(itemx, itemy);

			market_player_preview.SetSize(1.5, 1.5);

			// align to center 
			market_player_preview.SetPos(-0.225, -0.225);
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateScale
	// ------------------------------------------------------------
	void UpdateScale(int mouse_x, int mouse_y, bool is_dragging, int wheel)
	{
		float w, h, x, y;		
		market_item_preview.GetPos(x, y);
		market_item_preview.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 4);
		h = h + (m_CharacterScaleDelta / 4);
		if ( w > 0.5 && w < 4 )
		{
			market_item_preview.SetSize(w, h);
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 8);
			float new_y = y - (m_CharacterScaleDelta / 8);
			market_item_preview.SetPos(new_x, new_y);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateScalePlayerPreview
	// ------------------------------------------------------------
	void UpdateScalePlayerPreview(int mouse_x, int mouse_y, bool is_dragging, int wheel)
	{
		float w, h, x, y;		
		market_player_preview.GetPos(x, y);
		market_player_preview.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 8);
		h = h + (m_CharacterScaleDelta / 8);
		if ( w > 0.5 && w < 6 )
		{
			market_player_preview.SetSize(w, h);
	
			//! Align to center
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 16);
			float new_y = y - (m_CharacterScaleDelta / 16);
			market_player_preview.SetPos(new_x, new_y);

			UpdateLastPositionPlayerPreview(w);
			if (!m_PlayerPosAnimRunning)
				UpdatePositionPlayerPreview(m_LastPositionPlayerPreviewY);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetPositionPlayerPreviewTargetY
	// ------------------------------------------------------------
	float GetPositionPlayerPreviewTargetY()
	{
		//! Keep focus on item slot when zooming in

		bool isVestSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Vest");
		bool isBodySlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Body");
		bool isBackSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Back");
		bool isHipsSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Hips");
		bool isLegsSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Legs");
		bool isFeetSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Feet");
		bool isHeadgearSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Headgear");
		bool isMaskSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Mask");
		bool isEyewearSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Eyewear");
		bool isArmbandSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Armband");
		bool isGlovesSlot = m_SelectedItemSlot == InventorySlots.GetSlotIdFromString("Gloves");
		bool isTopSlot = isVestSlot || isBodySlot || isBackSlot || isArmbandSlot;
		bool isHeadSlot = isHeadgearSlot || isMaskSlot || isEyewearSlot;

		float v = POSITION_PLAYER_PREVIEW_Y_OFFSET;
		if (isTopSlot)
			v = -0.4375;
		else if (isHeadSlot)
			v = -0.75;
		else if (isLegsSlot)
			v = 0.275;
		else if (isFeetSlot)
			v = 0.625;

		return v;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetPositionPlayerPreviewInterpolatedY
	// ------------------------------------------------------------
	float GetPositionPlayerPreviewInterpolatedY(float w, float targetY)
	{
		//! Normalize to 0..1
		float normalized = ExpansionMath.LinearConversion(POSITION_PLAYER_PREVIEW_W_THRESHOLD_MIN, POSITION_PLAYER_PREVIEW_W_THRESHOLD_MAX, w);

		//! Smooth normalized value
		float smooth = ExpansionMath.SmoothStep(normalized, 2);

		//! Interpolate to targetY
		float y = ExpansionMath.LinearConversion(0, 1, smooth, POSITION_PLAYER_PREVIEW_Y_OFFSET, targetY);

		return y;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdatePositionPlayerPreview
	// ------------------------------------------------------------
	void UpdatePositionPlayerPreview(float y)
	{
		market_player_preview.SetModelPosition(Vector(0, y, 0));
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateLastPositionPlayerPreview
	// ------------------------------------------------------------
	void UpdateLastPositionPlayerPreview(float w)
	{
		m_LastPositionPlayerPreviewW = w;
		if (!m_PlayerPosAnimRunning)
			m_LastPositionPlayerPreviewY = GetPositionPlayerPreviewInterpolatedY(w, GetPositionPlayerPreviewTargetY());
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu AnimatePositionPlayerPreview
	// ------------------------------------------------------------
	int AnimatePositionPlayerPreview(int animID = 0, int frameNumber = 1)
	{
		if (!animID)
		{
			m_PlayerPosAnimRunning = true;
			m_PlayerPosCurrentAnimID++;
			animID = m_PlayerPosCurrentAnimID;
		}
		else if (m_PlayerPosCurrentAnimID != animID)
			return 0;

		float targetInterpolatedY = GetPositionPlayerPreviewInterpolatedY(m_LastPositionPlayerPreviewW, GetPositionPlayerPreviewTargetY());

		int fps = 60;
		float frames = fps * POSITION_PLAYER_PREVIEW_ANIMATION_DURATION;
		float step = (targetInterpolatedY - m_LastPositionPlayerPreviewY) / frames * frameNumber;
		float currentY = m_LastPositionPlayerPreviewY + step;

		float smooth;

		if (targetInterpolatedY != m_LastPositionPlayerPreviewY)  //! Prevent division by zero
			smooth = ExpansionMath.SmoothStep(currentY, 2, m_LastPositionPlayerPreviewY, targetInterpolatedY);
		else
			smooth = currentY;
		MarketPrint("" + smooth);

		UpdatePositionPlayerPreview(smooth);

		if (currentY != targetInterpolatedY && frameNumber < frames)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(AnimatePositionPlayerPreview, Math.Round(1000 / fps), false, animID, frameNumber + 1);
		}
		else
		{
			m_PlayerPosAnimRunning = false;
			m_LastPositionPlayerPreviewY = currentY;
		}

		return animID;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateRotation
	// ------------------------------------------------------------
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[2] = o[2] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);
		
		market_item_preview.SetModelOrientation(o);
		
		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UpdateRotationPlayerPreview
	// ------------------------------------------------------------
	void UpdateRotationPlayerPreview(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[2] = o[2] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);
		
		market_player_preview.SetModelOrientation(o);
		
		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}
				
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterPrimeMag
	// ------------------------------------------------------------
	void SetFilterPrimeMag(bool state)
	{
		m_ShowPrimeMag = state;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowPrimeMag
	// ------------------------------------------------------------
	bool ShowPrimeMag()
	{
		return m_ShowPrimeMag;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterSecondMag
	// ------------------------------------------------------------
	void SetFilterSecondMag(bool state)
	{
		m_ShowSecondMag = state;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowSecondMag
	// ------------------------------------------------------------
	bool ShowSecondMag()
	{
		return m_ShowSecondMag;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterPrimeAttach
	// ------------------------------------------------------------
	void SetFilterPrimeAttach(bool state)
	{
		m_ShowPrimeAttach = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowPrimeAttach
	// ------------------------------------------------------------
	bool ShowPrimeAttach()
	{
		return m_ShowPrimeAttach;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterSecondAttach
	// ------------------------------------------------------------
	void SetFilterSecondAttach(bool state)
	{
		m_ShowSecondAttach = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowSecondAttach
	// ------------------------------------------------------------
	bool ShowSecondAttach()
	{
		return m_ShowSecondAttach;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterHandMag
	// ------------------------------------------------------------
	void SetFilterHandMag(bool state)
	{
		m_ShowHandMag = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowHandMag
	// ------------------------------------------------------------
	bool ShowHandMag()
	{
		return m_ShowHandMag;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterHandAttach
	// ------------------------------------------------------------
	void SetFilterHandAttach(bool state)
	{
		m_ShowHandAttach = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowHandAttach
	// ------------------------------------------------------------
	bool ShowHandAttach()
	{
		return m_ShowHandAttach;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterPrimeBullet
	// ------------------------------------------------------------
	void SetFilterPrimeBullet(bool state)
	{
		m_ShowPrimeBullets = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowPrimeBullet
	// ------------------------------------------------------------
	bool ShowPrimeBullet()
	{
		return m_ShowPrimeBullets;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterSellables
	// ------------------------------------------------------------
	void SetFilterSellables(bool state)
	{
		m_ShowSellable = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowSellables
	// ------------------------------------------------------------
	bool ShowSellables()
	{
		return m_ShowSellable;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterPurchasables
	// ------------------------------------------------------------
	void SetFilterPurchasables(bool state)
	{
		m_ShowPurchasables = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowPurchasables
	// ------------------------------------------------------------
	bool ShowPurchasables()
	{
		return m_ShowPurchasables;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterSecondBullet
	// ------------------------------------------------------------
	void SetFilterSecondBullet(bool state)
	{
		m_ShowSecondBullets = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowSecondBullet
	// ------------------------------------------------------------
	bool ShowSecondBullet()
	{
		return m_ShowSecondBullets;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetFilterHandBullet
	// ------------------------------------------------------------
	void SetFilterHandBullet(bool state)
	{
		m_ShowHandBullets = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu ShowHandBullet
	// ------------------------------------------------------------
	bool ShowHandBullet()
	{
		return m_ShowHandBullets;
	}
	
	void ResetDropdownFilterStates()
	{
		m_ShowPrimeMag = false;
		m_ShowSecondMag = false;
		m_ShowPrimeAttach = false;
		m_ShowSecondAttach = false;
		m_ShowHandAttach = false;
		m_ShowHandMag = false;
		m_ShowPrimeBullets = false;
		m_ShowSecondBullets = false;
		m_ShowHandBullets = false;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu SetIsLoading
	// ------------------------------------------------------------
	void SetIsLoading(bool state, ExpansionMarketMenuState menuState = ExpansionMarketMenuState.INVALID)
	{
		if (menuState == ExpansionMarketMenuState.INVALID)
		{
			if (state)
				menuState = ExpansionMarketMenuState.LOADING;
			else
				menuState = ExpansionMarketMenuState.NONE;
		}

		SetMenuState(menuState);

		market_loading.Show(state);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu IsLoading
	// ------------------------------------------------------------
	bool IsLoading()
	{
		return m_CurrentState != ExpansionMarketMenuState.NONE || !m_Complete;
	}
		
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnNetworkItemUpdate
	// ------------------------------------------------------------
	protected void OnNetworkItemUpdate()
	{
		MarketPrint("OnNetworkItemUpdate - Start");
		
		m_MarketModule.EnumeratePlayerInventory(PlayerBase.Cast(GetGame().GetPlayer()));
		
		if (GetSelectedMarketItemElement() && GetSelectedMarketItem())
		{
			EXPrint("OnNetworkItemUpdate() -> GetSelectedMarketItemElement().UpdateView()");
			GetSelectedMarketItemElement().UpdateView();
			UpdateItemFieldFromBasicNetSync();
		}
		
		MarketPrint("OnNetworkItemUpdate - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetTraderObject
	// ------------------------------------------------------------
	ExpansionTraderObjectBase GetTraderObject()
	{
		return m_TraderObject;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu PlayMarketSound
	// ------------------------------------------------------------
	void PlayMarketSound()
	{
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
		{	
			if (GetGame().GetPlayer())
			{
				string soundConfig;
				string path = "cfgVehicles " + GetSelectedMarketItem().ClassName + " " + "AnimEvents ";
				int events_sources_count = GetGame().ConfigGetChildrenCount(path);
				string soundName = "";
				EffectSound sound;
				
				MarketPrint("PlayMarketSound - Search sound: " + path);
				MarketPrint("PlayMarketSound - Effents count: " + events_sources_count);
				
				for (int i = 0; i < events_sources_count; i++)
				{
					string soundWeapons;
					GetGame().ConfigGetChildName(path, i, soundWeapons);
					MarketPrint("PlayMarketSound - Path 2: " + path + soundWeapons + " pickUpItem" + " soundSet");
					
					if (GetGame().ConfigIsExisting(path + soundWeapons + " pickUpItem" + " soundSet"))
					{
						GetGame().ConfigGetText(path + soundWeapons + " pickUpItem" + " soundSet", soundName);
						MarketPrint("PlayMarketSound - Sound: " + soundName);
					}
				}
				
				if (soundName != "")
				{
					sound = SEffectManager.PlaySound(soundName, GetGame().GetPlayer().GetPosition());
				}
				else
				{
					sound = SEffectManager.PlaySound("pickUpBackPack_Metal_SoundSet", GetGame().GetPlayer().GetPosition());
				}
				
				sound.SetSoundAutodestroy(true);
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu UsePlayerPreview
	// ------------------------------------------------------------
	private bool UsePlayerPreview()
	{
		if (GetSelectedMarketItemElement().GetPreviewObject())
			return GetSelectedMarketItemElement().GetPreviewObject().IsInherited(Clothing_Base);
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu MarketPrint
	// ------------------------------------------------------------
	void MarketPrint(string text)
	{
	#ifdef EXPANSIONMODMARKET_DEBUG
		Print("ExpansionMarketMenu::" + text);
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnToggleCategoriesButtonClick
	// ------------------------------------------------------------
	void OnToggleCategoriesButtonClick()
	{
		int i;
		ExpansionMarketMenuCategory category;

		//! This takes a player expanding/collapsing single categories into account
		bool toggleState = CategoriesExpanded == 0;
		
		for (i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
		{
			category = m_MarketMenuController.MarketCategories[i];
			category.ToggleCategory(toggleState);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetSelectedMarketItemElement
	// ------------------------------------------------------------
	ExpansionMarketMenuItem GetSelectedMarketItemElement()
	{
		return m_SelectedMarketItemElement;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetSelectedMarketItem
	// ------------------------------------------------------------
	ExpansionMarketItem GetSelectedMarketItem()
	{
		return m_SelectedMarketItem;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu GetUpdateTickRate
	// ------------------------------------------------------------
	override float GetUpdateTickRate()
	{
		return 0.5;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu Update
	// ------------------------------------------------------------
	override void Update()
	{
		//! Buy/Sell Shortcuts
		if (GetFocus() != market_filter_box)
		{
			if (ExpansionStatic.Key_B() && !m_KeyInput && m_SelectedMarketItem != NULL && m_TraderItemStock > 0)
			{
				OnBuyButtonClick();
				m_KeyInput = true;
				return;
			}
			else if (ExpansionStatic.Key_S() && !m_KeyInput && m_SelectedMarketItem != NULL && m_PlayerStock > 0)
			{
				OnSellButtonClick();
				m_KeyInput = true;
				return;
			}
		}
		
		//! Quantity Shortcuts
		if (GetSelectedMarketItem() != NULL)
		{
			if (ExpansionStatic.Key_UP() || ExpansionStatic.Key_RIGHT())
			{
				OnIncrementQuantityButtonClick();
			}
			else if (ExpansionStatic.Key_DOWN() || ExpansionStatic.Key_LEFT())
			{
				OnDecrementQuantityButtonClick();
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetMenuState
	// ------------------------------------------------------------
	ExpansionMarketMenuState GetMenuState()
	{
		return m_CurrentState;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetMenuState
	// ------------------------------------------------------------
	void SetMenuState(ExpansionMarketMenuState state)
	{
		EXPrint(ToString() + "::SetMenuState " + typename.EnumToString(ExpansionMarketMenuState, m_CurrentState) + " -> " + typename.EnumToString(ExpansionMarketMenuState, state));
		m_CurrentState = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetCurrentTraderItemStock
	// ------------------------------------------------------------
	int GetCurrentTraderItemStock()
	{
		return m_TraderItemStock;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetCurrentPlayerItemStock
	// ------------------------------------------------------------
	int GetCurrentPlayerItemStock()
	{
		return m_PlayerStock;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetCurrentItemBuyPrice
	// ------------------------------------------------------------
	int GetCurrentItemBuyPrice()
	{
		return m_BuyPrice;
	}	
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetCurrentItemSellPrice
	// ------------------------------------------------------------
	int GetCurrentItemSellPrice()
	{
		return m_SellPrice;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetMarketTrader
	// ------------------------------------------------------------
	ExpansionMarketTrader GetMarketTrader()
	{
		return m_TraderMarket;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu OnSelectAttachmentsButtonClick
	// ------------------------------------------------------------
	void OnSelectAttachmentsButtonClick()
	{
		market_menu_content.Show(false);
		market_menu_attachments_content.Show(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu AllowCategoryButtonInputs
	// ------------------------------------------------------------
	void AllowCategoryButtonInputs(bool state)
	{
		for (int i = 0; i < m_MarketMenuController.MarketCategories.Count(); i++)
		{
			ExpansionMarketMenuCategory category = m_MarketMenuController.MarketCategories[i];
			category.AllowButtonInputs(state);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetDropdownState
	// ------------------------------------------------------------	
	void SetDropdownState()
	{
		m_IsDropdownExpanded = !m_IsDropdownExpanded;
		dropdown_container.Show( m_IsDropdownExpanded );
		dropdown_content.Show( m_IsDropdownExpanded );
		dropdown_expand_image.Show( m_IsDropdownExpanded );
		dropdown_collapse_image.Show( !m_IsDropdownExpanded );
		dropdown_selector.Update();
				
		if (!m_IsDropdownExpanded)
		{
			dropdown_container.SetFlags(WidgetFlags.NOFOCUS | WidgetFlags.IGNOREPOINTER);
		}
		else
		{
			skin_selector_container.ClearFlags(WidgetFlags.NOFOCUS | WidgetFlags.IGNOREPOINTER);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu SetSkinSelectorState
	// ------------------------------------------------------------	
	void SetSkinSelectorState()
	{
		m_IsSkinSelectorExpanded = !m_IsSkinSelectorExpanded;
		//skin_selector.Show( m_IsSkinSelectorExpanded );
		skin_selector_content.Show( m_IsSkinSelectorExpanded );
		skin_selector_expand_image.Show( m_IsSkinSelectorExpanded );
		skin_selector_collapse_image.Show( !m_IsSkinSelectorExpanded );
		skin_selector.Update();
				
		if (!m_IsSkinSelectorExpanded)
		{
			skin_selector_container.SetFlags(WidgetFlags.NOFOCUS | WidgetFlags.IGNOREPOINTER);
			skin_selector_container.Show(false);
			skin_selector_container.VScrollToPos(0);
		}
		else
		{
			skin_selector_container.ClearFlags(WidgetFlags.NOFOCUS | WidgetFlags.IGNOREPOINTER);
			skin_selector_container.Show(true);			
			skin_selector_container.VScrollToPos(0);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetMarketFilters
	// ------------------------------------------------------------
	ExpansionMarketFilters GetMarketFilters()
	{
		return m_MarketFilters;
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu HideItemManager
	// ------------------------------------------------------------		
	void HideItemManager()
	{
		market_menu_content.Show(true);
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetMarketMenuController
	// ------------------------------------------------------------			
	ExpansionMarketMenuController GetMarketMenuController()
	{
		return m_MarketMenuController;
	}
		
	ExpansionMarketMenuItemManager GetMarketMenuItemManager()
	{
		return m_MarketMenuItemManager;
	}

	// ------------------------------------------------------------
	// ExpansionMarketMenu OnOpenItemManagerButtonClick
	// ------------------------------------------------------------	
	void OnOpenItemManagerButtonClick()
	{
		if (!m_MarketMenuItemManager.IsVisible())
		{
			market_menu_content.Show(false);
			m_MarketMenuItemManager.SetMarketItem(GetSelectedMarketItem());
			m_MarketMenuItemManager.Show();
		}
		else
		{
			market_menu_content.Show(true);
			m_MarketMenuItemManager.Hide();
			
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMarketMenu GetCurrentSelectedAttachmentIDs
	// ------------------------------------------------------------	
	array<int> GetCurrentSelectedAttachmentIDs(bool recursive = false, ExpansionMarketItem item = NULL)
	{
		array<int> attachmentIDs = new array<int>;
		if (!item)
			item = GetSelectedMarketItem();
		foreach (string attachment: item.SpawnAttachments)
		{
			ExpansionMarketItem attachmentItem = GetExpansionSettings().GetMarket().GetItem(attachment);
			attachmentIDs.Insert(attachmentItem.ItemID);
			if (recursive)
				attachmentIDs.InsertAll(GetCurrentSelectedAttachmentIDs(recursive, attachmentItem));
		}
		return attachmentIDs;
	}
};

class ExpansionMarketMenuController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionMarketMenuCategory> MarketCategories = new ObservableCollection<ref ExpansionMarketMenuCategory>(this);
	ref ObservableCollection<ref ExpansionMarketMenuDropdownElement> DropdownElements = new ObservableCollection<ref ExpansionMarketMenuDropdownElement>(this);
	ref ObservableCollection<ref ExpansionMarketMenuSkinsDropdownElement> SkinsDropdownElements = new ObservableCollection<ref ExpansionMarketMenuSkinsDropdownElement>(this);
	
	string MarketName;
	string PlayerTotalMoney;
	string MarketItemName;
	string MarketItemDesc;
	Object MarketItemPreview;	
	string MarketItemTotalSellPrice;
	string MarketItemTotalBuyPrice;	
	string MarketItemStockPlayer;
	string MarketItemStockTrader;
	string MarketQuantity;
	string MarketIcon;
	string MarketItemInfoName;
	Object MarketPlayerPreview;
	string CurrencySellIcon;
	string CurrencyBuyIcon;
	bool ShowSellables;
	bool ShowPurchasables;
	bool IncludeAttachments;
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuController PropertyChanged
	// ------------------------------------------------------------
	override void PropertyChanged(string property_name)
	{
		ExpansionMarketMenu menu;
		menu = ExpansionMarketMenu.Cast(GetParent());
		if (property_name == "ShowSellables")
		{
			EXPrint("ExpansionMarketMenuController::PropertyChanged - ShowSellables " + ShowSellables);
			GetExpansionClientSettings().MarketMenuFilterSellableState = ShowSellables;
			if (menu)
			{
				menu.SetFilterSellables(ShowSellables);
				menu.UpdateOptionFilterStrings();
				menu.UpdateMarketCategories();
			}
		}
		else if (property_name == "ShowPurchasables")
		{
			EXPrint("ExpansionMarketMenuController::PropertyChanged - ShowPurchasables " + ShowPurchasables);
			GetExpansionClientSettings().MarketMenuFilterPurchasableState = ShowPurchasables;
			if (menu)
			{
				menu.SetFilterPurchasables(ShowPurchasables);
				menu.UpdateOptionFilterStrings();
				menu.UpdateMarketCategories();
			}
		}
		else if (property_name == "IncludeAttachments")
		{
			EXPrint("ExpansionMarketMenuController::PropertyChanged - IncludeAttachments " + IncludeAttachments);
			if (menu)
				menu.OnAttachmentsCheckboxStateChange();
		}
	}
};