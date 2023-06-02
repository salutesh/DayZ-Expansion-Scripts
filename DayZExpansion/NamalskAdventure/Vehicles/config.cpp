#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Vehicles
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DayZExpansion_Vehicles_Data","DayZExpansion_Vehicles_Ground_Truck_01"};
	};
};
class CfgVehicles
{
	class Truck_01_Base;
	class Truck_01_Covered: Truck_01_Base
	{
		doors[] += {"Truck_01_Hood"};
	};
};
