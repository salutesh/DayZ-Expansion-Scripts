/**
 * ExpansionActionDebugRestoreEntity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDebugRestoreEntity: ActionInteractBase
{
	string m_EntityName;

	void ExpansionActionDebugRestoreEntity()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
	}

	override string GetText()
	{
		return "[DEBUG] Restore " + m_EntityName;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		auto placeholder = ExpansionDebugGoat.Cast(target.GetObject());
		if (!placeholder)
			return false;

		m_EntityName = placeholder.Expansion_GetStoredEntityDisplayName();

		return true;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		auto placeholder = ExpansionDebugGoat.Cast(action_data.m_Target.GetParentOrObject());
		if (!placeholder)
			return;

		if (ExpansionEntityStorageModule.RestoreFromFile(placeholder.m_Expansion_EntityStorageID))
			GetGame().ObjectDelete(placeholder);
	}
}
