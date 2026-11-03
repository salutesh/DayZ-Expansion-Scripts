modded class DigitalCodeLock
{
	void DigitalCodeLock()
	{
		m_Expansion_KnownUIDs = new TStringArray;
	}

	override bool IsPlayerOnBackside(PlayerBase player)
	{
		bool isFacingBackside = super.IsPlayerOnBackside(player);

		ExpansionBaseBuilding exbb;
		if (Class.CastTo(exbb, GetHierarchyParent()))
			isFacingBackside = exbb.Expansion_IsPlayerInside(player, "");

		return isFacingBackside;
	}

	override bool Expansion_IsPlayerInside(PlayerBase player, string selection)
	{
		return IsPlayerOnBackside(player);
	}

	override bool Expansion_IsLock()
	{
		return true;
	}

	override bool Expansion_IsCodeLock()
	{
		return true;
	}

	override bool ExpansionIsLocked()
	{
		return IsLocked();
	}

	override void SetCode(string code, PlayerBase player = NULL, bool setUser = true, bool updateLock = true)
	{
		m_CodeLockComponent.SetLockCode(code);
	}

	override bool HasCode()
	{
		return HasPIN();
	}

	override string GetCode()
	{
		return GetLockPIN();
	}

	override int GetCodeLength()
	{
		return GetLockPIN().Length();
	}

	override void ExpansionLock()
	{
		if (IsDamageDestroyed())
			return;

		SetLockState(true);
	}

	override void ExpansionUnlock()
	{
		SetLockState(false);
	}

	override void ExpansionOnDestroyed(Object killer)
	{
		if (IsDamageDestroyed())
			return;

		SetHealth(0);
		SetLockState(false);
	}
	
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);

		Expansion_RegisterLockRPCs();
		
		if (g_Game.IsServer())
		{
			//! Vanilla only checks the Att_CodeLock slot and requires construction,
			//! we do the inverse so the codelock starts working once attached regardless of parent
			string slotName = InventorySlots.GetSlotName(slot_id);
			if (slotName.IndexOf("Att_CodeLock") == -1)
			{
				//! Start energy consumption once code lock is attached with a battery
				Battery9V battery = GetBattery();
				if (battery && battery.GetCompEM().GetEnergy() > 0 && !GetCompEM().IsWorking())
				{
					m_CodeLockComponent.SetStage(CodeLockStage.SETTING);
					GetCompEM().SwitchOn();
				}
				
				UpdateVisuals();
			}
		}
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_Expansion_KnownUIDs.Count());
		foreach (string knownUID: m_Expansion_KnownUIDs)
		{
			ctx.Write(knownUID);
		}
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (ctx.GetVersion() < 53)
			return true;

		int count;
		if (!ctx.Read(count))
			return false;

		while (count--)
		{
			string knownUID;
			if (!ctx.Read(knownUID))
				return false;

			m_Expansion_KnownUIDs.Insert(knownUID);
			EXError.Info(this, knownUID);
		}

		return true;
	}
	#endif
}
