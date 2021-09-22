/**
 * DayZPlayerImplement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static bool m_ExpansionShowDeadScreen = false;

modded class DayZPlayerImplement
{
	//private ExpansionParachute m_ActiveParachute;

	protected bool m_WasRaisedIronsight;
	
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
		super.ShowDeadScreen( show, duration );
		
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
			
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				m_ExpansionShowDeadScreen = true;
		}
		else
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Call( StopDeathDarkeningEffect );
			
			if (GetExpansionSettings().GetGeneral().UseDeathScreen)
				m_ExpansionShowDeadScreen = false;
		}
		#endif
	}
	
	/*
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
	*/
	/*
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
	*/

	// ------------------------------------------------------------
	// DayZPlayerImplement StartCommand_ExpansionFall
	// ------------------------------------------------------------
	override void StartCommand_ExpansionFall( float pYVelocity )
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
	// DayZPlayerImplement CameraHandler
	// ------------------------------------------------------------
	override int CameraHandler( int pCameraMode )
	{
		ExpansionVehicleBase exTrans;
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

			//if ( HasParachute() )
			//{
			//	return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;
			//}
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

	override void HandleWeapons( float pDt, Entity pInHands, HumanInputController pInputs, out bool pExitIronSights )
	{
		super.HandleWeapons( pDt, pInHands, pInputs, pExitIronSights );

		if ( m_WasRaisedIronsight == ( m_MovementState.IsRaised() && m_CameraIronsight && !m_CameraOptics ) )
			return;

		m_WasRaisedIronsight = m_MovementState.IsRaised() && m_CameraIronsight && !m_CameraOptics;

		Weapon_Base weapon = Weapon_Base.Cast( pInHands );
		if ( weapon )
		{
			ItemOptics optic = weapon.GetAttachedOptics();
			if ( optic )
			{
				if ( optic.IsKindOf( "ExpansionHAMROptic" ) )
				{
					// Check for secondary optic and switch it on/off appropriately when going in/out of optics view
					ItemOptics secondaryOptic = ItemOptics.Cast( optic.FindAttachmentBySlotName( "pistolOptics" ) );
					if ( secondaryOptic && secondaryOptic.HasEnergyManager() )
					{
						if ( m_CameraIronsight && !m_CameraOptics )
						{
							secondaryOptic.GetCompEM().SwitchOn();
							secondaryOptic.HideSelection("hide");
						} else
						{
							secondaryOptic.GetCompEM().SwitchOff();
							secondaryOptic.ShowSelection("hide");
						}
					}
				}
			}
		}
	}
}