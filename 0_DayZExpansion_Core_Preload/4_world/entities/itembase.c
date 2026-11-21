/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	bool m_Expansion_CDASObsoleteError;

	//! Always allow putting as attachment if root is player that is not yet loaded
	//! (normal CanPutAsAttachment checks may otherwise fail if they check for items that may not yet be loaded)
	//! --------------------------------------------------------------------------------------------------------------

	override bool CanPutAsAttachment(EntityAI parent)
	{
		PlayerBase player = PlayerBase.Cast(parent.GetHierarchyRootPlayer());

		if (player && !player.IsPlayerLoaded())
			return true;

		return super.CanPutAsAttachment(parent);
	}

	//! Allow receiving attachments with cargo when parent isn't player (e.g. furniture mod items or atts on them)
	//! --------------------------------------------------------------------------------------------------------------

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		GameInventory attachmentInv = attachment.GetInventory();
		if (attachmentInv && attachmentInv.GetCargo() && attachmentInv.GetCargo().GetItemCount() > 0)
		{
			if (GetHierarchyParent() && !GetHierarchyParent().IsInherited(PlayerBase))
				return Ex_NonPlayerParent_CanReceiveAttachmentWithCargo(attachment, slotId);
		}

		return super.CanReceiveAttachment(attachment, slotId);
	}

	bool Ex_NonPlayerParent_CanReceiveAttachmentWithCargo(EntityAI attachment, int slotId)
	{
		bool ret;

		InventoryLocation loc = new InventoryLocation();
		attachment.GetInventory().GetCurrentInventoryLocation(loc);
		if (loc && loc.IsValid() && !GetInventory().AreChildrenAccessible())
			ret = false;
		else
			ret = CheckAttachmentReceiveExclusion(attachment, slotId);  //! Vanilla EntityAI::CanReceiveAttachment just calls CheckAttachmentReceiveExclusion since DayZ 1.27

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "Ex_NPP_CanRecvAttWithCargo " + attachment + " " + InventorySlots.GetSlotName(slotId) + " " + ret);
	#endif

		return ret;
	}

	//! Always allow loading attachments
	//! @note this override is currently not technically needed because vanilla ItemBase::CanLoadAttachment is commented out,
	//! but you never know...
	override bool CanLoadAttachment(EntityAI attachment)
	{
		return true;
	}

	//! Prevent being able to combine/split stacks in locked inventory
	//! --------------------------------------------------------------------------------------------------------------

    override void CombineItemsClient(EntityAI entity2, bool use_stack_max = true)
    {
        if (!Expansion_IsInventoryLocked())
            super.CombineItemsClient(entity2, use_stack_max);
    }

    override void OnRightClick()
    {
        if (!Expansion_IsInventoryLocked())
            super.OnRightClick();
    }

	override bool CanBeSplit()
	{
		if (!super.CanBeSplit())
			return false;

		return !Expansion_IsInventoryLocked();
	}

	override bool CanBeCombined(EntityAI other_item, bool reservation_check = true, bool stack_max_limit = false)
	{
		if (!super.CanBeCombined(other_item, reservation_check, stack_max_limit))
			return false;

		return !Expansion_IsInventoryLocked();
	}

	bool Expansion_IsInventoryLocked(out int lockType = 0, out EntityAI lockedEntity = null, bool checkParent = true)
	{
		return ExpansionStatic.IsInventoryLocked(this, lockType, lockedEntity, checkParent);
	}

	//! --------------------------------------------------------------------------------------------------------------

	//! @note unlike vanilla, use max lifetime of the instance, not max lifetime from CE
	//! Fixes stone oven despawning prematurely, among other things, and properly supports setting max lifetime via script
	override void MaxLifetimeRefreshCalc()
	{
		if ( (!g_Game.IsMultiplayer() || g_Game.IsServer()) && GetEconomyProfile() )
		{
			float lifetime = GetLifetimeMax();
			int frequency = GetCEApi().GetCEGlobalInt("FlagRefreshFrequency");
			if ( frequency <= 0 )
			{
				frequency = GameConstants.REFRESHER_FREQUENCY_DEFAULT;
			}
			
			bool refresherViable;
			if (frequency <= lifetime)
				refresherViable = true;
			if ( refresherViable != m_RefresherViable )
			{
				m_RefresherViable = refresherViable;
				SetSynchDirty();
			}
		}
	}
}
