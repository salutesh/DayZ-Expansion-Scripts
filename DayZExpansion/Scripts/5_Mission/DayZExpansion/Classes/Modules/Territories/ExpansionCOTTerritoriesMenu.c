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
	
	private Widget m_TerritoryInfoPanel;
	private TextWidget m_InfoName;
	private TextWidget m_InfoTerritoryID;
	private TextWidget m_InfoOwnerID;
	private TextWidget m_InfoPosition;
	private TextWidget m_InfoLevel;
	
	private Widget m_TerritoryInfoButtonsPanel;
	private WrapSpacerWidget m_TerritoryInfoButtonsContent;
	private ButtonWidget m_DeleteTerritoryButton;
	private ButtonWidget m_CancelEditButton;
	
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
		
	private Widget m_ObjectContainerPanel;
	private GridSpacerWidget m_ObjectContainerContent;
	
	private Widget m_TerritoryObjectsPanel;
	private GridSpacerWidget m_TerritoryObjectsContent;
	
	private ButtonWidget m_TerritoryObjectsListRefreshButton;
	
	private Widget m_ObjectInfoPanel;
	private TextWidget m_ObjectName;
	private TextWidget m_ObjectID;
		
	private Widget m_ObjectButtonsPanel; 
	private ButtonWidget m_DeleteObjectButton;
	private ButtonWidget m_CancelObjectEditButton;
	
	private Widget m_ObjectPreviewPanel;
	private ItemPreviewWidget m_ObjectPreview; 
	
	private ExpansionTerritoryModule m_TerritoryModule;
	private ref array<ref ExpansionCOTTerritoriesMapMarker> m_MapMarkers;
	private ref array<ref ExpansionCOTTerritoriesListEntry> m_TerritoryEntries;
	private ref array<ref ExpansionCOTTerritoriesMemberListEntry> m_MemberListEntries;
	private ref ExpansionTerritory m_CurrentTerritory;
	private ref ExpansionTerritoryMember m_CurrentMember;
	private ref array<Object> m_TerritoryObjects;
	private ref array<ref ExpansionCOTTerritoriesObjectsListEntry> m_ObjectListEntries;
	private ref array<ref ExpansionCOTTerritoriesContainerListEntry> m_ContainerListEntries;
	private ref array<EntityAI> m_ContainerObjects;
	private Object m_CurrentObject;
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMenu Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMenu()
	{
		if ( !Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) ) )
			return;
		
		m_MapMarkers =  new array<ref ExpansionCOTTerritoriesMapMarker>;
		m_TerritoryEntries = new array<ref ExpansionCOTTerritoriesListEntry>;
		m_MemberListEntries = new array<ref ExpansionCOTTerritoriesMemberListEntry>;
		m_TerritoryObjects = new array<Object>;
		m_ObjectListEntries = new array<ref ExpansionCOTTerritoriesObjectsListEntry>;
		m_ContainerListEntries = new array<ref ExpansionCOTTerritoriesContainerListEntry>;
		m_ContainerObjects = new array<EntityAI>;
		
		m_CurrentTerritory = null;
		m_CurrentMember = null;
		m_CurrentObject = null;
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
		m_TerritoryMapPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_map_panel" ) );	
		m_TerritoryMap = MapWidget.Cast( layoutRoot.FindAnyWidget( "territory_map" ) );	
		
		m_TerritoryListPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_list_panel" ) );	
		m_TerritoryListContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "territory_list_content" ) );	
		
		m_TerritoryListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "territory_refresh_button" ) );
		
		m_TerritoryInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_info_panel" ) );
		m_InfoName = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_name_value" ) );
		m_InfoTerritoryID = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_id_value" ) );
		m_InfoOwnerID = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_ownerid_value" ) );
		m_InfoPosition = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_position_value" ) );
		m_InfoLevel = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_level_value" ) );
		
		m_TerritoryInfoButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "territory_info_buttons_panel" ) );
		m_TerritoryInfoButtonsContent = WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget( "territory_info_buttons_spacer" ) );
		m_DeleteTerritoryButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_territory_button" ) );
		m_CancelEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_edit_button" ) );
		
		m_TerritoryMembersPanel = Widget.Cast( layoutRoot.FindAnyWidget( "member_list_panel" ) );
		m_TerritoryMembersContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "member_list_content" ) );
		
		m_TerritoryMemberInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "member_info_panel" ) ); 
		m_MemberName = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_name_value" ) );		
		m_MemberID = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_id_value" ) );
		m_MemberRank = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_rank_value" ) );
		m_MemberStatusIcon = ImageWidget.Cast( layoutRoot.FindAnyWidget( "member_info_status_icon" ) );
		m_MemberStatus = TextWidget.Cast( layoutRoot.FindAnyWidget( "member_info_status_value" ) );
		
		m_TerritoryMemberInfoButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "member_info_buttons_panel" ) ); 
		m_CancelMemberEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_member_edit_button" ) );
		
		m_ObjectContainerPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_container_list_panel" ) );
		m_ObjectContainerContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "object_container_list_content" ) );
		
		m_TerritoryObjectsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_list_panel" ) );
		m_TerritoryObjectsContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "object_list_content" ) );
		
		m_TerritoryObjectsListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "objects_refresh_button" ) );
		
		m_ObjectInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_info_panel" ) );
		m_ObjectName = TextWidget.Cast( layoutRoot.FindAnyWidget( "object_info_name_value" ) );
		m_ObjectID = TextWidget.Cast( layoutRoot.FindAnyWidget( "object_info_id_value" ) );
		
		m_ObjectButtonsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_info_buttons_panel" ) ); 
		m_DeleteObjectButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_object_button" ) );
		m_CancelObjectEditButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_object_edit_button" ) );
		
		m_ObjectPreviewPanel = Widget.Cast( layoutRoot.FindAnyWidget( "object_preview_panel" ) );
		m_ObjectPreview = ItemPreviewWidget.Cast( layoutRoot.FindAnyWidget( "object_preview" ) );
	}

	// ------------------------------------------------------------
	// Expansion LoadTerritories
	// ------------------------------------------------------------
	void LoadTerritories()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoriesCOTForm::LoadTerritories:: - Start");
		#endif
		
		m_MapMarkers.Clear();
		m_TerritoryEntries.Clear();
		
		if ( m_TerritoryModule && m_TerritoryModule.GetServerTerritories().Count() != 0 )
		{		
			for ( int i = 0; i < m_TerritoryModule.GetServerTerritories().Count(); i++ )
			{
				ExpansionTerritory currentTerritory = m_TerritoryModule.GetServerTerritories().Get( i );

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "ExpansionTerritoriesCOTForm::LoadTerritories:: - currentTerritory is: " + currentTerritory.ToString() );
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
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionTerritoriesCOTForm::LoadTerritories:: - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// DeleteTerritory
	// ------------------------------------------------------------	
	private void DeleteTerritory()
	{
		if ( m_CurrentTerritory )
		{
			m_TerritoryModule.DeleteTerritoryAdmin( m_CurrentTerritory.GetTerritoryID() );
			
			SyncAndRefreshTerritorys();
			
			m_TerritoryListPanel.Show( true );
			m_TerritoryMapPanel.Show( true );
			m_TerritoryMemberInfoPanel.Show( true );
			m_TerritoryMemberInfoButtonsPanel.Show( true );
			m_TerritoryInfoPanel.Show( false );
			m_TerritoryMembersPanel.Show( false );
			m_TerritoryInfoButtonsPanel.Show( false );
			m_TerritoryObjectsPanel.Show( false );
			
			m_MemberListEntries.Clear();
			
			m_CurrentTerritory = NULL;
		}
	}

	// ------------------------------------------------------------
	// CancelTerritoryEdit
	// ------------------------------------------------------------	
	private void CancelTerritoryEdit()
	{
		foreach( ExpansionCOTTerritoriesMapMarker marker : m_MapMarkers )
		{
			marker.ShowMarker();
		}	
		
		m_TerritoryListPanel.Show( true );
		m_TerritoryMapPanel.Show( true );
		m_TerritoryInfoPanel.Show( false );
		m_TerritoryMembersPanel.Show( false );
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryObjectsPanel.Show( false );
		m_InfoName.SetText( "" );
		m_InfoTerritoryID.SetText( "" );
		m_InfoOwnerID.SetText( "" );
		m_InfoPosition.SetText( "" );
		m_InfoLevel.SetText( "" );
		
		m_MemberListEntries.Clear();
		
		m_CurrentTerritory = null;
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
		m_ObjectPreviewPanel.Show( false );
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
			m_Module.DeleteObject( m_CurrentObject );
			
			m_ObjectInfoPanel.Show( false );
			m_ObjectButtonsPanel.Show( false );
			m_TerritoryInfoPanel.Show( true );
			m_TerritoryInfoButtonsPanel.Show( true );
			m_TerritoryObjectsPanel.Show( true );
			m_TerritoryMembersPanel.Show( true );
		}
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
			SyncAndRefreshTerritorys();
		}
		
		if ( w == m_CancelObjectEditButton )
		{
			CancelObjectEdit();
		}
		
		if ( w == m_DeleteObjectButton )
		{
			DeleteObject();
		}
		
		if ( w == m_TerritoryObjectsListRefreshButton )
		{
			if (m_CurrentTerritory) 
				CreateTerritorieObjectsList(m_CurrentTerritory);
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();
		
		SyncAndRefreshTerritorys();
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
		m_MemberListEntries.Clear();
		m_ObjectListEntries.Clear();
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
		
		CreateTerritorieObjectsList( territory );
	}
	
	// ------------------------------------------------------------
	// Expansion CreateTerritorieMembersList
	// ------------------------------------------------------------
	void CreateTerritorieMembersList(ExpansionTerritory territory)
	{
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
	}
	
	// ------------------------------------------------------------
	// Expansion CreateTerritorieObjectsList
	// ------------------------------------------------------------
	void CreateTerritorieObjectsList(ExpansionTerritory territory)
	{
		m_ObjectListEntries.Clear();
		m_TerritoryObjects.Clear();
		
		vector position = territory.GetPosition();
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		GetGame().GetObjectsAtPosition3D( position, GetExpansionSettings().GetTerritory().TerritorySize, objects, proxyCargos );
		
		if ( objects && objects.Count() > 0 )
		{
			for ( int k = 0; k < objects.Count(); k++ )
			{
				Object obj = objects.Get( k );
				if ( IsBaseBuildingObject( obj ) )
				{
					ExpansionCOTTerritoriesObjectsListEntry objectListEntry = new ExpansionCOTTerritoriesObjectsListEntry( m_TerritoryObjectsContent, this, obj );
					m_ObjectListEntries.Insert( objectListEntry );
					m_TerritoryObjects.Insert( obj );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion IsBaseBuildingObject
	// ------------------------------------------------------------
	bool IsBaseBuildingObject(Object obj)
	{
		if ( obj.IsInherited( BaseBuildingBase ) )
			return true;
		
		if ( obj.IsInherited( ExpansionKitBase ) && !obj.IsInherited( ExpansionFlagKitBase ) )
			return true;
		
		//! They all use a more general base like Container_Base or BaseBuildingBase
		//if ( obj.IsInherited( ExpansionSafeBase ) )
		//	return true;
		
		//if ( obj.IsInherited( ExpansionCamoBox ) )
		//	return true;
	
		//if ( obj.IsInherited( ExpansionGunrack ) )
		//	return true;
		
		//if ( obj.IsInherited( ExpansionStove ) )
		//	return true;
		
		if ( obj.IsInherited( TentBase ) ) // Not really needed but better be safe
			return true;
		
		if ( obj.IsInherited( UndergroundStash ) )
			return true;
		
		if ( obj.IsInherited( Pot ) ) // cooking pot, often used to store items
			return true;
		
		if ( obj.IsInherited( FireplaceBase ) ) // barrel holes (used a lot by smart players), Fire stuff in general
			return true;
		
		if ( obj.IsInherited( Container_Base ) ) // seachest, tent, box, barrel, bags and so on
			return true;
		
		if ( obj.IsInherited( WorldContainer_Base ) ) // improvised tent, bin, fridge, mostly scrapped items
			return true;
		
		if ( obj.IsInherited( Fence ) ) // we don't want the kit, we want the basebuilding
			return true;
		
		if ( obj.IsInherited( Watchtower ) ) // we don't want the kit, we want the basebuilding
			return true;
	
		if ( obj.IsInherited( ExpansionTerritoryFlag ) )
			return true;
	
		if ( obj.IsInherited( CarScript ) ) // Cars
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryMemberInfo
	// ------------------------------------------------------------
	void SetTerritoryMemberInfo(ExpansionTerritoryMember member)
	{
		m_TerritoryListPanel.Show( false );
		m_TerritoryInfoPanel.Show( false );
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryMembersPanel.Show( false );
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
	}
	
	// ------------------------------------------------------------
	// Expansion SetTerritoryObjectInfo
	// ------------------------------------------------------------
	void SetTerritoryObjectInfo(Object obj)
	{
		m_TerritoryListPanel.Show( false );
		m_TerritoryInfoPanel.Show( false );
		m_TerritoryInfoButtonsPanel.Show( false );
		m_TerritoryMembersPanel.Show( false );
		m_TerritoryObjectsPanel.Show( false );
		m_ObjectInfoPanel.Show( true );
		m_ObjectButtonsPanel.Show( true );
		m_ObjectPreviewPanel.Show( true );
		
		if ( obj.IsInherited(Container_Base) )
		{
			m_ObjectContainerPanel.Show( true );
			
			CreateContainerObjectsList( obj );
		} else if ( obj.IsInherited(ExpansionTerritoryFlag) )
		{
			//!TODO
		}
		
		string name;
		name = obj.GetDisplayName();
		if (name == "")
			name = obj.ClassName();
				
		m_ObjectName.SetText( name );
		m_ObjectID.SetText( obj.GetID().ToString() );	
		m_ObjectPreview.SetItem( EntityAI.Cast( obj ) );
		
		m_CurrentObject = obj;
	}
	
	// ------------------------------------------------------------
	// Expansion CreateContainerObjectsList
	// ------------------------------------------------------------
	void CreateContainerObjectsList(Object obj)
	{
		m_ContainerListEntries.Clear();
		m_ContainerObjects.Clear();
		
		array<EntityAI> items = new array<EntityAI>;
		
		Container_Base container = Container_Base.Cast( obj );
		container.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
				
		if ( items && items.Count() > 0 )
		{
			for ( int k = 0; k < items.Count(); k++ )
			{
				EntityAI item = items.Get( k );
				if ( item && item != container )
				{
					ExpansionCOTTerritoriesContainerListEntry itemListEntry = new ExpansionCOTTerritoriesContainerListEntry( m_ObjectContainerContent, this, item );
					m_ContainerListEntries.Insert( itemListEntry );
					m_ContainerObjects.Insert( item );
				}
			}
		}
	}
	
		
	// ------------------------------------------------------------
	// Expansion SetContainerObjectInfo
	// ------------------------------------------------------------
	void SetContainerObjectInfo(EntityAI obj)
	{
		m_ObjectContainerPanel.Show( false );
				
		string name;
		name = obj.GetDisplayName();
		if (name == "")
			name = obj.ClassName();
				
		m_ObjectName.SetText( name );
		m_ObjectID.SetText( obj.GetID().ToString() );	
		m_ObjectPreview.SetItem( obj );
		
		m_CurrentObject = obj;
	}
	
	// ------------------------------------------------------------
	// Expansion SyncAndRefreshTerritorys
	// ------------------------------------------------------------
	void SyncAndRefreshTerritorys()
	{		
		m_TerritoryModule.RequestServerTerritories();
		
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( LoadTerritories, 500, false );
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
}