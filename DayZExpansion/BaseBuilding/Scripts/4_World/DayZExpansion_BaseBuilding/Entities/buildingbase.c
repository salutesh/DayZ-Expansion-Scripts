modded class BuildingBase
{
	protected ref map<string, float> m_Expansion_HealthBeforeHit;
	protected float m_Expansion_DamageMultiplier = 1.0;

	void BuildingBase()
	{
		if (CanUseConstruction())
			m_Expansion_HealthBeforeHit = new map<string, float>;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		Rebuilding rebuilding;
		if (!Class.CastTo(rebuilding, GetConstructionBasic()) || rebuilding.GetConstructionPartArray().Count() == 0)
			return;

		//! Locks may attach before the construction data is loaded and miss the door orientation
		GameInventory inventory = GetInventory();
		int count = inventory.AttachmentCount();
		for (int i = 0; i < count; ++i)
		{
			ExpansionCodeLock codeLock;
			if (Class.CastTo(codeLock, inventory.GetAttachmentFromIndex(i)))
				codeLock.Expansion_RefreshDoorInfoServer();
		}
	}
	
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		//! A lock locks the door itself and nothing else may lock it while the lock is attached
		//! Vanilla already deals with CombinationLock and DigitalCodeLock, we deal with anything else
		if (g_Game.IsServer() && !item.IsInherited(CombinationLock) && !item.IsInherited(DigitalCodeLock))
		{
			ItemBase ib;
			if (Class.CastTo(ib , item) && ib.Expansion_IsLock())
			{
				int doorIndex = Expansion_GetDoorIndexByLockSlot(slot_name);
				if (doorIndex > -1 && IsDoorLocked(doorIndex))
					UnlockDoor(doorIndex);
			}
		}
	}

	int Expansion_GetDoorIndexByLockSlot(string slotName)
	{
		AdditionalDoorInfosHolder doorInfoHolder = m_AdditionalDoorInfosMap.Get(GetType());
		if (!doorInfoHolder)
			return -1;

		map<int, ref AdditionalDoorInfo> doorsInfoMap = doorInfoHolder.GetDoorInfoMap();
		if (!doorsInfoMap)
			return -1;

		foreach (int doorInfoIndex, AdditionalDoorInfo doorInfo : doorsInfoMap)
		{
			if (doorInfo.m_RelatedInventorySlotNames && doorInfo.m_RelatedInventorySlotNames.Find(slotName) > -1)
				return doorInfoIndex;
		}

		return -1;
	}

	override bool EEOnDamageCalculated(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if (!super.EEOnDamageCalculated( damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef))
			return false;

		if (!Expansion_CanBeDamaged())
			return false;

		if (CanUseConstruction())
			m_Expansion_HealthBeforeHit[dmgZone] = GetHealth(dmgZone, "Health");

		if (damageType == DT_EXPLOSION)
			m_Expansion_DamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		else
			m_Expansion_DamageMultiplier = GetExpansionSettings().GetRaid().ProjectileDamageMultiplier;

		if (!m_Expansion_DamageMultiplier)
		{
		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.BASEBUILDING, this, "EEOnDamageCalculated - damage multiplier is zero");
		#endif
			return false;
		}

		if (damageType == DT_EXPLOSION && !GetExpansionSettings().GetRaid().CheckExplosionDamageAllowed(this, source))
			return false;

		return true;
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

