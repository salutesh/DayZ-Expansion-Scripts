/**
 * ExpansionActionDisconnectWinch.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDisconnectWinch: ActionInteractBase
{
	protected ExpansionHelicopterScript m_Helicopter;

	void ExpansionActionDisconnectWinch()
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
		return "#STR_USRACT_DISCONNECT_WINCH";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//Removed
		//return false;

		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				if ( Class.CastTo( m_Helicopter, trans ) )
				{
					if ( m_Helicopter.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
					{
                        return false;//m_Helicopter.IsWinched();
                    }
                }
            }
        }
		return false;
	}
	
	override void OnStartServer( ActionData action_data )
	{
        super.OnStartServer( action_data );

        if ( !m_Helicopter )
            return;

        // m_Helicopter.DestroyWinch();

        Message( action_data.m_Player, "Disconnected winch" );
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}