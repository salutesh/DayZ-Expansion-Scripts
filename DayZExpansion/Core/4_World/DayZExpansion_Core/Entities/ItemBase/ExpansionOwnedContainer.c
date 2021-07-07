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
	protected PlayerIdentity m_ExpansionContainerOwnerIdentity;

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
		m_ExpansionContainerOwnerIdentity = identity;
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

		if (modName != "DZ_Expansion_Market")
			return;

		storage.Write(m_ExpansionContainerUID);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorage storage, string modName)
	{
		if (!super.CF_OnStoreLoad( storage, modName))
			return false;

		if (modName != "DZ_Expansion_Market")
			return true;

		if (Expansion_Assert_False(storage.Read(m_ExpansionContainerUID), "[" + this + "] Failed reading m_ExpansionContainerUID"))
			return false;

		return true;
	}
	#endif
}
