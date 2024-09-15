class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Ramps
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
		units[]=
		{
			"ExpansionRampKit",
			"ExpansionRamp6x1_5x6",
			"ExpansionRamp3x1_5x6"
		};
		weapons[]={};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionRampKit: ExpansionKitLarge
	{
		scope=2;
		displayName="$STR_EXPANSION_BB_RAMP_KIT";
		descriptionShort="$STR_EXPANSION_BB_RAMP_KIT_DESC";
		placingTypes[]=
		{
			"ExpansionRamp6x1_5x6KitPlacing"
		};
	};
	class ExpansionRampBaseKitPlacing: ExpansionRampKit
	{
		scope=1;
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_6x1_5x6_Placing.p3d";
		slopeTolerance=0.85000002;
		hiddenSelections[]=
		{
			"placing"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,1,1,1.0,co)"
		};
	};
	class ExpansionRamp6x1_5x6KitPlacing: ExpansionRampBaseKitPlacing
	{
		scope=1;
		displayname="$STR_EXPANSION_BB_RAMP_6X1_5X6_PLACE";
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_6x1_5x6_Placing.p3d";
		deployType="ExpansionRamp6x1_5x6";
		class ExpansionSnapping
		{
			type="RAMP";
			xOffset=3;
			yOffset=0;
			zOffset=3;
			xSize=6;
			ySize=1.5;
			zSize=6;
		};
	};
	class ExpansionRamp3x1_5x6KitPlacing: ExpansionRampBaseKitPlacing
	{
		scope=1;
		displayname="$STR_EXPANSION_BB_RAMP_3X1_5X6_PLACE";
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_3x1_5x6_Placing.p3d";
		deployType="ExpansionRamp3x1_5x6";
		class ExpansionSnapping
		{
			type="RAMP";
			xOffset=3;
			yOffset=0;
			zOffset=3;
			xSize=3;
			ySize=1.5;
			zSize=3;
		};
	};
	class ExpansionRampBase: ExpansionBaseBuilding
	{
		scope=0;
		displayname="$STR_EXPANSION_BB_RAMP_BASE";
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_6x1_5x6.p3d";
		vehicleClass="Expansion_Construction";
		bounding="BSphere";
		overrideDrawArea="3.0";
		forceFarBubble="true";
		carveNavmesh=1;
		physLayer="item_large";
		createProxyPhysicsOnInit="true";
		createdProxiesOnInit[]=
		{
			"Deployed"
		};
		rotationFlags=2;
		attachments[]=
		{
			"Material_Nails",
			"Material_WoodenPlanks",
			"Att_ExpansionLumber"
		};
		class GUIInventoryAttachmentsProps
		{
			class Material
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Materials";
				description="";
				attachmentSlots[]=
				{
					"Att_ExpansionLumber",
					"Material_Nails",
					"Material_WoodenPlanks"
				};
				icon="cat_bb_material";
				selection="interact";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class AnimRotate
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class Deployed: AnimSourceHidden
			{
			};
			class wood_base: AnimSourceHidden
			{
			};
			class wood_frame: AnimSourceHidden
			{
			};
			class wood_finished: AnimSourceHidden
			{
			};
		};
		class Construction
		{
			class interact
			{
				class wood_base
				{
					name="$STR_EXPANSION_BB_WOOD_RAMP_BASE";
					is_base=1;
					id=1;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=10;
					dismantle_action_type=10;
					material_type=1;
					snapping_show[]={};
					class Materials
					{
						class Material1
						{
							type="ExpansionLumber3";
							slot_name="Att_ExpansionLumber";
							quantity=2;
						};
					};
				};
				class wood_frame
				{
					name="$STR_EXPANSION_BB_WOOD_RAMP_FRAME";
					is_base=0;
					id=2;
					required_parts[]=
					{
						"wood_base"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=10;
					dismantle_action_type=10;
					material_type=2;
					snapping_show[]={1,2,3};
					order=1;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=5;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=15;
						};
					};
				};
				class wood_finished
				{
					name="$STR_EXPANSION_BB_WOOD_RAMP_FINISHED";
					is_base=0;
					id=3;
					required_parts[]=
					{
						"wood_frame"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=10;
					dismantle_action_type=10;
					material_type=2;
					snapping_show[]={};
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=5;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=15;
						};
					};
				};
			};
		};
		class ExpansionSnapping
		{
			type="RAMP";
			default_hide[]={};
			class Pos_0
			{
				class Dir_0
				{
					allow[]=
					{
						"FLOOR",
						"RAMP"
					};
					offset[]={1,1};
				};
				class Dir_1
				{
					allow[]=
					{
						"WALL"
					};
					offset[]={2};
				};
			};
			class Pos_1
			{
				class Dir_0
				{
					allow[]=
					{
						"FLOOR",
						"RAMP"
					};
					offset[]={1,1};
				};
				class Dir_1
				{
					allow[]=
					{
						"WALL"
					};
					offset[]={2};
				};
			};
			class Pos_2
			{
				class Dir_0
				{
					allow[]=
					{
						"RAMP"
					};
					offset[]={1};
				};
			};
		};
	};
	class ExpansionRamp6x1_5x6: ExpansionRampBase
	{
		scope=2;
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_6x1_5x6.p3d";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30000;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Melee
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class Projectile
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
	class ExpansionRamp3x1_5x6: ExpansionRampBase
	{
		scope=2;
		model="\DayZExpansion\Objects\Basebuilding\Ramps\Ramp_3x1_5x6.p3d";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30000;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								""
							}
						},
						
						{
							0.69999999,
							
							{
								""
							}
						},
						
						{
							0.5,
							
							{
								""
							}
						},
						
						{
							0.30000001,
							
							{
								""
							}
						},
						
						{
							0,
							
							{
								""
							}
						}
					};
				};
			};
			class GlobalArmor
			{
				class Melee
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class Projectile
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage=1;
					};
					class Blood
					{
						damage=0;
					};
					class Shock
					{
						damage=0;
					};
				};
			};
		};
	};
};
