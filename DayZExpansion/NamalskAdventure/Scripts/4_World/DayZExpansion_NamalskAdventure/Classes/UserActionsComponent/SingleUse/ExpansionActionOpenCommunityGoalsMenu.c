/**
 * ExpansionActionOpenCommunityGoalsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenCommunityGoalsMenu: ActionInteractBase
{
	void ExpansionActionOpenCommunityGoalsMenu()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override string GetText()
	{
		return "Community Goals";
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Expansion_CommunityGoals_Board goals_board;
		if (!Class.CastTo(goals_board, target.GetObject()))
		{
			return false;
		}
		
		return true;
	}

	override void OnExecuteClient(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		super.OnExecuteClient(action_data);
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("NA_CommunityGoalsMenu");
	}

	override void OnExecuteServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		Expansion_CommunityGoals_Board goals_board;
		if (!Class.CastTo(goals_board, action_data.m_Target.GetObject()))
		{
			return;
		}

		PlayerBase player;
		if (!Class.CastTo(player, action_data.m_Player))
		{
			return;
		}

		ExpansionCommunityGoalsModule.GetModuleInstance().SendCommunityGoalData(goals_board, player.GetIdentity());
	}
}