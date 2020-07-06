#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Gps
	{
		units[] = {"ExpansionGPS"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionGPS: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ITEM_GPS_NAME";
		descriptionShort = "$STR_EXPANSION_ITEM_GPS_DESC";
		model = "\DayZExpansion\Objects\Gear\Gps\GPS.p3d";
		rotationFlags = 34;
		itemSize[] = {2,2};
		weight = 110;
		animClass = "Pistol";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeLightBlunt";
				range = 1;
			};
			class Heavy
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 1;
			};
			class Sprint
			{
				ammo = "MeleeLightBlunt_Heavy";
				range = 2.8;
			};
		};
		attachments[] = {"BatteryD"};
		repairableWithKits[] = {5,7};
		repairCosts[] = {30,25};
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOffWhenInCargo = 0;
			energyUsagePerSecond = 0.0046296297;
			plugType = 1;
			attachmentAction = 1;
			updateInterval = 30;
		};
	};
};
