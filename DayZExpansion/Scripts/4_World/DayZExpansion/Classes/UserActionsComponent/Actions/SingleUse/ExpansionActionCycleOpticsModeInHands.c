/**
 * ExpansionActionCycleOpticsModeInHands.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Todo, once this is working try to make it work like weapon flashlights so it isnt needed to remove item to hands. 
class ExpansionActionCycleOpticsModeInHands extends ActionSingleUseBase
{
	void ExpansionActionCycleOpticsModeInHands()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "#STR_USRACT_CYCLE_RETICLE";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ITEM_ON;
		
		ExpansionReflexMRSOptic optic = ExpansionReflexMRSOptic.Cast( item );
		if ( optic )
		{
			return true;
		}

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
	}
	
	override void OnStartClient( ActionData action_data )
	{
		//! sound handling ?
	}
	
	override void OnStartServer( ActionData action_data )
	{
		ExpansionReflexMRSOptic optic = ExpansionReflexMRSOptic.Cast(action_data.m_MainItem);
		if( optic )
		{
			optic.CycleOpticMode();
		}
	}
}