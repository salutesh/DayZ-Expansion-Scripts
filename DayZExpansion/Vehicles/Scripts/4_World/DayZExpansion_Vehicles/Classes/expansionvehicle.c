modded class ExpansionVehicle
{
	protected float m_BoundingRadius;
	protected vector m_BoundingBox[2];

	protected float m_ModelZeroPointDistanceFromGround = -1;

	//! Vehicle locking & ownership
	protected ref map<string, ref ExpansionDoor> m_Doors = new map<string, ref ExpansionDoor>();
	protected bool m_CanBeLocked;
	protected ExpansionVehicleLockState m_LockState;
	protected float m_LockComplexity = 1.0;
	protected ExpansionKeyChainBase m_KeyChain;
	ref ExpansionNetsyncData m_NetsyncData;
	//! Following three only used if vehicle has no keychain slot
	protected bool m_HasOwner;
	protected string m_OwnerUID;
	protected string m_OwnerName;

	//! After pairing a key, it's the ID of the master key.
	//! This allows "changing locks" on vehicles so old paired keys no longer work
	protected int m_MasterKeyPersistentIDA;
	protected int m_MasterKeyPersistentIDB;
	protected int m_MasterKeyPersistentIDC;
	protected int m_MasterKeyPersistentIDD;

	protected float m_AutoCoverTimestamp;

	override void Init()
	{
		EntityAI vehicle = GetEntity();

		vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_MasterKeyPersistentIDA");
		vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_MasterKeyPersistentIDB");
		vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_MasterKeyPersistentIDC");
		vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_MasterKeyPersistentIDD");
		vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_LockState", 0, ExpansionVehicleLockState.COUNT);
		vehicle.RegisterNetSyncVariableFloat("m_ExpansionVehicle.m_LockComplexity", 0, 0, 2);

		vehicle.RegisterNetSyncVariableBool("m_ExpansionVehicle.m_HasLifetime");

		int i;
		int count;
		string path;
		ExpansionDoor door;

		if (vehicle.ConfigIsExisting("ExpansionAttachments"))
		{
			path = "CfgVehicles " + vehicle.GetType() + " ExpansionAttachments";
			count = GetGame().ConfigGetChildrenCount(path);

			for (i = 0; i < count; i++)
			{
				string attachmentName;
				GetGame().ConfigGetChildName(path, i, attachmentName);

				string attachmentPath = path + " " + attachmentName;
				door = new ExpansionDoor(this, attachmentName, attachmentPath);

				if (door.m_IsDoor)
					m_CanBeLocked = true;

				string slotName = door.m_InventorySlot;
				slotName.ToLower();

				m_Doors.Insert(slotName, door);
			}
		}

		if (vehicle.ConfigIsExisting("doors"))
		{
			TStringArray doors = {};
			vehicle.ConfigGetTextArray("doors", doors);

			foreach (string doorName: doors)
			{
				if (!m_Doors.Contains(doorName))
				{
					door = new ExpansionDoor(this, doorName, string.Empty);

					doorName.ToLower();
					m_Doors.Insert(doorName, door);

					m_CanBeLocked = true;
				}
			}
		}
		else
		{
			//! TODO: Determine doors from attachment slot names
			//TStringArray attachments = {};
			//vehicle.ConfigGetTextArray("attachments", attachments);
		}

		if (GetGame().IsServer())
		{
			auto settings = GetExpansionSettings().GetVehicle();

			foreach (ExpansionVehiclesConfig vehcfg: settings.VehiclesConfig)
			{
				if (vehicle.IsKindOf(vehcfg.ClassName))
				{
					m_LockComplexity = vehcfg.LockComplexity;
					break;
				}
			}
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		if (GetGame().IsServer())
		{
			EntityAI entity = GetEntity();

			if (entity.GetLifetime() > 0)
			{
				m_HasLifetime = true;
				entity.SetSynchDirty();
			}

			m_BoundingRadius = entity.ClippingInfo(m_BoundingBox);

			m_AutoCoverTimestamp = GetGame().GetTickTime();
		}
	}

	bool HasLifetime()
	{
		return m_HasLifetime;
	}

	ExpansionBoatScript GetExpansionBoat()
	{
		return ExpansionBoatScript.Cast(GetEntity());
	}

	ExpansionHelicopterScript GetExpansionHelicopter()
	{
		return ExpansionHelicopterScript.Cast(GetEntity());
	}

	ExpansionVehicleBase GetExpansionVehicleBase()
	{
		return ExpansionVehicleBase.Cast(GetEntity());
	}

	ExpansionVehicleCarBase GetExpansionVehicleCar()
	{
		return ExpansionVehicleCarBase.Cast(GetEntity());
	}

	ExpansionVehicleBoatBase GetExpansionVehicleBoat()
	{
		return ExpansionVehicleBoatBase.Cast(GetEntity());
	}

	ExpansionVehicleBikeBase GetExpansionVehicleBike()
	{
		return ExpansionVehicleBikeBase.Cast(GetEntity());
	}

	ExpansionVehicleHelicopterBase GetExpansionVehicleHelicopter()
	{
		return ExpansionVehicleHelicopterBase.Cast(GetEntity());
	}

	ExpansionVehiclePlaneBase GetExpansionVehiclePlane()
	{
		return ExpansionVehiclePlaneBase.Cast(GetEntity());
	}

	override bool CanBeDamaged()
	{
		if (GetExpansionSettings().GetVehicle().DisableVehicleDamage)
			return false;

		return super.CanBeDamaged();
	}

	void AutoCover()
	{
		auto settings = GetExpansionSettings().GetVehicle(false);

		if (!settings.EnableVehicleCovers || settings.VehicleAutoCoverTimeSeconds <= 0)
			return;

		EntityAI vehicle = GetEntity();

		//! Prevent autocover if this is a CE spawned vehicle (lifetime will be 0 in that case) and autocovering spawned vehicles is disabled
		if (vehicle.GetLifetime() <= 0 && !settings.EnableAutoCoveringDEVehicles)
			return;

		//! Defer autocover if engine is on or player is within vehicle bounding radius
		float playerAvoidanceRadius = m_BoundingRadius * 1.5;
		if (playerAvoidanceRadius <= 0)
			playerAvoidanceRadius = 150;
		if (EngineIsOn() || !GetCEApi().AvoidPlayer(GetPosition(), playerAvoidanceRadius))
		{
			m_AutoCoverTimestamp = GetGame().GetTickTime();
			return;
		}

		if (GetGame().GetTickTime() - m_AutoCoverTimestamp > settings.VehicleAutoCoverTimeSeconds)
		{
			EntityAI cover = vehicle.FindAttachmentBySlotName("CamoNet");
			if (settings.VehicleAutoCoverRequireCamonet && !cover)
			{
				m_AutoCoverTimestamp = GetGame().GetTickTime();
				return;
			}

			if (!CanCover())
			{
				m_AutoCoverTimestamp = GetGame().GetTickTime();
				return;
			}

			Cover(cover);
		}
	}

	bool CanCover()
	{
		EntityAI vehicle = GetEntity();

		if (vehicle.IsDamageDestroyed())
			return false;

		auto settings = GetExpansionSettings().GetVehicle(false);

		if (!settings.IsLoaded())
			return false;

		if (!settings.EnableVehicleCovers)
			return false;

		if (!m_HasLifetime && !settings.AllowCoveringDEVehicles)
			return false;

		if (GetCrew(false, true).Count() > 0)
			return false;

		if (!settings.CanCoverWithCargo)
		{
			if (MiscGameplayFunctions.Expansion_HasAnyCargo(vehicle))
				return false;
		}

		return true;
	}

	bool Cover(EntityAI cover = null, out ExpansionEntityStoragePlaceholder placeholder = null)
	{
		string coverType;

		if (cover)
			coverType = cover.GetType();
		else
			coverType = "CamoNet";

		string placeholderType = GetPlaceholderType(coverType);

		auto settings = GetExpansionSettings().GetVehicle();
		bool storeCargo = settings.UseVirtualStorageForCoverCargo;
		array<EntityAI> transferAttachments;

		EntityAI vehicle = GetEntity();

		ExpansionKeyChainBase keychain;
		if (settings.ShowVehicleOwners)
		{
			keychain = ExpansionKeyChainBase.Cast(vehicle.GetAttachmentByType(ExpansionKeyChainBase));
			if (keychain)
			{
				transferAttachments = {};
				transferAttachments.Insert(keychain);
			}
		}

		if (ExpansionEntityStoragePlaceholder.Expansion_StoreEntityAndReplace(vehicle, placeholderType, GetPosition(), ECE_OBJECT_SWAP, placeholder, storeCargo, transferAttachments))
		{
			EXTrace.Print(EXTrace.VEHICLES, this, "Covered vehicle " + GetType() + " " + GetPosition() + " with " + coverType);

			//! If the cover was on the vehicle itself, it will be pending deletion and must not be moved to placeholder
			if (cover && !cover.IsSetForDeletion())
			{
				Man player = cover.GetHierarchyRootPlayer();
				if (player)
				{
					bool result = player.ServerTakeEntityToTargetAttachmentEx(placeholder, cover, InventorySlots.GetSlotIdFromString("CamoNet"));
					EXTrace.Print(EXTrace.VEHICLES, this, "Moved " + cover + " to " + placeholder + "? " + result);
				}
			}

			if (settings.ShowVehicleOwners && !keychain && HasOwner())
			{
				int slotId = InventorySlots.GetSlotIdFromString("KeyChain");
				string color = ExpansionKeyChainBase.Expansion_GetRandomKeychain();
				keychain = ExpansionKeyChainBase.Cast(placeholder.GetInventory().CreateAttachmentEx(color, slotId));
				if (keychain)
					keychain.Expansion_AssignOwner(GetOwnerUID(), GetOwnerName());
			}

			return true;
		}

		return false;
	}

	void SetMasterKeyPersistentID(int b1, int b2, int b3, int b4)
	{
		m_MasterKeyPersistentIDA = b1;
		m_MasterKeyPersistentIDB = b2;
		m_MasterKeyPersistentIDC = b3;
		m_MasterKeyPersistentIDD = b4;

		GetEntity().SetSynchDirty();
	}

	void GetMasterKeyPersistentID(out int b1, out int b2, out int b3, out int b4)
	{
		b1 = m_MasterKeyPersistentIDA;
		b2 = m_MasterKeyPersistentIDB;
		b3 = m_MasterKeyPersistentIDC;
		b4 = m_MasterKeyPersistentIDD;
	}

	string GetMasterKeyPersistentIDString()
	{
		string id;
		id += ExpansionStatic.IntToHex(m_MasterKeyPersistentIDA);
		id += ExpansionStatic.IntToHex(m_MasterKeyPersistentIDB);
		id += ExpansionStatic.IntToHex(m_MasterKeyPersistentIDC);
		id += ExpansionStatic.IntToHex(m_MasterKeyPersistentIDD);
		return id;
	}

	float GetLockComplexity()
	{
		return m_LockComplexity;
	}

	override void SetLockState(ExpansionVehicleLockState lockState)
	{
		m_LockState = lockState;

		GetEntity().SetSynchDirty();
	}

	override ExpansionVehicleLockState GetLockState()
	{
		return m_LockState;
	}

	bool HasKey()
	{
		return m_LockState != ExpansionVehicleLockState.NOLOCK;
	}

	void PairKey(notnull ExpansionCarKey key)
	{
		EntityAI vehicle = GetEntity();

		key.PairToVehicle(vehicle);

		SetLockState(ExpansionVehicleLockState.UNLOCKED);

		KeyMessage("PairKey (" + vehicle + ", " + key + ")");
	}

	override void ResetKeyPairing()
	{
		if (GetGame().IsServer())
		{
			m_MasterKeyPersistentIDA = 0;
			m_MasterKeyPersistentIDB = 0;
			m_MasterKeyPersistentIDC = 0;
			m_MasterKeyPersistentIDD = 0;

			SetLockState(ExpansionVehicleLockState.NOLOCK);

			auto keychain = ExpansionKeyChainBase.Cast(GetEntity().GetAttachmentByType(ExpansionKeyChainBase));
			if (keychain)
				keychain.Expansion_ResetOwner();
			else
				ResetOwner();
		}
	}

	bool IsPairedTo(ExpansionCarKey key)
	{
		if (!HasKey())
		{
			KeyMessage("IsPairedTo: vehicle doesn't have paired key!");
			return false;
		}

		if (!key.IsPairedTo(this))
		{
			KeyMessage("IsPairedTo: key is not paired to vehicle!");
			return false;
		}

		KeyMessage("IsPairedTo: key is paired to vehicle!");

		return true;
	}

	void Lock(ExpansionCarKey key)
	{
		KeyMessage("LockCar");
		KeyMessage("key=" + key);
		if (key && !IsPairedTo(key) && !key.IsInherited(ExpansionCarAdminKey))
			return;

		SetLockState(ExpansionVehicleLockState.READY_TO_LOCK);
		KeyMessage("LockCar::READY_TO_LOCK");
	}

	void Unlock(ExpansionCarKey key)
	{
		if (key && !IsPairedTo(key) && !key.IsInherited(ExpansionCarAdminKey))
			return;

		SetLockState(ExpansionVehicleLockState.UNLOCKED);
		KeyMessage("Unlock::UNLOCKED");

		OnUnlocked();
	}

	void ForceUnlock(ExpansionVehicleLockState lockState = ExpansionVehicleLockState.FORCEDUNLOCKED)
	{
		SetLockState(lockState);

		OnUnlocked();
	}

	override bool IsLocked()
	{
		switch (m_LockState)
		{
			case ExpansionVehicleLockState.LOCKED:
			case ExpansionVehicleLockState.FORCEDLOCKED:
				return true;
		}

		return false;
	}

	bool IsReadyToLock()
	{
		switch (m_LockState)
		{
			case ExpansionVehicleLockState.READY_TO_LOCK:
			case ExpansionVehicleLockState.READY_TO_FORCELOCK:
				return true;
		}

		return false;
	}

	bool IsInventoryVisible()
	{
		//! @note never allow inventory access while forced locked
		if (m_LockState == ExpansionVehicleLockState.FORCEDLOCKED)
			return false;

		auto settings = GetExpansionSettings().GetVehicle(false);
		if (settings.IsLoaded() && settings.VehicleLockedAllowInventoryAccess)
			return true;

		if (settings.IsLoaded() && settings.VehicleLockedAllowInventoryAccessWithoutDoors && !AllDoorsClosed())
			return true;

		//! @note we explicitly check for LOCKED state instead of IsLocked() as we don't want to be able to access inventory if forced locked
		if (m_LockState == ExpansionVehicleLockState.LOCKED)
			return false;

		return true;
	}

	bool AllDoorsClosed(bool includeHoodAndTrunk = true)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	int DoorCount()
	{
		return m_Doors.Count();
	}

	bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!GetEntity().IsInitialized())
			return true;

		if (attachment.IsInherited(CarWheel))
			return true;

		if (IsLocked())
			return false;

		return true;
	}

	bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!GetEntity().IsInitialized())
			return true;

		if (IsLocked())
			return false;

		ExpansionKeyChainBase keychain;
		if (Class.CastTo(keychain, attachment) && keychain.Expansion_HasOwner())
			return false;

		return true;
	}

	bool CanUpdateLock(float dt)
	{
		if (!GetGame().IsServer())
			return false;

		if (IsReadyToLock())
			return true;

		return false;
	}

	void UpdateLock(float dt)
	{
		ExpansionVehicleLockState lockState;

		if (m_LockState == ExpansionVehicleLockState.READY_TO_FORCELOCK)
			lockState = ExpansionVehicleLockState.FORCEDLOCKED;
		else if (AllDoorsClosed() || !GetExpansionSettings().GetVehicle().VehicleRequireAllDoors)
			lockState = ExpansionVehicleLockState.LOCKED;

		if (lockState)
		{
			SetLockState(lockState);

			KeyMessage("UpdateLock::" + typename.EnumToString(ExpansionVehicleLockState, m_LockState));

			OnLocked();
		}
	}

	override void OnCECreate()
	{
		super.OnCECreate();

		EntityAI vehicle = GetEntity();

		array<EntityAI> items = {};
		items.Reserve(vehicle.GetInventory().CountInventory());

		vehicle.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		foreach (EntityAI item: items)
		{
			ExpansionCarKey key;
			if (Class.CastTo(key, item))
			{
				PairKey(key);
				break;
			}
		}
	}
	
	void OnHierarchyAfterLoad()
	{
		if (GetExpansionSettings().GetVehicle().ShowVehicleOwners)
		{
			EntityAI vehicle = GetEntity();
			int slotId = InventorySlots.GetSlotIdFromString("KeyChain");
			if (vehicle.GetInventory().HasAttachmentSlot(slotId))
			{
				auto keychain = ExpansionKeyChainBase.Cast(vehicle.GetAttachmentByType(ExpansionKeyChainBase));
				if (!keychain || !keychain.Expansion_HasOwner())
				{
					array<ExpansionCarKey> keys = {};
					ExpansionCarKey.GetKeysForVehicle(this, keys);
					foreach (ExpansionCarKey key: keys)
					{
						//! Will assign a keychain if key is in player inventory
						if (key.IsMaster() && !key.GetAttachmentByType(ExpansionKeyChainBase))
							key.Expansion_AssignKeychain(key.GetHierarchyRootPlayer(), vehicle);
					}
				}
			}
		}
	}

	void OnItemAttached(EntityAI item, string slotName)
	{
		if (GetGame().IsServer())
		{
#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
			ExpansionDoor door;
			string slotNameLower = slotName;
			slotNameLower.ToLower();
			if (m_Doors.Find(slotNameLower, door))
			{
				door.SetDoor(CarDoor.Cast(item));
			}
#endif
		}

		if (item.IsInherited(ExpansionKeyChainBase))
		{
			ResetOwner();  //! Owner info stored on keychain, reset owner info on vehicle if present
			m_KeyChain = ExpansionKeyChainBase.Cast(item);
		}
	}

	void OnItemDetached(EntityAI item, string slotName)
	{
		if (item.IsInherited(ExpansionKeyChainBase))
			m_KeyChain = null;
	}

	void OnSimulate(float dt)
	{
	#ifdef EXPANSION_VEHICLE_DOOR_JOINTS
		foreach (ExpansionDoor door: m_Doors)
		{
			door.OnUpdate(dt);
		}
	#endif
	}

	void OnPostSimulate(float dt)
	{
		if (CanUpdateLock(dt))
		{
			UpdateLock(dt);
		}
	}

	void OnDoorOpened(string source)
	{
		if (HasKey())
		{
			if (m_LockState == ExpansionVehicleLockState.READY_TO_LOCK)
			{
				SetLockState(ExpansionVehicleLockState.UNLOCKED);
				KeyMessage("OnDoorOpened::UNLOCKED");
			}
		}
	}

	void OnDoorClosed(string source)
	{
	}

	bool NameOverride(out string output)
	{
		bool ret;

		EntityAI vehicle = GetEntity();

		if (IsLocked())
		{
			output = string.Format("%1 (%2)", vehicle.ConfigGetString("displayName"), "#locked");
			ret = true;
		}

		string ownerName = GetOwnerName();
		if (ownerName)
		{
			if (!output)
				output = vehicle.ConfigGetString("displayName");
			output = string.Format("%1's %2", ownerName, output);
			ret = true;
		}

		return ret;
	}

	void AssignOwner(notnull Man player, bool send = true)
	{
		if (player.GetIdentity())
			AssignOwner(player.GetIdentity(), send);
		else
			AssignOwner("", player.GetDisplayName(), send);
	}

	void AssignOwner(notnull PlayerIdentity owner, bool send = true)
	{
		AssignOwner(owner.GetId(), owner.GetName(), send);
	}

	void AssignOwner(string ownerUID, string ownerName, bool send = true)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
