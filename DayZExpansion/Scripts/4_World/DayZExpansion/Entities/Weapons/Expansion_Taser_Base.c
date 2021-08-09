/**
 * Expansion_Taser_Base.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_Taser_Base extends FNX45_Base
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
};

class Mag_Expansion_Taser extends Mag_FNX45_15Rnd
{
	override void SetActions()
	{
		super.SetActions();

		RemoveAction( ActionLoadMagazine );
		RemoveAction( ActionEmptyMagazine );
		RemoveAction( ActionLoadMagazineQuick );
	}
};