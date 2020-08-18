/**
 * ExpansionActionTogglePowerSwitch.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionTogglePowerSwitch: ActionInteractBase
{
	void ExpansionActionTogglePowerSwitch()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Flip switch";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		ItemBase target_PG = ItemBase.Cast( targetObject );
		if ( !target_PG )
			return false;

		if ( target_PG.ExpansionIsPowerSource() && target_PG.HasEnergyManager() /*&&  target_PG.GetCompEM().CanSwitchOn() && target_PG.GetCompEM().CanWork()*/ )
			return true;

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject )
		{
			ItemBase target_IB = ItemBase.Cast( targetObject );
			if ( target_IB.GetCompEM().IsSwitchedOn() )
			{
				target_IB.GetCompEM().SwitchOff();
			} else
			{
				target_IB.GetCompEM().SwitchOn();
			}
		}
	}
}
