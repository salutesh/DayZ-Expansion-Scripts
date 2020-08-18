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
				if ( s_ExpansionPlayerAttachment && car.CanObjectAttach( action_data.m_Player ) && car.LeavingSeatDoesAttachment( vehCommand.GetVehicleSeat() ) )
				{
					vehCommand.KeepInVehicleSpaceAfterLeave( true );

					vehCommand.GetOutVehicle();
				} else if ( car.GetSpeedometer() <= 8 )
				{
					vehCommand.GetOutVehicle();
				} else
				{
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

	
	override void OnUpdate( ActionData action_data )
	{
		if ( action_data.m_State && action_data.m_State == UA_START)
		{
			if ( !action_data.m_Player.GetCommand_Vehicle() )
			{
				End( action_data );
				return;
			}
		}

		super.OnUpdate( action_data );
	}
};