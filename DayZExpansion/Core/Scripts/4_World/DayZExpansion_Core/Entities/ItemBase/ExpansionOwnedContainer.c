/**
 * ExpansionOwnedContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Cargo container that's only accessible by the designated owner
class ExpansionOwnedContainer: Container_Base
{
	protected ref ExpansionNetsyncData m_Expansion_NetsyncData;

	void ExpansionOwnedContainer()
	{
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);

		if (m_Expansion_NetsyncData.SI_Receive)
			m_Expansion_NetsyncData.SI_Receive.Insert(Expansion_OnOwnerSync);
	}

	void ~ExpansionOwnedContainer()
	{
		if (m_Expansion_NetsyncData && m_Expansion_NetsyncData.SI_Receive)
			m_Expansion_NetsyncData.SI_Receive.Remove(Expansion_OnOwnerSync);
	}

	void Expansion_OnOwnerSync()
	{
#ifdef DIAG
		string uid;
		m_Expansion_NetsyncData.Get(0, uid);
		EXTrace.Print(EXTrace.ENABLE, this, "ExpansionOwnedContainer::Expansion_OnOwnerSync - owner " + uid);
#endif
	}

	override bool CanCombineAttachment(notnull EntityAI e, int slot, bool stack_max_limit = false)
	{
		if (!super.CanCombineAttachment(e, slot, stack_max_limit))
			return false;

		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		return ExpansionIsContainerOwner();
	}

	//! NOTE: Despite being a vanilla method, nothing in vanilla checks CanDisplayCargo.
	//! We implement this ourself in 5_Mission/GUI/InventoryNew/Inherited/ContainerWithCargoAndAttachments.
	override bool CanDisplayCargo()
	{
		return ExpansionIsContainerOwner();
	}
	
	override bool CanLoadAttachment(EntityAI attachment)
	{
		return true;
	}
	
	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return true;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return ExpansionIsContainerOwner(Man.Cast(parent));
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!super.CanSwapItemInCargo(child_entity, new_entity))
			return false;

		return GetGame().IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	//! This hides the container from vicinity completely
	override bool IsInventoryVisible()
	{
		return ExpansionIsContainerOwner();
	}

	override bool IsHealthVisible()
	{
		return false;
	}

	override bool NameOverride(out string output)
	{
		if (!GetGame().GetPlayer())
			return false;

		PlayerIdentity identity = GetGame().GetPlayer().GetIdentity();

		if (!ExpansionIsContainerOwner(identity))
			return false;

		GetGame().ObjectGetDisplayName(this, output);
		output += " - " + identity.GetName();

		return true;
	}

	void ExpansionSetContainerOwner(Man player)
	{
		ExpansionSetContainerOwner(player.GetIdentity());
	}

	void ExpansionSetContainerOwner(PlayerIdentity identity)
	{
		if (identity)
			ExpansionSetContainerOwner(identity.GetId());
	}

	protected void ExpansionSetContainerOwner(string uid)
	{
		m_Expansion_NetsyncData.Set(0, uid);
	}

	//! Callable client only
	bool ExpansionIsContainerOwner()
	{
		return ExpansionIsContainerOwner(GetGame().GetPlayer());
	}

	//! Callable client/server
	bool ExpansionIsContainerOwner(Man player)
	{
		if (!player)
			return false;

		return ExpansionIsContainerOwner(player.GetIdentity());
	}

	//! Callable client/server
	bool ExpansionIsContainerOwner(PlayerIdentity identity)
	{
		if (!identity)
			return false;

		return ExpansionIsContainerOwner(identity.GetId());
	}

	//! Callable client/server
	bool ExpansionIsContainerOwner(string uid)
	{
		if (!m_Expansion_NetsyncData)
			return false;

		string owner;
		m_Expansion_NetsyncData.Get(0, owner);
		return owner != string.Empty && owner == uid;
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		m_Expansion_NetsyncData.OnRPC(sender, rpc_type, ctx);
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		if (!Expansion_PersistOwner())
			return;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		string uid;
		m_Expansion_NetsyncData.Get(0, uid);
		ctx.Write(uid);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		if (!Expansion_PersistOwner())
			return true;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		string uid;
		if (!ctx.Read(uid))
			return false;

		ExpansionSetContainerOwner(uid);

		EXTrace.Print(EXTrace.ENABLE, this, "::CF_OnStoreLoad - owner " + uid);

		return true;
	}

	bool Expansion_PersistOwner()
	{
		return true;
	}
	#endif
}

class ExpansionTemporaryOwnedContainer: ExpansionOwnedContainer
{
	protected bool m_ExpansionCanReceiveItems;

	void ExpansionTemporaryOwnedContainer()
	{
		if (GetGame().IsServer())  //! Server or SP
		{
			SetAllowDamage(false);

			//! Delete after 20 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}

		if (!GetGame().IsDedicatedServer())  //! Client or SP
		{
			//! Warn about pending deletion after 15 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionStorageNotification, 1000 * 60 * 15, false, "STR_EXPANSION_TEMPORARY_STORAGE_EXPIRATION_WARNING");

			ClearFlags(EntityFlags.VISIBLE, false);
		}
	}

	override void Expansion_OnOwnerSync()
	{
		super.Expansion_OnOwnerSync();

		//! @note GetGame().GetPlayer() will only be non-NULL after player has fully loaded in
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionCheckContainerOwner, 1000, true);
	}

	void ExpansionCheckContainerOwner()
	{
		if (!GetGame().GetPlayer())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(ExpansionCheckContainerOwner);

		EXTrace.Print(EXTrace.ENABLE, this, "ExpansionTemporaryOwnedContainer::Expansion_OnOwnerSync - is owner? " + ExpansionIsContainerOwner());

		if (ExpansionIsContainerOwner())
			SetFlags(EntityFlags.VISIBLE, false);
	}

	// default to visible unless on ground, owner will always see it once netsync received
	// allow other players to see the container while it's being carried (no more mimes!)
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		EXTrace.Print(EXTrace.ENABLE, this, "EEItemLocationChanged " + typename.EnumToString(InventoryLocationType, oldLoc.GetType()) + " -> " + typename.EnumToString(InventoryLocationType, newLoc.GetType()));

		super.EEItemLocationChanged(oldLoc, newLoc);

		if (GetGame().IsDedicatedServer())
			return;

		//! @note call to ClearFlags needs to be delayed one frame, else other clients won't see other items taken to hand afterwards
		//! Can't use ScriptCallQueue::Call to call ClearFlags directly, CTDs other clients
		if (!ExpansionIsContainerOwner() && newLoc.GetType() == InventoryLocationType.GROUND)
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_SetInvisible);
		else
			SetFlags(EntityFlags.VISIBLE, false);
	}

	void Expansion_SetInvisible()
	{
		ClearFlags(EntityFlags.VISIBLE, false);
	}

	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	override bool CanCombineAttachment(notnull EntityAI e, int slot, bool stack_max_limit = false)
	{
		return false;
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return m_ExpansionCanReceiveItems;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return m_ExpansionCanReceiveItems;
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		return false;
	}

	void ExpansionSetCanReceiveItems(bool state)
	{
		m_ExpansionCanReceiveItems = state;
	}

	void ExpansionStorageNotification(string msg)
	{
		if (IsEmpty())
			return;

		string uid;
		m_Expansion_NetsyncData.Get(0, uid);
		PlayerBase player = PlayerBase.GetPlayerByUID(uid);
		if (!player || !player.GetIdentity())
			return;

		StringLocaliser title = new StringLocaliser("STR_EXPANSION_TEMPORARY_STORAGE");
		StringLocaliser text = new StringLocaliser(msg);

		ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_TRADER, COLOR_EXPANSION_NOTIFICATION_ORANGE).Create(player.GetIdentity());
	}

	//! TODO: Remove ExpansionCheckStorage once Quests got updated
	void ExpansionCheckStorage()
	{
		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	void ExpansionDeleteStorage()
	{
		ExpansionStorageNotification("STR_EXPANSION_TEMPORARY_STORAGE_EXPIRED");

		Delete();
	}

	override bool Expansion_CanUseVirtualStorage(bool restoreOverride = false)
	{
		return false;
	}
}
