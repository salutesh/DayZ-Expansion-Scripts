/**
 * ExpansionMapMenuPlayerMarker.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuPlayerMarker extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected TextWidget m_Name;
	protected ImageWidget m_Icon1;
	protected ImageWidget m_Icon2;
	protected MapWidget m_MapWidget;
	protected int m_MarkerAlpha;
	protected ref ExpansionMapMenu m_MapMenu;
	protected PlayerBase m_Player;
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuPlayerMarker Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuPlayerMarker(Widget parent, MapWidget mapwidget, PlayerBase player)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenuPlayerMarker::ExpansionMapMenuPlayerMarker - Start");
		#endif
		
		if (!player) return;
		
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_player_marker.layout", parent) );

		m_Name			= TextWidget.Cast( m_Root.FindAnyWidget("player_name_tag") );
		m_Icon1			= ImageWidget.Cast( m_Root.FindAnyWidget("arrow_image") );
		m_Icon2			= ImageWidget.Cast( m_Root.FindAnyWidget("arrow_image0") );

		m_MapWidget		= mapwidget;
		m_Player		= player; 

		m_Root.SetHandler(this);
		
		Init();

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenuPlayerMarker::ExpansionMapMenuPlayerMarker - End");
		#endif
	}
	
	void Init()
	{
		//! Get Map Menu
		m_MapMenu = ExpansionMapMenu.Cast(GetGame().GetUIManager().GetMenu());
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuPlayerMarker Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuPlayerMarker()
	{
		delete m_Root;
	}

	// ------------------------------------------------------------
	// Expansion SetMarkerAlpha
	// ------------------------------------------------------------
	void SetMarkerAlpha(int alpha)
	{
		m_MarkerAlpha = alpha;
	}
	
	// ------------------------------------------------------------
	// Expansion Update
	// ------------------------------------------------------------
	void Update( float timeslice )
	{
		UpdatePlayerMarker();
	}
	
	// ------------------------------------------------------------
	// Expansion UpdatePlayerMarker 
	// ------------------------------------------------------------
	void UpdatePlayerMarker()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenuPlayerMarker::UpdatePlayerMarker - Start");
		#endif
		
		vector mapPos;
		vector finalMapPos;
		vector mapDir;
		string playerName
		vector cameraOri;
		float normalizedAngle;
		int dir;
		
		if ( m_Player )
		{
			mapPos = m_Player.GetPosition();
			
			finalMapPos = m_MapWidget.MapToScreen( mapPos );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenuPlayerMarker::UpdatePlayerMarker - Final Pos: " + finalMapPos.ToString() );
			#endif
			
			cameraOri = mapDir.VectorToAngles();
			normalizedAngle = Math.NormalizeAngle( cameraOri[0]) ;
			dir = Math.Round( normalizedAngle );
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMapMenuPlayerMarker::UpdatePlayerMarker - Final Direction: " + dir.ToString() );
			#endif
			
			if ( m_Player.GetIdentity() )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionMapMenuPlayerMarker::UpdatePlayerMarker - Identity: " + m_Player.GetIdentity() );
				#endif

				playerName = m_Player.GetIdentityName();
				m_Name.SetText( playerName );
			}
			
			m_Root.SetPos(finalMapPos[0], finalMapPos[1], true);
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMapMenuPlayerMarker::UpdatePlayerMarker - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion Show
	// ------------------------------------------------------------
	void Show()
	{
		m_Root.Show( true );
	}
	
	// ------------------------------------------------------------
	// Expansion Hide
	// ------------------------------------------------------------
	void Hide()
	{
		m_Root.Show( false );
	}
}