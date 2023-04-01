#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","ns_dayz_gear_lehs","DayZExpansion_Core_Objects_CircuitBoard"};
	};
};
class CfgVehicles
{
	class Clothing;
	class dzn_lehs: Clothing
	{
		attachments[] += {"Att_ExpansionCircuitBoard"};
	};
};
