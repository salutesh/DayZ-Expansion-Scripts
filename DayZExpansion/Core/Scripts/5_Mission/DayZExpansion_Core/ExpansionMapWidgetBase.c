/**
 * ExpansionMapWidgetBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapWidgetBase: ScriptedWidgetEventHandler
{
	reference string MapWidgetName;
	protected MapWidget m_MapWidget;
	protected Widget m_LayoutRoot;
	protected Widget m_Frame;
	protected TextWidget m_Name;
	protected EditBoxWidget m_EditName;
	protected string m_MarkerName;
	protected ImageWidget m_Icon;
	protected ButtonWidget m_EditButton;
	protected Widget m_DragWidget;
	
	protected ExpansionIcon m_CurrentExpansionIcon;
	protected string m_CurrentIcon;
	protected vector m_WorldPosition;
	protected bool m_Dragging;
	protected float m_DragOffsetX;
	protected float m_DragOffsetY;
	protected bool m_Initialized;
	protected int m_PrimaryColor;
	protected int m_HoverColor;
	protected bool m_MouseHover;

	void ExpansionMapWidgetBase(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		m_LayoutRoot = g_Game.GetWorkspace().CreateWidgets(GetLayoutPath(), parent);
		m_MapWidget = mapWidget;
		if (autoInit)
			OnInit(m_LayoutRoot);

		m_LayoutRoot.SetHandler(this);
	}

	void ~ExpansionMapWidgetBase()
	{
		if (m_LayoutRoot)
			m_LayoutRoot.Unlink();
	}

	bool HasBeenInitialized()
	{
		return m_Initialized;
	}

	void Init()
	{
		m_Initialized = true;
		OnInit(m_LayoutRoot);
	}

	Widget GetLayoutRoot()
	{
		return m_LayoutRoot;
	}

	MapWidget GetMapWidget()
	{
		return m_MapWidget;
	}

	string GetLayoutPath()
	{
		return "DayZExpansion/Core/GUI/layouts/map/expansion_map_marker.layout";
	}

	string GetDropSoundEffect()
	{
		return "Expansion_Draws_SoundSet";
	}

	bool IsEditButton(Widget w)
	{
		return w == m_EditButton;
	}

	bool IsDragWidget(Widget w)
	{
		return w == m_EditButton;
	}

	bool IsDragging()
	{
		return m_Dragging;
	}

	protected void OnInit(Widget layoutRoot)
	{
		Class.CastTo(m_Frame, layoutRoot.FindAnyWidget("marker_frame"));
		Class.CastTo(m_Name, layoutRoot.FindAnyWidget("marker_name"));
		Class.CastTo(m_EditName, layoutRoot.FindAnyWidget("marker_edit_name"));
		Class.CastTo(m_Icon, layoutRoot.FindAnyWidget("marker_icon"));
		Class.CastTo(m_EditButton, layoutRoot.FindAnyWidget("marker_button"));
		Class.CastTo(m_DragWidget, layoutRoot.FindAnyWidget("marker_icon_panel"));

		m_Frame.SetColor(ARGB(0, 0, 0, 0));
		m_PrimaryColor = ARGB(220, 220, 220, 220);
	}

	void Update(float pDt)
	{
		if (!m_Dragging)
		{
			vector position = m_MapWidget.MapToScreen(m_WorldPosition);
			m_LayoutRoot.SetPos(position[0], position[1], true);
		}
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w != NULL && IsEditButton(w) && button == MouseState.LEFT)
		{
			OnMarkerClick();
			return true;
		}
		return false;
	}

	override bool OnDrag(Widget w, int x, int y)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			m_LayoutRoot.GetPos(m_DragOffsetX, m_DragOffsetY);
			m_DragOffsetX = x - m_DragOffsetX;
			m_DragOffsetY = y - m_DragOffsetY;
			m_Dragging = true;
			return true;
		}

		return false;
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY);
			return true;
		}
		return false;
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (w != NULL && IsDragWidget(w) && CanDrag())
		{
			SetPosition(x - m_DragOffsetX, y - m_DragOffsetY, true);
			m_Dragging = false;
			return true;
		}
		return false;
	}

	void SetPosition(int x, int y, bool performDropEvent = false)
	{
		m_LayoutRoot.SetPos(x, y, true);
		vector newPos = m_MapWidget.ScreenToMap(Vector(x, y, 0));
		newPos[1] = g_Game.SurfaceY(newPos[0], newPos[2]) + 1.0;
		SetPosition(newPos, performDropEvent);
	}

	void SetPosition(vector position, bool performDropEvent = false)
	{
		m_WorldPosition = position;
		vector mPos = m_MapWidget.MapToScreen(m_WorldPosition);
		m_LayoutRoot.SetPos(mPos[0], mPos[1], true);
		if (performDropEvent)
		{
			OnDrop();
		}
	}

	vector GetPosition()
	{
		return m_WorldPosition;
	}

	void OnMarkerClick()
	{
	}

	void OnDrop()
	{
		if (IsMissionClient() && GetDropSoundEffect() != "")
		{
			SEffectManager.PlaySound(GetDropSoundEffect(), g_Game.GetPlayer().GetPosition());
		}
	}

	void Show()
	{
		Show(true);
	}

	void Show(bool show)
	{
		if (show)
		{
			m_Name.SetColor(m_PrimaryColor);
			m_Icon.SetColor(m_PrimaryColor);
		}
		m_LayoutRoot.Show(show);
	}

	void Hide()
	{
		Show(false);
	}

	void SetName(StringLocaliser name)
	{
		m_MarkerName = name.Format();
		m_Name.SetText(name.Format());
		m_EditName.SetText(name.Format());
	}

	void SetName(string name)
	{
		m_MarkerName = name;
		m_Name.SetText(name);
		m_EditName.SetText(name);
	}

	string GetName()
	{
		return m_MarkerName;
	}

	void SetIcon(ExpansionIcon icon)
	{
		m_CurrentExpansionIcon = icon;
		if (!m_CurrentExpansionIcon)
			return;

		m_Icon.LoadImageFile(0, m_CurrentExpansionIcon.IconPath);
	}

	void SetIcon(string icon)
	{
		ExpansionIcon expIcon = ExpansionIcons.Get(icon);
		if (expIcon)
		{
			SetIcon(expIcon);
		}
		else
		{
			m_CurrentIcon = icon;
			m_Icon.LoadImageFile(0, m_CurrentIcon);
		}			
	}

	void SetPrimaryColor(int color)
	{
		m_PrimaryColor = color;

		if (!m_MouseHover)
		{
			m_Name.SetColor(m_PrimaryColor);
			m_Icon.SetColor(m_PrimaryColor);
		}
	}

	int GetPrimaryColor()
	{
		return m_PrimaryColor;
	}

	void SetHoverColour(int color)
	{
		m_HoverColor = color;
	}

	int GetHoverColor()
	{
		return m_HoverColor;
	}

	bool IsMouseHovering()
	{
		return m_MouseHover;
	}

	ImageWidget GetIconWidget()
	{
		return m_Icon;
	}

	Widget GetDragWidget()
	{
		return m_DragWidget;
	}

	void SetIgnorePointer(bool state)
	{
		if (state)
		{
			GetLayoutRoot().SetFlags(WidgetFlags.IGNOREPOINTER);
			m_DragWidget.SetFlags(WidgetFlags.IGNOREPOINTER);
			m_EditButton.SetFlags(WidgetFlags.IGNOREPOINTER);
		}
		else
		{
			GetLayoutRoot().ClearFlags(WidgetFlags.IGNOREPOINTER);
			m_DragWidget.ClearFlags(WidgetFlags.IGNOREPOINTER);
			m_EditButton.ClearFlags(WidgetFlags.IGNOREPOINTER);
		}
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL && IsEditButton(w))
		{
			m_MouseHover = true;
			m_Name.SetColor(m_HoverColor);
			m_Icon.SetColor(m_HoverColor);
			return true;
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL && IsEditButton(w))
		{
			m_MouseHover = false;
			m_Name.SetColor(m_PrimaryColor);
			m_Icon.SetColor(m_PrimaryColor);
			return true;
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	bool CanDrag()
	{
		return true;
	}
};
