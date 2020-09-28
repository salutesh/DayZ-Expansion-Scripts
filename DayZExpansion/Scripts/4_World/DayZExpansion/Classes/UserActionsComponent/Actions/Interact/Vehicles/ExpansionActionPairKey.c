/**
 * ExpansionActionPairKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionPairKey: ActionInteractBase
{
	void ExpansionActionPairKey()
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
		return "#STR_EXPANSION_PAIR_KEY";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car;
		ExpansionCarKey key;

		if ( !Class.CastTo( car, target.GetObject() ) )
		{
			return false;
		}

		if ( !Class.CastTo( key, player.GetItemInHands() ) )
		{
			return false;
		}

		if ( car.HasKey() )
		{
			return false;
		}

		if ( key.IsPaired() )
		{
			return false;
		}
		
		if ( key.IsInherited( ExpansionCarAdminKey ) )
		{
			return false;
		}
		
		return true;
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		CarScript car = CarScript.Cast( action_data.m_Target.GetObject() );
		car.PairKeyTo( ExpansionCarKey.Cast( action_data.m_Player.GetItemInHands() ) );
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
}
