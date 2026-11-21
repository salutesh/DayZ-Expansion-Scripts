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
	private bool m_Expansion_DoesSupportVirtualStorage;
	bool m_Expansion_HasEntityStorage;
	bool m_Expansion_DeferredDeleteEntityStorageFiles;
	bool m_Expansion_DeletingContents;
	bool m_Expansion_RestoringContents;

	void ItemBase()
	{
		m_Expansion_DoesSupportVirtualStorage = Expansion_DoesSupportVirtualStorage();
		if (m_Expansion_DoesSupportVirtualStorage)
		{
			if (g_Game.IsServer() && !m_Expansion_GlobalID)
				m_Expansion_GlobalID = new ExpansionGlobalID();

			RegisterNetSyncVariableBool("m_Expansion_HasEntityStorage");
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (g_Game.IsServer() && g_Game.IsMultiplayer() && m_Expansion_GlobalID)
		{
			m_Expansion_HasEntityStorage = Expansion_HasEntityStorage();

			if (m_Expansion_HasEntityStorage)
			{
				//! Prevent dupes
				if (!Expansion_IsEmptyIgnoringLockedSlots() && ExpansionEntityStorageModule.DeleteFiles(m_Expansion_GlobalID.IDToHex()))
					m_Expansion_HasEntityStorage = false;

				SetSynchDirty();
			}
		}
	}

//! TODO: This is fine but probably overkill and not really needed. Also, if keeping it, it should be implemented more thoroughly
//! by additionally checking if container is empty if items in cargo of attached items are moved out (OnChildItemRemoved)
/*
	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		Expansion_DeleteEntityStorageFilesIfEmpty();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		Expansion_DeleteEntityStorageFilesIfEmpty();
	}
*/

	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		//! Don't allow to move items into parent with contents in entity storage and move them back to where they came from

		InventoryLocationType oldType = oldLoc.GetType();

		if (oldType == InventoryLocationType.UNKNOWN)
			return;

		if (oldType == InventoryLocationType.TEMP)
			return;

		EntityAI oldParent = oldLoc.GetParent();
		EntityAI newParent = newLoc.GetParent();

		if (oldParent == newParent)
			return;

		ItemBase newParentIB;
		if (!Class.CastTo(newParentIB, newParent))
			return;

		if (!newParentIB.m_Expansion_HasEntityStorage || newParentIB.m_Expansion_RestoringContents)
			return;

		auto dstLoc = new InventoryLocation();
		dstLoc.Copy(oldLoc);

		Man player;
		PlayerIdentity identity;

		if (oldParent)
		{
			player = oldParent.GetHierarchyRootPlayer();
			if (player)
				identity = player.GetIdentity();
		}
		else if (oldType != InventoryLocationType.GROUND)
		{
			GameInventory.SetGroundPosByOwner(newParent, this, dstLoc);
		}

		string dstLocDump = dstLoc.DumpToString();
		string newLocDump = newLoc.DumpToString();
		EXError.Warn(this, string.Format("Moving back to old location %1 because contents of new location %2 are in virtual storage", dstLocDump, newLocDump), {});

	#ifdef SERVER
		if (identity)
	#else
		if ((!player || !g_Game.IsMultiplayer()) && vector.DistanceSq(GetPosition(), g_Game.GetPlayer().GetPosition()) < UAMaxDistances.LARGE * UAMaxDistances.LARGE)
	#endif
		{
			string displayName = newParent.ConfigGetStringRaw("displayName");
			g_Game.FormatRawConfigStringKeys(displayName);
			ExpansionNotification("Virtual Storage", string.Format("%1 cannot receive items because its contents are currently in virtual storage. Restore the contents first before trying to put other items in.", displayName)).Error(identity);
		}

		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_MoveToLocation, dstLoc);
	}

	protected void Expansion_MoveToLocation(InventoryLocation dstLoc)
	{
		if (!dstLoc || !dstLoc.IsValid())
			return;

		auto srcLoc = new InventoryLocation();

		if (!GetInventory().GetCurrentInventoryLocation(srcLoc))
			return;

		EXError.Warn(this, "Expansion_MoveToLocation " + srcLoc.DumpToString() + " -> " + dstLoc.DumpToString(), {});

		GameInventory.LocationSyncMoveEntity(srcLoc, dstLoc);
	}

	override void EEDelete(EntityAI parent)
	{
		if (g_Game.IsServer() && g_Game.IsMultiplayer() && m_Expansion_GlobalID && m_Expansion_GlobalID.m_IsSet)
		{
			EntityAI savedEntityToBeDeleted = ExpansionEntityStorageModule.GetSavedEntityToBeDeleted();
			if (this != savedEntityToBeDeleted && !Expansion_IsContainedIn(savedEntityToBeDeleted))
			{
				string name = m_Expansion_GlobalID.IDToHex();
				if (ExpansionEntityStorageModule.DeleteFiles(name))
					EXPrint(this, "EEDelete - deleted " + name + " from entity storage");
			}
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

	override void Expansion_SetActions()
	{
		super.Expansion_SetActions();

		if (m_Expansion_DoesSupportVirtualStorage)
		{
			AddAction(ExpansionActionStoreContents);
			AddAction(ExpansionActionRestoreContents);
		}
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		if (Expansion_DoesSupportVirtualStorage() != m_Expansion_DoesSupportVirtualStorage)
		{
			//! 3rd party mods doing stupid shit...
			string tmp;
			DumpStackString(tmp);
			TStringArray lines = {};
			tmp.Split("\n", lines);

			TStringArray stack = {};

			for (int i = 1; i < lines.Count(); i++)
			{
				string line = lines[i];
				string lineLower = line;
				lineLower.ToLower();

				if (lineLower.IndexOf("dayzexpansion") == -1)
					stack.Insert(line);
			}

			EXError.Error(this, "overridden Expansion_DoesSupportVirtualStorage() return value must not change during runtime!", stack);
		}

		if (m_Expansion_DoesSupportVirtualStorage && g_Game.IsServer() && !m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();

		super.CF_OnStoreSave(storage);

		if (m_Expansion_DeferredDeleteEntityStorageFiles && !ExpansionEntityStorageModule.IsCurrentEntity(this))
		{
			array<EntityAI> contents = MiscGameplayFunctions.Expansion_GetItems(this);  //! Direct children
			ItemBase item;

			foreach (EntityAI entity: contents)
			{
				if (Class.CastTo(item, entity) && !item.m_Expansion_IsStoreSaved)
					return;  //! Return early if any contents not yet persisted
			}

			//! All contents have been persisted, we are safe to delete the entity storage file
			ExpansionEntityStorageModule.DeleteFiles(m_Expansion_GlobalID.IDToHex());

			m_Expansion_DeferredDeleteEntityStorageFiles = false;
		}
	}
#endif

	bool Expansion_HasEntityStorage()
	{
		if (g_Game.IsClient())
			return m_Expansion_HasEntityStorage;

		if (!m_Expansion_GlobalID || !m_Expansion_GlobalID.m_IsSet)
			return false;

		if (FileExist(Expansion_GetEntityStorageFileName()) && !m_Expansion_DeferredDeleteEntityStorageFiles)
			return true;

		return false;
	}

	//! Evaluated ONE TIME in constructor. Mods overriding this must NOT change return value during runtime.
	bool Expansion_DoesSupportVirtualStorage()
	{
		if (IsContainer() || IsItemTent() || (IsFireplace() && IsInherited(BarrelHoles_ColorBase)))
			return true;

		return false;
	}

	override bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		if (restoreOverride)
			return m_Expansion_HasEntityStorage;

		if (!m_Expansion_DoesSupportVirtualStorage)
			return false;

		if (g_Game.IsServer() && !m_Expansion_GlobalID)
			return false;

		auto settings = GetExpansionSettings().GetBaseBuilding(false);

		if (!settings.IsLoaded() || !settings.EnableVirtualStorage)
			return false;

		if (ExpansionStatic.IsAnyOf(this, settings.VirtualStorageExcludedContainers, true))
			return false;

		return true;
	}

	void Expansion_DeleteContentsExceptLockedSlots()
	{
		m_Expansion_DeletingContents = true;

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
					if (!GetInventory().GetSlotLock(il.GetSlot()) && !ExpansionStatic.IsLock(attachment.GetType()))
						g_Game.ObjectDelete(attachment);
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
					g_Game.ObjectDelete(item);
			}
		}

		m_Expansion_DeletingContents = false;
	}

	void Expansion_DeleteEntityStorageFilesIfEmpty()
	{
		if (!g_Game.IsClient() && !m_Expansion_DeletingContents && Expansion_HasEntityStorage() && Expansion_IsEmptyIgnoringLockedSlots())
		{
			EntityAI savedEntityToBeDeleted = ExpansionEntityStorageModule.GetSavedEntityToBeDeleted();
			if (!ExpansionEntityStorageModule.IsCurrentEntity(this) && !Expansion_IsContainedIn(savedEntityToBeDeleted))
				ExpansionEntityStorageModule.DeleteFiles(m_Expansion_GlobalID.IDToHex());
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
					if (!GetInventory().GetSlotLock(il.GetSlot()) && !ExpansionStatic.IsLock(attachment.GetType()))
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
		if (!m_Initialized)  //! Don't allow to store if not initialized
			return false;

		if (m_Expansion_HasEntityStorage)
		{
			if (!Expansion_IsEmptyIgnoringLockedSlots())
				EXError.Warn(this, GetType() + " " + GetPosition() + " already has entity storage");

			return false;
		}

		if (Expansion_IsEmptyIgnoringLockedSlots())
		{
			if (Expansion_HasEntityStorage())
			{
				EXError.Warn(this, GetType() + " " + GetPosition() + " is empty (ignoring locked slots) - deleting entity storage files");

				ExpansionEntityStorageModule.DeleteFiles(m_Expansion_GlobalID.IDToHex());
			}

			return false;
		}

		if (!m_Expansion_GlobalID.m_IsSet)
			m_Expansion_GlobalID.Acquire();

		if (ExpansionEntityStorageModule.SaveToFile(this, Expansion_GetEntityStorageFileName(), true))
		{
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_StoreContents - " + GetPosition() + " - saved inventory with storage ID " + m_Expansion_GlobalID.IDToHex());

			Expansion_DeleteContentsExceptLockedSlots();

			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_StoreContents - " + GetPosition() + " - is inventory empty (ignoring locked slots)? " + Expansion_IsEmptyIgnoringLockedSlots());

			m_Expansion_HasEntityStorage = true;
			m_Expansion_DeferredDeleteEntityStorageFiles = false;
			SetSynchDirty();

			return true;
		}
		else
		{
			EXError.Error(this, GetType() + " " + GetPosition() + " - could not save inventory");
		}

		return false;
	}

	override bool Expansion_RestoreContents(PlayerBase player = null)
	{
		string errorMessage;
		return Expansion_RestoreContentsEx(player, errorMessage);
	}

	bool Expansion_RestoreContentsEx(PlayerBase player = null, out string errorMessage = "")
	{
		if (m_Expansion_HasEntityStorage && m_Expansion_GlobalID.m_IsSet && Expansion_IsEmptyIgnoringLockedSlots())
		{
			if (!IsOpen())
			{
				string displayName = ConfigGetStringRaw("displayName");
				g_Game.FormatRawConfigStringKeys(displayName);
				errorMessage = displayName + " is not open.";
				EXError.Warn(this, GetType() + " " + GetPosition() + " - could not restore inventory with storage ID " + m_Expansion_GlobalID.IDToHex() + ": " + errorMessage);
				return false;
			}

			//! Set to true so we can receive stored atts and cargo
			m_Expansion_RestoringContents = true;

			//! If container is ruined, set health above zero so we can restore contents
			float health = GetHealth();
			if (!health)
				SetHealth01("", "", 1.0);

			EntityAI entity = this;
			//! When storage contents are loaded into the container, keep files
			//! to avoid data loss in case of server crash before the container was persisted.
			//! After container was persisted, storage files are deleted in CF_OnStoreSave.
			bool success = ExpansionEntityStorageModule.RestoreFromFile(Expansion_GetEntityStorageFileName(), entity, null, null, false);
			if (success)
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::Expansion_RestoreContents - " + GetPosition() + " - restored inventory with storage ID " + m_Expansion_GlobalID.IDToHex());

				m_Expansion_HasEntityStorage = false;
				m_Expansion_DeferredDeleteEntityStorageFiles = true;
				SetSynchDirty();
			}
			else
			{
				if (!FileExist(Expansion_GetEntityStorageFileName()))
					errorMessage = "Storage file with ID " + m_Expansion_GlobalID.IDToHex() + " does not exist.";
				else
					errorMessage = "Internal error.";

				EXError.Error(this, GetType() + " " + GetPosition() + " - could not restore inventory with storage ID " + m_Expansion_GlobalID.IDToHex() + ": " + errorMessage);
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

	void Expansion_StoreOrRestoreParentContents(bool store)
	{
		auto il = new InventoryLocation();
		GetInventory().GetCurrentInventoryLocation(il);
		if (il.GetType() == InventoryLocationType.ATTACHMENT)
		{
			ItemBase parent;
			if (Class.CastTo(parent, il.GetParent()) && parent.Expansion_CanUseVirtualStorage(!store))
			{
				if (store)
				{
					if (!m_Expansion_HasEntityStorage)
						parent.Expansion_StoreContents();
				}
				else
				{
					if (IsOpen())
						parent.Expansion_RestoreContents();
				}
			}
		}
	}
};
