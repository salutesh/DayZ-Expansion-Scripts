/**
 * ExpansionP2PMarketMenuDetailsView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuDetailsView: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuDetailsViewController m_P2PMarketMenuDetailsController;
	protected ref ExpansionP2PMarketMenu m_P2PMarketMenu;
	protected ref ExpansionP2PMarketSettings m_P2PMarketSettings;
	
	protected ref ExpansionMarketMenuTooltip m_MarketPriceTooltip;

	protected ImageWidget health_image;
	protected Widget item_quantity;
	protected Widget quantity_bar_panel;
	protected ProgressBarWidget quantity_bar;
	protected TextWidget quantity_value;
	protected Widget quantity_color;
	protected Widget item_liquid;
	protected Widget liquid_color;
	protected Widget item_cargo_count;
	protected Widget item_attachments_count;
	protected Widget item_foodstate;
	protected Widget foodstate_color;
#ifdef EXPANSIONMODHARDLINE
	protected Widget item_rarity;
	protected Widget rarity_color;
	protected TextWidget rarity_value;
#endif

	protected string m_ListPriceString;
	protected int m_ListCost;

	protected EditBoxWidget listing_price_editbox;
	protected TextWidget price_text;
	protected Widget listing_cost_panel;
	
	protected Widget listing_lowest_panel;
	protected Widget listing_highest_panel;
	protected Widget listing_market_panel;

	protected ButtonWidget back_button;
	protected TextWidget back_button_text;

	protected GridSpacerWidget confirm_button_panel;
	protected ButtonWidget confirm_button;
	protected TextWidget confirm_button_text;

	protected ItemPreviewWidget view_item_preview;
	protected PlayerPreviewWidget view_player_preview;

	Object m_CurrentPreviewObject;
	ref ExpansionPlayerPreview m_PlayerPreview;
	protected vector m_CharacterOrientation;
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;

	protected TextWidget info_title;
	protected GridSpacerWidget info_grid;
	protected RichTextWidget item_description;

	void ExpansionP2PMarketMenuDetailsView(ExpansionP2PMarketMenu menu)
	{
		Class.CastTo(m_P2PMarketMenuDetailsController, GetController());

		m_P2PMarketMenu = menu;
		m_P2PMarketSettings = GetExpansionSettings().GetP2PMarket();
		
		if (GetExpansionSettings().GetMarket().MarketSystemEnabled)
		{
			m_MarketPriceTooltip = new ExpansionMarketMenuTooltip();
			m_MarketPriceTooltip.SetContentOffset(-0.234375, 0.006944);
			m_MarketPriceTooltip.AddEntry("#STR_EXPANSION_MARKET_P2P_MARKET_TOOLTIP_DESC");
		}
	}

	void ~ExpansionP2PMarketMenuDetailsView()
	{
		if (m_PlayerPreview)
			m_PlayerPreview = null;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_menu_details_view.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionP2PMarketMenuDetailsViewController;
	}

	private bool UsePlayerPreview()
	{
		if (m_P2PMarketMenu.GetSelectedPreviewObject())
			return m_P2PMarketMenu.GetSelectedPreviewObject().IsInherited(Clothing_Base);
		return false;
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

	void SpawnAttachments(array<ref ExpansionP2PMarketContainerItem> attachments, EntityAI parent, int skinIndex = 0)
	{
		foreach (ExpansionP2PMarketContainerItem attachment: attachments)
		{
			string attachmentName = attachment.GetClassName();
			ExpansionItemSpawnHelper.SpawnAttachment(attachmentName, parent, skinIndex);
		}
	}

	void UpdateScale(int mouse_x, int mouse_y, bool is_dragging, int wheel)
	{
		float w, h, x, y;
		view_item_preview.GetPos(x, y);
		view_item_preview.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 4);
		h = h + (m_CharacterScaleDelta / 4);
		if ( w > 0.5 && w < 4 )
		{
			view_item_preview.SetSize(w, h);

			// align to center
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 8);
			float new_y = y - (m_CharacterScaleDelta / 8);
			view_item_preview.SetPos(new_x, new_y);
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[2] = o[2] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);

		view_item_preview.SetModelOrientation(o);

		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}

	void ResetPreview()
	{
		m_CurrentPreviewObject = NULL;
	}
	
	void UpdatePreview()
	{
		if (!m_P2PMarketMenu.GetSelectedPreviewObject())
			return;

		string previewClassName = GetPreviewClassName(m_P2PMarketMenu.GetSelectedPreviewObject().ClassName());
		
		if (!UsePlayerPreview())
		{
			view_item_preview.Show(true);
			view_player_preview.Show(false);
			m_P2PMarketMenuDetailsController.ViewItemPreview = m_P2PMarketMenu.GetSelectedPreviewObject(;
			m_P2PMarketMenuDetailsController.NotifyPropertyChanged("ViewItemPreview");

			m_CurrentPreviewObject = m_P2PMarketMenu.GetSelectedPreviewObject();
		}
		else
		{
			view_item_preview.Show(false);
			view_player_preview.Show(true);

			if (!m_PlayerPreview)
				m_PlayerPreview = new ExpansionPlayerPreview(this, view_player_preview);

			m_PlayerPreview.Update(previewClassName);
		}
	}

	override void UpdatePlayerPreviewObject(Object previewObject)
	{
		m_P2PMarketMenuDetailsController.ViewPlayerPreview = previewObject;
		m_P2PMarketMenuDetailsController.NotifyPropertyChanged("ViewPlayerPreview");
	}

	override void SpawnPlayerPreviewAttachments(EntityAI item)
	{
		if (m_P2PMarketMenu.GetSelectedContainerItems().Count() > 0)
		{
			int skinIndex;
			ItemBase itemIB;
			if (Class.CastTo(itemIB, m_P2PMarketMenu.GetSelectedPreviewObject()))
				skinIndex = itemIB.ExpansionGetCurrentSkinIndex();

			SpawnAttachments(m_P2PMarketMenu.GetSelectedContainerItems(), item);
		}
	}

	override void SetCurrentPreviewObject(Object obj)
	{
		m_CurrentPreviewObject = obj;
	}

	ExpansionP2PMarketMenuDetailsViewController GetDetailsViewController()
	{
		return m_P2PMarketMenuDetailsController;
	}

	void OnBackClick()
	{
		m_P2PMarketMenu.OnBackClick();
	}

	string GetListPriceString()
	{
		return m_ListPriceString;
	}

	int GetListCost()
	{
		return m_ListCost;
	}

	void ShowItemQuantity(bool state)
	{
		item_quantity.Show(state);
		quantity_value.Show(state);
		
		if (state)
			quantity_value.SetColor(ARGB(255, 255, 255, 255));
	}
	
	void ShowQuantityBar(bool state)
	{
		item_quantity.Show(state);
		quantity_bar_panel.Show(state);
		quantity_bar.Show(state);
		quantity_value.Show(state);
		
		if (state)
			quantity_value.SetColor(ARGB(255, 0, 0, 0));
	}

	void ShowQuantityColor(bool state, int color = -1)
	{
		item_quantity.Show(state);
		quantity_bar_panel.Show(state);
		quantity_color.Show(state);
		quantity_value.Show(state);
		
		if (state)
		{
			quantity_value.SetColor(ARGB(255, 255, 255, 255));
			quantity_color.SetColor(color | 0x7F000000);
		}
	}

	void ShowItemLiquid(bool state)
	{
		item_liquid.Show(state);
	}
	
	void SetLiquidColor(int color)
	{
		liquid_color.SetColor(color | 0x7F000000);
	}

	void ShowItemCargoCount(bool state)
	{
		item_cargo_count.Show(state);
	}
	
	void ShowItemAttachmentsCount(bool state)
	{
		item_attachments_count.Show(state);
	}
	
	void ShowItemFoodState(bool state)
	{
		item_foodstate.Show(state);
	}
	
	void SetFoodStateColor(int color)
	{
		foodstate_color.SetColor(color | 0x7F000000);
	}
	
#ifdef EXPANSIONMODHARDLINE
	void ShowItemRarity(bool state)
	{
		item_rarity.Show(state);
	}
	
	void SetRarityColor(int color)
	{
		rarity_color.SetColor(color | 0x7F000000);
	}
	
	void SetRarityTextColor(int color)
	{
		rarity_value.SetColor(color | 0x7F000000);
	}
#endif
	
	void ShowConfirmButton(bool state)
	{
		confirm_button_panel.Show(state);
	}

	void ClearAttachments()
	{
		m_P2PMarketMenuDetailsController.AttachmentItems.Clear();
	}

	void AddAttachmentEntry(ExpansionP2PMarketMenuCargoItem attachment)
	{
		m_P2PMarketMenuDetailsController.AttachmentItems.Insert(attachment);
	}

	void ClearCargo()
	{
		m_P2PMarketMenuDetailsController.CargoItems.Clear();
	}

	void AddCargoEntry(ExpansionP2PMarketMenuCargoItem cargo)
	{
		m_P2PMarketMenuDetailsController.CargoItems.Insert(cargo);
	}

	void SetViewList()
	{
		price_text.Show(false);
		listing_price_editbox.Show(true);
		listing_cost_panel.Show(true);
		info_title.Show(true);
		info_grid.Show(true);
		item_description.Show(false);
		listing_lowest_panel.Show(true);
		listing_highest_panel.Show(true);
		if (GetExpansionSettings().GetMarket().MarketSystemEnabled)
			listing_market_panel.Show(true);
	}

	void SetViewListing()
	{
		price_text.Show(true);
		listing_price_editbox.Show(false);
		listing_cost_panel.Show(false);
		info_title.Show(false);
		info_grid.Show(false);
		item_description.Show(true);
		listing_lowest_panel.Show(false);
		listing_highest_panel.Show(false);
		listing_market_panel.Show(false);
	}
	
	void OnConfirmButtonClick()
	{
		m_P2PMarketMenu.OnConfirmButtonClick();
	}

	ProgressBarWidget GetQuantityBarWidget()
	{
		return quantity_bar;
	}

	EditBoxWidget GetEditBoxWidget()
	{
		return listing_price_editbox;
	}

	ImageWidget GetHealthImageWidget()
	{
		return health_image;
	}
	
	RichTextWidget GetItemDescriptionWidget()
	{
		return item_description;
	}
	
	ExpansionMarketMenuTooltip GetMarketPriceTooltip()
	{
		return m_MarketPriceTooltip;
	}

	override void OnShow()
	{
		super.OnShow();
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w != NULL && w == listing_price_editbox)
		{
			bool valid = true;
			string priceText = listing_price_editbox.GetText();
			TStringArray allNumbers = {"0","1","2","3","4","5","6","7","8","9"};
			for (int i = 0; i < priceText.Length(); i++)
			{
				if (allNumbers.Find(priceText.Get(i)) == -1)
					valid = false;
			}

			if (valid)
			{
				int price = priceText.ToInt();
				m_ListPriceString = priceText;
				m_ListCost = Math.Ceil(price * m_P2PMarketSettings.ListingPricePercent / 100);
				m_P2PMarketMenuDetailsController.ListCost = m_ListCost.ToString();
				m_P2PMarketMenuDetailsController.NotifyPropertyChanged("ListCost");
			}
		}

		return false;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == back_button)
			{
				back_button.SetColor(ARGB(255, 255, 255, 255));
				back_button_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == confirm_button)
			{
				confirm_button.SetColor(ARGB(255, 255, 255, 255));
				confirm_button_text.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
			else if (w == listing_market_panel)
			{
				if (m_MarketPriceTooltip)
					m_MarketPriceTooltip.Show();
				
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == back_button)
			{
				back_button.SetColor(ARGB(255, 0, 0, 0));
				back_button_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == confirm_button)
			{
				confirm_button.SetColor(ARGB(255, 0, 0, 0));
				confirm_button_text.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
			else if (w == listing_market_panel)
			{
				if (m_MarketPriceTooltip)
					m_MarketPriceTooltip.Hide();
				
				return true;
			}
		}

		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if (w == view_item_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_CharacterScaleDelta = wheel ;
		}
		else if (w == view_player_preview)
		{
			m_PlayerPreview.OnMouseWheel(w, x, y, wheel);
			m_CharacterScaleDelta = wheel ;
		}

		return false;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w == view_item_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}
		else if (w == view_player_preview)
		{
			m_PlayerPreview.OnMouseButtonDown(w, x, y, button);
			GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}

		return false;
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		float itemx, itemy;
		if (w == view_item_preview)
		{
			m_CharacterOrientation = vector.Zero;

			view_item_preview.SetModelPosition(Vector(0,0,0.5));
			view_item_preview.SetModelOrientation(m_CharacterOrientation);

			view_item_preview.GetPos(itemx, itemy);

			view_item_preview.SetSize(1.5, 1.5);

			// align to center
			view_item_preview.SetPos(-0.225, -0.225);
		}
		else if (w == view_player_preview)
		{
			m_CharacterOrientation = vector.Zero;

			m_PlayerPreview.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
		}

		return false;
	}

	override void OnHide()
	{
		super.OnHide();

		if (m_PlayerPreview)
			m_PlayerPreview = null;
	}
};

class ExpansionP2PMarketMenuDetailsViewController: ExpansionViewController
{
	Object ViewItemPreview;
	Object ViewPlayerPreview;
	string SelectedName;
	string PriceEditBox;
	string ListingPrice;
	string ListCost;
	string LowestPrice;
	string HighestPrice;
	string MarketPrice;
	string InfoTextOne;
	string InfoTextTwo;
	string InfoTextThree;
	string ItemDescription;
	string ItemQuantity;
	string QuantityText;
	string ItemCount;
	string CargoItemsCount;
	string AttachmentItemsCount;
	string ItemHealth;
	string LiquidType;
	string FoodState;
	string Rarity;
	string ConfirmButtonText;
	ref ObservableCollection<ref ExpansionP2PMarketMenuCargoItem> AttachmentItems = new ObservableCollection<ref ExpansionP2PMarketMenuCargoItem>(this);
	ref ObservableCollection<ref ExpansionP2PMarketMenuCargoItem> CargoItems = new ObservableCollection<ref ExpansionP2PMarketMenuCargoItem>(this);
};