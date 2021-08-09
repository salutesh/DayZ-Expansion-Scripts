/**
 * ExpansionSpawnSelectionMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnSelectionMarker extends ExpansionMapMarker
{
	private UIScriptedMenu m_Menu;
	private ref ExpansionSpawnLocation m_Location;
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMarker Constructor
	// ------------------------------------------------------------
	void ExpansionSpawnSelectionMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMarker SetLocation
	// ------------------------------------------------------------
	void SetLocation( ExpansionSpawnLocation location)
	{		
		m_Location = location;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker CanEditName
	// ------------------------------------------------------------	
	override bool CanEditName()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker CanEdit
	// ------------------------------------------------------------	
	override bool CanEdit()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker Update
	// ------------------------------------------------------------	
	override void Update( float pDt )
	{
		vector mapPos = GetMapWidget().MapToScreen( m_Location.Positions[0] );

		float x;
		float y;
		
		GetLayoutRoot().GetParent().GetScreenPos( x, y );
		GetLayoutRoot().SetPos( mapPos[0] - x, mapPos[1] - y, true );
	}
	
	// ------------------------------------------------------------
	// ExpansionMapMarker OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		switch ( w )
		{
		case m_EditButton:
			ExpansionSpawnSelectionMenu menu = ExpansionSpawnSelectionMenu.Cast( GetGame().GetUIManager().GetMenu() );
			if ( menu )
				menu.SetSpawnPoint( m_Location, false );
			break;
		}
		
		return false;
	}
}