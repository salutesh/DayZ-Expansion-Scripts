/**
 * ExpansionGPSPositionArrow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGPSPositionArrow extends ScriptedWidgetEventHandler
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

	protected ExpansionIngameHud m_ExpansionHud;
	
	protected ref Timer	m_ArrowUpdateTimer;
	
	protected int m_ArrowColor = ARGB(255, 234, 32, 39);
	
	// ------------------------------------------------------------
	// Expansion ExpansionGPSPositionArrow Constructor
	// ------------------------------------------------------------
	void ExpansionGPSPositionArrow(Widget parent, ExpansionIngameHud hud)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::ExpansionGPSPositionArrow Start");
		#endif
		m_Root = Widget.Cast( GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/map/expansion_map_arrow.layout", parent) );

		m_PlayerPosArrowPanel = Widget.Cast( m_Root.FindAnyWidget( "arrow_panel" ) );
		m_PlayerPosArrowImagePanel = Widget.Cast( m_Root.FindAnyWidget( "arrow_image_panel" ) );
		m_PlayerPosArrowName = TextWidget.Cast( m_Root.FindAnyWidget( "player_name_tag" ) );
		m_PlayerPosArrowImage = ImageWidget.Cast( m_Root.FindAnyWidget( "arrow_image" ) );

		m_ExpansionHud = hud;

		m_PlayerPosArrowName.SetText("");
		m_PlayerPosArrowName.Show(false);
		
		m_PlayerPosArrowImage.SetColor(m_ArrowColor);
		
		m_Root.SetHandler(this);

		if( m_ExpansionHud.GetGPSState() )
			SetPlayerArrowPosition();

		//! Update Timer
		m_ArrowUpdateTimer = new Timer();
		m_ArrowUpdateTimer.Run( 0.01, this, "Update", NULL, true ); // Call Update all 0.01 seconds

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::ExpansionGPSPositionArrow End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionGPSPositionArrow Deconstructor
	// ------------------------------------------------------------
	void ~ExpansionGPSPositionArrow()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::~ExpansionGPSPositionArrow Start");
		#endif
		delete m_Root;

		m_ArrowUpdateTimer.Stop();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::~ExpansionGPSPositionArrow End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SetPlayerArrowPosition 
	// ------------------------------------------------------------
	void SetPlayerArrowPosition()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::SetPlayerArrowPosition Start");
		#endif
		vector mapPos;
		int mapDir;
		string playerName
		
		mapPos = m_ExpansionHud.GetCurrentMapPos();
		mapDir = m_ExpansionHud.GetCurrentMapDir();
		m_Root.SetPos(mapPos[0], mapPos[1], true);
		m_PlayerPosArrowImagePanel.SetRotation(0, 0, mapDir, true);
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::SetPlayerArrowPosition End");
		#endif
	}

	//============================================
	// Expansion Update
	//============================================
	void Update( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::Update Start");
		#endif
		SetPlayerArrowPosition();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGPSPositionArrow::Update End");
		#endif
	}
	
	//============================================
	// Expansion Update
	//============================================
	vector GetArrowPosition()
	{
		float x;
		float y;
		
		m_Root.GetPos(x, y);
		
		return Vector(x, y, 0);
	}
	
	//============================================
	// Expansion Show
	//============================================
	void Show(bool state)
	{
		m_Root.Show(state);
	}
		
	//============================================
	// Expansion SetColor
	//============================================
	void SetColor(int color)
	{
		m_PlayerPosArrowImage.SetColor(color);
	}
}