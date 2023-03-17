#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Quests_Teleporter
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgSlots{};
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Teleporter_Pad: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\Teleporter_Pad.p3d";
	};
	class Expansion_Teleporter_Big: HouseNoDestruct
	{
		scope = 1;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\teleporter.p3d";
		class Doors
		{
			class Doors1
			{
				displayName = "door 1";
				component = "doors1";
				soundPos = "doors1_action";
				animPeriod = 3;
				initPhase = 0.0;
				initOpened = 1.0;
				soundOpen = "doorMetalSmallOpen";
				soundClose = "doorMetalSmallClose";
				soundLocked = "doorMetalSmallRattle";
				soundOpenABit = "doorMetalSmallOpenABit";
			};
		};
	};
};
