/**
 * ExpansionActionUncoverVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUncoverVehicle: ExpansionActionRestoreEntity
{
	void ExpansionActionUncoverVehicle()
	{
		m_Text = "#STR_EXPANSION_ACTION_UNCOVER";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		if (item)
			return false;

		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		ExpansionEntityStoragePlaceholder placeholder;
		if (!Expansion_OnFinishProgressServer(action_data, false, placeholder))
			return;

		string placeholderType = placeholder.GetType();
		string coverType = "CamoNet";
	
		if (placeholderType.Contains("_Civil"))
			coverType += "Civil";
		else if (placeholderType.Contains("_Desert"))
			coverType += "Desert";
		else if (placeholderType.Contains("_Winter"))
			coverType += "Winter";

		if (!action_data.m_Player.GetHumanInventory().CreateInHands(coverType))
			GetGame().CreateObject(coverType, action_data.m_Player.GetPosition());

		auto camoNetAttachment = placeholder.GetInventory().FindAttachment(InventorySlots.GetSlotIdFromString("CamoNet"));
		//! TODO: Find a way to take existing attachment to hand instead of recreating. ServerTakeEntityToHands doesn't work :-(
		auto camoNet = action_data.m_Player.GetHumanInventory().CreateInHands(coverType);
		if (camoNet && camoNetAttachment)
			camoNet.SetHealth(camoNetAttachment.GetHealth());

		GetGame().ObjectDelete(placeholder);
	}
}
