/**
 * ExpansionWeaponUtils.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionWeaponUtils
{
	//! Maps ammo type to health type to damage applied
	static ref map<string, ref map<string, float>> s_DamageAppliedByAmmo = new map<string, ref map<string, float>>();

	//! Maps weapon classname to init speed mult
	static ref map<string, float> s_InitSpeedMultipliers = new map<string, float>();

	static float GetDamageAppliedByAmmo(string ammoTypeName, string healthType="Health")
	{
		map<string, float> healthTypeMap;
		float damageApplied;

		if (!s_DamageAppliedByAmmo.Find(ammoTypeName, healthTypeMap))
		{
			healthTypeMap = new map<string, float>;
			s_DamageAppliedByAmmo.Insert(ammoTypeName, healthTypeMap);
		}

		if (!healthTypeMap.Find(healthType, damageApplied))
		{
			string bulletTypeNamePath = "CfgMagazines " + ammoTypeName + " ammo";
			if (GetGame().ConfigIsExisting(bulletTypeNamePath))
			{
				string bulletTypeName = GetGame().ConfigGetTextOut(bulletTypeNamePath);
				string damageAppliedPath = "CfgAmmo " + bulletTypeName + " DamageApplied " + healthType + " damage";
				if (GetGame().ConfigIsExisting(damageAppliedPath))
					damageApplied = GetGame().ConfigGetFloat(damageAppliedPath);
			}
			healthTypeMap.Insert(healthType, damageApplied);
			//EXPrint(ammoTypeName + " " + healthType + " " + damageApplied, " [ExpansionWeaponUtils] ");
		}

		return damageApplied;
	}

	static float GetWeaponInitSpeedMultiplier(string weaponTypeName)
	{
		float mult = 1.0;

		if (!s_InitSpeedMultipliers.Find(weaponTypeName, mult))
		{
			string path = "CfgWeapons " + weaponTypeName + " initSpeedMultiplier";
			if (GetGame().ConfigIsExisting(path))
				mult = GetGame().ConfigGetFloat(path);
			s_InitSpeedMultipliers.Insert(weaponTypeName, mult);
			//EXPrint(weaponTypeName + " " + mult, " [ExpansionWeaponUtils] ");
		}

		return mult;
	}
}
