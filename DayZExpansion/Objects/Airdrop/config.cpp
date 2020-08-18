#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Airdrop
	{
		units[] = {"ExpansionAirdropContainer","ExpansionAirdropContainer_Medical","ExpansionAirdropContainer_Military","ExpansionAirdropContainer_Basebuilding","ExpansionAirdropPlane"};
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
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class ExpansionAirdropContainer: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AIRDROP";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_co.paa"};
	};
	class ExpansionAirdropContainer_Medical: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MEDICAL_AIRDROP";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_med_co.paa"};
	};
	class ExpansionAirdropContainer_Military: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MILITARY_AIRDROP";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Objects\Airdrop\data\Airdrop_mil_co.paa"};
	};
	class ExpansionAirdropContainer_Basebuilding: ExpansionAirdropContainerBase
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BASEBUILDING_AIRDROP";
		hiddenSelections[] = {"body"};
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
