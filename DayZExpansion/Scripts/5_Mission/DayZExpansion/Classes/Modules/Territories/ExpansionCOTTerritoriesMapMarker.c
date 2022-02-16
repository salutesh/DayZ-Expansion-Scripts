/**
 * ExpansionCOTTerritoriesMapMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
class ExpansionCOTTerritoriesMapMarker extends ExpansionMapMarker
{
	private JMFormBase m_Menu;
	private ref ExpansionTerritory m_Territory;
	
	private GridSpacerWidget m_MemberEntries;
	private ButtonWidget m_CancelButton;
	private ButtonWidget m_EditTerritoryButton;
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker Constructor
	// ------------------------------------------------------------
	void ExpansionCOTTerritoriesMapMarker(Widget parent, MapWidget mapWidget, bool autoInit = true)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::ExpansionCOTTerritoriesMapMarker - Start");
		#endif
	
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::ExpansionCOTTerritoriesMapMarker - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker SetCOTMenu
	// ------------------------------------------------------------
	void SetCOTMenu(JMFormBase menu)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::SetCOTMenu - Start");
		#endif
		
		m_Menu = menu;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::SetCOTMenu - End");
		#endif
	}
	// ------------------------------------------------------------
	// ExpansionCOTTerritoriesMapMarker SetTerritory
	// ------------------------------------------------------------
	void SetTerritory( ExpansionTerritory territory)
	{
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::SetCOTMenu - Start");
		#endif
		
		m_Territory = territory;
		
		#ifdef EXPANSION_COT_TERRITORY_MODULE_DEBUG
		EXLogPrint("ExpansionCOTTerritoriesMapMarker::SetCOTMenu - End");
		#endif
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
		vector mapPos = GetMapWidget().MapToScreen( m_Territory.GetPosition() );

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
			ExpansionCOTTerritoriesMenu menu = ExpansionCOTTerritoriesMenu.Cast( m_Menu );
			if ( menu )
			{
				menu.SetTerritoryInfo( m_Territory );
			}
			break;
		}

		return false;
	}
};
#endif
