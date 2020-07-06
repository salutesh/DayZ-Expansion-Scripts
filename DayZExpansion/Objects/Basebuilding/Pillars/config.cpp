#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Pillars
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionWoodPillarKit","ExpansionWoodPillar"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionWoodPillarKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BB_PILLAR_KIT";
		descriptionShort = "$STR_EXPANSION_BB_PILLAR_KIT_DESC";
		model = "\DZ\gear\camping\fence_kit.p3d";
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
		class AnimEvents
		{
			class SoundWeapon
			{
				class crafting_1
				{
					soundSet = "FenceKit_crafting_1_SoundSet";
					id = 1111;
				};
				class crafting_2
				{
					soundSet = "FenceKit_crafting_2_SoundSet";
					id = 1112;
				};
				class crafting_3
				{
					soundSet = "FenceKit_crafting_3_SoundSet";
					id = 1113;
				};
				class crafting_4
				{
					soundSet = "FenceKit_crafting_4_SoundSet";
					id = 1114;
				};
				class crafting_5
				{
					soundSet = "FenceKit_crafting_5_SoundSet";
					id = 1115;
				};
			};
		};
	};
	class ExpansionWoodPillarKitPlacing: ExpansionWoodPillarKit
	{
		scope = 1;
		model = "\DayZExpansion\Objects\basebuilding\Pillars\woodpillar\WoodPillar_Placing.p3d";
		slopeTolerance = 0.85;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
		class ExpansionSnapping
		{
			type = "PILLAR";
			xOffset = 0;
			yOffset = 0;
			zOffset = 0;
			xSize = 0.1;
			ySize = 3;
			zSize = 0.1;
		};
	};
	class ExpansionWoodPillar: ExpansionBaseBuilding
	{
		scope = 2;
		displayname = "$STR_EXPANSION_BB_WOOD_PILLAR";
		model = "\DayZExpansion\Objects\Basebuilding\Pillars\woodpillar\WoodPillar.p3d";
		vehicleClass = "Expansion_Construction";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
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
			class AnimRotate
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class Pillar_Level_1: AnimSourceHidden{};
		};
		class Construction
		{
			class pillar
			{
				class Pillar_Level_1
				{
					name = "$STR_EXPANSION_BB_WOOD_PILLAR_BASE";
					is_base = 1;
					id = 1;
					show_on_init = 1;
					required_parts[] = {};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					class Materials{};
				};
			};
		};
		class ExpansionSnapping
		{
			type = "PILLAR";
			default_hide[] = {};
			class Pos_0
			{
				class Dir_0
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_1
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_2
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_3
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_4
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_5
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_6
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_7
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
			};
			class Pos_1
			{
				class Dir_0
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_1
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_2
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_3
				{
					allow[] = {"WALL"};
					offset[] = {0};
				};
				class Dir_4
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_5
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_6
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_7
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {0,0};
				};
				class Dir_8
				{
					allow[] = {"PILLAR"};
					offset[] = {0};
				};
			};
			class Pos_2
			{
				class Dir_0
				{
					allow[] = {"PILLAR"};
					offset[] = {0};
				};
			};
			class Pos_3
			{
				class Dir_0
				{
					allow[] = {"PILLAR"};
					offset[] = {0};
				};
			};
			class Pos_4
			{
				class Dir_0
				{
					allow[] = {"PILLAR"};
					offset[] = {0};
				};
			};
			class Pos_5
			{
				class Dir_0
				{
					allow[] = {"PILLAR"};
					offset[] = {0};
				};
			};
		};
	};
};