#endif
		
		if (ownerUID == m_OwnerUID && ownerName == m_OwnerName)
			return;

		m_HasOwner = true;
		m_OwnerUID = ownerUID;
		m_OwnerName = ownerName;
		m_NetsyncData.Set(0, m_OwnerName);
		if (send)
			m_NetsyncData.Send(null);
		
#ifdef EXTRACE
		EXTrace.Add(trace, m_OwnerUID);
		EXTrace.Add(trace, m_OwnerName);
#endif
	}

	bool HasOwner()
	{
		return m_HasOwner;
	}

	bool IsOwner(notnull Man player)
	{
		return IsOwner(player.GetIdentity());
	}

	bool IsOwner(notnull PlayerIdentity identity)
	{
		return IsOwner(identity.GetId());
	}

	bool IsOwner(string playerUID)
	{
		if (playerUID == GetOwnerUID())
			return true;

		return false;
	}

	void ResetOwner()
	{
		if (!m_HasOwner)
			return;

		m_HasOwner = false;
		m_OwnerUID = "";
		m_OwnerName = "";
		m_NetsyncData.Set(0, m_OwnerName);
		m_NetsyncData.Send(null);
	}

	string GetOwnerName()
	{
		if (m_KeyChain)
			return m_KeyChain.Expansion_GetOwnerName();

		return m_OwnerName;
	}

	string GetOwnerUID()
	{
		if (m_KeyChain)
			return m_KeyChain.Expansion_GetOwnerUID();

		return m_OwnerUID;
	}

	float GetModelZeroPointDistanceFromGround()
	{
		if (m_ModelZeroPointDistanceFromGround < 0)
		{
			string path = "CfgVehicles " + GetType() + " modelZeroPointDistanceFromGround";
			if (GetGame().ConfigIsExisting(path))
			{
				m_ModelZeroPointDistanceFromGround = GetGame().ConfigGetFloat(path);
			}
			else
			{
				vector minMax[2];
				GetEntity().GetCollisionBox(minMax);
				float diff = -minMax[0][1];
				if (diff > 0)
					m_ModelZeroPointDistanceFromGround = diff;
				else
					m_ModelZeroPointDistanceFromGround = 0;
			}

			EXTrace.Print(EXTrace.VEHICLES, this, GetType() + " modelZeroPointDistanceFromGround " + m_ModelZeroPointDistanceFromGround);
		}

		return m_ModelZeroPointDistanceFromGround;
	}

	string GetPlaceholderType(string coverType)
	{
		string type = GetType();

		string skinBase = GetEntity().ConfigGetString("skinBase");
		if (skinBase)
			type = skinBase;

		string placeholderType;

		TStringArray coverVariants = {"Civil", "Desert", "Winter", ""};
		foreach (string coverVariant: coverVariants)
		{
			if (coverVariant == string.Empty)
				placeholderType = type + "_Cover";
			else if (coverType.Contains(coverVariant))
				placeholderType = type + "_Cover_" + coverVariant;
			else
				continue;

			if (GetGame().ConfigIsExisting("CfgVehicles " + placeholderType))
				break;
			else
				placeholderType = "Expansion_Generic_Vehicle_Cover";
		}

		return placeholderType;
	}

	ExpansionPhysicsState GetPhysicsState()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}

