/**
 * ActionTurnOffPowerGenerator.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTurnOffPowerGenerator
{
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor(UAMaxDistances.LARGE);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionGenerator_Base pg;
		if ( Class.CastTo(pg,target.GetObject()) )
		{
			if ( pg.HasEnergyManager()  &&  pg.GetCompEM().CanSwitchOff() )
			{
				string selection = pg.GetActionComponentName( target.GetComponentIndex() );
				if ( pg.IsSwitch(selection) )
					return true;
			}
			return false;
		}
		
		return super.ActionCondition(player,target,item);
	}
};
