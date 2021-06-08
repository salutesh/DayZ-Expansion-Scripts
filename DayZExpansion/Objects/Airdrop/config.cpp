#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Airdrop
	{
		units[] = {"ExpansionAirdropContainer","ExpansionAirdropContainer_Grey","ExpansionAirdropContainer_Blue","ExpansionAirdropContainer_Olive","ExpansionAirdropContainer_Medical","ExpansionAirdropContainer_Military","ExpansionAirdropContainer_Military_GreenCamo","ExpansionAirdropContainer_Military_MarineCamo","ExpansionAirdropContainer_Military_OliveCamo","ExpansionAirdropContainer_Military_OliveCamo2","ExpansionAirdropContainer_Military_WinterCamo","ExpansionAirdropContainer_Basebuilding","ExpansionAirdropContainer_Big","ExpansionAirdropContainer_Grey_Big","ExpansionAirdropContainer_Blue_Big","ExpansionAirdropContainer_Olive_Big","ExpansionAirdropContainer_Medical_Big","ExpansionAirdropContainer_Military_Big","ExpansionAirdropContainer_Military_GreenCamo_Big","ExpansionAirdropContainer_Military_MarineCamo_Big","ExpansionAirdropContainer_Military_OliveCamo_Big","ExpansionAirdropContainer_Military_OliveCamo2_Big","ExpansionAirdropContainer_Military_WinterCamo_Big","ExpansionAirdropContainer_Basebuilding_Big","ExpansionAirdropPlane"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Container_Base;
	class ExpansionAirdropContainerBase: Container_Base
	{
		scope = 0;
		vehicleClass = "Expansion_Airdrop";
		displayName = "$STR_EXPANSION_AIRDROP";
		model = "\DayZExpansion\Objects\airdrop\container.p3d";
		class GUIInventoryAttachmentsProps{};
		forceFarBubble = "true";
		carveNavmesh = 1;
		itemsCargoSize[] = {10,100};
		rotationFlags = 2;
		itemSize[] = {20,20};
		weight = 1000000;
		physLayer = "item_large";
		cargoClass = "";
		inventoryCondition = "true";
		storageCategory = 1;
		openable = 0;
		lootCategory = "Containers";
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
		class Cargo
		{
			itemsCargoSize[] = {10,100};
		};
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class ExpansionAirdropContainer: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class ExpansionAirdropContainer_Grey: ExpansionAirdropContainer
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_grey_co.paa"};
	};
	class ExpansionAirdropContainer_Blue: ExpansionAirdropContainer
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_blue_co.paa"};
	};
	class ExpansionAirdropContainer_Olive: ExpansionAirdropContainer
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_olive_co.paa"};
	};
	class ExpansionAirdropContainer_Medical: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MEDICAL_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_med_co.paa"};
	};
	class ExpansionAirdropContainer_Military: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MILITARY_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_co.paa"};
	};
	class ExpansionAirdropContainer_Military_GreenCamo: ExpansionAirdropContainer_Military
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_green_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_MarineCamo: ExpansionAirdropContainer_Military
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_marine_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_OliveCamo: ExpansionAirdropContainer_Military
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_olive_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_OliveCamo2: ExpansionAirdropContainer_Military
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_olive_camo2_co.paa"};
	};
	class ExpansionAirdropContainer_Military_WinterCamo: ExpansionAirdropContainer_Military
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_winter_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Basebuilding: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BASEBUILDING_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_basebuilding_co.paa"};
	};
	class ExpansionAirdropContainerBigBase: ExpansionAirdropContainerBase
	{
		class Cargo
		{
			itemsCargoSize[] = {20,100};
		};
	};
	class ExpansionAirdropContainer_Big: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class ExpansionAirdropContainer_Grey_Big: ExpansionAirdropContainer_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_grey_co.paa"};
	};
	class ExpansionAirdropContainer_Blue_Big: ExpansionAirdropContainer_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_blue_co.paa"};
	};
	class ExpansionAirdropContainer_Olive_Big: ExpansionAirdropContainer_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_olive_co.paa"};
	};
	class ExpansionAirdropContainer_Medical_Big: ExpansionAirdropContainerBigBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MEDICAL_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_med_co.paa"};
	};
	class ExpansionAirdropContainer_Military_Big: ExpansionAirdropContainerBigBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MILITARY_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_co.paa"};
	};
	class ExpansionAirdropContainer_Military_GreenCamo_Big: ExpansionAirdropContainer_Military_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_green_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_MarineCamo_Big: ExpansionAirdropContainer_Military_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_marine_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_OliveCamo_Big: ExpansionAirdropContainer_Military_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_olive_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Military_OliveCamo2_Big: ExpansionAirdropContainer_Military_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_olive_camo2_co.paa"};
	};
	class ExpansionAirdropContainer_Military_WinterCamo_Big: ExpansionAirdropContainer_Military_Big
	{
		scope = 2;
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_winter_camo_co.paa"};
	};
	class ExpansionAirdropContainer_Basebuilding_Big: ExpansionAirdropContainerBigBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BASEBUILDING_AIRDROP";
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_basebuilding_co.paa"};
	};
	class ExpansionAirdropParticle: HouseNoDestruct
	{
		vehicleClass = "Expansion_Airdrop";
		scope = 1;
	};
	class ExpansionAirdropPlane: HouseNoDestruct
	{
		vehicleClass = "Expansion_Airdrop";
		scope = 2;
		model = "\DayZExpansion\Objects\Airdrop\AirdropC130J.p3d";
		class AnimationSources
		{
			class gear
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class rotor
			{
				source = "user";
				animPeriod = 0.0009999999;
				initPhase = 0;
			};
		};
	};
};
