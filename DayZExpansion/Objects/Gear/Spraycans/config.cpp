class CfgPatches
{
	class DayZExpansion_Objects_Gear_Spraycans
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
	class Inventory_Base;
	class ExpansionSpraycanBase: Inventory_Base
	{
		scope=0;
		displayName="$STR_EXPANSION_SPRAYCAN_BASE_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_BASE_DESC";
		model="\dz\gear\consumables\spraycan.p3d";
		weight=100;
		itemSize[]={2,3};
		stackedUnit="ml";
		varQuantityInit=150;
		varQuantityMin=0;
		varQuantityMax=150;
		quantityBar=1;
		lootTag[]=
		{
			"Work"
		};
		lootCategory="Materials";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_black_co.paa"
		};
	};
};
