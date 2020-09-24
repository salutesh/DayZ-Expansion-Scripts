/**
 * ExpansionActionStartPlayingGuitar.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionStartPlayingGuitar: ActionSingleUseBase
{
	void ExpansionActionStartPlayingGuitar()
	{
		m_StanceMask = /* DayZPlayerConstants.STANCEMASK_CROUCH | */ DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCIPresent;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Start Playing Guitar";
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Expansion_Guitar guitar;
		if ( !Class.CastTo( guitar, item ) )
			return false;

		if ( player.GetCommand_Vehicle() )
			return false;

		if ( player.GetCommand_ExpansionGuitar() )
			return false;

		return true;
	}

	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		
		action_data.m_Player.StartCommand_ExpansionGuitar( Expansion_Guitar.Cast( action_data.m_MainItem ) );
	}
}