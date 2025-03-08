class CfgPatches
{
	class DayZExpansion_Objects_Missions
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Container_Base;
	class HouseNoDestruct;
	class ExpansionAirdropPlaneBase: HouseNoDestruct
	{
		scope=0;
	};
	class ExpansionAirdropPlaneBasic: ExpansionAirdropPlaneBase
	{
		vehicleClass="Expansion_Airdrop";
		scope=2;
		model="\DZ\structures\Wrecks\Aircraft\Wreck_C130J_2.p3d";
		forceFarBubble="true";
	};
	class ExpansionAirdropStarDestroyer: ExpansionAirdropPlaneBase
	{
		vehicleClass="Expansion_Airdrop";
		scope=2;
		model="\DayZExpansion\Missions\Objects\stardestroyer_edit2.p3d";
		forceFarBubble="true";
	};
	class ExpansionAirdropContainerBase: Container_Base
	{
		scope=0;
		vehicleClass="Expansion_Airdrop";
		displayName="$STR_EXPANSION_AIRDROP";
		model="\DayZExpansion\Missions\Objects\container.p3d";
		forceFarBubble="true";
		carveNavmesh=1;
		itemsCargoSize[]={10,100};
		rotationFlags=2;
		itemSize[]={20,20};
		weight=300000;
		physLayer="item_large";
		cargoClass="";
		inventoryCondition="true";
		storageCategory=1;
		openable=0;
		lootCategory="Containers";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=100000;
					healthLevels[]={};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage=0;
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
						damage=0;
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
		class Cargo
		{
			itemsCargoSize[]={10,100};
		};
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_co.paa"
		};
	};
	class ExpansionAirdropContainer: ExpansionAirdropContainerBase
	{
		scope=2;
		displayName="$STR_EXPANSION_AIRDROP";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_co.paa"
		};
	};
	class ExpansionAirdropContainer_Grey: ExpansionAirdropContainer
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_grey_co.paa"
		};
	};
	class ExpansionAirdropContainer_Blue: ExpansionAirdropContainer
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_blue_co.paa"
		};
	};
	class ExpansionAirdropContainer_Olive: ExpansionAirdropContainer
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_olive_co.paa"
		};
	};
	class ExpansionAirdropContainer_Medical: ExpansionAirdropContainerBase
	{
		scope=2;
		displayName="$STR_EXPANSION_MEDICAL_AIRDROP";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_med_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military: ExpansionAirdropContainerBase
	{
		scope=2;
		displayName="$STR_EXPANSION_MILITARY_AIRDROP";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military_GreenCamo: ExpansionAirdropContainer_Military
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_green_camo_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military_MarineCamo: ExpansionAirdropContainer_Military
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_marine_camo_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military_OliveCamo: ExpansionAirdropContainer_Military
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_olive_camo_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military_OliveCamo2: ExpansionAirdropContainer_Military
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_olive_camo2_co.paa"
		};
	};
	class ExpansionAirdropContainer_Military_WinterCamo: ExpansionAirdropContainer_Military
	{
		scope=2;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_mil_winter_camo_co.paa"
		};
	};
	class ExpansionAirdropContainer_Basebuilding: ExpansionAirdropContainerBase
	{
		scope=2;
		displayName="$STR_EXPANSION_BASEBUILDING_AIRDROP";
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Missions\Objects\data\Airdrop_basebuilding_co.paa"
		};
	};
	class ExpansionAirdropParticle: HouseNoDestruct
	{
		vehicleClass="Expansion_Airdrop";
		scope=1;
	};
};
