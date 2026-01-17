/**
 * ExpansionActionUseQuestItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUseQuestItem: ActionSingleUseBase
{
	void ExpansionActionUseQuestItem()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DROPITEM_HANDS;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_PRONE;
		m_Text = "???";
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

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item && (item.Expansion_GetQuestID() == -1 || !item.Expansion_IsQuestGiver()))
			return false;

		int questID = item.Expansion_GetQuestID();
		string playerUID = player.GetIdentity().GetId();
		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetPlayerQuestData(playerUID);
		if (!playerData)
			return false;

		ExpansionQuestState questState = playerData.GetQuestStateByQuestID(questID);
		if (questState > ExpansionQuestState.NONE)
			return false;

		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (ExpansionQuestModule.GetModuleInstance() && action_data.m_MainItem)
			ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenuForQuest(action_data.m_Player.GetIdentity(), action_data.m_MainItem.Expansion_GetQuestID());
	}
};