/**
 * ExpansionServerInfoSetting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionServerInfoSetting
{
	string m_Name;
	string m_Description;
	bool m_IsBoolSetting;
	bool m_State;
	string m_Value;
	
	// ------------------------------------------------------------
	// ExpansionServerInfoSettings Constructor
	// ------------------------------------------------------------
	void ExpansionServerInfoSetting(string name, string desc, bool isbool, bool state = false, string value = "")
	{		
		m_Name = name;
		m_Description = desc;
		m_IsBoolSetting = isbool;
		m_State = state;
		m_Value = value;
	}
}