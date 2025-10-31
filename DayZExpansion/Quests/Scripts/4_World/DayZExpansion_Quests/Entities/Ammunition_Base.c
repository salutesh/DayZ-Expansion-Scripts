/**
 * Ammunition_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Ammunition_Base
{
	override bool CanAddCartridges(int count)
	{
		if (!super.CanAddCartridges(count))
			return false;

		if (Expansion_IsQuestItem())
			return false;

		return true;
	}

	override bool IsCompatiableAmmo( ItemBase ammo )
	{
		if (!super.IsCompatiableAmmo(ammo))
			return false;

		if (Expansion_IsQuestItem() || ammo.Expansion_IsQuestItem())
		{
			if (Expansion_IsDeliveryItem() || ammo.Expansion_IsDeliveryItem())
				return false;
			
			if (Expansion_GetQuestID() != ammo.Expansion_GetQuestID())
				return false;
		}

		return true;
	}
	
	override bool CanBeSplit()
	{
		if (!super.CanBeSplit())
			return false;
		
		if (!m_Expansion_SkipQuestItemCheck && Expansion_IsQuestItem())
			return false;

		return true;
	}

	//! @note Can't call super here, need access to new pile :-(
	override void SplitItemToInventoryLocation(notnull InventoryLocation dst)
	{
		if (!CanBeSplit())
			return;

		Magazine new_pile = Magazine.Cast(GameInventory.LocationCreateEntity(dst, GetType(), ECE_IN_INVENTORY, RF_DEFAULT));
		if (new_pile)
		{
			MiscGameplayFunctions.TransferItemProperties(dst.GetItem(), new_pile);

			int numberOfTransferredCartridges = Expansion_TransferCartridges(new_pile, Math.Floor(GetAmmoCount() * 0.5));

			if (numberOfTransferredCartridges)
			{
				Expansion_OnStackSizeChanged(-numberOfTransferredCartridges);
				new_pile.Expansion_OnStackSizeChanged(numberOfTransferredCartridges);
			}
		}
	}

	//! @note Can't call super here, need access to new pile :-(
	override void SplitItem(PlayerBase player)
	{
		if (!CanBeSplit())
			return;

		Magazine new_pile = Magazine.Cast(player.CreateCopyOfItemInInventoryOrGround(this));
		if (new_pile)
		{
			int numberOfTransferredCartridges = Expansion_TransferCartridges(new_pile, Math.Floor(GetAmmoCount() * 0.5));

			if (numberOfTransferredCartridges)
			{
				Expansion_OnStackSizeChanged(-numberOfTransferredCartridges);
				new_pile.Expansion_OnStackSizeChanged(numberOfTransferredCartridges);
			}
		}
	}

	override void CombineItems(ItemBase other_item, bool use_stack_max = false)
	{
		int ammoCount = GetAmmoCount();

		super.CombineItems(other_item, use_stack_max);

		if (!CanBeCombined(other_item))
			return;
		
		if (other_item.GetType() != GetType())
			return;

		int delta = GetAmmoCount() - ammoCount;
		if (delta)
		{
			Expansion_OnStackSizeChanged(delta);

			Ammunition_Base otherPile;
			if (Class.CastTo(otherPile, other_item))
				otherPile.Expansion_OnStackSizeChanged(-delta);
		}
	}
};
