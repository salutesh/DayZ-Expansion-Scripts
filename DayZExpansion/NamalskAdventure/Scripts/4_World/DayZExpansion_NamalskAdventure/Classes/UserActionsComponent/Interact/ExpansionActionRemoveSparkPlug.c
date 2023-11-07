/**
 * ExpansionActionRemoveSparkPlug.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRemoveSparkPlug: ActionInteractBase
{
	void ExpansionActionRemoveSparkPlug()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		EntityAI target_EAI = EntityAI.Cast(target.GetObject());
		string selection = target_EAI.GetActionComponentName(target.GetComponentIndex());
		int slot_id_plug = InventorySlots.GetSlotIdFromString("SparkPlug");
		ItemBase sparkPlug = ItemBase.Cast(target_EAI.GetInventory().FindAttachment(slot_id_plug));
		
		EXTrace.Add(trace, target_EAI);
		EXTrace.Add(trace, selection);
		EXTrace.Add(trace, sparkPlug);
		
		if (selection != "sparkplug" || !sparkPlug)
			return false;

		m_Text = "Remove";
		if (item && item.GetType() == "SparkPlug" && !item.IsRuined())
			m_Text = "Switch";
		
		return true;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		EntityAI target_EAI = EntityAI.Cast(action_data.m_Target.GetObject());
		int slot_id_plug = InventorySlots.GetSlotIdFromString("SparkPlug");
		ItemBase sparkPlug = ItemBase.Cast(target_EAI.GetInventory().FindAttachment(slot_id_plug));
		
		//! Get current health level of the spark plug
		float resultHealth = sparkPlug.GetHealth01("", "");
		
		//! Delete the spark plug from the entity attachment slot
		GetGame().ObjectDelete(sparkPlug);
		
		//! If player has an other spark plug in his hands try the switch
		if (action_data.m_MainItem && action_data.m_MainItem.GetType() == "SparkPlug" && !action_data.m_MainItem.IsRuined())
		{
			target_EAI.LocalTakeEntityAsAttachment(action_data.m_MainItem);
		}
		
		//! Spawn the "old" spark plug in the players hands
		ItemBase newSparkPlug = ItemBase.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("SparkPlug"));
		if (newSparkPlug)
			newSparkPlug.SetHealth01("", "", resultHealth);
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		EntityAI target_EAI = EntityAI.Cast(action_data.m_Target.GetObject());
		int slot_id_plug = InventorySlots.GetSlotIdFromString("SparkPlug");
		ItemBase sparkPlug = ItemBase.Cast(target_EAI.GetInventory().FindAttachment(slot_id_plug));
		
		//! Get current health level of the spark plug
		float resultHealth = sparkPlug.GetHealth01("", "");
		
		//! Delete the spark plug from the entity attachment slot
		GetGame().ObjectDelete(sparkPlug);
		
		//! If player has an other spark plug in his hands try the switch
		if (action_data.m_MainItem && action_data.m_MainItem.GetType() == "SparkPlug" && !action_data.m_MainItem.IsRuined())
		{
			target_EAI.LocalTakeEntityAsAttachment(action_data.m_MainItem);
		}
		
		//! Spawn the "old" spark plug in the players hands
		ItemBase newSparkPlug = ItemBase.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("SparkPlug"));
		if (newSparkPlug)
			newSparkPlug.SetHealth01("", "", resultHealth);
	}
};