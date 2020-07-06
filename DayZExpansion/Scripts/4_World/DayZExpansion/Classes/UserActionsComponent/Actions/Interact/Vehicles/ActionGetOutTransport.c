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
	override void Start( ActionData action_data )
	{
		CarScript car;
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			if ( car.CanObjectAttach( action_data.m_Player ) && car.LeavingSeatDoesAttachment( vehCommand.GetVehicleSeat() ) )
			{
				vehCommand.KeepInVehicleSpaceAfterLeave( true );
			} else
			{
				vehCommand.KeepInVehicleSpaceAfterLeave( false );
			}
		}

		super.Start( action_data );

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
};