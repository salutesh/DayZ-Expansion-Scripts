#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionGrass10: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Structures\Grass\Grass10M.p3d";
		vehicleClass = "Expansion_Static";
	};
};
