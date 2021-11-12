/**
 * ExpansionKillFeedMessageType.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionKillFeedMessageType
{
	UNKNOWN,

	FALL,
	CAR_HIT_DRIVER,
	CAR_HIT_NODRIVER,
	HELI_HIT_DRIVER,
	HELI_HIT_NODRIVER,
	CAR_CRASH,
	CAR_CRASH_CREW,
	HELI_CRASH,
	HELI_CRASH_CREW,
	BOAT_CRASH,
	BOAT_CRASH_CREW,
	FIRE,
	AREA,
	BARBEDWIRE,
	SPECIAL_EXPLOSION,
	WEAPON_EXPLOSION,

	SUICIDE,
	BLEEDING,
	STARVATION,
	DEHYDRATION,

	WEAPON,
	WEAPON_NODISTANCE,
	MELEWEAPON,
	BAREHANDS,
	INFECTED,
	ANIMAL,
	
	KILLED_UNKNOWN,
	DIED_UNKNOWN
}