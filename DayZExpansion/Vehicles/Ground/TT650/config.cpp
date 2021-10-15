#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_TT650
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled"};
	};
};
class CfgSlots
{
	class Slot_expansion_tt650_wheel_1
	{
		name = "expansion_tt650_wheel_1";
		displayName = "$STR_EXPANSION_VEHICLE_TT650_WHEEL";
		selection = "wheel_1";
		ghostIcon = "wheel";
	};
	class Slot_expansion_tt650_wheel_2
	{
		name = "expansion_tt650_wheel_2";
		displayName = "$STR_EXPANSION_VEHICLE_TT650_WHEEL";
		selection = "wheel_2";
		ghostIcon = "wheel";
	};
};
