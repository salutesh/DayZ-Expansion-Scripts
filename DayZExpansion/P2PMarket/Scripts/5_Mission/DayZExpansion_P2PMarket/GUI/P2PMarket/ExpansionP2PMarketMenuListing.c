/**
 * ExpansionP2PMarketMenuListing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuListing: ExpansionP2PMarketMenuItemBase
{
	protected ref ExpansionP2PMarketMenuListingController m_P2PMarketMenuListingController;
	protected ExpansionP2PMarketListing m_Listing;

	protected bool m_IsOwnedItem;
	protected bool m_IsSoldItem;

	protected Widget time_panel;
	protected Widget name_panel;
	protected WrapSpacerWidget price_panel;
	protected TextWidget discount_text;
	protected TextWidget price_text
	protected ImageWidget price_icon;
	protected TextWidget time_text
	protected TextWidget name_text;
	protected Widget sold_icon_panel;
	protected ImageWidget sold_icon;
	
	protected Widget quickbuy_panel;
	
	protected int m_RemaningTime;

	void ExpansionP2PMarketMenuListing(ExpansionP2PMarketListingBase item, ExpansionP2PMarketMenu menu)
	{
		Class.CastTo(m_P2PMarketMenuListingController, GetController());

		m_Listing = ExpansionP2PMarketListing.Cast(item);
		
		if (m_Listing.GetListingState() == ExpansionP2PMarketListingState.LISTED)
		{
			m_P2PMarketMenu.GetListingsWrapSpacer().AddChild(GetLayoutRoot());
		}
		else if (m_Listing.GetListingState() == ExpansionP2PMarketListingState.SOLD)
		{
			m_P2PMarketMenu.GetSoldItemsWrapSpacer().AddChild(GetLayoutRoot());
		}

		SetView();
	}

	void SetView()
	{
		SetRemainingTime();
		UpdatePreviewObject();
		
		string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_Listing.GetClassName());
		string displayText = displayName;
		if (m_Listing.GetContainerItems().Count() > 0)
			displayText = "[+] " + displayName;
		
		if (m_Listing.GetQuantityType() == ExpansionItemQuantityType.PC || m_Listing.GetQuantityType() == ExpansionItemQuantityType.MAGAZINE)
			displayText = displayText + " - [" + Math.Round(m_Listing.GetQuantity()) + "]";
		
		m_P2PMarketMenuListingController.ItemName = displayText;
		m_P2PMarketMenuListingController.NotifyPropertyChanged("ItemName");

		string moneyString = ExpansionStatic.IntToCurrencyString(m_Listing.GetPrice(), ",");
		m_P2PMarketMenuListingController.Price = moneyString;
		m_P2PMarketMenuListingController.NotifyPropertyChanged("Price");

		auto setting = GetExpansionSettings().GetP2PMarket();
		m_P2PMarketMenuListingController.RemainingTime = ExpansionStatic.GetTimeString(m_RemaningTime);
		m_P2PMarketMenuListingController.NotifyPropertyChanged("RemainingTime");
		
		m_P2PMarketMenuListingController.OwnerName = m_Listing.GetOwnerName();
		m_P2PMarketMenuListingController.NotifyPropertyChanged("OwnerName");
		
		string playerUID = GetGame().GetPlayer().GetIdentity().GetId();
		if (m_Listing.GetListingState() == ExpansionP2PMarketListingState.LISTED && m_Listing.GetOwnerUID() == playerUID)
		{
			SetIsOwnedItem();
		}
		else if (m_Listing.GetListingState() == ExpansionP2PMarketListingState.SOLD && m_Listing.GetOwnerUID() == playerUID)
		{
			SetIsSoldItem();
		}
		
	#ifdef EXPANSIONMODHARDLINE
		if (GetExpansionSettings().GetHardline().EnableItemRarity)
			SetRarityColor(m_Listing.GetRarity());
	#endif
	}

	protected void SetIsOwnedItem()
	{
		m_IsOwnedItem = true;
		discount_text.SetColor(ARGB(255, 26, 188, 156));
		price_text.SetColor(ARGB(255, 255, 255, 255));
		price_icon.SetColor(ARGB(255, 255, 255, 255));

		int discountPrice = ExpansionP2PMarketModule.GetDiscountPrice(m_Listing.GetPrice());
		int discountPercent = GetExpansionSettings().GetP2PMarket().ListingOwnerDiscountPercent;
		
		auto localiser = new CF_Localiser("STR_EXPANSION_MARKET_P2P_DISCOUNT", discountPercent.ToString());
		m_P2PMarketMenuListingController.Discount = localiser.Format();
		m_P2PMarketMenuListingController.NotifyPropertyChanged("Discount");

		m_P2PMarketMenuListingController.Price = ExpansionStatic.IntToCurrencyString(discountPrice, ",");
		m_P2PMarketMenuListingController.NotifyPropertyChanged("Price");
	}
	
	protected void SetIsSoldItem()
	{
		m_IsSoldItem = true;
		price_text.SetColor(ARGB(255, 26, 188, 156));
		price_icon.SetColor(ARGB(255, 26, 188, 156));
		sold_icon_panel.Show(true);
		quickbuy_panel.Show(false);
		time_panel.Show(false);
		name_panel.Show(false);
	}

	protected void SetRemainingTime()
	{
		int listingTime = m_Listing.GetListingTime();
		int currentTime = CF_Date.Now(true).GetTimestamp();
		int timedif = (currentTime - listingTime);

		int cooldownTime = GetExpansionSettings().GetP2PMarket().MaxListingTime;
		m_RemaningTime = cooldownTime - timedif;
	}
	
	void OnQuickBuyClick()
	{
		m_P2PMarketMenu.QuickbuyListing(this);
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_listing.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuListingController;
	}
	
	override void UpdateFoodStage()
	{
		Edible_Base food_item = Edible_Base.Cast(m_Object);
		if (food_item && food_item.HasFoodStage())
		{
			FoodStage foodStage = food_item.GetFoodStage();
			foodStage.ChangeFoodStage(m_Listing.GetFoodStageType());

			int listingTime = m_Listing.GetListingTime();
			int currentTime = CF_Date.Now(true).GetTimestamp();
			int elapsed = (currentTime - listingTime);

			EXPrint(ToString() + "::UpdatePreviewObject - Time elapsed: " + ExpansionStatic.GetTimeString(elapsed));
			FoodStageType processedFoodStage = food_item.Expansion_GetProcessedFoodStageDecay(elapsed, false);
			EXPrint(ToString() + "::UpdatePreviewObject - Processed food stage: " + typename.EnumToString(FoodStageType, processedFoodStage));
			foodStage = food_item.GetFoodStage();
			foodStage.ChangeFoodStage(processedFoodStage);
			
			m_Listing.SetFoodStageType(processedFoodStage);
		}
	}

	void OnItemButtonClick(ButtonCommandArgs args)
	{
		int button = args.GetMouseButton();
		bool buttonState = args.GetButtonState();
		
		if (button == MouseState.LEFT && buttonState)
		{
			if (!m_IsSoldItem)
			{
				m_P2PMarketMenu.SetSelectedListingItem(this);
			}
			else
			{
				m_P2PMarketMenu.GetSaleFromListing(m_Listing);
			}
		}
	}

	ExpansionP2PMarketListing GetListing()
	{
		return m_Listing;
	}

	bool IsOwnerdItem()
	{
		return m_IsOwnedItem;
	}

	int GetRemainingTime()
	{
		return m_RemaningTime;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && w == item_button)
		{
			if (button == MouseState.MIDDLE && m_Object && m_Listing)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_Listing.GetRarity();
#endif
				MissionGameplay.InspectItem(m_P2PMarketMenu, m_Object, m_Listing.GetHealthLevel(), m_Listing.GetLiquidType(), m_Listing.IsBloodContainer(), m_Listing.GetQuantityType(), m_Listing.GetQuantity(), m_Object.GetQuantityMax(), m_Listing.GetFoodStageType(), m_Listing.GetClassName(), rarity);
				return true;
			}
		}
		
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && w == item_button)
		{
			if (!m_ItemTooltip && m_Object && m_Listing)
			{
				int rarity = -1;
#ifdef EXPANSIONMODHARDLINE
				rarity = m_Listing.GetRarity();
#endif
				m_ItemTooltip = MissionGameplay.SetItemTooltip(m_Object, m_Listing.GetHealthLevel(), m_Listing.GetLiquidType(), m_Listing.IsBloodContainer(), m_Listing.GetQuantityType(), m_Listing.GetQuantity(), m_Object.GetQuantityMax(), m_Listing.GetFoodStageType(), m_Listing.GetClassName(), rarity);
			}

			item_name_text.SetColor(ARGB(255, 0, 0, 0));
			time_text.SetColor(ARGB(255, 0, 0, 0));
			discount_text.SetColor(ARGB(255, 0, 0, 0));
			price_text.SetColor(ARGB(255, 0, 0, 0));
			price_icon.SetColor(ARGB(255, 0, 0, 0));
			name_text.SetColor(ARGB(255, 0, 0, 0));
			
			if (m_IsSoldItem)
			{
				sold_icon.SetColor(ARGB(255, 0, 0, 0));
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL && w == item_button)
		{
			if (m_ItemTooltip)
			{
				MissionGameplay.Expansion_DestroyItemTooltip();
				m_ItemTooltip = null;
			}

		#ifdef EXPANSIONMODHARDLINE
			if (m_RarityColor != -1)
			{
				item_name_text.SetColor(m_RarityColor);
			}
			else
			{
				item_name_text.SetColor(ARGB(255, 255, 255, 255));
			}
		#else
			item_name_text.SetColor(ARGB(255, 255, 255, 255));
		#endif
			
			if (!m_IsOwnedItem && !m_IsSoldItem)
			{
				price_text.SetColor(ARGB(255, 255, 255, 255));
				price_icon.SetColor(ARGB(255, 255, 255, 255));
			}
			else if (m_IsOwnedItem && !m_IsSoldItem)
			{
				price_text.SetColor(ARGB(255, 255, 255, 255));
				price_icon.SetColor(ARGB(255, 255, 255, 255));
				discount_text.SetColor(ARGB(255, 26, 188, 156));
			}
			else if (m_IsSoldItem)
			{
				price_text.SetColor(ARGB(255, 26, 188, 156));
				price_icon.SetColor(ARGB(255, 26, 188, 156));
				sold_icon.SetColor(ARGB(255, 226, 65, 66));
			}

			time_text.SetColor(ARGB(255, 255, 255, 255));
			name_text.SetColor(ARGB(255, 255, 255, 255));
		}

		return false;
	}
};

class ExpansionP2PMarketMenuListingController: ExpansionP2PMarketMenuItemBaseController
{
	string Discount;
	string Price;
	string RemainingTime;
	string OwnerName;
};