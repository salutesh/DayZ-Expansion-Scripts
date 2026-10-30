modded class DigitalCodeLock
{
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
		SetLockState(true);
	}

	override void ExpansionUnlock()
	{
		SetLockState(false);
	}
		
	override void OnWasAttached(EntityAI parent, int slot_id)
	{
		super.OnWasAttached(parent, slot_id);
		
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
}
