/**
 * ExpansionActionAdminUnpairKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleActionAdminUnpairKey: ActionInteractBase
{
	void ExpansionVehicleActionAdminUnpairKey()
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
		return "#STR_EXPANSION_UNPAIR_KEY";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target || !player )
			return false;

		ExpansionVehicleBase car;
		if ( !Class.CastTo( car, target.GetParentOrObject() ) )
			return false;

		ExpansionCarAdminKey key;
		if ( !Class.CastTo( key, player.GetItemInHands() ) )
			return false;

		if ( !car.HasKey() )
			return false;
		
		return true;
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		ExpansionVehicleBase car = ExpansionVehicleBase.Cast( action_data.m_Target.GetParentOrObject() );

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
		ExpansionCarKey.GetKeysForVehicle( car, keys );

		for ( int i = 0; i < keys.Count(); ++i )
			keys[i].Unpair( true );
			
#ifdef EXPANSIONMOD
		if ( GetExpansionSettings().GetLog().AdminTools )
			GetExpansionSettings().GetLog().PrintLog("[AdminTools] Player \"" + action_data.m_Player.GetIdentity().GetName() + "\" (id=" + action_data.m_Player.GetIdentity().GetId() + " pos=" + action_data.m_Player.GetPosition() + ")" + " used the Admin CarKey to unpair a " + car.GetType() );
#endif

		car.ResetKeyPairing();
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
};