/**
 * ExpansionActionInsertSupplyCrateKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionInsertSupplyCrateKey: ActionSingleUseBase
{
	void ExpansionActionInsertSupplyCrateKey()
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
		if (!IsInReach(player, target, UAMaxDistances.DEFAULT))
			return false;

		if (item.IsDamageDestroyed())
			return false;
		
		Expansion_SupplyCrate_Key_Base key = Expansion_SupplyCrate_Key_Base.Cast(item);
		if (!key)
			return false;

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(target.GetObject());
		if (!crate)
			return false;

		if (key.GetKeyType() != crate.GetValidKeyType())
			return false;
		
		if (crate.HasLootDelay())
			return false;

		return crate.CanInsertKey();
	}

	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(action_data.m_Target.GetObject());
		if (crate)
		{
			ClearInventoryReservationEx(action_data);
			AttachKey(action_data);
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(action_data.m_Target.GetObject());
		if (crate)
		{
			ClearInventoryReservationEx(action_data);
			AttachKey(action_data);
		}
	}

	protected void AttachKey(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(action_data.m_Target.GetObject());
		Expansion_SupplyCrate_Key_Base key = Expansion_SupplyCrate_Key_Base.Cast(action_data.m_MainItem);
		if (crate && key)
		{
			int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
			action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(crate, key, slot_id_key);
		}
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};