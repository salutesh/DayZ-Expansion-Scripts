/**
 * ExpansionMarketMenuItemManagerPreset.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItemManagerPreset
{
	string ClassName;
	string PresetName;
	ref array<string> ItemAttachments = new array<string>;
	
	void AddAttachment(string className)
	{
		int findIndex = -1;
		findIndex = ItemAttachments.Find(className);
		if (findIndex == -1)
		{
			ItemAttachments.Insert(className);
		}
	}
	
	void RemoveAttachment(string className)
	{
		int findIndex = -1;
		findIndex = ItemAttachments.Find(className);
		if (findIndex > -1)
		{
			ItemAttachments.Remove(findIndex);
		}
	}
	
	void SaveItemPreset(string path)
	{		
		JsonFileLoader<ExpansionMarketMenuItemManagerPreset>.JsonSaveFile(path + PresetName + ".json", this);
	}

	static ExpansionMarketMenuItemManagerPreset LoadItemPreset(string presetName, string path)
	{
		ExpansionMarketMenuItemManagerPreset data = new ExpansionMarketMenuItemManagerPreset;
		JsonFileLoader<ExpansionMarketMenuItemManagerPreset>.JsonLoadFile(path + presetName + ".json", data);
		data.PresetName = presetName;
		
		return data;
	}
	
	array<string> GetAttachments()
	{
		return ItemAttachments;
	}
};