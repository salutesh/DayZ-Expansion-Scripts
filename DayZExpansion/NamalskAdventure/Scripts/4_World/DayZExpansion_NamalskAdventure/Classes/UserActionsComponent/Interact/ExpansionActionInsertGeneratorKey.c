/**
 * ExpansionActionInsertGeneratorKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionInsertGeneratorKey: ActionSingleUseBase
{
	void ExpansionActionInsertGeneratorKey()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Text = "Insert Key";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item.IsDamageDestroyed())
			return false;

		Expansion_Bunker_Generator trgt = Expansion_Bunker_Generator.Cast(target.GetObject());
		if (!trgt)
			return false;

		return trgt.CanInsertKey();
	}

	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (GetGame().IsMultiplayer())
			return;
		
		ClearInventoryReservationEx(action_data);
		AttachKey(action_data);
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		ClearInventoryReservationEx(action_data);
		AttachKey(action_data);
	}
	
	protected void AttachKey(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Expansion_Bunker_Generator generator = Expansion_Bunker_Generator.Cast(action_data.m_Target.GetObject());
		Expansion_Bunker_Generator_Key key = Expansion_Bunker_Generator_Key.Cast(action_data.m_MainItem);		
		if (generator && key)
		{
			int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
			action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(generator, key, slot_id_key);
		}
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};