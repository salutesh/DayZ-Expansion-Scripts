/**
 * ExpansionCOTVehiclesListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTVehiclesListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_VehicleName;
	protected TextWidget m_VehicleID;
	protected ButtonWidget m_EditButton;
	protected ImageWidget m_VehicleStatusIcon;
	
	protected ref ExpansionVehicleMetaData m_Vehicle;
	protected ref ExpansionCOTVehiclesMenu m_COTVehicleMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTVehiclesListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTVehiclesListEntry(Widget parent, ExpansionCOTVehiclesMenu menu, ExpansionVehicleMetaData vehicle)
	{
		m_Root					= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/vehicles/Vehicles_List_Entry_New.layout", parent );
		m_VehicleName			= TextWidget.Cast( m_Root.FindAnyWidget( "vehicle_name" ) );
		m_VehicleID				= TextWidget.Cast( m_Root.FindAnyWidget( "vehicle_id" ) );
		m_EditButton			= ButtonWidget.Cast( m_Root.FindAnyWidget( "edit_button" ) );
		m_VehicleStatusIcon		= ImageWidget.Cast( m_Root.FindAnyWidget( "vehicle_status_icon" ) );
		
		m_COTVehicleMenu		= menu;
		m_Vehicle				= vehicle;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTVehiclesListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTVehiclesListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		if ( m_Vehicle )
		{
			string displayName;
			GetGame().ConfigGetText( "cfgVehicles " + m_Vehicle.m_ClassName + " displayName", displayName );
			m_VehicleName.SetText( displayName );

			m_VehicleID.SetText( " - ID: " + m_Vehicle.m_NetworkIDLow.ToString() + "-" + m_Vehicle.m_NetworkIDHigh.ToString() );
			
			if ( m_Vehicle.m_DestructionType != EXDT_NONE )
			{
				m_VehicleStatusIcon.SetColor( ARGB( 255,255,0,0 ) );
			} else
			{
				m_VehicleStatusIcon.SetColor( ARGB( 255,0,255,0 ) );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_EditButton && w == m_EditButton )
		{			
			if ( m_COTVehicleMenu && m_Vehicle )
			{
				m_COTVehicleMenu.SetVehicleInfo( m_Vehicle );
				return true;
			}
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_EditButton && w == m_EditButton )
		{
			m_VehicleName.SetColor( ARGB( 255, 0, 0, 0 ) );
			m_VehicleID.SetColor( ARGB( 255, 0, 0, 0 ) );
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_EditButton && w == m_EditButton )
		{
			m_VehicleName.SetColor( ARGB( 255, 255, 255, 255 ) );
			m_VehicleID.SetColor( ARGB( 255, 255, 255, 255 ) );
		}

		return false;
	}
};