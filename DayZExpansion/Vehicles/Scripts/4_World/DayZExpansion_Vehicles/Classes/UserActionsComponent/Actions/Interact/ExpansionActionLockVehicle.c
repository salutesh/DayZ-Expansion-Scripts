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
		ExpansionVehicle vehicle;
		ExpansionCarKey key;

		//! The intention is for vehicles to only be lockable if player has its key (or admin key) in hand

		if ( !ExpansionVehicle.Get(vehicle, player) && !ExpansionVehicle.Get(vehicle, target.GetParentOrObject()) )
			return false;

		if ( !Class.CastTo( key, player.GetItemInHands() ) )
		{
			return false;
		}

		if ( !vehicle.IsPairedTo( key ) && !key.IsInherited( ExpansionCarAdminKey ) )
		{
			return false;
		}

		if ( vehicle.IsLocked() || vehicle.IsReadyToLock() )
		{
			return false;
		}

		return true;
	}
	
	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );
				
		ExpansionVehicle vehicle;
		ExpansionCarKey key;

		if ( ExpansionVehicle.Get(vehicle, action_data.m_Player) || ExpansionVehicle.Get(vehicle, action_data.m_Target.GetParentOrObject()) )
		{
			key = ExpansionCarKey.Cast( action_data.m_Player.GetItemInHands() );

			vehicle.Lock( key );

			if ( GetExpansionSettings().GetLog().AdminTools && key && key.IsInherited( ExpansionCarAdminKey ) )
				GetExpansionSettings().GetLog().PrintLog("[AdminTools] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to lock " + vehicle.GetType() + " (id=" + vehicle.GetPersistentIDString() + " pos=" + vehicle.GetPosition() + ")");
			else if ( GetExpansionSettings().GetLog().VehicleCarKey && key )
				GetExpansionSettings().GetLog().PrintLog("[VehicleCarKey] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the  "+ key.GetType() +" to lock " + vehicle.GetType() + " (id=" + vehicle.GetPersistentIDString() + " pos=" + vehicle.GetPosition() + ")");
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