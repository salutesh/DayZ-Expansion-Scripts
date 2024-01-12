/**
 * Expansion_Taser_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Taser_Base: FNX45_Base
{
	override void SetActions()
	{
		super.SetActions();
		RemoveAction( FirearmActionLoadBullet );
		RemoveAction( FirearmActionMechanicManipulate );
		RemoveAction( ActionTurnOnWeaponFlashlight );
		RemoveAction( ActionTurnOffWeaponFlashlight );
		RemoveAction( FirearmActionLoadBulletQuick );
	}

	override void OnDebugSpawn()
	{
		SpawnAttachedMagazine("Mag_Expansion_Taser");
	}
};

class Expansion_Taser_Debug: Expansion_Taser_Base
{
#ifdef DIAG
	void Expansion_Taser_Debug()
	{
		if (GetGame().IsServer())
			SetAllowDamage(false);
	}

	override void EEFired(int muzzleType, int mode, string ammoType)
	{
		super.EEFired(muzzleType, mode, ammoType);

		//! Always have unlimited ammo, even if vanilla debug option is disabled
		if ((ItemBase.GetDebugActionsMask() & DebugActionType.UNLIMITED_AMMO) == 0)
		{
			Magazine magazine = GetMagazine(GetCurrentMuzzle());
			if (magazine)
			{
				if (GetGame().IsServer())
					magazine.ServerSetAmmoMax();
				else
					magazine.LocalSetAmmoMax();
			}
		}
	}
#endif
};

class Mag_Expansion_Taser: Mag_FNX45_15Rnd
{
	override void SetActions()
	{
		super.SetActions();

		RemoveAction( ActionLoadMagazine );
		RemoveAction( ActionEmptyMagazine );
		RemoveAction( ActionLoadMagazineQuick );
	}
};