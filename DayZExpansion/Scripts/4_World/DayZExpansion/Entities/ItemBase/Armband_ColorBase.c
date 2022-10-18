/**
 * Armband_ColorBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHardlineArmband extends Armband_ColorBase
{
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		if (!parent.IsInherited(SurvivorBase))
			return;

		if (slot_id != InventorySlots.GetSlotIdFromString("Armband"))
			return;

		super.OnWasAttached(parent, slot_id);
	}
	
	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}
	
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
};

class ExpansionArmbandBandit extends ExpansionHardlineArmband {};
class ExpansionArmbandHero extends ExpansionHardlineArmband {};
