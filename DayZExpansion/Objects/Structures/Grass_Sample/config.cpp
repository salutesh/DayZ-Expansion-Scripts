#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Grass_Sample
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
	class Expansion_Grass_color1: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color1.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_color2: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color2.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_color3: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Expansion_Grass_color3.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_Sample_Cluter_1: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_1.p3d";
		vehicleClass = "Expansion_Static";
	};
	class Expansion_Grass_Sample_Cluter_2: HouseNoDestruct
	{
		scope = 1;
		shadow = 0;
		model = "\DayZExpansion\Objects\Structures\Grass_Sample\Grass_Sample_Cluter_2.p3d";
		vehicleClass = "Expansion_Static";
	};
};
