/**
 * ExpansionIcon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionIcon : Managed
{
	string Name;
	string Localisation;
	string IconPath;

	void ExpansionIcon( string name, string localisation, string path )
	{
		Name = name;
		Localisation = localisation;
		IconPath = path;
	}
};