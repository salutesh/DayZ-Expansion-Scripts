/**
 * ExpansionActionTurnOffGeneratorOnGround.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionTurnOffGeneratorOnGround: ActionInteractBase
{
	private ExpansionGenerator m_Generator;

	// ------------------------------------------------------------
	void ExpansionActionTurnOffGeneratorOnGround()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
	}

	// ------------------------------------------------------------
	override string GetText()
	{
		return "#switch_off";
	}

	// ------------------------------------------------------------
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !Class.CastTo( m_Generator, target.GetObject() ) )
		{
			return false;
		}		
		
		if ( !m_Generator.HasEnergyManager() )
		{
			return false;
		}

		if ( !m_Generator.GetCompEM().CanSwitchOff() )
		{
			return false;
		}
		
		return true;
	}
	
	// ------------------------------------------------------------
	override void OnExecuteServer( ActionData action_data )
	{
		m_Generator.GetCompEM().SwitchOff();
	}
}