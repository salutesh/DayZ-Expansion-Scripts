class CfgPatches
{
	class Expansion_Lasers_ANPEQ15
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Switchable_Base;
	class Expansion_ANPEQ15_Base: Switchable_Base
	{
		scope=0;
		displayName="$STR_EXPANSION_ANPEQ15";
		descriptionShort="$STR_EXPANSION_ANPEQ15_DESC";
		model="DayZExpansion\Objects\Weapons\Attachments\Lasers\ANPEQ\peq.p3d";
		rotationFlags=17;
		reversed=1;
		weight=80;
		itemSize[]={2,1};
		inventorySlot[]=
		{
			"weaponFlashlight"
		};
		attachments[]=
		{
			"BatteryD"
		};
		repairableWithKits[]={5,7};
		repairCosts[]={30,25};
		class EnergyManager
		{
			hasIcon=1;
			autoSwitchOffWhenInCargo=1;
			energyUsagePerSecond=0.0099999998;
			plugType=1;
			attachmentAction=1;
			updateInterval=30;
		};
	};
	class Expansion_ANPEQ15_Red: Expansion_ANPEQ15_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_ANPEQ15";
		descriptionShort="$STR_EXPANSION_ANPEQ15_DESC";
	};
	class Expansion_ANPEQ15_Green: Expansion_ANPEQ15_Base
	{
		scope=2;
		displayName="$STR_EXPANSION_ANPEQ15";
		descriptionShort="$STR_EXPANSION_ANPEQ15_DESC";
	};
	class Expansion_ANPEQ15_GreenNV: Expansion_ANPEQ15_Green
	{
	};
	class Expansion_ANPEQ15_IR: Expansion_ANPEQ15_Base
	{
		scope=0;
		displayName="ANPEQ15 IR";
		descriptionShort="$STR_EXPANSION_ANPEQ15_DESC";
	};
};
