#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_250N
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Vehicles_Parts","DZ_Sounds_Effects"};
	};
};
class CfgSlots
{
	class Slot_Expansion_250N_wheel_1
	{
		name = "expansion_250N_wheel_1";
		displayName = "$STR_EXPANSION_VEHICLE_250N_WHEEL";
		selection = "wheel_1";
		ghostIcon = "wheel";
	};
	class Slot_Expansion_250N_wheel_2
	{
		name = "expansion_250N_wheel_2";
		displayName = "$STR_EXPANSION_VEHICLE_250N_WHEEL";
		selection = "wheel_2";
		ghostIcon = "wheel";
	};
};
