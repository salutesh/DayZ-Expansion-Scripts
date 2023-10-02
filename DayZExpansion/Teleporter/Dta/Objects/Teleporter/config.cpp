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
class CfgVehicles
{
	class HouseNoDestruct;
	class Expansion_Teleporter_Base: HouseNoDestruct
	{
		scope = 1;
	};
	class Expansion_Teleporter_Pad: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\Teleporter_Pad.p3d";
	};
	class Expansion_Teleporter_Big: Expansion_Teleporter_Base
	{
		scope = 2;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\teleporter.p3d";
		displayName = "Teleporter MK.1";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		class AnimationSources
		{
			class body
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class carbattery
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class battery_installed
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
		};
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
