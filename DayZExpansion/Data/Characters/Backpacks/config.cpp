#define _ARMA_

class CfgPatches
{
	class Expansion_Backpacks
	{
		units[] = {"ExpansionCoyoteBlack","ExpansionDesertTortilla"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters_Backpacks"};
	};
};
class CfgSlots
{
	class Slot_ExpansionParachute
	{
		name = "expansionparachute";
		displayName = "Parachute";
		descriptionShort = "";
		selection = "expansionparachute";
		ghostIcon = "parachute";
	};
	class Slot_ExpansionParachutePacked
	{
		name = "expansionparachutepacked";
		displayName = "Parachute";
		descriptionShort = "";
		selection = "";
		ghostIcon = "parachute";
	};
};
class CfgVehicles
{
	class Clothing_Base;
	class Clothing;
	class AliceBag_ColorBase;
	class ChildBag_ColorBase;
	class HuntingBag;
	class TaloonBag_ColorBase;
	class TortillaBag;
	class GhillieSuit_ColorBase;
	class ImprovisedBag;
	class AssaultBag_ColorBase;
	class CoyoteBag_ColorBase;
	class ExpansionCoyoteBlack: CoyoteBag_ColorBase
	{
		scope = 2;
		visibilityModifier = 0.95;
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa","DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa","DayZExpansion\Data\characters\backpacks\data\CoyoteBP_black_co.paa"};
	};
	class ExpansionDesertTortilla: TortillaBag
	{
		scope = 2;
		visibilityModifier = 0.95;
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"DayZExpansion\Data\characters\backpacks\data\tortila_desertmarpat_co.paa"};
	};
	class ExpansionParachuteBag: Clothing
	{
		scope = 0;
		displayName = "DOESN'T WORK - DO NOT USE";
		descriptionShort = "DOESN'T WORK - DO NOT USE";
		visibilityModifier = 0.95;
		hiddenSelectionsTextures[] = {"DZ\characters\backpacks\data\Taloon_Blue_co.paa","DZ\characters\backpacks\data\Taloon_Blue_co.paa","DZ\characters\backpacks\data\Taloon_Blue_co.paa"};
		model = "\dz\characters\backpacks\taloon_g.p3d";
		inventorySlot[] = {"Back"};
		attachments[] = {"expansionparachute","expansionparachutepacked"};
		itemInfo[] = {"Clothing","Back"};
		rotationFlags = 16;
		weight = 1300;
		itemSize[] = {4,5};
		itemsCargoSize[] = {7,6};
		absorbency = 0.3;
		heatIsolation = 0.1;
		repairableWithKits[] = {5,3};
		repairCosts[] = {30.0,25.0};
		soundAttType = "Outdoor";
		randomQuantity = 3;
		hiddenSelections[] = {"camoGround","camoMale","camoFemale"};
		class ClothingTypes
		{
			male = "\DZ\characters\backpacks\taloon_m.p3d";
			female = "\DZ\characters\backpacks\taloon_f.p3d";
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\Characters\backpacks\data\taloon.rvmat"}},{0.7,{"DZ\Characters\backpacks\data\taloon.rvmat"}},{0.5,{"DZ\Characters\backpacks\data\taloon_damage.rvmat"}},{0.3,{"DZ\Characters\backpacks\data\taloon_damage.rvmat"}},{0.0,{"DZ\Characters\backpacks\data\taloon_destruct.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem_Light
				{
					soundSet = "pickUpBackPack_Plastic_Light_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpBackPack_Plastic_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "taloonbag_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
};
