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
	protected ItemBase m_Item;

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
		m_Item = item;
		if (m_Item && m_Item.GetQuestID() == -1 || !m_Item.IsQuestGiver())
		{
			m_Item = null;
			return false;
		}

		int questID = m_Item.GetQuestID();
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
		if (ExpansionQuestModule.GetModuleInstance() && m_Item)
			ExpansionQuestModule.GetModuleInstance().RequestOpenQuestMenu(action_data.m_Player.GetIdentity(), m_Item.GetQuestID());
	}
};