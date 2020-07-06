#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass
	{
		units[] = {"ExpansionGrassTall","ExpansionGrass3","ExpansionGrass10"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionGrassTall: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Grass\GrassTall.p3d";
		vehicleClass = "Expansion_Static";
	};
	class ExpansionGrass3: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Grass\Grass3M.p3d";
		vehicleClass = "Expansion_Static";
	};
	class ExpansionGrass10: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Grass\Grass10M.p3d";
		vehicleClass = "Expansion_Static";
	};
};
