#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Lighthouse_Lamp
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Lighthouse_Lamp: HouseNoDestruct
	{
		scope = 1;
		model = "DayZExpansion\Objects\Structures\LightHouse_Lamp\Expansion_Lighthouse_Lamp.p3d";
	};
};
