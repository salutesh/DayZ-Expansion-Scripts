/**
 * Expansion_LobotomyDartGun.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_LobotomyDartGun: FNX45_Base
{
	override void SetActions()
	{
		super.SetActions();
		RemoveAction(FirearmActionMechanicManipulate);
		RemoveAction(ActionTurnOnWeaponFlashlight);
		RemoveAction(ActionTurnOffWeaponFlashlight);
	}

	override void OnDebugSpawn()
	{
		SpawnAttachedMagazine("Mag_Expansion_LobotomyDart");
	}
};

class Expansion_LobotomyDartGun_Debug: Expansion_LobotomyDartGun
{
#ifdef DIAG_DEVELOPER
	void Expansion_LobotomyDartGun_Debug()
	{
		if (g_Game.IsServer())
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
				if (g_Game.IsServer())
					magazine.ServerSetAmmoMax();
				else
					magazine.LocalSetAmmoMax();
			}
		}
	}
#endif
}

class Mag_Expansion_LobotomyDart: Mag_FNX45_15Rnd
{
}