#ifdef EXPANSION_MODSTORAGE
	void OnStoreSave(CF_ModStorage ctx)
	{
		ctx.Write(m_MasterKeyPersistentIDA);
		ctx.Write(m_MasterKeyPersistentIDB);
		ctx.Write(m_MasterKeyPersistentIDC);
		ctx.Write(m_MasterKeyPersistentIDD);

		int lockState = m_LockState;
		ctx.Write(lockState);

		ctx.Write(m_HasOwner);

		if (m_HasOwner)
		{
			ctx.Write(m_OwnerUID);
			ctx.Write(m_OwnerName);
		}
	}

	bool OnStoreLoad(CF_ModStorage ctx)
	{
		if (!ctx.Read(m_MasterKeyPersistentIDA))
			return false;

		if (!ctx.Read(m_MasterKeyPersistentIDB))
			return false;

		if (!ctx.Read(m_MasterKeyPersistentIDC))
			return false;

		if (!ctx.Read(m_MasterKeyPersistentIDD))
			return false;

		int lockState;
		if (!ctx.Read(lockState))
			return false;

		m_LockState = lockState;

		if (ctx.GetVersion() >= 53 && !OnStoreLoad_Owner(ctx))
			return false;

		return true;
	}

	bool OnStoreLoad_Owner(CF_ModStorage ctx)
	{
		if (!ctx.Read(m_HasOwner))
			return false;

		if (m_HasOwner)
		{
			if (!ctx.Read(m_OwnerUID))
				return false;

			if (!ctx.Read(m_OwnerName))
				return false;

			m_NetsyncData.Set(0, m_OwnerName);
		}

		return true;
	}
#endif

	void SetHasPilot(bool state)
	{
	}

	bool LeavingSeatDoesAttachment(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsTowing()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void DestroyTow()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool IsRotorDamaged()
	{
		EntityAI entity = GetEntity();
		ExpansionHelicopterScript hs;
		ExpansionVehicleHelicopterBase evhb;
		if (Class.CastTo(hs, entity))
			return hs.Expansion_IsRotorDamaged();
		else if (Class.CastTo(evhb, entity))
			return evhb.Expansion_IsRotorDamaged();

		return false;
	}
}
