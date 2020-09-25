/**
 * ExpansionMapMarkerList.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMarkerList extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	
	private Widget m_MarkerListPanel;
	
	private ButtonWidget m_PersonalMarkersToggle;
	private ImageWidget m_PersonalMarkersToggleIcon;
	private WrapSpacerWidget m_PersonalMarkersContent;
	private ButtonWidget m_PersonalMarkers2DToggle;
	private ImageWidget m_PersonalMarkers2DToggleIcon;
	private ButtonWidget m_PersonalMarkers3DToggle;
	private ImageWidget m_PersonalMarkers3DToggleIcon;
	
	private ButtonWidget m_PartyMarkersToggle;
	private ImageWidget m_PartyMarkersToggleIcon;
	private WrapSpacerWidget m_PartyMarkersContent;
	private ButtonWidget m_PartyMarkers2DToggle;
	private ImageWidget m_PartyMarkers2DToggleIcon;
	private ButtonWidget m_PartyMarkers3DToggle;
	private ImageWidget m_PartyMarkers3DToggleIcon;
	
	private ButtonWidget m_MemberMarkersToggle;
	private ImageWidget m_MemberMarkersToggleIcon;
	private WrapSpacerWidget m_MemberMarkersContent;
	private ButtonWidget m_MemberMarkers2DToggle;
	private ImageWidget m_MemberMarkers2DToggleIcon;
	private ButtonWidget m_MemberMarkers3DToggle;
	private ImageWidget m_MemberMarkers3DToggleIcon;
	
	private ButtonWidget m_ServerMarkersToggle;
	private ImageWidget m_ServerMarkersToggleIcon;
	private WrapSpacerWidget m_ServerMarkersContent;
	private ButtonWidget m_ServerMarkers2DToggle;
	private ImageWidget m_ServerMarkers2DToggleIcon;
	private ButtonWidget m_ServerMarkers3DToggle;
	private ImageWidget m_ServerMarkers3DToggleIcon;
	
	private static bool m_MarkerListState;
	private ButtonWidget m_MarkerListToggle;
	private ImageWidget m_MarkerListToggleIcon;
	
	private Widget m_MarkerListInfoPanel;
	private ImageWidget m_MarkerListInfoIcon;
	
	private ref array<ref ExpansionMapMarkerListEntry> m_PersonalMarkers;
	private ref array<ref ExpansionMapMarkerListEntry> m_PartyMarkers;
	private ref array<ref ExpansionMapMarkerListEntry> m_MemberMarkers;
	private ref array<ref ExpansionMapMarkerListEntry> m_ServerMarkers;
	
	private ExpansionMapMenu m_MapMenu;
	private ExpansionMarkerModule m_MarkerModule;
	
	private static int COLOR_NOFOCUS = ARGB(140,35,35,35);
	private static int COLOR_FOCUS = ARGB(140,255,255,255);
	
	private ref ExpansionUITooltip m_InfoTooltip;
	
	void OnWidgetScriptInit( Widget w )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::OnWidgetScriptInit - Start");
		#endif
		
		m_Root = w;
		m_Root.SetHandler( this );
	
		m_MarkerListPanel = Widget.Cast(m_Root.FindAnyWidget("MarkerListPanel"));
		
		m_PersonalMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("PersonalMarkerHeaderButton"));
		m_PersonalMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("PersonalMarkerHeaderIcon"));
		m_PersonalMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("PersonalMarkerContent"));
		m_PersonalMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_personal_2d"));
		m_PersonalMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_personal_2d_icon"));
		m_PersonalMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_personal_3d"));
		m_PersonalMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_personal_3d_icon"));
		
		m_PartyMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("PartyMarkerHeaderButton"));
		m_PartyMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("PartyMarkerHeaderIcon"));
		m_PartyMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("PartyMarkerContent"));
		m_PartyMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_2d"));
		m_PartyMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_2d_icon"));	
		m_PartyMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_3d"));
		m_PartyMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_3d_icon"));	
			
		m_MemberMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("MemberMarkerHeaderButton"));
		m_MemberMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("MemberMarkerHeaderIcon"));
		m_MemberMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("MemberMarkerContent"));
		m_MemberMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_2d"));
		m_MemberMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_2d_icon"));
		m_MemberMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_3d"));
		m_MemberMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_3d_icon"));	
		
		m_ServerMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("ServerMarkerHeaderButton"));
		m_ServerMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("ServerMarkerHeaderIcon"));
		m_ServerMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("ServerMarkerContent"));
		m_ServerMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_server_2d"));
		m_ServerMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_server_2d_icon"));
		m_ServerMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_server_3d"));
		m_ServerMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_server_3d_icon"));
		
		m_MarkerListToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("MarkerListShow"));
		m_MarkerListToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("MarkerListShowIcon"));
		
		m_MarkerListInfoPanel = Widget.Cast(m_Root.FindAnyWidget("MarkerListPanelFooterInfoPanel"));
		m_MarkerListInfoIcon = ImageWidget.Cast(m_Root.FindAnyWidget("MarkerListPanelFooterInfoIcon"));
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::OnWidgetScriptInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Init
	// ------------------------------------------------------------
	void Init( ExpansionMapMenu mapmenu )
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::Init - Start");
		#endif

		m_MapMenu = mapmenu;

		m_MarkerModule = ExpansionMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMarkerModule ) );
		
		m_PersonalMarkers = new array<ref ExpansionMapMarkerListEntry>();
		m_PartyMarkers = new array<ref ExpansionMapMarkerListEntry>();
		m_MemberMarkers = new array<ref ExpansionMapMarkerListEntry>();
		m_ServerMarkers = new array<ref ExpansionMapMarkerListEntry>();
		
		string arrow = ExpansionIcons.GetPath( "Arrow" );

		m_PersonalMarkersToggleIcon.LoadImageFile( 0, arrow );
		m_PartyMarkersToggleIcon.LoadImageFile( 0, arrow );
		m_ServerMarkersToggleIcon.LoadImageFile( 0, arrow );
		
		m_PersonalMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_2D_OFF );
		m_PersonalMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_2D_ON );
		m_PersonalMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_3D_OFF );
		m_PersonalMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_3D_ON );
		
		m_PartyMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_2D_OFF );
		m_PartyMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_2D_ON );
		m_PartyMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_3D_OFF );
		m_PartyMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_3D_ON );
		
		m_MemberMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_2D_OFF );
		m_MemberMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_2D_ON );
		m_MemberMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_3D_OFF );
		m_MemberMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_3D_ON );
		
		m_ServerMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_2D_OFF );
		m_ServerMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_2D_ON );
		m_ServerMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_NOTIFICATION_ICON_3D_OFF );
		m_ServerMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_NOTIFICATION_ICON_3D_ON );
		
		UpdateToggleStates();
		
		// Allows for persistent marker list state
		ShowPanel( m_MarkerListState );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::Init - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// AddPersonalEntry
	// ------------------------------------------------------------
	void AddPersonalEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPersonalEntry - Start");
		#endif
		
		ExpansionMapMarkerListEntry entry = new ExpansionMapMarkerListEntry(m_MapMenu, m_PersonalMarkersContent, marker);
		m_PersonalMarkers.Insert(entry);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPersonalEntry - End");
		#endif
	}

	// ------------------------------------------------------------
	// RemovePersonalEntry
	// ------------------------------------------------------------	
	void RemovePersonalEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - Start");
		#endif
		
		for ( int i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			if(m_PersonalMarkers[i].GetMarker() == marker)
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - Remove entry [" + i + "] from m_PersonalMarkers");
				#endif
				
				m_PersonalMarkers.Remove(i);
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// AddPartyEntry
	// ------------------------------------------------------------
	void AddPartyEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPartyEntry - Start");
		#endif
		
		ExpansionMapMarkerListEntry entry = new ExpansionMapMarkerListEntry(m_MapMenu, m_PartyMarkersContent, marker);
		m_PartyMarkers.Insert(entry);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPartyEntry - End");
		#endif
	}

	// ------------------------------------------------------------
	// RemovePartyEntry
	// ------------------------------------------------------------	
	void RemovePartyEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - Start");
		#endif
		
		for ( int i = 0; i < m_PartyMarkers.Count(); ++i )
		{
			if(m_PartyMarkers[i].GetMarker() == marker)
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - Remove entry [" + i + "] from m_PartyMarkers");
				#endif
				
				m_PartyMarkers.Remove(i);
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// AddMemberEntry
	// ------------------------------------------------------------
	void AddMemberEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddMemberEntry - Start");
		#endif
		
		ExpansionMapMarkerListEntry entry = new ExpansionMapMarkerListEntry(m_MapMenu, m_MemberMarkersContent, marker);
		m_MemberMarkers.Insert(entry);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddMemberEntry - End");
		#endif
	}

	// ------------------------------------------------------------
	// RemoveMemberEntry
	// ------------------------------------------------------------	
	void RemoveMemberEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - Start");
		#endif
		
		for ( int i = 0; i < m_MemberMarkers.Count(); ++i )
		{
			if(m_MemberMarkers[i].GetMarker() == marker)
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - Remove entry [" + i + "] from m_MemberMarkers");
				#endif
				
				m_MemberMarkers.Remove(i);
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// AddServerEntry
	// ------------------------------------------------------------
	void AddServerEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddServerEntry - Start");
		#endif
		
		ExpansionMapMarkerListEntry entry = new ExpansionMapMarkerListEntry(m_MapMenu, m_ServerMarkersContent, marker);
		m_ServerMarkers.Insert(entry);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddServerEntry - End");
		#endif
	}

	// ------------------------------------------------------------
	// RemoveServerEntry
	// ------------------------------------------------------------	
	void RemoveServerEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveServerEntry - Start");
		#endif
		
		for ( int i = 0; i < m_ServerMarkers.Count(); ++i )
		{
			if ( m_ServerMarkers[i].GetMarker() == marker )
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemoveServerEntry - Remove entry [" + i + "] from m_ServerMarkers");
				#endif
				
				m_ServerMarkers.Remove(i);
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveServerEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// UpdateToggleStates
	// ------------------------------------------------------------
	void UpdateToggleStates()
	{
		m_PersonalMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PERSONAL ) );
		m_PersonalMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PERSONAL ) );
		
		m_PartyMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PARTY ) );
		m_PartyMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PARTY ) );

		m_MemberMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PLAYER ) );
		m_MemberMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PLAYER ) );

		m_ServerMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.SERVER ) );
		m_ServerMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.SERVER ) );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{

		switch (w)
		{
			case m_MarkerListToggle:
				m_MarkerListState = !m_MarkerListPanel.IsVisible();
				ShowPanel( m_MarkerListState );
				break;
			case m_PersonalMarkersToggle:
				ShowPersonal( !m_PersonalMarkersContent.IsVisible() );
				break;
			case m_PartyMarkersToggle:
				ShowParty( !m_PartyMarkersContent.IsVisible() );
				break;
			case m_ServerMarkersToggle:
				ShowServer( !m_ServerMarkersContent.IsVisible() );
				break;
			case m_MemberMarkersToggle:
				ShowMembers( !m_MemberMarkersContent.IsVisible() );
				break;
			case m_PersonalMarkers2DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP );
				break;
			case m_PersonalMarkers3DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD );
				break;
			case m_PartyMarkers2DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_MAP );
				break;
			case  m_PartyMarkers3DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD );
				break;
			case  m_MemberMarkers2DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP );
				break;
			case m_MemberMarkers3DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD );
				break;
			case m_ServerMarkers2DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_MAP );
				break;
			case m_ServerMarkers3DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD );
				break;
		}

		m_MarkerModule.Refresh();
		
		UpdateToggleStates();
		
		return false;
	}

	
	// ------------------------------------------------------------
	// ShowPanel
	// ------------------------------------------------------------
	void ShowPanel(bool state)
	{
		m_MarkerListPanel.Show(state);
		
		if (state) m_MarkerListToggleIcon.SetRotation(0,0,270);
		else m_MarkerListToggleIcon.SetRotation(0,0,90);
	}
	
	// ------------------------------------------------------------
	// ShowPersonal
	// ------------------------------------------------------------	
	void ShowPersonal(bool state)
	{
		m_PersonalMarkersContent.Show(state);
		
		if (state) m_PersonalMarkersToggleIcon.SetFlags(WidgetFlags.FLIPV);
		else m_PersonalMarkersToggleIcon.ClearFlags(WidgetFlags.FLIPV);
	}
	
	// ------------------------------------------------------------
	// ShowParty
	// ------------------------------------------------------------	
	void ShowParty(bool state)
	{
		m_PartyMarkersContent.Show(state);	
			
		if (state) m_PartyMarkersToggleIcon.SetFlags(WidgetFlags.FLIPV);
		else m_PartyMarkersToggleIcon.ClearFlags(WidgetFlags.FLIPV);
	}
		
	// ------------------------------------------------------------
	// ShowMembers
	// ------------------------------------------------------------	
	void ShowMembers(bool state)
	{
		m_MemberMarkersContent.Show(state);	
			
		if (state) m_MemberMarkersToggleIcon.SetFlags(WidgetFlags.FLIPV);
		else m_MemberMarkersToggleIcon.ClearFlags(WidgetFlags.FLIPV);
	}
	
	// ------------------------------------------------------------
	// ShowServer
	// ------------------------------------------------------------	
	void ShowServer(bool state)
	{
		m_ServerMarkersContent.Show(state);
		
		if (state) m_ServerMarkersToggleIcon.SetFlags(WidgetFlags.FLIPV);
		else m_ServerMarkersToggleIcon.ClearFlags(WidgetFlags.FLIPV);
	}
	
	// ------------------------------------------------------------
	// Expansion GetPersonalEntrys
	// ------------------------------------------------------------	
	ref array<ref ExpansionMapMarkerListEntry> GetPersonalEntrys()
	{
		return m_PersonalMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyEntrys
	// ------------------------------------------------------------	
	ref array<ref ExpansionMapMarkerListEntry> GetPartyEntrys()
	{
		return m_PartyMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyEntrys
	// ------------------------------------------------------------	
	ref array<ref ExpansionMapMarkerListEntry> GetMemberEntrys()
	{
		return m_MemberMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion GetServerEntrys
	// ------------------------------------------------------------	
	ref array<ref ExpansionMapMarkerListEntry> GetServerEntrys()
	{
		return m_ServerMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion IsListVisible
	// ------------------------------------------------------------	
	bool IsListVisible()
	{
		return m_MarkerListPanel.IsVisible();
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		if (w == m_PersonalMarkersToggle || w == m_PartyMarkersToggle || w == m_ServerMarkersToggle || w == m_MemberMarkersToggle)
		{
			w.SetColor(COLOR_FOCUS);
		} else if (w == m_MarkerListInfoPanel)
		{
			m_MarkerListInfoIcon.SetColor(ARGB(140,243,156,18));			
			if (!m_InfoTooltip.IsVisible() && GetExpansionSettings().GetMap().ShowMapStats )
			{
				m_InfoTooltip.ShowTooltip();
			}
			
		} else if (w == m_MarkerListToggle)
		{
			m_MarkerListToggleIcon.SetColor(ARGB(140,243,156,18));
		}

		if ( w.GetName().IndexOf( "visibility_marker_option_" ) == 0 )
		{
			w.GetChildren().SetColor( ARGB( 255, 150, 150, 150 ) );
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		if (w == m_PersonalMarkersToggle || w == m_PartyMarkersToggle || w == m_ServerMarkersToggle || w == m_MemberMarkersToggle)
		{
			w.SetColor(COLOR_NOFOCUS);
		} else if (w == m_MarkerListInfoPanel)
		{
			m_MarkerListInfoIcon.SetColor(ARGB(140,220,220,220));
			if (m_InfoTooltip.IsVisible())
			{
				m_InfoTooltip.HideTooltip();
			}
		} else if (w == m_MarkerListToggle)
		{
			m_MarkerListToggleIcon.SetColor(ARGB(255,220,220,220));
		}

		if ( w.GetName().IndexOf( "visibility_marker_option_" ) == 0 )
		{
			w.GetChildren().SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		SetFocus(null);
		
		return false;
	}
	
	void HideTooltips()
	{
		if (m_InfoTooltip.IsVisible())
			m_InfoTooltip.HideTooltip();
		
		int i;
		for ( i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			if(m_PersonalMarkers[i].GetTooltip().IsVisible())
			{				
				m_PersonalMarkers[i].GetTooltip().HideTooltip();
			}
		}
		
		for ( i = 0; i < m_PartyMarkers.Count(); ++i )
		{
			if(m_PartyMarkers[i].GetTooltip().IsVisible())
			{				
				m_PartyMarkers[i].GetTooltip().HideTooltip();
			}
		}
		
		for ( i = 0; i < m_MemberMarkers.Count(); ++i )
		{
			if(m_MemberMarkers[i].GetTooltip().IsVisible())
			{				
				m_MemberMarkers[i].GetTooltip().HideTooltip();
			}
		}
		
		for ( i = 0; i < m_ServerMarkers.Count(); ++i )
		{
			if(m_ServerMarkers[i].GetTooltip().IsVisible())
			{				
				m_ServerMarkers[i].GetTooltip().HideTooltip();
			}
		}
	}
}