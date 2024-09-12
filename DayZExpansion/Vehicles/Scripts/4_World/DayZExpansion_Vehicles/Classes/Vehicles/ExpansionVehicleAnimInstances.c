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
 * @note IMPORTANT: Values must match respective VehicleType in "Vehicles.agr"!
 * @note lowest Expansion anim instance value should be last enum value from vanilla VehicleAnimInstances + 1
 */
class ExpansionVehicleAnimInstances
{
	// Cars - Expansion - using vanilla anims
	static int EXPANSION_VODNIK = VehicleAnimInstances.SEDAN;
	static int EXPANSION_TRACTOR = VehicleAnimInstances.V3S;
	static int EXPANSION_UAZ = VehicleAnimInstances.SEDAN;

#ifdef EXPANSIONMODANIMATIONS

	// Cars - Expansion - using Expansion anims
	static int EXPANSION_BUS = 11;

	// Boats
	static int EXPANSION_UTILITY_BOAT = 10;
	static int EXPANSION_ZODIAC = 12;
	static int EXPANSION_LHD = 12;

	// Bikes
	static int EXPANSION_TT650 = 13;
	static int EXPANSION_N250 = 14;
	static int EXPANSION_OLDBIKE = 16;

	// Helicopters
	static int EXPANSION_MH6 = 12;
	static int EXPANSION_MERLIN = 15;
	static int EXPANSION_UH1H = 12;
	static int EXPANSION_GYROCOPTER = 12;

#else

	// Cars
	static int EXPANSION_BUS = VehicleAnimInstances.HMMWV;

	// Boats
	static int EXPANSION_UTILITY_BOAT = VehicleAnimInstances.SEDAN;
	static int EXPANSION_ZODIAC = VehicleAnimInstances.SEDAN;
	static int EXPANSION_LHD = VehicleAnimInstances.SEDAN;

	// Bikes
	static int EXPANSION_TT650 = VehicleAnimInstances.SEDAN;
	static int EXPANSION_N250 = VehicleAnimInstances.SEDAN;
	static int EXPANSION_OLDBIKE = VehicleAnimInstances.SEDAN;

	// Helicopters
	static int EXPANSION_MH6 = VehicleAnimInstances.HMMWV;
	static int EXPANSION_MERLIN = VehicleAnimInstances.SEDAN;
	static int EXPANSION_UH1H = VehicleAnimInstances.HMMWV;
	static int EXPANSION_GYROCOPTER = VehicleAnimInstances.SEDAN;

#endif
};
