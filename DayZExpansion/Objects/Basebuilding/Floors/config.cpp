#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Floors
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionFloorKit","ExpansionFloor6x6","ExpansionFloor3x3","ExpansionFloor3x6"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionFloorKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BB_FLOOR_KIT";
		descriptionShort = "$STR_EXPANSION_BB_FLOOR_KIT_DESC";
		model = "\DZ\gear\camping\fence_kit.p3d";
		placingTypes[] = {"ExpansionFloor6x6KitPlacing"};
	};
	class ExpansionFloorBaseKitPlacing: ExpansionFloorKit
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_6x6_Placing.p3d";
		slopeTolerance = 0.85;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionFloor6x6KitPlacing: ExpansionFloorBaseKitPlacing
	{
		scope = 1;
		displayname = "$STR_EXPANSION_BB_FLOOR_6X6_PLACE";
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_6x6_Placing.p3d";
		deployType = "ExpansionFloor6x6";
		class ExpansionSnapping
		{
			type = "FLOOR";
			xOffset = 3;
			yOffset = 0;
			zOffset = 3;
			xSize = 6;
			ySize = 0.1;
			zSize = 6;
		};
	};
	class ExpansionFloor3x3KitPlacing: ExpansionFloorBaseKitPlacing
	{
		scope = 1;
		displayname = "$STR_EXPANSION_BB_FLOOR_3X3_PLACE";
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_3x3_Placing.p3d";
		deployType = "ExpansionFloor3x3";
		class ExpansionSnapping
		{
			type = "FLOOR";
			xOffset = 1.5;
			yOffset = 0;
			zOffset = 1.5;
			xSize = 3;
			ySize = 0.1;
			zSize = 3;
		};
	};
	class ExpansionFloor3x6KitPlacing: ExpansionFloorBaseKitPlacing
	{
		scope = 1;
		displayname = "$STR_EXPANSION_BB_FLOOR_3X6_PLACE";
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_3x6_Placing.p3d";
		deployType = "ExpansionFloor3x6";
		class ExpansionSnapping
		{
			type = "FLOOR";
			xOffset = 1.5;
			yOffset = 0;
			zOffset = 3;
			xSize = 3;
			ySize = 0.1;
			zSize = 6;
		};
	};
	class ExpansionFloorBase: ExpansionBaseBuilding
	{
		scope = 0;
		displayname = "$STR_EXPANSION_BB_FLOOR_BASE";
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_6x6.p3d";
		vehicleClass = "Expansion_Construction";
		createProxyPhysicsOnInit = "true";
		createdProxiesOnInit[] = {"Deployed"};
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		physLayer = "item_large";
		rotationFlags = 2;
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
		attachments[] = {"Att_ExpansionLumber","Material_Nails","Material_WoodenPlanks"};
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
			class wood_floorframe: AnimSourceHidden{};
			class wood_floorfinished: AnimSourceHidden{};
		};
		class Construction
		{
			class interact
			{
				class wood_base
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_BASE";
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
				class wood_floorframe
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_FRAME";
					is_base = 0;
					id = 2;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_hatchframe"};
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
				class wood_floorfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_FINISHED";
					is_base = 0;
					id = 3;
					required_parts[] = {"wood_floorframe"};
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
			type = "FLOOR";
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
				class Dir_2
				{
					allow[] = {"WALL","RAMP","STAIR"};
					offset[] = {4,3,3};
				};
			};
			class Pos_1
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
				class Dir_2
				{
					allow[] = {"WALL","RAMP","STAIR"};
					offset[] = {4,3,3};
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
				class Dir_2
				{
					allow[] = {"WALL","RAMP","STAIR"};
					offset[] = {4,3,3};
				};
			};
			class Pos_3
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
				class Dir_2
				{
					allow[] = {"WALL","RAMP","STAIR"};
					offset[] = {4,3,3};
				};
			};
		};
	};
	class ExpansionFloor6x6: ExpansionFloorBase
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_6x6.p3d";
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
			class wood_floorframe: AnimSourceHidden{};
			class wood_floorfinished: AnimSourceHidden{};
			class wood_hatchframe: AnimSourceHidden{};
			class wood_hatchfinished: AnimSourceHidden{};
		};
		class Construction: Construction
		{
			class interact: interact
			{
				class wood_base
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_BASE";
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
				class wood_floorframe
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_FRAME";
					is_base = 0;
					id = 2;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_hatchframe"};
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
				class wood_floorfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_FLOOR_FINISHED";
					is_base = 0;
					id = 3;
					required_parts[] = {"wood_floorframe"};
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
				class wood_hatchframe
				{
					name = "$STR_EXPANSION_BB_WOOD_HATCH_FRAME";
					is_base = 0;
					id = 4;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_floorframe"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {4,5};
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
				class wood_hatchfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_HATCH_FINISHED";
					is_base = 0;
					id = 5;
					required_parts[] = {"wood_hatchframe"};
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
		class ExpansionSnapping: ExpansionSnapping
		{
			default_hide[] = {4,5};
			class Pos_4
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {2};
				};
			};
			class Pos_5
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {2};
				};
			};
			class Pos_6
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {4};
				};
				class Dir_1
				{
					allow[] = {"STAIR"};
					offset[] = {5};
				};
			};
			class Pos_7
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {4};
				};
				class Dir_1
				{
					allow[] = {"STAIR"};
					offset[] = {5};
				};
			};
			class Pos_8
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {4};
				};
				class Dir_1
				{
					allow[] = {"STAIR"};
					offset[] = {5};
				};
			};
			class Pos_9
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {4};
				};
				class Dir_1
				{
					allow[] = {"STAIR"};
					offset[] = {5};
				};
			};
		};
	};
	class ExpansionFloor3x3: ExpansionFloorBase
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_3x3.p3d";
	};
	class ExpansionFloor3x6: ExpansionFloorBase
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Floors\Floor_3x6.p3d";
		class ExpansionSnapping: ExpansionSnapping
		{
			class Pos_4
			{
				class Dir_0
				{
					allow[] = {"STAIR"};
					offset[] = {1};
				};
				class Dir_1
				{
					allow[] = {"STAIR"};
					offset[] = {1};
				};
			};
		};
	};
};
