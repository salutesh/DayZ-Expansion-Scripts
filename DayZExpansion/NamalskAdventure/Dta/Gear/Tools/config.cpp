#define _ARMA_

class CfgPatches
{
	class NamalskAdventure_Gear_Tools
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Consumables","DZ_Gear_Containers","DZ_Gear_Tools"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class AmmoCleaningKit: Inventory_Base
	{
		scope = 2;
		displayName = "Ammunition Cleaning Kit";
		descriptionShort = "An ammunition cleaning kit designed to clean up your loose ammo.";
		model = "\dz\gear\tools\cleaning_kit_wood.p3d";
		debug_ItemCategory = 2;
		animClass = "Knife";
		rotationFlags = 17;
		stackedUnit = "percentage";
		quantityBar = 1;
		varQuantityInit = 100;
		varQuantityMin = 0;
		varQuantityMax = 100;
		weight = 780;
		weightPerQuantityUnit = 0;
		itemSize[] = {2,3};
		fragility = 0.01;
		repairKitType = 100;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"}},{0.7,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"}},{0.5,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_damage.rvmat"}},{0.3,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_damage.rvmat"}},{0,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_destruct.rvmat"}}};
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
		soundImpactType = "wood";
	};
	class MedicalSupplies: Inventory_Base
	{
		scope = 2;
		displayName = "Medical Supplies";
		descriptionShort = "PLACEHOLDER";
		model = "\dz\gear\containers\FirstAidKit.p3d";
		rotationFlags = 17;
		weight = 140;
		itemSize[] = {2,2};
		randomQuantity = 2;
		isMeleeWeapon = 1;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"\dz\gear\containers\data\firstaidkit_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 80;
					healthLevels[] = {{1,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.7,{"DZ\gear\containers\data\FirsAidKit.rvmat"}},{0.5,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0.3,{"DZ\gear\containers\data\FirsAidKit_damage.rvmat"}},{0,{"DZ\gear\containers\data\FirsAidKit_destruct.rvmat"}}};
				};
			};
		};
		soundImpactType = "textile";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem_Light
				{
					soundSet = "pickUpCourierBag_Light_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpCourierBag_SoundSet";
					id = 797;
				};
			};
		};
	};
	class AmmunitionSupplies: Inventory_Base
	{
		scope = 2;
		displayName = "Ammunition Supplies";
		descriptionShort = "PLACEHOLDER";
		model = "\dz\gear\tools\cleaning_kit_wood.p3d";
		rotationFlags = 17;
		weight = 140;
		itemSize[] = {2,2};
		randomQuantity = 2;
		isMeleeWeapon = 1;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_co.paa"};
		hiddenSelectionsMaterials[] = {"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"}},{0.7,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit.rvmat"}},{0.5,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_damage.rvmat"}},{0.3,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_damage.rvmat"}},{0,{"DayZExpansion\NamalskAdventure\Dta\Gear\Tools\data\Ammo_Cleaning_Kit_destruct.rvmat"}}};
				};
			};
		};
		soundImpactType = "wood";
	};
	class Paper;
	class CommunityGoalTicket: Paper
	{
		scope = 2;
		displayName = "Community Goal Ticket";
		descriptionShort = "PLACEHOLDER";
		model = "\dz\gear\consumables\PunchedCard.p3d";
		weight = 10;
		itemSize[] = {1,1};
		destroyOnEmpty = 1;
		canBeSplit = 1;
		isMeleeWeapon = 1;
		varQuantityInit = 1;
		varQuantityMin = 0;
		varQuantityMax = 100;
		varQuantityDestroyOnMin = 1;
		varStackMax = 100;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"\dz\gear\consumables\data\punchedcard_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLevels[] = {{1,{"DZ\gear\consumables\data\PunchedCard.rvmat"}},{0.7,{"DZ\gear\consumables\data\PunchedCard.rvmat"}},{0.5,{"DZ\gear\consumables\data\PunchedCard_damage.rvmat"}},{0.3,{"DZ\gear\consumables\data\PunchedCard_damage.rvmat"}},{0,{"DZ\gear\consumables\data\PunchedCard_destruct.rvmat"}}};
				};
			};
		};
	};
};
