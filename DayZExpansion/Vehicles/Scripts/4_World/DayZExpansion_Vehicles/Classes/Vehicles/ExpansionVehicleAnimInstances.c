/**
 * ExpansionVehicleAnimInstances.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * @note IMPORTANT: Must match VehicleType in "DayZExpansion\Animations\Player\Vehicles.agr"
 */
class ExpansionVehicleAnimInstances
{
	// Cars - vanilla (must match vanilla VehicleAnimInstances)
	static int EX_CIVVAN = 0;
	static int EX_V3S = 1;
	static int EX_SEDAN = 2;
	static int EX_HATCHBACK = 3;
	static int EX_BUS = 4;
	static int EX_S120 = 5;
	static int EX_MULTICAR = 6;
	static int EX_GOLF = 7;
	static int EX_HMMWV = 8;

	// Cars - Expansion - using vanilla anims
	static int EXPANSION_VODNIK = 0;
	static int EXPANSION_TRACTOR = 1;
	static int EXPANSION_UAZ = 2;

	// Cars - Expansion - using Expansion anims
	static int EXPANSION_BUS = 10;

	// Boats
	static int EXPANSION_UTILITY_BOAT = 9;
	static int EXPANSION_ZODIAC = 11;
	static int EXPANSION_LHD = 11;

	// Bikes
	static int EXPANSION_TT650 = 12;
	static int EXPANSION_N250 = 13;
	static int EXPANSION_OLDBIKE = 15;

	// Helicopters
	static int EXPANSION_MH6 = 11;
	static int EXPANSION_MERLIN = 14;
	static int EXPANSION_UH1H = 11;
	static int EXPANSION_GYROCOPTER = 11;
};
