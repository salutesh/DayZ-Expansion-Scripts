/**
 * ExpansionBuildNoBuildZone.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBuildNoBuildZone
{
	string Name;
	vector Center;
	float Radius;
	ref TStringArray Items;
	bool IsWhitelist;
	string CustomMessage;  //! Custom message for this zone if conditions are not met (optional, uses default message if not set)

	void ExpansionBuildNoBuildZone( string name, vector center, float radius, TStringArray items, bool is_whitelist = true, string message = "" )
	{
		Name = name;
		Center = center;
		Radius = radius;
		Items = items;
		IsWhitelist = is_whitelist;
		CustomMessage = message;
	}
}
