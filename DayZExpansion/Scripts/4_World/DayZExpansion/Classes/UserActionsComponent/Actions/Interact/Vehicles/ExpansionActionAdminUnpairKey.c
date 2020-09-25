/**
 * ExpansionActionAdminUnpairKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionAdminUnpairKey: ActionInteractBase
{
	void ExpansionActionAdminUnpairKey()
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

		CarScript car;
		if ( !Class.CastTo( car, target.GetObject() ) )
			return false;

		ExpansionCarAdminKey key;
		if ( !Class.CastTo( key, player.GetItemInHands() ) )
			return false;

		if ( !car.HasKey() )
			return false;
		
		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );

		CarScript car = CarScript.Cast( action_data.m_Target.GetObject() );

		array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
		ExpansionCarKey.GetKeysForVehicle( car, keys );

		for ( int i = 0; i < keys.Count(); ++i )
			keys[i].Unpair( true );

		if ( keys.Count() != 0 )
			car.ResetKeyPairing();
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
}
