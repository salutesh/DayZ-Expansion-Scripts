#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Core_NavmeshCarvers
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionNavmeshCarver: HouseNoDestruct
	{
		scope = 0;
		carveNavmesh = 1;
		physLayer = "item_large";
	};
	class ExpansionLargeAnimalNavmeshCarver: ExpansionNavmeshCarver
	{
		scope = 1;
		model = "DayZExpansion\Core\Objects\NavmeshCarvers\AnimalNavmeshCarver.p3d";
	};
};
