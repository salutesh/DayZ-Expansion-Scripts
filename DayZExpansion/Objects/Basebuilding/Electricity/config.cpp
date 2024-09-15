class CfgPatches
{
	class DayZExpansion_BaseBuilding_Electricity
	{
		units[]=
		{
			"ExpansionPowerSwitch",
			"ExpansionBaseLightBase"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class cfgvehicles
{
	class Inventory_Base;
	class ExpansionPowerSwitch: Inventory_Base
	{
		scope=2;
		displayName="Power Switch";
		descriptionShort="";
		model="DayZExpansion\Objects\Basebuilding\Electricity\powerswitch.p3d";
		weight=4;
		itemSize[]={3,3};
		stackedUnit="w";
		quantityBar=1;
		varQuantityInit=50;
		varQuantityMin=0;
		varQuantityMax=50;
		varQuantityDestroyOnMin=0;
		rotationFlags=17;
		canBeSplit=0;
		class AnimationSources
		{
			class switch
			{
				source="user";
				animPeriod=0.5;
				initPhase=0;
			};
		};
		class EnergyManager
		{
			hasIcon=1;
			switchOnAtSpawn=1;
			isPassiveDevice=0;
			energyStorageMax=50;
			energyAtSpawn=50;
			convertEnergyToQuantity=1;
			reduceMaxEnergyByDamageCoef=1;
			autoSwitchOffWhenInCargo=1;
			energyUsagePerSecond=0.0099999998;
			plugType=2;
			attachmentAction=0;
			updateInterval=30;
		};
	};
	class ExpansionBaseLightBase: Inventory_Base
	{
		scope=2;
		displayName="Test light";
		descriptionShort="";
		model="DayZExpansion\Objects\Basebuilding\Electricity\TempLight.p3d";
		weight=4;
		itemSize[]={3,3};
		rotationFlags=17;
		canBeSplit=0;
		stackedUnit="l";
	};
};
