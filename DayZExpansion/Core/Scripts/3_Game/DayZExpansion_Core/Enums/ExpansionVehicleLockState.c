/**
 * ExpansionVehicleLockState.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleLockState
{
	NOLOCK = 0,
	UNLOCKED,
	READY_TO_LOCK,
	LOCKED,
	READY_TO_FORCELOCK,
	FORCEDLOCKED,
	FORCEDUNLOCKED,
	//Use count here, for network optimization to know max
	COUNT
};
