#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Walls
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DayZExpansion_Objects_BaseBuilding_Data"};
		units[] = {"ExpansionWallKit","ExpansionWall6x3"};
		weapons[] = {};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionWallKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BB_WALL_KIT";
		descriptionShort = "$STR_EXPANSION_BB_WALL_KIT_DESC";
		placingTypes[] = {"ExpansionWall6x3KitPlacing"};
	};
	class ExpansionWallBaseKitPlacing: ExpansionWallKit
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Walls\Wall_6x3_Placing.p3d";
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionWall6x3KitPlacing: ExpansionWallBaseKitPlacing
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Basebuilding\Walls\Wall_6x3_Placing.p3d";
		deployType = "ExpansionWall6x3";
		class ExpansionSnapping
		{
			type = "WALL";
			xOffset = 3;
			yOffset = 0;
			zOffset = 0;
			xSize = 6;
			ySize = 3;
			zSize = 0.1;
		};
	};
	class ExpansionWallBase: ExpansionBaseBuilding
	{
		scope = 0;
		displayname = "$STR_EXPANSION_BB_WALL_BASE";
		model = "\DayZExpansion\Objects\Basebuilding\Walls\Wall_6x3.p3d";
		vehicleClass = "Expansion_Construction";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		carveNavmesh = 1;
		physLayer = "item_large";
		createProxyPhysicsOnInit = "true";
		createdProxiesOnInit[] = {"Deployed"};
		rotationFlags = 2;
		attachments[] = {"Material_Nails","Material_WoodenPlanks","Att_ExpansionLumber","Att_ExpansionLumberThird","Att_ExpansionLumberHalf","Att_ExpansionCodeLock_1","Att_ExpansionCodeLock_2"};
		class GUIInventoryAttachmentsProps
		{
			class Material
			{
				name = "$STR_CfgVehicles_Fence_Att_Category_Materials";
				description = "";
				attachmentSlots[] = {"Att_ExpansionLumber","Att_ExpansionLumberThird","Att_ExpansionLumberHalf","Material_Nails","Material_WoodenPlanks"};
				icon = "cat_bb_material";
				selection = "interact";
			};
			class Attachments
			{
				name = "$STR_CfgVehicles_Fence_Att_Category_Attachments";
				description = "";
				attachmentSlots[] = {"Att_ExpansionCodeLock_1","Att_ExpansionCodeLock_2"};
				icon = "cat_bb_attachments";
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
			class window_rotate: AnimRotate
			{
				animPeriod = 0.3;
			};
			class door_rotate: AnimRotate
			{
				animPeriod = 0.6;
			};
			class gate_rotate: AnimRotate
			{
				animPeriod = 0.6;
			};
			class window_ll_rotate: window_rotate{};
			class window_LR_rotate: window_rotate{};
			class window_RL_rotate: window_rotate{};
			class window_RR_rotate: window_rotate{};
			class gate_L_rotate: gate_rotate{};
			class gate_R_rotate: gate_rotate{};
			class Deployed: AnimSourceHidden{};
			class wood_base: AnimSourceHidden{};
			class wood_wallframe: AnimSourceHidden{};
			class wood_wallfinished: AnimSourceHidden{};
			class wood_base_third: AnimSourceHidden{};
			class wood_wallframe_third: AnimSourceHidden{};
			class wood_wallfinished_third: AnimSourceHidden{};
			class wood_base_half: AnimSourceHidden{};
			class wood_wallframe_half: AnimSourceHidden{};
			class wood_wallfinished_half: AnimSourceHidden{};
			class wood_gateframe: AnimSourceHidden{};
			class wood_gatefinished: AnimSourceHidden{};
			class wood_windowframe: AnimSourceHidden{};
			class wood_windowfinished: AnimSourceHidden{};
			class wood_doorframe: AnimSourceHidden{};
			class wood_doorfinished: AnimSourceHidden{};
			class wood_window_ll: AnimSourceHidden{};
			class wood_window_rl: AnimSourceHidden{};
			class wood_window_lr: AnimSourceHidden{};
			class wood_window_rr: AnimSourceHidden{};
			class wood_door: AnimSourceHidden{};
			class wood_gate_l: AnimSourceHidden{};
			class wood_gate_r: AnimSourceHidden{};
			class wood_window_ll_rotate: window_LL_rotate{};
			class wood_window_lr_rotate: window_LR_rotate{};
			class wood_window_rl_rotate: window_RL_rotate{};
			class wood_window_RR_rotate: window_RR_rotate{};
			class wood_door_rotate: door_rotate{};
			class wood_gate_L_rotate: gate_L_rotate{};
			class wood_gate_R_rotate: gate_R_rotate{};
		};
		class Construction
		{
			class interact
			{
				class wood_base
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_BASE";
					is_base = 1;
					id = 1;
					required_parts[] = {};
					conflicted_parts[] = {"wood_base_third","wood_base_half"};
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
							quantity = 2;
						};
					};
				};
				class wood_base_third
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_BASE_THIRD";
					is_base = 0;
					id = 10;
					required_parts[] = {};
					conflicted_parts[] = {"wood_base","wood_base_half"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 1;
					snapping_show[] = {};
					class Materials
					{
						class Material1
						{
							type = "ExpansionLumber1";
							slot_name = "Att_ExpansionLumberThird";
							quantity = 2;
						};
					};
				};
				class wood_wallframe_third
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FRAME";
					is_base = 0;
					id = 11;
					required_parts[] = {"wood_base_third"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 4;
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 2;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 5;
						};
					};
				};
				class wood_wallfinished_third
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FINISHED";
					is_base = 0;
					id = 12;
					required_parts[] = {"wood_wallframe_third"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 3;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 5;
						};
					};
				};
				class wood_base_half
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_BASE_HALF";
					is_base = 0;
					id = 13;
					required_parts[] = {};
					conflicted_parts[] = {"wood_base","wood_base_third"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 1;
					snapping_show[] = {};
					class Materials
					{
						class Material1
						{
							type = "ExpansionLumber1_5";
							slot_name = "Att_ExpansionLumberHalf";
							quantity = 2;
						};
					};
				};
				class wood_wallframe_half
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FRAME";
					is_base = 0;
					id = 14;
					required_parts[] = {"wood_base_half"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 4;
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
							quantity = 10;
						};
					};
				};
				class wood_wallfinished_half
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FINISHED";
					is_base = 0;
					id = 15;
					required_parts[] = {"wood_wallframe_half"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
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
							quantity = 10;
						};
					};
				};
				class wood_wallframe
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FRAME";
					is_base = 0;
					id = 8;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_doorframe","wood_gateframe","wood_windowframe"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 4;
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
				class wood_wallfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_WALL_FINISHED";
					is_base = 0;
					id = 9;
					required_parts[] = {"wood_wallframe"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
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
				class wood_doorframe
				{
					name = "$STR_EXPANSION_BB_WOOD_DOOR_FRAME";
					is_base = 0;
					id = 2;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_wallframe","wood_gateframe","wood_windowframe"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 3;
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
				class wood_doorfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_DOOR_FINISHED";
					is_base = 0;
					id = 3;
					required_parts[] = {"wood_doorframe"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
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
				class wood_gateframe
				{
					name = "$STR_EXPANSION_BB_WOOD_GATE_FRAME";
					is_base = 0;
					id = 4;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_wallframe","wood_doorframe","wood_windowframe"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 2;
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
				class wood_gatefinished
				{
					name = "$STR_EXPANSION_BB_WOOD_GATE_FINISHED";
					is_base = 0;
					id = 5;
					required_parts[] = {"wood_gateframe"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
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
				class wood_windowframe
				{
					name = "$STR_EXPANSION_BB_WOOD_WINDOW_FRAME";
					is_base = 0;
					id = 6;
					required_parts[] = {"wood_base"};
					conflicted_parts[] = {"wood_wallframe","wood_gateframe","wood_doorframe"};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {3};
					order = 1;
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
				class wood_windowfinished
				{
					name = "$STR_EXPANSION_BB_WOOD_WINDOW_FINISHED";
					is_base = 0;
					id = 7;
					required_parts[] = {"wood_windowframe"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 10;
					dismantle_action_type = 10;
					material_type = 2;
					snapping_show[] = {};
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
			type = "WALL";
			default_hide[] = {3};
			class Pos_0
			{
				class Dir_0
				{
					allow[] = {"WALL","PILLAR"};
					offset[] = {0,0};
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
					allow[] = {"FLOOR","RAMP","STAIR"};
					offset[] = {1,1,1};
				};
				class Dir_2
				{
					allow[] = {"WALL"};
					offset[] = {4};
				};
			};
			class Pos_2
			{
				class Dir_0
				{
					allow[] = {"WALL","PILLAR"};
					offset[] = {0,0};
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
			};
			class Pos_3
			{
				class Dir_0
				{
					allow[] = {"WALL"};
					offset[] = {1};
				};
				class Dir_1
				{
					allow[] = {"FLOOR","RAMP","STAIR"};
					offset[] = {1,1,1};
				};
				class Dir_2
				{
					allow[] = {"FLOOR","RAMP","STAIR"};
					offset[] = {1,1,1};
				};
			};
		};
	};
	class ExpansionWall6x3: ExpansionWallBase
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Walls\Wall_6x3.p3d";
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
	};
};
