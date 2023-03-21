#define _ARMA_

class CfgPatches
{
	class DayZExpansion_PersonalStorage_Objects
	{
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DayZExpansion_Dta_Core"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionStaticObjectBase;
	class ExpansionPersonalStorageBase: ExpansionStaticObjectBase
	{
		scope = 0;
		forceFarBubble = "true";
	};
	class ExpansionPersonalStorageChest: ExpansionPersonalStorageBase
	{
		scope = 1;
		model = "\DZ\gear\camping\sea_chest.p3d";
	};
	class ExpansionPersonalProtectiveCaseBase: Container_Base
	{
		scope = 0;
		displayName = "Survivor's Protection Case";
		descriptionShort = "This high-tech case was developed by a team of scientists to withstand even the harshest conditions on the island. Its advanced technology allows it to protect the contents from the elements and keep them safe from any harm. It's the perfect companion for any survivor, providing a reliable and secure storage option for all their essential items.";
		model = "\dz\gear\containers\Protector_Case.p3d";
		hiddenSelectionsTextures[] = {"dz\gear\containers\data\loot_protectorcase_co.paa"};
		inventorySlot[] = {"ExpansionPersonalContainer","ExpansionPersonalContainer_Swap"};
		rotationFlags = 17;
		weight = 980;
		itemSize[] = {3,4};
		isMeleeWeapon = 1;
		allowOwnedCargoManipulation = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 750;
					healthLevels[] = {{1,{"DZ\gear\containers\data\Protector_Case.rvmat"}},{0.7,{"DZ\gear\containers\data\Protector_Case.rvmat"}},{0.5,{"DZ\gear\containers\data\Protector_Case_damage.rvmat"}},{0.3,{"DZ\gear\containers\data\Protector_Case_damage.rvmat"}},{0,{"DZ\gear\containers\data\Protector_Case_destruct.rvmat"}}};
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
		soundImpactType = "plastic";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "smallprotectorcase_pickup_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "smallprotectorcase_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class ExpansionPersonalProtectiveCaseAlpha: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Alpha Protector Case";
		itemsCargoSize[] = {2,2};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_orange_co.paa"};
	};
	class ExpansionPersonalProtectiveCaseBeta: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Beta Protector Case";
		itemsCargoSize[] = {3,2};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_red_co.paa"};
	};
	class ExpansionPersonalProtectiveCaseGamma: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Gamma Protector Case";
		itemsCargoSize[] = {4,2};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_green_co.paa"};
	};
	class ExpansionPersonalProtectiveCaseDelta: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Delta Protector Case";
		itemsCargoSize[] = {3,3};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_purple_co.paa"};
	};
	class ExpansionPersonalProtectiveCaseEpsilon: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Epsilon Protector Case";
		itemsCargoSize[] = {3,4};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_blue_co.paa"};
	};
	class ExpansionPersonalProtectiveCaseOmega: ExpansionPersonalProtectiveCaseBase
	{
		scope = 2;
		displayName = "Omega Protector Case";
		itemsCargoSize[] = {4,4};
		hiddenSelectionsTextures[] = {"DayZExpansion\PersonalStorage\Objects\ProtectorCase\data\expansion_protectorcase_black_co.paa"};
	};
};
