/**
 * ActionGetOutTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class GetOutTransportActionData
{
	bool keepInVehicleSpaceAfterLeave = false;
};

modded class ActionGetOutTransport
{
	/**
	 * Very sorry other modders, can't call super in this function. Will cause crashes.
	 * If bohemia overrides ::OnStart instead of ::Start then I will be able to call super
	 */
	override void Start( ActionData action_data )
	{
		action_data.m_State = UA_START;
		
		OnStart( action_data );
		
		if ( GetGame().IsServer() )
		{
			OnStartServer( action_data );
		} else
		{
			OnStartClient( action_data );
		}	
		
		InformPlayers( action_data.m_Player, action_data.m_Target, UA_START );

		actionDebugPrint( "[action] " + Object.GetDebugName( action_data.m_Player ) + " started " + ToString() + " item=" + Object.GetDebugName( action_data.m_MainItem ) );

		PerformGetOutTransport( action_data );

		if ( IsMissionClient() )
		{
			GetUApi().GetInputByName( "UACarLeft" ).ForceDisable( false );
			GetUApi().GetInputByName( "UACarRight" ).ForceDisable( false );
			GetUApi().GetInputByName( "UACarForward" ).ForceDisable( false );
			GetUApi().GetInputByName( "UACarBack" ).ForceDisable( false );
			
			GetUApi().GetInputByName( "UACarShiftGearUp" ).ForceDisable( false );
			GetUApi().GetInputByName( "UACarShiftGearDown" ).ForceDisable( false );
		}
	}

	private void PerformGetOutTransport( ActionData action_data )
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand )
		{
			CarScript car;
			if ( Class.CastTo( car, vehCommand.GetTransport() ) )
			{
				float speed = car.GetSpeedometer();
				bool keepInVehicleSpaceAfterLeave = s_ExpansionPlayerAttachment && car.CanObjectAttach( action_data.m_Player ) && car.LeavingSeatDoesAttachment( vehCommand.GetVehicleSeat() );

				GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast( action_data );
				got_action_data.m_StartLocation = got_action_data.m_Player.GetPosition();
				got_action_data.m_Car = car;
				got_action_data.m_CarSpeed = speed;
				got_action_data.keepInVehicleSpaceAfterLeave = keepInVehicleSpaceAfterLeave;

				vehCommand.KeepInVehicleSpaceAfterLeave( keepInVehicleSpaceAfterLeave );

				if ( keepInVehicleSpaceAfterLeave )
				{
					vehCommand.GetOutVehicle();
				} else if ( speed <= 8 )
				{
					vehCommand.GetOutVehicle();
				} else
				{
					got_action_data.m_WasJumpingOut = true;
					vehCommand.JumpOutVehicle();
				}

				//action_data.m_Player.GetItemAccessor().HideItemInHands(false);
				//action_data.m_Player.GetItemAccessor().OnItemInHandsChanged();
				
				GetDayZGame().GetBacklit().OnLeaveCar();
				if ( action_data.m_Player.GetInventory() )
					action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);							
			}
		}
	}

	override void OnEndServer( ActionData action_data )
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast( action_data );
		if ( got_action_data.keepInVehicleSpaceAfterLeave )
			return;

		super.OnEndServer( action_data );
	}
};