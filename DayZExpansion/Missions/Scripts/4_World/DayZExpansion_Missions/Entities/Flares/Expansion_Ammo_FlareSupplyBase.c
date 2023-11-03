/**
 * Expansion_Ammo_FlareSupplyBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Flaregun
{
	override void OnFire(int muzzle_index)
	{
		if (GetGame().IsServer())
		{
			float ammoDamage;
			string ammoType;
			GetCartridgeInfo(muzzle_index, ammoDamage, ammoType);
			switch (ammoType)
			{
				case "Bullet_Expansion_FlareSupply":
				case "Bullet_Expansion_FlareSupplyRed":
				case "Bullet_Expansion_FlareSupplyGreen":
				case "Bullet_Expansion_FlareSupplyBlue":
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionMissionModule.s_Instance.CallAirdrop, 2000, false, GetPosition());
					break;
			}
		}

		super.OnFire(muzzle_index);
	}
}

class Expansion_Ammo_FlareSupplyBase: Ammo_Flare {};
class Expansion_Ammo_FlareSupplyRed: Ammo_Flare {};
class Expansion_Ammo_FlareSupplyGreen: Ammo_Flare {};
class Expansion_Ammo_FlareSupplyBlue: Ammo_Flare {};
