/**
 * ExpansionMapMarkerType.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionMapMarkerType
{
	//! if a type is removed, just rename it to "REMOVED", if a type is added, append to the end
	UNKNOWN		= 0,
	PERSONAL	= 1,
	PARTY		= 2,
	SERVER		= 3,
	PLAYER		= 4,
	PARTY_QUICK	= 5
};