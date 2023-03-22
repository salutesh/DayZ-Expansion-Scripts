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
	protected static float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MIN = 1.5;
	protected static float POSITION_PLAYER_PREVIEW_W_THRESHOLD_MAX = 4;
	protected static float POSITION_PLAYER_PREVIEW_Y_OFFSET = -0.125;
	protected static float POSITION_PLAYER_PREVIEW_ANIMATION_DURATION = 0.5;

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
	PlayerBase m_PlayerPreview;
	protected int m_SelectedItemSlot;
	protected float m_LastPositionPlayerPreviewW;
	protected float m_LastPositionPlayerPreviewY;
	protected int m_PlayerPosCurrentAnimID;
	protected bool m_PlayerPosAnimRunning;
	protected vector m_CharacterOrientation;
	protected int m_CharacterRotationX;
	protected int m_CharacterRotationY;
	protected int m_CharacterScaleDelta;

	protected RichTextWidget item_description;

	void ExpansionPersonalStorageMenuDetailsView(ExpansionPersonalStorageMenu menu)
	{
		Class.CastTo(m_PersonalStorageMenuDetailsController, GetController());

		m_PersonalStorageMenu = menu;
	}

	void ~ExpansionPersonalStorageMenuDetailsView()
	{
		if (m_PlayerPreview)
			GetGame().ObjectDelete(m_PlayerPreview);
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

	void UpdateScalePlayerPreview(int mouse_x, int mouse_y, bool is_dragging, int wheel)
	{
		float w, h, x, y;
		view_player_preview.GetPos(x, y);
		view_player_preview.GetSize(w,h);
		w = w + (m_CharacterScaleDelta / 8);
		h = h + (m_CharacterScaleDelta / 8);
		if ( w > 0.5 && w < 6 )
		{
			view_player_preview.SetSize(w, h);

			//! Align to center
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - (m_CharacterScaleDelta / 16);
			float new_y = y - (m_CharacterScaleDelta / 16);
			view_player_preview.SetPos(new_x, new_y);

			UpdateLastPositionPlayerPreview(w);
			if (!m_PlayerPosAnimRunning)
				UpdatePositionPlayerPreview(m_LastPositionPlayerPreviewY);
		}
	}

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

	void UpdatePositionPlayerPreview(float y)
	{
		view_player_preview.SetModelPosition(Vector(0, y, 0));
	}

	void UpdateLastPositionPlayerPreview(float w)
	{
		m_LastPositionPlayerPreviewW = w;
		if (!m_PlayerPosAnimRunning)
			m_LastPositionPlayerPreviewY = GetPositionPlayerPreviewInterpolatedY(w, GetPositionPlayerPreviewTargetY());
	}

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

	void UpdateRotationPlayerPreview(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_CharacterOrientation;
		o[2] = o[2] + (m_CharacterRotationY - mouse_y);
		o[1] = o[1] - (m_CharacterRotationX - mouse_x);

		view_player_preview.SetModelOrientation(o);

		if (!is_dragging)
		{
			m_CharacterOrientation = o;
		}
	}

	void ResetPreview()
	{
		m_CurrentPreviewObject = NULL;
		m_PlayerPreview = NULL;
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

				m_PersonalStorageMenuDetailsController.ViewPlayerPreview = m_PlayerPreview;
				m_PersonalStorageMenuDetailsController.NotifyPropertyChanged("ViewPlayerPreview");
			}
			else
			{
				//! Remove existing attachments on player preview
				array<EntityAI> previewAttachments = new array<EntityAI>;
				for (i = 0; i < m_PlayerPreview.GetInventory().AttachmentCount(); i++)
				{
					EntityAI previewItem = m_PlayerPreview.GetInventory().GetAttachmentFromIndex(i);
					//! Removing the attachment here would change attachment count, so add it to a temp array for later removal
					if (previewItem)
						previewAttachments.Insert(previewItem);
				}

				foreach (EntityAI previewAttachment: previewAttachments)
				{
					if (previewAttachment)
						m_PlayerPreview.GetInventory().LocalDestroyEntity(previewAttachment);
				}
			}

			//! Add selected item first to override any current player attachment on same slot
			EntityAI item = m_PlayerPreview.GetInventory().CreateAttachment(previewClassName);
			if (m_PersonalStorageMenu.GetSelectedContainerItems().Count() > 0)
			{
				int skinIndex;
				ItemBase itemIB;
				if (Class.CastTo(itemIB, m_PersonalStorageMenu.GetSelectedPreviewObject()))
					skinIndex = itemIB.ExpansionGetCurrentSkinIndex();

				SpawnAttachments(m_PersonalStorageMenu.GetSelectedContainerItems(), item);
			}

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
				m_LastPositionPlayerPreviewY = view_player_preview.GetModelPosition()[1];

			AnimatePositionPlayerPreview();
		}
	}

	ExpansionPersonalStorageMenuDetailsViewController GetDetailsViewController()
	{
		return m_PersonalStorageMenuDetailsController;
	}

	void OnBackClick()
	{
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
			GetGame().GetDragQueue().Call(this, "UpdateScalePlayerPreview");
			m_CharacterScaleDelta = wheel ;
		}

		return false;
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (w == view_item_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
			return true;
		}
		else if (w == view_player_preview)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotationPlayerPreview");
			g_Game.GetMousePos(m_CharacterRotationX, m_CharacterRotationY);
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

			view_player_preview.SetModelPosition(Vector(0,0,0.5));
			view_player_preview.SetModelOrientation(m_CharacterOrientation);

			view_player_preview.GetPos(itemx, itemy);

			view_player_preview.SetSize(1.5, 1.5);

			// align to center
			view_player_preview.SetPos(-0.225, -0.225);
		}

		return false;
	}

	override void OnHide()
	{
		super.OnHide();

		if (m_PlayerPreview)
			GetGame().ObjectDelete(m_PlayerPreview);
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