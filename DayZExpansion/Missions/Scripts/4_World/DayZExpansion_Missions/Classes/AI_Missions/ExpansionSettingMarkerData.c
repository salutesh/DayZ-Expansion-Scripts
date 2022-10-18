/**
 * ExpansionSettingMarkerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSettingMarkerData
 * @brief		This class handle ai missions meta
 **/
class ExpansionSettingMarkerData: ExpansionColorSettingsBase
{
	string Name;
	string Icon;
	string Color;
	vector Position;
	bool Is3D;
	
	// ------------------------------------------------------------
	// Expansion ExpansionSettingMarkerData
	// ------------------------------------------------------------
	void ExpansionSettingMarkerData(string name = "", string icon = "", string color = "", vector position = "0 0 0", bool is3d = false)
	{
		Name 		= name;
		Icon 		= icon;
		Color 		= color;
		Position 	= position;
		Is3D 		= is3d;
	}
	
	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults
		if (!Get("Color"))
			Set("Color", ARGB(255, 235, 69, 235));
	}
};