#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Truck_01
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Vehicles_Data"};
	};
};
class CfgVehicles
{
	class Truck_01_Base;
	class Truck_01_Covered: Truck_01_Base{};
};
