/**
 * ExpansionActionRecruitAI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRecruitAI: ActionInteractBase
{
	void ExpansionActionRecruitAI()
	{
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREMOD_COME;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.Cursor;
		m_Text = "#STR_EXPANSION_AI_RECRUIT";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		eAIBase tAI;
		if (!Class.CastTo(tAI, target.GetObject()))
			return false;

		if (!tAI.IsAlive() || tAI.IsUnconscious())
			return false;

		if (player.GetGroup() == tAI.GetGroup())
			return false;

		//! Don't allow to recruit other player's AI
		if (tAI.GetGroup().GetLeader() && !tAI.GetGroup().GetLeader().IsAI())
			return false;

		//! Don't allow to recruit passive or invincible AI
		if (tAI.eAI_IsPassive() || tAI.GetGroup().GetFaction().IsInvincible())
			return false;

		if (!GetExpansionSettings().GetAI(false).IsLoaded())
			return false;

		if (tAI.GetGroup().GetFaction().IsGuard())
		{
			if (!GetExpansionSettings().GetAI().CanRecruitGuards)
				return false;
		}
		else
		{
			if (tAI.PlayerIsEnemy(player) || !GetExpansionSettings().GetAI().CanRecruitFriendly)
				return false;
		}

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		auto tAI = eAIBase.Cast(action_data.m_Target.GetObject());

		eAIGroup group = action_data.m_Player.GetGroup();

		//! If player has no group, create new group with player as leader
		if (!group)
			group = eAIGroup.GetGroupByLeader(action_data.m_Player, true, tAI.GetGroup().GetFaction());
		else
			action_data.m_Player.Expansion_SetFormerGroup(group);

		//! Add AI to player group
		tAI.SetGroup(group, false);
	}
};
