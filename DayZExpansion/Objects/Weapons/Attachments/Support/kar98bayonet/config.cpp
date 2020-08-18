#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Kar98Bayonet
	{
		units[] = {"Expansion_Kar98_Bayonet"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Muzzles"};
	};
};
class cfgVehicles
{
	class AK_Bayonet;
	class Expansion_Kar98_Bayonet: AK_Bayonet
	{
		scope = 2;
		displayName = "$STR_EXPANSION_KAR98_BAYONET";
		descriptionShort = "$STR_EXPANSION_KAR98_BAYONET_DESC";
		model = "DayZExpansion\Objects\Weapons\Attachments\Support\kar98bayonet\k98bayonet.p3d";
		repairableWithKits[] = {5,4};
		repairCosts[] = {30,25};
		animClass = "Knife";
		rotationFlags = 17;
		reversed = 0;
		weight = 426;
		itemSize[] = {1,3};
		itemModelLength = 0.388;
		openItemSpillRange[] = {0,20};
		inventorySlot = "expansionWeaponBayonetKar";
		canSkinBodies = 1;
		recoilModifier[] = {1,1,1};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\attachments\data\Russian_AKM_Bayonet.rvmat"}},{0.7,{"DZ\weapons\attachments\data\Russian_AKM_Bayonet.rvmat"}},{0.5,{"DZ\weapons\attachments\data\Russian_AKM_Bayonet_damage.rvmat"}},{0.3,{"DZ\weapons\attachments\data\Russian_AKM_Bayonet_damage.rvmat"}},{0.0,{"DZ\weapons\attachments\data\Russian_AKM_Bayonet_destruct.rvmat"}}};
				};
			};
		};
		class MeleeModes
		{
			class Default
			{
				ammo = "MeleeKnife";
				range = 1.1;
			};
			class Heavy
			{
				ammo = "MeleeKnife_Heavy";
				range = 1.1;
			};
			class Sprint
			{
				ammo = "MeleeKnife_Heavy";
				range = 3.3;
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class drop
				{
					soundset = "CombatKnife_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxyk98bayonet: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "expansionWeaponBayonetKar";
		model = "DayZExpansion\Objects\Weapons\Attachments\Support\kar98bayonet\k98bayonet.p3d";
	};
};
class CfgSlots
{
	class Slot_expansionWeaponBayonetKar
	{
		name = "expansionWeaponBayonetKar";
		displayName = "expansionWeaponBayonetKar";
		ghostIcon = "bayonetak";
	};
};
