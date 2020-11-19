/**
 * ExpansionServerInfos.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionServerInfos
 * @brief
 **/
class ExpansionServerInfos
{
	ref array<ref ExpansionServerInfoButtonData> ServerButtons;
	ref array<ref ExpansionServerInfoSection> ServerSections;
	
	string ServerName;
	
	// ------------------------------------------------------------
	// ExpansionServerInfos Constructor
	// ------------------------------------------------------------
	void ExpansionServerInfos(array<ref ExpansionServerInfoButtonData> buttons, array<ref ExpansionServerInfoSection> sections, string name)
	{
		this.ServerButtons = buttons;
		this.ServerSections = sections;
		this.ServerName = name;
	}
};