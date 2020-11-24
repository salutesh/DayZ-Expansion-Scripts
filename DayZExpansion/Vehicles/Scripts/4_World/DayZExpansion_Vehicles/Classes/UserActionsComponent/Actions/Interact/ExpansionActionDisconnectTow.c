/**
 * ExpansionActionDisconnectTow.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_VEHICLE_TOWING
class ExpansionActionDisconnectTow: ActionInteractBase
{
	private bool m_IsWinch;

	void ExpansionActionDisconnectTow()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if ( m_IsWinch )
			return "#STR_EXPANSION_DISCONNECT_WINCH";

		return "#STR_EXPANSION_DISCONNECT_TOW";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{
			if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				m_IsWinch = car.IsHelicopter();

				return car.IsTowing();
			}
		}
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();

		CarScript car;
		if ( vehCommand && Class.CastTo( car, vehCommand.GetTransport() ) )
		{				
			if ( car.CrewMemberIndex( action_data.m_Player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				car.DestroyTow();
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}
#endif