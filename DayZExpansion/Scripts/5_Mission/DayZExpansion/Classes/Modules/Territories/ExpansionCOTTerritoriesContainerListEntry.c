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
	private Widget m_Root;
	private TextWidget m_ObjectName;
	private TextWidget m_ObjectID;
	private ImageWidget m_StatusIcon;
	private ButtonWidget m_EditButton;
	
	private ref ExpansionCOTTerritoriesMenu m_TerritoryCOTMenu;
	private ref ExpansionEntityMetaData m_Object;
	
	// ------------------------------------------------------------
	// Expansion ExpansionCOTTerritoriesContainerListEntry Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesContainerListEntry(Widget parent, ExpansionCOTTerritoriesMenu menu, ref ExpansionEntityMetaData object)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::ExpansionCOTTerritoriesContainerListEntry - Start");
		#endif
		
		m_Root					= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/COT/territories/Territories_ObjectList_Entry_New.layout", parent ) );
		m_ObjectName			= TextWidget.Cast( m_Root.FindAnyWidget( "object_name" ) );
		m_ObjectID				= TextWidget.Cast( m_Root.FindAnyWidget( "object_id" ) );
		m_StatusIcon			= ImageWidget.Cast( m_Root.FindAnyWidget( "object_status_icon" ) );
		m_EditButton			= ButtonWidget.Cast( m_Root.FindAnyWidget( "object_entry" ) );
	
		m_TerritoryCOTMenu		= menu;
		m_Object				= object;
		
		m_Root.SetHandler( this );
		
		SetEntry();
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
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
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::SetEntry - Start");
		#endif
		
		if ( m_Object )
		{
			m_ObjectName.SetText( m_Object.m_ClassName );
			m_ObjectID.SetText( " - ID: " + m_Object.m_ID.ToString() );
			
			if ( m_Object.IsDamageDestroyed() )
			{
				m_StatusIcon.SetColor( ARGB( 255, 192, 57, 43 ) );
			}
			else
			{
				m_StatusIcon.SetColor( ARGB( 255, 39, 174, 96 ) );
			}
			
			
			#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
			EXLogPrint("ExpansionCOTTerritoriesContainerListEntry::SetEntry - Set name: " + m_Object.ClassName() );
			#endif
		}
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
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
			m_ObjectID.SetColor( ARGB( 255, 0, 0, 0 ) );
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
			m_ObjectID.SetColor( ARGB( 255, 255, 255, 255 ) );
		}

		return false;
	}
}