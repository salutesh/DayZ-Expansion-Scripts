/**
 * ExpansionActionRemoveGeneratorKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRemoveGeneratorKey: ActionInteractBase
{
	protected const int KEY_HEALTH_DELTA = 2;

	void ExpansionActionRemoveGeneratorKey()
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

		Expansion_Bunker_Generator generator = Expansion_Bunker_Generator.Cast(target.GetObject());
		if (!generator)
			return false;

		int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
		Expansion_Bunker_Generator_Key key = Expansion_Bunker_Generator_Key.Cast(generator.GetInventory().FindAttachment(slot_id_key));
		if (!key)
			return false;

		if (!player.GetInventory().CanAddEntityIntoHands(key))
			return false;

		if (key.GetHierarchyRootPlayer() == player)
			return false;

		if (!generator.IsActive())
			return false;

		return generator.HasKey();
	}

	override void OnExecuteServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		Expansion_Bunker_Generator generator = Expansion_Bunker_Generator.Cast(action_data.m_Target.GetObject());
		if (generator)
		{
			ExDebugPrint("::OnExecuteServer - Crate: " + generator.ToString());
			int slot_id_key = InventorySlots.GetSlotIdFromString("Att_ExpansionGeneratorKey");
			generator.GetInventory().SetSlotLock(slot_id_key, false);

			Expansion_Bunker_Generator_Key currentKey = Expansion_Bunker_Generator_Key.Cast(generator.GetInventory().FindAttachment(slot_id_key));
			ExDebugPrint("::OnExecuteServer - Attached key: " + currentKey.ToString());
			int healthLvl = currentKey.GetHealthLevel();
			GetGame().ObjectDelete(currentKey);

			Expansion_Bunker_Generator_Key newKey = Expansion_Bunker_Generator_Key.Cast(action_data.m_Player.GetHumanInventory().CreateInHands("Expansion_Bunker_Generator_Key"));
			newKey.AddHealthLevel(healthLvl + KEY_HEALTH_DELTA);
		}
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};