#ifdef DIAG_DEVELOPER
		EXTrace.PrintHit(EXTrace.GENERAL_ITEMS, this, "EEHitBy", damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
#endif
		if (!CanUseConstruction())
			return;

		string damageZone = dmgZone;
		if (damageZone == "")
			damageZone = "GlobalHealth";

		float health = m_Expansion_HealthBeforeHit[dmgZone];
		float dmg = damageResult.GetDamage(damageZone, "Health");

		bool applyDamageCorrection;
		if ((damageType == DT_EXPLOSION || damageType == DT_FIRE_ARM) && m_Expansion_DamageMultiplier != 1.0)
			applyDamageCorrection = true;

		if (damageType == DT_EXPLOSION && ExpansionDamageSystem.IsEnabledForExplosionTarget(this))
		{
			ExpansionDamageSystem.OnExplosionHit(source, this, ammo);

			//! Use our own damage system for consistent explosion damage
			//! Note that this only works as intended if damage source root is not a player,
			//! else won't be able to get actual source's position in relation to target

			if (!GetHierarchyParent() && source && !source.GetHierarchyRootPlayer())
			{
				float baseDmg = ExpansionDamageSystem.GetExplosionDamage(source, this, ammo);
				if (baseDmg > dmg)
				{
					ExpansionDamageSystem.Log("Overriding " + source.ToString() + " damage dealt to " + ToString() + " at " + GetPosition() + " " + dmg.ToString() + " -> " + baseDmg.ToString());
					dmg = baseDmg;
					applyDamageCorrection = true;
				}
			}
		}

		if (applyDamageCorrection)
		{
			//! damageMultiplier > 1 applies bonus damage
			//! damageMultiplier < 1 negates damage (partly if multiplier > 0 or fully if 0)
			//! damageMultiplier == 1 effectively does nothing
			if (health > 0)
				SetHealth(damageZone, "Health", Math.Max(health - (dmg * m_Expansion_DamageMultiplier), 0));
		}

		GetExpansionSettings().GetLog().RaidLog(this, source, damageZone, ammo, health, dmg, m_Expansion_DamageMultiplier);
	}

	override void SetActions()
	{
		if (!CanUseConstruction() || GetDoorCount() == 0)
			return;

		AddAction(ExpansionActionOpen);

		//! Order matters. Have ExpansionActionEnterCodeLock AFTER ExpansionActionOpen
		//! so that "Open locked" shows on locked items without having to cycle through actions in the UI.
		AddAction(ExpansionActionEnterCodeLock);
		AddAction(ExpansionActionChangeCodeLock);

		//! Order matters. Have ExpansionActionClose AFTER ExpansionActionEnterCodeLock
		//! so that "Lock" shows on opened items without having to cycle through actions in the UI.
		AddAction(ExpansionActionClose);

		//! Order matters. Vanilla actions AFTER Expansion actions so that "Close & lock" can show before "Close".
		super.SetActions();
	}

	override bool CanDoorBeOpened(notnull DoorManipulationParams params)
	{
		bool ret = super.CanDoorBeOpened(params);

		int doorIndex = params.m_DoorIndex;

		if (!ret && IsDoorOpen(doorIndex))
			return false;

		ItemBase codelock = Expansion_GetCodeLock(doorIndex);
		if (codelock && codelock.ExpansionIsLocked())
		{
			//! If an Expansion codelock is attached, open action is handled by ExpansionActionOpen
			if (!codelock.IsInherited(ExpansionCodeLock))
			{
				//! Allow open locked if known user
				PlayerBase player;
				if (Class.CastTo(player, params.m_Caller))
				{
					if (codelock.IsKnownUser(player) || codelock.Expansion_CanUnlockFromInside(player, ""))
						return !params.m_CheckIfLocked;  //! enable ActionLockedDoors
				}
			}

			return false;
		}

		return ret;
	}

	bool Expansion_CanBeDamaged()
	{
		if (!CanUseConstruction())
			return true;

		auto settings = GetExpansionSettings().GetRaid();
		return settings.IsRaidableNow();
	}

	bool Expansion_HasCodeLock(int doorIndex)
	{
		return Expansion_GetCodeLock(doorIndex) != null;
	}

	void Expansion_CloseAndLock(int doorIndex)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		ItemBase codelock = Expansion_GetCodeLock(doorIndex);
		if (codelock && !codelock.ExpansionIsLocked())
			codelock.ExpansionLock();
		else if (!IsDoorLocked(doorIndex))
			LockDoor(doorIndex, true);
	}

	void Expansion_UnlockAndOpen(int doorIndex)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		ItemBase codelock = Expansion_GetCodeLock(doorIndex);
		if (codelock && codelock.ExpansionIsLocked())
			codelock.ExpansionUnlock();
		else if (IsDoorLocked(doorIndex))
			UnlockDoor(doorIndex, false);

		if (CanDoorBeOpened(doorIndex, true))
			OpenDoor(doorIndex);
	}

	bool Expansion_CanOpen(PlayerBase player, int doorIndex)
	{
		ItemBase codelock = Expansion_GetCodeLock(doorIndex);
		if (!codelock || !codelock.ExpansionIsLocked() || codelock.IsKnownUser(player) || codelock.Expansion_CanUnlockFromInside(player, ""))
			return !IsDoorOpen(doorIndex);

		return false;
	}

	bool Expansion_CanClose(PlayerBase player, int doorIndex)
	{
		return CanDoorBeClosed(doorIndex);
	}

	void Expansion_Open(int doorIndex)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		if (CanDoorBeOpened(doorIndex, true))
			OpenDoor(doorIndex);
	}

	void Expansion_Close(int doorIndex)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.BASEBUILDING, this);
	#endif

		if (CanDoorBeClosed(doorIndex))
			CloseDoor(doorIndex);
	}

	ItemBase Expansion_GetCodeLock(int doorIndex)
	{
		AdditionalDoorInfo doorInfo = GetDoorInfo(doorIndex);
		if (doorInfo && doorInfo.m_RelatedInventorySlotNames)
		{
			ItemBase item;
			foreach (string slotName : doorInfo.m_RelatedInventorySlotNames)
			{
				if (Class.CastTo(item, FindAttachmentBySlotName(slotName)) && item.Expansion_IsCodeLock())
					return item;
			}
		}

		return null;
	}

	int Expansion_FindAttachmentDoorIndex(EntityAI attachment)
	{
		int doorCount = GetDoorCount();
		for (int doorIndex = 0; doorIndex < doorCount; ++doorIndex)
		{
			AdditionalDoorInfo doorInfo = GetDoorInfo(doorIndex);
			if (doorInfo && doorInfo.m_RelatedInventorySlotNames)
			{
				foreach (string slotName : doorInfo.m_RelatedInventorySlotNames)
				{
					if (FindAttachmentBySlotName(slotName) == attachment)
						return doorIndex;
				}
			}
		}

		return -1;
	}
}
