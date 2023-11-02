#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Objects_Anomalies
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Expansion_Anomaly_Base: Inventory_Base
	{
		scope = 1;
		model = "\DayZExpansion\NamalskAdventure\Dta\Objects\Anomalies\Expansion_Anomaly.p3d";
		bounding = "BSphere";
		forceFarBubble = "true";
		itemBehaviour = 2;
		handheld = "false";
		allowOwnedCargoManipulation = 1;
		attachments[] = {"Att_ExpansionAnomalyCore"};
		weight = 10000;
		inventoryCondition = "true";
		storageCategory = 1;
		openable = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100000;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 1;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 1;
					};
				};
				class Meele
				{
					class Health
					{
						damage = 1;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,100};
		};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "Anomaly Core";
				description = "Not needed.";
				attachmentSlots[] = {"Att_ExpansionAnomalyCore"};
				icon = "set:expansion_inventory image:anomaly";
			};
		};
	};
	class Expansion_Anomaly_Singularity: Expansion_Anomaly_Base
	{
		scope = 2;
		displayName = "Anomaly - Singularity";
	};
	class Expansion_Anomaly_Teleport: Expansion_Anomaly_Base
	{
		scope = 2;
		displayName = "Anomaly - Warper";
	};
	class Expansion_Anomaly_Fire: Expansion_Anomaly_Base
	{
		scope = 2;
		displayName = "Anomaly - Fire";
	};
};
