#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_HangedBody
	{
		units[] = {"Expansion_hanged_new","Expansion_hanged_green"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Container_Base;
	class Expansion_hanged_new: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\hanged_body\hanged_new.p3d";
	};
	class Expansion_hanged_green: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\hanged_body\hanged_green.p3d";
	};
	class Expansion_tireSwing: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DayZExpansion\Objects\Structures\hanged_body\Expansion_tireSwing.p3d";
	};
	class Expansion_HangedSkull: HouseNoDestruct
	{
		vehicleClass = "Expansion_Static";
		scope = 1;
		model = "\DayZExpansion\Objects\Structures\hanged_body\Expansion_HangedSkull.p3d";
	};
};
