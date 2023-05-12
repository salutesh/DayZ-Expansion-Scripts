#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Weapons_Ammunition
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Ammunition"};
	};
};
class CfgMagazines
{
	class Magazine_Base;
	class Ammunition_Base: Magazine_Base
	{
		repairableWithKits[] += {100};
		repairCosts[] += {25};
	};
};
