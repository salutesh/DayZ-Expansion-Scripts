/**
 * ExpansionMapMenuPositionArrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMapMenuPositionArrow extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Widget m_PlayerPosArrowPanel;
	protected Widget m_PlayerPosArrowImagePanel;
	protected TextWidget m_PlayerPosArrowName;
	protected ImageWidget m_PlayerPosArrowImage;

	protected int m_ArrowColorAlpha;
	protected int m_ArrowColorRed;
	protected int m_ArrowColorGreen;
	protected int m_ArrowColorBlue;

	protected ExpansionMapMenu m_MapMenu;

	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuPositionArrow Constructor
	// ------------------------------------------------------------
	void ExpansionMapMenuPositionArrow(Widget parent, ExpansionMapMenu menu)
	{
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_arrow.layout", parent) );

		m_PlayerPosArrowPanel = Widget.Cast( m_Root.FindAnyWidget( "arrow_panel" ) );
		m_PlayerPosArrowImagePanel = Widget.Cast( m_Root.FindAnyWidget( "arrow_image_panel" ) );
		m_PlayerPosArrowName = TextWidget.Cast( m_Root.FindAnyWidget( "player_name_tag" ) );
		m_PlayerPosArrowImage = ImageWidget.Cast( m_Root.FindAnyWidget( "arrow_image" ) );

		m_MapMenu = menu;

		m_Root.SetHandler(this);

		SetPlayerArrowPosition();
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMapMenuPositionArrow Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionMapMenuPositionArrow()
	{
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// Expansion GetArrowWidget
	// ------------------------------------------------------------
	Widget GetArrowWidget()
	{
		return m_PlayerPosArrowImage;
	}

	// ------------------------------------------------------------
	// Expansion SetPlayerArrowPosition 
	// ------------------------------------------------------------
	void SetPlayerArrowPosition()
	{
		vector mapPos;
		int mapDir;
		string playerName
		
		PlayerBase m_Player = PlayerBase.Cast( GetGame().GetPlayer() );
		
		if ( m_Player )
		{
			mapPos = m_MapMenu.GetCurrentMapPos();
			mapDir = m_MapMenu.GetCurrentMapDir();

			if ( m_Player.GetIdentity() )
			{
				playerName = m_Player.GetIdentityName();
				m_PlayerPosArrowName.SetText( playerName );
			}
			else
			{
				m_PlayerPosArrowName.SetText( "YOU" );
			}
			
			m_Root.SetPos(mapPos[0], mapPos[1], true);
			m_PlayerPosArrowImagePanel.SetRotation(0, 0, mapDir, true);
		}
	}
	
	//============================================
	// Expansion OnUpdate
	//============================================
	void Update( float timeslice )
	{
		SetPlayerArrowPosition();
	}
	
	//============================================
	// Expansion SetArrowColor
	//============================================
	void SetArrowColor(int a, int r, int g, int b)
	{
		m_PlayerPosArrowImage.SetColor( ARGB(a, r, g, b) );
		m_PlayerPosArrowName.SetColor( ARGB(a, r, g, b) );

		m_ArrowColorAlpha = a;
		m_ArrowColorRed = r;
		m_ArrowColorGreen = g;
		m_ArrowColorBlue = b;

		UpdateArrowColor();
	}	

	//============================================
	// Expansion SetArrowAlpha
	//============================================
	void SetArrowAlpha(int a)
	{
		m_ArrowColorAlpha = a;

		UpdateArrowColor();
	}
	
	//============================================
	// Expansion UpdateArrowColor
	//============================================
	void UpdateArrowColor()
	{
		m_PlayerPosArrowImage.SetColor( ARGB(m_ArrowColorAlpha, m_ArrowColorRed, m_ArrowColorGreen, m_ArrowColorBlue) );
		m_PlayerPosArrowName.SetColor( ARGB(m_ArrowColorAlpha, m_ArrowColorRed, m_ArrowColorGreen, m_ArrowColorBlue) );
	}
}