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
	private bool m_IsUsing;

	void ExpansionActionSwitchBoatController()
	{
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon	 = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	

	override string GetText()
	{
		if ( m_IsUsing )
			return "#STR_EXPANSION_VEHICLE_BOAT_TOGGLE_MOTOR_OFF";

		return "#STR_EXPANSION_VEHICLE_BOAT_TOGGLE_MOTOR_ON";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionBoatScript boat;
		if ( Class.CastTo( boat, player.GetParent() ) )
		{
			m_IsUsing = boat.IsUsingBoatController();
			if ( boat.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				if ( boat.EngineIsOn() || boat.MotorIsOn() )
					return false;

				return boat.IsCar();
			}
		}

		return false;
	}

	override void OnAnimationEvent( ActionData action_data )
	{
		if ( IsMissionClient() && !action_data.m_WasExecuted )
		{
			ExpansionBoatScript boat;
			if ( Class.CastTo( boat, action_data.m_Player.GetParent() ) )
				boat.SetUsingBoatController( !boat.IsUsingBoatController() );
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