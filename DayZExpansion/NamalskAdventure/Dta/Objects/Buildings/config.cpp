#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Buildings
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Camping","DZ_Structures_Industrial"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class PowerGenerator;
	class RepairBenchContainer: Container_Base
	{
		scope = 2;
		displayName = "Workbench Container";
		descriptionShort = "PLACEHOLDER";
		model = "\DZ\gear\camping\sea_chest.p3d";
		hologramMaterial = "sea_chest";
		hologramMaterialPath = "dz\gear\camping\data";
		itemBehaviour = 0;
		itemSize[] = {10,10};
		attachments[] = {"Shoulder","Melee","Vest","Body","Hips","Legs","Back","Headgear","Mask","Eyewear","Gloves","Feet","Armband"};
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\camping\data\sea_chest_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 600;
					transferToAttachmentsCoef = 0.06;
					healthLevels[] = {{1,{"DZ\gear\camping\data\sea_chest.rvmat"}},{0.7,{"DZ\gear\camping\data\sea_chest.rvmat"}},{0.5,{"DZ\gear\camping\data\sea_chest_damage.rvmat"}},{0.3,{"DZ\gear\camping\data\sea_chest_damage.rvmat"}},{0,{"DZ\gear\camping\data\sea_chest_destruct.rvmat"}}};
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage = 8;
					};
					class Blood
					{
						damage = 8;
					};
					class Shock
					{
						damage = 8;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
	};
	class RepairBenchGenerator: PowerGenerator
	{
		scope = 2;
		model = "\DZ\gear\camping\power_generator.p3d";
		attachments[] += {"SparkPlug"};
	};
};
