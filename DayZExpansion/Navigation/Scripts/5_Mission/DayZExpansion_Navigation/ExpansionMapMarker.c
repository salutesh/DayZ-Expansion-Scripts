/**
 * ExpansionMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionMapMarker : ExpansionMapWidgetBase
{
	private ref ExpansionMarkerData m_Data;

	protected WrapSpacerWidget m_IconEntries;
	protected ref array<ref ExpansionMapMarkerIconItem> m_IconTypesArray;
	
	protected SliderWidget m_ColorRGB_Red;
	protected SliderWidget m_ColorRGB_Green;
	protected SliderWidget m_ColorRGB_Blue;

	protected ButtonWidget m_LeftButton;
	protected ButtonWidget m_RightButton;

	protected Widget m_State3DContainer;
	protected Widget m_StatePartyContainer;

	protected CheckBoxWidget m_State3DCheckbox;
	protected CheckBoxWidget m_StatePartyCheckbox;
	protected CheckBoxWidget m_StateDragLockCheckbox;

	protected Widget m_EditPanel;
	
	private bool m_Creating;
	private int m_CreationPosX;
	private int m_CreationPosY;

	protected ExpansionMapMenu m_MapMenu;
	private ExpansionMarkerModule m_MarkerModule;

	protected ref ExpansionUITooltip m_PositionToolTip;

	void ExpansionMapMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		m_IconTypesArray = new array<ref ExpansionMapMarkerIconItem>;
		Class.CastTo(m_MarkerModule, GetModuleManager().GetModule(ExpansionMarkerModule));
	}

	void ~ExpansionMapMarker()
	{
		delete m_IconTypesArray;
		delete m_PositionToolTip;
	}

	protected override void OnInit(Widget layoutRoot)
	{
		super.OnInit(layoutRoot);

		m_HoverColor = ARGB(255, 255, 255, 255);
	}

	protected void OnEditInit(Widget layoutRoot)
	{
		Class.CastTo(m_IconEntries, layoutRoot.FindAnyWidget("marker_icons_content"));
		Class.CastTo(m_LeftButton, layoutRoot.FindAnyWidget("marker_button_left"));
		Class.CastTo(m_RightButton, layoutRoot.FindAnyWidget("marker_button_right"));

		Class.CastTo(m_ColorRGB_Red, layoutRoot.FindAnyWidget("marker_color_picker_rgb_red_slider"));
		Class.CastTo(m_ColorRGB_Green, layoutRoot.FindAnyWidget("marker_color_picker_rgb_green_slider"));
		Class.CastTo(m_ColorRGB_Blue, layoutRoot.FindAnyWidget("marker_color_picker_rgb_blue_slider"));

		Class.CastTo(m_State3DContainer, layoutRoot.FindAnyWidget("marker_state_3d_container"));
		Class.CastTo(m_StatePartyContainer, layoutRoot.FindAnyWidget("marker_state_party_container"));

		Class.CastTo(m_State3DCheckbox, layoutRoot.FindAnyWidget("marker_state_3d_checkbox"));
		Class.CastTo(m_StatePartyCheckbox, layoutRoot.FindAnyWidget("marker_state_party_checkbox"));
		Class.CastTo(m_StateDragLockCheckbox, layoutRoot.FindAnyWidget("marker_state_lock_checkbox"));

		array<ExpansionIcon> iconsSorted = ExpansionIcons.Sorted();
		foreach (ExpansionIcon icon: iconsSorted)
		{
			m_IconTypesArray.Insert(new ExpansionMapMarkerIconItem(m_IconEntries, icon, this));
		}
	}

	void SetMapMenu(ExpansionMapMenu menu)
	{
		m_MapMenu = menu;
	}

	string GetEditLayoutPath()
	{
		return "DayZExpansion/Navigation/GUI/layouts/expansion_map_marker_edit.layout";
	}

	bool CanEditName()
	{
		return true;
	}

	bool CanEdit()
	{
		return true;
	}

	bool IsEditting()
	{
		return m_EditPanel && m_EditPanel.IsVisible();
	}

	void SetCreation(bool creating)
	{
		m_Creating = creating;
	}

	bool IsCreating()
	{
		return m_Creating;
	}

	bool IsRGBSlider(Widget w)
	{
		return w == m_ColorRGB_Red || w == m_ColorRGB_Green || w == m_ColorRGB_Blue;
	}

	void SetMarkerData(ExpansionMarkerData data)
	{
		m_Data = data;

		if (m_Data)
		{
			m_Data.SetHandler(this);
			if (HasBeenInitialized())
				SetFromMarkerData();
		}
	}

	void SetFromMarkerData()
	{
		if (m_Data && !IsEditting())
		{
			if (!IsDragging())
				SetPosition(m_Data.GetPosition());
			else
				m_Data.SetPosition(GetPosition());
			SetName(m_Data.GetName());
			SetIcon(m_Data.GetIconName());
			SetPrimaryColor(m_Data.GetColor());
			GetLayoutRoot().Show(!ShouldHide());
		}
	}

	ExpansionMarkerData GetMarkerData()
	{
		return m_Data;
	}

	override void SetIcon(ExpansionIcon icon)
	{
		super.SetIcon(icon);

		if (!m_CurrentExpansionIcon)
			return;

		for (int i = 0; i < m_IconTypesArray.Count(); ++i)
		{
			m_IconTypesArray[i].UpdateSelection(m_CurrentExpansionIcon);
		}
	}
	
	override void SetIcon(string icon)
	{
		super.SetIcon(icon);
		
		if (m_CurrentIcon == string.Empty)
			return;

		for (int i = 0; i < m_IconTypesArray.Count(); ++i)
		{
			m_IconTypesArray[i].UpdateSelection(m_CurrentIcon);
		}
	}

	override void SetPrimaryColor(int color)
	{
		super.SetPrimaryColor(color);

		if (m_ColorRGB_Red && m_ColorRGB_Green && m_ColorRGB_Blue)
		{
			int a = (color >> 24) & 0xFF;
			int r = (color >> 16) & 0xFF;
			int g = (color >> 8) & 0xFF;
			int b = (color)&0xFF;

			m_ColorRGB_Red.SetCurrent(r);
			m_ColorRGB_Green.SetCurrent(g);
			m_ColorRGB_Blue.SetCurrent(b);
		}

		for (int i = 0; i < m_IconTypesArray.Count(); ++i)
		{
			m_IconTypesArray[i].SetColor(m_PrimaryColor);
		}
	}

	private bool ShouldHide()
	{
		if (!m_Data.IsMapVisible())
			return true;

		if (!m_MarkerModule.IsMapVisible(m_Data.GetType()))
			return true;

		return false;
	}

	override void Update(float pDt)
	{
		super.Update(pDt);

		if (m_Data)
		{
			if (ShouldHide())
			{
				GetLayoutRoot().Show(false);

				if (IsEditting())
					CloseEditPanel();
			}
			else
			{
				GetLayoutRoot().Show(true);

				if (!IsDragging() && !IsEditting())
					SetPosition(m_Data.GetPosition());
			}

			if (m_MouseHover && KeyState(KeyCode.KC_LSHIFT))
			{
				vector pos = GetPosition();
				m_PositionToolTip = new ExpansionUITooltip(string.Format("<p>X: %1 Y: %2 Z: %3</p>", pos[0], pos[1], pos[2]), "<p>" + GetName() + "<p>");
				m_PositionToolTip.SetTextPos("center");
				int x, y;
				GetMousePos(x, y);
				m_PositionToolTip.SetPos(x, y);
				m_PositionToolTip.ShowTooltip();
			}
			else
			{
				delete m_PositionToolTip;
			}
		}
		else
		{
			GetLayoutRoot().Show(IsCreating());
		}

		if (IsCreating() || IsEditting())
		{
			//! Center edit window on screen
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float w, h;
			m_EditPanel.GetSize(w, h);
			m_LayoutRoot.SetPos(screen_w / 2 - w / 2 + 25, screen_h / 2 - 75);
		}
	}

	override void SetPosition(int x, int y, bool performDropEvent = false)
	{
		if (IsCreating())
		{
			m_CreationPosX = x;
			m_CreationPosY = y;
		}
			
		super.SetPosition(x, y, performDropEvent);
	}

	override void SetPosition(vector position, bool performDropEvent = false)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]) + 1.0;

		super.SetPosition(position, performDropEvent);
	}

	bool IsDragLocked()
	{
		return m_Data && m_Data.GetLockState() && !ExpansionStatic.Key_SHIFT();
	}

	override void OnDrop()
	{
		super.OnDrop();

		if (IsDragLocked() && !IsDragging())
			return;

		if (IsCreating())
			return;

		if (!m_Data)
			return;

		int type = m_Data.GetType();
		switch (type)
		{
		case ExpansionMapMarkerType.PERSONAL:
			m_Data.SetPosition(GetPosition());
			break;
#ifdef EXPANSIONMODGROUPS
		case ExpansionMapMarkerType.PARTY:
			ExpansionPartyModule party;
			if (Class.CastTo(party, GetModuleManager().GetModule(ExpansionPartyModule)))
				party.UpdatePositionMarker(m_Data.GetUID(), GetPosition());
			break;
#endif
		default:
			Error("Marker type " + typename.EnumToString(ExpansionMapMarkerType, type) + " not supported.");
			return;
		}
	}

	void ShowEditPanel()
	{
		if (!CanEdit() || !m_MapMenu)
			return;

		if (!m_Data)
		{
			SetName("New Marker");
		}

		if (!IsCreating() && !m_MapMenu.SetSelectedMarker(m_Data))
			return;

		if (!m_EditPanel)
		{
			m_EditPanel = GetGame().GetWorkspace().CreateWidgets(GetEditLayoutPath(), m_LayoutRoot.FindAnyWidget("marker_frame"));
			m_EditPanel.Show(false);

			OnEditInit(m_EditPanel);
		}

		SetFromMarkerData();
		m_Frame.SetColor(ARGB(220, 0, 0, 0));
		m_EditPanel.Show(true);

		if (CanEditName())
		{
			m_EditName.Show(true);
			m_EditName.SetText("");
			m_Name.Show(false);
		}

		if (IsCreating())
		{
			bool canCreateParty = false;

#ifdef EXPANSIONMODGROUPS
			ExpansionPartyModule party;
			if (Class.CastTo(party, GetModuleManager().GetModule(ExpansionPartyModule)))
				canCreateParty = party.HasParty() && GetExpansionSettings().GetParty().CanCreatePartyMarkers && GetExpansionSettings().GetParty().EnableParties;
#endif

			m_StatePartyContainer.Show(canCreateParty);
			m_State3DContainer.Show(GetExpansionSettings().GetMap().CanCreate3DMarker);
			m_State3DCheckbox.Show(GetExpansionSettings().GetMap().CanCreate3DMarker);
			m_State3DCheckbox.SetChecked(false);
			m_StateDragLockCheckbox.SetChecked(GetExpansionClientSettings().DefaultMarkerLockState);

			m_LeftButton.SetText("#STR_EXPANSION_MAP_MARKER_CANCEL_BUTTON_LABLE");
			m_RightButton.SetText("#STR_EXPANSION_MAP_MARKER_CREATE_BUTTON_LABLE");
		}
		else
		{
			m_StatePartyContainer.Show(false);

			if (!m_Data.Is3D())
			{
				m_State3DContainer.Show(GetExpansionSettings().GetMap().CanCreate3DMarker);
				m_State3DCheckbox.Show(GetExpansionSettings().GetMap().CanCreate3DMarker);
			}

			m_State3DCheckbox.SetChecked(m_Data.Is3D());
			m_StateDragLockCheckbox.SetChecked(m_Data.GetLockState());

			m_EditName.SetText(m_Data.GetName());
			m_LeftButton.SetText("#STR_EXPANSION_MAP_MARKER_DELETE_SHORT_BUTTON_LABLE");
			m_RightButton.SetText("#STR_EXPANSION_MAP_MARKER_UPDATE_TITLE");
		}

		ExpansionMapMenu mapmenu = ExpansionMapMenu.Cast(GetGame().GetUIManager().GetMenu());
		if (mapmenu)
		{
			mapmenu.SetIsEditingMarker(true);
		}
	}

	void CloseEditPanel()
	{
		m_Frame.SetColor(ARGB(0, 0, 0, 0));
		m_EditPanel.Show(false);
		m_EditName.Show(false);
		m_Name.Show(true);

		ExpansionMapMenu mapmenu = ExpansionMapMenu.Cast(GetGame().GetUIManager().GetMenu());
		if (mapmenu)
		{
			mapmenu.SetIsEditingMarker(false);
		}
	}

	void OnCancleButtonClick()
	{
		if (CanEdit())
		{
			CloseEditPanel();

			SetFromMarkerData();

			ExpansionMapMenu menu;
			if (Class.CastTo(menu, GetGame().GetUIManager().GetMenu()))
			{
				if (IsCreating())
				{
					menu.DeletePreviewMarker();
				}
				else
				{
					menu.DeleteSelectedMarker();
				}
			}
		}
	}

	void OnCreateButtonClick()
	{
		if (CanEdit())
		{
			string name = m_MarkerName;

			if (CanEditName())
			{
				name = m_EditName.GetText();
			}

			CloseEditPanel();

			if (IsCreating())
			{
				if (m_StatePartyCheckbox.IsChecked())
				{
					m_Data = ExpansionMarkerData.Create(ExpansionMapMarkerType.PARTY);
				}
				else
				{
					m_Data = ExpansionMarkerData.Create(ExpansionMapMarkerType.PERSONAL);
				}

				SetPosition(m_CreationPosX, m_CreationPosY);
			}

			m_Data.SetName(name);
			
			if (m_CurrentExpansionIcon)
				m_Data.SetIcon(m_CurrentExpansionIcon);
			else if (m_CurrentIcon != string.Empty)
				m_Data.SetIcon(m_CurrentIcon);
			
			m_Data.SetColor(m_PrimaryColor);
			m_Data.SetPosition(GetPosition());
			m_Data.Set3D(m_State3DCheckbox.IsChecked());
			m_Data.SetLockState(m_StateDragLockCheckbox.IsChecked());

			ExpansionMapMenu menu;
			if (Class.CastTo(menu, GetGame().GetUIManager().GetMenu()))
			{
				if (IsCreating())
				{
					menu.CompleteCreationNewMarker();
				}
				else
				{
					menu.UpdateSelectedMarker();
				}
			}

			SetFromMarkerData();

			m_Creating = false;
		}
	}

	override void OnMarkerClick()
	{
		ShowEditPanel();
	}

	void FocusOnMarker(bool shouldEdit = false)
	{
		GetMapWidget().SetScale(0.05);
		GetMapWidget().SetMapPos(GetPosition());

		if (m_Data)
		{
			if (m_Data.GetType() == ExpansionMapMarkerType.PARTY || m_Data.GetType() == ExpansionMapMarkerType.PERSONAL)
			{
				if (shouldEdit)
				{
					ShowEditPanel();
				}
			}
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL)
		{
			if (w == m_LeftButton)
			{
				OnCancleButtonClick();

				return true;
			}

			if (w == m_RightButton)
			{
				OnCreateButtonClick();

				return true;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w != NULL && IsRGBSlider(w))
		{
			SetPrimaryColor(ARGB(255, m_ColorRGB_Red.GetCurrent(), m_ColorRGB_Green.GetCurrent(), m_ColorRGB_Blue.GetCurrent()));

			return true;
		}

		return super.OnChange(w, x, y, finished);
	}

	override bool CanDrag()
	{
		return !IsDragLocked() && !IsEditting();
	}
};
