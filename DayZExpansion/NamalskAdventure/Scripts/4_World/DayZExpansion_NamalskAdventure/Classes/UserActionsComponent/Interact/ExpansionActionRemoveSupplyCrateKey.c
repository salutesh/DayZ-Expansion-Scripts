/**
 * ExpansionActionRemoveSupplyCrateKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRemoveSupplyCrateKey: ActionInteractBase
{
	protected const int KEY_HEALTH_DELTA = 2;

	void ExpansionActionRemoveSupplyCrateKey()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_LockTargetOnUse = false;
		m_Text = "Remove Key";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem	= new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!IsInReach(player, target, UAMaxDistances.DEFAULT))
			return false;

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(target.GetObject());
		if (!crate)
			return false;

		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
		Expansion_SupplyCrate_Key key = Expansion_SupplyCrate_Key.Cast(crate.GetInventory().FindAttachment(slot_id_key));
		if (!key)
			return false;

		if (!player.GetInventory().CanAddEntityIntoHands(key))
			return false;

		if (key.GetHierarchyRootPlayer() == player)
			return false;

		if (!crate.IsCrateOpened())
			return false;

		return crate.HasKey();
	}

	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(action_data.m_Target.GetObject());
		if (crate)
		{
			DebugTrace("::OnExecuteServer - Crate: " + crate.ToString());
			int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionSupplyCrateKey");
			crate.GetInventory().SetSlotLock(slot_id_key, false);

			Expansion_SupplyCrate_Key currentKey = Expansion_SupplyCrate_Key.Cast(crate.GetInventory().FindAttachment(slot_id_key));
			DebugTrace("::OnExecuteServer - Attached key: " + currentKey.ToString());
			int healthLvl = currentKey.GetHealthLevel();
			GetGame().ObjectDelete(currentKey);

			Expansion_SupplyCrate_Key newKey = Expansion_SupplyCrate_Key.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("Expansion_SupplyCrate_Key"));
			newKey.AddHealthLevel(healthLvl + KEY_HEALTH_DELTA);

			crate.CloseCrate();
		}
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};