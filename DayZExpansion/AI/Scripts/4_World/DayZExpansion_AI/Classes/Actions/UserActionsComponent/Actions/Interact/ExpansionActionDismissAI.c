/**
 * ExpansionActionDismissAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDismissAI: ActionInteractBase
{
	void ExpansionActionDismissAI()
	{
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREMOD_MOVE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.Cursor;
		m_Text = "#STR_EXPANSION_AI_DISMISS";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool CanBeUsedSwimming()
	{
		return true;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		eAIBase tAI;
		if (!Class.CastTo(tAI, target.GetObject()))
			return false;

		if (!tAI.IsAlive() || tAI.IsUnconscious())
			return false;

		if (!player.GetGroup())
			return false;

		if (player.GetGroup() != tAI.GetGroup())
			return false;

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		auto tAI = eAIBase.Cast(action_data.m_Target.GetObject());

		eAIGroup group = tAI.GetGroup();  //! This is the group that both the player and AI are currently in

		eAIGroup formerGroup = tAI.Expansion_GetFormerGroup();

		//! If AI has no former group or is the same as player group, create empty group
		if (!formerGroup || formerGroup == action_data.m_Player.GetGroup())
			formerGroup = eAIGroup.CreateGroup(group.GetFaction());

		//! Add AI to group
		tAI.SetGroup(formerGroup);

		//! Remove player from group if they had no former group
		if (group.Count() == 1 && !action_data.m_Player.Expansion_GetFormerGroup())
			action_data.m_Player.SetGroup(null);
	}
};
