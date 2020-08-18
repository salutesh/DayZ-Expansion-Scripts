/**
 * ExpansionActionDisconnectElectricityToSource.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDisconnectElectricityToSource: ActionInteractBase
{
	void ExpansionActionDisconnectElectricityToSource()
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
		return "Disconnect";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ItemBase elec_power = ItemBase.Cast( target.GetObject() );
		if ( !elec_power || !player.GetItemInHands() )
			return false;

		if ( !elec_power.ExpansionIsPowerSource() )
			return false;

		if ( !player.GetItemInHands().ExpansionCanRecievePower() )
			return false;

		if ( !player.GetItemInHands().ExpansionIsConnected( elec_power ) )
			return false;
			
		// Is he in a territory ?
		if ( player.IsInTerritory() )
			return player.IsInsideOwnTerritory(); // show the prompt if it's his territory


		return true;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		ItemBase elec_recieve = ItemBase.Cast( action_data.m_Player.GetItemInHands() );

		elec_recieve.ExpansionDisconnect();
	}

}
