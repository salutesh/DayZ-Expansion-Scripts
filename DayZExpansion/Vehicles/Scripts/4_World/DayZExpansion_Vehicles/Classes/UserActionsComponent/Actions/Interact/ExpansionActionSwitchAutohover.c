/**
 * ExpansionActionSwitchAutoHover.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[RegisterAction(ExpansionActionSwitchAutoHover)]
class ExpansionActionSwitchAutoHover: ActionInteractBase
{
	private bool m_IsOn;

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
		if ( m_IsOn )
			return "#STR_EXPANSION_VEHICLE_TOGGLE_AUTOHOVER_OFF";

		return "#STR_EXPANSION_VEHICLE_TOGGLE_AUTOHOVER_ON";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.IsCameraInsideVehicle() && CarDoor.Cast( target.GetObject() ) )
			return false;

		ExpansionHelicopterScript d_helicopter;
		if ( Class.CastTo( d_helicopter, player.GetParent() ) )
		{
			m_IsOn = d_helicopter.IsAutoHover();
			return d_helicopter.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		ExpansionVehicleHelicopterBase helicopter;
		if ( Class.CastTo( helicopter, player.GetParent() ) )
		{
			m_IsOn = helicopter.IsAutoHover();
			return helicopter.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return false;
	}

	override void OnAnimationEvent( ActionData action_data )
	{
		if ( IsMissionClient() && !action_data.m_WasExecuted )
		{
			ExpansionHelicopterScript d_helicopter;
			if ( Class.CastTo( d_helicopter, action_data.m_Player.GetParent() ) )
				d_helicopter.SwitchAutoHover();

			ExpansionVehicleHelicopterBase helicopter;
			if ( Class.CastTo( helicopter, action_data.m_Player.GetParent() ) )
				helicopter.SwitchAutoHover();
		}
		
		super.OnAnimationEvent( action_data );
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};

[RegisterAction(ExpansionActionSwitchAutoHoverInput)]
class ExpansionActionSwitchAutoHoverInput: ExpansionActionSwitchAutoHover
{
	override typename GetInputType()
	{
		return ExpansionInputSwitchAutoHover;
	}
};
