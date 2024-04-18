/**
 * ExpansionActionUpgradePersonalStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUpgradePersonalStorage: ActionSingleUseBase
{
	void ExpansionActionUpgradePersonalStorage()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "#STR_EXPANSION_PERSONALSTORAGE_ACTION_UPGRADE";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTCursorNoObject(UAMaxDistances.DEFAULT);
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionPersonalStorageHub hub;
		if (!Class.CastTo(hub, target.GetObject()))
			return false;

		ExpansionPersonalStorageUpgrade upgrade;
		if (!Class.CastTo(upgrade, item))
			return false;

		return upgrade.Expansion_CanUpgradePersonalStorage(player);
	}

	override void OnExecuteServer(ActionData action_data)
	{
		ExpansionPersonalStorageHub hub;
		if (Class.CastTo(hub, action_data.m_Target.GetObject()))
			hub.Expansion_ClosePersonalStorage(action_data.m_Player);

		ExpansionPersonalStorageUpgrade upgrade;
		if (Class.CastTo(upgrade, action_data.m_MainItem))
			upgrade.Expansion_UpgradePersonalStorage(action_data.m_Player);
	}
}
