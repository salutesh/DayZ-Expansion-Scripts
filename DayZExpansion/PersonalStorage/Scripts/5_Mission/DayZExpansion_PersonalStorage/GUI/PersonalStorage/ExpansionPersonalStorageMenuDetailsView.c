/**
 * ExpansionPersonalStorageMenuDetailsView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuDetailsView: ExpansionScriptView
{
	protected ref ExpansionPersonalStorageMenuDetailsViewController m_PersonalStorageMenuDetailsController;
	protected ref ExpansionPersonalStorageMenu m_PersonalStorageMenu;

	protected ImageWidget health_image;
	protected Widget item_quantity;
	protected Widget quantity_bar_panel;
	protected Widget quantity_color;
	protected ProgressBarWidget quantity_bar;
	protected TextWidget quantity_value;
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

	protected RichTextWidget item_description;

	void ExpansionPersonalStorageMenuDetailsView(ExpansionPersonalStorageMenu menu)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		Class.CastTo(m_PersonalStorageMenuDetailsController, GetController());

		m_PersonalStorageMenu = menu;
	}

	void ~ExpansionPersonalStorageMenuDetailsView()
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		if (m_PlayerPreview)
			m_PlayerPreview = null;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_menu_details_view.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuDetailsViewController;
	}

	private bool UsePlayerPreview()
	{
		if (m_PersonalStorageMenu.GetSelectedPreviewObject())
			return m_PersonalStorageMenu.GetSelectedPreviewObject().IsInherited(Clothing_Base);
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

	void SpawnAttachments(array<ref ExpansionPersonalStorageContainerItem> attachments, EntityAI parent, int skinIndex = 0)
	{
		foreach (ExpansionPersonalStorageContainerItem attachment: attachments)
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
		if (!m_PersonalStorageMenu.GetSelectedPreviewObject())
			return;

		string previewClassName = GetPreviewClassName(m_PersonalStorageMenu.GetSelectedPreviewObject().ClassName());

		if (!UsePlayerPreview())
		{
			view_item_preview.Show(true);
			view_player_preview.Show(false);
			m_PersonalStorageMenuDetailsController.ViewItemPreview = m_PersonalStorageMenu.GetSelectedPreviewObject();
			m_PersonalStorageMenuDetailsController.NotifyPropertyChanged("ViewItemPreview");

			m_CurrentPreviewObject = m_PersonalStorageMenu.GetSelectedPreviewObject();
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
		m_PersonalStorageMenuDetailsController.ViewPlayerPreview = previewObject;
		m_PersonalStorageMenuDetailsController.NotifyPropertyChanged("ViewPlayerPreview");
	}

	override void SpawnPlayerPreviewAttachments(EntityAI item)
	{
		if (m_PersonalStorageMenu.GetSelectedContainerItems().Count() > 0)
		{
			int skinIndex;
			ItemBase itemIB;
			if (Class.CastTo(itemIB, m_PersonalStorageMenu.GetSelectedPreviewObject()))
				skinIndex = itemIB.ExpansionGetCurrentSkinIndex();

			SpawnAttachments(m_PersonalStorageMenu.GetSelectedContainerItems(), item);
		}
	}

	override void SetCurrentPreviewObject(Object obj)
	{
		m_CurrentPreviewObject = obj;
	}

	ExpansionPersonalStorageMenuDetailsViewController GetDetailsViewController()
	{
		return m_PersonalStorageMenuDetailsController;
	}

	void OnBackClick()
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		m_PersonalStorageMenu.OnBackClick();
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
		m_PersonalStorageMenuDetailsController.AttachmentItems.Clear();
	}

	void AddAttachmentEntry(ExpansionPersonalStorageMenuCargoItem attachment)
	{
		m_PersonalStorageMenuDetailsController.AttachmentItems.Insert(attachment);
	}

	void ClearCargo()
	{
		m_PersonalStorageMenuDetailsController.CargoItems.Clear();
	}

	void AddCargoEntry(ExpansionPersonalStorageMenuCargoItem cargo)
	{
		m_PersonalStorageMenuDetailsController.CargoItems.Insert(cargo);
	}

	void SetViewDeposit()
	{
		item_description.Show(true);
	}

	void SetViewItem()
	{
		item_description.Show(true);
	}

	void OnConfirmButtonClick()
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		m_PersonalStorageMenu.OnConfirmButtonClick();
	}

	ProgressBarWidget GetQuantityBarWidget()
	{
		return quantity_bar;
	}

	ImageWidget GetHealthImageWidget()
	{
		return health_image;
	}

	RichTextWidget GetItemDescriptionWidget()
	{
		return item_description;
	}

	override void OnShow()
	{
		super.OnShow();
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

class ExpansionPersonalStorageMenuDetailsViewController: ExpansionViewController
{
	Object ViewItemPreview;
	Object ViewPlayerPreview;
	string SelectedName;
	string ItemDescription;
	string ItemQuantity;
	string ItemBarQuantity;
	string QuantityText;
	string ItemCount;
	string CargoItemsCount;
	string AttachmentItemsCount;
	string ItemHealth;
	string LiquidType;
	string FoodState;
	string Rarity;
	string ConfirmButtonText;
	ref ObservableCollection<ref ExpansionPersonalStorageMenuCargoItem> AttachmentItems = new ObservableCollection<ref ExpansionPersonalStorageMenuCargoItem>(this);
	ref ObservableCollection<ref ExpansionPersonalStorageMenuCargoItem> CargoItems = new ObservableCollection<ref ExpansionPersonalStorageMenuCargoItem>(this);
};