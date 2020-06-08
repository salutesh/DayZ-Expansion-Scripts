/**
 * ExpansionMapMenuMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuMarker extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_Name;
	protected ImageWidget m_Icon;
	protected ButtonWidget m_MarkerButton;
	protected Widget m_MarkerDragging;
	protected MapWidget m_MapWidget;
	protected vector m_MarkerPos;
	protected string m_MarkerName;
	protected string m_MarkerIcon;
	protected int m_MarkerColor;
	protected int m_MarkerAlpha;
	protected autoptr ExpansionMapMenu m_MapMenu;

	protected autoptr ExpansionMapMarker m_MarkerData;
	protected autoptr Expansion3DMarkerModule m_3DMarkerModule;
	
	protected float m_OffsetX;
	protected float m_OffsetY;
	
	protected bool m_IsTempMarker;
	protected bool m_Dragging;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarker Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuMarker(Widget parent, MapWidget mapwidget, vector position, string name, int color, string icon, ref ExpansionMapMarker marker = NULL)
	{
		m_Root 				= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/map/expansion_map_marker.layout", parent ) );

		m_Name				= TextWidget.Cast( m_Root.FindAnyWidget("marker_name") );
		m_Icon				= ImageWidget.Cast( m_Root.FindAnyWidget("marker_icon") );
		m_MarkerButton		= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_button") );
		m_MarkerDragging 	= ButtonWidget.Cast( m_Root.FindAnyWidget("marker_icon_panel") );

		m_MapWidget			= mapwidget;
		m_MarkerPos			= position;
		m_MarkerName		= name;
		m_MarkerIcon		= icon;
		m_MarkerColor		= color;
		m_MarkerData		= marker;
		
		m_Root.SetHandler( this );
		
		Init();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarker Init
	// ------------------------------------------------------------
	void Init()
	{
		if (!m_MarkerData)
			m_IsTempMarker = true;
		else
			m_IsTempMarker = false;
		
		//! Get Map Menu
		m_MapMenu = ExpansionMapMenu.Cast(GetGame().GetUIManager().GetMenu());
		
		//! Get 3D Marker Module
		m_3DMarkerModule = Expansion3DMarkerModule.Cast(GetModuleManager().GetModule(Expansion3DMarkerModule));
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuMarker()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteMarker
	// ------------------------------------------------------------
	void DeleteMarker()
	{
		m_Root.Unlink();
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeColor
	// ------------------------------------------------------------
	void ChangeColor(int color)
	{
		m_MarkerColor = color;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangePositon
	// ------------------------------------------------------------
	void ChangePositon(vector position)
	{
		m_MarkerPos = position;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeIcon
	// ------------------------------------------------------------
	void ChangeIcon(string icon)
	{
		m_MarkerIcon = icon;
	}
	
	// ------------------------------------------------------------
	// Expansion ChangeName
	// ------------------------------------------------------------
	void ChangeName(string name)
	{
		m_MarkerName = name;
	}
	
	//============================================
	// Expansion GetMarkerName
	//============================================
	string GetMarkerName()
	{
		return m_MarkerName;
	}
	
	//============================================
	// Expansion Update
	//============================================
	void Update( float timeslice )
	{
		if (m_MarkerIcon)
			m_Icon.LoadImageFile(0, m_MarkerIcon);
		
		if (m_MarkerColor)
			m_Icon.SetColor(m_MarkerColor);
			m_Name.SetColor(m_MarkerColor);
		
		if (m_MarkerName)
			m_Name.SetText(m_MarkerName);
		
		if (m_MarkerPos && !m_Dragging)
		{
			vector mapPos = m_MapWidget.MapToScreen(m_MarkerPos);
			m_Root.SetPos(mapPos[0], mapPos[1], true);
		}
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if (m_MarkerButton && w == m_MarkerButton && !m_IsTempMarker)
		{
			m_Icon.SetColor(ARGB(255,255,255,255));
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		int color = m_MarkerColor;
		if (m_MarkerButton && w == m_MarkerButton && !m_IsTempMarker)
		{
			m_Icon.SetColor(color);
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( m_MarkerButton && m_MarkerData && w == m_MarkerButton )
		{
			bool isPartyMarker = m_MarkerData.IsPartyMarker();
			string markerName = m_MarkerData.GetMarkerText();
			m_MapMenu.GetMarkerWindow().EditMarker(markerName, isPartyMarker, this);

			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnDrag
	// ------------------------------------------------------------
	override bool OnDrag( Widget w, int x, int y )
	{		
		if ( w == m_MarkerButton )
		{
			//! Get current position of the widget 
			m_Root.GetPos( m_OffsetX, m_OffsetY );

			m_OffsetX = x - m_OffsetX;
			m_OffsetY = y - m_OffsetY;
			
			m_Dragging = true;
			
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnDragging
	// ------------------------------------------------------------
	override bool OnDragging( Widget w, int x, int y, Widget reciever )
	{
		if ( w == m_MarkerButton )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenuMarker::OnDragging - Dragging m_MarkerButton");
			#endif
			
			SetPosition( x - m_OffsetX, y - m_OffsetY );

			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnDrop
	// ------------------------------------------------------------
	override bool OnDrop( Widget w, int x, int y, Widget reciever )
	{
		if ( w == m_MarkerButton )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenuMarker::OnDrop - Drop m_MarkerButton");
			#endif
			
			SetPosition( x - m_OffsetX, y - m_OffsetY, true );
			
			m_Dragging = false;
			
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetPosition
	// ------------------------------------------------------------
	protected void SetPosition( int x, int y, bool dropped = false )
	{
		m_Root.SetPos( x, y, true );
		m_MarkerPos = m_MapWidget.ScreenToMap( Vector( x, y, 0 ) );
		
		m_MapMenu.SetCurrentMarkerPosition( m_MarkerPos );
		m_MarkerData.SetPosition( m_MarkerPos );
		
		if (dropped)
		{
			ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(GetModuleManager().GetModule(ExpansionPartyModule));
			if ( partyModule && m_MarkerData && m_MarkerData.IsPartyMarker())
			{
				partyModule.UpdatePositionMarker( m_MarkerData.GetMarkerText(), m_MarkerPos );
			}
			
			ExpansionMapMarkerModule mapMarkerModule = ExpansionMapMarkerModule.Cast(GetModuleManager().GetModule(ExpansionMapMarkerModule));
			if ( mapMarkerModule && m_MarkerData && !m_MarkerData.IsPartyMarker() )
			{
				mapMarkerModule.SaveClientMarkers();
			}
		}
		
		m_3DMarkerModule.RefreshMarkers();
	}
	
	// ------------------------------------------------------------
	// Expansion Show
	// ------------------------------------------------------------
	void Show()
	{
		m_Root.Show( true );
	}
	
	// ------------------------------------------------------------
	// Expansion Hide
	// ------------------------------------------------------------
	void Hide()
	{
		m_Root.Show( false );
	}
}