/**
 * ExpansionCOTVehiclesMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTVehiclesMenu: JMFormBase
{
	private ExpansionCOTVehiclesModule m_Module;
		
	protected Widget m_VehicleListPanel;
	protected GridSpacerWidget m_VehiclesListContent;
	protected ButtonWidget m_VehicleListRefreshButton;
	
	protected Widget m_VehiclesMarkersPanel;
	protected Widget m_VehiclesMapPanel;
	protected MapWidget m_VehiclesMap;
	
	protected Widget m_VehiclesOptionsPanel;
	protected ButtonWidget m_DeleteUnclaimedButton;
	protected ButtonWidget m_DeleteDestroyedButton;
	protected ButtonWidget m_DeleteAllButton;
	
	protected Widget m_VehicleInfoPanel;
	protected TextWidget m_VehicleInfoID;
	protected TextWidget m_VehicleInfoName;
	protected TextWidget m_VehicleInfoClassName;
	protected TextWidget m_VehicleInfoPos;
	protected TextWidget m_VehicleInfoOri;
	protected TextWidget m_VehicleInfoType;
	protected TextWidget m_VehicleInfoExploded;
	protected TextWidget m_VehicleInfoDestroyed;
	protected TextWidget m_VehicleInfoKeys;
	
	protected Widget m_VehicleOptionsPanel;
	protected ButtonWidget m_DeleteVehicleButton;
	protected ButtonWidget m_CancleVehicleEdit;
	
	protected ref array<ref ExpansionCOTVehiclesMapMarker> m_MapMarkers;
	protected ref array<ref ExpansionCOTVehiclesListEntry> m_VehicleEntries;
	
	protected ExpansionVehicleMetaData m_CurrentVehicle;
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesMenu Constructor
	// ------------------------------------------------------------
	void ExpansionCOTVehiclesMenu()
	{
		m_MapMarkers = new array<ref ExpansionCOTVehiclesMapMarker>;
		m_VehicleEntries = new array<ref ExpansionCOTVehiclesListEntry>;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTVehiclesMenu Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTVehiclesMenu()
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
		//! Vehicles List
		m_VehicleListPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicles_list_panel" ) );
		m_VehiclesListContent = GridSpacerWidget.Cast( layoutRoot.FindAnyWidget( "vehicles_list_content" ) );
		m_VehicleListRefreshButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "vehicles_refresh_button" ) );
		
		//! Marker Panel
		m_VehiclesMarkersPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicles_markers_panel" ) );
		
		//! Vehicles Map
		m_VehiclesMapPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicles_map_panel" ) );
		m_VehiclesMap = MapWidget.Cast( layoutRoot.FindAnyWidget( "vehicles_map" ) );
		
		//! Vehicles Options
		m_VehiclesOptionsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicles_buttons_panel" ) );
		m_DeleteUnclaimedButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_unclaimed_vehicle_button" ) );
		m_DeleteDestroyedButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_destroyed_vehicle_button" ) );
		m_DeleteAllButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_all_button" ) );
		
		//! Vehicle Info
		m_VehicleInfoPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicle_info_panel" ) );
		m_VehicleInfoID = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_id_value" ) );
		m_VehicleInfoName = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_name_value" ) );
		m_VehicleInfoClassName = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_classname_value" ) );
		m_VehicleInfoPos = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_pos_value" ) );
		m_VehicleInfoOri = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_ori_value" ) );
		m_VehicleInfoType = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_type_value" ) );
		m_VehicleInfoExploded = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_exploded_value" ) );
		m_VehicleInfoDestroyed = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_destroyed_value" ) );
		m_VehicleInfoKeys = TextWidget.Cast( layoutRoot.FindAnyWidget( "info_keys_value" ) );
		
		//! Vehicle Options
		m_VehicleOptionsPanel = Widget.Cast( layoutRoot.FindAnyWidget( "vehicle_info_buttons_panel" ) );
		m_DeleteVehicleButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "delete_vehicle_button" ) );
		m_CancleVehicleEdit = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "cancel_edit_button" ) );
	}

	// ------------------------------------------------------------
	// Expansion LoadVehicles
	// ------------------------------------------------------------
	void LoadVehicles()
	{		
		m_MapMarkers.Clear();
		m_VehicleEntries.Clear();
		
		if ( m_Module && m_Module.GetServerVehicles().Count() != 0 )
		{		
			for ( int i = 0; i < m_Module.GetServerVehicles().Count(); i++ )
			{
				ExpansionVehicleMetaData currentVehicle = m_Module.GetServerVehicles().Get( i );
				
				int color;
				string marker;

				GetVehicleTypeInfo( currentVehicle.m_VehicleType, color, marker );
				
				//! Create map marker for vehicle
				ExpansionCOTVehiclesMapMarker vehicleMapMarker = new ExpansionCOTVehiclesMapMarker( m_VehiclesMarkersPanel, m_VehiclesMap, currentVehicle.m_Position, color, marker, currentVehicle, this );
				m_MapMarkers.Insert( vehicleMapMarker );
				
				//! Create list entry for vehicle
				ExpansionCOTVehiclesListEntry vehicleListEntry = new ExpansionCOTVehiclesListEntry(m_VehiclesListContent, this, currentVehicle);
				m_VehicleEntries.Insert( vehicleListEntry );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion GetVehicleTypeInfo
	// ------------------------------------------------------------
	private void GetVehicleTypeInfo( int type, out int color, out string marker )
	{
		if ( type & EXVT_CAR )
		{
			color = ARGB( 255, 243, 156, 18 );
			marker = "DayZExpansion\\GUI\\icons\\marker\\marker_car.paa";
			return;
		}
		if ( type & EXVT_BOAT )
		{
			color = ARGB( 255, 243, 156, 18 );
			marker = "DayZExpansion\\GUI\\icons\\marker\\marker_boat.paa";
			return;
		}
		if ( type & EXVT_HELICOPTER )
		{
			color = ARGB( 255, 243, 156, 18 );
			marker = "DayZExpansion\\GUI\\icons\\marker\\marker_helicopter.paa";
			return;
		}
		if ( type & EXVT_PLANE )
		{
			color = ARGB( 255, 243, 156, 18 );
			marker = "DayZExpansion\\GUI\\icons\\marker\\marker_helicopter.paa";
			return;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadVehicles
	// ------------------------------------------------------------
	void SetVehicleInfo(ExpansionVehicleMetaData vehicle)
	{		
		//! m_VehicleListPanel.Show( false );
		m_VehiclesMapPanel.Show( false );
		m_VehiclesOptionsPanel.Show( false );
		
		HideMapMarkers();
		
		m_VehicleInfoPanel.Show( true );
		m_VehicleOptionsPanel.Show( true );
		
		m_VehicleInfoID.SetText( vehicle.m_ID.ToString() );

		string displayName;
		GetGame().ConfigGetText( "cfgVehicles " + vehicle.m_ClassName + " displayName", displayName );
		m_VehicleInfoName.SetText( displayName );

		m_VehicleInfoClassName.SetText( vehicle.m_ClassName );
		m_VehicleInfoPos.SetText( "X: " + vehicle.m_Position[0] + " Z: " + vehicle.m_Position[1] + " Y: " + vehicle.m_Position[2] );
		m_VehicleInfoOri.SetText( "Yaw: " + vehicle.m_Orientation[0] + " Pitch: " + vehicle.m_Orientation[1] + " Roll: " + vehicle.m_Orientation[2] );
		m_VehicleInfoType.SetText( vehicle.GetVehicleType() );
		m_VehicleInfoExploded.SetText( vehicle.IsExploded().ToString() );
		m_VehicleInfoDestroyed.SetText( vehicle.IsDestroyed().ToString() );
		m_VehicleInfoKeys.SetText( vehicle.m_HasKeys.ToString() );
		
		m_CurrentVehicle = vehicle;
	}
	
	// ------------------------------------------------------------
	// Expansion SyncAndRefreshVehicles
	// ------------------------------------------------------------
	void SyncAndRefreshVehicles()
	{		
		m_Module.RequestServerVehicles();
		
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( LoadVehicles, 500, false );
	}
			
	// ------------------------------------------------------------
	// Expansion ShowMapMarkers
	// ------------------------------------------------------------
	void ShowMapMarkers()
	{
		for ( int i = 0; i < m_MapMarkers.Count(); i++ )
		{
			ExpansionCOTVehiclesMapMarker marker = m_MapMarkers.Get( i );
			marker.ShowMarker();
		}
	}
		
	// ------------------------------------------------------------
	// Expansion HideMapMarkers
	// ------------------------------------------------------------
	void HideMapMarkers()
	{
		for ( int i = 0; i < m_MapMarkers.Count(); i++ )
		{
			ExpansionCOTVehiclesMapMarker marker = m_MapMarkers.Get( i );
			marker.HideMarker();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion BackToList
	// ------------------------------------------------------------	
	void BackToList()
	{
		SyncAndRefreshVehicles();
		
		m_VehicleListPanel.Show( true );
		m_VehiclesMapPanel.Show( true );
		m_VehiclesOptionsPanel.Show( true );
		
		m_VehicleInfoPanel.Show( false );
		m_VehicleOptionsPanel.Show( false );
		
		m_VehicleInfoID.SetText( "" );
		m_VehicleInfoName.SetText( "" );
		m_VehicleInfoClassName.SetText( "" );
		m_VehicleInfoPos.SetText( "" );
		m_VehicleInfoOri.SetText( "" );
		m_VehicleInfoType.SetText( "" );
		m_VehicleInfoExploded.SetText( "" );
		m_VehicleInfoDestroyed.SetText( "" );
		m_VehicleInfoKeys.SetText( "" );
		
		m_CurrentVehicle = NULL;
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------	
	override bool OnClick( Widget w, int x, int y, int button )
	{		
		if ( w == m_VehicleListRefreshButton )
		{
			SyncAndRefreshVehicles();
			
			return true;
		}
		
		if ( w == m_CancleVehicleEdit )
		{
			BackToList();
			
			return true;
		}

		if ( w == m_DeleteAllButton )
		{
			m_Module.DeleteVehicleAll( );
		}

		if ( w == m_DeleteDestroyedButton )
		{
			m_Module.DeleteVehicleDestroyed( );
		}

		if ( w == m_DeleteUnclaimedButton )
		{
			m_Module.DeleteVehicleUnclaimed( );
		}
		
		if ( w == m_DeleteVehicleButton )
		{
			if ( m_CurrentVehicle )
			{
				m_Module.DeleteVehicle( m_CurrentVehicle.m_ID );
				
				BackToList();
			}
			
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion OnShow
	// ------------------------------------------------------------
	override void OnShow()
	{
		super.OnShow();
		
		SyncAndRefreshVehicles();
	}
	
	// ------------------------------------------------------------
	// Expansion OnHide
	// ------------------------------------------------------------
	override void OnHide()
	{
		super.OnHide();
	}
}