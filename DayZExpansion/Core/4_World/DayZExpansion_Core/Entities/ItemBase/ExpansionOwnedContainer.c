/**
 * ExpansionOwnedContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Cargo container that's only accessible by the designated owner
class ExpansionOwnedContainer: Container_Base
{
	protected string m_ExpansionContainerUID;

	void ExpansionOwnedContainer()
	{
		if (GetGame().IsClient())  //! Only client, not server or COM
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionRequestContainerUID, 250);
	}

	override bool CanDisplayAttachmentSlot(string slot_name)
	{
		if (!super.CanDisplayAttachmentSlot(slot_name))
			return false;

		return ExpansionIsContainerOwner();
	}

	override bool CanDisplayCargo()
	{
		return ExpansionIsContainerOwner();
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return ExpansionIsContainerOwner(Man.Cast(parent));
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
		m_ExpansionContainerUID = uid;
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
		return m_ExpansionContainerUID == uid;
	}

	void ExpansionRequestContainerUID()
	{
		ScriptRPC rpc = new ScriptRPC;
		rpc.Send(this, ExpansionRPC.SyncOwnedContainerUID, true, NULL);
	}

	void ExpansionSendContainerUID()
	{
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write(m_ExpansionContainerUID);
		rpc.Send(this, ExpansionRPC.SyncOwnedContainerUID, true, NULL);
	}

	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);

		if (rpc_type == ExpansionRPC.SyncOwnedContainerUID)
		{
			if (IsMissionHost())
			{
				ExpansionSendContainerUID();
			}
			else
			{
				if (!ctx.Read(m_ExpansionContainerUID))
				{
					Error(ToString() + "::OnRPC ExpansionRPC.SyncOwnedContainerUID couldn't read m_ExpansionContainerUID");
				}
			}
		}
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef CF_MODULE_MODSTORAGE
		if (GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET)
		{
			super.OnStoreSave(ctx);
			return;
		}
		#endif

		super.OnStoreSave(ctx);
				
		ctx.Write(m_ExpansionContainerUID);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (Expansion_Assert_False(super.OnStoreLoad(ctx, version), "[" + this + "] Failed reading OnStoreLoad super"))
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if (version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET)
			return true;
		#endif
		
		if (Expansion_Assert_False(ctx.Read(m_ExpansionContainerUID), "[" + this + "] Failed reading m_ExpansionContainerUID"))
			return false;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorage storage, string modName)
	{
		super.CF_OnStoreSave(storage, modName);

		if (modName != "DZ_Expansion_Core")
			return;

		storage.Write(m_ExpansionContainerUID);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorage storage, string modName)
	{
		if (!super.CF_OnStoreLoad( storage, modName))
			return false;

		if (modName != "DZ_Expansion_Core")
			return true;

		if (Expansion_Assert_False(storage.Read(m_ExpansionContainerUID), "[" + this + "] Failed reading m_ExpansionContainerUID"))
			return false;

		return true;
	}
	#endif
}

class ExpansionTemporaryOwnedContainer: ExpansionOwnedContainer
{
	protected bool m_ExpansionCanReceiveItems;

	void ExpansionTemporaryOwnedContainer()
	{
		if (IsMissionHost())  //! Server or COM
		{
			SetAllowDamage(false);

			//! Check if empty every 5 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionCheckStorage, 5000, true );

			//! Delete after 20 minutes
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionDeleteStorage, 1000 * 60 * 20, false);
		}

		//! Warn about pending deletion after 15 minutes
		if (IsMissionClient())  //! Client or COM
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionStorageNotification, 1000 * 60 * 15, false, "STR_EXPANSION_MARKET_TEMPORARY_STORAGE_EXPIRATION_WARNING");
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		return m_ExpansionCanReceiveItems;
	}
	
	override bool CanLoadAttachment(EntityAI attachment)
	{
		return true;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return m_ExpansionCanReceiveItems;
	}
	
	override bool CanLoadItemIntoCargo(EntityAI item)
	{
		return true;
	}

	void ExpansionSetCanReceiveItems(bool state)
	{
		m_ExpansionCanReceiveItems = state;
	}

	void ExpansionStorageNotification(string msg)
	{
		if (IsEmpty())
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(m_ExpansionContainerUID);
		if (!player || !player.GetIdentity())
			return;

		StringLocaliser title = new StringLocaliser("STR_EXPANSION_MARKET_TITLE");
		StringLocaliser text = new StringLocaliser(msg);

		ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_TRADER, COLOR_EXPANSION_NOTIFICATION_ORANGE).Create(player.GetIdentity());
	}

	void ExpansionCheckStorage()
	{
		if (IsEmpty())
			ExpansionDeleteStorage();
	}

	void ExpansionDeleteStorage()
	{
		ExpansionStorageNotification("STR_EXPANSION_MARKET_TEMPORARY_STORAGE_EXPIRED");

		GetGame().ObjectDelete(this);
	}
}
