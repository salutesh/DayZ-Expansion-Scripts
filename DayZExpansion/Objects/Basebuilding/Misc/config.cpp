#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Misc
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		units[] = {"ExpansionHelipadKit","ExpansionHelipadKitPlacing","ExpansionHelipad","ExpansionHescoKit","ExpansionHescoKitPlacing","ExpansionHesco","ExpansionBarbedWireKit","ExpansionBarbedWireKitPlacing","ExpansionBarbedWire","ExpansionGunrack","ExpansionCone","ExpansionConePlacing","ExpansionSignDanger","ExpansionSignDangerPlacing","ExpansionSignDanger2","ExpansionSignDanger2Placing","ExpansionSignDanger3","ExpansionSignDanger3Placing","ExpansionSignRoadBarrier","ExpansionSignRoadBarrierPlacing","ExpansionStove","ExpansionStovePlacing"};
		weapons[] = {};
		ammo[] = {"BarbedWireDamage"};
	};
};
class CfgSlots
{
	class Slot_Shoulder1
	{
		name = "Shoulder1";
		displayName = "Weapon1";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder2
	{
		name = "Shoulder2";
		displayName = "Weapon2";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder3
	{
		name = "Shoulder3";
		displayName = "Weapon3";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder4
	{
		name = "Shoulder4";
		displayName = "Weapon4";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder5
	{
		name = "Shoulder5";
		displayName = "Weapon5";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder6
	{
		name = "Shoulder6";
		displayName = "Weapon6";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder7
	{
		name = "Shoulder7";
		displayName = "Weapon7";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder8
	{
		name = "Shoulder8";
		displayName = "Weapon8";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder9
	{
		name = "Shoulder9";
		displayName = "Weapon9";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder10
	{
		name = "Shoulder10";
		displayName = "Weapon10";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder11
	{
		name = "Shoulder11";
		displayName = "Weapon11";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder12
	{
		name = "Shoulder12";
		displayName = "Weapon12";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder13
	{
		name = "Shoulder13";
		displayName = "Weapon13";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder14
	{
		name = "Shoulder14";
		displayName = "Weapon14";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder15
	{
		name = "Shoulder15";
		displayName = "Weapon15";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder16
	{
		name = "Shoulder16";
		displayName = "Weapon16";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder17
	{
		name = "Shoulder17";
		displayName = "Weapon17";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder18
	{
		name = "Shoulder18";
		displayName = "Weapon18";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder19
	{
		name = "Shoulder19";
		displayName = "Weapon19";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder20
	{
		name = "Shoulder20";
		displayName = "Weapon20";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder21
	{
		name = "Shoulder21";
		displayName = "Weapon21";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder22
	{
		name = "Shoulder22";
		displayName = "Weapon22";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder23
	{
		name = "Shoulder23";
		displayName = "Weapon23";
		ghostIcon = "Shoulder";
		show = "false";
	};
	class Slot_Shoulder24
	{
		name = "Shoulder24";
		displayName = "Weapon24";
		ghostIcon = "Shoulder";
		show = "false";
	};
};
class CfgWeapons
{
	class RifleCore;
	class Rifle_Base: RifleCore
	{
		inventorySlot[] = {"Shoulder1","Shoulder2","Shoulder3","Shoulder4","Shoulder5","Shoulder6","Shoulder7","Shoulder8","Shoulder9","Shoulder10","Shoulder11","Shoulder12","Shoulder13","Shoulder14","Shoulder15","Shoulder16","Shoulder17","Shoulder18","Shoulder19","Shoulder20","Shoulder21","Shoulder22","Shoulder23","Shoulder24","Shoulder","Melee"};
	};
};
class CfgAmmo
{
	class MeleeDamage;
	class BarbedWireDamage: MeleeDamage
	{
		class DamageApplied
		{
			type = "Melee";
			bleedThreshold = 1;
			class Health
			{
				damage = 0;
			};
			class Blood
			{
				damage = 100;
			};
			class Shock
			{
				damage = 0;
			};
		};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionKitLarge;
	class ExpansionBaseBuilding;
	class ExpansionHelipadKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_EXPANSION_HELIPAD";
		descriptionShort = "$STR_EXPANSION_HELIPAD_DESC";
		model = "\DZ\gear\camping\fence_kit.p3d";
		itemSize[] = {8,8};
	};
	class ExpansionHelipadKitPlacing: ExpansionHelipadKit
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\helipad\Helipad_Preview.p3d";
		slopeTolerance = 0.85;
	};
	class ExpansionHelipad: ExpansionBaseBuilding
	{
		scope = 2;
		displayname = "$STR_EXPANSION_HELIPAD";
		descriptionShort = "$STR_EXPANSION_HELIPAD_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\helipad\Helipad.p3d";
		vehicleClass = "Expansion_Construction";
		physLayer = "item_small";
		carveNavmesh = 0;
		placement = "ForceSlopeOnTerrain";
		overrideDrawArea = "2.0";
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
	};
	class ExpansionHescoKit: ExpansionKitLarge
	{
		scope = 2;
		rotationFlags = 4;
		displayName = "$STR_EXPANSION_HESCO";
		descriptionShort = "$STR_EXPANSION_HESCO_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Hesco\HescoKit.p3d";
		itemSize[] = {8,8};
	};
	class ExpansionHescoKitPlacing: ExpansionHescoKit
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Hesco\Hesco_Preview.p3d";
		slopeTolerance = 0.85;
	};
	class ExpansionHesco: ExpansionBaseBuilding
	{
		scope = 2;
		displayname = "$STR_EXPANSION_HESCO";
		descriptionShort = "$STR_EXPANSION_HESCO_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Hesco\Hesco.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Crafted";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 1;
					Blood = 0;
					Shock = 0;
				};
				class FragGrenade
				{
					Health = 10;
					Blood = 0;
					Shock = 0;
				};
			};
		};
	};
	class ExpansionBarbedWireKit: ExpansionKitLarge
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_BarbedWire0";
		descriptionShort = "$STR_CfgVehicles_BarbedWire1";
		model = "\DZ\gear\camping\Barbed_wire.p3d";
		itemSize[] = {8,8};
	};
	class ExpansionBarbedWireKitPlacing: ExpansionBarbedWireKit
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\BarbedWire\Wire_Preview.p3d";
		slopeTolerance = 0.85;
	};
	class ExpansionBarbedWire: ExpansionBaseBuilding
	{
		scope = 2;
		displayName = "$STR_CfgVehicles_BarbedWire0";
		descriptionShort = "$STR_CfgVehicles_BarbedWire1";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\BarbedWire\Wire.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Crafted";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
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
	};
	class ExpansionGunrack: Container_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_GUNRACK";
		descriptionShort = "$STR_EXPANSION_GUNRACK_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\gunrack.p3d";
		hologramMaterial = "sea_chest";
		hologramMaterialPath = "dz\gear\camping\data";
		weight = 10000;
		itemBehaviour = 0;
		itemSize[] = {10,10};
		carveNavmesh = 1;
		rotationFlags = 2;
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\dayzexpansion\objects\basebuilding\misc\gunrack\data\expansion_gunrack_co.paa"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 600;
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
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet = "seachest_movement_SoundSet";
					id = 1;
				};
				class pickUpItem_Light
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "seachest_drop_SoundSet";
					id = 898;
				};
			};
		};
		attachments[] = {"Shoulder1","Shoulder2","Shoulder3","Shoulder4","Shoulder5","Shoulder6","Shoulder7","Shoulder8","Shoulder9","Shoulder10","Shoulder11","Shoulder12","Shoulder13","Shoulder14","Shoulder15","Shoulder16","Shoulder17","Shoulder18","Shoulder19","Shoulder20","Shoulder21","Shoulder22","Shoulder23","Shoulder24"};
		class GUIInventoryAttachmentsProps
		{
			class Attachments
			{
				name = "$STR_EXPANSION_GUNRACK";
				description = "";
				attachmentSlots[] = {"Shoulder1","Shoulder2","Shoulder3","Shoulder4","Shoulder5","Shoulder6","Shoulder7","Shoulder8","Shoulder9","Shoulder10","Shoulder11","Shoulder12","Shoulder13","Shoulder14","Shoulder15","Shoulder16","Shoulder17","Shoulder18","Shoulder19","Shoulder20","Shoulder21","Shoulder22","Shoulder23","Shoulder24"};
				icon = "weaponwrap";
			};
		};
	};
	class ExpansionCone: Container_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_CONE";
		descriptionShort = "$STR_EXPANSION_CONE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Cone.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		itemSize[] = {5,5};
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
	};
	class ExpansionConePlacing: ExpansionCone
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Cone_Preview.p3d";
	};
	class ExpansionSignDanger: Container_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_SIGN_DANGER";
		descriptionShort = "$STR_EXPANSION_SIGN_DANGER_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		itemSize[] = {5,5};
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
	};
	class ExpansionSignDangerPlacing: ExpansionSignDanger
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger_Preview.p3d";
	};
	class ExpansionSignDanger2: ExpansionSignDanger
	{
		scope = 2;
		displayname = "$STR_EXPANSION_SIGN_DANGER";
		descriptionShort = "$STR_EXPANSION_SIGN_DANGER_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger2.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		itemSize[] = {5,5};
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
	};
	class ExpansionSignDanger2Placing: ExpansionSignDanger2
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger2_Preview.p3d";
	};
	class ExpansionSignDanger3: ExpansionSignDanger
	{
		scope = 2;
		displayname = "$STR_EXPANSION_SIGN_DANGER";
		descriptionShort = "$STR_EXPANSION_SIGN_DANGER_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger3.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		itemSize[] = {5,5};
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
	};
	class ExpansionSignDanger3Placing: ExpansionSignDanger3
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Danger3_Preview.p3d";
	};
	class ExpansionSignRoadBarrier: Container_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_SIGN_ROADBARRIER";
		descriptionShort = "$STR_EXPANSION_SIGN_ROADBARRIER_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Roadbarrier.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		itemSize[] = {5,5};
		hiddenSelections[] = {"light"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\Objects\Basebuilding\Misc\Signs\Data\bariera_lampa.rvmat"};
		attachments[] = {"BatteryD"};
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
		class EnergyManager
		{
			hasIcon = 1;
			autoSwitchOffWhenInCargo = 1;
			energyUsagePerSecond = 0.014;
			plugType = 1;
			attachmentAction = 1;
			updateInterval = 30;
		};
	};
	class ExpansionSignRoadBarrierPlacing: ExpansionSignRoadBarrier
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\Signs\Roadbarrier_Preview.p3d";
	};
	class ExpansionStove: Container_Base
	{
		scope = 2;
		displayname = "$STR_EXPANSION_STOVE";
		descriptionShort = "$STR_EXPANSION_STOVE_DESC";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\furniture\Kitchenstove_Elec.p3d";
		vehicleClass = "Expansion_Construction";
		heavyItem = 1;
		lootCategory = "Tools";
		useEntityHierarchy = "true";
		physLayer = "item_large";
		bounding = "BSphere";
		allowOwnedCargoManipulation = 1;
		itemSize[] = {10,15};
		attachments[] = {"GasCanister","CookingEquipment"};
		class AnimationSources
		{
			class Doors1
			{
				source = "user";
				animPeriod = 0.8;
				initPhase = 0;
			};
		};
		class EnergyManager
		{
			switchOnAtSpawn = 0;
			autoSwitchOff = 1;
			autoSwitchOffWhenInCargo = 1;
			energyStorageMax = 0;
			energyUsagePerSecond = 1;
			energyAtSpawn = 0;
			powerSocketsCount = 0;
			plugType = 7;
			attachmentAction = 1;
		};
		class Cargo
		{
			itemsCargoSize[] = {10,10};
			allowOwnedCargoManipulation = 1;
			openable = 0;
		};
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
	};
	class ExpansionStovePlacing: ExpansionStove
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Basebuilding\Misc\furniture\Kitchenstove_Elec_Preview.p3d";
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxygun1: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder1";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun1.p3d";
	};
	class Proxygun2: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder2";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun2.p3d";
	};
	class Proxygun3: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder3";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun3.p3d";
	};
	class Proxygun4: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder4";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun4.p3d";
	};
	class Proxygun5: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder5";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun5.p3d";
	};
	class Proxygun6: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder6";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun6.p3d";
	};
	class Proxygun7: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder7";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun7.p3d";
	};
	class Proxygun8: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder8";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun8.p3d";
	};
	class Proxygun9: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder9";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun9.p3d";
	};
	class Proxygun10: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder10";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun10.p3d";
	};
	class Proxygun11: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder11";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun11.p3d";
	};
	class Proxygun12: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder12";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun12.p3d";
	};
	class Proxygun13: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder13";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun13.p3d";
	};
	class Proxygun14: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder14";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun14.p3d";
	};
	class Proxygun15: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder15";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun15.p3d";
	};
	class Proxygun16: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder16";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun16.p3d";
	};
	class Proxygun17: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder17";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun17.p3d";
	};
	class Proxygun18: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder18";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun18.p3d";
	};
	class Proxygun19: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder19";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun19.p3d";
	};
	class Proxygun20: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder20";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun20.p3d";
	};
	class Proxygun21: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder21";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun21.p3d";
	};
	class Proxygun22: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder22";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun22.p3d";
	};
	class Proxygun23: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder23";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun23.p3d";
	};
	class Proxygun24: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Shoulder24";
		model = "\DayZExpansion\Objects\Basebuilding\Misc\gunrack\proxy\gun24.p3d";
	};
};
