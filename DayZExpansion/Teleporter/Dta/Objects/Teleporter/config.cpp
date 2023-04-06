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
	class Expansion_Teleporter_Pad: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\Teleporter_Pad.p3d";
	};
	class Expansion_Teleporter_Big: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Teleporter\Dta\Objects\Teleporter\teleporter.p3d";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		openable = 1;
		carveNavmesh = 1;
		itemBehaviour = 2;
		attachments[] = {"CarBattery","Att_ExpansionCircuitBoard"};
		rotationFlags = 2;
		soundImpactType = "metal";
		oldpower = 0;
		ChargeEnergyPerSecond = 1;
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOff = 1;
			energyUsagePerSecond = 0.01;
			plugType = 5;
			attachmentAction = 1;
		};
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
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "$STR_attachment_accessories";
				description = "";
				attachmentSlots[] = {"CarBattery","Att_ExpansionCircuitBoard"};
				icon = "set:expansion_iconset image:icon_options";
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
