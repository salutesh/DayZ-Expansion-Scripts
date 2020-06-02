/**
 * ExpansionActionCutParachute.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class ExpansionActionCutParachuteCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousQuantityEdible(UAQuantityConsumed.EAT_NORMAL,UATimeSpent.DEFAULT);
	}
}

class ExpansionActionCutParachute: ActionContinuousBase
{
	ExpansionParachuteBag m_Bag;

	void ExpansionActionCutParachute()
	{
		m_CallbackClass = ExpansionActionCutParachuteCB;
		// TODO: Register coord cut action in anim graph
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
		m_CommandUIDProne = DayZPlayerConstants.CMD_ACTIONFB_EAT;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTSelf;
	}

	override string GetText()
	{
		return "#STR_USRACT_CUT_PARACHUTE";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{		
        if ( !player.HasParachute() )
		{
			return false;
		}

		return true;
	}
	
	override void OnEndClient( ActionData action_data )
	{
        action_data.m_Player.CutParachute();
	}
	
	override void OnEndServer( ActionData action_data )
	{
        action_data.m_Player.CutParachute();
	}
}