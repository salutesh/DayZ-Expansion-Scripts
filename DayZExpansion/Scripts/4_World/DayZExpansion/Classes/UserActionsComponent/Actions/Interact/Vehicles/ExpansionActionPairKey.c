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
	protected CarScript m_Car;
	protected ExpansionCarKey m_KeysInHand;

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
		m_Car = NULL;
		m_KeysInHand = NULL;

		if ( !target || !player )
			return false;

		if ( !Class.CastTo( m_Car, target.GetObject() ) )
		{
			return false;
		}

		if ( !Class.CastTo( m_KeysInHand, player.GetItemInHands() ) )
		{
			return false;
		}

		if ( m_Car.HasKey() )
		{
			return false;
		}

		if ( m_KeysInHand.IsPaired() )
		{
			return false;
		}
		
		if ( m_KeysInHand.IsInherited( ExpansionCarAdminKey ) )
		{
			return false;
		}
		
		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );

		m_Car.PairKeyTo( m_KeysInHand );
	}

	override bool CanBeUsedInRestrain()
	{
		return false;
	}
};
