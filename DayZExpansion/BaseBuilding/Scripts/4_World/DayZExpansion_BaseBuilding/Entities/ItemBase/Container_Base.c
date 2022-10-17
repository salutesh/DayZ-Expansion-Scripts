/**
 * Container_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Container_Base
{
	int m_Expansion_EntityStorageID = -1;  //! Legacy
	bool m_Expansion_HasEntityStorage;
	bool m_Expansion_RestoringContents;
	ref ExpansionGlobalID m_Expansion_GlobalID = new ExpansionGlobalID();

	void Container_Base()
	{
		RegisterNetSyncVariableBool("m_Expansion_HasEntityStorage");
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			//! Convert legacy
			if (m_ExpansionSaveVersion == 42 && m_Expansion_EntityStorageID != -1)
			{
				if (!m_Expansion_GlobalID.m_IsSet)
					m_Expansion_GlobalID.Acquire();

				string oldFileName = ExpansionEntityStorageModule.GetFileName(m_Expansion_EntityStorageID);
				if (FileExist(oldFileName) && CopyFile(oldFileName, Expansion_GetEntityStorageFileName()))
					DeleteFile(oldFileName);
			}

			m_Expansion_HasEntityStorage = Expansion_HasEntityStorage();
			SetSynchDirty();

			//! Prevent dupes
			if (m_Expansion_HasEntityStorage && !Expansion_IsEmptyIgnoringLockedSlots())
				Expansion_DeleteContentsExceptLockedSlots();
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && m_Expansion_GlobalID.m_IsSet)
		{
			string fileName = Expansion_GetEntityStorageFileName();
			if (FileExist(fileName))
				DeleteFile(fileName);
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

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		if (!m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();

		m_Expansion_GlobalID.OnStoreSave(ctx);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		int version = ctx.GetVersion();
		if (version < 42)
			return true;

		if (version == 42)
		{
			if (!ctx.Read(m_Expansion_EntityStorageID))
				return false;
		}

		if (version < 43)
			return true;

		if (!m_Expansion_GlobalID.OnStoreLoad(ctx))
			return false;

		return true;
	}
	#endif

	override void SetActions()
	{
		super.SetActions();

		AddAction(ExpansionActionStoreContents);
		AddAction(ExpansionActionRestoreContents);
	}

	override bool Expansion_HasEntityStorage()
	{
		if (GetGame().IsClient())
			return m_Expansion_HasEntityStorage;

		if (!m_Expansion_GlobalID.m_IsSet)
			return false;

		return FileExist(Expansion_GetEntityStorageFileName());
	}

	bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		if (!restoreOverride && !GetExpansionSettings().GetBaseBuilding().EnableVirtualStorage)
			return false;

		if (ExpansionStatic.IsAnyOf(this, GetExpansionSettings().GetBaseBuilding().VirtualStorageExcludedContainers, true))
			return false;

		return true;
	}

	void Expansion_DeleteContentsExceptLockedSlots()
	{
		array<EntityAI> items = new array<EntityAI>;
		GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		InventoryLocation il = new InventoryLocation();
		foreach (EntityAI item: items)
		{
			if (item != this)
			{
				item.GetInventory().GetCurrentInventoryLocation(il);
				if (il.GetType() != InventoryLocationType.ATTACHMENT || (!GetInventory().GetSlotLock(il.GetSlot()) && !item.IsKindOf("CombinationLock") && !item.IsKindOf("ExpansionCodeLock")))
					GetGame().ObjectDelete(item);
			}
		}
	}

	bool Expansion_IsEmptyIgnoringLockedSlots()
	{
		int count = GetInventory().AttachmentCount();
		InventoryLocation il = new InventoryLocation();
		for (int i = 0; i < GetInventory().AttachmentCount(); i++)
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex(i);
			if (attachment)
			{
				attachment.GetInventory().GetCurrentInventoryLocation(il);
				if (GetInventory().GetSlotLock(il.GetSlot()) || attachment.IsKindOf("CombinationLock") || attachment.IsKindOf("ExpansionCodeLock"))
					count--;
			}
		}
		CargoBase cargo = GetInventory().GetCargo();
		if (cargo)
			count += cargo.GetItemCount();
		return count == 0;
	}

	string Expansion_GetEntityStorageFileName()
	{
		return ExpansionEntityStorageModule.GetFileName(m_Expansion_GlobalID.IDToHex());
	}

	bool Expansion_StoreContents()
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

	bool Expansion_RestoreContents(PlayerBase player = null)
	{
		if (m_Expansion_HasEntityStorage && m_Expansion_GlobalID.m_IsSet && Expansion_IsEmptyIgnoringLockedSlots())
		{
			//! Set to true so we can receive stored atts and cargo
			m_Expansion_RestoringContents = true;

			EntityAI entity = this;
			bool success = ExpansionEntityStorageModule.RestoreFromFile(Expansion_GetEntityStorageFileName(), entity, null, player);
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

			//! Set to false again
			m_Expansion_RestoringContents = false;

			return success;
		}

		return false;
	}

	override void Open()
	{
		super.Open();

		if (GetGame().IsServer() && Expansion_CanUseVirtualStorage(true))
			Expansion_RestoreContents();
	}

	override void Close()
	{
		if (GetGame().IsServer() && Expansion_CanUseVirtualStorage())
			Expansion_StoreContents();

		super.Close();
	}
}
