#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass_Sample
	{
		units[] = {"ExpansionGrass_Sample_Test","ExpansionGrass_Sample_Cluter_1","ExpansionGrass_Sample_Cluter_2"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionGrass_Sample_Test: HouseNoDestruct
	{
		scope = 2;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\c_samplegrass.p3d";
		vehicleClass = "Expansion_Static";
	};
	class ExpansionGrass_Sample_Cluter_1: HouseNoDestruct
	{
		scope = 2;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_1.p3d";
		vehicleClass = "Expansion_Static";
	};
	class ExpansionGrass_Sample_Cluter_2: HouseNoDestruct
	{
		scope = 2;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_2.p3d";
		vehicleClass = "Expansion_Static";
	};
};
