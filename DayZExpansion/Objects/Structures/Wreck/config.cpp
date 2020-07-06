#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_IvyAutomn
	{
		units[] = {"Expansion_Wreck_Volha_Police"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Wreck_Volha_Police: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 2;
		model = "\DZ\structures\wrecks\Vehicles\Wreck_Volha_Police.p3d";
	};
};
