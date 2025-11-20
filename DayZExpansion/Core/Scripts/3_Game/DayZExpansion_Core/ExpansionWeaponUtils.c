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

	//! Maps bullet type to health type to damage applied
	static ref map<string, ref map<string, float>> s_DamageAppliedByBullet = new map<string, ref map<string, float>>();

	//! Maps weapon classname to init speed mult
	static ref map<string, float> s_InitSpeedMultipliers = new map<string, float>();

	static float GetDamageAppliedByAmmo(string ammoTypeName, string healthType="Health")
	{
		map<string, float> healthTypeMap;
		float damageApplied;

		if (s_DamageAppliedByAmmo.Find(ammoTypeName, healthTypeMap))
		{
			damageApplied = healthTypeMap[healthType];
		}
		else
		{
			string bulletTypeNamePath = "CfgMagazines " + ammoTypeName + " ammo";
			if (g_Game.ConfigIsExisting(bulletTypeNamePath))
			{
				string bulletTypeName = g_Game.ConfigGetTextOut(bulletTypeNamePath);
				damageApplied = GetDamageAppliedByBullet(bulletTypeName, healthType);
				s_DamageAppliedByAmmo[ammoTypeName] = s_DamageAppliedByBullet[bulletTypeName];
			}
		}

		return damageApplied;
	}

	static float GetDamageAppliedByBullet(string bulletTypeName, string healthType="Health")
	{
		map<string, float> healthTypeMap;
		float damageApplied;

		if (!s_DamageAppliedByBullet.Find(bulletTypeName, healthTypeMap))
		{
			healthTypeMap = new map<string, float>;
			s_DamageAppliedByBullet[bulletTypeName] = healthTypeMap;
		}

		if (!healthTypeMap.Find(healthType, damageApplied))
		{
			string damageAppliedPath = "CfgAmmo " + bulletTypeName + " DamageApplied " + healthType + " damage";
			if (g_Game.ConfigIsExisting(damageAppliedPath))
				damageApplied = g_Game.ConfigGetFloat(damageAppliedPath);
			healthTypeMap[healthType] = damageApplied;
		}

		return damageApplied;
	}

	static float GetWeaponInitSpeedMultiplier(string weaponTypeName)
	{
		float mult = 1.0;

		if (!s_InitSpeedMultipliers.Find(weaponTypeName, mult))
		{
			string path = "CfgWeapons " + weaponTypeName + " initSpeedMultiplier";
			if (g_Game.ConfigIsExisting(path))
				mult = g_Game.ConfigGetFloat(path);
			s_InitSpeedMultipliers.Insert(weaponTypeName, mult);
			//EXPrint(weaponTypeName + " " + mult, " [ExpansionWeaponUtils] ");
		}

		return mult;
	}
}
