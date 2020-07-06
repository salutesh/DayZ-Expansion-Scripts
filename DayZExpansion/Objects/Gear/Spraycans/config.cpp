#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Consumables
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionSpraycanBase: Inventory_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_SPRAYCAN_BASE_NAME";
		descriptionShort = "$STR_EXPANSION_SPRAYCAN_BASE_DESCRIPTION";
		model = "\dz\gear\consumables\spraycan.p3d";
		weight = 100;
		itemSize[] = {2,3};
		stackedUnit = "ml";
		varQuantityInit = 100.0;
		varQuantityMin = 0.0;
		varQuantityMax = 100.0;
		quantityBar = 1;
		lootTag[] = {"Work"};
		lootCategory = "Materials";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DZ\gear\consumables\data\spraycan_black_co.paa"};
	};
	class ExpansionSpraycanGreen;
	class ExpansionSpraycanBlueline;
	class ExpansionSpraycanDigital;
	class ExpansionSpraycanEliptical;
	class ExpansionSpraycanFurious;
	class ExpansionSpraycanGrayWatcher;
	class ExpansionSpraycanJeans;
	class ExpansionSpraycanResistance;
	class ExpansionSpraycanShadow;
	class ExpansionSpraycanSpeedy;
	class ExpansionSpraycanSunset;
	class ExpansionSpraycanVrana;
	class ExpansionSpraycanWasp;
	class ExpansionSpraycanWave;
	class ExpansionSpraycanMedical;
	class ExpansionSpraycanUber;
	class ExpansionSpraycanDahoman;
	class ExpansionSpraycanIon;
	class ExpansionSpraycanPresident;
	class ExpansionSpraycanBlue;
	class ExpansionSpraycanWeeb;
	class ExpansionSpraycanMh6Default: ExpansionSpraycanGreen{};
	class ExpansionSpraycanMh6Blueline: ExpansionSpraycanBlueline{};
	class ExpansionSpraycanMh6Digital: ExpansionSpraycanDigital{};
	class ExpansionSpraycanMh6Eliptical: ExpansionSpraycanEliptical{};
	class ExpansionSpraycanMh6Furious: ExpansionSpraycanFurious{};
	class ExpansionSpraycanMh6GrayWatcher: ExpansionSpraycanGrayWatcher{};
	class ExpansionSpraycanMh6Jeans: ExpansionSpraycanJeans{};
	class ExpansionSpraycanMh6Resistance: ExpansionSpraycanResistance{};
	class ExpansionSpraycanMh6Shadow: ExpansionSpraycanShadow{};
	class ExpansionSpraycanMh6Speedy: ExpansionSpraycanSpeedy{};
	class ExpansionSpraycanMh6Sunset: ExpansionSpraycanSunset{};
	class ExpansionSpraycanMh6Vrana: ExpansionSpraycanVrana{};
	class ExpansionSpraycanMh6Wasp: ExpansionSpraycanWasp{};
	class ExpansionSpraycanMh6Wave: ExpansionSpraycanWave{};
	class ExpansionSpraycanUh1hDefault: ExpansionSpraycanGreen{};
	class ExpansionSpraycanUh1hMedical: ExpansionSpraycanMedical{};
	class ExpansionSpraycanUh1hUber: ExpansionSpraycanUber{};
	class ExpansionSpraycanUh1hResistance: ExpansionSpraycanResistance{};
	class ExpansionSpraycanMerlinDefault: ExpansionSpraycanGreen{};
	class ExpansionSpraycanMerlinDahoman: ExpansionSpraycanDahoman{};
	class ExpansionSpraycanMerlinIon: ExpansionSpraycanIon{};
	class ExpansionSpraycanMerlinPresident: ExpansionSpraycanPresident{};
	class ExpansionSpraycanMerlinVrana: ExpansionSpraycanVrana{};
	class ExpansionSpraycanMerlinWave: ExpansionSpraycanWave{};
	class ExpansionSpraycanBusDefault: ExpansionSpraycanBlue{};
	class ExpansionSpraycanBusWeeb: ExpansionSpraycanWeeb{};
};
