/**
 * ExpansionCOTTerritoriesListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_TerritoryName;
	protected TextWidget m_TerritoryID;
	protected ButtonWidget m_EditButton;
	
	protected ref ExpansionTerritory m_Territory;
	protected ref ExpansionCOTTerritoriesMenu m_COTTerritoryMenu;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, ExpansionTerritory territory)
	{
		m_Root					= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/territories/Territories_List_Entry.layout", parent );
		m_TerritoryName			= TextWidget.Cast( m_Root.FindAnyWidget( "territory_name" ) );
		m_TerritoryID			= TextWidget.Cast( m_Root.FindAnyWidget( "territory_id" ) );
		m_EditButton			= ButtonWidget.Cast( m_Root.FindAnyWidget( "edit_button" ) );
		
		m_COTTerritoryMenu		= menu;
		m_Territory				= territory;
		
		m_Root.SetHandler( this );
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		if ( m_Territory )
		{
			m_TerritoryName.SetText( m_Territory.GetTerritoryName() );
			m_TerritoryID.SetText( "ID: " + m_Territory.GetTerritoryID().ToString() );
		}
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_EditButton && w == m_EditButton )
		{			
			if ( m_COTTerritoryMenu && m_Territory)
			{
				m_COTTerritoryMenu.SetTerritoryInfo( m_Territory );
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
			m_TerritoryName.SetColor( ARGB( 255, 96, 125, 139 ) );
			m_TerritoryID.SetColor( ARGB( 255, 96, 125, 139 ) );
			return true;
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
			m_TerritoryName.SetColor( ARGB( 255, 255, 255, 255 ) );
			m_TerritoryID.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}

		return false;
	}
}