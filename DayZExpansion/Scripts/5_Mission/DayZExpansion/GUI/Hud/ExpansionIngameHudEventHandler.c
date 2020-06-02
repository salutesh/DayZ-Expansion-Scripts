/**
 * ExpansionIngameHudEventHandler.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionIngameHudEventHandler extends ScriptedWidgetEventHandler
{
	protected ExpansionIngameHud 				m_ExpansionHud;
	protected ref Timer							m_HandlerUpdateTimer;
	
	private bool								m_HasGPSItem;
	private float								m_CloseTime = 0;
	
	//============================================
	// ExpansionIngameHudEventHandler Constructor
	//============================================
	void ExpansionIngameHudEventHandler(ExpansionIngameHud hud_expansion)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::ExpansionIngameHudEventHandler:: - Start");
		#endif
		m_ExpansionHud = hud_expansion;
		
		//! Update Timer
		m_HandlerUpdateTimer = new Timer();
		m_HandlerUpdateTimer.Run( 1, this, "Update", NULL, true ); // Call Update all 1 seconds
		
		m_HasGPSItem = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::ExpansionIngameHudEventHandler:: - End");
		#endif
	}
	
	//============================================
	// ExpansionIngameHudEventHandler Destructor
	//============================================
	void ~ExpansionIngameHudEventHandler()
	{
		m_HandlerUpdateTimer.Stop();
	}
	
	//============================================
	// Expansion GetExpansionHud
	//============================================
	ExpansionIngameHud GetExpansionHud()
	{
		return m_ExpansionHud;
	}
	
	//============================================
	// Expansion OnClick
	//============================================
	override bool OnClick( Widget w, int x, int y, int button )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::OnClick:: - Start");
		#endif
		//super.OnClick( w, x, y, button );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::OnClick:: - End");
		#endif
		return m_ExpansionHud.OnClick( w, x, y, button );
	}
	
	//============================================
	// Expansion OnMouseWheel
	//============================================
	override bool OnMouseWheel(Widget w, int y, int y, int wheel)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::OnMouseWheel:: - Start");
		#endif
		//super.OnMouseWheel( w, x, y, wheel );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionIngameHudEventHandler::OnMouseWheel:: - End");
		#endif
		return m_ExpansionHud.OnMouseWheel( w, x, y, wheel );
	}
	
	//============================================
	// Expansion OnUpdate
	//============================================
	void Update(float timeslice)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::Update - Start");
		#endif
		
		PlayerBase player;
		m_CloseTime += timeslice;
		
		//! If GPS is visable and player has no GPS item then close the GPS overlay when setting is enabled
		if ( GetExpansionSettings().GetGeneral().NeedGPSItemForKeyBinding && m_ExpansionHud.GetGPSState() )
		{
			player = PlayerBase.Cast( GetGame().GetPlayer() );
			
			if (!player)
			{
				Error("ExpansionIngameHudEventHandler::Update - null player");
				return;
			}
			
			if ( !player.HasItemGPS() && m_HasGPSItem )
				ToggleHUDGPS();
		}

		if ( GetUApi().GetInputByName("UAExpansionGPSToggle").LocalHoldBegin() && GetExpansionSettings() && GetExpansionSettings().GetGeneral() && GetExpansionSettings().GetGeneral().EnableHUDGPS && m_CloseTime > 0.75 )
		{
			//! If it's already open, just close it
			if ( m_ExpansionHud.GetGPSState() )
			{				
				ToggleHUDGPS();
			}
			else if ( !m_ExpansionHud.GetGPSState() )
			{
				if ( GetExpansionSettings().GetGeneral().NeedGPSItemForKeyBinding )
				{
					player = PlayerBase.Cast( GetGame().GetPlayer() );
					if (!player)
					{
						Error("ExpansionIngameHudEventHandler::Update - null player");
						return;
					}
					
					if ( player.HasItemGPS() )
					{
						m_HasGPSItem = true;
						ToggleHUDGPS();
					}
				}
				else
				{
					ToggleHUDGPS();
				}
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::Update - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ToggleHUDGPS
	//! Hides/Shows HUD GPS elements
	// ------------------------------------------------------------
	void ToggleHUDGPS()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::ToggleHUDGPS - Start");
		#endif
		
		if ( !m_ExpansionHud.GetGPSState() )
		{			
			GPSShow();
		}
		else if ( m_ExpansionHud.GetGPSState() )
		{
			GPSHide();
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::ToggleHUDGPS - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GPSShow
	// ------------------------------------------------------------
	void GPSShow()
	{
		m_CloseTime = 0;
		
		m_ExpansionHud.ShowGPS( true );
		
		if ( m_ExpansionHud.GetGPSMapStatsState() )
			m_ExpansionHud.ShowGPSMapStats( false );
		
		if ( !m_ExpansionHud.GetGPSMapState() )
			m_ExpansionHud.ShowGPSMap( true );
	}
	
	// ------------------------------------------------------------
	// Expansion GPSHide
	// ------------------------------------------------------------
	void GPSHide()
	{
		m_ExpansionHud.ShowGPS( false );
	}
}