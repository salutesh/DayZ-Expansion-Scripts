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

	protected ref ExpansionHumanST m_ExpansionST;
	
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
			GetGame().GetUIManager().ScreenFadeIn( duration, "#dayz_implement_dead", FadeColors.BLACK, FadeColors.WHITE );
		}
		else
		{
			GetGame().GetUIManager().ScreenFadeOut(0);
		}
		
		if (duration > 0)
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( StopDeathDarkeningEffect, duration*1000, false );
			
			if (GetExpansionSettings().GetGeneral().UseDeadScreen)
				m_ExpansionShowDeadScreen = true;
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( StopDeathDarkeningEffect );
			
			if (GetExpansionSettings().GetGeneral().UseDeadScreen)
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
	// DayZPlayerImplement StartCommand_ExpansionGuitar
	// ------------------------------------------------------------
	ExpansionHumanCommandGuitar StartCommand_ExpansionGuitar( Expansion_Guitar guitar )
	{
		return NULL;
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionGuitar
	// ------------------------------------------------------------
	ExpansionHumanCommandGuitar GetCommand_ExpansionGuitar()
	{
		return ExpansionHumanCommandGuitar.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement StartCommand_ExpansionVehicle
	// ------------------------------------------------------------
	ExpansionHumanCommandVehicle StartCommand_ExpansionVehicle( Object vehicle, int seatIdx, int seat_anim )
	{
		return NULL;
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionVehicle
	// ------------------------------------------------------------
	ExpansionHumanCommandVehicle GetCommand_ExpansionVehicle()
	{
		return ExpansionHumanCommandVehicle.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement StartCommand_ExpansionLeaveVehicle
	// ------------------------------------------------------------
	ExpansionHumanCommandLeavingVehicle StartCommand_ExpansionLeaveVehicle( Object vehicle )
	{
		return NULL;
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionVehicle
	// ------------------------------------------------------------
	ExpansionHumanCommandLeavingVehicle GetCommand_ExpansionLeaveVehicle()
	{
		return ExpansionHumanCommandLeavingVehicle.Cast( GetCommand_Script() );
	}
	
	// ------------------------------------------------------------
	// DayZPlayerImplement GetExpansionTransport
	// ------------------------------------------------------------
	ExpansionVehicleScript GetExpansionTransport()
	{
		ExpansionHumanCommandVehicle script;
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
		
		ExpansionHumanCommandFall fallCommand;
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
	}
			
	// ------------------------------------------------------------
	// DayZPlayerImplement GetCommand_ExpansionFall
	// ------------------------------------------------------------
	ExpansionHumanCommandFall GetCommand_ExpansionFall()
	{		
		return ExpansionHumanCommandFall.Cast( GetCommand_Script() );
	}
				
	// ------------------------------------------------------------
	// DayZPlayerImplement HeadingModel
	// ------------------------------------------------------------
	override bool HeadingModel( float pDt, SDayZPlayerHeadingModel pModel )
	{		
		if ( GetCommand_ExpansionVehicle() )
		{
			m_fLastHeadingDiff = 0;

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
		ExpansionHumanCommandVehicle vehicleCommand = GetCommand_ExpansionVehicle();
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
	
	override bool IsShootingFromCamera()
	{
		bool disableMagicCrosshair = GetExpansionSettings().GetGeneral().DisableMagicCrosshair; 
		return !disableMagicCrosshair;
	}	
}