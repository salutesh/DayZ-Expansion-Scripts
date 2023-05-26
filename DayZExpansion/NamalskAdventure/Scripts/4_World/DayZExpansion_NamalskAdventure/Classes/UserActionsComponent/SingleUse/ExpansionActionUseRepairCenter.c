/**
 * ExpansionActionUseRepairCenter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUseRepairCenter: ActionInteractBase
{
	void ExpansionActionUseRepairCenter()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override string GetText()
	{
		return "Use Workbench";
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Land_Repair_Center repair_center;
		if (!Class.CastTo(repair_center, target.GetObject()))
		{
			return false;
		}

		bool is_in_range = vector.Distance(repair_center.WorldToModel(player.GetPosition()), "0.0 -1.5 0.0") < 1.5;
		bool is_active = repair_center.GetActiveState();
		return (is_in_range && is_active);
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		super.OnExecuteClient(action_data);
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("NA_WorkbenchMenu");
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Land_Repair_Center repair_center;
		if (!Class.CastTo(repair_center, action_data.m_Target.GetObject()))
		{
			return;
		}
		
		PlayerBase player;
		if (!Class.CastTo(player, action_data.m_Player))
		{
			return;
		}
		
		ExpansionNamalskModule.GetModuleInstance().SendWorkbenchData(repair_center, player.GetIdentity());
	}
}