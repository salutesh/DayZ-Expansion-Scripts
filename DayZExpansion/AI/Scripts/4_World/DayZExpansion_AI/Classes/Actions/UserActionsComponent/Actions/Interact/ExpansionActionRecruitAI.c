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

		eAIGroup group = tAI.GetGroup();
		eAIGroup playerGroup = player.GetGroup();

		if (group == playerGroup)
			return false;

		//! Don't allow to recruit other player's AI
		if (group.GetLeader() && !group.GetLeader().IsAI())
			return false;

		eAIFaction faction = group.GetFaction();

		//! Don't allow to recruit passive or invincible AI
		if (tAI.eAI_IsPassive() || faction.IsInvincible())
			return false;

		auto settings = GetExpansionSettings().GetAI(false);

		if (!settings.IsLoaded())
			return false;

		bool isPlayerMoving;
		bool friendly;
		if (faction.IsGuard())
		{
			if (!settings.CanRecruitGuards)
				return false;

			if (GetGame().IsServer() && tAI.eAI_GetTargetThreat(player.GetTargetInformation()) > 0.2)
			{
				ExpansionNotification("STR_EXPANSION_HOSTILE", "STR_EXPANSION_AI_CANNOT_RECRUIT_HOSTILE_TEMP").Error(player.GetIdentity());
				return false;
			}
		}
		else if (!settings.CanRecruitFriendly)
		{
			return false;
		}
		else if (tAI.PlayerIsEnemy(player, false, isPlayerMoving, friendly))
		{
			if (GetGame().IsServer() && friendly)
				ExpansionNotification("STR_EXPANSION_HOSTILE", "STR_EXPANSION_AI_CANNOT_RECRUIT_HOSTILE_TEMP").Error(player.GetIdentity());

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
