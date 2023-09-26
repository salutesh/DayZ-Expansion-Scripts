#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DZ_Gear_Containers","ns_dayz_gear_lehs","ns_dayz_gear_head","DayZExpansion_Core_Objects_CircuitBoard"};
	};
};
class CfgVehicles
{
	class SmallProtectorCase;
	class Clothing;
	class dzn_lehs: Clothing
	{
		attachments[] += {"Att_ExpansionCircuitBoard"};
	};
	class ExpansionAnomalyCoreProtectiveCase: SmallProtectorCase
	{
		displayName = "EVR Protection Case";
		descriptionShort = "This high-tech reinforced, weather-resistant container was developed by a team of scientists to withstand even the harshest conditions on the island. This case is the ultimate solution for protecting your valuable loot during EVR storms.";
		itemSize[] = {3,4};
		storageCategory = 10;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Gear\data\expansion_anomalycase_gray_co.paa"};
	};
};
