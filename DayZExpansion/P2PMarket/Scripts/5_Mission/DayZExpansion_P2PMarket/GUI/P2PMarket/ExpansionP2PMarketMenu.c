/**
 * ExpansionP2PMarketMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionP2PMarketMenuViewState
{
	NONE = 0,
	ViewBrowse = 1,
	ViewList = 2,
	ViewSales = 3,
	DetailViewPlayerItem = 4,
	DetailViewListingItem = 5,
	InspectListingItem = 6
};

class ExpansionP2PMarketMenu: ExpansionScriptViewMenu
{
	protected const int LISTINGS_PER_PAGE_COUNT = 14;
	protected const int PAGES_BUTTONS_COUNT = 10;
	
	protected ref ExpansionP2PMarketMenuController m_P2PMarketMenuController;
	protected ExpansionP2PMarketModule m_P2PMarketModule;
	protected ExpansionMarketModule m_MarketModule;

	protected ref array<ref ExpansionP2PMarketPlayerItem> m_PlayerItems = {};
	protected ref array<ref ExpansionP2PMarketMenuListing> m_ItemListings = {};
	protected ref array<ref ExpansionP2PMarketMenuListing> m_SoldListings = {};
	protected int m_SoldListingsCount;
	protected int m_ListingsCount;

	protected ExpansionP2PMarketMenuItem m_SelectedPlayerItem;
	protected ExpansionP2PMarketMenuListing m_SelectedListing;
	Object m_SelectedPreviewObject;
	protected ref array<ref ExpansionP2PMarketContainerItem> m_SelectedContainerItems = {};

	protected int m_TraderID = -1;
	protected bool m_GlobalTrader;
	protected ref TStringArray m_Currencies;
	protected int m_DisplayCurrencyValue = 1;
	protected string m_DisplayCurrencyName;
	protected int m_DisplayCurrencyPrecision = 2;
	protected bool m_RequestsLocked;
	protected bool m_DefaultEditboxText;
	protected bool m_EditBoxLocked;
	protected int m_PlayerWorth;
	protected int m_ListCost;

	protected int m_CurrentItemPrice;

	protected ref TStringArray m_BloodContainers = {"BloodContainerBase"};

	protected bool m_CategoriesToggleState;

	protected Widget categories_panel;
	protected Widget inventory_panel;
	protected Widget details_panel;
	protected ScrollWidget inventory_scroller;
	protected WrapSpacerWidget inventory_header;
	protected WrapSpacerWidget inventory_content;
	protected ScrollWidget player_items_scroller;
	protected WrapSpacerWidget player_items_content;
	protected ScrollWidget sold_items_scroller;
	protected WrapSpacerWidget sold_items_content;

	protected ButtonWidget listing_category_listings_button;
	protected TextWidget listing_category_listings_text;
	protected ImageWidget listing_category_listings_icon;
	protected TextWidget listing_category_listings_count;

	protected ButtonWidget listing_category_all_button;
	protected TextWidget listing_category_all_text;
	protected ImageWidget listing_category_all_icon;
	protected TextWidget listing_category_all_count;

	protected ButtonWidget toggle_categories_button;
	protected TextWidget toggle_categories_text;

	protected Widget market_filter_panel;
	protected EditBoxWidget market_filter_box;

	protected GridSpacerWidget tabs_panel;
	protected ButtonWidget tabs_button_browse;
	protected TextWidget tabs_button_browse_text;
	protected WrapSpacerWidget tabs_button_browse_bg;
	
	protected ButtonWidget tabs_button_list;
	protected TextWidget tabs_button_list_text;
	protected WrapSpacerWidget tabs_button_list_bg;
	
	protected ButtonWidget tabs_button_sales;
	protected TextWidget tabs_button_sales_text;
	protected WrapSpacerWidget tabs_button_sales_bg;

	protected GridSpacerWidget exit_button_panel;
	protected ButtonWidget exit_button;
	protected TextWidget exit_button_text;

	protected ButtonWidget retrieve_button;
	protected TextWidget retrieve_button_text;

	protected WrapSpacerWidget listing_categories_content;
	protected WrapSpacerWidget player_categories_content;
	protected Widget loading;

	protected GridSpacerWidget toggle_categories_panel;

	protected GridSpacerWidget retrieve_button_panel;

	protected GridSpacerWidget player_sales_content;
	
	protected WrapSpacerWidget pages_spacer;
	protected ButtonWidget navigation_page_first;
	protected ButtonWidget navigation_page_prev;
	protected ButtonWidget navigation_page_next;
	protected ButtonWidget navigation_page_last;
	
	protected ButtonWidget market_filter_clear;
	protected ImageWidget market_filter_clear_icon;

	protected ExpansionP2PMarketSettings m_P2PMarketSettings;

	protected ref ExpansionP2PMarketMenuDetailsView m_ItemDetailsView;
	protected ExpansionP2PMarketMenuViewState m_ViewState = ExpansionP2PMarketMenuViewState.ViewBrowse;
	protected ExpansionP2PMarketMenuViewState m_PreviousViewState;

	protected ref ExpansionP2PMarketMenuListHeader m_ListHeader;

	protected ref ExpansionDialog_P2PMarketMenu m_MenuDialog;

	protected string m_PlayerUID;

	protected int m_CategoryIndex = -1;
	protected int m_SubCategoryIndex = -1;

	protected int m_PageIndex = 0;
	protected int m_PageStartNum;
	protected int m_PageEndNum;
	protected int m_PagesCount;
	protected ref array<ButtonWidget> m_BtnPages;

	protected int m_SoldTotalIncome;
	protected bool m_InitPhase = true;
	protected bool m_OwnedListings;
	protected bool m_AllListings;
	protected int m_OwnedListingsCount;
	protected int m_ValidListingsCount;
	protected bool m_UseValidListingsCount;
	protected ExpansionP2PMarketMenuItemBase m_InspectingElement;

	void ExpansionP2PMarketMenu()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		Class.CastTo(m_P2PMarketMenuController, GetController());
		Class.CastTo(m_P2PMarketModule, CF_ModuleCoreManager.Get(ExpansionP2PMarketModule));
		Class.CastTo(m_MarketModule, CF_ModuleCoreManager.Get(ExpansionMarketModule));

		m_P2PMarketModule.GetListingsSI().Insert(SetTraderItems);
		m_P2PMarketModule.GetListingDetailsSI().Insert(OnListingDetailsRecived);
		m_P2PMarketModule.GetCallbackSI().Insert(OnModuleCallback);
		m_P2PMarketModule.GetUpdateSI().Insert(OnListingsUpdate);
		
		m_P2PMarketSettings = GetExpansionSettings().GetP2PMarket();

		m_ItemDetailsView = new ExpansionP2PMarketMenuDetailsView(this);
		m_ItemDetailsView.Hide();

		m_ListHeader = new ExpansionP2PMarketMenuListHeader(this);
		inventory_header.AddChild(m_ListHeader.GetLayoutRoot());
		m_ListHeader.SetSort(0, false);

		m_PlayerUID = g_Game.GetPlayer().GetIdentity().GetId();
		
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (player)
			player.Expansion_GetOnRestrainedStateChaged().Insert(OnRestrainedStateChaged);
		
		m_BtnPages = new array<ButtonWidget>;
		for (int i = 1; i <= PAGES_BUTTONS_COUNT; ++i)
		{
			ButtonWidget page_button = ButtonWidget.Cast(GetLayoutRoot().FindAnyWidget("navigation_page" + i));
			if (page_button)
			{
				page_button.Show(false);
				m_BtnPages.Insert(page_button);
			}
		}
	}

	void ~ExpansionP2PMarketMenu()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (m_P2PMarketModule)
		{
			m_P2PMarketModule.GetListingsSI().Remove(SetTraderItems);
			m_P2PMarketModule.GetListingDetailsSI().Remove(OnListingDetailsRecived);
			m_P2PMarketModule.GetCallbackSI().Remove(OnModuleCallback);
			m_P2PMarketModule.GetUpdateSI().Remove(OnListingsUpdate);
		}

		if (m_ItemDetailsView)
			m_ItemDetailsView.Destroy();

		if (m_ListHeader)
			m_ListHeader.Destroy();

		if (m_MenuDialog)
			m_MenuDialog.Destroy();

		if (m_PlayerItems)
			m_PlayerItems.Clear();

		if (m_ItemListings)
			m_ItemListings.Clear();

		if (m_SoldListings)
			m_SoldListings.Clear();
		
		for (int i = 0; i < m_P2PMarketMenuController.PlayerItems.Count(); ++i)
		{
			ExpansionP2PMarketMenuItem playerItem = m_P2PMarketMenuController.PlayerItems[i];
			playerItem.Destroy();
		}
		
		m_P2PMarketMenuController.PlayerItems.Clear();
		m_P2PMarketMenuController.ListingCategories.Clear();
		m_P2PMarketMenuController.PlayerCategories.Clear();
		
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (player)
			player.Expansion_GetOnRestrainedStateChaged().Remove(OnRestrainedStateChaged);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuController;
	}

	void SetTraderItems(ExpansionP2PMarketRecivedData data, bool soldListings)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		data.Debug();
		
		if (m_InitPhase)
		{
			if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
			{
				tabs_button_browse_bg.SetColor(ARGB(140, 255, 255, 255));
				tabs_button_browse_text.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_button.SetColor(ARGB(140, 255, 255, 255));
				listing_category_all_text.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_icon.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_count.SetColor(ARGB(255, 0, 0, 0));
				m_AllListings = true;
			}
			
			if (data.m_TraderID > -1)
			{
				m_TraderID = data.m_TraderID;
				m_GlobalTrader = data.m_IsGlobal;
			}
	
			if (data.m_TraderName != "")
			{
				#ifdef DIAG_DEVELOPER
				string nameAddition;
				if (m_GlobalTrader)
				{
					nameAddition = " [GLOBAL]";
				}
				nameAddition += " - [" + data.m_TraderID + "]";
				m_P2PMarketMenuController.TraderName = data.m_TraderName + nameAddition;
				#else
				m_P2PMarketMenuController.TraderName = data.m_TraderName;
				#endif
				m_P2PMarketMenuController.NotifyPropertyChanged("TraderName");
			}
	
			if (data.m_IconName != "")
			{
				m_P2PMarketMenuController.MarketIcon = ExpansionIcons.GetPath(data.m_IconName);
				if (m_P2PMarketMenuController.MarketIcon == "")
					m_P2PMarketMenuController.MarketIcon = data.m_IconName;
	
				m_P2PMarketMenuController.NotifyPropertyChanged("MarketIcon");
			}
			
			if (data.m_Currencies && data.m_Currencies.Count())
			{
				m_Currencies = data.m_Currencies;
			}
	
			if (data.m_DisplayCurrencyValue > -1)
			{
				m_DisplayCurrencyValue = data.m_DisplayCurrencyValue;
			}
			
			if (data.m_DisplayCurrencyName != "")
			{
				m_DisplayCurrencyName = data.m_DisplayCurrencyName;
				m_DisplayCurrencyPrecision = ExpansionStatic.GetPrecision(m_DisplayCurrencyValue);
			}
		}
		
		if (m_TraderID == -1)
		{
			EXError.Error(this, "::SetTraderItems - Trader ID is invalid: " + m_TraderID);
		}

		m_SoldListingsCount = data.m_SoldListingsCount;
		if (!m_UseValidListingsCount)
			m_ListingsCount = data.m_ListingsCount;
		else
			m_ListingsCount = data.m_ValidListingsCount;

		m_ItemListings.Clear();		
		m_SoldListings.Clear();
		
		array<ref ExpansionP2PMarketListing> listings = data.m_Listings;
		if (listings && listings.Count() > 0)
		{
			if (soldListings)
			{
				m_SoldTotalIncome = data.m_SoldTotalIncome;
				m_P2PMarketMenuController.PlayerSales = GetDisplayPrice(m_SoldTotalIncome, false, true, true);
				m_P2PMarketMenuController.PlayerSoldListings = m_SoldListingsCount.ToString();
				m_P2PMarketMenuController.NotifyPropertiesChanged({"PlayerSales", "PlayerSoldListings"});
			}
			
			foreach (ExpansionP2PMarketListing listing: listings)
			{
				ExpansionP2PMarketMenuListing newListing = new ExpansionP2PMarketMenuListing(listing, this);
				if (!soldListings)
				{
					m_ItemListings.Insert(newListing);
				}
				else
				{
					m_SoldListings.Insert(newListing);
				}
			}

			SortListingsByName();
		}

		if (m_SoldListingsCount > 0)
		{
			tabs_button_sales.Show(true);
		}
		else
		{
			tabs_button_sales.Show(false);
		}
		
		//! Get and set all listing categories
		if (m_InitPhase)
		{
			foreach (int i, ExpansionP2PMarketMenuCategory category: m_P2PMarketSettings.MenuCategories)
			{
				if (i == m_P2PMarketSettings.MenuCategories.Count() - 1)
				{
					StringLocaliser sl = new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTCATEGORISED");
					category.SetDisplayName(sl.Format());
				}
				
				ExpansionP2PMarketMenuCategoryElement categoryElement = new ExpansionP2PMarketMenuCategoryElement(this, category);
				m_P2PMarketMenuController.ListingCategories.Insert(categoryElement);
			}
		}
		else
		{
			for (int j = m_P2PMarketMenuController.ListingCategories.Count() - 1; j >= 0; --j)
			{
				m_P2PMarketMenuController.ListingCategories[j].UpdateData();
			}
		}

		//! Get and set all listings count
		string listingsCountText = "[0]";
		if (data.m_ListingsCount > 0)
			listingsCountText = "[" + data.m_ListingsCount + "]";

		m_P2PMarketMenuController.AllListingsCount = listingsCountText;
		m_P2PMarketMenuController.NotifyPropertyChanged("AllListingsCount");

		//! Get and set player listings count
		string playerListingsCountText = "[0]";
		if (data.m_OwnedListingsCount > 0)
		{
			m_OwnedListingsCount = data.m_OwnedListingsCount;
			playerListingsCountText = "[" + data.m_OwnedListingsCount + "]";
		}

		m_P2PMarketMenuController.PlayerListingsCount = playerListingsCountText;
		m_P2PMarketMenuController.NotifyPropertyChanged("PlayerListingsCount");
		
		UpdatePageButtons();
		UpdatePlayerCurrency();
		loading.Show(false);
		m_InitPhase = false;
	}

	protected void UpdatePlayerItems()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		m_PlayerItems.Clear();

		m_P2PMarketModule.EnumeratePlayerInventory(PlayerBase.Cast(g_Game.GetPlayer()));
		array<EntityAI> items = m_P2PMarketModule.LocalGetEntityInventory();
		
		if (items && items.Count() > 0)
		{
			foreach (EntityAI item: items)
			{
				//! Only add items that have the player as direct parent.
				if ((item.GetHierarchyParent() && item.GetHierarchyParent().IsInherited(SurvivorBase)) || ExpansionVehicle.Get(item))
				{
					//! Hardcoded excluded type names where the item should never get added and shown in the menu.
					if (ExpansionP2PMarketModule.m_HardcodedExcludes.Find(item.GetType()) > -1)
						continue;

					ExpansionP2PMarketPlayerItem newPlayerItem = new ExpansionP2PMarketPlayerItem();
					newPlayerItem.SetFromItem(item, m_PlayerUID);
					m_PlayerItems.Insert(newPlayerItem);
				}
			}

			m_P2PMarketMenuController.PlayerItems.Clear();

			PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
			array<EntityAI> slotItems = MiscGameplayFunctions.Expansion_GetAttachments(player);
			array<string> slotNames = new array<string>;
			slotNames.Insert("All");

			foreach (ExpansionP2PMarketPlayerItem playerItem: m_PlayerItems)
			{
				if (!playerItem.GetItem())
					continue;

				for (int s = 0; s < slotItems.Count(); ++s)
				{
					EntityAI slotItem = slotItems[s];
					if (!slotItem)
						continue;

					int playerItemSlotID;
					string playerItemSlotName;
					if (playerItem.GetItem() == slotItem)
					{
						slotItem.GetInventory().GetCurrentAttachmentSlotInfo(playerItemSlotID, playerItemSlotName);
						slotNames.Insert(playerItemSlotName);
						playerItem.SetSlotName(playerItemSlotName);
						continue;
					}
					else if (playerItem.GetItem().GetHierarchyParent() == slotItem)
					{
						playerItem.SetSlotName(playerItemSlotName);
						continue;
					}
				}

				ExpansionP2PMarketMenuItem playerItemEntry = new ExpansionP2PMarketMenuItem(playerItem, this);
				m_P2PMarketMenuController.PlayerItems.Insert(playerItemEntry);
			}

			m_P2PMarketMenuController.PlayerCategories.Clear();
			foreach (string slotName: slotNames)
			{
				ExpansionP2PMarketMenuInventoryCategoryElement inventoryCategoryElement = new ExpansionP2PMarketMenuInventoryCategoryElement(this, slotName);
				m_P2PMarketMenuController.PlayerCategories.Insert(inventoryCategoryElement);
			}
		}
		
		loading.Show(false);
	}
	
	void AddPlayerItem()
	{
		
	}

	protected void UpdatePlayerCurrency()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		array<int> monies = new array<int>;
		m_PlayerWorth = m_MarketModule.GetPlayerWorth(PlayerBase.Cast(g_Game.GetPlayer()), monies, m_Currencies);

		m_P2PMarketMenuController.PlayerMoney = GetDisplayPrice(m_PlayerWorth, false, true, true);
		m_P2PMarketMenuController.NotifyPropertyChanged("PlayerMoney");
	}

	void OnModuleCallback(int callback, string type = string.Empty, int price = 0, int listingPrice = 0, Object blockingObject = null)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif

		switch (m_ViewState)
		{
			case ExpansionP2PMarketMenuViewState.ViewList:
				g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdatePlayerItems, 250);
				if (market_filter_box.GetText() != string.Empty)
					SearchInPlayerItems();
				break;

			case ExpansionP2PMarketMenuViewState.ViewBrowse:
				if (market_filter_box.GetText() != string.Empty)
					OnConfirmSearch();
				break;
		}
		
		string displayName = ExpansionStatic.GetItemDisplayNameWithType(type);
		string priceString = GetDisplayPrice(price, false, true, true);
		string listingPriceString = GetDisplayPrice(listingPrice, false, true, true);
		
		int i;
		switch (callback)
		{
			case ExpansionP2PMarketModuleCallback.ItemListed:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PLACED_SALE_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PLACED_SALE_DESC", displayName, priceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 3, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ItemPurchased:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_RETRIEVED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASED_DESC", displayName, priceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 3, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.SaleRetrieved:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_RETRIEVED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_RETRIEVED_ITEM_DESC", displayName, priceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 3, ExpansionNotificationType.TOAST).Create();
				if (m_SoldListings.Count() == 0)
				{
					tabs_button_sales.Show(false);
					OnBrowseButtonClick();
				}
				else
				{
					g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(UpdatePlayerCurrency, 100);
				}
				break;
			}
			case ExpansionP2PMarketModuleCallback.AllSalesRetrieved:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_RETRIEVED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_RETRIEVED_ALL_ITEMS_DESC", priceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 3, ExpansionNotificationType.TOAST).Create();
				tabs_button_sales.Show(false);
				OnBrowseButtonClick();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorListingPriceTooLow:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_PRICE_LOW_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_PRICE_LOW_DESC", displayName, priceString, listingPriceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorNotEnoughMoneyToList:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_NOT_ENOUGH_MONEY_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_NOT_ENOUGH_MONEY_DESC", displayName, priceString, listingPriceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorNotEnoughMoney:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_NOT_ENOUGH_MONEY_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_NOT_ENOUGH_MONEY", displayName, priceString), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorVehicleMissingAttachment:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_VEH_MISSING_ATT_ERROR_DESC", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorVehicleRuinedAttachment:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_RUINED_ATT_ERROR_DESC", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorNoVehicleSpawnPosition:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASE_FAILED_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_NO_VEH_SPAWN_DESC", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorVehicleSpawnPositionBlocked:
			{
				PlayerBase blockingPlayer;
				string blockingObjectDisplayName;
				if (Class.CastTo(blockingPlayer, blockingObject) && blockingPlayer.GetIdentity())
				{
					blockingObjectDisplayName = blockingPlayer.GetIdentityName();  //! So you can call 'em out in chat - unless it's yourself...
				}
				else
				{
					blockingObjectDisplayName = blockingObject.GetDisplayName();
				}
				string positionString = ExpansionStatic.VectorToString(blockingObject.GetPosition(), ExpansionVectorToString.Labels);
				StringLocaliser localiser = new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_BLOCKED_VEH_SPAWN_DESC", displayName, blockingObjectDisplayName, positionString);
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PURCHASE_FAILED_TITLE"), localiser, ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorVehicleLockpicked:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_VEHICLE_LOCKPICKED_DESC", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorVehicleKeysMissing:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_VEHICLE_KEYS_MISSING", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.ErrorPersistency:
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_LISTING_ERROR_TITLE"), new StringLocaliser("STR_EXPANSION_MARKET_P2P_NOTIF_PERSISTENCY_MISSING", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create();
				break;
			}
			case ExpansionP2PMarketModuleCallback.MsgItemGotSold:
			{
				tabs_button_sales.Show(true);
				break;
			}
		}

		m_RequestsLocked = false;
	}
	
	void OnListingsUpdate(string globalIDText = "")
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		//! Cleanup removed listing if its currently in players listing items
		if (globalIDText != "")
		{
			for (int i = m_ItemListings.Count() - 1; i >= 0; --i)
			{
				ExpansionP2PMarketMenuListing listing = m_ItemListings[i];
				if (listing.GetListing().GetEntityStorageBaseName() == globalIDText)
				{
					//! Close item inspection if client is currently inspecting a listing item or one of its cargo/attachment items in the details view.
					if (m_ViewState == ExpansionP2PMarketMenuViewState.InspectListingItem)
					{
						if (m_InspectingElement)
						{
							ExpansionP2PMarketMenuListing listingElement;
							if (Class.CastTo(listingElement, m_InspectingElement))
							{
								if (listingElement.GetListing().GetEntityStorageBaseName() == globalIDText)
								{
									m_InspectingElement.CloseItemInspection(true);
								}
							}
							
							ExpansionP2PMarketMenuCargoItem cargoElement;
							if (Class.CastTo(cargoElement, m_InspectingElement))	
							{
								if (cargoElement.GetParent() && ExpansionP2PMarketMenuListing.Cast(cargoElement.GetParent()).GetListing().GetEntityStorageBaseName() == globalIDText)
								{
									m_InspectingElement.CloseItemInspection(true);
									
									ClearSelected();
									GetDetailsView().Hide();
						
									categories_panel.Show(true);
									inventory_panel.Show(true);
									player_items_scroller.Show(true);
									details_panel.Show(false);
							
									tabs_panel.Show(true);
									market_filter_panel.Show(true);
									categories_panel.Show(true);
									m_PreviousViewState = m_ViewState;
									m_ViewState = ExpansionP2PMarketMenuViewState.ViewList;
									ShowListView();
								}
							}							
						}
					}
					
					listing.Hide();
					listing.Destroy();
	
					m_ItemListings.RemoveOrdered(i);
				}
			}
		}
		
		switch (m_ViewState)
		{
			case ExpansionP2PMarketMenuViewState.ViewSales:
			{
				m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, true, m_CategoryIndex, m_SubCategoryIndex);
				break;
			}
			case ExpansionP2PMarketMenuViewState.ViewBrowse:
			{
				if (m_ItemListings && m_ItemListings.Count() < LISTINGS_PER_PAGE_COUNT)
				{
					array<string> searchTypeNames;
					string searchText = market_filter_box.GetText();
					if (searchText != "")
					{
						searchTypeNames = ExpansionItemNameTable.GetTypeNamesByString(searchText);
					}
				
					m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, false, m_CategoryIndex, m_SubCategoryIndex, searchTypeNames);
				}
				break;
			}
			case ExpansionP2PMarketMenuViewState.ViewList:
			case ExpansionP2PMarketMenuViewState.DetailViewPlayerItem:
			{
				UpdatePlayerCurrency();
				UpdatePlayerItems();
				break;
			}
		}
	}

	//! This method allows mods to override GetPreviewClassName while still keeping the original code in one place
	string GetPreviewClassName(string className, bool ignoreBaseBuildingKits = false)
	{
		return ExpansionStatic.GetPreviewClassName(className, ignoreBaseBuildingKits);
	}

	//! Gets price text and checks if it contains only numbers.
	//! Returns price as positive int if check passed, else displays error notification and returns -1.
	protected int GetCheckPrice()
	{
		int price = GetDetailsView().GetListPrice();
		
		if (price == -1)
			ExpansionNotification("#STR_EXPANSION_MARKET_P2P_NOTIF_ERROR_TITLE" , "#STR_EXPANSION_MARKET_P2P_NOTIF_ERROR_ONLYNUMBERS_DESC").Error();

		return price;
	}

	protected void UpdateItemInfoDamage(int damageLevel)
	{
		string text;
		int color;

		switch (damageLevel)
		{
			case GameConstants.STATE_RUINED:
			{
				text = "#inv_inspect_ruined";
				color = Colors.COLOR_RUINED;
				break;
			}
			case GameConstants.STATE_BADLY_DAMAGED:
			{
				text = "#inv_inspect_badly";
				color = Colors.COLOR_BADLY_DAMAGED;
				break;
			}

			case GameConstants.STATE_DAMAGED:
			{
				text = "#inv_inspect_damaged";
				color = Colors.COLOR_DAMAGED;
				break;
			}

			case GameConstants.STATE_WORN:
			{
				text = "#inv_inspect_worn";
				color = Colors.COLOR_WORN;
				break;
			}

			case GameConstants.STATE_PRISTINE:
			{
				text = "#inv_inspect_pristine";
				color = Colors.COLOR_PRISTINE;
				break;

			default:
			}
			{
				text = "#STR_EXPANSION_ERROR_TITLE";
				color = Colors.COLOR_PRISTINE;
				break;
			}
		}

		GetDetailsView().GetDetailsViewController().ItemHealth = text;
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemHealth");
		GetDetailsView().GetHealthImageWidget().SetColor(color | 0x7F000000);
	}

	void HideItemDetailElements()
	{
		GetDetailsView().ShowQuantityColor(false);
		GetDetailsView().ShowItemQuantity(false);
		GetDetailsView().ShowItemLiquid(false);
		GetDetailsView().ShowItemCargoCount(false);
		GetDetailsView().ShowItemAttachmentsCount(false);
		GetDetailsView().ShowItemFoodState(false);
		#ifdef EXPANSIONMODHARDLINE
		GetDetailsView().ShowItemRarity(false);
		#endif
	}

	void SetSelectedPlayerItem(ExpansionP2PMarketMenuItem item)
	{
		if (!item || !item.GetPlayerItem())
			return;

		GetDetailsView().ResetPreview();
		m_SelectedPlayerItem = item;
		m_SelectedListing = null;
		m_SelectedPreviewObject = item.GetPreviewObject();

		m_SelectedContainerItems = item.GetPlayerItem().GetContainerItems();

		GetDetailsView().SetViewList();

		GetDetailsView().GetDetailsViewController().SelectedName = item.GetPreviewObject().GetDisplayName();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("SelectedName");

		market_filter_panel.Show(false);
		categories_panel.Show(false);
		player_items_scroller.Show(false);
		inventory_panel.Show(false);
		tabs_panel.Show(false);

		GetDetailsView().ShowConfirmButton(true);
		GetDetailsView().GetDetailsViewController().ConfirmButtonText = "#STR_EXPANSION_MARKET_P2P_CREATESALE_BUTTON_LABEL";
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ConfirmButtonText");

		details_panel.AddChild(GetDetailsView().GetLayoutRoot());
		details_panel.Show(true);

		HideItemDetailElements();

		UpdateItemInfoDamage(item.GetPlayerItem().GetHealthLevel());

		item.GetPlayerItem().UpdateContainerItems();

		if (item.GetPlayerItem().GetLiquidType() != -1)
		{
			bool isBloodContainer = ExpansionStatic.IsAnyOf(item.GetPlayerItem().GetClassName(), m_BloodContainers);
			int liquidColor;
			GetDetailsView().ShowItemLiquid(true);
			GetDetailsView().GetDetailsViewController().LiquidType = GetLiquidTypeString(item.GetPlayerItem().GetLiquidType(), isBloodContainer, liquidColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("LiquidType");
			GetDetailsView().SetLiquidColor(liquidColor);
		}

		if (item.GetPlayerItem().GetFoodStageType() != -1)
		{
			int foodStageColor;
			GetDetailsView().ShowItemFoodState(true);
			GetDetailsView().GetDetailsViewController().FoodState = GetFoodStageString(item.GetPlayerItem().GetFoodStageType(), foodStageColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("FoodState");
			GetDetailsView().SetFoodStateColor(foodStageColor);
		}

		if (item.GetPlayerItem().GetQuantity() > 0)
		{
			int quantityColor;
			if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.POWER || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.MILLILITER || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.PERCENTAGE || item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.GRAM)
			{
				GetDetailsView().GetQuantityBarWidget().SetCurrent(item.GetPlayerItem().GetQuantity());
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(item.GetPlayerItem().GetQuantityType(), item.GetPlayerItem().GetQuantity(), quantityColor);
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityBar(true);
			}
			else if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.HEATISOLATION)
			{
				int quantity = item.GetPlayerItem().GetQuantity();
				string isolationText;
				int isolationColor;
				if (quantity <= GameConstants.HEATISO_THRESHOLD_BAD)
				{
					isolationText = "#inv_inspect_iso_bad";
					isolationColor = GetTemperatureColor(10);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_BAD) && (quantity <= GameConstants.HEATISO_THRESHOLD_LOW))
				{
					isolationText = "#inv_inspect_iso_low";
					isolationColor = GetTemperatureColor(20);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_LOW) && (quantity <= GameConstants.HEATISO_THRESHOLD_MEDIUM))
				{
					isolationText = "#inv_inspect_iso_medium";
					isolationColor = GetTemperatureColor(30);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_MEDIUM) && (quantity <= GameConstants.HEATISO_THRESHOLD_HIGH))
				{
					isolationText = "#inv_inspect_iso_high";
					isolationColor = GetTemperatureColor(50);
				}
				else
				{
					isolationText = "#inv_inspect_iso_excel";
					isolationColor = GetTemperatureColor(70);
				}

				GetDetailsView().GetDetailsViewController().QuantityText = "#STR_EXPANSION_MARKET_P2P_ITEMINFO_ISOLATIONLABEL";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = isolationText;
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityColor(true, isolationColor);
			}
			else if (item.GetPlayerItem().GetQuantityType() == ExpansionItemQuantityType.PC)
			{
				GetDetailsView().GetDetailsViewController().QuantityText = "#STR_EXPANSION_MARKET_P2P_ITEMINFO_QUANTITY_LABEL";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(item.GetPlayerItem().GetQuantityType(), item.GetPlayerItem().GetQuantity(), quantityColor);
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowItemQuantity(true);
			}
		}

		#ifdef EXPANSIONMODHARDLINE
		if (item.GetPlayerItem().GetRarity() != ExpansionHardlineItemRarity.NONE)
		{
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, item.GetPlayerItem().GetRarity());
			string rarityText = "#STR_EXPANSION_HARDLINE_" + rarityName;
			int rarityColor;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, rarityColor);
			int a, r, g, b;
			ExpansionStatic.IntToARGB(rarityColor, a, r, g, b);

			GetDetailsView().GetDetailsViewController().Rarity = rarityText;
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("Rarity");
			GetDetailsView().SetRarityColor(rarityColor);

			if (rarityName.Contains("Common"))
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 0, 0, 0));
			}
			else
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 255, 255, 255));
			}

			GetDetailsView().ShowItemRarity(true);
		}
		#endif

		GetDetailsView().ClearAttachments();
		GetDetailsView().ClearCargo();

		if (item.GetPlayerItem().GetContainerItems().Count() > 0)
		{
			int cargoItemsCount = 0;
			int attachmentsCount = 0;
			array<ref ExpansionP2PMarketContainerItem> containerItems = item.GetPlayerItem().GetContainerItems();
			if (containerItems && containerItems.Count() > 0)
			{
				ExpansionP2PMarketMenuCargoItem containerElement;
				for (int i = 0; i < containerItems.Count(); ++i)
				{
					ExpansionP2PMarketContainerItem containerItem = item.GetPlayerItem().GetContainerItems().Get(i);
					if (!containerItem)
						continue;

					containerElement = new ExpansionP2PMarketMenuCargoItem(containerItem, this);
					if (!containerItem.IsAttached())
					{
						GetDetailsView().AddCargoEntry(containerElement);
						cargoItemsCount++;
					}
					else
					{
						GetDetailsView().AddAttachmentEntry(containerElement);
						attachmentsCount++;
					}

					array<ref ExpansionP2PMarketContainerItem> containerItemsOfContainerItem = containerItem.GetContainerItems();
					if (containerItemsOfContainerItem && containerItemsOfContainerItem.Count() > 0)
					{
						foreach (ExpansionP2PMarketContainerItem containerItemOfContainerItem: containerItemsOfContainerItem)
						{
							containerElement = new ExpansionP2PMarketMenuCargoItem(containerItemOfContainerItem, this);
							if (!containerItemOfContainerItem.IsAttached())
							{
								GetDetailsView().AddCargoEntry(containerElement);
								cargoItemsCount++;
							}
							else
							{
								GetDetailsView().AddAttachmentEntry(containerElement);
								attachmentsCount++;
							}
						}
					}
				}
			}

			if (cargoItemsCount > 0)
			{
				GetDetailsView().ShowItemCargoCount(true);
				GetDetailsView().GetDetailsViewController().CargoItemsCount = cargoItemsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("CargoItemsCount");
			}

			if (attachmentsCount > 0)
			{
				GetDetailsView().ShowItemAttachmentsCount(true);
				GetDetailsView().GetDetailsViewController().AttachmentItemsCount = attachmentsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("AttachmentItemsCount");
			}
		}

		int lowest = GetLowestListingPrice(item.GetPlayerItem().GetClassName());
		if (lowest > 0)
		{
			GetDetailsView().GetDetailsViewController().LowestPrice = GetDisplayPrice(lowest, false, true, true);
		}
		else
		{
			GetDetailsView().GetDetailsViewController().LowestPrice = "#STR_EXPANSION_MARKET_P2P_NAN";
		}

		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("LowestPrice");

		int highest = GetHighestListingPrice(item.GetPlayerItem().GetClassName());
		if (highest > 0)
		{
			GetDetailsView().GetDetailsViewController().HighestPrice = GetDisplayPrice(highest, false, true, true);
		}
		else
		{
			GetDetailsView().GetDetailsViewController().HighestPrice = "#STR_EXPANSION_MARKET_P2P_NAN";
		}

		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("HighestPrice");

		if (GetExpansionSettings().GetMarket().MarketSystemEnabled)
		{
			int marketPrice = GetPlayerItemMarketPrice(item.GetPlayerItem());
			if (marketPrice > 0)
			{
				GetDetailsView().GetDetailsViewController().MarketPrice = GetDisplayPrice(marketPrice, false, true, true);
			}
			else
			{
				GetDetailsView().GetDetailsViewController().MarketPrice = "#STR_EXPANSION_MARKET_P2P_NAN";
			}

			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("MarketPrice");
		}

		int depositTime = m_P2PMarketSettings.SalesDepositTime;
		int depositTimeDays = Math.Round(depositTime / 86400);

		auto localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_INFOBOX_EXPIREDSALES_DESC", depositTimeDays.ToString());
		GetDetailsView().GetDetailsViewController().InfoTextOne = localiser.Format();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("InfoTextOne");

		int maxListingTime = m_P2PMarketSettings.MaxListingTime;
		int listingTimeDays = Math.Round(maxListingTime / 86400);

		localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_INFOBOX_EXPIREDLISTING_DESC", depositTimeDays.ToString());
		GetDetailsView().GetDetailsViewController().InfoTextTwo = localiser.Format();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("InfoTextTwo");

		int feePercent = m_P2PMarketModule.GetListingPricePercent(m_TraderID, PlayerBase.Cast(g_Game.GetPlayer()));

		localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_INFOBOX_LISTINGFEE_DESC", feePercent.ToString());
		GetDetailsView().GetDetailsViewController().InfoTextThree = localiser.Format();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("InfoTextThree");

		GetDetailsView().Show();
		GetDetailsView().UpdatePreview();
		
		GetDetailsView().UpdateItemInfoTooltip(m_SelectedPlayerItem.GetItem());

		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionP2PMarketMenuViewState.DetailViewPlayerItem;

		PlayObjectSound();
	}

	protected int GetPlayerItemMarketPrice(ExpansionP2PMarketPlayerItem playerItem)
	{
		int price = ExpansionP2PMarketModule.GetMarketPrice(playerItem.GetClassName());
		array<ref ExpansionP2PMarketContainerItem> containerItems = playerItem.GetContainerItems();
		if (containerItems && containerItems.Count() > 0)
		{
			foreach (ExpansionP2PMarketContainerItem containerItem: containerItems)
			{
				int containerItemPrice = ExpansionP2PMarketModule.GetMarketPrice(containerItem.GetClassName());
				price += containerItemPrice;
			}
		}

		return price;
	}

	protected string GetQuantityString(int quantityType, float quantity, out int color)
	{
		string quantity_str;
		if (quantityType == ExpansionItemQuantityType.PC)
		{
			if (quantity == 1)
			{
				quantity_str =  quantity.ToString() + " " + "#inv_inspect_piece";
				color = Colors.COLOR_DEFAULT;
			}
			else
			{
				quantity_str =  quantity.ToString() + " " + "#inv_inspect_pieces";
				color = Colors.COLOR_DEFAULT;
			}
		}
		else if (quantityType == ExpansionItemQuantityType.PERCENTAGE)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.GRAM)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.MILLILITER)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.POWER)
		{
			quantity_str = "#inv_inspect_remaining " + quantity.ToString() + "#inv_inspect_percent";
			color = Colors.COLOR_DEFAULT;
		}
		else if (quantityType == ExpansionItemQuantityType.MAGAZINE)
		{
			if (quantity == 1)
			{
				quantity_str = quantity.ToString() + " " + "#inv_inspect_piece";
				color = Colors.COLOR_DEFAULT;
			}
			else
			{
				quantity_str = quantity.ToString() + " " + "#inv_inspect_pieces";
				color = Colors.COLOR_DEFAULT;
			}
		}
		else
		{
			quantity_str = "";
			color = 0;
		}

		return quantity_str;
	}

	protected string GetFoodStageString(int foodStage, out int color)
	{
		if (foodStage == -1)
		{
			color = Colors.COLOR_ROTTEN;
			return "ERROR";
		}

		string text;
		switch (foodStage)
		{
			case FoodStageType.RAW:
			{
				text = "#inv_inspect_raw";
				color = Colors.COLOR_RAW;
				break;
			}
			case FoodStageType.BAKED:
			{
				text = "#inv_inspect_baked";
				color = Colors.COLOR_BAKED;
				break;
			}
			case FoodStageType.BOILED:
			{
				text = "#inv_inspect_boiled";
				color = Colors.COLOR_BOILED;
				break;
			}
			case FoodStageType.DRIED:
			{
				text = "#inv_inspect_dried";
				color = Colors.COLOR_DRIED;
				break;
			}
			case FoodStageType.BURNED:
			{
				text = "#inv_inspect_burned";
				color = Colors.COLOR_BURNED;
				break;
			}
			case FoodStageType.ROTTEN:
			{
				text = "#inv_inspect_rotten";
				color = Colors.COLOR_ROTTEN;
				break;
			}
		}

		return text;
	}

	protected string GetLiquidTypeString(int liquid_type, bool isBloodContainer, out int color)
	{
		string text;
		if (isBloodContainer)
		{
			if (liquid_type > -1)
			{
				string type;
				bool positive;
				string blood_type_name = BloodTypes.GetBloodTypeName(liquid_type, type, positive);
				text = "#inv_inspect_blood: " + blood_type_name;
				color = Colors.COLOR_LIQUID;
			}
			else if (liquid_type == -1)
			{
				text = "#inv_inspect_blood";
				color = Colors.COLOR_LIQUID;
			}
		}
		else if (liquid_type > -1)
		{
			text = ExpansionWorld.GetLiquidDisplayName(liquid_type, true, color);
		}

		return text;
	}

	void SetSelectedListingItem(ExpansionP2PMarketMenuListing listing)
	{
		if (!listing || !listing.GetListing())
			return;

		GetDetailsView().ResetPreview();
		m_SelectedListing = listing;
		m_SelectedPlayerItem = null;
		m_P2PMarketModule.RequestListingDetails(m_TraderID, listing.GetListing().GetGlobalID());
	}
	
	void OnListingDetailsRecived(ExpansionP2PMarketListing itemListing)
	{
		if (!m_SelectedListing || !itemListing)
			return;
		
		m_SelectedPreviewObject = m_SelectedListing.GetPreviewObject();
		m_SelectedContainerItems = itemListing.GetContainerItems();

		GetDetailsView().SetViewListing();
		GetDetailsView().GetDetailsViewController().SelectedName = m_SelectedListing.GetPreviewObject().GetDisplayName();
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("SelectedName");

		if (!m_SelectedListing.GetPreviewObject().IsInherited(ZombieBase) || !m_SelectedListing.GetPreviewObject().IsTransport())
		{
			GetDetailsView().GetDetailsViewController().ItemDescription = ExpansionStatic.GetItemDescriptionWithType(m_SelectedListing.GetListing().GetClassName());
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemDescription");
			GetDetailsView().GetItemDescriptionWidget().Update();
		}

		if (m_SelectedListing.IsOwnerdItem())
		{
			int discountPrice = ExpansionP2PMarketModule.GetDiscountPrice(itemListing.GetPrice());
			GetDetailsView().GetDetailsViewController().ListingPrice = GetDisplayPrice(discountPrice, false, true, true);
		}
		else
		{
			GetDetailsView().GetDetailsViewController().ListingPrice = GetDisplayPrice(itemListing.GetPrice(), false, true, true);
		}

		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ListingPrice");

		market_filter_panel.Show(false);
		categories_panel.Show(false);
		inventory_panel.Show(false);
		tabs_panel.Show(false);

		GetDetailsView().ShowConfirmButton(true);
		GetDetailsView().GetDetailsViewController().ConfirmButtonText = "#STR_EXPANSION_MARKET_P2P_PURCHASE_BUTTON_LABEL";
		GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ConfirmButtonText");

		if (m_ListHeader.IsVisible())
			m_ListHeader.Hide();

		details_panel.AddChild(GetDetailsView().GetLayoutRoot());
		details_panel.Show(true);

		HideItemDetailElements();

		UpdateItemInfoDamage(itemListing.GetHealthLevel());

		itemListing.UpdateContainerItems();

		if (itemListing.GetLiquidType() != -1)
		{
			bool isBloodContainer = ExpansionStatic.IsAnyOf(itemListing.GetClassName(), m_BloodContainers);
			int liquidColor;
			GetDetailsView().ShowItemLiquid(true);
			GetDetailsView().GetDetailsViewController().LiquidType = GetLiquidTypeString(itemListing.GetLiquidType(), isBloodContainer, liquidColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("LiquidType");
			GetDetailsView().SetLiquidColor(liquidColor);
		}

		if (itemListing.GetFoodStageType() != -1)
		{
			int foodStageColor;
			GetDetailsView().ShowItemFoodState(true);
			GetDetailsView().GetDetailsViewController().FoodState = GetFoodStageString(itemListing.GetFoodStageType(), foodStageColor);
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("FoodState");
			GetDetailsView().SetFoodStateColor(foodStageColor);
		}

		if (itemListing.GetQuantity() > 0)
		{
			int quantityColor;
			if (itemListing.GetQuantityType() == ExpansionItemQuantityType.POWER || itemListing.GetQuantityType() == ExpansionItemQuantityType.MILLILITER || itemListing.GetQuantityType() == ExpansionItemQuantityType.PERCENTAGE || itemListing.GetQuantityType() == ExpansionItemQuantityType.GRAM)
			{
				GetDetailsView().GetDetailsViewController().QuantityText = "#STR_EXPANSION_MARKET_P2P_ITEMINFO_QUANTITY_LABEL";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetQuantityBarWidget().SetCurrent(itemListing.GetQuantity());
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(itemListing.GetQuantityType(), itemListing.GetQuantity(), quantityColor);;
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityBar(true);
			}
			else if (itemListing.GetQuantityType() == ExpansionItemQuantityType.HEATISOLATION)
			{
				int quantity = itemListing.GetQuantity();
				string isolationText;
				int isolationColor;
				if (quantity <= GameConstants.HEATISO_THRESHOLD_BAD)
				{
					isolationText = "#inv_inspect_iso_bad";
					isolationColor = GetTemperatureColor(10);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_BAD) && (quantity <= GameConstants.HEATISO_THRESHOLD_LOW))
				{
					isolationText = "#inv_inspect_iso_low";
					isolationColor = GetTemperatureColor(20);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_LOW) && (quantity <= GameConstants.HEATISO_THRESHOLD_MEDIUM))
				{
					isolationText = "#inv_inspect_iso_medium";
					isolationColor = GetTemperatureColor(30);
				}
				else if ((quantity > GameConstants.HEATISO_THRESHOLD_MEDIUM) && (quantity <= GameConstants.HEATISO_THRESHOLD_HIGH))
				{
					isolationText = "#inv_inspect_iso_high";
					isolationColor = GetTemperatureColor(50);
				}
				else
				{
					isolationText = "#inv_inspect_iso_excel";
					isolationColor = GetTemperatureColor(70);
				}

				GetDetailsView().GetDetailsViewController().QuantityText = "#STR_EXPANSION_MARKET_P2P_ITEMINFO_ISOLATIONLABEL";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = isolationText;
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowQuantityColor(true, isolationColor);
			}
			else if (itemListing.GetQuantityType() == ExpansionItemQuantityType.PC)
			{
				GetDetailsView().GetDetailsViewController().QuantityText = "#STR_EXPANSION_MARKET_P2P_ITEMINFO_QUANTITY_LABEL";
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("QuantityText");
				GetDetailsView().GetDetailsViewController().ItemQuantity = GetQuantityString(itemListing.GetQuantityType(), itemListing.GetQuantity(), quantityColor);
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ItemQuantity");
				GetDetailsView().ShowItemQuantity(true);
			}
		}

		GetDetailsView().ClearAttachments();
		GetDetailsView().ClearCargo();

		if (itemListing.GetContainerItems().Count() > 0)
		{
			GetDetailsView().ShowItemCargoCount(true);
			GetDetailsView().GetDetailsViewController().CargoItemsCount = itemListing.GetContainerItems().Count().ToString();
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("CargoItemsCount");

			int cargoItemsCount;
			int attachmentsCount;
			array<ref ExpansionP2PMarketContainerItem> containerItems = itemListing.GetContainerItems();
			if (containerItems && containerItems.Count() > 0)
			{
				ExpansionP2PMarketMenuCargoItem containerElement;
				for (int i = 0; i < containerItems.Count(); ++i)
				{
					ExpansionP2PMarketContainerItem containerItem = containerItems[i];
					if (!containerItem)
						continue;

					containerElement = new ExpansionP2PMarketMenuCargoItem(containerItem, this);				
					containerElement.SetParent(m_SelectedListing);
					
					if (!containerItem.IsAttached())
					{
						GetDetailsView().AddCargoEntry(containerElement);
						cargoItemsCount++;
					}
					else
					{
						GetDetailsView().AddAttachmentEntry(containerElement);
						attachmentsCount++;
					}

					array<ref ExpansionP2PMarketContainerItem> containerItemsOfContainerItem = containerItem.GetContainerItems();
					if (containerItemsOfContainerItem && containerItemsOfContainerItem.Count() > 0)
					{
						foreach (ExpansionP2PMarketContainerItem containerItemOfContainerItem: containerItemsOfContainerItem)
						{
							containerElement = new ExpansionP2PMarketMenuCargoItem(containerItemOfContainerItem, this);
							containerElement.SetParent(m_SelectedListing);

							if (!containerItemOfContainerItem.IsAttached())
							{
								GetDetailsView().AddCargoEntry(containerElement);
								cargoItemsCount++;
							}
							else
							{
								GetDetailsView().AddAttachmentEntry(containerElement);
								attachmentsCount++;
							}
						}
					}
				}
			}

			if (cargoItemsCount > 0)
			{
				GetDetailsView().ShowItemCargoCount(true);
				GetDetailsView().GetDetailsViewController().CargoItemsCount = cargoItemsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("CargoItemsCount");
			}

			if (attachmentsCount > 0)
			{
				GetDetailsView().ShowItemAttachmentsCount(true);
				GetDetailsView().GetDetailsViewController().AttachmentItemsCount = attachmentsCount.ToString();
				GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("AttachmentItemsCount");
			}
		}

		#ifdef EXPANSIONMODHARDLINE
		if (itemListing.GetRarity() != ExpansionHardlineItemRarity.NONE)
		{
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, itemListing.GetRarity());
			string rarityText = "#STR_EXPANSION_HARDLINE_" + rarityName;
			int rarityColor;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, rarityColor);
			int a, r, g, b;
			ExpansionStatic.IntToARGB(rarityColor, a, r, g, b);

			GetDetailsView().GetDetailsViewController().Rarity = rarityText;
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("Rarity");
			GetDetailsView().SetRarityColor(rarityColor);

			if (rarityName.Contains("Common"))
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 0, 0, 0));
			}
			else
			{
				GetDetailsView().SetRarityTextColor(ARGB(255, 255, 255, 255));
			}

			GetDetailsView().ShowItemRarity(true);
		}
		#endif

		GetDetailsView().Show();
		GetDetailsView().UpdatePreview();

		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionP2PMarketMenuViewState.DetailViewListingItem;

		PlayObjectSound();
	}

	void OnBackClick()
	{
		ClearSelected();

		if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem)
		{
			OnListButtonClick();
		}
		else if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			OnBrowseButtonClick();
		}
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
		{
			OnSalesButtonClick();
		}
	}

	void GetSaleFromListing(TIntArray globalID)
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (m_RequestsLocked)
			return;
		
		if (m_SoldListingsCount - 1 == 0)
			OnBrowseButtonClick();	//! Switch from sales to browse view when there is no sales left.

		m_P2PMarketModule.GetSaleFromListing(globalID, m_TraderID, m_PageIndex);
	}

	protected void ClearSelected()
	{
		m_SelectedListing = null;
		m_SelectedPlayerItem = null;
		m_SelectedPreviewObject = null;
		m_SelectedContainerItems.Clear();
		m_CurrentItemPrice = 0;

		GetDetailsView().ShowConfirmButton(false);

		if (m_PreviousViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem || m_PreviousViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			GetDetailsView().GetDetailsViewController().SelectedName = "";
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("SelectedName");

			GetDetailsView().GetEditBoxWidget().SetText("");
			GetDetailsView().GetDetailsViewController().ListCost = "";
			GetDetailsView().GetDetailsViewController().NotifyPropertyChanged("ListCost");
		}
	}

	void SortListingsByName(bool reverse = false)
	{
		TStringArray displayName = new TStringArray;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			displayName.Insert(entry.GetPreviewObject().GetDisplayName());
		}

		displayName.Sort(reverse);

		foreach (ExpansionP2PMarketMenuListing currentEntry: m_ItemListings)
		{
			if (!currentEntry || !currentEntry.GetListing())
				continue;

			string name = currentEntry.GetPreviewObject().GetDisplayName();
			int index = displayName.Find(name) + 1;
			currentEntry.SetSort(index, false);
		}
	}

	void SortListingsByOwnerName(bool reverse = false)
	{
		TStringArray ownerName = new TStringArray;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			ownerName.Insert(entry.GetListing().GetOwnerName());
		}

		ownerName.Sort(reverse);

		foreach (ExpansionP2PMarketMenuListing currentEntry: m_ItemListings)
		{
			if (!currentEntry || !currentEntry.GetListing())
				continue;

			string name = entry.GetListing().GetOwnerName();
			int index = ownerName.Find(name) + 1;
			currentEntry.SetSort(index, false);
		}
	}

	void SortListingsByPrice(bool reverse = false)
	{
		TIntArray prices = new TIntArray;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			prices.Insert(entry.GetListing().GetPrice());
		}

		prices.Sort(reverse);

		foreach (ExpansionP2PMarketMenuListing currentEntry: m_ItemListings)
		{
			if (!currentEntry || !currentEntry.GetListing())
				continue;

			int price = currentEntry.GetListing().GetPrice();
			int index = prices.Find(price) + 1;
			currentEntry.SetSort(index, false);
		}
	}

	void SortListingsByTime(bool reverse = false)
	{
		TIntArray times = new TIntArray;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			times.Insert(entry.GetRemainingTime());
		}

		times.Sort(reverse);

		foreach (ExpansionP2PMarketMenuListing currentEntry: m_ItemListings)
		{
			if (!currentEntry || !currentEntry.GetListing())
				continue;

			int time = currentEntry.GetRemainingTime();
			int index = times.Find(time) + 1;
			currentEntry.SetSort(index, false);
		}
	}

	void OnListButtonClick()
	{
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewList)
			return;
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
		{
			m_ListHeader.SetListView(true);
			tabs_panel.Show(false);
		}
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
		{
			retrieve_button_panel.Show(false);
			player_sales_content.Show(false);
		}

		if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem || m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			ClearSelected();
			GetDetailsView().Hide();

			categories_panel.Show(true);
			inventory_panel.Show(true);
			player_items_scroller.Show(true);
			details_panel.Show(false);
		}

		tabs_panel.Show(true);
		market_filter_panel.Show(true);
		categories_panel.Show(true);
		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionP2PMarketMenuViewState.ViewList;
		ShowListView();
	}

	void OnBrowseButtonClick()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
			return;
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem || m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			ClearSelected();
			GetDetailsView().Hide();

			categories_panel.Show(true);
			inventory_panel.Show(true);
			player_items_scroller.Show(false);
			details_panel.Show(false);
		}
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
		{
			player_sales_content.Show(false);
			retrieve_button_panel.Show(false);
		}

		m_ListHeader.Show();
		m_ListHeader.SetListView(false);
		tabs_panel.Show(true);
		market_filter_panel.Show(true);
		categories_panel.Show(true);
		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionP2PMarketMenuViewState.ViewBrowse;
		ShowBrowseView();
	}

	void OnSalesButtonClick()
	{
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
			return;
		
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
		{
			//m_ListHeader.Hide();
			m_ListHeader.SetListView(true);
		}

		if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem || m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			ClearSelected();
			GetDetailsView().Hide();

			player_items_scroller.Show(false);
			details_panel.Show(false);
		}

		tabs_panel.Show(true);
		market_filter_panel.Show(false);
		categories_panel.Show(false);
		inventory_panel.Show(true);
		retrieve_button_panel.Show(true);
		player_sales_content.Show(true);

		m_PreviousViewState = m_ViewState;
		m_ViewState = ExpansionP2PMarketMenuViewState.ViewSales;
		ShowSalesView();
	}

	void OnRetrieveAllSalesClick()
	{
		if (m_ViewState != ExpansionP2PMarketMenuViewState.ViewSales || m_RequestsLocked)
			return;

		m_P2PMarketModule.RequestAllPlayerSales(m_TraderID);
	}

	protected void ShowBrowseView()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		inventory_scroller.Show(true);
		player_items_scroller.Show(false);
		sold_items_scroller.Show(false);

		listing_categories_content.Show(true);
		player_categories_content.Show(false);
		toggle_categories_panel.Show(true);
		
		tabs_button_list_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_list_text.SetColor(ARGB(255, 255, 255, 255));
		tabs_button_sales_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_sales_text.SetColor(ARGB(255, 255, 255, 255));

		m_PageIndex = 0;
		m_OwnedListings = false;
		m_CategoryIndex = -1;
		m_SubCategoryIndex = -1;
		m_PageStartNum = 1;
		
		m_P2PMarketModule.RequestBasicListingData(m_TraderID, 0, false);
	}

	protected void ShowListView()
	{
		inventory_scroller.Show(false);
		player_items_scroller.Show(true);
		sold_items_scroller.Show(false);
		pages_spacer.Show(false);

		listing_categories_content.Show(false);
		player_categories_content.Show(true);
		toggle_categories_panel.Show(false);
		
		tabs_button_browse_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_browse_text.SetColor(ARGB(255, 255, 255, 255));
		tabs_button_sales_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_sales_text.SetColor(ARGB(255, 255, 255, 255));
		
		UpdatePlayerItems();
	}

	protected void ShowSalesView()
	{
		inventory_scroller.Show(false);
		player_items_scroller.Show(false);
		sold_items_scroller.Show(true);
		categories_panel.Show(false);
		toggle_categories_panel.Show(false);

		listing_categories_content.Show(false);
		player_categories_content.Show(false);

		m_PageIndex = 0;
		m_OwnedListings = false;
		m_CategoryIndex = -1;
		m_SubCategoryIndex = -1;
		m_PageStartNum = 1;
		
		tabs_button_browse_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_browse_text.SetColor(ARGB(255, 255, 255, 255));
		tabs_button_list_bg.SetColor(ARGB(255, 19, 23, 27));
		tabs_button_list_text.SetColor(ARGB(255, 255, 255, 255));
		
		m_P2PMarketModule.RequestBasicListingData(m_TraderID, 0, true);
	}

	void Listings_Filter_ClassNameAZ()
	{
		SortListingsByName();
	}

	void Listings_Filter_ClassNameZA()
	{
		SortListingsByName(true);
	}

	void Listings_Filter_OwnerNameAZ()
	{
		SortListingsByName();
	}

	void Listings_Filter_OwnerNameZA()
	{
		SortListingsByOwnerName(true);
	}

	void Listings_Filter_PriceLH()
	{
		SortListingsByPrice();
	}

	void Listings_Filter_PriceHL()
	{
		SortListingsByPrice(true);
	}

	void Listings_Filter_TimeSL()
	{
		SortListingsByTime();
	}

	void Listings_Filter_TimeLS()
	{
		SortListingsByTime(true);
	}

	//! CATEGORIES
	void OnClickCategory_Listings()
	{
		m_PageIndex = 0;
		m_OwnedListings = true;
		m_AllListings = false;
		m_CategoryIndex = -1;
		m_SubCategoryIndex = -1;
		m_PageStartNum = 1;

		listing_category_listings_button.SetColor(ARGB(140, 255, 255, 255));
		listing_category_listings_text.SetColor(ARGB(255, 0, 0, 0));
		listing_category_listings_icon.SetColor(ARGB(255, 0, 0, 0));
		listing_category_listings_count.SetColor(ARGB(255, 0, 0, 0));
		
		listing_category_all_button.SetColor(ARGB(255, 0, 0, 0));
		listing_category_all_text.SetColor(ARGB(255, 255, 255, 255));
		listing_category_all_icon.SetColor(ARGB(255, 255, 255, 255));
		listing_category_all_count.SetColor(ARGB(255, 255, 255, 255));

		for (int i = 0; i < m_P2PMarketMenuController.ListingCategories.Count(); ++i)
		{
			ExpansionP2PMarketMenuCategoryElement category = m_P2PMarketMenuController.ListingCategories[i];			
			category.SetSelected(false);
			
			ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement> subCategories = category.GetSubCategories();
			for (int j = 0; j < subCategories.Count(); ++j)
			{
				ExpansionP2PMarketMenuSubCategoryElement subCategory = subCategories[j]; 
				subCategory.SetSelected(false);
			}
		}
		
		m_P2PMarketModule.RequestBasicListingData(m_TraderID, 0, false, -1, -1, null, m_OwnedListings);
	}

	void OnClickCategory_All()
	{
		m_PageIndex = 0;
		m_OwnedListings = false;
		m_AllListings = true;
		m_CategoryIndex = -1;
		m_SubCategoryIndex = -1;
		m_PageStartNum = 1;

		listing_category_all_button.SetColor(ARGB(140, 255, 255, 255));
		listing_category_all_text.SetColor(ARGB(255, 0, 0, 0));
		listing_category_all_icon.SetColor(ARGB(255, 0, 0, 0));
		listing_category_all_count.SetColor(ARGB(255, 0, 0, 0));
		
		listing_category_listings_button.SetColor(ARGB(255, 0, 0, 0));
		listing_category_listings_text.SetColor(ARGB(255, 255, 255, 255));
		listing_category_listings_icon.SetColor(ARGB(255, 255, 255, 255));
		listing_category_listings_count.SetColor(ARGB(255, 255, 255, 255));
		
		for (int i = 0; i < m_P2PMarketMenuController.ListingCategories.Count(); ++i)
		{
			ExpansionP2PMarketMenuCategoryElement category = m_P2PMarketMenuController.ListingCategories[i];			
			category.SetSelected(false);
			
			ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement> subCategories = category.GetSubCategories();
			for (int j = 0; j < subCategories.Count(); ++j)
			{
				ExpansionP2PMarketMenuSubCategoryElement subCategory = subCategories[j]; 
				subCategory.SetSelected(false);
			}
		}

		m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, false, m_CategoryIndex, m_SubCategoryIndex, null, m_OwnedListings);
	}

	void UpdateMenuCategory(ExpansionP2PMarketCategoryListings categoryData)
	{
		for (int i = 0; i < m_P2PMarketMenuController.ListingCategories.Count(); ++i)
		{
			ExpansionP2PMarketMenuCategoryElement category = m_P2PMarketMenuController.ListingCategories[i];
			bool isSelected = (category.GetCategoryData() == categoryData);
			bool isChildSelected = false;
			
			category.SetSelected(isSelected);
			
			ObservableCollection<ref ExpansionP2PMarketMenuSubCategoryElement> subCategories = category.GetSubCategories();
			for (int j = 0; j < subCategories.Count(); ++j)
			{
				ExpansionP2PMarketMenuSubCategoryElement subCategory = subCategories[j]; 
				isChildSelected = (subCategory.GetCategoryData() == categoryData);
				subCategory.SetSelected(isChildSelected);
			}
		}

		listing_category_listings_button.SetColor(ARGB(255, 0, 0, 0));
		listing_category_listings_text.SetColor(ARGB(255, 255, 255, 255));
		listing_category_listings_icon.SetColor(ARGB(255, 255, 255, 255));
		listing_category_listings_count.SetColor(ARGB(255, 255, 255, 255));

		listing_category_all_button.SetColor(ARGB(255, 0, 0, 0));
		listing_category_all_text.SetColor(ARGB(255, 255, 255, 255));
		listing_category_all_icon.SetColor(ARGB(255, 255, 255, 255));
		listing_category_all_count.SetColor(ARGB(255, 255, 255, 255));
		
		m_PageIndex = 0;	
		m_OwnedListings = false;
		m_AllListings = false;
		m_CategoryIndex = categoryData.m_CategoryIndex;
		m_SubCategoryIndex = categoryData.m_SubCategoryIndex;
		m_PageStartNum = 1;
		
		if (market_filter_box.GetText() != "")
			market_filter_box.SetText("");

		m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, false, m_CategoryIndex, m_SubCategoryIndex, null, m_OwnedListings);
	}
	
	void OnConfirmSearch()
	{
		m_PageIndex = 0;
		m_OwnedListings = false;
		m_PageStartNum = 1;
		
		string searchText = market_filter_box.GetText();
		ErrorEx("" + m_UseValidListingsCount, ErrorExSeverity.INFO);
		m_UseValidListingsCount = (searchText != "");
		array<string> searchTypeNames = ExpansionItemNameTable.GetTypeNamesByString(searchText);
		m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, false, m_CategoryIndex, m_SubCategoryIndex, searchTypeNames, m_OwnedListings);
	}

	//! MENU EVENTS
	void OnConfirmButtonClick()
	{
		if (m_RequestsLocked)
			return;

		CF_Localiser localiser;

		if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewListingItem && m_SelectedListing)
		{
			int price = m_SelectedListing.GetListing().GetPrice();
			if (m_SelectedListing.IsOwnerdItem())
				price = ExpansionP2PMarketModule.GetDiscountPrice(m_SelectedListing.GetListing().GetPrice());

			string priceString = GetDisplayPrice(price, false, true, true);
			localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_CONFIRM_BUTTON_BUY_LABEL", m_SelectedListing.GetPreviewObject().GetDisplayName(), priceString);

			SetDialogView(localiser.Format());
		}
		else if (m_ViewState == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem && m_SelectedPlayerItem)
		{
			//! Only numbers are allowed
			m_CurrentItemPrice = GetCheckPrice();
			if (m_CurrentItemPrice == -1 || m_CurrentItemPrice == 0)
				return;

			int listingPrice = Math.Ceil(m_CurrentItemPrice * m_P2PMarketModule.GetListingPricePercent(m_TraderID, PlayerBase.Cast(g_Game.GetPlayer())) / 100);
			string currentItemPriceString = GetDisplayPrice(m_CurrentItemPrice, false, true, true);
			string listingPriceString = GetDisplayPrice(listingPrice, false, true, true);
			localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_CONFIRM_BUTTON_SELL_LABEL", m_SelectedPlayerItem.GetPreviewObject().GetDisplayName(), currentItemPriceString, listingPriceString);

			SetDialogView(localiser.Format());
		}
	}

	protected void SetDialogView(string text)
	{
		if (m_MenuDialog)
			m_MenuDialog.Destroy();

		m_MenuDialog = new ExpansionDialog_P2PMarketMenu(this, text);
		m_MenuDialog.Show();
	}

	void CancelDialogView()
	{
		GetDetailsView().ShowConfirmButton(true);
		GetDetailsView().Show();
		tabs_button_browse.Show(true);
		tabs_button_list.Show(true);
		exit_button.Show(true);
	}

	void OnConfirmListingClick()
	{
		ErrorEx("Requests locked: " + m_RequestsLocked, ErrorExSeverity.INFO);
		if (!m_SelectedPlayerItem || !m_SelectedPlayerItem.GetPlayerItem() || m_RequestsLocked)
			return;

		EntityAI playerItem = m_SelectedPlayerItem.GetPlayerItem().GetItem();
		if (!playerItem)
			return;

		m_RequestsLocked = true;
		m_P2PMarketModule.RequestListItem(m_TraderID, playerItem, m_CurrentItemPrice);
		GetDetailsView().UpdatePreview();
		OnBackClick();
	}

	void OnPurchaseListingClick()
	{
		ErrorEx("Requests locked: " + m_RequestsLocked, ErrorExSeverity.INFO);
		if (!m_SelectedListing || !m_SelectedListing.GetListing() || m_RequestsLocked)
			return;

		m_RequestsLocked = true;
		m_P2PMarketModule.RequestPurchaseItem(m_TraderID, m_SelectedListing.GetListing(), m_PageIndex, m_CategoryIndex, m_SubCategoryIndex, m_OwnedListings);
		GetDetailsView().UpdatePreview();
		OnBackClick();
	}

	void QuickbuyListing(ExpansionP2PMarketMenuListing listing)
	{
		ErrorEx("Requests locked: " + m_RequestsLocked, ErrorExSeverity.INFO);
		if (!listing || !listing.GetListing() || m_RequestsLocked)
			return;

		m_RequestsLocked = true;
		m_P2PMarketModule.RequestPurchaseItem(m_TraderID, listing.GetListing(), m_PageIndex, m_CategoryIndex, m_SubCategoryIndex, m_OwnedListings);
		GetDetailsView().UpdatePreview();
		OnBrowseButtonClick();
	}

	void OnExitClick()
	{
		CloseMenu();
	}

	void OnFilterButtonClick()
	{
		market_filter_box.SetText("");
		OnSearchFilterChange();
	}

	void UpdateInventorySlotFilter(string slotName)
	{
		for (int i = 0; i < m_P2PMarketMenuController.PlayerItems.Count(); ++i)
		{
			ExpansionP2PMarketMenuItem playerItem = m_P2PMarketMenuController.PlayerItems[i];
			if (playerItem.GetPlayerItem().GetSlotName() != slotName && slotName != "All")
			{
				playerItem.Hide();
			}
			else
			{
				playerItem.Show();
			}
		}
	}

	int GetInventoryCategoryItemsCount(string slotName)
	{
		int count;
		if (slotName == "All")
		{
			return m_P2PMarketMenuController.PlayerItems.Count();
		}
		else
		{
			for (int i = 0; i < m_P2PMarketMenuController.PlayerItems.Count(); ++i)
			{
				ExpansionP2PMarketMenuItem playerItem = m_P2PMarketMenuController.PlayerItems[i];
				if (playerItem.GetPlayerItem().GetSlotName() == slotName)
					count++;
			}
		}

		return count;
	}

	void OnSearchFilterChange()
	{
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewList)
		{
			SearchInPlayerItems();
		}
		else if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
		{
			OnConfirmSearch();
		}
	}
	
	protected bool IsValidSearchListing(string searchText, ExpansionP2PMarketListing listing)
	{
		bool hasName;
		bool hasChildWithName;
		string displayName;
		
		TStringArray tokens = new TStringArray;
		searchText.Split(" ", tokens);
		
		array<ref ExpansionP2PMarketContainerItem> containerItems = listing.GetContainerItems();
		if (containerItems && containerItems.Count() > 0)
		{
			for (int i = 0; i < containerItems.Count(); ++i)
			{
				ExpansionP2PMarketContainerItem containerItem = containerItems[i];
				displayName = ExpansionStatic.GetItemDisplayNameWithType(containerItem.GetClassName());
				displayName.ToLower();			
				
				foreach(string sC: tokens)
				{
					if (displayName.IndexOf(sC) != -1)
						hasChildWithName = true;
				}
			}
		}
		
		displayName = ExpansionStatic.GetItemDisplayNameWithType(listing.GetClassName());
		displayName.ToLower();

		foreach(string sM: tokens)
		{
			if (displayName.IndexOf(sM) != -1)
				hasName = true;
		}
		
		if (hasName || hasChildWithName)
			return true;

		return false;
	}

	protected void SearchInPlayerItems()
	{
		if (!m_P2PMarketMenuController.PlayerItems || m_P2PMarketMenuController.PlayerItems.Count() == 0)
			return;

		bool hasChildWithName;
		string displayName;
		string searchText = market_filter_box.GetText();
		TStringArray tokens;
		if (searchText != "")
		{
			searchText.ToLower();
			tokens = new TStringArray;
			searchText.Split(" ", tokens);
		}

		for (int j = 0; j < m_P2PMarketMenuController.PlayerItems.Count(); ++j)
		{
			ExpansionP2PMarketMenuItem playerItem = m_P2PMarketMenuController.PlayerItems[j];
			if (playerItem && playerItem.GetPreviewObject())
			{
				if (!playerItem.IsVisible() || searchText == string.Empty)
				{
					playerItem.Show();
				}

				hasChildWithName = false;
				ObservableCollection<ref ExpansionP2PMarketMenuItem> cargoEntries = playerItem.GetCargoItemElemens();
				if (cargoEntries && cargoEntries.Count() > 0)
				{
					for (int k = 0; k < cargoEntries.Count(); ++k)
					{
						ExpansionP2PMarketMenuItem cargoEntry = cargoEntries[k];
						if (!cargoEntry.IsVisible() || searchText == string.Empty)
						{
							cargoEntry.Show();
							continue;
						}
						
						displayName = cargoEntry.GetPreviewObject().GetDisplayName();
						displayName.ToLower();
						foreach(string sC: tokens)
						{
							if (displayName.IndexOf(searchText) > -1)
							{
								hasChildWithName = true;
							}
						}
						
						if (!hasChildWithName)
							cargoEntry.Hide();
					}
				}
			}

			displayName = playerItem.GetPreviewObject().GetDisplayName();
			displayName.ToLower();
			foreach(string sM: tokens)
			{
				if (displayName.IndexOf(searchText) == -1 && !hasChildWithName)
					playerItem.Hide();
			}
		}
	}

	protected void PlayObjectSound()
	{
		if (g_Game.IsClient() || !g_Game.IsMultiplayer())
		{
			if (g_Game.GetPlayer())
			{
				string typeName;
				string soundConfig;
				string path;
				if (m_SelectedPlayerItem)
					typeName = m_SelectedPlayerItem.GetPreviewObject().GetType();
				else if (m_SelectedListing)
					typeName = m_SelectedListing.GetPreviewObject().GetType();

				path = "cfgVehicles " + typeName + " " + "AnimEvents ";

				int events_sources_count = g_Game.ConfigGetChildrenCount(path);
				string soundName = "";
				EffectSound sound;

				for (int i = 0; i < events_sources_count; ++i)
				{
					string soundWeapons;
					g_Game.ConfigGetChildName(path, i, soundWeapons);

					if (g_Game.ConfigIsExisting(path + soundWeapons + " pickUpItem" + " soundSet"))
						g_Game.ConfigGetText(path + soundWeapons + " pickUpItem" + " soundSet", soundName);
					else if (g_Game.ConfigIsExisting(path + soundWeapons + " pickUpItem_Light" + " soundSet"))
						g_Game.ConfigGetText(path + soundWeapons + " pickUpItem_Light" + " soundSet", soundName);
				}

				if (soundName != "")
					sound = SEffectManager.PlaySound(soundName, g_Game.GetPlayer().GetPosition());
				else
					sound = SEffectManager.PlaySound("pickUpBackPack_Metal_SoundSet", g_Game.GetPlayer().GetPosition());

				sound.SetSoundAutodestroy(true);
			}
		}
	}

	void OnToggleCategoriesClick()
	{
		m_CategoriesToggleState = !m_CategoriesToggleState;
		for (int i = 0; i < m_P2PMarketMenuController.ListingCategories.Count(); ++i)
		{
			ExpansionP2PMarketMenuCategoryElement categoryElement = m_P2PMarketMenuController.ListingCategories[i];
			if (!categoryElement)
				continue;

			categoryElement.ShowSubCategories(m_CategoriesToggleState);
		}
	}

	void UpdatePageButtons()
	{
		int entriesCount = 0;
	    if (m_CategoryIndex > -1 || m_SubCategoryIndex > -1)
	    {
	        ExpansionP2PMarketCategoryListings categoryData = m_P2PMarketModule.GetCategoryListingsData(m_CategoryIndex, m_SubCategoryIndex, m_TraderID, m_GlobalTrader);
	        if (categoryData)
	        {
	            entriesCount = categoryData.m_Count;
	        }
	    }
	    else if (m_OwnedListings)
	    {
	        entriesCount = m_OwnedListingsCount;
	    }
	    else
		{
			if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
				entriesCount = m_SoldListingsCount;
			else if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewBrowse)
				entriesCount = m_ListingsCount;
		}
		
		m_PagesCount = Math.Ceil(entriesCount / LISTINGS_PER_PAGE_COUNT);
		if (m_PagesCount > 1)
		{
			pages_spacer.Show(true);
	
			int btn_index_center = Math.Round(PAGES_BUTTONS_COUNT / 2) - 1; 
			m_PageStartNum = 1;
			m_PageEndNum = PAGES_BUTTONS_COUNT;
			
			if (m_PagesCount <= PAGES_BUTTONS_COUNT)
			{
				m_PageEndNum = m_PagesCount;
				
				// Pages are less like buttons - changing selected button
				for (int i = 1; i <= m_BtnPages.Count(); ++i)
				{
					bool is_page_button_enable = (i <= m_PagesCount);
					int page_index = (i - 1);
					m_BtnPages[page_index].Show(is_page_button_enable);
					m_BtnPages[page_index].SetText(i.ToString());
					
					if (m_PageIndex == page_index)
					{
						m_BtnPages[page_index].SetState(true);
					}
					else
					{
						m_BtnPages[page_index].SetState(false);
					}
				}
			}
			else
			{
				// Pages are more like buttons				
				if (m_PageIndex > btn_index_center)
				{
					m_PageStartNum = m_PageIndex - btn_index_center + 1;
					m_PageEndNum = m_PageIndex + (PAGES_BUTTONS_COUNT - btn_index_center);
					if (m_PageEndNum > m_PagesCount)
					{
						// Right side (end of pages)
						m_PageStartNum -= m_PageEndNum - m_PagesCount;
						m_PageEndNum -= m_PageEndNum - m_PagesCount;
					}
				}
				
				
				// Button Selection
				int selected_btn_index = m_PageIndex;
				if (m_PageIndex > btn_index_center)
				{
					if (m_PageEndNum == m_PagesCount)
					{
						//Right
						 selected_btn_index = PAGES_BUTTONS_COUNT - (m_PagesCount - m_PageIndex);
					}
					else
					{
						//Center
						selected_btn_index = btn_index_center;
					}
				}
				
				for (int j = 0; j < m_BtnPages.Count(); ++j)
				{
					m_BtnPages[j].SetText((m_PageStartNum + j).ToString());
					m_BtnPages[j].SetState((j == selected_btn_index));
					m_BtnPages[j].Show(true);
				}
			}
			
			// Button GoTo Prev Page
			if (m_PageIndex > 0)
			{
				navigation_page_prev.SetText("<");
				navigation_page_prev.Enable(true);
			}
			else
			{
				navigation_page_prev.SetText("");
				navigation_page_prev.Enable(false);					
			}
			
			// Button GoTo Next Page
			if (m_PageIndex < (m_PagesCount - 1))
			{
				navigation_page_next.SetText(">");
				navigation_page_next.Enable(true);
			}
			else
			{
				navigation_page_next.SetText("");
				navigation_page_next.Enable(false);
			}
			
			// Button GoTo First Page
			if (m_PageStartNum == 1)
			{
				navigation_page_first.SetText("");
				navigation_page_first.Enable(false);
			}
			else
			{
				navigation_page_first.SetText("<<");
				navigation_page_first.Enable(true);
			}
			
			// Button GoTo Last Page
			if (m_PageEndNum == m_PagesCount)
			{
				navigation_page_last.SetText("");
				navigation_page_last.Enable(false);
			}
			else
			{
				navigation_page_last.SetText(">>");
				navigation_page_last.Enable(true);
			}
		}
		else
		{
			pages_spacer.Show(false);
		}
	}
	
	void OnClickPagePrev()
	{
		if (m_PageIndex > 0)
		{
			SetPageIndex(m_PageIndex - 1);
		}
		else
		{
			SetPageIndex(m_PagesCount - 1);
		}
	}
	
	void OnClickPageNext()
	{
		if (m_PageIndex < (m_PagesCount - 1))
		{
			SetPageIndex(m_PageIndex + 1);
		}
		else
		{
			SetPageIndex(0);
		}
	}
	
	void OnClickPageEnd()
	{
		SetPageIndex(m_PagesCount - 1);
	}
	
	void OnClickPageFirst()
	{
		SetPageIndex(0);
	}
	
	bool OnClickPage(Widget w)
	{
		for (int i = 0; i < m_BtnPages.Count(); ++i)
		{
			if (m_BtnPages[i] == w)
			{
				//SetPageIndex(i);
				SetPageIndex(m_PageStartNum + i - 1);
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if (w == market_filter_box)
		{
			SetFocus(market_filter_box);
		}
		else if (button == MouseState.LEFT)
		{
			bool click = OnClickPage(w);
			if (click)
			{
				return true;
			}
		}

		return false;
	}
	
	void SetPageIndex(int page_index)
	{
		m_PageIndex = page_index;
		
		bool soldListings;
		if (m_ViewState == ExpansionP2PMarketMenuViewState.ViewSales)
		{
			soldListings = true;
		}
		
		array<string> searchTypeNames;
		string searchText = market_filter_box.GetText();
		if (searchText != "")
		{
			searchTypeNames = ExpansionItemNameTable.GetTypeNamesByString(searchText);
		}
		
		m_P2PMarketModule.RequestBasicListingData(m_TraderID, m_PageIndex, soldListings, m_CategoryIndex, m_SubCategoryIndex, searchTypeNames, m_OwnedListings);
	}

	override void OnShow()
	{
		#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);
		#endif
		
		super.OnShow();

		loading.Show(true);
		SetFocus(GetLayoutRoot());
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == tabs_button_browse)
			{
				tabs_button_browse_bg.SetColor(ARGB(140, 255, 255, 255));
				tabs_button_browse_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == tabs_button_list)
			{
				tabs_button_list_bg.SetColor(ARGB(140, 255, 255, 255));
				tabs_button_list_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == tabs_button_sales)
			{
				tabs_button_sales_bg.SetColor(ARGB(140, 255, 255, 255));
				tabs_button_sales_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}	
			else if (w == exit_button)
			{
				exit_button_panel.SetColor(ARGB(140, 255, 255, 255));
				exit_button_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == retrieve_button)
			{
				retrieve_button.SetColor(ARGB(140, 255, 255, 255));
				retrieve_button_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == listing_category_listings_button)
			{
				listing_category_listings_button.SetColor(ARGB(140, 255, 255, 255));
				listing_category_listings_text.SetColor(ARGB(255, 0, 0, 0));
				listing_category_listings_icon.SetColor(ARGB(255, 0, 0, 0));
				listing_category_listings_count.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == listing_category_all_button)
			{
				listing_category_all_button.SetColor(ARGB(140, 255, 255, 255));
				listing_category_all_text.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_icon.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_count.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == toggle_categories_button)
			{
				toggle_categories_panel.SetColor(ARGB(140, 255, 255, 255));
				toggle_categories_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == market_filter_clear)
			{
				market_filter_clear_icon.SetColor(ARGB(140, 255, 180, 24));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == tabs_button_browse && m_ViewState != ExpansionP2PMarketMenuViewState.ViewBrowse)
			{
				tabs_button_browse_bg.SetColor(ARGB(255, 19, 23, 27));
				tabs_button_browse_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == tabs_button_list && m_ViewState != ExpansionP2PMarketMenuViewState.ViewList)
			{
				tabs_button_list_bg.SetColor(ARGB(255, 19, 23, 27));
				tabs_button_list_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == tabs_button_sales && m_ViewState != ExpansionP2PMarketMenuViewState.ViewSales)
			{
				tabs_button_sales_bg.SetColor(ARGB(255, 19, 23, 27));
				tabs_button_sales_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == exit_button)
			{
				exit_button_panel.SetColor(ARGB(255, 0, 0, 0));
				exit_button_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == retrieve_button)
			{
				retrieve_button.SetColor(ARGB(255, 0, 0, 0));
				retrieve_button_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == listing_category_listings_button && !m_OwnedListings)
			{
				listing_category_listings_button.SetColor(ARGB(255, 0, 0, 0));
				listing_category_listings_text.SetColor(ARGB(255, 255, 255, 255));
				listing_category_listings_icon.SetColor(ARGB(255, 255, 255, 255));
				listing_category_listings_count.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == listing_category_all_button && !m_AllListings)
			{
				listing_category_all_button.SetColor(ARGB(255, 0, 0, 0));
				listing_category_all_text.SetColor(ARGB(255, 255, 255, 255));
				listing_category_all_icon.SetColor(ARGB(255, 255, 255, 255));
				listing_category_all_count.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == toggle_categories_button)
			{
				toggle_categories_panel.SetColor(ARGB(255, 0, 0, 0));
				toggle_categories_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == market_filter_clear)
			{
				market_filter_clear_icon.SetColor(ARGB(255, 251, 252, 254));
				return true;
			}
		}

		return false;
	}

	ExpansionP2PMarketMenuDetailsView GetDetailsView()
	{
		return m_ItemDetailsView;
	}

	WrapSpacerWidget GetListingsWrapSpacer()
	{
		return inventory_content;
	}

	WrapSpacerWidget GetSoldItemsWrapSpacer()
	{
		return sold_items_content;
	}

	Object GetSelectedPreviewObject()
	{
		return m_SelectedPreviewObject;
	}

	array<ref ExpansionP2PMarketContainerItem> GetSelectedContainerItems()
	{
		return m_SelectedContainerItems;
	}

	protected int GetLowestListingPrice(string typeName)
	{
		int lowest;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			if (entry.GetListing().GetClassName() != typeName)
				continue;

			if (lowest == 0 || lowest != 0 && entry.GetListing().GetPrice() < lowest)
				lowest = entry.GetListing().GetPrice();
		}

		return lowest;
	}

	protected int GetHighestListingPrice(string typeName)
	{
		int highest;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			if (entry.GetListing().GetClassName() != typeName)
				continue;

			if (highest == 0 || highest != 0 && entry.GetListing().GetPrice() > highest)
				highest = entry.GetListing().GetPrice();
		}

		return highest;
	}

	int GetCategoryListingsCount(ExpansionP2PMarketMenuCategoryBase category)
	{
		int count;
		foreach (ExpansionP2PMarketMenuListing entry: m_ItemListings)
		{
			if (!entry || !entry.GetListing())
				continue;

			TStringArray included = category.GetIncluded();
			TStringArray excluded = category.GetExcluded();
			if (ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), included) && !ExpansionStatic.IsAnyOf(entry.GetPreviewObject(), excluded))
				count++;
		}

		return count;
	}
	
	void SetIsInspectingItem(bool state, ExpansionP2PMarketMenuItemBase element)
	{
		if (state)
		{
			m_ViewState = ExpansionP2PMarketMenuViewState.InspectListingItem;
		}
		else
		{
			m_ViewState = ExpansionP2PMarketMenuViewState.ViewBrowse;
		}

		m_InspectingElement = element;
	}

	ExpansionP2PMarketMenuViewState GetViewState()
	{
		return m_ViewState;
	}

	string GetDisplayPrice(int price, bool shorten = false, bool format = true, bool includeDisplayCurrencyName = false)
	{
		return ExpansionMarketModule.GetDisplayPriceEx(price, shorten, format, includeDisplayCurrencyName, m_DisplayCurrencyValue, m_DisplayCurrencyPrecision, m_DisplayCurrencyName);
	}

	int GetDisplayCurrencyValue()
	{
		return m_DisplayCurrencyValue;
	}
	
	void OnRestrainedStateChaged(bool isRestrained)
	{
		if (isRestrained)
		{
			OnExitClick();
		}
	}
	
	int GetPageIndex()
	{
		return m_PageIndex;
	}
	
	int GetTraderID()
	{
		return m_TraderID;
	}
	
	bool IsGlobalTrader()
	{
		return m_GlobalTrader;
	}
	
	EditBoxWidget GetSearchFilterBox()
	{
		return market_filter_box;
	}
	
	int GetCategoryIndex()
	{
		return m_CategoryIndex;
	}
	
	int GetSubCategoryIndex()
	{
		return m_SubCategoryIndex;
	}
};

class ExpansionP2PMarketMenuController: ExpansionViewController
{
	string PlayerMoney;
	string ConfirmButtonText;
	string PlayerListingsCount;
	string AllListingsCount;
	string TraderName;
	string MarketIcon;
	string PlayerSales;
	string PlayerSoldListings;
	ref ObservableCollection<ref ExpansionP2PMarketMenuItem> PlayerItems = new ObservableCollection<ref ExpansionP2PMarketMenuItem>(this);
	ref ObservableCollection<ref ExpansionP2PMarketMenuCategoryElement> ListingCategories = new ObservableCollection<ref ExpansionP2PMarketMenuCategoryElement>(this);
	ref ObservableCollection<ref ExpansionP2PMarketMenuInventoryCategoryElement> PlayerCategories = new ObservableCollection<ref ExpansionP2PMarketMenuInventoryCategoryElement>(this);
};