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
	protected string m_Expansion_OwnerUID;  //! Server only
	protected int m_Expansion_OwnerPlainID[2];  //! Server + client (temp)
	protected int m_Expansion_OwnerPlainID0;  //! Server + client (netsynced)
	protected int m_Expansion_OwnerPlainID1;  //! Server + client (netsynced)

	void ExpansionOwnedContainer()
	{
		RegisterNetSyncVariableInt("m_Expansion_OwnerPlainID0");
		RegisterNetSyncVariableInt("m_Expansion_OwnerPlainID1");
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		Expansion_UpdateOwner();
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		Expansion_UpdateOwner();
	}

	protected void Expansion_UpdateOwner()
	{
		if (m_Expansion_OwnerPlainID[0] != m_Expansion_OwnerPlainID0 || m_Expansion_OwnerPlainID[1] != m_Expansion_OwnerPlainID1)
		{
			m_Expansion_OwnerPlainID[0] = m_Expansion_OwnerPlainID0;
			m_Expansion_OwnerPlainID[1] = m_Expansion_OwnerPlainID1;

			string steamId = PlayerIdentity.Expansion_PlainIdToString(m_Expansion_OwnerPlainID);

			m_Expansion_OwnerUID = PlayerIdentity.Expansion_PlainIdToId(steamId);

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.MISC, this, "::Expansion_UpdateOwner - owner " + m_Expansion_OwnerUID);
			EXTrace.Print(EXTrace.MISC, this, "::Expansion_UpdateOwner - owner steam64 " + steamId);
		#endif

		#ifndef SERVER
			Expansion_OnOwnerSync();
		#endif
		}
	}

	void Expansion_OnOwnerSync()
	{
	}

	override bool CanCombineAttachment(notnull EntityAI e, int slot, bool stack_max_limit = false)
	{
		if (!super.CanCombineAttachment(e, slot, stack_max_limit))
			return false;

		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
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
		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		if (!super.CanReceiveItemIntoCargo(item))
			return false;

		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanReleaseCargo(EntityAI cargo)
	{
		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
	}

	override bool CanSwapItemInCargo(EntityAI child_entity, EntityAI new_entity)
	{
		if (!super.CanSwapItemInCargo(child_entity, new_entity))
			return false;

		return g_Game.IsDedicatedServer() || ExpansionIsContainerOwner();
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
		if (!g_Game.GetPlayer())
			return false;

		PlayerIdentity identity = g_Game.GetPlayer().GetIdentity();

		if (!ExpansionIsContainerOwner(identity))
			return false;

		g_Game.ObjectGetDisplayName(this, output);
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
		{
			m_Expansion_OwnerUID = identity.GetId();

			identity.Expansion_PlainIdToInt(m_Expansion_OwnerPlainID);

			m_Expansion_OwnerPlainID0 = m_Expansion_OwnerPlainID[0];
			m_Expansion_OwnerPlainID1 = m_Expansion_OwnerPlainID[1];

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.MISC, this, "::ExpansionSetContainerOwner - owner " + m_Expansion_OwnerUID);
			EXTrace.Print(EXTrace.MISC, this, "::ExpansionSetContainerOwner - owner steam64 " + identity.GetPlainId());
		#endif

		#ifdef SERVER
			SetSynchDirty();
		#endif
		}
	}

	//! DEPRECATED, use ExpansionSetContainerOwner(PlayerIdentity identity)
	void ExpansionSetContainerOwner(string uid)
	{
		m_Expansion_OwnerUID = uid;

		string steamId = PlayerBase.Expansion_GetPlainIDByUID(uid);
		int idLen = steamId.Length();

		m_Expansion_OwnerPlainID0 = steamId.Substring(0, 9).ToInt();
		m_Expansion_OwnerPlainID1 = steamId.Substring(9, idLen - 9).ToInt();

		m_Expansion_OwnerPlainID[0] = m_Expansion_OwnerPlainID0;
		m_Expansion_OwnerPlainID[1] = m_Expansion_OwnerPlainID1;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MISC, this, "::ExpansionSetContainerOwner - owner " + m_Expansion_OwnerUID);
		EXTrace.Print(EXTrace.MISC, this, "::ExpansionSetContainerOwner - owner steam64 " + steamId);
	#endif

	#ifdef SERVER
		SetSynchDirty();
	#endif
	}

	PlayerBase ExpansionGetContainerOwner()
	{
		return PlayerBase.GetPlayerByUID(ExpansionGetContainerOwnerUID());
	}

	string ExpansionGetContainerOwnerUID()
	{
		return m_Expansion_OwnerUID;
	}

	//! Callable client only
	bool ExpansionIsContainerOwner()
	{
		return ExpansionIsContainerOwner(g_Game.GetPlayer());
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
		string owner = ExpansionGetContainerOwnerUID();
		return owner != string.Empty && owner == uid;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		if (!Expansion_PersistOwner())
			return;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		ctx.Write(m_Expansion_OwnerPlainID0);
		ctx.Write(m_Expansion_OwnerPlainID1);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		if (!Expansion_PersistOwner())
			return true;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		if (ctx.GetVersion() < 50)
		{
			string uid;
			if (!ctx.Read(uid))
				return false;

			ExpansionSetContainerOwner(uid);
		}
		else
		{
			if (!ctx.Read(m_Expansion_OwnerPlainID0))
				return false;

			if (!ctx.Read(m_Expansion_OwnerPlainID1))
				return false;

			Expansion_UpdateOwner();
		}

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
		if (g_Game.IsServer())  //! Server or SP
		{
			SetAllowDamage(false);

			//! Delete after 20 minutes
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}

		if (!g_Game.IsDedicatedServer())  //! Client or SP
		{
			//! Warn about pending deletion after 15 minutes
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ExpansionStorageNotification, 1000 * 60 * 15, false, "STR_EXPANSION_TEMPORARY_STORAGE_EXPIRATION_WARNING");

			Expansion_SetInvisible(this);
		}
	}

	void ~ExpansionTemporaryOwnedContainer()
	{
		if (!g_Game)
			return;

		if (!g_Game.IsDedicatedServer())
		{
			ScriptCallQueue callQueue = g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY);
			callQueue.Remove(ExpansionCheckContainerOwner);
			callQueue.Remove(Expansion_SetInvisible);
		}
	}

	override void Expansion_OnOwnerSync()
	{
		super.Expansion_OnOwnerSync();

		//! @note g_Game.GetPlayer() will only be non-NULL after player has fully loaded in
		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(ExpansionCheckContainerOwner, 1000, true);
	}

	void ExpansionCheckContainerOwner()
	{
		if (!g_Game.GetPlayer())
			return;

		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(ExpansionCheckContainerOwner);

		EXTrace.Print(EXTrace.MISC, this, "ExpansionTemporaryOwnedContainer::Expansion_OnOwnerSync - is owner? " + ExpansionIsContainerOwner());

		if (ExpansionIsContainerOwner())
			Expansion_SetInvisible(this, false);
	}

	// default to visible unless on ground, owner will always see it once netsync received
	// allow other players to see the container while it's being carried (no more mimes!)
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		EXTrace.Print(EXTrace.MISC, this, "EEItemLocationChanged " + typename.EnumToString(InventoryLocationType, oldLoc.GetType()) + " -> " + typename.EnumToString(InventoryLocationType, newLoc.GetType()));

		super.EEItemLocationChanged(oldLoc, newLoc);

		if (g_Game.IsDedicatedServer())
			return;

		//! @note call to ClearFlags needs to be delayed one frame, else other clients won't see other items taken to hand afterwards
		//! Can't use ScriptCallQueue::Call to call ClearFlags directly, CTDs other clients
		if (!ExpansionIsContainerOwner() && newLoc.GetType() == InventoryLocationType.GROUND)
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(Expansion_SetInvisible, this, true);
		else
			g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(Expansion_SetInvisible, this, false);
	}

	void Expansion_SetInvisible(EntityAI item, bool invisible = true)
	{
		//! Client or SP
		if (!g_Game.IsDedicatedServer())
		{
			if (invisible)
				item.ClearFlags(EntityFlags.VISIBLE, false);
			else
				item.SetFlags(EntityFlags.VISIBLE, false);
		}
	}

	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		if (IsEmpty())
			ExpansionDeleteStorage();

		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(Expansion_SetInvisible, this, false);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (IsEmpty())
			ExpansionDeleteStorage();

		g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(Expansion_SetInvisible, this, false);
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

		PlayerBase player = PlayerBase.GetPlayerByUID(m_Expansion_OwnerUID);
		if (!player || !player.GetIdentity())
			return;

		StringLocaliser title = new StringLocaliser("STR_EXPANSION_TEMPORARY_STORAGE");
		StringLocaliser text = new StringLocaliser(msg);

		ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_TRADER, COLOR_EXPANSION_NOTIFICATION_ORANGE).Create(player.GetIdentity());
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
