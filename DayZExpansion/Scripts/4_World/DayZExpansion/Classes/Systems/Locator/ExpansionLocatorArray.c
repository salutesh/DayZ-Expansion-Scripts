/**
 * ExpansionLocatorArray.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/


/**@class		ExpansionLocatorArray
 * @brief		This class handle expansion locator array
 **/
class ExpansionLocatorArray
{
	vector position;
	string name;
	string type;
	
	void ExpansionLocatorArray( vector pos, string Name, string Type ) 
	{
		this.position = pos;
		this.name = Name;
		this.type = Type;
	}
}