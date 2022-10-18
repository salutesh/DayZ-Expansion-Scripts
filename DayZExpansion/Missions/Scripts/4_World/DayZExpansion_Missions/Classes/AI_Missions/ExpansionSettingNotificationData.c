/**
 * ExpansionSettingNotificationData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSettingNotificationData
 * @brief		This class handle ai missions meta
 **/
class ExpansionSettingNotificationData: ExpansionColorSettingsBase
{
	string Title;
	string Description;
	string Icon;
	string Color;
	
	// ------------------------------------------------------------
	// Expansion ExpansionSettingNotificationData
	// ------------------------------------------------------------
	void ExpansionSettingNotificationData(string title = "", string description = "", string icon = "", string color = "")
	{
		Title 		= title;
		Description = description;
		Icon 		= icon;
		Color 		= color;
	}
	
	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults
		if (!Get("Color"))
			Set("Color", ARGB(255, 235, 69, 235));
	}
};