/**
 * ActionAttachToConstruction.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionAttachToConstruction
{	
	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition(player, target, item))
			return true;

		ExpansionBaseBuilding exbb;
		if (Class.CastTo(exbb, target.GetObject()))
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			int slot_id = construction_action_data.GetAttachmentSlotFromSelection(player, exbb, item, "interact");
			if (slot_id != -1)
			{
				if (exbb.CheckSlotVerticalDistance(slot_id, player) && exbb.IsPlayerInside(player, ""))
				{
					construction_action_data.SetSlotId(slot_id);
					return true;
				}
			}
		}

		return false;
	}
}
