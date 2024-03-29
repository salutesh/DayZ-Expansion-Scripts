/**
 * ExpansionActionUnlockVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleActionUnlockVehicle: ActionInteractBase
{
	//! DO NOT STORE VARIABLES FOR SERVER SIDE OPERATION

	void ExpansionVehicleActionUnlockVehicle()
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
		return "#unlock";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{		
		ExpansionVehicleBase car;
		ExpansionCarKey key;
		
		//! The intention is for vehicles to only be unlockable from outside if player has its key (or admin key) in hand,
		//! and to ALWAYS be unlockable from inside (so player can't accidentally lock themself in)

		if ( player.GetCommand_ExpansionVehicle() )
		{
			if ( !Class.CastTo( car, player.GetCommand_ExpansionVehicle().GetExpansionVehicle() ) )
				return false;

			if ( !car.HasKey() )
				return false;
		} else
		{
			if ( !Class.CastTo( car, target.GetParentOrObject() ) )
				return false;

			if ( !Class.CastTo( key, player.GetItemInHands() ) )
				return false;

			if ( !car.IsCarKeys( key ) && !key.IsInherited( ExpansionCarAdminKey ) )
				return false;
		}

		if ( car.GetLockedState() == ExpansionVehicleLockState.UNLOCKED || car.GetLockedState() == ExpansionVehicleLockState.NOLOCK || car.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED )
			return false;
		
		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );
		
		ExpansionVehicleBase car;
		ExpansionCarKey key;

		if ( action_data.m_Player.GetCommand_ExpansionVehicle() )
		{
			car = ExpansionVehicleBase.Cast( action_data.m_Player.GetCommand_ExpansionVehicle().GetObject() );
		} else
		{
			car = ExpansionVehicleBase.Cast( action_data.m_Target.GetParentOrObject() );
			key = ExpansionCarKey.Cast( action_data.m_Player.GetItemInHands() );
		}
		
		if ( car )
		{
			car.UnlockCar( key );			
			
			if ( GetExpansionSettings().GetLog().AdminTools && key && key.IsInherited( ExpansionCarAdminKey ) )
				GetExpansionSettings().GetLog().PrintLog("[AdminTools] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to unlock " + car.GetType() + " (id=" + car.GetVehiclePersistentIDString() + " pos=" + car.GetPosition() + ")");
			else if ( GetExpansionSettings().GetLog().VehicleCarKey && key )
				GetExpansionSettings().GetLog().PrintLog("[VehicleCarKey] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to unlock " + car.GetType() + " (id=" + car.GetVehiclePersistentIDString() + " pos=" + car.GetPosition() + ")");
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