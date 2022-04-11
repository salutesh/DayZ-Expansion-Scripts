#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Explosives
	{
		units[] = {"ExpansionSatchel","Expansion_C4_Explosion"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		ammo[] = {"ExpansionDummy_Explosive"};
	};
};
class CfgAmmo
{
	class Dummy_Heavy;
	class ExpansionDummy_Explosive: Dummy_Heavy
	{
		class DamageApplied
		{
			type = "Melee";
			class Health
			{
				damage = 100;
			};
			class Blood
			{
				damage = 10;
			};
			class Shock
			{
				damage = 15;
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HouseNoDestruct;
	class ExpansionExplosive: Inventory_Base
	{
		scope = 1;
		displayName = "$STR_EXPANSION_HANDMADE_EXPLOSIVE";
		descriptionShort = "$STR_EXPANSION_HANDMADE_EXPLOSIVE_DESC";
		vehicleClass = "Expansion_Explosives";
		model = "\DayZExpansion\Objects\Weapons\Explosives\explosive.p3d";
		weight = 4;
		itemSize[] = {2,3};
		rotationFlags = 17;
		canBeSplit = 0;
		stackedUnit = "l";
	};
	class ExpansionExplosivePlacing: ExpansionExplosive
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel_preview.p3d";
		slopeTolerance = 0.95;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionSatchel: ExpansionExplosive
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SATCHEL_EXPLOSIVE";
		descriptionShort = "$STR_EXPANSION_SATCHEL_EXPLOSIVE_DESC";
		vehicleClass = "Expansion_Explosives";
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel.p3d";
		weight = 4;
		itemSize[] = {3,4};
		rotationFlags = 17;
		canBeSplit = 0;
		stackedUnit = "l";
	};
	class ExpansionSatchelPlacing: ExpansionSatchel
	{
		scope = 1;
		model = "\DayZExpansion\Objects\Weapons\Explosives\satchel_preview.p3d";
		slopeTolerance = 0.95;
		hiddenSelections[] = {"placing"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,1,1,1.0,co)"};
	};
	class ExpansionExplosiveBase: ExpansionExplosive
	{
		scope = 1;
	};
	class Expansion_C4_Explosion: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\data\lightpoint.p3d";
		ammoType = "ExpansionC4_Ammo";
	};
};
