#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Floors
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionStairKit","ExpansionStairS1_5x3x3"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionStairKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BB_STAIR_KIT";
		descriptionShort = "$STR_EXPANSION_BB_STAIR_KIT_DESC";
		model = "\DZ\gear\camping\fence_kit.p3d";
		placingTypes[] = {"ExpansionStairS1_5x3x3KitPlacing"};
	};
	class ExpansionStairBaseKitPlacing: ExpansionStairKit
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Stairs\Stair_S_1_5x3x3_Placing.p3d";
		slopeTolerance = 0.85;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionStairS1_5x3x3KitPlacing: ExpansionStairBaseKitPlacing
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Stairs\Stair_S_1_5x3x3_Placing.p3d";
		deployType = "ExpansionStairS1_5x3x3";
		class ExpansionSnapping
		{
			type = "STAIR";
			xOffset = 0.75;
			yOffset = 0;
			zOffset = 1.5;
			xSize = 1.5;
			ySize = 3;
			zSize = 3;
		};
	};
	class ExpansionStairBase: ExpansionBaseBuilding
	{
		scope = 0;
		displayname = "$STR_EXPANSION_BB_STAIR_BASE";
		model = "\DayZExpansion\Objects\Basebuilding\Stairs\Stair_S_1_5x3x3.p3d";
		vehicleClass = "Expansion_Construction";
		createProxyPhysicsOnInit = "true";
		createdProxiesOnInit[] = {"Deployed"};
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		physLayer = "item_large";
		rotationFlags = 2;
		attachments[] = {"Att_ExpansionLumber","Material_Nails","Material_WoodenPlanks"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 30000;
					healthLevels[] = {{1.0,{""}},{0.7,{""}},{0.5,{""}},{0.3,{""}},{0.0,{""}}};
				};
			};
			class GlobalArmor
			{
				class Melee
				{
					class Health
					{
						damage = 1;
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
				class Projectile
				{
					class Health
					{
						damage = 1;
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
		class GUIInventoryAttachmentsProps
		{
			class Material
			{
				name = "$STR_CfgVehicles_Fence_Att_Category_Materials";
				description = "";
				attachmentSlots[] = {"Att_ExpansionLumber","Material_Nails","Material_WoodenPlanks"};
				icon = "cat_bb_material";
				selection = "interact";
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
			class Deployed: AnimSourceHidden{};
			class wood_base: AnimSourceHidden{};
			class wood_handrailing: AnimSourceHidden{};
			class wood_support: AnimSourceHidden{};
			class wood_tread: AnimSourceHidden{};
		};
		class Construction
		{
			class interact
			{
				class wood_base
				{
					name = "$STR_EXPANSION_BB_WOOD_STAIR_BASE";
					is_base = 1;
					id = 1;
					required_parts[] = {};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 1;
					snapping_show[] = {};
					class Materials
					{
						class Material1
						{
							type = "ExpansionLumber3";
							slot_name = "Att_ExpansionLumber";
							quantity = 4;
						};
					};
				};
				class wood_handrailing
				{
					name = "$STR_EXPANSION_BB_WOOD_STAIR_HANDRAIL";
					is_base = 0;
					id = 2;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 5;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 15;
						};
					};
				};
				class wood_support
				{
					name = "$STR_EXPANSION_BB_WOOD_STAIR_SUPPORT";
					is_base = 0;
					id = 3;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 5;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 15;
						};
					};
				};
				class wood_tread
				{
					name = "$STR_EXPANSION_BB_WOOD_STAIR_TREAD";
					is_base = 0;
					id = 4;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 5;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 15;
						};
					};
				};
			};
		};
		class ExpansionSnapping
		{
			type = "STAIR";
			default_hide[] = {};
			class Pos_0
			{
				class Dir_0
				{
					allow[] = {"FLOOR","RAMP","STAIR"};
					offset[] = {1,1,1};
				};
				class Dir_1
				{
					allow[] = {"WALL"};
					offset[] = {2};
				};
			};
			class Pos_1
			{
				class Dir_0
				{
					allow[] = {"FLOOR","RAMP"};
					offset[] = {1,1};
				};
				class Dir_1
				{
					allow[] = {"WALL"};
					offset[] = {2};
				};
			};
			class Pos_2
			{
				class Dir_0
				{
					allow[] = {"FLOOR","RAMP","STAIR"};
					offset[] = {1,1,1};
				};
				class Dir_1
				{
					allow[] = {"WALL"};
					offset[] = {2};
				};
			};
		};
	};
	class ExpansionStairS1_5x3x3: ExpansionStairBase
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Stairs\Stair_S_1_5x3x3.p3d";
	};
};
