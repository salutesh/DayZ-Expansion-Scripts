#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Safes
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionSafeLarge","ExpansionSafeMini","ExpansionSafeMedium","ExpansionSafeMediumPlacing"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionSafeBase: Container_Base
	{
		scope = 0;
		displayname = "$STR_EXPANSION_SAFE";
		vehicleClass = "Expansion_Construction";
		expansionIsOpenable = 1;
		overrideDrawArea = "8.0";
		carveNavmesh = 1;
		heavyItem = 1;
		weight = 10000;
		itemSize[] = {10,10};
		repairableWithKits[] = {5,8};
		repairCosts[] = {30.0,25.0};
		lootCategory = "Crafted";
		lootTag[] = {"Work"};
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		allowOwnedCargoManipulation = 1;
		class AnimationSources
		{
			class safe_door
			{
				source = "user";
				animPeriod = 0.4;
				initPhase = 0;
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,10};
			allowOwnedCargoManipulation = 1;
			cargoRecievesDamage = 0;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
		attachments[] = {};
		class GUIInventoryAttachmentsProps{};
	};
	class ExpansionSafeLarge: ExpansionSafeBase
	{
		scope = 2;
		displayname = "$STR_EXPANSION_LARGE_SAFE";
		descriptionShort = "$STR_EXPANSION_SAFE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_01\Safe_01.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\basebuilding\safes\safe_01\data\safe_01_co.paa"};
		hologramMaterial = "";
		hologramMaterialPath = "";
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			allowOwnedCargoManipulation = 1;
			cargoRecievesDamage = 0;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 20000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
	};
	class ExpansionSafeMini: ExpansionSafeBase
	{
		scope = 2;
		displayname = "$STR_EXPANSION_MINI_SAFE";
		descriptionShort = "$STR_EXPANSION_SAFE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_02\Safe_02.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\basebuilding\safes\safe_02\data\safe_ca.paa"};
		hologramMaterial = "";
		hologramMaterialPath = "";
		class Cargo
		{
			itemsCargoSize[] = {10,10};
			allowOwnedCargoManipulation = 1;
			cargoRecievesDamage = 0;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
	};
	class ExpansionSafeMedium: ExpansionSafeBase
	{
		scope = 2;
		displayname = "$STR_EXPANSION_MEDIUM_SAFE";
		descriptionShort = "$STR_EXPANSION_SAFE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Safes\Safe_03\Safe_03.p3d";
		class Cargo
		{
			itemsCargoSize[] = {10,25};
			cargoRecievesDamage = 0;
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
		};
	};
	class ExpansionSafeMediumPlacing: ExpansionSafeMedium
	{
		hiddenSelections[] = {"safe","safe_door"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)","#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)"};
	};
};
