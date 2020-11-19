/**
 * ExpansionSpawnSelectionEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSpawnSelectionEntry extends ScriptedWidgetEventHandler
{	
	protected Widget m_Root;
	protected ButtonWidget m_Button;
	protected TextWidget m_NameWidget;
		
	protected ref ExpansionSpawnLocation m_Location;
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionEntry Constructor
	// ------------------------------------------------------------	
	void ExpansionSpawnSelectionEntry(Widget parent, ref ExpansionSpawnLocation location)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/ui/expansion_spawn_selection_entry.layout", parent ) );
		m_Button = ButtonWidget.Cast( m_Root.FindAnyWidget( "spawn_entry" ) );
		m_NameWidget = TextWidget.Cast( m_Root.FindAnyWidget( "name" ) );
		
		m_Location = location;
		
		m_Root.SetHandler(this);
		
		SetEntry();
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionEntry Destructor
	// ------------------------------------------------------------	
	void ~ExpansionSpawnSelectionEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionEntry SetEntry
	// ------------------------------------------------------------
	protected void SetEntry()
	{
		m_NameWidget.SetText( m_Location.Name );
	}
	
	// ------------------------------------------------------------
	// ExpansionSpawnSelectionEntry OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch ( w )
		{
		case m_Button:
			ref ExpansionSpawnSelectionMenu menu = ExpansionSpawnSelectionMenu.Cast( GetGame().GetUIManager().GetMenu() );
			if ( menu )
				menu.SetSpawnPoint( m_Location );
			break;
		}
		
		return false;
	}
}