#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Animals_lepus_europaeus
	{
		units[] = {"Animal_lepuseuropaeus","RabbitPelt","RabbitLegMeat"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Animals"};
	};
};
class CfgVehicles
{
	class AnimalBase;
	class Inventory_Base;
	class Pelt_Base: Inventory_Base{};
	class Edible_Base: Inventory_Base{};
	class NutritionModifiers;
	class BaseFoodStageTransitions;
	class MeatStageTransitions: BaseFoodStageTransitions{};
	class AnimalCorpsesStageTransitions: BaseFoodStageTransitions{};
	class FoodAnimationSources;
	class RabbitPelt: Pelt_Base
	{
		scope = 2;
		descriptionShort = "$STR_CfgVehicles_RabbitPelt1";
		model = "\dz\gear\consumables\Pelt_Rabbit.p3d";
		weight = 220;
		itemSize[] = {3,2};
		peltGain = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 200;
					healthLevels[] = {{1.0,{"DZ\gear\consumables\data\pelt_rabbit.rvmat"}},{0.7,{"DZ\gear\consumables\data\pelt_rabbit.rvmat"}},{0.5,{"DZ\gear\consumables\data\pelt_rabbit_damage.rvmat"}},{0.3,{"DZ\gear\consumables\data\pelt_rabbit_damage.rvmat"}},{0.0,{"DZ\gear\consumables\data\pelt_rabbit_destruct.rvmat"}}};
				};
			};
		};
	};
	class RabbitLegMeat: Edible_Base
	{
		scope = 2;
		displayName = "$STR_RabbitLegMeat0";
		descriptionShort = "$STR_RabbitLegMeat1";
		model = "\dz\gear\food\meat_leg.p3d";
		rotationFlags = 17;
		weight = 0;
		interactionWeight = 1;
		quantityBar = 1;
		varQuantityInit = 110;
		varQuantityMin = 0;
		varQuantityMax = 110;
		itemSize[] = {1,2};
		absorbency = 0.3;
		inventorySlot = "Ingredient";
		isMeleeWeapon = 1;
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeFist";
				range = 1.0;
			};
			class Heavy
			{
				ammo = "MeleeFist_Heavy";
				range = 1.0;
			};
			class Sprint
			{
				ammo = "MeleeFist_Heavy";
				range = 2.8;
			};
		};
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_leg_raw_CO.paa","dz\gear\food\data\meat_leg_baked_CO.paa","dz\gear\food\data\meat_leg_baked_CO.paa","dz\gear\food\data\meat_leg_dried_CO.paa","dz\gear\food\data\meat_leg_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_leg_raw.rvmat","dz\gear\food\data\meat_leg_baked.rvmat","dz\gear\food\data\meat_leg_boiled.rvmat","dz\gear\food\data\meat_leg_dried.rvmat","dz\gear\food\data\meat_leg_burnt.rvmat","dz\gear\food\data\meat_leg_rotten.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 50;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
		class AnimationSources: FoodAnimationSources{};
		class Food
		{
			class FoodStages
			{
				class Raw
				{
					visual_properties[] = {0,0,0};
					nutrition_properties[] = {1,517,338,218,1,4};
					cooking_properties[] = {0,0};
				};
				class Rotten
				{
					visual_properties[] = {-1,-1,5};
					nutrition_properties[] = {1,150,338,30,1,"4 +     16"};
					cooking_properties[] = {0,0};
				};
				class Baked
				{
					visual_properties[] = {0,1,1};
					nutrition_properties[] = {1,480,184,155,1};
					cooking_properties[] = {70,45};
				};
				class Boiled
				{
					visual_properties[] = {0,2,2};
					nutrition_properties[] = {1,480,184,155,1};
					cooking_properties[] = {70,55};
				};
				class Dried
				{
					visual_properties[] = {0,3,3};
					nutrition_properties[] = {1,380,120,150,1};
					cooking_properties[] = {70,45,80};
				};
				class Burned
				{
					visual_properties[] = {0,4,4};
					nutrition_properties[] = {1,100,50,40,1,16};
					cooking_properties[] = {100,30};
				};
			};
			class FoodStageTransitions: MeatStageTransitions{};
		};
		class NoiseImpact
		{
			strength = 600.0;
			type = "sound";
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class openTunaCan
				{
					soundSet = "openTunaCan_SoundSet";
					id = 204;
				};
				class Eating_TakeFood
				{
					soundSet = "Eating_TakeFood_Soundset";
					id = 889;
				};
				class Eating_BoxOpen
				{
					soundSet = "Eating_BoxOpen_Soundset";
					id = 893;
				};
				class Eating_BoxShake
				{
					soundSet = "Eating_BoxShake_Soundset";
					id = 894;
				};
				class Eating_BoxEnd
				{
					soundSet = "Eating_BoxEnd_Soundset";
					id = 895;
				};
			};
		};
	};
	class Animal_LepusEuropaeus: AnimalBase
	{
		simulation = "dayzanimal";
		scope = 2;
		model = "\DayZExpansion\Animals\lepus_europaeus\lepus_europaeus.p3d";
		displayName = "$STR_EXPANSION_AI_LEPUS_EUROOAEUS";
		descriptionShort = "$STR_EXPANSION_AI_LEPUS_EUROOAEUS_DESC";
		aiAgentTemplate = "AmbientLife";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
				};
			};
		};
		class Skinning
		{
			class ObtainedSteaks
			{
				item = "RabbitLegMeat";
				count = 2;
				quantityMinMaxCoef[] = {0.5,1};
			};
			class ObtainedPelt
			{
				item = "RabbitPelt";
				count = 1;
				quantity = 1;
				transferToolDamageCoef = 1;
			};
			class ObtainedGuts
			{
				item = "SmallGuts";
				count = 0;
				quantityMinMaxCoef[] = {0.5,1};
			};
			class ObtainedBones
			{
				item = "Bone";
				count = 1;
				quantityMinMaxCoef[] = {0.2,0.3};
				transferToolDamageCoef = 1;
			};
		};
		class enfanimsys
		{
			meshObject = "DayZExpansion\Animals\lepus_europaeus\data\lepus_europaeus_skeleton.xob";
			graphname = "dz\animals\animations\!graph_files\AmbientLife\AmbientLife_Graph.agr";
			defaultinstance = "DayZExpansion\animations\Animals\lepus_europaeus\animations\!graph_files\AmbientLife\LepusEuropaeus_Skeleton_AnimInstance.asi";
			startnode = "Master_SM";
			skeletonName = "lepus_europaeus_skeleton.xob";
		};
		class AnimEvents
		{
			class Steps
			{
				class Walk1
				{
					soundLookupTable = "PawSmallWalk_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 1;
				};
				class Walk2
				{
					soundLookupTable = "PawSmallWalk_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 2;
				};
				class Walk3
				{
					soundLookupTable = "PawSmallWalk_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 3;
				};
				class Walk4
				{
					soundLookupTable = "PawSmallWalk_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 4;
				};
				class Grazing1
				{
					soundLookupTable = "PawSmallGrazing_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 9;
				};
				class Grazing2
				{
					soundLookupTable = "PawSmallGrazing_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 10;
				};
				class Bodyfall
				{
					soundLookupTable = "PawSmallBodyfall_LookupTable";
					noise = "DeerStepNoise";
					effectSet[] = {"DeerStepEffect1","DeerStepEffect2"};
					id = 11;
				};
			};
			class Sounds{};
			class Damages
			{
				class Bite
				{
					damage = "DeerBiteDamage";
					id = 1;
				};
			};
		};
		class CommandMoveSettings
		{
			useSpeedMapping = 1;
			movementSpeedMapping[] = {0.0,0.25,0.8,3.0};
		};
	};
};
