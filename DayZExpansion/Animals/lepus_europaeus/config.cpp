class CfgPatches
{
	class DayZExpansion_Animals_lepus_europaeus
	{
		units[]=
		{
			"Animal_lepuseuropaeus",
			"RabbitPelt"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Animals",
			"DZ_Gear_Consumables"
		};
	};
};
class CfgVehicles
{
	class AnimalBase;
	class Pelt_Base;
	class RabbitPelt: Pelt_Base
	{
		scope=2;
		descriptionShort="$STR_CfgVehicles_RabbitPelt1";
		model="\dz\gear\consumables\Pelt_Rabbit.p3d";
		weight=220;
		itemSize[]={3,2};
		peltGain=1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\consumables\data\pelt_rabbit.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\consumables\data\pelt_rabbit.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\consumables\data\pelt_rabbit_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\consumables\data\pelt_rabbit_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\consumables\data\pelt_rabbit_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Animal_LepusEuropaeus: AnimalBase
	{
		simulation="dayzanimal";
		scope=2;
		model="\DayZExpansion\Animals\lepus_europaeus\lepus_europaeus.p3d";
		displayName="$STR_EXPANSION_AI_LEPUS_EUROPAEUS";
		descriptionShort="$STR_EXPANSION_AI_LEPUS_EUROPAEUS_DESC";
		aiAgentTemplate="AmbientLife";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=15;
					healthLabels[]={1,0.69999999,0.5,0.30000001,0};
				};
				class Blood
				{
					hitpoints=5000;
				};
				class Shock
				{
					hitpoints=100;
				};
			};
		};
		class Skinning
		{
			class ObtainedSteaks
			{
				item="RabbitLegMeat";
				count=2;
				quantityMinMaxCoef[]={0.5,1};
			};
			class ObtainedPelt
			{
				item="RabbitPelt";
				count=1;
				quantity=1;
				transferToolDamageCoef=1;
			};
			class ObtainedGuts
			{
				item="SmallGuts";
				count=0;
				quantityMinMaxCoef[]={0.5,1};
			};
			class ObtainedBones
			{
				item="Bone";
				count=1;
				quantityMinMaxCoef[]={0.2,0.30000001};
				transferToolDamageCoef=1;
			};
		};
		class enfanimsys
		{
			meshObject="DayZExpansion\Animals\lepus_europaeus\data\lepus_europaeus_skeleton.xob";
			graphname="dz\animals\animations\!graph_files\AmbientLife\AmbientLife_Graph.agr";
			defaultinstance="DayZExpansion\animations\Animals\lepus_europaeus\animations\!graph_files\AmbientLife\LepusEuropaeus_Skeleton_AnimInstance.asi";
			startnode="Master_SM";
			skeletonName="lepus_europaeus_skeleton.xob";
		};
		class AnimEvents
		{
			class Steps
			{
				class Walk1
				{
					soundLookupTable="PawSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=1;
				};
				class Walk2
				{
					soundLookupTable="PawSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=2;
				};
				class Walk3
				{
					soundLookupTable="PawSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=3;
				};
				class Walk4
				{
					soundLookupTable="PawSmallWalk_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=4;
				};
				class Grazing1
				{
					soundLookupTable="PawSmallGrazing_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=9;
				};
				class Grazing2
				{
					soundLookupTable="PawSmallGrazing_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=10;
				};
				class Bodyfall
				{
					soundLookupTable="PawSmallBodyfall_LookupTable";
					noise="DeerStepNoise";
					effectSet[]=
					{
						"DeerStepEffect1",
						"DeerStepEffect2"
					};
					id=11;
				};
			};
			class Sounds
			{
			};
			class Damages
			{
				class Bite
				{
					damage="DeerBiteDamage";
					id=1;
				};
			};
		};
		class CommandMoveSettings
		{
			useSpeedMapping=1;
			movementSpeedMapping[]={0,0.25,0.80000001,3};
		};
	};
};
