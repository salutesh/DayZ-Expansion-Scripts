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
	bool m_Expansion_HasEntityStorage;
	bool m_Expansion_RestoringContents;
	ref ExpansionGlobalID m_Expansion_GlobalID;

	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsServer() && GetGame().IsMultiplayer() && m_Expansion_GlobalID)
		{
			m_Expansion_HasEntityStorage = Expansion_HasEntityStorage();
			SetSynchDirty();

			//! Prevent dupes
			if (m_Expansion_HasEntityStorage && !Expansion_IsEmptyIgnoringLockedSlots())
				Expansion_DeleteContentsExceptLockedSlots();
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && m_Expansion_GlobalID && m_Expansion_GlobalID.m_IsSet)
		{
			if (GetHierarchyRoot() != ExpansionEntityStorageModule.GetSavedEntityToBeDeleted())
				ExpansionEntityStorageModule.DeleteFiles(m_Expansion_GlobalID.IDToHex());
		}

		super.EEDelete(parent);
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
		if (!super.CanDisplayAttachmentCategory(category_name))
			return false;

		if (m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		if (m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanDisplayCargo()
	{
		if (!super.CanDisplayCargo())
			return false;

		if (m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		if (!super.CanPutInCargo(parent))
			return false;

		if (m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		if (!super.CanPutIntoHands(parent))
			return false;
		
		if (m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (m_Expansion_RestoringContents)
			return true;

		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		//! Check for m_Initialized set by vanilla DeferredInit() to make sure already attached items can be loaded from storage
		if (m_Initialized && m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (m_Expansion_RestoringContents)
			return true;

		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		//! Check for m_Initialized set by vanilla DeferredInit() to make sure cargo items can be loaded from storage
		if (m_Initialized && m_Expansion_HasEntityStorage)
			return false;

		return true;
	}

	bool Expansion_HasEntityStorage()
	{
		if (GetGame().IsClient())
			return m_Expansion_HasEntityStorage;

		if (!m_Expansion_GlobalID || !m_Expansion_GlobalID.m_IsSet)
			return false;

		return FileExist(Expansion_GetEntityStorageFileName());
	}

	override bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		if (restoreOverride)
			return m_Expansion_HasEntityStorage;

		auto settings = GetExpansionSettings().GetBaseBuilding(false);

		if (!settings.IsLoaded() || !settings.EnableVirtualStorage)
			return false;

		if (ExpansionStatic.IsAnyOf(this, settings.VirtualStorageExcludedContainers, true))
			return false;

		return true;
	}

	void Expansion_DeleteContentsExceptLockedSlots()
	{
		int i;
		if (!ExpansionIsOpenable() && !IsNonExpansionOpenable())
		{
			InventoryLocation il = new InventoryLocation();
			for (i = GetInventory().AttachmentCount() - 1; i >= 0; i--)
			{
				EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
				if (attachment)
				{
					attachment.GetInventory().GetCurrentInventoryLocation(il);
					if (!GetInventory().GetSlotLock(il.GetSlot()) && !attachment.IsKindOf("CombinationLock") && !attachment.IsKindOf("ExpansionCodeLock"))
						GetGame().ObjectDelete(attachment);
				}
			}
		}
		CargoBase cargo = GetInventory().GetCargo();
		if (cargo)
		{
			for (i = cargo.GetItemCount() - 1; i >= 0; i--)
			{
				EntityAI item = cargo.GetItem(i);
				if (item)
					GetGame().ObjectDelete(item);
			}
		}
	}

	bool Expansion_IsEmptyIgnoringLockedSlots()
	{
		int count;
		if (!ExpansionIsOpenable() && !IsNonExpansionOpenable())
		{
			InventoryLocation il = new InventoryLocation();
			for (int i = 0; i < GetInventory().AttachmentCount(); i++)
			{
				EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
				if (attachment)
				{
					attachment.GetInventory().GetCurrentInventoryLocation(il);
					if (!GetInventory().GetSlotLock(il.GetSlot()) && !attachment.IsKindOf("CombinationLock") && !attachment.IsKindOf("ExpansionCodeLock"))
						count++;
				}
			}
		}
		CargoBase cargo = GetInventory().GetCargo();
		if (cargo)
			count += cargo.GetItemCount();
		return count == 0;
	}

	override string Expansion_GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(m_Expansion_GlobalID.IDToHex());
	}

	override bool Expansion_StoreContents()
	{
		if (m_Expansion_HasEntityStorage || Expansion_IsEmptyIgnoringLockedSlots())
			return false;

		if (!m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();

		if (ExpansionEntityStorageModule.SaveToFile(this, Expansion_GetEntityStorageFileName(), true))
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_StoreContents - " + GetPosition() + " - saved inventory with storage ID " + m_Expansion_GlobalID.IDToHex());

			Expansion_DeleteContentsExceptLockedSlots();

			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_StoreContents - " + GetPosition() + " - is inventory empty (ignoring locked slots)? " + Expansion_IsEmptyIgnoringLockedSlots());

			m_Expansion_HasEntityStorage = true;
			SetSynchDirty();

			return true;
		}
		else
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_StoreContents - " + GetPosition() + " - could not save inventory");
		}

		return false;
	}

	override bool Expansion_RestoreContents(PlayerBase player = null)
	{
		if (m_Expansion_HasEntityStorage && m_Expansion_GlobalID.m_IsSet && Expansion_IsEmptyIgnoringLockedSlots())
		{
			//! Set to true so we can receive stored atts and cargo
			m_Expansion_RestoringContents = true;

			//! If container is ruined, set health above zero so we can restore contents
			float health = GetHealth();
			if (!health)
				SetHealth01("", "", 1.0);

			EntityAI entity = this;
			bool success = ExpansionEntityStorageModule.RestoreFromFile(Expansion_GetEntityStorageFileName(), entity);
			if (success)
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_RestoreContents - " + GetPosition() + " - restored inventory with storage ID " + m_Expansion_GlobalID.IDToHex());

				m_Expansion_HasEntityStorage = false;
				SetSynchDirty();
			}
			else
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_RestoreContents - " + GetPosition() + " - could not restore inventory with storage ID " + m_Expansion_GlobalID.IDToHex());
			}

			//! Restore original health
			if (!health)
				SetHealth(0.0);

			//! Set to false again
			m_Expansion_RestoringContents = false;

			return success;
		}

		return false;
	}
};
