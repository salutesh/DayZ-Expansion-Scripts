/**
 * ExpansionBookSettingCategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookSettingCategory
{
    string CategoryName;
    ref array<ref ExpansionBookSetting> Settings = new array<ref ExpansionBookSetting>;
	
	void ExpansionBookSettingCategory(string name)
	{
		CategoryName = name;
	}
	
	void AddSetting(string title, string text = "", string value = "")
	{
		ExpansionBookSetting setting = new ExpansionBookSetting(title, text, value);
		Settings.Insert(setting);
	}
	
	
	void AddSettingBool(string title, string text = "", bool value = false)
	{
		string value_text;
		
		if (value)
		{
			value_text = "Enabled";
		}
		else
		{
			value_text = "Disabled";
		}
		
		AddSetting(title, text, value_text);
	}
};

class ExpansionBookSetting
{
    string SettingTitle;
    string SettingText;
	string SettingValue;

    void ExpansionBookSetting(string title, string text, string value)
	{
		SettingTitle = title;
		SettingText = text;
		SettingValue = value;
	}
};