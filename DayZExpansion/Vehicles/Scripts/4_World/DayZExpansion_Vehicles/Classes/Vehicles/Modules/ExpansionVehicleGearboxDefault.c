/*
 * ExpansionVehicleGearboxDefault.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleGearboxDefault : ExpansionVehicleGearboxAdvanced
{
	float m_Reverse;

	void ExpansionVehicleGearboxDefault(EntityAI pVehicle, string rootPath)
	{
		string path;
		
		path = rootPath + " reverse";
		if (GetGame().ConfigIsExisting(path))
			m_Reverse = GetGame().ConfigGetFloat(path);

		m_Gears.Insert("R");
		m_Gears.Insert("N");

		for (int i = 0; i < m_Ratios.Count(); i++)
			m_Gears.Insert("" + i);

		m_Ratios.InsertAt(-m_Reverse, 0);
		m_Ratios.InsertAt(0.0, 1);
	}
};
