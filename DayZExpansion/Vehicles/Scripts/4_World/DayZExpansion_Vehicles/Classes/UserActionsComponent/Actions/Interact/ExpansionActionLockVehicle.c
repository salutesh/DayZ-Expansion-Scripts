/**
 * ExpansionActionLockVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionLockVehicle: ActionInteractBase
{
	//! DO NOT STORE VARIABLES FOR SERVER SIDE OPERATION

	void ExpansionActionLockVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#lock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car;
		ExpansionCarKey key;

		if ( player.GetCommand_Vehicle() )
		{
			if ( !Class.CastTo( car, player.GetCommand_Vehicle().GetTransport() ) )
				return false;
		} else
		{
			if ( !Class.CastTo( car, target.GetObject() ) )
				return false;

			if ( !Class.CastTo( key, player.GetItemInHands() ) )
				return false;

			if ( !car.IsCarKeys( key ) )
				return false;
		}

		if ( !car.HasKey() )
			return false;

		if ( car.GetLockedState() == ExpansionVehicleLockState.LOCKED || car.GetLockedState() == ExpansionVehicleLockState.READY_TO_LOCK )
			return false;
		
		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );
				
		CarScript car;
		ExpansionCarKey key;

		if ( action_data.m_Player.GetCommand_Vehicle() )
		{
			car = CarScript.Cast( action_data.m_Player.GetCommand_Vehicle().GetTransport() );
		} else
		{
			car = CarScript.Cast( action_data.m_Target.GetObject() );
			key = ExpansionCarKey.Cast( action_data.m_Player.GetItemInHands() );
		}
		
		if ( car )
		{
			car.LockCar( key );
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
};