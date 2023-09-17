/**
 * MagazineStorage.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MagazineStorage
{
	override bool IsCompatiableAmmo( ItemBase ammo )
	{
		if (!super.IsCompatiableAmmo(ammo))
			return false;

		if (Expansion_IsQuestItem() || ammo.Expansion_IsQuestItem())
		{
			if (Expansion_IsDeliveryItem() || ammo.Expansion_IsDeliveryItem())
				return false;
			
			if (Expansion_GetQuestID() != ammo.Expansion_GetQuestID())
				return false;
		}

		return true;
	}
};