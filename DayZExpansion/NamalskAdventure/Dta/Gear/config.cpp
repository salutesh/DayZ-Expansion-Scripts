#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DZ_Gear_Containers","ns_dayz_gear_lehs","ns_dayz_gear_tools","DayZExpansion_Core_Objects_CircuitBoard"};
	};
};
class CfgVehicles
{
	class Clothing;
	class Container_Base;
	class SmallProtectorCase;
	class dzn_module_card;
	class GUIInventoryAttachmentsProps;
	class Inventory_Base;
	class dzn_apsi: Clothing
	{
		attachments[] += {"Att_ExpansionCircuitBoard"};
	};
	class dzn_lehs: Clothing
	{
		attachments[] += {"Att_ExpansionCircuitBoard"};
	};
	class ExpansionAnomalyCoreProtectiveCase: SmallProtectorCase
	{
		displayName = "EVR Protection Case";
		descriptionShort = "This high-tech reinforced, weather-resistant container was developed by a team of scientists to withstand even the harshest conditions on the island. This case is the ultimate solution for protecting your valuable loot during EVA storms.";
		itemSize[] = {3,4};
		storageCategory = 10;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Gear\data\expansion_anomalycase_gray_co.paa"};
	};
	class ExpansionTeleportModuleCard: dzn_module_card
	{
		scope = 2;
		displayName = "Teleport Module Card";
		descriptionShort = "PLACEHOLDER";
		itemSize[] = {1,1};
		weight = 30;
		rotationFlags = 1;
		model = "nst\ns_dayz\gear\tools\module_card.p3d";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Gear\data\teleport_modules_card_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\NamalskAdventure\Dta\Gear\data\teleport_modules_card.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{"\DayZExpansion\NamalskAdventure\Dta\Gear\data\teleport_modules_card.rvmat"}},{0.7,{"\DayZExpansion\NamalskAdventure\Dta\Gear\data\teleport_modules_card.rvmat"}},{0.5,{"nst\ns_dayz\gear\tools\data\modules_card_damage.rvmat"}},{0.3,{"nst\ns_dayz\gear\tools\data\modules_card_damage.rvmat"}},{0.0,{"nst\ns_dayz\gear\tools\data\modules_card_destruct.rvmat"}}};
				};
			};
		};
	};
};
