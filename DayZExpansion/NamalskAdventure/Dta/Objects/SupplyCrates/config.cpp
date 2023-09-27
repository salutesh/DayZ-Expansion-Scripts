#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_SupplyCrates
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DayZExpansion_NamalskAdventure_Items","ns3"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Container_Base;
	class Expansion_SupplyCrate_Base: Container_Base
	{
		scope = 1;
		displayName = "Supply Crate";
		descriptionShort = "Not needed.";
		handheld = "false";
		weight = 50000;
		physLayer = "item_large";
		carveNavmesh = 1;
		storageCategory = 10;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_medic.p3d";
		hiddenSelections[] = {"door1",""};
		hiddenSelectionsMaterials[] = {"nst\ns3\structures\crates\data\lantia_supply_crate.rvmat","nst\ns3\structures\crates\data\lantia_supply_crate_em.rvmat"};
		heavyItem = 1;
		absorbency = 0.1;
		itemSize[] = {10,5};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband","Att_ExpansionSupplyCrateKey"};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
		};
		class AnimationSources
		{
			class Door1
			{
				source = "user";
				initPhase = 0;
				animPeriod = 1;
			};
		};
		class Doors
		{
			class Door1
			{
				displayName = "door 1";
				component = "Door1";
				soundPos = "door1_action";
				animPeriod = 1;
				initPhase = 0;
				initOpened = 0.5;
				soundOpen = "doorWoodGateSmallOpen";
				soundClose = "doorWoodGateSmallClose";
				soundOpenABit = "doorWoodGateSmallOpenABit";
				soundLocked = "doorWoodGateRattle";
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1000;
				};
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
				class Melee
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
			class DamageZones
			{
				class door1
				{
					class Health
					{
						hitpoints = 1000;
						transferToGlobalCoef = 0;
					};
					componentNames[] = {"door1"};
					fatalInjuryCoef = -1;
					class ArmorType
					{
						class Projectile
						{
							class Health
							{
								damage = 3;
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
						class Melee
						{
							class Health
							{
								damage = 5;
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
	};
	class Expansion_SupplyCrate_Bunker_Orange: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_medic.p3d";
	};
	class Expansion_SupplyCrate_Bunker_Green: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_military.p3d";
	};
	class Expansion_SupplyCrate_Bunker_Brown: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_tools.p3d";
	};
	class Expansion_SupplyCrate_World_T1_Orange: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_medic.p3d";
	};
	class Expansion_SupplyCrate_World_T1_Green: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_military.p3d";
	};
	class Expansion_SupplyCrate_World_T1_Brown: Expansion_SupplyCrate_Base
	{
		scope = 2;
		model = "\nst\ns3\structures\crates\supplycrates_spawn_tools.p3d";
	};
};
