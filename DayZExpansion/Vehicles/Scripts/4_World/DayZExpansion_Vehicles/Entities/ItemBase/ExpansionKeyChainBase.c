class ExpansionKeyChainBase : ItemBase
{
	static ref TStringArray s_Expansion_KeychainColors = {"Red", "White", "Black", "Blue", "Green", "Grey", "Orange", "Pink", "Purple", "Yellow"};

	protected string m_Expansion_OwnerUID;
	protected string m_Expansion_OwnerName;

	void ExpansionKeyChainBase()
	{
		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);

	#ifdef SERVER
		PlayerBase.s_Expansion_SI_OnPlayerConnected.Insert(Expansion_OnPlayerConnected);
	#else
		if (GetGame().IsClient())
			m_Expansion_NetsyncData.SI_Receive.Insert(Expansion_UpdateOwnerName);
	#endif
	}

	void Expansion_UpdateOwnerName()
	{
		m_Expansion_NetsyncData.Get(0, m_Expansion_OwnerName);
	}

	void Expansion_OnPlayerConnected(PlayerBase player, PlayerIdentity identity)
	{
		if (Expansion_HasOwner() && identity.GetId() == m_Expansion_OwnerUID && identity.GetName() != m_Expansion_OwnerName)
		{
			m_Expansion_OwnerName = identity.GetName();
			m_Expansion_NetsyncData.Set(0, m_Expansion_OwnerName);

			//! Only need to send updated data if already sent, else client will request it
			if (m_Expansion_NetsyncData.m_WasDataSent)
				m_Expansion_NetsyncData.Send(null);
		}
	}

	override bool DescriptionOverride(out string output)
	{
		if (m_Expansion_OwnerName)
		{
			output = string.Format(ConfigGetString("descriptionShort") + "\n\n#STR_EXPANSION_KEYS: %1\n%2\n", m_Expansion_OwnerName, Expansion_GetPersistentIDString());
			return true;
		}

		return false;
	}

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

		ctx.Write(m_Expansion_OwnerUID);
		ctx.Write(m_Expansion_OwnerName);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (ctx.GetVersion() < 50)
			return true;

		if (!ctx.Read(m_Expansion_OwnerUID))
			return false;

		if (!ctx.Read(m_Expansion_OwnerName))
			return false;

		m_Expansion_NetsyncData.Set(0, m_Expansion_OwnerName);

		return true;
	}
#endif

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		if (GetHierarchyParent() && Expansion_HasOwner())
			Expansion_LockSlot(true);
	}

	protected string Expansion_GetPersistentIDString()
	{
		int a, b, c, d;
		ExpansionVehicle vehicle;
		ExpansionCarKey key;
		if (ExpansionVehicle.Get(vehicle, GetHierarchyParent()))
		{
			vehicle.GetMasterKeyPersistentID(a, b, c, d);
		}
		else if (Class.CastTo(key, GetHierarchyParent()))
		{
			key.GetMasterKeyPersistentID(a, b, c, d);
		}

		return ExpansionStatic.GetPersistentIDString(a, b, c, d);
	}

	bool Expansion_HasOwner()
	{
		return m_Expansion_OwnerUID != "";
	}

	string Expansion_GetOwnerUID()
	{
		return m_Expansion_OwnerUID;
	}

	string Expansion_GetOwnerName()
	{
		return m_Expansion_OwnerName;
	}

	void Expansion_AssignOwner(notnull Man player, bool send = true)
	{
		if (player.GetIdentity())
			Expansion_AssignOwner(player.GetIdentity(), send);
		else
			Expansion_AssignOwner("", player.GetDisplayName(), send);
	}

	void Expansion_AssignOwner(notnull PlayerIdentity owner, bool send = true)
	{
		Expansion_AssignOwner(owner.GetId(), owner.GetName(), send);
	}

	void Expansion_AssignOwner(string ownerUID, string ownerName, bool send = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
#endif 
		
		if (ownerUID == m_Expansion_OwnerUID && ownerName == m_Expansion_OwnerName)
			return;

		m_Expansion_OwnerUID = ownerUID;
		m_Expansion_OwnerName = ownerName;
		m_Expansion_NetsyncData.Set(0, m_Expansion_OwnerName);
		if (send)
			m_Expansion_NetsyncData.Send(null);
		Expansion_LockSlot(true);
		
#ifdef EXTRACE
		EXTrace.Add(trace, m_Expansion_OwnerUID);
		EXTrace.Add(trace, m_Expansion_OwnerName);
#endif 
	}

	void Expansion_ResetOwner()
	{
		if (!m_Expansion_OwnerUID && !m_Expansion_OwnerName)
			return;

		m_Expansion_OwnerUID = "";
		m_Expansion_OwnerName = "";
		m_Expansion_NetsyncData.Set(0, m_Expansion_OwnerName);
		m_Expansion_NetsyncData.Send(null);
		Expansion_LockSlot(false);
	}

	void Expansion_LockSlot(bool state)
	{
		InventoryLocation invLoc = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation(invLoc);

		if (invLoc.IsValid())
			GetHierarchyParent().GetInventory().SetSlotLock(invLoc.GetSlot(), state);
	}

	static string Expansion_GetRandomKeychain()
	{
		return "ExpansionKeyChain_" + s_Expansion_KeychainColors.GetRandomElement();
	}
}

class ExpansionKeyChain_Red: ExpansionKeyChainBase{};
class ExpansionKeyChain_White: ExpansionKeyChainBase{};
class ExpansionKeyChain_Black: ExpansionKeyChainBase{};
class ExpansionKeyChain_Blue: ExpansionKeyChainBase{};
class ExpansionKeyChain_Green: ExpansionKeyChainBase{};
class ExpansionKeyChain_Grey: ExpansionKeyChainBase{};
class ExpansionKeyChain_Orange: ExpansionKeyChainBase{};
class ExpansionKeyChain_Pink: ExpansionKeyChainBase{};
class ExpansionKeyChain_Purple: ExpansionKeyChainBase{};
class ExpansionKeyChain_Yellow: ExpansionKeyChainBase{};
