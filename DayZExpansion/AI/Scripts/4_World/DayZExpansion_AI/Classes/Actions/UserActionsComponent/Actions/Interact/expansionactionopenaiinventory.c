/**
 * ExpansionActionOpenAIInventory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenAIInventory: ActionInteractBase
{
	void ExpansionActionOpenAIInventory()
	{
		m_CommandUID = DayZPlayerConstants.CMD_GESTUREMOD_COME;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_HUDCursorIcon = CursorIcons.Cursor;
		m_Text = "#STR_EXPANSION_AI_OPEN_INVENTORY";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		eAIBase ai;
		if (!Class.CastTo(ai, target.GetObject()))
			return false;

		if (!ai.IsAlive() || ai.IsUnconscious() || !ai.Expansion_CanBeLooted())
			return false;

		if (!player.GetGroup())
			return false;

		if (player.GetGroup() != ai.GetGroup())
			return false;

		return true;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		auto ai = eAIBase.Cast(action_data.m_Target.GetObject());

		ai.eAI_SetIsInventoryVisible(true, action_data.m_Player);
	}

	override void OnEndClient(ActionData action_data)
	{
		g_Game.GetMission().ShowInventory();
	}
};
