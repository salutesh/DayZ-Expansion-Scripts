/**
 * ExpansionCOTTerritoriesObjectsListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesObjectsListEntry extends ScriptedWidgetEventHandler
{
	private Widget m_Root;
	private TextWidget m_ObjectName;
	private ButtonWidget m_EditButton;
	
	private ref ExpansionCOTTerritoriesMenu m_TerritoryCOTMenu;
	private Object m_Object;

	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesObjectsListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesObjectsListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, Object object)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesObjectsListEntry::ExpansionCOTTerritoriesObjectsListEntry - Start");
		#endif
		
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/territories/Territories_ObjectList_Entry.layout", parent ) );
		m_ObjectName			= TextWidget.Cast( m_Root.FindAnyWidget( "object_name" ) );
		m_EditButton			= ButtonWidget.Cast( m_Root.FindAnyWidget( "object_edit_button" ) );
	
		m_TerritoryCOTMenu		= menu;
		m_Object				= object;
		
		m_Root.SetHandler( this );
		
		SetEntry();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesObjectsListEntry::ExpansionCOTTerritoriesObjectsListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesObjectsListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesObjectsListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesObjectsListEntry::SetEntry - Start");
		#endif
		
		if ( m_Object )
		{
			m_ObjectName.SetText( m_Object.ClassName() );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionCOTTerritoriesObjectsListEntry::SetEntry - Set name: " + m_Object.ClassName() );
			#endif
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesObjectsListEntry::SetEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( m_EditButton && w == m_EditButton )
		{			
			if ( m_TerritoryCOTMenu )
			{
				m_TerritoryCOTMenu.SetTerritoryObjectInfo( m_Object );
			}
		}
		
		return false;
	}
}