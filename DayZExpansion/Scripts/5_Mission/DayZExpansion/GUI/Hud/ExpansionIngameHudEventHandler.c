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
	private bool								m_GPSWasOpened = false;
	
	private bool								m_HasGPSForCompassItem;
	private bool								m_HasCompassItem;
	private bool								m_CompassWasOpened;
	PlayerBase									m_Player;
	
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
		m_HasGPSForCompassItem = false;
		m_HasCompassItem = false;
		
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
	// Expansion OnUpdate
	//============================================
	void Update(float timeslice)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::Update - Start");
		#endif
		
		if ( GetGame() && GetGame().GetPlayer() && GetGame().GetPlayer().IsAlive() )
		{
			m_Player = PlayerBase.Cast(GetGame().GetPlayer());
			
			m_CloseTime += timeslice;
			
			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			{
				if ( GetExpansionHud().GetGPSState() )
				{
					SetWasGPSOpened(true);
				}
				
				if ( GetExpansionHud().GetCompassState() )
				{
					SetWasCompassOpened(true);
				}
				
				return;
			}
			
			//! GPS HUD
			UpdateGPS();
			
			//! COMPASS HUD	
			if ( GetExpansionSettings() && GetExpansionSettings().GetMap() )
			{
				if (GetExpansionSettings().GetMap().EnableHUDCompass)
				{
					if ( GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass || GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass )
					{
						UpdateCompass();
					}
					else if ( GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass && GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass )
					{
						CompassShow();
					}
				}
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::Update - End");
		#endif
	}
	
	//============================================
	// Expansion UpdateGPS
	//============================================
	void UpdateGPS()
	{
		//! If GPS is visable and player has no GPS item then close the GPS overlay when setting is enabled
		if ( GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding && GetExpansionHud().GetGPSState() )
		{
			if (!m_Player)
			{
				Error("ExpansionIngameHudEventHandler::Update - null player");
				return;
			}
			
			if ( !m_Player.HasItemGPS() && m_HasGPSItem )
			{
				m_HasGPSItem = false;
				ToggleHUDGPS();
			}
		}

		if ( GetUApi().GetInputByName("UAExpansionGPSToggle").LocalHoldBegin() && GetExpansionSettings() && GetExpansionSettings().GetGeneral() && GetExpansionSettings().GetMap().EnableHUDGPS && m_CloseTime > 0.75 )
		{
			//! If it's already open, just close it
			if ( GetExpansionHud().GetGPSState() )
			{
				ToggleHUDGPS();
			}
			else if ( !GetExpansionHud().GetGPSState() )
			{
				if ( GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding )
				{
					if (!m_Player)
					{
						Error("ExpansionIngameHudEventHandler::Update - null player");
						return;
					}
					
					if ( m_Player.HasItemGPS() )
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
	}
	
	//============================================
	// Expansion UpdateCompass
	//============================================
	void UpdateCompass()
	{
		if ( m_Player )
		{			
			if ( GetExpansionHud().GetCompassState() )
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Compass HUD IS visible!" );
				#endif
	
				if ( GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass )
				{					
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Need to remove Item compass to hide compass HUD!" );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemGPS(): " + m_Player.HasItemGPS() );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasGPSForCompassItem: " + m_HasGPSForCompassItem );
					#endif
					
					if ( !m_Player.HasItemGPS() && m_HasGPSForCompassItem )
					{
						m_HasGPSForCompassItem = false;
						CompassHide();
						
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Player has no Item GPS but compass is visible. HIDE!");
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemGPS(): " + m_Player.HasItemGPS() );
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasGPSForCompassItem: " + m_HasGPSForCompassItem );
						#endif
					}
				}
				
				if ( GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass )
				{
					if ( !m_Player.HasItemCompass() && m_HasCompassItem )
					{
						m_HasCompassItem = false;
						CompassHide();
						
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Player has no Item Compass but compass is visible. HIDE!");
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemCompass(): " + m_Player.HasItemCompass() );
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasCompassItem: " + m_HasCompassItem );
						#endif
					}
				}
			}
			else
			{
				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Compass HUD is NOT visible!" );
				#endif
				
				if ( GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass )
				{					
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Need GPS Item for Show Compass HUD" );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemGPS(): " + m_Player.HasItemGPS() );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasGPSForCompassItem: " + m_HasGPSForCompassItem );
					#endif
					
					if ( m_Player.HasItemGPS() )
					{
						m_HasGPSForCompassItem = true;
						CompassShow();
						
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Player has Item GPS but compass is not visible. SHOW!");
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemGPS(): " + m_Player.HasItemGPS() );
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasGPSForCompassItem: " + m_HasGPSForCompassItem );
						#endif
					}
				}
				
				if ( GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Need Compass Item for Show Compass HUD" );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemCompass(): " + m_Player.HasItemCompass() );
					EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasCompassItem: " + m_HasCompassItem );
					#endif
					
					if ( m_Player.HasItemCompass() )
					{
						m_HasCompassItem = true;
						CompassShow();
						
						#ifdef EXPANSIONEXLOGPRINT
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - Player has Item Compass but compass is not visible. SHOW!");
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - HasItemCompass(): " + m_Player.HasItemCompass() );
						EXLogPrint("ExpansionIngameHudEventHandler::UpdateCompass - m_HasCompassItem: " + m_HasCompassItem );
						#endif
					}
				}
			}
		}
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
		
		if ( !GetExpansionHud().GetGPSState() )
		{			
			GPSShow();
		}
		else if ( GetExpansionHud().GetGPSState() )
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
		
		GetExpansionHud().ShowGPS( true );
		
		if ( GetExpansionHud().GetGPSMapStatsState() )
			GetExpansionHud().ShowGPSMapStats( false );
		
		if ( !GetExpansionHud().GetGPSMapState() )
			GetExpansionHud().ShowGPSMap( true );
	}
	
	// ------------------------------------------------------------
	// Expansion GPSHide
	// ------------------------------------------------------------
	void GPSHide()
	{
		GetExpansionHud().ShowGPS( false );
	}
	
	// ------------------------------------------------------------
	// Expansion WasGPSOpened
	// ------------------------------------------------------------
	bool WasGPSOpened()
	{
		return m_GPSWasOpened;
	}
	
	// ------------------------------------------------------------
	// Expansion WasGPSOpened
	// ------------------------------------------------------------
	void SetWasGPSOpened(bool state)
	{
		m_GPSWasOpened = state;
		ToggleHUDGPS();
	}
	
	
	// ------------------------------------------------------------
	// Expansion ToggleHUDCompass
	//! Hides/Shows HUD Compass elements
	// ------------------------------------------------------------
	void ToggleHUDCompass()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::ToggleHUDCompass - Start");
		#endif
		
		CompassShow(!GetExpansionHud().GetCompassState());

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionIngameHudEventHandler::ToggleHUDCompass - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CompassShow
	// ------------------------------------------------------------
	void CompassShow(bool state = true)
	{
		GetExpansionHud().ShowCompass( state );
	}
	
	// ------------------------------------------------------------
	// Expansion CompassHide
	// ------------------------------------------------------------
	void CompassHide()
	{
		CompassShow( false );
	}
	
	// ------------------------------------------------------------
	// Expansion WasCompassOpened
	// ------------------------------------------------------------
	bool WasCompassOpened()
	{
		return m_CompassWasOpened;
	}
	
	// ------------------------------------------------------------
	// Expansion SetWasCompassOpened
	// ------------------------------------------------------------
	void SetWasCompassOpened(bool state)
	{
		m_CompassWasOpened = state;
		ToggleHUDCompass();
	}
}