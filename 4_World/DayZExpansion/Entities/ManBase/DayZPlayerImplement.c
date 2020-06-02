/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static bool m_ExpansionShowDeadScreen = false;

modded class DayZPlayerImplement
{
	private ExpansionParachute m_ActiveParachute;
	
	// ------------------------------------------------------------
	// DayZPlayerImplement Constructor
	// ------------------------------------------------------------
	void DayZPlayerImplement()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::DayZPlayerImplement - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::DayZPlayerImplement - End");
		#endif
	}

	// ------------------------------------------------------------
	// DayZPlayerImplement ShowDeadScreen
	// ------------------------------------------------------------	
	override void ShowDeadScreen(bool show, float duration)
	{
	#ifndef NO_GUI		
		if ( show && IsPlayerSelected() )
		{
			GetGame().GetUIManager().ScreenFadeIn( duration, "", FadeColors.BLACK, FadeColors.WHITE );
			GetGame().GetUIManager().EnterScriptedMenu(MENU_INGAME, null);
			//! m_ExpansionShowDeadScreen = true;
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
			//! GetGame().GetUIManager().CloseAll();
			//! m_ExpansionShowDeadScreen = false;
		}
		
		if (duration > 0)
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( StopDeathDarkeningEffect, duration*1000, false );
			m_ExpansionShowDeadScreen = true;
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( StopDeathDarkeningEffect );
			GetGame().GetUIManager().CloseAll();
			m_ExpansionShowDeadScreen = false;
		}
	#endif
	}

	// ------------------------------------------------------------
	// DayZPlayerImplement SendChatMessage
	// ------------------------------------------------------------
	void SendChatMessage( string message )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::SendChatMessage - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::SendChatMessage - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement StartCommand_ExpansionVehicle
	// ------------------------------------------------------------
	ExpansionHumanVehicleCommand StartCommand_ExpansionVehicle( Object vehicle, int seatIdx, int seat_anim )
	{
		return NULL;
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionVehicle
	// ------------------------------------------------------------
	ExpansionHumanVehicleCommand GetCommand_ExpansionVehicle()
	{
		return ExpansionHumanVehicleCommand.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetExpansionTransport
	// ------------------------------------------------------------
	ExpansionVehicleScript GetExpansionTransport()
	{
		ExpansionHumanVehicleCommand script;
		if ( Class.CastTo( script, GetCommand_Script() ) )
		{
			return script.GetTransport();
		}

		return NULL;
	}
	
	void OnCommandExpansionVehicleStart()
	{
	}
	
	void OnCommandExpansionVehicleFinish()
	{
	}
		
	// ------------------------------------------------------------
	// DayZPlayerImplement DeployParachute
	// ------------------------------------------------------------
	void DeployParachute( ExpansionParachuteBag bag )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::DeployParachute - Start");
		#endif
		
		ExpansionHumanFallCommand fallCommand;
		if ( Class.CastTo( fallCommand, GetCommand_Script() ) )
		{
			m_ActiveParachute = fallCommand.DeployParachute( bag );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::DeployParachute - End");
		#endif
	}

	bool HasParachute()
	{
		return m_ActiveParachute != NULL;
	}
		
	// ------------------------------------------------------------
	// DayZPlayerImplement CutParachute
	// ------------------------------------------------------------
	void CutParachute()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::CutParachute - Start");
		#endif
		
		if ( m_ActiveParachute )
		{
			m_ActiveParachute.CutParachute();

			m_ActiveParachute = NULL;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::CutParachute - End");
		#endif
	}
			
	// ------------------------------------------------------------
	// DayZPlayerImplement StartCommand_ExpansionFall
	// ------------------------------------------------------------
	void StartCommand_ExpansionFall( float pYVelocity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::StartCommand_ExpansionFall - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::StartCommand_ExpansionFall - End");
		#endif
	}
			
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionFall
	// ------------------------------------------------------------
	ExpansionHumanFallCommand GetCommand_ExpansionFall()
	{		
		return ExpansionHumanFallCommand.Cast( GetCommand_Script() );
	}
				
	// ------------------------------------------------------------
	// DayZPlayerImplement HeadingModel
	// ------------------------------------------------------------
	override bool HeadingModel( float pDt, SDayZPlayerHeadingModel pModel )
	{		
		if ( GetCommand_ExpansionVehicle() )
		{
			m_fLastHeadingDiff = 0;
			#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::HeadingModel - Return false");
		#endif
			return false;
		}
		
		return super.HeadingModel( pDt, pModel );
	}
				
	// ------------------------------------------------------------
	// DayZPlayerImplement CameraHandler
	// ------------------------------------------------------------
	override int CameraHandler( int pCameraMode )
	{
		ExpansionVehicleScript exTrans;
		ExpansionHumanVehicleCommand vehicleCommand = GetCommand_ExpansionVehicle();
		if ( vehicleCommand )
			exTrans = vehicleCommand.GetTransport();

		if ( m_Camera3rdPerson )
		{
			if ( vehicleCommand && exTrans )
			{
				int cameraType = exTrans.Get3rdPersonCameraType();
				if ( cameraType != -1 )
					return cameraType;
			}

			/*CarScript car;
			if ( Class.CastTo( car, GetParent() ) && !GetCommand_Vehicle() )
			{
				int carCameraType = car.Get3rdPersonCameraType();
				if ( carCameraType != -1 )
				{
					#ifdef EXPANSIONEXPRINT
		EXPrint( "DayZPlayerImplement::CameraHandler - Return carCameraType: " + carCameraType.ToString() );
		#endif
					return carCameraType;
				}
			}*/

			if ( HasParachute() )
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;
			}
		} else
		{
			if ( vehicleCommand )
			{
				return DayZPlayerCameras.DAYZCAMERA_1ST_VEHICLE;
			}
		}

		return super.CameraHandler( pCameraMode );
	}
		
	// ------------------------------------------------------------
	// DayZPlayerImplement CommandHandler
	// ------------------------------------------------------------
	override void CommandHandler( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::CommandHandler - Start");
		#endif

		/*	
		Print( "Player Instance Type: " + GetInstanceType() );

		Print( "INSTANCETYPE_SERVER: " + DayZPlayerInstanceType.INSTANCETYPE_SERVER );
		Print( "INSTANCETYPE_CLIENT: " + DayZPlayerInstanceType.INSTANCETYPE_CLIENT );
		Print( "INSTANCETYPE_REMOTE: " + DayZPlayerInstanceType.INSTANCETYPE_REMOTE );

		Print( "INSTANCETYPE_AI_SERVER: " + DayZPlayerInstanceType.INSTANCETYPE_AI_SERVER );
		Print( "INSTANCETYPE_AI_REMOTE: " + DayZPlayerInstanceType.INSTANCETYPE_AI_REMOTE );
		Print( "INSTANCETYPE_AI_SINGLEPLAYER: " + DayZPlayerInstanceType.INSTANCETYPE_AI_SINGLEPLAYER );
		*/
		
		super.CommandHandler( pDt, pCurrentCommandID, pCurrentCommandFinished );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerImplement::CommandHandler - End");
		#endif
	}
}