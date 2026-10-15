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

enum ExpansionVehicleAnimInstancesHelper: VehicleAnimInstances
{
	EXPANSION_START
}

/**
 * @note IMPORTANT: Values must match respective VehicleType in "Vehicles.agr"!
 * @note lowest Expansion anim instance value should be last enum value from vanilla VehicleAnimInstances + 1 (using ExpansionVehicleAnimInstancesHelper.EXPANSION_START guarantees this)
 */
class ExpansionVehicleAnimInstances
{
	// Cars - Expansion - using vanilla anims
	static int EXPANSION_VODNIK = VehicleAnimInstances.SEDAN;
	static int EXPANSION_TRACTOR = VehicleAnimInstances.V3S;
	static int EXPANSION_UAZ = VehicleAnimInstances.SEDAN;

	// Boats - Expansion - using vanilla anims
	static int EXPANSION_ZODIAC = VehicleAnimInstances.ZODIAC;

#ifdef EXPANSIONMODANIMATIONS

	// Cars - Expansion - using Expansion anims
	static int EXPANSION_BUS = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 1;

	// Boats
	static int EXPANSION_UTILITY_BOAT = ExpansionVehicleAnimInstancesHelper.EXPANSION_START;
	static int EXPANSION_LHD = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 5;

	// Bikes
	static int EXPANSION_TT650 = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 3;
	static int EXPANSION_N250 = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 4;
	static int EXPANSION_OLDBIKE = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 6;

	// Helicopters
	static int EXPANSION_MH6 = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 2;
	static int EXPANSION_MERLIN = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 5;
	static int EXPANSION_UH1H = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 2;
	static int EXPANSION_GYROCOPTER = ExpansionVehicleAnimInstancesHelper.EXPANSION_START + 5;

#else

	// Cars
	static int EXPANSION_BUS = VehicleAnimInstances.SEDAN;

	// Boats
	static int EXPANSION_UTILITY_BOAT = VehicleAnimInstances.ZODIAC;
	static int EXPANSION_LHD = VehicleAnimInstances.SEDAN;

	// Bikes
	static int EXPANSION_TT650 = VehicleAnimInstances.SEDAN;
	static int EXPANSION_N250 = VehicleAnimInstances.SEDAN;
	static int EXPANSION_OLDBIKE = VehicleAnimInstances.SEDAN;

	// Helicopters
	static int EXPANSION_MH6 = VehicleAnimInstances.SEDAN;
	static int EXPANSION_MERLIN = VehicleAnimInstances.SEDAN;
	static int EXPANSION_UH1H = VehicleAnimInstances.SEDAN;
	static int EXPANSION_GYROCOPTER = VehicleAnimInstances.SEDAN;

#endif
};
