/**
 * WeaponManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class WeaponManager
{
	override bool CanLoadBullet(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if (!super.CanLoadBullet(wpn, mag, reservationCheck))
			return false;

		if (mag.Expansion_IsQuestItem())
		{
			if (wpn.Expansion_IsQuestItem())
			{
				if (mag.Expansion_GetQuestID() != wpn.Expansion_GetQuestID())
					return false;
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	/*override bool CanAttachMagazine(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if (!super.CanAttachMagazine(wpn, mag, reservationCheck))
			return false;

		if (mag.Expansion_IsQuestItem())
			return false;

		return true;
	}

	override bool CanSwapMagazine(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if (!super.CanSwapMagazine(wpn, mag, reservationCheck))
			return false;

		if (mag.Expansion_IsQuestItem())
			return false;

		return true;
	}*/
};
