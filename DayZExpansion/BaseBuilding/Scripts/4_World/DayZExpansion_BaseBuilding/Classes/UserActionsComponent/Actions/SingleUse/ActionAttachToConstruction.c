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

	int Expansion_GetAttachmentSlotID(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionBaseBuilding exbb;
		if (Class.CastTo(exbb, target.GetObject()))
		{
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			int slot_id = construction_action_data.GetAttachmentSlotFromSelection(player, exbb, item, "interact");
			if (slot_id != -1)
			{
				if (exbb.CheckSlotVerticalDistance(slot_id, player) && exbb.IsPlayerInside(player, ""))
				{
					return slot_id;
				}
			}
		}

		return InventorySlots.INVALID;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (super.ActionCondition(player, target, item))
			return true;

		int slotID = Expansion_GetAttachmentSlotID(player, target, item);
		if (slotID != InventorySlots.INVALID)
		{
		#ifdef DAYZ_1_29
			ConstructionActionData construction_action_data = player.GetConstructionActionData();
			construction_action_data.SetSlotId(slotID);
		#endif

			return true;
		}

		return false;
	}

#ifndef DAYZ_1_29
	//! 1.30+
	override bool Post_SetupAction( ActionData action_data )
	{
		int slotID = Expansion_GetAttachmentSlotID(action_data.m_Player, action_data.m_Target, action_data.m_MainItem);
		if (slotID != InventorySlots.INVALID)
		{	
			if (!g_Game.IsDedicatedServer())
				AttachToConstructionActionData.Cast(action_data).m_SlotID = slotID;
		
			return true;
		}

		return super.Post_SetupAction(action_data);
	}
#endif
}
