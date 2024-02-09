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

	//! Always allow receiving attachments if hierarchy root is not player
	//! --------------------------------------------------------------------------------------------------------------

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!GetHierarchyRootPlayer())
			return true;

		return super.CanReceiveAttachment(attachment, slotId);
	}

	//! Always allow loading attachments
	//! @note this override is currently not technically needed because vanilla ItemBase::CanLoadAttachment is commented out,
	//! but you never know...
	override bool CanLoadAttachment(EntityAI attachment)
	{
		return true;
	}

	//! Fix vanilla CanDisplayAttachmentSlot(string slot_name) not calling CanDisplayAttachmentSlot(int slot_id)
	//! --------------------------------------------------------------------------------------------------------------

	override bool CanDisplayAttachmentSlot(string slot_name)
	{
		if (!m_Expansion_CDASObsoleteError)
		{
			string tmp;
			DumpStackString(tmp);
			TStringArray lines = {};
			tmp.Split("\n", lines);

			TStringArray stack = {};

			for (int i = 1; i < lines.Count(); i++)
			{
				string line = lines[i];

				stack.Insert(line);

				if (line.IndexOf("CanDisplayAttachmentSlot()") == -1)
					break;
			}

			if (stack.Count() > 1)
				EXError.ErrorOnce(this, "Obsolete function - use CanDisplayAttachmentSlot with slot id parameter", stack);
			else
				EXError.ErrorOnce(null, "Obsolete function call - use CanDisplayAttachmentSlot with slot id parameter", stack);

			m_Expansion_CDASObsoleteError = true;
		}

		return CanDisplayAttachmentSlot(InventorySlots.GetSlotIdFromString(slot_name));
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

	bool Expansion_IsInventoryLocked()
	{
		EntityAI root = GetHierarchyRoot();
		if (root != this && root.GetInventory() && root.GetInventory().IsInventoryLocked())
			return true;

		return GetInventory().IsInventoryLocked();
	}

	//! --------------------------------------------------------------------------------------------------------------

	//! @note unlike vanilla, use max lifetime of the instance, not max lifetime from CE
	//! Fixes stone oven despawning prematurely, among other things, and properly supports setting max lifetime via script
	override void MaxLifetimeRefreshCalc()
	{
		if ( (!GetGame().IsMultiplayer() || GetGame().IsServer()) && GetEconomyProfile() )
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
