#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Camping
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Gear_Camping"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class BatteryCharger: Inventory_Base
	{
		attachments[] = {"CarBattery","TruckBattery","ExpansionHelicopterBattery","ExpansionAircraftBattery"};
	};
};
