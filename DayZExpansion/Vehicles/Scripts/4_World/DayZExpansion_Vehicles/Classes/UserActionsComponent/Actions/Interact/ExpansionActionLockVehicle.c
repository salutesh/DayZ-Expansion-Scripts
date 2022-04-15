/**
 * ExpansionActionLockVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		//! The intention is for vehicles to only be lockable if player has its key (or admin key) in hand

		//! Is the player inside a vehicle ?
		if ( player.GetCommand_Vehicle() )
		{
			if ( !Class.CastTo( car, player.GetCommand_Vehicle().GetTransport() ) )
			{
				return false;
			}
		} else
		{
			//! Is this a "car" ?
			if ( !Class.CastTo( car, target.GetParentOrObject() ) )
			{
				return false;
			}
		}

		// Key in the player hand ?
		if ( !Class.CastTo( key, player.GetItemInHands() ) )
		{
			return false;
		}

		// Key is paired to this "car" ?
		if ( !car.IsCarKeys( key ) && !key.IsInherited( ExpansionCarAdminKey ) )
		{
			return false;
		}

		// Is the car already locked ?
		if ( car.GetLockedState() == ExpansionVehicleLockState.LOCKED || car.GetLockedState() == ExpansionVehicleLockState.READY_TO_LOCK )
		{
			return false;
		}

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
			car = CarScript.Cast( action_data.m_Target.GetParentOrObject() );
			key = ExpansionCarKey.Cast( action_data.m_Player.GetItemInHands() );
		}
		
		if ( car )
		{
			car.LockCar( key );
#ifdef EXPANSIONMODVEHICLE	
			if ( GetExpansionSettings().GetLog().VehicleCarKey && key )
				GetExpansionSettings().GetLog().PrintLog("[VehicleCarKey] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to lock a " + car.GetType() );
#endif

			if ( GetExpansionSettings().GetLog().AdminTools && key && key.IsInherited( ExpansionCarAdminKey ) )
				GetExpansionSettings().GetLog().PrintLog("[AdminTools] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to lock a " + car.GetType() );
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