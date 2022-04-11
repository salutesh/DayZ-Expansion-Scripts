/**
 * ExpansionMapMarkerList.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	
#ifdef EXPANSIONMODGROUPS
	private Widget m_PartyMarkersPanel;
	private ButtonWidget m_PartyMarkersToggle;
	private ImageWidget m_PartyMarkersToggleIcon;
	private WrapSpacerWidget m_PartyMarkersContent;
	private ButtonWidget m_PartyMarkers2DToggle;
	private ImageWidget m_PartyMarkers2DToggleIcon;
	private ButtonWidget m_PartyMarkers3DToggle;
	private ImageWidget m_PartyMarkers3DToggleIcon;
	
	private Widget m_MemberMarkersPanel;
	private ButtonWidget m_MemberMarkersToggle;
	private ImageWidget m_MemberMarkersToggleIcon;
	private WrapSpacerWidget m_MemberMarkersContent;
	private ButtonWidget m_MemberMarkers2DToggle;
	private ImageWidget m_MemberMarkers2DToggleIcon;
	private ButtonWidget m_MemberMarkers3DToggle;
	private ImageWidget m_MemberMarkers3DToggleIcon;
#endif
	
	private ButtonWidget m_ServerMarkersToggle;
	private ImageWidget m_ServerMarkersToggleIcon;
	private WrapSpacerWidget m_ServerMarkersContent;
	private ButtonWidget m_ServerMarkers2DToggle;
	private ImageWidget m_ServerMarkers2DToggleIcon;
	private ButtonWidget m_ServerMarkers3DToggle;
	private ImageWidget m_ServerMarkers3DToggleIcon;
	
	private static bool m_MarkerListState;
	private static bool m_MarkerListInitState;
	private ButtonWidget m_MarkerListToggle;
	private ImageWidget m_MarkerListToggleIcon;
	
	private Widget m_MarkerListInfoPanel;
	private ImageWidget m_MarkerListInfoIcon;
	
	private ref array<ref ExpansionMapMarkerListEntry> m_PersonalMarkers;
#ifdef EXPANSIONMODGROUPS
	private ref array<ref ExpansionMapMarkerListEntry> m_PartyMarkers;
	private ref array<ref ExpansionMapMarkerListEntry> m_MemberMarkers;
#endif
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
		
	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkersPanel = Widget.Cast(m_Root.FindAnyWidget("PartyMarkerPanel"));
		m_PartyMarkersPanel.Show(true);
		m_PartyMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("PartyMarkerHeaderButton"));
		m_PartyMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("PartyMarkerHeaderIcon"));
		m_PartyMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("PartyMarkerContent"));
		m_PartyMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_2d"));
		m_PartyMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_2d_icon"));	
		m_PartyMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_3d"));
		m_PartyMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_party_3d_icon"));	
		
		m_MemberMarkersPanel = Widget.Cast(m_Root.FindAnyWidget("MemberMarkerPanel"));
		m_PartyMarkersPanel.Show(true);
		m_MemberMarkersToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("MemberMarkerHeaderButton"));
		m_MemberMarkersToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("MemberMarkerHeaderIcon"));
		m_MemberMarkersContent = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("MemberMarkerContent"));
		m_MemberMarkers2DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_2d"));
		m_MemberMarkers2DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_2d_icon"));
		m_MemberMarkers3DToggle = ButtonWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_3d"));
		m_MemberMarkers3DToggleIcon = ImageWidget.Cast(m_Root.FindAnyWidget("visibility_marker_option_member_3d_icon"));	
	#endif
		
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
	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkers = new array<ref ExpansionMapMarkerListEntry>();
		m_MemberMarkers = new array<ref ExpansionMapMarkerListEntry>();
	#endif
		m_ServerMarkers = new array<ref ExpansionMapMarkerListEntry>();
		
		string arrow = ExpansionIcons.GetPath( "Arrow" );

		m_PersonalMarkersToggleIcon.LoadImageFile( 0, arrow );
		#ifdef EXPANSIONMODGROUPS
		m_PartyMarkersToggleIcon.LoadImageFile( 0, arrow );
		#endif
		m_ServerMarkersToggleIcon.LoadImageFile( 0, arrow );
		
		m_PersonalMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_2D_OFF );
		m_PersonalMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_2D_ON );
		m_PersonalMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_3D_OFF );
		m_PersonalMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_3D_ON );
	
	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_2D_OFF );
		m_PartyMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_2D_ON );
		m_PartyMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_3D_OFF );
		m_PartyMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_3D_ON );
		
		m_MemberMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_2D_OFF );
		m_MemberMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_2D_ON );
		m_MemberMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_3D_OFF );
		m_MemberMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_3D_ON );
	#endif
		
		m_ServerMarkers2DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_2D_OFF );
		m_ServerMarkers2DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_2D_ON );
		m_ServerMarkers3DToggleIcon.LoadImageFile( 0, EXPANSION_ICON_3D_OFF );
		m_ServerMarkers3DToggleIcon.LoadImageFile( 1, EXPANSION_ICON_3D_ON );
		
		m_InfoTooltip = new ExpansionUITooltip( "<p>#STR_EXPANSION_MARKERLIST_INFO_1</p><p>#STR_EXPANSION_MARKERLIST_INFO_2</p><p>#STR_EXPANSION_MARKERLIST_INFO_3</p><p>#STR_EXPANSION_MARKERLIST_INFO_4</p>" );
		m_InfoTooltip.SetFixPosition( true );
		m_InfoTooltip.SetOffset( 45, -2 );
		
		UpdateToggleStates();

		if (m_MarkerListInitState == false)
			m_MarkerListState = GetExpansionClientSettings().ShowMapMarkerList;
		
		// Allows for persistent marker list state
		ShowPanel( m_MarkerListState );
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::Init - End");
		#endif
	}
	
	static void Sort(array<ref ExpansionMapMarkerListEntry> listEntries)
	{
		TStringArray markerNames = new TStringArray;

		foreach (ExpansionMapMarkerListEntry entry: listEntries)
		{
			markerNames.Insert(entry.GetData().GetName());
		}

		markerNames.Sort();

		foreach (ExpansionMapMarkerListEntry currentEntry: listEntries)
		{
			string name = currentEntry.GetData().GetName();
			int index = markerNames.Find(name);
			currentEntry.SetSort(index, false);
		}
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
		
		Sort(m_PersonalMarkers);

		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPersonalEntry - End");
		#endif
	}

	void UpdatePersonalEntry(ExpansionMapMarkerListEntry listEntry)
	{
		listEntry.Update();
		Sort(m_PersonalMarkers);
	}

	// ------------------------------------------------------------
	// RemovePersonalEntry
	// ------------------------------------------------------------	
	void RemovePersonalEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - Start");
		#endif
		
		for ( int i = m_PersonalMarkers.Count() - 1; i >= 0 ; --i )
		{
			if ( m_PersonalMarkers[i].GetMarker() == marker )
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - Remove entry [" + i + "] from m_PersonalMarkers");
				#endif
				
				m_PersonalMarkers.RemoveOrdered( i );
				break;
			}
		}
		
		m_MapMenu.Refresh();
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePersonalEntry - End");
		#endif
	}
	
	ExpansionMapMarkerListEntry GetEntry(ExpansionMapMarker marker, array<ref ExpansionMapMarkerListEntry> listEntries)
	{
		foreach (ExpansionMapMarkerListEntry listEntry: listEntries)
		{
			if ( listEntry && listEntry.GetMarker() == marker )
				return listEntry;
		}

		return NULL;
	}

	ExpansionMapMarkerListEntry GetPersonalEntry(ExpansionMapMarker marker)
	{
		return GetEntry(marker, m_PersonalMarkers);
	}

	bool HasPersonalEntry(ExpansionMapMarker marker)
	{
		return GetPersonalEntry(marker) != NULL;
	}

#ifdef EXPANSIONMODGROUPS
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
		
		Sort(m_PartyMarkers);
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddPartyEntry - End");
		#endif
	}

	void UpdatePartyEntry(ExpansionMapMarkerListEntry listEntry)
	{
		listEntry.Update();
		Sort(m_PartyMarkers);
	}

	// ------------------------------------------------------------
	// RemovePartyEntry
	// ------------------------------------------------------------	
	void RemovePartyEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - Start");
		#endif
		
		for ( int i = m_PartyMarkers.Count() - 1; i >= 0; --i )
		{
			if ( m_PartyMarkers[i].GetMarker() == marker )
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - Remove entry [" + i + "] from m_PartyMarkers");
				#endif
				
				m_PartyMarkers.RemoveOrdered( i );
				break;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemovePartyEntry - End");
		#endif
	}

	ExpansionMapMarkerListEntry GetPartyEntry(ExpansionMapMarker marker)
	{
		return GetEntry(marker, m_PartyMarkers);
	}
	
	bool HasPartyEntry(ExpansionMapMarker marker)
	{
		return GetPartyEntry(marker) != NULL;
	}
	
	// ------------------------------------------------------------
	// AddMemberEntry
	// ------------------------------------------------------------
	void AddMemberEntry(ExpansionMapMarker marker)
	{
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddMemberEntry - Start");
		#endif
		
		if ( marker.GetMarkerData() )
		{
			ExpansionMapMarkerListEntry entry = new ExpansionMapMarkerListEntry(m_MapMenu, m_MemberMarkersContent, marker);
			m_MemberMarkers.Insert(entry);
		
			Sort(m_MemberMarkers);
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::AddMemberEntry - End");
		#endif
	}

	void UpdateMemberEntry(ExpansionMapMarkerListEntry listEntry)
	{
		listEntry.Update();
		Sort(m_MemberMarkers);
	}

	// ------------------------------------------------------------
	// RemoveMemberEntry
	// ------------------------------------------------------------	
	void RemoveMemberEntry(ExpansionMapMarker marker)
	{	
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - Start");
		#endif
		
		for ( int i = m_MemberMarkers.Count() - 1; i >= 0 ; --i )
		{
			if ( m_MemberMarkers[i].GetMarker() == marker )
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - Remove entry [" + i + "] from m_MemberMarkers");
				#endif
				
				m_MemberMarkers.RemoveOrdered( i );
				break;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveMemberEntry - End");
		#endif
	}

	ExpansionMapMarkerListEntry GetMemberEntry(ExpansionMapMarker marker)
	{
		return GetEntry(marker, m_MemberMarkers);
	}
	
	bool HasMemberEntry(ExpansionMapMarker marker)
	{
		return GetMemberEntry(marker) != NULL;
	}
#endif

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
		
		for ( int i = m_ServerMarkers.Count() - 1; i >= 0 ; --i )
		{
			if ( m_ServerMarkers[i].GetMarker() == marker )
			{
				#ifdef EXPANSION_MAP_MENU_DEBUG
				EXLogPrint("ExpansionMapMarkerList::RemoveServerEntry - Remove entry [" + i + "] from m_ServerMarkers");
				#endif
				
				m_ServerMarkers.RemoveOrdered( i );
				break;
			}
		}
		
		#ifdef EXPANSION_MAP_MENU_DEBUG
		EXLogPrint("ExpansionMapMarkerList::RemoveServerEntry - End");
		#endif
	}

	ExpansionMapMarkerListEntry GetServerEntry(ExpansionMapMarker marker)
	{
		return GetEntry(marker, m_ServerMarkers);
	}
	
	bool HasServerEntry(ExpansionMapMarker marker)
	{
		return GetServerEntry(marker) != NULL;
	}
	
	// ------------------------------------------------------------
	// RemoveMemberEntry
	// ------------------------------------------------------------
	void RemoveEntry( ExpansionMapMarker marker )
	{
		if (!marker)
			return;
		
		switch (marker.GetMarkerData().GetType())
		{	
			case ExpansionMapMarkerType.PERSONAL:
				RemovePersonalEntry(marker);
			break;		
			case ExpansionMapMarkerType.SERVER:
				RemoveServerEntry(marker);
			break;
		#ifdef EXPANSIONMODGROUPS
			case ExpansionMapMarkerType.PARTY:
				RemovePartyEntry(marker);
			break;
			case ExpansionMapMarkerType.PLAYER:
				RemoveMemberEntry(marker);
			break;
		#endif
		}
		
		m_Root.Update();
	}
	
	// ------------------------------------------------------------
	// UpdateToggleStates
	// ------------------------------------------------------------
	void UpdateToggleStates()
	{
		m_PersonalMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PERSONAL ) );
		m_PersonalMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PERSONAL ) );
		
	#ifdef EXPANSIONMODGROUPS
		m_PartyMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PARTY ) );
		m_PartyMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PARTY ) );

		m_MemberMarkers2DToggleIcon.SetImage( m_MarkerModule.IsMapVisible( ExpansionMapMarkerType.PLAYER ) );
		m_MemberMarkers3DToggleIcon.SetImage( m_MarkerModule.IsWorldVisible( ExpansionMapMarkerType.PLAYER ) );
	#endif
		
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
			case m_ServerMarkersToggle:
				ShowServer( !m_ServerMarkersContent.IsVisible() );
				break;
		#ifdef EXPANSIONMODGROUPS
			case m_PartyMarkersToggle:
				ShowParty( !m_PartyMarkersContent.IsVisible() );
				break;
			case m_MemberMarkersToggle:
				ShowMembers( !m_MemberMarkersContent.IsVisible() );
				break;
		#endif
			case m_PersonalMarkers2DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP );
				break;
			case m_PersonalMarkers3DToggle:
				m_MarkerModule.FlipVisibility( ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD );
				break;
		#ifdef EXPANSIONMODGROUPS
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
			#endif
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

		m_MarkerListInitState = true;
		
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

#ifdef EXPANSIONMODGROUPS
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
#endif
	
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
	array<ref ExpansionMapMarkerListEntry> GetPersonalEntrys()
	{
		return m_PersonalMarkers; 
	}
	
#ifdef EXPANSIONMODGROUPS
	// ------------------------------------------------------------
	// Expansion GetPartyEntrys
	// ------------------------------------------------------------	
	array<ref ExpansionMapMarkerListEntry> GetPartyEntrys()
	{
		return m_PartyMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion GetPartyEntrys
	// ------------------------------------------------------------	
	array<ref ExpansionMapMarkerListEntry> GetMemberEntrys()
	{
		return m_MemberMarkers; 
	}
	
	// ------------------------------------------------------------
	// Expansion GetServerEntrys
	// ------------------------------------------------------------	
	array<ref ExpansionMapMarkerListEntry> GetServerEntrys()
	{
		return m_ServerMarkers; 
	}
#endif
	
	// ------------------------------------------------------------
	// Expansion IsListVisible
	// ------------------------------------------------------------	
	bool IsListVisible()
	{
		return m_MarkerListState;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{
#ifdef EXPANSIONMODGROUPS
		if (w == m_PersonalMarkersToggle || w == m_PartyMarkersToggle || w == m_ServerMarkersToggle || w == m_MemberMarkersToggle)
#else
		if (w == m_PersonalMarkersToggle || w == m_ServerMarkersToggle)
#endif
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
#ifdef EXPANSIONMODGROUPS
		if (w == m_PersonalMarkersToggle || w == m_PartyMarkersToggle || w == m_ServerMarkersToggle || w == m_MemberMarkersToggle)
#else
		if (w == m_PersonalMarkersToggle || w == m_ServerMarkersToggle)
#endif
		{
			w.SetColor(COLOR_NOFOCUS);
			return true;
		} else if (w == m_MarkerListInfoPanel)
		{
			m_MarkerListInfoIcon.SetColor(ARGB(140,220,220,220));
			if (m_InfoTooltip.IsVisible())
			{
				m_InfoTooltip.HideTooltip();
			}
			return true;
		} else if (w == m_MarkerListToggle)
		{
			m_MarkerListToggleIcon.SetColor(ARGB(255,220,220,220));
			return true;
		}

		if ( w.GetName().IndexOf( "visibility_marker_option_" ) == 0 )
		{
			w.GetChildren().SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}
		
		//SetFocus(null);
		
		return false;
	}
	
	// ------------------------------------------------------------
	// HideTooltips
	// ------------------------------------------------------------	
	void HideTooltips()
	{
		if ( m_InfoTooltip.IsVisible() )
			m_InfoTooltip.HideTooltip();
		
		int i;
		for ( i = 0; i < m_PersonalMarkers.Count(); ++i )
		{
			if(m_PersonalMarkers[i].GetTooltip().IsVisible())
			{				
				m_PersonalMarkers[i].GetTooltip().HideTooltip();
			}
		}
		
	#ifdef EXPANSIONMODGROUPS
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
	#endif
		
		for ( i = 0; i < m_ServerMarkers.Count(); ++i )
		{
			if(m_ServerMarkers[i].GetTooltip().IsVisible())
			{				
				m_ServerMarkers[i].GetTooltip().HideTooltip();
			}
		}
	}
}