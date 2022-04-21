/**
 * ExpansionTracing.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTracing
{
	//! Please keep these at the top and use below instead of true to enable tracing when using DayZDiag or DayZ Experimental
#ifdef DIAG
	static bool ENABLE = true;
#else
	#ifdef BUILD_EXPERIMENTAL
		static bool ENABLE = true;
	#else
		static bool ENABLE = false;
	#endif
#endif

	//! Please keep the rest alphabetically ordered when inserting new variables

	static bool AI;

	static bool BASEBUILDING;

	static bool BOOK;
	
	static bool CE;

	static bool CHAT = ENABLE;

	static bool CHICKEN;
	
	static bool COT_GROUPS = ENABLE;
	
	static bool DATACOLLECTION = ENABLE;

	static bool GENERAL_ITEMS;

	static bool GLOBAL;

	static bool LIGHTHOUSE;

	static bool LIGHTS;

	static bool MAPPING;
	
	static bool MARKER;

	static bool MARKET;

	static bool MISSIONS;

	static bool NOTIFICATIONS;

	static bool PLAYER;

	static bool PLAYER_MONITOR;
	
	static bool RESPAWN = ENABLE;

	static bool SETTINGS;

	static bool SKIN;

	static bool TERRITORY;

	static bool UI;

	static bool VEHICLES = ENABLE;

	static bool WEAPONS;

	static bool ZONES;
};
