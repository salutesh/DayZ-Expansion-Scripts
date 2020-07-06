#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gear_Box
	{
		units[] = {"ExpansionHeroSet","ExpansionBanditSet"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionHeroSet: Inventory_Base
	{
		scope = 2;
		weight = 50;
		displayName = "$STR_EXPANSION_HERO_SET";
		descriptionShort = "$STR_EXPANSION_HERO_SET_DESC";
		model = "\DayZExpansion\Objects\Gear\Box\box.p3d";
		itemSize[] = {5,5};
	};
	class ExpansionBanditSet: Inventory_Base
	{
		scope = 2;
		weight = 50;
		displayName = "$STR_EXPANSION_BANDIT_SET";
		descriptionShort = "$STR_EXPANSION_BANDIT_SET_DESC";
		model = "\DayZExpansion\Objects\Gear\Box\box.p3d";
		itemSize[] = {5,5};
	};
};
