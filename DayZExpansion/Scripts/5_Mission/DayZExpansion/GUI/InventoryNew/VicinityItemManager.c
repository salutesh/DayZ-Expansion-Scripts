/**
 * VicinityItemManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class VicinityItemManager
{
	override bool IsObstructed (Object filtered_object)
	{
			if ( filtered_object.IsInherited(ExpansionBaseBuilding) )
			{
				return false;
			}
			return super.IsObstructed(filtered_object);
	}
};