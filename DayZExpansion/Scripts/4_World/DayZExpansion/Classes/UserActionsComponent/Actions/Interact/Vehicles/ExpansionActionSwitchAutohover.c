/**
 * ExpansionActionSwitchAutoHover.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionSwitchAutoHover: ActionInteractBase
{
	private ExpansionHelicopterScript m_Vehicle;

	void ExpansionActionSwitchAutoHover()
	{
		m_CommandUID		= DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask		= DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon		= CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}	

	override string GetText()
	{
		if ( m_Vehicle.IsAutoHover() )
			return "#STR_EXPANSION_VEHICLE_TOGGLE_AUTOHOVER_OFF";

		return "#STR_EXPANSION_VEHICLE_TOGGLE_AUTOHOVER_ON";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( Class.CastTo( m_Vehicle, player.GetParent() ) )
		{
			if ( m_Vehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				return true;
			}
		}

		return false;
	}

	override void OnAnimationEvent( ActionData action_data )
	{
		if ( IsMissionClient() && !action_data.m_WasExecuted )
			m_Vehicle.SwitchAutoHover();
		
		super.OnAnimationEvent( action_data );
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}

class ExpansionActionSwitchAutoHoverInput: ExpansionActionSwitchAutoHover
{
	override typename GetInputType()
	{
		return ExpansionInputSwitchAutoHover;
	}
}