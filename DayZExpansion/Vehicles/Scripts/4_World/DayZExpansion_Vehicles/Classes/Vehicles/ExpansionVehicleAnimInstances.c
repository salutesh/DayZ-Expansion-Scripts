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

#else

	// Cars
	static int EXPANSION_BUS = VehicleAnimInstances.HMMWV;

#ifdef SURVIVOR_ANIMS
	// Boats
	static int EXPANSION_UTILITY_BOAT = VehicleAnimInstances.HMMWV;
	static int EXPANSION_ZODIAC = CustomVehicleAnimInstances.HELICOPTER;
	static int EXPANSION_LHD = CustomVehicleAnimInstances.HELICOPTER;

	// Bikes
	static int EXPANSION_TT650 = CustomVehicleAnimInstances.MOTORCYCLE;
	static int EXPANSION_N250 = CustomVehicleAnimInstances.MOTORCYCLE;
	static int EXPANSION_OLDBIKE = CustomVehicleAnimInstances.MOTORCYCLE;

	// Helicopters
	static int EXPANSION_MH6 = CustomVehicleAnimInstances.HELICOPTER;
	static int EXPANSION_MERLIN = CustomVehicleAnimInstances.HELICOPTER;
	static int EXPANSION_UH1H = CustomVehicleAnimInstances.HELICOPTER;
	static int EXPANSION_GYROCOPTER = CustomVehicleAnimInstances.HELICOPTER;
#else
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

#endif
};
