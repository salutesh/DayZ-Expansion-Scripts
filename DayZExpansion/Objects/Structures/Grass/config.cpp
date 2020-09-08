#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass
	{
		units[] = {"ExpansionGrass10"};
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
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Grass\Grass10M.p3d";
		vehicleClass = "Expansion_Static";
	};
};
