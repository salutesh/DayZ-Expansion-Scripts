#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Parachute
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters","DZ_Gear_Optics"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionParachute: Inventory_Base
	{
		scope = 0;
		displayName = "DOESN'T WORK - DO NOT USE";
		descriptionShort = "DOESN'T WORK - DO NOT USE";
		model = "\DayZExpansion\Objects\Gear\Parachute\ExpansionParachute.p3d";
		rotationFlags = 1;
		inventorySlot[] = {"expansionparachute","expansionparachutepacked"};
		weight = 100;
	};
};
