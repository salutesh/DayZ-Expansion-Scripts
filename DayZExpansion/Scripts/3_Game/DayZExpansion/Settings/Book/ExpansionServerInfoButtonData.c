/**
 * ExpansionServerInfoButtonData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionServerInfoButtonData
 * @brief
 **/
class ExpansionServerInfoButtonData
{
	string IconPath;	//! Icon that gets displayed on the button
	string URL;			//! URL of the website that gets opened when button is clicked
	string Tooltip;		//! Tooltip text that gets diplayed when pointing on the button
	int Color;			//! Icon color when pointing on the button.
	
	// ------------------------------------------------------------
	// ExpansionServerInfoButtonData Constructor
	// ------------------------------------------------------------
	void ExpansionServerInfoButtonData(string icon, string url, string tooltip, int color)
	{
		this.IconPath = icon;
		this.URL = url;
		this.Tooltip = tooltip;
		this.Color = color;
	}
}