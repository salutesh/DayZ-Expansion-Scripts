/**
 * ExpansionMapMarkerListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerListEntry: ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private Widget m_ListPanel;
	private ImageWidget m_Icon;
	private string m_Text;
	private TextWidget m_Name;
	private ButtonWidget m_3DToggle;
	private ImageWidget m_3DToggleIcon;
	private ButtonWidget m_2DToggle;
	private ImageWidget m_2DToggleIcon;
	private ButtonWidget m_DeleteEntry;
	private ImageWidget m_DeleteEntryIcon;
	
	private ref ExpansionMapMenu m_MapMenu;
	private ref ExpansionMapMarker m_Marker;
	private ref ExpansionMarkerData m_MarkerData;
	private ref ExpansionMarkerModule m_MarkerModule;
	
	private static int COLOR_NOFOCUS = ARGB(220,0,0,0);
	private static int COLOR_FOCUS = ARGB(220,220,220,220);
	
	private bool m_MouseHover;
	private bool m_ConfirmDelete;
	
	private ref ExpansionUITooltip m_InfoTooltip;
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerListEntry Constructor
	// ------------------------------------------------------------	
	void ExpansionMapMarkerListEntry(  ExpansionMapMenu map_menu, Widget list_category_parent, ExpansionMapMarker marker )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::ExpansionMapMarkerListEntry - Start");
		#endif
		
		m_Root = GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/Navigation/GUI/layouts/expansion_map_markerlist_entry.layout", list_category_parent );
		m_ListPanel = Widget.Cast(m_Root.FindAnyWidget("MarkerListEntryPanel"));
		m_Icon = ImageWidget.Cast(m_Root.FindAnyWidget("MarkerListEntryIcon"));
		m_Name = TextWidget.Cast(m_Root.FindAnyWidget("MarkerListEntryName"));
		m_2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_entry_2d"));
		m_2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_entry_2d_icon"));
		m_3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_entry_3d"));
		m_3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_entry_3d_icon"));
		m_DeleteEntry = ButtonWidget.Cast(m_Root.FindAnyWidget("marker_option_entry_delete")); 
		m_DeleteEntryIcon = ImageWidget.Cast(m_Root.FindAnyWidget("marker_option_entry_delete_icon"));
		m_DeleteEntryIcon.LoadImageFile(0, "DayZExpansion\\Core\\GUI\\icons\\misc\\trashcan.edds");
		m_DeleteEntryIcon.LoadImageFile(1, "DayZExpansion\\Core\\GUI\\icons\\misc\\trashcan_checkmark.edds");
		
		m_MapMenu = map_menu;
		m_Marker = marker;
		m_MarkerData = marker.GetMarkerData();
		m_MarkerModule = ExpansionMarkerModule.Cast( CF_ModuleCoreManager.Get( ExpansionMarkerModule ) );
		
		m_InfoTooltip = new ExpansionUITooltip("<p>" + "X:" + m_Marker.GetMarkerData().GetPosition()[0] + " Y:" + m_Marker.GetMarkerData().GetPosition()[1] + " Z:" + m_Marker.GetMarkerData().GetPosition()[2] + "</p>");
		m_InfoTooltip.SetTextPos("center");
		
		m_Root.SetHandler(this);
		
		SetEntry();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::ExpansionMapMarkerListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarkerListEntry Destructor
	// ------------------------------------------------------------	
	void ~ExpansionMapMarkerListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Set
	// ------------------------------------------------------------	
	void SetEntry()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::Set - Start");
		#endif
		
		if (m_Marker && m_Marker.GetMarkerData())
		{
			m_Icon.LoadImageFile(0, ExpansionIcons.GetPath(m_Marker.GetMarkerData().GetIconName()));
			m_Icon.SetImage(0);
			m_Icon.SetColor(m_Marker.GetMarkerData().GetColor());
			m_Text = m_Marker.GetMarkerData().GetName();
			m_Name.SetText(m_Text);
			m_Name.SetColor(m_Marker.GetMarkerData().GetColor());
			
			m_3DToggleIcon.LoadImageFile(0, EXPANSION_ICON_3D_OFF);
			m_3DToggleIcon.LoadImageFile(1, EXPANSION_ICON_3D_ON);
			
			m_2DToggleIcon.LoadImageFile(0, EXPANSION_ICON_2D_OFF);
			m_2DToggleIcon.LoadImageFile(1, EXPANSION_ICON_2D_ON);
			
			TextWidget button_lable = TextWidget.Cast(m_Root.FindAnyWidget("MarkerListEntry3DButtonLable"));
			if (!m_Marker.GetMarkerData().Is3D())
			{
				m_3DToggleIcon.Show(false);
				m_3DToggle.SetFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			} else
			{
				m_3DToggleIcon.Show(true);
				m_3DToggle.ClearFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			}
			
		#ifdef EXPANSIONMODGROUPS
			if (m_Marker.GetMarkerData().IsInherited(ExpansionPlayerMarkerData) && !GetExpansionSettings().GetParty().ShowPartyMemberMapMarkers)
			{
				m_2DToggleIcon.Show(false);
				m_2DToggle.SetFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			} else
			{
				m_2DToggleIcon.Show(true);
				m_2DToggle.ClearFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			}
		#endif

			if (!m_Marker.CanEdit())
			{
				m_DeleteEntryIcon.Show(false);
				m_DeleteEntry.SetFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			} else
			{
				m_DeleteEntryIcon.Show(true);
				m_DeleteEntry.ClearFlags(WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS);
			}

			m_2DToggleIcon.SetImage(m_MarkerData.IsMapVisible());
			m_3DToggleIcon.SetImage(m_MarkerData.IsWorldVisible());
			
			m_InfoTooltip.SetText("<p>" + "X:" + m_Marker.GetMarkerData().GetPosition()[0] + " Y:" + m_Marker.GetMarkerData().GetPosition()[1] + " Z:" + m_Marker.GetMarkerData().GetPosition()[2] + "</p>");
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::Set - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// UpdateEntry
	// ------------------------------------------------------------	
	private void UpdateEntry()
	{
		if (m_Marker != NULL && m_Marker.GetMarkerData())
		{
			m_Icon.LoadImageFile(0, ExpansionIcons.GetPath(m_Marker.GetMarkerData().GetIconName()));
			m_Icon.SetImage(0);
			m_Icon.SetColor(m_Marker.GetMarkerData().GetColor());
			m_Text = m_Marker.GetMarkerData().GetName();
			m_Name.SetText(m_Text);
			m_Name.SetColor(m_Marker.GetMarkerData().GetColor());
			
			if ( !m_Marker.GetMarkerData().Is3D() )
			{
				m_3DToggleIcon.Show( false );
				m_3DToggle.SetFlags( WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS );
			} else
			{
				m_3DToggleIcon.Show( true );
				m_3DToggle.ClearFlags( WidgetFlags.IGNOREPOINTER | WidgetFlags.NOFOCUS );
			}
			
			m_InfoTooltip.SetText("<p>" + "X:" + m_Marker.GetMarkerData().GetPosition()[0] + " Y:" + m_Marker.GetMarkerData().GetPosition()[1] + " Z:" + m_Marker.GetMarkerData().GetPosition()[2] + "</p>");
		}
	}
		
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::OnClick");
		#endif
		
		if (m_3DToggle && w == m_3DToggle)
		{
			if (m_MarkerData.IsWorldVisible())
			{
				m_MarkerModule.RemoveVisibility(m_MarkerData, EXPANSION_MARKER_VIS_WORLD);
			} else
			{
				m_MarkerModule.SetVisibility(m_MarkerData, EXPANSION_MARKER_VIS_WORLD);
			}
			
			m_MarkerModule.Refresh();
			UpdateToggleStates();

			return true;
		}
		
		if (m_2DToggle && w == m_2DToggle)
		{
			if (m_MarkerData.IsMapVisible())
			{
				m_MarkerModule.RemoveVisibility(m_MarkerData, EXPANSION_MARKER_VIS_MAP);
			} else
			{
				m_MarkerModule.SetVisibility(m_MarkerData, EXPANSION_MARKER_VIS_MAP);
			}
			
			m_MarkerModule.Refresh();
			UpdateToggleStates();

			return true;
		}
		
		if (m_DeleteEntry && w == m_DeleteEntry)
		{
			//! @note need to click delete button twice to confirm deletion
			if (m_ConfirmDelete)
			{
				m_MapMenu.DeleteMarker(m_Marker);
			}
			else
			{
				m_ConfirmDelete = true;
				m_DeleteEntryIcon.SetColor(ARGB(255, 240, 240, 240));
				m_DeleteEntryIcon.SetImage(1);
			}

			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseButtonDown
	// ------------------------------------------------------------
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( button == 0 && w == m_ListPanel )
		{
			m_Marker.FocusOnMarker( true );
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// GetMarker
	// ------------------------------------------------------------
	ExpansionMapMarker GetMarker()
	{
		return m_Marker;
	}

	ExpansionMarkerData GetData()
	{
		return m_MarkerData;
	}
	
	string GetName()
	{
		return m_Text;
	}

	// ------------------------------------------------------------
	// Update
	// ------------------------------------------------------------
	void Update()
	{
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::Update - Start");
		#endif
				
		UpdateEntry();
		
		#ifdef EXPANSION_MAP_MENU_UPDATE_DEBUG
	   	EXLogPrint("ExpansionMapMarkerListEntry::Update - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveMarker
	// Events when remove marker with delete key
	// ------------------------------------------------------------	
	void RemoveMarker()
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::RemoveMarker - Start");
		#endif
		
		if (IsMouseHovering())
		{
			m_MapMenu.DeleteMarker(m_Marker);
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::RemoveMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::OnMouseEnter");
		#endif
		
		if (w == m_ListPanel)
		{
			m_ListPanel.SetColor(COLOR_FOCUS);
			m_MouseHover = true;
			m_InfoTooltip.ShowTooltip();
		}

		if ( w.GetName().Contains( "marker_option_entry_" ) )
		{
			w.GetChildren().SetColor(ARGB(220, 220, 220, 220));
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerListEntry::OnMouseLeave");
		#endif

		if (w == m_ListPanel)
		{
			m_ListPanel.SetColor(COLOR_NOFOCUS);
			m_MouseHover = false;
			m_InfoTooltip.HideTooltip();
		}

		if (w == m_DeleteEntry)
		{
			m_ConfirmDelete = false;
			m_DeleteEntryIcon.SetImage(0);
		}

		if ( w.GetName().Contains( "marker_option_entry_" ) )
		{
			w.GetChildren().SetColor(ARGB(255, 220, 220, 220));
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// IsMouseHovering
	// ------------------------------------------------------------	
	bool IsMouseHovering()
	{
		return m_MouseHover;
	}
	
	// ------------------------------------------------------------
	// UpdateToggleStates
	// ------------------------------------------------------------	
	void UpdateToggleStates()
	{
		m_2DToggleIcon.SetImage(m_MarkerData.IsMapVisible());
		m_3DToggleIcon.SetImage(m_MarkerData.IsWorldVisible());
	}
	
	// ------------------------------------------------------------
	// GetTooltip
	// ------------------------------------------------------------	
	ExpansionUITooltip GetTooltip()
	{
		return m_InfoTooltip;
	}

	void SetSort(int sort, bool immedUpdate = true)
	{
		m_Root.SetSort(sort, immedUpdate);
	}
}