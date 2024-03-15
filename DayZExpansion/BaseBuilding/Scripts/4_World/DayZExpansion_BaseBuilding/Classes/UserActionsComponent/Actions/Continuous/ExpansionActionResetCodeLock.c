/**
 * ExpansionActionResetCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionResetCodeLockCB: ActionContinuousBaseCB
{	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(0.75);
	}
};

class ExpansionActionResetCodeLock: ActionContinuousBase
{
	void ExpansionActionResetCodeLock()
	{
		m_CallbackClass = ExpansionActionResetCodeLockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_Text = "#STR_EXPANSION_BB_CODE_RESET_CODE";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ItemBase targetItem;
		if (Class.CastTo(targetItem, target.GetParentOrObject()))
		{
			ExpansionCodeLock cl = targetItem.ExpansionGetCodeLock();
			string selection = targetItem.GetActionComponentName(target.GetComponentIndex());

			if (cl && cl.IsLockedInSlot() && !targetItem.ExpansionIsLocked() && targetItem.IsPlayerInside(player, selection))
				return true;
		}

		return false;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
	#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this, "" + action_data.m_Target.GetParentOrObject());
	#endif

		ItemBase targetItem;
		if (Class.CastTo(targetItem, action_data.m_Target.GetParentOrObject()))
			targetItem.SetCode("");
	}

	override string GetAdminLogMessage(ActionData action_data)
	{
		return " reset " + action_data.m_Target.GetParentOrObject().GetDisplayName() + " with " + action_data.m_MainItem.GetDisplayName() + " at " + action_data.m_Target.GetParentOrObject().GetPosition();
	}
};
