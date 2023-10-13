/**
 * ExpansionActionUnpackHalloweenGift.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUnpackHalloweenGift: ActionContinuousBase
{
	void ExpansionActionUnpackHalloweenGift()
	{
		m_CallbackClass = ActionUnpackGiftCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_LOW;
		m_Text = "#STR_Unwrap";
	}

	override void CreateConditionComponents()
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	override bool HasTarget()	
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		return true;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		Expansion_Halloween_GiftBox_Small gift = Expansion_Halloween_GiftBox_Small.Cast(action_data.m_MainItem);
		if (gift)
		{
			gift.Open();
			gift.DeleteSafe();
		}
	}
};