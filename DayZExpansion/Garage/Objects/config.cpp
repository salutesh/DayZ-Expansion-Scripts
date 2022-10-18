#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Garage_Objects
	{
		units[] = {"ExpansionGarageVehicle"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionGarageVehicle: Inventory_Base
	{
		scope = 2;
		model = "DayZExpansion\Garage\Objects\GarageVehiclePlaceholder.p3d";
		itemsCargoSize[] = {10,100};
	};
};
