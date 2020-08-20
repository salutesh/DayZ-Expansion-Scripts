/**
 * ExpansionCOTTerritoriesMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesMenu: JMFormBase
{
	private ExpansionCOTTerritoriesModule m_Module;
	
	private Widget m_TerritoryMapPanel;
	private MapWidget m_TerritoryMap;

	private Widget m_TerritoryListPanel;
	private GridSpacerWidget m_TerritoryListContent;
	
	private ButtonWidget m_TerritoryListRefreshButton;
	private TextWidget m_TerritoryListRefreshButtonLable;
	
	private Widget m_TerritoryInfoPanel;
	private TextWidget m_InfoName;
	private TextWidget m_InfoTerritoryID;
	private TextWidget m_InfoOwnerID;
	private TextWidget m_InfoPosition;
	private TextWidget m_InfoLevel;
	
	private Widget m_TerritoryInfoButtonsPanel;
	private WrapSpacerWidget m_TerritoryInfoButtonsContent;
	private ButtonWidget m_TeleportTerritoryButton;
	private TextWidget m_TeleportTerritoryButtonLable;
	private ButtonWidget m_DeleteTerritoryButton;
	private TextWidget m_DeleteTerritoryButtonLable;
	private ButtonWidget m_CancelEditButton;
	private TextWidget m_CancelEditButtonLable;
	
	private Widget m_TerritoryMembersPanel;
	private GridSpacerWidget m_TerritoryMembersContent;
	
	private Widget m_TerritoryMemberInfoPanel;
	private TextWidget m_MemberName;
	private TextWidget m_MemberID;
	private TextWidget m_MemberRank;
	private ImageWidget m_MemberStatusIcon;
	private TextWidget m_MemberStatus;
	
	private Widget m_TerritoryMemberInfoButtonsPanel;
	private ButtonWidget m_CancelMemberEditButton;
	private TextWidget m_CancelMemberEditButtonLable;
		
	private Widget m_ObjectContainerPanel;
	private GridSpacerWidget m_ObjectContainerContent;
	
	private ButtonWidget m_ObjectContainerListRefreshButton;
	private TextWidget m_ObjectContainerListRefreshButtonLable;
	
	private Widget m_TerritoryObjectsPanel;
	private GridSpacerWidget m_TerritoryObjectsContent;
	
	private ButtonWidget m_ObjectsListRefreshButton;
	private TextWidget m_ObjectsListRefreshButtonLable;
	
	private Widget m_ObjectInfoPanel;
	private TextWidget m_ObjectName;
	private TextWidget m_ObjectID;
		
	private Widget m_ObjectButtonsPanel; 
	private ButtonWidget m_DeleteObjectButton;
	private TextWidget m_DeleteObjectButtonLable;
	private ButtonWidget m_CancelObjectEditButton;
	private TextWidget m_CancelObjectEditButtonLable;
	
	private ref array<ref ExpansionCOTTerritoriesMapMarker> m_MapMarkers;
	
	private ref array<ref ExpansionCOTTerritoriesListEntry> m_TerritoryEntries;
	private ref array<ref ExpansionCOTTerritoriesMemberListEntry> m_MemberListEntries;
	
	private ref array<ref ExpansionEntityMetaData> m_TerritoryObjects;
	private ref array<ref ExpansionCOTTerritoriesObjectsListEntry> m_ObjectListEntries;
	private ref array<ref ExpansionCOTTerritoriesContainerListEntry> m_ContainerListEntries;
	private ref array<ref ExpansionEntityMetaData> m_ContainerObjects;
	
	private ref ExpansionTerritory m_CurrentTerritory;
	private ref ExpansionEntityMetaData m_CurrentTerritoryFlag;
	private ref ExpansionTerritoryMember m_CurrentMember;
	private ref ExpansionEntityMetaData m_CurrentObject;
	
	static ref ScriptInvoker m_COTTerritorySetTerritorysInvoker = new ScriptInvoker();
	static ref ScriptInvoker m_COTTerritorySetObjectsInvoker = new ScriptInvoker();
	static ref ScriptInvoker m_COTTerritorySetContainerObjectsInvoker = new ScriptInvoker();
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMenu Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMenu()
	{		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::ExpansionCOTTerritoriesMenu - Start" );
		#endif
		
		m_MapMarkers =  new array<ref ExpansionCOTTerritoriesMapMarker>;
		m_TerritoryEntries = new array<ref ExpansionCOTTerritoriesListEntry>;
		m_MemberListEntries = new array<ref ExpansionCOTTerritoriesMemberListEntry>;
		m_TerritoryObjects = new array<ref ExpansionEntityMetaData>;
		m_ObjectListEntries = new array<ref ExpansionCOTTerritoriesObjectsListEntry>;
		m_ContainerListEntries = new array<ref ExpansionCOTTerritoriesContainerListEntry>;
		m_ContainerObjects = new array<ref ExpansionEntityMetaData>;
		
		m_CurrentTerritory = null;
		m_CurrentTerritoryFlag = null;
		m_CurrentMember = null;
		m_CurrentObject = null;
		
		m_COTTerritorySetTerritorysInvoker.Insert( SetTerritories );
		m_COTTerritorySetObjectsInvoker.Insert( SetTerritoryObjects );
		m_COTTerritorySetContainerObjectsInvoker.Insert( SetContainerObjects );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::ExpansionCOTTerritoriesMenu - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMenu Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesMenu()
	{
	}
	
	// ------------------------------------------------------------
	// Expansion SetModule
	// ------------------------------------------------------------
	protected override bool SetModule( ref JMRenderableModuleBase mdl )
	{
		return Class.CastTo( m_Module, mdl );
	}
	
	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::OnInit - Start" );
		#endif
		
		m_TerritoryMapPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territories_map_panel" ) );	
		m_TerritoryMap = MapWidget.Cast( layoutRoot.FindAnyWidget( "territories_map" ) );	
		
		m_TerritoryListPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territories_list_panel" ) );	
		m_TerritoryListContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "territories_list_content" ) );	
		
		m_TerritoryListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "territories_refresh_button" ) );
		m_TerritoryListRefreshButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "territories_refresh_button_lable" ) );
		
		m_TerritoryInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_info_panel" ) );
		m_InfoName = TextWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_name_value" ) );
		m_InfoTerritoryID = TextWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_id_value" ) );
		m_InfoOwnerID = TextWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_ownerid_value" ) );
		m_InfoPosition = TextWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_pos_value" ) );
		m_InfoLevel = TextWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_level_value" ) );
		
		m_TerritoryInfoButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_info_buttons_panel" ) );
		m_TerritoryInfoButtonsContent = WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_buttons_spacer" ) );
		
		m_DeleteTerritoryButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_territory_button" ) );
		m_DeleteTerritoryButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "delete_vehicle_button_label" ) );
		
		m_TeleportTerritoryButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "teleport_to_button" ) );
		m_TeleportTerritoryButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "teleport_to_button_label" ) );
		
		m_CancelEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_edit_button" ) );
		m_CancelEditButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "cancel_edit_button_label" ) );
		
		m_TerritoryMembersPanel = Widget.Cast( layoutRoot.FindAnyWidget( "members_list_panel" ) );
		m_TerritoryMembersContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "members_list_content" ) );
		
		m_TerritoryMemberInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "member_info_panel" ) ); 
		m_MemberName = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_name_value" ) );		
		m_MemberID = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_id_value" ) );
		m_MemberRank = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_rank_value" ) );
		m_MemberStatusIcon = ImageWidget.Cast( layoutRoot.FindAnyWidget( "member_info_status_icon" ) );
		m_MemberStatus = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_status_value" ) );
		
		m_TerritoryMemberInfoButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "member_info_buttons_panel" ) ); 
		
		m_CancelMemberEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_member_edit_button" ) );
		m_CancelMemberEditButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "cancel_member_edit_button_label" ) );
		
		m_ObjectContainerPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_container_list_panel" ) );
		m_ObjectContainerContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "object_container_list_content" ) );
		
		m_ObjectContainerListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "object_container_refresh_button" ) );
		m_ObjectContainerListRefreshButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "object_container_refresh_button_lable" ) );
		m_ObjectContainerListRefreshButton.Show(false); //! DOES NOT WORK YET
		
		m_TerritoryObjectsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "objects_list_panel" ) );
		m_TerritoryObjectsContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "objects_list_content" ) );
		
		m_ObjectsListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "objects_refresh_button" ) );
		m_ObjectsListRefreshButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "objects_refresh_button_lable" ) );
		
		m_ObjectInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_info_panel" ) );
		m_ObjectName = TextWidget.Cast( layoutRoot.FindAnyWidget( "object_info_name_value" ) );
		m_ObjectID = TextWidget.Cast( layoutRoot.FindAnyWidget( "object_info_id_value" ) );
		
		m_ObjectButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_info_buttons_panel" ) ); 
		
		m_DeleteObjectButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_object_button" ) );
		m_DeleteObjectButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "delete_object_button_label" ) );
		
		m_CancelObjectEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_object_edit_button" ) );
		m_CancelObjectEditButtonLable = TextWidget.Cast( layoutRoot.FindAnyWidget( "cancel_object_edit_button_label" ) );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::OnInit - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritories
	// ------------------------------------------------------------
	void SetTerritories( array<ref ExpansionTerritory> territories )
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritories - Start" );
		#endif
		
		m_MapMarkers.Clear();
		m_TerritoryEntries.Clear();
		
		for ( int i = 0; i < territories.Count(); i++ )
		{
			ref ExpansionTerritory currentTerritory = territories[i];
			
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritories - currentTerritory is: " + currentTerritory.ToString() );
			#endif
				
			vector pos = currentTerritory.GetPosition();
			string ownerID = currentTerritory.GetOwnerID();
			string name = currentTerritory.GetTerritoryName();
			
			//! Create map marker for territory
			ExpansionCOTTerritoriesMapMarker territoryMapMarker = new ExpansionCOTTerritoriesMapMarker( layoutRoot, m_TerritoryMap, pos, name, ARGB( 255, 75, 123, 236 ), "DayZExpansion\\GUI\\icons\\marker\\marker_home.paa", currentTerritory, this );
			m_MapMarkers.Insert( territoryMapMarker );
			
			//! Create list entry for territory
			ExpansionCOTTerritoriesListEntry territoryListEntry = new ExpansionCOTTerritoriesListEntry(m_TerritoryListContent, this, currentTerritory);
			m_TerritoryEntries.Insert( territoryListEntry );
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritories - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryObjects
	// ------------------------------------------------------------
	void SetTerritoryObjects(array<ref ExpansionEntityMetaData> objects_datas)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryObjects - Start" );
		#endif
		
		m_ObjectListEntries.Clear();
		m_TerritoryObjects.Clear();
		
		for ( int i = 0; i < objects_datas.Count(); i++ )
		{
			ref ExpansionEntityMetaData obj = objects_datas.Get( i );
			ExpansionCOTTerritoriesObjectsListEntry objectListEntry = new ExpansionCOTTerritoriesObjectsListEntry( m_TerritoryObjectsContent, this, obj );
			m_ObjectListEntries.Insert( objectListEntry );
			m_TerritoryObjects.Insert( obj );
			
			if ( obj.m_ClassName == "ExpansionTerritoryFlag" )
			{
				m_CurrentTerritoryFlag = obj;
			}
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryObjects - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetContainerObjects
	// ------------------------------------------------------------
	void SetContainerObjects(ref ExpansionEntityMetaData object)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetContainerObjects - Start" );
		#endif
		
		for ( int i = 0; i < m_TerritoryObjects.Count(); i++ )
		{
			ref ExpansionEntityMetaData obj = m_TerritoryObjects[i];
			if (!obj)
			{
				#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
				EXLogPrint( "ExpansionCOTTerritoriesMenu::SetContainerObjects - [ERROR]: ExpansionEntityMetaData is NULL" );
				#endif
				continue;
			}
			
			if ( obj.m_ID == object.m_ID )
			{
				delete obj;
				m_TerritoryObjects.Insert( object );
			}
		}
		
		m_CurrentObject = object;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetContainerObjects - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// DeleteTerritory
	// ------------------------------------------------------------	
	private void DeleteTerritory()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::DeleteTerritory - Start" );
		#endif
		
		if ( m_CurrentTerritory )
		{
			ExpansionTerritoryModule territories_module = ExpansionTerritoryModule.Cast( GetModuleManager().GetModule(ExpansionTerritoryModule) );
			if (!territories_module)
				return;
			
			territories_module.DeleteTerritoryAdmin( m_CurrentTerritory.GetTerritoryID() );
			
			m_Module.RequestServerTerritories();
			
			m_TerritoryListPanel.Show( true );
			m_TerritoryMapPanel.Show( true );
			m_TerritoryMemberInfoPanel.Show( false );
			m_TerritoryMemberInfoButtonsPanel.Show( false );
			m_TerritoryInfoPanel.Show( false );
			m_TerritoryMembersPanel.Show( false );
			m_TerritoryInfoButtonsPanel.Show( false );
			m_TerritoryObjectsPanel.Show( false );
			
			m_MemberListEntries.Clear();
			
			m_CurrentTerritory = NULL;
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::DeleteTerritory - End" );
		#endif
	}

	// ------------------------------------------------------------
	// CancelTerritoryEdit
	// ------------------------------------------------------------	
	private void CancelTerritoryEdit()
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::CancelTerritoryEdit - Start" );
		#endif
		
		foreach( ExpansionCOTTerritoriesMapMarker marker : m_MapMarkers )
		{
			marker.ShowMarker();
		}	
		
		m_TerritoryListPanel.Show( true );
		m_TerritoryMapPanel.Show( true );
		m_TerritoryInfoPanel.Show( false );
		
		if (m_TerritoryMemberInfoPanel.IsVisible())
			m_TerritoryMemberInfoPanel.Show( false );
		
		if (m_TerritoryMembersPanel.IsVisible())
			m_TerritoryMembersPanel.Show( false );
		
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryObjectsPanel.Show( false );
		m_InfoName.SetText( "" );
		m_InfoTerritoryID.SetText( "" );
		m_InfoOwnerID.SetText( "" );
		m_InfoPosition.SetText( "" );
		m_InfoLevel.SetText( "" );
		
		m_MemberListEntries.Clear();
		
		m_CurrentTerritory = NULL;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::CancelTerritoryEdit - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// CancelMemberEdit
	// ------------------------------------------------------------	
	private void CancelMemberEdit()
	{
		m_TerritoryMemberInfoPanel.Show( false );
		m_TerritoryMemberInfoButtonsPanel.Show( false );
		m_TerritoryInfoPanel.Show( true );
		m_TerritoryInfoButtonsPanel.Show( true );
		m_TerritoryObjectsPanel.Show( true );
		m_TerritoryMembersPanel.Show( true );
		
		m_CurrentMember = null;
	}
	
	// ------------------------------------------------------------
	// CancelObjectEdit
	// ------------------------------------------------------------		
	private void CancelObjectEdit()
	{
		m_ObjectInfoPanel.Show( false );
		m_ObjectButtonsPanel.Show( false );
		m_TerritoryInfoPanel.Show( true );
		m_TerritoryInfoButtonsPanel.Show( true );
		m_TerritoryObjectsPanel.Show( true );
		m_TerritoryMembersPanel.Show( true );
		
		if ( m_ObjectContainerPanel.IsVisible() )
		{
			m_ObjectContainerPanel.Show( false );
		}
		
		m_CurrentObject = null;
	}
	
	// ------------------------------------------------------------
	// DeleteObject
	// ------------------------------------------------------------		
	private void DeleteObject()
	{
		if ( m_CurrentObject )
		{
			m_Module.DeleteObject( m_CurrentObject.m_NetworkIDLow, m_CurrentObject.m_NetworkIDHigh );
			
			m_ObjectInfoPanel.Show( false );
			m_ObjectButtonsPanel.Show( false );
			m_TerritoryInfoPanel.Show( true );
			m_TerritoryInfoButtonsPanel.Show( true );
			m_TerritoryObjectsPanel.Show( true );
			m_TerritoryMembersPanel.Show( true );
			
			m_Module.RequestTerritoryObjects( m_CurrentTerritory.GetPosition() );
		}
		
		m_CurrentObject = null;
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_DeleteTerritoryButton )
		{
			DeleteTerritory();
		}
		
		if ( w == m_CancelEditButton )
		{
			CancelTerritoryEdit();
		}
		
		if ( w == m_CancelMemberEditButton )
		{
			CancelMemberEdit();
		}
		
		if ( w == m_TerritoryListRefreshButton )
		{
			m_Module.RequestServerTerritories();
		}
		
		if ( w == m_CancelObjectEditButton )
		{
			CancelObjectEdit();
		}
		
		if ( w == m_DeleteObjectButton )
		{
			DeleteObject();
		}
		
		if ( w == m_ObjectsListRefreshButton )
		{
			if (m_CurrentTerritory) 
				m_Module.RequestTerritoryObjects( m_CurrentTerritory.GetPosition() );
		}
		
		//! TODO: Fix me!
		/*if ( w == m_ObjectContainerListRefreshButton )
		{
			if (m_CurrentObject)
				m_Module.RequestUpdateObjectData( m_CurrentObject.m_NetworkIDLow, m_CurrentObject.m_NetworkIDHigh );
		}*/
		
		//! TODO: Tweak me!
		if ( w == m_TeleportTerritoryButton )
		{
			if (m_CurrentTerritory)
				m_Module.RequestTeleportToTerritory( m_CurrentTerritoryFlag.m_NetworkIDLow, m_CurrentTerritoryFlag.m_NetworkIDHigh );
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();
		
		m_Module.RequestServerTerritories();
	}
	
	// ------------------------------------------------------------
	// Expansion OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryInfo
	// ------------------------------------------------------------
	void SetTerritoryInfo(ExpansionTerritory territory)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryInfo - Start" );
		#endif
		
		if (m_MemberListEntries.Count() > 0 )
			m_MemberListEntries.Clear();
	
		if (m_ObjectListEntries.Count() > 0 )
			m_ObjectListEntries.Clear();
		
		if (m_TerritoryObjects.Count() > 0 )
			m_TerritoryObjects.Clear();
		
		for ( int i = 0; i < m_MapMarkers.Count(); i++ )
		{
			ExpansionCOTTerritoriesMapMarker marker = m_MapMarkers.Get( i );
			marker.HideMarker();
		}
		
		m_TerritoryListPanel.Show( false );
		m_TerritoryMapPanel.Show( false );
		m_TerritoryMemberInfoPanel.Show( false );
		m_TerritoryMemberInfoButtonsPanel.Show( false );
		m_TerritoryInfoPanel.Show( true );
		m_TerritoryMembersPanel.Show( true );
		m_TerritoryInfoButtonsPanel.Show( true );
		m_TerritoryObjectsPanel.Show( true );
		
		m_InfoName.SetText( territory.GetTerritoryName() );
		m_InfoTerritoryID.SetText( territory.GetTerritoryID().ToString() );
		m_InfoOwnerID.SetText( territory.GetOwnerID() );
		m_InfoPosition.SetText( "X: " + territory.GetPosition()[0].ToString() + " Y: " + territory.GetPosition()[2].ToString() );
		m_InfoLevel.SetText( territory.GetTerritoryLevel().ToString() );
		
		m_CurrentTerritory = territory;
		
		CreateTerritorieMembersList( territory );
		
		m_Module.RequestTerritoryObjects( territory.GetPosition() );
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryInfo - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateTerritorieMembersList
	// ------------------------------------------------------------
	void CreateTerritorieMembersList(ExpansionTerritory territory)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::CreateTerritorieMembersList - Start" );
		#endif
		
		m_MemberListEntries.Clear();
		
		if ( territory.GetTerritoryMembers().Count() > 0 )
		{
			for ( int j = 0; j < territory.GetTerritoryMembers().Count(); j++ )
			{
				ExpansionTerritoryMember member = territory.GetTerritoryMembers().Get( j );
				if ( member )
				{
					ExpansionCOTTerritoriesMemberListEntry memberListEntry = new ExpansionCOTTerritoriesMemberListEntry( m_TerritoryMembersContent, this, member );
					m_MemberListEntries.Insert( memberListEntry );
				}
			}
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::CreateTerritorieMembersList - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryMemberInfo
	// ------------------------------------------------------------
	void SetTerritoryMemberInfo(ExpansionTerritoryMember member)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryMemberInfo - Start" );
		#endif
		
		m_TerritoryListPanel.Show( false );
		m_TerritoryInfoPanel.Show( false );
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryMembersPanel.Show( false );
		
		if ( m_TerritoryObjectsPanel.IsVisible() )
			m_TerritoryObjectsPanel.Show( false );
		
		m_TerritoryMemberInfoPanel.Show( true );
		m_TerritoryMemberInfoButtonsPanel.Show( true );
		
		m_MemberName.SetText( member.GetName() );
		m_MemberID.SetText( member.GetID() );	
		m_MemberRank.SetText( member.GetRankName() );	
		
		if ( IsOnline( member.GetID() ) )
		{
			m_MemberStatus.SetText( "Online" );
			m_MemberStatusIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
		}
		else
		{
			m_MemberStatus.SetText( "Offline" );
			m_MemberStatusIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
		}
		
		m_CurrentMember = member;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryMemberInfo - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryObjectInfo
	// ------------------------------------------------------------
	void SetTerritoryObjectInfo(ref ExpansionEntityMetaData obj)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryObjectInfo - Start" );
		#endif
		
		m_TerritoryListPanel.Show( false );
		m_TerritoryInfoPanel.Show( false );
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryMembersPanel.Show( false );
		m_TerritoryMemberInfoPanel.Show( false );
		m_TerritoryMemberInfoButtonsPanel.Show( false );
		m_TerritoryObjectsPanel.Show( false );
		m_ObjectInfoPanel.Show( true );
		m_ObjectButtonsPanel.Show( true );
		
		string name;
		name = obj.m_DisplayName;
		if (name == "")
			name = obj.m_ClassName;
				
		m_ObjectName.SetText( name );
		m_ObjectID.SetText( obj.m_ID.ToString() );
		
		if (obj.m_ContainerObj.Count() > 0)
		{
			m_ContainerListEntries.Clear();
			m_ContainerObjects.Clear();
			
			for ( int k = 0; k < obj.m_ContainerObj.Count(); k++ )
			{
				ref ExpansionEntityMetaData container_obj = obj.m_ContainerObj.Get( k );
				ExpansionCOTTerritoriesContainerListEntry itemListEntry = new ExpansionCOTTerritoriesContainerListEntry( m_ObjectContainerContent, this, container_obj );
				m_ContainerListEntries.Insert( itemListEntry );
				m_ContainerObjects.Insert( container_obj );
			}
			
			m_ObjectContainerPanel.Show( true );
		}
		
		m_CurrentObject = obj;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetTerritoryObjectInfo - End" );
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion SetContainerObjectInfo
	// ------------------------------------------------------------
	void SetContainerObjectInfo(ref ExpansionEntityMetaData obj)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetContainerObjectInfo - Start" );
		#endif
		
		m_ObjectContainerPanel.Show( false );
				
		string name;
		name = obj.m_DisplayName;
		if (name == "")
			name = obj.m_ClassName;
				
		m_ObjectName.SetText( name );
		m_ObjectID.SetText( obj.m_ID.ToString() );	
		
		m_CurrentObject = obj;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint( "ExpansionCOTTerritoriesMenu::SetContainerObjectInfo - End" );
		#endif
	}

	// ------------------------------------------------------------
	// IsOnline
	// ------------------------------------------------------------
	bool IsOnline( string uid )
	{
		for ( int i = 0; i < ClientData.m_PlayerList.m_PlayerList.Count(); i++ )
		{
			ref SyncPlayer player = ClientData.m_PlayerList.m_PlayerList.Get( i );

			if ( player )
			{
				if ( player.m_RUID == uid )
				{
					return true;
				}
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( w == m_TerritoryListRefreshButton )
		{
			m_TerritoryListRefreshButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_ObjectsListRefreshButton )
		{
			m_ObjectsListRefreshButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_ObjectContainerListRefreshButton )
		{
			m_ObjectContainerListRefreshButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_DeleteTerritoryButton )
		{
			m_DeleteTerritoryButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_CancelEditButton )
		{
			m_CancelEditButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_TeleportTerritoryButton )
		{
			m_TeleportTerritoryButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_CancelMemberEditButton )
		{
			m_CancelMemberEditButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_DeleteObjectButton )
		{
			m_DeleteObjectButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}
		
		if ( w == m_CancelObjectEditButton )
		{
			m_CancelObjectEditButtonLable.SetColor( ARGB( 255, 0, 0, 0 ) );
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( w == m_TerritoryListRefreshButton )
		{
			m_TerritoryListRefreshButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_ObjectsListRefreshButton )
		{
			m_ObjectsListRefreshButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_ObjectContainerListRefreshButton )
		{
			m_ObjectContainerListRefreshButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_DeleteTerritoryButton )
		{
			m_DeleteTerritoryButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_CancelEditButton )
		{
			m_CancelEditButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_TeleportTerritoryButton )
		{
			m_TeleportTerritoryButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_CancelMemberEditButton )
		{
			m_CancelMemberEditButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_DeleteObjectButton )
		{
			m_DeleteObjectButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if ( w == m_CancelObjectEditButton )
		{
			m_CancelObjectEditButtonLable.SetColor( ARGB( 255, 255, 255, 255 ) );
		}

		return false;
	}
}