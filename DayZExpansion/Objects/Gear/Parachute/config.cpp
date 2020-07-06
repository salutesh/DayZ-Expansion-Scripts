#define _ARMA_

class CfgPatches
{
	class Expasnsion_Parachute
	{
		units[] = {"ExpansionParachute"};
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
		scope = 2;
		displayName = "$STR_EXPANSION_PARACHUTE";
		descriptionShort = "$STR_EXPANSION_PARACHUTE_DESC";
		model = "\DayZExpansion\Objects\Gear\Parachute\ExpansionParachute.p3d";
		rotationFlags = 1;
		inventorySlot[] = {"expansionparachute","expansionparachutepacked"};
		weight = 100;
	};
};
