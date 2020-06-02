/**
 * ExpansionCOTTerritoriesContainerListEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTTerritoriesContainerListEntry extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_ObjectName;
	protected ButtonWidget m_EditButton;
	
	protected ref ExpansionCOTTerritoriesMenu m_TerritoryCOTMenu;
	protected EntityAI m_Object;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesContainerListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesContainerListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, EntityAI object)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::ExpansionCOTTerritoriesContainerListEntry - Start");
		#endif
		
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/territories/Territories_ObjectList_Entry.layout", parent ) );
		m_ObjectName			= TextWidget.Cast( m_Root.FindAnyWidget( "object_name" ) );
		m_EditButton			= ButtonWidget.Cast( m_Root.FindAnyWidget( "object_edit_button" ) );
	
		m_TerritoryCOTMenu		= menu;
		m_Object				= object;
		
		m_Root.SetHandler( this );
		
		SetEntry();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::ExpansionCOTTerritoriesContainerListEntry - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesContainerListEntry Destructor
	// ------------------------------------------------------------
	void ~ExpansionCOTTerritoriesContainerListEntry()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion SetEntry
	// ------------------------------------------------------------
	void SetEntry()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::SetEntry - Start");
		#endif
		
		if ( m_Object )
		{
			m_ObjectName.SetText( m_Object.ClassName() );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::SetEntry - Set name: " + m_Object.ClassName() );
			#endif
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::SetEntry - End");
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
				m_TerritoryCOTMenu.SetContainerObjectInfo( m_Object );
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
			m_ObjectName.SetColor( ARGB( 255, 0, 0, 0 ) );
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
			m_ObjectName.SetColor( ARGB( 255, 255, 255, 255 ) );
			return true;
		}

		return false;
	}
}