/**
 * ExpansionActionSwitchBoatController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionSwitchBoatController: ActionInteractBase
{
	private ExpansionBoatScript m_Boat;

	void ExpansionActionSwitchBoatController()
	{
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	

	override string GetText()
	{
		if ( m_Boat.IsUsingBoatController() )
			return "#STR_EXPANSION_VEHICLE_BOAT_TOGGLE_MOTOR_OFF";

		return "#STR_EXPANSION_VEHICLE_BOAT_TOGGLE_MOTOR_ON";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			if ( Class.CastTo( m_Boat, vehCommand.GetTransport() ) )
			{
				if ( m_Boat.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					if ( m_Boat.EngineIsOn() || m_Boat.MotorIsOn() )
						return false;

                    return m_Boat.IsCar();
				}
			}
		}

		return false;
	}

	override void OnAnimationEvent( ActionData action_data )
	{
		if ( IsMissionClient() && !action_data.m_WasExecuted )
        {
			m_Boat.SetUsingBoatController( !m_Boat.IsUsingBoatController() );
        }
		
		super.OnAnimationEvent( action_data );
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}

class ExpansionActionSwitchBoatControllerInput: ExpansionActionSwitchBoatController
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return false;
	}
}