#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Data
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionKitLargePlacing","ExpansionKitSmallPlacing","ExpansionBaseBuilding"};
		weapons[] = {};
	};
};
class CfgExpansionSnapping
{
	class Wall
	{
		type = "WALL";
	};
	class Floor
	{
		type = "FLOOR";
	};
	class Ramp
	{
		type = "RAMP";
	};
	class Pillar
	{
		type = "PILLAR";
	};
	class Stair
	{
		type = "STAIR";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionKitBase: Inventory_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_KIT_BASE";
		descriptionShort = "$STR_EXPANSION_KIT_BASE_DESC";
		model = "\DZ\gear\camping\watchtower_kit.p3d";
		rotationFlags = 17;
		itemSize[] = {1,5};
		weight = 280;
		itemBehaviour = 1;
		attachments[] = {"Rope"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
				class GlobalArmor
				{
					class Projectile
					{
						class Health
						{
							damage = 0;
						};
						class Blood
						{
							damage = 0;
						};
						class Shock
						{
							damage = 0;
						};
					};
					class FragGrenade
					{
						class Health
						{
							damage = 0;
						};
						class Blood
						{
							damage = 0;
						};
						class Shock
						{
							damage = 0;
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class Inventory: AnimSourceHidden{};
			class Placing: AnimSourceHidden{};
		};
	};
	class ExpansionKitBasePlacing: ExpansionKitBase
	{
		scope = 0;
		model = "\DZ\gear\camping\watchtower_kit_placing.p3d";
		storageCategory = 10;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"dz\gear\consumables\data\pile_of_planks_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\camping\data\fence_pile_of_planks.rvmat"};
		hologramMaterial = "tent_medium";
		hologramMaterialPath = "dz\gear\camping\data";
		alignHologramToTerain = 0;
		slopeTolerance = 0.3;
	};
	class ExpansionKitLarge: ExpansionKitBase
	{
		model = "\DZ\gear\camping\watchtower_kit.p3d";
		itemSize[] = {1,5};
		weight = 200;
	};
	class ExpansionKitLargePlacing: ExpansionKitLarge
	{
		scope = 2;
		model = "\DZ\gear\camping\watchtower_kit_placing.p3d";
		storageCategory = 10;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"dz\gear\consumables\data\pile_of_planks_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\camping\data\fence_pile_of_planks.rvmat"};
		hologramMaterial = "tent_medium";
		hologramMaterialPath = "dz\gear\camping\data";
		alignHologramToTerain = 0;
		slopeTolerance = 0.3;
	};
	class ExpansionKitSmall: ExpansionKitBase
	{
		model = "\DZ\gear\camping\fence_kit.p3d";
		itemSize[] = {1,3};
		weight = 100;
	};
	class ExpansionKitSmallPlacing: ExpansionKitSmall
	{
		scope = 2;
		model = "\DZ\gear\camping\fence_kit_placing.p3d";
		storageCategory = 10;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"dz\gear\consumables\data\pile_of_planks_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\camping\data\fence_pile_of_planks.rvmat"};
		hologramMaterial = "tent_medium";
		hologramMaterialPath = "dz\gear\camping\data";
		alignHologramToTerain = 0;
		slopeTolerance = 0.3;
	};
	class ExpansionBaseBuilding: Inventory_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BASEBUILDING";
		descriptionShort = "$STR_EXPANSION_BASEBUILDING_DESC";
		vehicleClass = "Expansion_Construction";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		carveNavmesh = 1;
		rotationFlags = 2;
		itemSize[] = {20,20};
		weight = 1000;
		lootCategory = "Crafted";
		useEntityHierarchy = "true";
		physLayer = "item_large";
	};
};
