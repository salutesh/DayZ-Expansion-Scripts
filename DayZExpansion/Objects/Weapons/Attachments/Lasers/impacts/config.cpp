#define _ARMA_

class CfgPatches
{
	class Expansion_Lasers_Impacts
	{
		units[] = {"Expansion_Laser_Beam","Expansion_Laser_Impact1","Expansion_Laser_Impact2","Expansion_Laser_Impact3","Expansion_Laser_Impact4","Expansion_Laser_Impact5","Expansion_Laser_Impact6"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Weapons_Melee"};
	};
};
class CfgSlots
{
	class Slot_expansionLaserBeam
	{
		name = "expansionLaserBeam";
		displayName = "$STR_EXPANSION_ANPEQ15 Laser";
		ghostIcon = "suppressor";
	};
};
class cfgVehicles
{
	class HouseNoDestruct;
	class Inventory_Base;
	class Expansion_Laser_Beam: Inventory_Base
	{
		scope = 2;
		displayName = "You should not have this.";
		descriptionShort = "You should not have this.";
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_beam.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
		itemSize[] = {1,3};
	};
	class Expansion_Laser_Impact1: HouseNoDestruct
	{
		scope = 2;
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_1.p3d";
	};
	class Expansion_Laser_Impact2: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_2.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
	};
	class Expansion_Laser_Impact3: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_3.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
	};
	class Expansion_Laser_Impact4: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_4.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
	};
	class Expansion_Laser_Impact5: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_5.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
	};
	class Expansion_Laser_Impact6: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_red_6.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1.0,co)"};
		hiddenSelectionsMaterials[] = {"dz\weapons\projectiles\data\tracer_red.rvmat"};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class Proxylaser_beam: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "expansionLaserBeam";
		model = "DayZExpansion\Objects\Weapons\Attachments\Lasers\impacts\laser_beam.p3d";
	};
};
