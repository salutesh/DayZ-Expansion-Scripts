/**
 * DayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZGame
{
	protected static ref map<string, string> m_Expansion_MarketAmmoBoxes = new map<string, string>;

	void DayZGame()
	{
		//! Ammo boxes and corresponding ammo are only needed on client
	#ifndef SERVER
		int count = ConfigGetChildrenCount(CFG_VEHICLESPATH);
	
		EXPrint(ToString() + " - enumerating " + count + " " + CFG_VEHICLESPATH + " entries");

		for (int i = 0; i < count; i++)
		{
			string className;

			ConfigGetChildName(CFG_VEHICLESPATH, i, className);

			if (ConfigGetInt(CFG_VEHICLESPATH + " " + className + " scope") != 2)
				continue;

			if (!IsKindOf(className, "Box_Base"))
				continue;

			string path = CFG_VEHICLESPATH + " " + className + " Resources";
			if (ConfigIsExisting(path))
			{
				int resCount = ConfigGetChildrenCount(path);

				for (int j = 0; j < resCount; j++)
				{
					string childName;
					ConfigGetChildName(path, j, childName);
					if (!IsKindOf(childName, "Ammunition_Base"))
						continue;

					m_Expansion_MarketAmmoBoxes.Insert(childName, className);
				}
			}
		}

		EXPrint(ToString() + " - found " + m_Expansion_MarketAmmoBoxes.Count() + " ammo boxes with corresponding ammo");
	#endif
	}

	string Expansion_GetAmmoBoxByAmmoName(string name)
	{
		return m_Expansion_MarketAmmoBoxes.Get(name);
	}
}
